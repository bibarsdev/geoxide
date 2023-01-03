
#ifndef __GX_DX11_RENDERER_CONTEXT_BASE_H__
#define __GX_DX11_RENDERER_CONTEXT_BASE_H__

#include <Geoxide/RendererContext.h>

#include "DX11_Mesh.h"

#define MATERIAL_BUFFER_SIZE 1024

namespace Geoxide {

	class DX11_RendererDevice;

	class DX11_RendererContext : public RendererContext
	{
	public:
		struct __declspec(align(16)) PerSceneStruct
		{
			float3 eyePos;
		};

		struct __declspec(align(16)) PerObjectStruct
		{
			float4x4 worldViewProj;
			float4x4 world;
		};

		struct __declspec(align(16)) SkeletonStruct
		{
			float4x4 bones[128];
		};

		struct __declspec(align(16)) LightStruct
		{
			float3 position;
			float fallOff;
			float3 color;
		};

		struct __declspec(align(16)) PerPointLightStruct
		{
			LightStruct light;
			float4x4 viewProj;
		};

	public:
		DX11_RendererContext(DX11_RendererDevice* dev, bool deferred);
		~DX11_RendererContext();

		void begin(const SceneDesc& scene) override;
		void end() override;

		void submit(Mesh* mesh, cfloat4x4 world) override;
		void submitSkinned(Mesh* mesh, cfloat4x4 world, const float4x4* matrices, uint32_t numMatrices)override;
		void flush() override;

		void execute(GpuCommandList* list) override;
		GpuCommandList* finish() override;

		void release() override;

	public:
		void updateResource(ID3D11Resource* res, uint32_t dest, const void* data, uint32_t size);
		void draw();

		void doShadowPass();
		void doGPass();
		void doLightingPass();
		void doForwardPass();

	public:
		ComPtr<ID3D11DeviceContext> mCtx;
		ComPtr<ID3D11RenderTargetView> mRTV;
		ComPtr<ID3D11DepthStencilView> mDSV;

		ComPtr<ID3D11ShaderResourceView> mDSVSRV;

		D3D11_VIEWPORT mVP;

		ComPtr<ID3D11Buffer> mPerSceneBuffer;
		ComPtr<ID3D11Buffer> mPerObjectBuffer;
		ComPtr<ID3D11Buffer> mPerLightBuffer;
		ComPtr<ID3D11Buffer> mSkeletonBuffer;
		ComPtr<ID3D11Buffer> mMaterialBuffer;

		PointLightDesc* mPointLights;
		uint32_t mNumPointLights;

		float4x4 mViewProj;
		XMMATRIX mPointLightProj;

		ComPtr<ID3D11ShaderResourceView> mPositionSRV;
		ComPtr<ID3D11ShaderResourceView> mNormalSRV;
		ComPtr<ID3D11ShaderResourceView> mAlbedoSpecSRV;
		ComPtr<ID3D11ShaderResourceView> mPointLightShadowSRV;

		ComPtr<ID3D11RenderTargetView> mPositionRTV;
		ComPtr<ID3D11RenderTargetView> mNormalRTV;
		ComPtr<ID3D11RenderTargetView> mAlbedoSpecRTV;

		ComPtr<ID3D11DepthStencilView> mPointLightShadowDSV;

		ComPtr<ID3D11VertexShader> mPointLightPassVS;
		ComPtr<ID3D11PixelShader> mPointLightPassPS;
		
		ComPtr<ID3D11VertexShader> mShadowPassVS;
		ComPtr<ID3D11PixelShader> mShadowPassPS;

		ComPtr<ID3D11InputLayout> mPointLightPassInputLayout;
		ComPtr<ID3D11InputLayout> mShadowPassInputLayout;

		ComPtr<ID3D11Buffer> mQuadVertexBuffer;
		ComPtr<ID3D11Buffer> mQuadIndexBuffer;

		UINT mQuadVertexStride;

		ComPtr<ID3D11BlendState> mOpaque;
		ComPtr<ID3D11DepthStencilState> mDepthNone;
		ComPtr<ID3D11DepthStencilState> mDepthDefault;
		ComPtr<ID3D11RasterizerState> mCullBack;
		ComPtr<ID3D11SamplerState> mLinearWrap;

		std::vector<float4x4> mLights;

	public:
		std::vector<DX11_DrawOp*> mGPass;
		std::vector<DX11_DrawOp*> mForwardPass;
	};

}

#endif
