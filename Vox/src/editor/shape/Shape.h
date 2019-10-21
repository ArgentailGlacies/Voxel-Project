#pragma once

#include "editor/shape/ShapeMesher.h"
#include "scene/SceneNode.h"
#include "world/Block.h"
#include "world/Query.h"

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
		Shape(core::Scene & scene);
		virtual ~Shape();
		
		/**
			Assigns the shape and position of the shape. The actual shape will be remeshed such that
			it span from the from location to the to location. The actual shape of the mesh will be
			determined by the mesher function.

			@param from The position the shape should start at.
			@param to The position the shape should end at.
		*/
		void stretch(const glm::ivec3 & from, const glm::ivec3 & to);

		/**
			Obtains a query for reading from any world, containing the entire volume spanned out by
			the shape.
		*/
		inline WorldQuery read() const { return query(m_from, m_to, {}); }
		/**
			Obtains a query for writing to any world, containing the entire volume spanned out by
			the shape. The query will be filled with the provided block.

			@param block The block which should fill the entire volume spanned by the shape.
		*/
		inline WorldQuery write(const Block & block) const { return query(m_from, m_to, block); }
		
		/**
			Specifies whether the shape should be visible or not. If the shape is visible, the shape
			mesh will be rendered to the screen.

			@param visible Whether the shape should be considered visible or not.
		*/
		inline void setVisible(bool visible) { m_visible = visible; }

	protected:
		/**
			Retrieves a world query which exactly match the volume spanned out by the shape. The
			query can be used to read from and/or write to any world.

			@param from The first corner specifying the volume the shape fills.
			@param to The second corner specifying the volume the shape fills.
			@param block The block which should fill the query; does not matter if reading blocks.
		*/
		virtual WorldQuery query(const glm::ivec3 & from, const glm::ivec3 & to, const Block & block) const = 0;
		/**
			Generates a mesh which represents the overall volume spanned out by the shape. The shape
			does not need to be convex, nor does it need to be a single, connected volume.

			@param from The first corner specifying the volume the mesh should fill.
			@param to The second corner specifying the volume the mesh should fill.
		*/
		virtual ShapeMeshPtr mesh(const glm::ivec3 & from, const glm::ivec3 & to) const = 0;

	private:
		/**
			Renders the shape mesh, if it is set to be visible.
		*/
		void render() const;

		// ...

		core::Scene & m_scene;
		core::SceneEntry m_transform = 0;
		core::SceneEntry m_renderer = 0;

		ShapeMeshPtr m_mesh;
		bool m_visible = false;

		glm::ivec3 m_from = {};
		glm::ivec3 m_to = {};
	};

	// ...

	/**
		The point shape represents a single block volume. The volume will never be larger than a
		single block.
	*/
	class ShapePoint : public Shape
	{
	public:
		ShapePoint(core::Scene & scene) : Shape(scene) {}

	private:
		virtual WorldQuery query(const glm::ivec3 & from, const glm::ivec3 & to, const Block & block) const override final;
		virtual ShapeMeshPtr mesh(const glm::ivec3 & from, const glm::ivec3 & to) const override final;
	};

	/**
		The rectangle class represents a volume which is rectangular in shape. The volume may be
		hollow, where the walls have a varying thickness.
	*/
	class ShapeRectangle : public Shape
	{
	public:
		ShapeRectangle(core::Scene & scene) : Shape(scene) {}

	private:
		virtual WorldQuery query(const glm::ivec3 & from, const glm::ivec3 & to, const Block & block) const override final;
		virtual ShapeMeshPtr mesh(const glm::ivec3 & from, const glm::ivec3 & to) const override final;
	};
}