#pragma once

#include "asset/AssetUtil.h"
#include "io/File.h"
#include "opengl/Texture.h"

#include <allegro5/bitmap.h>
#include <gsl/pointers>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace vox
{
	using Bitmap = gsl::not_null<ALLEGRO_BITMAP*>;

	class BlockTextureAtlas : public core::Texture
	{
	public:
		BlockTextureAtlas() noexcept;
		BlockTextureAtlas(const BlockTextureAtlas &) = delete;
		BlockTextureAtlas(BlockTextureAtlas &&) = delete;
		~BlockTextureAtlas() noexcept;

		BlockTextureAtlas & operator=(const BlockTextureAtlas &) = delete;
		BlockTextureAtlas & operator=(BlockTextureAtlas &&) = delete;

		/**
			Retrieves the total number of tiles in the atlas.

			@return The number of tiles added to the atlas.
		*/
		inline auto size() const { return m_depth; }

		/**
			Loads the provided image file and appends it to the atlas. If the image is larger than
			the atlas dimensions, the image will be split into components, each the same size as the
			atlas itself.

			@param file The file containing the bitmap to attach.
			@param row True if the bitmap should be parsed as rows, false to be parsed as columns.
			@return The index of the image, or zero if the image does not exist.
		*/
		unsigned int attach(const util::File & file, bool row = true);
		/**
			Attaches the provided bitmap to the atlas. The ownership of the bitmap is transferred to
			the atlas. If the image is larger than the atlas dimensions, the image will be split
			into components, each the same dimensions as the atlas itself.

			@param file The file containing the bitmap to attach.
			@param row True if the bitmap should be parsed as rows, false to be parsed as columns.
			@return The index of the bitmap.
		*/
		unsigned int attach(Bitmap && bitmap, bool row = true);

		/**
			Builds the texture atlas from all provided bitmaps. No more bitmaps should be attached
			to the atlas after it has been built. The atlas should never be built more than once.
		*/
		void build();

	private:
		void cleanup();

		unsigned int m_width = 0;
		unsigned int m_height = 0;
		unsigned int m_depth = 0;

		std::unordered_map<std::string, unsigned int> m_handles;
		std::vector<ALLEGRO_BITMAP*> m_bitmaps;
		std::vector<ALLEGRO_BITMAP*> m_frames;
	};

	class BlockTextureAtlasBuilder : public core::Builder<BlockTextureAtlas>
	{
	public:
		inline virtual std::unique_ptr<BlockTextureAtlas> build() override final
		{
			return std::make_unique<BlockTextureAtlas>();
		}
	};
}