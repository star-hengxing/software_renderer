#pragma once

#include <source_location>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <ranges>

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

using usize = std::size_t;
using f64 	= double;

template <std::integral T>
constexpr auto range(T end)
{
	return std::views::iota(static_cast<T>(0), end);
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
void map(const Container1& src, Container2& dst, Function f)
{
	std::ranges::transform(src.begin(), src.end(), dst.begin(), f);
	// std::ranges::copy(std::views::transform(src, f), dst.begin());
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

template <typename T, typename... Ts>
void println(const T& arg, const Ts&... args)
{
	std::cout << arg;
    ((std::cout << '\n' << args), ...);
}
template <typename T, typename... Ts>
void print(const T& arg, const Ts&... args)
{
 	std::cout << arg;
    ((std::cout << ' ' << args), ...);
}

void FPS();
