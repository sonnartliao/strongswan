#ifndef _IPC_MSG_QUEU_H_
#define _IPC_MSG_QUEU_H_

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/sysinfo.h>
#include <sys/msg.h>
#include <string.h>
#include <stdint.h>

#include "ipc_file_util.h"
#include "ipc_msg_queue_comm.h"
#include "ipc_msg_queue_sub_bbuguard.h"
#include "ipc_msg_queue_sub_cpcellapp.h"
#include "ipc_msg_queue_sub_cpgnbapp.h"
#include "ipc_msg_queue_sub_cpupproxy.h"
#include "ipc_msg_queue_sub_cpupproxy.h"
#include "ipc_msg_queue_sub_duapp.h"
#include "ipc_msg_queue_sub_fho.h"
#include "ipc_msg_queue_sub_netconf.h"
#include "ipc_msg_queue_sub_oam.h"
#include "ipc_msg_queue_sub_ru.h"
#include "ipc_msg_queue_sub_strongswanikeapp.h"

#define SELF_THREAD_ID syscall(SYS_gettid)
#define CPU_CORES get_nprocs_conf()

#define MAX_QUEUE_BUFFER_LEN 8192
#define RET_MSG_QUEUE_FAILED -1
#define SIZE_LONG_INT sizeof(long)
#define MAX_RETRY_COUNT 5

#define IS_MSG_QUEUE_READY(msgQueID) (msgQueID != RET_MSG_QUEUE_FAILED)
#define IS_MSG_QUEUE_RET_FAILED(retValue) (retValue == RET_MSG_QUEUE_FAILED)
#define IS_MSG_QUEUE_RET_SUCCESS(retValue) (retValue != RET_MSG_QUEUE_FAILED)

#define CHECK_MSG_QUEUE_READY_RN(msgQueID)           \
    do                                               \
    {                                                \
        if (IS_MSG_QUEUE_RET_FAILED(msgQueID))       \
        {                                            \
            println("message queue is not created"); \
            return;                                  \
        }                                            \
    } while (0)

#define CHECK_MSG_QUEUE_READY_RV(msgQueID, returnValue) \
    do                                                  \
    {                                                   \
        if (IS_MSG_QUEUE_RET_FAILED(msgQueID))          \
        {                                               \
            println("message queue is not created");    \
            return returnValue;                         \
        }                                               \
    } while (0)

#define CHECK_MSG_QUEUE_MODULEID_RV(moduleId, returnValue)                    \
    do                                                                        \
    {                                                                         \
        if (moduleId <= MODULE_MIN_INVALID || moduleId >= MODULE_MAX_INVLAID) \
        {                                                                     \
            println("module id %d is not invalid", moduleId);                 \
            return returnValue;                                               \
        }                                                                     \
    } while (0)

#define CHECK_MSG_QUEUE_MODULEID_RN(moduleId)                                 \
    do                                                                        \
    {                                                                         \
        if (moduleId <= MODULE_MIN_INVALID || moduleId >= MODULE_MAX_INVLAID) \
        {                                                                     \
            println("module id %d is not invalid", moduleId);                 \
            return;                                                           \
        }                                                                     \
    } while (0)
/******************************************************************************************************
 *       queue messages definition
 *******************************************************************************************************/
typedef struct QueueInfo
{
    int nQueueId;
    key_t key;
    ModuleID moduleId;
    const char *pQeueName;
    const char *pQeueFile;
} QueueInfo;

typedef void (*QueueMessageProcessor)(QueueInfo *pQueueInfo, QueueMessageHeader *pHeader, uint32_t nTotalLen);

#ifdef __cplusplus
extern "C"
{
#endif

    bool msgqueue_initialize(ModuleID eModuleId, uint32_t nInstID, uint32_t nRunningCoreId, QueueMessageProcessor fnHandler);
    bool msgqueue_send(ModuleID targetModule, QueueMessageHeader *pQueueMsg, uint32_t nTotalLen);
    const char *msgqueue_fetch_name(ModuleID eModuleId);
    const char *msgqueue_fetch_owner_name();
    ModuleID msgqueue_fetch_owner_module_id();

    void msgqueue_bind_thread_core(uint8_t nCoreId);

#ifdef __cplusplus
}
#endif

#endif
