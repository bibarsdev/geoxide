
#ifndef __GX_D3D11_SHADER_H__
#define __GX_D3D11_SHADER_H__

namespace Geoxide {

	class D3D11Shader : public Shader
	{
	public:
		virtual ~D3D11Shader() = default;

		ComPtr<ID3D11DeviceChild> shader;
	};

	class D3D11VertexShader : public D3D11Shader
	{
	public:
		D3D11VertexShader(D3D11RendererBase* gfx, const ShaderInit& args);
		virtual ~D3D11VertexShader() = default;

		ComPtr<ID3D11InputLayout> layout;
		UINT WVPMatrixOffset, worldMatrixOffset;
	};

	class D3D11PixelShader : public D3D11Shader
	{
	public:
		D3D11PixelShader(D3D11RendererBase* gfx, const ShaderInit& args);
		virtual ~D3D11PixelShader() = default;
	};

}

#endif
