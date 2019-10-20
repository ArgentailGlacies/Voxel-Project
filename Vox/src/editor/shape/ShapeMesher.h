#pragma once

#include "opengl/Mesh.h"

#include <functional>
#include <glm/vec3.hpp>
#include <memory>

namespace vox
{
	using ShapeMesh = std::unique_ptr<core::Mesh<glm::vec3>>;
	using ShapeMesher = std::function<ShapeMesh(const glm::ivec3 &, const glm::ivec3 &)>;

	// ...

	/**
		Creates a shape mesh representing a single block. The from coordinate is ignored; the mesh
		will only occupy the block at the to coordinate.

		@param from Ignored, not used in the point mesher.
		@param to The coordinate of the point which should be meshed.
	*/
	ShapeMesh meshPoint(const glm::ivec3 & from, const glm::ivec3 & to);
}