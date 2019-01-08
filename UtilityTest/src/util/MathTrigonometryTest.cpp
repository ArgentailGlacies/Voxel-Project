
#include "util/MathTrigonometry.h"

#include "Common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace util::math
{
	TEST_CLASS(MathTrigonometryTest)
	{
	public:
		TEST_METHOD(MathTrigonometry_basic)
		{
			Assert::IsTrue(equal(0.5, cos(60.0)));
			Assert::IsTrue(equal(0.5f, sin(30.0f)));
			Assert::IsTrue(equal(1.0f, tan(45.0f)));
		}

		TEST_METHOD(MathTrigonometry_angle)
		{
			Assert::IsTrue(equal(90.0f, angle(glm::vec3{ 0.0f, 0.0f, 1.0f }, glm::vec3{ 1.0f, 0.0f, 0.0f })));
			Assert::IsTrue(equal(60.0f, angle(glm::vec3{ 0.0f, 1.0f, 1.0f }, glm::vec3{ 1.0f, 1.0f, 0.0f })));
			Assert::IsTrue(equal(45.0f, angle(glm::vec3{ 1.0f, 1.0f, 0.0f }, glm::vec3{ 1.0f, 0.0f, 0.0f })));
		}

		TEST_METHOD(MathTrigonometry_cartesian)
		{
			Assert::IsTrue(equal({ 1.0f, 0.0f, 0.0f }, cartesian(0.0f, 0.0f)));
			Assert::IsTrue(equal({ 0.0f, 1.0f, 0.0f }, cartesian(90.0f, 0.0f)));
			Assert::IsTrue(equal({ 0.0f, 0.0f, 1.0f }, cartesian(0.0f, 90.0f)));
			Assert::IsTrue(equal({ 0.0f, 0.0f, -1.0f }, cartesian(0.0f, -90.0f)));
			Assert::IsTrue(equal({ 0.5f, -0.5f, 0.707107f }, cartesian(-45.0f, 45.0f)));
		}
		TEST_METHOD(MathTrigonometry_spherical)
		{
			Assert::IsTrue(equal({ 0.0f, 00.0f }, spherical(glm::vec3{ 1.0f, 0.0f, 0.0f })));
			Assert::IsTrue(equal({ 90.0f, 00.0f }, spherical(glm::vec3{ 0.0f, 1.0f, 0.0f })));
			Assert::IsTrue(equal({ 0.0f, 90.0f }, spherical(glm::vec3{ 0.0f, 0.0f, 1.0f })));
			Assert::IsTrue(equal({ 45.0f, 00.0f }, spherical(glm::vec3{ 0.5f, 0.5f, 0.0f })));
		}

	private:
		template<typename T> bool equal(const T & v1, const T & v2)
		{
			return abs(v1 - v2) <= static_cast<T>(0.0001);
		}
		template<typename T> bool equal(const glm::tvec2<T> & v1, const glm::tvec2<T> & v2)
		{
			return equal(v1.x, v2.x) && equal(v1.y, v2.y);
		}
		template<typename T> bool equal(const glm::tvec3<T> & v1, const glm::tvec3<T> & v2)
		{
			return equal(v1.x, v2.x) && equal(v1.y, v2.y) && equal(v1.z, v2.z);
		}
	};
}