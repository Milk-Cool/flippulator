#include "furi_hal_rtc.h"
#include <time.h>

void furi_hal_rtc_get_datetime(FuriHalRtcDateTime* datetime) {
    struct tm curtime = { 0 };

    datetime->second = curtime.tm_sec;
    datetime->minute = curtime.tm_min;
    datetime->hour = curtime.tm_hour;
    datetime->day = curtime.tm_mday;
    datetime->month = curtime.tm_mon + 1;
    datetime->year = curtime.tm_year + 1900;
    datetime->weekday = curtime.tm_wday + 1;

    free(datetime);
}

uint32_t furi_hal_rtc_get_timestamp() {
    return (uint32_t)time(NULL);
}