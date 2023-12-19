#ifndef _IPC_MSG_QUEUE_MSG_DEF_H_
#define _IPC_MSG_QUEUE_MSG_DEF_H_

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "ipc_msg_queue_comm.h"

/******************************************************************************************************
 *       service messages definition
 *******************************************************************************************************/
typedef enum EQueueErrCode
{
    QUEUE_CODE_SUCCESS,
    QUEUE_CODE_FAILD,

    QUEUE_CODE_SA_NOT_EXIST

} EQueueErrCode;

typedef enum ModuleID
{
    MODULE_MIN_INVALID,

    MODULE_OAM,
    MODULE_NETCONF_AGENT,
    MODULE_CPCELL_APP,
    MODULE_CPGNB_APP,
    MODULE_CPUP_PROXY,
    MODULE_DU_APP,

    MODULE_FHO_AGENT,
    MODULE_RU_AGENT,
    MODULE_BBU_GUARD,

    MODULE_STRONGSWAN_IKE_APP,

    MODULE_MAX_INVLAID
} ModuleID;

typedef enum MessageType
{
    MESSAGE_MIN_INVLAID,

    MESSAGE_OAM,
    MESSAGE_NETCONF_AGENT,
    MESSAGE_CPCELL_APP,
    MESSAGE_CPGNB_APP,
    MESSAGE_CPUP_PROXY,
    MESSAGE_DU_APP,

    MESSAGE_FHO_AGENT,
    MESSAGE_RU_AGENT,
    MESSAGE_BBU_GUARD,

    MESSAGE_STRONGSWAN_IKE_APP,

    // broadcast message
    MESSAGE_BROADCAST = 1000,

    MESSAGE_MAX_INVLAID
} MessageType;

typedef struct QueueMessageHeader
{
    MessageType eMsgType;
    uint32_t eSubMsgType;
    ModuleID eSourceModule;
    ModuleID eDestModule;
    uint32_t timestamp;
    uint32_t nPayloadLen;
    uint8_t payload[0];
} QueueMessageHeader;

#define QUEUE_MSG_HDR_LEN sizeof(QueueMessageHeader)

#define QUEUE_MSG_MALLOC(pMsgHdr, payloadLen)                                   \
    do                                                                          \
    {                                                                           \
        pMsgHdr = (QueueMessageHeader *)malloc(QUEUE_MSG_HDR_LEN + payloadLen); \
        memset(pMsgHdr, 0, QUEUE_MSG_HDR_LEN + payloadLen);                     \
        pMsgHdr->nPayloadLen = payloadLen;                                      \
    } while (0)

#define QUEUE_MSG_FREE(pMsgHdr) \
    do                          \
    {                           \
        if (pMsgHdr)            \
        {                       \
            free(pMsgHdr);      \
            pMsgHdr = NULL;     \
        }                       \
    } while (0)

#define QUEUE_MSG_LEN(pMsgHdr) (pMsgHdr->nPayloadLen + QUEUE_MSG_HDR_LEN)

#endif
