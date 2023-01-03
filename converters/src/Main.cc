
#if defined(GX_MESH_CONVERTER)
#include "Geoxide/MeshConverter.h"
#elif defined(GX_TEXTURE_CONVERTER)
#include "Geoxide/TextureConverter.h"
#else
#error No converter
#endif // DEBUG

using namespace Geoxide;

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cout << "No arguments entered! use \"meshconv -h\" for help.\n";
		exit(-1);
	}

	std::vector<std::string> args(argv + 1, argv + argc);

#if defined(GX_MESH_CONVERTER)
	MeshConverter meshconv(args);
	meshconv.start();
#elif defined(GX_TEXTURE_CONVERTER)
	TextureConverter meshconv(args);
	meshconv.start();
#endif // DEBUG

	return 0;
}
