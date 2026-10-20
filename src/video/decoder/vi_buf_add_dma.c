#include "types.h"

typedef struct {
    u64 lo;
    u64 hi;
} ViBufDmaTag;

/* Only the input-buffer fields used by this unit are described here. */
typedef struct {
    s64 *data;
    ViBufDmaTag *tag;
    s32 n;
    s32 dmaStart;
    s32 dmaN;
    s32 readBytes;
    u8 pad18[0x28];
    s32 semaphore;
    s32 isActive;
} ViBuf;

#define VIBUF_ELM_SIZE 2048
#define DMA_ID_REFE    0
#define DMA_ID_REF     3
#define D4_CHCR        ((volatile u32 *)0x1000B400)
#define D4_MADR        ((volatile u32 *)0x1000B410)

extern s32 WaitSema(s32 semaphore);
extern s32 SignalSema(s32 semaphore);
extern void err_message(s32 message) __asm__("FUN_0023ab78");
extern char D_001E8B20[];
extern void set_d4_chcr(s32 control) __asm__("FUN_0023bbb0");
extern u32 get_fifo_index(ViBuf *buffer, s32 dma_address) __asm__("FUN_0023baf8");
extern void sc_tag2(u64 *output, u32 high_word, u32 middle_word,
                           u32 low_word) __asm__("func_0023BC20");

s32 vi_buf_add_dma(ViBuf *f) __asm__("FUN_0023bf70");

/* Append complete MPEG input blocks and restart channel 4 when needed. */
s32 vi_buf_add_dma(ViBuf *buffer) {
    s32 i;
    s32 index;
    s32 id;
    s32 last;
    u32 d4chcr;
    u32 madr;
    s32 isNewData = 0;
    s32 consume;
    s32 read_start, read_n;

    WaitSema(buffer->semaphore);

    if (!buffer->isActive) {
        err_message((s32)D_001E8B20);
        return 0;
    }

    set_d4_chcr((DMA_ID_REFE << 28) | (0 << 8) | (1 << 2) | 1);
    d4chcr = *D4_CHCR;
    madr = *D4_MADR;

    index = get_fifo_index(buffer, (s32)madr);
    consume = (index + buffer->n - buffer->dmaStart) % buffer->n;
    buffer->dmaStart = (buffer->dmaStart + consume) % buffer->n;
    buffer->dmaN -= consume;

    read_start = (buffer->dmaStart + buffer->dmaN) % buffer->n;
    read_n = buffer->readBytes / VIBUF_ELM_SIZE;
    buffer->readBytes %= VIBUF_ELM_SIZE;

    if (read_n > 0) {
        last = (buffer->dmaStart + buffer->dmaN - 1 + buffer->n) % buffer->n;
        sc_tag2((u64 *)(buffer->tag + last), (u32)((u8 *)buffer->data + VIBUF_ELM_SIZE * last),
                       DMA_ID_REF, VIBUF_ELM_SIZE / 16);
        isNewData = 1;
    }

    index = read_start;
    for (i = 0; i < read_n; i++) {
        id = (i == read_n - 1) ? DMA_ID_REFE : DMA_ID_REF;
        sc_tag2((u64 *)(buffer->tag + index), (u32)((u8 *)buffer->data + VIBUF_ELM_SIZE * index), id,
                       VIBUF_ELM_SIZE / 16);
        index = (index + 1) % buffer->n;
    }

    buffer->dmaN += read_n;

    if (buffer->dmaN) {
        if (isNewData) {
            d4chcr = (d4chcr & 0x0fffffff) | (DMA_ID_REF << 28);
        }
        set_d4_chcr(d4chcr | 0x100);
    }

    SignalSema(buffer->semaphore);

    return 1;
}

extern s32 func_0023BF70(ViBuf *f) __attribute__((alias("FUN_0023bf70")));
