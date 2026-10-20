#include "types.h"

typedef struct PadPort {
    u8 pad[0xC];
    u8 *unkC;
    u8 pad2[816 - 0x10];
} PadPort;

typedef struct DmaState {
    s32 last0;
    s32 last1;
} DmaState;

extern PadPort D_0015B540[];

__attribute__((section(".data"))) extern DmaState D_00132DD8;

s32 scePad2CheckDma(s32 socket_index) {
    u32 sides[2];
    u32 dma_buffer_base;
    s32 value;

    dma_buffer_base = (u32)D_0015B540[socket_index].unkC;
    value = *(s32 *)(dma_buffer_base + 0x7C);
    sides[0] = dma_buffer_base;
    sides[1] = dma_buffer_base + 0x80;
    if (value == 0) {
        return 0;
    }
    if (D_00132DD8.last0 == value && D_00132DD8.last1 == *(s32 *)(sides[1] + 0x7C)) {
        return 0;
    }
    D_00132DD8.last0 = *(s32 *)(sides[0] + 0x7C);
    D_00132DD8.last1 = *(s32 *)(sides[1] + 0x7C);
    return 1;
}
