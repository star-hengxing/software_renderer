#include "util.hpp"

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
    static usize fps = 0;
    
    fps++;
    current_time = steady_clock::now();
    
    if(duration_cast<milliseconds>(current_time - last_time).count() > 1000)
    {
        std::cout << "FPS: " << fps << '\n';
        fps = 0;
        last_time = current_time;
    }
}
