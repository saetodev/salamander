#include "types.h"
#include "input.h"

#include <stdio.h>
#include <stdlib.h>

#include <array>
#include <memory>
#include <vector>
#include <string>

#include <SDL.h>

#include <imgui.h>
#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_sdlrenderer.h>

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
        ImGui_ImplSDL2_ProcessEvent(&event);

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

struct Transform {
    Vec2 position;
    Vec2 scale;
    Vec2 rotation;
};

int main(int argc, char **argv) {
    Input input = {};
    Platform platform = create_platform();

    /* ----- IMGUI ----- */
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(platform.window, platform.renderer);
    ImGui_ImplSDLRenderer_Init(platform.renderer);

    bool show_ui = true;

    /* ----- TIME ----- */
    u32 last_time = SDL_GetTicks();

    float time_step = 1.0f;
    float avg_timer = time_step;

    f32 ui_delta_time = 0.0f;

    /* ----- GAME OBJECTS ----- */
    std::vector<Transform> transform_components = {};

    for (int i = 0; i < 5; i++) {
        Transform transform = {
            Vec2( 32.0f + (48.0f * i), 32.0f),
            Vec2(1.0f, 1.0f),
        };

        transform_components.push_back(transform);
    }

    while (platform.running) {
        u32 now_time = SDL_GetTicks();
        f32 delta_time = (f32)(now_time - last_time) / 1000.0f;
        last_time = now_time;

        handle_events(&platform, input);

        avg_timer += delta_time;
        if (avg_timer >= time_step) {
            ui_delta_time = delta_time;
            avg_timer = 0.0f;
        }

        if (input.key_pressed(' ')) {
            show_ui = !show_ui;
            avg_timer = 0.0f;
        }

        ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        if (show_ui) {
            ImGui::Begin("UI Window", &show_ui);
            ImGui::Text("Delta Time: %f ms", ui_delta_time * 1000);

            if (ImGui::BeginListBox("Transforms")) {
                for (i32 i = 0; i < transform_components.size(); i++) {
                    const Transform &transform = transform_components.at(i);

                    if (ImGui::CollapsingHeader("test")) {
                        ImGui::Text("Position: x: %f, y: %f", transform.position.x, transform.position.y);
                        ImGui::Text("Scale: x: %f, y: %f", transform.scale.x, transform.scale.y);
                        ImGui::Text("Rotation: x: %f, y: %f", transform.rotation.x, transform.rotation.y);
                    }
                }

                ImGui::EndListBox();
            }

            ImGui::End();
        }

        ImGui::Render();

        SDL_SetRenderDrawColor(platform.renderer, 50, 125, 250, 255);
        SDL_RenderClear(platform.renderer);

        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
        
        SDL_RenderPresent(platform.renderer);
    }

    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    
    destroy_platform(&platform);

    return 0;
}