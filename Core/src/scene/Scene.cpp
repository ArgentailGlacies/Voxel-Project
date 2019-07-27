
#include "Scene.h"

#include "asset/AssetRegistry.h"
#include "scene/ScheduleContext.h"
#include "ui/Display.h"

#include "opengl/VAO.h"
#include "opengl/VBO.h"
#include "opengl/OpenGL.h"
#include "opengl/Program.h"
#include <allegro5/allegro.h>

namespace
{
	core::RenderKey key2D()
	{
		core::RenderKey key;
		key.setFullscreenLayer(core::FullscreenLayer::GUI);
		return key;
	}
	core::RenderKey key3D()
	{
		core::RenderKey key;
		return key;
	}

	/**
		Prepares for rendering 2D objects.
	*/
	void begin2D()
	{
		core::Program::reset();
		core::VAO::reset();
		core::VBO::reset(core::BufferType::ARRAY_BUFFER);

		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
	}
	/**
		Prepares for rendering 3D objects.
	*/
	void begin3D()
	{
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
	}
}

core::Scene::Scene(const AssetRegistry & assets, const Display & display, const UBORegistry & ubos)
	: m_assets(assets), m_display(display), m_ubos(ubos)
{
	m_nodes.emplace_back(std::make_unique<Node>()); // Root node, cameras are attched to it
	m_nodes.emplace_back(std::make_unique<Node>()); // Base node, it is attached to all cameras
	createCamera();
}
core::Scene::~Scene() = default;

void core::Scene::process()
{
	for (auto & camera : m_cameras)
		camera->process();
}
void core::Scene::render(float pf) const
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ScheduleContext context{ m_ubos, pf };
	m_nodes[ROOT_ENTRY]->schedule(context);
	context.queue.add(key2D(), begin2D);
	context.queue.add(key3D(), begin3D);
	context.queue.sort();
	context.queue.render();

	al_flip_display();
}

bool core::Scene::has(SceneEntry entry) const
{
	return entry < m_nodes.size() && m_nodes[entry] != nullptr;
}

void core::Scene::attach(SceneEntry source, SceneEntry target)
{
	if (!has(source) || !has(target))
		return;

	m_nodes[target]->attach(*m_nodes[source]);
}
void core::Scene::detach(SceneEntry source, SceneEntry target)
{
	if (!has(source) || !has(target) || source == DEFAULT_CAMERA || source == DEFAULT_ENTRY)
		return;

	auto & child = *m_nodes[source];
	auto & parent = *m_nodes[target];

	parent.detach(child);
	if (child.m_parents.empty())
	{
		// Cannot remove directly from set of children; the set would be modified during traversal
		std::unordered_set<Node *> detachables;
		for (auto * subchild : child.m_children)
		{
			if (subchild->m_parents.size() == 1)
				detachables.insert(subchild);
		}
		// Children are removed from the list here
		for (auto * subchild : detachables)
			detach(subchild->m_entry, source);

		m_nodes[source] = nullptr;
	}
}

core::SceneEntry core::Scene::createNode(SceneEntry parent)
{
	const auto entry = m_nodes.size();
	auto & node = m_nodes.emplace_back(std::make_unique<Node>());
	node->m_entry = entry;

	attach(entry, parent);
	return entry;
}

core::SceneEntry core::Scene::createCamera()
{
	const auto entry = m_nodes.size();
	const auto & camera = m_cameras.emplace_back(std::make_unique<Camera>(m_display));
	auto & node = m_nodes.emplace_back(std::make_unique<CameraNode>(*camera, m_cameras.size() - 1));
	node->m_entry = entry;

	// Attach camera to root node, and base node to camera
	attach(entry, ROOT_ENTRY);
	attach(DEFAULT_ENTRY, entry);
	return entry;
}
void core::Scene::destroyCamera(SceneEntry entry)
{
	m_cameras[getNode<CameraNode>(entry).m_id] = nullptr;
	detach(entry, ROOT_ENTRY);
}
core::Camera & core::Scene::getCamera(SceneEntry entry) const
{
	return *m_cameras[getNode<CameraNode>(entry).m_id];
}

core::SceneEntry core::Scene::createTransformation(SceneEntry parent)
{
	const auto entry = m_nodes.size();
	auto & node = m_nodes.emplace_back(std::make_unique<TransformNode>());
	node->m_entry = entry;

	attach(entry, parent);
	return entry;
}
void core::Scene::setTransformation(SceneEntry entry, const glm::mat4 & transformation)
{
	getNode<TransformNode>(entry).m_transformation = transformation;
}

core::SceneEntry core::Scene::createRender(
	const std::string & program,
	const std::string & texture,
	std::function<void()> && renderer,
	SceneEntry parent
)
{
	const auto entry = m_nodes.size();
	auto & node = m_nodes.emplace_back(std::make_unique<RenderNode>());
	node->m_entry = entry;

	auto & render = static_cast<RenderNode &>(*node);
	render.m_program = m_assets.get<Program>(program);
	render.m_texture = m_assets.get<Texture>(texture);
	render.m_render = renderer;

	attach(entry, parent);
	return entry;
}
void core::Scene::setTranslucency(SceneEntry entry, Translucency translucency)
{
	getNode<RenderNode>(entry).m_translucency = translucency;
}
void core::Scene::setViewportLayer(SceneEntry entry, ViewportLayer layer)
{
	getNode<RenderNode>(entry).m_viewportLayer = layer;
}
void core::Scene::setFullscreenLayer(SceneEntry entry, FullscreenLayer layer)
{
	getNode<RenderNode>(entry).m_fullscreenLayer = layer;
}
