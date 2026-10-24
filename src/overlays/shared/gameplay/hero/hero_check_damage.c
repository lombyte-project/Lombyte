/* Level overlay code: stubs become C as functions are matched. */
#include "types.h"
#include "rnc/gameplay/hero_state.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "qcopy.h"
#include "rnc/overlay/collision.h"
#include "rnc/math/circle_offset.h"
#include "rnc/overlay/level_globals.h"

extern CollisionHit coll_hit __asm__("D_L00_00173E40") __attribute__((section(".data")));
/* line query from -> to; fills the collision hit record, nonzero on a hit */
extern int collision_line(void *from, void *to, int mask, void *ignore, int) __asm__("FUN_001efa68");
extern int collision_hit_material(void) __asm__("FUN_001f0b58");
extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");
extern f32 atan2_f(f32, f32) __asm__("FUN_001f9e90");
extern f32 fast_add_rotations(f32, f32) __asm__("FUN_001fa580");
extern f32 fast_difference_between_rotations(f32, f32) __asm__("FUN_001fa688");

/*
 * Ledge probe: steps forward from `pos` along the hero's yaw looking for a
 * flat top (normal within 20 degrees of up, not material 9 or 12) at least
 * `min_height` above the floor and not above pos + up, then for the wall
 * under it. Returns 1 when the hero (facing `*dir`) faces that wall within
 * 65 degrees and the ledge edge is found within 2.4 units along it.
 * Constants live in locals because retail loads each one once.
 */
int probe_hero_ledge_edge(Vec4 *ledge_probe_origin, f32 *facing_yaw) __asm__("FUN_L00_0020c758");

int probe_hero_ledge_edge(Vec4 *ledge_probe_origin, f32 *facing_yaw)
{
    Vec4 probe, top, low, wall, edge;
    f32 drop, fwd, up, reach, min_height;
    f32 ledge_top_yaw, wall_yaw, wall_normal_offset, step, top_margin, max_slope, one, five, half, inc, limit;
    int found, hit, i, j, material;

    if (hero.unk308 == 2)
        return 0;

    drop = -0.7f;
    fwd = 0.3f;
    up = 1.5f;
    reach = 1.15f;
    min_height = 1.8f;
    if (hero.vehicle_mode == 1) {
        min_height = 0.7f;
        drop = -0.4f;
        fwd = 0.25f;
        reach = min_height;
        up = 0.725f;
    }
    wall_yaw = 0.0f;
    found = 0;
    qcopy(&probe, ledge_probe_origin);
    step = 0.25f;
    i = 0;
    top_margin = 0.15f;
    max_slope = 0.34906584f;       /* 20 degrees */
    probe.f[0] += fast_cos(hero.motion.rot.f[2]) * hero.unk234;
    probe.f[1] += fast_sin(hero.motion.rot.f[2]) * hero.unk234;
    do {
        probe.f[0] += fast_cos(hero.motion.rot.f[2]) * fwd * step;
        probe.f[1] += fast_sin(hero.motion.rot.f[2]) * fwd * step;
        qcopy(&top, &probe);
        top.f[2] += up + top_margin;
        qcopy(&low, &probe);
        low.f[2] += reach;
        if (collision_line(&top, &low, 4, hero.moby, 0)) {
            material = collision_hit_material();
            if (material != 9 && material != 12) {
                if (atan2_f(coll_hit.normal_z, vector_length_xy(&coll_hit.normal_x)) < max_slope)
                    found = 1;
            }
        }
        i++;
        if (found)
            break;
    } while (i < 4);
    if (!found)
        return 0;
    if (ledge_probe_origin->f[2] + up < coll_hit.point.f[2])
        return 0;

    qcopy(&top, &coll_hit.point);
    ledge_top_yaw = atan2_f(top.f[0] - ledge_probe_origin->f[0], top.f[1] - ledge_probe_origin->f[1]);
    if (top.f[2] - hero.ground_z.f < min_height)
        return 0;

    /* the wall below the top, tested at five heights */
    five = 5.0f;
    one = 1.0f;
    hit = 0;
    for (j = 0; j < 5; j++) {
        qcopy(&low, ledge_probe_origin);
        qcopy(&wall, &top);
        circle_offset(&wall, ledge_top_yaw, hero.unk234);
        wall.f[2] += drop * (one - j / five);
        low.f[2] = wall.f[2];
        if (collision_line(&low, &wall, 2, 0, 0)) {
            hit = 1;
            wall_yaw = atan2_f(coll_hit.normal_x, coll_hit.normal_y);
            break;
        }
    }
    if (!hit)
        return 0;

    if (fast_difference_between_rotations(*facing_yaw, fast_add_rotations(wall_yaw, 3.14159274f)) < 1.13446403f) {
        /* walk along the wall normal until the top ends */
        limit = 2.4f;
        inc = 0.07f;
        wall_normal_offset = 0.0f;
        half = 0.5f;
        do {
            qcopy(&wall, &top);
            wall.f[2] = top.f[2];
            wall.f[0] += fast_cos(wall_yaw) * wall_normal_offset;
            wall.f[1] += fast_sin(wall_yaw) * wall_normal_offset;
            qcopy(&low, &wall);
            wall.f[2] += half;
            low.f[2] -= half;
            if (collision_line(&wall, &low, 4, hero.moby, 0)) {
                wall_normal_offset += inc;
                continue;
            }
            /* edge found: its point is computed but unused, as in retail */
            qcopy(&edge, &wall);
            wall_normal_offset = inc * half;
            wall_normal_offset = -wall_normal_offset;
            edge.f[2] = top.f[2];
            edge.f[0] += fast_cos(wall_yaw) * wall_normal_offset;
            edge.f[1] += fast_sin(wall_yaw) * wall_normal_offset;
            if (0.52359879f < fast_difference_between_rotations(fast_add_rotations(wall_yaw, 3.14159274f), *facing_yaw))
                return 0;
            return 1;
        } while (wall_normal_offset < limit);
    }
    return 0;
}
struct LedgeSpot_20cd08 {
    Vec4 sphere;
    s32 *owner;
    s32 kind;
    u8 pad18[8];
};

extern struct LedgeSpot_20cd08 *D_L00_0015F70C;
extern s32 D_L00_0015F710;
extern f32 FUN_001f9b48(void *, void *);
extern f32 vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern f32 FUN_001f99c0(f32);
extern s32 FUN_L00_0025df68(void *, void *, void *, s32 *, f32 *, s32, f32, f32, f32);

int nearest_rail_point() __asm__("FUN_L00_0020cd08");
int FUN_L00_0020cd08(Vec4 *from, s32 **outOwner, Vec4 *to, s32 *outI, f32 *outF, s32 *outKind,
                     s32 *skip, s32 *only)
{
    Vec4 copied_query_position;
    s32 gotI;
    f32 gotF;
    s32 i;
    Vec4 *query_position = &copied_query_position;
    f32 distance_limit, height_limit, distance, height;
    struct Hero *hero_state;

    *(u128 *)query_position = *(u128 *)from;
    for (i = 0; i < D_L00_0015F710; i++) {
        if (skip != 0 && D_L00_0015F70C[i].owner == skip)
            continue;
        if (only != 0 && D_L00_0015F70C[i].owner != only)
            continue;
        if (*D_L00_0015F70C[i].owner == 0)
            continue;
        if (FUN_001f9b48(&D_L00_0015F70C[i], query_position) > D_L00_0015F70C[i].sphere.f[3])
            continue;
        if (!FUN_L00_0025df68(D_L00_0015F70C[i].owner, query_position, to, &gotI, &gotF, D_L00_0015F70C[i].kind,
                              12.0f, 10.0f, 0.0f))
            continue;
        hero_state = &hero;
        distance_limit = 0.9f;
        height_limit = 1.5f;
        if ((u32)hero_state->state.control_mode < 2)
            distance_limit = 0.3f;
        if (hero_state->unk1CA != 0) {
            height_limit = 10.0f;
            distance_limit += 0.5f;
        }
        distance = vector_distance(query_position, to);
        height = FUN_001f99c0(query_position->f[2] - to->f[2]);
        if (!(distance < distance_limit && height < height_limit))
            continue;
        *outOwner = D_L00_0015F70C[i].owner;
        *outI = gotI;
        *outF = gotF;
        *outKind = D_L00_0015F70C[i].kind;
        return 1;
    }
    return 0;
}
#include "qcopy.h"
#include "rnc/overlay/hero.h"
#include "rnc/overlay/collision.h"
#include "rnc/overlay/quad.h"
typedef struct {
    f32 x, y, z, w;
} __attribute__((aligned(16))) V_cf58;
extern u8 D_0013D4DD_cf58 __asm__("D_0013D4DD") __attribute__((section(".data")));
f32 FUN_001f9b48_c(void *, V_cf58 *) __asm__("FUN_001f9b48");
#include "rnc/overlay/path.h"
void find_grind_rail(void) __asm__("FUN_L00_0020cf58");
void find_grind_rail(void) {
    V_cf58 proximity_or_rail_point, proximity_or_query_position, third_proximity_point;

    s32 rail_owner, rail_query_integer_value, rail_kind;
    f32 rail_query_value;
    s32 control_mode;
    f32 height_above_rail;
    if (D_0013D4DD_cf58 == 0)
        return;
    hero.grind_rail = 0;
    if (hero.unk1C8 != 0)
        return;
    if (current_level_index == 16) {
        *(OvlQuad *)&proximity_or_rail_point = 0;
        proximity_or_rail_point.x = 115.0f;
        proximity_or_rail_point.y = 264.0f;
        proximity_or_rail_point.z = 133.0f;
        if (12.0f < FUN_001f9b48_c(&hero.motion.pos, &proximity_or_rail_point)) {
            *(OvlQuad *)&proximity_or_query_position = 0;
            proximity_or_query_position.x = 257.0f;
            proximity_or_query_position.y = 301.0f;
            proximity_or_query_position.z = 128.0f;
            if (12.0f < FUN_001f9b48_c(&hero.motion.pos, &proximity_or_query_position))
                return;
        }
    } else if (current_level_index == 14) {
        *(OvlQuad *)&proximity_or_rail_point = 0;
        proximity_or_rail_point.x = 157.0f;
        proximity_or_rail_point.y = 265.0f;
        proximity_or_rail_point.z = 51.0f;
        if (12.0f < FUN_001f9b48_c(&hero.motion.pos, &proximity_or_rail_point)) {
            *(OvlQuad *)&proximity_or_query_position = 0;
            proximity_or_query_position.x = 162.0f;
            proximity_or_query_position.y = 341.0f;
            proximity_or_query_position.z = 48.0f;
            if (12.0f < FUN_001f9b48_c(&hero.motion.pos, &proximity_or_query_position)) {
                *(OvlQuad *)&third_proximity_point = 0;
                third_proximity_point.x = 308.0f;
                third_proximity_point.y = 104.0f;
                third_proximity_point.z = 69.0f;
                if (12.0f < FUN_001f9b48_c(&hero.motion.pos, &third_proximity_point))
                    return;
            }
        }
    } else if (current_level_index == 6) {
        *(OvlQuad *)&proximity_or_rail_point = 0;
        proximity_or_rail_point.x = 177.0f;
        proximity_or_rail_point.y = 304.0f;
        proximity_or_rail_point.z = 143.0f;
        if (14.0f < FUN_001f9b48_c(&hero.motion.pos, &proximity_or_rail_point))
            return;
    }
    control_mode = hero.state.control_mode;
    if (control_mode != 2 && control_mode != 0 && control_mode != 1) {
        if ((u32)(hero.state.current - 41) < 2 &&
            (hero.unk570 != 0 || hero.unk5C4 != 0)) {
        } else {
            if (control_mode != 4)
                return;
            if (hero.unk41E == 0 && hero.unk1CA == 0)
                return;
        }
    }
    *(OvlQuad *)&proximity_or_query_position = *(OvlQuad *)&hero.motion.pos;
    if (!nearest_rail_point((OvlQuad *)&proximity_or_query_position, &rail_owner, &proximity_or_rail_point, &rail_query_integer_value, &rail_query_value, &rail_kind, 0, 0))
        return;
    if (hero.state.current == 41 && hero.unk5C4 != 0 && rail_owner == hero.unk560)
        return;
    height_above_rail = hero.motion.pos.f[2] - proximity_or_rail_point.z;
    if (!(-0.5f < height_above_rail))
        return;
    if (!(height_above_rail < 0.57f) && hero.unk1CA == 0)
        return;
    qcopy(&hero.unk500, &proximity_or_rail_point);
    hero.unk560 = rail_owner;
    hero.unk564 = rail_query_integer_value;
    hero.unk568 = rail_query_value;
    hero.unk584 = rail_kind;
    hero.grind_rail = 1;
}
#define NOT_SDA

#define MACRO_ADDR

/* OUT = the camera-space point (0.3, 0, 1.34) in world space
   (func_L00_00233EE0). */
/* Ported from rac1-decomp (src/overlays/shared/help_0020CDF0.c: func_L00_0020D990), where it is exact; names translated to the US level program. */

extern void transform_hero_local_offset_to_world(float *, float, float, float) __asm__("FUN_L00_00233660");

void compute_hero_fixed_offset_world_position(float *out) __asm__("FUN_L00_0020d2f8");

void compute_hero_fixed_offset_world_position(float *out) {
    transform_hero_local_offset_to_world(out, 0.3f, 0.0f, 1.34f);
}
typedef unsigned int u128_20d330 __attribute__((mode(TI), aligned(16)));
#include "qcopy.h"
typedef union {
    u128_20d330 q;
    float f[4];
} Vec4_20d330;
extern int D_0013CAE4_0020d330[] __asm__("D_0013CAE4");
void FUN_L00_0020d2f8_0020d330(Vec4_20d330 *) __asm__("FUN_L00_0020d2f8");
void update_hero_offset_rail_candidate(void) __asm__("FUN_L00_0020d330");

void update_hero_offset_rail_candidate(void) {
    struct Hero *P = &hero;
    Vec4_20d330 rail_query_position, rail_point;
    Vec4_20d330 *pb;
    int i20, i24, i2C;
    float rail_query_value;
    float query_height_above_rail, minimum_query_height_above_rail;
    P->unk5FC = 0;
    if (P->unk1E4 != 0)
        return;
    FUN_L00_0020d2f8_0020d330(&rail_query_position);
    if ((P->state.control_mode == 4 && P->unk41E == 0) || P->state.control_mode == 2) {
        pb = &rail_point;
        if (nearest_rail_point(&rail_query_position, &i20, pb, &i24, &rail_query_value, &i2C, 0, 0)) {
            float bz, az;
            int fl;
            az = rail_query_position.f[2];
            fl = D_0013CAE4_0020d330[0];
            bz = rail_point.f[2];
            minimum_query_height_above_rail = -0.7f;
            query_height_above_rail = az - bz;
            if (fl & 0x80)
                minimum_query_height_above_rail = -1.2f;
            if (minimum_query_height_above_rail < query_height_above_rail && query_height_above_rail < 0.4f) {
                qcopy(&hero.unk5E0, pb);
                hero.unk5F0 = (s32 *)i20;
                hero.unk5F4 = i24;
                hero.unk5F8 = rail_query_value;
                hero.unk60C = i2C;
                hero.unk5FC = 1;
            }
        }
    }
}
/* Returns the moby of item slot i when the slot is ready (state 2), else 0. */
s32 hero_slot_moby(s32 i) __asm__("FUN_L00_0020d460");
s32 hero_slot_moby(s32 i) {
    /* hero shifted by i slots, so items[0] is slot i */
    struct Hero *slot_owner = (struct Hero *)(((char *)&hero) + i * sizeof(struct HeroItemSlot));
    if (slot_owner->items[0].state == 2) {
        return (s32)slot_owner->items[0].moby;
    }
    return 0;
}
/* Returns the item id of slot i when the slot is ready (state 2), else -1. */
int hero_slot_item_id(int i) __asm__("FUN_L00_0020d498");
int hero_slot_item_id(int i) {
    enum {
        HERO_ITEM_SLOT_STATE_READY = 2
    };
    /* hero shifted by i slots, so items[0] is slot i */
    struct Hero *slot_owner = (struct Hero *)(((char *)&hero) + i * sizeof(struct HeroItemSlot));
    switch (slot_owner->items[0].state) {
    case HERO_ITEM_SLOT_STATE_READY:
        return slot_owner->items[0].item_id;
    }
    return -1;
}
#define NOT_SDA

#define MACRO_ADDR

/* Whether func_L00_0020DB30(ARG) is one of 10, 17, 20 or 25. */
/* Ported from rac1-decomp (src/overlays/shared/help_0020CDF0.c: func_L00_0020DB68), where it is exact; names translated to the US level program. */

extern int FUN_L00_0020d498_u(int) __asm__("FUN_L00_0020d498");

/* True when hero_slot_item_id(arg) is 0x11, 10, 0x14 or 0x19. */
int is_hero_slot_item_id_10_17_20_or_25(int slot_index) __asm__("FUN_L00_0020d4d0");

int is_hero_slot_item_id_10_17_20_or_25(int slot_index) {
    int item_id = FUN_L00_0020d498_u(slot_index);

    if (item_id == 0x11 || item_id == 10 || item_id == 0x14 || item_id == 0x19) {
        return 1;
    }
    return 0;
}
/* True when hero_slot_item_id(a) is 5, 6 or 7. */
s32 is_hero_slot_item_id_5_to_7(int slot_index) __asm__("FUN_L00_0020d518");

s32 is_hero_slot_item_id_5_to_7(int slot_index) {
    return (u32)(FUN_L00_0020d498_u(slot_index) - 5) < 3;
}
int FUN_L00_0020d498_0020d540() __asm__("FUN_L00_0020d498");
/* True when hero_slot_item_id returns 0x1C or 0x1D. */
int is_hero_slot_item_id_28_or_29(void) __asm__("FUN_L00_0020d540");

int is_hero_slot_item_id_28_or_29(void) {
    return (unsigned)(FUN_L00_0020d498_0020d540() - 0x1c) < 2;
}
/* True when hero.state.control_mode is 0x11/0x12 or state.current is 0x6A, 0x75, 0x76 or 0x82. */
s32 FUN_L00_0020d568(void);
s32 FUN_L00_0020d568(void) {
    struct Hero *hero_state = &hero;
    u32 control_mode = hero_state->state.control_mode;
    u32 current_state;
    if (control_mode - 0x11 < 2) {
        goto one;
    }
    current_state = hero_state->state.current;
    if (current_state == 0x6A) {
        goto one;
    }
    if (current_state == 0x82) {
        goto one;
    }
    if (current_state == 0x76) {
        goto one;
    }
    if (current_state != 0x75) {
        goto zero;
    }
one:
    return 1;
zero:
    return 0;
}
/* Returns a, or the byte at p+0xA5 when a is 0xFF. */
s32 FUN_L00_0020d5b8(u8 *moby_bytes, s32 animation_id) {
    if (animation_id == 0xFF)
        animation_id = moby_bytes[0xA5];
    return animation_id;
}
/* FUN_L00_0020d5d0 -- per-mode angle helper.
 *
 * Writes 0.0f to *out, then looks up the mode (0x1C..0x6F) in a jump table.
 * Modes 0x1C/0x1D/0x1F optionally overwrite *out with the sloped angle, modes
 * 0x32/0x33/0x5F/0x60/0x6B/0x6D/0x6E only report "handled", and everything
 * else (including a mode outside the table) falls through to 0.
 *
 * The 25.0f threshold is held in a local so it lands in $f1 and the load in $f0.
 * The case groups are written 33, then 22..., then 0/1/3: gcc emits the blocks
 * in source order, and retail lays them out in exactly that order.
 */

int FUN_L00_0020d5d0(int mode, float *out) __asm__("FUN_L00_0020d5d0");

int FUN_L00_0020d5d0(int mode, float *out) {
    int i;
    *out = 0.0f;
    i = mode - 0x1C;
    switch (i) {
    case 33:
        *out = -0.9599311f;
        return 1;
    case 22:
    case 23:
    case 63:
    case 64:
    case 79:
    case 81:
    case 82:
        return 1;
    case 0:
    case 1:
    case 3: {
        float c;
        c = 25.0f;
        if (c < hero.anim_frame) {
            *out = -1.3089969f;
        }
        return 1;
    }
    }
    return 0;
}
#define NOT_SDA

#define MACRO_ADDR

/* The value BACK entries ago (at most 31, and at most the entry count
   hero.unk21AC) in the 32-float ring hero.unk2128 whose newest entry is
   hero.unk21A8. Reading through a local hero pointer gives retail's
   base-first index add. */
/* Ported from rac1-decomp (src/overlays/shared/help_0020CDF0.c: func_L00_0020DCF0), where it is exact; names translated to the US level program. */

float get_hero_yaw_history(int back) __asm__("FUN_L00_0020d658");

float get_hero_yaw_history(int back) {
    struct Hero *hero_state = &hero;
    int history_lookback = back < 32 ? back : 31;

    if (hero_state->unk21AC < history_lookback) {
        history_lookback = hero_state->unk21AC;
    }
    return hero_state->unk2128[(hero_state->unk21A8 - history_lookback + 31) % 32];
}
typedef struct {
    char p0[0x2084];
    int w2084;
    char p1[0x2D8 - 0x2088 + 0x2000];
} G_20d6b0;
typedef struct {
    char p0[0x140];
    float v140[4];
    char p1[0x154 - 0x150];
    float f154;
    float f158;
} C_20d6b0;
extern C_20d6b0 player_camera __asm__("D_L00_00166C80");
extern float dist_20d6b0(void *, void *) __asm__("FUN_001f9b48");
extern float atan2_20d6b0(float, float) __asm__("FUN_001f9e90");
extern float adiff_20d6b0(float, float) __asm__("FUN_001fa688");
extern float hdist_20d6b0(void *, void *) __asm__("FUN_001f9b80");
extern int FUN_L00_0025e3b8_c(void *) __asm__("FUN_L00_0025e3b8");

float score_hero_target_candidate(char *target_moby, float yaw, float range, float ylim, float plim, int *out) __asm__("FUN_L00_0020d6b0");

float score_hero_target_candidate(char *target_moby, float yaw, float range, float ylim, float plim, int *out) {
    float v[4] __attribute__((aligned(16)));
    struct Hero *hero_state;
    float target_distance, hero_yaw_error, angular_penalty, elevation_angle_error, angular_limit_error, target_score;

    *out = 0;
    qcopy(v, &hero.motion.pos);
    hero_state = &hero;
    if (hero_state->state.current == 0x14) {
        v[2] = hero_state->ground_z.f;
    }
    target_distance = dist_20d6b0(v, target_moby + 0x10);
    if (range < target_distance)
        *out = 1;
    hero_yaw_error = adiff_20d6b0(atan2_20d6b0(*(float *)(target_moby + 0x10) - v[0], *(float *)(target_moby + 0x14) - v[1]), yaw);
    angular_penalty = hero_yaw_error;
    angular_limit_error = hero_yaw_error;
    elevation_angle_error = adiff_20d6b0(atan2_20d6b0(hdist_20d6b0(v, target_moby + 0x10), *(float *)(target_moby + 0x18) - v[2]), 0.0f);
    if (hero_state->state.current == 1 || hero_state->state.current == 0x1E) {
        C_20d6b0 *camera = &player_camera;
        float b = adiff_20d6b0(
            atan2_20d6b0(*(float *)(target_moby + 0x10) - camera->v140[0], *(float *)(target_moby + 0x14) - camera->v140[1]),
            camera->f158);
        elevation_angle_error = adiff_20d6b0(
            atan2_20d6b0(hdist_20d6b0(camera->v140, target_moby + 0x10), *(float *)(target_moby + 0x18) - camera->v140[2]),
            -camera->f154);
        angular_limit_error = elevation_angle_error;
        angular_penalty = b + elevation_angle_error;
        if (elevation_angle_error < b)
            angular_limit_error = b;
    }
    if (0.0f < ylim && ylim < angular_limit_error)
        *out = 1;
    if (0.0f < plim && plim < elevation_angle_error)
        *out = 1;
    target_score = target_distance + angular_penalty * target_distance;
    if (FUN_L00_0025e3b8_c(target_moby))
        target_score += 7.0f;
    return target_score;
}
extern char *active_mobies[] __asm__("D_L00_001ABA00") __attribute__((section(".data")));
float *FUN_002141f8_20d8f8(char *) __asm__("FUN_002141f8");
float score_hero_target_candidate(char *, float, float, float, float, int *) __asm__("FUN_L00_0020d6b0");
char *find_best_hero_heading_target(float range_limit, float reference_yaw, float angular_error_limit, float elevation_angle_limit) __asm__("FUN_L00_0020d8f8");

char *find_best_hero_heading_target(float range_limit, float reference_yaw, float angular_error_limit, float elevation_angle_limit) {
    char *best = 0;
    float bd = 1.0e8f;
    int i = 0;
    char *candidate_moby;
    while ((candidate_moby = active_mobies[i]) != 0) {
        float *candidate_eligibility_value = FUN_002141f8_20d8f8(candidate_moby);
        if (candidate_eligibility_value != 0 && *candidate_eligibility_value != 0.0f && (*(unsigned short *)(candidate_moby + 0x34) & 0x1000)) {
            int out;
            float candidate_score = score_hero_target_candidate(candidate_moby, reference_yaw, range_limit, angular_error_limit, elevation_angle_limit, &out);
            if (out == 0 && candidate_score < bd) {
                bd = candidate_score;
                best = candidate_moby;
            }
        }
        i++;
    }
    return best;
}
/* Maps 1, 2, 3 to 0x79, 0x77, 0x75; others to 0. */
s32 select_hero_condition_animation_id(s32 animation_selector) __asm__("FUN_L00_0020da10");

s32 select_hero_condition_animation_id(s32 animation_selector) {
    switch (animation_selector) {
    case 1:
        return 0x79;
    case 2:
        return 0x77;
    case 3:
        return 0x75;
    }
    return 0;
}
extern unsigned char D_0013D4C6_20da68 __asm__("D_0013D4C6") __attribute__((section(".data")));
unsigned char *FUN_002141f8_20da68(int) __asm__("FUN_002141f8");
int FUN_L00_0020da10_20da68(int) __asm__("FUN_L00_0020da10");
float FUN_001fa6c0_20da68(int) __asm__("FUN_001fa6c0");
int FUN_L00_0020d498_20da68(int) __asm__("FUN_L00_0020d498");
char *FUN_L00_0024f028_20da68(char *, int) __asm__("FUN_L00_0024f028");
float FUN_00213ed8_20da68(void *, float, float) __asm__("FUN_00213ed8");
void FUN_L00_0024f0e8_20da68(char *, char **) __asm__("FUN_L00_0024f0e8");
int FUN_001f96f8_20da68(int) __asm__("FUN_001f96f8");
void FUN_L00_0024f3e0_20da68(char *, char *, int, int, int, int) __asm__("FUN_L00_0024f3e0");
void FUN_L00_0024f190_20da68(char *, char *) __asm__("FUN_L00_0024f190");
void update_hero_condition_animation(void) __asm__("FUN_L00_0020da68");

void update_hero_condition_animation(void) {
    int on, anim, slot_zero_item_id, hero_animation_id;
    float spd, t;
    unsigned char *animation_selector_data;
    char *animation_object;
    char *reloaded_animation_object;
    anim = 0;
    spd = 1.0f;
    on = 0;
    if (hero.unk2274) {
        on = 1;
        animation_selector_data = FUN_002141f8_20da68(hero.unk2274);
        anim = 0x79;
        if (animation_selector_data && animation_selector_data[0x3C]) {
            anim = FUN_L00_0020da10_20da68(animation_selector_data[0x3C]);
            if (hero.state.control_mode == 1)
                spd = 0.7f;
        }
    }
    if (hero.unk1DC) {
        on = 1;
        anim = FUN_L00_0020da10_20da68(hero.unk22CE);
    }
    if (hero.health.hp == 1) {
        on = 1;
        hero_animation_id = (unsigned char)((char *)hero.moby)[0x53];
        if (hero_animation_id == 0x54 || hero_animation_id == 0x82 || hero_animation_id == 0x5A) {
            if (!hero.unkA9C)
                on = 0;
        }
        spd = 0.8f;
        anim = 0x7B;
    }
    if (hero.state.control_mode == 0x11 && !D_0013D4C6_20da68) {
        on = 1;
        anim = 0x78;
        t = FUN_001fa6c0_20da68(hero.unk22A0) / FUN_001fa6c0_20da68(10000);
        t = 1.0f - t;
        spd = t + 0.25f;
        if (1.0f < spd)
            spd = 1.0f;
    }
    slot_zero_item_id = FUN_L00_0020d498_20da68(0);
    if ((unsigned)(slot_zero_item_id - 0xF) < 2 || slot_zero_item_id == 0x13) {
        if (hero.unk20A8) {
            spd = 0.8f;
            on = 1;
            anim = 0x76;
        }
    }
    if (on) {
        if (!hero.unkD10) {
            hero.unkD10 = (u8 *)FUN_L00_0024f028_20da68((char *)hero.moby, 0x18);
            *(int *)(hero.unkD10 + 8) = 0;
        }
    } else {
        if (!hero.unkD10)
            return;
        if (FUN_00213ed8_20da68(hero.unkD10 + 8, 0.0f, frame_scale * 0.05f) == 0.0f) {
            FUN_L00_0024f0e8_20da68((char *)hero.moby, (char **)&hero.unkD10);
        }
    }
    animation_object = hero.unkD10;
    if (animation_object && on) {
        FUN_00213ed8_20da68(animation_object + 8, spd, frame_scale * 0.05f);
        reloaded_animation_object = hero.unkD10;
        if ((unsigned char)reloaded_animation_object[0x23] != anim) {
            FUN_L00_0024f3e0_20da68((char *)hero.moby, hero.unkD10, anim, 0, FUN_001f96f8_20da68(8), 1);
        } else {
            FUN_L00_0024f190_20da68((char *)hero.moby, reloaded_animation_object);
        }
    }
}
#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L00_0020dd20.s", FUN_L00_0020dd20);
#else
/* Near miss (SN BYTES 6/399, size-exact): left is $v1/$a0 swapped for the hero and item-slot
 * address temporaries (the prologue lui and the loop-head lui). */
extern s32 D_L00_0015F6A0_gp[2] __asm__("D_L00_0015F6A0") __attribute__((sda));
extern u8 D_L00_00197BC0_lui[] __asm__("D_L00_00197BC0") __attribute__((section(".data")));
extern s32 D_L00_00197300_20dd20[] __asm__("D_L00_00197300") __attribute__((section(".data")));
extern u8 D_L00_0017CA80_20dd20[] __asm__("D_L00_0017CA80") __attribute__((section(".data")));
struct AngleFields_20dd20 {
    u8 pad[0x5e4];
    f32 unk5E4;
    u8 pad5E8[0x3c];
    f32 unk624, unk628;
    u8 pad62C[0x68];
    f32 unk694;
    u8 pad698[0x3c];
    f32 unk6D4, unk6D8;
};
extern struct AngleFields_20dd20 D_L00_0017A680_20dd20 __asm__("D_L00_0017A680") __attribute__((section(".data")));
extern u8 *FUN_L00_0024f028_20dd20(struct Moby *, s32) __asm__("FUN_L00_0024f028");
extern void FUN_L00_0024f0e8_20dd20(struct Moby *, u8 **) __asm__("FUN_L00_0024f0e8");
extern void FUN_L00_0024f190_20dd20(struct Moby *, u8 *) __asm__("FUN_L00_0024f190");
extern f32 FUN_00213ed8_20dd20(f32 *, f32, f32) __asm__("FUN_00213ed8");
extern s32 return_hero_animation_id_unchanged(s32) __asm__("FUN_L00_002050b8");
extern s32 FUN_L00_0020d5d0_20dd20(s32, f32 *) __asm__("FUN_L00_0020d5d0");
extern s32 FUN_L00_0020d5b8_20dd20(struct Moby *, s32) __asm__("FUN_L00_0020d5b8");
extern s32 FUN_001f96f8_20dd20(s32) __asm__("FUN_001f96f8");

extern u8 *hero_slots[2] __asm__("D_00140050") __attribute__((section(".data")));

void FUN_L00_0020dd20(void) {
    struct Hero *h = &hero;
    s32 i;

    i = 0;
    do {
        u8 *o;
        u8 *t;
        s32 diff;
        struct { s16 v; } w;
        s32 r;
        s32 k;
        s32 j;
        u8 *ids;
        f32 out;
        struct Hero *p6;

        /* Fakematch: the empty loops put i in $s7 and the item base in $fp. */
        do { do { t = hero_slots[i]; } while (0); } while (0);

        if (t != 0 && *(s32 *)(t + 0x34) != 0) {
            if (FUN_00213ed8_20dd20((f32 *)(t + 8), 0.0f, frame_scale * 0.1f) == 0.0f) {
                FUN_L00_0024f0e8_20dd20(h->moby, &hero_slots[i]);
            }
        } else {
            s32 c = h->unk20AB;
            s32 want = 0;

            if (c != 0 && (i == 0 || c == 2)) {
                want = 1;
            }
            if (hero.unk20AF != 0) {
                u8 *slot;
                want = 0;
                slot = hero_slots[i];
                if (slot != 0) {
                    *(s32 *)(slot + 0x34) = 1;
                }
            }
            if (want) {
                struct Hero *p4 = &hero;
                if (hero_slots[i] == 0) {
                    u8 *slot = FUN_L00_0024f028_20dd20(p4->moby, D_L00_0015F6A0_gp[i]);
                    hero_slots[i] = slot;
                    *(f32 *)(slot + 8) = 0.0f;
                    *(s32 *)(hero_slots[i] + 0x18) = D_L00_00197300_20dd20[D_L00_00197BC0_lui[i + 1]];
                }
            } else {
                struct Hero *p5 = &hero;
                u8 *slot = hero_slots[i];
                if (slot == 0) {
                    continue;
                }
                if (FUN_00213ed8_20dd20((f32 *)(slot + 8), 0.0f, frame_scale * 0.1f) == 0.0f) {
                    FUN_L00_0024f0e8_20dd20(p5->moby, &hero_slots[i]);
                }
            }
        }

        o = hero_slots[i];
        p6 = &hero;
        if (o == 0) {
            continue;
        }
        {
            u8 *mb = (u8 *)p6->moby;
            diff = mb[0x52] != mb[0x53];
        }
        if (p6->unkD08[0] != 0 && p6->unkD14 == 0) {
            FUN_00213ed8_20dd20((f32 *)(o + 8), 0.0f, frame_scale * 0.1f);
        } else if (*(s32 *)(o + 0x34) == 0) {
            FUN_00213ed8_20dd20((f32 *)(o + 8), 1.0f, frame_scale * 0.1f);
        }

        w.v = 0;
        if (diff && hero.unkA9C != 0) {
            s32 a = return_hero_animation_id_unchanged(o[0x23]);
            s32 b = return_hero_animation_id_unchanged(((u8 *)hero.moby)[0x53]);
            if (a != b) {
                w.v = o[0x22] != o[0x23];
            }
        }
        *(s16 *)(o + 0x30) = w.v;

        r = FUN_L00_0020d5d0_20dd20(((u8 *)h->moby)[0x53], &out);
        if (r != 0 || o[0x32] != 0) {
            if (out != 0.0f) {
                f32 x = frame_scale_sq;
                u8 *tbl = (u8 *)&D_L00_0017A680_20dd20;
                *(f32 *)(tbl + 0x694) = out;
                *(f32 *)(tbl + 0x5E4) = out;
                *(f32 *)(tbl + 0x6D4) = x * 0.05f;
                *(f32 *)(tbl + 0x6D8) = x * 0.3f;
                *(f32 *)(tbl + 0x624) = x * 0.05f;
                *(f32 *)(tbl + 0x628) = x * 0.3f;
            }
            if (r != 0 && (o[0x32] == 0 || o[0x23] != 0)) {
                {
                    if (o[0x22] == o[0x23] || 0.5f < *(f32 *)(o + 0x24)) {
                        o[0x22] = o[0x23];
                        o[0x20] = o[0x21];
                    }
                    *(f32 *)(o + 0x24) = 0.0f;
                    o[0x23] = 0;
                    o[0x21] = 0;
                    *(f32 *)(o + 0x28) = 1.0f;
                    *(f32 *)(o + 0x2C) = 1.0f / (f32)FUN_001f96f8_20dd20(0xF);
                    o[0x32] = 1;
                    o[0x33] = 0;
                }
            }
            if (r == 0) {
                if (o[0x33] == 0) {
                    t = (u8 *)h->moby;
                    o[0x23] = FUN_L00_0020d5b8_20dd20((struct Moby *)t, t[0x53]);
                    o[0x21] = 0;
                    o[0x33] = 1;
                    *(f32 *)(o + 0x24) = 0.0f;
                    *(f32 *)(o + 0x28) = 0.0f;
                } else {
                    f32 f = *(f32 *)(o + 0x24) + frame_scale * 0.1f;
                    *(f32 *)(o + 0x24) = f;
                    if (1.0f < f) {
                        *(f32 *)(o + 0x24) = 1.0f;
                    }
                    if (*(f32 *)(o + 0x24) == 1.0f) {
                        o[0x32] = 0;
                    }
                }
            } else {
                *(f32 *)(o + 0x28) = 1.0f;
            }
            for (j = 0; j < 2; j++) {
                ids = o + 0x22;
                if (ids[j] < 0x17) {
                    o[6 + j] = 1;
                    *(u8 **)(o + 0x38 + j * 4) = D_L00_0017CA80_20dd20 + (i * 2 + j) * 0x400;
                } else {
                    o[6 + j] = 0;
                }
            }
            /* Fakematch: the empty loops change loop numbering, which moves the register allocation. */
            do { do { do { do { FUN_L00_0024f190_20dd20(h->moby, o); } while (0); } while (0); } while (0); } while (0);
        } else {
            u8 *enabled;

            if (*(s16 *)(o + 0x30) == 0) {
                for (k = 0; k < 2; k++) {
                    if (!diff || k == 1) {
                        u8 *p = o + 0x22;
                        t = (u8 *)hero.moby;
                        p[k] = FUN_L00_0020d5b8_20dd20((struct Moby *)t, *(t + k + 0x52));
                    }
                }
            }
            if (*(s16 *)(o + 0x30) != 0) {
                f32 f = *(f32 *)(o + 0x24) + frame_scale * 0.15f;
                *(f32 *)(o + 0x24) = f;
                if (1.0f <= f) {
                    o[0x22] = o[0x23];
                    o[0x20] = o[0x21];
                    *(s16 *)(o + 0x30) = 0;
                }
            } else {
                if (!diff) {
                    o[0x20] = ((u8 *)h->moby)[0x50];
                }
                o[0x21] = ((u8 *)h->moby)[0x51];
                *(f32 *)(o + 0x24) = *(f32 *)((u8 *)h->moby + 0x54);
            }
            enabled = o + 6;
            for (j = 0; j < 2; j++) {
                ids = o + 0x22;
                if (ids[j] < 0x17) {
                    enabled[j] = 1;
                    *(u8 **)(o + 0x38 + j * 4) = D_L00_0017CA80_20dd20 + (i * 2 + j) * 0x400;
                } else {
                    enabled[j] = 0;
                }
            }
            /* Fakematch: the empty loops change loop numbering, which moves the register allocation. */
            do { do { do { do { FUN_L00_0024f190_20dd20(h->moby, o); } while (0); } while (0); } while (0); } while (0);
        }
    } while (++i < 2);
}
#endif
typedef struct {
    u8 pad_0[0x28];
    s32 animation;         /* 0x28 */
    s32 animation_mode_12; /* 0x2C: used while state.control_mode is 12 */
    u8 pad_30[0x1C];
} R_0020e360;
extern R_0020e360 item_defs_lui[] __asm__("D_L00_00179AC0") __attribute__((section(".data")));
extern s32 D_L00_0015F6A0_gp[2] __asm__("D_L00_0015F6A0") __attribute__((sda));
s32 FUN_L00_0020d498_0020e360(s32) __asm__("FUN_L00_0020d498");
u8 *FUN_L00_0024f028_0020e360(u8 *, s32) __asm__("FUN_L00_0024f028");
s32 FUN_001f96f8_0020e360(s32) __asm__("FUN_001f96f8");
void FUN_L00_0024f3e0_0020e360(u8 *, u8 *, s32, s32, s32, s32) __asm__("FUN_L00_0024f3e0");
void create_missing_hero_item_slot_animations(s32 suppress_slot_animation_creation, s32 enable_slot_animation_creation) __asm__("FUN_L00_0020e360");

void create_missing_hero_item_slot_animations(s32 suppress_slot_animation_creation, s32 enable_slot_animation_creation) {
    s32 item_id_or_animation_id, i;
    u8 *slot_animation_object;
    if (hero.vehicle_mode)
        return;
    if (!hero.unk20A8)
        return;
    if (!hero.unk20AA)
        return;
    if (suppress_slot_animation_creation)
        return;
    if (!enable_slot_animation_creation)
        return;
    item_id_or_animation_id = FUN_L00_0020d498_0020e360(0);
    item_id_or_animation_id = item_defs_lui[item_id_or_animation_id].animation;
    if (item_id_or_animation_id == -1)
        return;
    for (i = 0; i < 2; i++) {
        if (hero.unkD08[i] != 0)
            continue;
        if (i == 1 && hero.unk20AB != 2)
            break;
        {
            slot_animation_object = FUN_L00_0024f028_0020e360((u8 *)hero.moby, D_L00_0015F6A0_gp[i]);
            hero.unkD08[i] = slot_animation_object;
            if (slot_animation_object) {
                hero.unkD14 = 0;
                *(s32 *)(slot_animation_object + 8) = 0;
                FUN_L00_0024f3e0_0020e360((u8 *)hero.moby, slot_animation_object, item_id_or_animation_id, 0,
                                          FUN_001f96f8_0020e360(0xF), 1);
                *(f32 *)(slot_animation_object + 0x28) = 1.0f;
            }
        }
    }
}

typedef struct {
    u8 p0[0x18];
    s32 unk18;             /* copied to hero.unk20AB; 2 animates both item slots */
    u8 p1[0x24 - 0x1C];
    s32 unk24;
    s32 animation;         /* 0x28 */
    s32 animation_mode_12; /* 0x2C: used while state.control_mode is 12 */
    s32 unk30;             /* copied to hero.unk20AA */
    u8 p2[0x4C - 0x34];
} ID_20e4c8;

extern ID_20e4c8 D_L00_00179AC0_c[] __asm__("D_L00_00179AC0");
extern s32 D_L00_0015F6A0_gp[2] __asm__("D_L00_0015F6A0") __attribute__((sda));
extern s32 FUN_L00_0020d498_c(s32) __asm__("FUN_L00_0020d498");
extern s32 FUN_001f96f8_c(s32) __asm__("FUN_001f96f8");
extern u8 *allocate_moby_animation_slot(u8 *, s32) __asm__("FUN_L00_0024f028");
extern void release_moby_animation_slot(u8 *, u8 **) __asm__("FUN_L00_0024f0e8");
extern void initialize_moby_animation_blend(u8 *, u8 *, s32, s32, s32, s32) __asm__("FUN_L00_0024f3e0");
extern void FUN_L00_002323b8_c(s32, s32, f32) __asm__("FUN_L00_002323b8");

void start_equipped_item_animation(void) __asm__("FUN_L00_0020e4c8");
void start_equipped_item_animation(void) {
    s32 item_id, hero_animation_id, animation_step_divisor, slot_animation_id, i;
    u8 *slot_animation_object;
    struct Hero *P;
    item_id = FUN_L00_0020d498_c(0);
    if (item_id == -1 || item_id == 0)
        return;
    P = &hero;
    P->unk2298 = item_id;
    hero_animation_id = D_L00_00179AC0_c[item_id].unk24;
    slot_animation_id = D_L00_00179AC0_c[item_id].animation;
    if ((s32)P->state.control_mode == 12)
        slot_animation_id = D_L00_00179AC0_c[item_id].animation_mode_12;
    P->unk20A8 = 1;
    {
        s32 k = D_L00_00179AC0_c[item_id].unk30;
        P->unk20AA = k;
    }
    animation_step_divisor = FUN_001f96f8_c(10);
    if (D_L00_00179AC0_c[item_id].unk18 == 0)
        animation_step_divisor = FUN_001f96f8_c(11);
    if (P->state.control_mode == 0 && P->unk22C8 == 0) {
        P->unk2294 = hero_animation_id;
        FUN_L00_002323b8_c(hero_animation_id, 0, (f32)animation_step_divisor);
        return;
    }
    if (slot_animation_id == -1)
        return;
    for (i = 0; i < 2 && (i != 1 || hero.unk20AB == 2); i++) {
        if (hero.unkD08[i] != 0) {
            release_moby_animation_slot((u8 *)hero.moby, &hero.unkD08[i]);
        }
        slot_animation_object = allocate_moby_animation_slot((u8 *)hero.moby, D_L00_0015F6A0_gp[i]);
        hero.unkD08[i] = slot_animation_object;
        if (slot_animation_object != 0) {
            hero.unk2294 = slot_animation_id;
            hero.unkD14 = 0;
            *(f32 *)(slot_animation_object + 8) = 1.0f;
            initialize_moby_animation_blend((u8 *)hero.moby, slot_animation_object, hero.unk2294, 0, animation_step_divisor, 1);
        }
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/help_0020CDF0.c: func_L00_0020ED30), where it is exact; names translated to the US level program. */

extern int hero_idle_sequence(int) __asm__("FUN_L00_00205000");
extern void set_hero_animation(int, int, float) __asm__("FUN_L00_002323b8");

void cancel_hero_item_animation(void) __asm__("FUN_L00_0020e698");
void cancel_hero_item_animation(void) {
    struct Hero *hero_state = &hero;
    if (hero_state->unk20A8 != 0) {
        if (hero_state->unkD08[0] != 0) {
            hero_state->unkD14 = 1;
        } else if (hero_state->state.current == 0) {
            unsigned char *hero_moby = (unsigned char *)hero_state->moby;
            if (hero_moby[0x53] == hero_state->unk2294) {
                int idle_animation_id = hero_idle_sequence(0);
                set_hero_animation(idle_animation_id, 0, -2.0f);
            }
        }
        hero.unk20A8 = 0;
        hero.unk20AA = 0;
    }
}
/* Ported from rac1-decomp (src/overlays/shared/help_0020CDF0.c: func_L00_0020EDC0). */

extern int get_ammo(s32) __asm__("FUN_L00_00233e98");
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
extern float approach_value(float *, float, float) __asm__("FUN_00213ed8");
extern void advance_moby_animation_blend(void *, unsigned char *) __asm__("FUN_L00_0024f190");
extern void vu_euler_rotation_basis(void *, void *) __asm__("FUN_001fa030");
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
extern char D_0013F990[];

#define HERO_ROTATION(h) (*(u128 *)((u8 *)(h) + 0x680))

void update_hero_held_items_and_orientation(void) __asm__("FUN_L00_0020e728");

void update_hero_held_items_and_orientation(void) {
    struct Hero *owner = &hero;
    int slot_index;

    if (owner->unk20A8 != 0 && owner->unk20AA != 0) {
        if (get_ammo(-1) == 0) {
            cancel_hero_item_animation();
        }
    }

    for (slot_index = 0; slot_index < 2; slot_index++) {
        struct Hero *slot_owner = &hero;
        unsigned char **slot = slot_owner->unkD08 + slot_index;
        unsigned char **current;
        int item_id;
        int animation;

        if (*slot == 0) {
            continue;
        }
        item_id = hero_slot_item_id(0);
        if (item_id == slot_owner->unk2298) {
            animation = item_defs_lui[item_id].animation;
            if (slot_owner->state.control_mode == 12) {
                animation = item_defs_lui[item_id].animation_mode_12;
            }
            if (animation != -1 && (*slot)[0x23] != animation) {
                slot_owner->unk2294 = animation;
                initialize_moby_animation_blend((u8 *)slot_owner->moby, *slot, slot_owner->unk2294, 0, scale_game_frames(8), 1);
            }
        }

        current = slot_owner->unkD08 + slot_index;
        {
        struct Hero *pose_owner = &hero;
        advance_moby_animation_blend(pose_owner->moby, *current);

        if (pose_owner->unkD14 != 0) {
            if (pose_owner->unkD14 == 2) {
                approach_value((float *)(*current + 8), 0.0f, frame_scale * 0.25f);
            } else {
                approach_value((float *)(*current + 8), 0.0f, frame_scale * 0.07f);
            }
            owner = &hero;
            if (*(float *)(*slot + 8) != 0.0f) {
                continue;
            }
            release_moby_animation_slot((u8 *)owner->moby, slot);
            return;
        } else {
            approach_value((float *)(*current + 8), 1.0f, frame_scale * 0.2f);
            if (((*current)[5] & 2) == 0) {
                continue;
            }
            if (pose_owner->unk20AA == 0) {
                pose_owner->unkD14 = 1;
                *(int *)(*current + 0x28) = 0;
                pose_owner->unk20A8 = 0;
                return;
            }
        }
        }
    }

    {
    struct Hero *pose_owner = &hero;
    if (pose_owner->state.control_mode == 15) {
        char *target = (char *)pose_owner->unk5D8;

        if (target != 0) {
            HERO_ROTATION(pose_owner) = pose_owner->motion.rot.q;
            pose_owner->unk688 = atan2_f(*(float *)(target + 0x10) - pose_owner->motion.pos.f[0],
                                   *(float *)(target + 0x14) - pose_owner->motion.pos.f[1]);
            pose_owner->unk684 = -pose_owner->unk580;
        } else {
            HERO_ROTATION(pose_owner) = pose_owner->motion.rot.q;
            pose_owner->unk688 = fast_add_rotations(pose_owner->unk57C, pose_owner->unk5D0);
            pose_owner->unk684 = -pose_owner->unk580;
        }
        {
            char *basis = D_0013F990;
            vu_euler_rotation_basis(basis, basis + 0x40);
            clear_u64_value(basis + 0x30);
            *(float *)(basis + 0x30) = 1.0f;
            transform_vector_by_basis(basis + 0x30, basis + 0x30, basis);
        }
    } else {
        char *moby = (char *)pose_owner->moby;
        u128 *basis = (u128 *)pose_owner->basis;
        float *offset = &pose_owner->unk670;
        basis[0] = *(u128 *)(moby + 0xC0);
        basis[1] = *(u128 *)(moby + 0xD0);
        basis[2] = *(u128 *)(moby + 0xE0);
        clear_u64_value(offset);
        pose_owner->unk670 = 1.0f;
        transform_vector_by_basis(offset, offset, basis);
        HERO_ROTATION(pose_owner) = pose_owner->motion.rot.q;
    }
    }
}
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/gameplay/entities/moby.h"

/* spawn_hero_item_mobies: spawns the hero item mobies (wrench into items[0] /
 * its own slot, items[3] and its second moby, items[2], items[1], and the
 * items[4]/[5] gadgets) when their slots are empty.
 *
 * Item definition table (0x4C bytes per item id). */
typedef struct {
    u8 pad0[0x8];
    s32 slot;      /* 0x08: hero item slot the item goes into */
    u8 padC[0x4];
    s32 cls;       /* 0x10: moby class of the item */
    s32 cls2;      /* 0x14: moby class of its second moby */
    s32 unk18;     /* 0x18: read as a word and stored as a byte; the codegen
                    * (address formed base + offset first) needs a word field */
    u8 pad1C[0x30];
} ItemDef_ea80;

extern ItemDef_ea80 item_defs_ea80[] __asm__("D_L00_00179AC0") __attribute__((section(".data")));
extern s32 D_L00_0015F3F8_ea80 __asm__("D_L00_0015F3F8");
extern s32 D_0015ED90;
extern s32 D_0015ED94;
extern s32 D_00141660_ea80[4] __asm__("D_00141660") __attribute__((section(".data")));
extern u8 item_available[] __asm__("D_0013D4C0") __attribute__((section(".data")));
extern struct Moby *create_moby(s32) __asm__("FUN_0020c4f8");
extern void select_world_object_resource_tables(s32, s32) __asm__("FUN_00204a40");
extern void memset_words(void *, s32, s32) __asm__("FUN_001f97e8");
extern void blend_moby_animation_ea80(void *, s32, s32, s32) __asm__("FUN_00212f90");
extern void spawn_indexed_class_a7_moby(s32) __asm__("FUN_L00_002b4138");

void spawn_hero_item_mobies(void) __asm__("FUN_L00_0020ea80");
void spawn_hero_item_mobies(void) {
    struct Moby *created_moby;
    s32 id;
    s32 slot;
    struct Hero *slot_owner;
    s32 i;
    struct Moby *slot_two_moby;
    struct Moby *slot_four_moby;
    struct Moby *slot_four_secondary_moby;
    struct Moby *slot_five_moby;
    s32 id3;
    s32 fallback_item_id;

    if (hero.items[0].moby == 0 && D_L00_0015F3F8_ea80 > hero.items[0].unk14 &&
        (hero.pending_item[0] == 0 || hero.pending_item[0] == 0x24)) {
        id = hero.selected_item[0];
        if (id == 0) {
            if (D_0015ED90 != 0 || (fallback_item_id = D_00141660_ea80[0]) == 0)
                id = 8;
            else
                id = fallback_item_id;
        }
        select_world_object_resource_tables(item_defs_ea80[id].cls, -1);
        for (i = 0; i < 3; i++) {
            memset_words(&hero.unk18F0[i], 0, sizeof(struct HeroEase));
            hero.unk18F0[i].unkA0 = -1;
        }
        hero.items[0].item_id = id;
        slot = item_defs_ea80[id].slot;
        slot_owner = (struct Hero *)((char *)&hero + slot * sizeof(struct HeroItemSlot));
        created_moby = create_moby(item_defs_ea80[slot_owner->items[0].item_id].cls);
        if (created_moby != 0) {
            created_moby->draw_dist = 0x20;
            created_moby->visible = 1;
            created_moby->spawn_frame = hero.moby->spawn_frame;
            if (((u8 *)created_moby->pclass)[6] != 0)
                created_moby->unk73 = 0x18;
            slot_owner->items[0].moby = created_moby;
            slot_owner->items[0].state = 2;
            if (id == 8) {
                slot_owner->items[0].button_mask = 0x80;
                blend_moby_animation_ea80(created_moby, 1, 0, 1);
            } else {
                slot_owner->items[0].button_mask = 0x20;
            }
            /* The slot address is rebuilt here instead of reusing h; using h
             * changes the register allocation away from retail. */
            hero.unk20AB = item_defs_ea80[((struct Hero *)((char *)&hero + slot * sizeof(struct HeroItemSlot)))->items[0].item_id].unk18;
        }
    }

    if (hero.items[3].moby == 0) {
        id3 = hero.selected_item[3];
        if (id3 == 0) {
            id3 = D_00141660_ea80[3];
            if (id3 == 0)
                id3 = D_0015ED94 != 0 ? 3 : 2;
        }
        hero.items[3].item_id = id3;
        created_moby = create_moby(item_defs_ea80[id3].cls);
        if (created_moby != 0) {
            created_moby->draw_dist = 0x20;
            created_moby->visible = 1;
            created_moby->spawn_frame = hero.moby->spawn_frame;
            if (((u8 *)created_moby->pclass)[6] != 0)
                created_moby->unk73 = 0x18;
            hero.items[3].state = 2;
            hero.items[3].moby = created_moby;
            if (hero.unk22D8 != 0)
                created_moby->flags |= 0x41;
            if (hero.unk22D8 == 0 && hero.items[3].item_id == 3) {
                spawn_indexed_class_a7_moby(0);
                spawn_indexed_class_a7_moby(1);
            }
        }
    }

    if (hero.items[3].moby2 == 0) {
        created_moby = create_moby(item_defs_ea80[1].cls);
        if (created_moby != 0) {
            created_moby->draw_dist = 0x20;
            created_moby->visible = 1;
            created_moby->spawn_frame = hero.moby->spawn_frame;
            if (((u8 *)created_moby->pclass)[6] != 0)
                created_moby->unk73 = 0x18;
            hero.items[3].state = 2;
            hero.items[3].moby2 = created_moby;
            if (hero.unk22D8 != 0)
                created_moby->flags |= 0x41;
        }
    }

    if (hero.items[2].moby == 0 && (D_00141660_ea80[2] != 0 || hero.selected_item[2] != 0)) {
        if (hero.selected_item[2] != 0)
            hero.items[2].item_id = hero.selected_item[2];
        else
            hero.items[2].item_id = D_00141660_ea80[2];
        slot_two_moby = create_moby(item_defs_ea80[hero.items[2].item_id].cls);
        if (slot_two_moby != 0) {
            slot_two_moby->draw_dist = 0x20;
            slot_two_moby->visible = 1;
            slot_two_moby->spawn_frame = hero.moby->spawn_frame;
            slot_two_moby->flags |= 2;
            slot_two_moby->update = 0;
            if (((u8 *)slot_two_moby->pclass)[6] != 0)
                slot_two_moby->unk73 = 0x18;
            slot_two_moby->flags |= 0x800;
            hero.items[2].moby = slot_two_moby;
            hero.items[2].state = 2;
        }
    }

    if (hero.items[1].moby == 0 && D_00141660_ea80[1] != 0) {
        hero.items[1].item_id = D_00141660_ea80[1];
        created_moby = create_moby(item_defs_ea80[D_00141660_ea80[1]].cls);
        if (created_moby != 0) {
            created_moby->draw_dist = 0x20;
            created_moby->visible = 1;
            created_moby->spawn_frame = hero.moby->spawn_frame;
            if (((u8 *)created_moby->pclass)[6] != 0)
                created_moby->unk73 = 0x18;
            hero.items[1].moby = created_moby;
            hero.items[1].state = 2;
        }
        created_moby = create_moby(item_defs_ea80[D_00141660_ea80[1]].cls2);
        if (created_moby != 0) {
            created_moby->draw_dist = 0x20;
            created_moby->visible = 1;
            created_moby->spawn_frame = hero.moby->spawn_frame;
            if (((u8 *)created_moby->pclass)[6] != 0)
                created_moby->unk73 = 0x18;
            hero.items[1].moby2 = created_moby;
            hero.items[1].state = 2;
        }
    }

    if (hero.items[4].moby == 0 && item_available[0x21] != 0) {
        slot_four_moby = create_moby(item_defs_ea80[0x21].cls);
        hero.items[4].item_id = 0x21;
        if (slot_four_moby != 0) {
            slot_four_moby->draw_dist = 0x20;
            slot_four_moby->visible = 1;
            slot_four_moby->spawn_frame = hero.moby->spawn_frame;
            if (((u8 *)slot_four_moby->pclass)[6] != 0)
                slot_four_moby->unk73 = 0x18;
            hero.items[4].moby = slot_four_moby;
            hero.items[4].state = 2;
        }
    }

    if (hero.items[4].moby2 == 0 && item_available[0x22] != 0) {
        slot_four_secondary_moby = create_moby(item_defs_ea80[0x22].cls);
        hero.items[4].item_id = 0x22;
        if (slot_four_secondary_moby != 0) {
            slot_four_secondary_moby->draw_dist = 0x20;
            slot_four_secondary_moby->visible = 1;
            slot_four_secondary_moby->spawn_frame = hero.moby->spawn_frame;
            hero.items[4].moby2 = slot_four_secondary_moby;
            hero.items[4].state = 2;
        }
    }

    if (hero.items[5].moby == 0 && item_available[0x23] != 0) {
        slot_five_moby = create_moby(item_defs_ea80[0x23].cls);
        hero.items[5].item_id = 0x23;
        if (slot_five_moby != 0) {
            slot_five_moby->draw_dist = 0x20;
            slot_five_moby->visible = 1;
            slot_five_moby->spawn_frame = hero.moby->spawn_frame;
            hero.items[5].moby = slot_five_moby;
            hero.items[5].state = 2;
        }
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/help_0020CDF0.c: func_L00_0020F750), where it is exact; names translated to the US level program. */

float absolute_float(float input) __asm__("func_001F99C0");

float sum_basis_absolute_differences(float *first_components, float *second_components) __asm__("FUN_L00_0020f0b8");

float sum_basis_absolute_differences(float *first_components, float *second_components) {
    float absolute_difference_sum = 0.0f;
    int i, j;
    for (i = 0; i < 3; i++) {
        float *second_component_cursor = (float *)((i << 4) + (int)second_components);
        float *first_component_cursor = (float *)((i << 4) + (int)first_components);
        for (j = 0; j < 3; j++) {
            absolute_difference_sum += absolute_float(*first_component_cursor++ - *second_component_cursor++);
        }
    }
    return absolute_difference_sum;
}
/* Advances the 16-slot history ring, scores slots against each other and averages the agreeing ones into arg+0x40. */
/* Ported from rac1-decomp (src/overlays/shared/help_0020CDF0.c: func_L00_0020F7F8), where it is exact; names translated to the US level program. */

typedef struct { float x, y, z, w; } __attribute__((aligned(16))) V4_F7F8;

extern char D_0013E533_lui[] __asm__("D_0013E533") __attribute__((section(".data")));
extern float sum_basis_absolute_differences(float *a, float *b) __asm__("FUN_L00_0020f0b8");
extern int D_L00_0015F694 __attribute__((sda));
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void clear_u64_value_c(void *) __asm__("func_001F99F8");
extern void copy_matrix3x4_alt() __asm__("FUN_001fa2b8");
extern void fast_vec_scale(void *, void *, f32) __asm__("func_001F9A68");
extern void matrix_to_euler(void *, void *) __asm__("FUN_00214598");
void normalize_vector_triplet(f32 *vectors) __asm__("FUN_00214128");
extern struct Hero hero_lui __asm__("D_0013F350") __attribute__((section(".data")));

void update_hero_item_rotation_from_history(char *rotation_output_moby, u8 *entry) __asm__("FUN_L00_0020f160");

void update_hero_item_rotation_from_history(char *rotation_output_moby, u8 *entry) {
    V4_F7F8 vv[4];
    float fa[16];
    int ri;
    int above_threshold_sample_count;
    float *sums;
    char *out;
    int history_window_size, i, k, acc;
    float mx;

    {
    char *b = ((char *)((char *)&hero_lui));
    copy_matrix3x4_alt(((char *)((char *)&hero_lui)) + 0x12F0 + *(int *)(b + 0x21B8) * 0x30);
    normalize_vector_triplet(((char *)((char *)&hero_lui)) + 0x12F0 + *(int *)(b + 0x21B8) * 0x30);

    *(int *)(b + 0x21B8) = (*(int *)(b + 0x21B8) + 1) % 16;
    *(int *)(b + 0x21BC) = *(int *)(b + 0x21BC) + 1;
    if (*(int *)(b + 0x21BC) > 16) {
        *(int *)(b + 0x21BC) = 16;
    }
    }

    for (k = 0; k < 3; k++) {
        clear_u64_value_c(&vv[k]);
    }

    {
    char *b = ((char *)((char *)&hero_lui));
    if (*(int *)(b + 0x208C) == 0x16) {
        D_L00_0015F694 = 14;
    } else if (*(int *)(b + 0x2084) == 2) {
        D_L00_0015F694 = 5;
    } else {
        D_L00_0015F694 = 11;
    }
    }

    {
    char *b = ((char *)((char *)&hero_lui));
    if (*(int *)(b + 0x21C0) < D_L00_0015F694) {
        *(int *)(b + 0x21C0) = *(int *)(b + 0x21C0) + 1;
    } else if (D_L00_0015F694 < *(int *)(b + 0x21C0)) {
        *(int *)(b + 0x21C0) = *(int *)(b + 0x21C0) - 1;
    }
    }

    {
    char *b = ((char *)((char *)&hero_lui));
    history_window_size = *(int *)(b + 0x21C0);
    if (history_window_size < 2) {
        history_window_size = 2;
    }
    if (*(int *)(b + 0x21BC) < history_window_size) {
        return;
    }
    }

    out = rotation_output_moby + 0x40;
    above_threshold_sample_count = 0;

    for (i = 0; i < history_window_size; i++) {
        char *b = ((char *)((char *)&hero_lui));
        int t;
        sums = fa;
        t = i - 15;
        ri = (*(int *)(b + 0x21B8) - t) % 16;
        for (k = 0; k < history_window_size; k++) {
            if (i != k) {
                char *b = ((char *)((char *)&hero_lui));
                int comparison_ring_index_offset = k - 15;
                int rj = (*(int *)(b + 0x21B8) - comparison_ring_index_offset) % 16;
                *(float *)((char *)sums - (-(i * 4))) += sum_basis_absolute_differences(
                    (float *)(((char *)((char *)&hero_lui)) + 0x12F0 + ri * 0x30),
                    (float *)(((char *)((char *)&hero_lui)) + 0x12F0 + rj * 0x30));
            }
        }
    }

    mx = 0.0f;
    for (i = 0; i < history_window_size; i++) {
        if (mx < fa[i]) {
            mx = fa[i];
        }
    }

    for (i = 0; i < history_window_size; i++) {
        if (mx * 0.2f < fa[i]) {
            above_threshold_sample_count++;
        }
    }

    acc = 0;
    for (i = 0; i < history_window_size; i++) {
        if (mx * 0.2f < fa[i] && above_threshold_sample_count > 0 && above_threshold_sample_count < 3) {
            continue;
        }
        acc++;
        {
        char *b = ((char *)((char *)&hero_lui));
        int t = i - 15;
        ri = (*(int *)(b + 0x21B8) - t) % 16;
        for (k = 0; k < 3; k++) {
            add_vector_xyz(&vv[k], &vv[k], ((char *)((char *)&hero_lui)) + 0x12F0 + ri * 0x30 + k * 16);
        }
        }
    }

    for (k = 0; k < 3; k++) {
        fast_vec_scale(&vv[k], &vv[k], 1.0f / (float)acc);
    }
    matrix_to_euler(vv, out);
}
#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L00_0020f580.s", FUN_L00_0020f580);
#else
extern s32 D_L00_0015F5C4_f580 __asm__("D_L00_0015F5C4");
extern s32 D_L00_0015F5CC_f580 __asm__("D_L00_0015F5CC");
extern u8 D_00179AC0_f580[] __asm__("D_L00_00179AC0");
extern u8 D_0013FE10_f580[] __asm__("D_0013FE10");
extern u8 D_0013FE40_f580[] __asm__("D_0013FE40");
extern u8 D_0017A5C0_f580[] __asm__("D_L00_0017A5C0");
extern u8 D_L00_0017A608[] __asm__("D_L00_0017A608");
extern u8 D_0017A650_f580[] __asm__("D_L00_0017A650");
extern u8 D_0017A668_f580[] __asm__("D_L00_0017A668");
extern u8 D_0017C480_f580[] __asm__("D_L00_0017C480");
extern u8 D_L00_0017C680[] __asm__("D_L00_0017C680");
extern u8 D_0017C880_f580[] __asm__("D_L00_0017C880");
extern u8 D_0017C980_f580[] __asm__("D_L00_0017C980");
extern u8 D_0017A960_f580[] __asm__("D_L00_0017A960");
extern u8 D_L00_0015F6A8[] __asm__("D_L00_0015F6A8");
extern s32 D_001404DC_f580 __asm__("D_001404DC");
extern u8 D_001410D0_f580[] __asm__("D_001410D0");
extern u8 opt_mirror_anim __asm__("D_0015EDB5");
extern s32 large_font_height __asm__("D_L00_0015F690");
extern void FUN_00250480_f580(void *) __asm__("FUN_0020d580");
extern void FUN_00250df8_f580(void *) __asm__("FUN_L00_00250df8");
extern void FUN_001ffcd8_f580(void *, void *) __asm__("FUN_001fa2b8");
extern void FUN_0025c170_f580(void *) __asm__("FUN_00214128");
extern void FUN_001ff6d0_f580(void *, void *, void *) __asm__("FUN_001f9cf8");
extern f32 FUN_001ff408_f580(void *) __asm__("FUN_001f9b20");
extern f32 FUN_001ff8b0_f580(f32, f32) __asm__("FUN_001f9e90");
extern void FUN_00250f98_f580(void *) __asm__("FUN_0020e098");
extern void FUN_L00_002092c8_f580(void *, void *, s32, s32, s32) __asm__("FUN_L00_002092c8");
extern void FUN_0025d330_f580(void *, void *) __asm__("FUN_00214598");
extern void FUN_001ff3c0_f580(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void *FUN_0024e838_f580(s32) __asm__("FUN_0020c4f8");
extern void FUN_0024f728_f580(void *, s32, void *) __asm__("FUN_0020cca8");
extern void enqueue_hero_glow_draw_callback_once(void) __asm__("FUN_L00_00207d00");
extern s32 FUN_001fef20_f580(s32) __asm__("FUN_001f96f8");
extern f32 FUN_00200240_f580(s32) __asm__("FUN_001fa6c0");
extern f32 FUN_001ff800_f580(f32) __asm__("FUN_001f9de0");
extern s32 FUN_00200250_f580(f32) __asm__("FUN_001fa6d0");
extern s32 FUN_L00_0020d540_f580(s32) __asm__("FUN_L00_0020d540");

void FUN_L00_0020f580(void)
{
    union { f32 direction[4]; u8 saved[0x20]; } scratch __attribute__((aligned(16)));
    struct { u8 position[0x30]; u8 rotation[0x10]; } transform __attribute__((aligned(16)));
    s32 i, j, index, offset, first, second, third, random_range;
    u8 *definition;
    s32 color_red, color_green, color_blue;
    u8 *moby, *effect;
    u8 *volatile source;
    u8 *slot;
    f32 angle;

    i = 0;
    do {
        if ((D_L00_0015F5C4_f580 != 2 && D_L00_0015F5C4_f580 != 6) || i == 4 || i == 5) {
            offset = i * 0x50;
            source = transform.position;
            slot = (u8 *)&hero + offset;
            j = 0;
            do {
                moby = *(u8 **)(slot + 0x1090);
                if (j == 1) moby = *(u8 **)(slot + 0x1094);
                if (moby != 0) {
                    *(u64 *)(moby + 0x38) = *(u64 *)((u8 *)hero.moby + 0x38);
                    definition = D_00179AC0_f580 + *(s32 *)(slot + 0x10b8) * 0x4c;
                    index = *(s32 *)(definition + 0xc);
                    if (i == 1 && j == 1) index = 3;
                    if (*(u8 *)(slot + 0x10aa) != 0) {
                        *(u128 *)((u8 *)&hero + offset + 0x1070) = *(u128 *)(D_0013FE40_f580 + index * 0x40);
                        FUN_0025d330_f580(D_0013FE10_f580 + index * 0x40, (u8 *)&hero + offset + 0x1080);
                        FUN_00250480_f580(moby);
                        FUN_00250df8_f580(moby);
                        *(u16 *)(moby + 0x34) &= 0xfffb;
                    } else {
                        *(u128 *)(moby + 0x10) = *(u128 *)(D_0013FE40_f580 + index * 0x40);
                        if (large_font_height == 0 || *(s32 *)(D_00179AC0_f580 + *(s32 *)(slot + 0x10b8) * 0x4c + 0x18) == 0) {
                            first = is_hero_slot_item_id_10_17_20_or_25(i);
                            second = is_hero_slot_item_id_5_to_7(i);
                            third = FUN_L00_0020d540_f580(i);
                            if (first == 0 && second == 0 && third == 0) FUN_00250480_f580(moby);
                            FUN_00250df8_f580(moby);
                            FUN_001ffcd8_f580(moby + 0xc0, D_0013FE10_f580 + index * 0x40);
                            if (first == 0 && second == 0 && third == 0) FUN_0025c170_f580(moby + 0xc0);
                            scratch.direction[0] = 1.0f;
                            scratch.direction[1] = 0.0f;
                            scratch.direction[2] = 0.0f;
                            FUN_001ff6d0_f580(scratch.direction, scratch.direction, moby + 0xc0);
                            *(f32 *)(moby + 0x44) = -FUN_001ff8b0_f580(FUN_001ff408_f580(scratch.direction), scratch.direction[2]);
                            *(f32 *)(moby + 0x48) = FUN_001ff8b0_f580(scratch.direction[0], scratch.direction[1]);
                            FUN_00250f98_f580(moby);
                            *(u16 *)(moby + 0x34) |= 6;
                            if (first != 0) {
                                FUN_L00_002092c8_f580(D_0017A5C0_f580, D_0017C480_f580, *(s32 *)(moby + 0x24), 0, 0);
                                *(s32 *)(moby + 0x54) = 0;
                                *(s32 *)(moby + 0x50) = 0;
                                *(void **)(moby + 0x68) = D_0017C480_f580;
                                *(void **)(moby + 0x6c) = D_0017C480_f580;
                            } else if (second != 0) {
                                if (*(s16 *)(moby + 0xa6) == 0x1b1)
                                    FUN_L00_002092c8_f580(D_L00_0017A608, D_L00_0017C680, *(s32 *)(moby + 0x24), 6, 0);
                                else
                                    FUN_L00_002092c8_f580(D_L00_0015F6A8, D_L00_0017C680, *(s32 *)(moby + 0x24), 0, 0);
                                *(s32 *)(moby + 0x54) = 0;
                                *(void **)(moby + 0x68) = D_L00_0017C680;
                                *(void **)(moby + 0x6c) = D_L00_0017C680;
                                *(s32 *)(moby + 0x50) = 0;
                            } else if (third != 0) {
                                *(u128 *)scratch.saved = *(u128 *)D_0017A960_f580;
                                *(u128 *)(scratch.saved + 0x10) = *(u128 *)(D_0017A960_f580 + 0xb0);
                                *(f32 *)(D_0017A960_f580 + 0) = 1.0f;
                                *(f32 *)(D_0017A960_f580 + 4) = 1.0f;
                                *(f32 *)(D_0017A960_f580 + 8) = 1.0f;
                                *(f32 *)(D_0017A960_f580 + 0xb0) = 1.0f;
                                *(f32 *)(D_0017A960_f580 + 0xb4) = 1.0f;
                                *(f32 *)(D_0017A960_f580 + 0xb8) = 1.0f;
                                if (j != 0) {
                                    FUN_L00_002092c8_f580(D_0017A668_f580, D_0017C980_f580, *(s32 *)(moby + 0x24), 0, 0);
                                    *(void **)(moby + 0x68) = D_0017C980_f580;
                                    *(void **)(moby + 0x6c) = D_0017C980_f580;
                                } else {
                                    FUN_L00_002092c8_f580(D_0017A650_f580, D_0017C880_f580, *(s32 *)(moby + 0x24), 0, 0);
                                    *(void **)(moby + 0x68) = D_0017C880_f580;
                                    *(void **)(moby + 0x6c) = D_0017C880_f580;
                                }
                                *(s32 *)(moby + 0x54) = 0;
                                *(s32 *)(moby + 0x50) = 0;
                                *(u128 *)D_0017A960_f580 = *(u128 *)scratch.saved;
                                *(u128 *)(D_0017A960_f580 + 0xb0) = *(u128 *)(scratch.saved + 0x10);
                            }
                            goto after_moby;
                        }
                        FUN_00250480_f580(moby);
                        if (large_font_height == 0 || i != 0)
                            FUN_0025d330_f580(D_0013FE10_f580 + index * 0x40, moby + 0x40);
                        else
                            update_hero_item_rotation_from_history(moby, D_0013FE10_f580 + index * 0x40);
                        FUN_00250df8_f580(moby);
                        if (opt_mirror_anim) FUN_001ff3c0_f580(moby + 0xe0, moby + 0xc0, moby + 0xd0);
                        FUN_00250f98_f580(moby);
                        *(u16 *)(moby + 0x34) |= 6;
                    }
                }
after_moby:
                if (i == 3 && j == 1 && moby != 0) {
                    if (*(s32 *)((u8 *)&hero + 0x118c) == 0) {
                        *(s32 *)((u8 *)&hero + 0x118c) = (s32)FUN_0024e838_f580(0x4b4);
                        if (*(s32 *)((u8 *)&hero + 0x118c) != 0) {
                            effect = (u8 *)*(s32 *)((u8 *)&hero + 0x118c);
                            *(u32 *)(effect + 0x90) = 0x801432d7;
                            qcopy(effect + 0x10, &hero.motion.pos);
                            qcopy(effect + 0x40, &hero.motion.rot);
                            *(u16 *)(effect + 0x32) = 0x40;
                            *(u8 *)((u8 *)*(s32 *)((u8 *)&hero + 0x118c) + 0x31) = 1;
                            *(u8 *)((u8 *)*(s32 *)((u8 *)&hero + 0x118c) + 0x30) = 0;
                            *(u16 *)((u8 *)*(s32 *)((u8 *)&hero + 0x118c) + 0x34) = 0;
                            *(f32 *)((u8 *)*(s32 *)((u8 *)&hero + 0x118c) + 0x2c) *= 1.3f;
                            FUN_00250df8_f580((u8 *)*(s32 *)((u8 *)&hero + 0x118c));
                        }
                    }
                    if (*(s32 *)((u8 *)&hero + 0x118c) != 0) {
                        effect = (u8 *)*(s32 *)((u8 *)&hero + 0x118c);
                        if (((*(u16 *)(effect + 0x34) ^ 1) & 1) != 0) {
                            FUN_0024f728_f580(moby, 6, source);
                            *(u128 *)((u8 *)&hero + 0x1d80) = *(u128 *)(source + 0x30);
                            enqueue_hero_glow_draw_callback_once();
                            *(u128 *)(effect + 0x10) = *(u128 *)(source + 0x30);
                            FUN_001ffcd8_f580(effect + 0xc0, source);
                            FUN_00250df8_f580(effect);
                            *(u64 *)(effect + 0x38) = *(u64 *)((u8 *)hero.moby + 0x38);
                            random_range = FUN_001fef20_f580(0x78);
                            angle = FUN_001ff800_f580(((f32)(D_L00_0015F5CC_f580 % random_range) / FUN_00200240_f580(random_range) * 2.0f) * 3.1415927f + -3.1415927f);
                            color_red = FUN_00200250_f580(angle * 90.0f);
                            color_green = FUN_00200250_f580(angle * 50.0f);
                            color_blue = FUN_00200250_f580(angle * 10.0f);
                            color_blue += 0x14;
                            color_green += 0x32;
                            color_red += 0xd7;
                            if (color_red > 0xff) color_red = 0xff;
                            *(u16 *)(effect + 0x34) |= 0x10;
                            *(u32 *)(effect + 0x90) = (color_blue << 16) | 0x80000000 | (color_green << 8) | color_red;
                        }
                    }
                }
                j++;
            } while ((j < 1 || i == 1 || i == 3 || i == 4) && j < 2);
        }
        i++;
    } while (i < 7);
}
#endif /* NON_MATCHING */
typedef struct {
    char pad[0x20];
    unsigned char t;
    char pad2[0x74 - 0x21];
    void (*fn)(void);
} O;
void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
void clear_hero_item_slot(int i, int respawn_threshold) __asm__("FUN_L00_0020fca8");

void clear_hero_item_slot(int i, int respawn_threshold) {
    struct Hero *slot_owner; /* hero shifted by i slots, so items[0] is slot i */
    {
        struct Hero *pre_removal_slot_owner = (struct Hero *)(((char *)&hero) + i * sizeof(struct HeroItemSlot));
        O *item_moby;
        pre_removal_slot_owner->items[0].unk14 = respawn_threshold;
        if (pre_removal_slot_owner->items[0].state != 3) {
            item_moby = (O *)pre_removal_slot_owner->items[0].moby;
            pre_removal_slot_owner->items[0].state = 3;
            if (item_moby && item_moby->t != 0xfe && item_moby->t != 0xfd && item_moby->fn)
                item_moby->fn();
        }
    }
    slot_owner = (struct Hero *)(((char *)&hero) + i * sizeof(struct HeroItemSlot));
    slot_owner->items[0].state = 0;
    slot_owner->items[0].item_id = 0;
    if (slot_owner->items[0].moby) {
        mark_moby_for_removal(slot_owner->items[0].moby);
        slot_owner->items[0].moby = 0;
    }
    if (slot_owner->items[0].moby2 && i != 3) {
        mark_moby_for_removal(slot_owner->items[0].moby2);
        slot_owner->items[0].moby2 = 0;
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/help_0020CDF0.c: func_L00_00210418), where it is exact; names translated to the US level program. */

extern int scale_game_frames(int) __asm__("FUN_001f96f8");

/* For the listed ids sets hero.unk22CE to 3 and hero.unk1DC to scale_game_frames(0x46). */
void set_hero_condition_animation_timer_for_item(int id) __asm__("FUN_L00_0020fd80");

void set_hero_condition_animation_timer_for_item(int id) {
    struct Hero *base;

    switch (id) {
    case 9:
    case 11:
    case 13:
    case 16:
    case 23:
        base = &hero;
        base->unk22CE = 3;
        base->unk1DC = scale_game_frames(0x46);
        break;
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/help_0020CDF0.c: func_L00_00210478), where it is exact; names translated to the US level program. */

extern int scale_game_frames(int) __asm__("FUN_001f96f8");

/* Sets the short at hero.unk1DA to scale_game_frames(0x46) for the listed ids. */
void set_hero_item_switch_timer_for_item(int id) __asm__("FUN_L00_0020fde0");

void set_hero_item_switch_timer_for_item(int id) {
    switch (id) {
    case 9:
    case 11:
    case 13:
    case 14:
    case 15:
    case 16:
    case 18:
    case 19:
    case 22:
    case 23:
    case 26:
        hero.unk1DA = scale_game_frames(0x46);
        break;
    }
}
s32 get_hero_target_pose_status(void) __asm__("FUN_L00_00233f38");
/* True when 00233f38, D_L00_0015F5C4 and hero.vehicle_mode are 0, control_mode <3 or 4, and base_condition is 0. */
s32 can_select_hero_item_31(void) __asm__("FUN_L00_0020fe30");
s32 can_select_hero_item_31(void) {
    struct Hero *hero_state;
    u32 control_mode;
    if (get_hero_target_pose_status())
        return 0;
    if (D_L00_0015F5C4)
        return 0;
    hero_state = &hero;
    if (hero_state->vehicle_mode)
        return 0;
    control_mode = hero_state->state.control_mode;
    if (control_mode < 3 || control_mode == 4)
        return hero_state->base_condition == 0;
    return 0;
}
/* Applies the wrench/item selection for slot i: resolves pending, queued and requested items and refreshes the slot when it changes. */
/* Ported from rac1-decomp (src/overlays/shared/help_0020CDF0.c: func_L00_00210558), where it is exact; names translated to the US level program. */

typedef struct {
    char pad[0x20];
    int unk20;
    char pad2[0x28];
} ItemDef;

extern char D_0013C940[];
extern char D_00141660[];
extern unsigned char D_0013D4C0[];
extern int D_0015ED8C;
extern int D_0015ED90;
extern int D_0015ED94;
extern ItemDef item_defs[] __asm__("D_L00_00179AC0");
extern int fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern int FUN_L00_0020d568_c() __asm__("FUN_L00_0020d568");
extern int allocate_hero_voice_for_entry(int, int) __asm__("FUN_L00_00216de8");
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern void clear_hero_idle_rotation_targets(void) __asm__("FUN_L00_0020a500");
extern void cancel_hero_item_animation(void) __asm__("FUN_L00_0020e698");
extern void replenish_hero_drones(void) __asm__("FUN_L00_002cdcc0");
s32 can_select_hero_item_31(void) __asm__("FUN_L00_0020fe30");
void set_hero_condition_animation_timer_for_item(int id) __asm__("FUN_L00_0020fd80");
void set_hero_item_switch_timer_for_item(int id) __asm__("FUN_L00_0020fde0");
void blend_moby_animation(void *arg0, int arg1, int arg2, int arg3) __asm__("FUN_00212f90");

void hero_switch_item(int i) __asm__("FUN_L00_0020fea0");
void hero_switch_item(int i) {
    int changed;
    int flag6;
    int flag4;
    unsigned char item_switch_mode;

    if (i == 0) {
        int v;
        struct Hero *g = &hero;
        v = g->items[0].item_id;
        if (v != 8 && v != 0x17 && v != 9 && g->items[0].unk1C == 2) {
            g->items[0].unk1C = 0;
        }
    }
    if (i == 0) {
        struct Hero *g = &hero;
        if (g->unk20A7 != 0 && g->items[0].item_id != 8) {
            g->unk20A6 = 1;
        }
    }
    if (i == 0) {
        struct Hero *g = &hero;
        if (g->unk20A7 != 0 && g->items[0].item_id == 8) {
            if (g->pending_item[0] == 0x1F) {
                g->pending_item[0] = 0;
            }
            if (g->pending_item[0] != 0x18 && g->pending_item[0] != 0x24) {
                return;
            }
        }
    }
    {
        struct Hero *g = &hero;
        if (g->unk20AC != 0 && i == 0) {
            if (g->unk20A6 == 0 || g->items[0].item_id == 8) {
                g->unk20A6 = 0;
                return;
            }
        }
    }
    changed = 0;
    {
        char *pad = D_0013C940;
        if ((*(int *)(pad + 0x1A4) & 0x80) || hero.unk20A6 != 0) {
            if (i == 0) {
                int v;
                struct Hero *g = &hero;
                v = g->items[0].item_id;
                if (v != 8) {
                    if (g->unk20A6 != 0 && v != 0 && v != 0x1F) {
                        g->saved_item[0] = v;
                    }
                    hero.selected_item[i] = 8;
                    changed = 1;
                }
            }
        }
    }
    if (i == 0) {
        char *pad = D_0013C940;
        if (*(int *)(pad + 0x1A4) & 0x20) {
            struct Hero *g = &hero;
            if (g->unk20A7 == 0) {
                if (g->items[0].item_id != *(int *)(D_00141660) && *(int *)(D_00141660) != 0) {
                    g->selected_item[0] = *(int *)(D_00141660);
                    changed = 1;
                }
            }
        }
    }
    if (i == 1) {
        struct Hero *g;
        if (FUN_L00_0020d568_c() || hero.state.current == 0x12) {
            int v;
            g = &hero;
            v = g->items[1].item_id;
            if (v != 0) {
                g->saved_item[1] = v;
                g->pending_item[1] = 0x26;
            }
        }
        g = &hero;
        if (g->state.control_mode == 0xF && D_0013D4C0[0x1D] != 0) {
            if (*(int *)(D_00141660 + 4) != 0x1D || *(int *)((char *)g + i * 0x50 + 0x10B8) == 0) {
                int v;
                g->pending_item[1] = 0x1D;
                v = *(int *)((char *)g + i * 0x50 + 0x10B8);
                if (v != 0) {
                    g->saved_item[i] = v;
                } else {
                    g->saved_item[i] = 0x26;
                }
            }
        }
        g = &hero;
        if (g->unk12E7 != 0 && D_0013D4C0[0x1C] != 0) {
            if (*(int *)(D_00141660 + 4) != 0x1C || *(int *)((char *)g + i * 0x50 + 0x10B8) == 0) {
                int v;
                g->pending_item[1] = 0x1C;
                v = *(int *)((char *)g + i * 0x50 + 0x10B8);
                if (v != 0) {
                    g->saved_item[i] = v;
                } else {
                    g->saved_item[i] = 0x26;
                }
            }
        }
    }
    flag6 = 0;
    if (i == 2) {
        struct Hero *g = &hero;
        struct Hero *h;
        struct Hero *hero_state;
        if (g->state.control_mode == 0x11 || g->state.current == 0x76 || g->state.current == 0x6A || g->state.current == 0x82 ||
            g->unk22CB != 0) {
            if (((int *)(D_00141660))[i] != 6 ||
                (hero_state = &hero, *(int *)((char *)hero_state + i * 0x50 + 0x10B8) == 0)) {
                h = &hero;
                if (*(int *)((char *)h + i * 0x50 + 0x10B8) != 6 && h->selected_item[i] != 6 &&
                    D_0013D4C0[6] != 0) {
                    int v;
                    h->pending_item[i] = 6;
                    flag6 = 1;
                    v = *(int *)((char *)h + i * 0x50 + 0x10B8);
                    if (v != 0) {
                        h->saved_item[i] = v;
                    } else {
                        h->saved_item[i] = 0x26;
                    }
                }
            }
        } else if (g->items[2].item_id == 6 && g->saved_item[2] != 0) {
            g->restore_item[2] = 1;
        }
    }
    flag4 = 0;
    if (i == 3) {
        struct Hero *g;
        if (FUN_L00_0020d568_c() || hero.state.current == 0x12) {
            if (*(int *)(D_00141660 + 0xC) != 4) {
                int v;
                g = &hero;
                v = g->items[3].item_id;
                if (v != 4 && D_0013D4C0[4] != 0) {
                    g->saved_item[3] = v;
                    flag4 = 1;
                    g->pending_item[3] = 4;
                }
            }
        } else if (hero.items[3].item_id == 4 && hero.saved_item[3] != 0) {
            hero.restore_item[3] = 1;
        }
        if (hero.state.current == HERO_STATE_LAND && ((int *)(D_00141660))[i] == 4) {
            hero.pending_item[i] = 2;
        }
    }
    {
        struct Hero *g = &hero;
        int *qb = g->restore_item;
        int *restore_item_request = qb + i;
        if (*restore_item_request != 0) {
            int *r;
            int *rb;
            int restored_item_id;
            *restore_item_request = 0;
            rb = g->saved_item;
            r = rb + i;
            restored_item_id = *r;
            if (restored_item_id != 0 && restored_item_id != *(int *)((char *)g + i * 0x50 + 0x10B8)) {
                if (restored_item_id == 0x26) {
                    restored_item_id = 0;
                }
                *r = 0;
                ((int *)(D_00141660))[i] = restored_item_id;
                g->selected_item[i] = restored_item_id;
                if (i == 0) {
                    item_defs[g->selected_item[0]].unk20 = 0x20;
                }
                changed = 1;
            }
        }
    }
    {
        struct Hero *g = &hero;
        int *s;
        int *sb;
        if (g->unk1FF5 != 0) {
            replenish_hero_drones();
            g->pending_item[i] = 0;
        }
        sb = g->pending_item;
        s = sb + i;
        if (*s == 0x1F) {
            if (can_select_hero_item_31()) {
                int v;
                if (g->unk22DE == 0) {
                    allocate_hero_voice_for_entry(0x18, 0);
                    g->unk22DE = 0x12;
                    g->unk22DC = 1;
                }
                changed = 1;
                g->selected_item[i] = *s;
                v = ((int *)(D_00141660))[i];
                ((int *)(D_00141660))[i] = *s;
                D_0015ED8C = v;
            }
            hero.pending_item[i] = 0;
        }
    }
    {
        int v;
        struct Hero *g = &hero;
        int *ab = g->pending_item;
        int *pending_item_entry = ab + i;
        v = *pending_item_entry;
        if (v != 0) {
            int *bb = g->selected_item;
            int *b = bb + i;
            if (v != *b) {
                if (v == 0x26) {
                    *b = 0;
                    ((int *)(D_00141660))[i] = 0;
                } else {
                    struct Hero *h;
                    int u;
                    if (i == 0 && g->pending_item[0] != 0x24 && g->items[0].item_id != 8) {
                        D_0015ED8C = *(int *)(D_00141660);
                    }
                    h = &hero;
                    u = h->pending_item[i];
                    h->selected_item[i] = u;
                    if (u != 0x24 && (flag6 == 0 || u != 6) && (flag4 == 0 || u != 4)) {
                        ((int *)(D_00141660))[i] = u;
                    }
                    if (i == 0) {
                        struct Hero *k = &hero;
                        item_defs[k->selected_item[0]].unk20 = 0x20;
                    }
                }
                changed = 1;
            } else {
                *pending_item_entry = 0;
            }
        }
    }
    {
        struct Hero *s = (struct Hero *)(((char *)&hero) + i * sizeof(struct HeroItemSlot));
        item_switch_mode = s->items[0].unk1C;
        if (item_switch_mode == 2) {
            changed = 0;
        }
        if (item_switch_mode == 1 && s->items[0].timer == 0) {
            s->items[0].timer = s->items[0].timer_reload;
        }
    }
    if (fast_dec_timer_s16(&hero.items[i].timer) == 0) {
        changed = 0;
    }
    if (changed) {
        struct Hero *g = &hero;
        *(int *)((char *)g + i * 0x50 + 0x10B0) = 3;
        clear_hero_idle_rotation_targets();
        g->unk1010 =
            random_int_between(scale_game_frames(0x32), scale_game_frames(0x5A));
        set_hero_item_switch_timer_for_item(*(hero.selected_item + i));
        set_hero_condition_animation_timer_for_item(*(hero.selected_item + i));
        if (g->unk20A8 != 0) {
            cancel_hero_item_animation();
        }
        if (i == 0) {
            if (*hero.selected_item == 8) {
                D_0015ED90 = 1;
            } else {
                D_0015ED90 = 0;
            }
        }
        if (i == 3) {
            if (hero.items[3].item_id == 3) {
                D_0015ED94 = 1;
            } else {
                D_0015ED94 = 0;
            }
        }
        {
            struct Hero *k = &hero;
            struct Hero *s = (struct Hero *)((char *)k + i * sizeof(struct HeroItemSlot));
            s->items[0].unk1A = 0;
            k->pending_item[i] = 0;
            k->unk20A6 = 0;
            if (k->unkD00 != 0) {
                *(int *)(k->unkD00 + 0x34) = 1;
            }
            if (k->unkD04 != 0) {
                *(int *)(k->unkD04 + 0x34) = 1;
            }
            if (s->items[0].moby != 0) {
                blend_moby_animation(s->items[0].moby, 2, 0, 2);
            }
            if (s->items[0].moby2 != 0 && i != 3) {
                blend_moby_animation(s->items[0].moby2, 2, 0, 2);
            }
        }
        {
            struct Hero *slot_owner = (struct Hero *)(((char *)&hero) + i * sizeof(struct HeroItemSlot));
            slot_owner->items[0].state = 3;
        }
    }
}
/* Ported from rac1-decomp (src/overlays/shared/help_0020CDF0.c: func_L00_00210E00), where it is exact; names translated to the US level program. */
#include "rnc/gameplay/entities/moby.h"
extern void update_hero_easing_record(void *) __asm__("FUN_L00_00205168");
extern s32 D_L00_0015F3F8;

#define ITEM_SLOT(i) ((struct Hero *)((char *)&hero_lui + (i) * sizeof(struct HeroItemSlot)))

void hero_update_items(void) __asm__("FUN_L00_00210748");
void hero_update_items(void) {
    enum {
        HERO_ITEM_SLOT_STATE_READY = 2,
        HERO_ITEM_SLOT_STATE_REMOVING = 3
    };
    int i;
    int k;
    struct Moby *moby;

    for (i = 0; i < 7; i++) {
        moby = ITEM_SLOT(i)->items[0].moby;
        if (moby != 0) {
            switch (ITEM_SLOT(i)->items[0].state) {
            case HERO_ITEM_SLOT_STATE_READY:
                ITEM_SLOT(i)->items[0].unk20++;
                hero_switch_item(i);
                if (i == 0) {
                    for (k = 0; k < 3; k++) {
                        if (hero_lui.unk18F0[k].unkA0 != -1) {
                            hero_lui.unk18F0[k].kind = 5;
                            update_hero_easing_record(&hero_lui.unk18F0[k]);
                        }
                    }
                }
                if ((moby->anim_flags & 2) && moby->prev_seq == 0) {
                    blend_moby_animation(moby, 1, 0, 2);
                }
                break;
            case HERO_ITEM_SLOT_STATE_REMOVING:
                ITEM_SLOT(i)->items[0].unk1B++;
                if ((moby->anim_flags & 2) || i == 1 || i == 0) {
                    clear_hero_item_slot(i, D_L00_0015F3F8 + 2);
                }
                break;
            }
            if (moby != 0 && moby->state != 0xFE && moby->state != 0xFD) {
                if (i != 6 && moby->update != 0) {
                    moby->update(moby);
                }
            }
        } else {
            hero_switch_item(i);
        }
    }
}
s32 FUN_L00_0020d568_u() __asm__("FUN_L00_0020d568");
void spawn_hero_item_mobies() __asm__("FUN_L00_0020ea80");
void FUN_L00_0020f580();
void animate_hero_slot4_and_slot5_visuals() __asm__("FUN_L00_00234808");
void update_hero_and_equipped_item_glows() __asm__("FUN_L00_002080b0");
void update_hero_equipped_items(void) __asm__("FUN_L00_00210928");

void update_hero_equipped_items(void) {
    if (hero.unk308 == 1 || (u32)(hero.state.control_mode - 0x15) < 2 ||
        FUN_L00_0020d568_u()) {
        hero.unk20A7 = 1;
        hero.unk20AC = 1;
    }
    if (FUN_L00_0020d568_u() || hero.state.control_mode == 0x15 ||
        hero.state.control_mode == 0x16 || hero.state.control_mode == 3 ||
        hero.state.current == 0x12) {
        hero.unk20AE = 1;
    } else {
        hero.unk20AE = 0;
    }
    spawn_hero_item_mobies();
    FUN_L00_0020f580();
    animate_hero_slot4_and_slot5_visuals();
    update_hero_and_equipped_item_glows();
    hero_update_items();
}
void cancel_hero_item_animation(void) __asm__("FUN_L00_0020e698");
void clear_moby_transform_history(void *) __asm__("FUN_L00_00262840");
void FUN_L00_00232fe8_210a08(void) __asm__("FUN_L00_00232fe8");
void FUN_L00_00206c08_210a08(void) __asm__("FUN_L00_00206c08");
void FUN_L00_00205538_210a08(void) __asm__("FUN_L00_00205538");
void update_hero_collision_capsule_dimensions(void) __asm__("FUN_L00_00211380");
void FUN_L00_00232628_210a08(void) __asm__("FUN_L00_00232628");
void FUN_L00_00257024_210a08(char *) __asm__("FUN_L00_00257024");
void apply_hero_and_item_scale_sequence(void) __asm__("FUN_L00_00207a58");
extern int hero_set_state(int, int) __asm__("FUN_L00_002223f8");
void enter_hero_vehicle_mode(int mode, int requested_state, char *vehicle_moby) __asm__("FUN_L00_00210a08");

void enter_hero_vehicle_mode(int mode, int requested_state, char *vehicle_moby) {
    struct Hero *hero_state;
    cancel_hero_item_animation();
    hero_state = &hero;
    hero_state->unk20A6 = 1;
    hero_update_items();
    hero_state->moby = (struct Moby *)hero_state->unkA88;
    clear_moby_transform_history(&hero_state->trail);
    *(int *)((char *)hero_state->moby + 0x98) = -1;
    FUN_L00_00232fe8_210a08();
    FUN_L00_00206c08_210a08();
    FUN_L00_00205538_210a08();
    hero_state->vehicle_mode = mode;
    hero_state->unkA84 = (u8 *)vehicle_moby;
    hero_state->moby = (struct Moby *)vehicle_moby;
    update_hero_collision_capsule_dimensions();
    hero_state->capsule_top = hero_state->unk228;
    hero_state->unk224 = hero_state->unk22C;
    *(unsigned short *)((char *)hero_state->moby + 0x34) |= 6;
    hero_state->anim_speed = 1.0f;
    hero_state->unkA94 = 1.0f;
    FUN_L00_00232628_210a08();
    FUN_L00_00257024_210a08((char *)hero_state->moby);
    apply_hero_and_item_scale_sequence();
    hero_state->unk22CC = requested_state;
    switch (mode) {
    case HERO_VEHICLE_MODE_CLANK:
        hero_state->health.unk22B0 = *(unsigned short *)&hero_state->health.hp;
        hero_state->health.hp = hero_state->health.unk22AC;
        break;
    case HERO_VEHICLE_MODE_GIANT_CLANK:
        hero_state->unk1630 = 200;
        break;
    }
    hero_set_state(requested_state, 1);
}
extern void FUN_0020c828_210b30(void *) __asm__("FUN_0020c828");
extern void set_animation_parameter(int, int) __asm__("FUN_001ff570");
extern void FUN_L00_00205538_210b30(void) __asm__("FUN_L00_00205538");
extern void FUN_L00_00206c08_210b30(void) __asm__("FUN_L00_00206c08");
extern void FUN_L00_00232628_210b30(void) __asm__("FUN_L00_00232628");
extern int hero_set_state(int, int) __asm__("FUN_L00_002223f8");

void exit_hero_vehicle_mode(void) __asm__("FUN_L00_00210b30");

void exit_hero_vehicle_mode(void) {
    struct Hero *hero_state = &hero;
    unsigned char *vehicle_moby;
    if (hero_state->vehicle_mode == 1) {
        hero_state->health.unk22AC = hero_state->health.hp;
        hero_state->health.hp = hero_state->health.unk22B0;
        if (hero_state->unk1620) {
            FUN_0020c828_210b30(hero_state->unk1620);
            hero_state->unk1620 = 0;
        }
        if (hero_state->unk1624) {
            FUN_0020c828_210b30(hero_state->unk1624);
            hero_state->unk1624 = 0;
        }
    }
    {
        struct Hero *vehicle_animation_owner = &hero;
        if (vehicle_animation_owner->vehicle_mode == 2 && vehicle_animation_owner->unk1634 != -1) {
            set_animation_parameter(vehicle_animation_owner->unk1634, 0);
            vehicle_animation_owner->unk1634 = -1;
        }
        FUN_L00_00205538_210b30();
    }
    {
        struct Hero *vehicle_exit_owner = &hero;
        vehicle_exit_owner->vehicle_mode = 0;
        vehicle_moby = vehicle_exit_owner->unkA84;
        if (vehicle_moby) {
            *(u16 *)(vehicle_moby + 0x34) &= ~6;
        }
        vehicle_exit_owner->unkA84 = 0;
        if (vehicle_exit_owner->unkA8C) {
            FUN_0020c828_210b30(vehicle_exit_owner->unkA8C);
            vehicle_exit_owner->unkA8C = 0;
        }
        FUN_L00_00206c08_210b30();
        {
            unsigned char *restored_hero_moby = vehicle_exit_owner->unkA88;
            vehicle_exit_owner->moby = (struct Moby *)restored_hero_moby;
            qcopy(restored_hero_moby + 0x10, &vehicle_exit_owner->motion.pos);
            *(int *)(restored_hero_moby + 0x98) = 0;
        }
        vehicle_exit_owner->unkA94 = 1.0f;
        FUN_L00_00232628_210b30();
        if (vehicle_exit_owner->state.current != 100 ||
            (D_L00_0015F5C4 != 2 && D_L00_0015F5C4 != 6)) {
            hero_set_state(HERO_STATE_IDLE, 1);
        }
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/help_0020CDF0.c: func_L00_00211338), where it is exact; names translated to the US level program. */

extern void set_vector_length_xy(float *, float *, float) __asm__("FUN_001f9c48");

/* Scales x/y by s and sets or scales z, per component or through set_vector_length_xy. */
void hero_knockback(float *v, int each, float s, float z) __asm__("FUN_L00_00210c80");
void hero_knockback(float *knockback_vector, int each, float horizontal_knockback_strength, float z) {
    if (each) {
        knockback_vector[0] *= horizontal_knockback_strength;
        knockback_vector[1] *= horizontal_knockback_strength;
        knockback_vector[2] *= z;
    } else {
        set_vector_length_xy(knockback_vector, knockback_vector, horizontal_knockback_strength);
        knockback_vector[2] = z;
    }
}

typedef struct {
    u8 pad0[0x10];
    OvlQuad v10;
    u8 *p20;
    s32 i24;
    u8 b28;
    u8 pad29[3];
    f32 f2C;
    s32 i30;
    u8 *p34;
    u8 pad38[8];
} T_210ce8;

typedef union {
    OvlQuad q;
    f32 f[4];
    s32 i[4];
} V_210ce8;

extern s32 D_0013DF88[] __asm__("D_0013DF88");
extern s32 D_0015EEA8 __asm__("D_0015EEA8");
extern f32 D_0015ED6C_210ce8 __asm__("D_0015ED6C");

s32 FUN_001fa6d0_210ce8(f32) __asm__("FUN_001fa6d0");
void FUN_L00_00206c08_210ce8(void) __asm__("FUN_L00_00206c08");
void FUN_001f9a28_210ce8(void *, void *, void *) __asm__("FUN_001f9a28");
f32 FUN_001fa580_210ce8(f32, f32) __asm__("FUN_001fa580");
f32 FUN_001f9dc8_210ce8(f32) __asm__("FUN_001f9dc8");
f32 FUN_001f9de0_210ce8(f32) __asm__("FUN_001f9de0");
void FUN_001f9a10_210ce8(void *, void *, void *) __asm__("FUN_001f9a10");
extern int hero_set_state(int, int) __asm__("FUN_L00_002223f8");
void FUN_L00_00216de8_210ce8(s32, s32) __asm__("FUN_L00_00216de8");
void remove_tracked_class_5f3_moby(void) __asm__("FUN_L00_002a9be0");
void FUN_001f99f8_210ce8(void *) __asm__("FUN_001f99f8");
void FUN_L00_00210c80_210ce8(void *, s32, f32, f32) __asm__("FUN_L00_00210c80");

s32 hero_check_damage(s32 arg) __asm__("FUN_L00_00210ce8");
extern T_210ce8 moby_hits[] __asm__("D_L00_00178100");
s32 hero_check_damage(s32 apply_damage_response) {
    u8 *hero_moby;
    T_210ce8 *hero_hit_entry;
    u8 *mob;
    s32 flag;
    s32 id;
    f32 horizontal_knockback_strength, vertical_knockback_strength, knockback_scale;
    V_210ce8 knockback_vector;

    if (hero.state.control_mode == 0x14) {
        return 0;
    }
    if (hero.state.control_mode == 7) {
        return 0;
    }
    if (hero.state.current == 0x32) {
        return 0;
    }
    if (hero.invulnerable_frames != 0) {
        return 0;
    }
    if (D_L00_0015F5C4 != 0) {
        return 0;
    }
    hero.unk2280 = 0;
    hero_moby = ((u8 *)hero.moby);
    if (hero_moby[0xA4] == 0xFF) {
        return 0;
    }
    hero_hit_entry = &moby_hits[hero_moby[0xA4]];
    if (hero_hit_entry->p34 != hero_moby) {
        return 0;
    }
    if (hero.vehicle_mode == 2) {
        if (!(hero_hit_entry->i24 & 2)) {
            return 0;
        }
    } else if ((hero_hit_entry->i24 ^ 1) & 1) {
        return 0;
    }
    D_0013DF88[current_level_index]++;
    hero.unk2280 = (struct Moby *)hero_hit_entry->p20;
    D_0015EEA8++;
    if (hero.state.control_mode == 0xF) {
        hero.unk5BE = 1;
        return 0;
    }
    if (hero.vehicle_mode == 2) {
        hero.unk1630 -= FUN_001fa6d0_210ce8(hero_hit_entry->f2C);
        if (hero.unk1630 < 0) {
            hero.unk1630 = 0;
        }
    }
    if (apply_damage_response == 0) {
        return 1;
    }
    FUN_L00_00206c08_210ce8();
    flag = 0;
    if (hero_hit_entry->i30 & 1) {
        qcopy(&knockback_vector, &hero_hit_entry->v10);
        if (*(f32 *)((u8 *)hero_hit_entry + 0x1C) == 5627.9248046875f) {
            flag = 1;
        }
    } else if (hero_hit_entry->p20 != 0) {
        FUN_001f9a28_210ce8(&knockback_vector, &hero.motion.pos, hero_hit_entry->p20 + 0x10);
    } else {
        knockback_vector.f[0] = FUN_001f9dc8_210ce8(FUN_001fa580_210ce8(hero.motion.rot.f[2], 3.1415927f));
        knockback_vector.f[1] = FUN_001f9de0_210ce8(FUN_001fa580_210ce8(hero.motion.rot.f[2], 3.1415927f));
        knockback_vector.i[2] = 0;
    }
    switch (hero.vehicle_mode) {
    case HERO_VEHICLE_MODE_ON_FOOT:
        mob = ((u8 *)hero.unk2280);
        if (mob != 0 && (*(s16 *)(mob + 0xA6) == 0x4EB || *(s16 *)(mob + 0xA6) == 0x558)) {
            id = 0x80;
        call_ret:
            hero_set_state(id, 1);
            return 1;
        }
        if (hero.state.control_mode == 0x16) {
            hero_set_state(0x6D, 1);
            hero.motion.unk120.f[2] = D_0015ED6C_210ce8 * 7.0f;
            return 1;
        }
        if (hero.state.control_mode == 0x12) {
            mob = ((u8 *)hero.unk2280);
            if (mob != 0 && *(s16 *)(mob + 0xA6) == 0x28F) {
                id = 0x82;
                goto call_ret;
            }
            hero_set_state(0x75, 1);
        } else if (hero.state.control_mode == 0x11) {
            mob = ((u8 *)hero.unk2280);
            if (mob != 0 && *(s16 *)(mob + 0xA6) == 0x28F) {
                id = 0x82;
                goto call_ret;
            }
            if (current_level_index == 0xF || current_level_index == 0x11) {
                mob = ((u8 *)hero.unk2280);
                if (mob != 0 && (*(s16 *)(mob + 0xA6) == 0x28F || *(s16 *)(mob + 0xA6) == 0x7B ||
                                 *(s16 *)(mob + 0xA6) == 0x29D)) {
                    FUN_L00_00216de8_210ce8(0x1C, 0);
                }
            }
            hero_set_state(0x76, 1);
        } else if (hero.state.control_mode == 3) {
            mob = ((u8 *)hero.unk2280);
            if (mob != 0) {
                id = 0x82;
                if (*(s16 *)(mob + 0xA6) == 0x28F) {
                    goto call_ret;
                }
            }
            hero_set_state(HERO_STATE_HURT, 1);
        } else {
            hero_set_state(HERO_STATE_HURT, 1);
        }
        knockback_scale = D_0015ED6C_210ce8;
        horizontal_knockback_strength = knockback_scale * 5.7f;
        vertical_knockback_strength = knockback_scale * 2.4f;
        if (hero.unk12E7 != 0) {
            horizontal_knockback_strength = 0.0f;
            vertical_knockback_strength = knockback_scale * 1.7f;
        }
        FUN_L00_00210c80_210ce8(&knockback_vector, flag, horizontal_knockback_strength, vertical_knockback_strength);
        if (flag != 0 && hero_hit_entry->b28 == 4) {
            knockback_vector.f[2] += knockback_vector.f[2];
        }
        break;
    case HERO_VEHICLE_MODE_CLANK:
        hero_set_state(0x46, 1);
        FUN_L00_00210c80_210ce8(&knockback_vector, flag, D_0015ED6C_210ce8 * 5.0f, D_0015ED6C_210ce8 * 3.0f);
        break;
    case HERO_VEHICLE_MODE_GIANT_CLANK:
        if ((hero_hit_entry->i24 & 4) || hero.unk1630 <= 0) {
            hero_set_state(0x5D, 1);
            remove_tracked_class_5f3_moby();
            if (hero.air_frames.s == 0) {
                FUN_L00_00210c80_210ce8(&knockback_vector, flag, D_0015ED6C_210ce8 * 7.0f,
                                        D_0015ED6C_210ce8 * 3.5f);
                break;
            }
        }
        FUN_001f99f8_210ce8(&knockback_vector);
        break;
    case 3:
        hero_set_state(0x56, 1);
        FUN_L00_00210c80_210ce8(&knockback_vector, flag, D_0015ED6C_210ce8 * 5.0f, D_0015ED6C_210ce8 * 2.4f);
        break;
    }
    FUN_001f9a10_210ce8(&hero.motion.velocity, &hero.motion.velocity, &knockback_vector);
    FUN_001f9a10_210ce8(&hero.motion.displacement, &hero.motion.displacement, &knockback_vector);
    return 1;
}
extern s32 D_0015EEAC __asm__("D_0015EEAC");
extern s32 D_0014EE90[] __asm__("D_0014EE90");
extern void clear_help_record_high_bits(void) __asm__("FUN_L00_00203a28");
extern int FUN_001f96f8_211250(int) __asm__("FUN_001f96f8");
extern void fade_to_black(int) __asm__("FUN_001f4a58");

void hero_respawn(void) __asm__("FUN_L00_00211250");
extern s32 D_0013DFD8[];
extern s32 D_0014E990[][16];
extern s32 D_L00_0015F5A4;
void hero_respawn(void) {
    s32 lvl = current_level_index;
    char *damage_source_moby;
    u32 t;
    D_0013DFD8[lvl]++;
    D_0015EEAC++;
    if (((char *)hero.unk2280) != 0 && *(s16 *)(((char *)hero.unk2280) + 0xA6) == 0x4D &&
        lvl == 15) {
        D_L00_0015F5A4 = 1;
    }
    clear_help_record_high_bits();
    damage_source_moby = ((char *)hero.unk2280);
    if (damage_source_moby != 0 && *(u8 *)(damage_source_moby + 0xB0) != 0xFF) {
        D_0014E990[current_level_index][*(u8 *)(damage_source_moby + 0xB0)]++;
        D_0014EE90[*(u8 *)(damage_source_moby + 0xB0)]++;
    }
    fade_to_black(FUN_001f96f8_211250(0x10));
    hero.unk20B1 = 1;
}
typedef struct {
    u8 pad[0xA6];
    s16 f;
} K2;
/* True when p is non-null and its field f is 0x47. */
s32 is_wrench_moby(K2 *candidate_moby) __asm__("FUN_L00_00211360");

s32 is_wrench_moby(K2 *p) __asm__("FUN_L00_00211360");
s32 is_wrench_moby(K2 *candidate_moby) {
    if (candidate_moby == 0) {
        return 0;
    }
    if (candidate_moby->f == 0x47) {
        return 1;
    }
    return 0;
}
extern f32 D_0015ED6C_211380 __asm__("D_0015ED6C");
f32 FUN_001f9b80_211380(void *, void *) __asm__("FUN_001f9b80");
f32 approach_value_211380(f32, f32, f32 *) __asm__("FUN_00213ed8");
void FUN_L00_0025b8c0_211380(f32 *, f32 *, f32, f32, f32, f32) __asm__("FUN_L00_0025b8c0");
void update_hero_collision_capsule_dimensions(void) {
    switch (hero.vehicle_mode) {
    case HERO_VEHICLE_MODE_ON_FOOT:
        hero.unk228 = 0.8f;
        hero.unk22C = 0.7f;
        hero.unk230 = 0.45f;
        break;
    case HERO_VEHICLE_MODE_CLANK:
        hero.unk228 = 0.59999996f;
        hero.unk22C = 0.45000002f;
        hero.unk230 = 0.3f;
        break;
    case HERO_VEHICLE_MODE_GIANT_CLANK:
        hero.unk228 = 5.25f;
        hero.unk22C = 4.45f;
        hero.unk230 = 3.75f;
        break;
    case 3:
        hero.unk228 = 0.8f;
        hero.unk22C = 0.6f;
        hero.unk230 = 0.45f;
        break;
    }
    if (hero.state.control_mode == 4) {
        if (hero.state_timer > hero.unk420 && hero.unk41E == 0)
            hero.unk22C = hero.unk434;
    } else if (hero.state.current == HERO_STATE_FALL) {
        hero.unk22C = 0.5f;
    } else if (hero.state.current == HERO_STATE_CROUCH) {
        hero.unk228 = 0.35000002f;
    } else if ((u32)(hero.state.control_mode - 0x11) < 2) {
        hero.unk228 = 0.0f;
        hero.unk22C = 0.0f;
    } else if (hero.unk12E2 && hero.grounded_frames) {
        hero.unk228 = 0.8f;
        hero.unk22C = 0.9f;
    } else if (hero.state.current == HERO_STATE_SINK) {
        hero.unk230 = 0.8f;
    }
    if (!hero.wall_hit || hero.state.prev_control_mode == 0x12 || hero.state.control_mode == 0x11 || hero.base_condition ||
        FUN_001f9b80_211380(hero.unk210.f, hero.motion.pos.f) > hero.unk234 * 0.5f) {
        f32 *capsule_top_ptr = &hero.capsule_top;
        struct Hero *hero_state = &hero;
        approach_value_211380(hero_state->unk228, frame_scale * 0.02f, capsule_top_ptr);
        approach_value_211380(hero_state->unk22C, frame_scale * 0.02f, capsule_top_ptr + 1);
        FUN_L00_0025b8c0_211380(capsule_top_ptr + 5, capsule_top_ptr + 6, hero_state->unk230, frame_scale_sq * 0.02f,
                                frame_scale_sq * 0.3f, D_0015ED6C_211380 * 4.0f);
    }
}
extern u8 resolve_hero_gravity_mode(void) __asm__("FUN_L00_00233440");
float get_capped_hero_stick_magnitude(void) __asm__("FUN_L00_00211830");

extern f32 get_capped_hero_stick_magnitude(void) __asm__("FUN_L00_00211830");
extern f32 FUN_L00_001ff408(void *);
extern void hero_update_movement(void) __asm__("FUN_L00_00217970");
extern void FUN_L00_001ffa70(void *, void *);
extern void FUN_L00_001ff9e8(void *);
extern void matrix_from_basis_rows(void *, void *) __asm__("FUN_L00_001ffcf8");
extern void update_hero_motion_and_collision_probes(void) __asm__("FUN_L00_00221310");
extern void update_hero_timers_and_reach_state(void) __asm__("FUN_L00_002215c8");
extern u8 controller_state[] __asm__("D_0013C940");

/* Per-frame hero input: stick (or d-pad) into unk1D20/24, the yaw and position
   history rings, then the hero basis matrix and the movement updates. */
void hero_update(void) __asm__("FUN_L00_00211670");
void hero_update(void)
{
    s32 index;
    s32 buttons;
    s32 negative_x;
    u8 mode;
    struct Hero *hero_state;
    u8 *source;
    Vec4 *positions;

    mode = resolve_hero_gravity_mode();
    hero_state = &hero;
    hero_state->unk20B3 = mode;
    hero_state->unk229C = get_capped_hero_stick_magnitude();
    hero_state->unk240 = 0;
    source = controller_state;
    hero_state->stick_x = *(f32 *)(source + 0x108);
    hero_state->stick_y = *(f32 *)(source + 0x10C);
    if (FUN_L00_001ff408(&hero_state->stick_x) < 0.25f) {
        /* Stick near centre: take the d-pad instead. The differences are
           written back into left and up, which is how retail allocates them. */
        s32 up, right, down, left;

        buttons = *(s32 *)(source + 0x1B0);
        right = (buttons >> 13) & 1;
        left = (buttons >> 15) & 1;
        down = (buttons >> 14) & 1;
        up = (buttons >> 12) & 1;
        left = right - left;
        up = down - up;
        hero_state->stick_x = (f32)left;
        hero_state->stick_y = (f32)up;
    }
    hero_update_movement();

    hero_state->unk2128[hero_state->unk21A8] = hero_state->motion.rot.f[2];
    hero_state->unk21A8 = (hero_state->unk21A8 + 1) % 32;
    ++hero_state->unk21AC;
    if (hero_state->unk21AC > 0x20)
        hero_state->unk21AC = 0x20;

    positions = hero_state->unk1B00;
    index = hero_state->unk21B0;
    qcopy_nc(&positions[index], &hero_state->motion.pos);
    hero_state->unk21B0 = (index + 1) % 32;
    ++hero_state->unk21B4;
    if (hero_state->unk21B4 > 0x20)
        hero_state->unk21B4 = 0x20;

    FUN_L00_001ffa70(hero_state, &hero_state->motion.rot);
    FUN_L00_001ff9e8(hero_state->unk40);
    matrix_from_basis_rows(hero_state->unk40, hero_state);
    update_hero_collision_capsule_dimensions();
    update_hero_motion_and_collision_probes();
    update_hero_timers_and_reach_state();
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/help_0020CDF0.c: func_L00_00211EE8), where it is exact; names translated to the US level program. */

extern float vector_length_xy(void *) __asm__("FUN_001f9b20");

/* Length from vector_length_xy on hero.stick_x, capped at 1. */
float get_capped_hero_stick_magnitude(void) {
    float stick_magnitude = vector_length_xy(&hero.stick_x);

    if (1.0f < stick_magnitude) {
        stick_magnitude = 1.0f;
    }
    return stick_magnitude;
}
#define NOT_SDA

#define MACRO_ADDR

/* Whether T lies in the window that ends at D_0013F450 + 0xAA8 and is
   +0xAAC long (T before the end, within that length of it). */
/* Ported from rac1-decomp (src/overlays/shared/help_0020CDF0.c: func_L00_00211F28), where it is exact; names translated to the US level program. */

/* True when t is before the time at hero.anim_frame by at most the span hero.unkAAC. */
int test_hero_animation_frame_window(float animation_frame_threshold) __asm__("FUN_L00_00211870");

int test_hero_animation_frame_window(float animation_frame_threshold) {
    float end = hero.anim_frame;

    if (animation_frame_threshold < end) {
        if (end - animation_frame_threshold <= hero.unkAAC) {
            return 1;
        }
    }
    return 0;
}
/* Clears hero.target_speed and unk194 as words. */
void clear_hero_target_and_ground_speeds(void) __asm__("FUN_L00_002118b0");

void clear_hero_target_and_ground_speeds(void) {
    struct Hero *hero_state = &hero;
    *(int *)&hero_state->target_speed = 0;
    *(int *)&hero_state->ground_speed = 0;
}
typedef union {
    OvlQuad q;
    float f[4];
} V_2118c8;
extern unsigned char D_0015EDB4_2118c8 __asm__("D_0015EDB4");
extern float D_L00_00166DD8_2118c8 __asm__("D_L00_00166DD8") __attribute__((section(".data")));
extern unsigned char D_L00_00166FD0_2118c8[] __asm__("D_L00_00166FD0")
    __attribute__((section(".data")));
extern float FUN_001f99c0_2118c8(float) __asm__("FUN_001f99c0");
extern float FUN_001f9b20_2118c8(void *) __asm__("FUN_001f9b20");
extern void FUN_L00_001ff378_2118c8(void *, void *, float) __asm__("FUN_L00_001ff378");
extern float compute_hero_ground_aligned_heading(void) __asm__("FUN_L00_002332d0");
extern void FUN_001f9cf8_2118c8(void *, void *, void *) __asm__("FUN_001f9cf8");
extern float FUN_001f9e90_2118c8(float, float) __asm__("FUN_001f9e90");
extern int FUN_001fa6d0_2118c8(float) __asm__("FUN_001fa6d0");
extern float FUN_001fa6c0_2118c8(int) __asm__("FUN_001fa6c0");
extern float FUN_001fa580_2118c8(float, float) __asm__("FUN_001fa580");

void stick_target(float scale, int mode) __asm__("FUN_L00_002118c8");
void stick_target(float scale, int mode) {
    V_2118c8 stick_vector;
    V_2118c8 q;
    float x;
    float stick_magnitude;
    float y;

    stick_vector.f[1] = hero.stick_y;
    stick_vector.f[0] = hero.stick_x;
    if (mode == 1) {
        if (FUN_001f99c0_2118c8(stick_vector.f[1]) > 0.7f && FUN_001f99c0_2118c8(stick_vector.f[0]) < 0.12f) {
            stick_vector.f[0] = 0.0f;
        }
        if (FUN_001f99c0_2118c8(stick_vector.f[0]) > 0.7f && FUN_001f99c0_2118c8(stick_vector.f[1]) < 0.12f) {
            stick_vector.f[1] = 0.0f;
        }
    }
    stick_magnitude = FUN_001f9b20_2118c8(&stick_vector);
    if (stick_magnitude > 1.0f) {
        FUN_L00_001ff378_2118c8(&stick_vector, &stick_vector, stick_magnitude);
    }
    if (stick_vector.f[0] == 0.0f && stick_vector.f[1] == 0.0f) {
        hero.target_speed = 0.0f;
        if (hero.unk20B3) {
            hero.motion.move_yaw = compute_hero_ground_aligned_heading();
        } else {
            hero.motion.move_yaw = hero.motion.rot.f[2];
        }
        q.q = 0;
        q.f[0] = 1.0f;
        FUN_001f9cf8_2118c8(&hero.motion.unk170, &q, (char *)hero.moby + 0xC0);
        return;
    }
    hero.motion.move_yaw = FUN_001f9e90_2118c8(-stick_vector.f[1], -stick_vector.f[0]);
    if (mode == 2) {
        hero.motion.move_yaw =
            FUN_001fa6c0_2118c8(FUN_001fa6d0_2118c8((hero.motion.move_yaw + 3.1415927f) * 0.63662f + 0.5f) & 3) *
                1.5707964f -
            3.1415927f;
    }
    if (mode == 3) {
        hero.motion.move_yaw =
            FUN_001fa6c0_2118c8(FUN_001fa6d0_2118c8((hero.motion.move_yaw + 3.1415927f) * 1.27324f + 0.5f) & 7) *
                0.785398f -
            3.1415927f;
    }
    if (mode == 4) {
        hero.motion.move_yaw = FUN_001fa580_2118c8(hero.motion.move_yaw, hero.motion.rot.f[2]);
    } else {
        hero.motion.move_yaw = FUN_001fa580_2118c8(hero.motion.move_yaw, D_L00_00166DD8_2118c8);
    }
    x = stick_vector.f[0];
    y = stick_vector.f[1];
    stick_vector.f[1] = -x;
    stick_vector.f[0] = -y;
    stick_vector.f[2] = stick_vector.f[3] = 0.0f;
    if (D_0015EDB4_2118c8) {
        stick_vector.f[1] = x;
    }
    FUN_001f9cf8_2118c8(&hero.motion.unk170, &stick_vector, D_L00_00166FD0_2118c8);
    hero.target_speed = scale * FUN_001f9b20_2118c8(&stick_vector);
}
extern float D_L00_0017BDB8[];

/* Hero speed clamp after a move update (twin of FUN_L01_00232290): in state 0x3F it scales unk190 by 3.5 frame steps with a floor; otherwise it picks a table speed, and in state 0x73 it slows by 0.8 down to a floor. */
void update_hero_target_speed_from_stick(void) __asm__("FUN_L00_00211be8");

void update_hero_target_speed_from_stick(void) {
    stick_target(1.0f, 0);
    if (hero.state.current == 0x3F) {
        float min = frame_time + frame_time;
        hero.target_speed = frame_time * 3.5f * hero.target_speed;
        if (hero.target_speed < min)
            hero.target_speed = min;
        return;
    }
    if (0.0f < hero.target_speed) {
        if (hero.target_speed < D_L00_0017BDB8[3])
            hero.target_speed = D_L00_0017BDB8[2] * frame_time;
        else
            hero.target_speed = D_L00_0017BDB8[6] * frame_time;
    }
    if (hero.state.current == 0x73) {
        float reduced_target_speed = hero.target_speed * 0.8f;
        float minimum_target_speed = frame_time * 2.5f;
        hero.target_speed = reduced_target_speed;
        if (reduced_target_speed < minimum_target_speed)
            hero.target_speed = minimum_target_speed;
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/help_0020CDF0.c: func_L00_002123B0), where it is exact; names translated to the US level program. */

extern char D_0013E533_lui[];
extern void euler_rotation_matrix(void *, void *) __asm__("FUN_001fa050");
extern void multiply_matrix(void *, void *, void *) __asm__("FUN_001fa378");
extern void FUN_00214598(void *, void *);

/* Multiplies the 001fa050 matrices of (x, y, z) and of hero.unk40, stored back by 00214598. */
void apply_hero_rotation_offset(float x, float y, float z) __asm__("FUN_L00_00211cf8");

void apply_hero_rotation_offset(float x, float y, float z) {
    float in[3];
    float input_rotation_matrix[16], hero_rotation_matrix[16];
    in[0] = x;
    in[1] = y;
    in[2] = z;
    euler_rotation_matrix(input_rotation_matrix, in);
    euler_rotation_matrix(hero_rotation_matrix, D_0013E533_lui + 0xEAD);
    multiply_matrix(hero_rotation_matrix, hero_rotation_matrix, input_rotation_matrix);
    FUN_00214598(hero_rotation_matrix, D_0013E533_lui + 0xEAD);
}
typedef float V4_211d78[4] __attribute__((aligned(16)));
void transform_vector(void *, void *, void *) __asm__("FUN_001f9d20");
float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
void apply_hero_rotation_offset(float, float, float) __asm__("FUN_L00_00211cf8");
extern float FUN_L00_0025bc98(char *, char *, int, float, float, float, float);
void turn_hero_toward_transformed_move_direction(int rotation_direction, float x, float y, float z) __asm__("FUN_L00_00211d78");

void turn_hero_toward_transformed_move_direction(int rotation_direction, float x, float y, float z) {
    V4_211d78 transformed_direction;
    float out;
    float zero = 0.0f;
    float transformed_direction_angle;
    transform_vector(transformed_direction, &hero.motion.unk170, hero.unk40);
    transformed_direction_angle = fast_arc_tan(transformed_direction[0], transformed_direction[1]);
    out = zero;
    hero.motion.unk188 = FUN_L00_0025bc98((char *)&out, (char *)&hero.motion.unk184, rotation_direction, transformed_direction_angle, x, y, z);
    apply_hero_rotation_offset(zero, zero, out);
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/help_0020CDF0.c: func_L00_002124E8), where it is exact; names translated to the US level program. */

extern float FUN_L00_0025bc98(char *, char *, int, float, float, float, float);
extern void turn_hero_toward_transformed_move_direction(int, float, float, float) __asm__("FUN_L00_00211d78");

/* Calls 00211d78 when hero.unk20B3 is set, else updates hero.motion.unk188 via 0025bc98. */
void turn_to(int a, float x, float y, float z) __asm__("FUN_L00_00211e30");
void turn_to(int rotation_direction, float x, float y, float z) {
    struct Hero *hero_state = &hero;
    if (hero_state->unk20B3 != 0) {
        turn_hero_toward_transformed_move_direction(rotation_direction, x, y, z);
    } else {
        hero_state->motion.unk188 =
            FUN_L00_0025bc98(&hero_state->motion.rot.f[2], &hero_state->motion.unk184, rotation_direction, hero_state->motion.move_yaw, x, y, z);
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/help_0020CDF0.c: func_L00_00212550), where it is exact; names translated to the US level program. */

extern int FUN_L00_0020d498_u(int) __asm__("FUN_L00_0020d498");
extern void turn_to(int, float, float, float) __asm__("FUN_L00_00211e30");

void turn_hero_toward_move_direction_for_item(void) __asm__("FUN_L00_00211e98");

void turn_hero_toward_move_direction_for_item(void) {
    float frame_scaled_turn_coefficient, frame_scaled_secondary_turn_coefficient, time_scaled_turn_amount;
    if (FUN_L00_0020d498_u(0) == 16) {
        frame_scaled_turn_coefficient = frame_scale_sq * 0.006f;
        frame_scaled_secondary_turn_coefficient = frame_scale_sq * 0.07f;
        time_scaled_turn_amount = frame_time * 13.962634086608887f;
    } else {
        frame_scaled_turn_coefficient = frame_scale_sq * 0.002f;
        frame_scaled_secondary_turn_coefficient = frame_scale_sq * 0.07f;
        time_scaled_turn_amount = frame_time * 6.981317043304443f;
    }
    turn_to(0, frame_scaled_turn_coefficient, frame_scaled_secondary_turn_coefficient, time_scaled_turn_amount);
}
extern float D_0015ED6C_211f38 __asm__("D_0015ED6C");
extern int FUN_L00_0020d498_211f38(int) __asm__("FUN_L00_0020d498");
extern void FUN_L00_00211e30_211f38(int, float, float, float) __asm__("FUN_L00_00211e30");

void turn_hero_toward_move_direction_with_stick_scale(void) __asm__("FUN_L00_00211f38");

void turn_hero_toward_move_direction_with_stick_scale(void) {
    struct Hero *hero_state = &hero;
    float stick_turn_scale, frame_scaled_turn_coefficient, frame_scaled_secondary_turn_coefficient, scaled_turn_amount;
    stick_turn_scale = (hero_state->unk229C + 1.0f) * 0.5f;
    frame_scaled_turn_coefficient = frame_scale_sq * 0.019f * stick_turn_scale;
    frame_scaled_secondary_turn_coefficient = frame_scale_sq * 0.1f;
    scaled_turn_amount = D_0015ED6C_211f38 * 9.948377f * stick_turn_scale;
    if (hero_state->unk3BE) {
        frame_scaled_turn_coefficient *= 1.1f;
        scaled_turn_amount *= 1.2f;
    }
    switch (FUN_L00_0020d498_211f38(0)) {
    case 15:
    case 16:
    case 21:
        if (hero_state->unk20A8) {
            frame_scaled_turn_coefficient = frame_scale_sq * 0.003f;
            frame_scaled_secondary_turn_coefficient = frame_scale_sq * 0.04f;
            scaled_turn_amount = D_0015ED6C_211f38 * 4.3633232f;
        }
        break;
    }
    FUN_L00_00211e30_211f38(0, frame_scaled_turn_coefficient, frame_scaled_secondary_turn_coefficient, scaled_turn_amount);
}
void FUN_00213ed8_00212088(f32 *, f32, f32) __asm__("FUN_00213ed8");
/* Approaches field b toward a, at rate x while below it and y otherwise. */
void speed_step(f32 x, f32 y) __asm__("FUN_L00_00212088");
void speed_step(f32 x, f32 y) {
    struct Hero *hero_state = &hero;
    if (hero_state->ground_speed < hero_state->target_speed)
        FUN_00213ed8_00212088(&hero_state->ground_speed, hero_state->target_speed, x);
    else
        FUN_00213ed8_00212088(&hero_state->ground_speed, hero_state->target_speed, y);
}
extern float fast_cos(float) __asm__("FUN_001f9dc8");

/* Builds a direction vector into hero.motion.velocity: either func_00215C00's
   spherical formula from the hero.ground_speed radius, the clamped angle arg0
   (falling back to hero.motion.rot.z above pi) and hero +0x2E4, or, when
   hero.unk20B3 is set, a flat vector (r,0,0) rotated by the matrix
   rotation_matrix builds from the hero moby's +0x40 axis. */
/* Ported from rac1-decomp (src/overlays/shared/help_0020CDF0.c: func_L00_00212790), where it is exact; names translated to the US level program. */

extern float fast_sin(float) __asm__("FUN_001f9de0");
extern void transform_vector(void *, void *, void *) __asm__("FUN_001f9d20");
extern void rotation_matrix(void *, void *) __asm__("FUN_001fa050");
extern void func_00215C00(void *, float, float, float) __asm__("FUN_00214db0");

void set_planar_vel(float radians) __asm__("FUN_L00_002120d8");
void set_planar_vel(float radians) {
    struct Hero *hero_state = &hero;
    float y;
    float ground_speed;
    float out[4];

    y = hero_state->motion.rot.f[2];
    if (radians <= 3.14159274f) {
        y = radians;
    }
    ground_speed = hero_state->ground_speed;

    if (hero_state->unk20B3 != 0) {
        float matrix[16];
        float velocity_vector[4];

        velocity_vector[0] = fast_cos(0.0f) * ground_speed;
        velocity_vector[1] = fast_sin(0.0f) * ground_speed;
        velocity_vector[2] = 0.0f;

        rotation_matrix(matrix, ((char *)hero_state->moby) + 0x40);
        transform_vector(velocity_vector, velocity_vector, matrix);

        qcopy(out, velocity_vector);
    } else {
        func_00215C00(out, ground_speed, y, hero_state->ground_pitch.f);
    }

    qcopy(&hero.motion.velocity, out);
}
typedef unsigned int u128_2121c0_002121c0 __attribute__((mode(TI), aligned(16)));
typedef union {
    u128_2121c0_002121c0 q_002121c0;
    float f[4];
} V_2121c0_002121c0;
extern float frame_time_sq __asm__("D_0015ED70");
extern float FUN_001f9b20_002121c0(void *) __asm__("FUN_001f9b20");
extern float FUN_001f9e90_002121c0(float, float) __asm__("FUN_001f9e90");
extern void FUN_00125180_002121c0(void *, void *) __asm__("FUN_00125180");
extern void FUN_001f9a68_002121c0(void *, void *, float) __asm__("FUN_001f9a68");
extern void FUN_001f9a10_002121c0(void *, void *, void *) __asm__("FUN_001f9a10");

void correct_hero_airborne_wall_velocity(void) __asm__("FUN_L00_002121c0");

void correct_hero_airborne_wall_velocity(void) {
    struct Hero *hero_state = &hero;
    V_2121c0_002121c0 horizontal_velocity_correction;
    float inward_velocity_component, x;
    if (hero_state->air_frames.s == 0)
        return;
    if (!(hero_state->ground_distance < 0.15f))
        return;
    if (!(0.0f < hero_state->motion.displacement.f[2]))
        return;
    if (hero_state->wall_hit == 0)
        return;
    if (!(0.0f < hero_state->unk200.f[2]))
        return;
    if (!(0.87266463f <= FUN_001f9e90_002121c0(hero_state->unk200.f[2], FUN_001f9b20_002121c0(&hero_state->unk200))))
        return;
    qcopy(&horizontal_velocity_correction, &hero_state->unk200);
    horizontal_velocity_correction.f[2] = 0.0f;
    FUN_00125180_002121c0(&horizontal_velocity_correction, &horizontal_velocity_correction);
    inward_velocity_component = -hero_state->motion.velocity.f[0] * horizontal_velocity_correction.f[0] - hero_state->motion.velocity.f[1] * horizontal_velocity_correction.f[1];
    if (0.0f < inward_velocity_component) {
        FUN_001f9a68_002121c0(&horizontal_velocity_correction, &horizontal_velocity_correction, inward_velocity_component);
        FUN_001f9a10_002121c0(&hero_state->motion.velocity, &hero_state->motion.velocity, &horizontal_velocity_correction);
        x = frame_time_sq * 54.0f;
        hero_state->motion.velocity.f[2] -= x + x;
    }
}
typedef struct {
    f32 f[4];
} V_212318;
typedef struct {
    u8 p0[0x18];
    s32 i18;
    s32 i1C;
    u8 p1[0x40 - 0x20];
    V_212318 v40;
} X_212318;
extern char D_0013F3D0_212318[] __asm__("D_0013F3D0") __attribute__((section(".data")));
extern X_212318 D_L00_00173E40_212318 __asm__("D_L00_00173E40") __attribute__((section(".data")));
extern f32 D_0015ED6C_212318 __asm__("D_0015ED6C");
extern f32 FUN_L00_002339d0_212318(void *) __asm__("FUN_L00_002339d0");
extern f32 FUN_001f9b20_212318(void *) __asm__("FUN_001f9b20");
extern f32 FUN_001f9dc8_212318(f32) __asm__("FUN_001f9dc8");
extern f32 FUN_001f9de0_212318(f32) __asm__("FUN_001f9de0");
extern void FUN_001f9a68_212318(void *, void *, f32) __asm__("FUN_001f9a68");
extern void FUN_001f9a10_212318(void *, void *, void *) __asm__("FUN_001f9a10");
extern int FUN_001efa68_212318(void *, void *, int, void *, int) __asm__("FUN_001efa68");
extern int FUN_001f0b58_212318(void) __asm__("FUN_001f0b58");
extern f32 FUN_001f9e90_212318(f32, f32) __asm__("FUN_001f9e90");
extern void FUN_00125180_212318(void *, void *) __asm__("FUN_00125180");
#define X D_L00_00173E40_212318

extern s32 D_L00_00173E5C __attribute__((section(".data")));
void clip_hero_velocity_against_probed_surfaces(int force) __asm__("FUN_L00_00212318");

void clip_hero_velocity_against_probed_surfaces(int force) {
    V_212318 primary_probe_start, primary_probe_offset_or_end, b, secondary_probe_offset_or_end;
    char *hero_position_base;
    struct Hero *g;
    f32 primary_probe_height_offset, k, secondary_probe_length_scale, primary_hit_normal_angle, inward_velocity_component;
    int r;

    if (hero.unk308 != 0)
        return;
    if (FUN_L00_002339d0_212318(&hero.motion.velocity) < D_0015ED6C_212318 * 0.1f)
        return;
    primary_probe_height_offset = 0.3700000047683716f;
    k = 0.5f;
    if ((u32)(hero.state.control_mode - 0x11) < 2) {
        primary_probe_height_offset = 0.0f;
        k = 0.7f;
    } else if (hero.vehicle_mode == 1) {
        primary_probe_height_offset = 0.35f;
        k = 0.27000001072883606f;
    } else if (hero.vehicle_mode == 2) {
        primary_probe_height_offset = 1.5f;
        k = 5.0f;
    }
    hero_position_base = D_0013F3D0_212318;
    qcopy(&primary_probe_start, hero_position_base);
    primary_probe_start.f[2] += primary_probe_height_offset;
    qcopy(&b, hero_position_base);
    b.f[2] += primary_probe_height_offset * 0.3f;
    if (force || FUN_001f9b20_212318(hero_position_base + 0x60) < 0.01f) {
        g = (struct Hero *)(hero_position_base - 0x80);
        secondary_probe_length_scale = 1.4f;
        primary_probe_offset_or_end.f[0] = FUN_001f9dc8_212318(g->motion.rot.f[2]) * k;
        primary_probe_offset_or_end.f[1] = FUN_001f9de0_212318(g->motion.rot.f[2]) * k;
        *(s32 *)&primary_probe_offset_or_end.f[2] = 0;
        secondary_probe_offset_or_end.f[0] = FUN_001f9dc8_212318(g->motion.rot.f[2]) * k * secondary_probe_length_scale;
        secondary_probe_offset_or_end.f[1] = FUN_001f9de0_212318(g->motion.rot.f[2]) * k * secondary_probe_length_scale;
        *(s32 *)&secondary_probe_offset_or_end.f[2] = 0;
    } else {
        qcopy(&primary_probe_offset_or_end, hero_position_base + 0x60);
        *(s32 *)&primary_probe_offset_or_end.f[2] = 0;
        FUN_001f9a68_212318(&primary_probe_offset_or_end, &primary_probe_offset_or_end, k / FUN_001f9b20_212318(&primary_probe_offset_or_end));
        qcopy(&secondary_probe_offset_or_end, &primary_probe_offset_or_end);
        FUN_001f9a68_212318(&secondary_probe_offset_or_end, &secondary_probe_offset_or_end, 1.4f);
    }
    FUN_001f9a10_212318(&primary_probe_offset_or_end, &primary_probe_offset_or_end, &primary_probe_start);
    FUN_001f9a10_212318(&secondary_probe_offset_or_end, &secondary_probe_offset_or_end, &b);
    r = -1;
    if (FUN_001efa68_212318(&b, &secondary_probe_offset_or_end, 4, hero.moby, 0) && D_L00_00173E5C > 0)
        r = FUN_001f0b58_212318();
    primary_hit_normal_angle = 0.0f;
    if (FUN_001efa68_212318(&primary_probe_start, &primary_probe_offset_or_end, 4, hero.moby, 0) && X.i1C > 0)
        primary_hit_normal_angle = FUN_001f9e90_212318(X.v40.f[2], FUN_001f9b20_212318(&X.v40));
    if (0.8726646304130554f <= primary_hit_normal_angle || r == 8 || r == 12) {
        hero.unk240 = X.i18;
        *(s32 *)&X.v40.f[2] = 0;
        FUN_00125180_212318(&X.v40, &X.v40);
        inward_velocity_component = -hero.motion.velocity.f[0] * X.v40.f[0] - hero.motion.velocity.f[1] * X.v40.f[1];
        if (0.0f < inward_velocity_component) {
            FUN_001f9a68_212318(&X.v40, &X.v40, inward_velocity_component);
            FUN_001f9a10_212318(&hero.motion.velocity, &hero.motion.velocity, &X.v40);
        }
    }
}
typedef unsigned int u128_002126b8 __attribute__((mode(TI), aligned(16)));
typedef struct {
    float x, y, z, w_002126b8;
} __attribute__((aligned(16))) Vec4_002126b8;
void FUN_001f9bf8_002126b8(Vec4_002126b8 *, void *, float) __asm__("FUN_001f9bf8");
void FUN_001f9a10_002126b8(void *, void *, void *) __asm__("FUN_001f9a10");
void clip_negative_segment_endpoint_xyz(Vec4_002126b8 *, void *) __asm__("FUN_L00_0025d238");
void hero_get_collision_ray_endpoints(Vec4_002126b8 *, Vec4_002126b8 *, int, float, float) __asm__("FUN_L00_002126b8");
void hero_get_collision_ray_endpoints(Vec4_002126b8 *ray_start, Vec4_002126b8 *ray_end, int mode, float fa, float fb) {
    Vec4_002126b8 ray_scratch_vectors[2];
    if (mode) {
        char *up = (char *)&hero.gravity_dir;
        char *base = up - 0x210;
        FUN_001f9bf8_002126b8(&ray_scratch_vectors[0], up, -fb);
        FUN_001f9a10_002126b8(ray_end, base, &ray_scratch_vectors[0]);
        FUN_001f9bf8_002126b8(&ray_scratch_vectors[0], up, -fa);
        FUN_001f9a10_002126b8(ray_start, base, &ray_scratch_vectors[0]);
        *(u128_002126b8 *)&ray_scratch_vectors[1] = *(u128_002126b8 *)ray_start;
        clip_negative_segment_endpoint_xyz(&ray_scratch_vectors[1], ray_end);
    } else {
        qcopy(&ray_scratch_vectors[0], &hero.motion.pos);
        qcopy(ray_end, &ray_scratch_vectors[0]);
        ray_end->z += fb;
        if (ray_end->z < 0.0f)
            ray_end->z = 0.0f;
        qcopy(ray_start, &ray_scratch_vectors[0]);
        ray_start->z += fa;
    }
}
typedef union {
    OvlQuad q;
    f32 f[4];
} V;
extern u8 D_L00_00173E40[] __attribute__((section(".data")));
extern V D_L00_00173E60_lui __asm__("D_L00_00173E60") __attribute__((section(".data")));
extern f32 D_L00_00173E68 __attribute__((section(".data")));

void FUN_001f99f8_c(void *) __asm__("FUN_001f99f8");
s32 collision_line(void *, void *, s32, void *, s32) __asm__("FUN_001efa68");
s32 coll_sound_class(void) __asm__("FUN_L00_001f3608");
s32 collision_hit_material(void) __asm__("FUN_001f0b58");
f32 resolve_camera_surface_height(void *, void *) __asm__("FUN_002135f0");
s32 is_moby_in_class_range_500_to_540(s32) __asm__("FUN_L00_0025e3b8");
u8 *suck_get_aim_height(s32) __asm__("FUN_002141f8");
f32 FUN_001f9b48(void *, void *);
void sceVu0Normalize(void *, void *) __asm__("FUN_00125180");
f32 compute_gravity_relative_vector_inclination(void *) __asm__("FUN_L00_00233d30");
f32 FUN_001f9e90_c(f32, f32) __asm__("FUN_001f9e90");
void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
f32 FUN_001f9ab0_c(void *, void *) __asm__("FUN_001f9ab0");
void FUN_001f9a68_c(void *, void *, f32) __asm__("FUN_001f9a68");
void FUN_001f9bf8_c(void *, void *, f32) __asm__("FUN_001f9bf8");
void compute_rotated_vector_angles(void *, void *) __asm__("FUN_L00_00216078");
void transform_hero_polar_offset_to_world(void *, f32, f32, f32) __asm__("FUN_L00_002335c8");
f32 FUN_001f99c0(f32);

extern CollisionHit HH __asm__("D_L00_00173E40") __attribute__((section(".data")));
#define HI(o) (*(s32 *)(D_L00_00173E40 + (o)))
#define HF(o) (*(f32 *)(D_L00_00173E40 + (o)))

void update_hero_ground_contact_and_surface_slopes(void) __asm__("FUN_L00_002127b8");

void update_hero_ground_contact_and_surface_slopes(void) {
    V collision_ray_end, ground_ray_start, ground_offset_or_angles, side_ray_start, unused_side_probe_point, side_hit_angles, side_hit_normal;
    f32 f;
    s32 collision_sound_class;
    s32 i;
    f32 side_probe_angle;
    u8 *hit_moby_flag_data;

    s32 k;
    f32 ground_ray_start_offset;

    hero.unk270.f[2] = 1.0f;
    hero.air_frames.u++;
    hero.frames_off_ground.u++;
    hero.unk270.i[0] = 0;
    hero.unk270.i[1] = 0;
    hero.ground_slope.i = 0;
    hero.ground_z.i = 0;
    FUN_001f99f8_c(&hero.ground_point);
    hero.unk30A = 0;
    hero.ground_distance = 32.0f;
    hero.ground_moby = 0;
    if (hero.unk2F8 != 0) {
        hero.unk2F8++;
    }
    k = 0;
    if (hero.unk20B3)
        k = 1;
    ground_ray_start_offset = hero.unk234 * 2.2f;
    if (hero.state.control_mode == 2) {
        ground_ray_start_offset = hero.unk234 * 2.9f;
    }
    hero_get_collision_ray_endpoints((Vec4_002126b8 *)&ground_ray_start, (Vec4_002126b8 *)&collision_ray_end, k, ground_ray_start_offset, -48.0f);
    if (collision_line(&ground_ray_start, &collision_ray_end, 2, hero.moby, 0) == 0) {
        goto tail;
    }
    collision_sound_class = coll_sound_class();
    if (collision_sound_class == -1) {
        collision_sound_class = 3;
    }
    hero.unk12ED = collision_sound_class;
    hero.ground_type = collision_hit_material();
    if (hero.ground_type == 0) {
        hero.water_surface_z = resolve_camera_surface_height(((u8 *)&D_L00_00173E60_lui), &hero.unk280);
        if (hero.motion.pos.f[2] < hero.water_surface_z && hero.unk2F8 <= 0) {
            hero.unk2F8 = 1;
        }
        if (collision_line(&ground_ray_start, &collision_ray_end, 0x24, hero.moby, 0) == 0) {
            goto d1;
        }
        collision_sound_class = coll_sound_class();
        if (current_level_index == 1 || current_level_index == 0x12) {
            collision_sound_class = 3;
        }
        hero.unk12ED = collision_sound_class;
    }
    if (hero.ground_type == 0xD) {
        hero.water_surface_z = D_L00_00173E68;
        ground_ray_start.f[2] = D_L00_00173E68 - 0.01f;
        if (collision_line(&ground_ray_start, &collision_ray_end, 4, hero.moby, 0) == 0) {
            goto d1;
        }
    }
    if (hero.ground_type == 3) {
        hero.unk2F4 = D_L00_00173E68;
        ground_ray_start.f[2] = D_L00_00173E68 - 0.01f;
        if (collision_line(&ground_ray_start, &collision_ray_end, 4, hero.moby, 0) == 0) {
            goto d1;
        }
    }
    if (hero.ground_type == 0xB) {
        hero.unk2F4 = D_L00_00173E68;
        goto d1;
    }
    if (hero.ground_type == 1 && (u32)(hero.state.current - 0x7C) < 2) {
        goto d1;
    }
    if (HH.moby != 0) {
        if (is_moby_in_class_range_500_to_540(HH.moby)) {
            hero.unk30A = 1;
        }
        hit_moby_flag_data = suck_get_aim_height(HH.moby);
        if (hit_moby_flag_data != 0 && (*(u16 *)(hit_moby_flag_data + 0x1E) & 8)) {
            hero.unk30A = 1;
        }
    }
    if (HH.unk1C > 0) {
        qcopy(&hero.ground_point, &HH.point);
        hero.ground_z.f = HH.point.f[2];
        hero.ground_distance = FUN_001f9b48(&hero.motion.pos, &hero.ground_point);
        hero.ground_moby = (struct Moby *)HH.moby;
        sceVu0Normalize(&hero.unk270, (u8 *)&HH.normal_x);
        hero.ground_slope.f = compute_gravity_relative_vector_inclination((u8 *)&HH.normal_x);
        hero.downhill_yaw = FUN_001f9e90_c(HH.normal_x, HH.normal_y);
        fast_vec_sub(&ground_offset_or_angles, &hero.ground_point, &hero.motion.pos);
        if (0.0f < FUN_001f9ab0_c(&ground_offset_or_angles, &hero.unk270)) {
            hero.ground_distance = -hero.ground_distance;
        }
        if (hero.ground_distance < 0.02f) {
            hero.frames_off_ground.s = 0;
            if (hero.ground_slope.f <= 0.87266463f || hero.unk20B3 == 1 || hero.state.control_mode == 0x16) {
                hero.air_frames.s = 0;
                hero.unkA80 = HH.moby;
            }
        }
    }
tail:
    if (hero.air_frames.s != 0) {
        hero.grounded_frames = 0;
    } else {
        hero.grounded_frames++;
        qcopy(&hero.unk2B0, &hero.ground_point);
    }
    if (hero.motion.pos.f[2] > hero.water_surface_z) {
        hero.unk2F8 = 0;
    }
d1:
    FUN_001f99f8_c(&hero.gravity_dir);
    switch (hero.unk20B3) {
    case HERO_GRAVITY_WORLD_DOWN:
        hero.gravity_dir.f[2] = -1.0f;
        break;
    case HERO_GRAVITY_OPPOSITE_GROUND_NORMAL:
        qcopy(&hero.gravity_dir, &hero.unk270);
        FUN_001f9a68_c(&hero.gravity_dir, &hero.gravity_dir, -1.0f);
        break;
    case HERO_GRAVITY_TOWARD_REFERENCE_POINT:
        fast_vec_sub(&hero.gravity_dir, &hero.motion.unkB0, &hero.motion.pos);
        FUN_001f9bf8_c(&hero.gravity_dir, &hero.gravity_dir, 1.0f);
        break;
    }
    hero.ground_pitch.i = 0;
    hero.unk2E8.i = 0;
    hero.unk2D0.i[0] = 0;
    hero.unk2D0.i[1] = 0;
    hero.unk2C8.i[0] = 0;
    hero.unk2C8.i[1] = 0;
    hero.unk2C0.i[0] = 0;
    hero.unk2C0.i[1] = 0;
    if (hero.state.control_mode == 0xF || hero.state.control_mode == 0x15 || hero.state.control_mode == 6 || hero.state.control_mode == 4 ||
        hero.state.control_mode == 5 || hero.state.control_mode == 3) {
        return;
    }
    if (!(hero.ground_distance < 0.25f)) {
        return;
    }
    compute_rotated_vector_angles(&hero.unk270, &ground_offset_or_angles);
    if (-0.78539819f < ground_offset_or_angles.f[1] && ground_offset_or_angles.f[1] < 0.78539819f) {
        hero.ground_pitch.f = ground_offset_or_angles.f[1];
    }
    if (-0.78539819f < ground_offset_or_angles.f[0] && ground_offset_or_angles.f[0] < 0.78539819f) {
        hero.unk2E8.f = ground_offset_or_angles.f[0];
    }
    for (i = 0; i < 2; i++) {
        side_probe_angle = 1.5707964f;
        if (i == 1) {
            side_probe_angle = -side_probe_angle;
        }
        transform_hero_polar_offset_to_world(&side_ray_start, 0.2f, side_probe_angle, 0.45f);
        transform_hero_polar_offset_to_world(&unused_side_probe_point, 0.2f, side_probe_angle, -0.45f);
        if (collision_line(&side_ray_start, &collision_ray_end, 0x22, hero.moby, 0)) {
            f = HH.point.f[2] - hero.ground_z.f;
            hero.unk2C0.f[i] = f;
            if (0.4f < FUN_001f99c0(f)) {
                hero.unk2C0.f[i] = 0.0f;
            }
            sceVu0Normalize(&side_hit_normal, (u8 *)&HH.normal_x);
            compute_rotated_vector_angles(&side_hit_normal, &side_hit_angles);
            if (-0.78539819f < side_hit_angles.f[1] && side_hit_angles.f[1] < 0.78539819f) {
                hero.unk2C8.f[i] = side_hit_angles.f[1];
            }
            if (-0.78539819f < side_hit_angles.f[0] && side_hit_angles.f[0] < 0.78539819f) {
                hero.unk2D0.f[i] = side_hit_angles.f[0];
            }
        } else {
            hero.unk2C0.i[i] = 0;
            hero.unk2C8.f[i] = hero.ground_pitch.f;
            hero.unk2D0.f[i] = hero.unk2E8.f;
        }
    }
}
/* Snaps the hero's follow position toward its target, stepping up by a bounded amount. */
/* Ported from rac1-decomp (src/overlays/shared/help_0020CDF0.c: func_L00_002136A8), where it is exact; names translated to the US level program. */

extern f32 D_0015ED6C_c __asm__("D_0015ED6C");
extern f32 dot_vectors_xyz(void *, void *) __asm__("func_001F9AB0");
extern f32 vector_length_xyz(void *) __asm__("FUN_001f9af0");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float vendor_spring(float *p, float *v, float t, float u1, float u2, float eps) __asm__("FUN_L00_0025b8c0");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
float absolute_float(float input) __asm__("func_001F99C0");
extern float func_L00_0025C918_2136A8(float *p, float t, float *v, float u1, float u2, float eps) __asm__("FUN_L00_0025b8c0");

void correct_hero_ground_penetration(void) __asm__("FUN_L00_00212ff0");

void correct_hero_ground_penetration(void) {
    struct Hero *globals = &hero;
    float ground_position_correction[4];
    if (globals->unk20B3 != 0) {
        float velocity_dot = dot_vectors_xyz(&globals->motion.velocity, &globals->unk270);
        float ground_dist;
        float ground_correction_length;
        if (globals->air_frames.s != 0) return;
        ground_dist = globals->ground_distance;
        if (!(ground_dist < 0.0f)) return;
        if (ground_dist < velocity_dot + 0.0001f || globals->state.current == 0x3E) {
            qcopy(&globals->motion.pos, &globals->ground_point);
            return;
        }
        subtract_vector_xyz(ground_position_correction, &globals->ground_point, &globals->motion.pos);
        ground_correction_length = vector_length_xyz(ground_position_correction);
        if (ground_correction_length < frame_scale * 0.21f) {
            if (frame_scale * 0.05f < ground_correction_length) ground_correction_length = frame_scale * 0.05f;
        } else {
            ground_correction_length = ground_correction_length * 0.5f;
        }
        normalize_vector_xyz(ground_position_correction, ground_position_correction, ground_correction_length);
        add_vector_xyz(&hero.motion.pos, &hero.motion.pos, ground_position_correction);
    } else {
        if (globals->air_frames.s == 0 && globals->motion.pos.f[2] < globals->ground_z.f) {
            float x = absolute_float(globals->motion.pos.f[2] - globals->ground_z.f);
            float y = absolute_float(globals->motion.velocity.f[2]) + 0.01f;
            if (x < y) {
                if (globals->wall_hit != 0) {
                    float distance_to_hero_position = vector_distance(&globals->unk210, &globals->motion.pos);
                    if (globals->unk234 * 0.5f < distance_to_hero_position) {
                        globals->motion.pos.f[2] = globals->ground_z.f;
                        return;
                    }
                } else {
                    globals->motion.pos.f[2] = globals->ground_z.f;
                    return;
                }
                func_L00_0025C918_2136A8((float *)(&globals->motion.pos.f[2]), globals->ground_z.f, (float *)(&globals->unk304), frame_scale_sq * 0.057f, frame_scale_sq * 0.3f, D_0015ED6C_c * 2.0f);
            } else {
                float displacement_z = globals->motion.displacement.f[2];
                char *position_z_ptr;
                if (displacement_z < 0.0f) {
                    float displacement_reverted_z = globals->motion.pos.f[2] - displacement_z;
                    float ground_z_limit = globals->ground_z.f;
                    globals->motion.pos.f[2] = displacement_reverted_z;
                    if (ground_z_limit < displacement_reverted_z) globals->motion.pos.f[2] = ground_z_limit;
                }
                position_z_ptr = D_0013E533_lui + 0xEA5;
                func_L00_0025C918_2136A8((float *)position_z_ptr, *(float *)(position_z_ptr + 0x250), (float *)(position_z_ptr + 0x27C), frame_scale_sq * 0.057f, frame_scale_sq * 0.3f, D_0015ED6C_c + D_0015ED6C_c);
            }
        } else {
            struct Hero *hero_state = &hero;
            *(int *)&hero_state->unk304 = 0;
        }
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/help_0020CDF0.c: func_L00_00213970), where it is exact; names translated to the US level program. */

extern char D_0013E533_lui[];
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
void build_spherical_offset(f32 *out, f32 scale, f32 a, f32 b) __asm__("FUN_00214db0");

void build_scaled_hero_move_impulse(float scale_factor, float impulse_length_reduction) __asm__("FUN_L00_002132b8");

void build_scaled_hero_move_impulse(float scale_factor, float impulse_length_reduction) {
    char *move_impulse = D_0013E533_lui + 0x173D;
    char *hero_base = move_impulse - 0x920;
    float scaled_impulse_length;
    build_spherical_offset(move_impulse, *(float *)(hero_base + 0x930), *(float *)(hero_base + 0x938), *(float *)(hero_base + 0x934));
    fast_vec_scale(move_impulse, move_impulse, scale_factor);
    scaled_impulse_length = fast_vec_length(move_impulse);
    if (scaled_impulse_length <= impulse_length_reduction) {
        clear_u64_value(move_impulse);
    } else {
        fast_vec_normalize(move_impulse, move_impulse, scaled_impulse_length - impulse_length_reduction);
    }
}
typedef unsigned int u128_213350 __attribute__((mode(TI), aligned(16)));
typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) Vec4_213350;
float fast_cos(float) __asm__("FUN_001f9dc8");
float fast_sin(float) __asm__("FUN_001f9de0");
void FUN_001f9ab0(void *, Vec4_213350 *);
/* dot_vectors_xyz of v with (cos, sin, 0) of the angle hero.motion.rot.z. */
void FUN_L00_00213350(u128_213350 *input_vector) {
    u128_213350 dot_operands[2];
    struct Hero *hero_state = &hero;
    dot_operands[0] = *input_vector;
    ((Vec4_213350 *)&dot_operands[1])->x = fast_cos(hero_state->motion.rot.f[2]);
    ((Vec4_213350 *)&dot_operands[1])->y = fast_sin(hero_state->motion.rot.f[2]);
    ((Vec4_213350 *)&dot_operands[1])->z = 0.0f;
    FUN_001f9ab0(&dot_operands[0], (Vec4_213350 *)&dot_operands[1]);
}
typedef unsigned int u128_2133a8 __attribute__((mode(TI), aligned(16)));
#include "qcopy.h"
typedef union {
    u128_2133a8 q;
    f32 f[4];
} V_2133a8;
extern u128_2133a8 coll_2133a8[] __asm__("D_L00_00173E70") __attribute__((section(".data")));
extern void vzero_2133a8(void *) __asm__("FUN_001f99f8");
extern void FUN_L00_002347c0(void);
extern void FUN_L00_00233810(void *, void *, f32);
extern void FUN_L00_002334d0(void *, void *, f32);
extern void vadd_2133a8(void *, void *, void *) __asm__("FUN_001f9a10");
extern void vsub_2133a8(void *, void *, void *) __asm__("FUN_001f9a28");
extern f32 vlen_2133a8(void *) __asm__("FUN_001f9af0");
extern s32 coll_sphere(void *, s32, s32, f32) __asm__("FUN_L00_001f0d60");
extern s32 coll2_2133a8(void *, s32, f32, s32, f32) __asm__("FUN_L00_001f19a0");
extern s32 FUN_L00_001f3170(void *, f32);
extern void vscl_2133a8(void *, void *, f32) __asm__("FUN_001f9bf8");
s32 resolve_hero_current_position_collisions(s32 mode) __asm__("FUN_L00_002133a8");

s32 resolve_hero_current_position_collisions(s32 mode) {
    V_2133a8 old;
    V_2133a8 collision_probe_offset;
    V_2133a8 collision_correction;
    s32 mask;
    s32 i;
    u128_2133a8 *collision_result;
    f32 probe_height_or_correction_length;
    if (hero.unk1CC != 0) {
        return 1;
    }
    qcopy(&old, &hero.motion.pos.q);
    vzero_2133a8(&collision_probe_offset);
    if (hero.unk22DA != 0) {
        FUN_L00_002347c0();
        hero.unk22DA = 0;
    }
    if (hero.state.control_mode != 0x11) {
        if (hero.unk20B3 != 0 || hero.unk1F8 != 0) {
            if (hero.unk20B3 == 1 || hero.unk1F8 != 0) {
                FUN_L00_00233810(&collision_probe_offset, &collision_probe_offset, 0.6f);
            } else {
                FUN_L00_002334d0(&collision_probe_offset, &collision_probe_offset, -hero.unk224);
            }
        } else {
            collision_probe_offset.f[2] = hero.unk224;
        }
    }
    vadd_2133a8(&hero.motion.pos.q, &hero.motion.pos.q, &collision_probe_offset);
    mask = 0x24;
    if (hero.state.current == HERO_STATE_SINK) {
        mask = 0xD24;
    }
    for (i = 0; i < 8; i++) {
        if (hero.unk20B3 != 0) {
            if (!coll_sphere(&hero.motion.pos.q, mask, ((s32)hero.moby), frame_scale * 0.4f))
                break;
        } else if (hero.state.control_mode == 0x11) {
            if (!coll_sphere(&hero.motion.pos.q, mask, ((s32)hero.moby), 0.6f))
                break;
        } else if (hero.state.control_mode == 0xF) {
            if (!coll_sphere(&hero.motion.pos.q, mask, ((s32)hero.moby), frame_scale * 0.45f))
                break;
        } else {
            s32 collision_query_result;
            probe_height_or_correction_length = hero.capsule_top - hero.unk224;
            if (probe_height_or_correction_length < 0.05f) {
                probe_height_or_correction_length = 0.05f;
            }
            collision_query_result = coll2_2133a8(&hero.motion.pos.q, mask, hero.unk234, ((s32)hero.moby), probe_height_or_correction_length);
            if (!(collision_query_result | FUN_L00_001f3170(&hero.motion.pos.q, hero.unk234)))
                break;
        }
        collision_result = coll_2133a8;
        qcopy(&hero.motion.pos.q, collision_result);
        qcopy(&hero.unk200.q, collision_result + 1);
        qcopy(&hero.unk210.q, collision_result - 1);
        hero.wall_hit = 1;
        hero.coll_hit_moby = ((u8 **)collision_result)[-6];
    }
    vsub_2133a8(&hero.motion.pos.q, &hero.motion.pos.q, &collision_probe_offset);
    vsub_2133a8(&collision_correction, &hero.motion.pos.q, &old);
    probe_height_or_correction_length = vlen_2133a8(&collision_correction);
    if (hero.unk234 * 1.5f < probe_height_or_correction_length) {
        if (mode == 0xF) {
            if (512.0f < collision_correction.f[0])
                collision_correction.f[0] = 512.0f;
            else if (collision_correction.f[0] < -512.0f)
                collision_correction.f[0] = -512.0f;
            if (512.0f < collision_correction.f[1])
                collision_correction.f[1] = 512.0f;
            else if (collision_correction.f[1] < -512.0f)
                collision_correction.f[1] = -512.0f;
            if (512.0f < collision_correction.f[2])
                collision_correction.f[2] = 512.0f;
            else if (collision_correction.f[2] < -512.0f)
                collision_correction.f[2] = -512.0f;
            vscl_2133a8(&collision_correction, &collision_correction, hero.unk234);
            vadd_2133a8(&hero.motion.pos.q, &old, &collision_correction);
        }
        return -1;
    }
    return 1;
}
/* Ported from rac1-decomp (src/overlays/shared/help_0020CDF0.c: func_L00_00213E60), where it is exact; names translated to the US level program. */

typedef struct {
    int x, y, z, w;
} Quad;

extern int FUN_L00_002133a8_002137a8(int index) __asm__("FUN_L00_002133a8");

void resolve_hero_collisions_with_history(void) __asm__("FUN_L00_002137a8");

void resolve_hero_collisions_with_history(void) {
    Quad *hero_position = (Quad *)&hero.motion.pos;
    Quad *table = (Quad *)hero.unk1B00;
    int i;

    for (i = 0; i < 16; i++) {
        struct Hero *base;

        if (FUN_L00_002133a8_002137a8(i) == 1)
            return;

        base = &hero;
        if (base->unk21B4 >= 2) {
            base->unk21B4--;
            base->unk21B0 = (base->unk21B0 + 31) % 32;
        }
        qcopy(hero_position, &table[base->unk21B0]);
    }
}
#include "rnc/gameplay/state/usage_stats.h"
#include "eetypes.h"
#include "qcopy.h"
/*
 * Hero movement step (FUN_L01_00233de0 twin; control mode 0xF caps the horizontal speed, 0xD and 0xE skip the cap): clamps the velocity, moves the hero by it (plus the
 * one-frame impulse at unk920 and the push at unkF0), runs collision, then
 * derives this frame's movement vectors and speeds:
 *   motion.unk100  position change this frame
 *   motion.unk110  movement direction scaled by the velocity along it
 *   motion.unk120  same, without the gravity-up component
 *   motion.unk130  same, gravity-up part only
 *   motion.unk140  displacement caused by the push
 *   motion.speed_xy  speed, unk164 horizontal speed, unk168 forward speed
 *   motion.unk16C  slope: vertical / horizontal change, clamped to +-0.5
 */

/* The FUN_L00_00233xxx helpers work in the hero's gravity frame (hero byte
   0x20B3: 0 = world z up, otherwise the frame of the hero's moby). */
extern void update_hero_attachment(s32, void *, void *) __asm__("FUN_L00_00233f80");
extern f32 gravity_length_xy(void *) __asm__("FUN_L00_002339d0");
extern f32 gravity_z(void *) __asm__("FUN_L00_00233a78");
extern void gravity_set_z(void *, void *, f32) __asm__("FUN_L00_00233b20");
extern void gravity_flatten(void *, void *) __asm__("FUN_L00_002338d0");
extern void set_gravity_relative_vector_length_xy(float *, float *, float) __asm__("FUN_L00_00233ba0");
extern f32 forward_component(void *) __asm__("FUN_L00_00213350");  /* dot with (cos, sin) of the hero yaw */

extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern f32 vector_length_xyz(void *) __asm__("FUN_001f9af0");
extern f32 dot_vectors_xyz(void *, void *) __asm__("FUN_001f9ab0");
extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void scale_vector_xyz(void *, void *, f32) __asm__("FUN_001f9a68");
extern void clear_vector(void *) __asm__("FUN_001f99f8");
extern void normalize_vector_xy(void *, void *, f32) __asm__("FUN_001f9c48");

extern void resolve_hero_collisions_with_history(void) __asm__("FUN_L00_002137a8");
extern void update_hero_ground_contact_and_surface_slopes(void) __asm__("FUN_L00_002127b8");
extern void correct_hero_ground_penetration(void) __asm__("FUN_L00_00212ff0");

void apply_hero_motion_and_resolve_collisions(void) __asm__("FUN_L00_00213880");

void apply_hero_motion_and_resolve_collisions(void)
{
    Vec4 old_pos;
    Vec4 movement_scratch_vector;
    Vec4 gravity_axis_velocity;
    f32 limit;
    f32 velocity_measurement;
    f32 saved_platform_delta_w;

    qcopy(&old_pos, &hero.motion.pos);
    update_hero_attachment(hero.unk4F8, &hero.motion.platform_delta, &hero.motion.rot);

    /* cap the speed; states 0x22 and 0x14 cap the horizontal part and the fall speed */
    if (hero.state.current == 0x22 || hero.state.current == HERO_STATE_WRENCH_SLAM) {
        if (hero.unk1CC == 0) {
            velocity_measurement = gravity_length_xy(&hero.motion.velocity);
            limit = hero.unk234 - 0.02f;
            if (limit < velocity_measurement) {
                normalize_vector_xyz(&hero.motion.velocity, &hero.motion.velocity, limit);
            }
        }
        velocity_measurement = gravity_z(&hero.motion.velocity);
        limit = -hero.ground_distance;
        if (velocity_measurement < limit) {
            if (0.0f < limit) {
                limit = 0.0f;
            }
            gravity_set_z(&hero.motion.velocity, &hero.motion.velocity, limit);
        }
    } else if (hero.state.control_mode == 0xF) {
        if (hero.unk1CC == 0) {
            velocity_measurement = vector_length_xy(&hero.motion.velocity);
            limit = hero.unk234 - 0.02f;
            if (limit < velocity_measurement) {
                normalize_vector_xy(&hero.motion.velocity, &hero.motion.velocity, limit);
            }
        }
    } else if (hero.state.control_mode != 0xD) {
        if (hero.state.control_mode != 0xE && hero.unk1CC == 0) {
            velocity_measurement = vector_length_xyz(&hero.motion.velocity);
            limit = hero.unk234 - 0.02f;
            if (limit < velocity_measurement) {
                normalize_vector_xyz(&hero.motion.velocity, &hero.motion.velocity, limit);
            }
        }
    }

    add_vector_xyz(&hero.motion.pos, &hero.motion.pos, &hero.motion.velocity);
    add_vector_xyz(&hero.motion.pos, &hero.motion.pos, &hero.move_impulse);
    clear_vector(&hero.move_impulse);
    hero.wall_hit = 0;
    hero.coll_hit_moby = 0;

    if (vector_length_xyz(&hero.motion.platform_delta) <= 0.0001f) {
        resolve_hero_collisions_with_history();
        update_hero_ground_contact_and_surface_slopes();
        subtract_vector_xyz(&hero.motion.displacement, &hero.motion.pos, &old_pos);
        correct_hero_ground_penetration();
    } else {
        update_hero_ground_contact_and_surface_slopes();
    }

    /* movement direction, scaled by the velocity along it (never negative) */
    subtract_vector_xyz(&hero.motion.unk110, &hero.motion.pos, &old_pos);
    qcopy(&hero.motion.unk130, &hero.motion.unk110);
    qcopy(&hero.motion.unk120, &hero.motion.unk110);
    normalize_vector_xyz(&hero.motion.unk110, &hero.motion.unk110, 1.0f);
    velocity_measurement = dot_vectors_xyz(&hero.motion.unk110, &hero.motion.velocity);
    if (velocity_measurement < 0.0f) {
        velocity_measurement = 0.0f;
    }
    normalize_vector_xyz(&hero.motion.unk110, &hero.motion.velocity, velocity_measurement);

    qcopy(&movement_scratch_vector, &hero.motion.velocity);
    gravity_flatten(&movement_scratch_vector, &movement_scratch_vector);
    gravity_flatten(&hero.motion.unk130, &hero.motion.unk130);
    normalize_vector_xyz(&hero.motion.unk130, &hero.motion.unk130, 1.0f);
    velocity_measurement = dot_vectors_xyz(&hero.motion.unk130, &movement_scratch_vector);
    if (velocity_measurement < 0.0f) {
        velocity_measurement = 0.0f;
    }
    normalize_vector_xyz(&hero.motion.unk130, &movement_scratch_vector, velocity_measurement);

    qcopy(&gravity_axis_velocity, &hero.motion.velocity);
    set_gravity_relative_vector_length_xy((float *)&gravity_axis_velocity, (float *)&gravity_axis_velocity, 0.0f);
    set_gravity_relative_vector_length_xy((float *)&hero.motion.unk120, (float *)&hero.motion.unk120, 0.0f);
    normalize_vector_xyz(&hero.motion.unk120, &hero.motion.unk120, 1.0f);
    velocity_measurement = dot_vectors_xyz(&hero.motion.unk120, &gravity_axis_velocity);
    if (velocity_measurement < 0.0f) {
        velocity_measurement = 0.0f;
    }
    normalize_vector_xyz(&hero.motion.unk120, &gravity_axis_velocity, velocity_measurement);

    hero.motion.speed_xy = vector_length_xyz(&hero.motion.unk110);
    hero.motion.unk164 = vector_length_xy(&hero.motion.unk110);
    /* read through the address, as retail reloads the base register here */
    movement_scratch_vector.q = (&hero.motion.unk110)->q;
    hero.motion.unk168 = forward_component(&movement_scratch_vector);
    if (hero.motion.unk168 < 0.0f) {
        hero.motion.unk168 = 0.0f;
    }

    /* apply the push, keeping its w */
    qcopy(&movement_scratch_vector, &hero.motion.pos);
    if (vector_length_xyz(&hero.motion.platform_delta) > 0.0001f) {
        add_vector_xyz(&hero.motion.pos, &hero.motion.pos, &hero.motion.platform_delta);
        saved_platform_delta_w = hero.motion.platform_delta.f[3];
        clear_vector(&hero.motion.platform_delta);
        hero.motion.platform_delta.f[3] = saved_platform_delta_w;
        resolve_hero_collisions_with_history();
        update_hero_ground_contact_and_surface_slopes();
        subtract_vector_xyz(&hero.motion.displacement, &hero.motion.pos, &old_pos);
        correct_hero_ground_penetration();
    }
    subtract_vector_xyz(&hero.motion.platform_carry, &hero.motion.pos, &movement_scratch_vector);
    {
        /* push w moves to unk140.w; a block-local keeps retail's order */
        f32 push_w = hero.motion.platform_delta.f[3];

        hero.motion.platform_delta.f[3] = 0.0f;
        hero.motion.platform_carry.f[3] = push_w;
    }

    subtract_vector_xyz(&hero.motion.displacement, &hero.motion.pos, &old_pos);
    hero.motion.unk16C = 0.0f;
    if (hero.motion.unk164 > 0.004f) {
        hero.motion.unk16C = hero.motion.displacement.f[2] / hero.motion.unk164;
        if (hero.motion.unk16C > 0.5f) {
            hero.motion.unk16C = 0.5f;
        } else if (hero.motion.unk16C < -0.5f) {
            hero.motion.unk16C = -0.5f;
        }
    }

    /* cap the step at 52 units per frame-rate unit */
    if (hero.motion.speed_xy > frame_time * 52.0f) {
        scale_vector_xyz(&hero.motion.displacement, &hero.motion.displacement, frame_time * 52.0f / hero.motion.speed_xy);
        hero.motion.speed_xy = frame_time * 52.0f;
    }
}
/* Ported from rac1-decomp (src/overlays/shared/help_0020CDF0.c: func_L00_002144A0), where it is exact; names translated to the US level program. */

/* Scales four vectors and one float of the hero by s. */
void scale_hero_motion(float scale_factor) __asm__("FUN_L00_00213de8");

void scale_hero_motion(float scale_factor) {
    char *movement_vector_base = D_0013E533_lui + 0xF1D;
    float *hero_float_base;

    fast_vec_scale(movement_vector_base, movement_vector_base, scale_factor);
    fast_vec_scale(movement_vector_base + 0x10, movement_vector_base + 0x10, scale_factor);
    fast_vec_scale(movement_vector_base - 0x20, movement_vector_base - 0x20, scale_factor);
    fast_vec_scale(movement_vector_base + 0x50, movement_vector_base + 0x50, scale_factor);
    hero_float_base = (float *)(movement_vector_base - 0x100);
    hero_float_base[0x194 / 4] = hero_float_base[0x194 / 4] * scale_factor;
}
/* Ported from rac1-decomp (src/overlays/shared/help_0020CDF0.c: func_L00_00214520). */
#define HERO_OF(v) ((struct Hero *)((char *)(v) - 0xE0))
extern Vec4 hero_velocity __asm__("D_0013F430");
extern f32 approach_value(f32 *p, f32 target, f32 maxstep) __asm__("FUN_00213ed8");
extern void add_hero_forward_offset(void *, void *, f32) __asm__("FUN_L00_00233708");

static inline struct Hero *hero_ptr(void) { return &hero; }

/* Steers the velocity's xy toward the move yaw at the target speed. */
void steer_hero_planar_velocity(f32 step) __asm__("FUN_L00_00213e68");

void steer_hero_planar_velocity(f32 step) {
    struct Hero *player = &hero;
    f32 delta[4];
    Vec4 impulse;
    f32 planar_speed;
    f32 yaw, speed, ratio, delta_length, velocity_length;
    Vec4 *velocity;

    switch (player->unk20B3) {
    case HERO_GRAVITY_WORLD_DOWN:
        yaw = player->motion.move_yaw;
        speed = player->target_speed;
        if (player->state.control_mode == 4) {
            if (hero_ptr()->unk30A == 0 && hero_ptr()->unk4A4 != 0 && hero_ptr()->wall_hit != 0)
                speed = frame_time * 2.1f;
        }
        if (hero_ptr()->unk30A != 0) {
            struct Hero *cap_owner = &hero;

            if (cap_owner->state.current == 0xE && frame_time < speed)
                speed = frame_time;
        }
        {
            struct Hero *ratio_owner = &hero;

            delta[0] = fast_cos(yaw) * speed;
            delta[1] = fast_sin(yaw) * speed;
            delta[2] = ratio_owner->motion.velocity.f[2];
            ratio = -(ratio_owner->motion.velocity.f[0] * delta[0] + ratio_owner->motion.velocity.f[1] * delta[1]);
            delta_length = vector_length_xy(delta);
            velocity_length = vector_length_xy(&ratio_owner->motion.velocity);
            if (delta_length == 0.0f) {
                ratio = 0.0f;
            } else if (velocity_length == 0.0f) {
                ratio = 0.0f;
            } else {
                ratio /= delta_length;
                ratio /= velocity_length;
            }
        }
        {
            struct Hero *clamp_owner = &hero;

            ratio = ratio + 1.0f;
            ratio *= 1.5f;
            if (clamp_owner->state.current == 0x81) {
                if (1.7f < ratio)
                    ratio = 1.7f;
                if (ratio < 1.0f)
                    ratio = 1.0f;
            } else {
                if (3.0f < ratio)
                    ratio = 3.0f;
                if (ratio < 1.0f)
                    ratio = 1.0f;
            }
        }
        velocity = &hero_velocity;
        step *= ratio;
        subtract_vector_xyz(delta, delta, velocity);
        if (step < vector_length_xy(delta))
            set_vector_length_xy(delta, delta, step);
        add_vector_xyz(velocity, velocity, delta);
        qcopy(&HERO_OF(velocity)->motion.unk130, velocity);
        impulse.q = HERO_OF(velocity)->move_impulse.q;
        ratio = forward_component(&impulse);
        {
            struct Hero *owner = HERO_OF(velocity);

            owner->ground_speed = owner->motion.unk168 - ratio;
        }
        break;
    case HERO_GRAVITY_OPPOSITE_GROUND_NORMAL:
    case HERO_GRAVITY_TOWARD_REFERENCE_POINT: {
        Vec4 *current_velocity = &hero_velocity;

        planar_speed = gravity_length_xy(current_velocity);
        approach_value(&planar_speed, HERO_OF(current_velocity)->target_speed, step);
        set_gravity_relative_vector_length_xy(current_velocity->f, current_velocity->f, 0.0f);
        add_hero_forward_offset(current_velocity, current_velocity, planar_speed);
        break;
    }
    }
}

/* Ported from rac1-decomp (src/overlays/shared/help_002274A8.c: HeroAirAccel). */
void air_accel(f32 step) __asm__("FUN_L01_00234358");

/* Same steering as FUN_L00_00213e68, without the copy into motion.unk130. */
void air_accel(f32 step) {
    struct Hero *player = &hero;
    f32 delta[4];
    Vec4 impulse;
    f32 planar_speed;
    f32 yaw, speed, ratio, delta_length, velocity_length;
    Vec4 *velocity;

    switch (player->unk20B3) {
    case HERO_GRAVITY_WORLD_DOWN:
        yaw = player->motion.move_yaw;
        speed = player->target_speed;
        if (player->state.control_mode == 4) {
            if (hero_ptr()->unk30A == 0 && hero_ptr()->unk4A4 != 0 && hero_ptr()->wall_hit != 0)
                speed = frame_time * 2.1f;
        }
        if (hero_ptr()->unk30A != 0) {
            struct Hero *cap_owner = &hero;

            if (cap_owner->state.current == 0xE && frame_time < speed)
                speed = frame_time;
        }
        {
            struct Hero *ratio_owner = &hero;

            delta[0] = fast_cos(yaw) * speed;
            delta[1] = fast_sin(yaw) * speed;
            delta[2] = ratio_owner->motion.velocity.f[2];
            ratio = -(ratio_owner->motion.velocity.f[0] * delta[0] + ratio_owner->motion.velocity.f[1] * delta[1]);
            delta_length = vector_length_xy(delta);
            velocity_length = vector_length_xy(&ratio_owner->motion.velocity);
            if (delta_length == 0.0f) {
                ratio = 0.0f;
            } else if (velocity_length == 0.0f) {
                ratio = 0.0f;
            } else {
                ratio /= delta_length;
                ratio /= velocity_length;
            }
        }
        {
            struct Hero *clamp_owner = &hero;

            ratio = ratio + 1.0f;
            ratio *= 1.5f;
            if (clamp_owner->state.current == 0x81) {
                if (1.7f < ratio)
                    ratio = 1.7f;
                if (ratio < 1.0f)
                    ratio = 1.0f;
            } else {
                if (3.0f < ratio)
                    ratio = 3.0f;
                if (ratio < 1.0f)
                    ratio = 1.0f;
            }
        }
        velocity = &hero_velocity;
        step *= ratio;
        subtract_vector_xyz(delta, delta, velocity);
        if (step < vector_length_xy(delta))
            set_vector_length_xy(delta, delta, step);
        add_vector_xyz(velocity, velocity, delta);
        impulse.q = HERO_OF(velocity)->move_impulse.q;
        ratio = forward_component(&impulse);
        {
            struct Hero *owner = HERO_OF(velocity);

            owner->ground_speed = owner->motion.unk168 - ratio;
        }
        break;
    case HERO_GRAVITY_OPPOSITE_GROUND_NORMAL:
    case HERO_GRAVITY_TOWARD_REFERENCE_POINT: {
        Vec4 *current_velocity = &hero_velocity;

        planar_speed = gravity_length_xy(current_velocity);
        approach_value(&planar_speed, HERO_OF(current_velocity)->target_speed, step);
        set_gravity_relative_vector_length_xy(current_velocity->f, current_velocity->f, 0.0f);
        add_hero_forward_offset(current_velocity, current_velocity, planar_speed);
        break;
    }
    }
}
#include "types.h"
#include "rnc/gameplay/hero.h"
extern int nearest_rail_point() __asm__("FUN_L00_0020cd08");

extern Vec4 hero_velocity __asm__("D_0013F430");
extern char hero_pos[] __asm__("D_0013F3D0");
extern f32 D_0015ED6C;
extern f32 frame_time_sq __asm__("D_0015ED70");
extern u8 D_0013D4C2 __attribute__((section(".data")));
extern s32 D_0013CAE0 __attribute__((section(".data")));

extern s32 hero_slot_item_id(s32) __asm__("FUN_L00_0020d498");
extern void subtract_vector_xyz(void *out, void *a, void *b) __asm__("FUN_001f9a28");
extern f32 gravity_z(void *) __asm__("FUN_L00_00233a78");
extern void gravity_set_z(void *, void *, f32) __asm__("FUN_L00_00233b20");
extern void FUN_L00_00233810(void *, void *, f32);
extern void FUN_L00_002334d0(void *, void *, f32);
extern f32 approach_value(f32 *p, f32 target, f32 maxstep) __asm__("FUN_00213ed8");
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
extern f32 sqrtf_(f32) __asm__("FUN_001f9988");

struct Curve {
    s32 start;
    s32 end;
    u8 pad8[0xC];
    struct HeroVelocityKey *keys;
    s32 key;
    s32 frames;
    f32 value;
};

void hero_integrate_velocity(void) __asm__("FUN_L00_00214108");
void hero_integrate_velocity(void) {
    struct Curve *velocity_curve;
    f32 z;
    Vec4 offset;
    f32 speed;
    f32 max_speed;
    s32 boost;
    s32 no_lift;
    f32 gravity_axis_offset;
    f32 floor;
    f32 movement_gravity_axis_velocity;

    if (hero.state.current == 0xE) {
        max_speed = D_0015ED6C * 7.0f;
        boost = 0;
        if (D_0013D4C2 != 0 && hero.unk22D8 == 0) {
            boost = hero_slot_item_id(3) == 3;
        }
        subtract_vector_xyz(&offset, hero_pos, hero_pos + 0x380);
        gravity_axis_offset = gravity_z(&offset);
        if (boost) {
            if (2.7f < gravity_axis_offset) {
                max_speed *= 0.47f;
            } else if (2.1f < gravity_axis_offset) {
                max_speed *= 0.7f;
            }
        } else {
            if (2.1f < gravity_axis_offset) {
                max_speed *= 0.45f;
            } else if (1.7f < gravity_axis_offset) {
                max_speed *= 0.7f;
            }
        }
        if (hero_slot_item_id(3) == 3 && hero.unk22D8 == 0) {
            max_speed *= 1.25f;
        }
        if (hero.state_timer < scale_game_frames(8)) {
            speed = gravity_z(&hero.motion.velocity);
            if (speed < max_speed) {
                approach_value(&speed, max_speed, frame_time_sq * 150.0f);
            }
            gravity_set_z(&hero.motion.velocity, &hero.motion.velocity, speed);
        }
    } else {
        no_lift = 0;
        if (hero.unk428 == 0.0f && hero.unk42C == 0.0f) {
            no_lift = 1;
        }
        if ((D_0013CAE0 & 0x40) || no_lift || hero.state.current == 0x12) {
            if (hero.unk430 < hero.unk48C && !(scale_game_frames(15) < hero.state_timer)) {
                hero.unk430 += (hero.unk48C - hero.unk488) / hero.unk498;
                if (hero.unk48C < hero.unk430) {
                    hero.unk430 = hero.unk48C;
                }
                hero.unk428 += sqrtf_((hero.unk430 + hero.unk430) * hero.unk4A0) - hero.unk42C - hero.unk428;
            }
        }
        if (!(hero.state_timer < hero.unk420) && 0.0f < hero.unk428) {
            FUN_L00_00233810(&hero.motion.velocity, &hero.motion.velocity, hero.unk428);
            hero.unk42C += hero.unk428;
            hero.unk428 = 0.0f;
        }
        if (hero.unk41C != 0) {
            if (hero.state_timer < hero.unk3D0) {
                goto tail;
            }
            if (hero.state_timer < hero.unk3D4) {
                velocity_curve = (struct Curve *)&hero.unk3D0;
                velocity_curve->frames++;
                while (velocity_curve->key == -1 || !(velocity_curve->frames < scale_game_frames(velocity_curve->keys[velocity_curve->key].frames))) {
                    velocity_curve->key++;
                    velocity_curve->frames = 0;
                    if (velocity_curve->keys[velocity_curve->key].value != -999999.0f) {
                        velocity_curve->value = velocity_curve->keys[velocity_curve->key].value * frame_time_sq;
                    }
                }
                if (velocity_curve->frames > 0) {
                    velocity_curve->value += velocity_curve->keys[velocity_curve->key].delta * frame_time_sq;
                }
                FUN_L00_00233810(&hero_velocity, &hero_velocity, velocity_curve->value);
            }
        }
    }
tail:
    if (hero.state.current == 0x1C || hero.state.current == 0x4C) {
        if (hero.state_timer < scale_game_frames(10)) {
            return;
        }
    }
    if (hero.state_timer < hero.unk420) {
        gravity_set_z(&hero.motion.velocity, &hero.motion.velocity, 0.0f);
        FUN_L00_002334d0(&hero.motion.velocity, &hero.motion.velocity, frame_time_sq * 48.0f);
    } else {
        FUN_L00_002334d0(&hero.motion.velocity, &hero.motion.velocity, hero.unk4A0);
        z = gravity_z(&hero.motion.velocity);
        movement_gravity_axis_velocity = gravity_z(&hero.motion.unk110);
        floor = movement_gravity_axis_velocity - 0.1f;
        if (z < floor) {
            z = floor;
        }
        floor = -(D_0015ED6C * 50.0f);
        if (z < floor) {
            z = floor;
        }
        /* Fakematch: unk20AD is a u8, so this test never fires. It only
           keeps g alive past the subtraction, which gives retail's
           mov.s $f2,$f0; every clean spelling (MIN/MAX, helpers, ternary,
           a constant in a variable) folds g into the sub.s instead. */
        boost = hero.unk20AD;
        if (boost & 0x100) {
            z = movement_gravity_axis_velocity;
        }
        gravity_set_z(&hero.motion.velocity, &hero.motion.velocity, z);
    }
}

f32 D_L00_0017A964[1] __attribute__((section(".data"))) = {0};
f32 D_L00_0017AA14[1] __attribute__((section(".data"))) = {0};
f32 D_L00_0017ACA4[1] __attribute__((section(".data"))) = {0};
f32 D_L01_0017ADE4[1] __attribute__((section(".data"))) = {0};
f32 D_L01_0017AE94[1] __attribute__((section(".data"))) = {0};
f32 D_L02_0017AF64[1] __attribute__((section(".data"))) = {0};
f32 D_L02_0017B014[1] __attribute__((section(".data"))) = {0};
f32 D_L02_0017B2A4[1] __attribute__((section(".data"))) = {0};
f32 D_L03_0017AA64[1] __attribute__((section(".data"))) = {0};
f32 D_L03_0017AB14[1] __attribute__((section(".data"))) = {0};
f32 D_L03_0017ADA4[1] __attribute__((section(".data"))) = {0};
f32 D_L04_0017AAE4[1] __attribute__((section(".data"))) = {0};
f32 D_L04_0017AB94[1] __attribute__((section(".data"))) = {0};
f32 D_L04_0017AE24[1] __attribute__((section(".data"))) = {0};
f32 D_L05_0017ADC4[1] __attribute__((section(".data"))) = {0};
f32 D_L05_0017AE74[1] __attribute__((section(".data"))) = {0};
f32 D_L06_0017B0E4[1] __attribute__((section(".data"))) = {0};
f32 D_L06_0017B194[1] __attribute__((section(".data"))) = {0};
f32 D_L07_0017A9E4[1] __attribute__((section(".data"))) = {0};
f32 D_L07_0017AA94[1] __attribute__((section(".data"))) = {0};
f32 D_L07_0017AD24[1] __attribute__((section(".data"))) = {0};
f32 D_L08_0017B164[1] __attribute__((section(".data"))) = {0};
f32 D_L08_0017B214[1] __attribute__((section(".data"))) = {0};
f32 D_L09_0017AAE4[1] __attribute__((section(".data"))) = {0};
f32 D_L09_0017AB94[1] __attribute__((section(".data"))) = {0};
f32 D_L09_0017AE24[1] __attribute__((section(".data"))) = {0};
f32 D_L10_0017ADE4[1] __attribute__((section(".data"))) = {0};
f32 D_L10_0017AE94[1] __attribute__((section(".data"))) = {0};
f32 D_L11_0017B6A4[1] __attribute__((section(".data"))) = {0};
f32 D_L12_0017ADE4[1] __attribute__((section(".data"))) = {0};
f32 D_L12_0017AE94[1] __attribute__((section(".data"))) = {0};
f32 D_L13_0017AC64[1] __attribute__((section(".data"))) = {0};
f32 D_L13_0017AD14[1] __attribute__((section(".data"))) = {0};
f32 D_L13_0017AFA4[1] __attribute__((section(".data"))) = {0};
f32 D_L14_0017B064[1] __attribute__((section(".data"))) = {0};
f32 D_L14_0017B114[1] __attribute__((section(".data"))) = {0};
f32 D_L14_0017B3A4[1] __attribute__((section(".data"))) = {0};
f32 D_L15_0017AF64[1] __attribute__((section(".data"))) = {0};
f32 D_L15_0017B014[1] __attribute__((section(".data"))) = {0};
f32 D_L15_0017B2A4[1] __attribute__((section(".data"))) = {0};
f32 D_L16_0017ADC4[1] __attribute__((section(".data"))) = {0};
f32 D_L16_0017AE74[1] __attribute__((section(".data"))) = {0};
f32 D_L17_0017B264[1] __attribute__((section(".data"))) = {0};
f32 D_L17_0017B314[1] __attribute__((section(".data"))) = {0};
f32 D_L18_0017B6A4[1] __attribute__((section(".data"))) = {0};
