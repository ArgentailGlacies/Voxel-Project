#pragma once

#include <ratio>

#define UNIT_TYPE(L, M, T, R, Θ, N, I, J) \
	::util::Unit<std::ratio<L>, std::ratio<M>, std::ratio<T>, std::ratio<R>, std::ratio<Θ>, std::ratio<N>, std::ratio<I>, std::ratio<J>>

namespace util
{
	template<typename L, typename M, typename T, typename R, typename Θ, typename N, typename I, typename J>
	struct Unit
	{
		/*enum
		{
			metre = L,		// The one-dimensional extent of an object
			kilogram = M,	// A measure of resistance to acceleration
			second = T,		// The duration of an event
			radian = R,		// The angular extent of an object
			kelvin = Θ,		// Average kinetic energy per degree of freedom of a system
			mole = N,		// Number of particles compared to the number of atoms in 0.012 kg of 12C
			ampere = I,		// Rate of flow of electrical charge per unit time
			candela = J,	// Wavelength-weighted power of emitted light per unit solid angle
		};*/
	};

	// Physical dimensions

	using Acceleration		= UNIT_TYPE(1, 0, -2, 0, 0, 0, 0, 0);
	using Angle				= UNIT_TYPE(0, 0, 0, 1, 0, 0, 0, 0);
	using Area				= UNIT_TYPE(2, 0, 0, 0, 0, 0, 0, 0);
	using Density			= UNIT_TYPE(-3, 1, 0, 0, 0, 0, 0, 0);
	using Energy			= UNIT_TYPE(2, 1, -2, 0, 0, 0, 0, 0);
	using Force				= UNIT_TYPE(1, 1, -2, 0, 0, 0, 0, 0);
	using Frequency			= UNIT_TYPE(0, 0, -1, 0, 0, 0, 0, 0);
	using Jerk				= UNIT_TYPE(1, 0, -3, 0, 0, 0, 0, 0);
	using Length			= UNIT_TYPE(1, 0, 0, 0, 0, 0, 0, 0);
	using Mass				= UNIT_TYPE(0, 1, 0, 0, 0, 0, 0, 0);
	using Momentum			= UNIT_TYPE(1, 1, -1, 0, 0, 0, 0, 0);
	using Number			= UNIT_TYPE(0, 0, 0, 0, 0, 0, 0, 0);
	using Pressure			= UNIT_TYPE(-1, 1, -2, 0, 0, 0, 0, 0);
	using Speed				= UNIT_TYPE(1, 0, -1, 0, 0, 0, 0, 0);
	using Temperature		= UNIT_TYPE(0, 0, 0, 0, 1, 0, 0, 0);
	using Time				= UNIT_TYPE(0, 0, 1, 0, 0, 0, 0, 0);
	using Volume			= UNIT_TYPE(3, 0, 0, 0, 0, 0, 0, 0);

	// ...

	template<typename T>
	class Scalar
	{
	public:
		constexpr explicit Scalar() noexcept = default;
		constexpr explicit Scalar(const long double & value) noexcept
			: m_value(value)
		{}

		constexpr auto & operator=(const Scalar & other) noexcept
		{
			m_value = other.m_value;
			return *this;
		}
		constexpr auto & operator()() const noexcept { return m_value; }

	private:
		long double m_value = 0.0L;
	};

	template<typename T>
	class Vector
	{
	public:
		constexpr explicit Vector() noexcept = default;
		constexpr explicit Vector(const Scalar<T> & value)
			: m_x(value), m_y(value), m_z(value)
		{}
		constexpr explicit Vector(const Scalar<T> & x, const Scalar<T> & y, const Scalar<T> & z) noexcept
			: m_x(x), m_y(y), m_z(z)
		{}

		constexpr auto & operator=(const Vector & other) noexcept
		{
			m_x = other.m_x;
			m_y = other.m_y;
			m_z = other.m_z;
			return *this;
		}

		constexpr auto & x() const { return m_x; }
		constexpr auto & y() const { return m_y; }
		constexpr auto & z() const { return m_z; }

	private:
		Scalar<T> m_x = 0.0L;
		Scalar<T> m_y = 0.0L;
		Scalar<T> m_z = 0.0L;
	};
}