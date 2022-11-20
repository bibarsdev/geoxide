
#ifndef __GX_TEXTURE_H__
#define __GX_TEXTURE_H__

#include "RendererEnum.h"

namespace Geoxide {

	struct TextureInit
	{
		const void* pixelData;
		uint32_t width, height;
		uint32_t arraySize;
		uint32_t mipLevels;
		TextureType type;
		ColorFormat format;
	};

	class Texture
	{
	public:
		virtual ~Texture() = default;
	};

}

#endif // !__GX_TEXTURE_H__

