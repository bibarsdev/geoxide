
#include "Geoxide/D3D11Renderer.h"

#include <backends/imgui_impl_dx11.h>

namespace Geoxide {

	D3D11Renderer::D3D11Renderer(HWND window, ImGuiContext* imCtx, ImGuiMemAllocFunc alloc_func, ImGuiMemFreeFunc free_func, void* user_data) :
		D3D11RendererBase(window)
	{
		ImGui::SetCurrentContext(imCtx);
		ImGui::SetAllocatorFunctions(alloc_func, free_func, user_data);

		ImGui_ImplDX11_Init(dev.Get(), ctx.Get());
	}

	D3D11Renderer::~D3D11Renderer()
	{
		ImGui_ImplDX11_Shutdown();
		Log::Info("Destroyed D3D11Renderer");
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
			Log::Error("Failed to create D3D11GpuProgram \'" + args.name + "\'");
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
			Log::Error("Failed to create D3D11Texture2D \'" + args.name + "\'");
		}

		return 0;
	}

	GpuBuffer* D3D11Renderer::newBuffer(const GpuBufferInit& args)
	{
		try
		{
			return new D3D11GpuBuffer(this, args);
		}
		catch (const std::exception& e)
		{
			Log::Error(e.what());
			Log::Error("Failed to create D3D11GpuBuffer \'" + args.name + "\'");
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
			Log::Error("Failed to create D3D11MeshData \'" + args.name + "\'");
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
			Log::Error("Failed to create D3D11RenderTarget \'" + args.name + "\'");
		}

		return 0;
	}

	DepthTarget* D3D11Renderer::newDepthTarget(const DepthTargetInit& args)
	{
		try
		{
			return new D3D11DepthTarget(this, args);
		}
		catch (const std::exception& e)
		{
			Log::Error(e.what());
			Log::Error("Failed to create D3D11DepthTarget \'" + args.name + "\'");
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

	void D3D11Renderer::beginScene(VectorConst clearColor, float depth, RenderTarget* renderTarget, DepthTarget* depthTarget)
	{
		D3D11RenderTarget* rt = (D3D11RenderTarget*)renderTarget;
		D3D11DepthTarget* dt = (D3D11DepthTarget*)depthTarget;

		ID3D11DepthStencilView* depthView = 0;

		if (dt)
		{
			depthView = dt->dsv.Get();
		}
		else
		{
			depthView = dsv.Get();
		}

		ctx->ClearDepthStencilView(depthView, D3D11_CLEAR_DEPTH, depth, 0);

		if (rt)
		{
			ctx->ClearRenderTargetView(rt->rtv.Get(), (float*)&clearColor);
			ctx->OMSetRenderTargets(1, rt->rtv.GetAddressOf(), depthView);
			ctx->RSSetViewports(1, &rt->vp);
		}
		else
		{
			ctx->ClearRenderTargetView(rtv.Get(), (float*)&clearColor);
			ctx->OMSetRenderTargets(1, rtv.GetAddressOf(), depthView);
			ctx->RSSetViewports(1, &vp);
		}

	}

	void D3D11Renderer::endScene()
	{
		drawImGui();
		sw->Present(1, 0);
	}

	void D3D11Renderer::draw(const DrawInput& args)
	{
		D3D11GpuProgram* program = (D3D11GpuProgram*)args.program;
		D3D11MeshData* meshData = (D3D11MeshData*)args.meshData;

		std::unique_ptr<ID3D11ShaderResourceView*> shaderResources(new ID3D11ShaderResourceView*[args.numTextures]);
		std::unique_ptr<ID3D11Buffer*> VSBuffers(new ID3D11Buffer* [args.numVSBuffers]);
		std::unique_ptr<ID3D11Buffer*> PSBuffers(new ID3D11Buffer* [args.numPSBuffers]);

		UINT bufferOffset = 0;

		ctx->IASetVertexBuffers(0, 1, meshData->vertexBuffer.GetAddressOf(), &meshData->vertexStride, &bufferOffset);
		ctx->IASetIndexBuffer(meshData->indexBuffer.Get(), meshData->indexFormat, 0);

		ctx->VSSetShader(program->vertexShader.Get(), 0, 0);
		ctx->PSSetShader(program->pixelShader.Get(), 0, 0);

		ctx->IASetInputLayout(program->inputLayout.Get());

		for (uint32_t i = 0; i < args.numTextures; i++)
		{
			D3D11Texture* texture = (D3D11Texture*)args.textures[i];
			shaderResources.get()[i] = texture->srv.Get();
		}

		ctx->PSSetShaderResources(0, args.numTextures, shaderResources.get());

		for (uint32_t i = 0; i < args.numVSBuffers; i++)
		{
			D3D11GpuBuffer* buffer = (D3D11GpuBuffer*)args.VSBuffers[i];
			VSBuffers.get()[i] = buffer->buffer.Get();
		}

		for (uint32_t i = 0; i < args.numPSBuffers; i++)
		{
			D3D11GpuBuffer* buffer = (D3D11GpuBuffer*)args.PSBuffers[i];
			PSBuffers.get()[i] = buffer->buffer.Get();
		}

		ctx->VSSetConstantBuffers(0, args.numVSBuffers, VSBuffers.get());
		ctx->PSSetConstantBuffers(0, args.numPSBuffers, PSBuffers.get());

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

		ctx->OMSetBlendState(*((ID3D11BlendState**)(&comstates.opaque) + args.state.blend), 0, 0xFFFFFFFF);
		ctx->OMSetDepthStencilState(*((ID3D11DepthStencilState**)(&comstates.depthNone) + args.state.zWrite), 0);
		ctx->RSSetState(*((ID3D11RasterizerState**)(&comstates.cullNone) + args.state.cull));
		ctx->PSSetSamplers(0, 1, ((ID3D11SamplerState**)(&comstates.pointWrap) + args.state.cull));

		ctx->DrawIndexed(args.indexCount, args.indexStart, 0);
	}

	void D3D11Renderer::startImGuiFrame()
	{
		ImGui_ImplDX11_NewFrame();
	}

	void D3D11Renderer::drawImGui()
	{
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	Renderer* NewRenderer(Window* wnd, ImGuiContext* imCtx, ImGuiMemAllocFunc alloc_func, ImGuiMemFreeFunc free_func, void* user_data)
	{
		try
		{
			return new D3D11Renderer((HWND)wnd->getNativeHandle(), imCtx, alloc_func, free_func, user_data);
		}
		catch (const std::exception& e)
		{
			Log::Error(e.what());
			Log::Error("Failed to create D3D11Renderer");
		}

		return 0;
	}
}
