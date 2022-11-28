
#include "Geoxide/D3D11GpuProgram.h"

namespace Geoxide {

	D3D11GpuProgram::D3D11GpuProgram(D3D11RendererBase* gfx, const GpuProgramInit& args) : 
		vsGlobalsSize(0), psGlobalsSize(0)
	{
		ComPtr<ID3DBlob> blob;
		ComPtr<ID3D11ShaderReflection> ref;
		ID3D11Device* dev = gfx->dev.Get();
		HRESULT hr = 0;

		const void* byteCode = 0;
		size_t byteCodeSize = 0;

		if (args.vertex.isCompiled)
		{
			byteCode = args.vertex.code;
			byteCodeSize = args.vertex.codeSize;
		}
		else
		{
			gfx->compileShader(args.vertex.name, args.vertex.code, args.vertex.codeSize, args.vertex.entry, "vs_5_0", &blob);

			byteCode = blob->GetBufferPointer();
			byteCodeSize = blob->GetBufferSize();
		}

		hr = dev->CreateVertexShader(byteCode, byteCodeSize, 0, &vertexShader);

		if (FAILED(hr))
			GX_FUNC_THROW("Failed to create ID3D11VertexShader");

		hr = D3DReflect(byteCode, byteCodeSize, __uuidof(ID3D11ShaderReflection), (void**)&ref);

		if (FAILED(hr))
			GX_FUNC_THROW("Failed to reflect vertex shader \'" + args.vertex.name + "\' for program \'" + args.name + "\'. " + GetFormattedMessage(hr));

		gfx->generateInputLayout(ref.Get(), byteCode, byteCodeSize, &inputLayout);
		gfx->reflectGlobalVariables(ref.Get(), globalVars, &vsGlobalsSize);

		blob.Reset();
		ref.Reset();

		if (args.pixel.isCompiled)
		{
			byteCode = args.pixel.code;
			byteCodeSize = args.pixel.codeSize;
		}
		else
		{
			gfx->compileShader(args.pixel.name, args.pixel.code, args.pixel.codeSize, args.pixel.entry, "ps_5_0", &blob);

			byteCode = blob->GetBufferPointer();
			byteCodeSize = blob->GetBufferSize();
		}

		hr = dev->CreatePixelShader(byteCode, byteCodeSize, 0, &pixelShader);

		if (FAILED(hr))
			GX_FUNC_THROW("Failed to create ID3D11PixelShader");

		hr = D3DReflect(byteCode, byteCodeSize, __uuidof(ID3D11ShaderReflection), (void**)&ref);

		if (FAILED(hr))
			GX_FUNC_THROW("Failed to reflect pixel shader \'" + args.pixel.name + "\' for program \'" + args.name + "\'. " + GetFormattedMessage(hr));

		gfx->reflectGlobalVariables(ref.Get(), globalVars, &psGlobalsSize);

		Log::Info("Created new D3D11GpuProgram \'" + args.name + "\'");

		Log::Trace("Vertex shader globals size: " + std::to_string(vsGlobalsSize));
		Log::Trace("Pixel shader globals size: " + std::to_string(psGlobalsSize));
		Log::Trace("Global variables: " + std::to_string(globalVars.size()));

		for (auto& var : globalVars)
			Log::Trace("	Variable \'" + var.first + "\': byteOffset=" + std::to_string(var.second));
	}

	uint32_t D3D11GpuProgram::getGlobalVariableOffset(const std::string& variable) const
	{
		auto& iter = globalVars.find(variable);

		if (iter == globalVars.end())
			return -1;

		return iter->second;
	}

}
