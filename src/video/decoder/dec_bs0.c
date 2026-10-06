/* Ported from rac1-decomp (src/game/movie/videodec.c, func_0023E298). */
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
extern int func_0012BA48(sceMpeg *);                             /* sceMpegIsEnd */
extern int sceMpegGetPicture(sceMpeg *, void *, int);            /* sceMpegGetPicture_pal */
extern void sceMpegReset(sceMpeg *);                             /* sceMpegReset_pal */
extern VoData *vo_buf_get_data(VoBuf *) __asm__("FUN_0023d288"); /* voBufGetData */
extern void vo_buf_inc_count(VoBuf *) __asm__("func_0023D210");  /* voBufIncCount */
extern void set_image_tag(void *, int, int,
                                     int) __asm__("func_0023B210"); /* setImageTag */
extern void switch_thread(void) __asm__("func_0023A770");           /* switchThread */
extern void err_message(char *) __asm__("func_0023AB78");       /* ErrMessage */
extern char D_001E8B38[];
extern char D_001E8B50[];
/* decBs0(VideoDec *) */
int dec_bs0(VideoDec *vd) __asm__("FUN_0023cec8");

int dec_bs0(VideoDec *vd) {
    VoData *voData;
    int status = 1;
    int i;

    while (!func_0012BA48(&vd->mpeg)) {
        if (func_0023CC80((int *)vd) == VD_STATE_ABORT) {
            status = -1;
            DebugPrint(D_001E8B38);
            break;
        }

        while (!(voData = vo_buf_get_data(&voBuf))) {
            switch_thread();
        }

        if (sceMpegGetPicture(&vd->mpeg, voData->v, 0x340) < 0) {
            err_message(D_001E8B50);
        }

        if (vd->mpeg.frameCount == 0) {
            int image_w = vd->mpeg.width;
            int image_h = vd->mpeg.height;

            for (i = 0; i < voBuf.size; i++) {
                set_image_tag(((VoTag *)voBuf.tag)[i].body,
                                         (int)((VoData *)voBuf.data)[i].v, image_w, image_h);
            }
        }

        vo_buf_inc_count(&voBuf);
        switch_thread();
    }
    sceMpegReset(&vd->mpeg);
    return status;
}

extern __typeof__(dec_bs0) func_0023CEC8 __attribute__((alias("FUN_0023cec8")));
