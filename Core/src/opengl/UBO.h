#pragma once

#include "opengl/Enums.h"
#include "opengl/VBO.h"

#include <string>
#include <unordered_map>

namespace core
{
	class UBO
	{
	public:
		UBO() = delete;
		UBO(unsigned int port, BufferUse use = BufferUse::DYNAMIC_DRAW);
		UBO(const UBO &) = delete;
		UBO(UBO &&) = delete;
		~UBO() = default;

		UBO & operator=(const UBO &) = delete;
		UBO & operator=(UBO &&) = delete;

		inline auto bind() const { return m_vbo.bind(); }
		inline auto unbind() const { return m_vbo.unbind(); }

		/**
			Checks whether the UBO contains the given member.

			@param member The member to check if exists.
			@return True iff the member exists in the UBO.
		*/
		bool has(const std::string & member) const;
		/**
			Adds the given member to the UBO. The member will only be added if the UBO has not yet
			been built. The member cannot be used before the UBO has been built.

			@param T The data type which should be added to the UBO.
			@param member The name of the member to add.
			@param True iff the member was added.
		*/
		template<typename T> bool add(const std::string & member);
		/**
			Retrieves the data stored in the UBO under the given member name, storing the data in
			the provided data element provided by the user. The UBo must be built before any data
			can be retrieved from it.

			@param T The data type which should be retrieved from the UBO.
			@param member The name of the member to read data from.
			@param data [out] The data stored in the UBO.
			@param True iff the member data was retrieved properly.
		*/
		template<typename T> bool get(const std::string & member, T & data) const;
		/**
			Stored the specified data in the UBO under the given member name. The UBo must be built
			before any data can be written to it.

			@param T The data type which should written to the UBO.
			@param member The name of the member to write data to.
			@param data The data which should be written to the UBO.
			@param True iff the member data was written properly.
		*/
		template<typename T> bool set(const std::string & member, const T & data) const;

		/**
			Finalizes the UBO, sending information about the UBO to OpenGl. The UBO must be built in
			order to be used. Once it has been built, the UBO is considered immutable and must not
			be changed in any way.
		*/
		bool build() const;

		inline auto port() const { return m_port; }
		inline auto handle() const { return m_vbo.handle(); }

	private:
		bool add(const std::string & member, unsigned int bytes);
		bool get(const std::string & member, unsigned int bytes, void * data) const;
		bool set(const std::string & member, unsigned int bytes, const void * data) const;

		VBO m_vbo;

		unsigned int m_port;
		unsigned int m_size;

		std::unordered_map<std::string, unsigned int> m_offsets;
		std::unordered_map<std::string, unsigned int> m_sizes;
	};
}

// ...

namespace core
{
	template<typename T>
	inline bool UBO::add(const std::string & member)
	{
		return add(member, sizeof(T));
	}
	template<typename T>
	inline bool UBO::get(const std::string & member, T & data) const
	{
		return get(member, sizeof(T), &data);
	}
	template<typename T>
	inline bool UBO::set(const std::string & member, const T & data) const
	{
		return set(member, sizeof(T), &data);
	}
}