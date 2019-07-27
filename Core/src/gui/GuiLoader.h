#pragma once

#include "asset/AssetRegistry.h"
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
		GuiLoader(const AssetRegistry & assets, GuiData & data, Widget & root)
			: m_assets(assets), m_data(data), m_root(root)
		{}

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
		const AssetRegistry & m_assets;
		GuiData & m_data;
		Widget & m_root;
	};
}