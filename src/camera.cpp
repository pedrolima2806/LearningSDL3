#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

struct Vec2 {
    float x, y;
};

struct Camera {
    Vec2 position;
    int width, height;
};

struct Player {
    Vec2 position;
    SDL_FRect rect;
    float speed;
};

int camera() {
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
    SDL_Surface *bgImage = IMG_Load("../assets/images/lightWorld.png");
    if (!bgImage)
    {
        std::cerr << "Failed to load background image:" << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    const float WORLD_WIDTH = static_cast<float>(bgImage->w);
    const float WORLD_HEIGHT = static_cast<float>(bgImage->h);

    SDL_Texture *bgTexture = SDL_CreateTextureFromSurface(renderer, bgImage);
    SDL_DestroySurface(bgImage);
    if (!bgTexture) {
        std::cerr << "Failed to create texture:" << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    //definição do player e camera
    Player player = {
        {0.0f, 0.0f},
        {0.0f, 0.0f, 50.0f, 50.0f},
        1000
    };

    int windowWidth = 0;
    int windowHeight = 0;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    Camera camera = {
        {0,0},
        windowWidth, windowHeight
    };

    Uint64 lastTime = SDL_GetTicks();

    bool running = true;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                running = false;
            }
            if (e.type == SDL_EVENT_WINDOW_RESIZED) {
                SDL_GetWindowSize(window, &camera.width, &camera.height);
            }
        }

        const Uint64 currentTime = SDL_GetTicks();
        const float deltaTime = static_cast<float>(currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        const bool* keys = SDL_GetKeyboardState(nullptr);
        if (keys[SDL_SCANCODE_W]) player.position.y -= player.speed * deltaTime;
        if (keys[SDL_SCANCODE_S]) player.position.y += player.speed * deltaTime;
        if (keys[SDL_SCANCODE_A]) player.position.x -= player.speed * deltaTime;
        if (keys[SDL_SCANCODE_D]) player.position.x += player.speed * deltaTime;

        player.rect.x = player.position.x;
        player.rect.y = player.position.y;

        camera.position.x = player.rect.x + player.rect.w / 2 - camera.width / 2;
        camera.position.y = player.rect.y + player.rect.h / 2 - camera.height / 2;

        if (camera.position.x < 0) camera.position.x = 0;
        if (camera.position.y < 0) camera.position.y = 0;
        if (camera.position.x + camera.width > WORLD_WIDTH)
            camera.position.x = WORLD_WIDTH - camera.width;
        if (camera.position.y + camera.height > WORLD_HEIGHT)
            camera.position.y = WORLD_HEIGHT - camera.height;

        SDL_FRect worldBounds = {
            -camera.position.x,
            -camera.position.y,
            WORLD_WIDTH,
            WORLD_HEIGHT
        };
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, bgTexture, nullptr, &worldBounds);


        SDL_FRect screenPlayer = {
            player.rect.x - camera.position.x,
            player.rect.y - camera.position.y,
            player.rect.w,
            player.rect.h
        };
        SDL_SetRenderDrawColor(renderer, 200, 50, 50, 255);
        SDL_RenderFillRect(renderer, &screenPlayer);
        SDL_RenderPresent(renderer);

    }

    SDL_DestroyTexture(bgTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
