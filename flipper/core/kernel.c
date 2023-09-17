#include "kernel.h"

void furi_delay_ms(uint32_t time) {
    #ifdef _WIN32
        Sleep(time);
    #else
        usleep(time * 1000);
    #endif
}
void furi_delay_us(uint32_t time) {
    #ifdef _WIN32
        Sleep(time / 1000); // Windows can't sleep for less than a millisecond
    #else
        usleep(time);
    #endif
}