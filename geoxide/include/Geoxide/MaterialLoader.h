
#ifndef __GX_MATERIAL_LOADER_H__
#define __GX_MATERIAL_LOADER_H__

#include "RendererStruct.h"

namespace Geoxide {

	struct MaterialData
	{
		std::string shader;

		RenderState state;
		
		std::unordered_map<std::string, Vector> vectors;
		std::unordered_map<std::string, float> floats;
		std::unordered_map<std::string, int> ints;
		std::unordered_map<std::string, bool> bools;

		std::unordered_map<std::string, std::string> textures;
	};

	class MaterialLoader
	{
	public:
		// returns read bytes size
		static size_t Read(const std::string& filepath, MaterialData& material);

		// returns written bytes size
		static size_t Write(const MaterialData& material, const std::string& filepath);

	public:
		static const RenderState kDefaultState;
	};

}

#endif // !__ENGINE_DDS_H__
