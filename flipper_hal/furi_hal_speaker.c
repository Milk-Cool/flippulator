#include "furi_hal_speaker.h"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

extern int16_t global_sound_current;

float vol_g = 0;
float freq_g = 0;

pthread_t sine_thread_id;

static void* sine_cb(void* ctx) {
    UNUSED(ctx);
    float frequency = freq_g;
    uint64_t time = 0;
    while(true) {
        global_sound_current = 100 * vol_g * sin((2 * M_PI * frequency * time + 0) / 44100);
        // printf("%f %f %f %hu\r\n", frequency, sin(2 * M_PI * frequency * time + 0), 100 * vol_g * sin(2 * M_PI * frequency * time + 0), global_sound_current);
        #ifdef _WIN32
            Sleep(1); // Windows can't sleep for less than a millisecond. What a shame!
        #else
            usleep(1.0 / 44100); // 16.666 ms // IFNOTWORK freq = frequency
        #endif
        time++;
    }
    return NULL;
}

void furi_hal_speaker_init() {
    return;
}

void furi_hal_speaker_deinit() {
    return;
}

bool furi_hal_speaker_acquire(uint32_t timeout) {
    UNUSED(timeout);
    return true;
}

void furi_hal_speaker_release() {
    return;
}

bool furi_hal_speaker_is_mine() {
    return true;
}

void furi_hal_speaker_start(float frequency, float volume) {
    vol_g = volume;
    freq_g = frequency;
    float freq_copy = frequency; // So that it does not disapper after the call
    pthread_create(&sine_thread_id, NULL, sine_cb, NULL);
}

void furi_hal_speaker_set_volume(float volume) {
    vol_g = volume;
    return;
}

void furi_hal_speaker_stop() {
    pthread_cancel(sine_thread_id);
}