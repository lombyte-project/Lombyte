#include "types.h"
#include "sda.h"

extern s32 *D_0015EF64;
extern u8 *D_0015EF60;
extern s32 D_001996FC NOT_SDA;
extern u8 *D_0015F6A0 __attribute__((sda));

void QueueDmaTransfer(s32 index) {
    u8 *entry;

    entry = D_0015EF60 + D_0015EF64[index];
    D_001996FC = *(s32 *)entry;
    entry += 8;
    D_0015F6A0 = entry;
}
