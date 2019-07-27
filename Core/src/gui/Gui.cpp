
#include "Gui.h"

#include "gui/GuiLoader.h"
#include "gui/WidgetProcessor.h"

void core::Gui::load(const util::File & file, const AssetRegistry & assets)
{
	GuiLoader{ assets, m_data, m_root }.load(file);
}

void core::Gui::process()
{
	process(m_root);
}
void core::Gui::render() const
{
	render(m_root, glm::vec2{});
}
void core::Gui::process(Widget & widget)
{
	gui::updatePosition(widget);
	
	for (auto & child : widget.m_family.m_children)
		process(*child);

	gui::updateSize(widget);
}
void core::Gui::render(const Widget & widget, const glm::vec2 & offset) const
{
	const auto pos = widget.m_bbox.m_pos + offset;

	// Render all aspects of this specific widget
	for (const auto & renderer : widget.m_renderers)
		renderer(widget, pos);

	// Render all child widgets on top of this widget
	for (const auto & child : widget.m_family.m_children)
		render(*child, pos);
}
