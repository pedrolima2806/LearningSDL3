#include <iostream>
#include <vector>
#include <oneapi/tbb/profiling.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

int textDisplay()
{

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "SDL_Init error: " << SDL_GetError() << std::endl;
    }

    if (!TTF_Init())
    {
        std::cerr << "TTF_Init error: " << SDL_GetError() << std::endl;
    }

    SDL_Window *window = SDL_CreateWindow("SDL Window", 640, 480, 0);
    if (!window)
    {
        std::cerr << "SDL_CreateWindow error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer)
    {
        std::cerr << "SDL_CreateRenderer error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    TTF_Font *font = TTF_OpenFont("../assets/fonts/sixty/SIXTY.TTF", 16);
    if (!font)
    {
        std::cerr << "SDL_CreateFont error: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    std::string text = u8"bom dia grupo";

    // vetor de superfícies
    std::vector<SDL_Surface *> surfaces;
    surfaces.reserve(text.size());

    // renderiza cada caractere como uma superfície
    for (char i : text)
    {
        SDL_Surface *textSurface = TTF_RenderGlyph_LCD(font, i, SDL_Color{255, 255, 255, 255}, SDL_Color{0, 0, 0, 255});
        if (!textSurface)
        {
            std::cerr << "TTF_RenderGlyph error: " << SDL_GetError() << std::endl;
            continue;
        }
        surfaces.push_back(textSurface);
    }

    // Calcula o tamanho da superfície total
    int textWidth = 0;
    int maxHeight = 0;
    for (auto &surf : surfaces)
    {
        textWidth += surf->w + 5; // o 5 adiciona espaço entre os caracteres
        if (surf->h > maxHeight)
            maxHeight = surf->h;
    }

    const SDL_FRect rect{0, 0, static_cast<float>(textWidth), static_cast<float>(maxHeight)};

    // cria superfície combinada
    SDL_Surface *combinedSurface = SDL_CreateSurface(textWidth, maxHeight, SDL_PIXELFORMAT_RGBA32);
    if (!combinedSurface)
    {
        std::cerr << "SDL_CreateSurface error creating combined surface: " << SDL_GetError() << std::endl;
        return 1;
    }
    // coloca cada caracter na superfície combinada
    int xOffset = 0;
    for (auto &surf : surfaces)
    {
        SDL_Rect destRect = {xOffset, 0, surf->w, surf->h};
        SDL_BlitSurface(surf, nullptr, combinedSurface, &destRect);
        xOffset += surf->w + 5;
        SDL_DestroySurface(surf); // destroi as superfícies após adicioná-las ao retângulo
    }

    // converter para textura
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, combinedSurface);
    SDL_DestroySurface(combinedSurface);
    if (!texture)
    {
        std::cerr << "SDL_CreateTexture error: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event event;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT ||
                event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_ESCAPE)
            {
                running = false;
            }
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            SDL_RenderTexture(renderer, texture, nullptr, &rect);
            SDL_RenderPresent(renderer);
        }
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();

    surfaces.clear();
    return 0;
}