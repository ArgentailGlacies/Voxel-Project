
#include "script/VoxModules.h"

#include "core/Engine.h"
#include "editor/EditorWorld.h"
#include "event/EventBus.h"
#include "io/Folder.h"
#include "mock/MockAssetRegistry.h"
#include "mock/MockUBORegistry.h"
#include "script/CoreModules.h"
#include "script/Script.h"
#include "scene/Scene.h"
#include "world/Universe.h"

#include "Context.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vox::script
{
	TEST_CLASS(VoxModulesTest)
	{
	public:
		TEST_METHOD(ModuleEngine_bind)
		{
			core::Engine engine;

			ModuleEngine{}.bind(m_script, engine);

			// Ensure that global variables have been added to correct namespace
			Assert::IsTrue(m_script.execute(R"( ENGINE )"));

			// Ensure that engine functionality has been added correctly
			Assert::IsTrue(m_script.execute(R"( ENGINE.stop() )"));
		}

		TEST_METHOD(ModuleUniverse_bind)
		{
			Universe universe{ "test", m_scene };

			core::ModuleGlm{}.bind(m_script);
			ModuleUniverse{}.bind(m_script, universe);

			// Ensure that global variables have been added to correct namespace
			Assert::IsTrue(m_script.execute(R"( UNIVERSE )"));

			// Ensure that universe functionality has been added correctly
			Assert::IsTrue(m_script.execute(R"( UNIVERSE.getBlockRegistry() )"));

			Assert::IsTrue(m_script.execute(R"( UNIVERSE.createWorld("world", false) )"));
			Assert::IsTrue(m_script.execute(R"( UNIVERSE.hasWorld("world") )"));
			Assert::IsTrue(m_script.execute(R"( UNIVERSE.getWorld("world") )"));
			Assert::IsTrue(m_script.execute(R"( UNIVERSE.destroyWorld("world") )"));

			// Ensure that world functionality has been added correctly
			m_script.execute(
				R"( var WORLD := UNIVERSE.createWorld("world", false); )"
				R"( var QUERY = readBlock(ivec3()); )"
				R"( var BLOCK = UNIVERSE.getBlockRegistry()["air"]; )"
			);

			Assert::IsTrue(m_script.execute(R"( WORLD.read(QUERY) )"));
			Assert::IsTrue(m_script.execute(R"( WORLD.write(writeBlock(BLOCK, ivec3())) )"));
			Assert::IsTrue(m_script.execute(R"( WORLD.finish() )"));

			// Ensure that query functionality has been added correctly
			Assert::IsTrue(m_script.execute(R"( readBlock(ivec3()) )"));
			//Assert::IsTrue(m_script.execute(R"( readCylinder(ivec3(), ivec3(), ...) )")); // TODO: Cylinder axis has not been exposed
			Assert::IsTrue(m_script.execute(R"( readEllipse(ivec3(), ivec3()) )"));
			Assert::IsTrue(m_script.execute(R"( readRectangle(ivec3(), ivec3()) )"));
			Assert::IsTrue(m_script.execute(R"( readLine(ivec3(), ivec3()) )"));
			Assert::IsTrue(m_script.execute(R"( readSphere(ivec3(), 1) )"));

			Assert::IsTrue(m_script.execute(R"( writeBlock(BLOCK, ivec3()) )"));
			//Assert::IsTrue(m_script.execute(R"( writeCylinder(BLOCK, ivec3(), ivec3(), ...) )")); // TODO: Cylinder axis has not been exposed
			Assert::IsTrue(m_script.execute(R"( writeEllipse(BLOCK, ivec3(), ivec3()) )"));
			Assert::IsTrue(m_script.execute(R"( writeRectangle(BLOCK, ivec3(), ivec3()) )"));
			Assert::IsTrue(m_script.execute(R"( writeLine(BLOCK, ivec3(), ivec3()) )"));
			Assert::IsTrue(m_script.execute(R"( writeSphere(BLOCK, ivec3(), 1) )"));

			// Ensure that world query functionality has been added correctly
			Assert::IsTrue(m_script.execute(R"( var WORLD_QUERY = WorldQuery() )"));
			Assert::IsTrue(m_script.execute(R"( WORLD_QUERY.count() )"));
			Assert::IsTrue(m_script.execute(R"( WORLD_QUERY.empty() )"));
			Assert::IsTrue(m_script.execute(R"( WORLD_QUERY.has(ivec3()) )"));
		}

		TEST_METHOD(ModuleWorldEditor_bind)
		{
			EditorWorld editor{ m_scene, m_bus };

			ModuleWorldEditor{}.bind(m_script, editor);

			// Ensure that global variables have been added to correct namespace
			Assert::IsTrue(m_script.execute(R"( EDITOR )"));
			Assert::IsTrue(m_script.execute(R"( EDITOR.grid )"));
			Assert::IsTrue(m_script.execute(R"( EDITOR.cursor )"));

			// Ensure that grid functionality has been added correctly
			Assert::IsTrue(m_script.execute(R"( EDITOR.grid.setSize(4, 2) )"));
			Assert::IsTrue(m_script.execute(R"( EDITOR.grid.setVisible(true) )"));

			// Ensure that cursor functionality has been added correctly
			Assert::IsTrue(m_script.execute(R"( EDITOR.cursor.lockAxis(AXIS_X, true) )"));
		}

	private:
		core::AssetRegistry m_assets = core::mockAssetRegistry();
		core::UBORegistry m_ubos = core::mockUBORegistry();
		core::Scene m_scene{ m_assets, display(), m_ubos };
		core::EventBus m_bus;

		core::Script m_script{ "script" };
	};
}