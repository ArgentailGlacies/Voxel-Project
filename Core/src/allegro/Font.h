#pragma once

#include "io/File.h"

#include <unordered_map>

struct ALLEGRO_FONT;

namespace core
{
	class Font
	{
	public:
		Font() = default;
		Font(const Font &) = delete;
		Font(Font && other) noexcept { *this = std::move(other); }
		~Font() noexcept { clear(); }

		Font & operator=(const Font &) = delete;
		Font & operator=(Font && other) noexcept;

		/**
			The font handle should not be used by external users in any way. It is only to be used
			by any text-rendering routines which must know about the base font handle they should
			render.
		*/
		using Handle = ALLEGRO_FONT *;

		/**
			The font style can be used to make the font apear bold or italic, emphasizing certain
			parts of the text.
		*/
		enum class Style
		{
			REGULAR,
			BOLD,
			BOLD_ITALIC,
			ITALIC,
		};
		/**
			Specifies how the font should look when rendering.
		*/
		enum class Flag
		{
			NORMAL = 0,
			MONOCHROME = 2,
		};

		// ...

		/**
			Loads the given file as the given style. The font will use the given file as the base
			font for all size variations of that specific style.

			@param file The file to the font file to load.
			@param style The style associated with this font file.
			@param size The base size for this font file.
		*/
		void prepare(const util::File & file, Style style, int size);

		/**
			Actually loads the font handle if it has not already been loaded. The handle is loaded
			with the given style and flag, and the size is given as the font base size times the
			size factor.

			If the style has not been previously prepared correctly, the built-in font will be used
			as the base font instead. If this is the case, flag and size will be ignored.

			@param style The style of the font handle to retrieve.
			@param flag The flag value to use for the font handle to retrieve.
			@param size The size multiplier for the font handle to retrieve.
		*/
		Handle handle(Style style = Style::REGULAR, Flag flag = Flag::NORMAL, float size = 1.0f);

	private:
		/**
			Erases any underlying font handles, releasing all used memory. Invoking this method will
			invalidate all handles which have been loaded previously.
		*/
		void clear();

		// ...

		/**
			Stores the base font data for any given style. The font will be loaded from the given
			file, with the given base size.
		*/
		struct Data
		{
			util::File m_file;

			int m_size;
		};
		/**
			Stores a set of user-specific data pointing to a specific font handle, to avoid loading
			the same font handle multiple times.
		*/
		struct Key
		{
			Style m_style;
			Flag m_flag;

			int m_size;
		};
		struct Hasher { size_t operator()(const Key & key) const; };

		std::unordered_map<Style, Data> m_data;
		std::unordered_map<Key, Handle, Hasher> m_handles;
	};
}