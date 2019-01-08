#pragma once

#include "opengl/Enums.h"
#include "opengl/UBO.h"

#include <optional>
#include <string>
#include <vector>
#include <unordered_map>

namespace core
{
	class UBORegistry
	{
	public:
		UBORegistry() = default;
		UBORegistry(const UBORegistry &) = delete;
		UBORegistry(UBORegistry &&) = default;
		~UBORegistry() = default;

		UBORegistry & operator=(const UBORegistry &) = delete;
		UBORegistry & operator=(UBORegistry &&) = default;
		
		/**
			Checks whether the uniform buffer object block exists or not.

			@param block The block to check if exists.
			@return True iff the block was found.
		*/
		bool has(const std::string & block) const;
		/**
			Checks whether the block contains the provided member or not.

			@param block The block to look into.
			@param member The member to check if exists.
			@return True iff the member was found.
		*/
		bool has(const std::string & block, const std::string & member) const;

		/**
			Adds a new member to a UBO block. If the block does not exist, it will be created. The
			block cannot be used before it has been built.

			@param T The data type which should be added to the block.
			@param block The name of the block to operate on.
			@param member The name of the member to add.
			@param True iff the member was added.
		*/
		template<typename T>
		bool add(const std::string & block, const std::string & member);
		/**
			Finalizes the block, sending information about the UBO to OpenGl. The block must be 
			built before it is used. Once it has been built, the UBO is considered immutable and
			must not be changed in any way.

			@param The block which should be built.
		*/
		bool build(const std::string & block);

		/**
			Retrieves data stored in the given block under the given member. The data will be
			written to the provided data field. The block must be built before attempting to read
			data from it.

			@param T The data type which should be retrieved from the block.
			@param member The name of the member to read data from.
			@param data [out] The data stored in the UBO.
			@param True iff the member data was retrieved properly.
		*/
		template<typename T>
		bool get(const std::string & block, const std::string & member, T & data) const;
		/**
			Attempts to store the provided data in the member of the given block. The block must be
			built before attempting to write data to it.

			@param T The data type which should written to the UBO.
			@param member The name of the member to write data to.
			@param data The data which should be written to the UBO.
			@param True iff the member data was written properly.
		*/
		template<typename T>
		bool set(const std::string & block, const std::string & member, const T & data) const;

		/**
			Retrieves the binding port for all ubos, along with the identifier they were registered
			under. Used when binding shader programs to ubos.

			@return The identifiers and ports of ubos.
		*/
		std::unordered_map<std::string, unsigned int> bindings() const;

	private:
		UBO & getBlock(const std::string & block);
		const UBO & getBlock(const std::string & block) const;

		std::unordered_map<std::string, UBO> m_ubos;
	};
}

namespace core
{
	template<typename T>
	inline bool UBORegistry::add(const std::string & block, const std::string & member)
	{
		return getBlock(block).add<T>(member);
	}
	template<typename T>
	inline bool UBORegistry::get(const std::string & block, const std::string & member, T & data) const
	{
		return has(block) ? getBlock(block).get<T>(member, data) : false;
	}
	template<typename T>
	inline bool UBORegistry::set(const std::string & block, const std::string & member, const T & data) const
	{
		return has(block) ? getBlock(block).set<T>(member, data) : false;
	}
}