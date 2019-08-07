
#include "Font.h"

#include <allegro5/allegro_font.h>
#include <plog/Log.h>

core::Font & core::Font::operator=(Font && other) noexcept
{
	return *this;
}

void core::Font::clear()
{
}

void core::Font::prepare(const util::File & file, Style style, int size)
{
}
core::Font::Handle core::Font::handle(Style style, Flag flag, float size)
{
	return nullptr;
}

// ...

size_t core::Font::Hasher::operator()(const Key & key) const
{
	return 0u;
}
