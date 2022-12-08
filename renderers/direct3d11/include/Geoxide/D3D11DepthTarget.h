
#ifndef __GX_D3D11_DEPTH_TARGET_H__
#define __GX_D3D11_DEPTH_TARGET_H__

namespace Geoxide {

	class D3D11DepthTarget : public DepthTarget
	{
	public:
		D3D11DepthTarget(D3D11RendererBase* gfx, const DepthTargetInit& args);
		~D3D11DepthTarget() = default;

		ComPtr<ID3D11DepthStencilView> dsv;
	};

}

#endif
