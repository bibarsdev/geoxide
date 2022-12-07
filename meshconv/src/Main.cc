
#include "Geoxide/MeshConverter.h"

using namespace Geoxide;

int main(int argc, char** argv)
{
	/*argc = 2;
	argv = new char* [argc];
	argv[1] = "D:/dev/models/link_statue/link_statue.dae";*/

	if (argc < 2)
	{
		std::cout << "No arguments entered! use \"meshconv -h\" for help.\n";
		exit(-1);
	}

	MeshConverter meshconv(std::vector<std::string>(argv + 1, argv + argc));
	meshconv.start();

	return 0;
}
