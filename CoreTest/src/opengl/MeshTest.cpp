
#include "opengl/Mesh.h"

#include <glm/vec3.hpp>

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::opengl
{
	TEST_CLASS(MeshTest)
	{
	public:
		TEST_METHOD(Mesh_build)
		{
			Mesh<glm::vec3> meshA;
			Mesh<glm::vec3> meshB;
			meshA.indices() = { 0, 1, 2 };
			meshA.vertices() = { { -1.0f, -1.0f, 0.0f }, { 1.0f, -1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } };

			Assert::IsTrue(meshA.build());
			Assert::IsFalse(meshB.build());
		}

		TEST_METHOD(Mesh_render)
		{
			Mesh<glm::vec3> mesh;
			mesh.indices() = { 0, 1, 2 };
			mesh.vertices() = { { -1.0f, -1.0f, 0.0f }, { 1.0f, -1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } };
			mesh.build();

			mesh.render();
		}
	};
}