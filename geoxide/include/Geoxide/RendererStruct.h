
#ifndef __GX_RENDERER_STRUCT_H__
#define __GX_RENDERER_STRUCT_H__

#include "RendererEnum.h"
#include "Texture.h"

namespace Geoxide {

	struct InputElement
	{
		const char* semantic;
		uint32_t semanticIndex;
		uint32_t offset, length;
		DataType type;
	};

	struct GpuProgramInit
	{
		struct
		{
			const void* byteCode;
			size_t byteCodeSize;
		} vertex, pixel;
		const InputElement* inputLayout;
		uint32_t inputLayoutSize;
	};

	struct TextureInit
	{
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
		const void* vertexData;
		const void* indexData;
		uint32_t vertexLength;
		uint32_t indexLength;
		uint32_t vertexDataSize;
		uint32_t indexDataSize;
	};

	struct RenderTargetInit
	{
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
