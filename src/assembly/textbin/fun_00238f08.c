#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00238f08/FUN_00238f08.s", FUN_00238f08);
#else
#include "types.h"

struct MenuPacket {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    u8 padC[2];
    u16 unkE;
    s16 unk10;
    u16 unk12;
    u8 pad14[4];
};

struct Slot {
    s32 id;
    s32 kind;
    u8 pad8[0xC];
};

struct Shop {
    u8 pad0[0x40];
    s32 hard;
    u8 pad44[0x14];
    s32 cur;
    s32 open;
    u8 pad60[0x70];
    struct Slot slots[1];
};

struct Goal {
    s32 bolts;
    s32 boltsHard;
    u16 cost;
    u16 costHard;
    u16 pad0C;
    u16 need;
    u8 pad10[8];
};

extern struct Shop D_001E63C0;
extern struct Goal D_001DFFB0[];
extern s32 D_0013D428[];
extern u8 D_0013D4E3[];
extern s32 D_0015ED98;
extern void func_001FB8F0(s32, s32, s32, s32, s32, s32, u32);
extern s32 func_001FDD10(s32);
extern void func_001F75F0(void *, u64, void *, s32);
extern void *memset(void *, s32, u32);

void FUN_00238f08(s32 arg0, s32 w, s32 h) {
    struct MenuPacket pkt;
    s32 cost;
    s32 msg;

    func_001FB8F0(0, 0, 0x200, 0x80, 0x200, 0x80, 0);
    memset(&pkt, 0, 0x18);
    pkt.unk2 = h;
    pkt.unk6 = w;
    pkt.unk8 = w >> 1;
    pkt.unkA = (h >> 1) - 7;
    pkt.unk10 = 0x10;
    pkt.unk12 = 1;
    if (D_001E63C0.open != 0) {
        if (D_001E63C0.slots[D_001E63C0.cur].kind == 1 && D_0013D428[D_001E63C0.slots[D_001E63C0.cur].id] >= D_001DFFB0[D_001E63C0.slots[D_001E63C0.cur].id].need) {
            msg = 0x5233;
        } else if (D_001E63C0.slots[D_001E63C0.cur].kind == 1) {
            if (D_001E63C0.hard != 0) {
                cost = D_001DFFB0[D_001E63C0.slots[D_001E63C0.cur].id].costHard;
            } else {
                cost = D_001DFFB0[D_001E63C0.slots[D_001E63C0.cur].id].cost;
            }
            msg = D_0015ED98 < cost ? 0x5234 : 0x5233;
        } else {
            if (D_0013D4E3[0] != 0) {
                cost = D_001DFFB0[D_001E63C0.slots[D_001E63C0.cur].id].boltsHard;
            } else {
                cost = D_001DFFB0[D_001E63C0.slots[D_001E63C0.cur].id].bolts;
            }
            msg = D_0015ED98 < cost ? 0x524E : 0x5233;
        }
    } else {
        msg = 0x5234;
    }
    if (msg != 0) {
        pkt.unk12 |= 4;
        func_001F75F0(&pkt, 0x80F0F0F0, func_001FDD10(0x5234), -1);
        pkt.unk12 ^= 4;
        pkt.unkA = (h - (s16)pkt.unkE) >> 1;
        func_001F75F0(&pkt, 0x80F0F0F0, func_001FDD10(0x5234), -1);
    }
}
#endif /* NON_MATCHING */
