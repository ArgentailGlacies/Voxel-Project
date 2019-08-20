
#include "mock/MockAssetRegistry.h"
#include "mock/MockUBORegistry.h"
#include "scene/Scene.h"

#include "Context.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::scene
{
	TEST_CLASS(SceneTest)
	{
	public:
		TEST_METHOD(Scene_has)
		{
			Scene scene{ m_assets, display(), m_ubos };
			const auto nodeA = scene.createRender("", "", []() {}, Scene::DEFAULT_ENTRY);
			const auto nodeB = scene.createRender("", "", []() {}, Scene::DEFAULT_ENTRY);
			scene.detach(nodeB, Scene::DEFAULT_ENTRY);

			Assert::IsTrue(scene.has(nodeA));
			Assert::IsFalse(scene.has(nodeB));
		}

		TEST_METHOD(Scene_attach)
		{
			int countA = 0;
			int countB = 0;

			Scene scene{ m_assets, display(), m_ubos };
			const auto nodeA = scene.createRender("", "", [&]() { countA++; }, Scene::DEFAULT_ENTRY);
			const auto nodeB = scene.createRender("", "", [&]() { countB++; }, Scene::DEFAULT_ENTRY);
			scene.attach(nodeB, nodeA);
			scene.render(0.0f);

			Assert::AreEqual(1, countA);
			Assert::AreEqual(2, countB);
		}
		TEST_METHOD(Scene_detach)
		{
			int countA = 0;
			int countB = 0;
			int countC = 0;

			Scene scene{ m_assets, display(), m_ubos };
			const auto camera = scene.createCamera();
			const auto nodeA = scene.createRender("", "", [&]() { countA++; }, Scene::DEFAULT_ENTRY);
			const auto nodeB = scene.createRender("", "", [&]() { countB++; }, Scene::DEFAULT_ENTRY);
			const auto nodeC = scene.createRender("", "", [&]() { countC++; }, Scene::DEFAULT_ENTRY);
			scene.attach(nodeB, camera);
			scene.detach(nodeB, Scene::DEFAULT_ENTRY);
			scene.detach(nodeC, Scene::DEFAULT_ENTRY);
			scene.render(0.0f);

			Assert::AreEqual(2, countA);
			Assert::AreEqual(1, countB);
			Assert::AreEqual(0, countC);
		}
		TEST_METHOD(Scene_detachChildren)
		{
			Scene scene{ m_assets, display(), m_ubos };
			const auto nodeA = scene.createTransformation(Scene::DEFAULT_ENTRY);
			const auto nodeB = scene.createTransformation(nodeA);
			const auto nodeC = scene.createTransformation(nodeB);
			const auto nodeD = scene.createTransformation(nodeC);
			scene.detach(nodeB, nodeA);

			Assert::IsTrue(scene.has(nodeA));
			Assert::IsFalse(scene.has(nodeB));
			Assert::IsFalse(scene.has(nodeC));
			Assert::IsFalse(scene.has(nodeD));
		}
		TEST_METHOD(Scene_detachIllegal)
		{
			Scene scene{ m_assets, display(), m_ubos };
			scene.detach(Scene::DEFAULT_CAMERA, 0); // Very illegal and should not happen
			scene.detach(Scene::DEFAULT_ENTRY, Scene::DEFAULT_CAMERA);

			Assert::IsTrue(scene.has(Scene::DEFAULT_CAMERA));
			Assert::IsTrue(scene.has(Scene::DEFAULT_ENTRY));
		}

		TEST_METHOD(Scene_createNode)
		{
			Scene scene{ m_assets, display(), m_ubos };
			const auto cameraA = scene.createNode(Scene::DEFAULT_ENTRY);
			const auto cameraB = scene.createNode(Scene::DEFAULT_ENTRY);

			Assert::AreNotEqual(cameraA, cameraB);
		}

		TEST_METHOD(Scene_createCamera)
		{
			Scene scene{ m_assets, display(), m_ubos };
			const auto cameraA = scene.createCamera();
			const auto cameraB = scene.createCamera();

			Assert::AreNotEqual(cameraA, cameraB);
		}
		TEST_METHOD(Scene_destroyCamera)
		{
			Scene scene{ m_assets, display(), m_ubos };
			const auto cameraA = scene.createCamera();
			const auto cameraB = scene.createCamera();
			const auto node = scene.createTransformation(Scene::DEFAULT_ENTRY);
			scene.destroyCamera(Scene::DEFAULT_CAMERA);
			scene.destroyCamera(cameraA);

			Assert::IsTrue(scene.has(Scene::DEFAULT_CAMERA));
			Assert::IsFalse(scene.has(cameraA));
			Assert::IsTrue(scene.has(cameraB));
			Assert::ExpectException<std::invalid_argument>([&]() { scene.destroyCamera(node); });
		}
		TEST_METHOD(Scene_getCamera)
		{
			Scene scene{ m_assets, display(), m_ubos };
			const auto camera = scene.createCamera();
			const auto node = scene.createTransformation(Scene::DEFAULT_ENTRY);
			
			Assert::IsFalse(&scene.getCamera(camera) == &scene.getCamera(Scene::DEFAULT_CAMERA));
			Assert::ExpectException<std::invalid_argument>([&]() { scene.getCamera(node); });
		}

		TEST_METHOD(Scene_createTransformation)
		{
			Scene scene{ m_assets, display(), m_ubos };
			const auto transformationA = scene.createTransformation(Scene::DEFAULT_ENTRY);
			const auto transformationB = scene.createTransformation(Scene::DEFAULT_ENTRY);

			Assert::AreNotEqual(transformationA, transformationB);
		}
		TEST_METHOD(Scene_setTransformation)
		{
			Scene scene{ m_assets, display(), m_ubos };
			const auto transformation = scene.createTransformation(Scene::DEFAULT_ENTRY);
			const auto node = scene.createNode(Scene::DEFAULT_ENTRY);

			scene.setTransformation(transformation, glm::mat4{ 2.0f });
			Assert::ExpectException<std::invalid_argument>([&] { scene.setTransformation(node, {}); });
		}

		TEST_METHOD(Scene_createRender)
		{
			Scene scene{ m_assets, display(), m_ubos };
			const auto renderA = scene.createRender("", "", []() {}, Scene::DEFAULT_ENTRY);
			const auto renderB = scene.createRender("", "", []() {}, Scene::DEFAULT_ENTRY);

			Assert::AreNotEqual(renderA, renderB);
		}
		TEST_METHOD(Scene_setTranslucency)
		{
			Scene scene{ m_assets, display(), m_ubos };
			const auto render = scene.createRender("", "", []() {}, Scene::DEFAULT_ENTRY);
			const auto node = scene.createNode(Scene::DEFAULT_ENTRY);

			scene.setTranslucency(render, Translucency::OPAQUE);
			Assert::ExpectException<std::invalid_argument>([&] { scene.setTranslucency(node, Translucency::OPAQUE); });
		}
		TEST_METHOD(Scene_setViewportLayer)
		{
			Scene scene{ m_assets, display(), m_ubos };
			const auto render = scene.createRender("", "", []() {}, Scene::DEFAULT_ENTRY);
			const auto node = scene.createNode(Scene::DEFAULT_ENTRY);

			scene.setViewportLayer(render, ViewportLayer::WORLD);
			Assert::ExpectException<std::invalid_argument>([&] { scene.setViewportLayer(node, ViewportLayer::WORLD); });
		}
		TEST_METHOD(Scene_setFullscreenLayer)
		{
			Scene scene{ m_assets, display(), m_ubos };
			const auto render = scene.createRender("", "", []() {}, Scene::DEFAULT_ENTRY);
			const auto node = scene.createNode(Scene::DEFAULT_ENTRY);

			scene.setFullscreenLayer(render, FullscreenLayer::GAME);
			Assert::ExpectException<std::invalid_argument>([&] { scene.setFullscreenLayer(node, FullscreenLayer::GAME); });
		}

	private:
		AssetRegistry m_assets = mockAssetRegistry();
		UBORegistry m_ubos = mockUBORegistry();
	};
}