
#include <plog/Log.h>

#include <CppUnitTest.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_MODULE_INITIALIZE(CoreInitialize)
{
	plog::init(plog::debug, "unit_test.log");
}
TEST_MODULE_CLEANUP(CoreCleanup)
{
}
