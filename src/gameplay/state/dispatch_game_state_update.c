#include "types.h"

extern s32 D_0013E050[];
extern s32 D_0015F434 __attribute__((sda));
extern s32 D_0015F618;
extern void func_001F4248(void);
extern void func_001F39D0(void);
extern void func_0022F288(void);

void dispatch_game_state_update(void) __asm__("FUN_00230ee8");

void dispatch_game_state_update(void) {
    s32 state;

    if (D_0015F618 != 0) {
        return;
    }
    state = D_0013E050[0];
    if ((u32)state >= 9U) {
        return;
    }
    switch (state) {
    case 0:
    case 8:
        func_001F4248();
        return;
    case 3:
    case 7:
        D_0015F434 = 0x7F;
        func_001F39D0();
        return;
    case 4:
        func_0022F288();
        break;
    }
}

extern __typeof__(dispatch_game_state_update) func_00230EE8 __attribute__((alias("FUN_00230ee8")));
