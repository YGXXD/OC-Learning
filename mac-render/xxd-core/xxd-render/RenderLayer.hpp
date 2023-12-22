#ifndef RENDER_LAYER_HPP
#define RENDER_LAYER_HPP

#include "RenderUtil.hpp"

namespace xxd
{
class Pipeline;
class RenderLayer 
{
public:
	RenderLayer(CA::MetalLayer* layer);
	~RenderLayer();

	void Draw();

private:
	void CreateBoxBuffer();
	MTL::PixelFormat renderFmt;
	MTL::PixelFormat depStFmt;

	NS::SharedPtr<MTL::Texture> depthImage;
	NS::SharedPtr<MTL::DepthStencilState> depthState;

	NS::SharedPtr<CA::MetalLayer> mtLayer;	
	std::unique_ptr<Pipeline> pipeline;	
	
	NS::SharedPtr<MTL::Buffer> boxVertexBuffer;
	NS::SharedPtr<MTL::Buffer> boxIndexBuffer;

	// Camera Data
	float aspect;
    float raduis;
    float theta;
    float phi;
};
}
#endif
