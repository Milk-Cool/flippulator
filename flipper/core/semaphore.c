#include "semaphore.h"
#include "check.h"
#include "kernel.h"
#include <time.h>
#include <string.h>

static bool done;

typedef struct {
    FuriSemaphore* semaphore;
    FuriThreadId id;
} FuriSemaphoreCtx;

FuriSemaphore* furi_semaphore_alloc(uint32_t max_count, uint32_t initial_count) {
    FuriSemaphore* semaphore = malloc(sizeof(FuriSemaphore));
    semaphore->max = max_count;
    semaphore->count = initial_count;
    semaphore->owners = malloc(sizeof(uint64_t) * max_count);
    for(unsigned int i = 0; i < initial_count; i++)
        semaphore->owners[i] = SEMAPHORE_NO_OWNER;
    return semaphore;
}

static void* acquire_cb(void* ctx_) {
    FuriSemaphoreCtx* ctx = ctx_;
    while(furi_semaphore_get_count(ctx->semaphore) == ctx->semaphore->max)
        furi_delay_tick(1);
    ctx->semaphore->owners[ctx->semaphore->count++] = (uint64_t)ctx->id;
    done = true;
    return NULL;
}

void furi_semaphore_free(FuriSemaphore* instance) {
    furi_assert(instance);
    
    free(instance->owners);
    free(instance);
}

FuriStatus furi_semaphore_acquire(FuriSemaphore* instance, uint32_t timeout) {
    furi_assert(instance);

    pthread_t thread_id;
    FuriThreadId id = furi_thread_get_current_id();
    FuriSemaphoreCtx ctx = { instance, (FuriThreadId)id };
    done = false;
    pthread_create(&thread_id, NULL, acquire_cb, &ctx);
    uint64_t start_time = (uint64_t)time(NULL);
    while(true) {
        furi_delay_tick(1);
        if(done) return FuriStatusOk;
        if((uint64_t)time(NULL) >= start_time + timeout) {
            pthread_cancel(thread_id);
            return FuriStatusErrorTimeout;
        }
    }
    return FuriStatusError;
}

FuriStatus furi_semaphore_release(FuriSemaphore* instance) {
    furi_assert(instance);

    uint64_t id = (uint64_t)furi_thread_get_current_id();
    for(unsigned int i = 0; i < instance->count; i++)
        if(instance->owners[i] == id) {
            memcpy(&instance->owners[i], &instance->owners[i + 1], instance->count - i - 1);
            return FuriStatusOk;
        }
    return FuriStatusErrorResource;
}

uint32_t furi_semaphore_get_count(FuriSemaphore* instance) {
    furi_assert(instance);

    return instance->count;
}