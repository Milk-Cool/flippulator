#include "h_recursive_mkdir.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>

// https://stackoverflow.com/a/2336245/22470070
void mkdir_rec(const char *dir) {
    char tmp[256];
    char *p = NULL;
    size_t len;

    snprintf(tmp, sizeof(tmp),"%s",dir);
    len = strlen(tmp);
    if (tmp[len - 1] == '/')
        tmp[len - 1] = 0;
    for (p = tmp + 1; *p; p++)
        if (*p == '/') {
            *p = 0;
            mkdir(tmp, S_IRWXU);
            *p = '/';
        }
    mkdir(tmp, S_IRWXU);
}