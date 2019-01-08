#pragma once

#include "scene/Camera.h"
#include "scene/SceneNode.h"

#include <glm/mat4x4.hpp>
#include <functional>
#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace core
{
	class AssetRegistry;
	class Camera;
	class Display;
	class UBORegistry;
	struct ScheduleContext;

	/**
		A scene is a collection of objects which determines how the game data should be rendered.
		The scene consists of objects such as cameras, transformations, renderables and similar. All
		objects are collected and sorted, then rendered in an optimal order.
	*/
	class Scene
	{
	public:
		Scene() = delete;
		Scene(const AssetRegistry & assets, const Display & display, const UBORegistry & ubos);
		Scene(const Scene &) = delete;
		Scene(Scene &&) = delete;
		~Scene();

		Scene & operator=(const Scene &) = delete;
		Scene & operator=(Scene &&) = delete;

		/**
			The scene will always contain a default camera with this identifier. The default camera
			cannot be deleted.
		*/
		static constexpr SceneEntry DEFAULT_CAMERA = 2;
		/**
			The scene will always contain a default node with this identifier. All other entries
			which should be visible from all cameras should be attached to this entry.
		*/
		static constexpr SceneEntry DEFAULT_ENTRY = 1;

		/**
			Performs one update tick on the scene, processing everything which needs to be ticked
			in a logic update.
		*/
		void process();
		/**
			The entire scene graph will be traversed to fill a render queue. All tasks in the queue
			will be performed in a specific order, such that the final result is as ideal as
			possible.

			@param pf The partial frame factor.
		*/
		void render(float pf) const;

		// ...

		/**
			Checks whether the entry exists in the scene or not.

			@param entry The node to check if exists in the scene.
			@return True iff the entry is in the scene and is valid.
		*/
		bool has(SceneEntry entry) const;

		/**
			Attaches one node to another one in the scene. Nodes are queued into the rendering
			queue as many times as the nodes appear in the scene graph. By attaching a node to
			multiple nodes, it may be rendered multiple times in a single frame.

			@param source The node which should be attached to another node.
			@param target The node the source node should be attached to.
		*/
		void attach(SceneEntry source, SceneEntry target);
		/**
			Detaches the node from the target node. If the source node is attached to no other
			nodes, it will be erased from the scene. All its child nodes will be detached first, and
			also deleted if they are not references by other nodes.

			@param source The node which should be detached from the scene.
			@param target The node the source node should be detached from.
		*/
		void detach(SceneEntry source, SceneEntry target);

		// ...

		/**
			Creates a new plain node with no special abilities. It can have children attached to
			itself, allowing more complicated node hiearchies.

			@param parent The node the new node is attached to.
			@return The entry representing the newly created node.
		*/
		SceneEntry createNode(SceneEntry parent);

		/**
			Creates a new camera. The camera will be attached automatically to the proper nodes in
			the scene, the users must never attach the camera to any other node. Other nodes may be
			attached to the camera, however, if that node should be visible to that camera.

			@return The entry representing the newly created camera node.
		*/
		SceneEntry createCamera();
		/**
			Destroys the camera stored in the given entry. All nodes referencing the camera will be
			detached before it is removed. Any dangling nodes will be erased. The default camera
			cannot be destroyed.

			@param entry The camera node to destroy.
		*/
		void destroyCamera(SceneEntry entry);
		/**
			Retrieves the camera for the given entry, if it exists. If there is no camera associated
			with the given entry, the exception std::invalid_argument is thrown.

			@param entry The node containing a camera to get.
			@return The camera associated with the given entry.
		*/
		Camera & getCamera(SceneEntry entry) const;

		/**
			Creates a new transformation node. The node must be manually attached to the scene. All
			nodes attaches under the transformation will be transformed by it. The transformation
			will be attached to the specified parent node.

			@param parent The node the transformation is attached to.
			@return The entry representing the newly created transformation node.
		*/
		SceneEntry createTransformation(SceneEntry parent);
		/**
			Changes the transformation of the specified entry to the provided transformation. If the
			node is not a transformation, this method does nothing.

			@param entry The node which should be updated.
			@param transformation The new transformation to store in the node.
		*/
		void setTransformation(SceneEntry entry, const glm::mat4 & transformation);

		/**
			Creates a new rendering routine. The node must be attached to the scene in order for
			anything to be rendered. Multiple nodes may be chained together to fully render an
			object consisting of multiple parts. The renderer will be attached to the specified
			parent node.

			@param program The name of the shader program used to run the renderer.
			@param texture The name of the texture to use when running the renderer.
			@param renderer The actual rendering routine.
			@param parent The node the renderer is attached to.
			@return The entry representing the newly render node.
		*/
		SceneEntry createRender(
			const std::string & program,
			const std::string & texture,
			std::function<void()> && renderer,
			SceneEntry parent
		);
		/**
			Changes the translucency of the specified entry to the provided rendering node. If the
			node is not a renderer, this method does nothing.

			@param entry The node which should be updated.
			@param translucency The new tranclucency of the node.
		*/
		void setTranslucency(SceneEntry entry, Translucency translucency);
		/**
			Changes the viewport layer of the specified entry to the provided rendering node. If the
			node is not a renderer, this method does nothing.

			@param entry The node which should be updated.
			@param layer The new viewport layer of the node.
		*/
		void setViewportLayer(SceneEntry entry, ViewportLayer layer);

	private:
		template<typename Type>
		Type & getNode(SceneEntry entry) const;

		static constexpr SceneEntry ROOT_ENTRY = 0;

		const AssetRegistry & m_assets;
		const Display & m_display;
		const UBORegistry & m_ubos;

		std::vector<std::unique_ptr<Camera>> m_cameras;
		std::vector<std::unique_ptr<Node>> m_nodes;

		// TODO: Recycle released ids when possible
	};
}

namespace core
{
	template<typename Type>
	inline Type & Scene::getNode(SceneEntry entry) const
	{
		if (!has(entry))
			throw std::invalid_argument("Entry " + std::to_string(entry) + " does not exist");
		auto * node = dynamic_cast<Type *>(m_nodes[entry].get());
		if (node == nullptr)
			throw std::invalid_argument("Entry " + std::to_string(entry) + " is has wrong type");
		return *node;
	}
}