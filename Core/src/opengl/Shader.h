#pragma once

#include "opengl/Enums.h"

#include <string>

namespace core
{
	class Shader
	{
	public:
		Shader() = delete;
		Shader(ShaderType type);
		Shader(const Shader &) = delete;
		Shader(Shader && other);
		~Shader();

		Shader & operator=(const Shader &) = delete;
		Shader & operator=(Shader && other);

		bool compile(const std::string & code) const;

		inline auto handle() const { return m_handle; }

	private:
		unsigned int m_handle = 0u;
	};
}