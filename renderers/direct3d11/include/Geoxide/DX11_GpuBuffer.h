
#ifndef __GX_DX11_GPU_BUFFER_H__
#define __GX_DX11_GPU_BUFFER_H__

#include <Geoxide/GpuBuffer.h>

namespace Geoxide {

	struct DX11_GpuBuffer : GpuBuffer
	{
		GX_IMPLEMENT_INTERNAL();

		ComPtr<ID3D11Buffer> buffer;
	};

	struct DX11_GpuBufferShader : DX11_GpuBuffer
	{
	};

	struct DX11_GpuBufferVertex : DX11_GpuBuffer
	{
		UINT vertexLength;
	};

	struct DX11_GpuBufferIndex : DX11_GpuBuffer
	{
		DXGI_FORMAT format;
	};

}

#endif
