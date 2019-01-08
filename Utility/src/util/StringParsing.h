#pragma once

#include "util/StringOperations.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <string>
#include <vector>

namespace util
{
	template<typename T>
	struct Parser
	{
		inline T parse(const std::string & string, const T & def) const
		{
			static_assert(false, "No overloaded parser found");
		}
	};

	// ...

	template<> struct Parser<bool>
	{
		inline bool parse(const std::string & string, const bool & def = false) const
		{
			switch (string[0])
			{
			case 't': case 'T': case 'y': case 'Y': case '1':
				return true;
			case 'f': case 'F': case 'n': case 'N': case '0':
				return false;
			default:
				return def;
			}
		}
	};
	template<> struct Parser<double>
	{
		inline double parse(const std::string & string, const double & def = 0.0) const
		{
			try { return std::stod(string); }
			catch (...) { return def; }
		}
	};
	template<> struct Parser<float>
	{
		inline float parse(const std::string & string, const float & def = 0.0f) const
		{
			try { return std::stof(string); }
			catch (...) { return def; }
		}
	};
	template<> struct Parser<int>
	{
		inline int parse(const std::string & string, const int & def = 0) const
		{
			try
			{
				int base = 10;
				if (string.find("0x") != std::string::npos || string.find("0X") != std::string::npos)
					base = 16;
				return std::stoi(string, 0u, base);
			}
			catch (...) { return def; }
		}
	};
	template<> struct Parser<unsigned int>
	{
		inline unsigned int parse(const std::string & string, const unsigned int & def = 0u) const
		{
			try
			{
				int base = 10;
				if (string.find("0x") != std::string::npos || string.find("0X") != std::string::npos)
					base = 16;
				return std::stoul(string, 0u, base);
			}
			catch (...) { return def; }
		}
	};
	template<> struct Parser<std::string>
	{
		inline std::string parse(const std::string & string, const std::string & def = "") const
		{
			return string;
		}
	};

	// ...

	template<typename T> struct Parser<std::vector<T>>
	{
		inline std::vector<T> parse(const std::string & string, const std::string & delim = ",") const
		{
			std::vector<T> values;
			const Parser<T> parser;
			for (const auto & part : split(string, delim))
				values.push_back(parser.parse(part));
			return values;
		}
	};

	template<typename T> struct Parser<glm::tvec2<T>>
	{
		inline glm::tvec2<T> parse(const std::string & string, const glm::tvec2<T> & def = {}) const
		{
			return parse(string, ",", def);
		}
		inline glm::tvec2<T> parse(const std::string & string, const std::string & delim, const glm::tvec2<T> & def = {}) const
		{
			const Parser<T> parser;
			const auto parts = split(string, delim);
			if (parts.size() == 2)
				return { parser.parse(parts[0], def.x), parser.parse(parts[1], def.y) };
			return def;
		}
	};
	template<typename T> struct Parser<glm::tvec3<T>>
	{
		inline glm::tvec3<T> parse(const std::string & string, const glm::tvec3<T> & def = {}) const
		{
			return parse(string, ",", def);
		}
		inline glm::tvec3<T> parse(const std::string & string, const std::string & delim, const glm::tvec3<T> & def = {}) const
		{
			const Parser<T> parser;
			const auto parts = split(string, delim);
			if (parts.size() == 3)
				return { parser.parse(parts[0], def.x), parser.parse(parts[1], def.y), parser.parse(parts[2], def.z) };
			return def;
		}
	};
	template<typename T> struct Parser<glm::tvec4<T>>
	{
		inline glm::tvec4<T> parse(const std::string & string, const glm::tvec4<T> & def = {}) const
		{
			return parse(string, ",", def);
		}
		inline glm::tvec4<T> parse(const std::string & string, const std::string & delim, const glm::tvec4<T> & def = {}) const
		{
			const Parser<T> parser;
			const auto parts = split(string, delim);
			if (parts.size() == 4)
				return { parser.parse(parts[0], def.x), parser.parse(parts[1], def.y), parser.parse(parts[2], def.z), parser.parse(parts[3], def.w) };
			return def;
		}
	};
}