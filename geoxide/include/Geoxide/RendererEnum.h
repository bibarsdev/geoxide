
#ifndef __GX_RENDERER_ENUM_H__
#define __GX_RENDERER_ENUM_H__

namespace Geoxide {

	enum DataType
	{
		kDataTypeUnknown,
		kDataTypeFloat,
		kDataTypeInt,
		kDataTypeUInt,
	};

	enum PrimitiveTopology
	{
		kPrimitiveTopologyUnknown,
		kPrimitiveTopologyPointList,
		kPrimitiveTopologyLineList,
		kPrimitiveTopologyTriangleList,
	};

	enum ColorFormat
	{
		kColorFormatUnknown,
		kColorFormatRGBA32,
	};

	enum TextureType
	{
		kTextureTypeUnknown,
		kTextureType2D,
	};
}

#endif
