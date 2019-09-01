
#include "StateEntry.h"

#include "core/Engine.h"
#include "editor/util/Grid.h"
#include "event/EventBus.h"
#include "event/Events.h"
#include "gui/GuiRegistry.h"
#include "scene/Camera.h"
#include "scene/Scene.h"
#include "ui/Display.h"
#include "ui/Keyboard.h"
#include "ui/Mouse.h"
#include "util/MathOperations.h"
#include "util/MathTrigonometry.h"
#include "world/Universe.h"
#include "world/World.h"
#include "world/BlockRegistry.h"
#include "world/Query.h"

game::StateEntry::StateEntry() = default;
game::StateEntry::~StateEntry() = default;

void game::StateEntry::initialize(core::Engine & engine)
{
	m_display = &engine.getDisplay();

	// Create a test gui
	engine.getGuiRegistry().open(engine.getDataFolder().file("guis/editor_world.xml"));

	// Create a test grid
	m_grid = std::make_unique<vox::Grid>(engine.getScene());

	// Move camera into sensible starting location
	m_camera = &engine.getScene().getCamera(core::Scene::DEFAULT_CAMERA);
	m_camera->setPosition({ 250.0f, 250.0f, 125.0f });
	m_camera->lookTowards({ 0.0f, 0.0f, 5.0f });

	// Register event listeners
	m_displayCloseListener = engine.getEventBus().add<core::DisplayClose>([&engine](auto & event)
	{
		engine.stop();
	});
	m_mouseMoveListener = engine.getEventBus().add<core::MouseMove>([this](auto & event)
	{
		m_mouseDelta = event.m_positionDelta;
	});
	m_keyPressListener = engine.getEventBus().add<core::KeyPress>([this](auto & event)
	{
		if (event.m_key == core::KeyboardKey::W)
			m_keyForward = true;
		else if (event.m_key == core::KeyboardKey::S)
			m_keyBack = true;
		else if (event.m_key == core::KeyboardKey::A)
			m_keyLeft = true;
		else if (event.m_key == core::KeyboardKey::D)
			m_keyRight = true;
		else if (event.m_key == core::KeyboardKey::LSHIFT)
			m_keyBoost = true;
		else if (event.m_key == core::KeyboardKey::SPACE)
			setMousePosition(*m_display, m_display->getSize() / 2);
	});
	m_keyReleaseListener = engine.getEventBus().add<core::KeyRelease>([this](auto & event)
	{
		if (event.m_key == core::KeyboardKey::W)
			m_keyForward = false;
		else if (event.m_key == core::KeyboardKey::S)
			m_keyBack = false;
		else if (event.m_key == core::KeyboardKey::A)
			m_keyLeft = false;
		else if (event.m_key == core::KeyboardKey::D)
			m_keyRight = false;
		else if (event.m_key == core::KeyboardKey::LSHIFT)
			m_keyBoost = false;
	});

	// Write a simple test world
	auto & blocks = engine.getUniverse().getBlockRegistry();
	auto & world = engine.getUniverse().createWorld("world");
	world.write(vox::writeRectangle(blocks["stone"], { -40, -40, 0 }, { 40, 40, 5 }));
	world.write(vox::writeRectangle(blocks["dirt"], { -40, -40, 6 }, { 40, 40, 7 }));
	world.write(vox::writeRectangle(blocks["grass"], { -40, -40, 8 }, { 40, 40, 8 }));
}

void game::StateEntry::process()
{
	const auto moveDistance = (m_keyBoost ? 350.0f : 35.0f) / 60.0f;

	auto rot = m_camera->getRotation() - 0.25f * m_mouseDelta;
	rot.y = util::max(-89.9f, util::min(89.9f, rot.y));
	m_mouseDelta = {};

	auto pos = m_camera->getPosition();
	if (m_keyForward)
		pos += moveDistance * util::cartesian(rot);
	else if (m_keyBack)
		pos -= moveDistance * util::cartesian(rot);
	if (m_keyLeft)
		pos += moveDistance * util::cartesian(rot.x + 90.0f, 0.0f);
	else if (m_keyRight)
		pos -= moveDistance * util::cartesian(rot.x + 90.0f, 0.0f);

	m_camera->setPosition(pos);
	m_camera->setRotation(rot);
}
