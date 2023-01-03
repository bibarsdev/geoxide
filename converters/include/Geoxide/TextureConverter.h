
#ifndef __TEXTURE_CONVERTER_H__
#define __TEXTURE_CONVERTER_H__

#include <Geoxide/DDSLoader.h>
#include "Common.h"

namespace Geoxide {

	namespace fs = std::filesystem;

	class TextureConverter
	{
	public:
		enum Flag
		{
			kFlagCubeMap = 0x1,
		};

	public:
		TextureConverter(const std::vector<std::string>& args);
		~TextureConverter() {}

		void start();

	private:
		void processTexture(const std::string& texFileName);

	private:
		fs::path mOutputFile;
		std::vector<fs::path> mInputFiles;
		uint32_t mFlags;
		CompressionType mCompression;

		DDSData mTexture;
		std::vector<uint8_t> mPixelData;

	};

}

#endif
