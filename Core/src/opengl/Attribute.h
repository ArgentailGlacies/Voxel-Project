#pragma once

#include "opengl/Enums.h"

namespace core
{
	/**
		Enables an attribute in a shader program.

		Only the following data formats are accepted:
		BYTE, FLOAT, INT, SHORT, UNSIGNED_BYTE, UNSIGNED_INT, UNSIGNED_SHORT

		@param index The index of the attribute to enable.
		@param count The number of values passed into the shader by the attribute.
		@param offset The byte offset of the attribute data in the vertex.
		@param stride The byte size of the vertex.
		@param format The data format passed in.
	*/
	class Attribute
	{
	public:
		Attribute() = delete;
		Attribute(
			unsigned int index,
			unsigned int count,
			unsigned int offset,
			unsigned int stride,
			DataFormat format
		);

		/**
			Enables the attribute such that it may be used in a shader program. The attribute must
			only be enabled while a VAO and a VBO is currently bound.
		*/
		void enable() const;
		/**
			Disables the attribute such that it may no longer be used in a shader program. The
			attribute must only be disabled while a VAO and a VBO is currently bound.
		*/
		void disable() const;

	private:
		unsigned int m_index;
		unsigned int m_count;
		unsigned int m_offset;
		unsigned int m_stride;
		DataFormat m_format;
	};
}