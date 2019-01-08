#include "Sprite.h"

bool core::Sprite::create(unsigned int width, unsigned int height)
{
	return false;
}
bool core::Sprite::load(const util::File & file)
{
	return false;
}

void core::Sprite::addFrame(const std::string & name, const Frame & frame)
{
}
bool core::Sprite::hasFrame(const std::string & name) const
{
	return false;
}
core::Sprite::Frame & core::Sprite::getFrame(const std::string & name)
{
	return m_frames[name];
}

glm::vec2 core::Sprite::getSize() const
{
	return glm::vec2();
}
glm::vec2 core::Sprite::getPos(const std::string & frame) const
{
	return glm::vec2();
}
glm::vec2 core::Sprite::getSize(const std::string & frame) const
{
	return glm::vec2();
}
glm::vec2 core::Sprite::getCenter(const std::string & frame) const
{
	return glm::vec2();
}
ALLEGRO_COLOR core::Sprite::getTint(const std::string & frame) const
{
	return ALLEGRO_COLOR();
}

void core::Sprite::draw(const glm::vec2 & pos) const
{
}
void core::Sprite::draw(const glm::vec2 & pos, const glm::vec2 & size) const
{
}
void core::Sprite::draw(const std::string & frame, const glm::vec2 & pos) const
{
}
void core::Sprite::draw(const std::string & frame, const glm::vec2 & pos, const glm::vec2 & size) const
{
}
