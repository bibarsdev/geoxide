
#ifndef __GX_MATERIAL_H__
#define __GX_MATERIAL_H__

#include "Texture.h"
#include "Shader.h"

namespace Geoxide {

	class Material
	{
	public:
		Material();
		Material(size_t uniformDataSize, Shader* pixelShader);
		~Material();

		std::vector<Texture*>& getTextures() { return textures; }
		Shader* getPixelShader() { return pixelShader; }
		void* getUniformData() { return uniformData; }
		size_t getUniformDataSize() { return uniformDataSize; }

		void allocateUniformData(uint32_t size);
		void copyUniformData(uint32_t destOffset, const void* src, uint32_t size);

	protected:
		std::vector<Texture*> textures;
		void* uniformData;
		size_t uniformDataSize;
		Shader* pixelShader;
	};

}

#endif // !__GX_MATERIAL_H__

