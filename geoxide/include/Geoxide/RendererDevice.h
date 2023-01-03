
#ifndef __GX_RENDERER_DEVICE_H__
#define __GX_RENDERER_DEVICE_H__

#include "Shader.h"
#include "GpuProgram.h"
#include "GpuBuffer.h"
#include "Texture.h"
#include "Mesh.h"

#include "RendererContext.h"

#include "Window.h"

#include <imgui.h>

namespace Geoxide {

	struct PerspectiveMatrixInput
	{
		float fov;
		float aspect;
		float nearZ;
		float farZ;
	};

	struct OrthographicMatrixInput
	{
		float width;
		float height;
		float nearZ;
		float farZ;
	};

	struct WriteBufferInput
	{
		GpuBuffer* buffer;
		const void* data;
		uint32_t destOffset;
		uint32_t dataSize;
	};

	struct ShaderCompileInput
	{
		std::string name;
		const void* code;
		uint32_t codeSize;
		const char* entry;
		ShaderType type;
	};

	class RendererDevice : public Internal
	{
	public:
		virtual Ref<RendererContext> getContext() = 0;

		virtual Ref<Shader> newShader(const ShaderDesc& desc, const void* byteCode, uint32_t byteCodeSize) = 0;
		virtual Ref<GpuProgram> newProgram(const GpuProgramDesc& desc, const GpuPassDesc* passes) = 0;
		virtual Ref<GpuBuffer> newBuffer(const GpuBufferDesc& desc, const void* data) = 0;
		virtual Ref<Texture> newTexture(const TextureDesc& desc, const void* data) = 0;
		virtual Ref<Mesh> newMesh(const MeshDesc& desc, const SubMeshDesc* subs) = 0;

		virtual Ref<RendererContext> newContext() = 0;

		virtual void makePerspectiveMatrix(const PerspectiveMatrixInput& args, float4x4& outMatrix) = 0;
		virtual void makeOrthographicMatrix(const OrthographicMatrixInput& args, float4x4& outMatrix) = 0;

		virtual void compileShader(const ShaderCompileInput& args, void** outByteCode, uint32_t* outByteCodeSize) = 0;

		virtual void writeBuffer(const WriteBufferInput& args) = 0;

		virtual void swapBuffers(uint32_t syncInterval) = 0;

		virtual void startImGuiFrame() = 0;
		virtual void drawImGui() = 0;

		static uint8_t* getSharedBufferPointer(Internal* buffer)
		{
			return (uint8_t*)buffer + sizeof(void*);
		}
	};

	using NewRendererDeviceProc = RendererDevice* (*)(Window*, ImGuiContext*, ImGuiMemAllocFunc, ImGuiMemFreeFunc, void*);
}

#endif
