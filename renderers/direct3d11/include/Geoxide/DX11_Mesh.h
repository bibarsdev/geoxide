
#ifndef __GX_DX11_MESH_H__
#define __GX_DX11_MESH_H__

#include <geoxide/Mesh.h>

#include "DX11_GpuProgram.h"

namespace Geoxide {

	struct DX11_DrawOp
	{
		bool skinned;
		DX11_GpuPass pass;

		ComPtr<ID3D11Buffer> vertexBuffer;
		uint32_t vertexStride;

		ComPtr<ID3D11Buffer> indexBuffer;
		DXGI_FORMAT indexFormat;

		D3D11_PRIMITIVE_TOPOLOGY topology;

		Local<uint8_t> materialData;
		uint32_t materialDataSize;

		float4x4 worldViewProj;
		float4x4 world;

		const float4x4* skeleton;
		uint32_t skeletonSize;

		Local<ComPtr<ID3D11ShaderResourceView>> textures;
		uint32_t numTextures;

		uint32_t indexStart, indexCount;
	};

	struct DX11_Mesh : Mesh
	{
		GX_IMPLEMENT_INTERNAL();

		std::vector<DX11_DrawOp> ops;
	};

}

#endif
