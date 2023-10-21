#include "crash.h"
#include <gui/gui.h>
#include <execinfo.h>

#define BACKTRACE_SIZE 20

static void print_bt() {
    void* arr[BACKTRACE_SIZE];
    char** strings;
    int size, i;

    size = backtrace(arr, BACKTRACE_SIZE);
    strings = backtrace_symbols(arr, size);

    if(strings != NULL) {
        printf("at:\n");
        for(i = 0; i < size; i++)
            printf("  %s\n", strings[i]);
    }
    
    free(strings);
}

void crash_print(const char* msg) {
    print_bt();
    printf("\e[1;91mCRASHED DUE TO: %s\e[0m\r\n", msg);
}
void crash(uint8_t code, const char* msg) {
    crash_print(msg);
    exit_sdl(code);
}