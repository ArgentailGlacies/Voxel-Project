
#include "Font.h"

#include "util/MathOperations.h"

#include <allegro5/allegro_font.h>
#include <plog/Log.h>

core::Font & core::Font::operator=(Font && other) noexcept
{
	return *this;
}

void core::Font::clear()
{
	for (const auto & [_, handle] : m_handles)
		al_destroy_font(handle);
	m_handles.clear();
}

void core::Font::prepare(const util::File & file, Style style, float size)
{
	m_data[style] = Data{ file, size };
}
core::Font::Handle core::Font::handle(Style style, Flag flag, float size)
{
	// If the key already exists, return that handle
	const auto key = getKey(style, flag, size);
	if (const auto it = m_handles.find(key); it != m_handles.end())
		return it->second;

	// If no file was found, return the built-in font
	const auto path = m_data[style].m_file.path();
	if (path.empty())
		return m_handles[key] = al_create_builtin_font();

	// Otherwise, load the given font if possible
	Handle font = al_load_font(path.c_str(), key.m_size, static_cast<int>(key.m_flag));
	if (font == nullptr)
	{
		LOG_WARNING << "Failed to load font '" << path << "', using built-in font instead";
		if (const auto it = m_handles.find(Key{}); it != m_handles.end())
			return it->second;
		return m_handles[Key{}] = al_create_builtin_font();
	}
	else
	{
		LOG_INFO << "Loaded font '" << path << "'";
		return m_handles[key] = font;
	}
}

core::Font::Key core::Font::getKey(Style style, Flag flag, float size) const
{
	const auto it = m_data.find(style);
	if (it == m_data.end() || it->second.m_file.path().empty())
		return Key{};
	return Key{ style, flag, util::round(size * it->second.m_size) };
}

// ...

bool core::Font::Key::operator==(const Key & other) const
{
	return m_style == other.m_style && m_flag == other.m_flag && m_size == other.m_size;
}

size_t core::Font::Hasher::operator()(const Key & key) const
{
	return (static_cast<int>(key.m_style) << 24) | (static_cast<int>(key.m_flag) << 16) | key.m_size;
}
