
#include "Sprite.h"

#include <allegro5/allegro.h>

bool core::Sprite::create(unsigned int width, unsigned int height)
{
	clear();
	m_handle = al_create_bitmap(width, height);
	return m_handle != nullptr;
}
bool core::Sprite::load(const util::File & file)
{
	clear();
	m_handle = al_load_bitmap(file.path().c_str());
	return m_handle != nullptr;
}
void core::Sprite::clear()
{
	if (m_handle != nullptr)
		al_destroy_bitmap(m_handle);
}

void core::Sprite::addFrame(const std::string & name, const Frame & frame)
{
	m_frames[name] = frame;
}
bool core::Sprite::hasFrame(const std::string & name) const
{
	return m_frames.find(name) != m_frames.end();
}

glm::vec2 core::Sprite::getSize() const
{
	glm::vec2 size;
	size.x = al_get_bitmap_width(m_handle);
	size.y = al_get_bitmap_height(m_handle);
	return size;
}
glm::vec2 core::Sprite::getPos(const std::string & frame) const
{
	const auto it = m_frames.find(frame);
	return it == m_frames.end() ? glm::vec2{} : it->second.m_pos;
}
glm::vec2 core::Sprite::getSize(const std::string & frame) const
{
	const auto it = m_frames.find(frame);
	return it == m_frames.end() ? glm::vec2{} : it->second.m_size;
}
glm::vec2 core::Sprite::getCenter(const std::string & frame) const
{
	const auto it = m_frames.find(frame);
	return it == m_frames.end() ? glm::vec2{} : it->second.m_center;
}
ALLEGRO_COLOR core::Sprite::getTint(const std::string & frame) const
{
	const auto it = m_frames.find(frame);
	return it == m_frames.end() ? ALLEGRO_COLOR{} : it->second.m_tint;
}

void core::Sprite::draw(const glm::vec2 & pos, int flags) const
{
	if (m_handle == nullptr)
		return;
	al_draw_bitmap(m_handle, pos.x, pos.y, flags);
}
void core::Sprite::draw(const glm::vec2 & pos, const glm::vec2 & size, int flags) const
{
	if (m_handle == nullptr)
		return;
	al_draw_scaled_bitmap(m_handle, 0.0f, 0.0f, size.x, size.y, pos.x, pos.y, size.x, size.y, flags);
}
void core::Sprite::draw(const std::string & frame, const glm::vec2 & pos, int flags) const
{
	if (m_handle == nullptr || !hasFrame(frame))
		return;
	const auto & f = m_frames.find(frame)->second;
	al_draw_tinted_bitmap_region(m_handle, f.m_tint, f.m_pos.x, f.m_pos.y, f.m_size.x, f.m_size.y, pos.x, pos.y, flags);
}
void core::Sprite::draw(const std::string & frame, const glm::vec2 & pos, const glm::vec2 & size, int flags) const
{
	if (m_handle == nullptr || !hasFrame(frame))
		return;
	const auto & f = m_frames.find(frame)->second;
	al_draw_tinted_scaled_rotated_bitmap_region(
		m_handle, f.m_pos.x, f.m_pos.y, f.m_size.x, f.m_size.y, f.m_tint,
		f.m_center.x, f.m_center.y, pos.x, pos.y, size.x, size.y, 0.0f, flags
	);
}
