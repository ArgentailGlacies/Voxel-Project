
#include "TextElement.h"

#include "util/MathOperations.h"

#include <allegro5/allegro_font.h>
#include <plog/Log.h>

namespace
{
	/**
		The Allegro color has the same layout as vec4s, and as such may be converted in this manner.
	*/
	inline auto & color(const glm::vec4 & tint)
	{
		return reinterpret_cast<const ALLEGRO_COLOR &>(tint);
	}

	/**
		Checks if the given codepoint is considered whitespace.

		@return True iff the codepoint is considered invisible.
	*/
	bool isWhitespace(int codepoint)
	{
		switch (codepoint)
		{
		case ' ':
		case '\n':
			return true;
		default:
			return false;
		}
	}
	/**
		Checks if the given codepoint is allowed to create a natural line break.

		@return True iff the codepoint can be a natural line break.
	*/
	bool isLinebreak(int codepoint)
	{
		switch (codepoint)
		{
		case '.':
		case ',':
		case '-':
		case ':':
		case ';':
		case '!':
		case '?':
			return true;
		default:
			return false;
		}
	}

	// ...

	/**
		A token represents a small segment of text, and the space required to fully draw the text.
	*/
	struct Token
	{
		int m_index = 0;
		int m_end = 0;

		glm::ivec2 m_size{};
	};

	/**
		The tokenizer accepts an Allegro font and text, then finds a series of tokens which can be
		drawn sequentially to fit within a given bounding box.
	*/
	class Tokenizer
	{
	public:
		Tokenizer(ALLEGRO_FONT * font, ALLEGRO_USTR * text, int position, int width)
			: m_font(font), m_text(text), m_position(position), m_width(width)
		{
			m_length = al_ustr_length(m_text);
		}

		/**
			@return True iff more tokens can be extracted from the element.
		*/
		inline bool valid() const { return m_index < m_length; }
		/**
			Attempts to retrieve another token from the current index and the current position. The
			index will be incremented by the length of the token, and the position will always be
			reset to 0.
		*/
		Token next();

	private:
		/**
			Checks how much more space is required to progress to the next character from the given
			character in the text.

			@param current The current codepoint being examined.
			@param next The next codepoint in the text.
			@return The advance to the next the glyph from the current glyph.
		*/
		glm::ivec2 getAdvance(int current, int next) const;

		// ...

		ALLEGRO_FONT * m_font;
		ALLEGRO_USTR * m_text;

		int m_width;
		int m_length;
		int m_position;
		int m_index = 0;
	};

	Token Tokenizer::next()
	{
		Token token;
		token.m_index = m_index;
		token.m_end = m_length;

		glm::ivec2 size{};

		int index = m_index;
		for (; index < m_length; ++index)
		{
			const int curr = al_ustr_get(m_text, index);
			const int next = al_ustr_get(m_text, index + 1);
			const auto advance = getAdvance(curr, next);

			// Must treat newlines in a special manner if they are the first character
			if (curr == '\n')
			{
				size.y = getAdvance(curr, next).y;
				index++;
				break;
			}
			// If the next character cannot fit, break unless there are no characters added yet
			if (m_position + advance.x > m_width && index != m_index)
				break;

			m_position += advance.x;
			size.x += advance.x;
			size.y = util::max(size.y, advance.y);

			// If the currect character is allowed to break a line, or is the last character...
			if (isLinebreak(curr) || next == -1)
			{
				token.m_end = index + 1;
				token.m_size = size;
			}
			// If the next character is a whitespace, then it can be skipped if necessary
			if (isWhitespace(next))
			{
				token.m_end = index + 2;
				token.m_size = size;
			}
		}

		// If no size was assigned to the token, use whatever has been found thus far
		if (token.m_size.x == 0)
		{
			token.m_size = size;
			token.m_end = index;
		}

		// Must make sure to update the tokenizer state
		m_index = token.m_end;
		m_position = 0;
		return token;
	}

	glm::ivec2 Tokenizer::getAdvance(int current, int next) const
	{
		const int advance = al_get_glyph_advance(m_font, current, next);

		int bby, bbh, _;
		al_get_glyph_dimensions(m_font, current, &_, &bby, &_, &bbh);

		return glm::ivec2{ advance, bbh };
	}
}

// ...

core::ElementText::ElementText(const Style & style, const std::string & text)
	: m_style(style)
{
	m_text = al_ustr_new(text.c_str());
}
core::ElementText::~ElementText()
{
	if (m_text != nullptr)
		al_ustr_free(m_text);
}

std::vector<core::Element::Task> core::ElementText::split(int position, int width) const
{
	std::vector<Task> tasks;

	Tokenizer tokenizer{ m_style.m_font, m_text, position, width };
	while (tokenizer.valid())
	{
		const auto token = tokenizer.next();
		const auto renderer = [this, token](auto & pos) { draw(pos, token.m_index, token.m_end); };

		tasks.push_back(Task{ renderer, token.m_size });
	}
	return tasks;
}

void core::ElementText::draw(const glm::vec2 & pos, int start, int end) const
{
	int x = 0;
	for (int i = start; i < end; ++i)
	{
		const int curr = al_ustr_get(m_text, i);
		const int next = al_ustr_get(m_text, i + 1);

		al_draw_glyph(m_style.m_font, color(m_style.m_color), pos.x + x, pos.y, curr);

		x += al_get_glyph_advance(m_style.m_font, curr, next);
	}
}
