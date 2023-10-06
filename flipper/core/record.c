#include "record.h"
#include <ptr_dict.h>

PtrDict* records;
bool initialized = false;

void furi_record_init() {
    records = ptr_dict_alloc();
    initialized = true;
}
bool furi_record_status() {
    return initialized;
}

void furi_record_create(const char* name, void* data) {
    ptr_dict_set(records, name, data);
}

void* furi_record_open(const char* name) {
    return ptr_dict_get(records, name);
}

void furi_record_close(const char* name) {
    // TODO
}