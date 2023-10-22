#include "gui_i.h"

#include <pthread.h>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <flippulator_defines.h>
#include <termios.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define FLIPPULATOR_FONT_SIZE 16

extern bool global_vibro_on;
extern float global_sound_freq;
extern float global_sound_volume;
extern uint8_t global_led[3];
extern uint8_t global_backlight_brightness;

extern struct termios global_old_tio;

static SDL_Renderer* renderer;
static SDL_Window* window;
static SDL_Rect rect;
static SDL_Event event;
static TTF_Font* HaxrCorp4089;
static SDL_Color Black = { 0x00, 0x00, 0x00 };
static SDL_AudioDeviceID audio_device;
static SDL_AudioSpec audio_spec;
static bool running = true;
static float s_time = 0;

#include <stdio.h>
#include <termios.h>

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
    tcsetattr(STDIN_FILENO, TCSANOW, &global_old_tio);
    exit(code);
}

static float sine(uint16_t* snd) {
    float to_ret = sin(s_time);

    s_time += global_sound_freq * M_PI * 2 / AUDIO_FREQUENCY;
    if(s_time >= M_PI * 2)
        s_time -= M_PI * 2;
    
    return to_ret;
}

static void sound_cb(void* ctx, uint8_t* stream, int len) {
    UNUSED(ctx);
    uint16_t* snd = (uint16_t*)stream;
    uint16_t vol_l = (global_sound_volume / 60.0) * 32767;
    len /= sizeof(*snd);
    for(int i = 0; i < len; i++) {
        if(global_sound_freq == 0) {
            snd[i] = 0;
            continue;
        }
        if(AUDIO_WAVE_TYPE) {
            snd[i] = sine(snd) > 0 ? vol_l : -vol_l;
        } else {
            snd[i] = vol_l * sine(snd);
        }
    }
}

// 6 buttons

// 0 Up
// 1 Down
// 2 Right
// 3 Left
// 4 OK
// 5 Back

#define BUTTONS_COUNT 6
// Not a define because it's only used
// in this file

static bool held_down[BUTTONS_COUNT];
static uint64_t held_time[BUTTONS_COUNT];
static InputKey key_map[] = {
    InputKeyUp, InputKeyDown, InputKeyRight,
    InputKeyLeft, InputKeyOk, InputKeyBack
};

static void* input_loop(void* _view_port) {
    ViewPort* view_port = _view_port;
    while(running) {
        for(uint8_t i = 0; i < BUTTONS_COUNT; i++) {
            if(!held_down[i]) continue;
            if(held_time[i] % INPUT_PRESS_TICKS == 0 && held_time[i] != 0) {
                InputEvent* e = malloc(sizeof(InputEvent));
                e->key = key_map[i];
                uint32_t presses = held_time[i] / INPUT_PRESS_TICKS;
                if(presses < INPUT_LONG_PRESS_COUNTS && presses > 0)
                    e->type = InputTypeShort;
                else if(presses == INPUT_LONG_PRESS_COUNTS)
                    e->type = InputTypeLong;
                else if(presses != 0)
                    e->type = InputTypeRepeat;
                view_port->input_callback(e, view_port->input_callback_context);
            }
            held_time[i]++;
        }
        furi_delay_tick(1);
    }
    return NULL;
}

static void* handle_input(void* _view_port) {
    ViewPort* view_port = _view_port;
    while(running) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) exit_sdl(0);
            if(event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
                InputEvent* e = malloc(sizeof(InputEvent));
                e->type = event.type == SDL_KEYDOWN ? InputTypePress : InputTypeRelease;
                bool flag = true;
                uint8_t key;
                if(event.key.keysym.sym == SDLK_UP) key = 0;
                else if(event.key.keysym.sym == SDLK_DOWN) key = 1;
                else if(event.key.keysym.sym == SDLK_LEFT) key = 3;
                else if(event.key.keysym.sym == SDLK_RIGHT) key = 2;
                else if(event.key.keysym.sym == SDLK_z) key = 4;
                else if(event.key.keysym.sym == SDLK_x) key = 5;
                else flag = false;
                if(!held_down[key] || event.type == SDL_KEYUP) {
                    if(flag) {
                        held_time[key] = 0;
                        held_down[key] = event.type == SDL_KEYDOWN;
                        e->key = key_map[key];
                    }
                    if(view_port->input_callback != NULL && flag)
                        view_port->input_callback(e, view_port->input_callback_context);
                }
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
    SDL_Rect message_rect = { x, y, 0, 0 };

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

        char* msg_vibro = malloc(sizeof(char) * 11);
        snprintf(msg_vibro, 11, "Vibro: %s", global_vibro_on ? "On" : "Off");
        renderMessage(msg_vibro, 20, 340, 100);
        char* msg_led = malloc(sizeof(char) * 13);
        snprintf(msg_led, 13, "LED: #%02x%02x%02x", global_led[0], global_led[1], global_led[2]);
        renderMessage(msg_led, 20, 380, 100);
        char* msg_bl = malloc(sizeof(char) * 16);
        snprintf(msg_bl, 16, "Backlight: 0x%02x", global_backlight_brightness);
        renderMessage(msg_bl, 20, 420, 100);

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
    SDL_Init(SDL_INIT_AUDIO);
    SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer);
    SDL_SetWindowTitle(window, FLIPPULATOR_APP_NAME);

    SDL_zero(audio_spec);
    audio_spec.freq = AUDIO_FREQUENCY;
    audio_spec.format = AUDIO_S16SYS;
    audio_spec.channels = 1;
    audio_spec.samples = 1024;
    audio_spec.callback = sound_cb;

    audio_device = SDL_OpenAudioDevice(
        NULL, 0, &audio_spec, NULL, 0);

    SDL_PauseAudioDevice(audio_device, 0);

    pthread_t draw_thread_id;
    pthread_create(&draw_thread_id, NULL, handle_gui, view_port);
    pthread_t input_thread_id;
    pthread_create(&input_thread_id, NULL, handle_input, view_port);
    pthread_t input_loop_id;
    pthread_create(&input_loop_id, NULL, input_loop, view_port);
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