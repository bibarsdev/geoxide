
#ifndef __GX_D3D11_MESH_DATA_H__
#define __GX_D3D11_MESH_DATA_H__

namespace Geoxide {

	class D3D11MeshData : public MeshData
	{
	public:
		D3D11MeshData(D3D11RendererBase* gfx, const MeshDataInit& args);
		~D3D11MeshData() = default;

		ComPtr<ID3D11Buffer> vertexBuffer;
		ComPtr<ID3D11Buffer> indexBuffer;
		uint32_t vertexStride;
		DXGI_FORMAT indexFormat;
		GX_DECLARE_LOG("D3D11MeshData");
	};

}

#endif
