#include "furi_hal_light.h"
#include <pthread.h>
#include <furi.h>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

extern uint8_t global_led[3];
extern uint8_t global_backlight_brightness;

static pthread_t blink_thread;
static Light light_g;
static uint8_t brightness_g;
static uint16_t on_time_g;
static uint16_t period_g;

static void* blink_cb(void* ctx) {
    UNUSED(ctx);
    while(true) {
        if(!light_g || !brightness_g || !on_time_g || !period_g) {
            #ifdef _WIN32
                Sleep(1);
            #else
                usleep(1000);
            #endif
            continue;
        }
        if(light_g == LightRed)
            global_led[0] = brightness_g;
        else if(light_g == LightGreen)
            global_led[1] = brightness_g;
        else if(light_g == LightBlue)
            global_led[2] = brightness_g;
        else if(light_g == LightBacklight)
            global_backlight_brightness = brightness_g;
        #ifdef _WIN32
            Sleep(on_time_g);
        #else
            usleep(on_time_g * 1000);
        #endif
        if(light_g == LightRed)
            global_led[0] = 0;
        else if(light_g == LightGreen)
            global_led[1] = 0;
        else if(light_g == LightBlue)
            global_led[2] = 0;
        else if(light_g == LightBacklight)
            global_backlight_brightness = 0;
        #ifdef _WIN32
            Sleep(period_g);
        #else
            usleep(period_g * 1000);
        #endif
    }
    return NULL;
}

void furi_hal_light_init() {
    return;
}

void furi_hal_light_set(Light light, uint8_t value) {
    if(light == LightRed)
        global_led[0] = value;
    else if(light == LightGreen)
        global_led[1] = value;
    else if(light == LightBlue)
        global_led[2] = value;
    else if(light == LightBacklight)
        global_backlight_brightness = value;
}

void furi_hal_light_blink_start(Light light, uint8_t brightness, uint16_t on_time, uint16_t period) {
    light_g = light;
    brightness_g = brightness;
    on_time_g = on_time;
    period_g = period;
    pthread_create(&blink_thread, NULL, blink_cb, NULL);
}

void furi_hal_light_blink_stop() {
    pthread_cancel(blink_thread);
}

void furi_hal_light_blink_set_color(Light light) {
    light_g = light;
}

void furi_hal_light_sequence(const char* sequence) {
    do {
        if(*sequence == 'R') {
            furi_hal_light_set(LightRed, 0xFF);
        } else if(*sequence == 'r') {
            furi_hal_light_set(LightRed, 0x00);
        } else if(*sequence == 'G') {
            furi_hal_light_set(LightGreen, 0xFF);
        } else if(*sequence == 'g') {
            furi_hal_light_set(LightGreen, 0x00);
        } else if(*sequence == 'B') {
            furi_hal_light_set(LightBlue, 0xFF);
        } else if(*sequence == 'b') {
            furi_hal_light_set(LightBlue, 0x00);
        } else if(*sequence == 'W') {
            furi_hal_light_set(LightBacklight, 0xFF);
        } else if(*sequence == 'w') {
            furi_hal_light_set(LightBacklight, 0x00);
        } else if(*sequence == '.') {
            furi_delay_ms(250);
        } else if(*sequence == '-') {
            furi_delay_ms(500);
        }
        sequence++;
    } while(*sequence != 0);
}