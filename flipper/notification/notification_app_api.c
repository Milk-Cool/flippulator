#include <furi.h>
#include <furi_hal.h>
#include "notification.h"
#include "notification_messages.h"
#include "notification_app.h"

void notification_message(NotificationApp* app, const NotificationSequence* sequence) {
    NotificationAppMessage m = {
        .type = NotificationLayerMessage, .sequence = sequence/*, .back_event = NULL*/};
    furi_check(furi_message_queue_put(app->queue, &m, FuriWaitForever) == FuriStatusOk);
};