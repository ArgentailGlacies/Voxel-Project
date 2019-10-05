
#include "script/VoxModules.h"

#include "core/Engine.h"
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
		}

	private:
		core::Script m_script{ "script" };
	};
}