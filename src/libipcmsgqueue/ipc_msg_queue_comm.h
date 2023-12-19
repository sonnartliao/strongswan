#ifndef _IPC_MSG_QUEUE_COMMON_H_
#define _IPC_MSG_QUEUE_COMMON_H_

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ZERO_ARRAY(array) memset(array, 0, sizeof(array))
#define ZERO_BUFF(buff, len) memset(buff, 0, len)

#define println(...)         \
    do                       \
    {                        \
        printf(__VA_ARGS__); \
        printf("\n");        \
    } while (0)

#define IPC_CHECK_PTR_RV(ptr, retvalue) \
    do                                  \
    {                                   \
        if (ptr == NULL)                \
            return retvalue;            \
    } while (0)

#define IPC_CHECK_PTR_RN(ptr) \
    do                        \
    {                         \
        if (ptr == NULL)      \
            return;           \
    } while (0)

#define IPC_CHECK_EXPR_RV(expr, retvalue) \
    do                                    \
    {                                     \
        if (!(expr))                      \
            return retvalue;              \
    } while (0)

#define IPC_CHECK_EXPR_RN(expr, retvalue) \
    do                                    \
    {                                     \
        if (!(expr))                      \
            return;                       \
    } while (0)

#endif