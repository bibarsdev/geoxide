
#ifndef __GX_RENDERER_STRUCT_H__
#define __GX_RENDERER_STRUCT_H__

#include "RendererEnum.h"
#include "Texture.h"
#include "GpuBuffer.h"
#include "MeshData.h"
#include "GpuProgram.h"

namespace Geoxide {

	struct ColorFormat
	{
		DataType type;
		uint32_t bitCount;
		uint32_t RMask;
		uint32_t GMask;
		uint32_t BMask;
		uint32_t AMask;
	};

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
		bool RTBindable;
		bool DTBindable;
	};

	struct GpuBufferInit
	{
		std::string name;
		const void* data;
		uint32_t dataSize;
		uint32_t stride;
		bool shaderBuffer;
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

	struct DepthTargetInit
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

	struct RenderState
	{
		BlendType blend;
		CullType cull;
		SamplerType sampler;
		bool wireFrame;
		bool zWrite;
		float opacity;
	};

	struct DrawInput
	{
		MeshData* meshData;
		uint32_t indexStart, indexCount;
		PrimitiveTopology topology;
		RenderState state;
		GpuProgram* program;
		GpuBuffer* const* VSBuffers; // will be bound in the same order as given starting from slot 0
		GpuBuffer* const* PSBuffers; // will be bound in the same order as given starting from slot 0
		uint32_t numVSBuffers;
		uint32_t numPSBuffers;
		Texture* const* textures; // will be bound in the same order as given starting from slot 0
		uint32_t numTextures;
	};

}

#endif
