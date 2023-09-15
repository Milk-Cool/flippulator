#pragma once
#include <stdbool.h>

void furi_record_init();
bool furi_record_status();

void furi_record_create(const char* name, void* data);
void* furi_record_open(const char* name);
void furi_record_close(const char* name);