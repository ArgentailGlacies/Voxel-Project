
#include "Context.h"

#include "allegro/Allegro.h"
#include "io/File.h"
#include "ui/Display.h"

#include <plog/Log.h>

#include <CppUnitTest.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

std::unique_ptr<core::Allegro> ALLEGRO;
std::unique_ptr<core::Display> DISPLAY;

TEST_MODULE_INITIALIZE(CoreInitialize)
{
	const util::File logFile{ "unit_test_vox.log" };
	logFile.erase();
	plog::init(plog::debug, logFile.path().c_str());

	ALLEGRO = std::make_unique<core::Allegro>();
	DISPLAY = std::make_unique<core::Display>(glm::uvec2{ 320u, 240u }, false);
}
TEST_MODULE_CLEANUP(CoreCleanup)
{
	DISPLAY = nullptr;
	ALLEGRO = nullptr;
}
