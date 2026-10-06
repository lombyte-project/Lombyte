#include "types.h"
struct Obj {
    u8 pad0[0x30];
    s32 slots[19];
    s32 handle;
};
extern s32 D_001A0314[];
extern s32 D_001CF874[];
extern s32 D_001CF758[];
extern void update_mission_list(struct Obj *) __asm__("FUN_0020b950");
extern s32 collect_mission_ids(void *, s32, void *, s32) __asm__("FUN_0020bc00");
s32 FUN_0021c420(struct Obj *obj) {
    s32 *slot;

    update_mission_list(obj);
    obj->handle = collect_mission_ids((void *)0x70000000, 0, (void *)0x70000100, 1);
    slot = obj->slots;
    slot += D_001A0314[0];
    if (*slot != -1) {
        D_001CF874[0] = ((s32 *)0x70000000)[*slot];
        D_001CF758[0] = ((s32 *)0x70000100)[*slot];
    }
    return 0;
}

extern __typeof__(FUN_0021c420) func_0021C420 __attribute__((alias("FUN_0021c420")));
