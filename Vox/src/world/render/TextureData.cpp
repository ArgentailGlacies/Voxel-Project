
#include "TextureData.h"

namespace
{
	constexpr unsigned int MASK_TEXTURE = 0x0000FFFF;
	constexpr unsigned int MASK_META = 0x00FF0000;
	constexpr unsigned int MASK_HASH = 0xFF000000;

	constexpr unsigned int BIT_META = 16;
	constexpr unsigned int BIT_HASH = 24;
}

vox::TextureData::TextureData(unsigned int texture) noexcept
	: m_data(texture)
{}
vox::TextureData::TextureData(unsigned int texture, unsigned int meta, unsigned int hash) noexcept
{
	setTexture(texture);
	setMeta(meta);
	setHash(hash);
}

bool vox::TextureData::operator==(const TextureData & other) const noexcept
{
	return ((m_data ^ other.m_data) & ~MASK_HASH) == 0;
}
bool vox::TextureData::operator!=(const TextureData & other) const noexcept
{
	return !(*this == other);
}

unsigned int vox::TextureData::getTexture() const
{
	return m_data & MASK_TEXTURE;
}
unsigned int vox::TextureData::getMeta() const
{
	return (m_data & MASK_META) >> BIT_META;
}
unsigned int vox::TextureData::getHash() const
{
	return (m_data & MASK_HASH) >> BIT_HASH;
}

void vox::TextureData::setTexture(unsigned int texture)
{
	m_data = (m_data & ~MASK_TEXTURE) | (texture & MASK_TEXTURE);
}
void vox::TextureData::setMeta(unsigned int meta)
{
	m_data = (m_data & ~MASK_META) | ((meta << BIT_META) & MASK_META);
}
void vox::TextureData::setHash(unsigned int hash)
{
	m_data = (m_data & ~MASK_HASH) | ((hash << BIT_HASH) & MASK_HASH);
}