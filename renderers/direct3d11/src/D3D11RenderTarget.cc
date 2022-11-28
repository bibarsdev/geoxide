
#include "Geoxide/D3D11RenderTarget.h"
#include "Geoxide/D3D11Texture.h"

namespace Geoxide {

	D3D11RenderTarget::D3D11RenderTarget(D3D11RendererBase* gfx, const RenderTargetInit& args)
	{
		D3D11Texture* texture = (D3D11Texture*)args.texture;

		gfx->createRenderTargetView((ID3D11Texture2D*)texture->texture.Get(), args.arrayIndex, rtv.GetAddressOf(), &vp);

		Log::Info("Created new D3D11RenderTarget \'" + args.name + "\'");
	}

}
