
#include "BlockData.h"

namespace
{
	constexpr unsigned int MASK_TYPE  = 0xFFF00000;
	constexpr unsigned int MASK_LIGHT = 0x000FFFFF;
	constexpr unsigned int MASK_CHANNEL = 0x0000001F;
	
	constexpr unsigned int BIT_TYPE  = 20;
	constexpr unsigned int BIT_RED   = 15;
	constexpr unsigned int BIT_GREEN = 10;
	constexpr unsigned int BIT_BLUE  = 5;
	constexpr unsigned int BIT_ALPHA = 0;
}

vox::BlockData::BlockData(unsigned int data) noexcept
	: m_data(data)
{}
vox::BlockData::BlockData(unsigned int type, const Light & light) noexcept
{
	setType(type);
	setLight(light);
}

bool vox::BlockData::hasLighting() const
{
	return (m_data & ~MASK_TYPE) != 0;
}

unsigned int vox::BlockData::getData() const
{
	return m_data;
}
unsigned int vox::BlockData::getType() const
{
	return m_data >> BIT_TYPE;
}
vox::Light vox::BlockData::getLight() const
{
	const auto r = (m_data >> BIT_RED) & MASK_CHANNEL;
	const auto g = (m_data >> BIT_GREEN) & MASK_CHANNEL;
	const auto b = (m_data >> BIT_BLUE) & MASK_CHANNEL;
	const auto a = (m_data >> BIT_ALPHA) & MASK_CHANNEL;
	return { r, g, b, a };
}

void vox::BlockData::setData(unsigned int data)
{
	m_data = data;
}
void vox::BlockData::setType(unsigned int type)
{
	m_data = (m_data & MASK_LIGHT) | type << BIT_TYPE;
}
void vox::BlockData::setLight(const Light & light)
{
	m_data = (m_data & MASK_TYPE) |
		light.r << BIT_RED | light.g << BIT_GREEN | light.b << BIT_BLUE | light.a << BIT_ALPHA;
}
