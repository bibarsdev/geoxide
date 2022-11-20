
#ifndef __GX_RENDERER_H__
#define __GX_RENDERER_H__

#include "Mesh.h"
#include "Camera.h"
#include "Window.h"

namespace Geoxide {

	// Avoid declaring too many virtual methods as much as possible.
	//

	class Renderer
	{
	public:
		virtual ~Renderer() = default;
		virtual bool hasInitialized() const = 0;
		virtual Texture* newTexture(const TextureInit& args) = 0;
		virtual Shader* newShader(const ShaderInit& args) = 0;
		virtual MeshData* newMeshData(const MeshDataInit& args) = 0;
		virtual void beginScene(ColorRGBA clearColor);
		virtual void setCamera(Camera* camera) = 0;
		virtual void drawMesh(Mesh* mesh, Transform& transform) = 0;
		virtual void endScene() = 0;
	};

	using NewRendererProc = Renderer* (*)(Window*);
}

#endif
