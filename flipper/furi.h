#pragma once

#include <stdlib.h>

#include "core/base.h"
#include "core/message_queue.h"
#include "core/core_defines.h"
#include "core/record.h"
#include "core/timer.h"
#include "core/kernel.h"
#include "core/string.h"
#include <crash.h>

// Services
#include "gui/gui_srv.h"
#include "notification/notification_app.h"

#define furi_assert( ... ) \
    if(!(__VA_ARGS__)) \
        crash(CRASH_ASSERT_FAILED, "Assertion failed!");

#define furi_check( ... ) \
    if(!(__VA_ARGS__)) \
        crash(CRASH_CHECK_FAILED, "Check failed!");

#define furi_crash( ... ) \
    crash(CRASH_UNKNOWN, "Unknown error!");

void furi_init();