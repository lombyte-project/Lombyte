#include "types.h"
#include "eetypes.h"

extern volatile u128 D_00133050[];
extern u128 D_001330A0[];
extern void SetD4ChcrVariant(s32);

#define IPU_CMD  ((volatile u32 *)0x10002000)
#define IPU_CTRL ((volatile s32 *)0x10002010)
#define IPU_IN_FIFO ((volatile u128 *)0x10007010)

void sceIpuInit(void)
{
    SetD4ChcrVariant(1);
    *IPU_CTRL = 0x40000000;
    while (*IPU_CTRL < 0) {
    }
    *IPU_CMD = 0;
    while (*IPU_CTRL < 0) {
    }
    *IPU_IN_FIFO = D_00133050[0];
    *IPU_IN_FIFO = D_00133050[1];
    *IPU_IN_FIFO = D_00133050[2];
    *IPU_IN_FIFO = D_00133050[3];
    *IPU_IN_FIFO = D_00133050[4];
    *IPU_IN_FIFO = D_00133050[4];
    *IPU_IN_FIFO = D_00133050[4];
    *IPU_IN_FIFO = D_00133050[4];
    *IPU_CMD = 0x50000000;
    while (*IPU_CTRL < 0) {
    }
    *IPU_CMD = 0x58000000;
    while (*IPU_CTRL < 0) {
    }
    *IPU_IN_FIFO = D_001330A0[0];
    *IPU_IN_FIFO = D_001330A0[1];
    *IPU_CMD = 0x60000000;
    while (*IPU_CTRL < 0) {
    }
    *IPU_CMD = 0x90000000;
    while (*IPU_CTRL < 0) {
    }
    *IPU_CTRL = 0x40000000;
    while (*IPU_CTRL < 0) {
    }
    *IPU_CMD = 0;
    while (*IPU_CTRL < 0) {
    }
}
