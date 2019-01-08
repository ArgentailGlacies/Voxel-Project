#pragma once

#include <glm/vec4.hpp>

namespace vox
{
	using Light = glm::tvec4<unsigned char>;

	class BlockData
	{
	public:
		BlockData() noexcept = default;
		BlockData(unsigned int data) noexcept;
		BlockData(unsigned int type, const Light & light) noexcept;

		inline auto & operator=(const BlockData & o) noexcept { m_data = o.m_data; return *this; }
		inline bool operator==(const BlockData & o) const noexcept { return m_data == o.m_data; }
		inline bool operator!=(const BlockData & o) const noexcept { return m_data != o.m_data; }

		/**
			Checks if the data has any nonzero light color or intensity values.

			@return True iff the data contains any color or light values.
		*/
		bool hasLighting() const;

		/**
			Retrieves the raw data of the block. This should normally not be used.

			@return The raw block data.
		*/
		unsigned int getData() const;
		/**
			Retrieves the stored block type. The information about the block type may be found in
			the block registry. The highest allowed block type is MAX_BLOCK_TYPE.

			@return The type of the block.
		*/
		unsigned int getType() const;
		/**
			Retrieves the color of the block. A light component cannot be greater than
			MAX_BLOCK_LIGHT.

			@return The light at the block.
		*/
		Light getLight() const;

		/**
			Assigns the raw data of the block. This should normally not be used.

			@param data The raw data to write into the block.
		*/
		void setData(unsigned int data);
		/**
			Assigns the type of the block. The block type must be less or equal to MAX_BLOCK_TYPE.

			@param type The type to write into the block.
		*/
		void setType(unsigned int type);
		/**
			Assigns the light of the block. The light component values must be less or equal to
			MAX_BLOCK_LIGHT.

			@param light The light level to write into the block.
		*/
		void setLight(const Light & light);

	private:
		unsigned int m_data = 0;
	};
}