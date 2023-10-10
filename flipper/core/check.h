#pragma once
#include <crash.h>

#define furi_assert( ... ) \
    if(!(__VA_ARGS__)) \
        crash(CRASH_ASSERT_FAILED, "Assertion failed!");

#define furi_check( ... ) \
    if(!(__VA_ARGS__)) \
        crash(CRASH_CHECK_FAILED, "Check failed!");

#define furi_crash( ... ) \
    crash(CRASH_UNKNOWN, "Unknown error!");
