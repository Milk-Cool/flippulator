#include "../filesystem_api_internal.h"
#include "storage_int.h"
#include <furi_hal.h>
#include "sd_notify.h"
#include <sys/stat.h>
#include <dirent.h>

#define TAG "StorageExt"

static struct stat st;

void storage_int_tick_internal(StorageData* storage, bool notify) {
    UNUSED(storage);
    UNUSED(notify);
}

void storage_int_tick(StorageData* storage) {
    storage_int_tick_internal(storage, true);
}

/******************* File Functions *******************/

static bool storage_int_file_open(
    void* ctx,
    File* file,
    const char* path,
    FS_AccessMode access_mode,
    FS_OpenMode open_mode) {
    UNUSED(ctx);

    FuriString* mode_str = furi_string_alloc_set_str("b");
    FuriString* path_str = furi_string_alloc_set_str("flippulator_sd/int");
    if(stat(furi_string_get_cstr(path_str), &st) == -1)
        mkdir(furi_string_get_cstr(path_str), 0700);

    if(access_mode & FSAM_READ) furi_string_cat(mode_str, "r");
    if(access_mode & FSAM_WRITE) furi_string_cat(mode_str, "w");
    if(open_mode & FSOM_OPEN_EXISTING) furi_string_cat(mode_str, "x");
    if(open_mode & FSOM_OPEN_ALWAYS);
    if(open_mode & FSOM_OPEN_APPEND) furi_string_cat(mode_str, "a");
    if(open_mode & FSOM_CREATE_NEW) crash(CRASH_STORAGE_FILE_UNSUPPORTED, CRASHTEXT_STORAGE_FILE_UNSUPPORTED);
    if(open_mode & FSOM_CREATE_ALWAYS);

    furi_string_cat(path_str, path);

    file->file = fopen(furi_string_get_cstr(path_str), furi_string_get_cstr(mode_str));
    file->type = FileTypeOpenFile;

    furi_string_free(mode_str);
    furi_string_free(path_str);

    return true;
}

static bool storage_int_file_close(void* ctx, File* file) {
    UNUSED(ctx);

    fclose(file->file);
    file->type = FileTypeClosed;

    return true;
}

static uint16_t
    storage_int_file_read(void* ctx, File* file, void* buff, uint16_t const bytes_to_read) {
    UNUSED(ctx);
    return (uint16_t)fread(file->file, 1, bytes_to_read, buff);
}

static uint16_t
    storage_int_file_write(void* ctx, File* file, const void* buff, uint16_t const bytes_to_write) {
    UNUSED(ctx);
    return (uint16_t)fwrite(buff, 1, bytes_to_write, file->file);
}

static bool
    storage_int_file_seek(void* ctx, File* file, const uint32_t offset, const bool from_start) {
    UNUSED(ctx);
    fseek(file->file, offset, from_start ? SEEK_SET : SEEK_CUR);
    return true;
}

static uint64_t storage_int_file_tell(void* ctx, File* file) {
    UNUSED(ctx);
    return ftell(file->file);
}

static bool storage_int_file_truncate(void* ctx, File* file) {
    UNUSED(ctx);
    crash(CRASH_UNSUPPORTED_FS_OPERATION, CRASHTEXT_UNSUPPORTED_FS_OPERATION);
    return false;
}

static bool storage_int_file_sync(void* ctx, File* file) {
    UNUSED(ctx);
    crash(CRASH_UNSUPPORTED_FS_OPERATION, CRASHTEXT_UNSUPPORTED_FS_OPERATION);
    return false;
}

static uint64_t storage_int_file_size(void* ctx, File* file) {
    UNUSED(ctx);
    size_t prev = ftell(file->file);
    fseek(file->file, 0, SEEK_END); // seek to end of file
    uint64_t size = ftell(file->file); // get current file pointer
    fseek(file->file, prev, SEEK_SET); // seek back to beginning of file
    return size;
}

static bool storage_int_file_eof(void* ctx, File* file) {
    return storage_int_file_tell(ctx, file) == storage_int_file_size(ctx, file);
}

/******************* Dir Functions *******************/

static bool storage_int_dir_open(void* ctx, File* file, const char* path) {
    UNUSED(ctx);
    FuriString* path_str = furi_string_alloc_set_str("flippulator_sd/int");
    if(stat(furi_string_get_cstr(path_str), &st) == -1)
        mkdir(furi_string_get_cstr(path_str), 0700);

    furi_string_cat(path_str, path);

    file->dir = opendir(furi_string_get_cstr(path_str));
    file->type = FileTypeOpenDir;

    furi_string_free(path_str);

    return true;
}

static bool storage_int_dir_close(void* ctx, File* file) {
    UNUSED(ctx);

    closedir(file->dir);

    return true;
}

static bool storage_int_dir_read(
    void* ctx,
    File* file,
    FileInfo* fileinfo,
    char* name,
    const uint16_t name_length) {
    UNUSED(ctx);
    
    struct dirent* direntry;
    if((direntry = readdir(file->dir)) == NULL)
        return false;
    
    if(fileinfo != NULL) {
        fileinfo->flags = direntry->d_type == 4 /* DT_DIR */ ? FSF_DIRECTORY : 0;
    }

    strncpy(name, direntry->d_name, name_length);

    return true;
}

static bool storage_int_dir_rewind(void* ctx, File* file) {
    UNUSED(ctx);
    rewinddir(file->dir);
    return true;
}
/******************* Common FS Functions *******************/

static FS_Error storage_int_common_stat(void* ctx, const char* path, FileInfo* fileinfo) {
    FuriString* path_str = furi_string_alloc_set_str("flippulator_sd/int");
    furi_string_cat(path_str, path);
    if(stat(furi_string_get_cstr(path_str), &st) == -1)
        return FSE_NOT_EXIST;
    stat(furi_string_get_cstr(path_str), &st);
    
    fileinfo->flags = S_ISREG(st.st_mode) ? 0 : FSF_DIRECTORY;
    fileinfo->size = st.st_size;

    free(path_str);
    return FSE_OK;
}

static FS_Error storage_int_common_remove(void* ctx, const char* path) {
    UNUSED(ctx);
    FuriString* path_str = furi_string_alloc_set_str("flippulator_sd/int");
    furi_string_cat(path_str, path);
    
    unlink(furi_string_get_cstr(path_str));

    free(path_str);
    return FSE_OK;
}

static FS_Error storage_int_common_mkdir(void* ctx, const char* path) {
    UNUSED(ctx);
    FuriString* path_str = furi_string_alloc_set_str("flippulator_sd/int");
    furi_string_cat(path_str, path);
    
    rmdir(furi_string_get_cstr(path_str));

    free(path_str);
    return FSE_OK;
}

static FS_Error storage_int_common_fs_info(
    void* ctx,
    const char* fs_path,
    uint64_t* total_space,
    uint64_t* free_space) {
    UNUSED(fs_path);
    UNUSED(ctx);
    (*total_space) = 999999;
    (*free_space) = 999999;
    return FSE_OK;
}

/******************* Init Storage *******************/
static const FS_Api fs_api = {
    .file =
        {
            .open = storage_int_file_open,
            .close = storage_int_file_close,
            .read = storage_int_file_read,
            .write = storage_int_file_write,
            .seek = storage_int_file_seek,
            .tell = storage_int_file_tell,
            .truncate = storage_int_file_truncate,
            .size = storage_int_file_size,
            .sync = storage_int_file_sync,
            .eof = storage_int_file_eof,
        },
    .dir =
        {
            .open = storage_int_dir_open,
            .close = storage_int_dir_close,
            .read = storage_int_dir_read,
            .rewind = storage_int_dir_rewind,
        },
    .common =
        {
            .stat = storage_int_common_stat,
            .mkdir = storage_int_common_mkdir,
            .remove = storage_int_common_remove,
            .fs_info = storage_int_common_fs_info,
        },
};

void storage_int_init(StorageData* storage) {
    storage->data = NULL;
    storage->api.tick = storage_int_tick;
    storage->fs_api = &fs_api;
}
