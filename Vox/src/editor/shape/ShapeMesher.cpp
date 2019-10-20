
#include "ShapeMesher.h"

#include "util/MathOperations.h"

namespace
{
	/**
		Finalizes a mesh by adding all wireframe attributes, and builds the mesh. The mesh will be
		ready for rendering after this function call.

		@param mesh The mesh which should be finalized.
	*/
	void build(vox::ShapeMesh & mesh)
	{
		mesh.addAttribute(core::Attribute{ 0, 3, 0, 12, core::DataFormat::FLOAT });
		mesh.build();
	}

	// ...

	/**
		Adds a box to the provided mesh. The box will span from the given from coordinate to the to
		coordinate plus one. The from and to coordinate may be equal to form a box of size one. It
		is irrelevant in which order the coordinates are specified, as long as they form a valid
		volume.

		@oaram mesh The mesh the box should be added to.
		@param from The starting coordinate of the box to add.
		@param to The ending coordinate of the box to add.
	*/
	void addBox(vox::ShapeMesh & mesh, const glm::ivec3 & from, const glm::ivec3 & to)
	{
		auto & vertices = mesh.vertices();
		auto & indices = mesh.indices();

		const auto count = vertices.size();
		const auto min = util::min(from, to);
		const auto max = util::max(from, to);

		// Add eight corner vertices
		vertices.emplace_back(min.x, min.y, min.z);
		vertices.emplace_back(min.x, min.y, max.z);
		vertices.emplace_back(min.x, max.y, min.z);
		vertices.emplace_back(min.x, max.y, max.z);
		vertices.emplace_back(max.x, min.y, min.z);
		vertices.emplace_back(max.x, min.y, max.z);
		vertices.emplace_back(max.x, max.y, min.z);
		vertices.emplace_back(max.x, max.y, max.z);

		// Add twelve lines
		indices.push_back(count + 0);
		indices.push_back(count + 1);
		indices.push_back(count + 2);
		indices.push_back(count + 3);
		indices.push_back(count + 4);
		indices.push_back(count + 5);
		indices.push_back(count + 6);
		indices.push_back(count + 7);

		indices.push_back(count + 0);
		indices.push_back(count + 2);
		indices.push_back(count + 1);
		indices.push_back(count + 3);
		indices.push_back(count + 4);
		indices.push_back(count + 6);
		indices.push_back(count + 5);
		indices.push_back(count + 7);

		indices.push_back(count + 0);
		indices.push_back(count + 4);
		indices.push_back(count + 1);
		indices.push_back(count + 5);
		indices.push_back(count + 2);
		indices.push_back(count + 6);
		indices.push_back(count + 3);
		indices.push_back(count + 7);
	}
}

vox::ShapeMeshPtr vox::meshPoint(const glm::ivec3 & from, const glm::ivec3 & to)
{
	ShapeMeshPtr mesh = std::make_unique<ShapeMesh>();
	addBox(*mesh, to, to);
	build(*mesh);
	return mesh;
}
