
#ifndef __GX_GPU_PROGRAM_H__
#define __GX_GPU_PROGRAM_H__

namespace Geoxide {

	class GpuProgram
	{
	public:
		virtual ~GpuProgram() = default;

		virtual uint32_t getGlobalVariableOffset(const std::string& variable) const = 0;
		virtual uint32_t getVSGlobalsSize() const = 0;
		virtual uint32_t getPSGlobalsSize() const = 0;
	};

}

#endif // !__GX_SHADER_H__

