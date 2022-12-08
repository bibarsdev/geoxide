
#include "Geoxide/D3D11DepthTarget.h"
#include "Geoxide/D3D11Texture.h"

namespace Geoxide {

	D3D11DepthTarget::D3D11DepthTarget(D3D11RendererBase* gfx, const DepthTargetInit& args)
	{
		D3D11Texture* texture = (D3D11Texture*)args.texture;

		gfx->createDepthStencilView((ID3D11Texture2D*)texture->texture.Get(), args.arrayIndex, dsv.GetAddressOf());

		Log::Info("Created new D3D11DepthTarget \'" + args.name + "\'");
	}

}
