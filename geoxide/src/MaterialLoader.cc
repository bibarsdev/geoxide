
#include "Geoxide/MaterialLoader.h"

#define NEXT_TOKEN() token = strtok(0, delim)
#define TOKEN_EQUAL(s) (strcmp(token, s) == 0)

#define UNEXPECTED_TOKEN()\
Log::Error("\'" + filepath + "\': Unexpected token \'" + token + "\'"); \
return 0

#define DICT_CHECK(dict, member)\
auto& iter = dict.find(token);\
if (iter != dict.end())\
material.member = iter->second;\
else\
{\
	UNEXPECTED_TOKEN();\
}

#define DICT_PARSE(dict, member, name)\
if (TOKEN_EQUAL(name))\
{\
	NEXT_TOKEN();\
	DICT_CHECK(dict, member);\
}

namespace Geoxide {

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

		static const char delim[] = " \t\n\r";

		char* token = strtok(buffer, delim);

		while (token)
		{
			if (TOKEN_EQUAL("shader"))
			{
				NEXT_TOKEN();
				material.shader = token;
			}
			else if (TOKEN_EQUAL("float4"))
			{
				char* name = NEXT_TOKEN();

				float x = (float)atof(NEXT_TOKEN());
				float y = (float)atof(NEXT_TOKEN());
				float z = (float)atof(NEXT_TOKEN());
				float w = (float)atof(NEXT_TOKEN());

				material.vector4s[name] = float4(x, y, z, w);
			}
			else if (TOKEN_EQUAL("float3"))
			{
				char* name = NEXT_TOKEN();

				float x = (float)atof(NEXT_TOKEN());
				float y = (float)atof(NEXT_TOKEN());
				float z = (float)atof(NEXT_TOKEN());

				material.vector3s[name] = float3(x, y, z);
			}
			else if (TOKEN_EQUAL("float2"))
			{
				char* name = NEXT_TOKEN();

				float x = (float)atof(NEXT_TOKEN());
				float y = (float)atof(NEXT_TOKEN());

				material.vector2s[name] = float2(x, y);
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

		file << "shader " << material.shader << '\n';

		file << '\n';
			
		for (auto& var : material.vector4s)
		{
			float4 vector = var.second;
			file << "float4 " << var.first << ' ' << vector.x << ' ' << vector.y << ' ' << vector.z << ' ' << vector.w << '\n';
		}

		for (auto& var : material.vector3s)
		{
			float3 vector = var.second;
			file << "float3 " << var.first << ' ' << vector.x << ' ' << vector.y << ' ' << vector.z<< '\n';
		}

		for (auto& var : material.vector2s)
		{
			float2 vector = var.second;
			file << "float2 " << var.first << ' ' << vector.x << ' ' << vector.y << '\n';
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
