#pragma once

#include <string_view>

#include <SDL2/SDL.h>

#include <Hinae/basic.hpp>

using namespace Hinae;

class Warp_SDL2
{
private:
    usize width;
    usize height;

    void* framebuffer      = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Window* window     = nullptr;
    SDL_Texture* texture   = nullptr;

    SDL_Event event;

public:
    Warp_SDL2() = default;
    Warp_SDL2(const Warp_SDL2&) = delete;
    Warp_SDL2(Warp_SDL2&&) = delete;
    ~Warp_SDL2() { quit(); }
    Warp_SDL2& operator = (const Warp_SDL2&) = delete;
    Warp_SDL2& operator = (Warp_SDL2&&) = delete;

    Warp_SDL2& init(usize width, usize height, std::string_view title);

    Warp_SDL2& quit();
    
    Warp_SDL2& write(const u8* buffer);

    Warp_SDL2& set_title(const char *title);

    i32 wait_key();

    i32 delay_key(u32 ms);

    /**
     * press ESC or 'q' quit
    */
    void run();
};
