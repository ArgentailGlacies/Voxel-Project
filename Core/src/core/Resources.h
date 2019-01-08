#pragma once

#include <string>

namespace core
{
	/**
		All resource paths are specified in this namespace. All paths are relative to the root data
		directory.
	*/
	namespace res::path
	{
		const std::string SHADERS = "shaders";
		const std::string UNIVERSE = "universe";
	}

	/**
		All ubo blocks and members are specified in this namespace.
	*/
	namespace res::ubo
	{
		const std::string MATRICES = "Matrices";

		const std::string PROJECTION = "projection";
		const std::string VIEW = "view";
		const std::string MODEL = "model";
	}

	/**
		All special assets which should receive special names.
	*/
	namespace res::asset
	{
		const std::string WORLD_SHADER_PROGRAM = "world";

		const std::string BLOCK_TEXTURE_ATLAS = "block_texture_atlas";
	}
}