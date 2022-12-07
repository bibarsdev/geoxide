
#include "Geoxide/MaterialLoader.h"

#define NEXT_TOKEN() token = strtok(0, delim)
#define TOKEN_EQUAL(s) (strcmp(token, s) == 0)

#define UNEXPECTED_TOKEN()\
Log::Error("\'" + filepath + "\': Unexpected token \'" + token + "\'"); \
return 0

#define STATE_CHECK(dict, name)\
auto& iter = dict.find(token);\
if (iter != dict.end())\
material.state.name = iter->second;\
else\
{\
	UNEXPECTED_TOKEN();\
}

#define STATE_PARSE(dict, name)\
if (TOKEN_EQUAL(#name))\
{\
	NEXT_TOKEN();\
	STATE_CHECK(dict, name);\
}

namespace Geoxide {

	const RenderState MaterialLoader::kDefaultState = { kBlendTypeOpaque, kCullTypeBackFace, kSamplerTypeLinearWrap, false, true, 1.f };

	const std::unordered_map<std::string, BlendType> kBlendDict = {
		{"opaque", kBlendTypeOpaque},
		{"alphaBlend", kBlendTypeAlphaBlend},
		{"additive", kBlendTypeAdditive},
		{"nonPremultiplied", kBlendTypeNonPremultiplied},
	};

	const std::unordered_map<std::string, CullType> kCullDict = {
		{"none", kCullTypeNone},
		{"backFace", kCullTypeBackFace},
		{"frontFace", kCullTypeFrontFace},
	};

	const std::unordered_map<std::string, SamplerType> kSamplerDict = {
		{"pointWarp", kSamplerTypePointWrap},
		{"pointClamp", kSamplerTypePointClamp},
		{"linearWarp", kSamplerTypeLinearWrap},
		{"linearClamp", kSamplerTypeLinearClamp},
		{"anisotropicWarp", kSamplerTypeAnisotropicWrap},
		{"anisotropicClamp", kSamplerTypeAnisotropicClamp},
	};

	const std::unordered_map<std::string, bool> kBoolDict = {
		{"on", true},
		{"off", false},
		{"true", true},
		{"false", false},
		{"1", true},
		{"0", false},
		{"yes", true},
		{"no", false},
	};

	const std::unordered_map<BlendType, std::string> kBlendDictRev = {
		{kBlendTypeOpaque, "opaque"},
		{kBlendTypeAlphaBlend, "alphaBlend"},
		{kBlendTypeAdditive, "additive"},
		{kBlendTypeNonPremultiplied, "nonPremultiplied"},
	};

	const std::unordered_map<CullType, std::string> kCullDictRev = {
		{kCullTypeNone, "none"},
		{kCullTypeBackFace, "backFace"},
		{kCullTypeFrontFace, "frontFace"},
	};

	const std::unordered_map<SamplerType, std::string> kSamplerDictRev = {
		{kSamplerTypePointWrap, "pointWarp"},
		{kSamplerTypePointClamp, "pointClamp"},
		{kSamplerTypeLinearWrap, "linearWarp"},
		{kSamplerTypeLinearClamp, "linearClamp"},
		{kSamplerTypeAnisotropicWrap, "anisotropicWarp"},
		{kSamplerTypeAnisotropicClamp, "anisotropicClamp"},
	};

	const std::unordered_map<bool, std::string> kBoolDictRev = {
		{true, "on"},
		{false, "off"},
	};

	size_t MaterialLoader::Read(const std::string& filepath, MaterialData& material)
	{
		std::ifstream file(filepath, std::ios::binary);

		if (!file.is_open())
		{
			Log::Error("Failed to open material file \'" + filepath + "\'");
			return 0;
		}

		file.seekg(0, std::ios::end);
		size_t size = file.tellg();
		file.seekg(0, std::ios::beg);

		if (!size)
		{
			Log::Error("Material file \'" + filepath + "\' is too small");
			return 0;
		}

		Local<char> bufferLoc = NewLocal<char>(size + 1);
		char* buffer = new char[size + 1];

		file.read(buffer, size);
		buffer[size] = 0;

		file.close();

		material.state = kDefaultState;

		static const char delim[] = " \t\n\r";

		char* token = strtok(buffer, delim);

		while (token)
		{
			if (TOKEN_EQUAL("set"))
			{
				NEXT_TOKEN();

				if (TOKEN_EQUAL("shader"))
				{
					NEXT_TOKEN();
					material.shader = token;
				}
				else if (TOKEN_EQUAL("opacity"))
				{
					NEXT_TOKEN();
					material.state.opacity = (float)atof(token);
				}
				else STATE_PARSE(kBlendDict, blend)
				else STATE_PARSE(kCullDict, cull)
				else STATE_PARSE(kSamplerDict, sampler)
				else STATE_PARSE(kBoolDict, wireFrame)
				else STATE_PARSE(kBoolDict, zWrite)
				else
				{
					UNEXPECTED_TOKEN();
				}

			}
			else if (TOKEN_EQUAL("vec"))
			{
				char* name = NEXT_TOKEN();

				float x = (float)atof(NEXT_TOKEN());
				float y = (float)atof(NEXT_TOKEN());
				float z = (float)atof(NEXT_TOKEN());
				float w = (float)atof(NEXT_TOKEN());

				material.vectors[name] = NewVector(x, y, z, w);
			}
			else if (TOKEN_EQUAL("float"))
			{
				char* name = NEXT_TOKEN();

				float v = (float)atof(NEXT_TOKEN());

				material.floats[name] = v;
			}
			else if (TOKEN_EQUAL("int"))
			{
				char* name = NEXT_TOKEN();

				int v = atoi(NEXT_TOKEN());

				material.ints[name] = v;
			}
			else if (TOKEN_EQUAL("bool"))
			{
				char* name = NEXT_TOKEN();

				bool v = atoi(NEXT_TOKEN());

				material.bools[name] = v;
			}
			else if (TOKEN_EQUAL("texture"))
			{
				char* name = NEXT_TOKEN();

				material.textures[name] = NEXT_TOKEN();
			}
			else
			{
				UNEXPECTED_TOKEN();
			}

			NEXT_TOKEN();
		}

		if (material.shader.empty())
		{
			Log::Error("\'" + filepath + "\': No shader specified");
			return 0;
		}

		return size;
	}

	size_t MaterialLoader::Write(const MaterialData& material, const std::string& filepath)
	{
		std::ofstream file(filepath);

		if (!file.is_open())
			return 0;

		if (material.shader.empty())
			return 0;

		file << "set shader " << material.shader << '\n';

		if (material.state.blend != kDefaultState.blend)
			file << "set blend " << kBlendDictRev.at(material.state.blend) << '\n';

		if (material.state.cull != kDefaultState.cull)
			file << "set cull " << kCullDictRev.at(material.state.cull) << '\n';

		if (material.state.sampler != kDefaultState.sampler)
			file << "set sampler " << kSamplerDictRev.at(material.state.sampler) << '\n';

		if (material.state.wireFrame != kDefaultState.wireFrame)
			file << "set wireFrame " << kBoolDictRev.at(material.state.wireFrame) << '\n';

		if (material.state.zWrite != kDefaultState.zWrite)
			file << "set zWrite " << kBoolDictRev.at(material.state.zWrite) << '\n';

		if (material.state.opacity != kDefaultState.opacity)
			file << "set opacity " << material.state.opacity << '\n';

		file << '\n';
			
		for (auto& var : material.vectors)
		{
			Vector vector = var.second;
			file << "vec " << var.first << ' ' << vector.x << ' ' << vector.y << ' ' << vector.z << ' ' << vector.w << '\n';
		}

		for (auto& var : material.floats)
			file << "float " << var.first << ' ' << var.second << '\n';

		for (auto& var : material.ints)
			file << "int " << var.first << ' ' << var.second << '\n';

		for (auto& var : material.bools)
			file << "bool " << var.first << ' ' << kBoolDictRev.at(var.second) << '\n';

		for (auto& var : material.textures)
			file << "texture " << var.first << ' ' << var.second << '\n';

		size_t size = file.tellp();

		return size;
	}
}
