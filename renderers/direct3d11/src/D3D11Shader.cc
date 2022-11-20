
#include "Geoxide/D3D11Shader.h"

namespace Geoxide {

	D3D11VertexShader::D3D11VertexShader(D3D11RendererBase* gfx, const ShaderInit& args)
	{
		gfx->createShader(args.code, args.codeSize,
			args.entryPoint, "vs_4_0", args.isCodeCompiled,
			shader.GetAddressOf(), layout.GetAddressOf());

		WVPMatrixOffset = args.WVPMatrixOffset;
		worldMatrixOffset = args.worldMatrixOffset;
	}

	D3D11PixelShader::D3D11PixelShader(D3D11RendererBase* gfx, const ShaderInit& args)
	{
		gfx->createShader(args.code, args.codeSize,
			args.entryPoint, "ps_4_0", args.isCodeCompiled,
			shader.GetAddressOf(), 0);
	}

}
