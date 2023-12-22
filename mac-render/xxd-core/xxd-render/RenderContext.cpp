#include "RenderContext.hpp"
#include "Shader.hpp"

xxd::RenderContext::RenderContext()
{
	mtDevice = NS::TransferPtr(MTL::CreateSystemDefaultDevice());
	mtQueue = NS::TransferPtr(mtDevice->newCommandQueue());

	shader = std::make_unique<Shader>(mtDevice.get());
}

xxd::RenderContext::~RenderContext()
{

}
