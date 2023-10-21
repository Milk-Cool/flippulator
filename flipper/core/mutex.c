#include "mutex.h"
#include "check.h"
#include "common_defines.h"
#include "kernel.h"
#include <time.h>

typedef struct {
    FuriMutex* mutex;
    FuriThreadId id;
    bool* done;
} FuriMutexCtx;

static void* acquire_cb(void* ctx_) {
    FuriMutexCtx* ctx = ctx_;
    while((uint64_t)furi_mutex_get_owner(ctx->mutex) != MUTEX_NO_OWNER)
        furi_delay_tick(1);
    ctx->mutex->owner = (uint64_t)ctx->id;
    (*ctx->done) = true;
    return NULL;
}

FuriMutex* furi_mutex_alloc(FuriMutexType type) {
    FuriMutex* mutex = (FuriMutex*)malloc(sizeof(uint64_t) * 2);
    mutex->type = type;
    mutex->owner = MUTEX_NO_OWNER;
    return mutex;
}

void furi_mutex_free(FuriMutex* instance) {
    furi_assert(instance);

    free(instance);
}

FuriStatus furi_mutex_acquire(FuriMutex* instance, uint32_t timeout) {
    pthread_t thread_id;
    FuriThreadId id = furi_thread_get_current_id();
    bool done = false;
    FuriMutexCtx ctx = { instance, (FuriThreadId)id, &done };
    pthread_create(&thread_id, NULL, acquire_cb, &ctx);
    uint64_t start_time = (uint64_t)time(NULL);
    while(true) {
        furi_delay_tick(1);
        if(done) return FuriStatusOk;
        if((uint64_t)time(NULL) >= start_time + timeout / 1000.0) {
            pthread_cancel(thread_id);
            return FuriStatusErrorTimeout;
        }
    }
    return FuriStatusError;
}

FuriStatus furi_mutex_release(FuriMutex* instance) {
    if(instance->owner != (uint64_t)furi_thread_get_current_id())
        return FuriStatusError;
    instance->owner = MUTEX_NO_OWNER;
    return FuriStatusOk;
}

FuriThreadId furi_mutex_get_owner(FuriMutex* instance) {
    return (FuriThreadId)instance->owner;;
}