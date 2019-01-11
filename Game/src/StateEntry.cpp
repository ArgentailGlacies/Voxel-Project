
#include "StateEntry.h"

#include "core/Engine.h"
#include "event/EventBus.h"
#include "event/Events.h"
#include "scene/Camera.h"
#include "scene/Scene.h"
#include "world/Universe.h"
#include "world/World.h"
#include "world/BlockRegistry.h"
#include "world/Query.h"

void game::StateEntry::initialize(core::Engine & engine)
{
	m_listener = engine.getEventBus().add<core::DisplayClose>([&engine](auto & event) { engine.stop(); });

	auto & camera = engine.getScene().getCamera(core::Scene::DEFAULT_CAMERA);
	camera.setPosition({ 50.0f, 50.0f, 20.0f });
	camera.lookTowards({ 0.0f, 0.0f, 5.0f });

	auto & blocks = engine.getUniverse().getBlockRegistry();
	auto & world = engine.getUniverse().createWorld("world");
	world.write(vox::writeRectangle(blocks["stone"], { -50, -50, 0 }, { 50, 50, 5 }));
	world.write(vox::writeRectangle(blocks["dirt"], { -50, -50, 6 }, { 50, 50, 7 }));
	world.write(vox::writeRectangle(blocks["grass"], { -50, -50, 8 }, { 50, 50, 8 }));
}

void game::StateEntry::process()
{
}
