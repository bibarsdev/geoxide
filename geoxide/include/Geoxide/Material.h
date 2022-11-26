
#ifndef __GX_MATERIAL_H__
#define __GX_MATERIAL_H__

#include "GpuProgram.h"
#include "Texture.h"

namespace Geoxide {

	class Material
	{
	public:
		Material() : mProgram(0) {}
		Material(GpuProgram* program) : mProgram(program) {}

		const auto& getTextures() const { return mTextures; }
		Texture* getTexture(uint32_t i) const { return mTextures[i]; }
		GpuProgram* getGpuProgram() const { return mProgram; }

		void addTexture(Texture* texture) { mTextures.push_back(texture); }

	protected:
		std::vector<Texture*> mTextures;
		GpuProgram* mProgram;
	};

}

#endif // !__GX_MATERIAL_H__

