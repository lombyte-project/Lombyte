#ifndef LOMBYTE_RNC_VIDEO_DECODER_VI_BUF_H
#define LOMBYTE_RNC_VIDEO_DECODER_VI_BUF_H

#include "types.h"
#include "eetypes.h"

/* One PTS/DTS entry of a ViBuf timestamp ring (vi_buf_reset clears them). */
struct ViBufTimeStamp {
    s64 pts;                  /* 0x0: -1 when empty (vi_buf_reset) */
    s64 dts;                  /* 0x8: -1 when empty (vi_buf_reset) */
    s32 pos;                  /* 0x10 */
    s32 len;                  /* 0x14 */
};

/*
 * Video input ring buffer fed to the IPU through DMA channel 4 (the type
 * name comes from the recovered symbol viBufDelete__FP5ViBuf). The data is
 * n blocks of 0x800 bytes, each with a DMA tag in `tag`.
 */
struct ViBuf {
    u128 *data;               /* 0x0: block storage; D4_MADR at reset, ptr = data + offset in vi_buf_begin_put */
    u128 *tag;                /* 0x4: n + 1 DMA tags (block i -> data + i * 0x800, last one loops back); D4_TADR at reset */
    s32 n;                    /* 0x8: block count; vi_buf_begin_put frees (n - (dma_n + 2)) blocks */
    s32 dma_start;            /* 0xC: first block owned by DMA, 0 at reset */
    s32 dma_n;                /* 0x10: blocks owned by DMA, 0 at reset; vi_buf_count = (dma_n << 11) + read_bytes */
    s32 read_bytes;           /* 0x14: bytes put since the last whole block; rounded up to 0x800 by vi_buf_flush, += bytes in vi_buf_end_put */
    s32 buff_size;            /* 0x18: ring size in bytes, modulus of the put offset (vi_buf_begin_put) */
    u32 d4_madr;              /* 0x1C: D4_MADR saved by vi_buf_stop_dma */
    u32 d4_tadr;              /* 0x20: D4_TADR saved by vi_buf_stop_dma */
    u32 d4_qwc;               /* 0x24: D4_QWC saved by vi_buf_stop_dma */
    u32 d4_chcr;              /* 0x28: D4_CHCR saved by vi_buf_stop_dma */
    u32 d3_madr;              /* 0x2C: D3_MADR saved by vi_buf_stop_dma */
    u32 d3_qwc;               /* 0x30: D3_QWC saved by vi_buf_stop_dma */
    u32 d3_chcr;              /* 0x34: D3_CHCR saved by vi_buf_stop_dma */
    u32 ipu_bp;               /* 0x38: IPU_BP saved by vi_buf_stop_dma */
    u32 ipu_ctrl;             /* 0x3C: IPU_CTRL saved by vi_buf_stop_dma */
    s32 sema;                 /* 0x40: semaphore around every access; DeleteSema in vi_buf_delete */
    s32 is_active;            /* 0x44: 1 at reset, 0 after vi_buf_stop_dma */
    s64 total_bytes;          /* 0x48: running total of bytes put (vi_buf_end_put) */
    struct ViBufTimeStamp *ts; /* 0x50: timestamp ring of n_ts entries */
    s32 n_ts;                 /* 0x54 */
    s32 count_ts;             /* 0x58: 0 at reset */
    s32 wt_ts;                /* 0x5C: 0 at reset */
};

#endif /* LOMBYTE_RNC_VIDEO_DECODER_VI_BUF_H */
