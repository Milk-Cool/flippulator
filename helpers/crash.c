#include "crash.h"

void crash(uint8_t code, const char* msg) { 
    printf("\e[1;91mCRASHED DUE TO: %s\e[0m\r\n", msg);
    exit(code);
}