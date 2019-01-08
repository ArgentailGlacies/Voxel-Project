
#include "opengl/Texture.h"

#include <glm/mat4x4.hpp>

#include "Common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::opengl
{
	TEST_CLASS(TextureTest)
	{
	public:
		TEST_METHOD(Texture_ctor)
		{
			Texture texture{ TextureType::TEXTURE_2D, DataFormat::UNSIGNED_BYTE, ColorFormat::RGBA };

			Assert::AreNotEqual(0u, texture.handle());
		}

		TEST_METHOD(Texture_bind)
		{
			Texture texture{ TextureType::TEXTURE_2D, DataFormat::UNSIGNED_BYTE, ColorFormat::RGBA };

			Assert::IsTrue(texture.bind());
			Assert::IsFalse(texture.bind());
		}
		TEST_METHOD(Texture_unbind)
		{
			Texture texture{ TextureType::TEXTURE_2D, DataFormat::UNSIGNED_BYTE, ColorFormat::RGBA };
			texture.bind();

			Assert::IsTrue(texture.unbind());
			Assert::IsFalse(texture.unbind());
		}

		TEST_METHOD(Texture_reserve)
		{
			Texture textureA{ TextureType::TEXTURE_1D, DataFormat::UNSIGNED_BYTE, ColorFormat::RGBA };
			Texture textureB{ TextureType::TEXTURE_2D, DataFormat::UNSIGNED_BYTE, ColorFormat::RGBA };
			Texture textureC{ TextureType::TEXTURE_3D, DataFormat::UNSIGNED_BYTE, ColorFormat::RGBA };

			Assert::IsTrue(textureA.reserve(100u, DataFormat::BYTE, ColorFormat::RGBA));
			Assert::IsTrue(textureB.reserve({ 32u, 32u }, DataFormat::BYTE, ColorFormat::RGBA));
			Assert::IsTrue(textureC.reserve({ 8u, 4u, 4u }, DataFormat::BYTE, ColorFormat::RGBA));
		}
		TEST_METHOD(Texture_write)
		{
			const float data[] = {
				0.25f, 0.5f, 0.75f, 1.0f,
				0.0f, 0.33f, 0.67f, 1.0f,
				0.0f, 0.0f, 0.0f, 1.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
			};

			Texture textureA{ TextureType::TEXTURE_1D, DataFormat::FLOAT, ColorFormat::RGBA };
			Texture textureB{ TextureType::TEXTURE_2D, DataFormat::FLOAT, ColorFormat::RGBA };
			Texture textureC{ TextureType::TEXTURE_3D, DataFormat::FLOAT, ColorFormat::RGBA };
			textureA.reserve(16u, DataFormat::FLOAT, ColorFormat::RGBA);
			textureB.reserve({ 4u, 4u }, DataFormat::FLOAT, ColorFormat::RGBA);
			textureC.reserve({ 2u, 2u, 2u }, DataFormat::FLOAT, ColorFormat::RGBA);

			Assert::IsTrue(textureA.write(2u, 4u, DataFormat::FLOAT, ColorFormat::RGBA, data));
			Assert::IsTrue(textureB.write({ 1u, 2u }, { 2u, 2u }, DataFormat::FLOAT, ColorFormat::RGBA, data));
			Assert::IsTrue(textureC.write({ 0u, 0u, 0u }, { 1u, 2u, 2u }, DataFormat::FLOAT, ColorFormat::RGBA, data));
		}

		TEST_METHOD(Texture_generateMipmaps)
		{
			const float data[] = {
				1.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 1.0f,
			};

			Texture texture{ TextureType::TEXTURE_1D, DataFormat::FLOAT, ColorFormat::RGB };
			texture.reserve(3u, DataFormat::FLOAT, ColorFormat::RGB, data);

			Assert::IsTrue(texture.generateMipmaps());
		}

		TEST_METHOD(Texture_setBorderColor)
		{
			Texture texture{ TextureType::TEXTURE_2D, DataFormat::UNSIGNED_BYTE, ColorFormat::RGBA };

			Assert::IsTrue(texture.setBorderColor({ 0.5f, 0.75f, 1.0f, 1.0f }));
		}
		TEST_METHOD(Texture_setFilter)
		{
			Texture texture{ TextureType::TEXTURE_2D, DataFormat::UNSIGNED_BYTE, ColorFormat::RGBA };

			Assert::IsTrue(texture.setFilter(FilterType::NEAREST_MIPMAP_LINEAR, FilterType::LINEAR));
		}
		TEST_METHOD(Texture_setWrap)
		{
			Texture texture{ TextureType::TEXTURE_2D, DataFormat::UNSIGNED_BYTE, ColorFormat::RGBA };
				
			Assert::IsTrue(texture.setWrap(WrapType::REPEAT_MIRROR));
		}
	};
}