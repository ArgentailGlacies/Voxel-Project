
#include "util/MathOperations.h"

#include "Common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace util::math
{
	TEST_CLASS(MathOperationsTest)
	{
		TEST_METHOD(MathOperations_minNumeric)
		{
			Assert::AreEqual(4, min(4, 7));
			Assert::AreEqual(-2.5f, min({ 4.1f, -2.5f, -1.0f, 5.25f }));
		}
		TEST_METHOD(MathOperations_maxNumeric)
		{
			Assert::AreEqual(7, max(4, 7));
			Assert::AreEqual(5.25f, max({ 4.1f, -2.5f, -1.0f, 5.25f }));
		}
		TEST_METHOD(MathOperations_minVector)
		{
			Assert::AreEqual({ -2, 0 }, min<int>({ 3, 0 }, { -2, 5 }));
			Assert::AreEqual(-1, min<int>({ 3, -1 }));
			Assert::AreEqual({ 2.5f, 0.125f, 6.0f },
				min<float>({ 3.7f, 2.0f, 6.0f }, { 2.5f, 0.125f, 12.0f }));
			Assert::AreEqual(-42.0, min<double>({ 3.14, 1337.0, -42.0 }));
			Assert::AreEqual({ -1, 4, 2, 0 }, min<int>({ -1, 8, 2, 0 }, { 4, 4, 4, 4 }));
			Assert::AreEqual(0, min<int>({ 6, 1, 8, 0 }));
		}
		TEST_METHOD(MathOperations_maxVector)
		{
			Assert::AreEqual({ 3, 5 }, max<int>({ 3, 0 }, { -2, 5 }));
			Assert::AreEqual(3, max<int>({ 3, -1 }));
			Assert::AreEqual({ 3.7f, 2.0f, 12.0f },
				max<float>({ 3.7f, 2.0f, 6.0f }, { 2.5f, 0.125f, 12.0f }));
			Assert::AreEqual(1337.0, max<double>({ 3.14, 1337.0, -42.0 }));
			Assert::AreEqual({ 4, 8, 4, 4 }, max<int>({ -1, 8, 2, 0 }, { 4, 4, 4, 4 }));
			Assert::AreEqual(8, max<int>({ 6, 1, 8, 0 }));
		}

		TEST_METHOD(MathOperations_abs)
		{
			Assert::AreEqual(4, abs(4));
			Assert::AreEqual(0, abs(0));
			Assert::AreEqual(2, abs(-2));

			Assert::AreEqual({ 5.44f, 5.44f }, abs<float>({ 5.44f, -5.44f }));
			Assert::AreEqual({ 0.0f, 2.1f, 1.5f }, abs<float>({ 0.0f, -2.1f, 1.5f }));
			Assert::AreEqual({ 0.0f, 0.1f, 3.5f, 42.0f }, abs<float>({ 0.0f, 0.1f, 3.5f, -42.0f }));
		}
		TEST_METHOD(MathOperations_sign)
		{
			Assert::AreEqual(1, sign(4));
			Assert::AreEqual(0, sign(0));
			Assert::AreEqual(-1, sign(-2));

			Assert::AreEqual({ 1, -1 }, sign<float>({ 5.44f, -5.44f }));
			Assert::AreEqual({ 0, -1, 1 }, sign<float>({ 0.0f, -2.1f, 1.5f }));
			Assert::AreEqual({ 0, 1, 1, -1 }, sign<float>({ 0.0f, 0.1f, 3.5f, -42.0f }));
		}
		TEST_METHOD(MathOperations_signp)
		{
			Assert::AreEqual(1, signp(3));
			Assert::AreEqual(1, signp(0));
			Assert::AreEqual(-1, signp(-1));

			Assert::AreEqual({ 1, -1 }, signp<float>({ 5.44f, -5.44f }));
			Assert::AreEqual({ 1, -1, 1 }, signp<float>({ 0.0f, -2.1f, 1.5f }));
			Assert::AreEqual({ 1, 1, 1, -1 }, signp<float>({ 0.0f, 0.1f, 3.5f, -42.0f }));
		}

		TEST_METHOD(MathOperations_rouding)
		{
			Assert::AreEqual(5, floor(5.9f));
			Assert::AreEqual(-4, floor(-3.1f));

			Assert::AreEqual(2, round(1.5f));
			Assert::AreEqual(2, round(2.49f));
			Assert::AreEqual(-5, round(-5.3f));
			Assert::AreEqual(-5, round(-4.5f));

			Assert::AreEqual(3.2f, round(3.24f, 0.1f), 0.01f);

			Assert::AreEqual(3, ceil(2.1f));
			Assert::AreEqual(-4, ceil(-4.9f));
		}

		TEST_METHOD(MathOperators_sqr)
		{
			Assert::AreEqual(9, sqr(-3));
			Assert::AreEqual(0.25f, sqr(0.5f));
			Assert::AreEqual({ 25.0, 0.0, 4.0 }, sqr(glm::dvec3{ 5.0, 0.0, 2.0 }));
		}

		TEST_METHOD(MathOperators_sqrt)
		{
			Assert::AreEqual(3, sqrt(9));
			Assert::AreEqual(4.5f, sqrt(20.25f));
			Assert::AreEqual({ 0.5, 9.0 }, sqrt(glm::dvec2{ 0.25, 81.0 }));
		}

		TEST_METHOD(MathOperators_pow)
		{
			Assert::AreEqual(9, pow(-3, 2));
			Assert::AreEqual(6.25f, pow(2.5f, 2));
			Assert::AreEqual(625, pow(5, 4.0));

			Assert::AreEqual({ 8, -27 }, pow(glm::ivec2{ 2, -3 }, 3));
			Assert::AreEqual({ 4.0f, 2.25f, 12.25f }, pow(glm::vec3{ 2.0f, 1.5f, 3.5f }, 2));
			Assert::AreEqual({ 1u, 0u, 256u, 6561u }, pow(glm::uvec4{ 1, 0, 2, 3 }, 8));
		}

		TEST_METHOD(MathOperators_lerp)
		{
			Assert::AreEqual(0.25, lerp(0.0, 1.0, 0.25));
			Assert::AreEqual(3.0f, lerp(0.0f, 4.0f, 0.75f));
			Assert::AreEqual(10, lerp(5, 15, 0.5f));

			Assert::AreEqual({ 0.5f, 1.5f, 3.0f },
				lerp(glm::vec3{}, glm::vec3{ 1.0f, 3.0f, 6.0f }, 0.5f));
		}

		TEST_METHOD(MathOperations_intbound)
		{
			Assert::AreEqual(0.0f, intbound(5, 1), 0.0001f);
			Assert::AreEqual(3.0f, intbound(2.1f, 0.3f), 0.0001f);
			Assert::AreEqual(2.5f, intbound(0.5, 0.2), 0.0001f);
		}
	};
}