
#ifndef __GX_DX11_GPU_PROGRAM_H__
#define __GX_DX11_GPU_PROGRAM_H__

#include <Geoxide/GpuProgram.h>

namespace Geoxide {

	struct DX11_GpuPass
	{
		ComPtr<ID3D11VertexShader> vertexShader;
		ComPtr<ID3D11VertexShader> skinnedVertexShader;
		ComPtr<ID3D11PixelShader> pixelShader;
		ComPtr<ID3D11InputLayout> inputLayout;
		ComPtr<ID3D11InputLayout> skinnedInputLayout;

		ComPtr<ID3D11DepthStencilState> depthState;
		ComPtr<ID3D11RasterizerState> rasterState;
		ComPtr<ID3D11SamplerState> samplerState;
		ComPtr<ID3D11BlendState> blendState;

		UINT sampleMask;

		RenderMode mode;
	};

	struct DX11_GpuProgram : GpuProgram
	{
		GX_IMPLEMENT_INTERNAL();

		Local<DX11_GpuPass> dxPasses;
	};

}

#endif
