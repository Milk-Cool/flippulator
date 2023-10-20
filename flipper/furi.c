#include "furi.h"

// Services
#include "gui/gui_srv.h"
#include "notification/notification_app.h"
#include "cli/cli.h"
#include "storage/storage.h"

// Termios
#include <termios.h>

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
    // storage_srv(NULL);
    // printf("Initialized the storage service.\n");
    cli_srv(NULL);
    printf("Initialized the CLI service.\n");
}