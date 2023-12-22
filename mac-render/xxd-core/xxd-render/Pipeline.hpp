#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include "RenderUtil.hpp"

namespace xxd
{
class Pipeline
{
public:
	Pipeline(MTL::PixelFormat render, MTL::PixelFormat depSt);	
	~Pipeline();

	inline MTL::RenderPipelineState* GetPSO(const std::string& key) { return pipelineMap.count(key) ? pipelineMap[key] : nullptr; };

	MTL::PixelFormat renderFmt;
	MTL::PixelFormat depStFmt;
private: 
	void CreatePSO();	
	std::unordered_map<std::string, MTL::RenderPipelineState*> pipelineMap;
};
}
#endif
