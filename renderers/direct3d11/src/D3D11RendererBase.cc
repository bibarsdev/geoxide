
namespace Geoxide {

	D3D11RendererBase::D3D11RendererBase(HWND window)
	{
		HRESULT hr = 0;

		DXGI_SWAP_CHAIN_DESC desc = {};
		desc.Windowed = TRUE;
		desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		desc.BufferCount = 2;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.OutputWindow = window;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;
		desc.SampleDesc.Count = 1;

#ifdef _DEBUG
		hr = D3D11CreateDeviceAndSwapChain(0, D3D_DRIVER_TYPE_HARDWARE, 0, /*D3D11_CREATE_DEVICE_DEBUG*/0, 0, 0, D3D11_SDK_VERSION,
			&desc, sw.GetAddressOf(), &dev, 0, &ctx);
#else
		hr = D3D11CreateDeviceAndSwapChain(0, D3D_DRIVER_TYPE_HARDWARE, 0, 0, 0, 0, D3D11_SDK_VERSION, &desc,
			outSwapChain->sw_.GetAddressOf(), &dev, 0, &ctx);
#endif // _DEBUG

		if (FAILED(hr))
			GX_FUNC_THROW("Failed to create ID3D11Device. " + GetFormattedMessage(hr));

		ComPtr<ID3D11Texture2D> rtvTexture;
		ComPtr<ID3D11Texture2D> dsvTexture;
		D3D11_TEXTURE2D_DESC tex2Ddesc = {};

		// Create the main render target
		sw->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)rtvTexture.GetAddressOf());

		createRenderTargetView(rtvTexture.Get(), 0, rtv.GetAddressOf(), &vp);

		// Create the main depth stencil view
		rtvTexture->GetDesc(&tex2Ddesc);

		createTexture2D(tex2Ddesc.Width, tex2Ddesc.Height, 0, 0, tex2Ddesc.ArraySize, tex2Ddesc.MipLevels,
			DXGI_FORMAT_R32_TYPELESS, dsvTexture.GetAddressOf(), 0, false, true, false, false);

		createDepthStencilView(dsvTexture.Get(), 0, dsv.GetAddressOf());

		// Create common states
		createBlendState(D3D11_BLEND_ONE, D3D11_BLEND_ZERO, &comstates.opaque);
		createBlendState(D3D11_BLEND_ONE, D3D11_BLEND_INV_SRC_ALPHA, &comstates.alphaBlend);
		createBlendState(D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_ONE, &comstates.additive);
		createBlendState(D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA, &comstates.nonPremultiplied);

		createDepthStencilState(false, false, false, &comstates.depthNone);
		createDepthStencilState(true, true, false, &comstates.depthDefault);

		createRasterizerState(D3D11_CULL_NONE, D3D11_FILL_SOLID, &comstates.cullNone);
		createRasterizerState(D3D11_CULL_FRONT, D3D11_FILL_SOLID, &comstates.cullClockwise);
		createRasterizerState(D3D11_CULL_BACK, D3D11_FILL_SOLID, &comstates.cullCounterClockwise);
		createRasterizerState(D3D11_CULL_NONE, D3D11_FILL_WIREFRAME, &comstates.wireframe);

		createSamplerState(D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_WRAP, &comstates.pointWrap);
		createSamplerState(D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_CLAMP, &comstates.pointClamp);
		createSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, &comstates.linearWrap);
		createSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP, &comstates.linearClamp);
		createSamplerState(D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_WRAP, &comstates.anisotropicWrap);
		createSamplerState(D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_CLAMP, &comstates.anisotropicClamp);
	}

	D3D11RendererBase::~D3D11RendererBase()
	{
	}
}
