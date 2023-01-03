
#ifndef __GX_COMMON_H__
#define __GX_COMMON_H__

enum CompressionType
{
	kCompressionTypeNone,
	kCompressionTypeBC3,
	kCompressionTypeBC4,
	kCompressionTypeBC5,
};

#define PARSE_COMPRESSION_TYPE(dest) \
std::string compName = *(++i);\
if (compName == "None")\
dest = kCompressionTypeNone;\
else if (compName == "DXT5" || compName == "BC3")\
dest = kCompressionTypeBC3;\
else if (compName == "BC4")\
dest = kCompressionTypeBC4;\
else if (compName == "BC5")\
dest = kCompressionTypeBC5;\
else\
{\
	std::cout << "ERROR: Unknown compression type \'" << compName << "\'! use \"meshconv -h\" for help.\n";\
	exit(-1);\
}

#define WRITE_BIN(buffer, b, size) buffer.insert(buffer.end(), (const uint8_t*)&b, (const uint8_t*)&b + size)

#endif
