#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00226b08/FUN_00226b08.s", FUN_00226b08);
#else
#include "types.h"

/* Reloads the level state while keeping the persistent parts: saves the
   save-game blocks to the scratchpad, runs the reset (func_00209370),
   copies them back (only the listed bytes of D_0013D4C0, and the
   D_00141EA0 slots whose item is still owned), bumps the session count,
   restores the two unlock flags, rereads the clock, then optionally
   restarts at checkpoint `slot`. */

typedef struct {
    u8 pad0[0x14];
    s32 slot;
    u8 pad18[8];
    struct {
        s32 unk0;
        u8 pad4[0x18];
    } entries[5];
    u8 pad[0xC0 - 0x20 - 5 * 0x1C];
    s32 unkC0;
    u8 padC4[0x18];
    s32 unkDC;
    s32 unkE0;
    u8 padE4[8];
    s32 unkEC;
} Checkpoints;

typedef struct {
    u8 pad0[0xE0];
    s32 unkE0;
} GameInfo;

extern Checkpoints D_0013D290;
extern GameInfo D_001D5BF0;
extern u8 D_0013D388[];
extern u8 D_0013D408[];
extern s32 D_0013D428[];
extern u8 D_0013D4C0[];
extern u8 D_0013E520[];
extern u8 D_0014BEC0[];
extern s32 D_00141EA0[];
extern u8 D_0015EDD0[];
extern s32 D_0015EDA0;
extern s32 D_0015ED98;
extern u8 D_0015EE1C;
extern u8 D_0015EE1D;
extern s32 D_0015EE20;
extern u8 D_0015EE98[];
extern s32 D_001D5BA0[];

extern void func_001F9838(void *dst, void *src, s32 size);
extern void func_00209370(void);
extern void func_0020ABB0(s32);
extern s32 sceCdReadClock(u8 *clock);
extern void sceScfGetLocalTimefromRTC(u8 *clock);

void FUN_00226b08(s32 slot) {
    s32 saved;
    s32 count;
    s32 flag4;
    s32 flag5;
    s32 *p;
    s32 *items = (s32 *)0x70000150;
    u8 *bytes;
    s32 i;

    func_001F9838((void *)0x70000000, D_0013E520, 0x28);
    func_001F9838((void *)0x70000030, D_0013D4C0, 0x25);
    func_001F9838((void *)0x70000060, D_0013D428, 0x94);
    func_001F9838((void *)0x70000100, D_0014BEC0, 0x50);
    func_001F9838((void *)0x70000150, D_00141EA0, 0x20);
    func_001F9838((void *)0x70000170, D_0015EDD0, 0xC);
    func_001F9838((void *)0x70000180, D_0013D408, 0x20);
    saved = D_0015ED98;
    count = D_0015EE20;
    flag4 = D_0013D388[4] != 0;
    flag5 = D_0013D388[5] != 0;
    bytes = (u8 *)0x70000030;
    func_00209370();
    func_001F9838(D_0013E520, (void *)0x70000000, 0x28);
    for (p = D_001D5BA0; *p != -1; p++) {
        D_0013D4C0[*p] = bytes[*p];
    }
    func_001F9838(D_0013D428, (void *)0x70000060, 0x94);
    func_001F9838(D_0014BEC0, (void *)0x70000100, 0x50);
    for (i = 0; i < 8; i++) {
        s32 item = items[i];
        D_00141EA0[i] = D_0013D4C0[item] ? item : 0;
    }
    func_001F9838(D_0015EDD0, (void *)0x70000170, 0xC);
    func_001F9838(D_0013D408, (void *)0x70000180, 0x20);
    D_0015ED98 = saved;
    if (flag4) {
        D_0013D388[4] = 1;
        D_0015EDA0 = 5;
    }
    if (flag5) {
        D_0013D388[5] = 1;
        D_0015EDA0 = 8;
    }
    D_0015EE20 = count + 1;
    D_0015EE1C = 0;
    D_0015EE1D = 0;
    sceCdReadClock(D_0015EE98);
    sceScfGetLocalTimefromRTC(D_0015EE98);
    if (slot >= 0) {
        D_0013D290.slot = slot;
        D_0013D290.entries[slot].unk0 = 0;
        func_0020ABB0(D_001D5BF0.unkE0);
        D_0013D290.unkEC = D_001D5BF0.unkE0;
        D_0013D290.unkC0 = 0;
        if (D_0013D290.unkDC < 0) {
            D_0013D290.unkE0 = 0;
            D_0013D290.unkDC = 0x13;
        }
    }
}
#endif /* NON_MATCHING */
