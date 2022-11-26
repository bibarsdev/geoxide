
#include "Geoxide/D3D11GpuProgram.h"

namespace Geoxide {

	D3D11GpuProgram::D3D11GpuProgram(D3D11RendererBase* gfx, const GpuProgramInit& args)
	{
		if (!gfx->createShader(args.vertex.byteCode, args.vertex.byteCodeSize,
			0, "vs_4_0", true,
			(ID3D11DeviceChild**)vertexShader.GetAddressOf(), 0))
		{
			sLog.error("Failed to create vertex shader");
			return;
		}

		if (!gfx->createShader(args.pixel.byteCode, args.pixel.byteCodeSize,
			0, "ps_4_0", true,
			(ID3D11DeviceChild**)pixelShader.GetAddressOf(), 0))
		{
			sLog.error("Failed to create pixel shader");
			return;
		}

		std::unique_ptr<D3D11_INPUT_ELEMENT_DESC> elements(new D3D11_INPUT_ELEMENT_DESC[args.inputLayoutSize]);

		for (uint32_t i = 0; i < args.inputLayoutSize; i++)
		{
			auto& argsElement = args.inputLayout[i];
			auto& element = elements.get()[i];

			element.SemanticName = argsElement.semantic;
			element.SemanticIndex = argsElement.semanticIndex;
			element.AlignedByteOffset = argsElement.offset;
			element.Format = gfx->getInputLayoutFormat(argsElement.length, argsElement.type);
			element.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			element.InputSlot = 0;
			element.InstanceDataStepRate = 0;
		}

		if (!gfx->createInputLayout(elements.get(), args.inputLayoutSize, args.vertex.byteCode, args.vertex.byteCodeSize, inputLayout.GetAddressOf()))
		{
			sLog.error("Failed to create input layout");
			return;
		}

		sLog.info("Created a new program");
	}

}
