
#ifndef __GX_D3D11_RENDERER_DEVICE_H__
#define __GX_D3D11_RENDERER_DEVICE_H__

#include <Geoxide/RendererDevice.h>

#include <d3dcompiler.h>

namespace Geoxide {

	class DX11_RendererDevice : public RendererDevice
	{
	public:
		using FormatKey = FormatDesc;
		using ILFormatKey = std::pair<uint32_t, DataType>;

		struct FormatKeyHash
		{
			size_t operator()(const FormatKey& p) const
			{
				return Hash(p);
			}
		};
		
		struct FormatKeyEqual
		{
			bool operator()(const FormatKey& _Left, const FormatKey& _Right) const
			{
				return _Left.bitCount == _Right.bitCount 
					&& _Left.type == _Right.type
					&& _Left.RMask == _Right.RMask 
					&& _Left.GMask == _Right.GMask 
					&& _Left.BMask == _Right.BMask
					&& _Left.AMask == _Right.AMask;
			}
		};

		struct ILFormatKeyHash
		{
			size_t operator()(const ILFormatKey& p) const
			{
				return Hash(p);
			}
		};

		using FormatMap = std::unordered_map<FormatKey, DXGI_FORMAT, FormatKeyHash, FormatKeyEqual>;
		using ILFormatMap = std::unordered_map<ILFormatKey, DXGI_FORMAT, ILFormatKeyHash>;

		using SRVFormatMap = std::unordered_map<DXGI_FORMAT, DXGI_FORMAT>;
		using RTVFormatMap = std::unordered_map<DXGI_FORMAT, DXGI_FORMAT>;
		using DSVFormatMap = std::unordered_map<DXGI_FORMAT, DXGI_FORMAT>;

		// TODO: ComPtr
		struct CommonStates
		{
			ID3D11BlendState* opaque;
			ID3D11BlendState* alphaBlend;
			ID3D11BlendState* additive;
			ID3D11BlendState* nonPremultiplied;

			ID3D11DepthStencilState* depthNone;
			ID3D11DepthStencilState* depthDefault;

			ID3D11RasterizerState* cullNone;
			ID3D11RasterizerState* cullFront;
			ID3D11RasterizerState* cullBack;

			ID3D11RasterizerState* cullNoneWireFrame;
			ID3D11RasterizerState* cullFrontWireFrame;
			ID3D11RasterizerState* cullBackWireFrame;

			ID3D11SamplerState* pointWrap;
			ID3D11SamplerState* pointClamp;
			ID3D11SamplerState* linearWrap;
			ID3D11SamplerState* linearClamp;
			ID3D11SamplerState* anisotropicWrap;
			ID3D11SamplerState* anisotropicClamp;
		};

	public:
		DX11_RendererDevice(HWND window, ImGuiContext* imCtx, ImGuiMemAllocFunc alloc_func, ImGuiMemFreeFunc free_func, void* user_data);
		~DX11_RendererDevice();

		Ref<RendererContext> getContext() override { return mRendererCtx; }

		Ref<Shader> newShader(const ShaderDesc& desc, const void* byteCode, uint32_t byteCodeSize) override;
		Ref<GpuProgram> newProgram(const GpuProgramDesc& desc, const GpuPassDesc* passes) override;
		Ref<GpuBuffer> newBuffer(const GpuBufferDesc& desc, const void* data) override;
		Ref<Texture> newTexture(const TextureDesc& desc, const void* data) override;
		Ref<Mesh> newMesh(const MeshDesc& desc, const SubMeshDesc* subs) override;

		Ref<RendererContext> newContext() override;

		void makePerspectiveMatrix(const PerspectiveMatrixInput & args, float4x4& outMatrix) override;
		void makeOrthographicMatrix(const OrthographicMatrixInput & args, float4x4& outMatrix) override;

		void compileShader(const ShaderCompileInput& args, void** outByteCode, uint32_t* outByteCodeSize) override;

		void writeBuffer(const WriteBufferInput & args) override;

		void swapBuffers(uint32_t syncInterval) override;

		void startImGuiFrame() override;
		void drawImGui() override;

		void release() override;

	public:
		void createShaderResource(ID3D11Resource* resource, D3D11_SRV_DIMENSION type, DXGI_FORMAT format, ID3D11ShaderResourceView** outSRV,
			UINT mostDetailedMip_FirstElement, UINT mipLevels_NumElements, UINT firstArraySlice_First2DArrayFace, UINT arraySize_NumCubes);
		void createTexture2D(UINT width, UINT height, const void* data, UINT pitch, UINT arraySize, UINT mips, DXGI_FORMAT format, UINT flags, ID3D11Texture2D** outTexture,
			ID3D11ShaderResourceView** outSRV, bool renderTargetBindable, bool depthStencilBindable, bool enableRead, bool enableWrite);
		void createRenderTargetView(ID3D11Texture2D* texture, UINT arrayIndex, ID3D11RenderTargetView** outRenderTarget, D3D11_VIEWPORT* outViewport);
		void createDepthStencilView(ID3D11Texture2D* texture, UINT arrayIndex, ID3D11DepthStencilView** outDepthStencil);
		void compileShader(const std::string& name, const void* codeBuffer, SIZE_T codeSize, const char* entryPoint, const char* target,
			ID3DBlob** outBlob);
		void generateInputLayout(ID3D11ShaderReflection* ref, const void* byteCodeBuffer, SIZE_T byteCodeSize, ID3D11InputLayout** outInputLayout);
		void createBuffer(UINT stride, UINT size, const void* data, UINT bindFlags,
			ID3D11Buffer** outBuffer, ID3D11ShaderResourceView** outSRV,
			bool enableRead, bool enableWrite);

		void createBlendState(D3D11_BLEND srcBlend, D3D11_BLEND destBlend, ID3D11BlendState** pResult);
		void createDepthStencilState(bool enable, bool writeEnable, bool reverseZ, ID3D11DepthStencilState** pResult);
		void createRasterizerState(D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode, ID3D11RasterizerState** pResult);
		void createSamplerState(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode, ID3D11SamplerState** pResult);

		void createGBuffer(UINT width, UINT height, DXGI_FORMAT format, ID3D11ShaderResourceView** outSRV, ID3D11RenderTargetView** outRTV);

		void updateResource(ID3D11Resource* res, uint32_t dest, const void* data, uint32_t size);

	public:
		static UINT bitsPerPixel(DXGI_FORMAT fmt);
		static DXGI_FORMAT translateFormat(const FormatDesc& format);
		static DXGI_FORMAT getInputLayoutFormat(uint32_t length, DataType type);
		static DXGI_FORMAT getSRVFormat(DXGI_FORMAT format);
		static DXGI_FORMAT getRTVFormat(DXGI_FORMAT format);
		static DXGI_FORMAT getDSVFormat(DXGI_FORMAT format);

	public:
		Ref<RendererContext> mRendererCtx;

		ComPtr<ID3D11Device> mDev;
		ComPtr<ID3D11DeviceContext> mCtx;
		ComPtr<IDXGISwapChain> mSC;
		ComPtr<ID3D11RenderTargetView> mRTV;
		ComPtr<ID3D11DepthStencilView> mDSV;
		ComPtr<ID3D11ShaderResourceView> mDSVSRV;

		D3D11_VIEWPORT mVP;

		ComPtr<ID3D11Buffer> mPerSceneBuffer;
		ComPtr<ID3D11Buffer> mPerObjectBuffer;
		ComPtr<ID3D11Buffer> mMaterialBuffer;

		CommonStates mComStates;

	public:
		static const FormatMap kFormatDictionary;
		static const ILFormatMap kILFormatDictionary;
		static const SRVFormatMap kSRVFormatDictionary;
		static const RTVFormatMap kRTVFormatDictionary;
		static const DSVFormatMap kDSVFormatDictionary;
	};

	extern "C" GXAPI RendererDevice* NewRendererDevice(Window * wnd, ImGuiContext * imCtx, ImGuiMemAllocFunc alloc_func, ImGuiMemFreeFunc free_func, void* user_data);
}

#endif
