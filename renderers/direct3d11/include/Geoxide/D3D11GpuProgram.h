
#ifndef __GX_D3D11_GPU_PROGRAM_H__
#define __GX_D3D11_GPU_PROGRAM_H__

namespace Geoxide {

	class D3D11GpuProgramResource : public GpuProgramResource
	{
	public:
		D3D11GpuProgramResource() :
			type(kGpuResourceTypeUnknwon), bindOffset(0) {}
		~D3D11GpuProgramResource() = default;

		GpuResourceType getType() const override { return type; }
		uint32_t getBindOffset() const override { return bindOffset; }

	public:
		GpuResourceType type;
		uint32_t bindOffset;
	};

	class D3D11GpuProgramVariable : public GpuProgramVariable
	{
	public:
		D3D11GpuProgramVariable() :
			byteOffset(0), size(0) {}
		~D3D11GpuProgramVariable() = default;

		uint32_t getByteOffset() const override { return byteOffset; }
		uint32_t getSize() const override { return size; }

	public:
		uint32_t byteOffset;
		uint32_t size;
	};

	class D3D11GpuProgramBuffer : public GpuProgramBuffer
	{
	public:
		D3D11GpuProgramBuffer() :
			bindOffset(0), size(0) {}
		~D3D11GpuProgramBuffer() = default;

		const GpuProgramVariable* getVariableByIndex(uint32_t i) const override { return &vars.begin().operator->()[i].second; }
		const GpuProgramVariable* getVariableByName(const std::string& name) const override;

		uint32_t getNumVariables() const override { return vars.size(); }
		uint32_t getBindOffset() const override { return bindOffset; }
		uint32_t getSize() const override { return size; }

	public:
		std::unordered_map<std::string, D3D11GpuProgramVariable> vars;
		uint32_t bindOffset;
		uint32_t size;
	};

	class D3D11GpuProgram : public GpuProgram
	{
	public:
		D3D11GpuProgram(D3D11RendererBase* gfx, const GpuProgramInit& args);
		~D3D11GpuProgram() = default;

		const GpuProgramBuffer* getBufferByIndex(uint32_t i) const override { return &buffers.begin().operator->()[i].second; }
		const GpuProgramBuffer* getBufferByName(const std::string& name) const override;

		const GpuProgramResource* getResourceByIndex(uint32_t i) const override { return &resources.begin().operator->()[i].second; }
		const GpuProgramResource* getResourceByName(const std::string& name) const override;

		uint32_t getNumBuffers() const override { return buffers.size(); }
		uint32_t getNumResources() const override { return resources.size(); }

	public:
		void reflect(ID3D11ShaderReflection* ref);

	public:
		ComPtr<ID3D11VertexShader> vertexShader;
		ComPtr<ID3D11PixelShader> pixelShader;
		ComPtr<ID3D11InputLayout> inputLayout;
		std::unordered_map<std::string, D3D11GpuProgramBuffer> buffers;
		std::unordered_map<std::string, D3D11GpuProgramResource> resources;
	};

}

#endif
