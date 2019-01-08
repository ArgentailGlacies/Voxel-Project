
#include "ChunkData.h"

#include <algorithm>

vox::ChunkDataCompressed vox::ChunkDataFlat::compress() const
{
	ChunkDataCompressed result;
	result.data().clear();

	BlockData current = m_data[0];
	for (Index i = 0; i < CHUNK_VOLUME<Index> - 1; ++i)
	{
		const auto next = m_data[i + 1];
		if (current != next)
		{
			result.data().push_back({ i, current });
			current = next;
		}
	}
	result.data().push_back({ CHUNK_VOLUME<Index> - 1, current });

	return result;
}

// ...

vox::ChunkDataCompressed::ChunkDataCompressed()
{
	m_data.push_back({ CHUNK_VOLUME<Index> - 1, 0 });
}

vox::ChunkDataFlat vox::ChunkDataCompressed::expand() const
{
	ChunkDataFlat result;

	Index index = 0;
	for (Index i = 0; i < CHUNK_VOLUME<Index>; ++i)
	{
		const auto data = m_data[index];
		result.write(i, data.m_data);

		if (i >= data.m_index)
			index++;
	}

	return result;
}

vox::BlockData vox::ChunkDataCompressed::read(Index index) const
{
	const auto it = std::lower_bound(m_data.begin(), m_data.end(), index,
		[](const auto & lhs, const auto & rhs) { return lhs.m_index < rhs; }
	);
	return it == m_data.end() ? BlockData{} : it->m_data;
}
