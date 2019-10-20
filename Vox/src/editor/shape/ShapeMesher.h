#pragma once

#include "opengl/Mesh.h"

#include <functional>
#include <glm/vec3.hpp>
#include <memory>

namespace vox
{
	using ShapeMesh = core::Mesh<glm::vec3>;
	using ShapeMeshPtr = std::unique_ptr<ShapeMesh>;
	using ShapeMesher = std::function<ShapeMeshPtr(const glm::ivec3 &, const glm::ivec3 &)>;

	// ...

	/**
		Creates a shape mesh representing a single block. The from coordinate is ignored; the mesh
		will only occupy the block at the to coordinate.

		@param from Ignored, not used in the point mesher.
		@param to The coordinate of the point which should be meshed.
	*/
	ShapeMeshPtr meshPoint(const glm::ivec3 & from, const glm::ivec3 & to);
	/**
		Creates a shape mesh representing a rectangular volume. The volume spans from the from
		coordinate to the to coordinate. The ordering of the coordinates does not matter; the entire
		volume will be included in the mesh.

		@param from The first corner forming the rectangle to mesh.
		@param to The second coordinate forming the rectangle to mesh.
	*/
	ShapeMeshPtr meshRectangle(const glm::ivec3 & from, const glm::ivec3 & to);
}