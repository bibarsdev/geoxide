
#include "Geoxide/D3D11Renderer.h"

namespace Geoxide {

	D3D11Renderer::D3D11Renderer(HWND window) :
		D3D11RendererBase(window) {
	}

	D3D11Renderer::~D3D11Renderer()
	{
		Log::Info("Destroyed D3D11Renderer");
	}

	bool D3D11Renderer::hasInitialized() const
	{
		return mHasInitialized;
	}

	GpuProgram* D3D11Renderer::newGpuProgram(const GpuProgramInit& args)
	{
		try
		{
			return new D3D11GpuProgram(this, args);
		}
		catch (const std::exception& e)
		{
			Log::Error(e.what());
			Log::Error("Failed to create D3D11GpuProgram");
		}

		return 0;
	}

	Texture* D3D11Renderer::newTexture(const TextureInit& args)
	{
		try
		{
			switch (args.type)
			{
			case kTextureType2D:
				return new D3D11Texture2D(this, args);
			default:
				GX_THROW("Unknown texture type. " + std::to_string(args.type));
			}
		}
		catch (const std::exception& e)
		{
			Log::Error(e.what());
			Log::Error("Failed to create D3D11Texture2D");
		}

		return 0;
	}

	MeshData* D3D11Renderer::newMeshData(const MeshDataInit& args)
	{
		try
		{
			return new D3D11MeshData(this, args);
		}
		catch (const std::exception& e)
		{
			Log::Error(e.what());
			Log::Error("Failed to create D3D11MeshData");
		}

		return 0;
	}

	RenderTarget* D3D11Renderer::newRenderTarget(const RenderTargetInit& args)
	{
		try
		{
			return new D3D11RenderTarget(this, args);
		}
		catch (const std::exception& e)
		{
			Log::Error(e.what());
			Log::Error("Failed to create D3D11RenderTarget");
		}

		return 0;
	}

	void D3D11Renderer::makePerspectiveMatrix(const PerspectiveMatrixInput& args, Matrix& outMatrix)
	{
		(XMMATRIX&)outMatrix = XMMatrixPerspectiveFovLH(args.fov, args.aspect, args.nearZ, args.farZ);
	}

	void D3D11Renderer::makeOrthographicMatrix(const OrthographicMatrixInput& args, Matrix& outMatrix)
	{
		(XMMATRIX&)outMatrix = XMMatrixOrthographicLH(args.width, args.height, args.nearZ, args.farZ);
	}

	void D3D11Renderer::makeLookAtMatrix(const LookAtMatrixInput& args, Matrix& outMatrix)
	{
		(XMMATRIX&)outMatrix = XMMatrixLookAtLH((XMVECTOR&)args.position, (XMVECTOR&)args.center, (XMVECTOR&)args.up);
	}

	void D3D11Renderer::beginScene(VectorConst clearColor, RenderTarget* renderTarget)
	{
		D3D11RenderTarget* rt = (D3D11RenderTarget*)renderTarget;
		
		if (rt)
		{
			ctx->ClearRenderTargetView(rt->rtv.Get(), (float*)&clearColor);
			ctx->OMSetRenderTargets(1, rt->rtv.GetAddressOf(), 0);
			ctx->RSSetViewports(1, &rt->vp);
		}
		else
		{
			ctx->ClearRenderTargetView(rtv.Get(), (float*)&clearColor);
			ctx->OMSetRenderTargets(1, rtv.GetAddressOf(), 0);
			ctx->RSSetViewports(1, &vp);
		}
	}

	void D3D11Renderer::endScene()
	{
		sw->Present(1, 0);
	}

	void D3D11Renderer::draw(const DrawInput& args)
	{
		D3D11GpuProgram* program = (D3D11GpuProgram*)args.program;
		D3D11MeshData* meshData = (D3D11MeshData*)args.meshData;

		std::unique_ptr<ID3D11ShaderResourceView*> shaderResources(new ID3D11ShaderResourceView*[args.numTextures]);

		D3D11_MAPPED_SUBRESOURCE mappedResource = {};
		UINT bufferOffset = 0;

		ctx->IASetVertexBuffers(0, 1, meshData->vertexBuffer.GetAddressOf(), &meshData->vertexStride, &bufferOffset);
		ctx->IASetIndexBuffer(meshData->indexBuffer.Get(), meshData->indexFormat, 0);

		ctx->VSSetShader(program->vertexShader.Get(), 0, 0);
		ctx->PSSetShader(program->pixelShader.Get(), 0, 0);

		ctx->IASetInputLayout(program->inputLayout.Get());

		ctx->Map(vsConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		copy(mappedResource.pData, args.vsUniformData, args.vsUniformDataSize, 1);
		ctx->Unmap(vsConstantBuffer.Get(), 0);

		ctx->Map(psConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		copy(mappedResource.pData, args.psUniformData, args.psUniformDataSize, 1);
		ctx->Unmap(psConstantBuffer.Get(), 0);

		ctx->VSSetConstantBuffers(0, 1, vsConstantBuffer.GetAddressOf());
		ctx->PSSetConstantBuffers(0, 1, psConstantBuffer.GetAddressOf());

		for (uint32_t i = 0; i < args.numTextures; i++)
		{
			D3D11Texture* texture = (D3D11Texture*)args.textures[i];
			shaderResources.get()[i] = texture->srv.Get();
		}

		ctx->PSSetShaderResources(0, args.numTextures, shaderResources.get());

		switch (args.topology)
		{
		default:
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

		ctx->DrawIndexed(args.indexCount, args.indexStart, 0);
	}

	D3D11Renderer* NewRenderer(Window* wnd)
	{
		try
		{
			return new D3D11Renderer((HWND)wnd->getNativeHandle());
		}
		catch (const std::exception& e)
		{
			Log::Error(e.what());
			Log::Error("Failed to create D3D11Renderer");
		}

		return 0;
	}
}
