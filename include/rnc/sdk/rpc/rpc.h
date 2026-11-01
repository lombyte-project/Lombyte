#ifndef LOMBYTE_RNC_SDK_RPC_RPC_H
#define LOMBYTE_RNC_SDK_RPC_RPC_H

#include "types.h"

struct EndPkt;
struct sceSifClientData;

s32  free_rpc_packet() __asm__("func_0011AD90");
s32  _lf_bind(void);
void QueuePeekReadDone();
void _request_bind();
void _request_end(struct EndPkt *, void *);
void _request_rdata();
s32  sceSifBindRpc(struct sceSifClientData *, s32, s32);
s32  sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, void *, void *);
s32  SceSifCheckStatRpc(void *);
void sceSifExitRpc(void) __asm__("InitializeRpcCommand");
void sceSifInitRpc(u32);

#endif /* LOMBYTE_RNC_SDK_RPC_RPC_H */
