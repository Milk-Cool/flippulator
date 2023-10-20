#pragma once
#include <crash.h>

#define furi_assert( ... ) \
    if(!(__VA_ARGS__)) \
        crash(CRASH_ASSERT_FAILED, CRASHTEXT_ASSERT_FAILED);

#define furi_check( ... ) \
    if(!(__VA_ARGS__)) \
        crash(CRASH_CHECK_FAILED, CRASHTEXT_CHECK_FAILED);

#define furi_crash( ... ) \
    crash(CRASH_UNKNOWN, CRASHTEXT_UNKNOWN);
