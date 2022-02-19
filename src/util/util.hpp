#pragma once

#include <cassert>

#include <source_location>
#include <iostream>
#include <limits>
#include <chrono>
#include <ranges>

#include <Hinae/Vector3.hpp>
#include <Image/Color.hpp>

#ifndef NAMESPACE_BEGIN
#define NAMESPACE_BEGIN(name) namespace name {
#endif

#ifndef NAMESPACE_END
#define NAMESPACE_END(name) }
#endif

#ifdef NDEBUG
#define dbg(expr) ((void)0)
#else
#define dbg(expr) std::cout<< '[' << std::source_location::current().file_name() << ':'<< std::source_location::current().line() << "] " << #expr << " = " << expr << '\n';
#endif // NDEBUG

template <std::integral T>
constexpr auto range(T end)
{
	return std::views::iota(ZERO<T>, end);
}

template <std::integral T>
constexpr auto range(T start, T end)
{
	return std::views::iota(start, end);
}

template <typename T>
struct EnumeratedElement
{
    usize index;
    T& element;
};

template <std::ranges::range T>
constexpr auto Enumerate(T& range, const usize step = 1) -> std::ranges::view auto 
{
    return range | std::views::transform(
        [i = usize{}, step](auto& element) mutable
		{
			i += step;
            return EnumeratedElement{i - step, element};
        }
    );
}

template <std::ranges::range Container1, std::ranges::range Container2, std::copy_constructible Function>
void map(const Container1& c1, Container2& c2, Function f)
{
	std::ranges::copy(std::views::transform(c1, f), c2.begin());
}

using namespace std::chrono;

struct Timer
{
private:
	using f64_ms = duration<f64, std::milli>;

	steady_clock::time_point t0;

public:
	Timer();

	static void display(f64 ms);

	void elapsed() const;

	template <typename T, typename... Args>
	static T elapsed(T fn(Args...), Args&&... args)
	{
		steady_clock::time_point start = steady_clock::now();
		if constexpr(std::is_same_v<T, void>)
		{
			fn(std::forward<Args>(args)...);
			f64 ms = duration_cast<f64_ms>(steady_clock::now() - start).count();
			display(ms);
		}
		else
		{
			T ret = fn(std::forward<Args>(args)...);
			f64 ms = duration_cast<f64_ms>(steady_clock::now() - start).count();
			display(ms);
			return ret;
		}
	}
};

template <std::ranges::range T>
void println(const T& args)
{
    for(const auto& v : args)
    {
        std::cout << v << ' ';
    }
    std::cout << '\n';
}

template <std::ranges::range... Ts>
void print(Ts... args)
{
    (println(args), ...);
}

template <typename... Ts>
void println(Ts... args)
{
    ((std::cout << args << '\n'), ...);
}

template <typename... Ts>
void print(Ts... args)
{
    ((std::cout << args << ' '), ...);
}

void FPS();

u8 f32_to_rgb(f32 f);

Color float_to_rgb(const Vector3f& c);

Vector3f rgb_to_float(const Color& c);
