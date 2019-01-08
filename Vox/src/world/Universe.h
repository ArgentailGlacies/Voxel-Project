#pragma once

#include "asset/Asset.h"
#include "io/Folder.h"
#include "world/BlockRegistry.h"
#include "world/render/BlockRenderRegistry.h"
#include "world/render/BlockTextureAtlas.h"
#include "world/render/WorldRender.h"
#include "world/World.h"

#include <string>
#include <unordered_map>

namespace core
{
	class AssetRegistry;
	class Scene;
}

namespace vox
{
	class Universe
	{
	public:
		Universe() = delete;
		Universe(const util::Folder & data, core::Scene & scene);
		Universe(const Universe &) = delete;
		Universe(Universe &&) = delete;
		~Universe() = default;

		Universe & operator=(const Universe &) = delete;
		Universe & operator=(Universe &&) = delete;

		void process();

		/**
			Loads the block texture atlas into the universe and ensures that the asset remains
			active for as long as the universe exists.

			@param assets All assets which are available in the system.
		*/
		void prepare(const core::AssetRegistry & assets);

		/**
			Constructs a new world with the provided name. If a world with the same name has already
			been created, the std::invalid_argument exception is thrown. If there is any world data
			avaliable, that data will be automatically loaded by the world. The world changes may be
			optionally saved to the world data directory, if desired.

			@param name The name of the world to create.
			@param autosave Whether any changes made to the world should be automatically saved.
		*/
		World & createWorld(const std::string & name, bool autosave = false);
		/**
			Destroys the world with the provided name. If the world should automatically save any
			changes made to it, all changes will be saved when the world is destroyed.

			@param name The name of the world to destroy.
		*/
		void destroyWorld(const std::string & name);
		/**
			Checks whether a world with the given name has previously been created or not.

			@param name The world to test if exists.
			@return True iff the world exists.
		*/
		bool hasWorld(const std::string & name) const;
		/**
			Retrieves a handle to the world with the given name. If the world does not exist, a
			std::invalid_argument exception is thrown.

			@param name The name of the world to gain access to.
			@param return The world with the given name.
		*/
		World & getWorld(const std::string & name) const;

		/**
			Retrieves the blocks that exists within the game.

			@return The block registry containing all blocks available.
		*/
		inline const auto & getBlockRegistry() const { return m_blockRegistry; }
		/**
			Retrieves the block renders that exists within the game.

			@return The block render registry containing all block renders available.
		*/
		inline const auto & getBlockRenderRegistry() const { return m_blockRenderRegistry; }

	private:
		void loadBlockRegistry();
		void storeBlockRegistry() const;
		void loadBlockRenderRegistry();

		// ...

		const util::Folder m_data;
		core::Scene & m_scene;

		mutable std::unordered_map<std::string, World> m_worlds;
		mutable std::unordered_map<std::string, WorldRender> m_renders;

		BlockRegistry m_blockRegistry;
		BlockRenderRegistry m_blockRenderRegistry;

		core::Asset<core::Texture>::Reference m_textureAtlas;
	};
}