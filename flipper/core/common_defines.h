#pragma once

#include "core_defines.h"
#include <pthread.h>

// Defines replacing the FreeRTOS ones
#define configMAX_PRIORITIES 5
#define configSTACK_DEPTH_TYPE uint16_t

typedef uint16_t StackType_t;
typedef pthread_t TaskHandle_t;