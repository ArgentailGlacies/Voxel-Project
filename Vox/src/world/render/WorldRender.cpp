
#include "WorldRender.h"

#include "asset/AssetRegistry.h"
#include "core/Resources.h"
#include "event/EventBus.h"
#include "scene/Scene.h"
#include "world/event/ChunkEvents.h"
#include "world/Universe.h"
#include "world/util/Limits.h"
#include "world/World.h"

#include <glm/gtc/matrix_transform.hpp>

vox::WorldRender::WorldRender(const Universe & universe, const World & world, core::Scene & scene)
	: m_mesher(world, universe.getBlockRenderRegistry()), m_scene(scene)
{
	auto & bus = world.getEventBus();
	m_chunkCreated = bus.add<ChunkCreated>([this](auto & event)
	{
		std::lock_guard guard{ m_modelLock };
		m_chunks.reserve(event.m_cpos);
	});
	m_chunkDestroyed = bus.add<ChunkDestroyed>([this](auto & event)
	{
		std::lock_guard guard{ m_modelLock };
		detach(m_chunks.get(event.m_cpos));
		m_chunks.remove(event.m_cpos);
	});
	m_chunkChanged = bus.add<ChunkChanged>([this](auto & event)
	{
		std::lock_guard guard{ m_taskLock };
		m_chunksToMesh.insert(event.m_cpos);
		m_chunksToMesh.insert(event.m_cpos + glm::ivec3{ 1, 0, 0 });
		m_chunksToMesh.insert(event.m_cpos - glm::ivec3{ 1, 0, 0 });
		m_chunksToMesh.insert(event.m_cpos + glm::ivec3{ 0, 1, 0 });
		m_chunksToMesh.insert(event.m_cpos - glm::ivec3{ 0, 1, 0 });
		m_chunksToMesh.insert(event.m_cpos + glm::ivec3{ 0, 0, 1 });
		m_chunksToMesh.insert(event.m_cpos - glm::ivec3{ 0, 0, 1 });
	});

	m_root = m_scene.createNode(core::Scene::DEFAULT_ENTRY);
}

void vox::WorldRender::process()
{
	processTasks();
	processModels();
}
void vox::WorldRender::processModels()
{
	std::lock_guard guard{ m_modelLock };
	ChunkMesher::Result result;
	while (m_mesher.popResult(result))
	{
		detach(m_chunks.get(result.m_cpos));
		m_chunks.set(result.m_cpos, std::move(result.m_model));
		attach(m_chunks.get(result.m_cpos), result.m_cpos);
	}
}
void vox::WorldRender::processTasks()
{
	std::lock_guard guard{ m_taskLock };
	for (const auto & cpos : m_chunksToMesh)
		m_mesher.pushTask(cpos);
	m_chunksToMesh.clear();
}

void vox::WorldRender::attach(ChunkModel * model, const glm::ivec3 & cpos)
{
	if (model == nullptr || model->empty())
		return;
	
	model->m_mesh.build();
	model->m_transform = m_scene.createTransformation(m_root);
	model->m_render = m_scene.createRender(
		core::res::asset::WORLD_SHADER_PROGRAM,
		core::res::asset::BLOCK_TEXTURE_ATLAS,
		[model]() { model->m_mesh.render(); },
		model->m_transform
	);

	const auto transform = glm::translate(glm::mat4{ 1.0f }, glm::vec3{ cpos * CHUNK_SIZE<int> });
	m_scene.setTransformation(model->m_transform, transform);
}
void vox::WorldRender::detach(ChunkModel * model)
{
	if (model == nullptr || model->empty())
		return;

	m_scene.detach(model->m_transform, m_root);
}
