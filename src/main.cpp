#include "types.h"
#include "input.h"

#include <stdio.h>
#include <stdlib.h>

#include <array>
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

#include <SDL.h>

#include <imgui.h>
#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_sdlrenderer.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class TimeStep {
public:
    void tick() {
        u32 now_time = SDL_GetTicks();
        m_delta_time = now_time - m_last_time;
        m_last_time = now_time;
    }

    f32 delta_time() const {
        return (f32)m_delta_time / 1000.0f;
    }

private:
    u32 m_last_time = 0;
    u32 m_delta_time = 0;
};

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

    /* ----- INIT IMGUI ----- */
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(platform.window, platform.renderer);
    ImGui_ImplSDLRenderer_Init(platform.renderer);

    return platform;
}

void destroy_platform(Platform *platform) {
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

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

SDL_Texture *load_texture(SDL_Renderer *renderer, const char *filename) {
    int w, h, comp;
    u8 *pixels = stbi_load(filename, &w, &h, &comp, 4);

    if (!pixels) {
        printf("failed to load image: %s\n", filename);
        return nullptr;
    }

    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STATIC, w, h);

    if (!texture) {
        printf("sdl create texture failed: %s\n", SDL_GetError());
        return nullptr;
    }

    SDL_UpdateTexture(texture, nullptr, pixels, w * 4);

    return texture;
}

struct Transform {
    Vec2 position;
    Vec2 scale;
    Vec2 rotation;
};

struct Sprite {
    SDL_Texture *texture;
    SDL_Rect source_rect;
};

enum EntityFlags {
    ACTIVE      = 1 << 0,
    SPRITE      = 1 << 1,
    PLAYER      = 1 << 2,
};

struct Entity {
    u32 id;
    u32 flags;

    Transform transform;
    Sprite sprite;
};

int main(int argc, char **argv) {
    Input input = {};
    TimeStep timestep = {};
    std::vector<Entity> entities = {};

    Platform platform = create_platform();

    Entity entity = {};

    entity.id = 1;
    entity.flags = ACTIVE | SPRITE | PLAYER;

    entity.transform.position = Vec2(32, 32);
    entity.transform.scale = Vec2(3, 3);

    entity.sprite.texture = load_texture(platform.renderer, "res/test.png");
    entity.sprite.source_rect = { 0, 0, 16, 16 };

    entities.push_back(entity);

    while (platform.running) {
        timestep.tick();

        handle_events(&platform, input);

        SDL_SetRenderDrawColor(platform.renderer, 50, 125, 250, 255);
        SDL_RenderClear(platform.renderer);

        for (Entity &entity : entities) {
            if (!(entity.flags & ACTIVE)) {
                continue;
            }

            if (entity.flags & PLAYER) {
                Vec2 velocity = {};

                velocity.x = input.key_down('d') - input.key_down('a');
                velocity.y = input.key_down('s') - input.key_down('w');

                entity.transform.position += velocity * 150 * timestep.delta_time();
            }

            if ((entity.flags & SPRITE) && entity.sprite.texture) {
                int w, h;
                SDL_QueryTexture(entity.sprite.texture, nullptr, nullptr, &w, &h);

                SDL_FRect dest = {
                    entity.transform.position.x,
                    entity.transform.position.y,
                    w * entity.transform.scale.x,
                    h * entity.transform.scale.y
                };

                SDL_RenderCopyF(platform.renderer, entity.sprite.texture, &entity.sprite.source_rect, &dest);
            }
        }
        
        SDL_RenderPresent(platform.renderer);
    }
    
    destroy_platform(&platform);

    return 0;
}