#ifndef RENDER_CONTEXT_HPP
#define RENDER_CONTEXT_HPP

#include "Singleton.hpp"
#include "RenderUtil.hpp"

namespace xxd
{
class Shader;
class RenderContext : public Singleton<RenderContext>
{
public:
	friend class Singleton<RenderContext>;

	inline MTL::Device* GetDevice() const { return mtDevice.get(); }
	inline MTL::CommandQueue* GetCommandQueue() const { return mtQueue.get(); }
	inline Shader* GetShader() const { return shader.get(); }
private:
	RenderContext();
	~RenderContext();

	NS::SharedPtr<MTL::Device> mtDevice;
	NS::SharedPtr<MTL::CommandQueue> mtQueue;

	std::unique_ptr<Shader> shader;
};
} 

#endif
