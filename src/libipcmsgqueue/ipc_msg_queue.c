#include <sched.h>
#include <syscall.h>
#include "ipc_msg_queue.h"
#include "ipc_file_util.h"

// message of queue
typedef struct QueueMessage
{
    long carrierType;
    uint8_t content[MAX_QUEUE_BUFFER_LEN];
} QueueMessage;

typedef struct QueueContext
{
    bool isInitialized;
    bool bRunning;
    int nBindCoreID;
    uint32_t nLightId;
    pthread_t tid;
    ModuleID ownerModuleId;
    QueueMessageProcessor fnAnyMsgHandle;
    QueueMessageProcessor fnControllerMsgHandle;
    QueueMessageProcessor fnDataMsgHandle;
    QueueInfo arrQueueInfo[MODULE_MAX_INVLAID];
} QueueContext;

#define FETCH_QUEUE_ID(moduleId) gContext.arrQueueInfo[moduleId].nQueueId
#define FETCH_QUEUE_NAME(moduleId) gContext.arrQueueInfo[moduleId].pQeueName

typedef void *(*ThreadRoutine)(void *param);
void *thread_main_routine(void *param);

/***************************************************************************************************************
 *   globle variables and function definition
 **************************************************************************************************************/
QueueContext gContext = {
    .isInitialized = false,
    .bRunning = true,
    .nBindCoreID = 0,
    .nLightId = 0,
    .tid = 0,
    .fnAnyMsgHandle = NULL,
    .fnControllerMsgHandle = NULL,
    .fnDataMsgHandle = NULL,
    .ownerModuleId = 0,
    .arrQueueInfo = {
        {
            .moduleId = MODULE_MIN_INVALID,
            .pQeueName = "mininvalid",
            .pQeueFile = "/tmp/msgqueue/mininvalid",
        },
        {
            .moduleId = MODULE_OAM,
            .pQeueName = "oam",
            .pQeueFile = "/tmp/msgqueue/oam",
        },
        {
            .moduleId = MODULE_NETCONF_AGENT,
            .pQeueName = "netconf",
            .pQeueFile = "/tmp/msgqueue/netconfagent",
        },
        {
            .moduleId = MODULE_CPCELL_APP,
            .pQeueName = "cpcellapp",
            .pQeueFile = "/tmp/msgqueue/cpcellapp",
        },
        {
            .moduleId = MODULE_CPGNB_APP,
            .pQeueName = "cpgnbapp",
            .pQeueFile = "/tmp/msgqueue/cpgnbapp",
        },
        {
            .moduleId = MODULE_CPUP_PROXY,
            .pQeueName = "cpcuproxy",
            .pQeueFile = "/tmp/msgqueue/cpupproxy",
        },
        {
            .moduleId = MODULE_DU_APP,
            .pQeueName = "duapp",
            .pQeueFile = "/tmp/msgqueue/duapp",
        },
        {
            .moduleId = MODULE_FHO_AGENT,
            .pQeueName = "fho",
            .pQeueFile = "/tmp/msgqueue/fho",
        },
        {
            .moduleId = MODULE_RU_AGENT,
            .pQeueName = "ruagent",
            .pQeueFile = "/tmp/msgqueue/ruagent",
        },
        {
            .moduleId = MODULE_BBU_GUARD,
            .pQeueName = "bbuguard",
            .pQeueFile = "/tmp/msgqueue/bbuguard",
        },
        {
            .moduleId = MODULE_STRONGSWAN_IKE_APP,
            .pQeueName = "strongswanikeapp",
            .pQeueFile = "/tmp/msgqueue/strongswanikeapp",
        },
    },
};

key_t generate_key(const char *pQueueFile)
{
    if (pQueueFile == NULL || strlen(pQueueFile) == 0)
    {
        perror("queue file name is invalid or empty");
        return 0;
    }

    key_t key = ftok(pQueueFile, 1);
    if (key == -1)
    {
        perror("ftok failed");
    }
    return key;
}

static bool create_thread(ThreadRoutine fnRoutine, void *param, pthread_t *pTid)
{
    if (pthread_create(pTid, NULL, fnRoutine, param) < 0)
    {
        println("create thread failed");
        return false;
    }
    return true;
}

static void bind_thread_core(uint8_t nCoreId)
{
    // cpu_set_t mask;
    // cpu_set_t get;

    // CPU_ZERO(&mask);
    // CPU_SET(nCoreId, &mask);
    // if (sched_setaffinity(0, sizeof(mask), &mask) == -1)
    // {
    //     println("warning: could not set CPU affinity, continuing...\n");
    // }

    // CPU_ZERO(&get);
    // if (sched_getaffinity(0, sizeof(get), &get) == -1)
    // {
    //     println("warning: cound not get thread affinity, continuing...\n");
    // }
}
bool msgqueue_initialize(ModuleID eModuleId, uint32_t nInstID, uint32_t nRunningCoreId, QueueMessageProcessor fnHandler)
{
    CHECK_MSG_QUEUE_MODULEID_RV(eModuleId, false);
    ModuleID modeId;
    if (gContext.isInitialized)
    {
        println("message queue for has already been created");
        return true;
    }
    for (modeId = MODULE_OAM; modeId < MODULE_MAX_INVLAID; modeId++)
    {
        QueueInfo *pQueuInfo = &gContext.arrQueueInfo[modeId];
        if (!create_file_if_absent(pQueuInfo->pQeueFile))
        {
            return false;
        }

        pQueuInfo->key = generate_key(pQueuInfo->pQeueFile);
        pQueuInfo->nQueueId = msgget(pQueuInfo->key, IPC_CREAT | 0666);
        CHECK_MSG_QUEUE_READY_RV(pQueuInfo->nQueueId, false);
        println("create message queue key 0x%x msgqid %d for module %d %s success",
                pQueuInfo->key, pQueuInfo->nQueueId, pQueuInfo->moduleId, pQueuInfo->pQeueName);
    }

    gContext.ownerModuleId = eModuleId;
    gContext.fnAnyMsgHandle = fnHandler;
    gContext.fnControllerMsgHandle = fnHandler;
    gContext.fnDataMsgHandle = fnHandler;
    gContext.nLightId = syscall(SYS_gettid);
    gContext.isInitialized = true;

    create_thread(thread_main_routine, &gContext.arrQueueInfo[eModuleId], &gContext.tid);

    return true;
}

bool msgqueue_send(ModuleID targetModule, QueueMessageHeader *pQueueMsg, uint32_t nMsgTotalLen)
{
    CHECK_MSG_QUEUE_MODULEID_RV(targetModule, false);
    IPC_CHECK_EXPR_RV(targetModule > MODULE_MIN_INVALID && targetModule < MODULE_MAX_INVLAID, false);
    IPC_CHECK_EXPR_RV(gContext.ownerModuleId > MODULE_MIN_INVALID && gContext.ownerModuleId < MODULE_MAX_INVLAID, false);

    QueueInfo *pTargetQueue;
    pTargetQueue = &gContext.arrQueueInfo[targetModule];
    CHECK_MSG_QUEUE_READY_RV(pTargetQueue->moduleId, false);

    QueueMessage msg;

    pQueueMsg->eSourceModule = gContext.ownerModuleId;
    pQueueMsg->eDestModule = targetModule;

    msg.carrierType = CARRIER_ANY;
    memset(msg.content, 0, sizeof(msg.content));
    memcpy(msg.content, pQueueMsg, nMsgTotalLen);

    uint32_t nSendCount = 0;
    int nResult = RET_MSG_QUEUE_FAILED;

    do
    {
        nResult = msgsnd(pTargetQueue->nQueueId, &msg, SIZE_LONG_INT + nMsgTotalLen, IPC_NOWAIT);
        if (IS_MSG_QUEUE_RET_SUCCESS(nResult))
        {
            println("send to module %s success", pTargetQueue->pQeueName);
            return true;
        }
        nSendCount++;
        perror("send error:");
        usleep(100000);
    } while (nSendCount < MAX_RETRY_COUNT);

    println("send message to %s success", pTargetQueue->pQeueName);

    return false;
}

void *thread_main_routine(void *param)
{
    QueueInfo *pQueueInfo = (QueueInfo *)param;
    QueueMessage msg;

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    gContext.nLightId = syscall(SYS_gettid);

    println("%s message queue receiver running,selfid=%u", pQueueInfo->pQeueName, gContext.nLightId);

    while (gContext.bRunning)
    {
        memset(&msg, 0, sizeof(msg));
        ssize_t nRecv = msgrcv(pQueueInfo->nQueueId, &msg, sizeof(msg.content), 0, 0);
        if (nRecv > 0)
        {
            if (msg.carrierType == CARRIER_ANY && gContext.fnAnyMsgHandle)
            {
                gContext.fnAnyMsgHandle(pQueueInfo, (QueueMessageHeader *)msg.content, nRecv - SIZE_LONG_INT);
            }
            else if (msg.carrierType == CARRIER_CONTROL && gContext.fnControllerMsgHandle)
            {
                gContext.fnControllerMsgHandle(pQueueInfo, (QueueMessageHeader *)msg.content, nRecv - SIZE_LONG_INT);
            }
            else if (msg.carrierType == CARRIER_DATA && gContext.fnDataMsgHandle)
            {
                gContext.fnDataMsgHandle(pQueueInfo, (QueueMessageHeader *)msg.content, nRecv - SIZE_LONG_INT);
            }
            else
            {
                perror("there is no any message handle");
            }
            continue;
        }
        perror("receive error");
    }

    return NULL;
}

const char *msgqueue_fetch_name(ModuleID eModuleId)
{
    CHECK_MSG_QUEUE_MODULEID_RV(eModuleId, "invalid module id");
    return FETCH_QUEUE_NAME(eModuleId);
}

const char *msgqueue_fetch_owner_name()
{
    return FETCH_QUEUE_NAME(gContext.ownerModuleId);
}

ModuleID msgqueue_fetch_owner_module_id()
{
    return gContext.ownerModuleId;
}
