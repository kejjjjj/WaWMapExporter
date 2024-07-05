#pragma once

#include <functional>
#include "typedefs.hpp"

template<typename T>
concept arithmetic = requires(T v) { std::is_arithmetic<T>(); };

template<arithmetic T>
struct rect
{
	T x = {};
	T y = {};
	T w = {};
	T h = {};
};

using irect = rect<int>;
using frect = rect<float>;
using drect = rect<double>;

ivec2 adjust_from_640x480(const fvec2 r);
fvec2 adjust_to_640x480(const ivec2 r);
int adjust_from_640(const float x);
int adjust_from_480(const float y);
float adjust_to_640(const int x);
float adjust_to_480(const int y);