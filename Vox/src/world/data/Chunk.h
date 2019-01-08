#pragma once

#include "world/data/ChunkData.h"
#include "world/util/Index.h"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <mutex>
#include <queue>
#include <unordered_map>
#include <shared_mutex>

namespace vox
{
	class BlockRegion;
	class ChunkQuery;

	class Chunk
	{
	public:
		/**
			Any number of threads may read concurrently from a chunk, provided there are no threads
			that are currently writing to it. When a thread is given a Reader object, the thread can
			be guaranteed that the chunk will remain unchanged for the duration of the reading.
		*/
		class Reader
		{
		public:
			Reader() = delete;
			Reader(const Chunk & chunk) noexcept;
			Reader(const Reader &) = delete;
			Reader(Reader &&) noexcept = default;
			~Reader() noexcept;

			Reader & operator=(const Reader &) = delete;
			Reader & operator=(Reader &&) = delete;

			/**
				Performs a quick read in the data associated with the reader. It is the caller's
				responsibility to ensure that the provided index is valid.

				@param index The index to read from in the chunk data.
				@return The block data stored at the specified index.
			*/
			inline auto read(Index index) const { return m_data.read(index); }
			/**
				Performs a quick read in the data associated with the reader. It is the caller's
				responsibility to ensure that the provided position is valid. The position is
				specified relative to the chunk position.

				@param pos The position to read from in the chunk data.
				@return The block data stored at the specified position.
			*/
			inline auto read(const glm::uvec3 & pos) const { return m_data.read(toIndex(pos)); }

		private:
			const Chunk & m_chunk;
			const ChunkDataCompressed & m_data;
		};

		/**
			Only a single thread may write to a chunk at any given time. When a thread is given a
			Writer object, the thread is guaranteed to be the only thread accessing the chunk data.
		*/
		class Writer
		{
		public:
			Writer() = delete;
			Writer(Chunk & chunk) noexcept;
			Writer(const Writer &) = delete;
			Writer(Writer &&) noexcept = default;
			~Writer() noexcept;

			Writer & operator=(const Writer &) = delete;
			Writer & operator=(Writer &&) = delete;

			/**
				Retrieves the chunk associated with the writer.

				@return The chunk which is being modified by this writer.
			*/
			inline auto & chunk() const { return m_chunk; }

			/**
				Performs a quick read in the data associated with the writer. It is the caller's
				responsibility to ensure that the provided index is valid.

				@param index The index to read from in the chunk data.
				@return The block data stored at the specified index.
			*/
			inline auto read(Index index) const { return m_data.read(index); }
			/**
				Performs a quick read in the data associated with the writer. It is the caller's
				responsibility to ensure that the provided position is valid. The position is
				specified relative to the chunk position.

				@param pos The position to read from in the chunk data.
				@return The block data stored at the specified position.
			*/
			inline auto read(const glm::uvec3 & pos) const { return m_data.read(toIndex(pos)); }

			/**
				Performs a quick write in the data associated with the writer. It is the caller's
				responsibility to ensure that the provided index is valid. This will not handle
				light values automatically, only use this when you know what you are doing!

				@param index The index to write to in the chunk data.
				@param data The block data to store at the specified index.
			*/
			inline void writeFast(Index index, BlockData data) { m_data.write(index, data); }
			/**
				Performs a write in the data associated with the writer. It is the caller's
				responsibility to ensure that the provided index is valid. Any light values will be
				properly handled internally by the chunk.

				@param index The index to write to in the chunk data.
				@param data The block data to store at the specified index.
			*/
			void write(Index index, BlockData data);
			/**
				Performs a quick write in the data associated with the writer. It is the caller's
				responsibility to ensure that the provided position is valid. The position is
				specified relative to the chunk position. Any light values will be properly handled
				internally by the chunk.

				@param pos The position to write to in the chunk data.
				@param data The block data to store at the specified index.
			*/
			inline void write(const glm::uvec3 & pos, BlockData data) { write(toIndex(pos), data); }

		private:
			Chunk & m_chunk;
			ChunkDataFlat m_data;
		};

		// ...

		Chunk() = delete;
		Chunk(const glm::ivec3 & cpos) : m_cpos(cpos) {}
		Chunk(const Chunk &) = delete;
		Chunk(Chunk &&) noexcept = default;
		~Chunk() noexcept = default;

		Chunk & operator=(const Chunk &) = delete;
		Chunk & operator=(Chunk &&) = delete;

		/**
			Retrieves the position of the chunk, in chunk coordinates.

			@return The chunk coordinate for this chunk.
		*/
		inline auto getPosition() const { return m_cpos; }
		/**
			Checks whether the chunk contains any blocks or not. Only bloks with id other than 0 are
			considered blocks in this context.

			@return True if there are any blocks with id other than 0 in the chunk.
		*/
		bool empty() const;

		/**
			Provides read access to the chunks data. The reader must not be stored for longer than
			absolutely neccessary, since the chunk cannot be modified while it is being read.
			Readers are being prioritized, any number of threads may read from a chunk at a time,
			although only one thread may write to it a time. Only after the last reader is removed
			will writing be allowed again.

			@return A chunk reader instance.
		*/
		inline Reader read() const { return *this; }
		/**
			Reads the provided query from the chunk. The chunk will only be read from if no other
			thread is writing to it.

			@param query The query which should be read from the chunk.
		*/
		void read(ChunkQuery & query) const;
		/**
			Reads the provided region from the chunk. The chunk will only be read from if no other
			thread is writing to it.

			@param query The region which should be read from the chunk.
		*/
		void read(BlockRegion & region,
			const glm::ivec3 & source,
			const glm::ivec3 & target,
			const glm::ivec3 & size
		) const;

		/**
			Provides read and write access to the chunks data. The writer must not be stored for
			longer than absolutely neccessary, since the chunk cannot be read from or written to by
			other threads while another writer exists.

			@return A chunk writer instance.
		*/
		inline Writer write() { return *this; }
		/**
			Writes the provided query into the chunk. The chunk will only be modified if no other
			thread is reading from or writing to it.

			@param query The query which should be written to the chunk.
		*/
		void write(const ChunkQuery & query);

		/**
			Marks the specified block for light calculations. If no light is specified, the light
			which is already stored at the block will be propagated out.

			@param index The index of the block to mark for propagation.
			@param data The light data to propagate, or none to use the data in the block.
		*/
		void pushLightPropagation(Index index, const Light & data);
		/**
			Marks the specified block for shadow calculations. The specified data will be removed
			from adjacent blocks if the light levels are nonzero and less than the specified amount.
			If the shadow is propagating over chunk borders, edgecase must be set to true.

			@param index The index of the block to mark for propagation.
			@param data The shadow data to propagate.
			@param edgecase Whether the propagation is across a chunk border or not.
		*/
		void pushDarkPropagation(Index index, const Light & data, bool edgecase);
		/**
			Retrieves a light propagation node if there are any available.

			@param index [out] The index of the node.
			@param data [out] The light data to propagate.
			@return True iff any valid nodes were available.
		*/
		bool popLightPropagation(Index & index, Light & data);
		/**
			Retrieves a shadow propagation node if there are any available.

			@param index [out] The index of the node.
			@param data [out] The light data to propagate.
			@param edgecase [out] Whether the propagation is across a chunk border or not.
			@return True iff any valid nodes were available.
		*/
		bool popDarkPropagation(Index & index, Light & data, bool & edgecase);

	private:
		ChunkDataCompressed m_data;
		glm::ivec3 m_cpos;

		using LightNode = std::pair<Index, Light>;
		using DarkNode = std::tuple<Index, Light, bool>;
		std::queue<LightNode> m_lightPropagation;
		std::queue<DarkNode> m_darkPropagation;

		mutable std::mutex m_lightLock;
		mutable std::mutex m_readLock;
		mutable std::shared_mutex m_writeLock;

		mutable unsigned int m_readers = 0;
	};
}