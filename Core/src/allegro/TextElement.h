#pragma once

#include "allegro/Font.h"

#include <allegro5/utf8.h>
#include <functional>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

namespace core
{
	class Element
	{
	public:
		/**
			The text rendering routine begins by splitting the text into multiple rendering units.
			Each renderer is then responsible for rendering a small part of the text, typically by
			rendering a text in a different style from the rest. Some tasks may involve rendering
			something other than text, a sprite, for example.
		*/
		using Renderer = std::function<void(const glm::vec2 & pos)>;
		/**
			The rendering task should ideally contain a rendering routine which determines how a
			part of a text element is rendered. The task is guaranteed to fit within its bounding
			box.
		*/
		struct Task
		{
			Renderer m_renderer;

			glm::vec2 m_pos = {};
			glm::vec2 m_size = {};

			bool m_newline = false;
			int m_index = 0;
			int m_length = 0;

			int m_lineHeight = 0;
			int m_lineSpan = 0;
		};

		/**
			Determines how a particular text element should be rendered. 
		*/
		struct Style
		{
			Font::Handle m_font = nullptr;

			glm::vec4 m_color = { 0.0f, 0.0f, 0.0f, 1.0f };

			bool m_strikethrough = false;
			bool m_underline = false;
		};

		// ...

		virtual ~Element() = default;

		/**
			Attempts to split the element into a series of rendering tasks, such that the element
			can be rendered to fit within the provided with. The initial position must be taken into
			account when splitting, such that the rendering task does not exceed the boundary.

			@param position The initial position the rendering task must start from.
			@param width The maximum width of the rendering task.
		*/
		virtual std::vector<Task> split(int position, int width) const = 0;
	};

	// ...

	/**
		Takes care of processing a text into a format which can easily be rendered by the rendering
		routine. The text will be split at newlines, and may be split at whitespace or other special
		characters.
	*/
	class ElementText : public Element
	{
	public:
		ElementText(const Style & style, const std::string & text);
		~ElementText();

		virtual std::vector<Task> split(int position, int width) const override final;

	private:
		/**
			Draws a part of the text element at the given position, with relevant styles applied to
			the text.

			@param offset The position the text should be drawn at.
			@param start The index of the first character to draw.
			@param end The index after the last character to draw.
		*/
		void draw(const glm::vec2 & offset, int start, int end) const;

		// ...

		ALLEGRO_USTR * m_text;
		Style m_style;
	};

	/**
		Takes care of processing a generic value into a format which is accepted by the rendering
		routine. The text is split according to the same rules as regular text.
	*/
	template<typename Type>
	class ElementValue : public Element
	{
	public:
		ElementValue(const Style & style, const Type & value, int precision, bool fixed);

		virtual std::vector<Task> split(int position, int width) const override final;

	private:
		/**
			Converts the given value to a string which can be sent to the underlying text element.

			@param value The value which should be converted.
			@param precision The precisition of floating point numbers.
			@param fixed Whether all the decimals should be included, even when zero.
			@return The string representation of the value.
		*/
		std::string string(const Type & value, int precision, bool fixed) const;

		// ...

		Style m_style;

		const Type & m_value;
		mutable Type m_cached;

		mutable std::unique_ptr<ElementText> m_text;

		int m_precision;
		bool m_fixed;
	};
}

// ...

namespace core
{
	template<typename Type>
	inline ElementValue<Type>::ElementValue(const Style & style, const Type & value, int precision, bool fixed)
		: m_style(style), m_value(value), m_cached(value), m_precision(precision), m_fixed(fixed)
	{
		m_text = std::make_unique<ElementText>(m_style, string(m_value, m_precision, m_fixed));
	}

	template<typename Type>
	inline std::string ElementValue<Type>::string(const Type & value, int precision, bool fixed) const
	{
		std::stringstream stream;
		if (fixed)
			stream << std::fixed << std::setprecision(precision) << value;
		else
			stream << std::setprecision(precision) << value;
		return stream.str();
	}
	template<typename Type>
	inline std::vector<Element::Task> ElementValue<Type>::split(int position, int width) const
	{
		if (m_value != m_cached)
		{
			m_cached = m_value;
			m_text = std::make_unique<ElementText>(m_style, string(m_value, m_precision, m_fixed));
		}
		return m_text ? m_text->split(position, width) : std::vector<Task>{};
	}
}