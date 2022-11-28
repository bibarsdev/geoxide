
#include "Geoxide/D3D11Texture.h"

namespace Geoxide {

	D3D11Texture2D::D3D11Texture2D(D3D11RendererBase* gfx, const TextureInit& args)
	{
		gfx->createTexture2D(
			args.width, args.height, args.pixelData,
			args.arraySize, args.mipLevels, gfx->translateFormat(args.format),
			(ID3D11Texture2D**)texture.GetAddressOf(), srv.GetAddressOf(),
			args.renderTo, args.renderTo, false, false);

		Log::Info("Created new D3D11Texture2D \'" + args.name + "\'");
	}

}
