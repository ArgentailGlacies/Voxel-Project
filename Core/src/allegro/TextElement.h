#pragma once

#include "allegro/Font.h"

#include <allegro5/utf8.h>
#include <functional>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
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

			glm::vec2 m_size;
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
		~ElementText() noexcept;

		virtual std::vector<Task> split(int position, int width) const override final;

	private:
		/**
			Draws a part of the text element at the given position, with relevant styles applied to
			the text.

			@param pos The position the text should be drawn at.
			@param start The index of the first character to draw.
			@param end The index after the last character to draw.
		*/
		void draw(const glm::vec2 & pos, int start, int end) const;

		// ...

		ALLEGRO_USTR * m_text;
		Style m_style;
	};

	/**
		Takes care of processing a generic value into a format which is accepted by the rendering
		routine. The text is split according to the same rules as regular text.
	*/
	template<typename T>
	class ElementValue : public Element
	{
	public:
		ElementValue(const Style & style, const T & value)
			: m_style(style), m_value(value), m_cached(value)
		{
			m_text = std::make_unique< ElementText>(m_style, std::to_string(value));
		}

		inline virtual std::vector<Task> split(int position, int width) const override final
		{
			if (m_value != m_cached)
			{
				m_cached = m_value;
				m_text = std::make_unique<ElementText>(m_style, std::to_string(m_value));
			}
			return m_text ? m_text->split(position, width) : std::vector<Task>{};
		}

	private:
		Style m_style;

		const T & m_value;
		mutable T m_cached;

		mutable std::unique_ptr<ElementText> m_text;
	};
}