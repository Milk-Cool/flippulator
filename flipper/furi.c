#include "furi.h"

#include <flippulator_defines.h>

// Services
#include "gui/gui_srv.h"
#include "notification/notification_app.h"
#include "cli/cli.h"
#include "storage/storage.h"

// Termios
#include <termios.h>

/* #define START_SRV(name, funcname) FuriThread* name = furi_thread_alloc();\
    furi_thread_set_stack_size(name, 2 * 1024);\
    furi_thread_set_callback(name, funcname);\
    furi_thread_start(name);*/
#define DEFINE_SRV(middlename, funcname) static void* middlename(void* ctx) { \
        UNUSED(ctx);\
        funcname(NULL);\
        return NULL;\
    }
#define START_SRV(middlename, threadname) pthread_t threadname;\
    pthread_create(&threadname, NULL, middlename, NULL);

DEFINE_SRV(gui_middle, gui_srv)
DEFINE_SRV(notification_middle, notification_srv)
DEFINE_SRV(storage_middle, storage_srv)
DEFINE_SRV(cli_middle, cli_srv)

void furi_init() {
    printf("Started initializing...\n");
    if(!furi_record_status()) {
        furi_record_init();
        printf("Initialized records.\n");
    }
    START_SRV(gui_middle, gui_thread)
    printf("Initialized the GUI service.\n");
    START_SRV(notification_middle, notification_thread)
    printf("Initialized the notification service.\n");
    START_SRV(storage_middle, storage_thread)
    printf("Initialized the storage service.\n");
    START_SRV(cli_middle, cli_thread)
    printf("Initialized the CLI service.\n");

    printf("Waiting for threads to start... (%dms)\n", FLIPPULATOR_WAIT_SERVICES_MS);

    furi_delay_ms(FLIPPULATOR_WAIT_SERVICES_MS); // Waiting for all threads to start
}