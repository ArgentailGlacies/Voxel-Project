
#include "io/File.h"

#include <plog/Log.h>

#include <CppUnitTest.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_MODULE_INITIALIZE(CoreInitialize)
{
	const util::File logFile{ "unit_test_util.log" };
	logFile.erase();
	plog::init(plog::debug, logFile.path().c_str());
}
TEST_MODULE_CLEANUP(CoreCleanup)
{
}
