
#include "MeshScene.h"

#ifdef GX_PLATFORM_WIN32
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#endif // GX_PLATFORM_WIN32

void MeshScene::prepareScene()
{
	mApp->setBackColor(NewVector(0.52f, 0.81f, 0.92f, 1));

	mDebug0 = -5;
	mDebug1 = 0;

	mMainCamera->setPosition(NewVector(0, 0, mDebug0));

	static const float cubeVertexData[][8] =
	{
		{-1, -1, -1, 1, 1, 0, 0, 1},
		{-1, 1, -1, 1, 0, 1, 0, 1},
		{1, 1, -1, 1, 0, 0, 1, 1},
		{1, -1, -1, 1, 1, 1, 0, 1},
		{-1, -1, 1, 1, 0, 1, 1, 1},
		{-1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 0, 1, 1},
		{1, -1, 1, 1, 1, 0, 0, 1},
	};

	static const uint32_t cubeIndexData[] = {
		0, 1, 2,
		0, 2, 3,
		4, 6, 5,
		4, 7, 6,
		4, 5, 1,
		4, 1, 0,
		3, 2, 6,
		3, 6, 7,
		1, 5, 6,
		1, 6, 2,
		4, 0, 3,
		4, 3, 7
	};

	MeshDataInit cubeMeshDesc = {};

	cubeMeshDesc.vertexData = cubeVertexData;
	cubeMeshDesc.vertexDataSize = sizeof(cubeVertexData);
	cubeMeshDesc.vertexLength = sizeof(cubeVertexData[0]);

	cubeMeshDesc.indexData = cubeIndexData;
	cubeMeshDesc.indexDataSize = sizeof(cubeIndexData);
	cubeMeshDesc.indexLength = sizeof(cubeIndexData[0]);

	mCubeMeshData = mGfx->newMeshData(cubeMeshDesc);

	GpuProgramInit colorProgramDesc = {};

	colorProgramDesc.name = "Color";

#ifdef GX_PLATFORM_WIN32

	ID3DBlob* vsByteCode = 0;
	ID3DBlob* psByteCode = 0;
	ID3DBlob* errorMsg = 0;

	HRESULT hr = D3DCompileFromFile(L"assets/shaders/Color.hlsl", 
		0, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"vertex", "vs_4_0",
		D3DCOMPILE_OPTIMIZATION_LEVEL3 | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR, 0,
		&vsByteCode, &errorMsg);

	if (FAILED(hr))
		if (errorMsg)
			Log::Fatal("Failed to compile vertex shader, errorMsg=" + std::string((const char*)errorMsg->GetBufferPointer()));
		else
			Log::Fatal("Failed to compile vertex shader");

	hr = D3DCompileFromFile(L"assets/shaders/Color.hlsl",
		0, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"pixel", "ps_4_0",
		D3DCOMPILE_OPTIMIZATION_LEVEL3 | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR, 0,
		&psByteCode, &errorMsg);

	if (FAILED(hr))
		if (errorMsg)
			Log::Fatal("Failed to compile pixel shader, errorMsg=" + std::string((const char*)errorMsg->GetBufferPointer()));
		else
			Log::Fatal("Failed to compile pixel shader");

	colorProgramDesc.vertex.name = "Color.vertex";
	colorProgramDesc.vertex.code = vsByteCode->GetBufferPointer();
	colorProgramDesc.vertex.codeSize = vsByteCode->GetBufferSize();
	colorProgramDesc.vertex.isCompiled = true;

	colorProgramDesc.pixel.name = "Color.pixel";
	colorProgramDesc.pixel.code = psByteCode->GetBufferPointer();
	colorProgramDesc.pixel.codeSize = psByteCode->GetBufferSize();
	colorProgramDesc.pixel.isCompiled = true;

	mColorProgram = mGfx->newGpuProgram(colorProgramDesc);

	vsByteCode->Release();
	psByteCode->Release();

	if (errorMsg)
		errorMsg->Release();

#endif // GX_PLATFORM_WIN32

	mCubeMaterial = new Material(mColorProgram);

	mCubeMesh = new Mesh(mCubeMeshData);
	mCubeMesh->addSubMesh(SubMesh{ 0, cubeMeshDesc.indexDataSize / cubeMeshDesc.indexLength, mCubeMaterial });

	mCubeObject = new MeshObject(mApp, mCubeMesh);
	mApp->getRootObject()->addChild(mCubeObject);
}

void MeshScene::destroyScene()
{
	SAFE_DELETE(mCubeMeshData);
	SAFE_DELETE(mColorProgram);
	SAFE_DELETE(mCubeMaterial);
	SAFE_DELETE(mCubeMesh);
	SAFE_DELETE(mCubeObject);
}

void MeshScene::onFrameStart(FrameEvent*)
{
	mDebug0 += 0.001f;
	mDebug1 += 0.01f;

	mMainCamera->yaw(mDebug1);
}

void MeshScene::onFrameEnd(FrameEvent*)
{
}
