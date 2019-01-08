#pragma once

#include "allegro/Sprite.h"
#include "asset/AssetUtil.h"

namespace core
{
	class SpriteLoader : public Loader<Sprite>
	{
	public:
		virtual std::unique_ptr<Sprite> load(const util::File & file) override final;
	};
}