/* Shared moby classes: veldin_ship, decoy_glove, metal_detector, helipack, horny_toad, infobot, particle_spawner_smoke__fire. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/hero.h"

#include "qcopy.h"

typedef unsigned int Q_2cfcb8 __attribute__((mode(TI)));
typedef struct { f32 x, y, z, w; } __attribute__((aligned(16))) V_2cfcb8;

struct Vars_2cfcb8;
typedef struct M_2cfcb8 {
    u8 p0[0x10];
    V_2cfcb8 pos;
    u8 state;
    u8 group;
    u8 p22[0x30 - 0x22];
    u8 update_dist;
    u8 p31[0x36 - 0x31];
    s16 occlusion;
    u8 p38[0x40 - 0x38];
    f32 rot_x;
    f32 rot_y;
    f32 rot_z;
    u8 p4c[0x78 - 0x4C];
    struct Vars_2cfcb8 *vars;
    u8 p7c[0xA6 - 0x7C];
    s16 oclass;
    u16 hA8;
    u8 pAA[0xB0 - 0xAA];
    u8 mission;
} M_2cfcb8;

typedef struct Vars_2cfcb8 {
    u8 p0[0xA0];
    M_2cfcb8 *next_member;
    M_2cfcb8 *pA4;
    u8 pA8[4];
    s32 flags;
    f32 height;
    u8 pB4[0xC0 - 0xB4];
    s32 route_index;
    u16 hC4;
    s16 hC6;
    u8 pC8[4];
    s32 iCC;
    V_2cfcb8 link_pos;
    V_2cfcb8 link_rot;
    M_2cfcb8 *standing_on;
    f32 stack_level;
    s32 iF8;
} Vars_2cfcb8;

typedef struct { s32 count; s32 pad_4[3]; struct { s32 pad_0[3]; s32 state; } entries[1]; } R_2cfcb8;
typedef struct { u8 p0[0x18]; M_2cfcb8 *moby; u8 p1c[0x28 - 0x1C]; f32 point_z; } H_2cfcb8;

extern s32 D_0014EE90_lui[] __asm__("D_0014EE90") __attribute__((section(".data")));
extern int D_L00_00173E40 __attribute__((section(".data")));
extern R_2cfcb8 *D_L00_001B04B0_2cfcb8[] __asm__("D_L00_001B04B0") __attribute__((section(".data")));

extern s32 FUN_00213260_2cfcb8(s32) __asm__("FUN_00213260");
extern f32 FUN_001fa580_2cfcb8(f32, f32) __asm__("FUN_001fa580");
extern void FUN_0020c828_2cfcb8(M_2cfcb8 *) __asm__("FUN_0020c828");
extern s32 FUN_001efa68_2cfcb8(V_2cfcb8 *, V_2cfcb8 *, s32, M_2cfcb8 *, s32) __asm__("FUN_001efa68");
extern s32 FUN_L00_0025e3b8_2cfcb8(M_2cfcb8 *) __asm__("FUN_L00_0025e3b8");
extern s32 FUN_L00_002603d0_2cfcb8(M_2cfcb8 *) __asm__("FUN_L00_002603d0");
extern int FUN_L00_00260668_2cfcb8(M_2cfcb8 *, M_2cfcb8 *, V_2cfcb8 *, f32 *, V_2cfcb8 *, V_2cfcb8 *) __asm__("FUN_L00_00260668");
extern int FUN_L00_002591d0_2cfcb8(M_2cfcb8 **, s32, s32, s32) __asm__("FUN_L00_002591d0");
extern int FUN_L00_002592b8_2cfcb8(M_2cfcb8 **, M_2cfcb8 *, s32, s32) __asm__("FUN_L00_002592b8");
extern void FUN_001f9a10_2cfcb8(V_2cfcb8 *, V_2cfcb8 *, void *) __asm__("FUN_001f9a10");
extern void FUN_001f97b0_2cfcb8(void) __asm__("FUN_001f97b0");
extern f32 FUN_001f9b48_2cfcb8(V_2cfcb8 *, V_2cfcb8 *) __asm__("FUN_001f9b48");

void FUN_L00_002cfcb8(M_2cfcb8 *moby) {
    V_2cfcb8 probe_top;
    V_2cfcb8 probe_bottom;
    V_2cfcb8 probe_up;
    V_2cfcb8 probe_down;
    M_2cfcb8 *member;
    M_2cfcb8 *other;
    Vars_2cfcb8 *vars;
    Vars_2cfcb8 *below_vars;
    V_2cfcb8 *src1, *src2, *src3, *src4;
    R_2cfcb8 *route;
    s32 group, flags_before, i, member_flags;
    M_2cfcb8 *below;

    vars = moby->vars;
    group = moby->group;
    vars->flags |= 4;
    if (group == 0xFF) {
        if (moby->oclass == 0x1F4 && FUN_00213260_2cfcb8(2) != 0 && moby->rot_x == 0.0f && moby->rot_y == 0.0f) {
            moby->rot_z = FUN_001fa580_2cfcb8(moby->rot_z, 1.5707964f);
        }
        if (moby->oclass == 0x1F5 && moby->mission != 0xFF && D_0014EE90_lui[moby->mission] < vars->iF8) {
            FUN_0020c828_2cfcb8(moby);
            return;
        }
        flags_before = vars->flags;
        vars->hC4 = moby->hA8;
        vars->height = 1.0f;
        vars->flags = flags_before | 1;
        src1 = &moby->pos;
        qcopy(&probe_top, src1);
        qcopy(&probe_bottom, src1);
        probe_top.z += 0.5f;
        probe_bottom.z = 0.1f;
        if (FUN_001efa68_2cfcb8(&probe_top, &probe_bottom, 2, 0, 0) == 0) {
            vars->pA4 = 0;
            return;
        }
        if ((*(H_2cfcb8 *)&D_L00_00173E40).moby == 0) {
            moby->pos.z = (*(H_2cfcb8 *)&D_L00_00173E40).point_z;
            vars->pA4 = 0;
            return;
        }
        if (FUN_L00_0025e3b8_2cfcb8((*(H_2cfcb8 *)&D_L00_00173E40).moby) != 0) return;
        vars->pA4 = 0;
        vars->flags |= 8;
        if (FUN_L00_002603d0_2cfcb8((*(H_2cfcb8 *)&D_L00_00173E40).moby) != 0) {
            if (moby != 0) moby->occlusion = 0x7F80;
            FUN_L00_00260668_2cfcb8(member, (*(H_2cfcb8 *)&D_L00_00173E40).moby, &moby->pos, &moby->rot_x, &vars->link_pos, &vars->link_rot);
            vars->stack_level = 0;
            vars->standing_on = (*(H_2cfcb8 *)&D_L00_00173E40).moby;
        }
        return;
    }

    FUN_L00_002591d0_2cfcb8(&moby, group, 0, 0);
    while (moby != 0) {
        vars = moby->vars;
        if (FUN_L00_0025e3b8_2cfcb8(moby) != 0) {
            if (moby->oclass == 0x1F4 && FUN_00213260_2cfcb8(2) != 0 && moby->rot_x == 0.0f && moby->rot_y == 0.0f) {
                moby->rot_z = FUN_001fa580_2cfcb8(moby->rot_z, 1.5707964f);
            }
            if (moby->oclass == 0x1F5 && moby->mission != 0xFF && D_0014EE90_lui[moby->mission] < vars->iF8) {
                FUN_0020c828_2cfcb8(moby);
            } else {
                member_flags = vars->flags;
                vars->hC4 = moby->hA8;
                vars->height = 1.0f;
                vars->flags = member_flags | 1;
                src2 = &moby->pos;
                qcopy(&probe_top, src2);
                qcopy(&probe_bottom, src2);
                probe_top.z += 0.5f;
                probe_bottom.z = 0.1f;
                if (FUN_001efa68_2cfcb8(&probe_top, &probe_bottom, 2, 0, 0) == 0) {
                    vars->pA4 = 0;
                } else {
                    if ((*(H_2cfcb8 *)&D_L00_00173E40).moby == 0) {
                        moby->pos.z = (*(H_2cfcb8 *)&D_L00_00173E40).point_z;
                        vars->pA4 = 0;
                    } else if (FUN_L00_0025e3b8_2cfcb8((*(H_2cfcb8 *)&D_L00_00173E40).moby) == 0 && (u16)((*(H_2cfcb8 *)&D_L00_00173E40).moby->oclass - 0x128) >= 2) {
                        vars->pA4 = 0;
                        moby->update_dist = 0xFF;
                        vars->flags |= 8;
                    } else if ((*(H_2cfcb8 *)&D_L00_00173E40).moby->vars == 0) {
                        vars->pA4 = 0;
                    } else if ((*(H_2cfcb8 *)&D_L00_00173E40).moby->group != moby->group) {
                        vars->pA4 = 0;
                    } else if (FUN_L00_0025e3b8_2cfcb8((*(H_2cfcb8 *)&D_L00_00173E40).moby) != 0) {
                        vars->pA4 = (*(H_2cfcb8 *)&D_L00_00173E40).moby;
                        src3 = &moby->pos;
                        below = (*(H_2cfcb8 *)&D_L00_00173E40).moby;
                        qcopy(src3, &below->pos);
                        FUN_001f9a10_2cfcb8(src3, src3, (u8 *)below + 0xE0);
                        vars = (*(H_2cfcb8 *)&D_L00_00173E40).moby->vars;
                        if (FUN_L00_0025e3b8_2cfcb8(moby) != 0) {
                            vars->next_member = moby;
                        } else {
                            FUN_001f97b0_2cfcb8();
                            vars->next_member = 0;
                        }
                    } else {
                        vars->pA4 = 0;
                        vars->iCC = 0;
                        moby->pos.z = (*(H_2cfcb8 *)&D_L00_00173E40).point_z;
                    }
                }
            }
        }
        FUN_L00_002592b8_2cfcb8(&moby, moby, 0, 0);
    }

    FUN_L00_002591d0_2cfcb8(&member, group, 0, 0);
    while (member != 0) {
        vars = member->vars;
        if (FUN_L00_0025e3b8_2cfcb8(member) != 0 && vars->pA4 == 0) {
            src4 = &member->pos;
            qcopy(&probe_up, src4);
            qcopy(&probe_down, src4);
            probe_up.z += vars->height * 0.5f;
            probe_down.z = 0.1f;
            if (FUN_001efa68_2cfcb8(&probe_up, &probe_down, 2, moby, 0) != 0) {
                        if ((*(H_2cfcb8 *)&D_L00_00173E40).moby != 0 && FUN_L00_002603d0_2cfcb8((*(H_2cfcb8 *)&D_L00_00173E40).moby) != 0) {
                    if (moby != 0) moby->occlusion = 0x7F80;
                    FUN_L00_00260668_2cfcb8(member, (*(H_2cfcb8 *)&D_L00_00173E40).moby, &member->pos, &member->rot_x, &vars->link_pos, &vars->link_rot);
                    vars->stack_level = 0;
                    vars->standing_on = (*(H_2cfcb8 *)&D_L00_00173E40).moby;
                    vars->flags |= 8;
                }
            }
            for (moby = member; moby != 0; moby = vars->next_member) {
                vars = moby->vars;
                below_vars = 0;
                if (vars->pA4 != 0) below_vars = vars->pA4->vars;
                if (below_vars != 0) {
                    vars->iCC = below_vars->iCC;
                    if (below_vars->standing_on != 0) {
                        moby->update_dist = 0xFF;
                        vars->flags |= 8;
                        qcopy(&vars->link_pos, &below_vars->link_pos);
                        qcopy(&vars->link_rot, &below_vars->link_rot);
                        vars->standing_on = below_vars->standing_on;
                        vars->stack_level = below_vars->stack_level + 1.0f;
                    }
                }
                if ((vars->hC6 != 0 || moby->oclass == 0x1F5) && vars->next_member != 0 && vars->route_index != -1) {
                    route = D_L00_001B04B0_2cfcb8[vars->route_index];
                    if (route->count == 0) {
                        vars->route_index = -1;
                    } else {
                        s32 entry_state = route->entries[0].state;
                        if (entry_state == 1 || entry_state == 3) {
                            vars->route_index = -1;
                        } else if (entry_state == 0x111) {
                            vars->route_index = -1;
                        } else if ((u32)(entry_state - 0x11) < 0x100) {
                            if (entry_state - 0x10 >= route->count) {
                                vars->route_index = -1;
                            } else {
                                route->entries[0].state = entry_state + 1;
                            }
                        } else {
                            route->entries[0].state = 0x11;
                            for (i = 0; i < D_L00_001B04B0_2cfcb8[vars->route_index]->count; i++) {
                                D_L00_001B04B0_2cfcb8[vars->route_index]->entries[i].state = 0;
                            }
                        }
                    }
                }
            }
        }
        FUN_L00_002592b8_2cfcb8(&member, member, 0, 0);
    }

    FUN_L00_002591d0_2cfcb8(&moby, group, 0, 0);
    while (moby != 0) {
        if (FUN_L00_0025e3b8_2cfcb8(moby) != 0) {
            FUN_L00_002592b8_2cfcb8(&other, moby, 0, 0);
            while (other != 0) {
                if (FUN_L00_0025e3b8_2cfcb8(other) != 0) {
                    FUN_001f9b48_2cfcb8(&moby->pos, &other->pos);
                }
                FUN_L00_002592b8_2cfcb8(&other, other, 0, 0);
            }
        }
        FUN_L00_002592b8_2cfcb8(&moby, moby, 0, 0);
    }
}
typedef struct {
    s32 v[7];
} T_2d0538;
typedef struct {
    f32 f[4];
} V_2d0538;
extern int D_0015ED84;
extern s32 D_L00_0015F598 __asm__("D_L00_0015F598");
extern volatile f32 D_0015ED6C_a[1] __asm__("D_0015ED6C");
#define D_0015ED6C_2d0538 D_0015ED6C_a[0]
extern float D_0015ED70;
extern u8 *D_L00_001B04B0_2d0538[] __asm__("D_L00_001B04B0") __attribute__((section(".data")));
void clear_u64_value(void *) __asm__("FUN_001f99f8");
void FUN_L00_00260400_2d0538(u8 *, s32, void *, void *, void *, void *) __asm__("FUN_L00_00260400");
void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
f32 absolute_float(f32) __asm__("FUN_001f99c0");
void mark_moby_for_removal_cf(u8 *, s32, s32) __asm__("FUN_L00_00257470");
s32 FUN_L00_00257218_2d0538(u8 *, s32 *) __asm__("FUN_L00_00257218");
f32 FUN_00213308_2d0538(void) __asm__("FUN_00213308");
f32 FUN_002132a8_2d0538(f32, f32) __asm__("FUN_002132a8");
f32 fast_cos(f32) __asm__("FUN_001f9dc8");
f32 fast_sin(f32) __asm__("FUN_001f9de0");
s32 FUN_L00_002595a0_2d0538(f32 *, f32 *, f32, f32, f32) __asm__("FUN_L00_002595a0");
void scale_vector_xyz(void *, void *, f32) __asm__("FUN_001f9a68");
void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
void FUN_L00_00261b48(s32, void *, void *, void *);
void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
u8 *FUN_L00_002c52a8_2d0538(u8 *, s32, s32, s32) __asm__("FUN_L00_002c52a8");

void crate_drop_bolts(u8 *a) __asm__("FUN_L00_002d0538");
extern T_2d0538 D_L00_001E9E30;
extern f32 D_L00_001618C8 __attribute__((sda));
extern f32 D_L00_001618CC __attribute__((sda));
extern f32 D_L00_001618D0 __attribute__((sda));
void crate_drop_bolts(u8 *a) {
    enum {
        CRATE_DROP_AUTO_SELECT = 0,
        CRATE_DROP_FALLBACK_ITEM = 1
    };
    T_2d0538 tmp;
    V_2d0538 trajectory_vector, adjusted_position, scratch_vector;
    s32 n;
    f32 f54, f58;
    s32 cnt, i;
    u8 *b, *p, *q;
    s16 h;
    f32 t, r, e;

    b = *(u8 **)(a + 0x78);
    h = *(s16 *)(a + 0xA6);
    tmp = D_L00_001E9E30;
    n = *(s16 *)(b + 0xC6) == 0;
    if (h == 0x1F5)
        n = 0;
    if (n) {
        clear_u64_value(&trajectory_vector);
        if (*(s32 *)(b + 0xF0)) {
            if (a)
                *(s16 *)(a + 0x36) = 0x7F80;
            FUN_L00_00260400_2d0538(a, *(s32 *)(b + 0xF0), a + 0x10, a + 0x40, &adjusted_position, &scratch_vector);
            fast_vec_sub(&trajectory_vector, &adjusted_position, a + 0x10);
        }
        if (D_0015ED84 == 0xF && absolute_float(*(f32 *)(a + 0x10) - 166.0f) < 6.0f &&
            absolute_float(*(f32 *)(a + 0x14) - 193.0f) < 6.0f) {
            s32 save = D_L00_0015F598;
            D_L00_0015F598 = 0;
            mark_moby_for_removal_cf(a, 0x100, -1);
            D_L00_0015F598 = save;
            return;
        }
        mark_moby_for_removal_cf(a, 0x100, *(s32 *)(b + 0xC0));
        return;
    }
    if (h != 0x1F5) {
        switch (*(u8 *)(b + 0xCB)) {
        case CRATE_DROP_FALLBACK_ITEM:
            n = 10;
            break;
        case 6:
            n = 11;
            break;
        case 8:
            n = 13;
            break;
        case 3:
            n = 15;
            break;
        case 2:
            n = 16;
            break;
        case 5:
            n = 17;
            break;
        case 10:
            n = 19;
            break;
        case 4:
            n = 20;
            break;
        case 11:
            n = 23;
            break;
        case 7:
            n = 24;
            break;
        case 9:
            n = 25;
            break;
        case CRATE_DROP_AUTO_SELECT:
        default:
            n = -1;
            break;
        }
        cnt = FUN_L00_00257218_2d0538(a, &n);
        for (i = 0; i < cnt; i++) {
            p = FUN_L00_002c52a8_2d0538(a + 0x10, n, -1, 0);
            if (!p)
                break;
            q = *(u8 **)(p + 0x78);
            *(s16 *)(q + 4) = *(s16 *)(b + 0xC4);
            t = FUN_00213308_2d0538();
            r = FUN_002132a8_2d0538(0.0f, D_L00_001618C8) * D_0015ED6C_2d0538;
            *(f32 *)(q + 0x10) = fast_cos(t) * r;
            {
                f32 sn = fast_sin(t) * r;
                {
                    f32 hi = D_L00_001618D0, lo = D_L00_001618CC;
                    *(f32 *)(q + 0x18) = 0.0f;
                    *(f32 *)(q + 0x14) = sn;
                    *(f32 *)(q + 0x18) = FUN_002132a8_2d0538(lo, hi) * D_0015ED6C_2d0538;
                }
            }
            if (*(s32 *)(b + 0xC0) >= 0) {
                s32 k = FUN_L00_002595a0_2d0538(
                    &f54, &f58, D_0015ED70 * 10.8f * -0.5f,
                    *(f32 *)(q + 0x18) - D_0015ED70 * 10.8f * -0.5f,
                    *(f32 *)(p + 0x18) -
                        *(f32 *)(D_L00_001B04B0_2d0538[*(s32 *)(b + 0xC0)] + 0x18));
                e = 120.0f;
                if ((f32)k >= 1.0f && f54 > 0.0f && f54 < 120.0f)
                    e = f54;
                scale_vector_xyz(&trajectory_vector, q + 0x10, e);
                add_vector_xyz(&trajectory_vector, &trajectory_vector, a + 0x10);
                FUN_L00_00261b48(*(s32 *)(b + 0xC0), a + 0x10, &trajectory_vector, &adjusted_position);
                fast_vec_sub(&trajectory_vector, &adjusted_position, p + 0x10);
                trajectory_vector.f[2] = 0.0f;
                normalize_vector_xyz(&scratch_vector, &trajectory_vector, 0.25f);
                fast_vec_sub(&trajectory_vector, &trajectory_vector, &scratch_vector);
                scale_vector_xyz(&trajectory_vector, &trajectory_vector, 1.0f / e);
                *(f32 *)(q + 0x10) = trajectory_vector.f[0];
                *(f32 *)(q + 0x14) = trajectory_vector.f[1];
            }
        }
    }
    if (*(s16 *)(a + 0xB4))
        *(s16 *)(a + 0xB4) = 0;
    mark_moby_for_removal_cf(a, 0x100, -1);
}
#include "qcopy.h"
typedef struct V2d1428 {
    f32 x, y, z, w;
} __attribute__((aligned(16))) V2d1428;
typedef struct M2d1428 M2d1428;
typedef struct O2d1428 {
    u8 p0[0x40];
    V2d1428 vel;
    u8 p50[0x10];
    u8 v60[0x40];
    struct M2d1428 *next;
    struct M2d1428 *parent;
    u8 pa8[4];
    s32 flags;
    f32 fB0;
    s32 iB4;
    u8 pb8[0x18];
    V2d1428 vD0;
    u8 vE0[0x10];
    void *path;
    f32 fF4;
} O2d1428;
struct M2d1428 {
    u8 p0[0x10];
    V2d1428 pos;
    u8 state;
    u8 p21[0x15];
    s16 h36;
    V2d1428 v40;
    u8 p50[0x28];
    O2d1428 *vars;
    u8 p7c[0x2A];
    s16 oclass;
    u8 pa8[0x14];
    u8 bBC;
    u8 pbd[3];
    u8 vC0[0x10];
};
typedef struct P2d1428 {
    u8 p0[0x3C];
    u32 flags;
} P2d1428;
extern s32 D_L00_0015F5CC;
extern f32 D_0015ED70_alt __asm__("FUN_0015ED70");
extern f32 D_0015ED6C_c __asm__("D_0015ED6C");
typedef struct G2d1428 {
    u8 p0[0x80];
    V2d1428 v80;
    u8 p90[0x190];
    f32 f220;
    u8 p224[0x10];
    f32 f234;
} G2d1428;
extern V2d1428 D_L00_00173E60;
s32 FUN_L00_0025e3b8(M2d1428 *);
void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
f32 FUN_001f99c0(f32);
f32 vector_length_xyz(void *) __asm__("FUN_001f9af0");
f32 approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
s32 coll_line_fix(void *, void *, s32, s32, s32) __asm__("FUN_001efa68");
void break_crate(M2d1428 *, s32, s32) __asm__("FUN_L00_002d09b8");
void FUN_L00_002d0538_c(M2d1428 *) __asm__("FUN_L00_002d0538");
void FUN_L00_002604f0(M2d1428 *, void *, void *, void *, void *, void *);
P2d1428 *FUN_L00_002603d0(void *);
void update_entity_matrix(void *, void *, void *, void *) __asm__("FUN_L00_00260738");

typedef union {
    f32 f[4];
    u128 q;
} __attribute__((aligned(16))) V2d09b8;
extern s32 D_L00_0015F5CC;
extern s32 D_L00_001618D4_2d09b8 __asm__("D_L00_001618D4");
extern s32 D_L00_001618D8_2d09b8 __asm__("D_L00_001618D8");
extern s32 D_L00_0015FFBC __asm__("D_L00_0015FFBC");
extern f32 D_0015ED6C_2d09b8 __asm__("D_0015ED6C");
extern f32 D_L00_0015F5D0 __asm__("D_L00_0015F5D0");
extern s32 D_001413DC_2d09b8 __asm__("D_001413DC") __attribute__((section(".data")));
extern int D_001413D0[] __attribute__((section(".data")));
extern f32 player_camera[] __asm__("D_L00_00166C80");
extern char camera_position[] __asm__("D_L00_00166DC0");
extern V2d09b8 D_L00_001DE6F0_2d09b8[] __asm__("D_L00_001DE6F0");
extern V2d09b8 D_L00_001DE750_2d09b8[] __asm__("D_L00_001DE750");
extern u32 D_L00_001E9E80_2d09b8[] __asm__("D_L00_001E9E80");
extern u32 D_L00_001E9E98_2d09b8[] __asm__("D_L00_001E9E98");
extern s32 absolute_int_2d09b8(s32) __asm__("FUN_001f99a8");
extern void FUN_0028dc90_2d09b8(s32, s32, void *, s32) __asm__("FUN_L00_0028dc90");
extern void FUN_0028dbe8_2d09b8(s32, s32, void *) __asm__("FUN_L00_0028dbe8");
extern s32 FUN_0025e3b8_2d09b8(void *) __asm__("FUN_L00_0025e3b8");
extern void FUN_001ff008_2d09b8(void) __asm__("FUN_001f97b0");
extern void FUN_0026b790_2d09b8(f32, f32, f32, f32, f32, void *, s32, s32) __asm__("FUN_L00_0026b790");
extern void FUN_001ff2a8_2d09b8(void *, void *, void *) __asm__("FUN_001f9a28");
extern f32 FUN_001ff8b0_2d09b8(f32, f32) __asm__("FUN_001f9e90");
extern f32 FUN_00200130_2d09b8(f32, f32) __asm__("FUN_001fa5c8");
extern s32 FUN_00200250_2d09b8(f32) __asm__("FUN_001fa6d0");
extern f32 FUN_00200178_2d09b8(f32) __asm__("FUN_001fa610");
extern void FUN_001ff260_2d09b8(void *) __asm__("FUN_001f99f8");
extern void FUN_001ffa50_2d09b8(void *, void *) __asm__("FUN_001fa030");
extern void FUN_001ff6d0_2d09b8(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void FUN_001ff278_2d09b8(void *, void *, void *) __asm__("FUN_001f9a10");
extern f32 FUN_002000e8_2d09b8(f32, f32) __asm__("FUN_001fa580");
extern f32 FUN_00257be8_2d09b8(f32, f32) __asm__("FUN_002132a8");
extern void FUN_001ff500_2d09b8(f32, void *, void *) __asm__("FUN_001f9bf8");
extern s32 FUN_00257b50_2d09b8(s32) __asm__("FUN_00213260");
extern void *FUN_002af8b0_2d09b8(void *, void *, void *, void *, s32, s32, f32, f32) __asm__("FUN_L00_002af8b0");
extern void FUN_00250df8_2d09b8(void *) __asm__("FUN_L00_00250df8");
extern void FUN_00257d78_2d09b8(f32, f32, void *) __asm__("FUN_L00_00257d78");
extern f32 FUN_00257cb0_2d09b8(void) __asm__("FUN_00213308");
extern u8 FUN_001fef20_2d09b8(s32) __asm__("FUN_001f96f8");
extern f32 FUN_001ff3d8_2d09b8(void *) __asm__("FUN_001f9af0");
extern s32 FUN_00257b90_2d09b8(s32, s32) __asm__("FUN_L00_00257b90");
extern void FUN_0026a9f0_2d09b8(void *, void *, s32, s32, s32, s32, s32, s32, f32, f32) __asm__("FUN_L00_0026a9f0");
extern void FUN_002ac910_2d09b8(void *, void *, void *, f32, s32, s32, s32, s32, s32) __asm__("FUN_L00_002ac910");

void break_crate(M2d1428 *moby, s32 hit, s32 source) __asm__("FUN_L00_002d09b8");
void break_crate(M2d1428 *moby, s32 hit, s32 source) {
    enum {
        CRATE_VARIANT_OPTIONAL_QUARTER_TURN = 0,
        CRATE_VARIANT_MISSION_THRESHOLD_GATED = 1,
        CRATE_VARIANT_DAMAGE_FLAG_GATED = 2,
        CRATE_VARIANT_IMPACT_TRIGGERED = 5,
        CRATE_VARIANT_INITIAL_DAMAGE_STAGE = 7,
        CRATE_VARIANT_SECOND_DAMAGE_STAGE = 8,
        CRATE_VARIANT_PENULTIMATE_DAMAGE_STAGE = 9,
        CRATE_VARIANT_FINAL_DAMAGE_STAGE = 10,
        CRATE_VARIANT_CONDITIONAL_RESPAWN_START = 11
    };
    u8 *vars = (u8 *)moby->vars;
    V2d09b8 position, away, rotation, basis[3], launch, spawn_pos, spin;
    s32 kind;
    M2d1428 *self;
    s32 close, count, trail_count, particle, i, choice;
    s32 first, second, first_color, second_color;
    f32 angle, distance, height, speed, rise;
    u32 colors[6], other_colors[6];
    void *spawned;
    u32 *effect, *velocity;

    close = 0;
    if (absolute_int_2d09b8(D_L00_0015F5CC - D_L00_001618D4_2d09b8) > 1) {
        if (moby->oclass == 0x1FA)
            FUN_0028dc90_2d09b8(0, 0, moby, 500);
        else
            FUN_0028dbe8_2d09b8(0, 0, moby);
        D_L00_001618D4_2d09b8 = D_L00_0015F5CC;
    }
    if (absolute_int_2d09b8(D_L00_0015F5CC - D_L00_001618D8_2d09b8) >= 2)
        D_L00_001618D8_2d09b8 = D_L00_0015F5CC;
    else
        close = 1;
    if (*(u8 **)(vars + 0xA0)) {
        *(u8 **)(*(u8 **)(*(u8 **)(vars + 0xA0) + 0x78) + 0xA4) = *(u8 **)(vars + 0xA4);
        *(u32 *)(*(u8 **)(*(u8 **)(vars + 0xA0) + 0x78) + 0xAC) |= 4;
    }
    if (*(u8 **)(vars + 0xA4)) {
        if (!FUN_0025e3b8_2d09b8(*(void **)(*(u8 **)(*(u8 **)(vars + 0xA4) + 0x78) + 0xA0)))
            FUN_001ff008_2d09b8();
        *(u8 **)(*(u8 **)(*(u8 **)(vars + 0xA4) + 0x78) + 0xA0) = *(u8 **)(vars + 0xA0);
        *(u32 *)(*(u8 **)(*(u8 **)(vars + 0xA4) + 0x78) + 0xAC) |= 4;
    }
    /* Fakematch: the self copy and the two empty tests on angle give retail's spill slots. */
    self = moby;
    {
        s32 i;
        qcopy(&position, &moby->pos);
        position.f[2] += 0.3f;
        FUN_0026b790_2d09b8(0.3f, 1.01f, 1.075f, 0.05f, 50000.0f, &position, 1, 0x40808080);
        for (i = 4; i >= 0; i--)
            FUN_0026b790_2d09b8(0.3f, 1.01f, 1.07f, 0.05f, 50000.0f, &position, 1, 0x40808080);
        if (source)
            qcopy(&away, (u8 *)source + 0x10);
        else
            FUN_001ff2a8_2d09b8(&away, &moby->pos, (*(u8 * *)D_001413D0) + 0x10);
        switch ((s16)((u16)moby->oclass - 500)) {
        case CRATE_VARIANT_MISSION_THRESHOLD_GATED: kind = 0x15C; break;
        case 3: kind = 0x15F; break;
        case CRATE_VARIANT_DAMAGE_FLAG_GATED:
            if (!angle) {
            }
            kind = 0x162;
            break;
        case CRATE_VARIANT_OPTIONAL_QUARTER_TURN: case CRATE_VARIANT_CONDITIONAL_RESPAWN_START: kind = 0x165; break;
        case 4: kind = 0x168; break;
        case CRATE_VARIANT_IMPACT_TRIGGERED: kind = 0x16B; break;
        case 6: kind = 0x16E; break;
        case CRATE_VARIANT_INITIAL_DAMAGE_STAGE: case CRATE_VARIANT_SECOND_DAMAGE_STAGE: case CRATE_VARIANT_PENULTIMATE_DAMAGE_STAGE: case CRATE_VARIANT_FINAL_DAMAGE_STAGE: kind = 0x171; break;
        default: kind = 0x95; break;
        }
        if (D_L00_0015FFBC >= 201) { count = 4; trail_count = 4; }
        else if (D_L00_0015FFBC >= 151) { count = 2; trail_count = 2; }
        else if (D_L00_0015FFBC >= 101) { count = 2; trail_count = 1; }
        else {
            if (!angle) {
            }
            if (D_L00_0015FFBC >= 51) { count = 1; trail_count = 0; }
            else { count = 1; trail_count = 0; }
        }
        angle = player_camera[80] - moby->pos.x;
        speed = player_camera[81] - moby->pos.y;
        angle = FUN_001ff8b0_2d09b8(angle, speed);
        angle = FUN_00200130_2d09b8(angle, moby->v40.z);
        angle += 3.1415925f;
        choice = FUN_00200250_2d09b8(angle / 1.57f);
        angle = FUN_00200178_2d09b8((f32)choice * 1.57f);
        FUN_001ff260_2d09b8(&rotation);
        rotation.f[2] = angle;
        FUN_001ffa50_2d09b8(&basis, &rotation);
        for (i = 0; i < count; ++i) {
            FUN_001ff6d0_2d09b8(&spawn_pos, &D_L00_001DE6F0_2d09b8[i], &basis);
            FUN_001ff278_2d09b8(&spawn_pos, &spawn_pos, &moby->pos);
            qcopy(&spin, &D_L00_001DE750_2d09b8[i]);
            spin.f[2] = FUN_002000e8_2d09b8(spin.f[2], rotation.f[2]);
            FUN_001ff2a8_2d09b8(&launch, &spawn_pos, &moby->pos);
            launch.f[2] = 0.0f;
            speed = FUN_00257be8_2d09b8(1.0f, 5.0f);
            FUN_001ff500_2d09b8(speed * D_0015ED6C_2d09b8, &launch, &launch);
            launch.f[2] = FUN_00257be8_2d09b8(4.0f, 9.0f) * D_0015ED6C_2d09b8;
            particle = FUN_00257b50_2d09b8(2);
            spawned = FUN_002af8b0_2d09b8(moby, &launch, &spawn_pos, &spin, kind, particle, 1.0f, 1.0f);
            if (spawned) {
                qcopy((u8 *)spawned + 0x40, &D_L00_001DE750_2d09b8[i]);
                *(f32 *)((u8 *)spawned + 0x48) = FUN_002000e8_2d09b8(*(f32 *)((u8 *)spawned + 0x48), rotation.f[2]);
                FUN_00250df8_2d09b8(spawned);
            }
        }
        for (i = 0; i < trail_count; i++) {
            spawn_pos.f[0] = FUN_00257be8_2d09b8(-0.5f, 0.5f);
            spawn_pos.f[1] = FUN_00257be8_2d09b8(-0.5f, 0.5f);
            spawn_pos.f[2] = FUN_00257be8_2d09b8(0.0f, 1.0f);
            FUN_001ff278_2d09b8(&spawn_pos, &spawn_pos, &moby->pos);
            speed = FUN_00257be8_2d09b8(6.0f, 12.0f) * D_0015ED6C_2d09b8;
            FUN_00257d78_2d09b8(speed, speed, &launch);
            if (launch.f[2] < 0.0f) launch.f[2] = -launch.f[2];
            spin.f[0] = FUN_00257cb0_2d09b8();
            spin.f[2] = FUN_00257cb0_2d09b8();
            particle = 1;
            if (FUN_00257b50_2d09b8(6) == 0)
                particle = FUN_00200250_2d09b8(FUN_00257be8_2d09b8(3.0f, 6.0f));
            FUN_002af8b0_2d09b8(moby, &launch, &spawn_pos, &spin, kind + 1, particle, 0.25f, 0.5f);
        }
    }
    *(u32 *)((u8 *)self + 0x94) = 0;
    moby->state = 3;
    *(u16 *)((u8 *)moby + 0x34) |= 1;
    moby->bBC = FUN_001fef20_2d09b8(15);
    if (hit) {
        if (D_001413DC_2d09b8 == 22)
            vars[0xCA] = FUN_001fef20_2d09b8(5);
        else
            vars[0xCA] = 1;
        FUN_001ff500_2d09b8(0.5f, &launch, (u8 *)moby + 0xE0);
        FUN_001ff278_2d09b8(&launch, &launch, &moby->pos);
        FUN_001ff2a8_2d09b8(&spawn_pos, &(*(V2d09b8 *)camera_position), &moby->pos);
        FUN_001ff260_2d09b8(&spin);
        distance = FUN_001ff3d8_2d09b8(&spawn_pos);
        choice = 10;
        if (distance < 8.0f) choice = FUN_00200250_2d09b8(distance) + 2;
        height = 0.0f;
        if (distance < 7.0f) height = 7.0f - distance;
        if (close && choice > 6) choice = 6;
        for (i = 0; i < choice; i++) {
            speed = FUN_00257be8_2d09b8(8.0f, 10.0f);
            __builtin_memcpy(colors, D_L00_001E9E80_2d09b8, sizeof(colors));
            __builtin_memcpy(other_colors, D_L00_001E9E98_2d09b8, sizeof(other_colors));
            rise = speed * D_0015ED6C_2d09b8 - height * D_0015ED6C_2d09b8;
            effect = colors + FUN_00257b50_2d09b8(6);
            velocity = other_colors + FUN_00257b50_2d09b8(6);
            first = FUN_001fef20_2d09b8(15);
            second = FUN_001fef20_2d09b8(20);
            first_color = FUN_00257b90_2d09b8(first, second);
            first = FUN_001fef20_2d09b8(25);
            second = FUN_001fef20_2d09b8(30);
            second_color = FUN_00257b90_2d09b8(first, second);
            FUN_0026a9f0_2d09b8(&launch, &spin, *effect, *velocity, first_color, second_color, 0, 0, 400000.0f, rise);
        }
        if (D_L00_0015F5D0 < 0.95f && distance > 9.0f && !close) {
            FUN_002ac910_2d09b8(moby, &launch, &spin, 4.0f, FUN_001fef20_2d09b8(15), 127, 127, 127, 32);
            FUN_002ac910_2d09b8(moby, &launch, &spin, 4.0f, FUN_001fef20_2d09b8(24), 127, 32, 0, 32);
            FUN_002ac910_2d09b8(moby, &launch, &spin, 4.0f, FUN_001fef20_2d09b8(20), 127, 64, 0, 48);
        }
        FUN_002ac910_2d09b8(moby, &launch, &spin, 3.5f, FUN_001fef20_2d09b8(27), 96, 16, 0, 64);
        FUN_002ac910_2d09b8(moby, &launch, &spin, 3.0f, FUN_001fef20_2d09b8(29), 32, 0, 0, 32);
    }
}
void FUN_L00_002d1428(M2d1428 *moby) {
    enum {
        CRATE_STATE_BREAKABLE = 1,
        CRATE_STATE_IMPACT_TRIGGERED = 5
    };
    O2d1428 *vars;
    M2d1428 *p, *parent_moby, *next;
    V2d1428 collision_start, g, c2, hero_local_offset, b, previous_position, previous_rotation, position_delta;
    f32 f, v;
    s32 hit;

    vars = moby->vars;
    if (vars->iB4 == D_L00_0015F5CC)
        return;
    p = moby;
    do {
        next = p;
        p = next->vars->parent;
    } while (p != 0);
    p = next;
    do {
        moby = p;
        vars = moby->vars;
        p = vars->next;
        if (!(vars->flags & 4))
            vars->flags &= ~0x10;
        vars->flags &= ~4;
        if (!FUN_L00_0025e3b8(moby))
            return;
        if (vars->flags & 2)
            return;
        vars->iB4 = D_L00_0015F5CC;
        if (moby->state == 0)
            continue;
        fast_vec_sub(&hero_local_offset, &hero.motion.pos, &moby->pos);
        transform_vector_by_basis(&hero_local_offset, &hero_local_offset, moby->vC0);
        if (FUN_001f99c0(hero_local_offset.x) < 0.55f && FUN_001f99c0(hero_local_offset.y) < 0.55f && hero_local_offset.z < 0.0f &&
            -(hero.capsule_top + hero.unk234) < hero_local_offset.z) {
            switch (moby->state) {
            case CRATE_STATE_BREAKABLE:
            case CRATE_STATE_IMPACT_TRIGGERED:
                break_crate(moby, moby->oclass == 0x1F9, 0);
                FUN_L00_002d0538_c(moby);
                break;
            }
        }
        f = D_0015ED70_alt;
        vars->vel.z -= f * 9.8f;
        vars->vel.z = (vars->vel.z < -0.3f) ? -0.3f : vars->vel.z;
        vars->vel.z = (0.3f < vars->vel.z) ? 0.3f : vars->vel.z;
        if (vars->path) {
            if (vars->parent == 0) {
                vars->vel.w -= f * 10.0f;
                if (approach_value(&vars->fF4, 0.0f, FUN_001f99c0(vars->vel.w)) == 0.0f)
                    vars->vel.w = 0.0f;
            } else {
                vars->vel.w -= f * 10.0f;
                if (approach_value(&vars->fF4, vars->parent->vars->fF4 + 1.0f, FUN_001f99c0(vars->vel.w)) ==
                    0.0f)
                    *(s32 *)&vars->vel.w = 0;
            }
            qcopy(&b, &vars->vD0);
            b.z += vars->fF4;
            if (moby)
                moby->h36 = 0x7F80;
            qcopy(&previous_position, &moby->pos);
            qcopy(&previous_rotation, &moby->v40);
            FUN_L00_002604f0(moby, vars->path, &b, vars->vE0, &moby->pos, &moby->v40);
            if (FUN_L00_002603d0(vars->path)->flags & 4) {
                qcopy(&vars->vD0, &b);
                vars->vD0.z -= vars->fF4;
            }
            fast_vec_sub(&position_delta, &moby->pos, &previous_position);
            position_delta.w = vars->vel.w;
            qcopy(&vars->vel, &position_delta);
            update_entity_matrix(vars->v60, &vars->vel, &previous_rotation, &moby->v40);
            continue;
        }
        if (vars->parent == 0) {
            qcopy(&collision_start, &moby->pos);
            qcopy(&g, &moby->pos);
            collision_start.z += vars->fB0 * 0.75f;
            g.z += vars->vel.z;
            hit = coll_line_fix(&collision_start, &g, 2, 0, 0);
            qcopy(&c2, &D_L00_00173E60);
        } else {
            qcopy(&c2, &vars->parent->pos);
            c2.z += 1.0f;
            hit = 0;
            if (moby->pos.z + vars->vel.z < c2.z)
                hit = 1;
        }
        if (hit != 0) {
            f = vector_length_xyz(&vars->vel);
            if (D_0015ED6C_c * 0.5f < f) {
                if (moby->oclass == 0x1F9 && moby->state == 1) {
                    moby->bBC = 0;
                    moby->state = 5;
                } else if ((parent_moby = vars->parent) != 0 && parent_moby->oclass == 0x1F9 && parent_moby->state == 1) {
                    parent_moby->state = 5;
                    vars->parent->bBC = 0;
                }
            }
            moby->pos.z = c2.z;
            vars->vel.z = 0.0f;
        } else {
            vars->flags |= 4;
            fast_vec_add(&moby->pos, &moby->pos, &vars->vel);
        }
    } while (p != 0);
}
#include "rnc/gameplay/entities/moby.h"


/* Ported from rac1-decomp (src/overlays/shared/vendor_002D1168.c: func_L00_002D2E60), where it is exact; names translated to the US level program. */

typedef struct {
    u128 points[4];
    u32 colors[4];
    struct { f32 u, v; } uv[4];
    u64 texture[4];
} __attribute__((aligned(16))) Quad_2d19b0;

typedef struct {
    f32 x, y, z, w;
} __attribute__((aligned(16))) Vec_2d19b0;

extern Vec_2d19b0 ship_normals[] __asm__("D_L00_001DF9A0");
extern Vec_2d19b0 ship_world_vertices[] __asm__("D_L00_001E04A0");
extern Vec_2d19b0 ship_vertices[] __asm__("D_L00_001DE7B0");
extern f32 ship_reflection_uvs[][2] __asm__("D_L00_001E0B00");
extern f32 ship_previous_uvs[][2] __asm__("D_L00_001E0E30");
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern f32 fast_vec_dot(void *a, void *b) __asm__("func_001F9AB0");
extern f32 square_root(f32) __asm__("func_001F9988");
extern s16 ship_quad_indices[][4][3] __asm__("D_L00_001DEE10");
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
extern int fast_dec_timer(void *) __asm__("FUN_001f9740");
extern s64 get_effect_texture(s32) __asm__("func_001F44B8");
extern short D_L00_001618E8 __attribute__((sda));
extern short D_L00_001618E0 __attribute__((sda));
extern short D_L00_001618EC __attribute__((sda));
extern short D_L00_001618F0 __attribute__((sda));
extern short D_L00_001618F4 __attribute__((sda));
extern short D_L00_001618DC __attribute__((sda));
extern short D_L00_001618E4 __attribute__((sda));
extern void fast_draw_quad_real(void *, int, int) __asm__("func_001F7D30");
extern void fast_vec_normalize(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void fast_vec_scale(void *, void *, f32) __asm__("func_001F9A68");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void transform_vector(void *, void *, void *) __asm__("func_001F9D20");
float absolute_float(float input) __asm__("func_001F99C0");

#define G_DC (*(s32 *)&D_L00_001618DC)
#define G_E0 (*(s32 *)&D_L00_001618E0)
#define G_E4 (*(s32 *)&D_L00_001618E4)
#define G_E8 (*(s32 *)&D_L00_001618E8)
#define G_EC (*(s32 *)&D_L00_001618EC)
#define G_F0 (*(s32 *)&D_L00_001618F0)
#define G_F4 (*(s32 *)&D_L00_001618F4)
#define XFORM(a, b, c) transform_vector((Vec4f *)(a), (Vec4f *)(b), (c))

#include "rnc/rendering/debug/debug_menu.h"

void veldin_ship_draw(u8 *moby) __asm__("FUN_L00_002d19b0");
void veldin_ship_draw(u8 *moby) {
    Quad_2d19b0 quad;
    Vec_2d19b0 normal, reflection, view;
    u8 *vars = *(u8 **)(moby + 0x78);
    s32 i, j, index, active = 0;
    f32 blend, len, x, y;

    quad.texture[1] = get_effect_texture(G_F0);
    quad.texture[2] = 0xff9000000260ULL;
    quad.texture[3] = (u64)(s64)G_DC | ((u64)(s64)G_E0 << 2) |
                      ((u64)(s64)G_E4 << 4) | ((u64)(s64)G_E8 << 6) |
                      ((u64)(s64)G_EC << 32);
    quad.texture[0] = 0;
    quad.colors[0] = quad.colors[1] = quad.colors[2] = quad.colors[3] = G_F4;
    if (absolute_float(player_camera[80] - *(f32 *)(moby + 0x10)) < 16.0f &&
        absolute_float(player_camera[81] - *(f32 *)(moby + 0x14)) < 16.0f)
        active = 1;
    if (active || moby[0x20] == 1) {
        moby[0xbc] = 1;
        fast_dec_timer((s32 *)(vars + 0x40));
        blend = convert_integer_to_float(*(s32 *)(vars + 0x40));
        blend /= convert_integer_to_float(scale_game_frames(0x3c));
        for (i = 0; i < 0x66; i++) {
            XFORM(&ship_world_vertices[i], &ship_vertices[i], vars);
            fast_vec_sub(&view, &ship_world_vertices[i], g_debug_camera.position);
            fast_vec_normalize(&view, &view, 1.0f);
            XFORM(&normal, &ship_normals[i], vars);
            fast_vec_normalize(&normal, &normal, 0.1f);
            len = fast_vec_dot(&normal, &view);
            fast_vec_scale(&reflection, &normal, len + len);
            fast_vec_sub(&reflection, &view, &reflection);
            fast_vec_normalize(&reflection, &reflection, 1.0f);
            reflection.z += 1.0f;
            len = square_root(reflection.z + reflection.z);
            len += len;
            if (moby[0x20] == 1 || *(s32 *)(vars + 0x40) == 0) {
                ship_reflection_uvs[i][0] = reflection.x / len + 0.5f;
                ship_reflection_uvs[i][1] = reflection.y / len + 0.5f;
            } else {
                x = reflection.x / len + 0.5f;
                y = reflection.y / len;
                ship_reflection_uvs[i][0] = x + (ship_previous_uvs[i][0] - x) * blend;
                x = y + 0.5f;
                ship_reflection_uvs[i][1] = x + (ship_previous_uvs[i][1] - x) * blend;
            }
        }
        if (moby[0x20] == 1)
            moby[0x20] = 2;
    } else {
        if (moby[0xbc] == 1) {
            moby[0xbc] = 0;
            for (i = 0; i < 0x66; i++) {
                ship_previous_uvs[i][0] = ship_reflection_uvs[i][0];
                ship_previous_uvs[i][1] = ship_reflection_uvs[i][1];
                XFORM(&ship_world_vertices[i], &ship_vertices[i], vars);
            }
        } else {
            for (i = 0; i < 0x66; i++)
                XFORM(&ship_world_vertices[i], &ship_vertices[i], vars);
        }
        *(s32 *)(vars + 0x40) = scale_game_frames(0x3c);
    }
    for (i = 0; i < 0x4a; i++) {
        for (j = 0; j < 4; j++) {
            index = ship_quad_indices[i][j][0];
            qcopy(&quad.points[j], &ship_world_vertices[index]);
            quad.uv[j].u = ship_reflection_uvs[index][0];
            quad.uv[j].v = ship_reflection_uvs[index][1];
        }
        fast_draw_quad_real(&quad, 0, 0);
    }
}
typedef struct M2d1e80 {
    u8 p0[0x20];
    u8 state;
    u8 p21[0x10];
    u8 vis;
    u8 p32[2];
    u16 flags;
    u8 p36[0x42];
    s32 *vars;
    u8 p7c[0x40];
    u8 bc;
} M2d1e80;
extern s32 D_L00_0015F5C4;
extern s32 D_L00_0016C890[];
extern void build_moby_bone_transform(void *, int, void *) __asm__("FUN_0020cca8");
void enqueue_callback_list_1(void *, void *) __asm__("FUN_001f4600");
void veldin_ship_update(M2d1e80 *m) __asm__("FUN_L00_002d1e80");
void veldin_ship_update(M2d1e80 *moby) {
    s32 *v = moby->vars;
    if (moby->state == 0) {
        moby->state = 1;
        moby->bc = 1;
        v[0x10] = 0;
    }
    if (D_L00_0015F5C4 == 2 && (D_L00_0016C890[0] < 3 || D_L00_0016C890[0] == 4)) {
        moby->vis = 0;
        moby->flags |= 1;
    } else {
        moby->vis = 1;
        moby->flags &= 0xFFFE;
    }
    build_moby_bone_transform(moby, 0, v);
    if (moby->vis)
        enqueue_callback_list_1(veldin_ship_draw, moby);
}
#include "rnc/input/pad_state.h"
#include "rnc/gameplay/entities/moby.h"

/* Same camera step as func_L00_002C88A8, writing the following vector in the target data. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D1168.c: func_L00_002D3608), where it is exact; names translated to the US level program. */

typedef struct Hero Hero;

typedef struct PadState PadState;

typedef int u128_2D3608 __attribute__((mode(TI)));

typedef struct Moby_2D3608 {
    char pad0[0x48];
    float yaw;
    char pad4C[4];
    struct Moby_2D3608 *target;
    char pad54[0x24];
    float *data;
} Moby_2D3608;

typedef struct Hit_2D3608 {
    char pad0[0x18];
    void *moby;
    int pad1C;
    float point[4];
} Hit_2D3608;

typedef float Vec_2D3608[4] __attribute__((aligned(16)));

typedef float Mat_2D3608[4][4] __attribute__((aligned(16)));

typedef float Mat3_2D3608[3][4] __attribute__((aligned(16)));

extern unsigned char D_001413D0_r[] __asm__("D_001413D0");
extern char D_0013F990[];
extern char camera_position[] __asm__("D_L00_00166DC0");
extern f32 fast_add_rotations_c(f32, f32) __asm__("func_001FA580");
extern f32 fast_cos_c(f32) __asm__("func_001F9DC8");
extern f32 fast_sin_c(f32) __asm__("func_001F9DE0");
extern f32 fast_subtract_rotations_c(f32, f32) __asm__("func_001FA5C8");
extern f32 square_root(f32) __asm__("func_001F9988");
extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");
extern float D_0015ED70;
extern float FUN_001f9e90_c(float, float) __asm__("FUN_001f9e90");
extern int D_0013C940;
extern int D_L00_00166C80;
extern int D_L00_00173E40;
extern int FUN_001efa68_c(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern int FUN_001fa2d8_c() __asm__("FUN_001fa2d8");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void build_spherical_offset_c(void *, f32, f32, f32) __asm__("func_00214DB0");
extern void copy_matrix3x4(void *, void *) __asm__("func_001FA2B8");
extern void scale_vector_xyz_c(void *, void *, float) __asm__("FUN_001f9a68");
extern void transform_vector(void *, void *, void *) __asm__("func_001F9D20");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("func_001F9CF8");
void FUN_L00_002c7320(void *pos, void *tgt, f32 *out, f32 speed, f32 unused, f32 range);
extern char player_camera_2D3608[] __asm__("D_L00_00166C80");
extern char camera_position[] __asm__("D_L00_00166DC0");
extern int D_L00_00173E40;
extern char D_0013F990_2D3608[] __asm__("D_0013F990");
extern char controller_state_2D3608[] __asm__("D_0013C940");
extern float D_0015ED70;
extern void func_001FA480_2D3608(void *, void *) __asm__("FUN_001fa2b8");
extern void func_001FA4A0_2D3608(void *, void *) __asm__("FUN_001fa2d8");
extern void func_001F9EC0_2D3608(void *, void *, void *) __asm__("FUN_001f9cf8");
extern float func_L00_001FF860_2D3608(float, float) __asm__("FUN_001f9e90");
extern float func_001FA748_2D3608(float, float) __asm__("FUN_001fa580");
extern float func_001FA790_2D3608(float, float) __asm__("FUN_001fa5c8");
extern float func_001F9F90_2D3608(float) __asm__("FUN_001f9dc8");
extern float func_001F9FA8_2D3608(float) __asm__("FUN_001f9de0");
extern float func_001F9B50_2D3608(float) __asm__("FUN_001f9988");
extern void func_001F9BD8_2D3608(void *, void *, void *) __asm__("FUN_001f9a10");
extern void func_001F9C30_2D3608(void *, void *, float) __asm__("FUN_001f9a68");
extern void func_001F9EE8_2D3608(void *, void *, void *) __asm__("FUN_001f9d20");
extern float func_001F9CE8_2D3608(void *) __asm__("FUN_001f9b20");
extern void func_00215C00_2D3608(void *, float, float, float) __asm__("FUN_00214db0");
extern int func_L00_001EFFF0_2D3608(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern void func_L00_002C8680_2D3608(float *pos, float *tgt, float *output, float speed, float unused, float range) __asm__("FUN_L00_002c7320");

void FUN_L00_002d2158(int collision_arg, Moby_2D3608 *carrier, float *position)
{
    Vec_2D3608 anchor;
    Mat3_2D3608 rotation;
    Mat_2D3608 inverse;
    Vec_2D3608 direction, aim, launch, origin_copy, endpoint;
    Moby_2D3608 *target;
    float *output;
    float value, distance, heading, vertical, pitch, length, fall_time;
    int hit;
    float range;
    float *anchor_ptr = anchor;

    *(u128_2D3608 *)anchor_ptr = *(u128_2D3608 *)position;
    func_001FA480_2D3608(rotation, player_camera_2D3608);
    func_001FA4A0_2D3608(inverse, rotation);
    target = carrier->target;
    if (target == 0) return;
    output = target->data;
    *(u128_2D3608 *)direction = 0;
    direction[0] = 1.0f;
    func_001F9EC0_2D3608(direction, direction, D_0013F990_2D3608);
    {
        Hero *hero = (Hero *)(D_0013F990_2D3608 - 0x640);
        anchor_ptr[0] = func_001F9F90_2D3608(func_001FA748_2D3608(-0.361968011f, func_L00_001FF860_2D3608(hero->unk670, hero->unk674))) * 0.859039009f;
        anchor_ptr[1] = func_001F9FA8_2D3608(func_001FA748_2D3608(-0.361968011f, func_L00_001FF860_2D3608(hero->unk670, hero->unk674))) * 0.859039009f;
        anchor_ptr[2] = 0.0f;
        func_001F9BD8_2D3608(anchor_ptr, anchor_ptr, D_0013F990_2D3608 - 0x5C0);
        anchor_ptr[2] = anchor_ptr[2] + 0.390819997f;
        func_L00_001FF860_2D3608(direction[0], direction[1]);
        range = (((PadState *)controller_state_2D3608)->held & 5) ? 3.5f : 2.5f;
        func_001F9C30_2D3608(aim, direction, range);
        func_001F9BD8_2D3608(aim, aim, anchor_ptr);
        if ((((PadState *)controller_state_2D3608)->held & 5) && hero->unk20AC == 0) {
            *(u128_2D3608 *)launch = 0;
            launch[2] = 1.0f;
            func_001F9EE8_2D3608(launch, launch, inverse);
            hit = 0;
            func_001FA790_2D3608(((Moby_2D3608 *)hero->moby)->yaw, func_L00_001FF860_2D3608(launch[0], launch[1]));
            heading = func_L00_001FF860_2D3608(launch[0], launch[1]);
            pitch = func_L00_001FF860_2D3608(func_001F9CE8_2D3608(launch), launch[2]);
            if (0.593411922f < pitch) pitch = 0.593411922f;
            if (pitch < -1.39626336f) pitch = -1.39626336f;
            func_00215C00_2D3608(launch, (((PadState *)controller_state_2D3608)->held & 5) ? 3.5f : 2.5f, heading, pitch);
            qcopy(origin_copy, player_camera_2D3608 + 0x140);
            length = func_001F9CE8_2D3608(launch);
            {
                float rise = launch[2];
                if (length != 0.0f) {
                    value = rise / length;
                } else {
                    value = rise / 0.00999999978f;
                }
            }
            fall_time = func_001F9B50_2D3608(D_0015ED70 * 9.0f * ((((PadState *)controller_state_2D3608)->held & 5) ? 3.5f : 2.5f) * 0.5f);
            distance = (fall_time + fall_time) * fall_time * (1.0f - value) / (D_0015ED70 * 9.0f);
            vertical = value * distance;
            if (0.0f < distance) {
                launch[0] = func_001F9F90_2D3608(heading) * distance;
                launch[1] = func_001F9FA8_2D3608(heading) * distance;
                launch[2] = vertical;
                func_001F9BD8_2D3608(endpoint, camera_position, launch);
                if (func_L00_001EFFF0_2D3608(camera_position, endpoint, 2, collision_arg, 0)
                    && ((Hit_2D3608 *)((char *)&D_L00_00173E40))->moby != *(void **)(D_001413D0_r)) {
                    qcopy(aim, ((Hit_2D3608 *)((char *)&D_L00_00173E40))->point);
                    hit = 1;
                    heading = func_L00_001FF860_2D3608(aim[0] - anchor_ptr[0], aim[1] - anchor_ptr[1]);
                }
                if (hit == 0) {
                    value = 5.0f;
                    aim[0] = func_001F9F90_2D3608(heading) * distance * value;
                    aim[1] = func_001F9FA8_2D3608(heading) * distance * value;
                    aim[2] = vertical * value;
                    func_001F9BD8_2D3608(aim, aim, camera_position);
                }
            }
        } else {
            *(int *)(D_0013F990_2D3608 + 0x410) = 0;
        }
    }
    func_L00_002C8680_2D3608(anchor_ptr, aim, output + 4, D_0015ED70 * 9.0f, 0.00000999999975f,
                             (((PadState *)controller_state_2D3608)->held & 5) ? 3.5f : 2.5f);
}
#include "qzero.h"

/* Decoy glove update: state machine on moby[0x20], per-object data block at moby[0x78]. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D1168.c: func_L00_002D3B20), where it is exact; names translated to the US level program. */

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} Vec4f_2d2670;

typedef int u128_D3B20 __attribute__((mode(TI)));

extern char D_0013A4E0[];
extern char D_0013E533[];
extern unsigned char D_0014161B;
extern int D_0015ED84;
extern int D_0015EEA4;
extern int FUN_L00_00216de8(int, int);
extern int FUN_L00_00233db8(int, int);
extern int FUN_L00_00233e98(int);
extern int FUN_L00_002c35c0(void *, void *, void *);
extern int fast_dec_timer(void *) __asm__("FUN_001f9740");
extern s32 scale_game_frames_c2(s32) __asm__("func_001F96F8");
extern short D_L00_001B0830[];
extern void FUN_L00_002223f8(int, int);
extern void FUN_L00_0024f7c8(void *, int, void *);
extern void mine_spawn_drops(void *, void *) __asm__("FUN_L00_0025f800");
extern void FUN_L00_002c3888(float *, char *, void *);
extern int allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
extern void mark_moby_for_removal() __asm__("FUN_0020c828");
extern void transform_vector_alt() __asm__("FUN_001f9d20");
void FUN_L00_002d2158_c(int collision_arg, Moby_2D3608 *carrier, float *position) __asm__("FUN_L00_002d2158");
extern void build_moby_bone_transform(void *, int, void *) __asm__("FUN_0020cca8");
extern u8 D_0013E539_lui __asm__("D_0013E539") __attribute__((section(".data")));
extern char D_0013CAE4[];
#include "rnc/gameplay/state/game_globals.h"
extern void build_moby_bone_transform(void *, int, void *) __asm__("FUN_0020cca8");

void decoy_glove_update(unsigned char *moby) __asm__("FUN_L00_002d2670");
void decoy_glove_update(unsigned char *moby) {
    enum {
        DECOY_GLOVE_INITIALIZE = 0,
        DECOY_GLOVE_WAIT_FOR_MOBY_FLAG = 1,
        DECOY_GLOVE_TARGET_FOLLOW_AND_SPAWN = 2,
        DECOY_GLOVE_RELEASE_DECOY = 3,
        DECOY_GLOVE_WAIT_AFTER_RELEASE = 4,
        DECOY_GLOVE_ENTER_SKIP_TARGET_FOLLOW_AND_SPAWN = 5,
        DECOY_GLOVE_SKIP_TARGET_FOLLOW_AND_SPAWN = 6
    };
    extern void mark_moby_for_removal();
    extern void transform_vector_alt();
    extern int allocate_voice_for_target_entry(int, int, void *);
    char *glove_vars;
    char *tracked_target_moby;
    char *g;
    float vb[4];
    float va[4];
    float vc[4];
    float vd[4];
    float pad0[4];
    float pad1[4];
    float pad2[4];
    u128_D3B20 loc;

    if (moby == 0) return;
    glove_vars = *(char **)(moby + 0x78);
    if (glove_vars == 0) return;
    *(u128_D3B20 *)va = 0;
    va[1] = -0.19f;
    va[2] = -0.11f;
    FUN_L00_0024f7c8(moby, 0, vb);
    build_moby_bone_transform(moby, 0, vd);
    transform_vector_alt(vc, va, vd);
    add_vector_xyz(glove_vars + 0x40, vb, vc);

    tracked_target_moby = *(char **)(glove_vars + 0x50);
    if (tracked_target_moby) {
        *(int *)(tracked_target_moby + 0x98) = 1;
        if (D_0013E539_lui && *(short *)(*(char **)(glove_vars + 0x50) + 0xA6) != 0x76C) {
            qzero(&loc);
            mine_spawn_drops(*(char **)(glove_vars + 0x50), D_L00_001B0830);
            ((void (*)(void *))mark_moby_for_removal)(*(char **)(glove_vars + 0x50));
            *(int *)(glove_vars + 0x50) = FUN_L00_002c35c0(moby, glove_vars + 0x40, &loc);
        }
        if (*(char **)(glove_vars + 0x50) != 0 &&
            *(unsigned char *)(*(char **)(glove_vars + 0x50) + 0x20) != 0xFE &&
            *(unsigned char *)(*(char **)(glove_vars + 0x50) + 0x20) != 0xFD)
            goto target_valid;
    }
    *(int *)(glove_vars + 0x50) = 0;
target_valid:
    g = ((char *)&hero);
    if (*(int *)(g + 0x2084) == 1) FUN_L00_002223f8(0x1E, 1);
    if (fast_dec_timer((int *)(glove_vars + 0x54))) {
        if (*(unsigned char *)(g + 0x20AC) == 0) {
            if (*(unsigned char *)(g + 0x20A8) && *(int *)(g + 0x1BC) == scale_game_frames_c2(0x11)) goto L3D10;
            if (*(int *)(((char *)&D_0013CAE4)) & *(int *)(g + 0x10A0)) {
                if (*(int *)(g + 0x2084) == 0x1E && FUN_L00_00233e98(-1)) goto L3D10;
            }
            g = ((char *)&hero);
            if (*(int *)(g + 0x2084) == 0x23 && *(int *)(g + 0x198) == scale_game_frames_c2(0x10)) goto L3D10;
        }
    }
    goto L3D34;
L3D10:
    FUN_L00_00216de8(0x1A, 0);
    FUN_L00_00233db8(-1, 1);
    moby[0x20] = 3;
L3D34:
    *(u128_D3B20 *)&loc = *(u128_D3B20 *)(glove_vars + 0x40);
    FUN_L00_002d2158_c((int)moby, (void *)glove_vars, (float *)&loc);

    switch (moby[0x20]) {
    case DECOY_GLOVE_INITIALIZE:
        *(int *)(glove_vars + 0x50) = 0;
        if (!(moby[0x70] & 2)) moby[0x20] = 1; else moby[0x20] = 2;
    case DECOY_GLOVE_WAIT_FOR_MOBY_FLAG:
        if (moby[0x70] & 2) moby[0x20] = 2;
        break;
    case DECOY_GLOVE_TARGET_FOLLOW_AND_SPAWN:
        break;
    case DECOY_GLOVE_RELEASE_DECOY:
        if (*(char **)(glove_vars + 0x50)) {
            char *stats = ((char *)&item_usage);
            if (*(unsigned short *)(stats + 0xC8) <= 0xFFFE) *(unsigned short *)(stats + 0xC8) = *(unsigned short *)(stats + 0xC8) + 1;
            if (scale_game_frames_c2(D_0015EEA4) / 600 > (int)*(unsigned short *)(stats + 0xCA))
                *(unsigned short *)(stats + 0xCA) = scale_game_frames_c2(D_0015EEA4) / 600;
            *(unsigned int *)(stats + 0xCC) = (*(unsigned int *)(stats + 0xCC) | (1 << D_0015ED84)) | 0x80000000;
            FUN_L00_002c3888((float *)(glove_vars + 0x40), *(char **)(glove_vars + 0x50), *(char **)(*(char **)(glove_vars + 0x50) + 0x78) + 0x10);
            *(int *)(glove_vars + 0x50) = 0;
        } else {
            allocate_voice_for_target_entry(0, 0, (int)moby);
        }
        moby[0x20] = 4;
        *(int *)(glove_vars + 0x54) = scale_game_frames_c2(0x14);
        break;
    case DECOY_GLOVE_WAIT_AFTER_RELEASE: {
        char *h = ((char *)&hero);
        if (*(int *)(h + 0x2084) != 0x23 && *(unsigned char *)(h + 0x20A8) == 0) moby[0x20] = 2;
        break;
    }
    case DECOY_GLOVE_ENTER_SKIP_TARGET_FOLLOW_AND_SPAWN:
        moby[0x20] = 6;
        break;
    case DECOY_GLOVE_SKIP_TARGET_FOLLOW_AND_SPAWN:
        return;
    default:
        break;
    }

    if (*(char **)(glove_vars + 0x50)) {
        qcopy(*(char **)(glove_vars + 0x50) + 0x10, glove_vars + 0x40);
        return;
    }
    if (FUN_L00_00233e98(-1)) {
        qzero(&loc);
        *(int *)(glove_vars + 0x50) = FUN_L00_002c35c0(moby, glove_vars + 0x40, &loc);
    } else {
        char *h = ((char *)&hero);
        if (*(unsigned char *)(h + 0x20A8)) {
            qzero(&loc);
            *(int *)(glove_vars + 0x50) = FUN_L00_002c35c0(moby, glove_vars + 0x40, &loc);
        }
    }
}
/* Ported from rac1-decomp src/overlays/shared/vendor_002D1168.c (func_L00_002D3F40) */
#include "sda.h"

/* Draws the trail of fading ring sprites for a moby. */

typedef float Vec_2D3F40[4] __attribute__((aligned(16)));

typedef int U128_2D3F40 __attribute__((mode(TI)));

typedef union Vu_2D3F40 {
    U128_2D3F40 q;
    Vec_2D3F40 f;
} Vu_2D3F40;

typedef struct Quad_2D3F40 {
    Vec_2D3F40 v[4];
    int c[4];
    float uv[8];
    long g[4];
} Quad_2D3F40;

typedef struct Data_2D3F40 {
    char pad0[0x10];
    Vec_2D3F40 pos; /* 0x10 */
    char pad20[4];
    float t; /* 0x24 */
    float tmax; /* 0x28 */
    char pad2C[4];
    Vec_2D3F40 dir; /* 0x30 */
} Data_2D3F40;

typedef struct Moby_2D3F40 {
    char pad0[0x78];
    Data_2D3F40 *data; /* 0x78 */
    char pad7C[0x14];
    int rgba; /* 0x90 */
} Moby_2D3F40;

extern float D_0015ED6C;
extern int D_L00_00161900 __attribute__((sda));
extern int D_L00_00161904 __attribute__((sda));
extern int D_L00_00161908 __attribute__((sda));
extern int D_L00_0016190C __attribute__((sda));
extern int D_L00_00161910 __attribute__((sda));
extern int D_L00_00161914 __attribute__((sda));
extern int D_L00_00161918 __attribute__((sda));
extern s32 truncate_float_to_s32(f32) __asm__("func_001FA6D0");
extern s64 get_effect_texture(s32) __asm__("func_001F44B8");
extern void cross_vectors_xyz(void *, void *, void *) __asm__("func_001F9AD8");
extern void draw_geometry_quad(void *, int, int) __asm__("func_001F7D30");
extern int func_001F4868_2D3F40(int) __asm__("FUN_001f44b8");
extern void func_L00_001FF4B0_2D3F40(void *, void *, float) __asm__("FUN_001f9bf8");
extern int func_001FA898_2D3F40(float) __asm__("FUN_001fa6d0");
extern void func_L00_001FD1D8_2D3F40(void *, void *, int) __asm__("FUN_001f7d30");

void FUN_L00_002d2a90(Moby_2D3F40 *moby) {
    Quad_2D3F40 q;
    Vu_2D3F40 tt[6];
    Data_2D3F40 *trail_data = moby->data;
    float t, unwrapped_phase, k;
    int col;
    int tex;

    tex = func_001F4868_2D3F40(D_L00_00161918);
    q.c[0] = q.c[1] = q.c[2] = q.c[3] = D_L00_00161914;
    q.g[1] = tex;
    q.g[2] = 0xFF9000000260L;
    q.g[0] = 4;
    q.g[3] = (long)D_L00_00161900 | ((long)D_L00_00161904 << 2) | ((long)D_L00_00161908 << 4) | ((long)D_L00_0016190C << 6) | ((long)D_L00_00161910 << 32);
    q.uv[4] = q.uv[0] = 0.0f;
    q.uv[6] = q.uv[2] = 1.0f;
    q.uv[3] = q.uv[1] = 0.0f;
    q.uv[7] = q.uv[5] = 1.0f;
    trail_data->t = trail_data->t + D_0015ED6C;
    if (1.0f < trail_data->t) {
        trail_data->t = trail_data->t - 1.0f;
    }
    if (trail_data->t > trail_data->tmax) {
        trail_data->tmax = trail_data->t;
    }
    t = trail_data->t;
    tt[1].q = 0;
    tt[1].f[0] = 1.0f;
    func_L00_001FF4B0_2D3F40(tt[0].f, trail_data->dir, 1.0f);
    cross_vectors_xyz(tt[2].f, tt[0].f, tt[1].f);
    func_L00_001FF4B0_2D3F40(tt[2].f, tt[2].f, 1.0f);
    cross_vectors_xyz(tt[1].f, tt[0].f, tt[2].f);
    func_L00_001FF4B0_2D3F40(tt[1].f, tt[1].f, 1.0f);
    for (unwrapped_phase = trail_data->t; trail_data->t - 1.0f < unwrapped_phase; unwrapped_phase -= 0.1f) {
        if (t <= trail_data->tmax) {
            k = t * 0.5f + 0.23f;
            func_L00_001FF4B0_2D3F40(tt[3].f, tt[0].f, t);
            func_L00_001FF4B0_2D3F40(tt[4].f, tt[1].f, k);
            func_L00_001FF4B0_2D3F40(tt[5].f, tt[2].f, k);
            fast_vec_sub(q.v[0], trail_data->pos, tt[3].f);
            qcopy(q.v[1], q.v[0]);
            qcopy(q.v[2], q.v[0]);
            qcopy(q.v[3], q.v[0]);
            fast_vec_sub(q.v[0], q.v[0], tt[4].f);
            add_vector_xyz(q.v[1], q.v[1], tt[4].f);
            fast_vec_sub(q.v[2], q.v[2], tt[4].f);
            add_vector_xyz(q.v[3], q.v[3], tt[4].f);
            fast_vec_sub(q.v[0], q.v[0], tt[5].f);
            add_vector_xyz(q.v[2], q.v[2], tt[5].f);
            fast_vec_sub(q.v[1], q.v[1], tt[5].f);
            add_vector_xyz(q.v[3], q.v[3], tt[5].f);
            col = (moby->rgba & 0xFFFFFF) | (func_001FA898_2D3F40((1.0f - t) * 255.0f) << 24);
            q.c[0] = q.c[1] = q.c[2] = q.c[3] = col;
            func_L00_001FD1D8_2D3F40(&q, 0, 1);
        }
        t -= 0.1f;
        if (t < 0.0f) {
            t += 1.0f;
        }
    }
}
#include "sda.h"

/* increments the selected packed nibble, saturating at fifteen */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D1168.c: func_L00_002D42D8), where it is exact; names translated to the US level program. */

extern int D_0015ED84;
extern unsigned char D_0014BF10[];

void FUN_L00_002d2e28(unsigned char *moby) {
    int row = D_0015ED84;
    if (row < 20) {
        int index = moby[0xBC] - 1;
        if (index & 1) {
            unsigned char *p = D_0014BF10 + (index / 2 + row * 16);
            unsigned int value = *p;
            int n = (value & 15) + 1;
            if (n >= 16)
                n = 15;
            *p = (value & 240) | n;
        } else {
            unsigned char *p = D_0014BF10 + (index / 2 + row * 16);
            unsigned int value = *p;
            int n = (value >> 4) + 1;
            if (n >= 16)
                n = 15;
            *p = (value & 15) | (n << 4);
        }
    }
}
#include "qcopy.h"
#include "qzero.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/input/pad_state.h"

/* metal_detector_update: update for a moby that tracks hero.unk2040, the nearest
 * oclass 0x25D moby. It orients itself towards it within the
 * D_L00_00161950/54 limits (degrees), tints itself by the distance
 * (hero.unk2044) while the 0x20 pad button is held, and at under 1.0 drains
 * the target's count in steps of 5, then removes it. */

typedef f32 Mtx2d2ee8[4][4] __attribute__((aligned(16)));
typedef f32 Vec2d2ee8[4] __attribute__((aligned(16)));

typedef struct Vars2d2ee8 {
    s32 voice0;         /* 0x00 */
    s32 voice1;         /* 0x04 */
    s32 unk8;           /* 0x08: 0x7FFF */
    s32 voice2;         /* 0x0C: distance hum */
    Vec4 pos;           /* 0x10 */
    u8 unk20;           /* 0x20 */
    u8 pad21[3];
    s32 unk24;          /* 0x24 */
    s32 unk28;          /* 0x28 */
    u8 pad2C[4];
    Vec4 unk30;         /* 0x30 */
} Vars2d2ee8;

typedef struct TargetVars2d2ee8 {
    u8 pad0[0x18];
    s32 count;          /* 0x18: drained by 5 per step */
} TargetVars2d2ee8;

typedef struct VoiceSlot2d2ee8 {
    u8 pad0[0x74];
    u8 active;          /* 0x74 */
    u8 pad75[0x13];
    struct Moby *owner; /* 0x88 */
} VoiceSlot2d2ee8;

extern u8 D_0013E550[] __attribute__((section(".data")));
extern u128 D_L00_001E1160 __attribute__((section(".data")));
extern u8 D_L00_001E1170[] __attribute__((section(".data")));
extern f32 D_L00_00161950 __attribute__((sda));
extern f32 D_L00_00161954 __attribute__((sda));
extern s32 D_L00_0016191C __attribute__((sda));
extern s32 D_L00_00161920 __attribute__((sda));
extern s32 D_L00_00161924 __attribute__((sda));
extern s32 D_L00_0016192C __attribute__((sda));
extern s32 D_L00_00161930 __attribute__((sda));
extern f32 D_L00_00161940 __attribute__((sda));

extern void moby_point_position(struct Moby *, s32, void *) __asm__("FUN_L00_0024f7c8");
extern void attach_manipulator(void *, s32, void *) __asm__("FUN_0020cb10");
extern f32 vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern f32 fast_arc_tan(f32, f32) __asm__("FUN_001f9e90");
extern f32 fast_subtract_rotations(f32, f32) __asm__("FUN_001fa5c8");
extern f32 fast_add_rotations(f32, f32) __asm__("FUN_001fa580");
extern void build_spherical_offset(void *, f32, f32, f32) __asm__("FUN_00214db0");
extern void noop_main_callback(void *, void *) __asm__("FUN_001e93e8");
extern void fast_vec_normalize(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void fast_vec_cross(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void transpose_rotation_matrix(void *, void *) __asm__("FUN_001fa2d8");
extern void multiply_matrix(void *, void *, void *) __asm__("FUN_001fa378");
extern void matrix_to_quaternion(void *, void *) __asm__("FUN_00214260");
extern void FUN_L00_00259f50(void *, void *);
extern void quaternion_nlerp(void *, void *, void *, f32) __asm__("FUN_001fa400");
extern s32 voice_is_playing(struct Moby *, s32) __asm__("FUN_L00_0028d8c0");
extern int allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
extern void engine_voice_set_pitch(s32, s32) __asm__("FUN_L00_0028df38");
extern f32 random_float_between(f32, f32) __asm__("FUN_002132a8");
extern void FUN_L00_0025f8e0(void *, f32);
extern void FUN_L00_002a7438(void *, void *, void *, s32, s32, s32);
/* the unit declares mark_moby_for_removal() without a prototype further down */
extern void delete_moby(void *) __asm__("FUN_0020c828");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
extern void FUN_L00_002d2a90_u(void) __asm__("FUN_L00_002d2a90");
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");

extern struct PadState controller_state_lui __asm__("D_0013C940") __attribute__((section(".data")));
void metal_detector_update(struct Moby *moby) __asm__("FUN_L00_002d2ee8");
void metal_detector_update(struct Moby *moby) {
    enum {
        METAL_DETECTOR_INITIALIZE = 0,
        METAL_DETECTOR_ACTIVE = 1,
        METAL_DETECTOR_TRACK_AND_COLLECT = 2
    };
    Vars2d2ee8 *vars;
    struct Moby *target_moby;
    TargetVars2d2ee8 *tv;
    Vec2d2ee8 u;
    Vec2d2ee8 v;
    Vec2d2ee8 w;
    Mtx2d2ee8 aim_basis;
    Mtx2d2ee8 normalized_bone_transform;
    Mtx2d2ee8 transposed_bone_rotation;
    Mtx2d2ee8 m4;
    f32 yaw;
    f32 pitch;
    f32 abs_yaw;
    f32 abs_pitch;
    f32 lim;
    f32 zero;
    f32 one;

    if (moby == 0) {
        return;
    }
    vars = (Vars2d2ee8 *)moby->pvars;
    if (vars == 0) {
        return;
    }
    moby_point_position(moby, 0, &vars->pos);
    switch (moby->state) {
    case METAL_DETECTOR_INITIALIZE:
        hero.unk2044 = 100000.0f;
        hero.unk2040 = 0;
        vars->voice0 = -1;
        vars->voice1 = -1;
        vars->voice2 = -1;
        vars->unk8 = 0x7FFF;
        vars->unk20 = controller_state_lui.unk189;
        vars->unk24 = 0;
        vars->unk28 = 0;
        qzero(&D_L00_001E1160);
        attach_manipulator(moby, 3, &D_L00_001E1160);
        moby->state = 1;
        break;
    case 3:
    case 4:
        break;
    case METAL_DETECTOR_ACTIVE:
    case METAL_DETECTOR_TRACK_AND_COLLECT:
        one = 1.0f;
        *(u128 *)v = 0;
        v[2] = one;
        if (hero.unk2040 != 0 && hero.unk2040->oclass == 0x25D && hero.unk2040->state != 0xFE &&
            hero.unk2040->state != 0xFD) {
            pitch = vector_distance(&vars->pos, &hero.unk2040->pos);
            pitch = fast_arc_tan(pitch, hero.unk2040->pos.z - vars->pos.f[2]);
            yaw = fast_arc_tan(hero.unk2040->pos.x - vars->pos.f[0], hero.unk2040->pos.y - vars->pos.f[1]);
            yaw = fast_subtract_rotations(yaw, hero.motion.rot.f[2]);
            pitch = fast_subtract_rotations(pitch, -1.5707964f);
            abs_yaw = FUN_001f99c0(yaw);
            abs_pitch = FUN_001f99c0(pitch);
            lim = D_L00_00161950 * 0.017453292f;
            if (lim < abs_yaw) {
                yaw /= abs_yaw;
                yaw *= lim;
            }
            lim = D_L00_00161954 * 0.017453292f;
            if (lim < abs_pitch) {
                pitch /= abs_pitch;
                pitch *= lim;
            }
            zero = 0.0f;
            yaw = fast_add_rotations(yaw, hero.motion.rot.f[2]);
            pitch = fast_add_rotations(pitch, -1.5707964f);
            build_spherical_offset(v, -3.0f, yaw, pitch);
            fast_vec_add(w, &vars->pos, v);
            noop_main_callback(&vars->pos, w);
            fast_vec_normalize(aim_basis[D_L00_00161920], v, (f32)D_L00_0016192C);
            fast_vec_cross(aim_basis[D_L00_00161924], aim_basis[D_L00_00161920], &D_L00_00161940);
            fast_vec_normalize(aim_basis[D_L00_00161924], aim_basis[D_L00_00161924], (f32)D_L00_00161930);
            fast_vec_cross(aim_basis[D_L00_0016191C], aim_basis[D_L00_00161924], aim_basis[D_L00_00161920]);
            aim_basis[3][2] = zero;
            aim_basis[3][1] = zero;
            aim_basis[3][0] = zero;
            aim_basis[3][3] = one;
            build_moby_bone_transform(moby, 2, normalized_bone_transform);
            fast_vec_normalize(normalized_bone_transform[0], normalized_bone_transform[0], one);
            fast_vec_normalize(normalized_bone_transform[1], normalized_bone_transform[1], one);
            fast_vec_normalize(normalized_bone_transform[2], normalized_bone_transform[2], one);
            transpose_rotation_matrix(transposed_bone_rotation, normalized_bone_transform);
            transposed_bone_rotation[3][2] = zero;
            transposed_bone_rotation[3][1] = zero;
            transposed_bone_rotation[3][0] = zero;
            transposed_bone_rotation[3][3] = one;
            multiply_matrix(m4, transposed_bone_rotation, aim_basis);
            matrix_to_quaternion(u, m4);
        } else {
            *(u128 *)w = 0;
            FUN_L00_00259f50(u, w);
        }
        qcopy(&vars->unk30, v);
        quaternion_nlerp(D_L00_001E1170, D_L00_001E1170, u, 0.1f);
        build_moby_bone_transform(moby, 3, w);
        qcopy(&vars->unk30, w);
        if (controller_state_lui.pressed & 0x20) {
            vars->unk24 = 0;
            vars->unk28 = 0;
        }
        if ((controller_state_lui.held & 0x20) && hero.items[0].unk20 > scale_game_frames(0xF)) {
            if (hero.unk2044 < 10.0f) {
                if (voice_is_playing(moby, vars->voice2) == 0 && vars->voice2 == -1) {
                    vars->voice2 = allocate_voice_for_target_entry(1, 4, moby);
                }
                if (vars->voice2 != -1) {
                    engine_voice_set_pitch(vars->voice2,
                                     truncate_float_to_s32((10.0f - (hero.unk2044 + hero.unk2044)) * 32767.0f / 60.0f) - 0x1FFF);
                }
                {
                    s32 c = truncate_float_to_s32(hero.unk2044 * 255.0f / 10.0f);
                    s32 col = (c << 8) | 0xFF000000;

                    moby->glow_color = ((c << 16) | col) | 0xFF;
                }
                target_moby = hero.unk2040;
                if (target_moby != 0 && target_moby->oclass == 0x25D && target_moby->state != 0xFE && target_moby->state != 0xFD && hero.unk2044 < 1.0f) {
                    tv = (TargetVars2d2ee8 *)target_moby->pvars;
                    while (tv->count > 0) {
                        w[0] = random_float_between(1.0f, 10.0f) * frame_time;
                        w[1] = random_float_between(-10.0f, 10.0f) * frame_time;
                        w[2] = random_float_between(1.0f, 12.0f) * frame_time;
                        transform_vector_by_basis(w, w, &hero);
                        {
                            Vec4f *target_position = &hero.unk2040->pos;

                            qcopy(target_position, &vars->pos);
                            FUN_L00_0025f8e0(target_position, 0.2f);
                        }
                        target_moby = hero.unk2040;
                        FUN_L00_002a7438(target_moby, &target_moby->pos, w, 0x13, 5, 0);
                        target_moby = hero.unk2040;
                        qcopy(&target_moby->pos, &vars->pos);
                        if (tv != 0) {
                            if (tv->count > 0) {
                                tv->count -= 5;
                                target_moby = hero.unk2040;
                            }
                            FUN_L00_002d2e28((u8 *)target_moby);
                        } else {
                            FUN_L00_002d2e28((u8 *)target_moby);
                        }
                    }
                    delete_moby(hero.unk2040);
                    hero.unk2040 = 0;
                    hero.unk2044 = 100000.0f;
                    vars->unk8 = 0x7FFF;
                    allocate_voice_for_target_entry(3, 0, moby);
                    if (vars->voice2 != -1) {
                        VoiceSlot2d2ee8 *e = (VoiceSlot2d2ee8 *)(D_0013E550 + vars->voice2 * 0x70);

                        if (e->owner == moby && e->active != 0) {
                            release_voice_slot(vars->voice2);
                        }
                    }
                    vars->voice2 = -1;
                }
            } else {
                moby->glow_color = 0x80808080;
                if (vars->voice0 != -1) {
                    VoiceSlot2d2ee8 *e = (VoiceSlot2d2ee8 *)(D_0013E550 + vars->voice0 * 0x70);

                    if (e->owner == moby && e->active != 0) {
                        release_voice_slot(vars->voice0);
                    }
                }
                vars->voice0 = -1;
                if (vars->voice1 != -1) {
                    VoiceSlot2d2ee8 *e = (VoiceSlot2d2ee8 *)(D_0013E550 + vars->voice1 * 0x70);

                    if (e->owner == moby && e->active != 0) {
                        release_voice_slot(vars->voice1);
                    }
                }
                vars->voice1 = -1;
                if (vars->voice2 != -1) {
                    VoiceSlot2d2ee8 *e = (VoiceSlot2d2ee8 *)(D_0013E550 + vars->voice2 * 0x70);

                    if (e->owner == moby && e->active != 0) {
                        release_voice_slot(vars->voice2);
                    }
                }
                vars->voice2 = -1;
            }
            enqueue_callback_list_1(FUN_L00_002d2a90_u, moby);
            return;
        }
        moby->glow_color = 0x80808080;
        if (vars->voice0 != -1) {
            VoiceSlot2d2ee8 *e = (VoiceSlot2d2ee8 *)(D_0013E550 + vars->voice0 * 0x70);

            if (e->owner == moby && e->active != 0) {
                release_voice_slot(vars->voice0);
            }
        }
        vars->voice0 = -1;
        if (vars->voice1 != -1) {
            VoiceSlot2d2ee8 *e = (VoiceSlot2d2ee8 *)(D_0013E550 + vars->voice1 * 0x70);

            if (e->owner == moby && e->active != 0) {
                release_voice_slot(vars->voice1);
            }
        }
        vars->voice1 = -1;
        if (vars->voice2 != -1) {
            VoiceSlot2d2ee8 *e = (VoiceSlot2d2ee8 *)(D_0013E550 + vars->voice2 * 0x70);

            if (e->owner == moby && e->active != 0) {
                release_voice_slot(vars->voice2);
            }
        }
        vars->voice2 = -1;
        break;
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/vendor_002D1168.c: func_L00_002D4C80), where it is exact; names translated to the US level program. */

extern char D_0013E533[];
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
void blend_moby_animation(MobyAnim *arg0, int arg1, int arg2, int arg3) __asm__("FUN_00212f90");

/* Blends the moby to animation 6 when D_0013F350+0x2084 is 8. */
void helipack_update(char *a) __asm__("FUN_L00_002d37d0");
void helipack_update(char *moby) {
    if (*(int *)(D_0013E533 + 0x2EA1) == 8) {
        if (*(unsigned char *)(moby + 0x53) != 6) {
            blend_moby_animation(moby, 6, 0, scale_game_frames(10));
        }
    }
}
#include "qcopy.h"
#include "rnc/overlay/quad.h"
void *create_moby_local(int) __asm__("FUN_0020c4f8");
void upd_a(void *) __asm__("FUN_L00_00250df8");
void FUN_L00_0025d1b8_2d3838(void *) __asm__("FUN_L00_0025d1b8");

unsigned char *mine_camera_blast(unsigned char *s, void *b, int c, void *d) __asm__("FUN_L00_002d3838");
unsigned char *mine_camera_blast(unsigned char *blast_settings, void *b, int c, void *d) {
    unsigned char *spawned_moby = create_moby_local(0x27F);
    if (spawned_moby) {
        unsigned char *blast_vars;
        spawned_moby[0x30] = 0xFF;
        *(unsigned short *)(spawned_moby + 0x34) |= 1;
        spawned_moby[0x31] = 1;
        blast_vars = *(unsigned char **)(spawned_moby + 0x78);
        spawned_moby[0x20] = 0;
        spawned_moby[0xBC] = 0;
        qcopy(spawned_moby + 0x10, b);
        if (d)
            qcopy(blast_vars, d);
        else
            qcopy(blast_vars, blast_settings);
        *(int *)(blast_vars + 0x20) = c;
        *(unsigned short *)(spawned_moby + 0x32) = blast_settings[0x3C];
        blast_vars[0x24] = blast_settings[0x10];
        blast_vars[0x25] = blast_settings[0x11];
        blast_vars[0x26] = blast_settings[0x12];
        blast_vars[0x27] = blast_settings[0x13];
        blast_vars[0x28] = blast_settings[0x14];
        blast_vars[0x29] = blast_settings[0x15];
        blast_vars[0x68] = blast_settings[0x3D];
        *(unsigned short *)(blast_vars + 0x6C) = *(unsigned short *)(blast_settings + 0x40);
        blast_vars[0x2A] = blast_settings[0x16];
        blast_vars[0x2B] = blast_settings[0x17];
        blast_vars[0x2C] = blast_settings[0x18];
        blast_vars[0x2D] = blast_settings[0x19];
        blast_vars[0x2E] = blast_settings[0x1A];
        blast_vars[0x69] = blast_settings[0x3E];
        *(unsigned short *)(blast_vars + 0x6E) = *(unsigned short *)(blast_settings + 0x42);
        blast_vars[0x2F] = blast_settings[0x1B];
        blast_vars[0x30] = blast_settings[0x1C];
        blast_vars[0x31] = blast_settings[0x1D];
        blast_vars[0x32] = blast_settings[0x1E];
        blast_vars[0x33] = blast_settings[0x1F];
        blast_vars[0x6A] = blast_settings[0x3F];
        *(unsigned short *)(blast_vars + 0x70) = *(unsigned short *)(blast_settings + 0x44);
        *(float *)(blast_vars + 0x34) = *(float *)(blast_settings + 0x20);
        *(float *)(blast_vars + 0x38) = *(float *)(blast_settings + 0x24);
        *(float *)(blast_vars + 0x3C) = *(float *)(blast_settings + 0x28);
        *(float *)(blast_vars + 0x40) = *(float *)(blast_settings + 0x2C);
        *(float *)(blast_vars + 0x44) = *(float *)(blast_settings + 0x30);
        blast_vars[0x6B] = blast_settings[0x48];
        *(unsigned short *)(blast_vars + 0x72) = *(unsigned short *)(blast_settings + 0x46);
        *(int *)(blast_vars + 0x48) = *(int *)(blast_settings + 0x34);
        *(int *)(blast_vars + 0x4C) = *(int *)(blast_settings + 0x38);
        blast_vars[0x1C] = blast_settings[0x49];
        *(int *)(blast_vars + 0x64) = -1;
        upd_a(spawned_moby);
        FUN_L00_0025d1b8_2d3838(spawned_moby);
    }
    return spawned_moby;
}
/* moby class 639 update: runs its state machine and eases its tracked values toward their targets */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D1168.c: func_L00_002D4EC0), where it is exact; names translated to the US level program. */

typedef struct { float x, y, z, w; } Vx __attribute__((aligned(16)));

extern char *moby_table __asm__("D_L00_0015FFD8");
extern f32 convert_integer_to_float_c(s32) __asm__("func_001FA6C0");
extern f32 random_float_between_c(f32, f32) __asm__("func_002132A8");
extern int fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern int FUN_L00_001fefc8(void *);
extern int FUN_L00_0023e738(float *, float, float, float, float, float);
extern int fast_dec_timer(void *) __asm__("FUN_001f9740");
extern s32 scale_game_frames_c(s32) __asm__("func_001F96F8");
extern unsigned char D_L00_001802C0[];
extern void FUN_L00_0023e838(int);
extern void fast_vec_scale_c(void *, void *, f32) __asm__("func_001F9A68");
extern void mark_moby_for_removal() __asm__("FUN_0020c828");
int FUN_001fa728(Vec4f *, float);
extern int fast_bsphere_check(void *, float) __asm__("FUN_001fa728");
extern void mark_moby_for_removal() __asm__("FUN_0020c828");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

void FUN_L00_002d3a10(unsigned char *m) {
    unsigned char *o;
    Vx tp;
    Vx base_position;
    Vx position_offset;
    unsigned char *moby_position;
    void *base_position_ptr;
    void *position_offset_ptr;
    int c0;
    int c1 = 0;
    int c2 = 0;
    int c3 = 0;
    int r;

    if (m == 0) return;
    o = *(unsigned char **)(m + 0x78);
    c0 = 0;
    if (o == 0) return;
    if (m[0x20] == 0) {
        unsigned char b1c;
        m[0x30] = 0xFF;
        b1c = o[0x1C];
        m[0xBC] = b1c;
        qcopy(o + 0x10, m + 0x10);
        *(int *)(o + 0x50) = -1;
        o[0x1C] = b1c;
        *(int *)(o + 0xC) = *(int *)(o + 0x4C);
        *(short *)(o + 0x74) = *(unsigned short *)(o + 0x6C);
        *(short *)(o + 0x76) = *(unsigned short *)(o + 0x6E);
        *(short *)(o + 0x78) = *(unsigned short *)(o + 0x70);
        *(short *)(o + 0x7A) = *(unsigned short *)(o + 0x72);
        *(float *)(o + 0x54) = convert_integer_to_float_c(o[0x25]) / 100.0f;
        *(float *)(o + 0x58) = convert_integer_to_float_c(o[0x2A]) / 100.0f;
        {
            float f5 = convert_integer_to_float_c(o[0x2F]) / 100.0f;
            *(float *)(o + 0x60) = *(float *)(o + 0x34);
            *(float *)(o + 0x5C) = f5;
        }
        if (*(int *)(o + 0x64) != -1) {
            *(char **)(o + 0x20) = moby_table + (*(int *)(o + 0x64) << 8);
        }
        if (*(float *)(o + 0x40) < 0.0f) *(float *)(o + 0x40) = -*(float *)(o + 0x40);
        if (*(float *)(o + 0x44) < 0.0f) *(float *)(o + 0x44) = -*(float *)(o + 0x44);
        m[0x20] = 1;
    }
    if (m[0xBC] != 0) {
        m[0xBC] = m[0xBC] - 1;
        return;
    }
    if (*(int *)(o + 0x48) != -1 && fast_dec_timer((int *)(o + 0x48))) {
        if (*(int *)(o + 0x50) != -1) {
            FUN_L00_0023e838(*(int *)(o + 0x50));
            *(int *)(o + 0x50) = -1;
        }
        mark_moby_for_removal(m);
        return;
    }
    {
        unsigned char *s = m + 0x10;
        qcopy(&tp, s);
        tp.w = *(float *)(o + 0x60);
        moby_position = s;
        s = (unsigned char *)&tp;
        r = fast_bsphere_check(s, 255.0f);
    }
    if (r != -1) {
        if (*(unsigned char **)(o + 0x20) != 0) {
            qcopy(&base_position, *(unsigned char **)(o + 0x20) + 0x10);
        } else {
            qcopy(&base_position, o + 0x10);
        }
        base_position_ptr = &base_position;
        if (!(*(int *)(o + 0x4C) & 0x800000)) {
            if (*(int *)(o + 0x4C) & 0x100000) {
                if (*(int *)(o + 0x4C) & 0x200000) {
                    position_offset.x = random_float_between_c(-*(float *)(o + 0x0), *(float *)(o + 0x0));
                    position_offset.y = random_float_between_c(-*(float *)(o + 0x4), *(float *)(o + 0x4));
                    position_offset.z = random_float_between_c(-*(float *)(o + 0x8), *(float *)(o + 0x8));
                    position_offset_ptr = &position_offset;
                } else {
                    int c = *(int *)(o + 0xC);
                    void *p = &position_offset;
                    *(int *)(o + 0xC) = 0;
                    fast_vec_scale_c(p, o, random_float_between_c(*(float *)(o + 0xC), 1.0f));
                    *(int *)(o + 0xC) = c;
                    position_offset_ptr = p;
                }
            } else {
                int c = *(int *)(o + 0xC);
                void *p = &position_offset;
                *(int *)(o + 0xC) = 0;
                qcopy(p, o);
                *(int *)(o + 0xC) = c;
                position_offset_ptr = p;
            }
            if (*(int *)(o + 0x4C) & 0x400000) {
                if (*(unsigned char **)(o + 0x20) != 0) {
                    transform_vector_by_basis(position_offset_ptr, position_offset_ptr, *(unsigned char **)(o + 0x20) + 0xC0);
                }
            }
            add_vector_xyz(moby_position, base_position_ptr, position_offset_ptr);
        } else {
            qcopy(moby_position, &base_position);
        }
        if (*(unsigned short *)(o + 0x72) != 0) {
            if (fast_dec_timer_s16(o + 0x7A)) {
                *(short *)(o + 0x7A) = scale_game_frames(*(unsigned short *)(o + 0x72));
                if (*(int *)(o + 0x4C) & 4) {
                    c0 = 1;
                    *(float *)(o + 0x60) = *(float *)(o + 0x3C);
                    *(int *)(o + 0x4C) &= 0xFFFFFFF7;
                }
            }
        }
        if (!(*(int *)(o + 0x4C) & 0x80000)) {
            s64 q = *(s64 *)(o + 0x48);
            if ((q & (0xC000L << 18)) == (0x8000L << 18)) *(int *)(o + 0x4C) ^= 0x2;
            if (!(*(int *)(o + 0x4C) & 0x2)) {
                if (*(int *)(o + 0x4C) & 0x8) {
                    float t = *(float *)(o + 0x60) - *(float *)(o + 0x44) * frame_scale;
                    *(float *)(o + 0x60) = t;
                    if (t <= *(float *)(o + 0x3C)) {
                        *(float *)(o + 0x60) = *(float *)(o + 0x3C);
                        if (*(int *)(o + 0x4C) & 0x4) {
                            if (scale_game_frames(*(unsigned short *)(o + 0x72)) == 0) *(int *)(o + 0x4C) ^= 0x8;
                        }
                    }
                } else {
                    float t = *(float *)(o + 0x60) + *(float *)(o + 0x40) * frame_scale;
                    *(float *)(o + 0x60) = t;
                    if (*(float *)(o + 0x38) <= t) {
                        *(float *)(o + 0x60) = *(float *)(o + 0x38);
                        if (!(*(int *)(o + 0x4C) & 0x1)) {
                            *(int *)(o + 0x4C) ^= 0x8;
                        } else {
                            *(unsigned char *)(o + 0x7F) = scale_game_frames(*(unsigned char *)(o + 0x6B));
                            *(int *)(o + 0x4C) = (*(int *)(o + 0x4C) | 0x2) ^ 0x8;
                        }
                    }
                }
            } else {
                if (*(unsigned char *)(o + 0x6B) != 0 && FUN_L00_001fefc8(o + 0x7F)) *(int *)(o + 0x4C) ^= 0x2;
            }
        } else {
            if (*(unsigned short *)(o + 0x72) == 0 || c0 == 1) {
                *(float *)(o + 0x60) = random_float_between_c(*(float *)(o + 0x3C), *(float *)(o + 0x38));
            }
        }
        if (*(unsigned short *)(o + 0x6C) != 0) {
            if (fast_dec_timer_s16(o + 0x74)) {
                *(short *)(o + 0x74) = scale_game_frames(*(unsigned short *)(o + 0x6C));
                *(int *)(o + 0x4C) &= 0xFFFFFF7F;
                *(float *)(o + 0x54) = (float)*(unsigned char *)(o + 0x27);
            }
        }
        if (!(*(int *)(o + 0x4C) & 0x10000)) {
            s64 q = *(s64 *)(o + 0x48);
            if ((q & (0xC000L << 22)) == (0x8000L << 22)) *(int *)(o + 0x4C) ^= 0x20;
            if (!(*(int *)(o + 0x4C) & 0x20)) {
                if (*(int *)(o + 0x4C) & 0x80) {
                    float a = convert_integer_to_float_c(o[0x27]) / 100.0f;
                    float b = convert_integer_to_float_c(o[0x29]) / 100.0f;
                    float t = *(float *)(o + 0x54) - b * frame_scale;
                    *(float *)(o + 0x54) = t;
                    if (t <= a) {
                        *(float *)(o + 0x54) = a;
                        if (*(int *)(o + 0x4C) & 0x40) {
                            if (*(unsigned short *)(o + 0x6C) == 0) *(int *)(o + 0x4C) ^= 0x80;
                        }
                    }
                } else {
                    float a = convert_integer_to_float_c(o[0x26]) / 100.0f;
                    float b = convert_integer_to_float_c(o[0x28]) / 100.0f;
                    float t = *(float *)(o + 0x54) + b * frame_scale;
                    *(float *)(o + 0x54) = t;
                    if (a <= t) {
                        *(float *)(o + 0x54) = a;
                        if (!(*(int *)(o + 0x4C) & 0x10)) {
                            *(int *)(o + 0x4C) ^= 0x80;
                        } else {
                            *(unsigned char *)(o + 0x7C) = scale_game_frames(o[0x68]);
                            *(int *)(o + 0x4C) = (*(int *)(o + 0x4C) | 0x20) ^ 0x80;
                        }
                    }
                }
            } else {
                if (o[0x68] != 0 && FUN_L00_001fefc8(o + 0x7C)) *(int *)(o + 0x4C) ^= 0x20;
            }
        } else {
            if (*(unsigned short *)(o + 0x6C) == 0 || c1 == 1) {
                float a = (float)func_001FA898_r((float)o[0x27]) / 100.0f;
                float b = (float)func_001FA898_r((float)o[0x26]) / 100.0f;
                *(float *)(o + 0x54) = random_float_between_c(a, b);
            }
        }
        if (*(unsigned short *)(o + 0x6E) != 0) {
            if (fast_dec_timer_s16(o + 0x76)) {
                *(short *)(o + 0x76) = scale_game_frames(*(unsigned short *)(o + 0x6E));
                *(int *)(o + 0x4C) &= 0xFFFFF7FF;
                *(float *)(o + 0x58) = (float)o[0x2C];
            }
        }
        if (!(*(int *)(o + 0x4C) & 0x20000)) {
            s64 q = *(s64 *)(o + 0x48);
            if ((q & (0xC000L << 26)) == (0x8000L << 26)) *(int *)(o + 0x4C) ^= 0x200;
            if (!(*(int *)(o + 0x4C) & 0x200)) {
                if (*(int *)(o + 0x4C) & 0x800) {
                    float a = convert_integer_to_float_c(o[0x2C]) / 100.0f;
                    float b = convert_integer_to_float_c(o[0x2E]) / 100.0f;
                    float t = *(float *)(o + 0x58) - b * frame_scale;
                    *(float *)(o + 0x58) = t;
                    if (t <= a) {
                        *(float *)(o + 0x58) = a;
                        if (*(int *)(o + 0x4C) & 0x400) {
                            if (*(unsigned short *)(o + 0x6E) == 0) *(int *)(o + 0x4C) ^= 0x800;
                        }
                    }
                } else {
                    float a = convert_integer_to_float_c(o[0x2B]) / 100.0f;
                    float b = convert_integer_to_float_c(o[0x2D]) / 100.0f;
                    float t = *(float *)(o + 0x58) + b * frame_scale;
                    *(float *)(o + 0x58) = t;
                    if (a <= t) {
                        *(float *)(o + 0x58) = a;
                        if (!(*(int *)(o + 0x4C) & 0x100)) {
                            *(int *)(o + 0x4C) ^= 0x800;
                        } else {
                            *(unsigned char *)(o + 0x7D) = scale_game_frames(o[0x69]);
                            *(int *)(o + 0x4C) = (*(int *)(o + 0x4C) | 0x200) ^ 0x800;
                        }
                    }
                }
            } else {
                if (o[0x69] != 0 && FUN_L00_001fefc8(o + 0x7D)) *(int *)(o + 0x4C) ^= 0x200;
            }
        } else {
            if (*(unsigned short *)(o + 0x6E) == 0 || c2 == 1) {
                float a = (float)func_001FA898_r((float)o[0x2C]) / 100.0f;
                float b = (float)func_001FA898_r((float)o[0x2B]) / 100.0f;
                *(float *)(o + 0x58) = random_float_between_c(a, b);
            }
        }
        if (*(unsigned short *)(o + 0x70) != 0) {
            if (fast_dec_timer_s16(o + 0x78)) {
                *(short *)(o + 0x78) = scale_game_frames(*(unsigned short *)(o + 0x70));
                *(int *)(o + 0x4C) &= 0xFFFF7FFF;
                *(float *)(o + 0x5C) = (float)o[0x31];
            }
        }
        if (!(*(int *)(o + 0x4C) & 0x40000)) {
            s64 q = *(s64 *)(o + 0x48);
            if ((q & (0xC000L << 30)) == (0x8000L << 30)) *(int *)(o + 0x4C) ^= 0x2000;
            if (!(*(int *)(o + 0x4C) & 0x2000)) {
                if (*(int *)(o + 0x4C) & 0x8000) {
                    float a = convert_integer_to_float_c(o[0x31]) / 100.0f;
                    float b = convert_integer_to_float_c(o[0x33]) / 100.0f;
                    float t = *(float *)(o + 0x5C) - b * frame_scale;
                    *(float *)(o + 0x5C) = t;
                    if (t <= a) {
                        *(float *)(o + 0x5C) = a;
                        if (*(int *)(o + 0x4C) & 0x4000) {
                            if (*(unsigned short *)(o + 0x70) == 0) *(int *)(o + 0x4C) ^= 0x8000;
                        }
                    }
                } else {
                    float a = convert_integer_to_float_c(o[0x30]) / 100.0f;
                    float b = convert_integer_to_float_c(o[0x32]) / 100.0f;
                    float t = *(float *)(o + 0x5C) + b * frame_scale;
                    *(float *)(o + 0x5C) = t;
                    if (a <= t) {
                        *(float *)(o + 0x5C) = a;
                        if (!(*(int *)(o + 0x4C) & 0x1000)) {
                            *(int *)(o + 0x4C) ^= 0x8000;
                        } else {
                            *(unsigned char *)(o + 0x7E) = scale_game_frames(o[0x6A]);
                            *(int *)(o + 0x4C) = (*(int *)(o + 0x4C) | 0x2000) ^ 0x8000;
                        }
                    }
                }
            } else {
                if (o[0x6A] != 0 && FUN_L00_001fefc8(o + 0x7E)) *(int *)(o + 0x4C) ^= 0x2000;
            }
        } else {
            if (*(unsigned short *)(o + 0x70) == 0 || c3 == 1) {
                float a = (float)func_001FA898_r((float)o[0x31]) / 100.0f;
                float b = (float)func_001FA898_r((float)o[0x30]) / 100.0f;
                *(float *)(o + 0x5C) = random_float_between_c(a, b);
            }
        }
        r = *(int *)(o + 0x50);
        if (r == -1) {
            float kk = convert_integer_to_float_c(o[0x24]) / 100.0f;
            *(int *)(o + 0x50) = FUN_L00_0023e738((float *)moby_position, *(float *)(o + 0x60), kk, *(float *)(o + 0x54), *(float *)(o + 0x58), *(float *)(o + 0x5C));
        } else {
            unsigned char *e = D_L00_001802C0 + (r << 5);
            qcopy(e + 0x10, moby_position);
            *(float *)(e + 0x1C) = *(float *)(o + 0x60);
            *(float *)(e + 0x0) = *(float *)(o + 0x54);
            *(float *)(e + 0x4) = *(float *)(o + 0x58);
            *(float *)(e + 0x8) = *(float *)(o + 0x5C);
            *(float *)(e + 0xC) = convert_integer_to_float_c(o[0x24]) / 100.0f;
        }
    } else {
        if (*(int *)(o + 0x50) != -1) {
            FUN_L00_0023e838(*(int *)(o + 0x50));
            *(int *)(o + 0x50) = r;
        }
    }
}
typedef struct Moby Moby;
extern void FUN_00212f90_2d4610(void *, int, int, int) __asm__("FUN_00212f90");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
typedef float ToadVec[4] __attribute__((aligned(16)));
typedef struct { u8 pad0[0x24]; float unk24; } ToadClass;
typedef struct { int count; int pad4[3]; ToadVec pts[1]; } ToadPath;
typedef struct {
    u8 pad0[0x29];
    u8 unk29;
    u8 pad2A[0x58 - 0x2A];
    u8 unk58;
    u8 pad59;
    u8 unk5A;
    u8 pad5B[0x70 - 0x5B];
    u8 unk70[0x138 - 0x70];
    s16 unk138;
    u8 pad13A[0x140 - 0x13A];
    void *unk140;
    u8 pad144[0x180 - 0x144];
    ToadVec unk180;
    u8 pad190[0x1C0 - 0x190];
    Moby *target;
    int unk1C4;
    u8 pad1C8[8];
    ToadVec home;
    int unk1E0;
    int unk1E4;
    int path;
    int follow_moby;
    int pickup_moby;
    float turn_speed;
    float unk1F8;
    float unk1FC;
    u8 unk200[0x40];
    ToadVec unk240;
    float unk250;
    float unk254;
    float unk258;
    float unk25C;
    u8 pad260[0x14];
    s16 path_point;
    s16 unk276;
} ToadVars;

extern char camera_position[] __asm__("D_L00_00166DC0");
extern char *moby_table __asm__("D_L00_0015FFD8") MACRO_ADDR;
extern ToadPath *toad_paths[] __asm__("D_L00_001B04B0");

extern void FUN_L00_002d5160_2d4610(void *) __asm__("FUN_L00_002d5160");
extern void FUN_L00_002d5830_2d4610(void *) __asm__("FUN_L00_002d5830");
extern float distance_xyz(void *, void *) __asm__("FUN_001f9b48");
extern void FUN_L00_0025a120_2d4610(void *) __asm__("FUN_L00_0025a120");
extern void FUN_L00_00263ac8_2d4610(float, int, int, unsigned char *) __asm__("FUN_L00_00263ac8");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern int truncate_float_to_s32(float) __asm__("FUN_001fa6d0");
extern int FUN_L00_002dc9f0_2d4610(void *, void *) __asm__("FUN_L00_002dc9f0");
extern float angle_atan2(float, float) __asm__("FUN_001f9e90");
extern float approach_angle(void *, float, void *, float, float, float) __asm__("FUN_L00_0025be00");
extern float FUN_L00_002d54c8_2d4610(unsigned char *m, float *t, float vel) __asm__("FUN_L00_002d54c8");
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern void FUN_L00_00261630_2d4610(char *moby, char *d, float a, float b) __asm__("FUN_L00_00261630");
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
extern int is_value_within_interpolated_window(void *, float) __asm__("FUN_00214cc8");
extern void FUN_L00_002599e8_2d4610(void *, float, float, float, int, int, int, int, int) __asm__("FUN_L00_002599e8");
extern void normalize_vector_xyz(void *, void *, float) __asm__("FUN_001f9bf8");
extern int FUN_L00_00261968_2d4610(int, void *, void *) __asm__("FUN_L00_00261968");
extern int random_integer_below(int) __asm__("FUN_00213260");
extern int FUN_L00_0025c698_2d4610(void *, void *) __asm__("FUN_L00_0025c698");
extern void FUN_0022da68_2d4610(int, int, int) __asm__("FUN_0022da68");
extern void FUN_L00_0025f3e8_2d4610(void *, void *, int, float, float) __asm__("FUN_L00_0025f3e8");
extern void FUN_0020c828_2d4610(void *) __asm__("FUN_0020c828");
extern float probe_ground_height(void *, int, float) __asm__("FUN_00213508");
extern void FUN_L00_00261d78_2d4610(int, float, void *, void *) __asm__("FUN_L00_00261d78");
extern float FUN_00213ed8_2d4610(float, float, float *) __asm__("FUN_00213ed8");

/* Ported from rac1-decomp (src/overlays/shared/vendor_002D1168.c: func_L00_002D5AC0), where it is exact; names translated to the US level program. */

/* Class 749 update (horny toad): state machine that walks a path or follows a target moby, hops, turns and dies; then settles its height and checks a pickup moby. */
void horny_toad_update(Moby *moby) __asm__("FUN_L00_002d4610");
extern float D_L00_0016196C __attribute__((sda));
extern float D_L00_00161980 __attribute__((sda));
extern float D_L00_00161984 __attribute__((sda));
extern short D_L00_00161960 __attribute__((sda));
void horny_toad_update(Moby *moby) {
    enum {
        HORNY_TOAD_INITIALIZE = 0,
        HORNY_TOAD_FACE_FOLLOW_MOBY = 1,
        HORNY_TOAD_MOVE_TO_PATH_POINT = 2,
        HORNY_TOAD_WAIT_TO_FOLLOW_PATH = 3,
        HORNY_TOAD_FOLLOW_PATH = 4,
        HORNY_TOAD_ROAM = 5,
        HORNY_TOAD_APPROACH_AND_FACE_TARGET = 6,
        HORNY_TOAD_PLAY_TARGET_ANIMATION = 7,
        HORNY_TOAD_RETURN_HOME = 8,
        HORNY_TOAD_RUN_MOVEMENT_SEQUENCE = 9,
        HORNY_TOAD_MOVE_TO_POINT_OR_FACE_TARGET = 10,
        HORNY_TOAD_RECOVER_FROM_REACTION = 11,
        HORNY_TOAD_DYING = 12
    };
    ToadVars *data;
    Moby *target;
    ToadVec v;

    data = (ToadVars *)moby->pvars;
    FUN_L00_002d5160_2d4610(moby);
    target = data->target;
    if (moby->visible != 0 && distance_xyz((*(ToadVec *)&moby->pos), camera_position) < 26.0f) {
        FUN_L00_0025a120_2d4610(moby);
        moby->shadow_range = 0x15;
    }
    FUN_L00_00263ac8_2d4610(2.1f, (int)moby, 1, data->unk200);
    switch (moby->state) {
    case HORNY_TOAD_INITIALIZE: {
        float k;
        moby->scale = ((ToadClass *)moby->pclass)->unk24 * D_L00_0016196C;
        qcopy(data->home, (*(ToadVec *)&moby->pos));
        if (moby->prev_seq != 1) FUN_00212f90_2d4610(moby, 1, 0, 0);
        data->unk29 = 1;
        data->unk140 = &D_L00_00161960;
        qcopy(data->unk240, (*(ToadVec *)&moby->pos));
        k = frame_time;
        data->unk25C = 1.5f;
        data->unk254 = k * 2.0f;
        data->unk258 = k * 2.9670596f;
        {
            *(u128 *)v = 0;
            v[0] = 150.0f;
            v[1] = 127.0f;
            if (vector_distance((*(ToadVec *)&moby->pos), v) < 5.0f) {
                data->unk25C = 0.75f;
            }
        }
        data->unk58 = truncate_float_to_s32(12.0f);
        data->unk5A = truncate_float_to_s32(2.0f);
        if (data->path >= 0) {
            if (data->unk1E4 >= 0) {
                ToadPath *path = toad_paths[data->path];
                qcopy(data->home, path->pts[path->count - 1]);
                qcopy((*(ToadVec *)&moby->pos), path->pts[0]);
                moby->state = 3;
            } else if (data->follow_moby >= 0) {
                qcopy(data->home, (*(ToadVec *)&moby->pos));
                moby->state = 1;
                if (moby->prev_seq != 2) FUN_00212f90_2d4610(moby, 2, 0, scale_game_frames(10));
            } else {
                FUN_0020c828_2d4610(moby);
                return;
            }
        } else {
            moby->state = 5;
        }
        break;
    }
    case HORNY_TOAD_RUN_MOVEMENT_SEQUENCE:
        if (FUN_L00_002dc9f0_2d4610(moby, data->unk70) != 0) {
            moby->state = 5;
            data->unk138 = 0;
        }
        break;
    case HORNY_TOAD_FACE_FOLLOW_MOBY: {
        Moby *t = (Moby *)(moby_table + (data->follow_moby << 8));
        if (t != 0 && t->state != 0xFE && t->state != 0xFD) {
            approach_angle(&moby->rot.z, angle_atan2((*(ToadVec *)&t->pos)[0] - (*(ToadVec *)&moby->pos)[0], (*(ToadVec *)&t->pos)[1] - (*(ToadVec *)&moby->pos)[1]), &data->turn_speed, frame_time_sq * 12.566371f, frame_time_sq * 12.566371f, frame_time * 12.566371f);
        } else {
            moby->state = 8;
            if (moby->prev_seq != 4) FUN_00212f90_2d4610(moby, 4, 0, scale_game_frames(10));
        }
        break;
    }
    case HORNY_TOAD_MOVE_TO_PATH_POINT:
        if (FUN_L00_002d54c8_2d4610((unsigned char *)moby, toad_paths[data->path]->pts[data->path_point], 0.5f) < 1.0f) {
            moby->state = 1;
            if (moby->prev_seq != 2) FUN_00212f90_2d4610(moby, 2, 0, scale_game_frames(10));
        }
        break;
    case HORNY_TOAD_WAIT_TO_FOLLOW_PATH:
        if (data->unk1C4 != 2) {
            moby->state = 4;
            if (moby->prev_seq != 4) FUN_00212f90_2d4610(moby, 4, 0, scale_game_frames(10));
        }
        break;
    case HORNY_TOAD_FOLLOW_PATH: {
        ToadPath *path = toad_paths[data->path];
        if (FUN_L00_002d54c8_2d4610((unsigned char *)moby, path->pts[data->path_point], 0.5f) < 1.0f) {
            if (data->path_point < path->count - 1) {
                data->path_point++;
            } else {
                moby->state = 5;
                if (moby->prev_seq != 1) FUN_00212f90_2d4610(moby, 1, 0, scale_game_frames(10));
            }
        } else if (data->unk1C4 != 2) {
            moby->state = 6;
            if (moby->prev_seq != 4) FUN_00212f90_2d4610(moby, 4, 0, scale_game_frames(10));
        }
        break;
    }
    case HORNY_TOAD_ROAM:
        if (moby->prev_seq != 4) {
            int n = random_int_between(0, 5);
            if (moby->prev_seq != 4) FUN_00212f90_2d4610(moby, 4, n, scale_game_frames(10));
            moby->anim_speed = random_float_between(0.92f, 1.08f);
        }
        FUN_L00_00261630_2d4610((char *)moby, (char *)data->unk240, 0.5f, 0.5f);
        if (data->unk1C4 != 2) {
            data->unk1FC = random_float_between(-20.0f, 20.0f) * 0.017453292f;
            moby->state = 6;
            if (moby->prev_seq != 4) FUN_00212f90_2d4610(moby, 4, 0, scale_game_frames(10));
        }
        break;
    case HORNY_TOAD_APPROACH_AND_FACE_TARGET:
        if (FUN_L00_002d54c8_2d4610((unsigned char *)moby, data->unk180, 0.5f) < D_L00_00161980
            && fast_difference_between_rotations(moby->rot.z, angle_atan2((*(ToadVec *)&target->pos)[0] - (*(ToadVec *)&moby->pos)[0], (*(ToadVec *)&target->pos)[1] - (*(ToadVec *)&moby->pos)[1])) < 0.17453292f) {
            moby->state = 7;
            if (moby->prev_seq != 5) FUN_00212f90_2d4610(moby, 5, (int)D_L00_00161984, scale_game_frames(10));
        } else if (data->unk1C4 == 2) {
            moby->state = 8;
            if (moby->prev_seq != 4) FUN_00212f90_2d4610(moby, 4, 0, scale_game_frames(10));
        }
        break;
    case HORNY_TOAD_PLAY_TARGET_ANIMATION:
        if (is_value_within_interpolated_window(moby, 34.0f) != 0) {
            FUN_L00_002599e8_2d4610(moby, 0.333f, 1.0f, 1.0f, 0, 1, 0, 1, 0);
        } else if (moby->anim_flags & 2) {
            if (data->unk1C4 == 2) {
                moby->state = 8;
                if (moby->prev_seq != 4) FUN_00212f90_2d4610(moby, 4, 0, scale_game_frames(10));
            } else if (vector_distance((*(ToadVec *)&moby->pos), (*(ToadVec *)&target->pos)) > 1.5f
                       || fast_difference_between_rotations(moby->rot.z, angle_atan2((*(ToadVec *)&target->pos)[0] - (*(ToadVec *)&moby->pos)[0], (*(ToadVec *)&target->pos)[1] - (*(ToadVec *)&moby->pos)[1])) > 0.17453292f) {
                moby->state = 6;
                if (moby->prev_seq != 4) FUN_00212f90_2d4610(moby, 4, 0, scale_game_frames(10));
            }
        }
        break;
    case HORNY_TOAD_RETURN_HOME:
        if (FUN_L00_002d54c8_2d4610((unsigned char *)moby, data->home, 0.5f) < 1.5f) {
            moby->state = 5;
            if (moby->prev_seq != 1) FUN_00212f90_2d4610(moby, 1, 0, scale_game_frames(10));
        } else if (data->unk1C4 != 2) {
            moby->state = 6;
            if (moby->prev_seq != 4) FUN_00212f90_2d4610(moby, 4, 0, scale_game_frames(10));
        }
        break;
    case HORNY_TOAD_MOVE_TO_POINT_OR_FACE_TARGET: {
        fast_vec_sub(v, data->unk180, (*(ToadVec *)&moby->pos));
        v[2] = 0.0f;
        normalize_vector_xyz(v, v, 2.5f);
        add_vector_xyz(v, v, (*(ToadVec *)&moby->pos));
        if (FUN_L00_00261968_2d4610(data->unk1E0, (*(ToadVec *)&moby->pos), v) != 0) {
            approach_angle(&moby->rot.z, angle_atan2((*(ToadVec *)&data->target->pos)[0] - (*(ToadVec *)&moby->pos)[0], (*(ToadVec *)&data->target->pos)[1] - (*(ToadVec *)&moby->pos)[1]), &data->turn_speed, frame_time_sq * 12.566371f, frame_time_sq * 12.566371f, frame_time * 12.566371f);
            if (moby->anim_flags & 2) {
                if (moby->prev_seq != random_integer_below(2) + 2) FUN_00212f90_2d4610(moby, random_integer_below(2) + 2, 0, scale_game_frames(20));
            }
        } else {
            if (moby->prev_seq != 4) FUN_00212f90_2d4610(moby, 4, 0, scale_game_frames(20));
            FUN_L00_002d54c8_2d4610((unsigned char *)moby, data->unk180, 0.5f);
        }
        if (moby->anim_flags & 2) {
            if (data->unk1C4 != 2) {
                moby->state = 6;
                if (moby->prev_seq != 4) FUN_00212f90_2d4610(moby, 4, 0, scale_game_frames(10));
            } else if (data->unk276 == 0) {
                moby->state = 8;
                if (moby->prev_seq != 4) FUN_00212f90_2d4610(moby, 4, 0, scale_game_frames(20));
            }
        }
        break;
    }
    case HORNY_TOAD_RECOVER_FROM_REACTION:
        if (FUN_L00_0025c698_2d4610(moby, data->unk70) & 0x140) {
            moby->state = 5;
            if (moby->prev_seq != 1) FUN_00212f90_2d4610(moby, 1, 0, scale_game_frames(10));
        }
        break;
    case HORNY_TOAD_DYING:
        if (FUN_L00_0025c698_2d4610(moby, data->unk70) & 0x140) {
            FUN_0022da68_2d4610(7, 0, (int)moby);
            FUN_L00_0025f3e8_2d4610(moby, (*(ToadVec *)&moby->pos), -1, 0.75f, 10.0f);
            FUN_0020c828_2d4610(moby);
            return;
        }
        break;
    }
    if (moby->state != 11 && moby->state != 12 && moby->state != 9) {
        FUN_00213ed8_2d4610(probe_ground_height((*(ToadVec *)&moby->pos), 0, 0.5f), frame_time * 7.0f, &(*(ToadVec *)&moby->pos)[2]);
        if (moby->state != 4) {
            FUN_L00_00261d78_2d4610(data->unk1E0, 0.25f, (*(ToadVec *)&moby->pos), (*(ToadVec *)&moby->pos));
        }
    }
    if (data->pickup_moby != -1) {
        Moby *o = (Moby *)(moby_table + (data->pickup_moby << 8));
        if (vector_distance((*(ToadVec *)&moby->pos), (*(ToadVec *)&o->pos)) < 1.0f) {
            if (o->oclass == 0x33E) {
                FUN_L00_002d5830_2d4610(o);
                data->pickup_moby = -1;
            }
        }
    }
}
#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L00_002d5160.s", FUN_L00_002d5160);
#else
extern s32 D_L00_0015F5C4_2d5160 __asm__("D_L00_0015F5C4");
extern f32 D_0015ED6C_2d5160 __asm__("D_0015ED6C");
extern float D_0015ED70;
extern f32 D_L00_00161970_2d5160 __asm__("D_L00_00161970") __attribute__((sda));
extern f32 D_L00_00161974_2d5160 __asm__("D_L00_00161974") __attribute__((sda));
extern f32 D_L00_00161978_2d5160 __asm__("D_L00_00161978") __attribute__((sda));
extern f32 D_L00_0016197C_2d5160 __asm__("D_L00_0016197C") __attribute__((sda));
extern f32 D_L00_00161988_2d5160 __asm__("D_L00_00161988") __attribute__((sda));
extern f32 D_L00_0016198C_2d5160 __asm__("D_L00_0016198C") __attribute__((sda));
extern unsigned char D_0013F3D0_2d5160[] __asm__("D_0013F3D0");
extern unsigned char *D_L00_001B04B0_2d5160[] __asm__("D_L00_001B04B0");
extern void *FUN_L00_0025a420_2d5160(void *, s32, s32) __asm__("FUN_L00_0025a420");
extern void FUN_L00_0025a478_2d5160(void *, void *, void *, s32, s32 *, f32 *, s32, s32) __asm__("FUN_L00_0025a478");
extern void FUN_L00_0025ab48_2d5160(void *, f32 *, void *, void *) __asm__("FUN_L00_0025ab48");
extern void FUN_L00_0025c558_2d5160(void *, void *, f32, s32, s32, s32) __asm__("FUN_L00_0025c558");
extern void FUN_L00_0025d458_2d5160(void *, void *) __asm__("FUN_L00_0025d458");
extern void FUN_L00_0025d538_2d5160(void *, void *) __asm__("FUN_L00_0025d538");
extern void FUN_L00_0025ff38_2d5160(f32, void *, void *, s32, s32, void *, s32) __asm__("FUN_L00_0025ff38");
extern void FUN_L00_00257470_2d5160(void *, s32, s32) __asm__("FUN_L00_00257470");
extern s32 FUN_L00_00257b50_2d5160(s32) __asm__("FUN_L00_00257b50");
extern f32 FUN_L00_00257be8_2d5160(f32, f32) __asm__("FUN_L00_00257be8");
extern void FUN_001fef98_2d5160(void *) __asm__("FUN_001f9770");
extern f32 FUN_001fef10_2d5160(f32) __asm__("FUN_001f96b0");
extern s16 FUN_00200250_2d5160(f32) __asm__("FUN_001fa6d0");
extern s32 FUN_001fef20_2d5160(s32) __asm__("FUN_001f96f8");
extern void FUN_L00_00257880_2d5160(void *, s32, s32, s32) __asm__("FUN_L00_00257880");

void FUN_L00_002d5160(unsigned char *m) {
    unsigned char *v = *(unsigned char **)(m + 0x78);
    unsigned char *hit;
    s32 result;
    f32 damage;
    f32 angle;
    f32 vec[4] __attribute__((aligned(16)));
    f32 forceX, forceY, forceZ, forceW, health, duration;
    f32 reactionC0, reactionC4;
    unsigned char *row;
    s32 index;
    unsigned char *effect;

    if (m[0x20] == 0)
        return;
    if (D_L00_0015F5C4_2d5160 == 2) {
        m[0x31] = 0;
        *(u16 *)(m + 0x34) |= 1;
    } else {
        m[0x31] = 1;
        *(u16 *)(m + 0x34) &= ~1;
    }
    damage = 0.0f;
    hit = FUN_L00_0025a420_2d5160(m, 0x330000, 0);
    FUN_L00_0025a478_2d5160(m, hit, v + 0x20, 0, &result, &damage, 0, 4);
    if (result != 1 && m[0x20] != 12 && damage != 0.0f) {
        forceX = D_L00_00161970_2d5160 * D_0015ED70;
        forceY = D_L00_00161974_2d5160 * D_0015ED70;
        forceZ = D_L00_00161978_2d5160 * D_0015ED6C_2d5160;
        forceW = D_L00_0016197C_2d5160 * D_0015ED6C_2d5160;
        health = *(f32 *)(v + 0x20) - damage;
        v[0xAD] = 0;
        duration = D_0015ED6C_2d5160 + D_0015ED6C_2d5160;
        *(volatile s32 *)(v + 0x94) = 0x29;
        *(volatile s32 *)(v + 0x90) = 0x200;
        *(f32 *)(v + 0x98) = 0.75f;
        *(f32 *)(v + 0x20) = health;
        *(f32 *)(v + 0x84) = forceY;
        *(f32 *)(v + 0x88) = forceZ;
        *(f32 *)(v + 0x8C) = forceW;
        *(f32 *)(v + 0x80) = forceX;
        *(f32 *)(v + 0xBC) = duration;
        *(u16 *)(m + 0x34) &= ~0x1000;
        *(OvlQuad *)vec = *(OvlQuad *)(hit + 0x10);
        FUN_L00_0025ab48_2d5160(vec, &angle, v + 0x88, v + 0x8C);
        FUN_L00_0025c558_2d5160(m, v + 0x70, angle, FUN_L00_00257b50_2d5160(2) + 6, 1, 0);
        reactionC0 = D_L00_0016198C_2d5160;
        reactionC4 = D_L00_00161988_2d5160;
        *(f32 *)(v + 0xC0) = reactionC0;
        *(f32 *)(v + 0xC4) = reactionC4;
        m[0x20] = 12;
        v[0x67] = 0x78;
        FUN_L00_0025d458_2d5160(m, v + 0x60);
        FUN_L00_00257470_2d5160(m, 0, -1);
    }
    m[0xA4] = 0xFF;
    FUN_L00_0025d538_2d5160(m, v + 0x60);
    if (m[0x20] == 3) {
        index = *(s32 *)(v + 0x1E4);
        effect = v + 0x180;
        FUN_L00_0025ff38_2d5160(64.0f, m, effect, 0, 0, D_L00_001B04B0_2d5160[index] + 0x10, *(s32 *)D_L00_001B04B0_2d5160[index]);
    } else {
        index = *(s32 *)(v + 0x1E0);
        effect = v + 0x180;
        FUN_L00_0025ff38_2d5160(24.0f, m, effect, 0, 0, D_L00_001B04B0_2d5160[index] + 0x10, *(s32 *)D_L00_001B04B0_2d5160[index]);
    }
    if (*(s32 *)(v + 0x1C0) == 0) {
        *(s32 *)(v + 0x1C0) = (s32)hero.moby;
        qcopy(effect, &hero.motion.pos);
    }
    FUN_001fef98_2d5160(v + 0x276);
    index = *(s32 *)(v + 0x38);
    if (index != 0) {
        *(s16 *)(v + 0x276) = FUN_00200250_2d5160(FUN_001fef10_2d5160(FUN_L00_00257be8_2d5160(180.0f, 240.0f)));
    }
    *(s32 *)(v + 0x38) = 0;
    if ((m[0x20] == 5 || m[0x20] == 8) && *(s16 *)(v + 0x276) != 0) {
        m[0x20] = 10;
        if (m[0x53] != 4)
            FUN_L00_00257880_2d5160(m, 4, 0, FUN_001fef20_2d5160(20));
    }
}
#endif
extern float D_0015ED6C_2d54c8 __asm__("D_0015ED6C");
/* D_0015ED70 is declared sda above (crate_drop_bolts), but here retail loads it
   absolute: reach it as D_0015ED6C + 4, past whose size the assembler skips $gp. */
#define D_0015ED70_2d54c8 (((float *)&D_0015ED6C_2d54c8)[1])
float angle_atan2(float, float) __asm__("FUN_001f9e90");
float distance_xyz(void *, void *) __asm__("FUN_001f9b48");
float FUN_001fa580_2d54c8(float, float) __asm__("FUN_001fa580");
float FUN_L00_0025be00_2d54c8(void *, float, void *, float, float,
                              float) __asm__("FUN_L00_0025be00");
float advance_accelerated_scalar(float *, float, float, float, float, float *) __asm__("FUN_00213f38");
float FUN_001f9dc8_2d54c8(float) __asm__("FUN_001f9dc8");
float FUN_001f9de0_2d54c8(float) __asm__("FUN_001f9de0");
void FUN_L00_00258830_2d54c8(void *, void *, int, float, float, float) __asm__("FUN_L00_00258830");

float FUN_L00_002d54c8(unsigned char *moby, float *target_position, float vel) {
    float vec[4] __attribute__((aligned(16)));
    unsigned char *v;
    float ang, target_distance, z, movement_yaw, lim, w;
    v = *(unsigned char **)(moby + 0x78);
    ang = angle_atan2(target_position[0] - *(float *)(moby + 0x10), target_position[1] - *(float *)(moby + 0x14));
    z = *(float *)(moby + 0x18);
    target_distance = distance_xyz(moby + 0x10, target_position);
    if (2.0f < target_distance)
        ang = FUN_001fa580_2d54c8(ang, *(float *)(v + 0x1FC));
    if (moby[0x52] != moby[0x53])
        return target_distance;
    FUN_L00_0025be00_2d54c8(moby + 0x48, ang, v + 0x1F4, D_0015ED70_2d54c8 * 12.566371f,
                            D_0015ED70_2d54c8 * 12.566371f, D_0015ED6C_2d54c8 * 12.566371f);
    advance_accelerated_scalar(&vel, target_distance, D_0015ED70_2d54c8 * 8.0f, D_0015ED70_2d54c8 * 12.0f,
                        D_0015ED6C_2d54c8 * 4.0f, (float *)(v + 0x1F8));
    if (*(float *)(v + 0x1F8) < 0.0f)
        *(float *)(v + 0x1F8) = 0.0f;
    movement_yaw = *(float *)(moby + 0x48);
    *(float *)(v + 0x278) = movement_yaw;
    vec[0] = FUN_001f9dc8_2d54c8(movement_yaw) * *(float *)(v + 0x1F8);
    vec[1] = FUN_001f9de0_2d54c8(*(float *)(v + 0x278)) * *(float *)(v + 0x1F8);
    vec[2] = 0.0f;
    vec[2] = *(float *)(v + 0x270) - D_0015ED70_2d54c8 * 10.0f;
    FUN_L00_00258830_2d54c8(moby, vec, 0x10, 0.5f, 0.5f, 0.0f);
    w = *(float *)(moby + 0x18) - z;
    lim = D_0015ED6C_2d54c8 * 4.0f;
    *(float *)(v + 0x270) = w;
    if (lim < w)
        *(float *)(v + 0x270) = lim;
    if (moby[0x70] & 2)
        return target_distance;
    return 37.0f;
}
typedef struct {
    char pad[0x78];
    s32 f;
} T6;
s32 FUN_L00_002d5800(T6 *moby) {
    s32 v;
    if (moby != 0) {
        v = moby->f;
        if (v != 0) {
            return v + 0xD0;
        }
        return 0;
    }
    return 0;
}

typedef struct {
    u8 pad[0x454];
    u8 killed[1]; /* per spawn id: spawn slot + 2 */
} KillFlags;
/* Row 0 holds the count; the keyed rows follow. */
typedef struct {
    s32 count;
    u8 pad04[8];
    f32 key;
} KeyRow;
typedef struct {
    s32 table; /* -1, or an index into D_L00_001B04B0 */
    f32 key;
} KeyVars;
extern KillFlags D_L00_001BA5D0 __attribute__((section(".data")));
extern KillFlags D_L00_001BB230 __attribute__((section(".data")));
extern u8 mission_state[8] __asm__("D_L00_0015FC88");
extern u8 D_0014C050[][16] __attribute__((section(".data")));

/* Kills a live moby: marks it in the level's spawn tables, starts its death sound and
 * clears its key out of the shared key table. */
void FUN_L00_002d5830(struct Moby *moby) {
    KeyVars *v = (KeyVars *)moby->pvars;
    s32 i;
    if (moby->state != 1) {
        return;
    }
    D_L00_001BA5D0.killed[(s16)moby->save_id] = moby->mission + 2;
    if (moby->mission == 0xFF || (mission_state[moby->mission] != 0xFF && D_0014C050[current_level_index][moby->mission] == 0xFF)) {
        D_L00_001BB230.killed[(s16)moby->save_id] = moby->mission + 2;
    }
    moby->state = 2;
    moby->command = 1;
    moby->glow_color = 0x80208020;
    allocate_voice_for_target_entry(0, 0, moby);
    if (v->table == -1) {
        return;
    }
    for (i = 0; i < ((KeyRow *)D_L00_001B04B0_2d0538[v->table])[0].count; i++) {
        KeyRow *row = &((KeyRow *)D_L00_001B04B0_2d0538[v->table])[i];
        if (row[1].key == v->key) {
            row[1].key = 0;
        }
    }
}
typedef unsigned int u128_2d5988 __attribute__((mode(TI), aligned(16)));
typedef union {
    u128_2d5988 q;
    float f[4];
} Vec4_2d5988;

typedef struct {
    u8 p0[0x60];
    float v60;
    float v64;
    float v68;
    float v6C;
} St_2d5988;

typedef struct {
    u8 p0[0x10];
    float v[3];
    u8 p1[0x20 - 0x1C];
    u8 flag;
    u8 p2[0x40 - 0x21];
    float f40;
    float f44;
    float f48;
    u8 p3[0x78 - 0x4C];
    St_2d5988 *st;
} Obj_2d5988;

typedef struct {
    s32 n;
    u8 pad[0x10 - 4];
    Vec4_2d5988 a[1];
    Vec4_2d5988 b[1];
} Path_2d5988;

extern float D_0015ED6C_2d5988 __asm__("D_0015ED6C");
extern float D_0015ED6C_pair_2d5988[2] __asm__("D_0015ED6C");
#define D_0015ED70_2d5988 D_0015ED6C_pair_2d5988[1]

extern void FUN_L00_0025df68_2d5988(void *, void *, void *, s32 *, f32 *, s32, f32, f32,
                                    f32) __asm__("FUN_L00_0025df68");
extern float turn_f(f32 *, f32, f32, f32, f32, f32) __asm__("FUN_L00_00258110");
extern float FUN_001f9a68_2d5988(void *, void *, f32) __asm__("FUN_001f9a68");
extern float FUN_001f9af0_2d5988(void *) __asm__("FUN_001f9af0");
extern float FUN_001f9bf8_2d5988(void *, void *, f32) __asm__("FUN_001f9bf8");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float FUN_001f9dc8_2d5988(f32) __asm__("FUN_001f9dc8");
extern float FUN_001f9de0_2d5988(f32) __asm__("FUN_001f9de0");
extern float FUN_001f9e90_2d5988(f32, f32) __asm__("FUN_001f9e90");
extern float FUN_001fa5c8_2d5988(f32, f32) __asm__("FUN_001fa5c8");
extern float convert_integer_to_float(s32) __asm__("FUN_001fa6c0");

extern float D_L00_00161990 __attribute__((sda));
void FUN_L00_002d5988(Obj_2d5988 *moby, Path_2d5988 *path, s32 arrival_state) {
    St_2d5988 *st;
    s32 idx;
    f32 t;
    Vec4_2d5988 v;
    Vec4_2d5988 w;
    f32 distance_along_path;
    f32 w_;
    f32 current_speed;
    f32 speed_increment;
    f32 l;
    f32 r;
    f32 a;
    f32 b;

    st = moby->st;
    FUN_L00_0025df68_2d5988(path, &moby->v[0], &w, &idx, &t, 0, 999.0f, 5.0f, 0.0f);
    distance_along_path = convert_integer_to_float(idx) * path->a[0].f[3] + t;
    w_ = distance_along_path + 2.0f;
    l = distance_xyz(&moby->v[0], (void *)((char *)path + path->n * 16));
    speed_increment = D_0015ED70_2d5988 * 16.0f;
    current_speed = st->v60;
    if (l <= (current_speed * current_speed) / (speed_increment + speed_increment)) {
        f32 low = current_speed - speed_increment;
        f32 b1 = D_L00_00161990 * D_0015ED6C_2d5988;
        st->v60 = low;
        if (low < b1)
            st->v60 = b1;
    } else {
        f32 high = current_speed + speed_increment;
        f32 b2 = D_0015ED6C_2d5988 * 16.0f;
        st->v60 = high;
        if (b2 < high)
            st->v60 = b2;
    }
    idx = truncate_float_to_s32(w_ / path->a[0].f[3]);
    t = (w_ - convert_integer_to_float(idx) * path->a[0].f[3]) / path->a[0].f[3];
    if (idx >= path->n - 1) {
        idx = path->n - 2;
        t = 1.0f;
    }
    fast_vec_sub(&w, &path->b[idx], &path->a[idx]);
    FUN_001f9a68_2d5988(&w, &w, t);
    add_vector_xyz(&w, &w, &path->a[idx]);
    l = vector_distance((void *)((char *)path + path->n * 16), &moby->v[0]);
    if (l < 1.0f) {
        if (st->v60 <= D_L00_00161990 * D_0015ED6C_2d5988) {
            moby->flag = arrival_state;
        }
    }
    if (idx + 1 < path->n - 8) {
        r = FUN_001f9e90_2d5988(path->a[idx + 1].f[0] - moby->v[0], path->a[idx + 1].f[1] - moby->v[1]);
        moby->f48 = turn_f(&st->v6C, moby->f48, r, 0.01f, 0.3f, 0.1f);
    } else {
        r = FUN_001f9e90_2d5988(hero.motion.pos.f[0] - moby->v[0], hero.motion.pos.f[1] - moby->v[1]);
        moby->f48 = turn_f(&st->v6C, moby->f48, r, 0.01f, 0.3f, 0.1f);
    }
    fast_vec_sub(&v, &w, &moby->v[0]);
    l = FUN_001f9af0_2d5988(&v);
    if (st->v60 < l)
        l = st->v60;
    FUN_001f9bf8_2d5988(&v, &v, l);
    add_vector_xyz(&moby->v[0], &moby->v[0], &v);
    r = FUN_001fa5c8_2d5988(FUN_001f9e90_2d5988(v.f[0], v.f[1]), moby->f48);
    {
        f32 xa, xb;
        xa = st->v60 * 0.34906584f * FUN_001f9dc8_2d5988(r) / (D_0015ED6C_2d5988 * 16.0f);
        xb = st->v60 * -0.34906584f * FUN_001f9de0_2d5988(r) / (D_0015ED6C_2d5988 * 16.0f);
        {
            f32 q44, k70;
            q44 = turn_f(&st->v68, moby->f44, xa, D_0015ED70_2d5988 * 0.52359879f,
                                          D_0015ED70_2d5988 * 1.04719758f,
                                          D_0015ED6C_2d5988 * 0.785398185f);
            k70 = D_0015ED70_2d5988;
            moby->f44 = q44;
            moby->f40 = turn_f(&st->v64, moby->f40, xb, k70 * 1.04719758f,
                                             k70 * 2.09439516f, D_0015ED6C_2d5988 * 1.57079637f);
        }
    }
}
#include "types.h"
#include "sda.h"
#include "rnc/gameplay/entities/moby.h"

/* Infobot (class 750) update: flies the path set D_L00_001B04B0 between
   planets, then plays the unlock sequence. */

typedef union {
    u128 q;
    f32 f[4];
    struct {
        f32 x, y, z, w;
    } v;
} Vec;

/* The moby position, read and written as one quadword. */
#define MPOS(m) ((Vec *)&(m)->pos)

typedef struct {
    s32 count;
    u8 pad4[0xC];
    Vec pts[1]; /* pts[i].v.w: length of segment i */
} Path;

typedef struct {
    s32 arrived;    /* 0x00 */
    s32 planet;     /* 0x04 */
    s32 cutscene;   /* 0x08 */
    s8 msg0;        /* 0x0C */
    s8 msg1;        /* 0x0D */
    s8 msg2;        /* 0x0E */
    u8 pad0F[0x41];
    s32 attach;     /* 0x50 */
    s32 carrier;    /* 0x54 */
    s32 path;       /* 0x58 */
    f32 bob;        /* 0x5C */
    f32 speed;      /* 0x60 */
    f32 spin[3];    /* 0x64: x, y, z turn rates */
    s32 slots[8];   /* 0x70 */
    u8 pad90[0x14];
    f32 sway;       /* 0xA4 */
    s32 seg;        /* 0xA8 */
    f32 yaw;        /* 0xAC */
    s32 waiting;    /* 0xB0 */
    u8 padB4[0xC];
    u128 home;      /* 0xC0 */
} Infobot;

typedef struct {
    Vec pos;
    u8 pad10[0x2218];
    s32 active;     /* 0x2228 */
} Player;

typedef struct {
    u8 pad0[0x80];
    Vec target;     /* 0x80 */
} Camera;

typedef struct {
    u8 pad0[0x30];
    Vec pos;        /* 0x30 */
    u8 pad40[0x30];
    Vec dir;        /* 0x70 */
} Cutscene;         /* 0x80 */

extern Path *D_L00_001B04B0[];
extern u8 level_available[] __asm__("D_0013DD40");
extern Player D_0013F3D0;
extern Camera D_0013F350;
extern f32 D_0015ED6C;
extern f32 D_0015ED70;
extern s32 D_L00_0015F5C4;
extern struct Moby *D_L00_0015FFD8;   /* the level's moby array */
extern Cutscene *D_L00_001600EC_2d5de8 __asm__("D_L00_001600EC");

extern void FUN_L00_002d6bf0_2d5de8(struct Moby *) __asm__("FUN_L00_002d6bf0");
extern void FUN_L00_002d6cd0_2d5de8(struct Moby *) __asm__("FUN_L00_002d6cd0");
extern void FUN_L00_002d5988_2d5de8(struct Moby *, Path *, s32) __asm__("FUN_L00_002d5988");
extern f32 FUN_001f9b48(void *, void *);
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void fast_vec_scale(void *, void *, f32) __asm__("FUN_001f9a68");
extern void fast_vec_normalize(void *, void *, f32) __asm__("FUN_001f9bf8");
extern f32 fast_vec_length(void *) __asm__("FUN_001f9af0");
extern f32 fast_arc_tan(f32, f32) __asm__("FUN_001f9e90");
extern f32 fast_cos(f32) __asm__("FUN_001f9dc8");
extern f32 fast_sin(f32) __asm__("FUN_001f9de0");
extern f32 fast_add_rotations(f32, f32) __asm__("FUN_001fa580");
extern f32 fast_subtract_rotations(f32, f32) __asm__("FUN_001fa5c8");
extern s32 FUN_001fa6d0(f32);
extern f32 convert_integer_to_float(s32) __asm__("FUN_001fa6c0");
extern f32 spring_turn_2d5de8(f32 angle, f32 target, f32 accel, f32 damping, f32 limit, f32 *speed) __asm__("FUN_L00_00258110");
extern void FUN_L00_0025df68(Path *, void *, void *, s32 *, f32 *, s32, f32, f32, f32);
extern s32 FUN_L00_00260400(struct Moby *, struct Moby *, void *, void *, void *, void *);
extern void FUN_L00_002607d0(s32);
extern void jet_clear_persistent(s32) __asm__("FUN_L00_002502a0");
extern void jet_reset_camera(void *, void *) __asm__("FUN_L00_00284e50");
extern void start_scene(s32) __asm__("FUN_L00_00298840");
extern void FUN_L00_00299460(s32);
extern void FUN_L00_00262d38(s32);
extern void FUN_L00_00266858(struct Moby *, s32);
extern void memcard_save_data(s32, s32) __asm__("FUN_0020b178");
extern void FUN_0020c828(struct Moby *);

/* Store each segment's length in its start point's w. */
static inline void measure_path(Path *path) {
    s32 j;

    for (j = 0; j < path->count - 2; j++) {
        path->pts[j].v.w = FUN_001f9b48(&path->pts[j], &path->pts[j + 1]);
    }
}

void infobot_update(struct Moby *moby) __asm__("FUN_L00_002d5de8");
void infobot_update(struct Moby *moby)
{
    enum {
        INFOBOT_INITIALIZE = 0,
        INFOBOT_IDLE = 1,
        INFOBOT_WAIT_FOR_PLAYER = 2,
        INFOBOT_WAIT_TO_FOLLOW_SLOT_PATH = 3,
        INFOBOT_FOLLOW_SLOT_PATH = 4,
        INFOBOT_ORBIT_CARRIER = 5,
        INFOBOT_FOLLOW_PATH = 6,
        INFOBOT_ORBIT_PATH_ENDPOINT = 7,
        INFOBOT_START_SCENE_SEQUENCE = 8,
        INFOBOT_WAIT_TO_DISPATCH_MSG1 = 9,
        INFOBOT_WAIT_TO_START_FINAL_SCENE = 10,
        INFOBOT_SAVE_AND_REMOVE = 11
    };
    Vec offset;
    Vec step;
    Vec point;
    s32 segment;
    f32 fraction;
    Infobot *infobot;
    Path *path;
    Vec *pos;
    struct Moby *carrier;
    Cutscene *scene;
    s32 i;
    f32 range;
    f32 distance;
    f32 end;
    f32 accel;
    f32 speed;
    f32 heading;
    f32 tilt_y;
    f32 tilt_x;
    f32 max_speed;

    offset.q = 0;
    infobot = (Infobot *)moby->pvars;
    switch (moby->state) {
    case INFOBOT_INITIALIZE:
        moby->update_dist = 0xFF;
        infobot->yaw = moby->rot.z;
        infobot->home = MPOS(moby)->q;
        if (infobot->arrived != 0) {
            moby->state = 2;
            FUN_L00_002d6bf0_2d5de8(moby);
        } else {
            if (infobot->waiting != 0) {
                moby->state = 2;
            } else {
                moby->state = 1;
            }
            moby->flags |= 0x41;
        }
        if (infobot->attach != -1) {
            moby->state = 5;
            FUN_L00_002d6bf0_2d5de8(moby);
            break;
        }
        if (infobot->slots[0] == -1) {
            break;
        }
        for (i = 0; i < 8; i++) {
            if (infobot->slots[i] == -1) {
                continue;
            }
            measure_path(D_L00_001B04B0[infobot->slots[i]]);
        }
        MPOS(moby)->q = D_L00_001B04B0[infobot->slots[0]]->pts[0].q;
        infobot->seg = 0;
        moby->state = 3;
        break;
    case INFOBOT_IDLE:
        break;
    case INFOBOT_WAIT_FOR_PLAYER:
        range = 1.0f;
        if (infobot->waiting != 0 && (moby->flags & 1)) {
            range = 3.0f;
        }
        if (infobot->planet == 0xC) {
            range = 3.0f;
        }
        moby->rot.z = spring_turn_2d5de8(moby->rot.z, infobot->yaw, 0.01f, 0.3f, 0.1f, &infobot->spin[2]);
        moby->rot.y = spring_turn_2d5de8(moby->rot.y, 0.0f, D_0015ED70 * 0.5235988f, D_0015ED70 * 1.0471976f, D_0015ED6C * 0.7853982f, &infobot->spin[1]);
        moby->rot.x = spring_turn_2d5de8(moby->rot.x, 0.0f, D_0015ED70 * 1.0471976f, D_0015ED70 * 2.0943952f, D_0015ED6C * 1.5707964f, &infobot->spin[0]);
        FUN_L00_002d6cd0_2d5de8(moby);
        if (level_available[infobot->planet] != 0) {
            FUN_0020c828(moby);
            return;
        }
        if (FUN_001f9b48(&D_0013F3D0, &moby->pos) < range && D_0013F3D0.active != 0) {
            moby->state = 8;
        }
        break;
    case INFOBOT_WAIT_TO_FOLLOW_SLOT_PATH:
        FUN_L00_002d6cd0_2d5de8(moby);
        if (level_available[infobot->planet] != 0) {
            FUN_0020c828(moby);
            return;
        }
        if (infobot->slots[infobot->seg] == -1) {
            moby->state = 2;
            break;
        }
        heading = fast_arc_tan(D_0013F350.target.v.x - moby->pos.x, D_0013F350.target.v.y - moby->pos.y);
        moby->rot.z = spring_turn_2d5de8(moby->rot.z, heading, 0.01f, 0.3f, 0.1f, &infobot->spin[2]);
        moby->rot.y = spring_turn_2d5de8(moby->rot.y, 0.0f, D_0015ED70 * 0.5235988f, D_0015ED70 * 1.0471976f, D_0015ED6C * 0.7853982f, &infobot->spin[1]);
        moby->rot.x = spring_turn_2d5de8(moby->rot.x, 0.0f, D_0015ED70 * 1.0471976f, D_0015ED70 * 2.0943952f, D_0015ED6C * 1.5707964f, &infobot->spin[0]);
        if (FUN_001f9b48(&D_0013F350.target, &moby->pos) < 8.0f) {
            moby->state = 4;
            FUN_L00_00266858(moby, 1);
        }
        break;
    case INFOBOT_FOLLOW_SLOT_PATH:
        FUN_L00_002d5988_2d5de8(moby, D_L00_001B04B0[infobot->slots[infobot->seg]], 3);
        FUN_L00_002d6cd0_2d5de8(moby);
        if (moby->state != 4) {
            infobot->seg++;
        }
        break;
    case INFOBOT_ORBIT_CARRIER:
        FUN_L00_002d6cd0_2d5de8(moby);
        if (level_available[infobot->planet] != 0) {
            FUN_0020c828(moby);
            return;
        }
        infobot->bob = fast_add_rotations(infobot->bob, D_0015ED6C * 3.1415927f);
        offset.v.x = fast_cos(infobot->bob);
        offset.v.y = fast_sin(infobot->bob);
        offset.v.z = 3.0f;
        moby->rot.z = fast_add_rotations(infobot->bob, 1.5707964f);
        if (infobot->carrier == -1) {
            break;
        }
        if (D_L00_0015FFD8[infobot->carrier].oclass != 0x336) {
            break;
        }
        if (D_L00_0015FFD8[infobot->carrier].command != 6) {
            break;
        }
        if (infobot->path == -1) {
            break;
        }
        moby->state = 6;
        infobot->attach = -1;
        MPOS(moby)->q = D_L00_001B04B0[infobot->path]->pts[0].q;
        moby->rot.z = fast_arc_tan(D_L00_001B04B0[infobot->path]->pts[1].v.x - D_L00_001B04B0[infobot->path]->pts[0].v.x,
                                D_L00_001B04B0[infobot->path]->pts[1].v.y - D_L00_001B04B0[infobot->path]->pts[0].v.y);
        measure_path(D_L00_001B04B0[infobot->path]);
        break;
    case INFOBOT_FOLLOW_PATH:
        path = D_L00_001B04B0[infobot->path];
        pos = (Vec *)&moby->pos;
        FUN_L00_002d6cd0_2d5de8(moby);
        FUN_L00_0025df68(path, pos, &point, &segment, &fraction, 0, 999.0f, 5.0f, 0.0f);
        end = convert_integer_to_float(segment) * path->pts[0].v.w + fraction + 2.0f;
        distance = FUN_001f9b48(pos, &path->pts[path->count - 1]);
        accel = D_0015ED70 * 16.0f;
        speed = infobot->speed;
        if (distance <= speed * speed / (accel + accel)) {
            infobot->speed = speed - accel;
            if (speed - accel < 0.0f) {
                infobot->speed = 0.0f;
            }
        } else {
            max_speed = D_0015ED6C * 16.0f;
            infobot->speed = speed + accel;
            if (max_speed < speed + accel) {
                infobot->speed = max_speed;
            }
        }
        segment = FUN_001fa6d0(end / path->pts[0].v.w);
        fraction = (end - convert_integer_to_float(segment) * path->pts[0].v.w) / path->pts[0].v.w;
        if (segment >= path->count - 1) {
            if (infobot->speed <= D_0015ED70 * 16.0f) {
                MPOS(moby)->q = ((Vec *)path)[path->count].q;
                moby->state = 7;
                break;
            }
            segment = path->count - 2;
            fraction = 1.0f;
        }
        fast_vec_sub(&point, &path->pts[segment + 1], &path->pts[segment]);
        fast_vec_scale(&point, &point, fraction);
        fast_vec_add(&point, &point, &path->pts[segment]);
        heading = fast_arc_tan(path->pts[segment + 1].v.x - path->pts[segment].v.x,
                         path->pts[segment + 1].v.y - path->pts[segment].v.y);
        moby->rot.z = spring_turn_2d5de8(moby->rot.z, heading, 0.01f, 0.3f, 0.1f, &infobot->spin[2]);
        pos = &step;
        fast_vec_sub(pos, &point, MPOS(moby));
        distance = fast_vec_length(pos);
        if (infobot->speed < distance) {
            distance = infobot->speed;
        }
        fast_vec_normalize(pos, pos, distance);
        fast_vec_add(MPOS(moby), MPOS(moby), pos);
        heading = fast_subtract_rotations(fast_arc_tan(step.v.x, step.v.y), moby->rot.z);
        tilt_y = infobot->speed * 0.34906584f * fast_cos(heading) / (D_0015ED6C * 16.0f);
        tilt_x = infobot->speed * -0.34906584f * fast_sin(heading) / (D_0015ED6C * 16.0f);
        moby->rot.y = spring_turn_2d5de8(moby->rot.y, tilt_y, D_0015ED70 * 0.5235988f, D_0015ED70 * 1.0471976f, D_0015ED6C * 0.7853982f, &infobot->spin[1]);
        moby->rot.x = spring_turn_2d5de8(moby->rot.x, tilt_x, D_0015ED70 * 1.0471976f, D_0015ED70 * 2.0943952f, D_0015ED6C * 1.5707964f, &infobot->spin[0]);
        break;
    case INFOBOT_ORBIT_PATH_ENDPOINT: {
        Path *p = D_L00_001B04B0[infobot->path];
        FUN_L00_002d6cd0_2d5de8(moby);
        infobot->sway = infobot->sway + D_0015ED6C;
        if (1.0f < infobot->sway) {
            infobot->sway = 1.0f;
        }
        infobot->bob = fast_add_rotations(infobot->bob, D_0015ED6C * 3.1415927f);
        offset.v.x = fast_cos(infobot->bob) * infobot->sway;
        offset.v.y = fast_sin(infobot->bob) * infobot->sway;
        offset.v.z = 0.0f;
        heading = fast_add_rotations(infobot->bob, 1.5707964f);
        moby->rot.z = spring_turn_2d5de8(moby->rot.z, heading, D_0015ED6C * 25.132742f, D_0015ED6C * 25.132742f, D_0015ED6C * 12.566371f, &infobot->spin[2]);
        fast_vec_add(&step, &offset, &p->pts[p->count - 1]);
        MPOS(moby)->q = step.q;
        if (FUN_001f9b48(&D_0013F3D0, &p->pts[p->count - 1]) < 2.0f && D_0013F3D0.active != 0) {
            moby->state = 8;
        }
        moby->rot.y = spring_turn_2d5de8(moby->rot.y, 0.0f, D_0015ED70 * 0.5235988f, D_0015ED70 * 1.0471976f, D_0015ED6C * 0.7853982f, &infobot->spin[1]);
        moby->rot.x = spring_turn_2d5de8(moby->rot.x, 0.0f, D_0015ED70 * 1.0471976f, D_0015ED70 * 2.0943952f, D_0015ED6C * 1.5707964f, &infobot->spin[0]);
        break;
    }
    case INFOBOT_START_SCENE_SEQUENCE:
        moby->flags |= 0x41;
        FUN_L00_002607d0(infobot->planet);
        jet_clear_persistent(moby->mission);
        if (infobot->cutscene != -1) {
            jet_reset_camera(&D_L00_001600EC_2d5de8[infobot->cutscene].pos, &D_L00_001600EC_2d5de8[infobot->cutscene].dir);
        }
        moby->state = 9;
        if (infobot->msg0 != -1) {
            start_scene(infobot->msg0);
        }
        break;
    case INFOBOT_WAIT_TO_DISPATCH_MSG1:
        if (D_L00_0015F5C4 == 2) {
            break;
        }
        moby->state = 10;
        if (infobot->msg1 != -1) {
            FUN_L00_00299460(infobot->msg1);
        }
        break;
    case INFOBOT_WAIT_TO_START_FINAL_SCENE:
        if (D_L00_0015F5C4 == 2) {
            break;
        }
        if (infobot->msg2 != -1) {
            start_scene(infobot->msg2);
        }
        moby->state = 11;
        break;
    case INFOBOT_SAVE_AND_REMOVE:
        if (D_L00_0015F5C4 == 2) {
            return;
        }
        memcard_save_data(0, -1);
        FUN_L00_00262d38(infobot->planet);
        FUN_0020c828(moby);
        return;
    }
    if (infobot->attach != -1 && infobot->carrier != -1) {
        carrier = &D_L00_0015FFD8[infobot->carrier];
        if (carrier->oclass == 0x336) {
            step.q = 0;
            FUN_L00_00260400(moby, carrier, &D_L00_001600EC_2d5de8[infobot->attach].pos, &step,
                             &D_L00_001600EC_2d5de8[infobot->attach].pos, &step);
            fast_vec_add(&point, &offset, &D_L00_001600EC_2d5de8[infobot->attach].pos);
            MPOS(moby)->q = point.q;
        }
    }
}
typedef struct {
    u8 p0[0x10];
    f32 r[4];
    f32 a[4];
    s32 c[4];
    f32 x40, x44, x48, x4c;
} M002d6bf0;
typedef struct {
    u8 p0[0x78];
    M002d6bf0 *m;
} O002d6bf0;
s32 random_integer_below(s32) __asm__("FUN_00213260");
s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
void FUN_L00_002d6bf0(O002d6bf0 *moby) {
    M002d6bf0 *m = moby->m;
    s32 i;
    for (i = 0; i < 4; i++)
        m->r[i] = random_integer_below(0xFF);
    m->a[0] = -1.0f;
    m->a[1] = -2.25f;
    m->a[2] = 1.25f;
    m->a[3] = 2.5f;
    for (i = 0; i < 4; i++)
        m->c[i] = scale_game_frames(i * 0x40 + 0x3F);
    m->x40 = 2.5f;
    m->x44 = 3.0f;
    m->x4c = 2.5f;
    m->x48 = 3.0f;
}
typedef struct {
    char p0[0x10];
    float a[4];
    float b[4];
    int c[4];
    float d[4];
} P_2d6cd0;
extern char camera_position[] __asm__("D_L00_00166DC0") __attribute__((section(".data")));
extern int D_15F5C4_2d6cd0 __asm__("D_L00_0015F5C4");
extern void ff500_2d6cd0(void *, void *, float) __asm__("FUN_001f9bf8");
extern int fast_dec_timer(void *) __asm__("FUN_001f9740");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern float i2f_2d6cd0(int) __asm__("FUN_001fa6c0");
extern float fabs_2d6cd0(float) __asm__("FUN_001f99c0");
extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern int f2i_2d6cd0(float) __asm__("FUN_001fa6d0");
extern void FUN_L00_00272f68(void *, int, int, int, int, int, int, float);

void FUN_L00_002d6cd0(char *moby) {
    P_2d6cd0 *vars = *(P_2d6cd0 **)(moby + 0x78);
    union {
        OvlQuad q;
        float f[4];
    } v;
    float d[4] __attribute__((aligned(16)));
    float e[4] __attribute__((aligned(16)));
    int i, col;
    float f;

    if (*(unsigned short *)(moby + 0x34) & 1) {
        return;
    }
    v.q = *(OvlQuad *)(moby + 0x10);
    v.f[2] += 0.333000004f;
    fast_vec_sub(d, camera_position, moby + 0x10);
    ff500_2d6cd0(d, d, -0.3f);
    ff500_2d6cd0(e, d, 0.1f);
    add_vector_xyz(d, d, moby + 0x10);
    v.f[2] -= 0.333000004f;
    for (i = 0; i < 4; i++) {
        {
            float h = vars->a[i] + vars->b[i];
            vars->a[i] = h;
            if (255.0f <= h) {
                vars->a[i] = h - 255.0f;
            } else if (h <= 0.0f) {
                vars->a[i] = h + 255.0f;
            }
        }
        if (fast_dec_timer(&vars->c[i])) {
            vars->c[i] = scale_game_frames(0xFF);
        }
        f = i2f_2d6cd0(scale_game_frames(0xFF) - vars->c[i]);
        f = f / (float)scale_game_frames(0xFF);
        col = fast_tween_color(0x4040FFFF, 0x1040FFFF, fabs_2d6cd0(0.5f - f));
        if (D_15F5C4_2d6cd0 != 2) {
            FUN_L00_00272f68(d, col, f2i_2d6cd0(vars->a[i]) & 0xFF, 0x35, 1, 2, 0, vars->d[i]);
        }
        add_vector_xyz(d, d, e);
    }
}
typedef struct {
    float x, y, z, w;
} V;
typedef struct {
    s16 start; /* 00 */
    s16 count; /* 02 */
    int idx;   /* 04 */
    char pad08[0x18];
    float unk20, unk24, unk28; /* 20 24 28 */
    s16 h2C, h2E;        /* 2C 2E */
    int x30, x34;        /* 30 34 */
    int col;             /* 38 */
    u8 c0, c1, c2;       /* 3C 3D 3E */
    u8 pad3F;
    s16 extra;  /* 40 */
    s16 flag42; /* 42 */
    float f44, f48, f4C, f50;
    int texture;      /* 54 */
    int mesh_texture; /* 58 */
    u8 mesh_alpha, mesh_red, mesh_green, mesh_blue; /* 5C..5F */
} Q;
typedef struct {
    V v0, v1, v2;
    float pos[3];
    float w;
    u8 pad[0x40];
} T;
typedef struct {
    u8 pad0[4];
    int x04;
    u8 pad08[2];
    u8 b0A;
    u8 b0B;
} E2;
typedef struct {
    u8 pad0[3];
    u8 b03;
    u8 pad4[6];
    s16 h0A;
    u8 pad0C[0x14];
    E2 sub;
} E;
typedef struct {
    u8 pad[0x10];
    V pos;
    u8 state;
    u8 pad21[0xF];
    u8 b30;
    u8 pad31[0x47];
    Q *q;
} P;

extern T *D_L00_001600EC;
/* Particle pool of 1200 entries, one array per field (more fields below FUN_L00_002d73e8). */
V D_L00_001CB7E0[1200] = {0};
V D_L01_001CBC60[1200] = {0};
V D_L14_001CBEE0[1200] = {0};
u8 D_L00_001D2860[1200] = {0};
u8 D_L01_001D2CE0[1200] = {0};
u8 D_L14_001D2F60[1200] = {0};
s16 D_L00_001D5740[1200] = {0};
s16 D_L01_001D5BC0[1200] = {0};
s16 D_L14_001D5E40[1200] = {0};
extern float D_L00_001619B4 __attribute__((sda));
extern float D_L00_001619B8 __attribute__((sda));
extern int D_L00_001619BC __attribute__((sda));
extern int D_L00_001619C0 __attribute__((sda));
extern int D_L00_001619C4 __attribute__((sda));
extern int D_L00_001619C8 __attribute__((sda));
extern float D_L00_001619E0 __attribute__((sda));
extern V D_L00_00166DC0;
void FUN_0020c828_c(P *) __asm__("FUN_0020c828");
float fast_vec_length(void *) __asm__("FUN_001f9af0");
int FUN_L00_002a4dc8(int);
int FUN_00213260_c(int) __asm__("FUN_00213260");
void FUN_L00_002d7bf0(P *, int);
void FUN_L00_002d7d58_c(P *, int) __asm__("FUN_L00_002d7d58");
extern int fast_bsphere_check(void *, float) __asm__("FUN_001fa728");
float vector_distance(void *, void *) __asm__("FUN_001f9b80");
float FUN_L00_00257c48(float, float);
void FUN_001f9d20(V *, V *, void *);
float random_float_between(float, float) __asm__("FUN_002132a8");
int FUN_L00_00257b90(int, int);
E *FUN_L00_0026d000(V *, int, float, float, V *, float, int, float);
s16 FUN_001f96f8_c(int) __asm__("FUN_001f96f8");
void FUN_001f4600_c(void *, P *) __asm__("FUN_001f4600");
void FUN_L00_002d73e8(P *);

void particle_spawner_smoke_fire_update(P *p) __asm__("FUN_L00_002d6f28");
void particle_spawner_smoke_fire_update(P *spawner) {
    enum {
        PARTICLE_SPAWNER_INITIALIZE = 0,
        PARTICLE_SPAWNER_ACTIVE = 1
    };
    Q *q = spawner->q;
    u8 *tbl;
    V v, a, b;
    float g, f;
    int i, j, particle_start, e, col, r;
    E *ent;
    E2 *sub;

    switch (spawner->state) {
    case PARTICLE_SPAWNER_INITIALIZE:
        spawner->state = 1;
        spawner->b30 = 0xFF;
        if (q->idx == -1) {
            FUN_0020c828_c(spawner);
            return;
        }
        q->f48 = fast_vec_length(&D_L00_001600EC[q->idx].v1);
        g = fast_vec_length(&D_L00_001600EC[q->idx].v0);
        if (q->f48 < g)
            q->f48 = g;
        if (q->extra)
            q->f44 = q->f48;
        else
            q->f44 = 3.0f;
        q->f50 = fast_vec_length(&D_L00_001600EC[q->idx].v2) * 0.5f;
        particle_start = q->start = FUN_L00_002a4dc8(q->count + q->extra);
        e = particle_start + q->count;
        for (i = particle_start; i < e; i++) {
            r = FUN_00213260_c(2);
            D_L00_001CB7E0[i].w = 1.0f;
            D_L00_001D2860[i] = r;
            FUN_L00_002d7bf0(spawner, i);
            D_L00_001D5740[i] = 0;
        }
        j = e;
        e = e + q->extra;
        for (i = j; i < e; i++) {
            r = FUN_00213260_c(2);
            D_L00_001CB7E0[i].w = 1.0f;
            D_L00_001D2860[i] = r;
            FUN_L00_002d7d58_c(spawner, i);
        }
        D_L00_001600EC[q->idx].w = 1.0f;
        q->col = q->c2 << 16 | q->c1 << 8 | q->c0;
        return;
    case PARTICLE_SPAWNER_ACTIVE:
        tbl = (u8 *)&D_L00_001600EC[q->idx];
        qcopy(&v, tbl + 0x30);
        v.z += 9.0f;
        v.w = 10.0f;
        if (fast_bsphere_check(&v, D_L00_001619E0) < 0)
            return;
        if (q->flag42 && vector_distance(&D_L00_00166DC0, &spawner->pos) < 128.0f) {
            for (e = 0; e < D_L00_001619BC; e++) {
                a.x = FUN_L00_00257c48(0.0f, 1.0f);
                a.y = FUN_L00_00257c48(0.0f, 1.0f);
                a.z = 1.0f;
                a.w = 1.0f;
                FUN_001f9d20(&b, &a, tbl);
                b.z += 0.5f;
                a.x = FUN_L00_00257c48(0.0f, frame_time * 0.25f);
                a.y = FUN_L00_00257c48(0.0f, frame_time * 0.25f);
                a.z = q->unk20 * frame_time;
                f = random_float_between(q->unk24, q->unk28);
                col = FUN_L00_00257b90(q->h2C, q->h2E) << 24 | D_L00_001619C8;
                r = FUN_L00_00257b90(D_L00_001619C0, D_L00_001619C4);
                ent = FUN_L00_0026d000(&b, r, D_L00_001619B4, 1.0f, &a, D_L00_001619B8, col, f);
                if (ent) {
                    sub = &ent->sub;
                    ent->h0A = FUN_001f96f8_c(FUN_L00_00257b90(q->x30, q->x34));
                    sub->x04 = 3;
                    sub->b0A = col >> 24;
                    sub->b0B = ent->h0A;
                    if (FUN_00213260_c(2))
                        ent->b03 = 0x44;
                }
            }
        }
        FUN_001f4600_c(FUN_L00_002d73e8, spawner);
        return;
    }
}
/* Ported from rac1-decomp (nonmatching/shared/func_L00_002D8898.c: func_L00_002D8898). */
#include "rnc/rendering/geometry_quad.h"
extern float D_L00_001D02E0[];
extern float D_L00_001D15A0[];
extern float D_L00_001D31C0[];
extern float D_L00_001D4480[];
extern s16 D_L00_001D60A0[];
extern u8 D_L00_001D2D10[];
extern s16 D_L00_001D6A00[];
extern float D_L00_00161374;
extern float D_L00_00161384;
extern float D_L00_0016138C;
extern unsigned short D_L00_00161388_gp __asm__("D_L00_00161388") __attribute__((sda));
extern int D_L00_00161998 __attribute__((sda));
extern float D_L00_0016199C __attribute__((sda));
extern u8 D_L00_001619A0[4] __attribute__((sda));
extern u8 D_L00_001619A8[8] __attribute__((sda));
extern int D_L00_001E1380[][4];
extern float D_L00_001E12C0[][4];
extern float D_L00_001E1440[];
extern float D_L00_001E1444[];
extern float D_L00_001E11A0[][4];
extern float D_L00_001E11E0[];
extern float D_L00_001E1200[][4];
extern float D_L00_001E13E0[];
extern float D_L00_001E13E4[];
extern void vu1_add_g_sregister(s32, s64) __asm__("FUN_00233980");
void FUN_L00_002d73e8(P *p) {
    Vec4 position, up;
    Vec4 facing[4], flipped[4];
    struct GeometryQuad quad;
    Q *q = p->q;
    T *volume;
    int mesh_alpha, mesh_color, end;
    Vec4 *transform;
    int alpha, i, k, j, index, l;
    float width, height, phase, one;
    float *vertex;
    Vec4 *front;
    int (*rows)[4];

    mesh_alpha = q->mesh_alpha << 24;
    mesh_color = q->mesh_blue << 16 | q->mesh_green << 8 | q->mesh_red;
    volume = &D_L00_001600EC[q->idx];
    end = q->start + q->count + q->extra;
    qcopy(&position, &p->pos);
    up.q = 0;
    position.f[3] = 1.0f;
    up.f[2] = 1.0f;
    fast_vec_sub(&facing[0], &D_L00_00166DC0, &position);
    fast_vec_normalize(&facing[0], &facing[0], 1.0f);
    fast_vec_cross(&facing[1], &facing[0], &up);
    fast_vec_normalize(&facing[1], &facing[1], 1.0f);
    fast_vec_cross(&facing[2], &facing[1], &facing[0]);
    qcopy(&facing[3], &position);
    /* Fakematch: the do-while(0) gives flipped's address s1 and facing's s2 as retail does. */
    do {
        fast_vec_scale(&flipped[0], &facing[0], -1.0f);
        fast_vec_cross(&flipped[1], &flipped[0], &up);
        fast_vec_normalize(&flipped[1], &flipped[1], 1.0f);
        fast_vec_cross(&flipped[2], &flipped[1], &flipped[0]);
        qcopy(&flipped[3], &facing[3]);
    } while (0);
    /* Fakematch: called as returning int (the draw above needs the s64 prototype) so
     * the texture is sign-extended into the quad as retail does. */
    quad.texture = ((int (*)(int))get_effect_texture)(q->texture + 0x28);
    quad.texture_state = 0xFF9000000260L;
    quad.primitive = 0x8000000048L;
    quad.reserved.value = 0;
    vu1_add_g_sregister(0x4A, 0);
    vu1_add_g_sregister(0x47, 0x51001);
    /* Fakematch: the front copy gives the GCSE copy of facing's address a source with
     * no s2 preference, so the final loop's pointers rotate as retail. */
    front = facing;
    for (i = q->start; i < end; i++) {
        phase = D_L00_001D31C0[i] - D_L00_001D4480[i] * frame_scale;
        D_L00_001D31C0[i] = phase;
        transform = D_L00_001D2860[i] ? flipped : front;
        if (phase <= -8.0f) {
            D_L00_001D31C0[i] = phase + 8.0f;
        }
        transform_vector(&transform[3], &D_L00_001CB7E0[i], volume);
        width = D_L00_001D02E0[i];
        height = D_L00_001D15A0[i];
        if (D_L00_001D2D10[i]) {
            /* Fakematch: reading the index table through rows sets its address ahead of the
             * primitive constant, which then takes v1 as retail. */
            rows = D_L00_001E1380;
            quad.primitive = 0x8000000044L;
            one = 1.0f;
            for (k = 2; k < 4; k++) {
                for (j = 0; j < 4; j++) {
                    index = rows[k][j];
                    vertex = D_L00_001E12C0[index];
                    quad.positions[j].f[0] = vertex[0];
                    quad.positions[j].f[1] = vertex[1] * width;
                    quad.positions[j].f[2] = vertex[2] * height;
                    quad.positions[j].f[3] = one;
                    transform_vector(&quad.positions[j], &quad.positions[j], transform);
                    quad.texture_coordinates[j][0] = D_L00_001E1444[index * 2] + D_L00_001D31C0[i];
                    quad.texture_coordinates[j][1] = D_L00_001E1440[index * 2];
                    quad.colors[j] = mesh_alpha * D_L00_001619A8[index] | mesh_color;
                }
                fast_draw_quad_real(&quad, 0, 0);
            }
        } else {
            if (D_L00_001D5740[i]) {
                fast_dec_timer_s16(&D_L00_001D5740[i]);
                alpha = truncate_float_to_s32((1.0f - convert_integer_to_float(D_L00_001D5740[i]) * D_L00_00161384) * D_L00_0016199C);
            } else if (D_L00_001D60A0[i]) {
                if (fast_dec_timer_s16(&D_L00_001D60A0[i])) {
                    FUN_L00_002d7bf0(p, i);
                    alpha = 0;
                    D_L00_001D2860[i] = random_integer_below(2);
                } else {
                    alpha = truncate_float_to_s32(convert_integer_to_float(D_L00_001D60A0[i]) * D_L00_0016138C * D_L00_0016199C);
                }
            } else {
                if (fast_dec_timer_s16(&D_L00_001D6A00[i])) {
                    D_L00_001D60A0[i] = D_L00_00161388_gp;
                }
                alpha = D_L00_00161998;
            }
            alpha <<= 24;
            quad.primitive = 0x8000000048L;
            one = 1.0f;
            for (l = 0; l < 4; l++) {
                quad.positions[l].f[0] = D_L00_001E11A0[l][0];
                quad.positions[l].f[1] = D_L00_001E11A0[l][1] * width;
                quad.positions[l].f[2] = D_L00_001E11A0[l][2] * height;
                quad.positions[l].f[3] = one;
                quad.texture_coordinates[l][0] = D_L00_001E11E0[l * 2] - D_L00_001D31C0[i];
                quad.texture_coordinates[l][1] = D_L00_001E11E0[l * 2 + 1];
                quad.colors[l] = alpha * D_L00_001619A0[l] | q->col;
                transform_vector(&quad.positions[l], &quad.positions[l], transform);
            }
            fast_draw_quad_real(&quad, 0, 0);
        }
    }
    quad.texture = ((int (*)(int))get_effect_texture)(q->mesh_texture + 0x28);
    quad.primitive = 0x8000000044L;
    qcopy(&facing[3], volume->pos);
    facing[3].f[2] -= q->f50;
    if (q->flag42 == 0) {
        for (k = 0; k < 6; k++) {
            for (j = 0; j < 4; j++) {
                index = D_L00_001E1380[k][j];
                vertex = D_L00_001E1200[index];
                quad.positions[j].f[0] = vertex[0];
                quad.positions[j].f[1] = vertex[1] * q->f48;
                quad.positions[j].f[2] = vertex[2] * q->f4C;
                quad.positions[j].f[3] = 1.0f;
                transform_vector(&quad.positions[j], &quad.positions[j], facing);
                quad.texture_coordinates[j][0] = D_L00_001E13E4[index * 2] + D_L00_00161374;
                quad.texture_coordinates[j][1] = D_L00_001E13E0[index * 2];
                quad.colors[j] = D_L00_001619A8[index] * 0xFF000000 | mesh_color;
            }
            fast_draw_quad_real(&quad, 0, 0);
        }
    }
}

s16 D_L00_001D60A0[1200] = {0};
s16 D_L01_001D6520[1200] = {0};
s16 D_L14_001D67A0[1200] = {0};
s16 D_L00_001D6A00[1200] = {0};
s16 D_L01_001D6E80[1200] = {0};
s16 D_L14_001D7100[1200] = {0};
extern u16 D_L00_00161380_2d7bf0 __asm__("D_L00_00161380");
extern int D_L00_0016137C __asm__("D_L00_0016137C");

float D_L00_001D02E0[1200] = {0};
float D_L01_001D0760[1200] = {0};
float D_L14_001D09E0[1200] = {0};
float D_L00_001D15A0[1200] = {0};
float D_L01_001D1A20[1200] = {0};
float D_L14_001D1CA0[1200] = {0};
float D_L00_001D31C0[1200] = {0};
float D_L01_001D3640[1200] = {0};
float D_L14_001D38C0[1200] = {0};
float D_L00_001D4480[1200] = {0};
float D_L01_001D4900[1200] = {0};
float D_L14_001D4B80[1200] = {0};
u8 D_L00_001D2D10[1200] = {0};
u8 D_L01_001D3190[1200] = {0};
u8 D_L14_001D3410[1200] = {0};
/* Resets particle i of the emitter: random direction, random speeds from
 * the emitter's ranges, and a start delay staggered by index. */
void FUN_L00_002d7bf0(P *spawner, int i) {
    char *v = (char *)spawner->q;
    V *particle_position;
    int emitter_particle_index;
    float sampled_x = FUN_L00_00257c48(0.0f, 1.0f);
    particle_position = &D_L00_001CB7E0[i];
    particle_position->x = sampled_x;
    particle_position->y = FUN_L00_00257c48(0.0f, 1.0f);
    particle_position->z = FUN_L00_00257c48(0.0f, 1.0f);
    D_L00_001D02E0[i] = random_float_between(*(float *)(v + 0x10), *(float *)(v + 0x14));
    D_L00_001D15A0[i] = random_float_between(*(float *)(v + 0x18), *(float *)(v + 0x1C));
    D_L00_001D31C0[i] = 0.0f;
    D_L00_001D4480[i] = random_float_between(*(float *)(v + 0x8), *(float *)(v + 0xC));
    D_L00_001D5740[i] = D_L00_00161380_2d7bf0;
    D_L00_001D60A0[i] = 0;
    emitter_particle_index = i - *(s16 *)v;
    D_L00_001D6A00[i] = (emitter_particle_index + 1) * D_L00_0016137C;
    D_L00_001D2D10[i] = 0;
}

typedef struct {
    float x, y, z, w;
} V_2d7d58;
extern V_2d7d58 D_L00_001CB7E0_2d7d58[] __asm__("D_L00_001CB7E0") __attribute__((section(".data")));
float spread_k(float, float) __asm__("FUN_L00_00257c48");
float FUN_002132a8_2d7d58(float, float) __asm__("FUN_002132a8");
extern float D_L00_001D02E0_lui[] __asm__("D_L00_001D02E0") __attribute__((section(".data")));
extern float D_L00_001D15A0_lui[] __asm__("D_L00_001D15A0") __attribute__((section(".data")));
extern float D_L00_001D31C0_lui[] __asm__("D_L00_001D31C0") __attribute__((section(".data")));
extern float D_L00_001D4480_lui[] __asm__("D_L00_001D4480") __attribute__((section(".data")));
extern u8 D_L00_001D2D10_lui[] __asm__("D_L00_001D2D10") __attribute__((section(".data")));
void FUN_L00_002d7d58(char *spawner, int i) {
    char *v = *(char **)(spawner + 0x78);
    float b;
    V_2d7d58 *particle_position;
    float sampled_x = spread_k(0.0f, 1.0f);
    particle_position = &D_L00_001CB7E0_2d7d58[i];
    particle_position->x = sampled_x;
    particle_position->y = spread_k(0.0f, 1.0f);
    particle_position->z = spread_k(0.0f, 1.0f);
    b = *(float *)(v + 0x4C);
    D_L00_001D02E0_lui[i] =
        FUN_002132a8_2d7d58(*(float *)(v + 0x44), *(float *)(v + 0x44) + 2.0f);
    D_L00_001D15A0_lui[i] = FUN_002132a8_2d7d58(b, b + 4.0f);
    D_L00_001D31C0_lui[i] = 0.0f;
    D_L00_001D4480_lui[i] = FUN_002132a8_2d7d58(0.003f, 0.005f);
    D_L00_001D2D10_lui[i] = 1;
}
#include "eetypes.h"
#include "qcopy.h"
unsigned char *create_moby(int) __asm__("FUN_0020c4f8");
void FUN_L00_0025d1b8(unsigned char *);
void set_moby_color(unsigned char *, int, int, int) __asm__("FUN_L00_002502f0");
float random_angle_radians(void) __asm__("FUN_00213308");
void FUN_L00_00250df8(unsigned char *);
unsigned char *FUN_L00_002d7e90(u128 *v, float s);
unsigned char *FUN_L00_002d7e90(u128 *v, float scale_multiplier) {
    unsigned char *spawned_moby = create_moby(0x307);
    if (spawned_moby) {
        FUN_L00_0025d1b8(spawned_moby);
        set_moby_color(spawned_moby, 0x28, 0x28, 0x46);
        spawned_moby[0x30] = 0xff;
        *(short *)(spawned_moby + 0x32) = 0x40;
        *(float *)(spawned_moby + 0x2c) *= scale_multiplier;
        qcopy(spawned_moby + 0x10, v);
        *(float *)(spawned_moby + 0x48) = random_angle_radians();
        FUN_L00_00250df8(spawned_moby);
    }
    return spawned_moby;
}
void mark_moby_for_removal() __asm__("FUN_0020c828");
/* Grows the float at 0x2C by 2.5% and lowers byte 0x23 by 3; removes the moby below 4. */
void FUN_L00_002d7f38(u8 *moby) {
    *(f32 *)(moby + 0x2C) *= 1.025f;
    if ((u8)(moby[0x23] -= 3) < 4)
        mark_moby_for_removal();
}

f32 D_L01_001FA840[1] __attribute__((section(".data"))) = {0};
f32 D_L01_001FA880[1] __attribute__((section(".data"))) = {0};
f32 D_L14_001E04D0[1] __attribute__((section(".data"))) = {0};
f32 D_L14_001E0510[1] __attribute__((section(".data"))) = {0};
