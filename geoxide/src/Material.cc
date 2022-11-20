
#include "Geoxide/Material.h"

namespace Geoxide {

	Material::Material() :
		uniformData(0),
		uniformDataSize(0),
		pixelShader(0) {}

	Material::Material(size_t uniformDataSize, Shader* pixelShader) :
		uniformData(0),
		pixelShader(pixelShader)
	{
		allocateUniformData(uniformDataSize);
	}

	Material::~Material()
	{
		if (uniformData)
			delete uniformData;
	}

	void Material::allocateUniformData(uint32_t size)
	{
		if (uniformData)
			delete uniformData;
		uniformData = new uint8_t[size];
		uniformDataSize = size;
	}

	void Material::copyUniformData(uint32_t destOffset, const void* src, uint32_t size)
	{
		memcpy((uint8_t*)uniformData + destOffset, src, size);
	}

}
