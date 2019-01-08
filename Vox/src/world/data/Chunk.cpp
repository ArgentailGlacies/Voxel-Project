
#include "Chunk.h"

#include "world/data/BlockRegion.h"
#include "world/data/ChunkQuery.h"

vox::Chunk::Reader::Reader(const Chunk & chunk) noexcept
	: m_chunk(chunk), m_data(chunk.m_data)
{
	std::lock_guard guard{ m_chunk.m_readLock };
	if (++m_chunk.m_readers == 1)
		m_chunk.m_writeLock.lock();
}
vox::Chunk::Reader::~Reader() noexcept
{
	std::lock_guard guard{ m_chunk.m_readLock };
	if (--m_chunk.m_readers == 0)
		m_chunk.m_writeLock.unlock();
}

vox::Chunk::Writer::Writer(Chunk & chunk) noexcept
	: m_chunk(chunk)
{
	m_chunk.m_writeLock.lock();
	m_data = chunk.m_data.expand();
}
vox::Chunk::Writer::~Writer() noexcept
{
	m_chunk.m_data = m_data.compress();
	m_chunk.m_writeLock.unlock();
}

void vox::Chunk::Writer::write(Index index, BlockData data)
{
	const auto oldData = m_data.read(index);
	if (oldData.hasLighting())
		m_chunk.pushDarkPropagation(index, oldData.getLight(), false);
	if (data.hasLighting())
		m_chunk.pushLightPropagation(index, {});

	m_data.write(index, data);
}

// ...

bool vox::Chunk::empty() const
{
	const auto reader = read();
	const auto & data = m_data.data();
	for (const auto & data : m_data.data())
	{
		if (data.m_data.getType() != 0)
			return false;
	}
	return true;
}

void vox::Chunk::read(ChunkQuery & query) const
{
	const auto reader = read();
	for (auto & it : query)
		it.m_data = reader.read(it.m_index);
}
void vox::Chunk::read(BlockRegion & region,
	const glm::ivec3 & source,
	const glm::ivec3 & target,
	const glm::ivec3 & size) const
{
	const auto reader = read();
	
	// If the volume is exactly one chunk with no offset, a faster method may be used
	if (source == glm::ivec3{} && target == glm::ivec3{} && size == glm::ivec3{ CHUNK_SIZE<int> })
	{
		const auto & data = m_data.data();
		unsigned int current = 0;
		unsigned int index = 0;
			
		glm::ivec3 pos;
		for (pos.z = 0; pos.z < CHUNK_SIZE<int>; ++pos.z)
		for (pos.y = 0; pos.y < CHUNK_SIZE<int>; ++pos.y)
		for (pos.x = 0; pos.x < CHUNK_SIZE<int>; ++pos.x)
		{
			const auto & entry = data[current];
			region.write(pos, entry.m_data);

			if (++index > entry.m_index)
				++current;
		}
	}
	else
	{
		glm::ivec3 pos;
		for (pos.z = 0; pos.z < size.z; ++pos.z)
		for (pos.y = 0; pos.y < size.y; ++pos.y)
		for (pos.x = 0; pos.x < size.x; ++pos.x)
			region.write(pos + target, reader.read(pos + source));
	}
}

void vox::Chunk::write(const ChunkQuery & query)
{
	auto writer = write();
	for (auto & it : query)
		writer.write(it.m_index, it.m_data);
}

void vox::Chunk::pushLightPropagation(Index index, const Light & data)
{
	std::lock_guard guard{ m_lightLock };
	m_lightPropagation.emplace(index, data);
}
void vox::Chunk::pushDarkPropagation(Index index, const Light & data, bool edgecase)
{
	std::lock_guard guard{ m_lightLock };
	m_darkPropagation.emplace(index, data, edgecase);
}
bool vox::Chunk::popLightPropagation(Index & index, Light & data)
{
	std::lock_guard guard{ m_lightLock };
	if (m_lightPropagation.empty())
		return false;

	const auto & node = m_lightPropagation.front();
	index = node.first;
	data = node.second;
	m_lightPropagation.pop();

	if (m_lightPropagation.empty())
		std::queue<LightNode>{}.swap(m_lightPropagation);
	return true;
}
bool vox::Chunk::popDarkPropagation(Index & index, Light & data, bool & edgecase)
{
	std::lock_guard guard{ m_lightLock };
	if (m_darkPropagation.empty())
		return false;

	const auto & node = m_darkPropagation.front();
	index = std::get<0>(node);
	data = std::get<1>(node);
	edgecase = std::get<2>(node);
	m_darkPropagation.pop();

	if (m_darkPropagation.empty())
		std::queue<DarkNode>{}.swap(m_darkPropagation);
	return true;
}
