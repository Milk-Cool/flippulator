#include <furi_hal_light.h>
#include <furi.h>
#include <furi_hal.h>
#include "notification.h"
#include "notification_messages.h"
#include "notification_app.h"
#include <pthread.h>
#include <stdio.h>

void notification_message_save_settings(NotificationApp* app) {
    // TODO
}

static NotificationApp* notification_app_alloc() {
    NotificationApp* app = malloc(sizeof(NotificationApp));
    app->queue = furi_message_queue_alloc(8, sizeof(NotificationAppMessage));
    return app;
}

static void* notification_cb(void* ctx) {
    NotificationApp* app = ctx;
    NotificationAppMessage message;
    while(true)
        if(furi_message_queue_get(app->queue, &message, 100) == FuriStatusOk) {
            // printf("%d %lu %lu %lu\n", notification_sequence == NULL ? 1 : 0, sizeof(notification_sequence) / sizeof(NotificationMessage), sizeof(notification_sequence), sizeof(NotificationMessage));
            uint32_t notification_message_index = 0;
            const NotificationMessage* notification_message = (*message.sequence)[notification_message_index];
            while(notification_message != NULL) {
                switch(notification_message->type) {
                    case NotificationMessageTypeLedDisplayBacklight:
                        furi_hal_light_set(LightBacklight, notification_message->data.led.value);
                        break;
                    case NotificationMessageTypeLedDisplayBacklightEnforceOn:
                        break;
                    case NotificationMessageTypeLedDisplayBacklightEnforceAuto:
                        break;
                    case NotificationMessageTypeLedRed:
                        furi_hal_light_set(LightRed, notification_message->data.led.value);
                        break;
                    case NotificationMessageTypeLedGreen:
                        furi_hal_light_set(LightGreen, notification_message->data.led.value);
                        break;
                    case NotificationMessageTypeLedBlue:
                        furi_hal_light_set(LightBlue, notification_message->data.led.value);
                        break;
                    case NotificationMessageTypeLedBlinkStart:
                        furi_hal_light_blink_start(notification_message->data.led_blink.color, 255, notification_message->data.led_blink.on_time, notification_message->data.led_blink.period);
                        break;
                    case NotificationMessageTypeLedBlinkColor:
                        furi_hal_light_blink_set_color(notification_message->data.led_blink.color);
                        break;
                    case NotificationMessageTypeLedBlinkStop:
                        furi_hal_light_blink_stop();
                        break;
                    case NotificationMessageTypeVibro:
                        furi_hal_vibro_on(notification_message->data.vibro.on);
                        break;
                    case NotificationMessageTypeSoundOn:
                        furi_hal_speaker_stop();
                        furi_hal_speaker_start(notification_message->data.sound.frequency, notification_message->data.sound.volume * 60);
                        break;
                    case NotificationMessageTypeSoundOff:
                        furi_hal_speaker_stop();
                        break;
                    case NotificationMessageTypeDelay:
                        furi_delay_ms(notification_message->data.delay.length);
                        break;
                    // TODO: settings
                    case NotificationMessageTypeDoNotReset:
                    case NotificationMessageTypeForceSpeakerVolumeSetting:
                    case NotificationMessageTypeForceVibroSetting:
                    case NotificationMessageTypeForceDisplayBrightnessSetting:
                    case NotificationMessageTypeLedBrightnessSettingApply:
                    case NotificationMessageTypeLcdContrastUpdate:
                        break;
                }
                notification_message = (*message.sequence)[++notification_message_index];
            }
        } else furi_delay_ms(1);
    return NULL;
}

void notification_srv_init() {
    NotificationApp* app = notification_app_alloc();
    furi_record_create(RECORD_NOTIFICATION, app);
    pthread_t thread;
    pthread_create(&thread, NULL, notification_cb, app);
}