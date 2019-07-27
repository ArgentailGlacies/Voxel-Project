#pragma once

namespace core
{
	class VAO
	{
	public:
		VAO();
		VAO(const VAO &) = delete;
		VAO(VAO &&) = delete;
		~VAO();

		VAO & operator=(const VAO &) = delete;
		VAO & operator=(VAO &&) = delete;

		bool bind() const;
		bool unbind() const;
		static bool reset();

		inline auto handle() const { return m_handle; }

	private:
		unsigned int m_handle = 0;
	};
}