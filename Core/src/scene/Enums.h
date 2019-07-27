#pragma once

namespace core
{
	/**
		Determines when an object should be rendered. Opaque objects are rendered before transparent
		objects. Transparent objects will additionally be sorted differently from opaque objects in
		order to improve rendering quality.
	*/
	enum class Translucency
	{
		OPAQUE,
		TRANSPARENT,
	};

	/**
		Determines in which core phase the object should render in. The game is rendered in 3D, then
		the gui is rendered in 2D on top of everything else.
	*/
	enum class FullscreenLayer
	{
		GAME,
		GUI,
	};

	/**
		Determines in which phase an object should render in. Skyboxes are always rendered before
		anything else, followed by the world.
	*/
	enum class ViewportLayer
	{
		SKYBOX,
		WORLD,
	};
}