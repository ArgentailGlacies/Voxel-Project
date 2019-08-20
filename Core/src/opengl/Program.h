#pragma once

namespace core
{
	class Shader;

	class Program
	{
	public:
		Program();
		Program(const Program &) = delete;
		Program(Program && other);
		~Program();

		Program & operator=(const Program &) = delete;
		Program & operator=(Program && other);

		bool bind() const;
		bool unbind() const;
		static bool reset();

		bool attach(const Shader & shader) const;
		bool link() const;

		inline auto handle() const { return m_handle; }

	private:
		unsigned int m_handle = 0u;
	};
}