#include "gui_i.h"

#include <pthread.h>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define FLIPPULATOR_FONT_SIZE 16

extern bool global_vibro_on;

static SDL_Renderer* renderer;
static SDL_Window* window;
static SDL_Rect rect;
static SDL_Event event;
static TTF_Font* HaxrCorp4089;
static SDL_Color Black = { 0x00, 0x00, 0x00 };
static bool running = true;

#include <stdio.h>

Gui* gui_alloc() {
    Gui* gui = malloc(sizeof(Gui));
    gui->canvas = canvas_init();
    return gui;
}

void exit_sdl(uint8_t code) {
    // FIXME: crashes when exiting
    // SDL_DestroyRenderer(renderer);
    // SDL_DestroyWindow(window);
    // SDL_Quit();
    exit(code);
}

// TODO: long presses and stuff
static void* handle_input(void* _view_port) {
    ViewPort* view_port = _view_port;
    while(running) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) exit_sdl(0);
            if(event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
                InputEvent* e = malloc(sizeof(InputEvent));
                e->type = event.type == SDL_KEYDOWN ? InputTypePress : InputTypeRelease;
                bool flag = true;
                if(event.key.keysym.sym == SDLK_UP) e->key = InputKeyUp;
                else if(event.key.keysym.sym == SDLK_DOWN) e->key = InputKeyDown;
                else if(event.key.keysym.sym == SDLK_LEFT) e->key = InputKeyLeft;
                else if(event.key.keysym.sym == SDLK_RIGHT) e->key = InputKeyRight;
                else if(event.key.keysym.sym == SDLK_z) e->key = InputKeyOk;
                else if(event.key.keysym.sym == SDLK_x) e->key = InputKeyBack;
                else flag = false;
                if(view_port->input_callback != NULL && flag)
                    view_port->input_callback(e, view_port->input_callback_context);
            }
        }
    }
    return NULL;
}

static void renderMessage(
    char* msg,
    int x,
    int y,
    int width
) {
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(HaxrCorp4089, msg, Black);
    SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    SDL_Rect message_rect = { x, y, NULL, NULL };

    TTF_SizeText(HaxrCorp4089, msg, &message_rect.w, &message_rect.h);

    message_rect.w *= 2;
    message_rect.h *= 2;

    SDL_RenderCopy(renderer, message, NULL, &message_rect);

    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(message);
}

// TODO: multiple viewports support
static void* handle_gui(void* _view_port) {
    ViewPort* view_port = _view_port;
    while(running) {
        if(view_port->draw_callback != NULL)
            view_port->draw_callback(view_port->gui->canvas, view_port->draw_callback_context);
        
        SDL_SetRenderDrawColor(renderer, 0xff, 0x82, 0x00, 0xff);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
        
        rect.x = 0;
        rect.y = 320;
        rect.w = 640;
        rect.h = 3;
        SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderFillRect(renderer, &rect);

        char* msg_vibro = malloc(sizeof(char) * 12);
        snprintf(msg_vibro, 12, "Vibro: %s", global_vibro_on ? "On" : "Off");
        renderMessage(msg_vibro, 20, 340, 100);

        for(uint8_t x = 0; x < view_port->width / 8; x++) // Tile X
            for(uint8_t y = 0; y < view_port->height / 8; y++) // Tile Y
                //if(canvas_get_buffer(view_port->gui->canvas)[view_port->gui->canvas->offset_x + x + (view_port->gui->canvas->offset_y + y) * view_port->width] != 1)
                //    continue;
                for(uint8_t i = 0; i < 8; i++) // Tile row
                    for(uint8_t j = 0; j < 8; j++) { // Tile column
                        if(!(canvas_get_buffer(view_port->gui->canvas)[x * 8 + y * view_port->width + i] & (1 << j))) continue;
                        rect.x = (8 * x + i) * 5;
                        rect.y = (8 * y + j) * 5;
                        rect.w = 5;
                        rect.h = 5;
                        SDL_RenderDrawRect(renderer, &rect);
                        SDL_RenderFillRect(renderer, &rect);
                    }
        SDL_RenderPresent(renderer);
        // ~60 FPS
        #ifdef _WIN32
            Sleep(16); // 16ms
        #else
            usleep(16666); // 16.666 ms
        #endif
    }
    return NULL;   
}
void gui_add_view_port(Gui* gui, ViewPort* view_port, GuiLayer layer) {
    UNUSED(layer);
    furi_assert(gui);
    furi_assert(view_port);
    gui->view_port = view_port;
    view_port->gui = gui;

    TTF_Init();

    HaxrCorp4089 = TTF_OpenFont("haxrcorp-4089.ttf", FLIPPULATOR_FONT_SIZE);

    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software");
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "Flippulator");

    pthread_t draw_thread_id;
    pthread_create(&draw_thread_id, NULL, handle_gui, view_port);
    pthread_t input_thread_id;
    pthread_create(&input_thread_id, NULL, handle_input, view_port);
}
void gui_remove_view_port(Gui* gui, ViewPort* view_port) {
    // TODO
}

int32_t gui_srv(void* p) {
    UNUSED(p);
    Gui* gui = gui_alloc();
    furi_record_create(RECORD_GUI, gui);
    return 0;
}