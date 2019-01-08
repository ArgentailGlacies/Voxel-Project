#pragma once

#include <algorithm>
#include <cmath>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace util
{
	template<typename T>
	constexpr T min(const T & lhs, const T & rhs)
	{
		return std::min(lhs, rhs);
	}
	template<typename T>
	constexpr T min(const std::initializer_list<T> & values)
	{
		return std::min(values);
	}
	template<typename T>
	constexpr glm::tvec2<T> min(const glm::tvec2<T> & lhs, const glm::tvec2<T> & rhs)
	{
		return { min(lhs.x, rhs.x), min(lhs.y, rhs.y) };
	}
	template<typename T>
	constexpr T min(const glm::tvec2<T> & v)
	{
		return min({ v.x, v.y });
	}
	template<typename T>
	constexpr glm::tvec3<T> min(const glm::tvec3<T> & lhs, const glm::tvec3<T> & rhs)
	{
		return { min(lhs.x, rhs.x), min(lhs.y, rhs.y), min(lhs.z, rhs.z) };
	}
	template<typename T>
	constexpr T min(const glm::tvec3<T> & v)
	{
		return min({ v.x, v.y, v.z });
	}
	template<typename T>
	constexpr glm::tvec4<T> min(const glm::tvec4<T> & lhs, const glm::tvec4<T> & rhs)
	{
		return { min(lhs.x, rhs.x), min(lhs.y, rhs.y), min(lhs.z, rhs.z), min(lhs.w, rhs.w) };
	}
	template<typename T>
	constexpr T min(const glm::tvec4<T> & v)
	{
		return min({ v.x, v.y, v.z, v.w });
	}

	template<typename T>
	constexpr T max(const T & lhs, const T & rhs)
	{
		return std::max(lhs, rhs);
	}
	template<typename T>
	constexpr T max(const std::initializer_list<T> & values)
	{
		return std::max(values);
	}
	template<typename T>
	constexpr glm::tvec2<T> max(const glm::tvec2<T> & lhs, const glm::tvec2<T> & rhs)
	{
		return { max(lhs.x, rhs.x), max(lhs.y, rhs.y) };
	}
	template<typename T>
	constexpr T max(const glm::tvec2<T> & v)
	{
		return max({ v.x, v.y });
	}
	template<typename T>
	constexpr glm::tvec3<T> max(const glm::tvec3<T> & lhs, const glm::tvec3<T> & rhs)
	{
		return { max(lhs.x, rhs.x), max(lhs.y, rhs.y), max(lhs.z, rhs.z) };
	}
	template<typename T>
	constexpr T max(const glm::tvec3<T> & v)
	{
		return max({ v.x, v.y, v.z });
	}
	template<typename T>
	constexpr glm::tvec4<T> max(const glm::tvec4<T> & lhs, const glm::tvec4<T> & rhs)
	{
		return { max(lhs.x, rhs.x), max(lhs.y, rhs.y), max(lhs.z, rhs.z), max(lhs.w, rhs.w) };
	}
	template<typename T>
	constexpr T max(const glm::tvec4<T> & v)
	{
		return max({ v.x, v.y, v.z, v.w });
	}

	// ...

	template<typename T> constexpr T abs(const T & v)
	{
		return max(v, -v);
	}
	template<typename T> constexpr glm::tvec2<T> abs(const glm::tvec2<T> & v)
	{
		return { abs(v.x), abs(v.y) };
	}
	template<typename T> constexpr glm::tvec3<T> abs(const glm::tvec3<T> & v)
	{
		return { abs(v.x), abs(v.y), abs(v.z) };
	}
	template<typename T> constexpr glm::tvec4<T> abs(const glm::tvec4<T> & v)
	{
		return { abs(v.x), abs(v.y), abs(v.z), abs(v.w) };
	}

	template<typename T> constexpr int sign(const T & v)
	{
		return (T{} < v) - (v < T{});
	}
	template<typename T> constexpr glm::tvec2<T> sign(const glm::tvec2<T> & v)
	{
		return { sign(v.x), sign(v.y) };
	}
	template<typename T> constexpr glm::tvec3<T> sign(const glm::tvec3<T> & v)
	{
		return { sign(v.x), sign(v.y), sign(v.z) };
	}
	template<typename T> constexpr glm::tvec4<T> sign(const glm::tvec4<T> & v)
	{
		return { sign(v.x), sign(v.y), sign(v.z), sign(v.w) };
	}
	template<typename T> constexpr int signp(const T & v)
	{
		return v < T{ 0 } ? -1 : 1;
	}
	template<typename T> constexpr glm::tvec2<T> signp(const glm::tvec2<T> & v)
	{
		return { signp(v.x), signp(v.y) };
	}
	template<typename T> constexpr glm::tvec3<T> signp(const glm::tvec3<T> & v)
	{
		return { signp(v.x), signp(v.y), signp(v.z) };
	}
	template<typename T> constexpr glm::tvec4<T> signp(const glm::tvec4<T> & v)
	{
		return { signp(v.x), signp(v.y), signp(v.z), signp(v.w) };
	}

	// ...

	template<typename T> constexpr int floor(const T & v)
	{
		return static_cast<int>(std::floor(v));
	}
	template<typename T> constexpr glm::ivec2 floor(const glm::tvec2<T> & v)
	{
		return { floor(v.x), floor(v.y) };
	}
	template<typename T> constexpr glm::ivec3 floor(const glm::tvec3<T> & v)
	{
		return { floor(v.x), floor(v.y), floor(v.z) };
	}
	template<typename T> constexpr glm::ivec4 floor(const glm::tvec4<T> & v)
	{
		return { floor(v.x), floor(v.y), floor(v.z), floor(v.w) };
	}

	template<typename T> constexpr int round(const T & v)
	{
		return static_cast<int>(std::round(v));
	}
	template<typename T> constexpr glm::ivec2 round(const glm::tvec2<T> & v)
	{
		return { round(v.x), round(v.y) };
	}
	template<typename T> constexpr glm::ivec3 round(const glm::tvec3<T> & v)
	{
		return { round(v.x), round(v.y), round(v.z) };
	}
	template<typename T> constexpr glm::ivec4 round(const glm::tvec4<T> & v)
	{
		return { round(v.x), round(v.y), round(v.z), round(v.w) };
	}

	template<typename T> constexpr int ceil(const T & v)
	{
		return static_cast<int>(std::ceil(v));
	}
	template<typename T> constexpr glm::ivec2 ceil(const glm::tvec2<T> & v)
	{
		return { ceil(v.x), ceil(v.y) };
	}
	template<typename T> constexpr glm::ivec3 ceil(const glm::tvec3<T> & v)
	{
		return { ceil(v.x), ceil(v.y), ceil(v.z) };
	}
	template<typename T> constexpr glm::ivec4 ceil(const glm::tvec4<T> & v)
	{
		return { ceil(v.x), ceil(v.y), ceil(v.z), ceil(v.w) };
	}

	// ...

	template<typename T> constexpr T sqr(const T & v)
	{
		return v * v;
	}

	template<typename T> constexpr T sqrt(const T & v)
	{
		return static_cast<T>(std::sqrt(v));
	}
	template<typename T> constexpr glm::tvec2<T> sqrt(const glm::tvec2<T> & v)
	{
		return { sqrt(v.x), sqrt(v.y) };
	}
	template<typename T> constexpr glm::tvec3<T> sqrt(const glm::tvec3<T> & v)
	{
		return { sqrt(v.x), sqrt(v.y), sqrt(v.z) };
	}
	template<typename T> constexpr glm::tvec4<T> sqrt(const glm::tvec4<T> & v)
	{
		return { sqrt(v.x), sqrt(v.y), sqrt(v.z), sqrt(v.w) };
	}

	template<typename T, typename P>
	constexpr T pow(const T & base, const P & exp)
	{
		return static_cast<T>(std::pow(base, exp));
	}
	template<typename T, typename P>
	constexpr glm::tvec2<T> pow(const glm::tvec2<T> & base, const P & exp)
	{
		return { pow(base.x, exp), pow(base.y, exp) };
	}
	template<typename T, typename P>
	constexpr glm::tvec3<T> pow(const glm::tvec3<T> & base, const P & exp)
	{
		return { pow(base.x, exp), pow(base.y, exp), pow(base.z, exp) };
	}
	template<typename T, typename P>
	constexpr glm::tvec4<T> pow(const glm::tvec4<T> & base, const P & exp)
	{
		return { pow(base.x, exp), pow(base.y, exp), pow(base.z, exp), pow(base.w, exp) };
	}

	// ...

	template<typename T, typename P> constexpr T lerp(const T & v1, const T & v2, const P & dt)
	{
		return static_cast<T>(v1 * (P{ 1 } - dt) + v2 * dt);
	}

	// ...

	template<typename T>
	constexpr float intbound(const T & s, const T & ds)
	{
		return static_cast<float>((ds > T{ 0 } ? ceil(s) - s : s - floor(s)) / abs(ds));
	}
	template<typename T>
	constexpr glm::vec2 intbound(const glm::tvec2<T>& s, const glm::tvec2<T>& ds)
	{
		return { intbound(s.x, ds.x), intbound(s.y, ds.y) };
	}
	template<typename T>
	constexpr glm::vec3 intbound(const glm::tvec3<T>& s, const glm::tvec3<T>& ds)
	{
		return { intbound(s.x, ds.x), intbound(s.y, ds.y), intbound(s.z, ds.z) };
	}
	template<typename T>
	constexpr glm::vec4 intbound(const glm::tvec4<T>& s, const glm::tvec4<T>& ds)
	{
		return { intbound(s.x, ds.x), intbound(s.y, ds.y), intbound(s.z, ds.z), intbound(s.w, ds.w) };
	}
}