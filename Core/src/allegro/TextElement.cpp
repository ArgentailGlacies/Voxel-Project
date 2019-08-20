
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
		Returns true iff the codepoint is considered a whitespace. Whitespaces are ignored when at
		the end of a line, but also acts as a natural linebreaker.

		@param codepoint The character which is being investigated.
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
		Returns true iff the codepoint is considered a natural linebreaker. The linebreaker will be
		included in a line, if the line is broken at the linebreaker.

		@param codepoint The character which is being investigated.
	*/
	bool isBreak(int codepoint)
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

	class Tokenizer
	{
	public:
		Tokenizer(ALLEGRO_USTR * text, ALLEGRO_FONT * font) : m_text(text), m_font(font) {}

		bool extract(core::ElementText::Task & task, int position, int width);

	private:
		ALLEGRO_USTR * m_text;
		ALLEGRO_FONT * m_font;

		core::ElementText::Task m_current;
		core::ElementText::Task m_snapshot;

		bool m_first = true;
	};

	bool Tokenizer::extract(core::ElementText::Task & task, int position, int width)
	{
		const int start = position;
		const int length = al_ustr_length(m_text);

		int index = m_snapshot.m_index + m_snapshot.m_length;
		if (index >= length)
			return false;

		// Must reset the current task to defaults
		m_current = {};
		m_current.m_index = index;
		m_current.m_pos = { 0, std::numeric_limits<float>::max() };
		m_snapshot = {};
		m_snapshot.m_index = index;

		// For as long as another character exists...
		while (true)
		{
			const auto codepoint = al_ustr_get_next(m_text, &index);
			if (codepoint < 0)
				break;

			// Calculate size of the codepoint
			glm::ivec2 pos;
			glm::ivec2 size;
			al_get_glyph_dimensions(m_font, codepoint, &pos.x, &pos.y, &size.x, &size.y);
			size.x = al_get_glyph_advance(m_font, codepoint, al_ustr_get(m_text, index));

			// If not enough space to fit character and no previous snapshot has been established
			if (position + size.x >= width && m_snapshot.m_length == 0)
			{
				// If not allowed to randomly inject a newline, generate a snapshot
				if (start == 0)
					m_snapshot = m_current;
				// Otherwise, force an empty newline and start over
				else if (codepoint != ' ')
				{
					task = {};
					task.m_newline = true;
					return true;
				}
			}

			// Advance to the next character
			m_current.m_length++;
			m_current.m_size.x += size.x;
			m_current.m_size.y = util::max(m_current.m_size.y, static_cast<float>(size.y));
			m_current.m_pos.y = util::min(m_current.m_pos.y, static_cast<float>(pos.y));

			// If encountering a whitespace character, generate a snapshot
			if (isWhitespace(codepoint))
				m_snapshot = m_current;

			// If not enough space for the current codepoint, restore snapshot and return
			if ((position += size.x) >= width || codepoint == '\n')
			{
				// If no previous snapshot were found, use whatever has been found
				if (m_snapshot.m_length == 0)
					m_snapshot = m_current;
				
				task = m_snapshot;
				task.m_newline = true;
				m_first = false;
				return true;
			}

			// If encountering a natural point to insert a linebreak, generate a snapshot
			if (isBreak(codepoint))
				m_snapshot = m_current;
		}

		// Reaching this indicates that we extracted the last task, must terminate next iteration
		task = m_current;
		m_snapshot.m_index = length;
		return true;
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
	Tokenizer tokenizer{ m_text, m_style.m_font };

	Task task;
	while (tokenizer.extract(task, position, width))
	{
		task.m_renderer = [this, task](const glm::vec2 & pos) { draw(pos, task.m_index, task.m_index + task.m_length); };
		task.m_lineHeight = al_get_font_line_height(m_style.m_font) - task.m_pos.y;
		task.m_lineSpan = al_get_font_ascent(m_style.m_font) - task.m_pos.y;
		tasks.push_back(task);
		position = task.m_newline ? 0 : position + task.m_size.x;
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