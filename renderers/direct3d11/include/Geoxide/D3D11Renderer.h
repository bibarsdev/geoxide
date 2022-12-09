
#ifndef __GX_D3D11_RENDERER_H__
#define __GX_D3D11_RENDERER_H__

#include "D3D11GpuProgram.h"
#include "D3D11Texture.h"
#include "D3D11GpuBuffer.h"
#include "D3D11MeshData.h"
#include "D3D11RenderTarget.h"
#include "D3D11DepthTarget.h"

namespace Geoxide {

	class D3D11Renderer : public D3D11RendererBase, public Renderer
	{
	public:
		D3D11Renderer(HWND window, ImGuiContext* imCtx, ImGuiMemAllocFunc alloc_func, ImGuiMemFreeFunc free_func, void* user_data);
		~D3D11Renderer();

		GpuProgram* newGpuProgram(const GpuProgramInit& args) override;
		Texture* newTexture(const TextureInit& args) override;
		GpuBuffer* newBuffer(const GpuBufferInit& args) override;
		MeshData* newMeshData(const MeshDataInit& args) override;
		RenderTarget* newRenderTarget(const RenderTargetInit& args) override;
		DepthTarget* newDepthTarget(const DepthTargetInit& args) override;
		GpuCommandList* newGpuCommandList() override;

		void makePerspectiveMatrix(const PerspectiveMatrixInput& args, Matrix& outMatrix) override;
		void makeOrthographicMatrix(const OrthographicMatrixInput& args, Matrix& outMatrix) override;
		void makeLookAtMatrix(const LookAtMatrixInput& args, Matrix& outMatrix) override;

		void beginScene(VectorConst clearColor, float depth, RenderTarget* renderTarget, DepthTarget* depthTarget) override;
		void endScene() override;

		void draw(const DrawInput& args) override;

		void startImGuiFrame() override;
		void drawImGui() override;
	};
	
	extern "C" GXAPI Renderer* NewRenderer(Window* wnd, ImGuiContext* imCtx, ImGuiMemAllocFunc alloc_func, ImGuiMemFreeFunc free_func, void* user_data);

}

#endif
