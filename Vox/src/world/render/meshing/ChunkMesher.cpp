
#include "ChunkMesher.h"

#include "opengl/Attribute.h"
#include "world/render/meshing/NaiveGreedyMesher.h"
#include "world/World.h"

namespace
{
	constexpr auto sleep_duration = std::chrono::milliseconds{ 10 };
}

vox::ChunkMesher::ChunkMesher(const World & world, const BlockRenderRegistry & blocks) noexcept
	: m_world(world), m_blocks(blocks)
{
	for (auto & worker : m_workers)
		worker = std::thread{ &ChunkMesher::handleWorker, this };
}
vox::ChunkMesher::~ChunkMesher() noexcept
{
	m_keepThreadsAlive = false;

	for (auto & worker : m_workers)
	{
		if (worker.joinable())
			worker.join();
	}
}

void vox::ChunkMesher::finish() const
{
	while (true)
	{
		std::this_thread::sleep_for(sleep_duration);

		std::lock_guard guard{ m_mutex };
		if (m_remainingTasks == 0)
			break;
	}
}
void vox::ChunkMesher::handleWorker()
{
	while (m_keepThreadsAlive)
	{
		Result task;
		while (popTask(task))
		{
			const auto region = m_world.readChunkMeshData(task.m_cpos);
			NaiveGreedyMesher{ m_blocks, region, *task.m_model }.mesh();
			pushResult(std::move(task));
		}
		std::this_thread::sleep_for(sleep_duration);
	}
}

void vox::ChunkMesher::pushTask(const glm::ivec3 & cpos)
{
	std::lock_guard guard{ m_mutex };
	m_remainingTasks++;
	m_tasks.emplace(cpos, std::make_unique<ChunkModel>());
}
bool vox::ChunkMesher::popTask(Result & task)
{
	std::lock_guard guard{ m_mutex };
	if (m_tasks.empty())
		return false;
	const auto it = m_tasks.begin();
	task.m_cpos = it->first;
	task.m_model = std::move(it->second);
	m_tasks.erase(it);
	return true;
}
void vox::ChunkMesher::pushResult(Result && result)
{
	std::lock_guard guard{ m_mutex };
	m_results.emplace(result.m_cpos, std::move(result.m_model));
	m_remainingTasks--;
}
bool vox::ChunkMesher::popResult(Result & result)
{
	std::lock_guard guard{ m_mutex };
	if (m_results.empty())
		return false;
	const auto it = m_results.begin();
	result.m_cpos = it->first;
	std::swap(it->second, result.m_model);
	m_results.erase(it->first);
	return true;
}
