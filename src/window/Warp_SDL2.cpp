#include "Warp_SDL2.hpp"

#include <iostream>

static void SDL_Error(std::string_view error)
{
    std::cerr << error.data() << ": " << SDL_GetError() << '\n';
}

Warp_SDL2& Warp_SDL2::init(usize width, usize height, std::string_view title)
{
    this->width  = width;
    this->height = height;

    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(
        title.data(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height, 0);
    if(!window)
    {
        SDL_Error("Error creating window");
        return *this;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if(!window)
    {
        SDL_Error("Error creating renderer");
        return *this;
    }
    
    texture = SDL_CreateTexture(renderer,  
        SDL_PIXELFORMAT_RGBA32,  
        SDL_TEXTUREACCESS_STREAMING,  
        width, height);
    if(!texture)
    {
        SDL_Error("Error creating texture");
        return *this;
    }

    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    return *this;
}

Warp_SDL2& Warp_SDL2::quit()
{
    if(texture)  SDL_DestroyTexture(texture);
    if(renderer) SDL_DestroyRenderer(renderer);
    if(window)   SDL_DestroyWindow(window);
    SDL_Quit();
    return *this;
}

Warp_SDL2& Warp_SDL2::write(const u8* buffer)
{
    SDL_UpdateTexture(texture, nullptr, buffer, width * sizeof(Uint32));
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
    return *this;
}

Warp_SDL2& Warp_SDL2::set_title(const char *title)
{
    SDL_SetWindowTitle(window, title);
    return *this;
}

i32 Warp_SDL2::wait_key()
{
    SDL_WaitEvent(&event);
    return (event.type == SDL_KEYDOWN ? event.key.keysym.sym : 0);
}

i32 Warp_SDL2::delay_key(u32 ms)
{
    SDL_Delay(ms);
    SDL_PollEvent(&event);
    return (event.type == SDL_KEYDOWN ? event.key.keysym.sym : 0);
}

void Warp_SDL2::run()
{
    for(;;)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                return;
            case SDL_KEYDOWN:
                {
                    i32 key = event.key.keysym.sym;
                    if(key == SDLK_ESCAPE || key == 'q')
                        return;
                }
                break;
            default:
                break;
        }
    }
}
