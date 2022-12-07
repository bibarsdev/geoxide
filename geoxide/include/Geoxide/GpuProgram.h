
#ifndef __GX_GPU_PROGRAM_H__
#define __GX_GPU_PROGRAM_H__

namespace Geoxide {

	enum GpuResourceType
	{
		kGpuResourceTypeUnknwon,
		kGpuResourceTypeTexture2D,
	};

	class GpuProgramResource
	{
	public:
		virtual ~GpuProgramResource() = default;

		virtual GpuResourceType getType() const = 0;
		virtual uint32_t getBindOffset() const = 0;
	};

	class GpuProgramVariable
	{
	public:
		virtual ~GpuProgramVariable() = default;

		virtual uint32_t getByteOffset() const = 0;
		virtual uint32_t getSize() const = 0;
	};

	class GpuProgramBuffer
	{
	public:
		virtual ~GpuProgramBuffer() = default;

		virtual const GpuProgramVariable* getVariableByIndex(uint32_t i) const = 0;
		virtual const GpuProgramVariable* getVariableByName(const std::string& name) const = 0;

		virtual uint32_t getNumVariables() const = 0;
		virtual uint32_t getBindOffset() const = 0;
		virtual uint32_t getSize() const = 0;
	};

	class GpuProgram
	{
	public:
		virtual ~GpuProgram() = default;

		virtual const GpuProgramBuffer* getBufferByIndex(uint32_t i) const = 0;
		virtual const GpuProgramBuffer* getBufferByName(const std::string& name) const = 0;

		virtual const GpuProgramResource* getResourceByIndex(uint32_t i) const = 0;
		virtual const GpuProgramResource* getResourceByName(const std::string& name) const = 0;

		virtual uint32_t getNumBuffers() const = 0;
		virtual uint32_t getNumResources() const = 0;
	};

}

#endif // !__GX_SHADER_H__

