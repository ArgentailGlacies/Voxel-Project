
#include "World.h"

#include "event/EventBus.h"
#include "world/BlockRegistry.h"
#include "world/data/ChunkStorage.h"
#include "world/data/Propagation.h"
#include "world/data/WorldQuery.h"
#include "world/event/ChunkEvents.h"
#include "world/util/Limits.h"

#include <chrono>
#include <glm/gtx/hash.hpp>
#include <mutex>
#include <queue>
#include <shared_mutex>
#include <thread>
#include <unordered_set>

namespace
{
	constexpr auto sleep_duration = std::chrono::milliseconds{ 10 };
}

class vox::World::Impl
{
	friend class World;

public:
	Impl(const BlockRegistry & blocks) noexcept;
	~Impl() noexcept;

	void read(WorldQuery & query) const;
	void write(WorldQuery && query);
	void finish();

	BlockRegion readChunkMeshData(const glm::ivec3 & cpos) const;

private:
	/**
		Checks if all the chunks around the provided position are empty, and that the provided chunk
		is also empty.

		@param cpos The position to check chunks around.
		@return True iff all chunks surrounding the position are empty.
	*/
	bool allChunksEmptyAround(const glm::ivec3 & cpos);
	/**
		Generates chunks surrounding the provided position, where there is a missing chunk.
		Positions already containing a chunk are skipped.

		@param cpos The position to generate chunks around.
	*/
	void generateChunksAround(const glm::ivec3 & cpos);
	/**
		Prepares the lighting within the chunk at the specified position. The chunk is assumed to
		contain no blocks - any blocks in the chunk will be removed. If the chunk is the topmost
		chunk, it is filled with full sunlight. Otherwise, its neighbors will propagate their light.

		@param cpos The chunk position of the chunk to generate light for.
	*/
	void generateChunkLighting(const glm::ivec3 & cpos);

	// ...

	const BlockRegistry & m_blocks;

	ChunkStorage m_chunks;

	// ...

	core::EventBus m_bus;
	core::Listener m_chunkLighten;
	core::Listener m_chunkDarken;

	// ...

	void scheduleWriteTask(WorldQuery && query);
	bool popWriteTask(WorldQuery & query);

	void handleWorker();
	void performWriteTask();
	void performLightTask();
	void performCleanTask();

	void incrementReaderCount() const;
	void decrementReaderCount() const;

	std::queue<WorldQuery> m_tasks;
	std::unordered_set<glm::ivec3> m_chunksToLighten;
	std::unordered_set<glm::ivec3> m_chunksToDarken;
	std::unordered_set<glm::ivec3> m_chunksToClean;

	std::mutex m_taskLock;
	std::mutex m_workLock;
	mutable std::mutex m_readLock;
	mutable std::shared_mutex m_writeLock;
	mutable unsigned int m_readers = 0;

	std::thread m_worker;
	bool m_keepThreadsAlive = true;
};

vox::World::Impl::Impl(const BlockRegistry & blocks) noexcept
	: m_blocks(blocks)
{
	m_worker = std::thread{ &Impl::handleWorker, this };

	m_chunkLighten = m_bus.add<ChunkLighten>([this](auto & event)
	{
		m_chunksToLighten.insert(event.m_cpos);
	});
	m_chunkDarken = m_bus.add<ChunkDarken>([this](auto & event)
	{
		m_chunksToDarken.insert(event.m_cpos);
	});
}
vox::World::Impl::~Impl() noexcept
{
	m_keepThreadsAlive = false;
	if (m_worker.joinable())  m_worker.join();
}

void vox::World::Impl::read(WorldQuery & query) const
{
	incrementReaderCount();
	for (auto & [cpos, data] : query)
	{
		if (m_chunks.has(cpos))
			m_chunks.getExact(cpos)->read(data);
	}
	decrementReaderCount();
}
void vox::World::Impl::write(WorldQuery && query)
{
	scheduleWriteTask(std::move(query));
}
void vox::World::Impl::finish()
{
	while (true)
	{
		std::this_thread::sleep_for(sleep_duration);

		std::lock_guard workGuard{ m_workLock };
		std::lock_guard readGuard{ m_readLock };
		std::lock_guard taskGuard{ m_taskLock };
		if (m_tasks.empty())
			break;
	}
}

vox::BlockRegion vox::World::Impl::readChunkMeshData(const glm::ivec3 & cpos) const
{
	incrementReaderCount();

	static constexpr auto SIZE = CHUNK_SIZE<int>;
	static constexpr auto EDGE = CHUNK_SIZE<int> - 1;

	BlockRegion region{ glm::ivec3{ -1 }, glm::uvec3{ SIZE + 2 } };
	if (const auto chunk = m_chunks.getExact(cpos))
		chunk->read(region, glm::ivec3{ 0, 0, 0 }, glm::ivec3{ 0, 0, 0 }, glm::ivec3{ SIZE, SIZE, SIZE });
	if (const auto chunk = m_chunks.getExact(cpos - glm::ivec3{ 1, 0, 0 }))
		chunk->read(region, glm::ivec3{ EDGE, 0, 0 }, glm::ivec3{ -1, 0, 0 }, glm::ivec3{ 1, SIZE, SIZE });
	if (const auto chunk = m_chunks.getExact(cpos + glm::ivec3{ 1, 0, 0 }))
		chunk->read(region, glm::ivec3{ 0, 0, 0 }, glm::ivec3{ SIZE, 0, 0 }, glm::ivec3{ 1, SIZE, SIZE });
	if (const auto chunk = m_chunks.getExact(cpos - glm::ivec3{ 0, 1, 0 }))
		chunk->read(region, glm::ivec3{ 0, EDGE, 0 }, glm::ivec3{ 0, -1, 0 }, glm::ivec3{ SIZE, 1, SIZE });
	if (const auto chunk = m_chunks.getExact(cpos + glm::ivec3{ 0, 1, 0 }))
		chunk->read(region, glm::ivec3{ 0, 0, 0 }, glm::ivec3{ 0, SIZE, 0 }, glm::ivec3{ SIZE, 1, SIZE });
	if (const auto chunk = m_chunks.getExact(cpos - glm::ivec3{ 0, 0, 1 }))
		chunk->read(region, glm::ivec3{ 0, 0, EDGE }, glm::ivec3{ 0, 0, -1 }, glm::ivec3{ SIZE, SIZE, 1 });
	if (const auto chunk = m_chunks.getExact(cpos + glm::ivec3{ 0, 0, 1 }))
		chunk->read(region, glm::ivec3{ 0, 0, 0 }, glm::ivec3{ 0, 0, SIZE }, glm::ivec3{ SIZE, SIZE, 1 });

	decrementReaderCount();
	return region;
}

bool vox::World::Impl::allChunksEmptyAround(const glm::ivec3 & cpos)
{
	glm::ivec3 pos;
	for (pos.x = cpos.x - 1; pos.x <= cpos.x + 1; ++pos.x)
	for (pos.y = cpos.y - 1; pos.y <= cpos.y + 1; ++pos.y)
	for (pos.z = cpos.z - 1; pos.z <= cpos.z + 1; ++pos.z)
	{
		if (m_chunks.has(pos) && !m_chunks.getExact(pos)->empty())
			return false;
	}
	return true;
}
void vox::World::Impl::generateChunksAround(const glm::ivec3 & cpos)
{
	std::lock_guard guard{ m_writeLock };

	glm::ivec3 pos;
	for (pos.x = cpos.x - 1; pos.x <= cpos.x + 1; ++pos.x)
	for (pos.y = cpos.y - 1; pos.y <= cpos.y + 1; ++pos.y)
	for (pos.z = cpos.z - 1; pos.z <= cpos.z + 1; ++pos.z)
	{
		if (m_chunks.has(pos))
			continue;

		m_chunks.create(pos);
		generateChunkLighting(pos);
		m_bus.post(ChunkCreated{ pos });
	}
}
void vox::World::Impl::generateChunkLighting(const glm::ivec3 & cpos)
{
	if (!m_chunks.has(cpos))
		return;
	auto * chunk = m_chunks.getExact(cpos);
	auto * above = m_chunks.getAbove(cpos);

	if (above == nullptr)
	{
		auto writer = chunk->write();
		for (Index i = 0; i < CHUNK_VOLUME<Index>; ++i)
			writer.writeFast(i, BlockData{ 0, Light{ 0, 0, 0, MAX_BLOCK_LIGHT } });
	}
	else
	{
		glm::uvec3 pos{ 0, 0, 0 };
		for (pos.x = 0; pos.x < CHUNK_SIZE<unsigned int>; ++pos.x)
		for (pos.y = 0; pos.y < CHUNK_SIZE<unsigned int>; ++pos.y)
			above->pushLightPropagation(toIndex(pos), {});
	}
}

void vox::World::Impl::handleWorker()
{
	while (m_keepThreadsAlive)
	{
		std::this_thread::sleep_for(sleep_duration);

		std::lock_guard guard{ m_workLock };
		performWriteTask();
		performLightTask();
		performCleanTask();
	}
}
void vox::World::Impl::performWriteTask()
{
	WorldQuery query;
	while (popWriteTask(query))
	{
		for (const auto & [cpos, data] : query)
		{
			generateChunksAround(cpos);
			m_chunks.getExact(cpos)->write(data);
			m_chunksToLighten.emplace(cpos);
			m_chunksToDarken.emplace(cpos);
			m_chunksToClean.emplace(cpos);
			m_bus.post(ChunkChanged{ cpos });
		}
	}
}
void vox::World::Impl::performLightTask()
{
	while (!m_chunksToDarken.empty())
	{
		std::unordered_set<glm::ivec3> set;
		std::swap(set, m_chunksToDarken);

		for (const auto & cpos : set)
		{
			auto writer = m_chunks.getExact(cpos)->write();
			PropagationContext context{ m_bus, m_blocks, m_chunks, writer };
			PropagationDark{ context, cpos }.spread();
			m_bus.post(ChunkChanged{ cpos });
		}
	}
	m_chunksToDarken.clear();

	while (!m_chunksToLighten.empty())
	{
		std::unordered_set<glm::ivec3> set;
		std::swap(set, m_chunksToLighten);

		for (const auto & cpos : set)
		{
			auto writer = m_chunks.getExact(cpos)->write();
			PropagationContext context{ m_bus, m_blocks, m_chunks, writer };
			PropagationLight{ context, cpos }.spread();
			m_bus.post(ChunkChanged{ cpos });
		}
	}
	m_chunksToLighten.clear();
}
void vox::World::Impl::performCleanTask()
{
	std::lock_guard guard{ m_writeLock };

	for (const auto & it : m_chunksToClean)
	{
		glm::ivec3 pos;
		for (pos.x = it.x - 1; pos.x <= it.x + 1; ++pos.x)
		for (pos.y = it.y - 1; pos.y <= it.y + 1; ++pos.y)
		for (pos.z = it.z - 1; pos.z <= it.z + 1; ++pos.z)
		{
			if (allChunksEmptyAround(pos))
			{
				m_chunks.destroy(pos);
				m_bus.post(ChunkDestroyed{ pos });
			}
		}
	}
	m_chunksToClean.clear();
}

void vox::World::Impl::scheduleWriteTask(WorldQuery && query)
{
	std::lock_guard guard{ m_taskLock };
	m_tasks.emplace(std::move(query));
}
bool vox::World::Impl::popWriteTask(WorldQuery & query)
{
	std::lock_guard guard{ m_taskLock };
	if (m_tasks.empty())
		return false;
	std::swap(query, m_tasks.front());
	m_tasks.pop();
	return true;
}

void vox::World::Impl::incrementReaderCount() const
{
	std::lock_guard guard{ m_readLock };
	if (++m_readers == 1)
		m_writeLock.lock();
}
void vox::World::Impl::decrementReaderCount() const
{
	std::lock_guard guard{ m_readLock };
	if (--m_readers == 0)
		m_writeLock.unlock();
}

// ...

vox::World::World(const BlockRegistry & blocks) : m_impl(std::make_unique<Impl>(blocks)) {}
vox::World::~World() = default;

void vox::World::read(WorldQuery & query) const { m_impl->read(query); }
void vox::World::write(WorldQuery && query) { m_impl->write(std::move(query)); }
void vox::World::finish() { m_impl->finish(); }

vox::BlockRegion vox::World::readChunkMeshData(const glm::ivec3 & cpos) const
{
	return m_impl->readChunkMeshData(cpos);
}

unsigned int vox::World::countChunks() const { return m_impl->m_chunks.count(); }
core::EventBus & vox::World::getEventBus() const { return m_impl->m_bus; }

