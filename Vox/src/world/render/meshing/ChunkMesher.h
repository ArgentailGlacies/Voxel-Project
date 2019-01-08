#pragma once

#include "world/data/BlockRegion.h"
#include "world/render/ChunkModel.h"

#include <glm/gtx/hash.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <mutex>
#include <thread>
#include <unordered_set>
#include <unordered_map>

namespace vox
{
	class BlockRenderRegistry;
	class World;

	/**
		The chunk mesher is a high-level class designed to synchronize chunk meshing operations.
		When a chunk must be meshed, a task should be dispatched to the mesher, which will then
		delegate a worker thread to finish the task. Finished models must be manually extracted
		from the mesher.
	*/
	class ChunkMesher
	{
	public:
		/**
			The result after a chunk meshing operation. The model will be ready for rendering
			immediately.
		*/
		struct Result
		{
			std::unique_ptr<ChunkModel> m_model;
			glm::ivec3 m_cpos;
		};

		// ...

		ChunkMesher() = delete;
		ChunkMesher(const World & world, const BlockRenderRegistry & blocks) noexcept;
		ChunkMesher(const ChunkMesher &) = delete;
		ChunkMesher(ChunkMesher &&) = delete;
		~ChunkMesher() noexcept;

		ChunkMesher & operator=(const ChunkMesher &) = delete;
		ChunkMesher & operator=(ChunkMesher &&) = delete;

		/**
			Blocks the calling thread until all chunk tasks are finished.
		*/
		void finish() const;

		/**
			@return The number of tasks that remains to be completed.
		*/
		inline auto size() const { return m_remainingTasks; }

		/**
			Provides the chunk mesher with a meshing task. The chunk will be meshed in a different
			thread. Once the task is finished, the final mesh will be available as a result.

			@param cpos The chunk position of the chunk that has to be meshed.
		*/
		void pushTask(const glm::ivec3 & cpos);
		/**
			Removes a result from the result list, if there are any available. The removed result is
			stored in the provided result.

			@param result [out] The result to store the popped result in.
			@return True iff there were any results removed from the result list.
		*/
		bool popResult(Result & result);

	private:
		void handleWorker();

		bool popTask(Result & task);
		void pushResult(Result && result);

		const World & m_world;
		const BlockRenderRegistry & m_blocks;

		std::unordered_map<glm::ivec3, std::unique_ptr<ChunkModel>> m_tasks;
		std::unordered_map<glm::ivec3, std::unique_ptr<ChunkModel>> m_results;

		std::thread m_workers[3];
		mutable std::mutex m_mutex;

		unsigned int m_remainingTasks = 0;
		bool m_keepThreadsAlive = true;
	};
}