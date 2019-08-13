#pragma once

#include "allegro/Text.h"
#include "gui/internal/Renderer.h"

#include <string>

namespace core
{
	/**
		Renders the contents of a label with a specific style. The user may append some pretext and
		posttext to the actual widget string value as well, if needed. The size of the text is
		determined by the size of the label widget.
	*/
	class RendererLabel : public Renderer
	{
	public:
		RendererLabel(
			const AssetRegistry & assets, const Widget & widget,
			const Text::Style & style, const std::string & pretext, const std::string & posttext
		);

		virtual void render(const Widget & widget, const glm::vec2 & offset) const override final;

	private:
		Text m_text;
	};
}