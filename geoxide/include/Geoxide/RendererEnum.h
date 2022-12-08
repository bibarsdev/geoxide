
#ifndef __GX_RENDERER_ENUM_H__
#define __GX_RENDERER_ENUM_H__

namespace Geoxide {

	enum DataType
	{
		kDataTypeUnknown,
		kDataTypeFloat,
		kDataTypeInt,
		kDataTypeUInt,
		kDataTypeByte,
		kDataTypeBC3,
		kDataTypeBC4,
		kDataTypeBC5,
	};

	enum PrimitiveTopology
	{
		kPrimitiveTopologyUnknown,
		kPrimitiveTopologyPointList,
		kPrimitiveTopologyLineList,
		kPrimitiveTopologyTriangleList,
	};

	enum TextureType
	{
		kTextureTypeUnknown,
		kTextureType2D,
	};

	enum BlendType
	{
		kBlendTypeOpaque,
		kBlendTypeAlphaBlend,
		kBlendTypeAdditive,
		kBlendTypeNonPremultiplied,
	};

	enum CullType
	{
		kCullTypeNone,
		kCullTypeBackFace,
		kCullTypeFrontFace,
	};

	enum SamplerType
	{
		kSamplerTypePointWrap,
		kSamplerTypePointClamp,
		kSamplerTypeLinearWrap,
		kSamplerTypeLinearClamp,
		kSamplerTypeAnisotropicWrap,
		kSamplerTypeAnisotropicClamp,
	};
}

#endif
