#include "types.h"
#include "kernel.h"

/* The DMA-busy bits of the VU1 chain (rendering/vu1_chain.c). */
extern volatile s32 vif_chain_flags __asm__("D_00160EE0") __attribute__((sda));

/* VIF1 DMA interrupt handler: clears the busy bit of the chain part whose tag
   ID raised the interrupt, all of them at the end of the chain, then resumes
   the channel. */
s32 vif1_dma_interrupt_handler(s32 cause) __asm__("func_00233E00");

s32 vif1_dma_interrupt_handler(s32 cause) {
    enum {
        VIF1_TAG_CLEAR_CHAIN_BUSY_BIT_1 = 0x02010000,
        VIF1_TAG_CLEAR_CHAIN_BUSY_BIT_2 = 0x02020000,
        VIF1_TAG_CLEAR_CHAIN_BUSY_BIT_3 = 0x02040000,
        VIF1_TAG_CLEAR_CHAIN_BUSY_BIT_4 = 0x02080000
    };
    u32 chcr = *(volatile u32 *)0x10009000;
    u32 tag = chcr & 0x03ff0000;

    if (*(volatile u32 *)0x10009030 != 0 && (chcr & 0xc)) {
        if ((chcr & 0x70000000) == 0x70000000) {
            vif_chain_flags &= ~0x1f;
        } else {
            if (tag & 0x02000000) {
                switch (tag) {
                case VIF1_TAG_CLEAR_CHAIN_BUSY_BIT_1:
                    vif_chain_flags &= ~0x2;
                    break;
                case VIF1_TAG_CLEAR_CHAIN_BUSY_BIT_2:
                    vif_chain_flags &= ~0x4;
                    break;
                case VIF1_TAG_CLEAR_CHAIN_BUSY_BIT_3:
                    vif_chain_flags &= ~0x8;
                    break;
                case VIF1_TAG_CLEAR_CHAIN_BUSY_BIT_4:
                    vif_chain_flags &= ~0x10;
                    break;
                }
            }
            *(volatile u32 *)0x10009000 = chcr | 0x100;
        }
    }
    CpuSync();
    CpuEnableInt();
    return 0;
}
