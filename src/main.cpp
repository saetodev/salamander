#include "types.h"
#include "input.h"

#include <stdio.h>
#include <stdlib.h>

#include <array>
#include <memory>

#include <SDL.h>

struct Platform {
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    bool running = false;
};

Platform create_platform() {
    Platform platform = {};

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("sdl init failed: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    platform.window = SDL_CreateWindow("SAL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);

    if (!platform.window) {
        printf("sdl create window failed: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    platform.renderer = SDL_CreateRenderer(platform.window, -1, SDL_RENDERER_ACCELERATED);

    if (!platform.renderer) {
        printf("sdl create renderer failed %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    platform.running = true;

    return platform;
}

void destroy_platform(Platform *platform) {
    SDL_DestroyRenderer(platform->renderer);
    SDL_DestroyWindow(platform->window);

    SDL_Quit();
}

void handle_events(Platform *platform, Input &input) {
    SDL_Event event;

    input.clear();

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT: {
                platform->running = false;
                break;
            }

            case SDL_KEYDOWN:
            case SDL_KEYUP: {
                SDL_Keycode key = event.key.keysym.sym;

                bool is_down = (event.key.state == SDL_PRESSED);
                bool was_down = (event.key.state == SDL_RELEASED) || (event.key.repeat != 0);

                input.set(key, is_down, was_down);

                break;
            }
        }
    }
}

int main(int argc, char **argv) {
    Input input = {};
    Platform platform = create_platform();

    while (platform.running) {
        handle_events(&platform, input);

        SDL_SetRenderDrawColor(platform.renderer, 50, 125, 250, 255);
        SDL_RenderClear(platform.renderer);
        SDL_RenderPresent(platform.renderer);
    }

    destroy_platform(&platform);

    return 0;
}