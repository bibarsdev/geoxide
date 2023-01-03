
#ifndef __GX_GPU_BUFFER_H__
#define __GX_GPU_BUFFER_H__

namespace Geoxide {

	enum GpuBufferType
	{
		kGpuBufferShader,
		kGpuBufferVertex,
		kGpuBufferIndex,
	};

	struct GpuBufferDesc
	{
		GpuBufferType type;
		uint32_t size;
		uint32_t stride;
	};

	struct GpuBuffer : Internal
	{
		GpuBufferDesc desc;
	};

}

#endif // !__GX_SHADER_H__

