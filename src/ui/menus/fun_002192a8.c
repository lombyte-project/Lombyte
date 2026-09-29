#include "types.h"

struct NO;
struct Own;
struct Mth;

struct Mth {
    u8 pad0[0x8];
    void (*unk8)(struct Mth *, s32);
    void (*unkC)(struct Mth *, s32);
};

struct NO {
    s32 (*unk0)(struct NO *);
    s32 unk4;
    void (*unk8)(struct NO *, s32);
    void (*unkC)(struct NO *, s32);
    u8 pad10[0x4];
    struct Ent *unk14;
};

struct Own {
    s32 ids[14];
    struct Own *unk38;
    s32 unk3C;
    struct Mth *unk40;
    struct NO *objs[14];
    u8 pad7C[0x4];
    struct Mth *unk80;
};

struct GameState {
    s32 state;
    struct Own *owner;
    struct Own *unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad18[0xB8];
    struct Own *unkD0;
    u8 padD4[0x30];
    s32 unk104;
    s32 unk108;
    s32 unk10C;
    s32 progress;
};

struct Ent3 {
    u8 pad0[0x10];
    u8 unk10;
};

struct Ent2 {
    u8 pad0[0x48];
    struct Ent3 *unk48[1];
};

struct Ent {
    u8 pad0[0x24];
    struct Ent2 *unk24;
    u8 pad28[0x30];
    f32 unk58;
};

extern struct GameState D_001D5BF0;
extern s32 D_0015EEB4;
extern s16 D_001516D8[];
extern s32 D_0015F5B8;
extern s32 D_0015F618;
extern s32 D_0015F604;
extern struct Ent *D_001D5D90[];

extern void func_00218D10(void);
extern void func_00218F98(void) __asm__("FUN_00218f98");
extern void FUN_00212e28();
extern s32 func_0022DA68(s32, s32, struct Ent *);
extern void func_001FBAB8(s32, struct Own *);
extern void CalculateDmaTransferAddress(void);
extern void update_fog(void) __asm__("FUN_001f2588");
extern void FUN_00212ed8(struct Ent *, s32, s32);
extern void func_002191B8(void) __asm__("FUN_002191b8");

void FUN_002192a8(void) {
    s32 i;
    s32 j;
    s32 k;
    s32 flag;
    struct NO *obj;
    struct NO **objs;
    struct Mth *m;

    D_001D5BF0.progress = D_001D5BF0.progress + 1;
    func_00218D10();
    if (D_001D5BF0.state == 0x14) {
        if (D_001D5BF0.unk14 != 0) {
            D_001D5BF0.unk14 = D_001D5BF0.unk14 - 1;
            if (D_001D5BF0.unk14 != 0) {
                return;
            }
        }
        if (D_001516D8[0] != 0) {
            return;
        }
        D_0015F5B8 = 0x1E000;
        CalculateDmaTransferAddress();
        update_fog();
        D_0015F618 = 1;
        D_001D5BF0.unk108 = 0;
        D_001D5BF0.unk10C = 0;
        D_001D5BF0.unk104 = 0;
        D_001D5BF0.unk10 = 0;
        D_0015F604 = 0;
        return;
    }
    if (D_001D5BF0.state == 0 || D_001D5BF0.state == 0x2D) {
        func_00218F98();
    }
    if (D_0015EEB4 & 1) {
        func_001FBAB8(3, D_001D5BF0.owner);
        return;
    }
    if (D_001D5BF0.state == 1) {
        D_001D5BF0.unk14 = (D_001D5BF0.unk14 < 1) ? 0 : D_001D5BF0.unk14 - 1;
        if (D_001D5BF0.unk14 == 0) {
            struct Own *o8 = D_001D5BF0.unk8;
            D_001D5BF0.unk8 = 0;
            D_001D5BF0.owner = o8;
            D_001D5BF0.state = o8->unk3C;
            for (i = 0; i < 14; i++) {
                obj = D_001D5BF0.owner->objs[i];
                if (obj != 0 && obj->unk8 != 0) {
                    obj->unk8(obj, 0);
                }
            }
        }
    } else if (D_001D5BF0.unk8 != 0) {
        if (D_001D5BF0.owner == D_001D5BF0.unk8) {
            func_0022DA68(3, 0x11, D_001D5D90[0]);
        } else {
            func_0022DA68(4, 0x11, D_001D5D90[0]);
        }
        for (i = 0; i < 14; i++) {
            obj = D_001D5BF0.owner->objs[i];
            if (obj != 0 && obj->unkC != 0) {
                obj->unkC(obj, 0);
            }
        }
        flag = (D_001D5BF0.unk8 == D_001D5BF0.owner->unk38);
        if (D_001D5BF0.owner == D_001D5BF0.unk8) {
            flag = flag ^ 1;
        }
        for (j = 0; j < 14; j++) {
            if (D_001D5BF0.unk8->objs[j] != 0) {
                D_001D5BF0.unk8->objs[j]->unk14 = D_001D5D90[j];
            }
            if (flag) {
                s32 n = D_001D5BF0.owner->ids[j];
                FUN_00212ed8(D_001D5D90[j], n, D_001D5D90[j]->unk24->unk48[n]->unk10 - 1);
                D_001D5D90[j]->unk58 = -1.0f;
            } else {
                FUN_00212ed8(D_001D5D90[j], D_001D5BF0.unk8->ids[j], 0);
                D_001D5D90[j]->unk58 = 1.0f;
            }
        }
        D_001D5BF0.state = 1;
        D_001D5BF0.unk14 = 12;
        D_001D5BF0.unkD0 = D_001D5BF0.owner;
        D_001D5BF0.owner = 0;
    }
    if (D_001D5BF0.owner != 0) {
        objs = D_001D5BF0.owner->objs;
        for (k = 0; k < 14; k++) {
            if (objs != 0 && objs[k] != 0 && objs[k]->unk0 != 0) {
                if (objs[k]->unk0(objs[k]) != 0) {
                    D_001D5BF0.unkC = 1;
                }
            }
        }
        if (D_001D5BF0.owner != 0 && D_001D5BF0.owner->unk80 != 0) {
            m = D_001D5BF0.owner->unk40;
            if (m->unkC != 0) {
                m->unkC(m, 1);
            }
            D_001D5BF0.owner->unk40 = D_001D5BF0.owner->unk80;
            D_001D5BF0.owner->unk80 = 0;
            m = D_001D5BF0.owner->unk40;
            if (m->unk8 != 0) {
                m->unk8(m, 1);
            }
        }
    }
    FUN_00212e28();
    if (D_001D5BF0.unkC != 0) {
        func_002191B8();
    }
}

extern __typeof__(FUN_002192a8) func_002192A8 __attribute__((alias("FUN_002192a8")));
