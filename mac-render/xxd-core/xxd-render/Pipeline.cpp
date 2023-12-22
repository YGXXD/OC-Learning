#include "Pipeline.hpp"
#include "RenderContext.hpp"
#include "Shader.hpp"

xxd::Pipeline::Pipeline(MTL::PixelFormat render, MTL::PixelFormat depSt) : renderFmt(render), depStFmt(depSt)
{
	CreatePSO();
}

xxd::Pipeline::~Pipeline()
{
    for(const auto& it : pipelineMap)
    {
        it.second->release();
    }
}

void xxd::Pipeline::CreatePSO()
{
	NS::SharedPtr<MTL::RenderPipelineDescriptor> mtPSODesc = NS::TransferPtr(MTL::RenderPipelineDescriptor::alloc()->init());

	// vertex(position color layout)
	NS::SharedPtr<MTL::VertexDescriptor> mtVertexDesc = NS::TransferPtr(MTL::VertexDescriptor::alloc()->init());
	NS::SharedPtr<MTL::VertexAttributeDescriptor> mtVertexPosition = NS::TransferPtr(MTL::VertexAttributeDescriptor::alloc()->init());
	mtVertexPosition->setFormat(MTL::VertexFormatFloat3);
	mtVertexPosition->setOffset(offsetof(xxd::vertex, position));
	mtVertexPosition->setBufferIndex(0);
	NS::SharedPtr<MTL::VertexAttributeDescriptor> mtVertexColor = NS::TransferPtr(MTL::VertexAttributeDescriptor::alloc()->init());
	mtVertexColor->setFormat(MTL::VertexFormatFloat3);
	mtVertexColor->setOffset(offsetof(xxd::vertex, color));
	mtVertexColor->setBufferIndex(0);
	NS::SharedPtr<MTL::VertexBufferLayoutDescriptor> mtVertexLayout = NS::TransferPtr(MTL::VertexBufferLayoutDescriptor::alloc()->init());
	mtVertexLayout->setStride(sizeof(xxd::vertex));
	mtVertexLayout->setStepRate(1);
	mtVertexLayout->setStepFunction(MTL::VertexStepFunctionPerVertex);

	mtVertexDesc->attributes()->setObject(mtVertexPosition.get(), 0);
	mtVertexDesc->attributes()->setObject(mtVertexColor.get(), 1);
	mtVertexDesc->layouts()->setObject(mtVertexLayout.get(), 0);

	// uniform
	NS::SharedPtr<MTL::PipelineBufferDescriptor> mtUniformDesc = NS::TransferPtr(MTL::PipelineBufferDescriptor::alloc()->init());
	mtUniformDesc->setMutability(MTL::MutabilityDefault);

	// color attachment
	NS::SharedPtr<MTL::RenderPipelineColorAttachmentDescriptor> colorAttachDesc = NS::TransferPtr(MTL::RenderPipelineColorAttachmentDescriptor::alloc()->init());
	colorAttachDesc->setPixelFormat(renderFmt);
	colorAttachDesc->setWriteMask(MTL::ColorWriteMaskAll);
	colorAttachDesc->setBlendingEnabled(false);

	mtPSODesc->setVertexDescriptor(mtVertexDesc.get());
	mtPSODesc->setVertexFunction(RenderContext::Get().GetShader()->GetShaderFunction("Render3DVertex"));
	mtPSODesc->setFragmentFunction(RenderContext::Get().GetShader()->GetShaderFunction("Render3DFragment"));
	mtPSODesc->setSampleCount(1);
	mtPSODesc->setRasterSampleCount(1);
	mtPSODesc->setRasterizationEnabled(true);
	mtPSODesc->vertexBuffers()->setObject(mtUniformDesc.get(), 1);
	mtPSODesc->colorAttachments()->setObject(colorAttachDesc.get(), 0);
	mtPSODesc->setDepthAttachmentPixelFormat(depStFmt);
	mtPSODesc->setStencilAttachmentPixelFormat(depStFmt);

	NS::Error* e;
	pipelineMap["Render3D"] = RenderContext::Get().GetDevice()->newRenderPipelineState(mtPSODesc.get(), &e);

}
