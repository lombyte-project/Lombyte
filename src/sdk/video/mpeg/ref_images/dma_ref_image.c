#include "types.h"
#include "eetypes.h"
#include "kernel.h"

/* One of the two image buffers of the Sony libmpeg decoder state. */
typedef struct MpegRefImageBuffer {
    u32 spr_address;
    u32 pad4;
    u32 first_source[4];
    u32 second_source[4];
    u8  pad28[0x12C - 0x28];
    s32 count;
    u8  pad130[0x140 - 0x130];
} MpegRefImageBuffer;

typedef struct MpegRefImageState {
    u8                 pad0[0x590];
    MpegRefImageBuffer buffers[2];
    s32                index;
} MpegRefImageState;

extern s32  DIntr(void);
extern s32  EnableInterrupts(void);
extern u128 mpeg_ref_image_chain[] __asm__("D_0015E840");

#define D9_CHCR (*(volatile u32 *)0x1000D400)
#define D9_QWC  (*(volatile u32 *)0x1000D420)
#define D9_TADR (*(volatile u32 *)0x1000D430)
#define D9_SADR (*(volatile u32 *)0x1000D480)

/* Sony libmpeg: builds a chain of ref tags for the current buffer in
   uncached memory and starts the to-scratchpad DMA channel on it. */
void dmaRefImage(MpegRefImageState *state) {
    s32  i;
    u64 *tag;
    s32  count;
    s32  interrupts;
    s32  id;
    u32  address;
    u64  first_tag;

    i = 0;
    tag = (u64 *)(((u32)mpeg_ref_image_chain & 0x0fffffff) | 0x20000000);
    count = state->buffers[state->index].count;
    if (count > 0) {
        do {
            first_tag = ((u64)(state->buffers[state->index].first_source[i] & 0x0fffffff) << 32) |
                        0x30000030;
            address = state->buffers[state->index].second_source[i];
            id = 3;
            if (i == count - 1) {
                id = 0;
            }
            tag[0] = first_tag;
            tag[2] = ((u64)(address & 0x0fffffff) << 32) | ((u64)id << 28) | 0x30;
            tag += 4;
            i++;
        } while (i < count);
    }
    interrupts = DIntr();
    CpuSync();
    D9_SADR = state->buffers[state->index].spr_address;
    D9_TADR = (u32)mpeg_ref_image_chain;
    D9_QWC = 0;
    D9_CHCR = 0x105;
    if (interrupts) {
        EnableInterrupts();
    }
}
