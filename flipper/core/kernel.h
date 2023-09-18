#pragma once

#include <stdint.h>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif


void furi_delay_ms(uint32_t time);
void furi_delay_tick(uint32_t time);
void furi_delay_us(uint32_t time);