
#include "Text.h"

#include "allegro/TextElement.h"
#include "asset/AssetRegistry.h"
#include "util/MathOperations.h"

namespace
{
	/**
		Retrieves the font style mathing the specific combination of bold and italic.

		@param bold Whether the font style should be bold.
		@param italic Whether the font style should be italic.
	*/
	core::Font::Style getStyle(bool bold, bool italic)
	{
		if (bold && italic)
			return core::Font::Style::BOLD_ITALIC;
		else if (bold)
			return core::Font::Style::BOLD;
		else if (italic)
			return core::Font::Style::ITALIC;
		else
			return core::Font::Style::REGULAR;
	}
	/**
		Retrieves the font flag which is suitable for the font if it should be monochrome.

		@param monochrome Whether the font should be monochrome or not.
	*/
	core::Font::Flag getFlag(bool monochrome)
	{
		if (monochrome)
			return core::Font::Flag::MONOCHROME;
		else
			return core::Font::Flag::NORMAL;
	}
}

// ...

core::Element::Style core::Text::asElementStyle(const Style & style)
{
	auto font = m_fonts[style.m_font] = m_assets.get<Font>(style.m_font);
	if (font.empty())
		return {};

	Element::Style elementStyle;
	elementStyle.m_font = font->handle(getStyle(style.m_bold, style.m_italic), getFlag(style.m_monochrome), style.m_size);
	elementStyle.m_color = style.m_color;
	elementStyle.m_strikethrough = style.m_strikethrough;
	elementStyle.m_underline = style.m_underline;
	return elementStyle;
}

void core::Text::draw(const glm::vec2 & pos, const glm::vec2 & size) const
{
	std::vector<std::vector<Element::Task>> m_tasks;

	// Process all elements to a collection of rows of tasks
	m_tasks.push_back({});

	int position = 0;
	for (const auto & element : m_elements)
	for (const auto & task : element->split(position, size.x))
	{
		m_tasks.back().push_back(task);
		if (task.m_newline)
		{
			m_tasks.push_back({});
			position = 0;
		}
		else
			position += task.m_size.x;
	}

	// All render tasks may now be performed
	float y = 0.0f;
	for (const auto & row : m_tasks)
	{
		// Must know the height of the full row before drawing it
		float height = 0.0f;
		for (const auto & task : row)
			height = util::max(height, task.m_size.y);

		// Must track where along the row the tasks should be rendered
		float x = 0.0f;
		for (const auto & task : row)
		{
			task.m_renderer(pos + glm::vec2{ x, y + height - task.m_size.y });
			x += task.m_size.x;
		}
		y += height;
	}
}
