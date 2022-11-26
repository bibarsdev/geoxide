
#include <d3dcompiler.h>

namespace Geoxide {

	bool D3D11RendererBase::createShaderResource(ID3D11Resource* resource, D3D11_SRV_DIMENSION type, DXGI_FORMAT format, ID3D11ShaderResourceView** outSRV,
		UINT mostDetailedMip_FirstElement, UINT mipLevels_NumElements, UINT firstArraySlice_First2DArrayFace, UINT arraySize_NumCubes)
	{
		HRESULT hr;

		CD3D11_SHADER_RESOURCE_VIEW_DESC srDesc(
			type, format,
			mostDetailedMip_FirstElement, mipLevels_NumElements,
			firstArraySlice_First2DArrayFace, arraySize_NumCubes);

		hr = dev->CreateShaderResourceView(resource, &srDesc, outSRV);

		if (FAILED(hr))
			return false;

		return true;
	}

	bool D3D11RendererBase::createTexture2D(UINT width, UINT height, const void* data, UINT arraySize, UINT mips, DXGI_FORMAT format,
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

		if (data)
		{
			D3D11_SUBRESOURCE_DATA texData;
			texData.pSysMem = data;
			texData.SysMemPitch = width * bytesPerPixel(format);
			texData.SysMemSlicePitch = texData.SysMemPitch * height;

			hr = dev->CreateTexture2D(&texDesc, &texData, outTexture);
		}
		else
			hr = dev->CreateTexture2D(&texDesc, 0, outTexture);

		if (FAILED(hr))
			return false;

		if (outSRV)
			return createShaderResource(*outTexture,
				(arraySize > 1 ? D3D11_SRV_DIMENSION_TEXTURE2DARRAY : D3D11_SRV_DIMENSION_TEXTURE2D),
				format, outSRV,
				0, mips, 0, arraySize);

		return true;
	}

	bool D3D11RendererBase::createRenderTargetView(ID3D11Texture2D* texture, UINT arrayIndex, ID3D11RenderTargetView** outRenderTarget, D3D11_VIEWPORT* outViewport)
	{
		HRESULT hr;
		D3D11_RESOURCE_DIMENSION rd = D3D11_RESOURCE_DIMENSION_UNKNOWN;
		D3D11_TEXTURE2D_DESC desc = {};

		texture->GetType(&rd);
		if (rd != D3D11_RESOURCE_DIMENSION_TEXTURE2D)
			return false;

		texture->GetDesc(&desc);

		CD3D11_RENDER_TARGET_VIEW_DESC rtDesc(
			(desc.ArraySize > 1 ? D3D11_RTV_DIMENSION_TEXTURE2DARRAY : D3D11_RTV_DIMENSION_TEXTURE2D),
			desc.Format, 0, arrayIndex, desc.ArraySize);

		hr = dev->CreateRenderTargetView(texture, &rtDesc, outRenderTarget);

		if (FAILED(hr))
			return false;

		if (outViewport)
			*outViewport = CD3D11_VIEWPORT((ID3D11Texture2D*)texture, *outRenderTarget);

		return true;
	}

	bool D3D11RendererBase::createDepthStencilView(ID3D11Texture2D* texture, UINT arrayIndex, ID3D11DepthStencilView** outDepthStencil)
	{
		HRESULT hr;
		D3D11_RESOURCE_DIMENSION rd = D3D11_RESOURCE_DIMENSION_UNKNOWN;
		D3D11_TEXTURE2D_DESC desc = {};

		texture->GetType(&rd);
		if (rd != D3D11_RESOURCE_DIMENSION_TEXTURE2D)
			return false;

		texture->GetDesc(&desc);

		CD3D11_DEPTH_STENCIL_VIEW_DESC dsDesc(
			(desc.ArraySize > 1 ? D3D11_DSV_DIMENSION_TEXTURE2DARRAY : D3D11_DSV_DIMENSION_TEXTURE2D),
			desc.Format == DXGI_FORMAT_R24G8_TYPELESS ? DXGI_FORMAT_D24_UNORM_S8_UINT : desc.Format,
			0, arrayIndex, desc.ArraySize);

		hr = dev->CreateDepthStencilView(texture, &dsDesc, outDepthStencil);

		if (FAILED(hr))
			return false;

		return true;
	}
	bool D3D11RendererBase::createShader(const void* codeBuffer, SIZE_T codeSize, const char* entryPoint, const char* type, bool isCompiled,
		ID3D11DeviceChild** outShader, ID3D11InputLayout** outInputLayout)
	{
		ComPtr<ID3DBlob> bytecode = 0;
		ComPtr<ID3DBlob> error = 0;
		HRESULT hr = 0;

		if (!isCompiled)
		{
			D3DCompile(codeBuffer, codeSize, 0, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, type, 0, 0, bytecode.GetAddressOf(), error.GetAddressOf());

			if (!bytecode)
				return false;

			codeBuffer = bytecode->GetBufferPointer();
			codeSize = bytecode->GetBufferSize();
		}

		switch (type[0])
		{
		case 'v':
			hr = dev->CreateVertexShader(codeBuffer, codeSize, 0, (ID3D11VertexShader**)outShader);
			break;
		case 'p':
			hr = dev->CreatePixelShader(codeBuffer, codeSize, 0, (ID3D11PixelShader**)outShader);
			break;
		default:
			return false;
			break;
		}

		if (FAILED(hr))
			return false;

		if (outInputLayout)
			return generateInputLayout(codeBuffer, codeSize, outInputLayout);

		return true;
	}

	bool D3D11RendererBase::createInputLayout(const D3D11_INPUT_ELEMENT_DESC* elements, UINT numElements, 
		const void* byteCodeBuffer, SIZE_T byteCodeSize, ID3D11InputLayout** outInputLayout)
	{
		HRESULT hr = dev->CreateInputLayout(elements, numElements, byteCodeBuffer, byteCodeSize, outInputLayout);
		
		if (FAILED(hr))
			return false;

		return true;
	}

	// copied from https://gist.github.com/Cody-Duncan/d85740563ceea99f6619
	bool D3D11RendererBase::generateInputLayout(const void* byteCodeBuffer, SIZE_T byteCodeSize, ID3D11InputLayout** outInputLayout)
	{
		ComPtr<ID3D11ShaderReflection> ref;
		HRESULT hr = D3DReflect(byteCodeBuffer, byteCodeSize, __uuidof(ID3D11ShaderReflection), (void**)ref.GetAddressOf());

		if (FAILED(hr))
			return false;

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

		return createInputLayout(inputLayoutDesc.data(), inputLayoutDesc.size(), byteCodeBuffer, byteCodeSize, outInputLayout);
	}

	bool D3D11RendererBase::createBuffer(UINT stride, UINT size, const void* data, D3D11_BIND_FLAG bindFlag,
		ID3D11Buffer** outBuffer, ID3D11ShaderResourceView** outSRV,
		bool enableRead, bool enableWrite)
	{
		HRESULT hr;

		D3D11_BUFFER_DESC bufDesc = {};
		bufDesc.ByteWidth = size;
		bufDesc.StructureByteStride = stride;
		bufDesc.Usage =
			(enableRead || enableWrite ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT);
		bufDesc.CPUAccessFlags =
			(enableRead ? D3D11_CPU_ACCESS_READ : 0) |
			(enableWrite ? D3D11_CPU_ACCESS_WRITE : 0);
		bufDesc.BindFlags = bindFlag |
			(outSRV ? D3D11_BIND_SHADER_RESOURCE : 0);

		D3D11_SUBRESOURCE_DATA bufData = {};
		bufData.pSysMem = data;

		hr = dev->CreateBuffer(&bufDesc, &bufData, outBuffer);

		if (FAILED(hr))
			return false;

		if (outSRV)
			return createShaderResource(*outBuffer,
				D3D11_SRV_DIMENSION_BUFFER,
				DXGI_FORMAT_UNKNOWN, outSRV,
				0, 1, 0, 0);

		return true;
	}

}
