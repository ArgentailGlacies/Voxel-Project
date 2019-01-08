#pragma once

#include "util/MathConstants.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace util
{
	template<typename T> constexpr T cos(const T & v)
	{
		return static_cast<T>(std::cos(v * DEG_TO_RAD<T>));
	}
	template<typename T> constexpr glm::tvec2<T> cos(const glm::tvec2<T> & v)
	{
		return { cos(v.x), cos(v.y) };
	}
	template<typename T> constexpr glm::tvec3<T> cos(const glm::tvec3<T> & v)
	{
		return { cos(v.x), cos(v.y), cos(v.z) };
	}
	template<typename T> constexpr glm::tvec4<T> cos(const glm::tvec4<T> & v)
	{
		return { cos(v.x), cos(v.y), cos(v.z), cos(v.w) };
	}
	template<typename T> constexpr T sin(const T & v)
	{
		return static_cast<T>(std::sin(v * DEG_TO_RAD<T>));
	}
	template<typename T> constexpr glm::tvec2<T> sin(const glm::tvec2<T> & v)
	{
		return { sin(v.x), sin(v.y) };
	}
	template<typename T> constexpr glm::tvec3<T> sin(const glm::tvec3<T> & v)
	{
		return { sin(v.x), sin(v.y), sin(v.z) };
	}
	template<typename T> constexpr glm::tvec4<T> sin(const glm::tvec4<T> & v)
	{
		return { sin(v.x), sin(v.y), sin(v.z), sin(v.w) };
	}
	template<typename T> constexpr T tan(const T & v)
	{
		return static_cast<T>(std::tan(v * DEG_TO_RAD<T>));
	}
	template<typename T> constexpr glm::tvec2<T> tan(const glm::tvec2<T> & v)
	{
		return { tan(v.x), tan(v.y) };
	}
	template<typename T> constexpr glm::tvec3<T> tan(const glm::tvec3<T> & v)
	{
		return { tan(v.x), tan(v.y), tan(v.z) };
	}
	template<typename T> constexpr glm::tvec4<T> tan(const glm::tvec4<T> & v)
	{
		return { tan(v.x), tan(v.y), tan(v.z), tan(v.w) };
	}

	template<typename T> constexpr T acos(const T & v)
	{
		return static_cast<T>(std::acos(v)) * RAD_TO_DEG<T>;
	}
	template<typename T> constexpr T asin(const T & v)
	{
		return static_cast<T>(std::asin(v)) * RAD_TO_DEG<T>;
	}
	template<typename T> constexpr T atan(const T & v)
	{
		return static_cast<T>(std::atan(v)) * RAD_TO_DEG<T>;
	}
	template<typename T> constexpr T atan2(const T & x, const T & y)
	{
		return static_cast<T>(std::atan2(y, x)) * RAD_TO_DEG<T>;
	}

	// ...

	template<typename T> T angle(const glm::tvec3<T> & v1, const glm::tvec3<T> & v2)
	{
		T length = glm::length(v1) * glm::length(v2);
		return length > T{ 0 } ? acos(glm::dot(v1, v2) / length) : T{ 0 };
	}

	template<typename T> glm::tvec3<T> cartesian(const T & yaw, const T & pitch)
	{
		const auto cosPitch = cos(pitch);
		return { cos(yaw) * cosPitch, sin(yaw) * cosPitch, sin(pitch) };
	}
	template<typename T> glm::tvec3<T> cartesian(const glm::tvec2<T> & v)
	{
		return cartesian(v.x, v.y);
	}
	template<typename T> glm::tvec3<T> cartesian(const glm::tvec3<T> & v)
	{
		return cartesian(v.x, v.y);
	}

	template<typename T> glm::tvec2<T> spherical(const glm::tvec3<T> & v)
	{
		glm::tvec2<T> rot;
		rot.x = atan2(v.x, v.y);
		rot.x = rot.x < T{ 0 } ? rot.x + T{ 360 } : rot.x;
		rot.y = T{ 90 } -angle(glm::tvec3<T>{ 0, 0, 1 }, v);
		return rot;
	}
}