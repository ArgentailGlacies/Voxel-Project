#pragma once

#include "scene/SceneNode.h"

#include <memory>
#include <vector>

namespace core
{
	class RenderComponent : public TransformNode
	{
	public:
		RenderComponent() = default;
		RenderComponent(const RenderComponent &) = delete;
		RenderComponent(RenderComponent &&) = default;
		~RenderComponent() = default;

		RenderComponent & operator=(const RenderComponent &) = delete;
		RenderComponent & operator=(RenderComponent &&) = default;

		/**
			Creates a new node of the specified type. The only nodes which should be connected to
			this node are those created in this manner. The render component must own all nodes
			attached to it.

			@param NodeType The type of the new node to create.
			@param params The parameters which are used to construct the given node.
			@return The node which was just created.
		*/
		template<typename NodeType, typename ...Params>
		NodeType & create(const Params & ...params);

		/**
			Creates and attaches a new node of the specified type. The render component will still
			own the newly created node.

			@param NodeType The type of the new node to attach to this as a child.
			@param params The parameters which are used to construct the given node.
			@return The node which was just created.
		*/
		template<typename NodeType, typename ...Params>
		NodeType & attach(const Params & ...params);

	private:
		std::vector<std::unique_ptr<RenderNode>> m_nodes;
	};
}

namespace core
{
	template<typename NodeType, typename ...Params>
	inline NodeType & RenderComponent::create(const Params & ...params)
	{
		m_nodes.push_back(std::make_unique<NodeType>(params...));
		return *static_cast<NodeType*>(m_nodes.back().get());
	}
	template<typename NodeType, typename ...Params>
	inline NodeType & RenderComponent::attach(const Params & ...params)
	{
		auto & node = create<NodeType>(params...);
		Node::attach(node);
		return node;
	}
}