#include <chrono>
#include <iostream>
#include <vector>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

int animatedSprites()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "SDL video init failed." << std::endl;
        return 1;
    }
    SDL_Window *window = SDL_CreateWindow("animatedSprites", 500, 500, 0);
    if (!window)
    {
        std::cerr << "Failed to create window:" << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer)
    {
        std::cerr << "Failed to create renderer:" << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_Surface *spriteSheetSurface = IMG_Load("../assets/images/Sonic.png");
    if (!spriteSheetSurface)
    {
        std::cerr << "Failed to load spriteSheet:" << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    Uint8 keyR = 0;
    Uint8 keyG = 0;
    Uint8 keyB = 0;
    Uint8 keyA = 0;
    if (!SDL_ReadSurfacePixel(spriteSheetSurface, 0, 0, &keyR, &keyG, &keyB, &keyA))
    {
        std::cerr << "Failed to read spriteSheet background color:" << SDL_GetError() << std::endl;
        SDL_DestroySurface(spriteSheetSurface);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_SetSurfaceColorKey(spriteSheetSurface, true, SDL_MapSurfaceRGB(spriteSheetSurface, keyR, keyG, keyB));
    SDL_Texture *spriteSheetTexture = SDL_CreateTextureFromSurface(renderer, spriteSheetSurface);
    SDL_DestroySurface(spriteSheetSurface);
    if (!spriteSheetTexture)
    {
        std::cerr << "Failed to load spriteSheet:" << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_SetTextureBlendMode(spriteSheetTexture, SDL_BLENDMODE_BLEND);
    // dados do spriteSheet
    size_t numberRows = 2;
    size_t numberColumns = 8;
    size_t widthSprite = 28;
    size_t heightSprite = 38;

    std::vector<SDL_FRect> rects;
    // recorta a sprite sheet em vários sprites:
    for (size_t row = 0; row < numberRows; ++row)
    {
        for (size_t column = 0; column < numberColumns; ++column)
        {
            rects.push_back(SDL_FRect{static_cast<float>(column * widthSprite),
                                      static_cast<float>(row * heightSprite),
                                      static_cast<float>(widthSprite),
                                      static_cast<float>(heightSprite)});
        }
    }
    // define as animações com base em suas colunas e linhas
    std::vector<std::pair<size_t, size_t>> animation1 = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}};
    std::vector<std::pair<size_t, size_t>> current = animation1;

    // frame timing
    double maxDuration = 150; // in ms for one frame of the animation
    double timeBuffer = 0;    // in ms
    bool running = true;
    size_t frameIndex = 0;
    auto previousTime = std::chrono::steady_clock::now();

    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
                running = false;
            if (event.type == SDL_EVENT_KEY_DOWN)
            {
                SDL_Keycode key = event.key.key;
                if (key == SDLK_ESCAPE)
                    running = false;
                else if (key == SDLK_Q)
                {
                    current = animation1;
                    frameIndex = 0;
                    timeBuffer = 0;
                }
            }
        }

        auto currentTime = std::chrono::steady_clock::now();
        double deltaMs = std::chrono::duration<double, std::milli>(currentTime - previousTime).count();
        previousTime = currentTime;
        timeBuffer += deltaMs;

        if (!current.empty())
        {
            while (timeBuffer >= maxDuration)
            {
                frameIndex = (frameIndex + 1) % current.size();
                timeBuffer -= maxDuration;
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        if (!current.empty())
        {
            auto currentPair = current[frameIndex];

            // posição no vetor 1d de sprites:
            size_t position = currentPair.first * numberColumns + currentPair.second;
            SDL_FRect src = rects[position];
            SDL_FRect dest = {75, 75, static_cast<float>(widthSprite * 5), static_cast<float>(heightSprite * 5)};
            SDL_RenderTexture(renderer, spriteSheetTexture, &src, &dest);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(1);
    }

    SDL_DestroyTexture(spriteSheetTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
