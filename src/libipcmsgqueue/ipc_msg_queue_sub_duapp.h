#ifndef _IPC_MSG_QUEUE_MSG_SUB_DUAPP_H_
#define _IPC_MSG_QUEUE_MSG_SUB_DUAPP_H_

#include "ipc_msg_queue_msg_def.h"
/**
 * Message definition for duapp
 */
typedef enum DuAppSubMessageType
{
    DUAPP_SUB_MSG_MIN_INVALID,
    DUAPP_SUB_MSG_IKE,
    DUAPP_SUB_MSG_MAX_INVALID
} DuAppSubMessageType;

#endif