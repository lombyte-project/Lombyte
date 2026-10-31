#include "types.h"
#include "rnc/runtime/core_state.h"
#include "rnc/sdk/rpc/cmd/sce_sif_exit_cmd.h"

void sceSifExitRpc(void) __asm__("InitializeRpcCommand");

void sceSifExitRpc(void) {
    sceSifExitCmd();
    RpcCommandState = 0;
}
