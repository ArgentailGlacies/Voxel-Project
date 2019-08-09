
#include "Text.h"

#include "allegro/TextElement.h"
#include "asset/AssetRegistry.h"

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

void core::Text::add(const Style & style, const std::string & text)
{
	auto font = m_fonts[style.m_font] = m_assets.get<Font>(style.m_font);
	if (font.empty())
		return;

	auto handle = font->handle(getStyle(style.m_bold, style.m_italic), getFlag(style.m_monochrome), style.m_size);
	auto element = std::make_unique<ElementText>(handle, text, style.m_color, style.m_strikethrough, style.m_underline);
	m_elements.push_back(std::move(element));
}

void core::Text::draw(const glm::vec2 & pos, const glm::vec2 & size) const
{
}
