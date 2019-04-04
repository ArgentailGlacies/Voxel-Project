#pragma once

#include <string>
#include <unordered_map>

namespace core
{
	class GuiData
	{
	public:
		/**
			Assigns a bool value to the specified field. Any old value will be overwritten.

			@param field The field to write the value to.
			@param value The value to write into the field.
		*/
		inline void setBool(const std::string & field, bool value) { m_bools[field] = value; }
		/**
			Assigns a float value to the specified field. Any old value will be overwritten.

			@param field The field to write the value to.
			@param value The value to write into the field.
		*/
		inline void setFloat(const std::string & field, float value) { m_floats[field] = value; }
		/**
			Assigns a string value to the specified field. Any old value will be overwritten.

			@param field The field to write the value to.
			@param value The value to write into the field.
		*/
		inline void setString(const std::string & field, const std::string & value) { m_strings[field] = value; }

		/**
			Retrieves the bool value from the given field.

			@param field The field to read from.
			@return The value of the field, or false if it has not been specified.
		*/
		auto getBool(const std::string & field) const -> bool;
		/**
			Retrieves the float value from the given field.

			@param field The field to read from.
			@return The value of the field, or 0.0f if it has not been specified.
		*/
		auto getFloat(const std::string & field) const -> float;
		/**
			Retrieves the string value from the given field.

			@param field The field to read from.
			@return The value of the field, or an empty string if it has not been specified.
		*/
		auto getString(const std::string & field) const -> std::string;

	private:
		std::unordered_map<std::string, bool> m_bools;
		std::unordered_map<std::string, float> m_floats;
		std::unordered_map<std::string, std::string> m_strings;
	};
}