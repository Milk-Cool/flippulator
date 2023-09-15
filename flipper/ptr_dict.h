#pragma once

#include <stdlib.h>
#include <crash.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    const char* name;
    void* value;
    void* next;
} PtrDictEl;

typedef struct {
    PtrDictEl* first;
} PtrDict;

PtrDictEl* ptr_dict_el_alloc();
PtrDict* ptr_dict_alloc();

void ptr_dict_set(PtrDict* dict, const char* name, void* value);
void* ptr_dict_get(PtrDict* dict, const char* name);