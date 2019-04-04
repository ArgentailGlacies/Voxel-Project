#pragma once

#include "event/Events.h"
#include "gui/GuiData.h"
#include "gui/Widget.h"

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
		Widgets m_widgets;
	};
}
