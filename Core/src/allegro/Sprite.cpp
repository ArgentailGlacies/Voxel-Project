
#include "Sprite.h"

#include <allegro5/allegro.h>

void core::Sprite::addFrame(const std::string & name, const Frame & frame)
{
	m_frames[name] = frame;
}
bool core::Sprite::hasFrame(const std::string & name) const
{
	return m_frames.find(name) != m_frames.end();
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
glm::vec4 core::Sprite::getTint(const std::string & frame) const
{
	const auto it = m_frames.find(frame);
	return it == m_frames.end() ? glm::vec4{} : it->second.m_tint;
}

void core::Sprite::draw(const glm::vec2 & pos) const
{
	m_bitmap.draw(pos, glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
}
void core::Sprite::draw(const glm::vec2 & pos, const glm::vec2 & size) const
{
	m_bitmap.draw(pos, size, glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
}
void core::Sprite::draw(const std::string & frame, const glm::vec2 & pos) const
{
	if (!hasFrame(frame))
		return;
	const auto & f = m_frames.find(frame)->second;
	m_bitmap.draw(f.m_pos, f.m_size, f.m_center, pos, f.m_size, f.m_tint);
}
void core::Sprite::draw(const std::string & frame, const glm::vec2 & pos, const glm::vec2 & size) const
{
	if (!hasFrame(frame))
		return;
	const auto & f = m_frames.find(frame)->second;
	m_bitmap.draw(f.m_pos, f.m_size, f.m_center, pos, size, f.m_tint);
}
