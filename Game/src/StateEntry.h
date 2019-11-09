#pragma once

#include "event/EventListener.h"
#include "state/State.h"
#include "script/Script.h"

#include <memory>

namespace vox
{
	class EditorWorld;
}

namespace game
{
	class StateEntry : public core::State
	{
	public:
		StateEntry();
		~StateEntry();

		virtual void initialize(core::Engine & engine) override final;
		virtual void process() override final {};

	private:
		std::unique_ptr<vox::EditorWorld> m_editor;

		core::Listener m_displayCloseListener;
		core::Script m_script{ "script" };
	};
}