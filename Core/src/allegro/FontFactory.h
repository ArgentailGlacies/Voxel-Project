#pragma once

#include "allegro/Font.h"
#include "asset/AssetUtil.h"

namespace core
{
	class FontLoader : public Loader<Font>
	{
	public:
		virtual std::unique_ptr<Font> load(const util::File & file) override final;
	};
}