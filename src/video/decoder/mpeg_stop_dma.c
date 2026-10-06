/* Ported from rac1-decomp (src/game/movie/videodec.c, func_0023E4B0). */
#ifndef COMMON_H
#include "sda.h"
#endif /* STRUCTS_H */
#ifndef EZMPEG_H
typedef struct {
    long pts;
    long dts;
    int pos;
    int len;
} TimeStamp;
typedef struct {
    int d4madr;
    int d4tadr;
    int d4qwc;
    int d4chcr;
    int d3madr;
    int d3qwc;
    int d3chcr;
    int ipubp;
    int ipuctrl;
} sceIpuDmaEnv;
typedef struct {
    long long *data;  /* 0x00 */
    long long *tag;   /* 0x04 */
    int n;            /* 0x08 */
    int dmaStart;     /* 0x0C */
    int dmaN;         /* 0x10 */
    int readBytes;    /* 0x14 */
    int buffSize;     /* 0x18 */
    sceIpuDmaEnv env; /* 0x1C */
    int sema;         /* 0x40 */
    int isActive;     /* 0x44 */
    long totalBytes;  /* 0x48 */
    TimeStamp *ts;    /* 0x50 */
    int n_ts;         /* 0x54 */
    int count_ts;     /* 0x58 */
    int wt_ts;        /* 0x5C */
} ViBuf;
typedef struct {
    int width;
    int height;
    int frameCount;
    long pts;
    long dts;
    unsigned long flags;
    long pts2nd;
    long dts2nd;
    unsigned long flags2nd;
    void *sys;
} sceMpeg; /* 0x48 */
typedef struct {
    int type;
} sceMpegCbData;
typedef struct {
    sceMpeg mpeg;       /* 0x00 */
    ViBuf vibuf;        /* 0x48 */
    unsigned int state; /* 0xA8 */
    int sema;
    int hid_endimage;
    int hid_vblank;
} VideoDec;
#endif
typedef struct {
    char _pad0[0xD9048];
    VideoDec videoDec; /* 0xD9048 */
} MovieGlobals;
extern MovieGlobals *D_0016120C MACRO_ADDR;
#define videoDec (D_0016120C->videoDec)
extern int vi_buf_stop_dma(ViBuf *) __asm__("func_0023C170"); /* viBufStopDMA */
/* mpegStopDMA */
int mpeg_stop_dma(sceMpeg *mp, sceMpegCbData *cbdata, void *anyData) __asm__("FUN_0023d0e0");

int mpeg_stop_dma(sceMpeg *mp, sceMpegCbData *cbdata, void *anyData) {
    vi_buf_stop_dma(&videoDec.vibuf);
    return 1;
}

extern __typeof__(mpeg_stop_dma) func_0023D0E0 __attribute__((alias("FUN_0023d0e0")));
