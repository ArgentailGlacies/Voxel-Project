
#include "SceneNode.h"

#include "core/Resources.h"
#include "opengl/UBORegistry.h"
#include "scene/Camera.h"
#include "scene/ScheduleContext.h"
#include "util/MathOperations.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

void core::Node::schedule(ScheduleContext & context) const
{
	for (auto * child : m_children)
		child->schedule(context);
}

void core::Node::attach(Node & child)
{
	child.m_parents.insert(this);
	m_children.insert(&child);
}
void core::Node::detach(Node & child)
{
	child.m_parents.erase(this);
	m_children.erase(&child);
}

// ...

void core::CameraNode::schedule(ScheduleContext & context) const
{
	// At this point, all information about 
	context.key.setViewport(m_id);
	context.key.setTranslucency(Translucency::OPAQUE);
	context.key.setViewportLayer(ViewportLayer::SKYBOX);
	context.key.setDepth(0);
	context.key.setProgram(0);
	context.key.setTexture(0);

	context.queue.add(context.key, [&]()
	{
		const auto projection = m_camera.calculateProjectionMatrix(context.partialFrame);
		const auto view = m_camera.calculateViewMatrix(context.partialFrame);

		context.ubos.set(res::ubo::MATRICES, res::ubo::PROJECTION, projection);
		context.ubos.set(res::ubo::MATRICES, res::ubo::VIEW, view);
	});

	context.camera = &m_camera;
	Node::schedule(context);
}

// ...

void core::TransformNode::schedule(ScheduleContext & context) const
{
	context.stack.push(m_transformation);
	update(context);

	Node::schedule(context);

	context.stack.pop();
	update(context);
}

void core::TransformNode::update(ScheduleContext & context) const
{
	// Must decompose the model-to-world matrix to be able to compute distance to current camera.
	glm::vec3 translation;
	glm::quat rotation;
	glm::vec3 scale;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(context.stack.top(), scale, rotation, translation, skew, perspective);

	// Distance to camera is difference in world position of model and camera. Depth must not be
	// zero and is thus offset by one, to ensure any system commands of depth zero are done first.
	const auto delta = translation - context.camera->getPosition();
	const auto distance = util::sqrt(glm::dot(delta, delta));
	context.key.setDepth(1 + static_cast<unsigned int>(distance));
}

// ...

void core::RenderNode::schedule(ScheduleContext & context) const
{
	// By this point, transformation nodes have calculated the depth value. Rendering nodes must
	// ensure that their viewport, translucency and material values are properly set.
	context.key.setViewportLayer(m_viewportLayer);
	context.key.setTranslucency(m_translucency);
	context.key.setProgram(m_program.empty() ? 0 : m_program->handle());
	context.key.setTexture(m_texture.empty() ? 0 : m_texture->handle());

	context.queue.add(context.key, [&, transformation = context.stack.top()]()
	{
		context.ubos.set(res::ubo::MATRICES, res::ubo::MODEL, transformation);

		if (!m_program.empty()) m_program->bind();
		if (!m_texture.empty()) m_texture->bind();
		if (m_render) m_render();
	});

	Node::schedule(context);
}
