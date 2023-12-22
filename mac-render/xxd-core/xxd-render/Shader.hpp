#ifndef SHADER_HPP
#define SHADER_HPP

#include "RenderUtil.hpp"

namespace xxd
{

typedef struct
{
	std::string key;
	std::string func;
}shaderName;

class Shader
{
public:
	Shader(MTL::Device* device);
	~Shader();

	void CreateShaderFunction(const std::vector<shaderName>& names, const std::string& shaderSrc);	
	inline MTL::Function* GetShaderFunction(const std::string& key) { return shaderMap.count(key) ? shaderMap[key] : nullptr; }
	inline MTL::Buffer* GetUniformBuffer() const { return unifomBuffer.get(); }
	inline void UploadUnform(const uniform& data) { memcpy(unifomBuffer->contents(), &data, sizeof(uniform)); };
private:
	void CreateUniformBuffer();
	void CreateShaderFunction();

	MTL::Device* mtDevice;
	NS::SharedPtr<MTL::Buffer> unifomBuffer;
	std::unordered_map<std::string, MTL::Function*> shaderMap; 
};
}
#endif
