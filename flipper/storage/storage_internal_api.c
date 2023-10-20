#include <core/record.h>
#include "storage.h"

FS_Error storage_int_backup(Storage* api, const char* dstname) {
    return FSE_OK;
}

FS_Error storage_int_restore(Storage* api, const char* srcname, Storage_name_converter converter) {
    return FSE_OK;
}