#pragma once

#include "state/State.h"

#include <memory>

namespace core
{
	class Engine;

	class StateManager
	{
	public:
		StateManager() = delete;
		StateManager(Engine & engine);
		StateManager(const StateManager &) = delete;
		StateManager(StateManager &&) = delete;
		~StateManager() = default;

		StateManager & operator=(const StateManager &) = delete;
		StateManager & operator=(StateManager &&) = delete;

		/**
			Swaps to the new state if applicable, then processes the state once.
		*/
		void process();

		/**
			Specifies what the next state should be. The state will be changed between process
			cycles, such that no state may be changed during a state transition. The new state
			will be automatically be initialized.

			@param args The arguments neccessary to construct an instance of the provided state.
			@return A reference to the next state.
		*/
		template<typename State, typename ...Args> State & set(Args && ...args);

	private:
		Engine & m_engine;

		std::unique_ptr<State> m_current, m_next;
	};
}

namespace core
{
	template<typename State, typename ...Args>
	inline State & StateManager::set(Args && ...args)
	{
		m_next = std::make_unique<State>(std::forward<Args>(args)...);
		return *static_cast<State*>(m_next.get());
	}
}