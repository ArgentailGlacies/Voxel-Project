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
		inline void addText(const Style & style, const std::string & text) { add<ElementText>(style, text); }
		/**
			Adds a value component with the given style. The value will be converted to a string
			and properly rendered.

			@param <T> The type of the value which should be rendered.
			@param style The style the value should be rendered with.
			@param value The value which should be rendered.
			@param precision The number of decimals used to render floating point values.
			@param fixed Whether all the decimals should be rendered, even when redundant.
		*/
		template<typename T>
		inline void addValue(const Style & style, const T & value, int precision = 2, bool fixed = false) { add<ElementValue<T>>(style, value, precision, fixed); }

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
		template<typename Element, typename ...Param>
		inline void add(const Style & style, const Param & ...params)
		{
			m_elements.push_back(std::make_unique<Element>(asElementStyle(style), params...));
		}

		/**
			Converts the given input style to a style which is compatible with elements.

			@param style The style to convert to an element style.
			@return The style as an element style.
		*/
		Element::Style asElementStyle(const Style & style);

		// ...

		const AssetRegistry & m_assets;

		std::unordered_map<std::string, Asset<Font>::Reference> m_fonts;
		std::vector<std::unique_ptr<Element>> m_elements;
	};
}