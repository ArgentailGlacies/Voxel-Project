
#include "world/Query.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <memory>

namespace vox::world
{
	TEST_CLASS(QueryTest)
	{
	public:
		TEST_METHOD(Query_readBlock)
		{
			const auto query = readBlock({ 1, 3, 4 });

			Assert::AreEqual(1u, query.count());
			Assert::IsTrue(query.has({ 1, 3, 4 }));
		}
		TEST_METHOD(Query_readCylinder)
		{
			const auto queryA = readCylinder({ 1, 3, 5 }, { -1, 4, 5 }, util::Axis::Z);
			const auto queryB = readCylinder({ 1, 3, 5 }, { -1, 3, 3 }, util::Axis::Y);
			const auto queryC = readCylinder({ 1, -1, 5 }, { 1, 3, 1 }, util::Axis::X);

			Assert::AreEqual(6u, queryA.count());
			Assert::IsTrue(queryA.has({ -1, 3, 5 }));
			Assert::IsTrue(queryA.has({ -1, 4, 5 }));
			Assert::IsTrue(queryA.has({ 0, 3, 5 }));
			Assert::IsTrue(queryA.has({ 0, 4, 5 }));
			Assert::IsTrue(queryA.has({ 1, 3, 5 }));
			Assert::IsTrue(queryA.has({ 1, 4, 5 }));

			Assert::AreEqual(9u, queryB.count());
			Assert::IsTrue(queryB.has({ -1, 3, 4 }));
			Assert::IsTrue(queryB.has({ 0, 3, 3 }));
			Assert::IsTrue(queryB.has({ 0, 3, 4 }));
			Assert::IsTrue(queryB.has({ 0, 3, 5 }));
			Assert::IsTrue(queryB.has({ 1, 3, 4 }));

			Assert::AreEqual(21u, queryC.count());
		}
		TEST_METHOD(Query_readEllipse)
		{
			const auto queryA = readEllipse({ -1, -1, -2 }, { 1, 1, 2 });
			const auto queryB = readEllipse({ 0, 0, 0 }, { 2, 1, 1 });
			const auto queryC = readEllipse({ 5, 2, -7 }, { 8, 5, -7 });

			Assert::AreEqual(21u, queryA.count());
			Assert::AreEqual(12u, queryB.count());
			Assert::AreEqual(12u, queryC.count());
		}
		TEST_METHOD(Query_readRectangle)
		{
			const auto query = readRectangle({ 1, 3, 4 }, { -1, 3, 3 });

			Assert::AreEqual(6u, query.count());
			Assert::IsTrue(query.has({ -1, 3, 3 }));
			Assert::IsTrue(query.has({ 0, 3, 3 }));
			Assert::IsTrue(query.has({ 1, 3, 3 }));
			Assert::IsTrue(query.has({ -1, 3, 4 }));
			Assert::IsTrue(query.has({ 0, 3, 4 }));
			Assert::IsTrue(query.has({ 1, 3, 4 }));
		}
		TEST_METHOD(Query_readLine)
		{
			const auto query = readLine({ -2, 2, 1 }, { 5, -1, 0 });

			Assert::AreEqual(8u, query.count());
			Assert::IsTrue(query.has({ -2, 2, 1 }));
			Assert::IsTrue(query.has({ -1, 2, 1 }));
			Assert::IsTrue(query.has({ 0, 1, 1 }));
			Assert::IsTrue(query.has({ 1, 1, 1 }));
			Assert::IsTrue(query.has({ 2, 0, 0 }));
			Assert::IsTrue(query.has({ 3, 0, 0 }));
			Assert::IsTrue(query.has({ 4, -1, 0 }));
			Assert::IsTrue(query.has({ 5, -1, 0 }));
		}
		TEST_METHOD(Query_readSphere)
		{
			const auto queryA = readSphere({ 3, -1, 4 }, 2);
			const auto queryB = readSphere({ -1, 0, 3 }, 3);
			const auto queryC = readSphere({ 4, -7, 2 }, 5);
			const auto queryD = readSphere({ 2, -1, 3 }, 0);

			Assert::AreEqual(8u, queryA.count());
			Assert::AreEqual(19u, queryB.count());
			Assert::AreEqual(81u, queryC.count());
			Assert::AreEqual(1u, queryD.count());
		}
	};
}