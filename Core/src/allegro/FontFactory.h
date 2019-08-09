#pragma once

#include "allegro/Font.h"
#include "asset/AssetUtil.h"

namespace core
{
	/**
		Loads up fonts from font resource files. The font resource file may define different font
		styles, different fonts for each style, and the base size for each style. Only font styles
		which are defined by Font::Style are valid, however.

		Example resource file:
		<font>
		  <style size="18" type="regular"     path="data/gfx/fonts/overlock.ttf" />
		  <style size="18" type="bold"        path="data/gfx/fonts/overlock-bold.ttf" />
		  <style size="18" type="bold-italic" path="data/gfx/fonts/overlock-bold-italic.ttf" />
		  <style size="18" type="italic"      path="data/gfx/fonts/overlock-italic.ttf" />
		</font>
	*/
	class FontLoader : public Loader<Font>
	{
	public:
		virtual std::unique_ptr<Font> load(const util::File & file) override final;
	};
}