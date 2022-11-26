
#ifndef __GX_RENDERER_H__
#define __GX_RENDERER_H__

#include "GpuProgram.h"
#include "Texture.h"
#include "MeshData.h"
#include "RenderTarget.h"

#include "RendererStruct.h"

#include "Window.h"

namespace Geoxide {

	// Avoid declaring too many virtual methods as much as possible.
	//

	class Renderer
	{
	public:
		virtual ~Renderer() = default;

		virtual bool hasInitialized() const = 0;

		virtual GpuProgram* newGpuProgram(const GpuProgramInit& args) = 0;
		virtual Texture* newTexture(const TextureInit& args) = 0;
		virtual MeshData* newMeshData(const MeshDataInit& args) = 0;
		virtual RenderTarget* newRenderTarget(const RenderTargetInit& args) = 0;

		virtual void makePerspectiveMatrix(const PerspectiveMatrixInput& args, Matrix& outMatrix) = 0;
		virtual void makeOrthographicMatrix(const OrthographicMatrixInput& args, Matrix& outMatrix) = 0;
		virtual void makeLookAtMatrix(const LookAtMatrixInput& args, Matrix& outMatrix) = 0;

		virtual void beginScene(VectorConst clearColor, RenderTarget* renderTarget) = 0;
		virtual void endScene() = 0;

		virtual void draw(const DrawInput& args) = 0;
	};

	using NewRendererProc = Renderer* (*)(Window*);
}

#endif
