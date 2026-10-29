#include "types.h"

s32 handle_sif_initialize_command(s32 *initialize_packet,
                                  s32 *command_state) __asm__("FUN_0011a448");

s32 handle_sif_initialize_command(s32 *initialize_packet, s32 *command_state) {
    s32 remote_receive_address;

    remote_receive_address = *(s32 *)((u8 *)initialize_packet + 0x10);
    *(s32 *)((u8 *)command_state + 0x8) = remote_receive_address;
    return remote_receive_address;
}

extern __typeof__(handle_sif_initialize_command) func_0011A448
    __attribute__((alias("FUN_0011a448")));
