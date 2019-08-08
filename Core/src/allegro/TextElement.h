#pragma once

#include "allegro/Font.h"

#include <functional>
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
			part of a text element is rendered. The task is guaranteed to fit within its width.
		*/
		struct Task
		{
			Renderer m_renderer;

			int width;
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
		ElementText() = delete;
		ElementText(Font::Handle handle, const std::string & text, const glm::vec4 & color, bool strikethrough, bool underline);

		virtual std::vector<Task> split(int position, int width) const override final;

	private:
		Font::Handle m_handle;

		std::string m_text;
		glm::vec4 m_color;

		bool m_strikethrough;
		bool m_underline;
	};
}