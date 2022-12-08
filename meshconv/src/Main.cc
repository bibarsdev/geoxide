
#include "Geoxide/MeshConverter.h"

using namespace Geoxide;
// https://youtu.be/Pivfl-Dfqno?t=1833

int main(int argc, char** argv)
{
	bool _debug = false;
	char _debugCommadline[] =
		"meshconv -mo D:/dev/geoxide/test/assets/models -to D:/dev/geoxide/test/assets/textures -mao D:/dev/geoxide/test/assets/materials -cdt BC3 "
		"-cst BC4 D:/dev/models/bunny/bunny.fbx D:/dev/models/lucy/lucy.fbx D:/dev/models/teapot.fbx D:/dev/models/link_statue/link_statue.fbx";
	std::vector<char*> _debug_argv;

	if (_debug)
	{
		char* token = strtok(_debugCommadline, " ");

		while (token)
		{
			_debug_argv.push_back(token);
			token = strtok(0, " ");
		}

		argc = _debug_argv.size();
		argv = _debug_argv.data();
	}

	if (argc < 2)
	{
		std::cout << "No arguments entered! use \"meshconv -h\" for help.\n";
		exit(-1);
	}

	MeshConverter meshconv(std::vector<std::string>(argv + 1, argv + argc));
	meshconv.start();

	return 0;
}
