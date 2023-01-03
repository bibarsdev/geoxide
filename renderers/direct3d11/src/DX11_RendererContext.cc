
#include "Geoxide/DX11_RendererContext.h"

#include "Geoxide/DX11_RendererDevice.h"

#include "Geoxide/DX11_Mesh.h"

#include "CompiledShaders/PointLightPassVS.h"
#include "CompiledShaders/PointLightPassPS.h"

#include "CompiledShaders/ShadowPassVS.h"

namespace Geoxide {

	DX11_RendererContext::DX11_RendererContext(DX11_RendererDevice* dev, bool deferred) :
		mPointLights(0), mNumPointLights(0)
	{
		HRESULT hr = 0;
		
		if (deferred)
		{
			dev->mDev->CreateDeferredContext(0, mCtx.GetAddressOf());
		}
		else
		{
			mCtx = dev->mCtx;
		}

		mRTV = dev->mRTV;
		mDSV = dev->mDSV;
		mDSVSRV = dev->mDSVSRV;

		mOpaque = dev->mComStates.opaque;
		mDepthNone = dev->mComStates.depthNone;
		mDepthDefault = dev->mComStates.depthDefault;
		mCullBack = dev->mComStates.cullBack;
		mLinearWrap = dev->mComStates.linearWrap;

		mVP = dev->mVP;

		PerSceneStruct perScene = {};
		PerObjectStruct perObject = {};
		PerPointLightStruct perLight = {};
		SkeletonStruct skeleton = {};
		uint8_t material[MATERIAL_BUFFER_SIZE];

		dev->createBuffer(0, sizeof(perScene), &perScene, D3D11_BIND_CONSTANT_BUFFER, mPerSceneBuffer.GetAddressOf(), 0, false, true);
		dev->createBuffer(0, sizeof(perObject), &perObject, D3D11_BIND_CONSTANT_BUFFER, mPerObjectBuffer.GetAddressOf(), 0, false, true);
		dev->createBuffer(0, sizeof(perLight), &perLight, D3D11_BIND_CONSTANT_BUFFER, mPerLightBuffer.GetAddressOf(), 0, false, true);
		dev->createBuffer(0, sizeof(skeleton), &skeleton, D3D11_BIND_CONSTANT_BUFFER, mSkeletonBuffer.GetAddressOf(), 0, false, true);
		dev->createBuffer(0, sizeof(material), material, D3D11_BIND_CONSTANT_BUFFER, mMaterialBuffer.GetAddressOf(), 0, false, true);

		dev->createGBuffer(mVP.Width, mVP.Height, DXGI_FORMAT_R32G32B32A32_TYPELESS, mPositionSRV.GetAddressOf(), mPositionRTV.GetAddressOf());
		dev->createGBuffer(mVP.Width, mVP.Height, DXGI_FORMAT_R32G32B32A32_TYPELESS, mNormalSRV.GetAddressOf(), mNormalRTV.GetAddressOf());
		dev->createGBuffer(mVP.Width, mVP.Height, DXGI_FORMAT_R32G32B32A32_TYPELESS, mAlbedoSpecSRV.GetAddressOf(), mAlbedoSpecRTV.GetAddressOf());

		ComPtr<ID3D11Texture2D> tempTexture;

		dev->createTexture2D(mVP.Width, mVP.Height, 0, 0, 1, 1, DXGI_FORMAT_R32_TYPELESS, 0, tempTexture.GetAddressOf(), mPointLightShadowSRV.GetAddressOf(),
			false, true, false, false);

		dev->createDepthStencilView(tempTexture.Get(), 0, mPointLightShadowDSV.GetAddressOf());
		/*dev->createDepthStencilView(tempTexture.Get(), 1, mPointLightShadowDSVs[1].GetAddressOf());
		dev->createDepthStencilView(tempTexture.Get(), 2, mPointLightShadowDSVs[2].GetAddressOf());
		dev->createDepthStencilView(tempTexture.Get(), 3, mPointLightShadowDSVs[3].GetAddressOf());
		dev->createDepthStencilView(tempTexture.Get(), 4, mPointLightShadowDSVs[4].GetAddressOf());
		dev->createDepthStencilView(tempTexture.Get(), 5, mPointLightShadowDSVs[5].GetAddressOf());*/

		ComPtr<ID3D11ShaderReflection> ref;

		hr = D3DReflect(g_PointLightVertex, sizeof(g_PointLightVertex), __uuidof(ID3D11ShaderReflection), (void**)ref.GetAddressOf());

		if (FAILED(hr))
			GX_FUNC_THROW("Failed to reflect Light Pass Vertex Shader");

		dev->generateInputLayout(ref.Get(), g_PointLightVertex, sizeof(g_PointLightVertex), mPointLightPassInputLayout.GetAddressOf());

		hr = dev->mDev->CreateVertexShader(g_PointLightVertex, sizeof(g_PointLightVertex), 0, &mPointLightPassVS);

		if (FAILED(hr))
			GX_FUNC_THROW("Failed to create Light Pass ID3D11VertexShader");

		hr = dev->mDev->CreatePixelShader(g_PointLightPixel, sizeof(g_PointLightPixel), 0, &mPointLightPassPS);

		if (FAILED(hr))
			GX_FUNC_THROW("Failed to create Light Pass ID3D11PixelShader");

		ref.Reset();

		hr = D3DReflect(g_ShadowVertex, sizeof(g_ShadowVertex), __uuidof(ID3D11ShaderReflection), (void**)ref.GetAddressOf());

		if (FAILED(hr))
			GX_FUNC_THROW("Failed to reflect Shadow Pass Vertex Shader");

		dev->generateInputLayout(ref.Get(), g_ShadowVertex, sizeof(g_ShadowVertex), mShadowPassInputLayout.GetAddressOf());

		hr = dev->mDev->CreateVertexShader(g_ShadowVertex, sizeof(g_ShadowVertex), 0, &mShadowPassVS);

		if (FAILED(hr))
			GX_FUNC_THROW("Failed to create Shadow Pass ID3D11VertexShader");

		float quadVertexData[][6] =
		{
			{-1, -1, 0.5f, 1.0f, 0.0f, 1.0f},
			{-1, 1, 0.5f, 1.0f, 0.0f, 0.0f},
			{1, 1, 0.5f, 1.0f, 1.0f, 0.0f},
			{1, -1, 0.5f, 1.0f, 1.0f, 1.0f},
		};

		UINT quadIndexData[] =
		{
			0, 1, 2,
			0, 2, 3,
		};

		dev->createBuffer(sizeof(quadVertexData[0]), sizeof(quadVertexData), quadVertexData, D3D11_BIND_VERTEX_BUFFER, mQuadVertexBuffer.GetAddressOf(), 0, false, false);
		dev->createBuffer(sizeof(quadIndexData[0]), sizeof(quadIndexData), quadIndexData, D3D11_BIND_INDEX_BUFFER, mQuadIndexBuffer.GetAddressOf(), 0, false, false);

		mQuadVertexStride = sizeof(quadVertexData[0]);
	
		//mPointLightProj = XMMatrixOrthographicLH(mVP.Width, mVP.Height, 1.0f, 100.f);
		mPointLightProj = XMMatrixPerspectiveFovLH(M_PI_2, mVP.Width / mVP.Height, 1.0f, 100.f);
	}

	DX11_RendererContext::~DX11_RendererContext()
	{
	}

	void DX11_RendererContext::begin(const SceneDesc& scene)
	{
		mViewProj = scene.viewProj;

		mPointLights = scene.pointLights;
		mNumPointLights = scene.pointLightCount;

		for (size_t i = 0; i < mNumPointLights; i++)
		{
			mLights.push_back((float4x4&&)(
				DirectX::XMMatrixLookAtLH(XMVectorSet(5, 10, 0, 0), XMVectorSet(0, 0, 0, 0), XMVectorSet(0, 1, 0, 0)) * mPointLightProj
				));
			/*mLights.push_back((float4x4&&)(DirectX::XMMatrixLookAtLH((XMVECTOR&)scene.pointLights->position[i], (XMVECTOR&)scene.pointLights->position[i] + XMVectorSet(-1, 0, 0, 0), XMVectorSet(0, 1, 0, 0)) * mPointLightProj));

			mLights.push_back((float4x4&&)(DirectX::XMMatrixLookAtLH((XMVECTOR&)scene.pointLights->position[i], (XMVECTOR&)scene.pointLights->position[i] + XMVectorSet(0, 1, 0, 0), XMVectorSet(0, 0, -1, 0)) * mPointLightProj));
			mLights.push_back((float4x4&&)(DirectX::XMMatrixLookAtLH((XMVECTOR&)scene.pointLights->position[i], (XMVECTOR&)scene.pointLights->position[i] + XMVectorSet(0, -1, 0, 0), XMVectorSet(0, 0, -1, 0)) * mPointLightProj));

			mLights.push_back((float4x4&&)(DirectX::XMMatrixLookAtLH((XMVECTOR&)scene.pointLights->position[i], (XMVECTOR&)scene.pointLights->position[i] + XMVectorSet(0, 0, 1, 0), XMVectorSet(0, 1, 0, 0)) * mPointLightProj));
			mLights.push_back((float4x4&&)(DirectX::XMMatrixLookAtLH((XMVECTOR&)scene.pointLights->position[i], (XMVECTOR&)scene.pointLights->position[i] + XMVectorSet(0, 0, -1, 0), XMVectorSet(0, 1, 0, 0)) * mPointLightProj));*/
		}

		PerSceneStruct perScene;
		perScene.eyePos = scene.eyePos;

		updateResource(mPerSceneBuffer.Get(), 0, &perScene, sizeof(perScene));

		mCtx->ClearDepthStencilView(mDSV.Get(), D3D11_CLEAR_DEPTH, scene.depth, 0);
		mCtx->ClearRenderTargetView(mRTV.Get(), scene.clearColor.v);

		mCtx->RSSetViewports(1, &mVP);

		float4 GClear = float4(0, 0, 0, 0);

		mCtx->ClearRenderTargetView(mPositionRTV.Get(), GClear.v);
		mCtx->ClearRenderTargetView(mNormalRTV.Get(), GClear.v);
		mCtx->ClearRenderTargetView(mAlbedoSpecRTV.Get(), GClear.v);

		mCtx->ClearDepthStencilView(mPointLightShadowDSV.Get(), D3D11_CLEAR_DEPTH, 1, 0);

	}

	void DX11_RendererContext::end()
	{
		mPointLights = 0;
		mNumPointLights = 0;
		mLights.clear();
	}

	void DX11_RendererContext::submit(Mesh* mesh, cfloat4x4 world)
	{
		DX11_Mesh* d3dMesh = (DX11_Mesh*)mesh;

		uint32_t numOps = d3dMesh->ops.size();

		for (uint32_t i = 0; i < numOps; i++)
		{
			DX11_DrawOp& op = d3dMesh->ops[i];

			op.world = world;
			op.worldViewProj = (world.get_transposed() * mViewProj).transpose();

			switch (op.pass.mode)
			{
			case kRenderModeForward:
				mForwardPass.push_back(&op);
				break;

			case kRenderModeDeferred:
				mGPass.push_back(&op);
				break;

			default:
				break;
			}

		}

	}

	void DX11_RendererContext::submitSkinned(Mesh* mesh, cfloat4x4 world, const float4x4* matrices, uint32_t numMatrices)
	{
		DX11_Mesh* d3dMesh = (DX11_Mesh*)mesh;

		uint32_t numOps = d3dMesh->ops.size();

		assert(numMatrices <= 128);

		for (uint32_t i = 0; i < numOps; i++)
		{
			DX11_DrawOp& op = d3dMesh->ops[i];

			op.skeleton = matrices;
			op.skeletonSize = numMatrices * sizeof(op.skeleton[0]);

			op.world = world;
			op.worldViewProj = (world.get_transposed() * mViewProj).transpose();

			switch (op.pass.mode)
			{
			case kRenderModeForward:
				mForwardPass.push_back(&op);
				break;

			case kRenderModeDeferred:
				mGPass.push_back(&op);
				break;

			default:
				break;
			}

		}
	}

	void DX11_RendererContext::flush()
	{
		doShadowPass();
		doGPass();
		doLightingPass();
		doForwardPass();
		
		mGPass.clear();
		mForwardPass.clear();
	}

	void DX11_RendererContext::doShadowPass()
	{
		UINT bufferOffset = 0;

		mCtx->OMSetRenderTargets(0, 0, mPointLightShadowDSV.Get());

		mCtx->VSSetConstantBuffers(0, 1, mPerObjectBuffer.GetAddressOf());

		mCtx->PSSetShader(0, 0, 0);

		mCtx->OMSetDepthStencilState(mDepthDefault.Get(), 0);
		mCtx->RSSetState(mCullBack.Get());

		mCtx->VSSetShader(mShadowPassVS.Get(), 0, 0);
		mCtx->IASetInputLayout(mShadowPassInputLayout.Get());

		for (auto p_op : mGPass)
		{
			DX11_DrawOp& op = *p_op;

			mCtx->IASetVertexBuffers(0, 1, op.vertexBuffer.GetAddressOf(), &op.vertexStride, &bufferOffset);
			mCtx->IASetIndexBuffer(op.indexBuffer.Get(), op.indexFormat, 0);

			mCtx->IASetPrimitiveTopology(op.topology);

			for (auto& lightViewProj : mLights)
			{
				float4x4 m[] = { (op.world.get_transposed() * lightViewProj).transpose() };

				updateResource(mPerObjectBuffer.Get(), 0, &m, sizeof(m));

				mCtx->DrawIndexed(op.indexCount, op.indexStart, 0);
			}
		}
	}

	void DX11_RendererContext::doGPass()
	{
		UINT bufferOffset = 0;

		mCtx->OMSetRenderTargets(3, mPositionRTV.GetAddressOf(), mDSV.Get());
		mCtx->OMSetBlendState(mOpaque.Get(), 0, 0xFFFFFFFF);

		mCtx->VSSetConstantBuffers(0, 1, mPerSceneBuffer.GetAddressOf());
		mCtx->VSSetConstantBuffers(1, 1, mPerObjectBuffer.GetAddressOf());
		mCtx->VSSetConstantBuffers(2, 1, mSkeletonBuffer.GetAddressOf());

		mCtx->PSSetConstantBuffers(0, 1, mMaterialBuffer.GetAddressOf());

		for (auto p_op : mGPass)
		{
			DX11_DrawOp& op = *p_op;

			mCtx->IASetVertexBuffers(0, 1, op.vertexBuffer.GetAddressOf(), &op.vertexStride, &bufferOffset);
			mCtx->IASetIndexBuffer(op.indexBuffer.Get(), op.indexFormat, 0);

			mCtx->IASetPrimitiveTopology(op.topology);

			if (op.skinned)
			{
				mCtx->VSSetShader(op.pass.skinnedVertexShader.Get(), 0, 0);
				mCtx->IASetInputLayout(op.pass.skinnedInputLayout.Get());
				updateResource(mSkeletonBuffer.Get(), 0, op.skeleton, op.skeletonSize);
			}
			else
			{
				mCtx->VSSetShader(op.pass.vertexShader.Get(), 0, 0);
				mCtx->IASetInputLayout(op.pass.inputLayout.Get());
			}

			mCtx->PSSetShader(op.pass.pixelShader.Get(), 0, 0);

			mCtx->PSSetShaderResources(0, op.numTextures, op.textures->GetAddressOf());

			updateResource(mPerObjectBuffer.Get(), 0, &op.worldViewProj, sizeof(PerObjectStruct));
			updateResource(mMaterialBuffer.Get(), 0, op.materialData.get(), op.materialDataSize);

			mCtx->OMSetDepthStencilState(op.pass.depthState.Get(), 0);
			mCtx->RSSetState(op.pass.rasterState.Get());
			mCtx->PSSetSamplers(0, 1, op.pass.samplerState.GetAddressOf());

			mCtx->DrawIndexed(op.indexCount, op.indexStart, 0);
		}

	}

	void DX11_RendererContext::doLightingPass()
	{
		UINT bufferOffset = 0;

		mCtx->OMSetRenderTargets(1, mRTV.GetAddressOf(), 0);

		mCtx->PSSetShaderResources(0, 4, mPositionSRV.GetAddressOf());
		mCtx->PSSetShaderResources(4, 1, mDSVSRV.GetAddressOf());

		mCtx->PSSetConstantBuffers(0, 1, mPerSceneBuffer.GetAddressOf());
		mCtx->PSSetConstantBuffers(1, 1, mPerLightBuffer.GetAddressOf());

		mCtx->IASetVertexBuffers(0, 1, mQuadVertexBuffer.GetAddressOf(), &mQuadVertexStride, &bufferOffset);
		mCtx->IASetIndexBuffer(mQuadIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		mCtx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		mCtx->IASetInputLayout(mPointLightPassInputLayout.Get());

		mCtx->VSSetShader(mPointLightPassVS.Get(), 0, 0);
		mCtx->PSSetShader(mPointLightPassPS.Get(), 0, 0);

		mCtx->OMSetDepthStencilState(mDepthNone.Get(), 0);
		mCtx->RSSetState(mCullBack.Get());
		mCtx->PSSetSamplers(0, 1, mLinearWrap.GetAddressOf());

		for (uint32_t i = 0; i < mNumPointLights; i++)
		{
			PerPointLightStruct point = {};
			point.light.color = float3(1, 1, 1);
			point.light.position = float3(-1, -1, 0).normalize();
			point.viewProj = mLights[i].get_transposed();

			updateResource(mPerLightBuffer.Get(), 0, &point, sizeof(point));

			mCtx->DrawIndexed(6, 0, 0);
		}

	}

	void DX11_RendererContext::doForwardPass()
	{
		UINT bufferOffset = 0;

		mCtx->VSSetConstantBuffers(0, 1, mPerSceneBuffer.GetAddressOf());
		mCtx->VSSetConstantBuffers(1, 1, mPerObjectBuffer.GetAddressOf());
		mCtx->VSSetConstantBuffers(2, 1, mSkeletonBuffer.GetAddressOf());

		mCtx->PSSetConstantBuffers(0, 1, mMaterialBuffer.GetAddressOf());

		for (auto p_op : mForwardPass)
		{
			DX11_DrawOp& op = *p_op;

			mCtx->IASetVertexBuffers(0, 1, op.vertexBuffer.GetAddressOf(), &op.vertexStride, &bufferOffset);
			mCtx->IASetIndexBuffer(op.indexBuffer.Get(), op.indexFormat, 0);

			mCtx->IASetPrimitiveTopology(op.topology);

			if (op.skinned)
			{
				mCtx->VSSetShader(op.pass.skinnedVertexShader.Get(), 0, 0);
				mCtx->IASetInputLayout(op.pass.skinnedInputLayout.Get());
				updateResource(mSkeletonBuffer.Get(), 0, op.skeleton, op.skeletonSize);
			}
			else
			{
				mCtx->VSSetShader(op.pass.vertexShader.Get(), 0, 0);
				mCtx->IASetInputLayout(op.pass.inputLayout.Get());
			}

			mCtx->PSSetShader(op.pass.pixelShader.Get(), 0, 0);

			mCtx->IASetInputLayout(op.pass.inputLayout.Get());

			mCtx->PSSetShaderResources(0, op.numTextures, op.textures->GetAddressOf());

			updateResource(mPerObjectBuffer.Get(), 0, &op.worldViewProj, 128);
			updateResource(mMaterialBuffer.Get(), 0, op.materialData.get(), op.materialDataSize);

			mCtx->OMSetDepthStencilState(op.pass.depthState.Get(), 0);
			mCtx->RSSetState(op.pass.rasterState.Get());
			mCtx->PSSetSamplers(0, 1, op.pass.samplerState.GetAddressOf());
			mCtx->OMSetBlendState(op.pass.blendState.Get(), 0, op.pass.sampleMask);

			mCtx->DrawIndexed(op.indexCount, op.indexStart, 0);
		}

	}

	void DX11_RendererContext::updateResource(ID3D11Resource* res, uint32_t dest, const void* data, uint32_t size)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource = {};

		mCtx->Map(res, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		memcpy((uint8_t*)mappedResource.pData + dest, data, size);

		mCtx->Unmap(res, 0);
	}

	void DX11_RendererContext::execute(GpuCommandList* list)
	{
		// TODO
	}

	GpuCommandList* DX11_RendererContext::finish()
	{
		// TODO
		return nullptr;
	}

	void DX11_RendererContext::release()
	{
		if (--mRefCount < 1)
		{
			delete this;
		}
	}
}
