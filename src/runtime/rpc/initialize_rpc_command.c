#include "types.h"
#include "rnc/runtime/core_state.h"

extern void ExitRpcCommand(void) __asm__("sceSifExitCmd");

void ResetRpcCommandState(void) __asm__("InitializeRpcCommand");

void ResetRpcCommandState(void) {
    ExitRpcCommand();
    RpcCommandState = 0;
}
