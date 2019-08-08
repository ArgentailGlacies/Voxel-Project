
#include "TextElement.h"

#include "util/MathOperations.h"

#include <allegro5/allegro_font.h>
#include <plog/Log.h>

namespace
{
	/**
		Checks if the given codepoint is allowed to create a natural line break.

		@return True iff the codepoint can be a natural line break.
	*/
	bool isLinebreak(int codepoint)
	{
		switch (codepoint)
		{
		case '\0':
		case '\n':
		case ' ':
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
}

// ...

core::ElementText::ElementText(Font::Handle font, const std::string & text, const glm::vec4 & color, bool strikethrough, bool underline)
	: m_font(font), m_color(color), m_strikethrough(strikethrough), m_underline(underline)
{
	m_text = al_ustr_new(text.c_str());
}
core::ElementText::~ElementText() noexcept
{
	if (m_text != nullptr)
		al_ustr_free(m_text);
}

std::vector<core::Element::Task> core::ElementText::split(int position, int width) const
{
	std::vector<Task> tasks;

	Task task;

	glm::ivec2 size{};

	for (int index = 0; al_ustr_next(m_text, &index); )
	{
		// Check if the codepoint can result in a natural new line
		const int currentCodepoint = al_ustr_get(m_text, index - 1);
		const int nextCodepoint = al_ustr_get(m_text, index);

		if (isLinebreak(currentCodepoint))
			task.m_size = size;

		// If not exceeding maximum width, update task size
		const auto advance = getAdvance(currentCodepoint, nextCodepoint);
		if (position + advance.x <= width)
		{
			position += advance.x;
			size.x += advance.x;
			size.y = util::max(size.y, advance.y);
		}
		// Create a new task and reset size and position
		else
		{
			position = 0;
			size.x -= task.m_size.x;
			size.y = task.m_size.y < size.y ? size.y : 0;
			tasks.push_back(task);
		}
	}

	// The last task is not picked up within the algorithm
	task.m_size = size;
	tasks.push_back(task);
	return tasks;
}

glm::ivec2 core::ElementText::getAdvance(int current, int next) const
{
	const int advance = al_get_glyph_advance(m_font, current, next);

	int bby, bbh, _;
	al_get_glyph_dimensions(m_font, current, &_, &bby, &_, &bbh);

	return glm::ivec2{ advance, bby + bbh };
}
