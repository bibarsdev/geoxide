
#ifndef __GX_FORMAT_H__
#define __GX_FORMAT_H__

namespace Geoxide {

	enum DataType
	{
		kDataUnknown,
		kDataFloat,
		kDataInt,
		kDataUInt,
		kDataByte,
		kDataBC3,
		kDataBC4,
		kDataBC5,
	};

	struct FormatDesc
	{
		DataType type;
		uint32_t bitCount;
		uint32_t RMask;
		uint32_t GMask;
		uint32_t BMask;
		uint32_t AMask;
	};

}

#endif // !__GX_TEXTURE_H__

