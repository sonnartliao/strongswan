#ifndef _IPC_MSG_QUEUE_MSG_SUB_STRONGSWANIKEAPP_H_
#define _IPC_MSG_QUEUE_MSG_SUB_STRONGSWANIKEAPP_H_

#include "ipc_msg_queue_msg_def.h"

/**
 * Message definition for StrongSwan IKE app
 */

#define ALG_KEY_LEN 32
#define ALG_IV_LEN 32
#define SPI_LEN 8

#define MAX_32_BYTES 32

#define ALG_AES_KEY_SIZE_16 16
#define ALG_AES_KEY_SIZE_24 24
#define ALG_AES_KEY_SIZE_32 32

typedef enum StrongSwanIKESubMessageType
{
    IKE_SUB_MSG_MIN_INVALID,

    IKE_SUB_MSG_IPSEC_KEYS_ADD_REQ,
    IKE_SUB_MSG_IPSEC_KEYS_ADD_RSP,

    IKE_SUB_MSG_IPSEC_KEYS_UPDATE_REQ,
    IKE_SUB_MSG_IPSEC_KEYS_UPDATE_RSP,

    IKE_SUB_MSG_IPSEC_KEYS_DEL_REQ,
    IKE_SUB_MSG_IPSEC_KEYS_DEL_RSP,

    IKE_SUB_MSG_Max_INVALID,
} StrongSwanIKESubMessageType;

typedef struct IPSecKeysAddReq
{
    uint64_t u64IsakmpSpiInitiator;
    uint64_t u64IsakmpSpiResponder;
    uint64_t u64EspSpiInitiator;
    uint64_t u64EspSpiResponsor;
    uint16_t u16InitiatorAlg;
    uint16_t u16InitiatorHash;
    uint16_t u16ResponderAlg;
    uint16_t u16ResponderHash;
    uint8_t initiatorKey[ALG_KEY_LEN];
    uint8_t initiatorIV[ALG_IV_LEN];
    uint8_t ResponderKey[ALG_KEY_LEN];
    uint8_t ResponderIV[ALG_IV_LEN];
    uint8_t arrSKei[MAX_32_BYTES];
    uint8_t arrSKer[MAX_32_BYTES];
    uint8_t arrSKai[MAX_32_BYTES];
    uint8_t arrSKar[MAX_32_BYTES];
} IPSecKeysAddReq;

typedef IPSecKeysAddReq IPSecKeysUpdateReq;

typedef struct IPSecKeysDelReq
{
    uint64_t u64IsakmpSpiInitiator;
    uint64_t u64IsakmpSpiResponder;
    uint64_t u64EspSpiInitiator;
    uint64_t u64EspSpiResponsor;
} IPSecKeysDelReq;

typedef struct IPSecKeysCommonRsp
{
    EQueueErrCode eErrCode;
    uint64_t u64IsakmpSpiInitiator;
    uint64_t u64IsakmpSpiResponder;
    uint64_t u64EspSpiInitiator;
    uint64_t u64EspSpiResponsor;
} IPSecKeysCommonRsp;

typedef IPSecKeysCommonRsp IPSecKeysAddRsp;
typedef IPSecKeysCommonRsp IPSecKeysDelRsp;
typedef IPSecKeysCommonRsp IPSecKeysUpdateRsp;

#define IPSEC_KEYS_ADD_REQ_LEN (QUEUE_MSG_HDR_LEN + sizeof(IPSecKeysAddReq))
#define IPSEC_KEYS_ADD_RSP_LEN (QUEUE_MSG_HDR_LEN + sizeof(IPSecKeysAddRsp))

#define IPSEC_KEYS_DEL_REQ_LEN (QUEUE_MSG_HDR_LEN + sizeof(IPSecKeysDelReq))
#define IPSEC_KEYS_DEL_RSP_LEN (QUEUE_MSG_HDR_LEN + sizeof(IPSecKeysDelRsp))

#define IPSEC_KEYS_UPDATE_REQ_LEN (QUEUE_MSG_HDR_LEN + sizeof(IPSecKeysUpdateReq))
#define IPSEC_KEYS_UPDATE_RSP_LEN (QUEUE_MSG_HDR_LEN + sizeof(IPSecKeysUpdateRsp))

#endif