/* Ported from rac1-decomp (src/game/loaders.c, func_00204FC0). */

#include "sda.h"
#include "rnc/gameplay/hero.h"

extern void FlushCache(int);

struct PartList {
    unsigned char pad00[6];
    unsigned char flag; /* 0x06 */
    unsigned char pad07[5];
    unsigned char used; /* 0x0C */
    unsigned char pad0D[0x3B];
    int entries[1]; /* 0x48 */
};

struct ColorSrc {
    unsigned char pad00[0x38];
    unsigned long color; /* 0x38 */
};

struct GlobalIndex {
    unsigned char pad00[0x26];
    short slot; /* 0x26 */
};

struct Moby {
    unsigned char pad00[0x24];
    struct PartList *parts; /* 0x24 */
    unsigned char pad28[0xA];
    unsigned short unk32; /* 0x32 */
    unsigned short unk34; /* 0x34 */
    unsigned char pad36[2];
    unsigned long color; /* 0x38 */
    unsigned char pad40[8];
    int attach; /* 0x48 */
    unsigned char pad4C[6];
    unsigned char idx;  /* 0x52 */
    unsigned char slot; /* 0x53 */
    unsigned char pad54[0x1E];
    unsigned char flag72; /* 0x72 */
    unsigned char flag73; /* 0x73 */
    unsigned char pad74[4];
    int model; /* 0x78 */
    unsigned char pad7C[0x18];
    int unk94; /* 0x94 */
};

struct ModelRec {
    unsigned short flags; /* 0x00 */
    unsigned char pad02[2];
    int part_count; /* 0x04 */
    unsigned short pad08;
    unsigned char pad0A[2];
    unsigned short num_parts; /* 0x0C */
    unsigned char pad0E[2];
    int end_off;     /* 0x10 */
    int part_off[1]; /* 0x14 */
};

struct TransferState {
    unsigned char pad00[0x38];
    int unk38; /* 0x38 */
    unsigned char pad3C[4];
    unsigned short unk40; /* 0x40 */
    unsigned char pad42[2];
    short num_parts; /* 0x44 */
    unsigned char pad46[2];
    unsigned short unk48; /* 0x48 */
    unsigned char pad4A[2];
    int unk4C;            /* 0x4C */
    int unk50;            /* 0x50 */
    int unk54;            /* 0x54 */
    struct ModelRec *rec; /* 0x58 */
    int unk5C;            /* 0x5C */
    unsigned char pad60[0x118];
    struct Moby *slots[1]; /* 0x178 */
};

extern struct TransferState D_0018CB20_t __asm__("D_0018CB20") NOT_SDA;
extern struct GlobalIndex D_0013E030_g __asm__("D_0013E030");
extern int D_0015F604 MACRO_ADDR;
extern int D_00160488[];
extern int func_0020C468_2(int, int) __asm__("FUN_0020b618");
extern struct Moby *func_0020D348_m(int) __asm__("FUN_0020c4f8");
void update_world_object_animation(void *arg0) __asm__("FUN_00204790");

void update_world_object_animation(void *arg0) {
    struct ModelRec *rec;
    struct Moby *mob;
    int *cp;
    int *sp;
    int i;
    int k;
    int idx;
    int id;
    int off;
    int endp;
    int pc;

    FlushCache(0);
    func_0020C468_2(D_0018CB20_t.unk5C, (int)D_0018CB20_t.rec);
    FlushCache(0);

    rec = D_0018CB20_t.rec;
    D_0018CB20_t.unk38 = 0;
    cp = rec->part_off;
    D_0018CB20_t.unk40 = rec->flags;
    pc = rec->part_count;
    D_0018CB20_t.unk48 = rec->pad08;
    D_0018CB20_t.num_parts = rec->num_parts;
    D_0018CB20_t.unk54 = (int)rec + rec->end_off;
    if (pc < 0x400) {
        D_0018CB20_t.unk4C = 0;
    } else {
        D_0018CB20_t.unk4C = (int)rec + pc;
    }

    for (i = 0; i < D_0018CB20_t.num_parts; i++) {
        off = *cp++;
        sp = (int *)((unsigned char *)rec + off);
        id = sp[0];
        sp = (int *)((unsigned char *)sp + 0xC);
        endp = (int)rec + sp[0];
        sp = (int *)((unsigned char *)sp + 4);
        if (D_0015F604 == 6 && i == 0 && id == 0x215) {
            id = D_00160488[D_0013E030_g.slot];
        }
        mob = D_0018CB20_t.slots[i];
        if (mob == 0) {
            mob = func_0020D348_m(id);
            idx = mob->parts->used;
            mob->parts->used = idx + 1;
            mob->idx = idx;
            mob->slot = idx;
            mob->unk32 = 0x1FF;
            mob->unk34 |= 6;
            mob->flag72 = 0xFF;
            mob->unk94 = 0;
            if (hero.moby == 0) {
                mob->color = 0x38383800000000;
            } else {
                mob->color = ((struct ColorSrc *)hero.moby)->color;
            }
            if (mob->parts->flag) {
                mob->flag73 = 0x18;
            }
            D_0018CB20_t.slots[i] = mob;
        }
        mob->model = endp;
        mob->parts->entries[mob->idx] = sp;
        for (k = 0; k < ((unsigned char *)sp)[0x10]; k++) {
            int *w = (int *)((unsigned char *)sp + 0x1C);

            w[k] = (int)sp + w[k];
        }
    }
}

extern __typeof__(update_world_object_animation) func_00204790
    __attribute__((alias("FUN_00204790")));
