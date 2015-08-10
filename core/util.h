#ifndef VECCORE_UTIL_H
#define VECCORE_UTIL_H

#if __cplusplus <= 201100L
#   error "VecCore requires a compiler with support for C++11 standard"
#endif

#include <cmath>
#include <limits>
#include <type_traits>

#include <core/simd.h>
#include <core/cuda.h>

namespace VecCore {

template <typename T>
inline constexpr T Deg(const T& x)
{
    return (x * static_cast<T>(180.0 / M_PI));
}

template <typename T>
inline constexpr T Rad(const T& x)
{
    return (x * static_cast<T>(M_PI / 180.0));
}

template <typename T>
inline constexpr T Sgn(const T& x)
{
    return (static_cast<T>(0) < x) - (x < static_cast<T>(0));
}

template <class T>
inline const T& Min(const T& a, const T& b)
{
    return a < b ? a : b;
}

template <class T>
inline const T& Max(const T& a, const T& b)
{
    return a > b ? a : b;
}

template <class T>
inline const T& Min(const T& a, const T& b, const T& c)
{
    return a < b ? Min(a, c) : Min(b, c);
}

template <class T>
inline const T& Max(const T& a, const T& b, const T& c)
{
    return a > b ? Max(a, c) : Max(b, c);
}

template <class T>
inline void SetMin(T& a, const T& b)
{
    if (a > b) a = b;
}

template <class T>
inline void SetMax(T& a, const T& b)
{
    if (a < b) a = b;
}

template <typename T>
inline bool Infinitesimal(const T& x)
{
    static_assert(std::is_floating_point<T>::value,
        "infinitesimal() is intended only for floating point types");

    return std::abs(x) < std::numeric_limits<T>::epsilon();
}

template <typename T>
inline bool AlmostEqual(const T& x, const T& y)
{
    static_assert(std::is_floating_point<T>::value,
        "almost_equal() is intended only for floating point types");

    if (x == y)
        return true;

    return std::abs(x - y) < (std::abs(x) + std::abs(y)) * std::numeric_limits<T>::epsilon();
}

template <class T>
inline bool InRange(const T& x, const T& min, const T& max)
{
    return min < max ? (x >= min) && (x <= max)
                     : (x >= max) && (x <= min);
}

template <class T>
T Clamp(const T& x, const T& xmin, const T& xmax)
{
    return Max<T>(xmin, Min<T>(x, xmax));
}

}

#endif