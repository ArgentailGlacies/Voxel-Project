#pragma once

#include "PhysicalQuantity.h"

#include <math.h>

#define UNIT_TEMPLATE template<\
		typename L, typename M, typename T, typename R, typename Θ, typename N, typename I, typename J\
	>
#define UNIT_TEMPLATE2 template<\
		typename L, typename M, typename T, typename R, typename Θ, typename N, typename I, typename J,\
		typename L2, typename M2, typename T2, typename R2, typename Θ2, typename N2, typename I2, typename J2\
	>
#define UNIT_TEMPLATEP template<\
		int P,\
		typename L, typename M, typename T, typename R, typename Θ, typename N, typename I, typename J\
	>

#define UNIT_SCALAR Scalar<Unit<L, M, T, R, Θ, N, I, J>>
#define UNIT_SCALAR2 Scalar<Unit<L2, M2, T2, R2, Θ2, N2, I2, J2>>
#define UNIT_SCALAR_ADD Scalar<Unit<\
		std::ratio_add<L, L2>, std::ratio_add<M, M2>, std::ratio_add<T, T2>, std::ratio_add<R, R2>,\
		std::ratio_add<Θ, Θ2>, std::ratio_add<N, N2>, std::ratio_add<I, I2>, std::ratio_add<J, J2>\
	>>
#define UNIT_SCALAR_SUB Scalar<Unit<\
		std::ratio_subtract<L, L2>, std::ratio_subtract<M, M2>, std::ratio_subtract<T, T2>, std::ratio_subtract<R, R2>,\
		std::ratio_subtract<Θ, Θ2>, std::ratio_subtract<N, N2>, std::ratio_subtract<I, I2>, std::ratio_subtract<J, J2>\
	>>
#define UNIT_SCALAR_MUL(D) Scalar<Unit<\
		std::ratio_multiply<L, D>, std::ratio_multiply<M, D>, std::ratio_multiply<T, D>, std::ratio_multiply<R, D>,\
		std::ratio_multiply<Θ, D>, std::ratio_multiply<N, D>, std::ratio_multiply<I, D>, std::ratio_multiply<J, D>\
	>>
#define UNIT_SCALAR_DIV(D) Scalar<Unit<\
		std::ratio_divide<L, D>, std::ratio_divide<M, D>, std::ratio_divide<T, D>, std::ratio_divide<R, D>,\
		std::ratio_divide<Θ, D>, std::ratio_divide<N, D>, std::ratio_divide<I, D>, std::ratio_divide<J, D>\
	>>

namespace util
{
	// Comparison operators

	UNIT_TEMPLATE constexpr auto operator==(const UNIT_SCALAR & lhs, const UNIT_SCALAR & rhs) noexcept { return lhs() == rhs(); }
	UNIT_TEMPLATE constexpr auto operator!=(const UNIT_SCALAR & lhs, const UNIT_SCALAR & rhs) noexcept { return lhs() != rhs(); }

	UNIT_TEMPLATE constexpr auto operator<(const UNIT_SCALAR & lhs, const UNIT_SCALAR & rhs) noexcept { return lhs() < rhs(); }
	UNIT_TEMPLATE constexpr auto operator<=(const UNIT_SCALAR & lhs, const UNIT_SCALAR & rhs) noexcept { return lhs() <= rhs(); }
	UNIT_TEMPLATE constexpr auto operator>(const UNIT_SCALAR & lhs, const UNIT_SCALAR & rhs) noexcept { return lhs() > rhs(); }
	UNIT_TEMPLATE constexpr auto operator>=(const UNIT_SCALAR & lhs, const UNIT_SCALAR & rhs) noexcept { return lhs() >= rhs(); }

	// Arithmetic operators

	UNIT_TEMPLATE constexpr auto operator+(const UNIT_SCALAR & lhs, const UNIT_SCALAR & rhs) noexcept { return UNIT_SCALAR{ lhs() + rhs() }; }
	UNIT_TEMPLATE constexpr auto & operator+=(const UNIT_SCALAR & lhs, const UNIT_SCALAR & rhs) noexcept { lhs() += rhs(); return lhs; }

	UNIT_TEMPLATE constexpr auto operator-(const UNIT_SCALAR & lhs) noexcept { return UNIT_SCALAR{ -lh() }; }
	UNIT_TEMPLATE constexpr auto operator-(const UNIT_SCALAR & lhs, const UNIT_SCALAR & rhs) noexcept { return UNIT_SCALAR{ lhs() - rhs() }; }
	UNIT_TEMPLATE constexpr auto & operator-=(const UNIT_SCALAR & lhs, const UNIT_SCALAR & rhs) noexcept { lhs() -= rhs(); return lhs; }

	UNIT_TEMPLATE2 constexpr auto operator*(const UNIT_SCALAR & lhs, const UNIT_SCALAR2 & rhs) noexcept { return UNIT_SCALAR_ADD{ lhs() * rhs() }; }
	UNIT_TEMPLATE constexpr auto operator*(const UNIT_SCALAR & lhs, const long double & rhs) noexcept { return UNIT_SCALAR{ lhs() * rhs }; }
	UNIT_TEMPLATE constexpr auto operator*(const long double & lhs, const UNIT_SCALAR & rhs) noexcept { return UNIT_SCALAR{ lhs * rhs() }; }

	UNIT_TEMPLATE2 constexpr auto operator/(const UNIT_SCALAR & lhs, const UNIT_SCALAR2 & rhs) noexcept { return UNIT_SCALAR_SUB{ lhs() / rhs() }; }
	UNIT_TEMPLATE constexpr auto operator/(const UNIT_SCALAR & lhs, const long double & rhs) noexcept { return UNIT_SCALAR{ lhs() / rhs }; }
	UNIT_TEMPLATE constexpr auto operator/(const long double & lhs, const UNIT_SCALAR & rhs) noexcept { return UNIT_SCALAR{ lhs / rhs() }; }

	// Functions

	UNIT_TEMPLATE constexpr auto sqr(const UNIT_SCALAR & v) noexcept { return v * v; }
	UNIT_TEMPLATE constexpr auto sqrt(const UNIT_SCALAR & v) noexcept { return UNIT_SCALAR_DIV(std::ratio<2>){ std::sqrt(v()) }; }
	UNIT_TEMPLATEP constexpr auto pow(const UNIT_SCALAR & v) noexcept { return UNIT_SCALAR_MUL(std::ratio<P>){ std::pow(v(), P) }; }
}