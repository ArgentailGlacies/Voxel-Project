#pragma once

#include "allegro/Font.h"
#include "allegro/TextElement.h"
#include "asset/Asset.h"

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace core
{
	class AssetRegistry;

	/**
		A single unit of text represents some information the user can read and easily understand.
		The representation does not need to be pure text, but may also include sprites and various
		effects to convey information to the user.
	*/
	class Text
	{
	public:
		Text(const AssetRegistry & assets) : m_assets(assets) {}

		// ...

		/**
			Represents how a single text unit should be rendered. Certain text elements might not
			contain any text, in which case only parts of the style will be respected.
		*/
		struct Style
		{
			std::string m_font;
			glm::vec4 m_color = { 0.0f, 0.0f, 0.0f, 1.0f };

			float m_size = 1.0f;

			bool m_bold = false;
			bool m_italic = false;
			bool m_monochrome = false;
			bool m_strikethrough = false;
			bool m_underline = false;
		};

		// ...

		/**
			Adds a text component with the given style. The text may contain newline characters.

			@param style The style the text should be rendered with.
			@param text The text which should be rendered.
		*/
		void add(const Style & style, const std::string & text);

		/**
			Renders the text at the given position. The text will automatically wrap at wrappable
			characters if the text would exceed the maximum given width. If no wrapable character
			was found, the text will be split in the middle of a word.
			
			If the size is constrained too heavily, it is possible the text cannot be rendered
			within the provided box and will bleed out.

			@param pos The position the text should be rendered at.
			@param size The size of the box the text is confined to.
		*/
		void draw(const glm::vec2 & pos, const glm::vec2 & size) const;

	private:
		const AssetRegistry & m_assets;

		std::unordered_map<std::string, Asset<Font>::Reference> m_fonts;
		std::vector<std::unique_ptr<Element>> m_elements;
	};
}