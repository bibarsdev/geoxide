
#ifndef __GX_D3D11_TEXTURE_H__
#define __GX_D3D11_TEXTURE_H__

namespace Geoxide {

	class D3D11Texture : public Texture
	{
	public:
		~D3D11Texture() = default;

		ComPtr<ID3D11ShaderResourceView> srv;
		ComPtr<ID3D11Resource> texture;
	};

	class D3D11Texture2D : public D3D11Texture
	{
	public:
		D3D11Texture2D(D3D11RendererBase* gfx, const TextureInit& args);
		~D3D11Texture2D() = default;
	};

	// TODO

}

#endif
