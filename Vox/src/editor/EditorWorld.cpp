
#include "EditorWorld.h"

#include "scene/Scene.h"

vox::EditorWorld::EditorWorld(core::Scene & scene, core::EventBus & bus)
	: m_grid(scene), m_handler(scene.getCamera(core::Scene::DEFAULT_CAMERA), bus)
{}
