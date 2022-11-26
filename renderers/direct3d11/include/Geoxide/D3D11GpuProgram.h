
#ifndef __GX_D3D11_GPU_PROGRAM_H__
#define __GX_D3D11_GPU_PROGRAM_H__

namespace Geoxide {

	class D3D11GpuProgram : public GpuProgram
	{
	public:
		D3D11GpuProgram(D3D11RendererBase* gfx, const GpuProgramInit& args);
		~D3D11GpuProgram() = default;

		ComPtr<ID3D11VertexShader> vertexShader;
		ComPtr<ID3D11PixelShader> pixelShader;
		ComPtr<ID3D11InputLayout> inputLayout;
		GX_DECLARE_LOG("D3D11GpuProgram");
	};

}

#endif
