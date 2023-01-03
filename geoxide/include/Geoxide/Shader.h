
#ifndef __GX_SHADER_H__
#define __GX_SHADER_H__

namespace Geoxide {

	enum ShaderType
	{
		kShaderVertex,
		kShaderFragment,
	};

	enum ShaderResourceType
	{
		kShaderResourceTexture2D,
	};

	struct ShaderResourceDesc
	{
		ShaderResourceType type;
		uint32_t bindOffset;
	};

	struct ShaderVariableDesc
	{
		uint32_t byteOffset;
		uint32_t size;
	};

	struct ShaderBufferDesc
	{
		std::unordered_map<std::string, ShaderVariableDesc> vars;
		uint32_t bindOffset;
		uint32_t size;
	};

	struct ShaderReflectionDesc
	{
		std::unordered_map<std::string, ShaderBufferDesc> buffers;
		std::unordered_map<std::string, ShaderResourceDesc> resources;
	};

	struct ShaderDesc
	{
		ShaderType type;
	};

	struct Shader : Internal
	{
		ShaderDesc desc;
		ShaderReflectionDesc reflection;
	};

}

#endif // !__GX_SHADER_H__

