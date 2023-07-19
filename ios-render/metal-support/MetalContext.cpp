//
//  MetalContext.cpp
//  metal-support
//
//  Created by chenqiudu on 2023/7/18.
//

#include "MetalContext.hpp"
#include <string>

using namespace MTL;
using namespace NS;

xxd::MetalContext::MetalContext()
{
    mtDevice = CreateSystemDefaultDevice();
    mtCmdQueue = mtDevice->newCommandQueue();
    
    mtRenderFormat = PixelFormatBGRA8Unorm_sRGB;
    mtDepthFormat = PixelFormatDepth32Float;
    
    float s = 1.f;
    vertex vertices[] = {
        (vertex){(simd_float3){ -s, -s, +s }, (simd_float3){ 1, 0, 0 }},
        (vertex){(simd_float3){ +s, -s, +s }, (simd_float3){ 0, 1, 0 }},
        (vertex){(simd_float3){ +s, +s, +s }, (simd_float3){ 0, 0, 1 }},
        (vertex){(simd_float3){ -s, +s, +s }, (simd_float3){ 0, 0, 0 }},
        
        (vertex){(simd_float3){ -s, -s, -s }, (simd_float3){ 1, 1, 0 }},
        (vertex){(simd_float3){ -s, +s, -s }, (simd_float3){ 0, 1, 1 }},
        (vertex){(simd_float3){ +s, +s, -s }, (simd_float3){ 1, 0, 1 }},
        (vertex){(simd_float3){ +s, -s, -s }, (simd_float3){ 1, 1, 1 }}
    };

    uint16_t indices[] = {
        0, 1, 2, /* front */
        2, 3, 0,

        1, 7, 6, /* right */
        6, 2, 1,

        7, 4, 5, /* back */
        5, 6, 7,

        4, 0, 3, /* left */
        3, 5, 4,

        3, 2, 6, /* top */
        6, 5, 3,

        4, 7, 1, /* bottom */
        1, 0, 4
    };
    
    CreateGeometry(8, vertices, 36,  indices);
    mtCameraBuffer = mtDevice->newBuffer(UInteger(sizeof(simd_float4x4)), ResourceStorageModeShared);
    
    using NS::StringEncoding::UTF8StringEncoding;
    
    Error* e = nullptr;
    const char* cPath = Bundle::mainBundle()->resourcePath()->utf8String();
    std::string pathStr(cPath);
    pathStr += "/metal-shader.metallib";
    auto mtLibURL = TransferPtr(URL::alloc()->initFileURLWithPath(String::string(pathStr.c_str(), UTF8StringEncoding)));
    
    auto mtLibrary = TransferPtr(mtDevice->newLibrary(mtLibURL.get(), &e));
    auto vertexShader = TransferPtr(mtLibrary->newFunction(String::string("vertexMain", UTF8StringEncoding)));
    auto fragmentShader = TransferPtr(mtLibrary->newFunction(String::string("fragmentMain", UTF8StringEncoding)));
    auto mtPSODesc = TransferPtr(RenderPipelineDescriptor::alloc()->init());
    mtPSODesc->setVertexFunction(vertexShader.get());
    mtPSODesc->setFragmentFunction(fragmentShader.get());
    mtPSODesc->colorAttachments()->object(0)->setPixelFormat(mtRenderFormat);
    //mtPSODesc->setDepthAttachmentPixelFormat(mtDepthFormat);

    mtPSO = mtDevice->newRenderPipelineState(mtPSODesc.get(), &e);
}

xxd::MetalContext::~MetalContext()
{
    mtCmdQueue->release();
    mtDevice->release();
    mtVertexBuffer->release();
    mtIndexBuffer->release();
}

bool xxd::MetalContext::CreateGeometry(uint16_t vertexNum, const xxd::vertex* vertexData, uint16_t indexNum, const uint16_t* indexData)
{
    auto vertexBufferSize = vertexNum * sizeof(vertex);
    auto indexBufferSize = indexNum * sizeof(uint16_t);
    mtVertexBuffer = mtDevice->newBuffer(vertexData, UInteger(vertexBufferSize), ResourceStorageModeShared);
    mtIndexBuffer = mtDevice->newBuffer(indexData, UInteger(indexBufferSize), ResourceStorageModeShared);
    
    return mtVertexBuffer && mtIndexBuffer;
}




