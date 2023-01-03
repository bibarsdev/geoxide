
#ifndef __GX_TEXTURE_H__
#define __GX_TEXTURE_H__

#include "Format.h"

namespace Geoxide {

	enum TextureDimension
	{
		kTextureDimension2D,
	};

	struct TextureDesc
	{
		TextureDimension dimension;
		uint32_t width, height;
		uint32_t arraySizeOrDepth;
		uint32_t mipLevels;
		uint32_t pitch;
		FormatDesc format;
		bool cube;
	};

	struct Texture : Internal
	{
		TextureDesc desc;
	};

}

#endif // !__GX_TEXTURE_H__

