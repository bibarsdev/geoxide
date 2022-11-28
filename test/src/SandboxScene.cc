
#include "SandboxScene.h"

#include <fstream>

void SandboxScene::prepareScene()
{
	std::ifstream colorHLSLFile("assets/shaders/Color.hlsl");

	std::string fileBuffer((std::istreambuf_iterator<char>(colorHLSLFile)), std::istreambuf_iterator<char>());

	colorHLSLFile.close();

	GpuProgramInit programDesc = {};

	programDesc.name = "Color";

	programDesc.vertex.name = "Color.vertex";
	programDesc.vertex.code = fileBuffer.c_str();
	programDesc.vertex.codeSize = fileBuffer.size();
	programDesc.vertex.entry = "vertex";

	programDesc.pixel.name = "Color.pixel";
	programDesc.pixel.code = fileBuffer.c_str();
	programDesc.pixel.codeSize = fileBuffer.size();
	programDesc.pixel.entry = "pixel";

	mColorProgram = mGfx->newGpuProgram(programDesc);
}

void SandboxScene::destroyScene()
{
	SAFE_DELETE(mColorProgram);
}

void SandboxScene::onFrameStart(FrameEvent*)
{
}

void SandboxScene::onFrameEnd(FrameEvent*)
{
}
