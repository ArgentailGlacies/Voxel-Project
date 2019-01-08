#pragma once

#include "CppUnitTest.h"

#include <glm/glm.hpp>

namespace
{
	template<typename T>
	std::string to_string(const glm::tvec2<T>& v)
	{
		return std::to_string(v.x) + ", " + std::to_string(v.y);
	}
	template<typename T>
	std::string to_string(const glm::tvec3<T>& v)
	{
		return std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z);
	}
	template<typename T>
	std::string to_string(const glm::tvec4<T>& v)
	{
		return std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ", " + std::to_string(v.w);
	}

	template<typename T>
	std::string to_string(const glm::tmat4x4<T>& m)
	{
		return to_string(m[0]) + "; " + to_string(m[1]) + "; " + to_string(m[2]) + "; " + to_string(m[3]);
	}
}

namespace Microsoft::VisualStudio::CppUnitTestFramework
{

	template<> inline std::wstring ToString(const glm::bvec2& v)
	{
		RETURN_WIDE_STRING(to_string(v).c_str());
	}
	template<> inline std::wstring ToString(const glm::dvec2& v)
	{
		RETURN_WIDE_STRING(to_string(v).c_str());
	}
	template<> inline std::wstring ToString(const glm::ivec2& v)
	{
		RETURN_WIDE_STRING(to_string(v).c_str());
	}
	template<> inline std::wstring ToString(const glm::uvec2& v)
	{
		RETURN_WIDE_STRING(to_string(v).c_str());
	}
	template<> inline std::wstring ToString(const glm::vec2& v)
	{
		RETURN_WIDE_STRING(to_string(v).c_str());
	}
	template<> inline std::wstring ToString(const glm::bvec3& v)
	{
		RETURN_WIDE_STRING(to_string(v).c_str());
	}
	template<> inline std::wstring ToString(const glm::dvec3& v)
	{
		RETURN_WIDE_STRING(to_string(v).c_str());
	}
	template<> inline std::wstring ToString(const glm::ivec3& v)
	{
		RETURN_WIDE_STRING(to_string(v).c_str());
	}
	template<> inline std::wstring ToString(const glm::uvec3& v)
	{
		RETURN_WIDE_STRING(to_string(v).c_str());
	}
	template<> inline std::wstring ToString(const glm::vec3& v)
	{
		RETURN_WIDE_STRING(to_string(v).c_str());
	}
	template<> inline std::wstring ToString(const glm::bvec4& v)
	{
		RETURN_WIDE_STRING(to_string(v).c_str());
	}
	template<> inline std::wstring ToString(const glm::dvec4& v)
	{
		RETURN_WIDE_STRING(to_string(v).c_str());
	}
	template<> inline std::wstring ToString(const glm::ivec4& v)
	{
		RETURN_WIDE_STRING(to_string(v).c_str());
	}
	template<> inline std::wstring ToString(const glm::uvec4& v)
	{
		RETURN_WIDE_STRING(to_string(v).c_str());
	}
	template<> inline std::wstring ToString(const glm::vec4& v)
	{
		RETURN_WIDE_STRING(to_string(v).c_str());
	}

	template<> inline std::wstring ToString(const glm::mat4x4& m)
	{
		RETURN_WIDE_STRING(to_string(m).c_str());
	}

}
