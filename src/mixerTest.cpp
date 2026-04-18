#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

int main () {
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "SDL video init failed." << std::endl;
        return 1;
    }
    if (!MIX_Init()) {
        std::cerr << "MIX_Init failed." << std::endl;
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
    MIX_Mixer *mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
    if (!mixer) {
        std::cerr << "Failed to open audio device:" << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    MIX_Audio *audio = MIX_LoadAudio (mixer, "/home/pedro/Music/Eu Não Sou Boa Influência pra Você - Seu Pereira e Coletivo 401 (youtube).mp3", false);
    if (audio) {
        MIX_PlayAudio(mixer, audio);
    }

    SDL_Delay(7000);


    MIX_DestroyAudio(audio);
    MIX_DestroyMixer(mixer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}