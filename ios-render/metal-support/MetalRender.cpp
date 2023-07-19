//
//  MetalRender.cpp
//  metal-support
//
//  Created by chenqiudu on 2023/7/18.
//

#include "MetalRender.h"
#include "MetalContext.h"
#include "MetalCamera.h"

xxd::Renderer::Renderer(xxd::Camera* cam, CA::MetalLayer* layer) : renderCam(cam), surface(layer)
{
    //surface = CA::MetalLayer::layer();
    surface->setDevice(MetalContext::Get().GetDevice());
    surface->setPixelFormat(MetalContext::Get().GetRenderFormat());
    //surface->setDepthFormat(MetalContext::Get().GetDepthFormat());
}

void xxd::Renderer::Draw()
{
    renderCam->theta += 0.01;
    
    NS::AutoreleasePool* pPool = NS::AutoreleasePool::alloc()->init();
    MTL::ClearColor clearColor = {0.6, 0.9, 0.4, 1};
    MTL::CommandBuffer* pCmd = MetalContext::Get().GetCmdQueue()->commandBuffer();
    
    CA::MetalDrawable* next = surface->nextDrawable();
    
    MTL::RenderPassDescriptor* pRpd = MTL::RenderPassDescriptor::alloc()->init();
    auto attachment = pRpd->colorAttachments()->object(0);
    attachment->setClearColor(clearColor);
    attachment->setLoadAction(MTL::LoadActionClear);
    attachment->setTexture(next->texture());
    pRpd->depthAttachment()->setClearDepth(1);
    
    
    MTL::RenderCommandEncoder* pEnc = pCmd->renderCommandEncoder(pRpd);

    *((simd_float4x4*)(MetalContext::Get().GetCameraBuffer()->contents())) = renderCam->GetViewPerspecMatrix();
    
    pEnc->setCullMode(MTL::CullModeBack);
    pEnc->setFrontFacingWinding(MTL::WindingClockwise);
    pEnc->setRenderPipelineState(MetalContext::Get().GetPSO());
    pEnc->setVertexBuffer(MetalContext::Get().GetVertexBuffer(), 0, 0);
    pEnc->setVertexBuffer(MetalContext::Get().GetCameraBuffer(), 0, 1);
    pEnc->drawIndexedPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle,
                                36, MTL::IndexType::IndexTypeUInt16,
                                MetalContext::Get().GetIndexBuffer(),
                                0,
                                1);
    pEnc->endEncoding();
    pCmd->presentDrawable(next);
    pCmd->commit();

    pPool->release();
}


