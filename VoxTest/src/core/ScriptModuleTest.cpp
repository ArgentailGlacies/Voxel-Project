
#include "core/ScriptModules.h"

#include "core/Engine.h"
#include "gui/GuiRegistry.h"
#include "mock/MockAssetRegistry.h"
#include "mock/MockUBORegistry.h"
#include "script/Script.h"
#include "state/State.h"
#include "state/StateManager.h"

#include <functional>

#include "Context.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace
{
	/**
		The mock state accepts a callback which should be invoked every tick while the state is
		active. If the specified number of cycles ticks by without the callback terminating the
		engine, the test is considered failed.
	*/
	class MockState : public core::State
	{
	public:
		MockState(int cycles, const std::function<void()> & callback) : m_cycles(cycles), m_callback(callback) {}

		virtual void initialize(core::Engine & engine) override final {}
		virtual void process() override final { if (m_cycles-- <= 0) Assert::Fail(); m_callback(); }

	private:
		int m_cycles;

		std::function<void()> m_callback;
	};
}

namespace core::setup
{
	TEST_CLASS(ScriptModuleTest)
	{
	public:
		TEST_METHOD(ScriptModule_initializeEngine)
		{
			Engine engine;
			engine.initialize();
			engine.getStates().set<MockState>(1, [this]() { m_script.execute("ENGINE.stop()"); });

			script::initializeEngine(m_script, engine);

			engine.start();
		}

		TEST_METHOD(ScriptModule_initializeGui)
		{
			GuiRegistry guis{ m_assets, display(), m_bus, m_scene };

			script::initializeGui(m_script, guis);
			script::initializeFileSystem(m_script);

			Assert::IsTrue(m_script.execute(R"( GUI_REGISTRY.open(File("foobar")) )"));
			Assert::IsTrue(m_script.execute(R"( GUI_REGISTRY.close(File("foobar")) )"));
		}

		TEST_METHOD(ScriptModule_initializeFileSystem)
		{
			script::initializeFileSystem(m_script);

			Assert::IsTrue(m_script.execute(R"( var fileA = File() )"));
			Assert::IsTrue(m_script.execute(R"( var fileB = File("foobar") )"));
			Assert::IsTrue(m_script.execute(R"( var folder = Folder("foobar") )"));
		}

	private:
		AssetRegistry m_assets = mockAssetRegistry();
		UBORegistry m_ubos = mockUBORegistry();
		EventBus m_bus;
		Scene m_scene{ m_assets, display(), m_ubos };

		Script m_script{ "script" };
	};
}