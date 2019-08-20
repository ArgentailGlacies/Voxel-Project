#pragma once

#include "opengl/Enums.h"

namespace core
{
	class VBO
	{
	public:
		VBO() = delete;
		VBO(BufferType type, BufferUse use = BufferUse::STATIC_DRAW);
		VBO(const VBO &) = delete;
		VBO(VBO &&) = delete;
		~VBO();

		VBO & operator=(const VBO &) = delete;
		VBO & operator=(VBO &&) = delete;

		bool bind() const;
		bool unbind() const;
		static bool reset(BufferType type);

		bool reserve(unsigned int bytes, const void * data = nullptr) const;
		bool write(unsigned int bytes, const void * data, unsigned int offset = 0u) const;
		bool read(unsigned int bytes, void * data, unsigned int offset = 0u) const;

		inline auto handle() const { return m_handle; }

	private:
		BufferType m_type;
		BufferUse m_use;

		unsigned int m_handle = 0;
	};
}