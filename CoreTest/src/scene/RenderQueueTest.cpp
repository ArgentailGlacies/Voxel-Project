
#include "scene/RenderQueue.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::scene
{
	TEST_CLASS(RenderQueueTest)
	{
	public:
		TEST_METHOD(RenderQueue_add)
		{
			bool added = false;

			RenderQueue queue;
			queue.add(RenderKey{}, [&]() { added = true; });
			queue.render();

			Assert::IsTrue(added);
		}

		TEST_METHOD(RenderQueue_sort)
		{
			int current = 0;
			int taskA = -1;
			int taskB = -1;
			int taskC = -1;
			int taskD = -1;
			int taskE = -1;

			RenderKey keyA, keyB, keyC, keyD, keyE;
			keyA.setViewportLayer(ViewportLayer::WORLD);
			keyB.setViewportLayer(ViewportLayer::WORLD);
			keyC.setViewportLayer(ViewportLayer::SKYBOX);
			keyD.setViewportLayer(ViewportLayer::WORLD);
			keyE.setViewportLayer(ViewportLayer::WORLD);
			keyA.setTranslucency(Translucency::TRANSPARENT);
			keyB.setTranslucency(Translucency::OPAQUE);
			keyC.setTranslucency(Translucency::OPAQUE);
			keyD.setTranslucency(Translucency::TRANSPARENT);
			keyE.setTranslucency(Translucency::OPAQUE);
			keyA.setProgram(1);
			keyB.setProgram(2);
			keyC.setProgram(0);
			keyD.setProgram(3);
			keyE.setProgram(2);
			keyA.setTexture(1);
			keyB.setTexture(2);
			keyC.setTexture(0);
			keyD.setTexture(3);
			keyE.setTexture(2);
			keyA.setDepth(2);
			keyB.setDepth(6);
			keyC.setDepth(0);
			keyD.setDepth(4);
			keyE.setDepth(7);

			RenderQueue queue;
			queue.add(keyA, [&]() { taskA = current++; });
			queue.add(keyB, [&]() { taskB = current++; });
			queue.add(keyC, [&]() { taskC = current++; });
			queue.add(keyD, [&]() { taskD = current++; });
			queue.add(keyE, [&]() { taskE = current++; });
			queue.sort();
			queue.render();

			Assert::AreEqual(0, taskC);
			Assert::AreEqual(1, taskB);
			Assert::AreEqual(2, taskE);
			Assert::AreEqual(3, taskD);
			Assert::AreEqual(4, taskA);
		}
	};
}