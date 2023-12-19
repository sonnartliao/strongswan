#ifndef _IPC_FILE_UTIL_H_
#define _IPC_FILE_UTIL_H_
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif
    bool create_file_if_absent(const char *pFullFileName);
#ifdef __cplusplus
}
#endif

#endif
