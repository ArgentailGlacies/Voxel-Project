
#include "StateEntry.h"

#include "core/Engine.h"
#include "event/EventBus.h"
#include "event/Events.h"
#include "gui/GuiRegistry.h"

#include "editor/EditorWorld.h"
#include "world/Universe.h"
#include "world/World.h"
#include "world/BlockRegistry.h"
#include "world/Query.h"

game::StateEntry::StateEntry() = default;
game::StateEntry::~StateEntry() = default;

void game::StateEntry::initialize(core::Engine & engine)
{
	// Register event listeners
	m_displayCloseListener = engine.getEventBus().add<core::DisplayClose>([&engine](auto & event)
	{
		engine.stop();
	});

	// Create a test gui
	engine.getGuiRegistry().open(engine.getDataFolder().file("guis/editor_world.xml"));

	// Write a simple test world and the editor
	m_editor = std::make_unique<vox::EditorWorld>(engine.getScene(), engine.getEventBus());

	auto & blocks = engine.getUniverse().getBlockRegistry();
	auto & world = engine.getUniverse().createWorld("world");
	world.write(vox::writeRectangle(blocks["stone"], { -40, -40, 0 }, { 40, 40, 5 }));
	world.write(vox::writeRectangle(blocks["dirt"], { -40, -40, 6 }, { 40, 40, 7 }));
	world.write(vox::writeRectangle(blocks["grass"], { -40, -40, 8 }, { 40, 40, 8 }));
}
