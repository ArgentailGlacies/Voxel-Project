
#include "state/StateManager.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core
{
	class Engine {};
}

namespace core::state
{
	class StateGeneric : public State
	{
	public:
		virtual void initialize(Engine & engine) override final
		{
			m_initialized = true;
		}
		virtual void process() override final
		{
			m_processed = true;
		}

		inline auto initialized() const { return m_initialized; }
		inline auto processed() const { return m_processed; }

	private:
		bool m_initialized = false;
		bool m_processed = false;
	};

	TEST_CLASS(StateManagerTest)
	{
	public:
		TEST_METHOD(StateManager_set)
		{
			StateManager manager{ m_engine };
			const auto & state = manager.set<StateGeneric>();

			Assert::IsFalse(state.initialized());
			Assert::IsFalse(state.processed());
		}

		TEST_METHOD(StateManager_process)
		{
			StateManager manager{ m_engine };
			const auto & state = manager.set<StateGeneric>();

			manager.process();
			Assert::IsTrue(state.initialized());
			Assert::IsTrue(state.processed());
		}

	private:
		Engine m_engine;
	};
}