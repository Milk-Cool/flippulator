#include "ptr_dict.h"

PtrDictEl* ptr_dict_el_alloc() {
    return (PtrDictEl*) malloc(sizeof(PtrDictEl));
}
PtrDict* ptr_dict_alloc() {
    PtrDict* dict = malloc(sizeof(PtrDict));
    dict->first = NULL;
    return dict;
}

// TODO: check for duplicates
void ptr_dict_set(PtrDict* dict, const char* name, void* value) {
    PtrDictEl* last_el = dict->first;
    if(last_el != NULL)
        while(last_el->next != NULL)
            last_el = last_el->next;
    PtrDictEl* new = ptr_dict_el_alloc();
    new->name = strdup(name);
    new->value = value;
    new->next = NULL;
    if(last_el == NULL) dict->first = new;
    else last_el->next = new;
}

void* ptr_dict_get(PtrDict* dict, const char* name) {
    PtrDictEl* el = dict->first;
    if(el == NULL) return NULL;
    while(strcmp(el->name, name) != 0 && el->next != NULL)
        el = el->next;
    if(strcmp(el->name, name) != 0)
        return NULL;
    return el->value;
}