#pragma once

#include "editor/shape/ShapeMesher.h"
#include "scene/SceneNode.h"

#include <glm/vec3.hpp>

namespace core
{
	class Scene;
}

namespace vox
{
	/**
		Whenever a user desires to modify anything within the world editor, the shape allows the
		user to see what will be affected by any tool. The shape should visualize the volume the
		currently active brush will affect.
	*/
	class Shape
	{
	public:
		Shape(core::Scene & scene, ShapeMesher mesher);
		~Shape();

		/**
			Assigns the shape and position of the shape. The actual shape will be remeshed such that
			it span from the from location to the to location. The actual shape of the mesh will be
			determined by the mesher function.

			@param from The position the shape should start at.
			@param to The position the shape should end at.
		*/
		inline void stretch(const glm::ivec3 & from, const glm::ivec3 & to) { m_mesh = m_mesher(from, to); }
		
		/**
			Specifies whether the shape should be visible or not. If the shape is visible, the shape
			mesh will be rendered to the screen.

			@param visible Whether the shape should be considered visible or not.
		*/
		inline void setVisible(bool visible) { m_visible = visible; }

	private:
		/**
			Renders the shape mesh, if it is set to be visible.
		*/
		void render() const;

		// ...

		core::Scene & m_scene;
		core::SceneEntry m_renderer = 0;

		ShapeMesher m_mesher;
		ShapeMesh m_mesh;
		bool m_visible = true;
	};
}