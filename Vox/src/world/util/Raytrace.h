#pragma once

#include <glm/vec3.hpp>

namespace vox
{
	class Raytrace
	{
	public:
		Raytrace() = delete;
		Raytrace(const glm::ivec3 & start, const glm::ivec3 & end);
		Raytrace(const glm::vec3 & start, const glm::vec3 & end);
		Raytrace(const glm::vec3 & start, const glm::vec3 & dir, float length);
		Raytrace(const Raytrace &) = default;
		Raytrace(Raytrace &&) = default;
		~Raytrace() = default;

		Raytrace & operator=(const Raytrace &) = default;
		Raytrace & operator=(Raytrace &&) = default;

		/**
			Checks if the ray has reached the last block or if there are still more blocks to go.

			@return True iff there are more blocks the ray can reach.
		*/
		bool valid() const;

		/**
			Moves the current ray position to the next block along its path.
		*/
		void next();
		/**
			Moves the current ray position to the next block along its path and retrieves the new
			position of the ray.

			@return The new position of the ray.
		*/
		inline auto nextPos() { next(); return getPos(); }
		/**
			Moves the current ray position to the next block along its path and retrieves the new
			block position of the ray.

			@return The new block position of the ray.
		*/
		inline auto nextBlockPos() { next(); return getBlockPos(); }

		/**
			Retrieves the current exact position of the ray.

			@return The current exact position.
		*/
		glm::vec3 getPos() const;
		/**
			Retrieves the previous exact position of the ray.

			@return The previous exact position.
		*/
		glm::vec3 getOldPos() const;
		/**
			Retrieves the current block position of the ray.

			@return The current block position.
		*/
		glm::ivec3 getBlockPos() const;
		/**
			Retrieves the previous block position of the ray.

			@return The previous block position.
		*/
		glm::ivec3 getOldBlockPos() const;

	private:
		glm::vec3 m_pos, m_oldPos;
		glm::vec3 m_dir;
		glm::vec3 m_step;

		glm::vec3 m_tMax, m_tDelta;
	};

	class RaytraceBresenham
	{
	public:
		RaytraceBresenham() = delete;
		RaytraceBresenham(const glm::ivec3 & start, const glm::ivec3 & end);
		RaytraceBresenham(const glm::vec3 & start, const glm::vec3 & end);
		RaytraceBresenham(const glm::vec3 & start, const glm::vec3 & dir, float length);
		RaytraceBresenham(const RaytraceBresenham &) = default;
		RaytraceBresenham(RaytraceBresenham &&) = default;
		~RaytraceBresenham() = default;

		RaytraceBresenham & operator=(const RaytraceBresenham &) = default;
		RaytraceBresenham & operator=(RaytraceBresenham &&) = default;

		/**
			Checks if the ray has reached the last block or if there are still more blocks to go.

			@return True iff there are more blocks the ray can reach.
		*/
		bool valid() const;

		/**
			Moves the current ray position to the next block along its path.
		*/
		void next();
		/**
			Moves the current ray position to the next block along its path and retrieves the new
			block position of the ray.

			@return The new block position of the ray.
		*/
		inline auto nextPos() { next(); return getPos(); }

		/**
			Retrieves the current block position of the ray.

			@return The current block position.
		*/
		glm::ivec3 getPos() const;
		/**
			Retrieves the previous block position of the ray.

			@return The previous block position.
		*/
		glm::ivec3 getOldPos() const;

	private:
		glm::ivec3 m_pos, m_oldPos;
		glm::ivec3 m_delta;
		glm::ivec3 m_step;
		glm::ivec3 m_error{};

		int m_totalSteps;
		int m_currentStep = 0;
	};
}