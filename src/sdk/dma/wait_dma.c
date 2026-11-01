#include "types.h"
#include "rnc/sdk/deci2.h"
#include "rnc/sdk/dma.h"

const u8 D_001533E0[] = "libdma: sync timeout\n";

void WaitDma(volatile u32 *chcr) {
    s32 count;

    count = 0x1000000;
    while (*chcr & 0x100) {
        if (--count < 0) {
            scePrintf(D_001533E0);
            sceDmaPause(chcr);
        }
    }
}
