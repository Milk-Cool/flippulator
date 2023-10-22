#include "cli_i.h"
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <gui/gui.h>

static bool initialized = false;
static bool connected = false;

typedef struct {
    uint8_t* buffer;
    size_t size;
    bool* done;
    size_t* out_size;
} RxData;
static void* rx_get(void* ctx_) {
    RxData* ctx = ctx_;
    for(; *ctx->out_size < ctx->size; (*ctx->out_size)++) {
        char c = getchar();
        if(c == 4) exit_sdl(0);
        ctx->buffer[*ctx->out_size] = c;
    }
    (*ctx->done) = true;
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
    bool rx_done = false;
    size_t rx_size = 0;
    pthread_t rx_thread_id;
    RxData ctx = { buffer, size, &rx_done, &rx_size };
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