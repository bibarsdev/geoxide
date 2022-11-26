
#define COPY_CASE(size, type) case size:{\
		type* _dest = (type*)dest;\
		const type* _src = (const type*)src;\
		for (size_t i = 0; i < numElements; i++)\
			_dest[i] = _src[i];\
		break;}

namespace Geoxide {

	D3D11RendererBase::ILFormatMap D3D11RendererBase::ILFormatDictionary = {
		{{1 * sizeof(float), kDataTypeFloat}, DXGI_FORMAT_R32_FLOAT},
		{{1 * sizeof(int), kDataTypeInt}, DXGI_FORMAT_R32_SINT},
		{{1 * sizeof(UINT), kDataTypeUInt}, DXGI_FORMAT_R32_UINT},

		{{2 * sizeof(float), kDataTypeFloat}, DXGI_FORMAT_R32G32_FLOAT},
		{{2 * sizeof(int), kDataTypeInt}, DXGI_FORMAT_R32G32_SINT},
		{{2 * sizeof(UINT), kDataTypeUInt}, DXGI_FORMAT_R32G32_UINT},

		{{3 * sizeof(float), kDataTypeFloat}, DXGI_FORMAT_R32G32B32_FLOAT},
		{{3 * sizeof(int), kDataTypeInt}, DXGI_FORMAT_R32G32B32_SINT},
		{{3 * sizeof(UINT), kDataTypeUInt}, DXGI_FORMAT_R32G32B32_UINT},

		{{4 * sizeof(float), kDataTypeFloat}, DXGI_FORMAT_R32G32B32A32_FLOAT},
		{{4 * sizeof(int), kDataTypeInt}, DXGI_FORMAT_R32G32B32A32_SINT},
		{{4 * sizeof(UINT), kDataTypeUInt}, DXGI_FORMAT_R32G32B32A32_UINT},
	};

	UINT D3D11RendererBase::bytesPerPixel(DXGI_FORMAT fmt)
	{
		switch (fmt)
		{
		case DXGI_FORMAT_R32G32B32A32_TYPELESS:
		case DXGI_FORMAT_R32G32B32A32_FLOAT:
		case DXGI_FORMAT_R32G32B32A32_UINT:
		case DXGI_FORMAT_R32G32B32A32_SINT:
			return 16;

		case DXGI_FORMAT_R32G32B32_TYPELESS:
		case DXGI_FORMAT_R32G32B32_FLOAT:
		case DXGI_FORMAT_R32G32B32_UINT:
		case DXGI_FORMAT_R32G32B32_SINT:
			return 12;

		case DXGI_FORMAT_R16G16B16A16_TYPELESS:
		case DXGI_FORMAT_R16G16B16A16_FLOAT:
		case DXGI_FORMAT_R16G16B16A16_UNORM:
		case DXGI_FORMAT_R16G16B16A16_UINT:
		case DXGI_FORMAT_R16G16B16A16_SNORM:
		case DXGI_FORMAT_R16G16B16A16_SINT:
		case DXGI_FORMAT_R32G32_TYPELESS:
		case DXGI_FORMAT_R32G32_FLOAT:
		case DXGI_FORMAT_R32G32_UINT:
		case DXGI_FORMAT_R32G32_SINT:
		case DXGI_FORMAT_R32G8X24_TYPELESS:
		case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
		case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
		case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
			return 8;

		case DXGI_FORMAT_R10G10B10A2_TYPELESS:
		case DXGI_FORMAT_R10G10B10A2_UNORM:
		case DXGI_FORMAT_R10G10B10A2_UINT:
		case DXGI_FORMAT_R11G11B10_FLOAT:
		case DXGI_FORMAT_R8G8B8A8_TYPELESS:
		case DXGI_FORMAT_R8G8B8A8_UNORM:
		case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
		case DXGI_FORMAT_R8G8B8A8_UINT:
		case DXGI_FORMAT_R8G8B8A8_SNORM:
		case DXGI_FORMAT_R8G8B8A8_SINT:
		case DXGI_FORMAT_R16G16_TYPELESS:
		case DXGI_FORMAT_R16G16_FLOAT:
		case DXGI_FORMAT_R16G16_UNORM:
		case DXGI_FORMAT_R16G16_UINT:
		case DXGI_FORMAT_R16G16_SNORM:
		case DXGI_FORMAT_R16G16_SINT:
		case DXGI_FORMAT_R32_TYPELESS:
		case DXGI_FORMAT_D32_FLOAT:
		case DXGI_FORMAT_R32_FLOAT:
		case DXGI_FORMAT_R32_UINT:
		case DXGI_FORMAT_R32_SINT:
		case DXGI_FORMAT_R24G8_TYPELESS:
		case DXGI_FORMAT_D24_UNORM_S8_UINT:
		case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
		case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
		case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
		case DXGI_FORMAT_R8G8_B8G8_UNORM:
		case DXGI_FORMAT_G8R8_G8B8_UNORM:
		case DXGI_FORMAT_B8G8R8A8_UNORM:
		case DXGI_FORMAT_B8G8R8X8_UNORM:
		case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
		case DXGI_FORMAT_B8G8R8A8_TYPELESS:
		case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
		case DXGI_FORMAT_B8G8R8X8_TYPELESS:
		case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
			return 4;

		case DXGI_FORMAT_R8G8_TYPELESS:
		case DXGI_FORMAT_R8G8_UNORM:
		case DXGI_FORMAT_R8G8_UINT:
		case DXGI_FORMAT_R8G8_SNORM:
		case DXGI_FORMAT_R8G8_SINT:
		case DXGI_FORMAT_R16_TYPELESS:
		case DXGI_FORMAT_R16_FLOAT:
		case DXGI_FORMAT_D16_UNORM:
		case DXGI_FORMAT_R16_UNORM:
		case DXGI_FORMAT_R16_UINT:
		case DXGI_FORMAT_R16_SNORM:
		case DXGI_FORMAT_R16_SINT:
		case DXGI_FORMAT_B5G6R5_UNORM:
		case DXGI_FORMAT_B5G5R5A1_UNORM:
		case DXGI_FORMAT_B4G4R4A4_UNORM:
			return 2;

		case DXGI_FORMAT_R8_TYPELESS:
		case DXGI_FORMAT_R8_UNORM:
		case DXGI_FORMAT_R8_UINT:
		case DXGI_FORMAT_R8_SNORM:
		case DXGI_FORMAT_R8_SINT:
		case DXGI_FORMAT_A8_UNORM:
		case DXGI_FORMAT_BC2_TYPELESS:
		case DXGI_FORMAT_BC2_UNORM:
		case DXGI_FORMAT_BC2_UNORM_SRGB:
		case DXGI_FORMAT_BC3_TYPELESS:
		case DXGI_FORMAT_BC3_UNORM:
		case DXGI_FORMAT_BC3_UNORM_SRGB:
		case DXGI_FORMAT_BC5_TYPELESS:
		case DXGI_FORMAT_BC5_UNORM:
		case DXGI_FORMAT_BC5_SNORM:
		case DXGI_FORMAT_BC6H_TYPELESS:
		case DXGI_FORMAT_BC6H_UF16:
		case DXGI_FORMAT_BC6H_SF16:
		case DXGI_FORMAT_BC7_TYPELESS:
		case DXGI_FORMAT_BC7_UNORM:
		case DXGI_FORMAT_BC7_UNORM_SRGB:
			return 1;

		default:
			sLog.error("Unknown DXGI format");
			return 0;
		}
	}

	DXGI_FORMAT D3D11RendererBase::translateFormat(ColorFormat format)
	{
		switch (format)
		{
		case kColorFormatRGBA32:
			return DXGI_FORMAT_R8G8B8A8_UNORM;
		default:
			sLog.error("Unknown color format");
			return DXGI_FORMAT_UNKNOWN;
		}
	}

	DXGI_FORMAT D3D11RendererBase::getInputLayoutFormat(uint32_t length, DataType type)
	{
		auto& iter = ILFormatDictionary.find({length, type});

		if (iter == ILFormatDictionary.end())
		{
			sLog.error("Unknown input layout element format, length=" + std::to_string(length) + ", type=" + std::to_string(type));
			return DXGI_FORMAT_UNKNOWN;
		}

		return iter->second;
	}

	void D3D11RendererBase::copy(void* dest, const void* src, UINT numElements, UINT elementLength)
	{
		switch (elementLength)
		{
		default:
			COPY_CASE(1, char);
			COPY_CASE(2, short);
			COPY_CASE(4, int);
			COPY_CASE(8, long long);
			COPY_CASE(16, XMVECTOR);
		}
	}

}
