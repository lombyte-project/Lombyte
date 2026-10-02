#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/world/data/select_world_object_resource_tables/FUN_00204a40.s", FUN_00204a40);
#else
/* Ported from rac1-decomp, the PAL decompilation notes (func_00205270, loaders.c). */
#include "sda.h"

typedef struct {
    char pad00[0x10];
    int cls;
    char pad14[0x38];
} GadgetRec;

/* A moby class: +0x00 the sequences, +0x06 the sequence count, +0x07
   the one to patch, +0x28 the 0x20-byte per-bank entries, +0x2C the
   collision data. */
typedef struct {
    char *seqs;
    char pad04[2];
    unsigned char seqCount;
    unsigned char seq;
} MobyClassHdr;

extern int D_0015FF44 MACRO_ADDR;
extern int D_0015FF48 MACRO_ADDR;
extern int D_0015FF4C MACRO_ADDR;
extern int D_0015FF50 MACRO_ADDR;
extern int D_001CBAC0[];
extern int D_001CBB20[];
extern char D_001CBBE0[][0x10];
extern short D_001CBD60[][0x10];
extern char D_001CAAC0[];
typedef struct { char pad[0x10]; char *p10; } Lvl;
extern Lvl D_001940C0;
extern unsigned char D_001B3AC0[] NOT_SDA;
extern char *D_001B3200[] NOT_SDA;
extern int D_001B6180[];
extern GadgetRec D_001863D0[];
extern unsigned char D_0013E520[];
extern long D_0019E6F0[];
extern void FlushCache(int);
extern void func_0020B618(int, void *);
extern void func_00203338(void *, void *, void *, int);

void FUN_00204a40(int cls, int mode) {
    int i;
    int n;
    char *data;
    int slot;
    char **pp;
    int flag;
    int j;

    if (D_0015FF4C >= 0 && D_001CBAC0[D_0015FF4C] == cls) {
        return;
    }
    for (D_0015FF4C = 0; D_0015FF4C < D_0015FF48; D_0015FF4C++) {
        if (D_001CBAC0[D_0015FF4C] == cls) {
            break;
        }
    }
    if (mode == -1) {
        mode = D_0015FF50 == 0;
    }
    D_0015FF50 = mode;
    data = D_001940C0.p10 + mode * 0x18000;
    FlushCache(0);
    func_0020B618(D_001CBB20[D_0015FF4C], data);
    FlushCache(0);
    D_001B3200[slot = D_001B3AC0[cls]] = data;
    D_001B6180[slot] = *(int *)(data + 0x2C);
    func_00203338(data, D_001CAAC0, D_001CBBE0[D_0015FF4C], cls);
    n = D_0015FF4C;
    flag = D_0015FF44;
    for (i = 0; i < 16; i++) {
        short v = D_001CBD60[n][i];
        if (v >= 0) {
            *(short *)(*(char **)(D_001B3200[slot] + 0x28) + i * 0x20 + 0x1A) = v;
            *(int *)(*(char **)(D_001B3200[slot] + 0x28) + i * 0x20 + 0x1C) = flag;
        }
    }
    for (j = 0; j < 0x25; j++) {
        if (D_001863D0[j].cls == cls) {
            MobyClassHdr *h;
            char *seq;
            char *e;

            if (D_0013E520[j] == 0) {
                return;
            }
            h = (MobyClassHdr *)D_001B3200[slot];
            if (h->seqCount == 0) {
                return;
            }
            seq = h->seqs + h->seq * 0x10;
            e = *(char **)seq + (*(int *)(seq + 4) - 4) * 0x10;
            *(long *)(e + 0x20) = D_0019E6F0[0];
            *(long *)(e + 0x30) = D_0019E6F0[2];
            return;
        }
    }
}
#endif /* NON_MATCHING */
