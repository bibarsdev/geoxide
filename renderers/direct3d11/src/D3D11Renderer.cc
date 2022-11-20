
#include "Geoxide/D3D11Renderer.h"

namespace Geoxide {

	D3D11Renderer::D3D11Renderer(HWND window) :
		D3D11RendererBase(window) {}

	D3D11Renderer::~D3D11Renderer()
	{
	}

	bool D3D11Renderer::hasInitialized() const
	{
		return hasConstructed;
	}

	Texture* D3D11Renderer::newTexture(const TextureInit& args)
	{
		switch (args.type)
		{
		case kTextureType2D:
			return new D3D11Texture2D(this, args);
		default:
			sLog.error("Unknown texture type, type=" + std::to_string(args.type));
			return 0;
		}
	}

	Shader* D3D11Renderer::newShader(const ShaderInit& args)
	{
		switch (args.type)
		{
		case kShaderTypeVertex:
			return new D3D11VertexShader(this, args);
		case kShaderTypePixel:
			return new D3D11PixelShader(this, args);
		default:
			sLog.error("Unknown shader type, type=" + std::to_string(args.type));
			return 0;
		}
	}

	MeshData* D3D11Renderer::newMeshData(const MeshDataInit& args)
	{
		return new D3D11MeshData(this, args);
	}

	void D3D11Renderer::beginScene(ColorRGBA clearColor)
	{
		ctx->ClearRenderTargetView(rtv.Get(), (float*)&clearColor);
	}

	void D3D11Renderer::setCamera(Camera* camera)
	{
		const Transform& transform = camera->getTransform();
		ProjectionType projectionType = camera->getProjectionType();
		float width = camera->getViewportWidth(),
			height = camera->getViewportHeight(),
			fov = camera->getFOV(),
			nearZ = camera->getNearZ(),
			farZ = camera->getFarZ();

		viewMatrix =
			XMMatrixTranslationFromVector(transform.position) *
			XMMatrixRotationQuaternion(transform.quaternion) *
			XMMatrixScalingFromVector(transform.scaling);

		switch (projectionType)
		{
		default:
		case kProjectionTypeUnknown:
		case kProjectionTypePerspective:
			projectionMatrix = XMMatrixPerspectiveFovLH(fov, width / height, nearZ, farZ);
			break;
		case kProjectionTypeOrthographic:
			projectionMatrix = XMMatrixOrthographicLH(width, height, nearZ, farZ);
			break;
		}
	}

	void D3D11Renderer::drawMesh(Mesh* mesh, Transform& transform)
	{
		D3D11MeshData* meshData = (D3D11MeshData*)mesh->getMeshData();
		D3D11VertexShader* vs = (D3D11VertexShader*)mesh->getVertexShader();
		UINT WVPMatrixLoation = vs->WVPMatrixOffset, worldMatrixLocation = vs->worldMatrixOffset;
		PrimitiveTopology primitiveTopology = mesh->getPrimitiveTopology();
		auto& subMeshes = mesh->getSubMeshes();
		UINT bufferOffset = 0;

		ctx->IASetVertexBuffers(0, 1, meshData->vertexBuffer.GetAddressOf(), &meshData->vertexStride, &bufferOffset);
		ctx->IASetIndexBuffer(meshData->indexBuffer.Get(), meshData->indexFormat, 0);

		ctx->VSSetShader((ID3D11VertexShader*)vs->shader.Get(), 0, 0);
		ctx->IASetInputLayout(vs->layout.Get());

		switch (primitiveTopology)
		{
		default:
		case kPrimitiveTopologyUnknown:
		case kPrimitiveTopologyTriangleList:
			ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			break;
		case kPrimitiveTopologyPointList:
			ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
			break;
		case kPrimitiveTopologyLineList:
			ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
			break;
		}

		for (auto& i = subMeshes.begin(); i != subMeshes.end(); i++)
		{
			const SubMesh& subMesh = i->second;

			const Transform& subTransform = subMesh.getTransform();
			Transform worldTransform;

			Material* material = subMesh.getMaterial();
			D3D11PixelShader* ps = (D3D11PixelShader*)material->getPixelShader();
			D3D11_MAPPED_SUBRESOURCE mappedResource = {};

			void* unifromData = material->getUniformData();
			size_t unifromDataSize = material->getUniformDataSize();

			auto& textures = material->getTextures();
			size_t numTextures = textures.size();

			std::unique_ptr<ID3D11ShaderResourceView*> srvs(new ID3D11ShaderResourceView*[numTextures]);

			worldTransform.position = XMVectorAdd(subTransform.position, transform.position);
			worldTransform.quaternion = XMVectorMultiply(subTransform.quaternion, transform.quaternion);
			worldTransform.scaling = XMVectorMultiply(subTransform.scaling, transform.scaling);

			XMMATRIX worldMatrix =
				XMMatrixTranslationFromVector(worldTransform.position) *
				XMMatrixRotationQuaternion(worldTransform.quaternion) *
				XMMatrixScalingFromVector(worldTransform.scaling);
			XMMATRIX WVP = worldMatrix * viewMatrix * projectionMatrix;

			ctx->Map(constantBuffer1.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

			uint8_t* pData = (uint8_t*)mappedResource.pData;

			if (WVPMatrixLoation != -1)
				copy(&pData[WVPMatrixLoation], &WVP, sizeof(WVP) / 16, 16);

			if (worldMatrixLocation != -1)
				copy(&pData[worldMatrixLocation], &worldMatrix, sizeof(worldMatrix) / 16, 16);

			ctx->Unmap(constantBuffer1.Get(), 0);

			ctx->VSSetConstantBuffers(0, 1, constantBuffer1.GetAddressOf());

			for (size_t i = 0; i < numTextures; i++)
				srvs.get()[i] = ((D3D11Texture*)(textures[i]))->srv.Get();

			ctx->PSSetShaderResources(0, numTextures, srvs.get());

			ctx->Map(constantBuffer2.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

			copy(mappedResource.pData, unifromData, unifromDataSize, 16);

			ctx->Unmap(constantBuffer2.Get(), 0);

			ctx->PSSetConstantBuffers(0, 1, constantBuffer2.GetAddressOf());

			ctx->PSSetShader((ID3D11PixelShader*)ps->shader.Get(), 0, 0);

			ctx->DrawIndexed(subMesh.getIndexCount(), subMesh.getIndexStart(), 0);
		}
	}

	void D3D11Renderer::endScene()
	{
		sw->Present(0, 0);
	}

	D3D11Renderer* NewRenderer(Window* wnd)
	{
		return new D3D11Renderer((HWND)wnd->getNativeHandle());
	}
}
