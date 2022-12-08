
#define COPY_CASE(size, type) case size:{\
		type* _dest = (type*)dest;\
		const type* _src = (const type*)src;\
		for (size_t i = 0; i < numElements; i++)\
			_dest[i] = _src[i];\
		break;}

namespace Geoxide {

	D3D11RendererBase::FormatMap D3D11RendererBase::formatDictionary =
	{
		{{kDataTypeUnknown, 4 * 8, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000}, DXGI_FORMAT_R8G8B8A8_TYPELESS},
		{{kDataTypeByte, 4 * 8, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000}, DXGI_FORMAT_R8G8B8A8_UNORM},

		{{kDataTypeUnknown, 4 * 8, 0x000000FF, 0x00000000, 0x00000000, 0x00000000}, DXGI_FORMAT_R32_TYPELESS},
		{{kDataTypeFloat, 4 * 8, 0x000000FF, 0x00000000, 0x00000000, 0x00000000}, DXGI_FORMAT_R32_FLOAT},
		{{kDataTypeInt, 4 * 8, 0x000000FF, 0x00000000, 0x00000000, 0x00000000}, DXGI_FORMAT_R32_SINT},
		{{kDataTypeUInt, 4 * 8, 0x000000FF, 0x00000000, 0x00000000, 0x00000000}, DXGI_FORMAT_R32_UINT},

		{{kDataTypeBC3, 0, 0, 0, 0, 0}, DXGI_FORMAT_BC3_UNORM},
		{{kDataTypeBC4, 0, 0, 0, 0, 0}, DXGI_FORMAT_BC4_UNORM},
		{{kDataTypeBC5, 0, 0, 0, 0, 0}, DXGI_FORMAT_BC5_UNORM},
	};

	D3D11RendererBase::ILFormatMap D3D11RendererBase::ILFormatDictionary =
	{
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

	UINT D3D11RendererBase::bitsPerPixel(DXGI_FORMAT fmt)
	{
		switch (fmt)
		{
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

	DXGI_FORMAT D3D11RendererBase::translateFormat(const ColorFormat& format)
	{
		auto& iter = formatDictionary.find(format);

		if (iter == formatDictionary.end())
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

	DXGI_FORMAT D3D11RendererBase::getInputLayoutFormat(uint32_t length, DataType type)
	{
		auto& iter = ILFormatDictionary.find({length, type});

		if (iter == ILFormatDictionary.end())
			GX_FUNC_THROW("Unknown input layout element format. length=" + std::to_string(length) + ", type=" + std::to_string(type));

		return iter->second;
	}

	void D3D11RendererBase::copy(void* dest, const void* src, UINT numElements, UINT elementLength)
	{
		switch (elementLength)
		{
			COPY_CASE(1, char);
			COPY_CASE(2, short);
			COPY_CASE(4, int);
			COPY_CASE(8, long long);
			COPY_CASE(16, XMVECTOR);
		default:
			GX_FUNC_THROW("Unknown element length. elementLength=" + std::to_string(elementLength));
		}
	}

}
