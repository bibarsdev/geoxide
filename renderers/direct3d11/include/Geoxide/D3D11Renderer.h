
#ifndef __GX_D3D11_RENDERER_H__
#define __GX_D3D11_RENDERER_H__

#include "D3D11Texture.h"
#include "D3D11Shader.h"
#include "D3D11MeshData.h"

namespace Geoxide {

	class D3D11Renderer : public D3D11RendererBase, public Renderer
	{
	public:
		D3D11Renderer(HWND window);
		~D3D11Renderer();

		bool hasInitialized() const override;
		Texture* newTexture(const TextureInit& args) override;
		Shader* newShader(const ShaderInit& args) override;
		MeshData* newMeshData(const MeshDataInit& args) override;
		void beginScene(ColorRGBA clearColor) override;
		void setCamera(Camera* camera) override;
		void drawMesh(Mesh* mesh, Transform& transform) override;
		void endScene() override;
	};
	
	extern "C" GXAPI D3D11Renderer* NewRenderer(Window* wnd);

}

#endif
