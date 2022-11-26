
#include "Geoxide/D3D11MeshData.h"

namespace Geoxide {

	D3D11MeshData::D3D11MeshData(D3D11RendererBase* gfx, const MeshDataInit& args)
	{
		if (!gfx->createBuffer(args.vertexLength, args.vertexDataSize, args.vertexData,
			D3D11_BIND_VERTEX_BUFFER, vertexBuffer.GetAddressOf(),
			0, 0, 0))
		{
			sLog.error("Failed to create veretx buffer");
			return;
		}

		if(!gfx->createBuffer(args.indexLength, args.indexDataSize, args.indexData,
			D3D11_BIND_INDEX_BUFFER, indexBuffer.GetAddressOf(),
			0, 0, 0))
		{
			sLog.error("Failed to create index buffer");
			return;
		}

		vertexStride = args.vertexLength;

		switch (args.indexLength)
		{
		case 2:
			indexFormat = DXGI_FORMAT_R16_UINT;
			break;
		case 4:
		default:
			indexFormat = DXGI_FORMAT_R32_UINT;
			break;
		}

		sLog.info("Created new mesh data");
	}

}
