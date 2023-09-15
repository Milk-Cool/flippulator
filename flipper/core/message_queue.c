#include "message_queue.h"
#include "core_defines.h"

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
    free(queue);
}

FuriStatus furi_message_queue_put(FuriMessageQueue* queue, const void* msg_ptr, uint32_t timeout) {
    UNUSED(timeout);
    const uint32_t pointer = ((uint32_t*) queue)[0];
    const uint32_t count = ((uint32_t*) queue)[1];
    const uint32_t size = ((uint32_t*) queue)[2];
    if(pointer == count) return FuriStatusError;
    memcpy(queue + 12 + size * pointer, msg_ptr, size);
    ((uint32_t*) queue)[0] += 1;
    return FuriStatusOk;
}
 
FuriStatus furi_message_queue_get(FuriMessageQueue* queue, void* msg_ptr, uint32_t timeout) {
    UNUSED(timeout);
    const uint32_t pointer = ((uint32_t*) queue)[0];
    const uint32_t size = ((uint32_t*) queue)[2];
    if(pointer == 0) return FuriStatusError;
    memcpy(msg_ptr, queue + 12, size);
    memcpy(queue + 12, queue + 12 + size, size * pointer);
    ((uint32_t*) queue)[0] -= 1;
    return FuriStatusOk;
}

uint32_t furi_message_queue_get_capacity(FuriMessageQueue* queue) {
    return ((uint32_t*) queue)[1];
}

uint32_t furi_message_queue_get_message_size(FuriMessageQueue* queue) {
    return ((uint32_t*) queue)[2];
}

uint32_t furi_message_queue_get_count(FuriMessageQueue* queue) {
    return ((uint32_t*) queue)[0];
}

uint32_t furi_message_queue_get_space(FuriMessageQueue* queue) {
    return furi_message_queue_get_capacity(queue) - furi_message_queue_get_count(queue);
}

FuriStatus furi_message_queue_reset(FuriMessageQueue* queue) {
    ((uint32_t*) queue)[0] = 0;
    return FuriStatusOk;
}