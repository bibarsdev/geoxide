
#ifndef __GX_GPU_BUFFER_H__
#define __GX_GPU_BUFFER_H__

namespace Geoxide {

	class GpuBuffer
	{
	public:
		virtual ~GpuBuffer() = default;

		virtual uint32_t getSize() const = 0;

		virtual void write(const void* data, uint32_t start, uint32_t count) = 0;
	};

}

#endif // !__GX_SHADER_H__

