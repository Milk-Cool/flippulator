#include "crash.h"
#include <ncurses.h>

void crash(uint8_t code, const char* msg) {
    endwin();
    printf("\e[1;91mCRASHED DUE TO: %s\e[0m\r\n", msg);
    exit(code);
}