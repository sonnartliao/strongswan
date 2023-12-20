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

    IKE_SUB_MSG_IPSEC_KEYS_ADD_UPDATE_REQ,
    IKE_SUB_MSG_IPSEC_KEYS_ADD_UPDATE_RSP,

    IKE_SUB_MSG_IPSEC_KEYS_DEL_REQ,
    IKE_SUB_MSG_IPSEC_KEYS_DEL_RSP,

    IKE_SUB_MSG_Max_INVALID,
} StrongSwanIKESubMessageType;

typedef struct IKEv2SaSpi
{
    uint64_t u64IsakmpSpiInitiator;
    uint64_t u64IsakmpSpiResponder;
    uint64_t u64EspSpiInitiator;
    uint64_t u64EspSpiResponder;

    int8_t arrSourceTrafficSelector[MAX_32_BYTES];
    int8_t arrDestTrafficSelector[MAX_32_BYTES];
} IKEv2SaSpi;

typedef struct IKEv2SaAlgs
{
    uint16_t u16Alg;
    uint16_t u16Hash;
    uint16_t u16Resolved1;
    uint16_t u16Resolved2;
} IKEv2SaAlgs;

typedef struct IKEv2SaKeys
{
    uint8_t initiatorKey[ALG_KEY_LEN];
    uint8_t initiatorIV[ALG_IV_LEN];
    uint8_t ResponderKey[ALG_KEY_LEN];
    uint8_t ResponderIV[ALG_IV_LEN];
    uint8_t arrSKei[MAX_32_BYTES];
    uint8_t arrSKer[MAX_32_BYTES];
    uint8_t arrSKai[MAX_32_BYTES];
    uint8_t arrSKar[MAX_32_BYTES];
} IKEv2SaKeys;

typedef struct IPSecKeysAddUpdateReq
{
    IKEv2SaSpi spis;
    IKEv2SaAlgs algs;
    IKEv2SaKeys keys;
} IPSecKeysAddUpdateReq;

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

typedef IPSecKeysCommonRsp IPSecKeysAddUpdateRsp;
typedef IPSecKeysCommonRsp IPSecKeysDelRsp;

#define MAX_IPSEC_IKE_SPI_SIZE sizeof(IKEv2SaSpi)
#define MAX_IPSEC_IKE_KEYS_SIZE sizeof(IKEv2SaKeys)
#define MAX_IPSEC_IKE_ALGS_SIZE sizeof(IKEv2SaAlgs)

#define MAX_IPSEC_IKE_ADD_UPDATE_REQ_SIZE (sizeof(IPSecKeysAddUpdateReq))
#define MAX_IPSEC_IKE_ADD_UPDATE_RSP_SIZE (sizeof(IPSecKeysAddUpdateReq))

#define MAX_IPSEC_IKE_DEL_REQ_SIZE (sizeof(IPSecKeysDelReq))
#define MAX_IPSEC_IKE_DEL_RSP_SIZE (sizeof(IPSecKeysDelRsp))

#endif