#pragma once

#include "asset/Asset.h"
#include "opengl/Program.h"
#include "opengl/Texture.h"
#include "scene/Enums.h"

#include <functional>
#include <glm/mat4x4.hpp>
#include <unordered_set>

namespace core
{
	class Camera;
	struct ScheduleContext;

	/**
		All objects added to the scene will be uniquely represented by an entry id. The id will be
		different for all entries in the scene.
	*/
	using SceneEntry = unsigned int;

	/**
		Scene nodes are organized in a tree structure. All nodes are connected to a root node or a
		child of the root node (with the exception of camera nodes, which are root nodes). The scene
		consists of any number of camera nodes, with a root node attached to all cameras. Any other
		node will then be attached either to this special root node to be visible for all cameras,
		or attached only to certain cameras to be visible only for those cameras.
	*/
	class Node
	{
		friend class Scene;

	public:
		virtual ~Node() = default;

		/**
			Schedules the rendering logic for the node and all child nodes. Certain nodes will not
			actually schedule anything in the rendering queue, but rather modify some data other
			nodes require to schedule their tasks.
		*/
		virtual void schedule(ScheduleContext & context) const;

		void attach(Node & child);
		void detach(Node & child);

	private:
		std::unordered_set<Node *> m_parents;
		std::unordered_set<Node *> m_children;

		SceneEntry m_entry;
	};

	/**
		The camera node holds a specific camera, which adds another viewpoint the entire scene
		should be rendered from. If the camera is invisible, it will be skipped and nothing will be
		rendered for that camera.
	*/
	class CameraNode : public Node
	{
		friend class Scene;

	public:
		CameraNode(const Camera & camera, unsigned int id) : m_camera(camera), m_id(id) {}

		/**
			Schedules a matrix UBO update into the render queue, such that the view and projection
			matrices are updated in shader programs. Will also schedule a binding of the correct FBO
			to the current OpenGL context.

			The render key will also be updated to reflect which camera is currently active. The
			context will also be changed, such that it holds a reference to this specific camera.
		*/
		virtual void schedule(ScheduleContext & context) const override final;

	private:
		const Camera & m_camera;

		unsigned int m_id;
	};

	/**
		Stores a transformation from model to world space. Multiple transformations may be chained
		in sequence after each other, where each following transformation is multiplied from the
		right into the cumulative transformation. The final transformation is a model-to-world
		transformation.
	*/
	class TransformNode : public Node
	{
		friend class Scene;

	public:
		/**
			Pushes the actual transformation into the transformation stack. Will also update the
			render key such that the correct depth information is stored.
		*/
		virtual void schedule(ScheduleContext & context) const override final;

	private:
		/**
			Recalculates the depth of the rendering key from the current top transformation in the
			transformation stack.
		*/
		void update(ScheduleContext & context) const;

		glm::mat4 m_transformation = glm::mat4{ 1.0f };
	};

	/**
		Every render node must operate with at most one texture and at most one shader program. If
		an entity requires multiple textures or shader programs to be fully rendered, the entity
		renderer must be built up from multiple render nodes.

		Render nodes may also chain off each other, with transformations in-between if desired. They
		may also be added to the same entity to render independently of each other.
	*/
	class RenderNode : public Node
	{
		friend class Scene;

	public:
		/**
			Inserts the rendering routine into the rendering queue. The render key will be updated
			to contain the proper viewport layer, translucency and material data, such that the
			rendering order is ideal.
			
			Will also update the model matrix in all shader programs to the current transformation
			and bind the active shader program and texture if applicable.
		*/
		virtual void schedule(ScheduleContext & context) const override final;

	private:
		ViewportLayer m_viewportLayer = ViewportLayer::WORLD;
		Translucency m_translucency = Translucency::OPAQUE;

		Asset<Program>::Reference m_program;
		Asset<Texture>::Reference m_texture;

		std::function<void()> m_render;
	};
}