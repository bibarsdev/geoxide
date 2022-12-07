
#include "Geoxide/D3D11GpuProgram.h"

namespace Geoxide {

	D3D11GpuProgram::D3D11GpuProgram(D3D11RendererBase* gfx, const GpuProgramInit& args) 
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

		reflect(ref.Get());

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

		reflect(ref.Get());

		Log::Info("Created new D3D11GpuProgram \'" + args.name + "\'");

		Log::Trace("Number of buffers: " + std::to_string(buffers.size()));
		Log::Trace("Number of resources: " + std::to_string(resources.size()));

		for (auto& buffer : buffers)
		{
			auto& second = buffer.second;

			Log::Trace("Buffer \'" + buffer.first + "\':");
			Log::Trace("	Number of variables: " + std::to_string(second.vars.size()));
			Log::Trace("	Bind offset: " + std::to_string(second.bindOffset));
			Log::Trace("	Size: " + std::to_string(second.size));
		}

		for (auto& resource : resources)
		{
			auto& second = resource.second;

			Log::Trace("Resource \'" + resource.first + "\':");
			Log::Trace("	Type: " + std::to_string(second.type));
			Log::Trace("	Bind offset: " + std::to_string(second.bindOffset));
		}
	}

	void D3D11GpuProgram::reflect(ID3D11ShaderReflection* ref)
	{
		D3D11_SHADER_DESC shaderDesc = {};
		ref->GetDesc(&shaderDesc);

		for (uint32_t i = 0; i < shaderDesc.ConstantBuffers; i++)
		{
			auto shaderBuffer = ref->GetConstantBufferByIndex(i);

			D3D11_SHADER_BUFFER_DESC bufferDesc = {};
			shaderBuffer->GetDesc(&bufferDesc);

			D3D11GpuProgramBuffer& buffer = buffers[bufferDesc.Name] = D3D11GpuProgramBuffer();

			buffer.size = bufferDesc.Size;

			for (uint32_t i = 0; i < bufferDesc.Variables; i++)
			{
				auto shaderVar = shaderBuffer->GetVariableByIndex(i);
				
				D3D11_SHADER_VARIABLE_DESC varDesc = {};
				shaderVar->GetDesc(&varDesc);

				D3D11GpuProgramVariable& var = buffer.vars[varDesc.Name] = D3D11GpuProgramVariable();

				var.byteOffset = varDesc.StartOffset;
				var.size = varDesc.Size;
			}
		}

		for (uint32_t i = 0; i < shaderDesc.BoundResources; i++)
		{
			D3D11_SHADER_INPUT_BIND_DESC resourceDesc = {};
			ref->GetResourceBindingDesc(i, &resourceDesc);

			switch (resourceDesc.Type)
			{
			case D3D_SIT_CBUFFER:
				buffers.at(resourceDesc.Name).bindOffset = resourceDesc.BindPoint;
				break;
			case D3D_SIT_TEXTURE:
			{
				D3D11GpuProgramResource& resource = resources[resourceDesc.Name] = D3D11GpuProgramResource();

				resource.bindOffset = resourceDesc.BindPoint;

				switch (resourceDesc.Dimension)
				{
				case D3D_SRV_DIMENSION_TEXTURE2D:
					resource.type = kGpuResourceTypeTexture2D;
					break;
				}

				break;
			}
			}
		}

	}

	const GpuProgramBuffer* D3D11GpuProgram::getBufferByName(const std::string& name) const 
	{
		auto& iter = buffers.find(name);

		if (iter == buffers.end())
			return 0;

		return &iter.operator->()->second;
	}

	const GpuProgramResource* D3D11GpuProgram::getResourceByName(const std::string& name) const
	{
		auto& iter = resources.find(name);

		if (iter == resources.end())
			return 0;

		return &iter.operator->()->second;
	}

	const GpuProgramVariable* D3D11GpuProgramBuffer::getVariableByName(const std::string& name) const
	{
		auto& iter = vars.find(name);

		if (iter == vars.end())
			return 0;

		return &iter.operator->()->second;
	}


}
