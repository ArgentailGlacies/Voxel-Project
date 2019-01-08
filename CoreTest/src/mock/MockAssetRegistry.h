#pragma once

#include "asset/AssetRegistry.h"
#include "opengl/Program.h"
#include "opengl/Texture.h"

namespace core
{
	inline AssetRegistry mockAssetRegistry()
	{
		AssetRegistry registry;

		registry.add<Program>("program", []()
		{
			return std::make_unique<Program>();
		});
		registry.add<Texture>("texture", []()
		{
			return std::make_unique<Texture>(
				TextureType::TEXTURE_2D,
				DataFormat::UNSIGNED_BYTE,
				ColorFormat::RGBA
			);
		});

		return registry;
	}
}