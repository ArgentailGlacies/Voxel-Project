
#include "opengl/UBORegistry.h"
#include "scene/SceneNode.h"
#include "scene/ScheduleContext.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Context.h"
#include "Common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::scene
{
	class SceneNode_MockNode : public Node
	{
	public:
		virtual void schedule(ScheduleContext & context) const override final { scheduled = true; }

		mutable bool scheduled = false;
	};

	TEST_CLASS(SceneNodeTest)
	{
	public:
		TEST_METHOD(Node_attach)
		{
			SceneNode_MockNode mockA, mockB;

			Node node;
			node.attach(mockA);
			node.schedule(m_context);

			Assert::IsTrue(mockA.scheduled);
			Assert::IsFalse(mockB.scheduled);
		}
		TEST_METHOD(Node_detach)
		{
			SceneNode_MockNode mockA, mockB;

			Node node;
			node.attach(mockA);
			node.attach(mockB);
			node.detach(mockA);
			node.schedule(m_context);

			Assert::IsFalse(mockA.scheduled);
			Assert::IsTrue(mockB.scheduled);
		}

		TEST_METHOD(CameraNode_schedule)
		{
			Assert::Fail();
		}

		TEST_METHOD(TransformNode_schedule)
		{
			Assert::Fail();
		}

		TEST_METHOD(RenderNode_schedule)
		{
			Assert::Fail();
		}

	private:
		UBORegistry m_ubos;
		ScheduleContext m_context{ m_ubos, 0.0f };
	};
}