#pragma once

#include "util/MathTemporaryFile.h"

#include <glm/vec3.hpp>

namespace vox
{
	/**
		The cursor represents a virtual mouse in 3D world space. The user is able to move it through
		world space in various manners. The cursor is able to select a specific part and make a
		generic selection to another location.
	*/
	class Cursor
	{
	public:
		/**
			Assigns the cursor's position in world space. If any axis are locked, the cursor will
			respect that. The cursor is only allowed to move along non-locked axes.

			@param position The new position of the cursor.
		*/
		void setPosition(const glm::ivec3 & position);
		/**
			Assigns whether the cursor's position should be considered valid, i.e. exist withing the
			world space. If the position is considered invalid, no callbacks will be invoked when
			any action is performed by the cursor.

			@param valid Whether the position of the cursor is valid or not.
		*/
		void setPositionValidity(bool valid);

		/**
			Retrieves the position which was locked in place. If no position is locked, then the
			returned position will be the same as the current cursor position.

			@return The last position which was locked, or the current position.
		*/
		glm::ivec3 getLockedPosition() const;
		/**
			Retrieves the current position of the cursor, factoring in locked axes.

			@return The current position of the cursor.
		*/
		glm::ivec3 getCurrentPosition() const;

		// ...

		/**
			Locks or unlocks the specified axis. The cursor cannot move along a locked axis once a
			position has been locked into place. The cursor will remain free to move along all axes
			for as long as the position is unlocked.

			@param axis The axis which should be locked or unlocked.
			@param locked Whether the axis should the locked or unlocked.
		*/
		void lockAxis(util::Axis axis, bool locked);
		/**
			Locks the position of the cursor such that it can no longer be moved around freely. Once
			in a locked state, the cursor may only move along non-locked axis, originating from the
			last non-locked position.

			@param locked Whether the position of the cursor should be locked or not.
		*/
		void lockPosition(bool locked);

	private:
		glm::bvec3 m_lockAxis = {};

		glm::ivec3 m_currentPosition = {};
		glm::ivec3 m_clickedPosition = {};

		bool m_lockedPosition = false;
		bool m_validPosition = false;
	};
}