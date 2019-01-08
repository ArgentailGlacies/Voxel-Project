#pragma once

#include "world/data/BlockData.h"
#include "world/util/Index.h"
#include "world/util/Limits.h"

#include <array>
#include <vector>

namespace vox
{
	class ChunkDataFlat;
	class ChunkDataCompressed;

	class ChunkDataFlat
	{
	public:
		/**
			Compresses the chunk data into a more compact representation. The chunk is compressed
			using a run-length encoding algoritm over the raw array data.

			@return A compressed representation of the data.
		*/
		ChunkDataCompressed compress() const;

		/**
			Reads the block at the specified index. The caller is responsible for ensuring that the
			provided index is valid.

			@param index The index to read the data from.
			@return The block data stored at the specified index.
		*/
		inline auto read(Index index) const { return m_data[index]; }

		/**
			Writes the given block data to the specified index. The caller is responsible for
			ensuring that the provided index is valid.

			@param index The index to store the data at.
			@oaram data The data to write to the specified index.
		*/
		inline void write(Index index, BlockData data) { m_data[index] = data; }

	private:
		std::array<BlockData, CHUNK_VOLUME<int>> m_data;
	};

	class ChunkDataCompressed
	{
	public:
		/**
			Represents one entry in the compressed chunk data. The index represents an index in a
			flat array, where the last block of with the exact data is. Suppose that a flat array
			containing the data { 38, 38, 38, 38, 38, 19, 80 } is compressed. The compressed data
			would then be represented as { { 4, 38 }, { 5, 19 }, { 6, 80 } }.
		*/
		struct Data
		{
			Index m_index;
			BlockData m_data;
		};

		ChunkDataCompressed();

		/**
			Expands the chunk data to a format which can easily be edited. The expanded format is a
			flat array.

			@return A flat array representation of the data.
		*/
		ChunkDataFlat expand() const;

		/**
			Reads the block at the specified index. The caller is responsible for ensuring that the
			provided index is valid.

			@param index The index to read the data from.
			@return The block data stored at the specified index.
		*/
		BlockData read(Index index) const;

		/**
			Retrieves a raw handle to the internal data of the chunk. This is not something which
			should normally be accessed. It is used for compression of chunks, as well as writing
			and reading data to and from disk.

			@return The raw data handle to the chunk.
		*/
		inline auto & data() { return m_data; }
		/**
			Retrieves a raw handle to the internal data of the chunk. This is not something which
			should normally be accessed. It is used for compression of chunks, as well as writing
			and reading data to and from disk.

			@return The raw data handle to the chunk.
		*/
		inline const auto & data() const { return m_data; }

	private:
		std::vector<Data> m_data;
	};
}