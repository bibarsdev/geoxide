
#ifndef __GX_DDS_LOADER_H__
#define __GX_DDS_LOADER_H__

// TODO

namespace Geoxide {

struct dds_pixelformat
{
	u32  size;
	u32  flags;
	u32  fourCC;
	u32  RGBBitCount;
	u32  RBitMask;
	u32  GBitMask;
	u32  BBitMask;
	u32  ABitMask;
};

struct dds_image
{
	struct header
	{
		u32           dize;
		u32           flags;
		u32           height;
		u32           width;
		u32           pitchOrLinearSize;
		u32           depth;
		u32           mipMapCount;
		u32           reserved1[11];
		dds_pixelformat ddspf;
		u32           caps;
		u32           caps2;
		u32           caps3;
		u32           caps4;
		u32           reserved2;
	} h;

	u8 data[1];
};

}

#endif // !__ENGINE_DDS_H__
