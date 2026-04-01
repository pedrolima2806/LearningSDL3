#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

int healthBar()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "SDL video init failed:" << SDL_GetError() << std::endl;
    };

    SDL_Window *window = SDL_CreateWindow("healthBar", 500, 500, 0);
    if (!window)
    {
        std::cerr << "Error creating window:" << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer)
    {
        std::cerr << "Error creating renderer:" << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Surface *healthBarSurface = IMG_Load("../assets/images/healthbar.png");
    if (!healthBarSurface)
    {
        std::cerr << "Error creating surface:" << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture *healthBarTexture = SDL_CreateTextureFromSurface(renderer, healthBarSurface);
    SDL_DestroySurface(healthBarSurface);
    if (!healthBarTexture)
    {
        std::cerr << "Error creating texture:" << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_FRect rect{100, 100, 200, 22};

    float x = 0; // posição x do mouse

    bool running = true;
    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT ||
                event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_ESCAPE)
            {
                running = false;
            }
            if (event.type == SDL_EVENT_MOUSE_MOTION)
            {
                x = event.motion.x;
                if (x < 100)
                    x = 100;
                if (x > 297)
                    x = 297;
            }
            SDL_FRect rect2{102, 102, std::max(static_cast<float>(0), x - 100), 18};

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderClear(renderer);

            SDL_RenderTexture(renderer, healthBarTexture, nullptr, &rect);

            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer, &rect2);

            SDL_RenderPresent(renderer);
        }
    }

    SDL_DestroyTexture(healthBarTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}