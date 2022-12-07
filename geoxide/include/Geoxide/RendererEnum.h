
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
		kCullTypeFrontFace,
		kCullTypeBackFace,
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
