
#ifndef __GX_MATERIAL_H__
#define __GX_MATERIAL_H__

#include "GpuProgram.h"
#include "Texture.h"

namespace Geoxide {

	class Material
	{
	public:
		Material() :
			mProgram(0), mVSDataSize(0), mPSDataSize(0), mWorldViewProjOffset(-1), mWorldOffset(-1), mTimeOffset(-1) {}

		Material(GpuProgram* program) :
			mProgram(program),
			mVSDataSize(program->getVSGlobalsSize()),
			mPSDataSize(program->getPSGlobalsSize()),
			mWorldViewProjOffset(program->getGlobalVariableOffset("gWorldViewProj")),
			mWorldOffset(program->getGlobalVariableOffset("gWorld")),
			mTimeOffset(program->getGlobalVariableOffset("gTime")) {}

		~Material() {}

		const auto& getTextures() const { return mTextures; }
		Texture* getTexture(uint32_t i) const { return mTextures[i]; }
		GpuProgram* getGpuProgram() const { return mProgram; }
		uint32_t getVSDataSize() const { return mVSDataSize; }
		uint32_t getPSDataSize() const { return mPSDataSize; }
		uint32_t getWorldViewProjOffset() const { return mWorldViewProjOffset; }
		uint32_t getWorldOffset() const { return mWorldOffset; }
		uint32_t getTimeOffset() const { return mTimeOffset; }

		void addTexture(Texture* texture) { mTextures.push_back(texture); }

	protected:
		std::vector<Texture*> mTextures;
		GpuProgram* mProgram;
		uint32_t mVSDataSize;
		uint32_t mPSDataSize;
		uint32_t mWorldViewProjOffset;
		uint32_t mWorldOffset;
		uint32_t mTimeOffset;
	};

}

#endif // !__GX_MATERIAL_H__

