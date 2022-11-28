
#ifndef __GX_D3D11_RENDER_TARGET_H__
#define __GX_D3D11_RENDER_TARGET_H__

namespace Geoxide {

	class D3D11RenderTarget : public RenderTarget
	{
	public:
		D3D11RenderTarget(D3D11RendererBase* gfx, const RenderTargetInit& args);
		~D3D11RenderTarget() = default;

		ComPtr<ID3D11RenderTargetView> rtv;
		D3D11_VIEWPORT vp;
	};

}

#endif
