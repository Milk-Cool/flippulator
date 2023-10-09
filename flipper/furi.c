#include "furi.h"

/*void furi_assert(void* expr) {
    if(!expr)
        crash(CRASH_ASSERT_FAILED, "Assertion failed!");
}
void furi_check(void* expr) {
    if(!expr)
        crash(CRASH_CHECK_FAILED, "Check failed!");
}*/

void furi_init() {
    printf("Started initializing...\n");
    if(!furi_record_status()) {
        furi_record_init();
        printf("Initialized records.\n");
    }
    gui_srv(NULL);
    printf("Initialized the GUI service.\n");
    notification_srv_init();
    printf("Initialized the notification service.\n");
}