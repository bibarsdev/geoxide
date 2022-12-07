
#include "Geoxide/D3D11GpuBuffer.h"

namespace Geoxide {

	D3D11GpuBuffer::D3D11GpuBuffer(D3D11RendererBase* gfx, const GpuBufferInit& args) :
		size(0)
	{
		gfx->createBuffer(
			args.stride, args.dataSize, args.data,
			args.shaderBuffer ? D3D11_BIND_CONSTANT_BUFFER : 0,
			buffer.GetAddressOf(), 0,
			false, args.shaderBuffer);

		ctx = gfx->ctx;
		size = args.dataSize;

		Log::Info("Created new D3D11GpuBuffer \'" + args.name + "\'");
	}

	void D3D11GpuBuffer::write(const void* data, uint32_t start, uint32_t count)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource = {};

		ctx->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		memcpy((uint8_t*)mappedResource.pData + start, data, count);

		ctx->Unmap(buffer.Get(), 0);
	}

}
