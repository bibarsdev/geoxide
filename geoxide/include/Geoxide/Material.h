
#ifndef __GX_MATERIAL_H__
#define __GX_MATERIAL_H__

#include "GpuProgram.h"
#include "Texture.h"
#include "GpuBuffer.h"

namespace Geoxide {

	class Material
	{
	public:
		struct Reflection
		{
			uint32_t sceneBufferIndex;
			uint32_t lightBufferIndex;
			uint32_t materialBufferIndex;
		};

	public:
		Material() :
			mTextures(0), mNumTextures(0), mProgram(0), mDataSize(0),
			mReflection({}) {}

		GpuProgram* getGpuProgram() const { return mProgram; }

		Texture* const* getTextures() const { return mTextures; }
		uint32_t getNumTextures() const { return mNumTextures; }

		uint32_t getSceneBufferIndex() const { return mReflection.sceneBufferIndex; }
		uint32_t getLightBufferIndex() const { return mReflection.lightBufferIndex; }
		uint32_t getMaterialBufferIndex() const { return mReflection.materialBufferIndex; }

		const uint8_t* getData() const { return mData.get(); }
		uint32_t getDataSize() const { return mDataSize; }

		const RenderState& getRenderState() { return mState; }

	private:
		GpuProgram* mProgram;
		Texture** mTextures;
		uint32_t mNumTextures;
		Reflection mReflection;
		Local<uint8_t> mData;
		uint32_t mDataSize;
		RenderState mState;

	private:
		friend class ResourceManager;
	};

}

#endif // !__GX_MATERIAL_H__

