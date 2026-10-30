#include "types.h"

/* System command 1 (sceSifInitCmd): stores the packet's value (+0x14) into
   the SIF register table (sif_cmd_data.sregs, +0x1C) at its index (+0x10). */
void handle_sif_set_system_register(s32 *packet, s32 *command_state) __asm__("FUN_0011a428");

void handle_sif_set_system_register(s32 *packet, s32 *command_state) {
    s32 *system_registers;
    s32  index;
    s32  value;

    index = *(s32 *)((u8 *)packet + 0x10);
    system_registers = *(s32 **)((u8 *)command_state + 0x1C);
    value = *(s32 *)((u8 *)packet + 0x14);
    system_registers[index] = value;
}

extern __typeof__(handle_sif_set_system_register) func_0011A428
    __attribute__((alias("FUN_0011a428")));
