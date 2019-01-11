#pragma once

#include "io/File.h"

#include <allegro5/bitmap.h>
#include <glm/vec2.hpp>
#include <string>
#include <unordered_map>

namespace core
{
	class Sprite
	{
	public:
		/**
			A sprite sheet is split into multiple segments known as frames. Each frame references a
			specific part of the sprite sheet.
		*/
		struct Frame
		{
			glm::vec2 m_pos = {};		// Position of the frame in the sprite sheet.
			glm::vec2 m_size = {};		// Size of the frame in the sprite sheet.
			glm::vec2 m_center = {};	// Center position of the frame, relative to frame.
			ALLEGRO_COLOR m_tint = {};	// Color multiplier of frame when rendering.
		};

		Sprite() = default;
		Sprite(const Sprite &) = delete;
		Sprite(Sprite &&) = default;
		~Sprite() { clear(); }

		Sprite & operator=(const Sprite &) = delete;
		Sprite & operator=(Sprite &&) = delete;

		/**
			Creates a new sprite sheet of the given size. The old sheet will be discarded if any
			were present. If the new sheet could not be generated, the old (if present) will not be
			discarded.

			@param width The width of the new sprite sheet.
			@param height The height of the new sprite sheet.
			@return True iff the new sheet was created successfully.
		*/
		bool create(unsigned int width, unsigned int height);
		/**
			Loads a sprite sheet from the provided file. The old sheet will be discarded if any
			were present. If the new sheet could not be loaded, the old (if present) will not be
			discarded.

			@param file The file containing the image data which should be loaded.
			@return True iff the new sheet was loaded successfully.
		*/
		bool load(const util::File & file);

		// ...

		/**
			Adds a new frame to the sprite, storing the frame under the specified name. Any frames
			already stored under that name will be overwritten.

			@param name The name of the frame to add.
			@param frame The new frame which should be added to the sprite.
		*/
		void addFrame(const std::string & name, const Frame & frame);
		/**
			Checks if the sprite contains a frame with the given name or not.

			@param name The name of the frame to look for.
			@return True iff the frame is part of the sprite.
		*/
		bool hasFrame(const std::string & name) const;

		/**
			@return The size of the entire sprite sheet.
		*/
		glm::vec2 getSize() const;

		/**
			@param frame The specific frame to obtain the position information of.
			@return The position of the frame.
		*/
		glm::vec2 getPos(const std::string & frame) const;
		/**
			@param frame The specific frame to obtain the size information of.
			@return The size of the frame.
		*/
		glm::vec2 getSize(const std::string & frame) const;
		/**
			@param frame The specific frame to obtain the center information of.
			@return The center of the frame.
		*/
		glm::vec2 getCenter(const std::string & frame) const;
		/**
			@param frame The specific frame to obtain the tint information of.
			@return The tint of the frame.
		*/
		ALLEGRO_COLOR getTint(const std::string & frame) const;

		// ...

		/**
			Draws the entire sprite sheet at the specified position.

			@param pos The position on the screen where the sprite should be drawn.
			@param flags The flags determining how the sprite should be rendered.
		*/
		void draw(const glm::vec2 & pos, int flags = 0) const;
		/**
			Draws the entire sprite sheet at the specified position, with the specified size.

			@param pos The position on the screen where the sprite should be drawn.
			@param size The size the sprite should have on screen.
			@param flags The flags determining how the sprite should be rendered.
		*/
		void draw(const glm::vec2 & pos, const glm::vec2 & size, int flags = 0) const;

		/**
			Draws the frame of the specified name at the specified position.

			@param frame The frame which should be drawn.
			@param pos The position on the screen where the frame should be drawn.
			@param flags The flags determining how the frame should be rendered.
		*/
		void draw(const std::string & frame, const glm::vec2 & pos, int flags = 0) const;
		/**
			Draws the frame of the specified name at the specified position, with the specified
			size.

			@param frame The frame which should be drawn.
			@param pos The position on the screen where the frame should be drawn.
			@param size The size the frame should have on screen.
			@param flags The flags determining how the frame should be rendered.
		*/
		void draw(const std::string & frame, const glm::vec2 & pos, const glm::vec2 & size, int flags = 0) const;

		// ...

		/**
			@return The underlying Allegro bitmap which is used by this sprite.
		*/
		inline auto * getHandle() const { return m_handle; }

	private:
		/**
			Deletes the sprite handle if any sprite handle is currently loaded.
		*/
		void clear();

		std::unordered_map<std::string, Frame> m_frames;

		ALLEGRO_BITMAP * m_handle = nullptr;
	};
}