
#include "util/PhysicalOperators.h"
#include "util/PhysicalUnit.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace util::phys
{
	TEST_CLASS(PhysicalUnitTest)
	{
	public:
		TEST_METHOD(unit_comparison)
		{
			static_assert(3_h == 180_min);
			static_assert(5_h != 5_min);

			static_assert(1_m > 40_cm);
			static_assert(5_kg >= 5000_g);
			static_assert(7_s < 2_min);
			static_assert(600_m <= 5_km);
		}

		TEST_METHOD(unit_arithmetic)
		{
			static_assert(8_K == 3_K + 5_K);
			static_assert(6.5_mps == 8_mps - 1.5_mps);
			static_assert(10_m == 5_mps * 2_s);
			static_assert(3_mps2 == 6_N / 2_kg);
		}

		TEST_METHOD(unit_function)
		{
			static_assert(4_m * 1_m == sqr(2_m));
			
			// TODO: Find a way to perform test during compilation
			const auto s = sqrt(10'000_Pa);
			Assert::IsTrue(10'000_Pa == s * s);

			// TODO: Find a way to perform test during compilation
			const auto p = pow<3>(2_mps);
			Assert::IsTrue(8_mps * 1_mps * 1_mps == p);
		}
	};
}