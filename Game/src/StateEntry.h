#pragma once

#include "state/State.h"

namespace game
{
	class StateEntry : public core::State
	{
	public:
		virtual void initialize(core::Engine & engine) override final;
		virtual void process() override final;
	};
}