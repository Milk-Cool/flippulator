#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define CRASH_OK 0
#define CRASH_ASSERT_FAILED 1
#define CRASH_CHECK_FAILED 2
#define CRASH_UNKNOWN 3
#define CRASH_UNKNOWN_NOTIFICATION_MESSAGE_TYPE 4
#define CRASH_STORAGE_FILE_UNSUPPORTED 5
#define CRASH_UNSUPPORTED_FS_OPERATION 6

#define CRASHTEXT_OK "Ok"
#define CRASHTEXT_ASSERT_FAILED "Assertion failed!"
#define CRASHTEXT_CHECK_FAILED "Check failed!"
#define CRASHTEXT_UNKNOWN "Unknown"
#define CRASHTEXT_UNKNOWN_NOTIFICATION_MESSAGE_TYPE "Unknown notification type!"
#define CRASHTEXT_STORAGE_FILE_UNSUPPORTED "File mode unsupported!"
#define CRASHTEXT_UNSUPPORTED_FS_OPERATION "Unsupported FS operation!"

void crash_print(const char* msg);
void crash(uint8_t code, const char* msg);