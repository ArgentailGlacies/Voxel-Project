
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
			Universe universe{ "", scene };

			// ...

			ModuleUniverse{}.bind(m_script, universe);
		}

	private:
		core::Script m_script{ "script" };
	};
}