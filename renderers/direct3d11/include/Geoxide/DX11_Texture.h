
#ifndef __GX_DX11_TEXTURE_H__
#define __GX_DX11_TEXTURE_H__

#include <Geoxide/Texture.h>

namespace Geoxide {

	struct DX11_Texture : Texture
	{
		GX_IMPLEMENT_INTERNAL();

		ComPtr<ID3D11Resource> texture;
		ComPtr<ID3D11ShaderResourceView> SRV;
	};

}

#endif
