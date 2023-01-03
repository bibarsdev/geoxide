
#ifndef __GX_DDS_LOADER_H__
#define __GX_DDS_LOADER_H__

namespace Geoxide {

	using DXGI_FORMAT = uint32_t;

	enum DDSFlag
	{
		kDDSFlagCaps = 0x1,
		kDDSFlagHeight = 0x2,
		kDDSFlagWidth = 0x4,
		kDDSFlagPitch = 0x8,
		kDDSFlagPixelFormat = 0x1000,
		kDDSFlagMipMapCount = 0x20000,
		kDDSFlagLinearSize = 0x80000,
		kDDSFlagDepth = 0x800000,
		kDDSFlagRequired = kDDSFlagCaps | kDDSFlagHeight | kDDSFlagWidth | kDDSFlagPixelFormat,
	};
	
	enum DDSPixelFlag
	{
		kDDSPixelFlagAlphaPixels = 0x1,
		kDDSPixelFlagAlpha = 0x2,
		kDDSPixelFlagFourCC = 0x4,
		kDDSPixelFlagRGB = 0x40,
		kDDSPixelFlagYUV = 0x200,
		kDDSPixelFlagLuminance = 0x20000,
	};

	enum DDSFourCC
	{
		kDDSFourCCDXT1 = '1TXD',
		kDDSFourCCDXT2 = '2TXD',
		kDDSFourCCDXT3 = '3TXD',
		kDDSFourCCDXT4 = '4TXD',
		kDDSFourCCDXT5 = '5TXD',
		kDDSFourCCBC4U = 'U4CB',
		kDDSFourCCATI1 = '1ITA',
		kDDSFourCCATI2 = '2ITA',
	};

	enum DDSComplexity
	{
		kDDSComplexityFlagComplex = 0x8,
		kDDSComplexityFlagMipMap = 0x400000,
		kDDSComplexityFlagTexture = 0x1000,
		kDDSComplexityFlagRequired = kDDSComplexityFlagTexture,
	};

	enum DDSDetailFlag
	{
		kDDSDetailFlagCubeMap = 0x200,
		kDDSDetailFlagCubeMapPositiveX = 0x400,
		kDDSDetailFlagCubeMapNegativeX = 0x800,
		kDDSDetailFlagCubeMapPositiveY = 0x1000,
		kDDSDetailFlagCubeMapNegativeY = 0x2000,
		kDDSDetailFlagCubeMapPositiveZ = 0x4000,
		kDDSDetailFlagCubeMapNegativeZ = 0x8000,
		kDDSDetailFlagVolume = 0x200000,
	};

	struct DDSData
	{
		struct PixelFormat
		{
			uint32_t  headerSize;
			uint32_t  flags;
			DDSFourCC  fourCC;
			uint32_t  RGBBitCount;
			uint32_t  RBitMask;
			uint32_t  GBitMask;
			uint32_t  BBitMask;
			uint32_t  ABitMask;
		};

		struct
		{
			uint32_t headerSize;
			uint32_t flags;
			uint32_t height;
			uint32_t width;
			uint32_t pitchOrLinearSize;
			uint32_t depth;
			uint32_t mipMapCount;
			uint32_t unused1[11];
			PixelFormat ddspf;
			uint32_t complexityFlags;
			uint32_t detailFlags;
			uint32_t unused2;
			uint32_t unused3;
			uint32_t unused4;
		} desc;

		size_t dataSize;
		void* data;
	};

	class DDSLoader
	{
	public:
		// returns read bytes size
		// make sure to call DDSLoader::Free() after you're done with dds
		static size_t Read(const std::string& filepath, DDSData& dds);

		// returns written bytes size
		static size_t Write(const DDSData& dds, const std::string& filepath);

		static void Free(DDSData& dds);
	};
}

#endif // !__ENGINE_DDS_H__
