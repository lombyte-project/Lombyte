#include "types.h"
struct Anim { u8 pad0[0x10]; u8 count; };
struct AnimSet { u8 pad0[0x48]; struct Anim *anims[1]; };
struct Obj {
    u8 pad0[0x24]; struct AnimSet *set; u8 pad28[0x28];
    u8 cur; u8 next; u8 sel; u8 sel2; u8 pad54[8]; f32 time; u8 pad60[8]; f32 *start; u8 pad6c[4]; u8 flags;
};
extern void func_0020C880(struct Obj *);

void set_moby_animation(struct Obj *o, s32 sel, s32 idx) __asm__("FUN_00212ed8");

void set_moby_animation(struct Obj *o, s32 sel, s32 idx) {
    struct Anim **slot;
    /* retail register file: a0 = &anims[sel], v0 = anim pointer / o->cur,
       a1 = count / o->next */
    register struct Anim **anims __asm__("$4");
    register s32 n __asm__("$5");
    register s32 v __asm__("$2");

    anims = o->set->anims;
    slot = &anims[sel];
    n = (*slot)->count;
    o->sel = sel;
    v = n - 1;
    if (idx < n) {
        v = idx;
    }
    o->cur = v;
    o->next = v + 1;
    if ((*slot)->count - 1 < o->next) {
        o->next = (*slot)->count - 1;
    }
    o->sel2 = sel;
    if (o->next >= o->set->anims[sel]->count) {
        o->next = 0;
    }
    func_0020C880(o);
    o->time = *o->start;
    o->flags &= ~2;
}

extern __typeof__(set_moby_animation) func_00212ED8 __attribute__((alias("FUN_00212ed8")));
