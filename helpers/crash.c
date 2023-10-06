#include "crash.h"
#include <gui/gui.h>

void crash_print(const char* msg) {
    printf("\e[1;91mCRASHED DUE TO: %s\e[0m\r\n", msg);
}
void crash(uint8_t code, const char* msg) {
    crash_print(msg);
    exit_sdl(code);
}