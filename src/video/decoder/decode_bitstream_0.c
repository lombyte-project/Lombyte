/* Ported from rac1-decomp, the PAL decompilation (src/game/movie/videodec.c, func_0023E298). */
#ifndef COMMON_H
#define MACRO_ADDR __attribute__((section(".sdata")))
#endif /* STRUCTS_H */
extern int DebugPrint();
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
#define VD_STATE_ABORT 1
typedef struct {
    sceMpeg mpeg;     /* 0x00 */
    ViBuf vibuf;      /* 0x48 */
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
typedef struct {
    void *data;
    void *tag;
    volatile int write;
    volatile int count;
    int size;
} VoBuf;
#define voBuf (*(VoBuf *)((char *)D_0016120C + 0xD9168))
typedef struct {
    char hdr[0x40];
    char body[0x138C0 - 0x40];
} VoTag;
typedef struct {
    char v[0xD0000];
} VoData;
extern int func_0012BA48(sceMpeg *);                      /* sceMpegIsEnd */
extern int sceMpegGetPicture(sceMpeg *, void *, int);         /* sceMpegGetPicture_pal */
extern void sceMpegReset(sceMpeg *);                     /* sceMpegReset_pal */
extern VoData *FUN_0023d288(VoBuf *); /* voBufGetData */
extern void func_0023D210(VoBuf *);    /* voBufIncCount */
extern void func_0023B210(void *, int, int, int);         /* setImageTag */
extern void func_0023AB78(char *);                        /* ErrMessage */
extern char D_001E8B38[];
extern char D_001E8B50[];
/* decBs0(VideoDec *) */
int decode_bitstream_0(VideoDec *vd) __asm__("FUN_0023cec8");

int decode_bitstream_0(VideoDec *vd) {
    VoData *voData;
    int status = 1;
    int i;

    while (!func_0012BA48(&vd->mpeg)) {
        if (func_0023CC80((int *)vd) == VD_STATE_ABORT) {
            status = -1;
            DebugPrint(D_001E8B38);
            break;
        }

        while (!(voData = FUN_0023d288(&voBuf))) {
            func_0023A770();
        }

        if (sceMpegGetPicture(&vd->mpeg, voData->v, 0x340) < 0) {
            func_0023AB78(D_001E8B50);
        }

        if (vd->mpeg.frameCount == 0) {
            int image_w = vd->mpeg.width;
            int image_h = vd->mpeg.height;

            for (i = 0; i < voBuf.size; i++) {
                func_0023B210(((VoTag *)voBuf.tag)[i].body,
                              (int)((VoData *)voBuf.data)[i].v, image_w, image_h);
            }
        }

        func_0023D210(&voBuf);
        func_0023A770();
    }
    sceMpegReset(&vd->mpeg);
    return status;
}
extern void func_0023A770(void);  /* switchThread */

extern __typeof__(decode_bitstream_0) func_0023CEC8 __attribute__((alias("FUN_0023cec8")));
