#include "furi.h"

// Services
#include "gui/gui_srv.h"
#include "notification/notification_app.h"
#include "cli/cli.h"

// Termios
#include <termios.h>

/*void furi_assert(void* expr) {
    if(!expr)
        crash(CRASH_ASSERT_FAILED, "Assertion failed!");
}
void furi_check(void* expr) {
    if(!expr)
        crash(CRASH_CHECK_FAILED, "Check failed!");
}*/

void furi_init() {
    /*setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);*/
    // setbuf(stdin, NULL);
    // setbuf(stdout, NULL);
    // setbuf(stderr, NULL);
    printf("Started initializing...\n");
    if(!furi_record_status()) {
        furi_record_init();
        printf("Initialized records.\n");
    }
    gui_srv(NULL);
    printf("Initialized the GUI service.\n");
    notification_srv_init();
    printf("Initialized the notification service.\n");
    cli_srv(NULL);
    printf("Initialized the CLI service.\n");
}