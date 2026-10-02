#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_0021e230/FUN_0021e230.s", FUN_0021e230);
#else
#include "types.h"
#include "qcopy.h"

struct Anim { u8 pad0[0xC]; u8 count; };
struct MobyVars { void *owner; u8 pad4[8]; s32 id; };
struct Moby {
    u8 pad0[0x10];
    f32 x;
    f32 y;
    f32 z;
    u8 pad1C[8];
    struct Anim *anim;
    u8 pad28[0xC];
    s16 unk34;
    u8 pad36[0xA];
    f32 unk40;
    f32 unk44;
    f32 unk48;
    u8 pad4C[0x28];
    void (*update)();
    struct MobyVars *vars;
    u8 pad7C[0x2A];
    s16 oclass;
};
struct Obj {
    u8 pad0[0x14];
    s32 unk14;
    u8 pad18[0x18];
    s32 flags;
    s32 state;
    f32 fade;
    u8 pad3C[8];
    struct Moby *moby;
    struct Moby *moby2;
};
struct Entry { u8 pad0[8]; s32 type; u8 padC[4]; s32 oclass; u8 pad14[0x38]; };
struct Pos { f32 x0; f32 x1; f32 y; f32 z; f32 u10; f32 u14; u8 pad18[8]; };
struct Level { u8 pad0[0x3C]; s32 index; u8 pad40[8]; u8 *table; };
struct Game { u8 pad0[0x40]; struct Level *level; };
struct Menu {
    u8 pad0[4];
    struct Game *game;
    u8 pad8[0x110];
    s32 unk118;
    s32 cur;
    s32 unk120;
    u8 pad124[0x1C];
    s32 unk140;
    s32 unk144;
};
struct Cam { u8 pad0[0x140]; f32 x; f32 y; f32 z; };
struct Flag { u8 pad0[0xD]; u8 unkD; };

extern struct Menu D_001D5BF0;
extern struct Entry D_001863D0[];
extern struct Cam D_00186F40;
extern struct Pos D_001E0408[];
extern u8 D_0013E520[];
extern s32 D_00140408[];
extern s32 D_0015FF50;
extern u8 D_001B3AC0[];
extern struct Flag *D_001B3200[];
extern void func_0021E698();

extern void func_001E9470(s32, s32);
extern void func_001E9478(struct Moby *, s32);
extern f32 func_001FA580(f32, f32);
extern void func_00204A40(s32, s32);
extern void func_00212ED8(struct Moby *, s32, s32);
extern struct Moby *func_00225490(s32);
extern struct Moby *func_00225530(struct Moby *);

s32 FUN_0021e230(struct Obj *obj) {
    struct Level *lv;
    struct Moby *m;
    struct Moby *m2;
    struct Entry *e;
    s32 id;
    s32 prev;
    s32 oclass;
    s32 type;
    s32 is3;
    s32 fresh;
    s32 odd;
    s32 anim;
    s32 n;
    struct Moby *src;
    s32 is2;
    struct MobyVars *vars;
    s32 is1;
    f32 off;
    f32 cx;

    lv = D_001D5BF0.game->level;
    id = *(s16 *)(lv->table + lv->index * 10 + 6);
    if (obj->moby != 0) {
        prev = obj->moby->oclass;
    } else {
        prev = -1;
    }
    fresh = 0;
    oclass = D_001863D0[id].oclass;
    if (id == 0x18) {
        oclass = 0x1DF;
    }
    obj->fade = func_001FA580(obj->fade, 0.01f);
    type = D_001863D0[id].type;
    is2 = type == 2; is3 = type == 3; is1 = type == 1; if (!is2 && !is3 && !is1) { fresh = 1; }
    if (id == 0x18) {
        fresh = 0;
    }
    odd = obj->flags & 1;
    switch (obj->state) {
    case 0:
        obj->state = 1;
        break;
    case 1:
        if (oclass != -1) {
            obj->state = 2;
        }
        break;
    case 2:
        if (oclass == -1) {
            obj->state = 1;
            break;
        }
        if (fresh) {
            if (D_00140408[0] != 0 && oclass != D_00140408[0]) {
                func_001E9470(0, 0);
            }
            if (oclass != D_001D5BF0.cur) {
                func_00204A40(oclass, D_001D5BF0.unk118 == 0);
                D_001D5BF0.unk144 = D_0015FF50;
                D_001D5BF0.unk140 = oclass;
                D_001D5BF0.unk120 = oclass;
                D_001B3200[D_001B3AC0[oclass]]->unkD = 0;
            }
        }
        anim = 1; m = func_00225490(oclass); if (id == 2) { anim = 6; }
        if (m != 0) {
            if (fresh && D_0013E520[id] != 0) {
                func_001E9478(m, obj->unk14);
            }
            obj->moby = m;
            m->unk34 = 0;
            cx = D_00186F40.x;
            if (odd) {
                m->x = cx + D_001E0408[id].x0;
            } else {
                m->x = cx + D_001E0408[id].x1;
            }
            m->y = D_00186F40.y + D_001E0408[id].y;
            m->z = D_00186F40.z + D_001E0408[id].z;
            m->unk40 = D_001E0408[id].u10;
            m->unk44 = D_001E0408[id].u14;
            m->unk48 = 3.1415927f;
            m->update = func_0021E698;
            vars = m->vars;
            vars->owner = obj;
            vars->id = id;
            n = m->anim->count - 1; if (anim < n) { n = anim; } func_00212ED8(m, n, 0);
        }
        if (is3 && m != 0) {
            m = func_00225490(D_001863D0[1].oclass);
            if (m != 0) {
                m->unk34 = 0;
                src = obj->moby;
                qcopy(&m->x, &src->x);
                qcopy(&m->unk40, &src->unk40);
                obj->moby2 = m;
                m->update = func_0021E698;
                vars = m->vars;
                vars->id = id;
                vars->owner = obj;
                func_00212ED8(m, (anim < m->anim->count - 1) ? anim : m->anim->count - 1, 0);
            }
        }
        obj->state = 3;
        break;
    case 3:
        if (prev != oclass) {
            obj->moby = func_00225530(obj->moby);
            obj->moby2 = func_00225530(obj->moby2);
            obj->state = 2;
        }
        break;
    }
    return 0;
}
#endif /* NON_MATCHING */
