/* Shared moby class crate (bolt_crate, nanotech_crate, metal_crate, oom_oom_box, ammo_crate): takes hits and gives its contents; settles crates on the ground or on each other, moves stacks, breaks crates and drops their bolts. */
#include "types.h"
#include "rnc/ui/vendor/vendor_item_info.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/mobys/animation/update_moby_animation_state.h"
#include "rnc/mobys/resolve_moby_hit_response.h"
#include "rnc/math/fast_math.h"
#include "rnc/game/item_state.h"
#include "rnc/globals.h"
#include "sda.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/pickups/crate_data/crate_drop_bolts.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/rendering/debug/debug_menu.h"

/* Class header view: pclass + 0xE and + 0x10. */
typedef struct {
    u8  pad00[0xE];
    u8  unkE;
    u8  pad0F;
    u32 unk10;
} Part;

typedef struct Moby Moby;

/* find_moby_hit result: hit moby in the low word of 0x20, flags in the high word. */
typedef struct {
    u8 pad00[0x20];
    union {
        u64 bits;
        struct {
            struct Moby *moby;
            u32          flags;
        } s;
    } hit;
    u8  pad28[0x2C - 0x28];
    f32 damage;
} Contact;

/* Crate update (pvars at moby + 0x78). */
typedef struct {
    u8  pad00[0x1E];
    u16 flags;
} Coll;

typedef struct {
    u8           pad00[0x20];
    Coll         coll;
    u64          unk40;
    u8           pad48[0xA0 - 0x48];
    struct Moby *rider; /* crate stacked on this one */
    struct Moby *below; /* crate this one sits on */
    s32          unkA8;
    u32          flags;
    u8           padB0[0xB4 - 0xB0];
    s32          frameB4;
    u64          unkB8; /* saved moby spawn_frame word */
    u8           padC0[0xC6 - 0xC0];
    s16          unkC6;
    u8           unkC8;
    u8           unkC9;
    u8           unkCA;
    u8           padCB[0xFC - 0xCB];
    s32          frameFC;
} CrateVars;

/* Fourth argument of collect_mobies_in_sphere. */
typedef struct {
    Vec4f        pos;
    struct Moby *moby;
    s32          unk14;
    u8           pad18[0x1C - 0x18];
    f32          unk1C;
    s32          unk20;
} Burst;

typedef unsigned int Q_2cfcb8 __attribute__((mode(TI)));

typedef struct {
    f32 x, y, z, w;
} __attribute__((aligned(16))) V_2cfcb8;

typedef struct M_2cfcb8 {
    u8           p0[0x10];
    V_2cfcb8     pos;
    u8           state;
    u8           group;
    u8           p22[0x30 - 0x22];
    u8           update_dist;
    u8           p31[0x36 - 0x31];
    s16          occlusion;
    u8           p38[0x40 - 0x38];
    f32          rot_x;
    f32          rot_y;
    f32          rot_z;
    u8           p4c[0x78 - 0x4C];
    struct Vars *vars;
    u8           p7c[0xA6 - 0x7C];
    s16          oclass;
    u16          hA8;
    u8           pAA[0xB0 - 0xAA];
    u8           mission;
} M_2cfcb8;

typedef struct Vars {
    u8        p0[0xA0];
    M_2cfcb8 *next_member;
    M_2cfcb8 *pA4;
    u8        pA8[4];
    s32       flags;
    f32       height;
    u8        pB4[0xC0 - 0xB4];
    s32       route_index;
    u16       hC4;
    s16       hC6;
    u8        pC8[4];
    s32       iCC;
    V_2cfcb8  link_pos;
    V_2cfcb8  link_rot;
    M_2cfcb8 *standing_on;
    f32       stack_level;
    s32       iF8;
} Vars;

typedef struct {
    s32 count;
    s32 pad_4[3];
    struct {
        s32 pad_0[3];
        s32 state;
    } entries[1];
} R_2cfcb8;

typedef struct {
    u8        p0[0x18];
    M_2cfcb8 *moby;
    u8        p1c[0x28 - 0x1C];
    f32       point_z;
} H_2cfcb8;

typedef struct {
    s32 v[7];
} T_2d0538;

typedef struct {
    f32 f[4];
} V_2d0538;

typedef struct V2d1428 {
    f32 x, y, z, w;
} __attribute__((aligned(16))) V2d1428;

typedef struct M2d1428 M2d1428;

typedef struct O2d1428 {
    u8              p0[0x40];
    V2d1428         vel;
    u8              p50[0x10];
    u8              v60[0x40];
    struct M2d1428 *next;
    struct M2d1428 *parent;
    u8              pa8[4];
    s32             flags;
    f32             fB0;
    s32             iB4;
    u8              pb8[0x18];
    V2d1428         vD0;
    u8              vE0[0x10];
    void           *path;
    f32             fF4;
} O2d1428;

struct M2d1428 {
    u8       p0[0x10];
    V2d1428  pos;
    u8       state;
    u8       p21[0x15];
    s16      h36;
    V2d1428  v40;
    u8       p50[0x28];
    O2d1428 *vars;
    u8       p7c[0x2A];
    s16      oclass;
    u8       pa8[0x14];
    u8       bBC;
    u8       pbd[3];
    u8       vC0[0x10];
};

typedef struct P2d1428 {
    u8  p0[0x3C];
    u32 flags;
} P2d1428;

typedef struct G2d1428 {
    u8      p0[0x80];
    V2d1428 v80;
    u8      p90[0x190];
    f32     f220;
    u8      p224[0x10];
    f32     f234;
} G2d1428;

typedef union {
    f32  f[4];
    u128 q;
} __attribute__((aligned(16))) V2d09b8;

extern s32            FUN_L00_00257b90(s32, s32);
extern void           set_moby_color(void *, u8, u8, u8) __asm__("FUN_L00_002502f0");
extern void           add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void           fast_vec_normalize(void *, void *, f32) __asm__("FUN_001f9bf8");
extern int D_0015ED84 MACRO_ADDR;

extern void fast_vec_normalize(void *out, void *a, f32 len) __asm__("FUN_001f9bf8");

extern u8                D_L00_00197BC0[];
extern struct MobyClass *D_L00_00197300[];
extern s32 D_0015ED84    MACRO_ADDR;
extern int               D_L00_0015F5CC_macro __asm__("D_L00_0015F5CC") MACRO_ADDR;
extern void             *D_001413D0_lui __asm__("D_001413D0") NOT_SDA;
extern s32 D_001413DC    NOT_SDA;

extern void  update_crate_stack_motion(M2d1428 *) __asm__("FUN_L00_002d1428");
extern void  ground_attached_update(M_2cfcb8 *) __asm__("FUN_L00_002cfcb8");
extern void  create_linked_class_0326_moby(struct Moby *) __asm__("FUN_L00_002d8138");
extern float scale_time(float) __asm__("FUN_001f96b0");
extern void  break_crate(M2d1428 *, s32, s32) __asm__("FUN_L00_002d09b8");
extern void  crate_drop_bolts(u8 *) __asm__("FUN_L00_002d0538");
extern s32   collect_mobies_in_sphere(f32, void *, s32, void *, void *) __asm__("FUN_L00_001f2868");
extern int   FUN_L00_001fefc8(void *);
extern int   get_first_moby_in_group(struct Moby **, int, int, int) __asm__("FUN_L00_002591d0");
extern int
get_next_moby_in_group(struct Moby **, struct Moby *, int, int) __asm__("FUN_L00_002592b8");

/* D_L00_001C40B0 rows (0x18 bytes); unkE is compared with weapon_ammo_counts. */

void crate_update(struct Moby *moby) __asm__("FUN_L00_002cf218");

/* Moby classes 500-502, 505, 511 */
void crate_update(struct Moby *moby) {
    enum {
        CRATE_STATE_INITIALIZE = 0,
        CRATE_STATE_PROCESS_HITS = 1,
        CRATE_STATE_CLASS_CHANGE_COOLDOWN = 2,
        CRATE_STATE_BROKEN = 3,
        CRATE_STATE_REACTIVATION_DELAY = 4,
        CRATE_STATE_DETONATION_COUNTDOWN = 5,
        CRATE_STATE_WAIT_RESPAWN = 6
    };
    Burst        burst;
    Vec4f        at;
    s32          hitKind;
    s32          hitB;
    s32          hitAngle;
    struct Moby *it;
    CrateVars   *crate_vars;
    Contact     *hit_contact;
    Coll        *coll;
    s32          touched;
    s32          doHit;

    crate_vars = (CrateVars *)moby->pvars;
    if (crate_vars->rider != NULL) {
        if (is_moby_in_class_range_500_to_540(crate_vars->rider) == 0) {
            crate_vars->rider = NULL;
        }
    }
    if (crate_vars->below != NULL) {
        if (is_moby_in_class_range_500_to_540(crate_vars->below) == 0) {
            crate_vars->below = NULL;
        }
    }
    crate_vars->frameFC = D_L00_0015F5CC_macro;
    coll = &crate_vars->coll;
    if (crate_vars->rider == NULL) {
        coll->flags |= 1;
    } else {
        coll->flags &= ~1;
    }
    if (crate_vars->frameB4 != D_L00_0015F5CC_macro && moby->state != 0 &&
        (crate_vars->flags & 0xC)) {
        update_crate_stack_motion(moby);
    }
    touched = 0;
    hit_contact = find_moby_hit(moby, 0x1830000, 0);
    if (hit_contact != NULL && hit_contact->hit.s.flags == 0x1000000) {
        hit_contact = NULL;
        touched = 1;
    }
    if (moby->oclass >= 0x1FB && moby->oclass <= 0x1FD) {
        resolve_moby_hit_response(moby, hit_contact, coll, 0, &hitKind, &hitB, &hitAngle, 4);
    }

    switch (moby->state) {
    case CRATE_STATE_INITIALIZE:
        clear_u64_value(&crate_vars->unk40);
        if (D_0015ED84 == 0x12 && moby->oclass == 0x1FF && crate_vars->unkC8 != 0) {
            moby->state = 6;
            moby->flags |= 0x41;
            moby->collision = 0;
            moby->flags &= ~0x1000;
        } else {
            moby->state = 1;
        }
        crate_vars->unkA8 = 0;
        if ((crate_vars->flags ^ 1) & 1) {
            ground_attached_update(moby);
        }
        crate_vars->flags |= 0x14;
        if (moby->oclass == 0x1FF) {
            crate_vars->unkC6 = 100;
        } else if (moby->oclass == 0x1F5) {
            create_linked_class_0326_moby(moby);
        }
        break;

    case CRATE_STATE_PROCESS_HITS:
        if (moby->oclass == 0x1F9 &&
            (hero.unk240 == (s32)moby || hero.coll_hit_moby == (u8 *)moby ||
             (hero.ground_moby == moby && hero.air_frames.s == 0) || hero.ledge_moby == (s32)moby ||
             touched)) {
            moby->command = 0;
            moby->state = 5;
            break;
        }
        if (hit_contact == NULL || !(hit_contact->damage > 0.0f)) {
            break;
        }
        doHit = 1;
        if (moby->oclass == 0x1F6 && !(hit_contact->hit.s.flags & 0x20000)) {
            doHit = 0;
        }
        if (moby->oclass == 0x1F9) {
            moby->state = 5;
            if (hit_contact->hit.s.flags & 0x20000) {
                moby->command = 1;
            } else {
                moby->command =
                    truncate_float_to_s32(scale_time(random_float_between(5.0f, 15.0f)));
            }
            crate_vars->unkB8 = moby->spawn_frame;
            doHit = 0;
        }
        if (moby->oclass >= 0x1FB && moby->oclass <= 0x1FD) {
            if (hit_contact->hit.s.flags & 0x20000) {
                moby->oclass = 0x1FE;
            }
            if (hitKind == 3) {
                moby->oclass = 0x1FE;
            }
            if ((hit_contact->hit.bits & 0x0050000000000000LL) || hitAngle == 0xB4) {
                s32 next_crate_class_id = moby->oclass + 1;

                if (next_crate_class_id >= 0x1FF) {
                    next_crate_class_id = 0x1FE;
                }
                moby->oclass = next_crate_class_id;
            }
            if (moby->oclass != 0x1FE) {
                moby->oclass++;
                moby->class_slot = D_L00_00197BC0[moby->oclass];
                moby->pclass = D_L00_00197300[moby->class_slot];
                update_moby_animation_state(moby);
                moby->unk72 = ((Part *)moby->pclass)->unkE;
                moby->state = 2;
                moby->command = scale_game_frames(15);
                doHit = 0;
            }
        }
        if (doHit == 0) {
            break;
        }
        break_crate(moby, moby->oclass == 0x1F9, hit_contact);
        if (crate_vars->unkC8 != 0 && hit_contact != NULL && hit_contact->hit.s.moby != NULL) {
            s16 hit_moby_class_id = hit_contact->hit.s.moby->oclass;

            if (hit_moby_class_id == 0x2CD || hit_moby_class_id == 0x22C) {
                s16 saved = moby->unkB4;

                moby->unkB4 = 0;
                crate_drop_bolts(moby);
                moby->unkB4 = saved;
                break;
            }
        }
        crate_drop_bolts(moby);
        break;

    case CRATE_STATE_CLASS_CHANGE_COOLDOWN:
        if (--moby->command == 0xFF) {
            moby->hit_slot = 0xFF;
            moby->state = 1;
        }
        break;

    case CRATE_STATE_BROKEN:
        if (moby->oclass == 0x1F9 && crate_vars->unkCA <= scale_game_frames(20)) {
            f32 burst_radius_scale;

            burst.moby = moby;
            burst.unk1C = 2.0f;
            clear_u64_value(&burst.pos);
            burst.unk20 = 0;
            fast_vec_normalize(&at, &moby->rot_row2, 0.5f);
            add_vector_xyz(&at, &at, &moby->pos);
            burst_radius_scale = convert_integer_to_float(crate_vars->unkCA) / scale_time(20.0f);
            burst.unk14 = 1;
            collect_mobies_in_sphere(
                burst_radius_scale + burst_radius_scale, &at, 0x10, moby, &burst);
            burst.unk14 = 0x30000;
            collect_mobies_in_sphere(burst_radius_scale * 3.0f, &at, 0x10, D_001413D0_lui, &burst);
            crate_vars->unkCA++;
            break;
        }
        if (crate_vars->unkC8 != 0) {
            moby->flags |= 1;
            moby->collision = 0;
            moby->flags &= ~0x1000;
            moby->visible = 0;
            if (moby->unkB4 >= 2) {
                moby->unkB4 = 1;
            }
            moby->state = 6;
            if (D_0015ED84 == 0x12) {
                crate_vars->unkC9 = FUN_L00_00257b90(300, 720);
            } else {
                crate_vars->unkC9 = FUN_L00_00257b90(30, 60);
            }
            break;
        }
        mark_moby_for_removal(moby);
        return;

    case CRATE_STATE_WAIT_RESPAWN: {
        qcopy(&burst.pos, &moby->pos);
        burst.pos.z += 0.5f;
        burst.pos.w = 1.0f;
        if (fast_bsphere_check(&burst.pos, moby->draw_dist) != -1) {
            break;
        }
        if (!FUN_L00_001fefc8(&crate_vars->unkC9)) {
            break;
        }
        if (moby->group == 0xFF) {
            moby->state = 1;
            moby->visible = 1;
            moby->flags &= ~1;
            moby->collision = ((Part *)moby->pclass)->unk10;
            moby->flags |= 0x1000;
            break;
        }
        {
            s32 ok = 1;

            if (D_0015ED84 == 0x12) {
                s32 low = 0;
                s32 total = 0;
                s32 i;

                for (i = 0; i < 0x25; i++) {
                    if (item_available[i] != 0) {
                        VendorItemInfo *weapon_item_info = &D_L00_001C40B0[i];

                        total++;
                        if (weapon_item_info->hE != 0 &&
                            (f32)weapon_ammo_counts[i] < (f32)weapon_item_info->hE * 0.8f) {
                            low++;
                        }
                    }
                }
                if ((low < 3 && total >= 3) || (total < 3 && low != total)) {
                    ok = 0;
                }
            }
            if (!ok) {
                break;
            }
        }
        get_first_moby_in_group(&it, moby->group, 0, 0);
        while (it != NULL) {
            if (moby != it && !(vector_distance_xy(&moby->pos, &it->pos) > 0.1f) &&
                it->state == 1) {
                CrateVars *ipv = (CrateVars *)it->pvars;

                if (ipv->rider == NULL) {
                    qcopy(&moby->pos, &it->pos);
                    moby->pos.z += 1.0f;
                    ipv->rider = moby;
                    crate_vars->below = it;
                    crate_vars->rider = NULL;
                    moby->state = 1;
                    moby->visible = 1;
                    moby->flags &= ~1;
                    moby->collision = ((Part *)moby->pclass)->unk10;
                    moby->flags |= 0x1000;
                    crate_vars->flags |= 4;
                    break;
                }
            }
            get_next_moby_in_group(&it, it, 0, 0);
        }
        if (moby->state != 1) {
            moby->pos.z = probe_ground_height(&moby->pos, 0, 0.5f);
            moby->state = 1;
            crate_vars->rider = NULL;
            crate_vars->below = NULL;
            moby->visible = 1;
            moby->flags &= ~1;
            moby->collision = ((Part *)moby->pclass)->unk10;
            moby->flags |= 0x1000;
        }
        break;
    }

    case CRATE_STATE_DETONATION_COUNTDOWN:
        if (moby->command == 0) {
            moby->command = scale_game_frames(60) * 3 - 1;
            crate_vars->unkB8 = moby->spawn_frame;
        } else if (moby->command > scale_game_frames(15) && hit_contact != NULL) {
            moby->command = truncate_float_to_s32(scale_time(random_float_between(5.0f, 15.0f)));
        }
        if (moby->command % scale_game_frames(60) == scale_game_frames(60) - 3 ||
            moby->command == scale_game_frames(13)) {
            allocate_voice_for_target_entry(1, 0x20, (char *)moby);
        }
        if (moby->command % scale_game_frames(60) > scale_game_frames(40) ||
            moby->command < scale_game_frames(15)) {
            set_moby_color(moby, 0xC8, 0x40, 0x40);
        } else {
            moby->spawn_frame = crate_vars->unkB8;
        }
        if (moby->command == 1 || D_001413DC == 0x16) {
            moby->spawn_frame = crate_vars->unkB8;
            break_crate(moby, 1, NULL);
            crate_drop_bolts(moby);
        }
        moby->command--;
        break;

    case CRATE_STATE_REACTIVATION_DELAY:
        if (FUN_L00_001fefc8(&moby->command)) {
            moby->state = 1;
            moby->flags &= ~1;
        }
        break;
    }
    moby->hit_slot = 0xFF;
}

u8 D_L01_00198040 NOT_SDA = 0;

u8 D_L02_001981C0 NOT_SDA = 0;

u8 D_L03_00197CC0 NOT_SDA = 0;
u8 D_L05_00198040 NOT_SDA = 0;

u8 D_L08_001983C0 NOT_SDA = 0;

u8 D_L09_00197D40 NOT_SDA = 0;

u8 D_L10_00198040 NOT_SDA = 0;
u8 D_L11_001985C0 NOT_SDA = 0;

u8 D_L12_00198040 NOT_SDA = 0;
u8 D_L13_00197EC0 NOT_SDA = 0;

u8 D_L14_001982C0 NOT_SDA = 0;

u8 D_L15_001981C0 NOT_SDA = 0;

u8 D_L16_00198040 NOT_SDA = 0;
u8 D_L17_001984C0 NOT_SDA = 0;

extern s32                D_0014EE90_lui[] __asm__("D_0014EE90") NOT_SDA;
extern int D_L00_00173E40 NOT_SDA;
extern R_2cfcb8          *D_L00_001B04B0_lui[] __asm__("D_L00_001B04B0") NOT_SDA;

extern s32 random_integer_below(s32) __asm__("FUN_00213260");
extern s32
FUN_001efa68_2cfcb8(V_2cfcb8 *, V_2cfcb8 *, s32, M_2cfcb8 *, s32) __asm__("FUN_001efa68");
extern s32 FUN_L00_002603d0_2cfcb8(M_2cfcb8 *) __asm__("FUN_L00_002603d0");
extern int convert_pose_to_reference_local(
    M_2cfcb8 *, M_2cfcb8 *, V_2cfcb8 *, f32 *, V_2cfcb8 *, V_2cfcb8 *) __asm__("FUN_L00_00260668");
extern void FUN_001f9a10_2cfcb8(V_2cfcb8 *, V_2cfcb8 *, void *) __asm__("FUN_001f9a10");
extern void raise_kernel_trap(void) __asm__("FUN_001f97b0");

extern int   D_0015ED84;
extern s32   D_L00_0015F598 __asm__("D_L00_0015F598");
extern float D_0015ED70;
extern u8   *D_L00_001B04B0[] NOT_SDA;
void probe_ground_height_cf(u8 *, s32, void *, void *, void *, void *) __asm__("FUN_L00_00260400");
void mark_moby_for_removal_cf(u8 *, s32, s32) __asm__("FUN_L00_00257470");
s32  FUN_L00_00257218_2d0538(u8 *, s32 *) __asm__("FUN_L00_00257218");
f32  random_angle_radians(void) __asm__("FUN_00213308");
s32  solve_quadratic_equation(f32 *, f32 *, f32, f32, f32) __asm__("FUN_L00_002595a0");
void FUN_L00_00261b48(s32, void *, void *, void *);
u8  *spawn_on_path(u8 *, s32, s32, s32) __asm__("FUN_L00_002c52a8");

void            crate_drop_bolts(u8 *a) __asm__("FUN_L00_002d0538");
extern T_2d0538 l00_crate_drop_unused_stack_copy_source __asm__("D_L00_001E9E30");

extern s32 D_L00_0015F5CC;
extern f32 D_0015ED70_alt __asm__("FUN_0015ED70");

extern V2d1428 collision_hit_point __asm__("D_L00_00173E60");
f32            approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
s32            collision_line(void *, void *, s32, s32, s32) __asm__("FUN_001efa68");
void           break_crate(M2d1428 *, s32, s32) __asm__("FUN_L00_002d09b8");
void           FUN_L00_002d0538_c(M2d1428 *) __asm__("FUN_L00_002d0538");
void           transform_attachment_pose(M2d1428 *, void *, void *, void *, void *, void *) __asm__("FUN_L00_002604f0");
P2d1428 *FUN_L00_002603d0(void *);
void     update_entity_matrix(void *, void *, void *, void *) __asm__("FUN_L00_00260738");

extern s32 D_L00_001618D4 __asm__("D_L00_001618D4");
extern s32 D_L00_001618D8 __asm__("D_L00_001618D8");
extern s32 D_L00_0015FFBC __asm__("D_L00_0015FFBC");
extern f32 D_L00_0015F5D0 __asm__("D_L00_0015F5D0");
extern s32 D_001413DC_lui __asm__("D_001413DC") NOT_SDA;
extern int D_001413D0[] NOT_SDA;

extern V2d09b8 D_L00_001DE6F0_2d09b8[] __asm__("D_L00_001DE6F0");
extern V2d09b8 D_L00_001DE750_2d09b8[] __asm__("D_L00_001DE750");
extern u32     D_L00_001E9E80_2d09b8[] __asm__("D_L00_001E9E80");
extern u32     D_L00_001E9E98_2d09b8[] __asm__("D_L00_001E9E98");
extern s32     absolute_int_2d09b8(s32) __asm__("FUN_001f99a8");
extern void    trooper_play_sound(s32, s32, void *, s32) __asm__("FUN_L00_0028dc90");
extern void    FUN_0028dbe8_2d09b8(s32, s32, void *) __asm__("FUN_L00_0028dbe8");
extern void    spawn_spinning_fade_in_out_effect_particle(
       f32, f32, f32, f32, f32, void *, s32, s32) __asm__("FUN_L00_0026b790");
extern void FUN_001ff500_2d09b8(f32, void *, void *) __asm__("FUN_001f9bf8");
extern void *
spawn_crate_debris(void *, void *, void *, void *, s32, s32, f32, f32) __asm__("FUN_L00_002af8b0");
extern void FUN_00250df8_2d09b8(void *) __asm__("FUN_L00_00250df8");
extern void random_vector(f32, f32, void *) __asm__("FUN_L00_00257d78");
extern f32  random_angle_radians(void) __asm__("FUN_00213308");
extern u8   FUN_001fef20_2d09b8(s32) __asm__("FUN_001f96f8");
extern void
spawn_particle(void *, void *, s32, s32, s32, s32, s32, s32, f32, f32) __asm__("FUN_L00_0026a9f0");
extern void spawn_glow_particle(void *, void *, void *, f32, s32, s32, s32, s32, s32) __asm__("FUN_L00_002ac910");

struct Vars;

void ground_attached_update(M_2cfcb8 *moby) __asm__("FUN_L00_002cfcb8");

void ground_attached_update(M_2cfcb8 *moby) {
    V_2cfcb8  probe_top;
    V_2cfcb8  probe_bottom;
    V_2cfcb8  probe_up;
    V_2cfcb8  probe_down;
    M_2cfcb8 *member;
    M_2cfcb8 *other;
    Vars     *vars;
    Vars     *below_vars;
    V_2cfcb8 *src1, *src2, *src3, *src4;
    R_2cfcb8 *route;
    s32       group, flags_before, i, member_flags;
    M_2cfcb8 *below;

    vars = moby->vars;
    group = moby->group;
    vars->flags |= 4;
    if (group == 0xFF) {
        if (moby->oclass == 0x1F4 && random_integer_below(2) != 0 && moby->rot_x == 0.0f &&
            moby->rot_y == 0.0f) {
            moby->rot_z = fast_add_rotations(moby->rot_z, 1.5707964f);
        }
        if (moby->oclass == 0x1F5 && moby->mission != 0xFF &&
            D_0014EE90_lui[moby->mission] < vars->iF8) {
            mark_moby_for_removal(moby);
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
        if (is_moby_in_class_range_500_to_540((*(H_2cfcb8 *)&D_L00_00173E40).moby) != 0)
            return;
        vars->pA4 = 0;
        vars->flags |= 8;
        if (FUN_L00_002603d0_2cfcb8((*(H_2cfcb8 *)&D_L00_00173E40).moby) != 0) {
            if (moby != 0)
                moby->occlusion = 0x7F80;
            convert_pose_to_reference_local(member,
                                            (*(H_2cfcb8 *)&D_L00_00173E40).moby,
                                            &moby->pos,
                                            &moby->rot_x,
                                            &vars->link_pos,
                                            &vars->link_rot);
            vars->stack_level = 0;
            vars->standing_on = (*(H_2cfcb8 *)&D_L00_00173E40).moby;
        }
        return;
    }

    get_first_moby_in_group(&moby, group, 0, 0);
    while (moby != 0) {
        vars = moby->vars;
        if (is_moby_in_class_range_500_to_540(moby) != 0) {
            if (moby->oclass == 0x1F4 && random_integer_below(2) != 0 && moby->rot_x == 0.0f &&
                moby->rot_y == 0.0f) {
                moby->rot_z = fast_add_rotations(moby->rot_z, 1.5707964f);
            }
            if (moby->oclass == 0x1F5 && moby->mission != 0xFF &&
                D_0014EE90_lui[moby->mission] < vars->iF8) {
                mark_moby_for_removal(moby);
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
                    } else if (is_moby_in_class_range_500_to_540(
                                   (*(H_2cfcb8 *)&D_L00_00173E40).moby) == 0 &&
                               (u16)((*(H_2cfcb8 *)&D_L00_00173E40).moby->oclass - 0x128) >= 2) {
                        vars->pA4 = 0;
                        moby->update_dist = 0xFF;
                        vars->flags |= 8;
                    } else if ((*(H_2cfcb8 *)&D_L00_00173E40).moby->vars == 0) {
                        vars->pA4 = 0;
                    } else if ((*(H_2cfcb8 *)&D_L00_00173E40).moby->group != moby->group) {
                        vars->pA4 = 0;
                    } else if (is_moby_in_class_range_500_to_540(
                                   (*(H_2cfcb8 *)&D_L00_00173E40).moby) != 0) {
                        vars->pA4 = (*(H_2cfcb8 *)&D_L00_00173E40).moby;
                        src3 = &moby->pos;
                        below = (*(H_2cfcb8 *)&D_L00_00173E40).moby;
                        qcopy(src3, &below->pos);
                        FUN_001f9a10_2cfcb8(src3, src3, (u8 *)below + 0xE0);
                        vars = (*(H_2cfcb8 *)&D_L00_00173E40).moby->vars;
                        if (is_moby_in_class_range_500_to_540(moby) != 0) {
                            vars->next_member = moby;
                        } else {
                            raise_kernel_trap();
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
        get_next_moby_in_group(&moby, moby, 0, 0);
    }

    get_first_moby_in_group(&member, group, 0, 0);
    while (member != 0) {
        vars = member->vars;
        if (is_moby_in_class_range_500_to_540(member) != 0 && vars->pA4 == 0) {
            src4 = &member->pos;
            qcopy(&probe_up, src4);
            qcopy(&probe_down, src4);
            probe_up.z += vars->height * 0.5f;
            probe_down.z = 0.1f;
            if (FUN_001efa68_2cfcb8(&probe_up, &probe_down, 2, moby, 0) != 0) {
                if ((*(H_2cfcb8 *)&D_L00_00173E40).moby != 0 &&
                    FUN_L00_002603d0_2cfcb8((*(H_2cfcb8 *)&D_L00_00173E40).moby) != 0) {
                    if (moby != 0)
                        moby->occlusion = 0x7F80;
                    convert_pose_to_reference_local(member,
                                                    (*(H_2cfcb8 *)&D_L00_00173E40).moby,
                                                    &member->pos,
                                                    &member->rot_x,
                                                    &vars->link_pos,
                                                    &vars->link_rot);
                    vars->stack_level = 0;
                    vars->standing_on = (*(H_2cfcb8 *)&D_L00_00173E40).moby;
                    vars->flags |= 8;
                }
            }
            for (moby = member; moby != 0; moby = vars->next_member) {
                vars = moby->vars;
                below_vars = 0;
                if (vars->pA4 != 0)
                    below_vars = vars->pA4->vars;
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
                if ((vars->hC6 != 0 || moby->oclass == 0x1F5) && vars->next_member != 0 &&
                    vars->route_index != -1) {
                    route = D_L00_001B04B0_lui[vars->route_index];
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
                            for (i = 0; i < D_L00_001B04B0_lui[vars->route_index]->count; i++) {
                                D_L00_001B04B0_lui[vars->route_index]->entries[i].state = 0;
                            }
                        }
                    }
                }
            }
        }
        get_next_moby_in_group(&member, member, 0, 0);
    }

    get_first_moby_in_group(&moby, group, 0, 0);
    while (moby != 0) {
        if (is_moby_in_class_range_500_to_540(moby) != 0) {
            get_next_moby_in_group(&other, moby, 0, 0);
            while (other != 0) {
                if (is_moby_in_class_range_500_to_540(other) != 0) {
                    fast_vec_dist(&moby->pos, &other->pos);
                }
                get_next_moby_in_group(&other, other, 0, 0);
            }
        }
        get_next_moby_in_group(&moby, moby, 0, 0);
    }
}

void crate_drop_bolts(u8 *crate_moby) {
    enum {
        CRATE_DROP_AUTO_SELECT = 0,
        CRATE_DROP_FALLBACK_ITEM = 1
    };
    T_2d0538 unused_global_copy;
    V_2d0538 trajectory_vector, adjusted_position, scratch_vector;
    s32      drop_type_or_skip_spawn;
    f32      larger_trajectory_time_root, smaller_trajectory_time_root;
    s32      requested_drop_count, i;
    u8      *crate_vars, *spawned_moby, *spawned_vars;
    s16      crate_class;
    f32      launch_angle, horizontal_launch_speed, trajectory_time;

    crate_vars = ((u8 *)MOBY(crate_moby)->pvars);
    crate_class = MOBY(crate_moby)->oclass;
    unused_global_copy = l00_crate_drop_unused_stack_copy_source;
    drop_type_or_skip_spawn = *(s16 *)(crate_vars + 0xC6) == 0;
    if (crate_class == 0x1F5)
        drop_type_or_skip_spawn = 0;
    if (drop_type_or_skip_spawn) {
        clear_u64_value(&trajectory_vector);
        if (*(s32 *)(crate_vars + 0xF0)) {
            if (crate_moby)
                MOBY(crate_moby)->occlusion = 0x7F80;
            probe_ground_height_cf(crate_moby,
                                   *(s32 *)(crate_vars + 0xF0),
                                   crate_moby + 0x10,
                                   crate_moby + 0x40,
                                   &adjusted_position,
                                   &scratch_vector);
            fast_vec_sub(&trajectory_vector, &adjusted_position, crate_moby + 0x10);
        }
        if (D_0015ED84 == 0xF && absolute_float(MOBY(crate_moby)->pos.x - 166.0f) < 6.0f &&
            absolute_float(MOBY(crate_moby)->pos.y - 193.0f) < 6.0f) {
            s32 save = D_L00_0015F598;
            D_L00_0015F598 = 0;
            mark_moby_for_removal_cf(crate_moby, 0x100, -1);
            D_L00_0015F598 = save;
            return;
        }
        mark_moby_for_removal_cf(crate_moby, 0x100, *(s32 *)(crate_vars + 0xC0));
        return;
    }
    if (crate_class != 0x1F5) {
        switch (*(u8 *)(crate_vars + 0xCB)) {
        case CRATE_DROP_FALLBACK_ITEM:
            drop_type_or_skip_spawn = 10;
            break;
        case 6:
            drop_type_or_skip_spawn = 11;
            break;
        case 8:
            drop_type_or_skip_spawn = 13;
            break;
        case 3:
            drop_type_or_skip_spawn = 15;
            break;
        case 2:
            drop_type_or_skip_spawn = 16;
            break;
        case 5:
            drop_type_or_skip_spawn = 17;
            break;
        case 10:
            drop_type_or_skip_spawn = 19;
            break;
        case 4:
            drop_type_or_skip_spawn = 20;
            break;
        case 11:
            drop_type_or_skip_spawn = 23;
            break;
        case 7:
            drop_type_or_skip_spawn = 24;
            break;
        case 9:
            drop_type_or_skip_spawn = 25;
            break;
        case CRATE_DROP_AUTO_SELECT:
        default:
            drop_type_or_skip_spawn = -1;
            break;
        }
        requested_drop_count = FUN_L00_00257218_2d0538(crate_moby, &drop_type_or_skip_spawn);
        for (i = 0; i < requested_drop_count; i++) {
            spawned_moby = spawn_on_path(crate_moby + 0x10, drop_type_or_skip_spawn, -1, 0);
            if (!spawned_moby)
                break;
            spawned_vars = ((u8 *)MOBY(spawned_moby)->pvars);
            *(s16 *)(spawned_vars + 4) = *(s16 *)(crate_vars + 0xC4);
            launch_angle = random_angle_radians();
            horizontal_launch_speed =
                random_float_between(0.0f, crate_drop_horizontal_launch_speed_max_gp) * frame_time;
            *(f32 *)(spawned_vars + 0x10) = fast_cos(launch_angle) * horizontal_launch_speed;
            {
                f32 sn = fast_sin(launch_angle) * horizontal_launch_speed;
                {
                    f32 hi = crate_drop_vertical_launch_speed_max_gp,
                        lo = crate_drop_vertical_launch_speed_min_gp;
                    *(f32 *)(spawned_vars + 0x18) = 0.0f;
                    *(f32 *)(spawned_vars + 0x14) = sn;
                    *(f32 *)(spawned_vars + 0x18) = random_float_between(lo, hi) * frame_time;
                }
            }
            if (*(s32 *)(crate_vars + 0xC0) >= 0) {
                s32 k = solve_quadratic_equation(
                    &larger_trajectory_time_root,
                    &smaller_trajectory_time_root,
                    D_0015ED70 * 10.8f * -0.5f,
                    *(f32 *)(spawned_vars + 0x18) - D_0015ED70 * 10.8f * -0.5f,
                    MOBY(spawned_moby)->pos.z -
                        *(f32 *)(D_L00_001B04B0[*(s32 *)(crate_vars + 0xC0)] + 0x18));
                trajectory_time = 120.0f;
                if ((f32)k >= 1.0f && larger_trajectory_time_root > 0.0f &&
                    larger_trajectory_time_root < 120.0f)
                    trajectory_time = larger_trajectory_time_root;
                fast_vec_scale(&trajectory_vector, spawned_vars + 0x10, trajectory_time);
                fast_vec_add(&trajectory_vector, &trajectory_vector, crate_moby + 0x10);
                FUN_L00_00261b48(*(s32 *)(crate_vars + 0xC0),
                                 crate_moby + 0x10,
                                 &trajectory_vector,
                                 &adjusted_position);
                fast_vec_sub(&trajectory_vector, &adjusted_position, spawned_moby + 0x10);
                trajectory_vector.f[2] = 0.0f;
                fast_vec_normalize(&scratch_vector, &trajectory_vector, 0.25f);
                fast_vec_sub(&trajectory_vector, &trajectory_vector, &scratch_vector);
                fast_vec_scale(&trajectory_vector, &trajectory_vector, 1.0f / trajectory_time);
                *(f32 *)(spawned_vars + 0x10) = trajectory_vector.f[0];
                *(f32 *)(spawned_vars + 0x14) = trajectory_vector.f[1];
            }
        }
    }
    if (MOBY(crate_moby)->unkB4)
        MOBY(crate_moby)->unkB4 = 0;
    mark_moby_for_removal_cf(crate_moby, 0x100, -1);
}

void break_crate(M2d1428 *moby, s32 hit, s32 source) __asm__("FUN_L00_002d09b8");

void break_crate(M2d1428 *moby, s32 hit, s32 source) {
    enum {
        CRATE_VARIANT_OPTIONAL_QUARTER_TURN = 0,
        CRATE_VARIANT_MISSION_THRESHOLD_GATED = 1,
        CRATE_VARIANT_DAMAGE_FLAG_GATED = 2,
        CRATE_VARIANT_IMPACT_TRIGGERED = 5,
        CRATE_VARIANT_CLASS_OVERRIDE_BREAK_SOUND = 6,
        CRATE_VARIANT_INITIAL_DAMAGE_STAGE = 7,
        CRATE_VARIANT_SECOND_DAMAGE_STAGE = 8,
        CRATE_VARIANT_PENULTIMATE_DAMAGE_STAGE = 9,
        CRATE_VARIANT_FINAL_DAMAGE_STAGE = 10,
        CRATE_VARIANT_CONDITIONAL_RESPAWN_START = 11
    };
    u8      *vars = (u8 *)moby->vars;
    V2d09b8  position, away, rotation, basis[3], launch, spawn_pos, spin;
    s32      kind;
    M2d1428 *broken_crate_moby;
    s32      close, count, trail_count, particle, i, choice;
    s32      random_range_lower_bound, random_range_upper_bound, first_color, second_color;
    f32      angle, distance, height, speed, rise;
    u32      colors[6], other_colors[6];
    void    *spawned;
    u32     *effect, *velocity;

    close = 0;
    if (absolute_int_2d09b8(D_L00_0015F5CC - D_L00_001618D4) > 1) {
        if (moby->oclass == 0x1FA)
            trooper_play_sound(0, 0, moby, 500);
        else
            FUN_0028dbe8_2d09b8(0, 0, moby);
        D_L00_001618D4 = D_L00_0015F5CC;
    }
    if (absolute_int_2d09b8(D_L00_0015F5CC - D_L00_001618D8) >= 2)
        D_L00_001618D8 = D_L00_0015F5CC;
    else
        close = 1;
    if (*(u8 **)(vars + 0xA0)) {
        *(u8 **)(*(u8 **)(*(u8 **)(vars + 0xA0) + 0x78) + 0xA4) = *(u8 **)(vars + 0xA4);
        *(u32 *)(*(u8 **)(*(u8 **)(vars + 0xA0) + 0x78) + 0xAC) |= 4;
    }
    if (*(u8 **)(vars + 0xA4)) {
        if (!is_moby_in_class_range_500_to_540(
                *(void **)(*(u8 **)(*(u8 **)(vars + 0xA4) + 0x78) + 0xA0)))
            raise_kernel_trap();
        *(u8 **)(*(u8 **)(*(u8 **)(vars + 0xA4) + 0x78) + 0xA0) = *(u8 **)(vars + 0xA0);
        *(u32 *)(*(u8 **)(*(u8 **)(vars + 0xA4) + 0x78) + 0xAC) |= 4;
    }
    /* Fakematch: the self copy and the two empty tests on angle give retail's spill slots. */
    broken_crate_moby = moby;
    {
        s32 i;
        qcopy(&position, &moby->pos);
        position.f[2] += 0.3f;
        spawn_spinning_fade_in_out_effect_particle(
            0.3f, 1.01f, 1.075f, 0.05f, 50000.0f, &position, 1, 0x40808080);
        for (i = 4; i >= 0; i--)
            spawn_spinning_fade_in_out_effect_particle(
                0.3f, 1.01f, 1.07f, 0.05f, 50000.0f, &position, 1, 0x40808080);
        if (source)
            qcopy(&away, (u8 *)source + 0x10);
        else
            fast_vec_sub(&away, &moby->pos, (*(u8 **)D_001413D0) + 0x10);
        switch ((s16)((u16)moby->oclass - 500)) {
        case CRATE_VARIANT_MISSION_THRESHOLD_GATED:
            kind = 0x15C;
            break;
        case 3:
            kind = 0x15F;
            break;
        case CRATE_VARIANT_DAMAGE_FLAG_GATED:
            if (!angle) {
            }
            kind = 0x162;
            break;
        case CRATE_VARIANT_OPTIONAL_QUARTER_TURN:
        case CRATE_VARIANT_CONDITIONAL_RESPAWN_START:
            kind = 0x165;
            break;
        case 4:
            kind = 0x168;
            break;
        case CRATE_VARIANT_IMPACT_TRIGGERED:
            kind = 0x16B;
            break;
        case CRATE_VARIANT_CLASS_OVERRIDE_BREAK_SOUND:
            kind = 0x16E;
            break;
        case CRATE_VARIANT_INITIAL_DAMAGE_STAGE:
        case CRATE_VARIANT_SECOND_DAMAGE_STAGE:
        case CRATE_VARIANT_PENULTIMATE_DAMAGE_STAGE:
        case CRATE_VARIANT_FINAL_DAMAGE_STAGE:
            kind = 0x171;
            break;
        default:
            kind = 0x95;
            break;
        }
        if (D_L00_0015FFBC >= 201) {
            count = 4;
            trail_count = 4;
        } else if (D_L00_0015FFBC >= 151) {
            count = 2;
            trail_count = 2;
        } else if (D_L00_0015FFBC >= 101) {
            count = 2;
            trail_count = 1;
        } else {
            if (!angle) {
            }
            if (D_L00_0015FFBC >= 51) {
                count = 1;
                trail_count = 0;
            } else {
                count = 1;
                trail_count = 0;
            }
        }
        angle = l00_camera.pos.f[0] - moby->pos.x;
        speed = l00_camera.pos.f[1] - moby->pos.y;
        angle = fast_arc_tan(angle, speed);
        angle = fast_subtract_rotations(angle, moby->v40.z);
        angle += 3.1415925f;
        choice = truncate_float_to_s32(angle / 1.57f);
        angle = fast_normalize_angle((f32)choice * 1.57f);
        clear_u64_value(&rotation);
        rotation.f[2] = angle;
        euler_rotation_basis(&basis, &rotation);
        for (i = 0; i < count; ++i) {
            transform_vector_by_basis(&spawn_pos, &D_L00_001DE6F0_2d09b8[i], &basis);
            fast_vec_add(&spawn_pos, &spawn_pos, &moby->pos);
            qcopy(&spin, &D_L00_001DE750_2d09b8[i]);
            spin.f[2] = fast_add_rotations(spin.f[2], rotation.f[2]);
            fast_vec_sub(&launch, &spawn_pos, &moby->pos);
            launch.f[2] = 0.0f;
            speed = random_float_between(1.0f, 5.0f);
            FUN_001ff500_2d09b8(speed * frame_time, &launch, &launch);
            launch.f[2] = random_float_between(4.0f, 9.0f) * frame_time;
            particle = random_integer_below(2);
            spawned =
                spawn_crate_debris(moby, &launch, &spawn_pos, &spin, kind, particle, 1.0f, 1.0f);
            if (spawned) {
                qcopy((u8 *)spawned + 0x40, &D_L00_001DE750_2d09b8[i]);
                *(f32 *)((u8 *)spawned + 0x48) =
                    fast_add_rotations(*(f32 *)((u8 *)spawned + 0x48), rotation.f[2]);
                FUN_00250df8_2d09b8(spawned);
            }
        }
        for (i = 0; i < trail_count; i++) {
            spawn_pos.f[0] = random_float_between(-0.5f, 0.5f);
            spawn_pos.f[1] = random_float_between(-0.5f, 0.5f);
            spawn_pos.f[2] = random_float_between(0.0f, 1.0f);
            fast_vec_add(&spawn_pos, &spawn_pos, &moby->pos);
            speed = random_float_between(6.0f, 12.0f) * frame_time;
            random_vector(speed, speed, &launch);
            if (launch.f[2] < 0.0f)
                launch.f[2] = -launch.f[2];
            spin.f[0] = random_angle_radians();
            spin.f[2] = random_angle_radians();
            particle = 1;
            if (random_integer_below(6) == 0)
                particle = truncate_float_to_s32(random_float_between(3.0f, 6.0f));
            spawn_crate_debris(moby, &launch, &spawn_pos, &spin, kind + 1, particle, 0.25f, 0.5f);
        }
    }
    *(u32 *)((u8 *)broken_crate_moby + 0x94) = 0;
    moby->state = 3;
    *(u16 *)((u8 *)moby + 0x34) |= 1;
    moby->bBC = FUN_001fef20_2d09b8(15);
    if (hit) {
        if (D_001413DC_lui == 22)
            vars[0xCA] = FUN_001fef20_2d09b8(5);
        else
            vars[0xCA] = 1;
        FUN_001ff500_2d09b8(0.5f, &launch, (u8 *)moby + 0xE0);
        fast_vec_add(&launch, &launch, &moby->pos);
        fast_vec_sub(&spawn_pos, &(*(V2d09b8 *)&g_debug_camera), &moby->pos);
        clear_u64_value(&spin);
        distance = fast_vec_length(&spawn_pos);
        choice = 10;
        if (distance < 8.0f)
            choice = truncate_float_to_s32(distance) + 2;
        height = 0.0f;
        if (distance < 7.0f)
            height = 7.0f - distance;
        if (close && choice > 6)
            choice = 6;
        for (i = 0; i < choice; i++) {
            speed = random_float_between(8.0f, 10.0f);
            __builtin_memcpy(colors, D_L00_001E9E80_2d09b8, sizeof(colors));
            __builtin_memcpy(other_colors, D_L00_001E9E98_2d09b8, sizeof(other_colors));
            rise = speed * frame_time - height * frame_time;
            effect = colors + random_integer_below(6);
            velocity = other_colors + random_integer_below(6);
            random_range_lower_bound = FUN_001fef20_2d09b8(15);
            random_range_upper_bound = FUN_001fef20_2d09b8(20);
            first_color = random_int_between(random_range_lower_bound, random_range_upper_bound);
            random_range_lower_bound = FUN_001fef20_2d09b8(25);
            random_range_upper_bound = FUN_001fef20_2d09b8(30);
            second_color = random_int_between(random_range_lower_bound, random_range_upper_bound);
            spawn_particle(&launch,
                           &spin,
                           *effect,
                           *velocity,
                           first_color,
                           second_color,
                           0,
                           0,
                           400000.0f,
                           rise);
        }
        if (D_L00_0015F5D0 < 0.95f && distance > 9.0f && !close) {
            spawn_glow_particle(
                moby, &launch, &spin, 4.0f, FUN_001fef20_2d09b8(15), 127, 127, 127, 32);
            spawn_glow_particle(
                moby, &launch, &spin, 4.0f, FUN_001fef20_2d09b8(24), 127, 32, 0, 32);
            spawn_glow_particle(
                moby, &launch, &spin, 4.0f, FUN_001fef20_2d09b8(20), 127, 64, 0, 48);
        }
        spawn_glow_particle(moby, &launch, &spin, 3.5f, FUN_001fef20_2d09b8(27), 96, 16, 0, 64);
        spawn_glow_particle(moby, &launch, &spin, 3.0f, FUN_001fef20_2d09b8(29), 32, 0, 0, 32);
    }
}

void update_crate_stack_motion(M2d1428 *moby) __asm__("FUN_L00_002d1428");

void update_crate_stack_motion(M2d1428 *moby) {
    enum {
        CRATE_STATE_BREAKABLE = 1,
        CRATE_STATE_IMPACT_TRIGGERED = 5
    };
    O2d1428 *vars;
    M2d1428 *chain_cursor, *parent_moby, *next;
    V2d1428  collision_start, collision_end, landing_position, hero_local_offset,
        adjusted_link_position, previous_position, previous_rotation, position_delta;
    f32 acceleration_scale_or_speed, v;
    s32 hit;

    vars = moby->vars;
    if (vars->iB4 == D_L00_0015F5CC)
        return;
    chain_cursor = moby;
    do {
        next = chain_cursor;
        chain_cursor = next->vars->parent;
    } while (chain_cursor != 0);
    chain_cursor = next;
    do {
        moby = chain_cursor;
        vars = moby->vars;
        chain_cursor = vars->next;
        if (!(vars->flags & 4))
            vars->flags &= ~0x10;
        vars->flags &= ~4;
        if (!is_moby_in_class_range_500_to_540(moby))
            return;
        if (vars->flags & 2)
            return;
        vars->iB4 = D_L00_0015F5CC;
        if (moby->state == 0)
            continue;
        fast_vec_sub(&hero_local_offset, &hero.motion.pos, &moby->pos);
        transform_vector_by_basis(&hero_local_offset, &hero_local_offset, moby->vC0);
        if (absolute_float(hero_local_offset.x) < 0.55f &&
            absolute_float(hero_local_offset.y) < 0.55f && hero_local_offset.z < 0.0f &&
            -(hero.capsule_top + hero.capsule_radius) < hero_local_offset.z) {
            switch (moby->state) {
            case CRATE_STATE_BREAKABLE:
            case CRATE_STATE_IMPACT_TRIGGERED:
                break_crate(moby, moby->oclass == 0x1F9, 0);
                FUN_L00_002d0538_c(moby);
                break;
            }
        }
        acceleration_scale_or_speed = D_0015ED70_alt;
        vars->vel.z -= acceleration_scale_or_speed * 9.8f;
        vars->vel.z = (vars->vel.z < -0.3f) ? -0.3f : vars->vel.z;
        vars->vel.z = (0.3f < vars->vel.z) ? 0.3f : vars->vel.z;
        if (vars->path) {
            if (vars->parent == 0) {
                vars->vel.w -= acceleration_scale_or_speed * 10.0f;
                if (approach_value(&vars->fF4, 0.0f, absolute_float(vars->vel.w)) == 0.0f)
                    vars->vel.w = 0.0f;
            } else {
                vars->vel.w -= acceleration_scale_or_speed * 10.0f;
                if (approach_value(&vars->fF4,
                                   vars->parent->vars->fF4 + 1.0f,
                                   absolute_float(vars->vel.w)) == 0.0f)
                    *(s32 *)&vars->vel.w = 0;
            }
            qcopy(&adjusted_link_position, &vars->vD0);
            adjusted_link_position.z += vars->fF4;
            if (moby)
                moby->h36 = 0x7F80;
            qcopy(&previous_position, &moby->pos);
            qcopy(&previous_rotation, &moby->v40);
            transform_attachment_pose(
                moby, vars->path, &adjusted_link_position, vars->vE0, &moby->pos, &moby->v40);
            if (FUN_L00_002603d0(vars->path)->flags & 4) {
                qcopy(&vars->vD0, &adjusted_link_position);
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
            qcopy(&collision_end, &moby->pos);
            collision_start.z += vars->fB0 * 0.75f;
            collision_end.z += vars->vel.z;
            hit = collision_line(&collision_start, &collision_end, 2, 0, 0);
            qcopy(&landing_position, &collision_hit_point);
        } else {
            qcopy(&landing_position, &vars->parent->pos);
            landing_position.z += 1.0f;
            hit = 0;
            if (moby->pos.z + vars->vel.z < landing_position.z)
                hit = 1;
        }
        if (hit != 0) {
            acceleration_scale_or_speed = fast_vec_length(&vars->vel);
            if (frame_time * 0.5f < acceleration_scale_or_speed) {
                if (moby->oclass == 0x1F9 && moby->state == 1) {
                    moby->bBC = 0;
                    moby->state = 5;
                } else if ((parent_moby = vars->parent) != 0 && parent_moby->oclass == 0x1F9 &&
                           parent_moby->state == 1) {
                    parent_moby->state = 5;
                    vars->parent->bBC = 0;
                }
            }
            moby->pos.z = landing_position.z;
            vars->vel.z = 0.0f;
        } else {
            vars->flags |= 4;
            fast_vec_add(&moby->pos, &moby->pos, &vars->vel);
        }
    } while (chain_cursor != 0);
}

T_2d0538 D_L01_0020B060 = {
    {0xd6, 0xde, 0xe1, 0xe2, 0xcc, 0xdf, 0xd5}
};

s32 D_L01_0016196C = 0;
s32 D_L01_00161970 = 0;

T_2d0538 D_L02_001FA7D0 = {
    {0xd6, 0xde, 0xe1, 0xe2, 0xcc, 0xdf, 0xd5}
};

s32 D_L02_001618F4 = 0;
s32 D_L02_001618F8 = 0;

T_2d0538 D_L03_001E3070 = {
    {0xd6, 0xde, 0xe1, 0xe2, 0xcc, 0xdf, 0xd5}
};

s32 D_L03_001618B4 = 0;
s32 D_L03_001618B8 = 0;

T_2d0538 D_L04_001DC380 = {
    {0xd6, 0xde, 0xe1, 0xe2, 0xcc, 0xdf, 0xd5}
};

s32 D_L04_0016196C = 0;
s32 D_L04_00161970 = 0;

T_2d0538 D_L05_00218540 = {
    {0xd6, 0xde, 0xe1, 0xe2, 0xcc, 0xdf, 0xd5}
};

s32 D_L05_0016193C = 0;
s32 D_L05_00161940 = 0;

T_2d0538 D_L06_00201230 = {
    {0xd6, 0xde, 0xe1, 0xe2, 0xcc, 0xdf, 0xd5}
};

s32 D_L06_001618F4 = 0;
s32 D_L06_001618F8 = 0;

T_2d0538 D_L07_002111B0 = {
    {0xd6, 0xde, 0xe1, 0xe2, 0xcc, 0xdf, 0xd5}
};

s32 D_L07_001618C4 = 0;
s32 D_L07_001618C8 = 0;

T_2d0538 D_L08_001FC100 = {
    {0xd6, 0xde, 0xe1, 0xe2, 0xcc, 0xdf, 0xd5}
};

s32 D_L08_00161D50 = 0;
s32 D_L08_00161D54 = 0;

T_2d0538 D_L09_00208BE0 = {
    {0xd6, 0xde, 0xe1, 0xe2, 0xcc, 0xdf, 0xd5}
};

s32 D_L09_00161984 = 0;
s32 D_L09_00161988 = 0;

T_2d0538 D_L10_001DCC20 = {
    {0xd6, 0xde, 0xe1, 0xe2, 0xcc, 0xdf, 0xd5}
};

s32 D_L10_001618E0 = 0;
s32 D_L10_001618E4 = 0;

T_2d0538 D_L11_0021B0F0 = {
    {0xd6, 0xde, 0xe1, 0xe2, 0xcc, 0xdf, 0xd5}
};

s32 D_L11_0016198C = 0;
s32 D_L11_00161990 = 0;

T_2d0538 D_L12_0020C5B0 = {
    {0xd6, 0xde, 0xe1, 0xe2, 0xcc, 0xdf, 0xd5}
};

s32 D_L12_00161AB0 = 0;
s32 D_L12_00161AB4 = 0;

T_2d0538 D_L13_001F50C0 = {
    {0xd6, 0xde, 0xe1, 0xe2, 0xcc, 0xdf, 0xd5}
};

s32 D_L13_001619AC = 0;
s32 D_L13_001619B0 = 0;

T_2d0538 D_L14_001FC790 = {
    {0xd6, 0xde, 0xe1, 0xe2, 0xcc, 0xdf, 0xd5}
};

s32 D_L14_00161B80 = 0;
s32 D_L14_00161B84 = 0;

T_2d0538 D_L15_001E31A0 = {
    {0xd6, 0xde, 0xe1, 0xe2, 0xcc, 0xdf, 0xd5}
};

s32 D_L15_00161B4C = 0;
s32 D_L15_00161B50 = 0;

T_2d0538 D_L16_001E8180 = {
    {0xd6, 0xde, 0xe1, 0xe2, 0xcc, 0xdf, 0xd5}
};

s32 D_L16_001619C0 = 0;
s32 D_L16_001619C4 = 0;

T_2d0538 D_L17_001E6950 = {
    {0xd6, 0xde, 0xe1, 0xe2, 0xcc, 0xdf, 0xd5}
};

s32 D_L17_001619E0 = 0;
s32 D_L17_001619E4 = 0;

T_2d0538 D_L18_001F2560 = {
    {0xd6, 0xde, 0xe1, 0xe2, 0xcc, 0xdf, 0xd5}
};

s32 D_L18_00161958 = 0;
s32 D_L18_0016195C = 0;

T_2d0538 l00_crate_drop_unused_stack_copy_source __asm__("D_L00_001E9E30") = {
    {0xd6, 0xde, 0xe1, 0xe2, 0xcc, 0xdf, 0xd5}
};

s32 D_L00_001618D4 __asm__("D_L00_001618D4") = 0;

s32 D_L00_001618D8 __asm__("D_L00_001618D8") = 0;
