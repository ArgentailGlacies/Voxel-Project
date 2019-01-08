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
		Determines in which phase an object should render in. Skyboxes are always rendered before
		anything else, followed by the world.
	*/
	enum class ViewportLayer
	{
		SKYBOX,
		WORLD,
	};
}