
#include "script/VoxModules.h"

#include "core/Engine.h"
#include "io/Folder.h"
#include "mock/MockAssetRegistry.h"
#include "mock/MockUBORegistry.h"
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
			core::Engine engine; // TODO: Starting the engine like this causes other tests to fail

			// ...

			ModuleEngine{}.bind(m_script, engine);

			// Ensure that global variables have been added to correct namespace
			Assert::IsTrue(m_script.execute(R"( ENGINE )"));

			// Ensure that engine functionality has been added correctly
			Assert::IsTrue(m_script.execute(R"( ENGINE.stop() )"));
		}

		TEST_METHOD(ModuleUniverse_bind)
		{
			core::AssetRegistry assets = core::mockAssetRegistry();
			core::UBORegistry ubos = core::mockUBORegistry();
			core::Scene scene{ assets, display(), ubos };
			Universe universe{ "test", scene };

			// ...

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
				R"( var WORLD := UNIVERSE.createWorld("world", false) )"
				R"( var QUERY = readBlock(...) )"
			);

			Assert::IsTrue(m_script.execute(R"( WORLD.read(QUERY) )"));
			Assert::IsTrue(m_script.execute(R"( WORLD.write(writeBlock(...)) )"));
			Assert::IsTrue(m_script.execute(R"( WORLD.finish() )"));

			// Ensure that query functionality has been added correctly
			Assert::IsTrue(m_script.execute(R"( readBlock(...) )"));
			Assert::IsTrue(m_script.execute(R"( readCylinder(...) )"));
			Assert::IsTrue(m_script.execute(R"( readEllipse(...) )"));
			Assert::IsTrue(m_script.execute(R"( readRectangle(...) )"));
			Assert::IsTrue(m_script.execute(R"( readLine(...) )"));
			Assert::IsTrue(m_script.execute(R"( readSphere(...) )"));

			Assert::IsTrue(m_script.execute(R"( writeBlock(...) )"));
			Assert::IsTrue(m_script.execute(R"( writeCylinder(...) )"));
			Assert::IsTrue(m_script.execute(R"( writeEllipse(...) )"));
			Assert::IsTrue(m_script.execute(R"( writeRectangle(...) )"));
			Assert::IsTrue(m_script.execute(R"( writeLine(...) )"));
			Assert::IsTrue(m_script.execute(R"( writeSphere(...) )"));
		}

	private:
		core::Script m_script{ "script" };
	};
}