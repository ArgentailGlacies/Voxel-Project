#pragma once

#include "io/File.h"

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <memory>

struct ALLEGRO_BITMAP;
struct ALLEGRO_LOCKED_REGION;

namespace core
{
	class Bitmap
	{
	public:
		Bitmap() = default;
		Bitmap(const util::File & file) { load(file); }
		Bitmap(unsigned int width, unsigned int height) { create(width, height); }
		Bitmap(const Bitmap & other) { *this = other; }
		Bitmap(Bitmap && other) { *this = std::move(other); }
		~Bitmap() { clear(); }

		Bitmap & operator=(const Bitmap & other);
		Bitmap & operator=(Bitmap && other);

		/**
			@return The size of the bitmap.
		*/
		glm::ivec2 getSize() const;

		// ...

		/**
			Constructs a child bitmap from this bitmap. The child will be invalid once the parent
			bitmap goes out of scope. It is the user's responsibility to ensure that the child
			bitmap is not used in that situation.

			@param pos The location of the child bitmap in the parent bitmap.
			@param size The size of the child bitmap.
			@return The child bitmap.
		*/
		Bitmap child(const glm::ivec2 & pos, const glm::ivec2 & size) const;

		/**
			Creates a new blank bitmap of the provided size. The bitmap should not be too large, as
			the memory requirements are significant.

			@param width The width of the bitmap to create.
			@param height The height of the bitmap to create.
			@return True iff the bitmap was successfully created.
		*/
		bool create(unsigned int width, unsigned int height);
		/**
			Loads a bitmap from the provided file. The bitmap may be of any file format accepted by
			Allegro.

			@param file The file containing the bitmap to load.
			@return True iff the bitmap was successfully loaded.
		*/
		bool load(const util::File & file);
		/**
			Saves the bitmap to the provided file. The bitmap may be saved as any file format
			supported by Allegro.

			@param file The file to save the bitmap to.
		*/
		void save(const util::File& file);

		/**
			Erases any underlying bitmap handles, releasing any used memory.
		*/
		void clear();

		// ...

		/**
			Determines how the user may interact with the bitmap after it has been locked.
		*/
		enum class LockMode
		{
			READWRITE = 0,
			READONLY = 1,
			WRITEONLY = 2
		};

		/**
			A locked region can be used to read or write data from or to the underlying bitmap in an
			efficient manner. The bitmap will automatically be unlocked once the locked region goes
			out of scope.
		*/
		class LockedRegion
		{
		public:
			LockedRegion(ALLEGRO_BITMAP * bitmap, ALLEGRO_LOCKED_REGION * region);
			~LockedRegion();

			/**
				@return The raw data associated with the bitmap. The data is on a format expected by
				OpenGL's textures and as such may be directly fed to it.
			*/
			const void * data() const;

		private:
			ALLEGRO_BITMAP * m_bitmap;
			ALLEGRO_LOCKED_REGION * m_region;
		};

		/**
			Locks the bitmap such that its data is immutable. The bitmap must not be used for any
			drawing operations while it is locked!
		*/
		LockedRegion lock(LockMode mode);

		// ...

		/**
			Renders the entire bitmap at the specified position with the specified tint.

			@param pos The position the bitmap should be rendered at.
			@param tint The color muliplier of the bitmap.
		*/
		void draw(const glm::vec2 & pos, const glm::vec4 & tint) const;
		/**
			Renders the entire bitmap at the specified position with the specified size and tint.

			@param pos The position the bitmap should be rendered at.
			@param size The desired size the bitmap should be rendered to.
			@param tint The color muliplier of the bitmap.
		*/
		void draw(const glm::vec2 & pos, const glm::vec2 & size, const glm::vec4 & tint) const;
		/**
			Renders part of the bitmap using the specified position, size, and center, at the
			specified target position and size, with the specified tint.

			@param source The position and size the bitmap should be rendered with.
			@param target The desired target and size the bitmap should be rendered with.
			@param tint The color muliplier of the bitmap.
		*/
		void draw(
			const glm::vec2 & sourcePos, const glm::vec2 & sourceSize, const glm::vec2 & center,
			const glm::vec2 & targetPos, const glm::vec2 & targetSize, const glm::vec4 & tint
		) const;

		// ...

		inline auto * handle() const { return m_handle; }

	private:
		ALLEGRO_BITMAP * m_handle = nullptr;
	};
}