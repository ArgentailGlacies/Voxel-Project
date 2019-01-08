#pragma once

#include "util/MathConstants.h"
#include "util/PhysicalQuantity.h"
#include "util/PhysicalOperators.h"

// Convenient types for normal use

using Angle = util::Scalar<util::Angle>;
using Acceleration = util::Vector<util::Acceleration>;
using AccelerationMagnitude = util::Scalar<util::Acceleration>;
using Energy = util::Scalar<util::Energy>;
using Force = util::Vector<util::Force>;
using ForceMagnitude = util::Scalar<util::Force>;
using Length = util::Scalar<util::Length>;
using Mass = util::Scalar<util::Mass>;
using Momentum = util::Vector<util::Momentum>;
using Position = util::Vector<util::Length>;
using Pressure = util::Scalar<util::Pressure>;
using Speed = util::Scalar<util::Speed>;
using Temperature = util::Scalar<util::Temperature>;
using Time = util::Scalar<util::Time>;
using Velocity = util::Vector<util::Speed>;

// Predefined quantities

constexpr const Angle radian{ 1.0 };				// Base unit
constexpr const Angle degree = 2_pi / 360.0 * radian;

constexpr const Length metre{ 1.0 };				// SI unit
constexpr const Length desimetre = 0.1 * metre;
constexpr const Length centimetre = 0.01 * metre;
constexpr const Length millimetre = 0.001 * metre;
constexpr const Length kilometre = 1'000 * metre;

constexpr const Mass kg{ 1.0 };						// SI unit
constexpr const Mass gramme = 0.001 * kg;
constexpr const Mass tonne = 1'000 * kg;

constexpr const Temperature kelvin{ 1.0 };			// SI unit
constexpr const Temperature watertriplepoint = 273.16 * kelvin;

constexpr const Time second{ 1.0 };					// SI unit
constexpr const Time minute = 60 * second;
constexpr const Time hour = 60 * minute;
constexpr const Time day = 24 * hour;

// ...

constexpr const AccelerationMagnitude G{ 9.80665 };

constexpr const Energy joule{ 1.0 };				// SI unit

constexpr const ForceMagnitude newton{ 1.0 };		// SI unit
constexpr const ForceMagnitude kilopond = kg * G;

constexpr const Pressure pascal{ 1.0 };				// SI unit
constexpr const Pressure bar = 100'000 * pascal;

// Convenient literals for normal use

constexpr AccelerationMagnitude operator"" _mps2(long double v) { return metre / second / second * v; }
constexpr AccelerationMagnitude operator"" _G(long double v) { return G * v; }
constexpr AccelerationMagnitude operator"" _mps2(unsigned long long int v) { return metre / second / second * static_cast<long double>(v); }
constexpr AccelerationMagnitude operator"" _G(unsigned long long int v) { return G * static_cast<long double>(v); }

constexpr Angle operator"" _rad(long double v) { return radian * v; }
constexpr Angle operator"" _deg(long double v) { return degree * v; }
constexpr Angle operator"" _rad(unsigned long long int v) { return radian * static_cast<long double>(v); }
constexpr Angle operator"" _deg(unsigned long long int v) { return degree * static_cast<long double>(v); }

constexpr ForceMagnitude operator"" _N(long double v) { return newton * v; }
constexpr ForceMagnitude operator"" _kp(long double v) { return kilopond * v; }
constexpr ForceMagnitude operator"" _N(unsigned long long int v) { return newton * static_cast<long double>(v); }
constexpr ForceMagnitude operator"" _kp(unsigned long long int v) { return kilopond * static_cast<long double>(v); }

constexpr Length operator"" _m(long double v) { return metre * v; }
constexpr Length operator"" _dm(long double v) { return desimetre * v; }
constexpr Length operator"" _cm(long double v) { return centimetre * v; }
constexpr Length operator"" _mm(long double v) { return millimetre * v; }
constexpr Length operator"" _km(long double v) { return kilometre * v; }
constexpr Length operator"" _m(unsigned long long int v) { return metre * static_cast<long double>(v); }
constexpr Length operator"" _dm(unsigned long long int v) { return desimetre * static_cast<long double>(v); }
constexpr Length operator"" _cm(unsigned long long int v) { return centimetre * static_cast<long double>(v); }
constexpr Length operator"" _mm(unsigned long long int v) { return millimetre * static_cast<long double>(v); }
constexpr Length operator"" _km(unsigned long long int v) { return kilometre * static_cast<long double>(v); }

constexpr Mass operator"" _kg(long double v) { return kg * v; }
constexpr Mass operator"" _g(long double v) { return gramme * v; }
constexpr Mass operator"" _t(long double v) { return tonne * v; }
constexpr Mass operator"" _kg(unsigned long long int v) { return kg * static_cast<long double>(v); }
constexpr Mass operator"" _g(unsigned long long int v) { return gramme * static_cast<long double>(v); }
constexpr Mass operator"" _t(unsigned long long int v) { return tonne * static_cast<long double>(v); }

constexpr Pressure operator"" _Pa(long double v) { return pascal * v; }
constexpr Pressure operator"" _bar(long double v) { return bar * v; }
constexpr Pressure operator"" _Pa(unsigned long long int v) { return pascal * static_cast<long double>(v); }
constexpr Pressure operator"" _bar(unsigned long long int v) { return bar * static_cast<long double>(v); }

constexpr Speed operator"" _mps(long double v) { return metre / second * v; }
constexpr Speed operator"" _kmph(long double v) { return kilometre / hour * v; }
constexpr Speed operator"" _mps(unsigned long long int v) { return metre / second * static_cast<long double>(v); }
constexpr Speed operator"" _kmph(unsigned long long int v) { return kilometre / hour * static_cast<long double>(v); }

constexpr Temperature operator"" _K(long double v) { return kelvin * v; }
constexpr Temperature operator"" _C(long double v) { return watertriplepoint + kelvin * v; }
constexpr Temperature operator"" _K(unsigned long long int v) { return kelvin * static_cast<long double>(v); }
constexpr Temperature operator"" _C(unsigned long long int v) { return watertriplepoint + kelvin * static_cast<long double>(v); }

constexpr Time operator"" _s(long double v) { return second * v; }
constexpr Time operator"" _min(long double v) { return minute * v; }
constexpr Time operator"" _h(long double v) { return hour * v; }
constexpr Time operator"" _day(long double v) { return day * v; }
constexpr Time operator"" _s(unsigned long long int v) { return second * static_cast<long double>(v); }
constexpr Time operator"" _min(unsigned long long int v) { return minute * static_cast<long double>(v); }
constexpr Time operator"" _h(unsigned long long int v) { return hour * static_cast<long double>(v); }
constexpr Time operator"" _day(unsigned long long int v) { return day * static_cast<long double>(v); }
