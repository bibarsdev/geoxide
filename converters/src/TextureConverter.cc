
// TODO: add multi-tasking

#include "Geoxide/TextureConverter.h"

#define STB_DXT_IMPLEMENTATION
#include <stb_dxt.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define BC3_ARGS , 1, STB_DXT_NORMAL

#define BLOCK4X4_COMPRESS(blockType, texelType, copyTexel, type, args, fCC)\
		{\
		uint32_t blockSize = sizeof(blockType);\
		uint32_t widthInBlocks = mTexture.desc.width / 4;\
		uint32_t heightInBlocks = mTexture.desc.height / 4;\
		uint32_t numBlocks = widthInBlocks * heightInBlocks;\
		blockType* blocks = new blockType[numBlocks];\
		for (uint32_t y = 0; y < heightInBlocks; y++)\
		{\
			for (uint32_t x = 0; x < widthInBlocks; x++)\
			{\
				texelType texel[4][4];\
				byte4* rawTexel[4] = {\
					rawPixels + mTexture.desc.width * (y * 4) + x * 4,\
					rawPixels + mTexture.desc.width * (y * 4 + 1) + x * 4,\
					rawPixels + mTexture.desc.width * (y * 4 + 2) + x * 4,\
					rawPixels + mTexture.desc.width * (y * 4 + 3) + x * 4\
				};\
				for (uint32_t yy = 0; yy < 4; yy++)\
				{\
					for (uint32_t xx = 0; xx < 4; xx++)\
					{\
						copyTexel\
					}\
				}\
				stb_compress_##type##_block((uint8_t*)(blocks + y * widthInBlocks + x), (const uint8_t*)&texel args);\
			}\
		}\
		mPixelData.insert(mPixelData.end(), (uint8_t*)blocks, (uint8_t*)(blocks + numBlocks));\
		delete blocks;\
		break;\
		}\

namespace Geoxide {

	TextureConverter::TextureConverter(const std::vector<std::string>& args) :
		mFlags(0), mTexture{},
		mCompression(kCompressionTypeNone)
	{
		if (args.empty())
		{
			std::cout << "No arguments entered! use \"meshconv -h\" for help.\n";
			exit(-1);
		}

		for (auto& i = args.begin(); i < args.end(); i++)
		{
			auto& arg = *i;

			if (arg[0] == '-')
			{
				std::string opt = arg.substr(1);

				if (opt == "o")
				{
					mOutputFile = *(++i);
					if (mOutputFile.empty())
					{
						std::cout << "ERROR: No output file path specified!\n";
						exit(-1);
					}
				}
				else if (opt == "c")
				{
					PARSE_COMPRESSION_TYPE(mCompression);
				}
				else if (opt == "cube")
				{
					mFlags |= kFlagCubeMap;
				}
				else if (opt == "h" || opt == "?" || opt == "help")
				{
					std::cout <<
						"usage: meshconv [options] files\n\n"
						"options:\n"
						"\t-cube    			     make a cube texture.\n"
						"\t-t TYPE					 compress textures using TYPE.\n"
						"\t-o FILEPATH				 override output file path with FILEPATH.\n"
						"\t-h, -?, -help             display this message.\n\n"
						"texture compression types: None, DXT5/BC3, BC4, BC5.\n";
				}
				else
				{
					std::cout << "ERROR: Unknown option \'" << arg << "\'! use \"texconv -h\" for help.\n";
					exit(-1);
				}
			}
			else
			{
				fs::path path(arg);
				fs::path pathNoExt = fs::path(path).replace_extension();

				if (pathNoExt.string().back() == '*')
				{
					fs::path ext = path.extension();
					fs::path root = path.root_directory();

					for (auto& p : fs::recursive_directory_iterator(root))
					{
						if (p.path().extension() == ext)
						{
							mInputFiles.push_back(p);
						}
					}

				}

				mInputFiles.push_back(path);
			}

		}
		
		if (mInputFiles.empty())
		{
			std::cout << "ERROR: No files entered!.\n";
			exit(-1);
		}

		if (mFlags & kFlagCubeMap)
		{
			if (mInputFiles.size() != 6)
			{
				std::cout << "ERROR: A cube texture requires 6 images!. " << mInputFiles.size() << " were entered\n";
				exit(-1);
			}
		}
		else if (mInputFiles.size() > 1)
		{
			std::cout << "ERROR: Too many images entered!.";
			exit(-1);
		}

		if (mOutputFile.empty())
		{
			mOutputFile = mInputFiles[0].filename().replace_extension();
		}
	}

	void TextureConverter::start()
	{
		for (auto& file : mInputFiles)
		{
			processTexture(file.string());
		}

		mTexture.desc.headerSize = sizeof(mTexture.desc);
		mTexture.desc.mipMapCount = 1;
		mTexture.desc.flags = kDDSFlagRequired;
		mTexture.desc.complexityFlags = kDDSComplexityFlagRequired;

		mTexture.desc.ddspf.headerSize = sizeof(mTexture.desc.ddspf);

		if (mCompression == kCompressionTypeNone)
		{
			mTexture.desc.flags |= kDDSFlagPitch;
			mTexture.desc.ddspf.flags |= kDDSPixelFlagRGB | kDDSPixelFlagAlphaPixels;

			mTexture.desc.ddspf.RGBBitCount = sizeof(byte4) * 8;

			// R8G8B8A8
			mTexture.desc.ddspf.RBitMask = 0x000000FF;
			mTexture.desc.ddspf.GBitMask = 0x0000FF00;
			mTexture.desc.ddspf.BBitMask = 0x00FF0000;
			mTexture.desc.ddspf.ABitMask = 0xFF000000;
		}
		else
		{
			mTexture.desc.flags |= kDDSFlagLinearSize;
			mTexture.desc.ddspf.flags |= kDDSPixelFlagFourCC;
		}

		if (mFlags & kFlagCubeMap)
		{
			mTexture.desc.complexityFlags |= kDDSComplexityFlagComplex;
			mTexture.desc.detailFlags |= kDDSDetailFlagCubeMap;
		}

		switch (mCompression)
		{
		case kCompressionTypeNone:
		{
			uint32_t pitch = mTexture.desc.width * sizeof(byte4);

			mTexture.desc.pitchOrLinearSize = pitch;
			break;
		}
		case kCompressionTypeBC3:
		{
			uint32_t pitch = mTexture.desc.width / 4 * sizeof(byte4x4);

			mTexture.desc.pitchOrLinearSize = pitch;
			mTexture.desc.ddspf.fourCC = kDDSFourCCDXT5;
			break;
		}

		case kCompressionTypeBC4:
		{
			uint32_t pitch = mTexture.desc.width / 4 * sizeof(byte2x2);

			mTexture.desc.pitchOrLinearSize = pitch;
			mTexture.desc.ddspf.fourCC = kDDSFourCCBC4U;
			break;
		}

		case kCompressionTypeBC5:
		{
			uint32_t pitch = mTexture.desc.width / 4 * sizeof(byte4x4);

			mTexture.desc.pitchOrLinearSize = pitch;
			mTexture.desc.ddspf.fourCC = kDDSFourCCATI2;
			break;
		}

		}

		mTexture.data = mPixelData.data();
		mTexture.dataSize = mPixelData.size();

		if (DDSLoader::Write(mTexture, mOutputFile.string()))
			std::cout << "Written texture to file \'" << mOutputFile << "\'.\n";
		else
			std::cout << "ERROR: Failed to write texture to file \'" << mOutputFile << "\'.\n";
	}

	void TextureConverter::processTexture(const std::string& texFileName)
	{
		int w = 0, h = 0;

		byte4* rawPixels = (byte4*)stbi_load(
			texFileName.c_str(),
			(int*)&w,
			(int*)&h,
			0,
			STBI_rgb_alpha
		);

		if (!rawPixels)
		{
			std::cout << "ERROR: Failed to load image file \"" << texFileName << "\'.\n";
			exit(-1);
		}

		if (mTexture.desc.width & mTexture.desc.height)
		{
			if (mTexture.desc.width != w || mTexture.desc.height != h)
			{
				std::cout << "ERROR: \"" << texFileName << "\": All images must have the same dimensions.\'.\n";
				exit(-1);
			}
		}
		else
		{
			mTexture.desc.width = w;
			mTexture.desc.height = h;
		}

		switch (mCompression)
		{
		case kCompressionTypeNone:
		{
			mPixelData.insert(mPixelData.end(), (uint8_t*)rawPixels, (uint8_t*)(rawPixels + w * h));
			break;
		}
		case kCompressionTypeBC3:
			BLOCK4X4_COMPRESS(byte4x4, byte4, texel[yy][xx] = rawTexel[yy][xx];, dxt, BC3_ARGS, kDDSFourCCDXT5);
		case kCompressionTypeBC4:
			BLOCK4X4_COMPRESS(byte2x2, uint8_t, texel[yy][xx] = rawTexel[yy][xx].x;, bc4, , kDDSFourCCBC4U);
		case kCompressionTypeBC5:
			BLOCK4X4_COMPRESS(byte4x4, byte2, texel[yy][xx].x = rawTexel[yy][xx].x; texel[yy][xx].y = rawTexel[yy][xx].y;, bc5, , kDDSFourCCATI2);
		}

		delete rawPixels;
	}
}
