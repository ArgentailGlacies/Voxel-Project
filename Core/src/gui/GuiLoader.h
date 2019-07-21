#pragma once

#include "gui/GuiData.h"
#include "gui/Widget.h"
#include "gui/WidgetLoader.h"
#include "io/File.h"

#include <pugixml/pugixml.hpp>

namespace core
{
	class GuiLoader
	{
	public:
		GuiLoader() = delete;
		GuiLoader(GuiData & data, Widget & root) : m_data(data), m_root(root), m_loader(data) {}

		/**
			Loads a graphical user interface from the specified file. If the file does not exist,
			this method will do nothing.

			@param file The file containing gui data to load.
		*/
		void load(const util::File & file);
		/**
			Loads a graphical user interface from the specified xml document.

			@param doc The document containing gui data to load.
		*/
		void load(const pugi::xml_document & doc);

	private:
		GuiData & m_data;
		Widget & m_root;
		WidgetLoader m_loader;
	};
}