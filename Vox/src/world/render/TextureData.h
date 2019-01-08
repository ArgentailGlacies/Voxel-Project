#pragma once

namespace vox
{
	class TextureData
	{
	public:
		TextureData() noexcept = default;
		TextureData(unsigned int texture) noexcept;
		TextureData(unsigned int texture, unsigned int meta, unsigned int hash) noexcept;

		bool operator==(const TextureData & other) const noexcept;
		bool operator!=(const TextureData & other) const noexcept;

		inline auto getData() const { return m_data; }

		/**
			Retrieves the base texture index when rendering block faces. Depending on the block
			properties, the actually rendered texture might not be the base texture.

			@return The base texture index.
		*/
		unsigned int getTexture() const;
		/**
			Retrieves the number of possible texture indices that may be used to render the block.

			@return The texture meta value.
		*/
		unsigned int getMeta() const;
		/**
			Retrieves the hashed value of the texture. Used to randomize natural textures.

			@return The texture hash value.
		*/
		unsigned int getHash() const;
		/**
			Assigns the base texture index associated with a block face. If no randomization or
			special effects are applied, the visible block face will hold this texture.

			@param texture The base texture index to use when rendering block faces.
		*/
		void setTexture(unsigned int texture);
		/**
			Assigns the meta value associated with the texture. Used to specify how many possible
			textures are available when randomizing or selecting textures.

			@param meta The meta value to write into the texture.
		*/
		void setMeta(unsigned int meta);
		/**
			Assigns the hashed value associated with the texture. Used to randomize natural
			textures or select a random texture. Must be less or equal to MAX_BLOCK_TEXTURE_HASH.

			@param hash The hash value to write into the texture.
		*/
		void setHash(unsigned int hash);

	private:
		unsigned int m_data = 0;
	};
}