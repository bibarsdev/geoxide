
#ifndef __GX_RENDERER_ENUM_H__
#define __GX_RENDERER_ENUM_H__

namespace Geoxide {

	enum PrimitiveTopology
	{
		kPrimitiveTopologyUnknown,
		kPrimitiveTopologyPointList,
		kPrimitiveTopologyLineList,
		kPrimitiveTopologyTriangleList,
	};

	enum ProjectionType
	{
		kProjectionTypeUnknown,
		kProjectionTypePerspective,
		kProjectionTypeOrthographic,
	};

	enum ColorFormat
	{
		kColorFormatUnknown,
		kColorFormatRGBA32,
	};

	enum ShaderType
	{
		kShaderTypeUnknown,
		kShaderTypeVertex,
		kShaderTypePixel,
	};

	enum TextureType
	{
		kTextureTypeUnknown,
		kTextureType2D,
	};
}

#endif
