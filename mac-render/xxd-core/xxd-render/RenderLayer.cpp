#include "RenderLayer.hpp"
#include "Pipeline.hpp"
#include "RenderContext.hpp"
#include "Shader.hpp"

xxd::RenderLayer::RenderLayer(CA::MetalLayer* layer) : 
	renderFmt(MTL::PixelFormatBGRA8Unorm_sRGB),
   	depStFmt(MTL::PixelFormatDepth32Float_Stencil8)
{
	mtLayer = NS::RetainPtr(layer);
	
	// create depth stencil buffer
	auto depthDesc = NS::RetainPtr(MTL::TextureDescriptor::texture2DDescriptor(depStFmt, layer->drawableSize().width, layer->drawableSize().height, false));
//	depthDesc->setWidth(layer->drawableSize().width);
//	depthDesc->setHeight(layer->drawableSize().height);
//	depthDesc->setPixelFormat(depStFmt);
//	depthDesc->setMipmapLevelCount(1);
	depthDesc->setSampleCount(1);
	depthDesc->setStorageMode(MTL::StorageModePrivate);
	depthDesc->setResourceOptions(MTL::ResourceOptionCPUCacheModeDefault);
	depthDesc->setUsage(MTL::TextureUsageRenderTarget);
	depthImage = NS::TransferPtr(RenderContext::Get().GetDevice()->newTexture(depthDesc.get()));

	auto depthStateDesc = NS::TransferPtr(MTL::DepthStencilDescriptor::alloc()->init());
	depthStateDesc->setDepthCompareFunction(MTL::CompareFunctionLess);
	depthStateDesc->setDepthWriteEnabled(true);
	depthState = NS::TransferPtr(RenderContext::Get().GetDevice()->newDepthStencilState(depthStateDesc.get()));

	// set layer attribute to create render targer buffer
	mtLayer->setDevice(RenderContext::Get().GetDevice());
	mtLayer->setPixelFormat(renderFmt);

   	pipeline = std::make_unique<xxd::Pipeline>(renderFmt, depStFmt);
	
	CreateBoxBuffer();
	aspect = layer->drawableSize().width / layer->drawableSize().height;
	raduis = 20;
    theta = 0;
    phi = M_PI / 4.f;
}

xxd::RenderLayer::~RenderLayer()
{

}

void xxd::RenderLayer::Draw()
{
	theta += 0.01;
    simd_float3 eyePos = {raduis * sinf(phi) * cosf(theta), raduis * sinf(phi) * sinf(theta), raduis * cosf(phi)};
    simd_float4x4 perspMat = xxd::MakePerspective(M_PI * 0.25, aspect, 3, 10000);
    simd_float4x4 viewMat = xxd::MakeViewLookAt(eyePos, (simd_float3){0, 0, 0}, (simd_float3){0, 0, 1});
    simd_float4x4 cameraMat = simd_mul(viewMat, perspMat); 

	RenderContext::Get().GetShader()->UploadUnform({cameraMat});
	
    MTL::ClearColor clearColor = {0.9, 0.6, 0.7, 1};
    MTL::CommandBuffer* pCmd = RenderContext::Get().GetCommandQueue()->commandBuffer();

    CA::MetalDrawable* next = mtLayer->nextDrawable();

    MTL::RenderPassDescriptor* pRpd = MTL::RenderPassDescriptor::alloc()->init();
    auto attachment = pRpd->colorAttachments()->object(0);
    attachment->setClearColor(clearColor);
    attachment->setLoadAction(MTL::LoadActionClear);
    attachment->setTexture(next->texture());
    pRpd->depthAttachment()->setClearDepth(1);
	pRpd->depthAttachment()->setLoadAction(MTL::LoadActionClear);
	pRpd->depthAttachment()->setTexture(depthImage.get());
    pRpd->stencilAttachment()->setClearStencil(1);
    pRpd->stencilAttachment()->setLoadAction(MTL::LoadActionClear);
    pRpd->stencilAttachment()->setTexture(depthImage.get());

    MTL::RenderCommandEncoder* pEnc = pCmd->renderCommandEncoder(pRpd);
    pRpd->release();
    
    pEnc->setCullMode(MTL::CullModeBack);
    pEnc->setFrontFacingWinding(MTL::WindingClockwise);
    pEnc->setRenderPipelineState(pipeline->GetPSO("Render3D"));
	pEnc->setDepthStencilState(depthState.get());
    pEnc->setVertexBuffer(boxVertexBuffer.get(), 0, 0);
    pEnc->setVertexBuffer(RenderContext::Get().GetShader()->GetUniformBuffer(), 0, 1);
    pEnc->drawIndexedPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle, 36, MTL::IndexType::IndexTypeUInt32,
                                boxIndexBuffer.get(), 0, 1);
    pEnc->endEncoding();
    pCmd->presentDrawable(next);
    pCmd->commit();
}

void xxd::RenderLayer::CreateBoxBuffer()
{
	MeshData boxMesh = xxd::MeshData::GenerateBoxMesh(1, 2, 1);	
	boxVertexBuffer = NS::TransferPtr(RenderContext::Get().GetDevice()->newBuffer(boxMesh.vertices.data(), boxMesh.vertices.size() * sizeof(vertex), MTL::ResourceStorageModeShared));	
	boxIndexBuffer = NS::TransferPtr(RenderContext::Get().GetDevice()->newBuffer(boxMesh.indices.data(), boxMesh.indices.size() * sizeof(uint32_t), MTL::ResourceStorageModeShared));
}
