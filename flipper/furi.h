#pragma once

#include <stdlib.h>
#include <ncurses.h>

#include "core/base.h"
#include "core/message_queue.h"
#include "core/core_defines.h"
#include "core/record.h"
#include <crash.h>

// Services
#include "gui/gui_srv.h"

#define furi_assert( ... ) \
    if(!__VA_ARGS__) \
        crash(CRASH_ASSERT_FAILED, "Assertion failed!");

#define furi_check( ... ) \
    if(!__VA_ARGS__) \
        crash(CRASH_CHECK_FAILED, "Check failed!");

void furi_init();