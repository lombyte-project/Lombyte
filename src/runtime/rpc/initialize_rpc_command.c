#include "types.h"
#include "rnc/runtime/core_state.h"

extern void exit_rpc_command(void) __asm__("sceSifExitCmd");

void reset_rpc_command_state(void) __asm__("InitializeRpcCommand");

void reset_rpc_command_state(void) {
    exit_rpc_command();
    RpcCommandState = 0;
}
