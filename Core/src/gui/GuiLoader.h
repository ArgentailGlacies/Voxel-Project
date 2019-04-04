#pragma once

#include "gui/GuiData.h"
#include "gui/Widget.h"
#include "io/File.h"

#include <pugixml/pugixml.hpp>

namespace core
{
	class GuiLoader
	{
	public:
		GuiLoader() = delete;
		GuiLoader(Widget & root, GuiData & data) : m_root(root), m_data(data) {}

		void load(const util::File & file);
		void load(const pugi::xml_document & doc);

	private:
		Widget & m_root;
		GuiData & m_data;
	};
}