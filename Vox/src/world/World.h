#pragma once

#include "event/EventBus.h"
#include "world/Block.h"
#include "world/data/BlockRegion.h"

#include <glm/vec3.hpp>
#include <memory>
#include <string>

namespace vox
{
	class BlockRegistry;
	class WorldQuery;

	class World
	{
	public:
		World() = delete;
		World(const BlockRegistry & blocks);
		World(const World &) = delete;
		World(World &&) = default;
		~World();

		World & operator=(const World &) = delete;
		World & operator=(World &&) = default;

		/**
			Reads the block data and stores it in the query. If a nonexisting chunk is being
			queried, that chunk will be skipped. If a read is performed simultaneously with a write
			operation, and the affected regions overlap, the order of the operations is undefined.
			
			While a read operation is being performed, no chunks will be created or deleted,
			although chunks may be modified. If a chunk changes after the read query has processed
			that chunk, the read values will not be updated to match the actual chunk data.

			The thread performing a read operation will block until the entire read operation is
			completed. Multiple threads may perform a read simultaneously, and will not block each
			other.

			@param query The blocks to read from the world.
		*/
		void read(WorldQuery & query) const;
		/**
			Writes the block data to the world, generating new chunks if required. While chunks are
			being generated, other threads attempting to read or write will be blocked. Once all
			chunks have been generated, the block data will be written asyncronously to the chunks.

			The order in which write operations are performed is the same in which they were
			invoked. If multiple threads attempt to schedule a world modification, the exact order
			in which write operations happen is undefined.

			@param query The blocks which should be written to the world.
		*/
		void write(WorldQuery && query);

		/**
			Blocks the calling thread until all world operations have completed. This may be used to
			guarantee that a write operation is finished before reading from the world. This will
			also wait until all read operations are done.
		*/
		void finish() const;

		// ...

		/**
			Reads the data stored in the provided chunk, padded with one block in all directions.
			The padded data will contain the neighboring chunks' data to simplify chunk meshing
			rountines.

			Functions otherwise similar to the main read method.
		*/
		BlockRegion readChunkMeshData(const glm::ivec3 & cpos) const;

		// ...

		// TODO: Improve method names and document them
		unsigned int countChunks() const;
		core::EventBus & getEventBus() const;

	private:
		class Impl;

		std::unique_ptr<Impl> m_impl;
	};
}