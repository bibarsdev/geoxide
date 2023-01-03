
#include "Geoxide/DX11_RendererDevice.h"

#include "Geoxide/DX11_Shader.h"
#include "Geoxide/DX11_GpuProgram.h"
#include "Geoxide/DX11_GpuBuffer.h"
#include "Geoxide/DX11_Texture.h"
#include "Geoxide/DX11_Mesh.h"

#include "Geoxide/DX11_RendererContext.h"

#include <backends/imgui_impl_dx11.h>

namespace Geoxide {

	DX11_RendererDevice::DX11_RendererDevice(HWND window, ImGuiContext* imCtx, ImGuiMemAllocFunc alloc_func, ImGuiMemFreeFunc free_func, void* user_data)
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
			&desc, mSC.GetAddressOf(), &mDev, 0, &mCtx);
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
		mSC->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)rtvTexture.GetAddressOf());

		createRenderTargetView(rtvTexture.Get(), 0, mRTV.GetAddressOf(), &mVP);

		// Create the main depth stencil view
		rtvTexture->GetDesc(&tex2Ddesc);

		createTexture2D(tex2Ddesc.Width, tex2Ddesc.Height, 0, 0, tex2Ddesc.ArraySize, tex2Ddesc.MipLevels,
			DXGI_FORMAT_R32_TYPELESS, 0, dsvTexture.GetAddressOf(), mDSVSRV.GetAddressOf(), false, true, false, false);

		createDepthStencilView(dsvTexture.Get(), 0, mDSV.GetAddressOf());

		// Create common states
		createBlendState(D3D11_BLEND_ONE, D3D11_BLEND_ZERO, &mComStates.opaque);
		createBlendState(D3D11_BLEND_ONE, D3D11_BLEND_INV_SRC_ALPHA, &mComStates.alphaBlend);
		createBlendState(D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_ONE, &mComStates.additive);
		createBlendState(D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA, &mComStates.nonPremultiplied);

		createDepthStencilState(false, false, false, &mComStates.depthNone);
		createDepthStencilState(true, true, false, &mComStates.depthDefault);

		createRasterizerState(D3D11_CULL_NONE, D3D11_FILL_SOLID, &mComStates.cullNone);
		createRasterizerState(D3D11_CULL_FRONT, D3D11_FILL_SOLID, &mComStates.cullFront);
		createRasterizerState(D3D11_CULL_BACK, D3D11_FILL_SOLID, &mComStates.cullBack);

		createRasterizerState(D3D11_CULL_NONE, D3D11_FILL_WIREFRAME, &mComStates.cullNoneWireFrame);
		createRasterizerState(D3D11_CULL_FRONT, D3D11_FILL_WIREFRAME, &mComStates.cullFrontWireFrame);
		createRasterizerState(D3D11_CULL_BACK, D3D11_FILL_WIREFRAME, &mComStates.cullBackWireFrame);

		createSamplerState(D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_WRAP, &mComStates.pointWrap);
		createSamplerState(D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_CLAMP, &mComStates.pointClamp);
		createSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, &mComStates.linearWrap);
		createSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP, &mComStates.linearClamp);
		createSamplerState(D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_WRAP, &mComStates.anisotropicWrap);
		createSamplerState(D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_CLAMP, &mComStates.anisotropicClamp);

		ImGui::SetCurrentContext(imCtx);
		ImGui::SetAllocatorFunctions(alloc_func, free_func, user_data);

		ImGui_ImplDX11_Init(mDev.Get(), mCtx.Get());

		mRendererCtx = new DX11_RendererContext(this, false);
	}

	DX11_RendererDevice::~DX11_RendererDevice()
	{
		ImGui_ImplDX11_Shutdown();
		Log::Info("Destroyed DX11_RendererDevice");
	}

	Ref<Shader> DX11_RendererDevice::newShader(const ShaderDesc& desc, const void* byteCode, uint32_t byteCodeSize)
	{
		try
		{
			ShaderReflectionDesc reflection;

			D3D11_SHADER_DESC shaderDesc = {};
			HRESULT hr = 0;

			ComPtr<ID3D11ShaderReflection> ref;

			hr = D3DReflect(byteCode, byteCodeSize, __uuidof(ID3D11ShaderReflection), (void**)ref.GetAddressOf());

			if (FAILED(hr))
			{
				GX_FUNC_THROW("Failed to reflect shader. " + GetFormattedMessage(hr));
			}

			ref->GetDesc(&shaderDesc);

			for (uint32_t i = 0; i < shaderDesc.ConstantBuffers; i++)
			{
				auto shaderBuffer = ref->GetConstantBufferByIndex(i);

				D3D11_SHADER_BUFFER_DESC bufferDesc = {};
				shaderBuffer->GetDesc(&bufferDesc);

				ShaderBufferDesc& buffer = reflection.buffers[bufferDesc.Name] = ShaderBufferDesc();

				buffer.size = bufferDesc.Size;

				for (uint32_t i = 0; i < bufferDesc.Variables; i++)
				{
					auto shaderVar = shaderBuffer->GetVariableByIndex(i);

					D3D11_SHADER_VARIABLE_DESC varDesc = {};
					shaderVar->GetDesc(&varDesc);

					ShaderVariableDesc& var = buffer.vars[varDesc.Name] = ShaderVariableDesc();

					var.byteOffset = varDesc.StartOffset;
					var.size = varDesc.Size;
				}
			}

			for (uint32_t i = 0; i < shaderDesc.BoundResources; i++)
			{
				D3D11_SHADER_INPUT_BIND_DESC resourceDesc = {};
				ref->GetResourceBindingDesc(i, &resourceDesc);

				switch (resourceDesc.Type)
				{
				case D3D_SIT_CBUFFER:
					reflection.buffers.at(resourceDesc.Name).bindOffset = resourceDesc.BindPoint;
					break;
				case D3D_SIT_TEXTURE:
				{
					ShaderResourceDesc& resource = reflection.resources[resourceDesc.Name] = ShaderResourceDesc();

					resource.bindOffset = resourceDesc.BindPoint;

					switch (resourceDesc.Dimension)
					{
					case D3D_SRV_DIMENSION_TEXTURE2D:
						resource.type = kShaderResourceTexture2D;
						break;
					}

					break;
				}
				}
			}

			switch (desc.type)
			{
			case kShaderVertex:
			{
				DX11_VertexShader* obj = new DX11_VertexShader();

				hr = mDev->CreateVertexShader(byteCode, byteCodeSize, 0, obj->vertexShader.GetAddressOf());

				if (FAILED(hr))
				{
					delete obj;
					GX_FUNC_THROW("Failed to create ID3D11VertexShader. " + GetFormattedMessage(hr));
				}

				generateInputLayout(ref.Get(), byteCode, byteCodeSize, obj->inputLayout.GetAddressOf());

				obj->desc = desc;
				obj->reflection = reflection;
				return obj;
			}

			case kShaderFragment:
			{
				DX11_PixelShader* obj = new DX11_PixelShader();

				hr = mDev->CreatePixelShader(byteCode, byteCodeSize, 0, obj->pixelShader.GetAddressOf());

				if (FAILED(hr))
				{
					delete obj;
					GX_FUNC_THROW("Failed to create ID3D11PixelShader. " + GetFormattedMessage(hr));
				}

				obj->desc = desc;
				obj->reflection = reflection;
				return obj;
			}

			default:
				GX_FUNC_THROW("Unknown shader type. " + std::to_string(desc.type));
				break;
			}
		}
		catch (const std::exception& e)
		{
			Log::Error(e.what());
			Log::Error("Failed to create DX11_Shader");
		}

		return 0;
	}

	Ref<GpuProgram> DX11_RendererDevice::newProgram(const GpuProgramDesc& desc, const GpuPassDesc* passes)
	{
		try
		{
			DX11_GpuProgram* program = new DX11_GpuProgram();

			program->dxPasses = Local<DX11_GpuPass>(new DX11_GpuPass[desc.numPasses]);

			for (uint32_t i = 0; i < desc.numPasses; i++)
			{
				const GpuPassDesc& pass = passes[i];
				DX11_GpuPass& dxPass = program->dxPasses.get()[i];

				DX11_VertexShader* vShader = (DX11_VertexShader*)pass.vertexShader;
				DX11_VertexShader* svShader = (DX11_VertexShader*)pass.skinnedVertexShader;
				DX11_PixelShader* pShader = (DX11_PixelShader*)pass.fragmentShader;

				dxPass.mode = pass.state.mode;

				dxPass.vertexShader = vShader->vertexShader.Get();
				dxPass.inputLayout = vShader->inputLayout.Get();

				if (svShader)
				{
					dxPass.skinnedVertexShader = svShader->vertexShader.Get();
					dxPass.skinnedInputLayout = svShader->inputLayout.Get();
				}

				dxPass.pixelShader = pShader->pixelShader.Get();

				if (pass.state.wireFrame)
				{
					dxPass.rasterState = *((ID3D11RasterizerState**)(&mComStates.cullNoneWireFrame) + pass.state.cull);
				}
				else
				{
					dxPass.rasterState = *((ID3D11RasterizerState**)(&mComStates.cullNone) + pass.state.cull);
				}

				dxPass.samplerState = *((ID3D11SamplerState**)(&mComStates.pointWrap) + pass.state.sampler);

				dxPass.blendState = *((ID3D11BlendState**)(&mComStates.opaque) + pass.state.blend);
				dxPass.depthState = *((ID3D11DepthStencilState**)(&mComStates.depthNone) + pass.state.zWrite);

				dxPass.sampleMask = 0xFFFFFFFF;
			}

			program->passes = Local<GpuPassDesc>(new GpuPassDesc[desc.numPasses]);
			std::copy(passes, passes + desc.numPasses, program->passes.get());

			program->desc = desc;
			return program;
		}
		catch (const std::exception& e)
		{
			Log::Error(e.what());
			Log::Error("Failed to create DX11_GpuProgram");
		}

		return 0;
	}

	Ref<GpuBuffer> DX11_RendererDevice::newBuffer(const GpuBufferDesc& desc, const void* data)
	{
		try
		{
			switch (desc.type)
			{
			case kGpuBufferShader:
			{
				DX11_GpuBufferShader* buffer = new DX11_GpuBufferShader();

				createBuffer(
					desc.stride, desc.size, data,
					D3D11_BIND_CONSTANT_BUFFER,
					buffer->buffer.GetAddressOf(), 0,
					true, true);

				buffer->desc = desc;
				return buffer;
			}

			case kGpuBufferVertex:
			{
				DX11_GpuBufferVertex* buffer = new DX11_GpuBufferVertex();

				createBuffer(
					desc.stride, desc.size, data,
					D3D11_BIND_VERTEX_BUFFER,
					buffer->buffer.GetAddressOf(), 0,
					false, false);

				buffer->vertexLength = desc.stride;

				buffer->desc = desc;
				return buffer;
			}

			case kGpuBufferIndex:
			{
				DX11_GpuBufferIndex* buffer = new DX11_GpuBufferIndex();

				createBuffer(
					desc.stride, desc.size, data,
					D3D11_BIND_INDEX_BUFFER,
					buffer->buffer.GetAddressOf(), 0,
					false, false);

				switch (desc.stride)
				{
				case 2:
					buffer->format = DXGI_FORMAT_R16_UINT;
					break;
				case 4:
					buffer->format = DXGI_FORMAT_R32_UINT;
					break;
				default:
					GX_FUNC_THROW("Index length unsupported. " + std::to_string(desc.stride));
					break;
				}

				buffer->desc = desc;
				return buffer;
			}

			default:
				GX_FUNC_THROW("Unknown buffer type. " + std::to_string(desc.type));
				break;
			}
		}
		catch (const std::exception& e)
		{
			Log::Error(e.what());
			Log::Error("Failed to create DX11_GpuBuffer");
		}

		return 0;
	}

	Ref<Texture> DX11_RendererDevice::newTexture(const TextureDesc& desc, const void* data)
	{
		try
		{
			DX11_Texture* texture = new DX11_Texture();
			UINT flags = 0;

			switch (desc.dimension)
			{
			case kTextureDimension2D:
			{
				flags |= desc.cube ? D3D11_RESOURCE_MISC_TEXTURECUBE : 0;

				createTexture2D(
					desc.width, desc.height, data, desc.pitch,
					desc.arraySizeOrDepth, desc.mipLevels, translateFormat(desc.format), flags,
					(ID3D11Texture2D**)texture->texture.GetAddressOf(), texture->SRV.GetAddressOf(),
					false, false, false, false);

				break;
			}

			default:
				GX_FUNC_THROW("Unknown texture dimension. " + std::to_string(desc.dimension));
				break;
			}

			texture->desc = desc;
			return texture;
		}
		catch (const std::exception& e)
		{
			Log::Error(e.what());
			Log::Error("Failed to create DX11_Texture");
		}

		return 0;
	}

	Ref<Mesh> DX11_RendererDevice::newMesh(const MeshDesc& desc, const SubMeshDesc* subs)
	{
		try
		{
			if (desc.subCount < 1)
			{
				GX_FUNC_THROW("Mesh too small");
			}

			DX11_Mesh* mesh = new DX11_Mesh();

			DX11_GpuBufferVertex* vBuffer = (DX11_GpuBufferVertex*)desc.vBuffer;
			DX11_GpuBufferIndex* iBuffer = (DX11_GpuBufferIndex*)desc.iBuffer;

			ID3D11Buffer* vertexBuffer = vBuffer->buffer.Get();
			ID3D11Buffer* indexBuffer = iBuffer->buffer.Get();

			UINT vertexStride = vBuffer->vertexLength;
			DXGI_FORMAT indexFormat = iBuffer->format;

			D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;

			switch (desc.topology)
			{
			case kPrimitiveTopologyTriangleList:
				topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
				break;
			case kPrimitiveTopologyPointList:
				topology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
				break;
			case kPrimitiveTopologyLineList:
				topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
				break;
			default:
				GX_FUNC_THROW("Unknown primitive topology. " + std::to_string(desc.topology));
				break;
			}

			for (uint32_t i = 0; i < desc.subCount; i++)
			{
				const SubMeshDesc& sub = subs[i];

				Material* mat = sub.material;

				DX11_GpuProgram* program = mat->program.getAs<DX11_GpuProgram>();

				for (uint32_t i = 0; i < program->desc.numPasses; i++)
				{
					mesh->ops.push_back(DX11_DrawOp());
					DX11_DrawOp& op = mesh->ops.back();

					//op.skinned = desc.isSkinned;

					op.pass = program->dxPasses.get()[i];

					op.vertexBuffer = vertexBuffer;
					op.vertexStride = vertexStride;

					op.indexBuffer = indexBuffer;
					op.indexFormat = indexFormat;

					op.indexCount = sub.indexCount;
					op.indexStart = sub.indexStart;

					op.topology = topology;

					op.materialDataSize = mat->dataSize;
					op.materialData = Local<uint8_t>(new uint8_t[op.materialDataSize]);

					memcpy(op.materialData.get(), mat->data.get(), op.materialDataSize);

					op.numTextures = mat->textures.size();
					op.textures = Local<ComPtr<ID3D11ShaderResourceView>>(new ComPtr<ID3D11ShaderResourceView>[op.numTextures]());

					for (uint32_t i = 0; i < op.numTextures; i++)
					{
						DX11_Texture* texture = mat->textures[i].getAs<DX11_Texture>();
						op.textures.get()[i] = texture->SRV.Get();
					}
				}

			}

			return mesh;
		}
		catch (const std::exception& e)
		{
			Log::Error(e.what());
			Log::Error("Failed to create DX11_Mesh");
		}

		return 0;
	}

	void DX11_RendererDevice::makePerspectiveMatrix(const PerspectiveMatrixInput& args, float4x4& outMatrix)
	{
		(XMMATRIX&)outMatrix = XMMatrixPerspectiveFovLH(args.fov, args.aspect, args.nearZ, args.farZ);
	}

	void DX11_RendererDevice::makeOrthographicMatrix(const OrthographicMatrixInput& args, float4x4& outMatrix)
	{
		(XMMATRIX&)outMatrix = XMMatrixOrthographicLH(args.width, args.height, args.nearZ, args.farZ);
	}

	Ref<RendererContext> DX11_RendererDevice::newContext()
	{
		return new DX11_RendererContext(this, true);
	}

	void DX11_RendererDevice::compileShader(const ShaderCompileInput& args, void** outByteCode, uint32_t* outByteCodeSize)
	{
		try
		{
			ComPtr<ID3DBlob> blob;

			switch (args.type)
			{
			case kShaderVertex:
				compileShader(args.name.c_str(), args.code, args.codeSize, args.entry, "vs_5_0", blob.GetAddressOf());
				break;
			case kShaderFragment:
				compileShader(args.name.c_str(), args.code, args.codeSize, args.entry, "ps_5_0", blob.GetAddressOf());
				break;
			default:
				break;
			}

			*outByteCodeSize = blob->GetBufferSize();
			*outByteCode = new BYTE[*outByteCodeSize];

			memcpy(*outByteCode, blob->GetBufferPointer(), *outByteCodeSize);
		}
		catch (const std::exception& e)
		{
			Log::Error(e.what());
			Log::Error("Failed to compile \'" + args.name + "\'");
		}

	}

	void DX11_RendererDevice::writeBuffer(const WriteBufferInput& args)
	{
	}

	void DX11_RendererDevice::swapBuffers(uint32_t sync)
	{
		mSC->Present(sync, 0);
	}

	void DX11_RendererDevice::startImGuiFrame()
	{
		ImGui_ImplDX11_NewFrame();
	}

	void DX11_RendererDevice::drawImGui()
	{
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
	
	void DX11_RendererDevice::createShaderResource(ID3D11Resource* resource, D3D11_SRV_DIMENSION type, DXGI_FORMAT format, ID3D11ShaderResourceView** outSRV,
		UINT mostDetailedMip_FirstElement, UINT mipLevels_NumElements, UINT firstArraySlice_First2DArrayFace, UINT arraySize_NumCubes)
	{
		HRESULT hr;

		CD3D11_SHADER_RESOURCE_VIEW_DESC srDesc(
			type, getSRVFormat(format),
			mostDetailedMip_FirstElement, mipLevels_NumElements,
			firstArraySlice_First2DArrayFace, arraySize_NumCubes);

		hr = mDev->CreateShaderResourceView(resource, &srDesc, outSRV);

		if (FAILED(hr))
			GX_FUNC_THROW("Failed to create ID3D11ShaderResourceView. " + GetFormattedMessage(hr));
	}

	void DX11_RendererDevice::createTexture2D(UINT width, UINT height, const void* data, UINT pitch, UINT arraySize, UINT mips, DXGI_FORMAT format, UINT flags,
		ID3D11Texture2D** outTexture, ID3D11ShaderResourceView** outSRV, bool renderTargetBindable, bool depthStencilBindable, bool enableRead, bool enableWrite)
	{
		HRESULT hr;

		D3D11_TEXTURE2D_DESC texDesc = {};
		texDesc.Width = width;
		texDesc.Height = height;
		texDesc.ArraySize = arraySize;
		texDesc.MipLevels = mips;
		texDesc.Format = format;
		texDesc.SampleDesc.Count = 1;
		texDesc.Usage =
			(enableRead || enableWrite ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT);
		texDesc.CPUAccessFlags =
			(enableRead ? D3D11_CPU_ACCESS_READ : 0) |
			(enableWrite ? D3D11_CPU_ACCESS_WRITE : 0);
		texDesc.BindFlags =
			(outSRV ? D3D11_BIND_SHADER_RESOURCE : 0) |
			(renderTargetBindable ? D3D11_BIND_RENDER_TARGET : 0) |
			(depthStencilBindable ? D3D11_BIND_DEPTH_STENCIL : 0);
		texDesc.MiscFlags = flags;

		if (data)
		{
			std::vector<D3D11_SUBRESOURCE_DATA> texDatas(texDesc.ArraySize);

			for (size_t i = 0; i < texDesc.ArraySize; i++)
			{
				D3D11_SUBRESOURCE_DATA& texData = texDatas[i];
				texData.pSysMem = (uint8_t*)data + (bitsPerPixel(texDesc.Format) * texDesc.Width + 7) / 8 * texDesc.Height * i;
				texData.SysMemPitch = pitch;
			}

			hr = mDev->CreateTexture2D(&texDesc, texDatas.data(), outTexture);
		}
		else
			hr = mDev->CreateTexture2D(&texDesc, 0, outTexture);

		if (FAILED(hr))
			GX_FUNC_THROW("Failed to create ID3D11Texture2D. " + GetFormattedMessage(hr));

		if (outSRV)
		{
			D3D11_SRV_DIMENSION dim = D3D11_SRV_DIMENSION_TEXTURE2D;

			if (flags & D3D11_RESOURCE_MISC_TEXTURECUBE)
			{
				dim = D3D11_SRV_DIMENSION_TEXTURECUBE;
			}
			else if (arraySize > 1)
			{
				dim = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
			}

			createShaderResource(*outTexture,
				dim,
				format, outSRV,
				0, mips, 0, arraySize);
		}
	}

	void DX11_RendererDevice::createRenderTargetView(ID3D11Texture2D* texture, UINT arrayIndex, ID3D11RenderTargetView** outRenderTarget, D3D11_VIEWPORT* outViewport)
	{
		HRESULT hr;
		D3D11_RESOURCE_DIMENSION rd = D3D11_RESOURCE_DIMENSION_UNKNOWN;
		D3D11_TEXTURE2D_DESC desc = {};

		texture->GetType(&rd);
		if (rd != D3D11_RESOURCE_DIMENSION_TEXTURE2D)
			GX_FUNC_THROW("Unsupported resource dimension. Only D3D11_RESOURCE_DIMENSION_TEXTURE2D is supported");

		texture->GetDesc(&desc);

		CD3D11_RENDER_TARGET_VIEW_DESC rtDesc(
			(desc.ArraySize > 1 ? D3D11_RTV_DIMENSION_TEXTURE2DARRAY : D3D11_RTV_DIMENSION_TEXTURE2D),
			getRTVFormat(desc.Format),
			0, arrayIndex, desc.ArraySize);

		hr = mDev->CreateRenderTargetView(texture, &rtDesc, outRenderTarget);

		if (FAILED(hr))
			GX_FUNC_THROW("Failed to create ID3D11RenderTargetView. " + GetFormattedMessage(hr));

		if (outViewport)
			*outViewport = CD3D11_VIEWPORT((ID3D11Texture2D*)texture, *outRenderTarget);
	}

	void DX11_RendererDevice::createDepthStencilView(ID3D11Texture2D* texture, UINT arrayIndex, ID3D11DepthStencilView** outDepthStencil)
	{
		HRESULT hr;
		D3D11_RESOURCE_DIMENSION rd = D3D11_RESOURCE_DIMENSION_UNKNOWN;
		D3D11_TEXTURE2D_DESC desc = {};

		texture->GetType(&rd);
		if (rd != D3D11_RESOURCE_DIMENSION_TEXTURE2D)
			GX_FUNC_THROW("Unsupported resource dimension. Only D3D11_RESOURCE_DIMENSION_TEXTURE2D is supported");

		texture->GetDesc(&desc);

		CD3D11_DEPTH_STENCIL_VIEW_DESC dsDesc(
			(desc.ArraySize > 1 ? D3D11_DSV_DIMENSION_TEXTURE2DARRAY : D3D11_DSV_DIMENSION_TEXTURE2D),
			getDSVFormat(desc.Format),
			0, arrayIndex, 1);

		hr = mDev->CreateDepthStencilView(texture, &dsDesc, outDepthStencil);

		if (FAILED(hr))
			GX_FUNC_THROW("Failed to create ID3D11DepthStencilView. " + GetFormattedMessage(hr));
	}

	void DX11_RendererDevice::compileShader(const std::string& name, const void* codeBuffer, SIZE_T codeSize, const char* entryPoint, const char* target,
		ID3DBlob** outBlob)
	{
		HRESULT hr = 0;

		ComPtr<ID3DBlob> errorMsg;

		hr = D3DCompile(
			codeBuffer, codeSize, name.c_str(),
			0, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entryPoint, target,
			D3DCOMPILE_OPTIMIZATION_LEVEL3, 0,
			outBlob, &errorMsg);

		if (FAILED(hr))
			if (errorMsg)
			{
				const char* pMsg = (const char*)errorMsg->GetBufferPointer();
				std::string sErrorMsg(pMsg, pMsg + errorMsg->GetBufferSize());

				GX_FUNC_THROW("Failed to compile shader \'" + name + "\' \'" + target + "\'. " + sErrorMsg);
			}
			else
				GX_FUNC_THROW("Failed to compile shader \'" + name + "\' \'" + target + "\'. " + GetFormattedMessage(hr));
	}

	// copied from https://gist.github.com/Cody-Duncan/d85740563ceea99f6619
	void DX11_RendererDevice::generateInputLayout(ID3D11ShaderReflection* ref, const void* byteCodeBuffer, SIZE_T byteCodeSize, ID3D11InputLayout** outInputLayout)
	{
		D3D11_SHADER_DESC shaderDesc = {};
		ref->GetDesc(&shaderDesc);

		unsigned int byteOffset = 0;
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
		for (unsigned int i = 0; i < shaderDesc.InputParameters; ++i)
		{
			D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
			ref->GetInputParameterDesc(i, &paramDesc);

			D3D11_INPUT_ELEMENT_DESC elementDesc;
			elementDesc.SemanticName = paramDesc.SemanticName;
			elementDesc.SemanticIndex = paramDesc.SemanticIndex;
			elementDesc.InputSlot = 0;
			elementDesc.AlignedByteOffset = byteOffset;
			elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			elementDesc.InstanceDataStepRate = 0;

			if (paramDesc.Mask == 1)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
				byteOffset += 4;
			}
			else if (paramDesc.Mask <= 3)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
				byteOffset += 8;
			}
			else if (paramDesc.Mask <= 7)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
				byteOffset += 12;
			}
			else if (paramDesc.Mask <= 15)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				byteOffset += 16;
			}

			inputLayoutDesc.push_back(elementDesc);
		}

		HRESULT hr = mDev->CreateInputLayout(inputLayoutDesc.data(), inputLayoutDesc.size(), byteCodeBuffer, byteCodeSize, outInputLayout);

		if (FAILED(hr))
			GX_FUNC_THROW("Failed to create ID3D11InputLayout. " + GetFormattedMessage(hr));
	}

	void DX11_RendererDevice::createBuffer(UINT stride, UINT size, const void* data, UINT bindFlags,
		ID3D11Buffer** outBuffer, ID3D11ShaderResourceView** outSRV,
		bool enableRead, bool enableWrite)
	{
		HRESULT hr;

		D3D11_BUFFER_DESC bufDesc = {};
		bufDesc.ByteWidth = size;
		bufDesc.StructureByteStride = stride;
		bufDesc.Usage =
			(enableWrite ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT);
		bufDesc.CPUAccessFlags =
			(enableRead ? D3D11_CPU_ACCESS_READ : 0) |
			(enableWrite ? D3D11_CPU_ACCESS_WRITE : 0);
		bufDesc.BindFlags = bindFlags |
			(outSRV ? D3D11_BIND_SHADER_RESOURCE : 0);

		D3D11_SUBRESOURCE_DATA bufData = {};
		bufData.pSysMem = data;

		hr = mDev->CreateBuffer(&bufDesc, &bufData, outBuffer);

		if (FAILED(hr))
			GX_FUNC_THROW("Failed to create ID3D11Buffer. " + GetFormattedMessage(hr));

		if (outSRV)
			createShaderResource(*outBuffer,
				D3D11_SRV_DIMENSION_BUFFER,
				DXGI_FORMAT_UNKNOWN, outSRV,
				0, 1, 0, 0);
	}

	void DX11_RendererDevice::createBlendState(D3D11_BLEND srcBlend, D3D11_BLEND destBlend, ID3D11BlendState** pResult)
	{
		D3D11_BLEND_DESC desc = {};

		desc.RenderTarget[0].BlendEnable = (srcBlend != D3D11_BLEND_ONE) ||
			(destBlend != D3D11_BLEND_ZERO);

		desc.RenderTarget[0].SrcBlend = desc.RenderTarget[0].SrcBlendAlpha = srcBlend;
		desc.RenderTarget[0].DestBlend = desc.RenderTarget[0].DestBlendAlpha = destBlend;
		desc.RenderTarget[0].BlendOp = desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		mDev->CreateBlendState(&desc, pResult);
	}

	void DX11_RendererDevice::createDepthStencilState(bool enable, bool writeEnable, bool reverseZ, ID3D11DepthStencilState** pResult)
	{
		D3D11_DEPTH_STENCIL_DESC desc = {};

		desc.DepthEnable = enable ? TRUE : FALSE;
		desc.DepthWriteMask = writeEnable ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
		desc.DepthFunc = reverseZ ? D3D11_COMPARISON_GREATER_EQUAL : D3D11_COMPARISON_LESS_EQUAL;

		desc.StencilEnable = FALSE;
		desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

		desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

		desc.BackFace = desc.FrontFace;

		mDev->CreateDepthStencilState(&desc, pResult);
	}

	void DX11_RendererDevice::createRasterizerState(D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode, ID3D11RasterizerState** pResult)
	{
		D3D11_RASTERIZER_DESC desc = {};

		desc.CullMode = cullMode;
		desc.FillMode = fillMode;
		desc.DepthClipEnable = TRUE;
		desc.MultisampleEnable = TRUE;

		mDev->CreateRasterizerState(&desc, pResult);
	}

	void DX11_RendererDevice::createSamplerState(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode, ID3D11SamplerState** pResult)
	{
		D3D11_SAMPLER_DESC desc = {};

		desc.Filter = filter;

		desc.AddressU = addressMode;
		desc.AddressV = addressMode;
		desc.AddressW = addressMode;

		desc.MaxAnisotropy = D3D11_MAX_MAXANISOTROPY;

		desc.MaxLOD = FLT_MAX;
		desc.ComparisonFunc = D3D11_COMPARISON_NEVER;

		mDev->CreateSamplerState(&desc, pResult);
	}

	void DX11_RendererDevice::createGBuffer(UINT width, UINT height, DXGI_FORMAT format, ID3D11ShaderResourceView** outSRV, ID3D11RenderTargetView** outRTV)
	{
		ComPtr<ID3D11Texture2D> tempTexture;

		createTexture2D(width, height, 0, 0, 1, 1, format, 0, &tempTexture, outSRV, true, false, false, false);
		createRenderTargetView(tempTexture.Get(), 0, outRTV, 0);
	}

	void DX11_RendererDevice::updateResource(ID3D11Resource* res, uint32_t dest, const void* data, uint32_t size)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource = {};

		mCtx->Map(res, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		memcpy((uint8_t*)mappedResource.pData + dest, data, size);

		mCtx->Unmap(res, 0);
	}

	const DX11_RendererDevice::FormatMap DX11_RendererDevice::kFormatDictionary =
	{
		{{kDataUnknown, 4 * 8, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000}, DXGI_FORMAT_R8G8B8A8_TYPELESS},
		{{kDataByte, 4 * 8, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000}, DXGI_FORMAT_R8G8B8A8_UNORM},

		{{kDataUnknown, 4 * 8, 0x000000FF, 0x00000000, 0x00000000, 0x00000000}, DXGI_FORMAT_R32_TYPELESS},
		{{kDataFloat, 4 * 8, 0x000000FF, 0x00000000, 0x00000000, 0x00000000}, DXGI_FORMAT_R32_FLOAT},
		{{kDataInt, 4 * 8, 0x000000FF, 0x00000000, 0x00000000, 0x00000000}, DXGI_FORMAT_R32_SINT},
		{{kDataUInt, 4 * 8, 0x000000FF, 0x00000000, 0x00000000, 0x00000000}, DXGI_FORMAT_R32_UINT},

		{{kDataBC3, 0, 0, 0, 0, 0}, DXGI_FORMAT_BC3_UNORM},
		{{kDataBC4, 0, 0, 0, 0, 0}, DXGI_FORMAT_BC4_UNORM},
		{{kDataBC5, 0, 0, 0, 0, 0}, DXGI_FORMAT_BC5_UNORM},
	};

	const DX11_RendererDevice::ILFormatMap DX11_RendererDevice::kILFormatDictionary =
	{
		{{1 * sizeof(float), kDataFloat}, DXGI_FORMAT_R32_FLOAT},
		{{1 * sizeof(int), kDataInt}, DXGI_FORMAT_R32_SINT},
		{{1 * sizeof(UINT), kDataUInt}, DXGI_FORMAT_R32_UINT},

		{{2 * sizeof(float), kDataFloat}, DXGI_FORMAT_R32G32_FLOAT},
		{{2 * sizeof(int), kDataInt}, DXGI_FORMAT_R32G32_SINT},
		{{2 * sizeof(UINT), kDataUInt}, DXGI_FORMAT_R32G32_UINT},

		{{3 * sizeof(float), kDataFloat}, DXGI_FORMAT_R32G32B32_FLOAT},
		{{3 * sizeof(int), kDataInt}, DXGI_FORMAT_R32G32B32_SINT},
		{{3 * sizeof(UINT), kDataUInt}, DXGI_FORMAT_R32G32B32_UINT},

		{{4 * sizeof(float), kDataFloat}, DXGI_FORMAT_R32G32B32A32_FLOAT},
		{{4 * sizeof(int), kDataInt}, DXGI_FORMAT_R32G32B32A32_SINT},
		{{4 * sizeof(UINT), kDataUInt}, DXGI_FORMAT_R32G32B32A32_UINT},
	};

	const DX11_RendererDevice::SRVFormatMap DX11_RendererDevice::kSRVFormatDictionary =
	{
		{DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT},
		{DXGI_FORMAT_R32G32B32_TYPELESS, DXGI_FORMAT_R32G32B32_FLOAT},
		{DXGI_FORMAT_R32G32_TYPELESS, DXGI_FORMAT_R32G32_FLOAT},
		{DXGI_FORMAT_R32_TYPELESS, DXGI_FORMAT_R32_FLOAT},

		{DXGI_FORMAT_R8G8B8A8_TYPELESS, DXGI_FORMAT_R8G8B8A8_UNORM},
		{DXGI_FORMAT_R8G8_TYPELESS, DXGI_FORMAT_R8G8_UNORM},
		{DXGI_FORMAT_R8_TYPELESS, DXGI_FORMAT_R8G8_UNORM},
	};

	const DX11_RendererDevice::RTVFormatMap DX11_RendererDevice::kRTVFormatDictionary =
	{
		{DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT},
		{DXGI_FORMAT_R32G32B32_TYPELESS, DXGI_FORMAT_R32G32B32_FLOAT},
		{DXGI_FORMAT_R32G32_TYPELESS, DXGI_FORMAT_R32G32_FLOAT},
		{DXGI_FORMAT_R32_TYPELESS, DXGI_FORMAT_R32_FLOAT},

		{DXGI_FORMAT_R8G8B8A8_TYPELESS, DXGI_FORMAT_R8G8B8A8_UNORM},
		{DXGI_FORMAT_R8G8_TYPELESS, DXGI_FORMAT_R8G8_UNORM},
		{DXGI_FORMAT_R8_TYPELESS, DXGI_FORMAT_R8G8_UNORM},
	};

	const DX11_RendererDevice::DSVFormatMap DX11_RendererDevice::kDSVFormatDictionary =
	{
		{DXGI_FORMAT_R24G8_TYPELESS, DXGI_FORMAT_D24_UNORM_S8_UINT},
		{DXGI_FORMAT_R32_TYPELESS, DXGI_FORMAT_D32_FLOAT},
		{DXGI_FORMAT_R16_TYPELESS, DXGI_FORMAT_D16_UNORM},
	};

	UINT DX11_RendererDevice::bitsPerPixel(DXGI_FORMAT fmt)
	{
		switch (fmt)
		{
		case DXGI_FORMAT_R32G32B32A32_TYPELESS:
		case DXGI_FORMAT_R32G32B32A32_FLOAT:
			return 128;

		case DXGI_FORMAT_R32G32B32_TYPELESS:
		case DXGI_FORMAT_R32G32B32_FLOAT:
			return 96;

		case DXGI_FORMAT_R32G32_TYPELESS:
		case DXGI_FORMAT_R32G32_FLOAT:
			return 64;

		case DXGI_FORMAT_R32_FLOAT:
		case DXGI_FORMAT_R32_TYPELESS:
		case DXGI_FORMAT_R8G8B8A8_UNORM:
			return 32;

		case DXGI_FORMAT_BC4_UNORM:
			return 4;

		case DXGI_FORMAT_BC3_UNORM:
		case DXGI_FORMAT_BC5_UNORM:
			return 8;

		default:
			GX_FUNC_THROW("Unknown DXGI format");
		}
	}

	DXGI_FORMAT DX11_RendererDevice::translateFormat(const FormatDesc& format)
	{
		auto& iter = kFormatDictionary.find(format);

		if (iter == kFormatDictionary.end())
			GX_FUNC_THROW(
				"Unknown color format. "
				"type=" + std::to_string(format.type) +
				", bitCount=" + std::to_string(format.bitCount) +
				", RMask=" + std::to_string(format.RMask) +
				", GMask=" + std::to_string(format.GMask) +
				", BMask=" + std::to_string(format.BMask) +
				", AMask=" + std::to_string(format.AMask));

		return iter->second;
	}

	DXGI_FORMAT DX11_RendererDevice::getInputLayoutFormat(uint32_t length, DataType type)
	{
		auto& iter = kILFormatDictionary.find({ length, type });

		if (iter == kILFormatDictionary.end())
			GX_FUNC_THROW("Unknown input layout element format. length=" + std::to_string(length) + ", type=" + std::to_string(type));

		return iter->second;
	}

	DXGI_FORMAT DX11_RendererDevice::getSRVFormat(DXGI_FORMAT format)
	{
		auto& iter = kSRVFormatDictionary.find(format);

		if (iter == kSRVFormatDictionary.end())
			return format;

		return iter->second;
	}

	DXGI_FORMAT DX11_RendererDevice::getRTVFormat(DXGI_FORMAT format)
	{
		auto& iter = kRTVFormatDictionary.find(format);

		if (iter == kRTVFormatDictionary.end())
			return format;

		return iter->second;
	}

	DXGI_FORMAT DX11_RendererDevice::getDSVFormat(DXGI_FORMAT format)
	{
		auto& iter = kDSVFormatDictionary.find(format);

		if (iter == kDSVFormatDictionary.end())
			return format;

		return iter->second;
	}

	void DX11_RendererDevice::release()
	{
		if (--mRefCount < 1)
		{
			delete this;
		}
	}

	RendererDevice* NewRendererDevice(Window* wnd, ImGuiContext* imCtx, ImGuiMemAllocFunc alloc_func, ImGuiMemFreeFunc free_func, void* user_data)
	{
		try
		{
			return new DX11_RendererDevice((HWND)wnd->getNativeHandle(), imCtx, alloc_func, free_func, user_data);
		}
		catch (const std::exception& e)
		{
			Log::Error(e.what());
			Log::Error("Failed to create DX11_RendererDevice");
		}

		return 0;
	}
}
