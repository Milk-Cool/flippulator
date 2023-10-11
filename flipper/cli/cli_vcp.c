#include "cli_i.h"
#include <stdio.h>
#include <pthread.h>
#include <time.h>

static bool initialized = false;
static bool connected = false;

static bool rx_done;
static size_t rx_size;
typedef struct {
    uint8_t* buffer;
    size_t size;
} RxData;
static void* rx_get(void* ctx_) {
    RxData* ctx = ctx_;
    for(; rx_size < ctx->size; rx_size++) {
        ctx->buffer[rx_size] = getchar();
    }
    rx_done = true;
    return NULL;
}

static void cli_vcp_init() {
    initialized = true;
    connected = true;
}

static void cli_vcp_deinit() {
    initialized = false;
}

static size_t cli_vcp_rx(uint8_t* buffer, size_t size, uint32_t timeout) {
    rx_done = false;
    rx_size = 0;
    pthread_t rx_thread_id;
    RxData ctx = { buffer, size };
    pthread_create(&rx_thread_id, NULL, rx_get, &ctx);
    uint64_t start_time = (uint64_t)time(NULL);
    while(!rx_done && (uint64_t)time(NULL) < start_time + timeout)
        furi_delay_tick(1);
    pthread_cancel(rx_thread_id);
    return rx_size;
}

static void cli_vcp_tx(const uint8_t* buffer, size_t size) {
    // fwrite(buffer, 1, size, stdout);
    for(unsigned int i = 0; i < size; i++)
        printf("%c", (char)buffer[i]);
}

static void cli_vcp_tx_stdout(const char* data, size_t size) {
    cli_vcp_tx((const uint8_t*)data, size);
}

static bool cli_vcp_is_connected() {
    return connected;
}

CliSession cli_vcp = {
    cli_vcp_init,
    cli_vcp_deinit,
    cli_vcp_rx,
    cli_vcp_tx,
    cli_vcp_tx_stdout,
    cli_vcp_is_connected
};