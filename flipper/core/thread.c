#include "thread.h"
#include "string.h"

#define THREADS_MAX 1024

static uint32_t flags_g;

typedef struct FuriThreadStdout FuriThreadStdout;

struct FuriThreadStdout {
    FuriThreadStdoutWriteCallback write_callback;
    FuriString* buffer;
};

struct FuriThread {
    FuriThreadState state;
    int32_t ret;

    FuriThreadCallback callback;
    void* context;

    FuriThreadStateCallback state_callback;
    void* state_context;

    char* name;
    char* appid;

    FuriThreadPriority priority;

    TaskHandle_t task_handle;
    size_t heap_size;

    FuriThreadStdout output;

    // Keep all non-alignable byte types in one place,
    // this ensures that the size of this structure is minimal
    bool is_service;
    bool heap_trace_enabled;
    
    bool suspended;

    configSTACK_DEPTH_TYPE stack_size;
};

static FuriThread* threads[THREADS_MAX];
static unsigned int threads_i = 0;

static void furi_thread_set_state(FuriThread* thread, FuriThreadState state) {
    furi_assert(thread);
    thread->state = state;
    if(thread->state_callback) {
        thread->state_callback(state, thread->state_context);
    }
}

FuriThread* furi_thread_alloc() {
    FuriThread* thread = malloc(sizeof(FuriThread));
    thread->output.buffer = furi_string_alloc();
    thread->is_service = false;

    furi_thread_set_appid(thread, "unknown");

    thread->heap_trace_enabled = false;

    threads[threads_i] = thread;
    threads_i++;

    return thread;
}

FuriThread* furi_thread_alloc_ex(
    const char* name,
    uint32_t stack_size,
    FuriThreadCallback callback,
    void* context) {
    FuriThread* thread = furi_thread_alloc();
    furi_thread_set_name(thread, name);
    furi_thread_set_stack_size(thread, stack_size);
    furi_thread_set_callback(thread, callback);
    furi_thread_set_context(thread, context);
    return thread;
}

void furi_thread_free(FuriThread* thread) {
    furi_assert(thread);

    // Ensure that use join before free
    furi_assert(thread->state == FuriThreadStateStopped);

    if(thread->name) free(thread->name);
    if(thread->appid) free(thread->appid);
    furi_string_free(thread->output.buffer);

    for(unsigned int i = 0; i < THREADS_MAX; i++)
        if(threads[i] == thread) {
            threads[i] = NULL;
            memcpy(&threads[i], &threads[i + 1], threads_i - i - 1);
            threads_i--;
            break;
        }

    free(thread);
}

void furi_thread_set_name(FuriThread* thread, const char* name) {
    furi_assert(thread);
    furi_assert(thread->state == FuriThreadStateStopped);
    if(thread->name) free(thread->name);
    thread->name = name ? strdup(name) : NULL;
}

void furi_thread_set_appid(FuriThread* thread, const char* appid) {
    furi_assert(thread);
    furi_assert(thread->state == FuriThreadStateStopped);
    if(thread->appid) free(thread->appid);
    thread->appid = appid ? strdup(appid) : NULL;
}

void furi_thread_mark_as_service(FuriThread* thread) {
    thread->is_service = true;
}

void furi_thread_set_stack_size(FuriThread* thread, size_t stack_size) {
    furi_assert(thread);
    furi_assert(thread->state == FuriThreadStateStopped);
    furi_assert(stack_size % 4 == 0);
    thread->stack_size = stack_size;
}

void furi_thread_set_callback(FuriThread* thread, FuriThreadCallback callback) {
    furi_assert(thread);
    furi_assert(thread->state == FuriThreadStateStopped);
    thread->callback = callback;
}

void furi_thread_set_context(FuriThread* thread, void* context) {
    furi_assert(thread);
    furi_assert(thread->state == FuriThreadStateStopped);
    thread->context = context;
}

void furi_thread_set_priority(FuriThread* thread, FuriThreadPriority priority) {
    furi_assert(thread);
    furi_assert(thread->state == FuriThreadStateStopped);
    furi_assert(priority >= FuriThreadPriorityIdle && priority <= FuriThreadPriorityIsr);
    thread->priority = priority;
}

void furi_thread_set_current_priority(FuriThreadPriority priority) {
    // do nothing
}

FuriThreadPriority furi_thread_get_current_priority() {
    // do nothing
    return FuriThreadPriorityNone;
}

void furi_thread_set_state_callback(FuriThread* thread, FuriThreadStateCallback callback) {
    furi_assert(thread);
    furi_assert(thread->state == FuriThreadStateStopped);
    thread->state_callback = callback;
}

void furi_thread_set_state_context(FuriThread* thread, void* context) {
    furi_assert(thread);
    furi_assert(thread->state == FuriThreadStateStopped);
    thread->state_context = context;
}

FuriThreadState furi_thread_get_state(FuriThread* thread) {
    furi_assert(thread);
    return thread->state;
}

typedef struct {
    FuriThreadCallback cb;
    void* ctx;
} ThreadCbPlusCtx;

static void* run(void* ctx_) {
    ThreadCbPlusCtx* ctx = ctx_;
    ctx->cb(ctx->ctx);
    return NULL;
}

void furi_thread_start(FuriThread* thread) {
    furi_assert(thread);
    furi_assert(thread->callback);
    furi_assert(thread->state == FuriThreadStateStopped);
    furi_assert(thread->stack_size > 0 && thread->stack_size < (UINT16_MAX * sizeof(StackType_t)));

    furi_thread_set_state(thread, FuriThreadStateStarting);

    pthread_t thread_id;
    ThreadCbPlusCtx ctx = { thread->callback, thread->context };
    pthread_create(&thread_id, NULL, run, &ctx);
    thread->task_handle = (TaskHandle_t)thread_id;

    furi_check(thread->task_handle);
}

void furi_thread_cleanup_tcb_event(TaskHandle_t task) {
    // do nothing
}

bool furi_thread_join(FuriThread* thread) {
    int32_t res = (int32_t)pthread_join((pthread_t)thread->task_handle, NULL);
    thread->ret = res;
    return res == 0;
}

FuriThreadId furi_thread_get_id(FuriThread* thread) {
    furi_assert(thread);
    return (FuriThreadId)thread->task_handle;
}

void furi_thread_enable_heap_trace(FuriThread* thread) {
    furi_assert(thread);
    furi_assert(thread->state == FuriThreadStateStopped);
    thread->heap_trace_enabled = true;
}

void furi_thread_disable_heap_trace(FuriThread* thread) {
    furi_assert(thread);
    furi_assert(thread->state == FuriThreadStateStopped);
    thread->heap_trace_enabled = false;
}

size_t furi_thread_get_heap_size(FuriThread* thread) {
    furi_assert(thread);
    furi_assert(thread->heap_trace_enabled == true);
    return thread->heap_size;
}

int32_t furi_thread_get_return_code(FuriThread* thread) {
    furi_assert(thread);
    furi_assert(thread->state == FuriThreadStateStopped);
    return thread->ret;
}

FuriThreadId furi_thread_get_current_id() {
    return (FuriThreadId)pthread_self();
}

FuriThread* furi_thread_get_current() {
    FuriThreadId id = furi_thread_get_current_id();
    for(unsigned int i = 0; i < THREADS_MAX; i++)
        if((FuriThreadId)threads[i]->task_handle == id)
            return threads[i];
    return NULL;
}

void furi_thread_yield() {
    // do nothing
}

/* Limits */
#define MAX_BITS_TASK_NOTIFY 31U
#define MAX_BITS_EVENT_GROUPS 24U

#define THREAD_FLAGS_INVALID_BITS (~((1UL << MAX_BITS_TASK_NOTIFY) - 1U))
#define EVENT_FLAGS_INVALID_BITS (~((1UL << MAX_BITS_EVENT_GROUPS) - 1U))

uint32_t furi_thread_flags_set(FuriThreadId thread_id, uint32_t flags) {
    UNUSED(thread_id);
    flags_g = flags;
    return flags;
}

uint32_t furi_thread_flags_clear(uint32_t flags) {
    flags_g = flags;
    return flags;
}

uint32_t furi_thread_flags_get(void) {
    return flags_g;
}

uint32_t furi_thread_flags_wait(uint32_t flags, uint32_t options, uint32_t timeout) {
    UNUSED(options);
    UNUSED(timeout);
    return flags_g;
}

uint32_t furi_thread_enumerate(FuriThreadId* thread_array, uint32_t array_items) {
    uint32_t count = 0, i = 0;
    // TODO: omptimize
    while(count < array_items && i < THREADS_MAX) {
        if(threads[i] != NULL) {
            thread_array[count] = threads[i];
            count++;
        }
        i++;
    }
    return count;
}

const char* furi_thread_get_name(FuriThreadId thread_id) {
    for(unsigned int i = 0; i < THREADS_MAX; i++)
        if((FuriThreadId)threads[i]->task_handle == thread_id)
            return threads[i]->name;
    return NULL;
}

const char* furi_thread_get_appid(FuriThreadId thread_id) {
    for(unsigned int i = 0; i < THREADS_MAX; i++)
        if((FuriThreadId)threads[i]->task_handle == thread_id)
            return threads[i]->appid;
    return NULL;
}

uint32_t furi_thread_get_stack_space(FuriThreadId thread_id) {
    for(unsigned int i = 0; i < THREADS_MAX; i++)
        if((FuriThreadId)threads[i]->task_handle == thread_id)
            return threads[i]->stack_size;
    return 0;
}

// TODO: implement
/*void furi_thread_set_stdout_callback(FuriThreadStdoutWriteCallback callback) {
    FuriThread* thread = furi_thread_get_current();
    furi_assert(thread);
    __furi_thread_stdout_flush(thread);
    thread->output.write_callback = callback;
}

FuriThreadStdoutWriteCallback furi_thread_get_stdout_callback() {
    FuriThread* thread = furi_thread_get_current();
    furi_assert(thread);
    return thread->output.write_callback;
}

size_t furi_thread_stdout_write(const char* data, size_t size) {
    FuriThread* thread = furi_thread_get_current();
    furi_assert(thread);
    if(size == 0 || data == NULL) {
        return __furi_thread_stdout_flush(thread);
    } else {
        if(data[size - 1] == '\n') {
            // if the last character is a newline, we can flush buffer and write data as is, wo buffers
            __furi_thread_stdout_flush(thread);
            __furi_thread_stdout_write(thread, data, size);
        } else {
            // string_cat doesn't work here because we need to write the exact size data
            for(size_t i = 0; i < size; i++) {
                furi_string_push_back(thread->output.buffer, data[i]);
                if(data[i] == '\n') {
                    __furi_thread_stdout_flush(thread);
                }
            }
        }
    }

    return size;
}

int32_t furi_thread_stdout_flush() {
    FuriThread* thread = furi_thread_get_current();
    furi_assert(thread);
    return __furi_thread_stdout_flush(thread);
}*/

void furi_thread_suspend(FuriThreadId thread_id) {
    for(unsigned int i = 0; i < THREADS_MAX; i++)
        if((FuriThreadId)threads[i]->task_handle == thread_id) {
            // pthread_suspend_np(threads[i]->task_handle);
            // TODO: furi log
            threads[i]->suspended = true;
        }
}

void furi_thread_resume(FuriThreadId thread_id) {
    for(unsigned int i = 0; i < THREADS_MAX; i++)
        if((FuriThreadId)threads[i]->task_handle == thread_id) {
            // pthread_unsuspend_np(threads[i]->task_handle);
            // TODO: furi log
            threads[i]->suspended = false;
        }
}

bool furi_thread_is_suspended(FuriThreadId thread_id) {
    for(unsigned int i = 0; i < THREADS_MAX; i++)
        if((FuriThreadId)threads[i]->task_handle == thread_id)
            return threads[i]->suspended;
    return false;
}