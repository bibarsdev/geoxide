
#include "Geoxide/ShaderManager.h"

#define NEXT_TOKEN() token = strtok(0, delim)
#define TOKEN_EQUAL(s) (strcmp(token, s) == 0)

#define UNEXPECTED_TOKEN()\
Log::Error("\'" + filepath + "\': Unexpected token \'" + token + "\'"); \
return 0

#define DICT_CHECK(dict, dest_member)\
auto& iter = dict.find(token);\
if (iter != dict.end())\
pass.dest_member = iter->second;\
else\
{\
	UNEXPECTED_TOKEN();\
}

#define DICT_PARSE(dict, dest_member, name)\
if (TOKEN_EQUAL(name))\
{\
	NEXT_TOKEN();\
	DICT_CHECK(dict, dest_member);\
}

#define COMPILE_SHADER(shader_type, dest_member)\
NEXT_TOKEN();\
auto& iter = compiledShaders.find(token);\
if (iter != compiledShaders.end())\
{\
	pass.dest_member = iter->second.get();\
}\
else\
{\
	Local<uint8_t> byteCode;\
	uint32_t byteCodeSize = 0;\
	ShaderCompileInput input = {};\
	input.code = buffer2Loc.get();\
	input.codeSize = programOffset;\
	input.entry = token;\
	input.name = filepath;\
	input.type = shader_type;\
	dev->compileShader(input, (void**)&byteCode, &byteCodeSize);\
	ShaderDesc shaderDesc = {};\
	shaderDesc.type = shader_type;\
	pass.dest_member = compiledShaders[token] = dev->newShader(shaderDesc, byteCode.get(), byteCodeSize);\
	pass.dest_member->addRef();\
}\


namespace Geoxide {

	const RenderStateDesc ShaderManager::kDefaultState = { kRenderModeForward, kBlendOpaque, kCullBackFace, kSamplerLinearWrap, false, true, 1.f };

	const std::unordered_map<std::string, RenderMode> kRenderModeDict = {
		{"Deferred", kRenderModeDeferred},
		{"Forward", kRenderModeForward},
	};

	const std::unordered_map<std::string, BlendType> kBlendDict = {
		{"Opaque", kBlendOpaque},
		{"AlphaBlend", kBlendAlphaBlend},
		{"Additive", kBlendAdditive},
		{"NonPremultiplied", kBlendNonPremultiplied},
	};

	const std::unordered_map<std::string, CullType> kCullDict = {
		{"None", kCullNone},
		{"Back", kCullBackFace},
		{"Front", kCullFrontFace},
	};

	const std::unordered_map<std::string, SamplerType> kSamplerDict = {
		{"PointWarp", kSamplerPointWrap},
		{"PointClamp", kSamplerPointClamp},
		{"LinearWarp", kSamplerLinearWrap},
		{"LinearClamp", kSamplerLinearClamp},
		{"AnisotropicWarp", kSamplerAnisotropicWrap},
		{"AnisotropicClamp", kSamplerAnisotropicClamp},
	};

	const std::unordered_map<std::string, bool> kBoolDict = {
		{"On", true},
		{"Off", false},
		{"True", true},
		{"False", false},
		{"1", true},
		{"0", false},
		{"Yes", true},
		{"No", false},
		{"Enable", true},
		{"Disable", false},
		{"Enabled", true},
		{"Disabled", false},
	};

	Ref<GpuProgram> ShaderManager::newProgramFromFile(RendererDevice* dev, const std::string& filepath)
	{
		std::ifstream file(filepath, std::ios::binary);

		if (!file.is_open())
		{
			Log::Error("Failed to open shader file \'" + filepath + "\'");
			return 0;
		}

		file.seekg(0, std::ios::end);
		size_t size = file.tellg();
		file.seekg(0, std::ios::beg);

		if (!size)
		{
			Log::Error("Shader file \'" + filepath + "\' is too small");
			return 0;
		}

		Local<char> buffer1Loc = Local<char>(new char[size + 1]);
		Local<char> buffer2Loc = Local<char>(new char[size + 1]);

		file.read(buffer1Loc.get(), size);
		buffer1Loc.get()[size] = 0;

		file.close();

		memcpy(buffer2Loc.get(), buffer1Loc.get(), size + 1);

		std::vector<GpuPassDesc> passes;
		std::unordered_map<std::string, Ref<Shader>> compiledShaders;

		static const char delim[] = " \t\n\r";

		char* buffer = buffer1Loc.get();

		char* token = strtok(buffer, delim);

		while (token)
		{
			bool programParsed = false;

			if (TOKEN_EQUAL("program"))
			{
				uint32_t programOffset = token - buffer;

				NEXT_TOKEN();

				if (TOKEN_EQUAL("{"))
				{
					NEXT_TOKEN();

					while (!TOKEN_EQUAL("}"))
					{
						if (TOKEN_EQUAL("pass"))
						{
							NEXT_TOKEN();

							if (TOKEN_EQUAL("{"))
							{
								NEXT_TOKEN();

								GpuPassDesc pass = { 0, 0, 0, kDefaultState };

								while (!TOKEN_EQUAL("}"))
								{
									if (TOKEN_EQUAL("Vertex"))
									{
										if (pass.vertexShader)
										{
											Log::Error("\'" + filepath + "\': Pass already has a vertex shader");
											return 0;
										}

										COMPILE_SHADER(kShaderVertex, vertexShader);
									}
									if (TOKEN_EQUAL("VertexSkinned"))
									{
										if (pass.skinnedVertexShader)
										{
											Log::Error("\'" + filepath + "\': Pass already has a skinned vertex shader");
											return 0;
										}

										COMPILE_SHADER(kShaderVertex, skinnedVertexShader);
									}
									else if (TOKEN_EQUAL("Fragment"))
									{
										if (pass.fragmentShader)
										{
											Log::Error("\'" + filepath + "\': Pass already has a fragment shader");
											return 0;
										}

										COMPILE_SHADER(kShaderFragment, fragmentShader);
									}
									else DICT_PARSE(kRenderModeDict, state.mode, "Lighting")
									else DICT_PARSE(kBlendDict, state.blend, "Blend")
									else DICT_PARSE(kCullDict, state.cull, "Cull")
									else DICT_PARSE(kSamplerDict, state.sampler, "Sampler")
									else DICT_PARSE(kBoolDict, state.wireFrame, "WireFrame")
									else DICT_PARSE(kBoolDict, state.zWrite, "ZWrite")
									
									NEXT_TOKEN();
								}

								if (!pass.vertexShader)
								{
									Log::Error("\'" + filepath + "\': No vertex shader specified");
									return 0;
								}

								if (!pass.fragmentShader)
								{
									Log::Error("\'" + filepath + "\': No fragment shader specified");
									return 0;
								}

								passes.push_back(pass);
							}
							else
							{
								UNEXPECTED_TOKEN();
							}

						}

						NEXT_TOKEN();
					}

					if (passes.empty())
					{
						Log::Error("\'" + filepath + "\': No passes parsed");
						return 0;
					}

				}
				else
				{
					UNEXPECTED_TOKEN();
				}

				programParsed = true;
			}

			NEXT_TOKEN();

			if (programParsed && token)
			{
				Log::Error("\'" + filepath + "\': Program descriptor is not at the end of file");
				return 0;
			}
		}

		GpuProgramDesc programDesc = {};
		programDesc.numPasses = passes.size();

		return dev->newProgram(programDesc, passes.data());
	}

}
