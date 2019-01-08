
#include "BlockTextureAtlas.h"

#include <allegro5/allegro.h>
#include <plog/Log.h>

vox::BlockTextureAtlas::BlockTextureAtlas() noexcept
	: Texture(
		core::TextureType::TEXTURE_2D_ARRAY,
		core::DataFormat::UNSIGNED_INT,
		core::ColorFormat::RGBA
	)
{}
vox::BlockTextureAtlas::~BlockTextureAtlas() noexcept
{
	cleanup();
}

unsigned int vox::BlockTextureAtlas::attach(const util::File & file, bool row)
{
	const auto path = file.path();
	if (const auto it = m_handles.find(path); it != m_handles.end())
		return it->second;

	if (!file.exists())
	{
		LOG_WARNING << "Attempted to load non-existing file '" + path << "'";
		return 0;
	}

	LOG_INFO << "Loading bitmap '" << path << "'...";
	const auto index = attach(gsl::make_not_null(al_load_bitmap(path.c_str())), row);
	m_handles.emplace(path, index);
	return index;
}
unsigned int vox::BlockTextureAtlas::attach(Bitmap && bitmap, bool row)
{
	const auto index = m_frames.size();
	const auto width = al_get_bitmap_width(bitmap);
	const auto height = al_get_bitmap_height(bitmap);

	if (m_width == 0u || m_height == 0u)
	{
		m_width = width;
		m_height = height;
	}

	m_bitmaps.push_back(bitmap);
	if (m_width != width || m_height != height)
	{
		const auto r = row ? 0 : 1;
		const auto c = row ? 1 : 0;

		glm::ivec2 pos;
		for (pos[c] = 0; pos[c] < height; pos[c] += m_height)
		for (pos[r] = 0; pos[r] < width; pos[r] += m_width)
		{
			auto subBitmap = al_create_sub_bitmap(bitmap, pos[r], pos[c], m_width, m_height);
			m_bitmaps.push_back(subBitmap);
			m_frames.push_back(subBitmap);
		}
	}
	else
		m_frames.push_back(bitmap);
	m_depth = m_frames.size();
	return index;
}

void vox::BlockTextureAtlas::build()
{
	reserve({ m_width, m_height, m_depth }, core::DataFormat::UNSIGNED_INT, core::ColorFormat::RGBA);

	unsigned int index = 0;
	for (auto & frame : m_frames)
	{
		const auto * region = al_lock_bitmap(frame, al_get_bitmap_format(frame), ALLEGRO_LOCK_READONLY);
		const void * data = static_cast<uint8_t*>(region->data) + (m_height - 1) * region->pitch;
		write({ 0, 0, index++ }, { m_width, m_height, 1 }, core::DataFormat::UNSIGNED_BYTE, core::ColorFormat::BGRA, data);
		al_unlock_bitmap(frame);
	}

	cleanup();
}
void vox::BlockTextureAtlas::cleanup()
{
	for (auto & bitmap : m_bitmaps)
		al_destroy_bitmap(bitmap);
	m_bitmaps.clear();
	m_frames.clear();
	m_handles.clear();
}
