
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
			RenderKey keyA, keyB, keyC, keyD;
			keyA.setViewportLayer(ViewportLayer::SKYBOX);
			keyB.setViewportLayer(ViewportLayer::WORLD);
			keyC.setViewportLayer(ViewportLayer::WORLD);
			keyD.setViewportLayer(ViewportLayer::WORLD);
			keyA.setTranslucency(Translucency::OPAQUE);
			keyB.setTranslucency(Translucency::OPAQUE);
			keyC.setTranslucency(Translucency::OPAQUE);
			keyD.setTranslucency(Translucency::OPAQUE);
			keyA.setProgram(4);
			keyB.setProgram(2);
			keyC.setProgram(2);
			keyD.setProgram(2);
			keyA.setTexture(1);
			keyB.setTexture(3);
			keyC.setTexture(2);
			keyD.setTexture(2);
			keyA.setDepth(9);
			keyB.setDepth(7);
			keyC.setDepth(3);
			keyD.setDepth(7);

			Assert::IsTrue(keyA.compile() < keyB.compile());
			Assert::IsTrue(keyA.compile() < keyC.compile());
			Assert::IsTrue(keyA.compile() < keyD.compile());
			Assert::IsTrue(keyB.compile() > keyC.compile());
			Assert::IsTrue(keyB.compile() > keyD.compile());
			Assert::IsTrue(keyC.compile() < keyD.compile());
		}
		TEST_METHOD(RenderKey_compileTransparent)
		{
			RenderKey keyA, keyB, keyC, keyD;
			keyA.setViewportLayer(ViewportLayer::SKYBOX);
			keyB.setViewportLayer(ViewportLayer::WORLD);
			keyC.setViewportLayer(ViewportLayer::WORLD);
			keyD.setViewportLayer(ViewportLayer::WORLD);
			keyA.setTranslucency(Translucency::TRANSPARENT);
			keyB.setTranslucency(Translucency::TRANSPARENT);
			keyC.setTranslucency(Translucency::TRANSPARENT);
			keyD.setTranslucency(Translucency::TRANSPARENT);
			keyA.setDepth(9);
			keyB.setDepth(7);
			keyC.setDepth(3);
			keyD.setDepth(7);
			keyA.setProgram(4);
			keyB.setProgram(2);
			keyC.setProgram(2);
			keyD.setProgram(2);
			keyA.setTexture(1);
			keyB.setTexture(3);
			keyC.setTexture(2);
			keyD.setTexture(2);

			Assert::IsTrue(keyA.compile() < keyB.compile());
			Assert::IsTrue(keyA.compile() < keyC.compile());
			Assert::IsTrue(keyA.compile() < keyD.compile());
			Assert::IsTrue(keyB.compile() < keyC.compile());
			Assert::IsTrue(keyB.compile() > keyD.compile());
			Assert::IsTrue(keyC.compile() > keyD.compile());
		}
	};
}