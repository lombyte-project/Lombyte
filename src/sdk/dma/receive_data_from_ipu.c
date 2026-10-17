#include "types.h"
extern s32 DIntr();
extern void EnableInterrupts();
void receiveDataFromIPU(s32 destination_address, s32 byte_count) {
    s32 old;

    old = DIntr();
    *(volatile u32 *)0x1000B010 = (destination_address & 0x0FFFFFFF) | 0x80000000;
    *(volatile u32 *)0x1000B020 = byte_count >> 4;
    *(volatile u32 *)0x1000B000 = 0x100;
    if (old != 0) {
        EnableInterrupts(0x100);
    }
}
