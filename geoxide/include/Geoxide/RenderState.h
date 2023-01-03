
#ifndef __GX_RENDER_STATE_H__
#define __GX_RENDER_STATE_H__

namespace Geoxide {

	enum BlendType
	{
		kBlendOpaque,
		kBlendAlphaBlend,
		kBlendAdditive,
		kBlendNonPremultiplied,
	};

	enum CullType
	{
		kCullNone,
		kCullFrontFace,
		kCullBackFace,
	};

	enum SamplerType
	{
		kSamplerPointWrap,
		kSamplerPointClamp,
		kSamplerLinearWrap,
		kSamplerLinearClamp,
		kSamplerAnisotropicWrap,
		kSamplerAnisotropicClamp,
	};

	enum RenderMode
	{
		kRenderModeForward,
		kRenderModeDeferred,
	};

	struct RenderStateDesc
	{
		RenderMode mode;
		BlendType blend;
		CullType cull;
		SamplerType sampler;
		bool wireFrame;
		bool zWrite;
		float opacity;
	};

}

#endif // !__GX_RENDER_STATE_H__

