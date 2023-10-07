#include "furi_hal_speaker.h"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

extern int16_t global_sound_current;
extern float global_sound_freq;
extern float global_sound_volume;

static float vol_g = 0;
static float freq_g = 0;
static bool thread_running = false;

pthread_t sine_thread_id;

static void* sine_cb(void* ctx) {
    UNUSED(ctx);
    /*int16_t vol_l = (vol_g / 60.0) * 32767;
    uint64_t time = 0;
    while(true) {
        global_sound_current = vol_l * sin(time * freq_g * M_PI * 2 / AUDIO_FREQUENCY);
        // printf("%f %f %f %hu\r\n", freq_g, sin(2 * M_PI * freq_g * time + 0), vol_l * sin(2 * M_PI * freq_g * time + 0), global_sound_current);
        // #ifdef _WIN32
        //     Sleep(1); // Windows can't sleep for less than a millisecond. What a shame!
        // #else
        //     usleep((1.0 / AUDIO_FREQUENCY) * 1000000); // IFNOTWORK freq = freq_g
        // #endif
        SDL_Delay((1.0 / AUDIO_FREQUENCY) * 1000);
        time++;
    }*/
    global_sound_freq = freq_g;
    global_sound_volume = vol_g;

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
    thread_running = true;
    pthread_create(&sine_thread_id, NULL, sine_cb, NULL);
}

void furi_hal_speaker_set_volume(float volume) {
    vol_g = volume;
    return;
}

void furi_hal_speaker_stop() {
    if(thread_running)
        pthread_cancel(sine_thread_id);
    thread_running = false;
    global_sound_freq = 0;
}