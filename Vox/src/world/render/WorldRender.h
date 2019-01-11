#pragma once

#include "event/EventListener.h"
#include "world/render/ChunkRenderStorage.h"
#include "world/render/meshing/ChunkMesher.h"

#include <mutex>
#include <unordered_set>

namespace core
{
	class Scene;
}

namespace vox
{
	class Universe;
	class World;

	class WorldRender
	{
	public:
		WorldRender() = delete;
		WorldRender(const Universe & universe, const World & world, core::Scene & scene);
		WorldRender(const WorldRender &) = delete;
		WorldRender(WorldRender &&) = delete;
		~WorldRender() = default;

		WorldRender & operator=(const WorldRender &) = delete;
		WorldRender & operator=(WorldRender &&) = delete;

		void process();

	private:
		void processModels();
		void processTasks();

		void attach(ChunkModel * model, const glm::ivec3 & cpos);
		void detach(ChunkModel * model);

		core::Scene & m_scene;
		core::SceneEntry m_root;

		ChunkRenderStorage m_chunks;
		ChunkMesher m_mesher;

		core::Listener m_chunkCreated;
		core::Listener m_chunkDestroyed;
		core::Listener m_chunkChanged;

		std::mutex m_modelLock;
		std::mutex m_taskLock;
		std::unordered_set<glm::ivec3> m_chunksToMesh;
	};
}