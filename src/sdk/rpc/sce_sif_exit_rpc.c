#include "types.h"
#include "rnc/runtime/core_state.h"

extern void exit_rpc_command(void) __asm__("sceSifExitCmd");

void sceSifExitRpc(void) __asm__("InitializeRpcCommand");

void sceSifExitRpc(void) {
    exit_rpc_command();
    RpcCommandState = 0;
}
