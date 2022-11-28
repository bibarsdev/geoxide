
#ifndef __GX_D3D11_GPU_PROGRAM_H__
#define __GX_D3D11_GPU_PROGRAM_H__

namespace Geoxide {

	class D3D11GpuProgram : public GpuProgram
	{
	public:
		D3D11GpuProgram(D3D11RendererBase* gfx, const GpuProgramInit& args);
		~D3D11GpuProgram() = default;

		uint32_t getGlobalVariableOffset(const std::string& variable) const override;
		uint32_t getVSGlobalsSize() const override { return vsGlobalsSize; }
		uint32_t getPSGlobalsSize() const override { return psGlobalsSize; }

		ComPtr<ID3D11VertexShader> vertexShader;
		ComPtr<ID3D11PixelShader> pixelShader;
		ComPtr<ID3D11InputLayout> inputLayout;
		std::unordered_map<std::string, uint32_t> globalVars;
		uint32_t vsGlobalsSize, psGlobalsSize;
	};

}

#endif
