#pragma once

#include <stdlib.h>

#include "core/base.h"
#include "core/message_queue.h"
#include "core/core_defines.h"
#include "core/record.h"
#include "core/timer.h"
#include "core/kernel.h"
#include "core/string.h"
#include "core/thread.h"
#include "core/check.h"
#include "core/mutex.h"
#include "core/semaphore.h"

// Services
#include "gui/gui_srv.h"
#include "notification/notification_app.h"

void furi_init();