#pragma once

#include "gui/GuiData.h"

namespace core
{
	class Gui
	{
	public:
		/**
			Performes all on-tick updates for all underlying widgets.
		*/
		void process();
		/**
			Renders all underlying widgets to the screen.
		*/
		void render() const;

	private:
		GuiData m_data;
	};
}
