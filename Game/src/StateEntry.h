#pragma once

#include "event/EventListener.h"
#include "state/State.h"

#include <functional>
#include <glm/vec2.hpp>

namespace core
{
	class Camera;
	class Display;
}
namespace vox
{
	class Grid;
}

namespace game
{
	class StateEntry : public core::State
	{
	public:
		StateEntry();
		~StateEntry();

		virtual void initialize(core::Engine & engine) override final;
		virtual void process() override final;

	private:
		core::Camera * m_camera = nullptr;
		core::Display * m_display = nullptr;

		core::Listener m_displayCloseListener;
		core::Listener m_mouseMoveListener;
		core::Listener m_keyPressListener;
		core::Listener m_keyReleaseListener;

		std::unique_ptr<vox::Grid> m_grid;

		bool m_keyForward = false;
		bool m_keyBack = false;
		bool m_keyLeft = false;
		bool m_keyRight = false;
		bool m_keyBoost = false;

		glm::vec2 m_mouseDelta = {};
	};
}