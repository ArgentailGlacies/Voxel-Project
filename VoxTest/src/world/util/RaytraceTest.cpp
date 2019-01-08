
#include "world/util/Raytrace.h"

#include "Common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vox::world::util
{
	TEST_CLASS(RaytraceTest)
	{
	public:
		TEST_METHOD(Raytrace_ctor)
		{
			Raytrace rayA{ glm::ivec3{ -2, 0, 2 }, glm::ivec3{ 3, 1, 4 } };
			Raytrace rayB{ glm::vec3{ 0.5f, 1.25f, 0.25f }, glm::vec3{ -0.5f, -20.0f, 0.0f } };
			Raytrace rayC{ glm::vec3{ -0.5f, 5.31f, -1.05f }, glm::vec3{ -1.0f, -0.0f, 0.0f }, 5.0f };

			Assert::AreEqual({ -1.5f, 0.5f, 2.5f }, rayA.getPos());
			Assert::AreEqual({ 0.5f, 1.25f, 0.25f }, rayB.getPos());
			Assert::AreEqual({ -0.5f, 5.31f, -1.05f }, rayC.getPos());
		}
		TEST_METHOD(Raytrace_valid)
		{
			Raytrace rayA{ glm::ivec3{ 0, 0, 0 }, glm::ivec3{ 0, 0, 0 } };
			Raytrace rayB{ glm::ivec3{ 0, 0, 0 }, glm::ivec3{ 1, 0, 0 } };

			Assert::IsFalse(rayA.valid());
			Assert::IsTrue(rayB.valid());
			rayB.next();
			Assert::IsFalse(rayB.valid());
		}

		TEST_METHOD(Raytrace_getPos)
		{
			Raytrace ray{ glm::ivec3{ 0, 0, 0 }, glm::ivec3{ -1, 0, 0 } };
			ray.next();

			Assert::AreEqual({ -0.5f, 0.5f, 0.5f }, ray.getPos());
			Assert::AreEqual({ 0.5f, 0.5f, 0.5f }, ray.getOldPos());
			Assert::AreEqual({ -1, 0, 0 }, ray.getBlockPos());
			Assert::AreEqual({ 0, 0, 0 }, ray.getOldBlockPos());
		}

		TEST_METHOD(Raytrace_next)
		{
			Raytrace ray{ glm::ivec3{ 0, 0, 0 }, glm::ivec3{ 0, -4, -6 } };

			ray.next();
			Assert::AreNotEqual({ 0.5f, 0.5f, 0.5f }, ray.getPos());
		}
		TEST_METHOD(Raytrace_nextPos)
		{
			Raytrace ray{ glm::ivec3{ 0, 0, 0 }, glm::ivec3{ 4, 0, 1 } };

			Assert::AreEqual({ 1.5f, 0.5f, 0.5f }, ray.nextPos());
			Assert::AreEqual({ 2.5f, 0.5f, 0.5f }, ray.nextPos());
			Assert::AreEqual({ 2.5f, 0.5f, 1.5f }, ray.nextPos());
			Assert::AreEqual({ 3.5f, 0.5f, 1.5f }, ray.nextPos());
			Assert::AreEqual({ 4.5f, 0.5f, 1.5f }, ray.nextPos());
		}
		TEST_METHOD(Raytrace_nextBlockPos)
		{
			Raytrace ray{ glm::ivec3{ 0, 0, 0 }, glm::ivec3{ -3, 2, 0 } };

			Assert::AreEqual({ -1, 0, 0 }, ray.nextBlockPos());
			Assert::AreEqual({ -1, 1, 0 }, ray.nextBlockPos());
			Assert::AreEqual({ -2, 1, 0 }, ray.nextBlockPos());
			Assert::AreEqual({ -2, 2, 0 }, ray.nextBlockPos());
			Assert::AreEqual({ -3, 2, 0 }, ray.nextBlockPos());
		}

		TEST_METHOD(Raytrace_iterating)
		{
			Raytrace ray{ { 10.0f, 10.0f, 10.0f }, { 1.0f, 0.0f, 0.0f }, 15.0f };

			for (unsigned int i = 0u; ray.valid(); ray.next(), ++i)
				Assert::IsTrue(i < 15u);
		}
	};

	TEST_CLASS(RaytraceBresenhamTest)
	{
	public:
		TEST_METHOD(RaytraceBresenham_ctor)
		{
			RaytraceBresenham rayA{ glm::ivec3{ -2, 0, 2 }, glm::ivec3{ 3, 1, 4 } };
			RaytraceBresenham rayB{ glm::vec3{ 0.5f, 1.25f, 0.25f }, glm::vec3{ -0.5f, -20.0f, 0.0f } };
			RaytraceBresenham rayC{ glm::vec3{ -0.5f, 5.31f, -1.05f }, glm::vec3{ -1.0f, -0.0f, 0.0f }, 5.0f };

			Assert::AreEqual({ -2, 0, 2 }, rayA.getPos());
			Assert::AreEqual({ 0, 1, 0 }, rayB.getPos());
			Assert::AreEqual({ -1, 5, -2 }, rayC.getPos());
		}
		TEST_METHOD(RaytraceBresenham_valid)
		{
			RaytraceBresenham rayA{ glm::ivec3{ 0, 0, 0 }, glm::ivec3{ 0, 0, 0 } };
			RaytraceBresenham rayB{ glm::ivec3{ 0, 0, 0 }, glm::ivec3{ 1, 0, 0 } };

			Assert::IsTrue(rayA.valid());
			rayA.next();
			Assert::IsFalse(rayA.valid());

			Assert::IsTrue(rayB.valid());
			rayB.next();
			Assert::IsTrue(rayB.valid());
			rayB.next();
			Assert::IsFalse(rayB.valid());
		}

		TEST_METHOD(RaytraceBresenham_getPos)
		{
			RaytraceBresenham ray{ glm::ivec3{ 0, 0, 0 }, glm::ivec3{ -1, 0, 0 } };
			ray.next();

			Assert::AreEqual({ -1, 0, 0 }, ray.getPos());
			Assert::AreEqual({ 0, 0, 0 }, ray.getOldPos());
		}

		TEST_METHOD(RaytraceBresenham_next)
		{
			RaytraceBresenham ray{ glm::ivec3{ 0, 0, 0 }, glm::ivec3{ 0, -4, -6 } };

			ray.next();
			Assert::AreNotEqual({ 0, 0, 0 }, ray.getPos());
		}
		TEST_METHOD(RaytraceBresenham_nextPos)
		{
			RaytraceBresenham ray{ glm::ivec3{ 0, 0, 0 }, glm::ivec3{ 5, 0, -2 } };

			Assert::AreEqual({ 1, 0, 0 }, ray.nextPos());
			Assert::AreEqual({ 2, 0, -1 }, ray.nextPos());
			Assert::AreEqual({ 3, 0, -1 }, ray.nextPos());
			Assert::AreEqual({ 4, 0, -2 }, ray.nextPos());
			Assert::AreEqual({ 5, 0, -2 }, ray.nextPos());
		}
	};
}