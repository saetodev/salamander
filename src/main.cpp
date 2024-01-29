#include "types.h"

#include <stdio.h>
#include <stdlib.h>

#include <array>
#include <memory>

#include <SDL.h>

struct Platform {
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    bool running = false;

    Platform() {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            printf("sdl init failed: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }

        window = SDL_CreateWindow("SAL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);

        if (!window) {
            printf("sdl create window failed: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

        if (!renderer) {
            printf("sdl create renderer failed %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }

        running = true;
    }

    ~Platform() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);

        SDL_Quit();
    }
};

class Input {
public:
    void clear() {
        m_keys_pressed.fill(false);
        m_keys_released.fill(false);
    }

    void set(int key, bool is_down, bool was_down) {
        if (key < 0 || key >= KEY_BUFFER_SIZE) {
            return;
        }

        m_keys_down[key] = is_down;
        m_keys_pressed[key] = is_down && !was_down;
        m_keys_released[key] = !is_down && was_down;
    }

    bool key_down(int key) {
        if (key < 0 || key >= KEY_BUFFER_SIZE) {
            return false;
        }

        return m_keys_down[key];
    }

    bool key_pressed(int key) {
        if (key < 0 || key >= KEY_BUFFER_SIZE) {
            return false;
        }

        return m_keys_pressed[key];
    }

    bool key_released(int key) {
        if (key < 0 || key >= KEY_BUFFER_SIZE) {
            return false;
        }

        return m_keys_released[key];
    }

private:
    static constexpr int KEY_BUFFER_SIZE = 256;
    
    std::array<bool, KEY_BUFFER_SIZE> m_keys_down = {};
    std::array<bool, KEY_BUFFER_SIZE> m_keys_pressed = {};
    std::array<bool, KEY_BUFFER_SIZE> m_keys_released = {};
};

int main(int argc, char **argv) {
    Input input = {};

    auto platform = std::make_unique<Platform>();

    while (platform->running) {
        input.clear();

        SDL_Event event;
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

        SDL_SetRenderDrawColor(platform->renderer, 50, 125, 250, 255);
        SDL_RenderClear(platform->renderer);
        SDL_RenderPresent(platform->renderer);

    }

    return 0;
}