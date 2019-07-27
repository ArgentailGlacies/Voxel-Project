
#include "RenderKey.h"

namespace
{
	constexpr unsigned long long MASK_FULLSCREEN_LAYER		= 0xC000'0000'0000'0000;
	constexpr unsigned long long MASK_VIEWPORT				= 0x3000'0000'0000'0000;
	constexpr unsigned long long MASK_VIEWPORT_LAYER		= 0x0E00'0000'0000'0000;
	constexpr unsigned long long MASK_TRANSLUCENCY			= 0x0100'0000'0000'0000;
	constexpr unsigned long long MASK_PROGRAM_OPAQUE		= 0x00FF'0000'0000'0000;
	constexpr unsigned long long MASK_TEXTURE_OPAQUE		= 0x0000'FFFF'FF00'0000;
	constexpr unsigned long long MASK_DEPTH_OPAQUE			= 0x0000'0000'00FF'FFFF;
	constexpr unsigned long long MASK_DEPTH_TRANSPARENT		= 0x00FF'FFFF'0000'0000;
	constexpr unsigned long long MASK_PROGRAM_TRANSPARENT	= 0x0000'0000'FF00'0000;
	constexpr unsigned long long MASK_TEXTURE_TRANSPARENT	= 0x0000'0000'00FF'FFFF;

	constexpr unsigned int BIT_FULLSCREEN_LAYER		= 62;
	constexpr unsigned int BIT_VIEWPORT				= 60;
	constexpr unsigned int BIT_VIEWPORT_LAYER		= 57;
	constexpr unsigned int BIT_TRANSLUCENCY			= 56;
	constexpr unsigned int BIT_PROGRAM_OPAQUE		= 32;
	constexpr unsigned int BIT_TEXTURE_OPAQUE		= 24;
	constexpr unsigned int BIT_DEPTH_OPAQUE			= 0;
	constexpr unsigned int BIT_DEPTH_TRANSPARENT	= 32;
	constexpr unsigned int BIT_PROGRAM_TRANSPARENT	= 24;
	constexpr unsigned int BIT_TEXTURE_TRANSPARENT	= 0;

	constexpr unsigned int MAX_DEPTH = 0xFF'FFFF;
}

core::RenderKey::Data core::RenderKey::compile() const
{
	Data data = 0;

	data |= (static_cast<Data>(m_fullscreenLayer) << BIT_FULLSCREEN_LAYER) & MASK_FULLSCREEN_LAYER;
	data |= (static_cast<Data>(m_viewport) << BIT_VIEWPORT) & MASK_VIEWPORT;
	data |= (static_cast<Data>(m_viewportLayer) << BIT_VIEWPORT_LAYER) & MASK_VIEWPORT_LAYER;
	data |= (static_cast<Data>(m_translucency) << BIT_TRANSLUCENCY) & MASK_TRANSLUCENCY;

	if (m_translucency == Translucency::OPAQUE)
	{
		data |= (static_cast<Data>(m_program) << BIT_PROGRAM_OPAQUE) & MASK_PROGRAM_OPAQUE;
		data |= (static_cast<Data>(m_texture) << BIT_TEXTURE_OPAQUE) & MASK_TEXTURE_OPAQUE;
		data |= (static_cast<Data>(m_depth) << BIT_DEPTH_OPAQUE) & MASK_DEPTH_OPAQUE;
	}
	else
	{
		data |= (static_cast<Data>(MAX_DEPTH - m_depth) << BIT_DEPTH_TRANSPARENT) & MASK_DEPTH_TRANSPARENT;
		data |= (static_cast<Data>(m_program) << BIT_PROGRAM_TRANSPARENT) & MASK_PROGRAM_TRANSPARENT;
		data |= (static_cast<Data>(m_texture) << BIT_TEXTURE_TRANSPARENT) & MASK_TEXTURE_TRANSPARENT;
	}

	return data;
}
