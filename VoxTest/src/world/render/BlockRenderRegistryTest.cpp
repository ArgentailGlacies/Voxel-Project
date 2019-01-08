
#include "world/render/BlockRenderRegistry.h"
#include "world/util/Limits.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vox::world::render
{
	TEST_CLASS(BlockRenderRegistryTest)
	{
	public:
		TEST_METHOD(BlockRenderRegistry_insert)
		{
			BlockRenderRegistry registry;
			const auto blockA = registry[0];
			const auto blockB = registry[2];

			Assert::ExpectException<std::invalid_argument>([&]() { registry[MAX_BLOCK_TYPE + 1]; });
		}

		TEST_METHOD(BlockRenderRegistry_extract)
		{
			BlockRenderRegistry registry;
			const auto blockA = registry[0];
			const auto blockB = registry[2];

			const auto & constant = registry;
			constant[0];
			constant[1];
			constant[2];
			Assert::ExpectException<std::invalid_argument>([&]() { constant[3]; });
		}
	};
}