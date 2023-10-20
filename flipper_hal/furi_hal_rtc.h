// TODO
#pragma once
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    // Time
    uint8_t hour; /**< Hour in 24H format: 0-23 */
    uint8_t minute; /**< Minute: 0-59 */
    uint8_t second; /**< Second: 0-59 */
    // Date
    uint8_t day; /**< Current day: 1-31 */
    uint8_t month; /**< Current month: 1-12 */
    uint16_t year; /**< Current year: 2000-2099 */
    uint8_t weekday; /**< Current weekday: 1-7 */
} FuriHalRtcDateTime;

void furi_hal_rtc_get_datetime(FuriHalRtcDateTime* datetime);
uint32_t furi_hal_rtc_get_timestamp();