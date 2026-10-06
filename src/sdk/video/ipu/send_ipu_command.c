#include "types.h"

typedef struct IpuCommandState {
    u8 reserved[0x818];
    u32 command_result;
} IpuCommandState;

extern u32 IpuCommandTable[16] __asm__("D_00132E70") __attribute__((section(".data")));

u32 SendIpuCommand(IpuCommandState *state, u32 command) __asm__("_sendIpuCommand");

u32 SendIpuCommand(IpuCommandState *state, u32 command) {
    volatile u32 *ipu_command = (volatile u32 *)0x10002000;
    u32 result;

    *ipu_command = command;
    result = IpuCommandTable[command >> 28];
    state->command_result = result;
    return result;
}
