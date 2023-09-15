#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define CRASH_OK 0
#define CRASH_ASSERT_FAILED 1
#define CRASH_CHECK_FAILED 2

void crash(uint8_t code, const char* msg);