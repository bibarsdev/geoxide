
#ifndef __GX_DX11_SHADER_H__
#define __GX_DX11_SHADER_H__

#include <Geoxide/Shader.h>

namespace Geoxide {

	struct DX11_Shader : Shader
	{
		GX_IMPLEMENT_INTERNAL();
	};

	struct DX11_VertexShader : DX11_Shader
	{
		ComPtr<ID3D11VertexShader> vertexShader;
		ComPtr<ID3D11InputLayout> inputLayout;
	};

	struct DX11_PixelShader : DX11_Shader
	{
		ComPtr<ID3D11PixelShader> pixelShader;
	};

}

#endif
