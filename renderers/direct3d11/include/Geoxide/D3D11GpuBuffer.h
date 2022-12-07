
#ifndef __GX_D3D11_GPU_BUFFER_H__
#define __GX_D3D11_GPU_BUFFER_H__

namespace Geoxide {

	class D3D11GpuBuffer : public GpuBuffer
	{
	public:
		D3D11GpuBuffer(D3D11RendererBase* gfx, const GpuBufferInit& args);
		~D3D11GpuBuffer() = default;

		uint32_t getSize() const override { return size; }

		void write(const void* data, uint32_t start, uint32_t count) override;

	public:
		ComPtr<ID3D11Buffer> buffer;
		ComPtr<ID3D11DeviceContext> ctx;
		uint32_t size;
	};

}

#endif
