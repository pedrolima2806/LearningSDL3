#include <iostream>
#include <string>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

typedef enum {
    STATE_MENU,
    STATE_PLAYING,
    STATE_PAUSE,
    STATE_EXIT,
}gameState;

void handle_menu(const SDL_Event *event, gameState *state);
void handle_playing(const SDL_Event *event, gameState *state);
void handle_pause(const SDL_Event *event, gameState *state);
void render_text (SDL_Renderer *renderer, TTF_Font *font, const std::string &text, float x, float y);


int gameStateFunction() {
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "SDL_Init error: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (!TTF_Init())
    {
        std::cerr << "TTF_Init error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
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
    //definições para execução do programa
    gameState state = STATE_MENU;
    bool running = true;

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                state = STATE_EXIT;
            }

            switch (state) {
                case STATE_MENU: handle_menu(&event, &state); break;
                case STATE_PLAYING: handle_playing(&event, &state); break;
                case STATE_PAUSE: handle_pause(&event, &state); break;
                default: break;
            }
        }

        if (state == STATE_EXIT) {
            running = false;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        switch (state) {
            case STATE_MENU:
                render_text(renderer, font, "Press Enter to Start", 50, 50);
                break;
            case STATE_PLAYING:
                render_text(renderer, font, "Playing... Press P to Pause, Esc to Quit", 50, 50);
                break;
            case STATE_PAUSE:
                render_text(renderer, font, "Paused... Press R to Resume, Esc to Quit", 50, 50);
                break;
            default: break;
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}

void handle_menu(const SDL_Event *event, gameState *state) {
    if (event->type == SDL_EVENT_KEY_DOWN && event->key.key == SDLK_RETURN) {
        *state = STATE_PLAYING;
    }
}

void handle_playing(const SDL_Event *event, gameState *state) {
    if (event->type == SDL_EVENT_KEY_DOWN && event->key.key == SDLK_P) {
        *state = STATE_PAUSE;
    }
    if (event->type == SDL_EVENT_KEY_DOWN && event->key.key == SDLK_ESCAPE) {
        *state = STATE_EXIT;
    }
}

void handle_pause(const SDL_Event *event, gameState *state) {
    if (event->type == SDL_EVENT_KEY_DOWN && event->key.key == SDLK_R) {
        *state = STATE_PLAYING;
    }
    if (event->type == SDL_EVENT_KEY_DOWN && event->key.key == SDLK_ESCAPE) {
        *state = STATE_EXIT;
    }
}

void render_text (SDL_Renderer *renderer, TTF_Font *font, const std::string &text, float x, float y) {
    constexpr SDL_Color white = {255, 255, 255, 255};
    SDL_Surface *surface = TTF_RenderText_Blended(font, text.c_str(), text.size(), white);
    if (!surface) return;

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    const SDL_FRect dst = {x, y, static_cast<float>(surface->w), static_cast<float>(surface->h)};
    SDL_RenderTexture(renderer, texture, nullptr, &dst);

    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
}
