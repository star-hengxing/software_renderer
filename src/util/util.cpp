#include "util.hpp"

using namespace std::chrono;

Timer::Timer() : t0(steady_clock::now()) {}

void Timer::display(f64 ms)
{
    print("time elapsed:", ms, "ms\n");
}

void Timer::elapsed() const
{
    f64 ms = duration_cast<f64_ms>(steady_clock::now() - t0).count();
    display(ms);
}

void FPS()
{
    static steady_clock::time_point last_time, current_time;
    static std::size_t fps = 0;
    
    fps++;
    current_time = steady_clock::now();
    
    if(duration_cast<milliseconds>(current_time - last_time).count() > 1000)
    {
        printf("FPS: %lld\n", fps);
        fps = 0;
        last_time = current_time;
    }
}

u8 f32_to_rgb(f32 f)
{
    if(f >= ONE<f32>) return 255;
    if(f <= ZERO<f32>) return 0;
    return static_cast<u8>(f * 255.0f);
}

Color float_to_rgb(const Vector3f& c)
{
    return {f32_to_rgb(c.x), f32_to_rgb(c.y), f32_to_rgb(c.z)};
}

Vector3f rgb_to_float(const Color& c)
{
    return Vector3f{static_cast<f32>(c.r), static_cast<f32>(c.g), static_cast<f32>(c.b)} / 255.0f;
}