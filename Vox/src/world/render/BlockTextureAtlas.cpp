
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

	const auto index = attach(core::Bitmap{ file }, row);
	m_handles.emplace(path, index);
	return index;
}
unsigned int vox::BlockTextureAtlas::attach(core::Bitmap && bitmap, bool row)
{
	if (bitmap.handle() == nullptr)
		return 0;

	const auto index = m_frames.size();
	const auto size = bitmap.getSize();

	if (m_width == 0 || m_height == 0)
	{
		m_width = size.x;
		m_height = size.y;
	}

	if (m_width != size.x || m_height != size.y)
	{
		const auto r = row ? 0 : 1;
		const auto c = row ? 1 : 0;

		glm::ivec2 pos;
		for (pos[c] = 0; pos[c] < size.y; pos[c] += m_height)
		for (pos[r] = 0; pos[r] < size.x; pos[r] += m_width)
			m_frames.push_back(bitmap.child({ pos[r], pos[c] }, { m_width, m_height }));

		m_bitmaps.push_back(std::move(bitmap));
	}
	else
		m_frames.push_back(std::move(bitmap));

	m_depth = m_frames.size();
	return index;
}

void vox::BlockTextureAtlas::build()
{
	reserve({ m_width, m_height, m_depth }, core::DataFormat::UNSIGNED_INT, core::ColorFormat::RGBA);

	unsigned int index = 0;
	for (auto & frame : m_frames)
	{
		write(
			{ 0, 0, index++ }, { m_width, m_height, 1 },
			core::DataFormat::UNSIGNED_BYTE, core::ColorFormat::BGRA,
			frame.lock(core::Bitmap::LockMode::READONLY).data()
		);
	}

	cleanup();
}
void vox::BlockTextureAtlas::cleanup()
{
	m_frames.clear();
	m_bitmaps.clear();
	m_handles.clear();
}
