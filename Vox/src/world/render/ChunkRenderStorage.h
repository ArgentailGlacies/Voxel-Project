#pragma once

#include "world/render/ChunkModel.h"

#include <glm/gtx/hash.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <unordered_map>

namespace vox
{
	/**
		Contains the collection of chunk models associated with a particular world. The models may
		be queried at a later point for rendering purposes.
	*/
	class ChunkRenderStorage
	{
	public:
		ChunkRenderStorage() = default;
		ChunkRenderStorage(const ChunkRenderStorage &) = delete;
		ChunkRenderStorage(ChunkRenderStorage &&) = delete;
		~ChunkRenderStorage() = default;

		ChunkRenderStorage & operator=(const ChunkRenderStorage &) = delete;
		ChunkRenderStorage & operator=(ChunkRenderStorage &&) = delete;

		/**
			Checks if there is a model at the specified location. A model is condidered existing if
			the coordinate holding data is present. As such, the model may still be nullptr and
			still be considered existing.

			@param cpos The position to check exists within the storage.
			@return True iff the position provided was found within the storage.
		*/
		bool has(const glm::ivec3 & cpos) const;

		/**
			Reserves a position, notifying the system that this position will contain a model at
			some point in the future. In order to store any models in the storage, the model
			position must be reserved first.

			@param cpos The position to reserve.
		*/
		void reserve(const glm::ivec3 & cpos);
		/**
			Stores a given model at a specified position. If the position has not been reserved
			before being attempted used, the model will not be stored.

			@param cpos The position to store the model at.
			@param model The model to store.
		*/
		void set(const glm::ivec3 & cpos, std::unique_ptr<ChunkModel> && model);
		/**
			Removes the model at the specified position, if it exist. Otherwise, nothing happens.

			@param cpos The position to elliminate from the storage.
		*/
		void remove(const glm::ivec3 & cpos);

		/**
			Retrieves the model at the specified position if it exists. If not, the exception
			std::invalid_argument is thrown.

			@param cpos The position of the chunk in chunk coordinates.
		*/
		ChunkModel * get(const glm::ivec3 & cpos) const;

		/**
			Retrieves all chunk models which are considered visible.

			// TODO: Must take a camera or some viewpoint in as parameter

			@return A collection of all models that should be rendered.
		*/
		std::unordered_map<glm::ivec3, ChunkModel*> getVisible() const;

	private:
		std::unordered_map<glm::ivec3, std::unique_ptr<ChunkModel>> m_models;
	};
}