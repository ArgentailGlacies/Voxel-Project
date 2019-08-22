#pragma once

#include "scene/SceneNode.h"

#include <glm/vec3.hpp>

namespace core
{
	class Scene;
}

namespace vox
{
	class Grid
	{
	public:
		Grid(core::Scene & scene);
		~Grid();

		/**
			Sets the center of the grid in world space. The position should be at an integer
			position for the best visual effect.

			@param pos The position in world space.
		*/
		void setPosition(const glm::vec3 & pos);
		/**
			Assigns the size of the grid and the spacing of each cell. The grid will contain size^2
			cells in total, each cell the size of spacing^2.

			@param size The number of cells along each axis.
			@param spacing The size of each individual cell.
		*/
		void setSize(int size, int spacing);

		/**
			Assigns the visibility of the grid to be either visible to the user or not.

			@param visible Whether the grid should be visible to the user or not.
		*/
		void setVisible(bool visible);

	private:
		core::Scene & m_scene;
		core::SceneEntry m_transform = 0;
		core::SceneEntry m_renderer = 0;
	};
}