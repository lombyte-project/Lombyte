#include "types.h"

typedef struct {
    char pad00[0x10];
    unsigned char nframes; /* 0x10 */
    signed char unk11;     /* 0x11 */
} MobyISeq;

typedef struct {
    char pad00[6];
    unsigned char unk06;  /* 0x06 */
    char pad07[5];
    unsigned char unk0C; /* 0x0C */
    char pad0D;
    unsigned char unk0E; /* 0x0E */
    unsigned char unk0F; /* 0x0F */
    int unk10;           /* 0x10 */
    char pad14[0x10];
    float unk24;         /* 0x24 */
    char pad28[0x18];
    int unk40;           /* 0x40 */
    unsigned short unk44; /* 0x44 */
    char pad46[2];
    MobyISeq *seq;       /* 0x48 */
} MobyIClass;

typedef struct {
    char pad00[0x21];
    unsigned char unk21;   /* 0x21 */
    unsigned char oClass;  /* 0x22 */
    unsigned char unk23;   /* 0x23 */
    MobyIClass *pClass;    /* 0x24 */
    char pad28[4];
    float unk2C;           /* 0x2C */
    char pad30[4];
    unsigned short flags;  /* 0x34 */
    unsigned short unk36;  /* 0x36 */
    unsigned long unk38;   /* 0x38 */
    char pad40[0x18];
    float unk58;           /* 0x58 */
    float unk5C;           /* 0x5C */
    char pad60[0x11];
    unsigned char unk71;   /* 0x71 */
    unsigned char unk72;   /* 0x72 */
    unsigned char unk73;   /* 0x73 */
    int unk74;             /* 0x74 */
    char pad78[4];
    unsigned char unk7C;   /* 0x7C */
    unsigned char unk7D;   /* 0x7D */
    unsigned char unk7E;   /* 0x7E */
    unsigned char unk7F;   /* 0x7F */
    char pad80[4];
    int unk84;             /* 0x84 */
    int unk88;             /* 0x88 */
    char pad8C[4];
    int unk90;             /* 0x90 */
    int unk94;             /* 0x94 */
    char pad98[8];
    unsigned char unkA0;   /* 0xA0 */
    unsigned char unkA1;   /* 0xA1 */
    unsigned char unkA2;   /* 0xA2 */
    unsigned char unkA3;   /* 0xA3 */
    unsigned char unkA4;   /* 0xA4 */
    char padA5;
    short unkA6;           /* 0xA6 */
    int unkA8;             /* 0xA8 */
    int unkAC;             /* 0xAC */
    char padB0[0xD];
    unsigned char unkBD;   /* 0xBD */
    char padBE[0x42];
} MobyI;

extern u8 D_001B3AC0[];
extern s32 D_001B3580[];
extern MobyIClass *D_001B3200[];
extern char *D_0015FF18;
extern void FillTransferWords();
extern void func_0020C880(void *);

/* InitMobyInstance: clears the 0x100-byte moby, fills its defaults (class
   byte from D_001B3AC0[oClass], colours, its slot index from the moby
   array base D_0015FF18), flags a class with no D_001B3580 entry, then
   copies the class record D_001B3200[class] (or marks the moby dead when
   there is none) and applies the animation-sequence rules after
   func_0020C880. The moby is a real struct so its non-byte stores are "in
   struct" and D_0015FF18's load can move above them; the class load comes
   first, and the default stores are ordered so that sched1's
   register-pressure tie-break (stores that free a register go first)
   reproduces retail's store order. */
void init_moby_instance(void *arg0, int oClass) __asm__("FUN_0020c5f0");

void init_moby_instance(void *arg0, int oClass) {
    MobyI *m = (MobyI *)arg0;
    unsigned char c;
    int idx;
    MobyIClass *pClass;

    FillTransferWords(m, 0, 0x100);
    c = D_001B3AC0[oClass];
    m->unk23 = 0x80;
    m->oClass = c;
    m->unkA4 = 0xFF;
    m->unk21 = 0xFF;
    m->unk71 = 0xFF;
    m->unk72 = 0xFF;
    m->unkA6 = oClass;
    m->unk38 = 0x40404000000000L;
    m->unk36 = 0x7F80;
    idx = ((char *)m - (char *)D_0015FF18) >> 8;
    m->unkA8 = idx << 16;
    m->unkAC = idx;
    m->unk7E = 0;
    m->unk7C = 0xFF;
    m->unkA0 = 0x7F;
    m->unkA2 = 0x80;
    m->unk7D = 0xFF;
    m->unkA1 = 0x7F;
    m->unkA3 = 0x80;
    m->unk74 = D_001B3580[m->oClass];
    if (m->unk74 == 0) {
        m->flags |= 2;
    }
    pClass = D_001B3200[m->oClass];
    if (pClass != 0) {
        MobyIClass *p;

        m->pClass = pClass;
        m->unk72 = pClass->unk0E;
        m->flags |= pClass->unk44;
        m->unk94 = pClass->unk10;
        m->unk2C = pClass->unk24;
        m->unk58 = 1.0f;
        m->unk5C = 1.0f;
        if (pClass->unk40 != 0) {
            m->flags |= 0x10;
            m->unk90 = pClass->unk40;
        }
        {
            /* retail re-reads m->pClass (lw v0, 0x24(s1)) before each of the
               three tests below: the stores in between go to the same struct,
               so cc1 must not carry the pointer across them. Reading the field
               through a volatile copy of the pointer is what keeps every read
               a real load; the stores still go through the plain m. */
            volatile MobyI *vm = m;

            if (vm->pClass->unk0F != 0) {
                m->unk7F = 0x18;
                m->flags |= 0x400;
                m->unk84 = 0;
                m->unk88 = 0;
                m->unkBD = 0;
            }
            if (vm->pClass->unk06 != 0) {
                m->unk73 = 0x18;
            }
            if (vm->pClass->seq == 0) {
                return;
            }
        }
        func_0020C880(m);
        if (m->pClass->seq->nframes >= 2) {
            m->flags &= 0xFFFD;
        }
        p = m->pClass;
        if (p->unk0C == 1) {
            if (p->seq->nframes < 2) {
                m->unk58 = 0.0f;
                if (p->seq->unk11 < 0) {
                    m->flags |= 0x40;
                }
            }
        }
        return;
    }
    m->pClass = 0;
    m->flags |= 5;
    m->unk94 = 0;
}

extern __typeof__(init_moby_instance) func_0020C5F0 __attribute__((alias("FUN_0020c5f0")));
