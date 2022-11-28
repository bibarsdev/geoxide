
#include "Geoxide/D3D11MeshData.h"

namespace Geoxide {

	D3D11MeshData::D3D11MeshData(D3D11RendererBase* gfx, const MeshDataInit& args)
	{
		gfx->createBuffer(args.vertexLength, args.vertexDataSize, args.vertexData,
			D3D11_BIND_VERTEX_BUFFER, vertexBuffer.GetAddressOf(),
			0, 0, 0);
		
		gfx->createBuffer(args.indexLength, args.indexDataSize, args.indexData,
			D3D11_BIND_INDEX_BUFFER, indexBuffer.GetAddressOf(),
			0, 0, 0);

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

		Log::Info("Created new D3D11MeshData \'" + args.name + "\'");
	}

}
