
#include "Geoxide/DDSLoader.h"

#define WRITE_BIN(b) file.write((const char*)&b, sizeof(b))
#define WRITE_NULL() file.write((const char*)&kNULL, sizeof(kNULL))
#define WRITE_STR(s) file << s; WRITE_NULL()
#define WRITE_BUFFER(b, size) file.write((const char*)b, size)

#define READ_BIN(b) file.read((char*)&b, sizeof(b))
#define READ_STR(s) std::getline(file, s, '\0')
#define READ_BUFFER(b, size) file.read((char*)b, size)

namespace Geoxide {
	
	constexpr int kMagicNumber = ' SDD';
	constexpr uint8_t kNULL = 0;

	size_t DDSLoader::Read(const std::string& filepath, DDSData& dds)
	{
		std::ifstream file(filepath, std::ios::binary);

		if (!file.is_open())
			return 0;

		int magicNumber = 0;

		READ_BIN(magicNumber);

		if (magicNumber != kMagicNumber)
		{
			Log::Error("\'" + filepath + "\' is not a DDS file");
			return 0;
		}

		READ_BIN(dds.desc);

		size_t pos = file.tellg();

		file.seekg(0, std::ios::end);
		size_t size = file.tellg();
		size_t dataSize = size - pos;
		file.seekg(pos);

		dds.dataSize = dataSize;
		dds.data = new uint8_t[dds.dataSize];

		READ_BUFFER(dds.data, dds.dataSize);

		return size;
	}

	size_t DDSLoader::Write(const DDSData& dds, const std::string& filepath)
	{
		std::ofstream file(filepath, std::ios::binary);

		if (!file.is_open())
			return 0;

		WRITE_BIN(kMagicNumber);
		WRITE_BIN(dds.desc);

		WRITE_BUFFER(dds.data, dds.dataSize);

		size_t size = file.tellp();

		return size;
	}


	void DDSLoader::Free(DDSData& dds)
	{
		delete dds.data;
	}
}
