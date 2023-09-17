#include "timer.h"

FuriTimer* furi_timer_alloc(FuriTimerCallback func, FuriTimerType type, void* context) {
    FuriTimer* timer = malloc(sizeof(FuriTimer));

    timer->cb = func;
    timer->type = type;
    timer->ctx = context;

    return timer;
}

void furi_timer_free(FuriTimer* instance) {
    while(furi_timer_is_running(instance))
        #ifdef _WIN32
            Sleep(2);
        #else
            usleep(2000);
        #endif
    free(instance);
}

static void* handler(void* ctx) {
    FuriTimer* timer = ctx;
    do {
        #ifdef _WIN32
            Sleep(timer->delay);
        #else
            usleep(timer->delay * 1000);
        #endif
        timer->cb(timer->ctx);
    } while(timer->type == FuriTimerTypePeriodic && timer->running);
    return NULL;
}

FuriStatus furi_timer_start(FuriTimer* instance, uint32_t ticks) {
    instance->delay = ticks;
    instance->running = 1;
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, handler, instance);
    return FuriStatusOk;
}

FuriStatus furi_timer_stop(FuriTimer* instance) {
    instance->running = 0;
    return FuriStatusOk;
}

uint32_t furi_timer_is_running(FuriTimer* instance) {
    return instance->running;
}