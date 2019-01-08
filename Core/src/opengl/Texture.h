#pragma once

#include "opengl/Enums.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace core
{
	class Texture
	{
	public:
		Texture() = delete;
		Texture(TextureType type, DataFormat data, ColorFormat color);
		Texture(const Texture &) = delete;
		Texture(Texture &&) = delete;
		~Texture();

		Texture & operator=(const Texture &) = delete;
		Texture & operator=(Texture &&) = delete;

		bool bind() const;
		bool unbind() const;

		inline auto handle() const { return m_handle; }

		// ...

		bool reserve(unsigned int size, DataFormat format, ColorFormat color, const void * data = nullptr) const;
		bool reserve(const glm::uvec2 & size, DataFormat format, ColorFormat color, const void * data = nullptr) const;
		bool reserve(const glm::uvec3 & size, DataFormat format, ColorFormat color, const void * data = nullptr) const;
		bool write(unsigned int offset, unsigned int size, DataFormat format, ColorFormat color, const void * data) const;
		bool write(const glm::uvec2 & offset, const glm::uvec2 & size, DataFormat format, ColorFormat color, const void * data) const;
		bool write(const glm::uvec3 & offset, const glm::uvec3 & size, DataFormat format, ColorFormat color, const void * data) const;

		bool generateMipmaps() const;

		bool setBorderColor(const glm::vec4 & color) const;
		bool setFilter(FilterType min, FilterType mag) const;
		bool setWrap(WrapType type) const;

	private:
		TextureType m_type;
		DataFormat m_data;
		ColorFormat m_color;

		unsigned int m_handle = 0;
	};
}