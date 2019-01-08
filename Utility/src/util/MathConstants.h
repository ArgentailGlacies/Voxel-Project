#pragma once

constexpr long double operator"" _pi(long double v) { return 3.14159265358979323846L * v; }
constexpr long double operator"" _pi(unsigned long long int v) { return 3.14159265358979323846L * static_cast<long double>(v); }

namespace util
{
	template<typename T> constexpr T PI = T(3.1415926535897932384626433L);
	template<typename T> constexpr T DEG_TO_RAD = PI<T> / T(180);
	template<typename T> constexpr T RAD_TO_DEG = T(180) / PI<T>;
}