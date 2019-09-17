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
		const std::string FONTS = "fonts";
		const std::string SHADERS = "shaders";
		const std::string SPRITES = "sprites";
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
		All script modules are specified in this namespace.
	*/
	namespace res::script
	{
		const std::string ENGINE = "engine";
		const std::string GUI = "gui";
		const std::string GUI_REGISTRY = "gui_registry";
	}

	/**
		All special assets which should receive special names.
	*/
	namespace res::asset
	{
		const std::string WIREFRAME_SHADER_PROGRAM = "wireframe";
		const std::string WORLD_SHADER_PROGRAM = "world";

		const std::string BLOCK_TEXTURE_ATLAS = "block_texture_atlas";
	}
}