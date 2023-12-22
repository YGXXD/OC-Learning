#include "Shader.hpp"

xxd::Shader::Shader(MTL::Device* device) : mtDevice(device)
{
	CreateUniformBuffer();
	CreateShaderFunction();
}

xxd::Shader::~Shader()
{
	for(const auto& it : shaderMap)
	{
		it.second->release();
	}
}

void xxd::Shader::CreateShaderFunction(const std::vector<shaderName>& names, const std::string& shaderSrc)
{
	if(names.size())
	{
		NS::Error* e = nullptr;
		auto library = NS::TransferPtr(mtDevice->newLibrary(NS::String::string(shaderSrc.c_str(), NS::UTF8StringEncoding), nullptr, &e));
		for(const auto& it : names)
		{
			if(!shaderMap.count(it.key))
			{
				shaderMap[it.key] = library->newFunction(NS::String::string(it.func.c_str(), NS::UTF8StringEncoding));
			}	
		}
	}
}

void xxd::Shader::CreateUniformBuffer()
{
    unifomBuffer = NS::TransferPtr(mtDevice->newBuffer(sizeof(uniform), MTL::ResourceStorageModeShared));
}

void xxd::Shader::CreateShaderFunction()
{
	NS::Error* e = nullptr;
	const char* cPath = NS::Bundle::mainBundle()->resourcePath()->utf8String();
    std::string pathStr(cPath);
    pathStr += "/XXdShader.metallib";
    auto mtLibURL = TransferPtr(NS::URL::alloc()->initFileURLWithPath(NS::String::string(pathStr.c_str(), NS::UTF8StringEncoding)));
	auto mtLibrary = TransferPtr(mtDevice->newLibrary(mtLibURL.get(), &e));	
	shaderMap["Render3DVertex"] = mtLibrary->newFunction(NS::String::string("render3DVertexMain", NS::UTF8StringEncoding));
    shaderMap["Render3DFragment"] = mtLibrary->newFunction(NS::String::string("render3DFragmentMain", NS::UTF8StringEncoding));
	shaderMap["Render2DVertex"] = mtLibrary->newFunction(NS::String::string("render2DVertexMain", NS::UTF8StringEncoding));
    shaderMap["Render2DFragment"] = mtLibrary->newFunction(NS::String::string("render2DFragmentMain", NS::UTF8StringEncoding));
}
