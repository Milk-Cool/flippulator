#include "furi.h"
#include <stdio.h>

/*void furi_assert(void* expr) {
    if(!expr)
        crash(CRASH_ASSERT_FAILED, "Assertion failed!");
}
void furi_check(void* expr) {
    if(!expr)
        crash(CRASH_CHECK_FAILED, "Check failed!");
}*/

void furi_init() {
    if(!furi_record_status()) {
        furi_record_init();
    }
    gui_srv(NULL);
}