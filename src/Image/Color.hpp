#pragma once

#include <Hinae/basic.hpp>

using namespace Hinae;

template <typename T>
struct basic_color;

using Color = basic_color<u8>;

template <typename T>
struct basic_color
{
	T r, g, b, a;

	basic_color(T v) : basic_color(v, v, v) {}
	basic_color(T r, T g, T b, T a = MAX_NUMBER<T>)
		: r(r), g(g), b(b), a(a) {}

	basic_color() = default;
	auto operator <=> (const basic_color&) const = default;

	static basic_color black()   { return {ZERO<T>}; }
	static basic_color red()     { return {MAX_NUMBER<T>, ZERO<T>, ZERO<T>}; }
	static basic_color green()   { return {ZERO<T>, MAX_NUMBER<T>, ZERO<T>}; }
	static basic_color blue()    { return {ZERO<T>, ZERO<T>, MAX_NUMBER<T>}; }
	static basic_color yellow()  { return {MAX_NUMBER<T>, MAX_NUMBER<T>, ZERO<T>}; }
	static basic_color magenta() { return {MAX_NUMBER<T>, ZERO<T>, MAX_NUMBER<T>}; }
	static basic_color cyan()    { return {ZERO<T>, MAX_NUMBER<T>, MAX_NUMBER<T>}; }
	static basic_color white()   { return {MAX_NUMBER<T>}; }

	void operator += (const basic_color& c) { r += c.r; g += c.g; b += c.b; }
	void operator -= (const basic_color& c) { r -= c.r; g -= c.g; b -= c.b; }
	void operator *= (const basic_color& c) { r *= c.r; g *= c.g; b *= c.b; }
	void operator /= (const basic_color& c) { r /= c.r; g /= c.g; b /= c.b; }

	basic_color operator + (const basic_color& c) const { return {r + c.r, g + c.g, b + c.b}; }
	basic_color operator - (const basic_color& c) const { return {r - c.r, g - c.g, b - c.b}; }
	basic_color operator * (const basic_color& c) const { return {r * c.r, g * c.g, b * c.b}; }
	basic_color operator / (const basic_color& c) const { return {r / c.r, g / c.g, b / c.b}; }

	void operator += (T v) { r += v; g += v; b += v; }
	void operator -= (T v) { r -= v; g -= v; b -= v; }
	void operator *= (T v) { r *= v; g *= v; b *= v; }
	void operator /= (T v) { r /= v; g /= v; b /= v; }

	basic_color operator + (T v) const { return {r + v, g + v, b + v}; }
	basic_color operator - (T v) const { return {r - v, g - v, b - v}; }
	basic_color operator * (T v) const { return {r * v, g * v, b * v}; }
	basic_color operator / (T v) const { return {r / v, g / v, b / v}; }
};

template <typename T>
std::ostream& operator << (std::ostream& os, const basic_color<T>& color)
{
	return os << std::make_tuple(color.r, color.g, color.b, color.a);
}

template <std::floating_point T>
u8 float_to_rgb_basic(T f)
{
    if(f >= ONE<T>) return 255;
    if(f <= ZERO<T>) return 0;
    return static_cast<u8>(f * static_cast<T>(255));
}

template <std::floating_point T>
Color float_to_rgb(const Vector3<T>& c)
{
    return {float_to_rgb_basic(c.x), float_to_rgb_basic(c.y), float_to_rgb_basic(c.z)};
}

template <std::floating_point T>
Vector3<T> rgb_to_float(const Color& c)
{
    return Vector3<T>{static_cast<T>(c.r), static_cast<T>(c.g), static_cast<T>(c.b)} / static_cast<T>(255);
}
