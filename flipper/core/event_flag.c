#include "event_flag.h"
#include "common_defines.h"
#include "check.h"

#include <pthread.h>
#include <time.h>

#define FURI_EVENT_FLAG_MAX_BITS_EVENT_GROUPS 24U
#define FURI_EVENT_FLAG_INVALID_BITS (~((1UL << FURI_EVENT_FLAG_MAX_BITS_EVENT_GROUPS) - 1U))

FuriEventFlag* furi_event_flag_alloc() {
    uint32_t* flags = malloc(sizeof(uint32_t));
    return (FuriEventFlag*)flags;
}

void furi_event_flag_free(FuriEventFlag* instance) {
    free(instance);
}

uint32_t furi_event_flag_set(FuriEventFlag* instance, uint32_t flags) {
    furi_assert(instance);
    
    (*instance) = flags;
    return flags;
}

uint32_t furi_event_flag_clear(FuriEventFlag* instance, uint32_t flags) {
    furi_assert(instance);
    
    (*instance) = 0;
    return 0;
}

uint32_t furi_event_flag_get(FuriEventFlag* instance) {
    furi_assert(instance);

    return *instance;
}

typedef struct {
    FuriEventFlag* flag;
    uint32_t val;
    bool* done;
} FuriEventFlagCtx;

static void* acquire_cb(void* ctx_) {
    FuriEventFlagCtx* ctx = ctx_;
    while((*ctx->flag) != ctx->val)
        furi_delay_tick(1);
    (*ctx->done) = true;
    return NULL;
}

uint32_t furi_event_flag_wait(
    FuriEventFlag* instance,
    uint32_t flags,
    uint32_t options,
    uint32_t timeout) {
    UNUSED(options);

    furi_assert(instance);
    bool done = false;
    pthread_t thread_id;
    FuriEventFlagCtx ctx = { instance, flags, &done };
    pthread_create(&thread_id, NULL, acquire_cb, &ctx);
    uint64_t start_time = (uint64_t)time(NULL);
    while(true) {
        furi_delay_tick(1);
        if(done) break;
        if((uint64_t)time(NULL) >= start_time + timeout / 1000.0) {
            pthread_cancel(thread_id);
            return 0;
        }
    }
    return flags;
}