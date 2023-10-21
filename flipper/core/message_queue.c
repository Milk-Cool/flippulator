#include "message_queue.h"
#include "core_defines.h"
#include "kernel.h"
#include "check.h"
#include <flippulator_defines.h>
#include <pthread.h>

FuriMessageQueue* furi_message_queue_alloc(uint32_t msg_count, uint32_t msg_size) {
    FuriMessageQueue* queue = (FuriMessageQueue*) malloc(12 + msg_count * msg_size);
    ((uint32_t*) queue)[0] = 0;
    ((uint32_t*) queue)[1] = msg_count;
    ((uint32_t*) queue)[2] = msg_size;
    return queue;
    // 4 bytes for pointer
    // 4 bytes for count
    // 4 bytes for size
    // Other bytes for messages
}

void furi_message_queue_free(FuriMessageQueue* queue) {
    furi_assert(queue);
    free(queue);
}

typedef struct {
    FuriMessageQueue* queue;
    const void** ptr;
    bool* done;
} FuriMessageQueueCtx;

static void* get_cb(void* ctx_) {
    FuriMessageQueueCtx* ctx = ctx_;
    while(((uint32_t*) ctx->queue)[0] == 0)
        furi_delay_tick(1);
    const uint32_t pointer = ((uint32_t*) ctx->queue)[0];
    const uint32_t size = ((uint32_t*) ctx->queue)[2];
    memcpy((void*)(*ctx->ptr), ctx->queue + 12, size);
    memcpy(ctx->queue + 12, ctx->queue + 12 + size, size * (pointer - 1));
    ((uint32_t*) ctx->queue)[0] -= 1;
    (*ctx->done) = true;
    return NULL;
}

static void* put_cb(void* ctx_) {
    FuriMessageQueueCtx* ctx = ctx_;
    const uint32_t pointer = ((uint32_t*) ctx->queue)[0];
    const uint32_t count = ((uint32_t*) ctx->queue)[1];
    const uint32_t size = ((uint32_t*) ctx->queue)[2];
    while(((uint32_t*) ctx->queue)[0] == count)
        furi_delay_tick(1);
    memcpy(ctx->queue + 12 + size * pointer, (const void*)(*ctx->ptr), size);
    ((uint32_t*) ctx->queue)[0] += 1;
    (*ctx->done) = true;
    return NULL;
}
 
FuriStatus furi_message_queue_put(FuriMessageQueue* queue, const void* msg_ptr, uint32_t timeout) {
    furi_assert(queue);
    bool done_put = false;
    FuriMessageQueueCtx ctx = { queue, &msg_ptr, &done_put };
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, put_cb, &ctx);
    uint64_t start_time = (uint64_t)time(NULL);
    while(true) {
        furi_delay_tick(1);
        if(done_put) return FuriStatusOk;
        if((uint64_t)time(NULL) >= start_time + timeout / 1000.0) {
            pthread_cancel(thread_id);
            return FuriStatusErrorTimeout;
        }
    }
    #ifdef FLIPPULATOR_USES_WEBASSEMBLY
    furi_delay_us(1);
    #endif
    return FuriStatusError;
}
 
FuriStatus furi_message_queue_get(FuriMessageQueue* queue, void* msg_ptr, uint32_t timeout) {
    furi_assert(queue);
    bool done_get = false;
    FuriMessageQueueCtx ctx = { queue, (const void **)&msg_ptr, &done_get };
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, get_cb, &ctx);
    uint64_t start_time = (uint64_t)time(NULL);
    while(true) {
        furi_delay_tick(1);
        if(done_get) return FuriStatusOk;
        if((uint64_t)time(NULL) >= start_time + timeout / 1000.0) {
            pthread_cancel(thread_id);
            return FuriStatusErrorTimeout;
        }
    }
    #ifdef FLIPPULATOR_USES_WEBASSEMBLY
    furi_delay_us(1);
    #endif
    return FuriStatusError;
}

uint32_t furi_message_queue_get_capacity(FuriMessageQueue* queue) {
    furi_assert(queue);
    return ((uint32_t*) queue)[1];
}

uint32_t furi_message_queue_get_message_size(FuriMessageQueue* queue) {
    furi_assert(queue);
    return ((uint32_t*) queue)[2];
}

uint32_t furi_message_queue_get_count(FuriMessageQueue* queue) {
    furi_assert(queue);
    return ((uint32_t*) queue)[0];
}

uint32_t furi_message_queue_get_space(FuriMessageQueue* queue) {
    furi_assert(queue);
    return furi_message_queue_get_capacity(queue) - furi_message_queue_get_count(queue);
}

FuriStatus furi_message_queue_reset(FuriMessageQueue* queue) {
    furi_assert(queue);
    ((uint32_t*) queue)[0] = 0;
    return FuriStatusOk;
}