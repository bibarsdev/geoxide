
#ifndef __GX_RENDERER_STRUCT_H__
#define __GX_RENDERER_STRUCT_H__

#include "RendererEnum.h"
#include "Texture.h"

namespace Geoxide {

	struct GpuProgramInit
	{
		std::string name;
		struct
		{
			std::string name;
			const void* code;
			size_t codeSize;
			const char* entry;
			bool isCompiled;
		} vertex, pixel;
	};

	struct TextureInit
	{
		std::string name;
		TextureType type;
		const void* pixelData;
		uint32_t width, height;
		uint32_t arraySize;
		uint32_t mipLevels;
		ColorFormat format;
		bool renderTo;
	};

	struct MeshDataInit
	{
		std::string name;
		const void* vertexData;
		const void* indexData;
		uint32_t vertexLength;
		uint32_t indexLength;
		uint32_t vertexDataSize;
		uint32_t indexDataSize;
	};

	struct RenderTargetInit
	{
		std::string name;
		Texture* texture;
		uint32_t arrayIndex;
	};

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

	struct LookAtMatrixInput
	{
		Vector position;
		Vector center;
		Vector up;
	};

	struct DrawInput
	{
		MeshData* meshData;
		uint32_t indexStart, indexCount;
		PrimitiveTopology topology;
		GpuProgram* program;
		const void* vsUniformData;
		const void* psUniformData;
		uint32_t vsUniformDataSize;
		uint32_t psUniformDataSize;
		Texture* const* textures; // will be bound in the same order as given starting from slot 0
		uint32_t numTextures;
	};

}

#endif
