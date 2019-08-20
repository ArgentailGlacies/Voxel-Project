
#include "scene/RenderKey.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::scene
{
	TEST_CLASS(RenderKeyTest)
	{
	public:
		TEST_METHOD(RenderKey_compileOpaque)
		{
			RenderKey keyA, keyB, keyC, keyD, keyE;

			keyA.setViewportLayer(ViewportLayer::SKYBOX);
			keyA.setTranslucency(Translucency::OPAQUE);
			keyA.setProgram(4);
			keyA.setTexture(1);
			keyA.setDepth(9);

			keyB.setViewportLayer(ViewportLayer::WORLD);
			keyB.setTranslucency(Translucency::OPAQUE);
			keyB.setProgram(2);
			keyB.setTexture(3);
			keyB.setDepth(7);

			keyC.setViewportLayer(ViewportLayer::WORLD);
			keyC.setTranslucency(Translucency::OPAQUE);
			keyC.setProgram(2);
			keyC.setTexture(2);
			keyC.setDepth(3);

			keyD.setViewportLayer(ViewportLayer::WORLD);
			keyD.setTranslucency(Translucency::OPAQUE);
			keyD.setProgram(2);
			keyD.setTexture(2);
			keyD.setDepth(7);

			keyE.setFullscreenLayer(FullscreenLayer::GUI);

			Assert::IsTrue(keyA.compile() < keyB.compile());
			Assert::IsTrue(keyA.compile() < keyC.compile());
			Assert::IsTrue(keyA.compile() < keyD.compile());
			Assert::IsTrue(keyA.compile() < keyE.compile());
			Assert::IsTrue(keyB.compile() > keyC.compile());
			Assert::IsTrue(keyB.compile() > keyD.compile());
			Assert::IsTrue(keyB.compile() < keyE.compile());
			Assert::IsTrue(keyC.compile() < keyD.compile());
			Assert::IsTrue(keyC.compile() < keyE.compile());
			Assert::IsTrue(keyD.compile() < keyE.compile());
		}
		TEST_METHOD(RenderKey_compileTransparent)
		{
			RenderKey keyA, keyB, keyC, keyD, keyE;

			keyA.setViewportLayer(ViewportLayer::SKYBOX);
			keyA.setTranslucency(Translucency::TRANSPARENT);
			keyA.setDepth(9);
			keyA.setProgram(4);
			keyA.setTexture(1);

			keyB.setViewportLayer(ViewportLayer::WORLD);
			keyB.setTranslucency(Translucency::TRANSPARENT);
			keyB.setDepth(7);
			keyB.setProgram(2);
			keyB.setTexture(3);

			keyC.setViewportLayer(ViewportLayer::WORLD);
			keyC.setTranslucency(Translucency::TRANSPARENT);
			keyC.setDepth(3);
			keyC.setProgram(2);
			keyC.setTexture(2);

			keyD.setViewportLayer(ViewportLayer::WORLD);
			keyD.setTranslucency(Translucency::TRANSPARENT);
			keyD.setDepth(7);
			keyD.setProgram(2);
			keyD.setTexture(2);

			keyE.setFullscreenLayer(FullscreenLayer::GUI);

			Assert::IsTrue(keyA.compile() < keyB.compile());
			Assert::IsTrue(keyA.compile() < keyC.compile());
			Assert::IsTrue(keyA.compile() < keyD.compile());
			Assert::IsTrue(keyA.compile() < keyE.compile());
			Assert::IsTrue(keyB.compile() < keyC.compile());
			Assert::IsTrue(keyB.compile() > keyD.compile());
			Assert::IsTrue(keyB.compile() < keyE.compile());
			Assert::IsTrue(keyC.compile() > keyD.compile());
			Assert::IsTrue(keyC.compile() < keyE.compile());
			Assert::IsTrue(keyD.compile() < keyE.compile());
		}
	};
}