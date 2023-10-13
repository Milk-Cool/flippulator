#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define CRASH_OK 0
#define CRASH_ASSERT_FAILED 1
#define CRASH_CHECK_FAILED 2
#define CRASH_UNKNOWN 3
#define CRASH_UNKNOWN_NOTIFICATION_MESSAGE_TYPE 4

void crash_print(const char* msg);
void crash(uint8_t code, const char* msg);