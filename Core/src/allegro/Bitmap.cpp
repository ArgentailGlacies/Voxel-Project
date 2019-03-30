
#include "Bitmap.h"

#include <allegro5/allegro.h>
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
}

core::Bitmap & core::Bitmap::operator=(Bitmap && other)
{
	std::swap(m_handle, other.m_handle);
	return *this;
}

glm::ivec2 core::Bitmap::getSize() const
{
	if (m_handle == nullptr)
		return glm::ivec2{};
	return glm::ivec2{ al_get_bitmap_width(m_handle), al_get_bitmap_height(m_handle) };
}

void core::Bitmap::clear()
{
	if (m_handle != nullptr)
		al_destroy_bitmap(m_handle);
	m_handle = nullptr;
}

core::Bitmap core::Bitmap::child(const glm::ivec2 & pos, const glm::ivec2 & size) const
{
	Bitmap child;
	if (m_handle != nullptr)
		child.m_handle = al_create_sub_bitmap(m_handle, pos.x, pos.y, size.x, size.y);
	return child;
}

bool core::Bitmap::create(unsigned int width, unsigned int height)
{
	clear();
	m_handle = al_create_bitmap(width, height);
	return m_handle != nullptr;
}
bool core::Bitmap::load(const util::File & file)
{
	if (!file.exists())
		LOG_WARNING << "Attempted to load non-existing bitmap '" + file.path() << "'";
	else
		LOG_INFO << "Loading bitmap '" << file.path() << "'...";

	clear();
	m_handle = al_load_bitmap(file.path().c_str());
	return m_handle != nullptr;
}
void core::Bitmap::save(const util::File & file)
{
	LOG_INFO << "Saving bitmap '" << file.path() << "'...";

	if (m_handle != nullptr)
		al_save_bitmap(file.path().c_str(), m_handle);
}

// ...

void core::Bitmap::draw(const glm::vec2 & pos, const glm::vec4 & tint) const
{
	if (m_handle != nullptr)
		al_draw_tinted_bitmap(m_handle, color(tint), pos.x, pos.y, 0);
}
void core::Bitmap::draw(const glm::vec2 & pos, const glm::vec2 & size, const glm::vec4 & tint) const
{
	if (m_handle != nullptr)
		al_draw_tinted_scaled_bitmap(m_handle, color(tint), 0.0f, 0.0f, size.x, size.y, pos.x, pos.y, size.x, size.y, 0);
}
void core::Bitmap::draw(
	const glm::vec2 & sourcePos, const glm::vec2 & sourceSize, const glm::vec2 & center,
	const glm::vec2 & targetPos, const glm::vec2 & targetSize, const glm::vec4 & tint,
	float angle
) const
{
	if (m_handle != nullptr)
		al_draw_tinted_scaled_rotated_bitmap_region(
			m_handle, sourcePos.x, sourcePos.y, sourceSize.x, sourceSize.y, color(tint), center.x, center.y,
			targetPos.x, targetPos.y, targetSize.x / sourceSize.x, targetSize.y / sourceSize.y, angle, 0
		);
}

// ...

core::Bitmap::LockedRegion::LockedRegion(ALLEGRO_BITMAP * bitmap, ALLEGRO_LOCKED_REGION * region)
	: m_bitmap(bitmap), m_region(region)
{}
core::Bitmap::LockedRegion::~LockedRegion()
{
	if (m_bitmap != nullptr)
		al_unlock_bitmap(m_bitmap);
}

const void * core::Bitmap::LockedRegion::data() const
{
	if (m_bitmap == nullptr || m_region == nullptr)
		return nullptr;
	const auto height = al_get_bitmap_height(m_bitmap);
	return static_cast<uint8_t*>(m_region->data) + (height - 1) * m_region->pitch;
}

core::Bitmap::LockedRegion core::Bitmap::lock(LockMode mode)
{
	if (m_handle == nullptr)
		return LockedRegion{ nullptr, nullptr };
	auto * region = al_lock_bitmap(m_handle, al_get_bitmap_format(m_handle), static_cast<int>(mode));
	return LockedRegion{ m_handle, region };
}
