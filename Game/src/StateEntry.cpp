
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

	// Set up an initial world editor and move camera into proper position
	auto & scene = engine.getScene();
	auto & camera = scene.getCamera(core::Scene::DEFAULT_CAMERA);
	camera.setPosition({ -14.43f, -14.43f, 14.43f });
	camera.lookTowards({ 0.0f, 0.0f, 0.0f });

	m_editor = std::make_unique<vox::EditorWorld>(scene, engine.getEventBus());

	// Create a test gui
	engine.getGuiRegistry().open(engine.getDataFolder().file("guis/editor_world.xml"));

	// Write a simple test world and the editor
	auto & blocks = engine.getUniverse().getBlockRegistry();
	auto & world = engine.getUniverse().createWorld("world");
	world.write(vox::writeRectangle(blocks["stone"], { -40, -40, 0 }, { 40, 40, 5 }));
	world.write(vox::writeRectangle(blocks["dirt"], { -40, -40, 6 }, { 40, 40, 7 }));
	world.write(vox::writeRectangle(blocks["grass"], { -40, -40, 8 }, { 40, 40, 8 }));
}
