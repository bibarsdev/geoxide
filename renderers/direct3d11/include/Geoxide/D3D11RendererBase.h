
#ifndef __GX_D3D11_RENDERER_BASE_H__
#define __GX_D3D11_RENDERER_BASE_H__

#include <Geoxide/Renderer.h>

namespace Geoxide {

	class D3D11RendererBase
	{
	public:
		using ILFormatKey = std::pair<uint32_t, DataType>;

		struct ILFormatKeyHash
		{
			size_t operator()(const ILFormatKey& p) const
			{
				auto hash1 = std::hash<uint32_t>{}(p.first);
				auto hash2 = std::hash<DataType>{}(p.second);

				return hash1 ^ hash2;
			}
		};

		using ILFormatMap = std::unordered_map<ILFormatKey, DXGI_FORMAT, ILFormatKeyHash>;

	public:
		D3D11RendererBase(HWND window);
		~D3D11RendererBase();

		bool createShaderResource(ID3D11Resource* resource, D3D11_SRV_DIMENSION type, DXGI_FORMAT format, ID3D11ShaderResourceView** outSRV,
			UINT mostDetailedMip_FirstElement, UINT mipLevels_NumElements, UINT firstArraySlice_First2DArrayFace, UINT arraySize_NumCubes);
		bool createTexture2D(UINT width, UINT height, const void* data, UINT arraySize, UINT mips, DXGI_FORMAT format, ID3D11Texture2D** outTexture,
			ID3D11ShaderResourceView** outSRV, bool renderTargetBindable, bool depthStencilBindable, bool enableRead, bool enableWrite);
		bool createRenderTargetView(ID3D11Texture2D* texture, UINT arrayIndex, ID3D11RenderTargetView** outRenderTarget, D3D11_VIEWPORT* outViewport);
		bool createDepthStencilView(ID3D11Texture2D* texture, UINT arrayIndex, ID3D11DepthStencilView** outDepthStencil);
		bool createShader(const void* codeBuffer, SIZE_T codeSize, const char* entryPoint, const char* type, bool isCompiled,
			ID3D11DeviceChild** outShader, ID3D11InputLayout** outInputLayout);
		bool createInputLayout(const D3D11_INPUT_ELEMENT_DESC* elements, UINT numElements, 
			const void* byteCodeBuffer, SIZE_T byteCodeSize, ID3D11InputLayout** outInputLayout);
		bool generateInputLayout(const void* byteCodeBuffer, SIZE_T byteCodeSize, ID3D11InputLayout** outInputLayout);
		bool createBuffer(UINT stride, UINT size, const void* data, D3D11_BIND_FLAG bindFlag,
			ID3D11Buffer** outBuffer, ID3D11ShaderResourceView** outSRV,
			bool enableRead, bool enableWrite);

		void createBlendState(D3D11_BLEND srcBlend, D3D11_BLEND destBlend, ID3D11BlendState** pResult);
		void createDepthStencilState(bool enable, bool writeEnable, bool reverseZ, ID3D11DepthStencilState** pResult);
		void createRasterizerState(D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode, ID3D11RasterizerState** pResult);
		void createSamplerState(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode, ID3D11SamplerState** pResult);

		static UINT bytesPerPixel(DXGI_FORMAT fmt);
		static DXGI_FORMAT translateFormat(ColorFormat format);
		static DXGI_FORMAT getInputLayoutFormat(uint32_t length, DataType type);
		static void copy(void* dest, const void* src, UINT numElements, UINT elementLength);

	public:
		bool hasConstructed;

		ComPtr<ID3D11Device> dev;
		ComPtr<ID3D11DeviceContext> ctx;
		ComPtr<IDXGISwapChain> sw;
		ComPtr<ID3D11RenderTargetView> rtv;
		ComPtr<ID3D11DepthStencilView> dsv;
		ComPtr<ID3D11Buffer> vsConstantBuffer;
		ComPtr<ID3D11Buffer> psConstantBuffer;

		D3D11_VIEWPORT vp;

		struct
		{
			ID3D11BlendState* opaque;
			ID3D11BlendState* alphaBlend;
			ID3D11BlendState* additive;
			ID3D11BlendState* nonPremultiplied;

			ID3D11DepthStencilState* depthNone;
			ID3D11DepthStencilState* depthDefault;

			ID3D11RasterizerState* cullNone;
			ID3D11RasterizerState* cullClockwise;
			ID3D11RasterizerState* cullCounterClockwise;
			ID3D11RasterizerState* wireframe;

			ID3D11SamplerState* pointWrap;
			ID3D11SamplerState* pointClamp;
			ID3D11SamplerState* linearWrap;
			ID3D11SamplerState* linearClamp;
			ID3D11SamplerState* anisotropicWrap;
			ID3D11SamplerState* anisotropicClamp;
		} comstates;

		static ILFormatMap ILFormatDictionary;

		GX_DECLARE_LOG("D3D11Renderer");
	};

}

#endif
