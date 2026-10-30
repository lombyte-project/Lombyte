/* Level 01 moby classes: rock_formation, fan, bridge_piece_1, destroyable_cave_wall, agnogg_ship_front, bridge_half, green_mushroom, door_left, pipe, beehive, mushroom_underside. */
#include "types.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "eetypes.h"
#include "qcopy.h"
#include "qzero.h"
#include "rnc/mobys/moby.h"
#include "rnc/data_labels.h"
#include "rnc/game/options.h"
#include "rnc/math/fast_math.h"
#include "rnc/game/item_state.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/save/memcard_save_data.h"

typedef union {
    u128 q;
    f32  f[4];
} DebrisVec;

typedef struct {
    u32 c[3];
} DebrisColors;

typedef struct {
    u8  pad0[0x2C];
    f32 force;
} DebrisHit;

extern DebrisColors l01_rock_formation_debris_oclass_choices __asm__("D_L01_00161C50");
extern s32          FUN_0022da68(s32, s32, void *);
extern int          FUN_L00_00257b90(int lo, int hi);
extern void spawn_effect_particle_with_target_scalar(void *, void *, int, int, float, int) __asm__("FUN_L00_0026ced0");
extern struct Moby *
spawn_debris_chunk(void *, void *, s32, f32, s32, f32, f32, f32, s32) __asm__("FUN_L01_002f8530");
extern void FUN_L00_00257470(void *, s32, s32);

/* Once hit hard enough: bursts into 200 smoke puffs and 40 debris chunks, then removes itself. */
void rock_formation_update(struct Moby *m) __asm__("FUN_L01_002f9810");

void rock_formation_update(struct Moby *rock_formation_moby) {
    DebrisHit *hit;
    s32        i;
    s32        j;
    f32        offset_component_or_particle_size;

    hit = find_moby_hit(rock_formation_moby, 0x10000, 0);
    rock_formation_moby->hit_slot = 0xFF;
    if (hit != NULL && hit->force > 0.0f) {
        FUN_0022da68(0, 0x10, rock_formation_moby);
        for (i = 0; i < 200; i++) {
            /* A block of its own: retail reuses these stack slots in the next loop. */
            {
                DebrisVec vel;
                DebrisVec pos;
                DebrisVec off;

                pos.q = 0;
                pos.f[0] = random_float_between(-1.0f, 1.0f);
                pos.f[1] = random_float_between(-1.0f, 1.0f);
                pos.f[2] = random_float_between(-1.0f, 1.0f);
                off.q = 0;
                vel.q = pos.q;
                offset_component_or_particle_size = random_float_between(-1.0f, 1.0f);
                offset_component_or_particle_size *= fast_cos(rock_formation_moby->rot.z);
                off.f[0] = offset_component_or_particle_size;
                offset_component_or_particle_size = random_float_between(-0.3f, 0.3f);
                offset_component_or_particle_size *= fast_sin(rock_formation_moby->rot.z);
                off.f[1] = offset_component_or_particle_size;
                off.f[2] = random_float_between(0.0f, 2.0f);
                pos.q = off.q;
                fast_vec_add(&pos, &pos, &rock_formation_moby->pos);
                fast_vec_normalize(&vel, &vel, random_float_between(0.5f, 1.5f) * frame_time);
                offset_component_or_particle_size = random_float_between(0.5f, 1.0f) * 210000.0f;
                spawn_effect_particle_with_target_scalar(
                    &pos,
                    &vel,
                    0x5F787878,
                    0x181818,
                    offset_component_or_particle_size,
                    scale_game_frames(FUN_L00_00257b90(30, 90)));
            }
        }
        for (j = 0; j < 40; j++) {
            DebrisColors colors;
            DebrisVec    vel;
            DebrisVec    pos;
            DebrisVec    off;

            colors = l01_rock_formation_debris_oclass_choices;
            pos.q = 0;
            pos.f[0] = random_float_between(-1.0f, 1.0f);
            pos.f[1] = random_float_between(-1.0f, 1.0f);
            pos.f[2] = random_float_between(-1.0f, 1.0f);
            off.q = 0;
            vel.q = pos.q;
            offset_component_or_particle_size = random_float_between(-1.2f, 1.2f);
            offset_component_or_particle_size *= fast_cos(rock_formation_moby->rot.z);
            off.f[0] = offset_component_or_particle_size;
            offset_component_or_particle_size = random_float_between(-0.4f, 0.4f);
            offset_component_or_particle_size *= fast_sin(rock_formation_moby->rot.z);
            off.f[1] = offset_component_or_particle_size;
            off.f[2] = random_float_between(0.5f, 2.0f);
            pos.q = off.q;
            fast_vec_add(&pos, &pos, &rock_formation_moby->pos);
            fast_vec_normalize(&vel, &vel, random_float_between(1.0f, 5.0f) * frame_time);
            spawn_debris_chunk(&pos,
                               &vel,
                               colors.c[random_integer_below(3)],
                               random_float_between(0.04f, 0.09f),
                               FUN_L00_00257b90(60, 180),
                               1.0f,
                               1.0f,
                               0.75f,
                               0);
        }
        FUN_L00_00257470(rock_formation_moby, 0, -1);
        mark_moby_for_removal(rock_formation_moby);
    }
}

typedef struct {
    f32 sway;
    f32 dir;
    s32 timer;
} SwayVars;

typedef struct {
    u8        pad0[0x10];
    u8        pos[0x10];
    u8        state;
    u8        pad21[0x27];
    f32       rotz;
    u8        pad4c[0x2C];
    SwayVars *pvars;
} SwayMoby;

typedef struct {
    s64   t;
    u8    pad8[8];
    void *moby;
    s32   flags;
    s32   x18;
    s32   x1c;
    s32   x20;
} CollQuery;

extern void copy_hero_moby_field_38() __asm__("FUN_L00_0025d1b8");
extern s32  rand(void) __asm__("FUN_001160d8");
extern void collect_mobies_in_sphere(void *, s32, void *, void *, f32) __asm__("FUN_L00_001f2868");

void fan_update(SwayMoby *m) __asm__("FUN_L01_002f9bf0");

void fan_update(SwayMoby *fan_moby) {
    SwayVars *sway_vars;
    CollQuery collision_query;

    sway_vars = fan_moby->pvars;
    if (fan_moby->state == 0) {
        copy_hero_moby_field_38(fan_moby);
        fan_moby->state = 1;
        sway_vars->sway = random_float_between(-1.0f, 1.0f);
        sway_vars->dir = (rand() & 1) ? 1.0f : -1.0f;
    }
    collision_query.moby = fan_moby;
    collision_query.flags = 0x10001;
    collision_query.x1c = 0;
    clear_u64_value(&collision_query);
    collision_query.x20 = 0;
    collect_mobies_in_sphere(fan_moby->pos, 0x10, fan_moby, &collision_query, 1.2f);
    if (sway_vars->dir == sway_vars->sway) {
        if (fast_dec_timer(&sway_vars->timer)) {
            sway_vars->dir = -sway_vars->dir;
            sway_vars->timer = scale_game_frames(0x5A);
        }
    }
    sway_vars->sway += frame_time * 4.0f * sway_vars->dir;
    if (sway_vars->sway > 1.0f) {
        sway_vars->sway = 1.0f;
    }
    if (sway_vars->sway < -1.0f) {
        sway_vars->sway = -1.0f;
    }
    fan_moby->rotz = fast_add_rotations(fan_moby->rotz, frame_time * -12.566371f * sway_vars->sway);
}

typedef union {
    u128 q;
    f32  f[4];
} BurstVec;

typedef struct {
    u32 c[3];
} BurstColors;

typedef struct {
    u8  pad0[0xA6];
    s16 oclass;
} BurstClassMoby;

typedef struct {
    u8              pad0[0x20];
    BurstClassMoby *moby;
} BurstTarget;

typedef struct {
    u8       pad0[0x10];
    BurstVec pos;
    u8       pad20[0x84];
    u8       bA4;
    u8       padA5[0xD];
    s16      id;
} BurstMoby;

extern s32 pal_mode_macro[] __asm__("D_0015ED80") MACRO_ADDR;
extern s32 moby_killed[] __asm__("D_L01_001BA950");

#include "rnc/overlay/level_save_state.h"

extern BurstColors l01_bridge_piece_debris_oclass_choices __asm__("D_L01_00161C60");
extern void        FUN_L01_002f8530(void *, void *, u32, s32, f32, f32, f32, f32, s32);

void bridge_piece_1_update(BurstMoby *self) __asm__("FUN_L01_002f9d80");

void bridge_piece_1_update(BurstMoby *self) {
    BurstTarget *hit_result;
    BurstColors  colors;
    BurstVec     debris_velocity;
    BurstVec     pos;
    f32          spawn_radius;
    f32          spawn_angle_radians;
    s32          i;

    hit_result = find_moby_hit(self, 0x830000, 0);
    if (l01_save_state.cleared[self->id] != 0 ||
        (D_0014C190_257218[current_level_index][self->id >> 5] >> (self->id & 0x1F)) & 1) {
        mark_moby_for_removal(self);
    }
    if (hit_result != NULL && hit_result->moby->oclass == 0x2AE) {
        for (i = 0; i < 20; i++) {
            colors = l01_bridge_piece_debris_oclass_choices;
            pos.q = 0;
            pos.f[0] = random_float_between(-frame_time, frame_time);
            pos.f[1] = random_float_between(-frame_time, frame_time);
            pos.f[2] = random_float_between(-frame_time, frame_time);
            debris_velocity.q = pos.q;
            spawn_radius = random_float_between(2.0f, 3.0f);
            spawn_angle_radians = random_angle_radians();
            pos.f[0] = fast_cos(spawn_angle_radians) * spawn_radius;
            pos.f[1] = fast_sin(spawn_angle_radians) * spawn_radius;
            pos.f[2] = 0.0f;
            fast_vec_normalize(&debris_velocity,
                               &debris_velocity,
                               random_float_between(frame_time * 5.0f, frame_time * 20.0f));
            fast_vec_add(&pos, &pos, &self->pos);
            FUN_L01_002f8530(&pos,
                             &debris_velocity,
                             colors.c[random_integer_below(3)],
                             FUN_L00_00257b90(0xB4, 0x12C),
                             0.05f,
                             1.0f,
                             1.0f,
                             0.75f,
                             0);
        }
        D_0014C190_257218[pal_mode_macro[1]][self->id >> 5] |= 1 << (self->id & 0x1F);
        moby_killed[self->id >> 5] |= 1 << (self->id & 0x1F);
        mark_moby_for_removal(self);
    } else {
        self->bA4 = 0xFF;
    }
}

typedef struct {
    u128 target;
    s32  owner;
    s32  color;
} BeamVars;

typedef struct {
    u8        pad0[0x10];
    u128      pos;
    u8        pad20[0xC];
    f32       scale;
    u8        b30;
    u8        b31;
    s16       h32;
    u8        pad34[0xC];
    f32       rx;
    f32       ry;
    f32       rz;
    u8        pad4c[0x2C];
    BeamVars *pvars;
} BeamMoby;

void set_moby_color(void *, s32, s32, s32) __asm__("FUN_L00_002502f0");
void spawn_source_attached_fading_effect_particle(void *, u32, s32, s32, f32) __asm__("FUN_L00_0026daa0");
extern void update_entity() __asm__("FUN_L00_00250df8");

BeamMoby *spawn_colored_half_scale_moby(s32   owner,
                                        u128 *pos,
                                        u128 *target,
                                        s32   color) __asm__("FUN_L01_002fa068");

BeamMoby *spawn_colored_half_scale_moby(s32 owner, u128 *pos, u128 *target, s32 color) {
    u128      position_copy;
    u128      target_copy;
    BeamMoby *spawned_moby;
    BeamVars *beam_vars;
    u128     *pp;
    u128     *tp;

    pp = &position_copy;
    tp = &target_copy;
    position_copy = *pos;
    target_copy = *target;
    spawned_moby = create_moby(0x2D2);
    if (spawned_moby != 0) {
        beam_vars = spawned_moby->pvars;
        beam_vars->owner = owner;
        spawned_moby->b30 = 0x7F;
        spawned_moby->h32 = 0x7F;
        spawned_moby->b31 = 1;
        spawned_moby->scale *= 0.5f;
        set_moby_color(spawned_moby, 0x7F, 0x7F, 0x7F);
        spawned_moby->rx = random_angle_radians();
        spawned_moby->ry = random_angle_radians();
        spawned_moby->rz = random_angle_radians();
        qcopy(&spawned_moby->pos, pp);
        qcopy(&beam_vars->target, tp);
        beam_vars->color = color;
        spawn_source_attached_fading_effect_particle(
            spawned_moby, 0x2F00007F, color, -1, 210000.0f);
        spawn_source_attached_fading_effect_particle(
            spawned_moby, 0x4F004F7F, beam_vars->color, -1, 115500.0f);
        update_entity(spawned_moby);
    }
    return spawned_moby;
}

/*
 * Explosion burst at `where` (uses the unit's BurstVec): three particles in
 * random colours from two six-colour palettes, two spawn_glow_particle effects
 * on `moby`, its sound unless the moby is dying (state 0xFD/0xFE), and a
 * light flash of brightness `light` (negative: 13).
 */
typedef struct {
    u32 c[6];
} BurstPalette;

/* Light used by the flash; mine_camera_blast places it at a position. */
typedef struct {
    u8  pad_00[0x20];
    f32 r;
    f32 g;
    f32 b;
} BurstLight;

extern BurstPalette D_L01_0020B4F0; /* inner colours */
extern BurstPalette D_L01_0020B508; /* outer colours */

BurstLight D_L01_001E3440 = {
    {0, 0, 0,    0, 0,    0,   0, 0,    0x66, 0x66, 0xa6, 0x3f, 0,    0, 0,    0,
     0, 0, 0xff, 0, 0x13, 0xf, 0, 0xff, 0,    0x13, 0x11, 0,    0x64, 0, 0x13, 0x13},
    13.0f,
    13.0f,
    13.0f
};

extern void
spawn_particle(void *, void *, u32, u32, f32, s32, s32, f32, s32, s32) __asm__("FUN_L00_0026a9f0");
/* s32 return is load-bearing: a void prototype changes the code */
extern s32 spawn_glow_particle(void *, void *, void *, f32, s32, s32, s32, s32, s32) __asm__("FUN_L00_002ac910");
/* allocate_voice_for_target_entry; the unit declares it under this name */
extern void mine_camera_blast(void *, void *, s32, s32) __asm__("FUN_L00_002d3838");

void spawn_explosion_effects(
    struct Moby *moby, u128 *where, s32 sound, f32 scale, f32 light) __asm__("FUN_L01_002fa1b0");

void spawn_explosion_effects(struct Moby *moby, u128 *where, s32 sound, f32 scale, f32 light) {
    BurstVec      burst_position;
    BurstVec      particle_base_velocity;
    BurstPalette  inner;
    BurstPalette  outer;
    BurstPalette *inner_p;
    BurstVec     *pos_p;
    f32           size;
    f32           speed;
    s32           i;

    /* pos and inner are used through pointers, as in retail */
    pos_p = &burst_position;
    burst_position.q = *where;
    clear_u64_value(&particle_base_velocity);
    inner_p = &inner;
    for (i = 2; i >= 0; i--) {
        speed = random_float_between(8.0f, 10.0f) * frame_time;
        size = scale * 400000.0f;
        inner = D_L01_0020B4F0;
        outer = D_L01_0020B508;
        spawn_particle(pos_p,
                       &particle_base_velocity,
                       inner_p->c[random_integer_below(6)],
                       outer.c[random_integer_below(6)],
                       size,
                       random_int_between(scale_game_frames(0xF), scale_game_frames(0x14)),
                       random_int_between(scale_game_frames(0x19), scale_game_frames(0x1E)),
                       speed * scale,
                       0,
                       0);
    }

    if (moby != 0) {
        spawn_glow_particle(moby,
                            pos_p,
                            &particle_base_velocity,
                            scale + scale,
                            scale_game_frames(0x14),
                            0x7F,
                            0,
                            0x40,
                            0x30);
        spawn_glow_particle(moby,
                            pos_p,
                            &particle_base_velocity,
                            scale * 1.5f,
                            scale_game_frames(0x1D),
                            0x20,
                            0,
                            0x20,
                            0);
        if (moby->state != 0xFE) {
            if (moby->state != 0xFD && sound != -1) {
                FUN_0022da68(sound, 0, moby);
            }
        }
    }

    if (light != 0.0f) {
        if (light > 0.0f) {
            D_L01_001E3440.g = light;
            D_L01_001E3440.b = light;
            D_L01_001E3440.r = light;
        } else {
            D_L01_001E3440.g = 13.0f;
            D_L01_001E3440.b = 13.0f;
            D_L01_001E3440.r = 13.0f;
        }
        mine_camera_blast(&D_L01_001E3440, pos_p, 0, 0);
    }
}

#include "rnc/math/vector.h"
#include "rnc/overlay/quad.h"

typedef struct {
    u128  vel;
    void *coll;
    u8    sound[4];
} DropVars;

typedef struct {
    u8        pad0[0x10];
    Vec4      pos;
    u8        pad20[0x58];
    DropVars *vars;
    u8        pad7C[0x2A];
    u16       hA6;
} DropMoby;

typedef struct {
    u128      dir;
    DropMoby *owner;
    s32       flags;
    u8        b28;
    u8        b29;
    u16       id;
    f32       scale;
    s32       count;
} DropProbe;

extern f32  scale_time(f32) __asm__("FUN_001f96b0");
extern s32  collision_line(void *, void *, s32, void *, void *) __asm__("FUN_001efa68");
extern s32  probe_world_sphere(f32, void *, s32, void *, void *) __asm__("FUN_L00_001f2868");
extern u8   D_L01_00167240[];
extern u128 D_L01_001742E0;

void update_birdbot_projectile(DropMoby *) __asm__("FUN_L01_002fa4c0");

void update_birdbot_projectile(DropMoby *projectile) {
    DropVars *projectile_vars;
    u128     *pos;
    Vec4      previous_position;
    DropProbe probe;
    u128      burst_position;
    f32       scaled_ten;
    f32       scaled_ten_product;
    f32       zero;
    f32       one;
    f32       ten;
    f32       half;

    projectile_vars = projectile->vars;
    pos = &projectile->pos.q;
    qcopy(&previous_position, pos);
    zero = 0.0f;
    one = 1.0f;
    ten = 10.0f;
    scaled_ten = scale_time(ten);
    scaled_ten_product = scale_time(ten);
    scaled_ten_product = scaled_ten_product * scale_time(ten);
    ((f32 *)&projectile_vars->vel)[2] -= (scaled_ten * zero + one) * 2.0f / scaled_ten_product;
    fast_vec_add(pos, pos, &projectile_vars->vel);
    if (projectile->pos.f[0] < zero || projectile->pos.f[1] < zero || projectile->pos.f[2] < zero ||
        vector_distance_xy(pos, D_L01_00167240) > 64.0f) {
        mark_moby_for_removal(projectile);
        return;
    }
    probe.owner = projectile;
    probe.flags = 0x10001;
    probe.scale = one;
    probe.count = 1;
    qcopy(&probe.dir, &projectile_vars->vel);
    ((f32 *)&probe.dir)[2] = one;
    ((f32 *)&probe.dir)[3] = 5627.9248f;
    probe.b28 = 1;
    probe.b29 = 1;
    probe.id = projectile->hA6;
    if (fast_dec_timer(projectile_vars->sound) != 0) {
        burst_position = projectile->pos.q;
        spawn_explosion_effects(projectile, &burst_position, 0, 0.5f, zero);
        mark_moby_for_removal(projectile);
    } else if (collision_line(&previous_position, pos, 0, projectile_vars->coll, &probe) != 0) {
        qcopy(pos, &D_L01_001742E0);
        burst_position = projectile->pos.q;
        spawn_explosion_effects(projectile, &burst_position, 0, 0.5f, zero);
        mark_moby_for_removal(projectile);
    } else {
        half = 0.5f;
        if (probe_world_sphere(half, pos, 0x10, projectile_vars->coll, &probe) != 0) {
            burst_position = projectile->pos.q;
            spawn_explosion_effects(projectile, &burst_position, 0, half, zero);
            mark_moby_for_removal(projectile);
        }
    }
}

extern DebrisColors l01_cave_wall_debris_oclass_choices __asm__("D_L01_00161C70");
extern u8           D_0013D388[];

/* Removed once collected; when hit, bursts into 500 smoke puffs and 100 chunks, marks itself collected and sets D_0013D388[0xE] (an item-availability flag). */
void destroyable_cave_wall_update(struct Moby *m) __asm__("FUN_L01_002fa800");

void destroyable_cave_wall_update(struct Moby *cave_wall_moby) {
    s32 i;
    s32 j;
    f32 offset_component_or_smoke_scale;

    if (l01_save_state.cleared[(s16)cave_wall_moby->save_id] != 0 ||
        (D_0014C190_257218[current_level_index][(s16)cave_wall_moby->save_id >> 5] >>
         (cave_wall_moby->save_id & 0x1F)) &
            1) {
        mark_moby_for_removal(cave_wall_moby);
        return;
    }
    if (find_moby_hit(cave_wall_moby, 0x800000, 0) != NULL) {
        FUN_0022da68(0, 0, cave_wall_moby);
        for (i = 0; i < 500; i++) {
            /* A block of its own: retail reuses these stack slots in the next loop. */
            {
                DebrisVec vel;
                DebrisVec pos;
                DebrisVec off;

                pos.q = 0;
                pos.f[0] = random_float_between(-1.0f, 1.0f);
                pos.f[1] = random_float_between(-1.0f, 1.0f);
                pos.f[2] = random_float_between(-1.0f, 1.0f);
                off.q = 0;
                vel.q = pos.q;
                offset_component_or_smoke_scale = random_float_between(-4.0f, 4.0f);
                offset_component_or_smoke_scale *= fast_cos(cave_wall_moby->rot.z);
                off.f[0] = offset_component_or_smoke_scale;
                offset_component_or_smoke_scale = random_float_between(-1.0f, 1.0f);
                offset_component_or_smoke_scale *= fast_sin(cave_wall_moby->rot.z);
                off.f[1] = offset_component_or_smoke_scale;
                off.f[2] = random_float_between(0.0f, 8.0f);
                pos.q = off.q;
                fast_vec_add(&pos, &pos, &cave_wall_moby->pos);
                fast_vec_normalize(&vel, &vel, random_float_between(1.5f, 3.5f) * frame_time);
                offset_component_or_smoke_scale = random_float_between(1.5f, 3.5f) * 210000.0f;
                spawn_effect_particle_with_target_scalar(
                    &pos,
                    &vel,
                    0x5F787878,
                    0x181818,
                    offset_component_or_smoke_scale,
                    scale_game_frames(FUN_L00_00257b90(120, 240)));
            }
        }
        for (j = 0; j < 100; j++) {
            DebrisColors colors;
            DebrisVec    vel;
            DebrisVec    pos;
            DebrisVec    off;

            colors = l01_cave_wall_debris_oclass_choices;
            pos.q = 0;
            pos.f[0] = random_float_between(-1.0f, 1.0f);
            pos.f[1] = random_float_between(-1.0f, 1.0f);
            pos.f[2] = random_float_between(-1.0f, 1.0f);
            off.q = 0;
            vel.q = pos.q;
            offset_component_or_smoke_scale = random_float_between(-4.0f, 4.0f);
            offset_component_or_smoke_scale *= fast_cos(cave_wall_moby->rot.z);
            off.f[0] = offset_component_or_smoke_scale;
            offset_component_or_smoke_scale = random_float_between(-1.0f, 1.0f);
            offset_component_or_smoke_scale *= fast_sin(cave_wall_moby->rot.z);
            off.f[1] = offset_component_or_smoke_scale;
            off.f[2] = random_float_between(1.0f, 8.0f);
            pos.q = off.q;
            fast_vec_add(&pos, &pos, &cave_wall_moby->pos);
            fast_vec_normalize(&vel, &vel, random_float_between(1.0f, 5.0f) * frame_time);
            spawn_debris_chunk(&pos,
                               &vel,
                               colors.c[random_integer_below(3)],
                               random_float_between(0.05f, 0.15f),
                               FUN_L00_00257b90(60, 180),
                               1.0f,
                               1.0f,
                               0.75f,
                               0);
        }
        D_0014C190_257218[current_level_index][(s16)cave_wall_moby->save_id >> 5] |=
            1 << (cave_wall_moby->save_id & 0x1F);
        moby_killed[(s16)cave_wall_moby->save_id >> 5] |= 1 << (cave_wall_moby->save_id & 0x1F);
        D_0013D388[0xE] = 1;
        mark_moby_for_removal(cave_wall_moby);
    }
}

typedef struct {
    u8  pad0[0xA6];
    s16 type;
} Moby;

typedef struct {
    u8    pad0[0x44];
    s16   count;
    u8    pad46[0x178 - 0x46];
    Moby *list[1];
} MobyList;

typedef struct {
    u8  b0;
    u8  active;
    u8  pad2[0x1E];
    f32 x;
    f32 y;
    f32 z;
} Effect;

extern s32    cutscene_mode __asm__("D_L01_0015F5C4");
extern Effect D_L01_0017C8C0;
extern void   attach_manipulator(void *, int, void *) __asm__("FUN_0020cb10");

extern MobyList fx_cutscene __asm__("D_L01_0016CCE0");
void            apply_cutscene_big_head_cheat(void) __asm__("FUN_L01_002fac80");

void apply_cutscene_big_head_cheat(void) {
    s32 i;

    if (cutscene_mode == 2 && big_head_cheat_enabled != 0) {
        for (i = 0; i < fx_cutscene.count; i++) {
            if (fx_cutscene.list[i]->type == 0x32B && D_L01_0017C8C0.active == 0) {
                attach_manipulator(fx_cutscene.list[i], 0, &D_L01_0017C8C0);
                D_L01_0017C8C0.z = D_L01_0017C8C0.y = D_L01_0017C8C0.x = 2.75f;
            }
        }
    }
}

#include "qcopy.h"
#include "sda.h"

/* Agnogg ship update (moby classes 730, 731, 790 on level 01): state machine on moby->state. */
/* Ported from rac1-decomp (src/overlays/l01_novalis/vendor_002FABE8.c: func_L01_002FC140), where it is exact; names translated to the US level program. */

typedef struct {
    float x, y, z, w;
} V4;

typedef struct {
    int pad0[4];
    int f10;
} Class_2FC140;

typedef struct {
    /* 0x00 */ char pad0[0x10];
    /* 0x10 */ V4   v;
    /* 0x20 */ int  pad20;
    /* 0x24 */ int  rec;
    /* 0x28 */ int  pad28;
    /* 0x2C */ int  timer;
} Link;

typedef struct {
    /* 0x00 */ char           pad0[0x10];
    /* 0x10 */ V4             pos;
    /* 0x20 */ unsigned char  state;
    /* 0x21 */ char           pad21[3];
    /* 0x24 */ Class_2FC140  *pclass;
    /* 0x28 */ char           pad28[8];
    /* 0x30 */ unsigned char  f30;
    /* 0x31 */ char           pad31;
    /* 0x32 */ unsigned short f32;
    /* 0x34 */ unsigned short flags;
    /* 0x36 */ char           pad36[0x1C];
    /* 0x52 */ unsigned char  f52;
    /* 0x53 */ unsigned char  f53;
    /* 0x54 */ char           pad54[0x1C];
    /* 0x70 */ unsigned char  f70;
    /* 0x71 */ char           pad71[7];
    /* 0x78 */ void          *data;
    /* 0x7C */ char           pad7C[0x18];
    /* 0x94 */ int            f94;
    /* 0x98 */ char           pad98[0xE];
    /* 0xA6 */ short          oclass;
    /* 0xA8 */ char           padA8[8];
    /* 0xB0 */ unsigned char  fB0;
    /* 0xB1 */ char           padB1;
    /* 0xB2 */ short          fB2;
    /* 0xB4 */ char           padB4[8];
    /* 0xBC */ unsigned char  fBC;
    /* 0xBD */ char           padBD[0x43];
} Moby_2FC140;

typedef struct {
    /* 0x00 */ float f0;
    /* 0x04 */ int   link;
    /* 0x08 */ short timer;
    /* 0x0A */ short fA;
    /* 0x0C */ float fC;
    /* 0x10 */ int   f10;
    /* 0x14 */ int   list[7];
    /* 0x30 */ int   rec;
    /* 0x34 */ int   f34;
    /* 0x38 */ int   f38;
    /* 0x3C */ int   f3C;
} Data;

#include "rnc/overlay/cuboid.h"

typedef struct {
    unsigned char pad0[0x454];
    unsigned char collected[1];
} Lv;

extern Moby_2FC140  *ents_macro __asm__("D_L01_0015FFD8") MACRO_ADDR;
extern LevelCuboid  *l01_cuboids_macro __asm__("D_L01_001600EC") MACRO_ADDR;
extern int           cutscene_mode __asm__("D_L01_0015F5C4") MACRO_ADDR;
extern char          D_L01_001BAD60_2FC140[] __asm__("D_L01_001BAA50");
extern char          D_L01_001BAAA0[] __asm__("D_L01_001BAAA0");
extern unsigned char D_0014C050_u[] __asm__("D_0014C050");
extern void          apply_cutscene_big_head_cheat(void) __asm__("FUN_L01_002fac80");
extern void          blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
extern void          start_scene(int) __asm__("FUN_L00_00298840");
extern void          deactivate_recorded_moby(void) __asm__("FUN_L00_0028e990");
extern int           is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");
extern float         vector_distance_xy(void *, void *) __asm__("FUN_001f9b80");
extern float         fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern void          set_moby_animation(void *, int, int) __asm__("FUN_00212ed8");
extern void          queue_scene_movie_with_game_language(int) __asm__("FUN_L00_00299460");
extern void          unlock_planet(int) __asm__("FUN_L00_002607d0");
extern void          queue_planet_unlock_message(int) __asm__("FUN_L00_00262d38");
extern void record_moby_context_bytes(void *, int, void *, int, void *) __asm__("FUN_L01_0029b0a0");
extern void jet_reset_camera(void *, void *) __asm__("FUN_L00_00284e50");
extern void show_tracked_moby(void) __asm__("FUN_L01_002a2480");

extern unsigned char D_L01_0015FC88[] MACRO_ADDR;
extern f32           frame_time_macro __asm__("D_0015ED6C") MACRO_ADDR;
extern s32           current_level_index_macro __asm__("D_0015ED84") MACRO_ADDR;
void                 agnogg_ship_front_update(Moby_2FC140 *moby) __asm__("FUN_L01_002fad68");

void agnogg_ship_front_update(Moby_2FC140 *moby) {
    enum {
        AGNOGG_SHIP_INITIALIZE = 0,
        AGNOGG_SHIP_WAIT_FOR_DESCENT_TRIGGER = 1,
        AGNOGG_SHIP_DESCEND_TO_TARGET_HEIGHT = 2,
        AGNOGG_SHIP_WAIT_FOR_ANIMATION_1_GATE = 3,
        AGNOGG_SHIP_WAIT_FOR_LINKED_MOBIES_INACTIVE = 4,
        AGNOGG_SHIP_WAIT_BEFORE_SCENE = 5,
        AGNOGG_SHIP_WAIT_FOR_SCENE_3_END = 6,
        AGNOGG_SHIP_WAIT_TO_START_SCENE_4 = 7,
        AGNOGG_SHIP_WAIT_FOR_SCENE_4_END = 8,
        AGNOGG_SHIP_HOLD_TARGET_HEIGHT = 10
    };
    Data *data;
    Link *linked_moby_data;
    int   level_table_byte;
    int   inactive_or_absent_link_count;
    int   flag;
    int   i;
    int   scene_start_gate_value;
    float fz;
    float fd;

    data = moby->data;
    apply_cutscene_big_head_cheat();
    fast_dec_timer_s16(&data->timer);
    switch (moby->state) {
    case AGNOGG_SHIP_INITIALIZE:
        fz = moby->pos.z - 0.75f;
        moby->f30 = 0xFF;
        moby->state = 1;
        data->timer = 0;
        fd = data->f0;
        data->fC = fz;
        moby->f94 = 0;
        moby->flags = moby->flags | 0x41;
        moby->pos.z = moby->pos.z + fd;
        break;
    case AGNOGG_SHIP_WAIT_FOR_DESCENT_TRIGGER:
        level_table_byte =
            *(unsigned char *)(moby->fB0 + (current_level_index_macro << 4) + D_0014C050_u);
        if (level_table_byte == 0xFF) {
            if (moby->f53 != 2) {
                blend_moby_animation(moby, 2, 0, scale_game_frames(0x14));
            }
            moby->state = 10;
            moby->flags = moby->flags & 0xFFBE;
            moby->f32 = level_table_byte;
            moby->f94 = moby->pclass->f10;
        } else {
            if (cutscene_mode == 0) {
                unsigned char *pp = alternate_item_available;
                if (pp[0xF] == 0) {
                    start_scene(5);
                    pp[0xF] = 1;
                }
            }
            deactivate_recorded_moby();
            if (is_point_inside_clip_volume(D_0013F3D0, data->f10) != 0) {
                moby->flags = moby->flags & 0xFFBE;
                moby->f94 = moby->pclass->f10;
                if (data->link != -1) {
                    linked_moby_data = ents_macro[data->link].data;
                    linked_moby_data->timer = scale_game_frames(0x1A4);
                    ents_macro[data->link].fBC = 1;
                    clear_u64_value(&l01_cuboids_macro[linked_moby_data->rec].rot);
                    clear_u64_value(&linked_moby_data->v);
                    l01_cuboids_macro[linked_moby_data->rec].rot.y = -fast_arc_tan(
                        vector_distance_xy(&l01_cuboids_macro[linked_moby_data->rec].pos,
                                           &moby->pos),
                        moby->pos.z - l01_cuboids_macro[linked_moby_data->rec].pos.z);
                    l01_cuboids_macro[linked_moby_data->rec].rot.z =
                        fast_arc_tan(moby->pos.x - l01_cuboids_macro[linked_moby_data->rec].pos.x,
                                     moby->pos.y - l01_cuboids_macro[linked_moby_data->rec].pos.y);
                }
                moby->state = 2;
            }
        }
        break;
    case AGNOGG_SHIP_DESCEND_TO_TARGET_HEIGHT:
        moby->pos.z = moby->pos.z - frame_time_macro * 5.0f;
        if (data->link != -1) {
            linked_moby_data = ents_macro[data->link].data;
            linked_moby_data->v.y = -fast_arc_tan(
                vector_distance_xy(&l01_cuboids_macro[linked_moby_data->rec].pos, &moby->pos),
                moby->pos.z - l01_cuboids_macro[linked_moby_data->rec].pos.z);
            linked_moby_data->v.z =
                fast_arc_tan(moby->pos.x - l01_cuboids_macro[linked_moby_data->rec].pos.x,
                             moby->pos.y - l01_cuboids_macro[linked_moby_data->rec].pos.y);
        }
        if (moby->f53 == 0) {
            if (moby->pos.z - data->fC <= 0.0f) {
                set_moby_animation(moby, 1, 0);
            }
        }
        if (moby->pos.z <= data->fC) {
            moby->pos.z = data->fC;
            moby->state = 3;
        }
        break;
    case AGNOGG_SHIP_WAIT_FOR_ANIMATION_1_GATE:
        if (moby->f52 != 1) {
            break;
        }
        if ((moby->f70 & 2) == 0) {
            break;
        }
        if (moby->f53 != 2) {
            blend_moby_animation(moby, 2, 0, scale_game_frames(0x14));
        }
        if (moby->oclass == 0x2DA) {
            moby->state = 10;
        } else {
            moby->state = 4;
        }
        break;
    case AGNOGG_SHIP_WAIT_FOR_LINKED_MOBIES_INACTIVE:
        flag = 1;
        inactive_or_absent_link_count = 0;
        i = 0;
        do {
            if (data->list[i] != -1 && (char *)ents_macro + (data->list[i] << 8) != 0 &&
                ((Moby_2FC140 *)((char *)ents_macro + (data->list[i] << 8)))->state != 0xFE &&
                ((Moby_2FC140 *)((char *)ents_macro + (data->list[i] << 8)))->state != 0xFD) {
                flag = 0;
                break;
            }
            inactive_or_absent_link_count++;
            i++;
        } while (inactive_or_absent_link_count < 3);
        if (flag == 0) {
            break;
        }
        scene_start_gate_value = D_001413DC[0];
        if (scene_start_gate_value == 7) {
            break;
        }
        if (scene_start_gate_value == 0x14) {
            break;
        }
        data->timer = scale_game_frames(0x2D);
        moby->state = 5;
        break;
    case AGNOGG_SHIP_WAIT_BEFORE_SCENE:
        if (data->timer != 0) {
            break;
        }
        data->fA = moby->f32;
        moby->flags = moby->flags | 0x41;
        start_scene(3);
        moby->state = 6;
        break;
    case AGNOGG_SHIP_WAIT_FOR_SCENE_3_END:
        if (cutscene_mode == 2) {
            break;
        }
        queue_scene_movie_with_game_language(3);
        moby->state = 7;
        break;
    case AGNOGG_SHIP_WAIT_TO_START_SCENE_4:
        if (cutscene_mode == 2) {
            break;
        }
        start_scene(4);
        moby->state = 8;
        break;
    case AGNOGG_SHIP_WAIT_FOR_SCENE_4_END:
        if (cutscene_mode == 2) {
            break;
        }
        unlock_planet(3);
        queue_planet_unlock_message(3);
        moby->flags = moby->flags & 0xFFBE;
        if (data->link != -1) {
            ((Lv *)D_L01_001BAD60_2FC140)->collected[ents_macro[data->link].fB2] =
                ents_macro[data->link].fB0 + 2;
            if (ents_macro[data->link].fB0 == 0xFF ||
                (D_L01_0015FC88[ents_macro[data->link].fB0] != 0xFF &&
                 D_0014C050_u[ents_macro[data->link].fB0 + (current_level_index_macro << 4)] ==
                     0xFF)) {
                l01_save_state.cleared[ents_macro[data->link].fB2] = ents_macro[data->link].fB0 + 2;
            }
        }
        if (data->f3C != -1) {
            linked_moby_data = ents_macro[data->f3C].data;
            linked_moby_data->timer = scale_game_frames(0xB4);
            ents_macro[data->f3C].fBC = 1;
            if (data->f34 != -1 && data->f38 != -1) {
                ents_macro[data->f34].fBC = 2;
                ents_macro[data->f38].fBC = 2;
                record_moby_context_bytes(
                    &ents_macro[data->f34].fBC, 1, &ents_macro[data->f34], 1, D_L01_001BAAA0);
                record_moby_context_bytes(
                    &ents_macro[data->f38].fBC, 1, &ents_macro[data->f38], 1, D_L01_001BAAA0);
            }
        }
        jet_clear_persistent(moby->fB0);
        if (data->rec != -1) {
            jet_reset_camera(&l01_cuboids_macro[data->rec].pos, &l01_cuboids_macro[data->rec].rot);
        }
        memcard_save_data(0, -1);
        show_tracked_moby();
        moby->state = 10;
        break;
    case AGNOGG_SHIP_HOLD_TARGET_HEIGHT:
        moby->pos.z = data->fC;
        break;
    }
}

typedef union {
    u128 q;
    f32  f[4];
} PendVec4;

typedef struct {
    u8       pad0[0x60];
    PendVec4 pivot;
    f32      swing;
    s32      sound;
} PendulumVars;

typedef struct {
    u8            pad0[0x10];
    PendVec4      pos;
    u8            state;
    u8            pad21[0x23];
    f32           rot_x;
    f32           rot_z;
    u8            pad4c[0x2C];
    PendulumVars *vars;
    u8            pad7c[0x34];
    u8            id;
    u8            padB1[0xB];
    u8            trigger;
} PendulumMoby;

extern u8   D_0014C050[];
extern void release_voice_slot(int) __asm__("FUN_0022d798");

void bridge_half_update(PendulumMoby *self) __asm__("FUN_L01_002fb8a8");

void bridge_half_update(PendulumMoby *self) {
    enum {
        BRIDGE_HALF_INITIALIZE = 0,
        BRIDGE_HALF_WAIT_FOR_TRIGGER = 1,
        BRIDGE_HALF_REDUCE_SWING_TO_ZERO = 2
    };
    PendulumVars *pendulum_vars;
    PendVec4      arm;
    PendVec4      swing_offset;
    s32           voice_slot_index;
    u8           *voice;

    pendulum_vars = self->vars;
    switch (self->state) {
    case BRIDGE_HALF_INITIALIZE:
        qcopy(&pendulum_vars->pivot, &self->pos);
        pendulum_vars->swing = 1.0f;
        self->state = 1;
        pendulum_vars->sound = -1;
        break;
    case BRIDGE_HALF_WAIT_FOR_TRIGGER:
        if (self->trigger == 2 || D_0014C050[self->id + current_level_index * 16] == 0xFF) {
            self->state = 2;
            self->trigger = 0;
        }
        break;
    case BRIDGE_HALF_REDUCE_SWING_TO_ZERO:
        pendulum_vars->swing -= frame_time * 0.5f;
        if (pendulum_vars->swing < 0.0f) {
            pendulum_vars->swing = 0.0f;
        }
        if (pendulum_vars->swing == 0.0f) {
            voice_slot_index = pendulum_vars->sound;
            if (voice_slot_index != -1) {
                voice = ((u8 *)&voice_pool) + voice_slot_index * 0x70;
                if (*(void **)(voice + 0x88) == self && voice[0x74] != 0) {
                    release_voice_slot(voice_slot_index);
                }
            }
            pendulum_vars->sound = -1;
            self->state = 1;
        } else if (!voice_is_playing(self, pendulum_vars->sound)) {
            pendulum_vars->sound = FUN_0022da68(0, 4, self);
        }
        break;
    }
    self->rot_x = pendulum_vars->swing * 0.62831855f;
    arm.q = 0;
    arm.f[2] = -11.0f;
    fast_vec_add(&arm, &arm, &pendulum_vars->pivot);
    swing_offset.f[0] = fast_cos(self->rot_z) * 11.0f * fast_sin(self->rot_x);
    swing_offset.f[1] = fast_sin(self->rot_z) * 11.0f * fast_sin(self->rot_x);
    swing_offset.f[2] = fast_cos(self->rot_x) * 11.0f;
    fast_vec_add(&self->pos, &arm, &swing_offset);
}

#include "sda.h"

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    u8  pad0C[0x12];
    u16 color;
    f32 f20;
    f32 f24;
    f32 f28;
    f32 f2C;
    u8  pad30[0x14];
    s32 i44;
    u8  rest[0x1148];
} PoolTile;

typedef struct {
    f32 f00, f04, f08, f0C, f10, f14, f18, f1C, f20, f24;
    s32 i28, i2C;
    f32 f30, f34, f38;
    u8  c3C, c3D;
} PoolLight;

typedef struct {
    f32 x, y, z, w;
} PoolVec;

extern PoolTile  pool_tiles[] __asm__("D_L01_001E34C0");
extern PoolLight pool_light __asm__("D_L01_001CAD00");
extern f32       pool_sun[] __asm__("D_L01_00180340");
extern u8        pool_tile_tex[] __asm__("D_L01_001FA6C0");
extern s32       pool_first[] __asm__("D_L01_001FA5E8");
extern s32       pool_count[] __asm__("D_L01_001FA608");
extern s32       pool_odds[] __asm__("D_L01_001FA628");
extern u32       pool_colors[] __asm__("D_L01_001FA590");
extern u128      pool_drips[] __asm__("D_L01_001FA650");
extern u8        pool_wave[] __asm__("D_L01_001FA6A0");
extern u8        camera_pos[] __asm__("D_L01_00167240");
extern s32       level_frame_count __asm__("D_L01_0015F5CC") MACRO_ADDR;
s32              pool_drip_timer __asm__("D_L01_00161C84") = 0;
extern PoolTile *pool_draw_tiles __asm__("D_L01_001612D0") MACRO_ADDR;
extern u8       *pool_draw_tex __asm__("D_L01_001612D4") MACRO_ADDR;
extern s32       pool_draw_count __asm__("D_L01_001612D8") MACRO_ADDR;
extern void      pool_init_tiles(void *, s32) __asm__("FUN_L01_002b7a48");
extern void      pool_bind_tile(void *, void *) __asm__("FUN_L01_002b7c68");
extern void      pool_set_scale(f32) __asm__("FUN_L01_002b7ef0");
extern void      pool_flush(void *, s32) __asm__("FUN_L01_002b7fe0");
extern void      pool_ripple(f32, f32, f32, f32, void *, s32) __asm__("FUN_L01_002b8288");
extern void      pool_splash(f32, f32, f32, f32, void *, s32, s32) __asm__("FUN_L00_002a3ec8");
extern s32       is_point_inside_clip_volume(void *, s32) __asm__("FUN_00214720");
extern void      render_level_water_grids(void) __asm__("FUN_L01_002fd0c0");
extern int       add_draw_callback(void (*)(void), void *) __asm__("FUN_001f4600");
extern void      pool_drip(void *, f32) __asm__("FUN_L01_002ffcd0");
extern void      pool_bubble(void *, void *, f32) __asm__("FUN_L01_00287b00");
extern void      pool_bubbles(void *, void *, f32, f32) __asm__("FUN_L01_00287c80");

extern u8 D_L01_00161200  MACRO_ADDR;
extern u8 D_L01_00161201  MACRO_ADDR;
extern u8 D_L01_00161202  MACRO_ADDR;
extern u8                 water_tint __asm__("D_L01_00161203") MACRO_ADDR;
extern u8 D_L01_00161204  MACRO_ADDR;
extern u8 D_L01_00161205  MACRO_ADDR;
extern u8 D_L01_00161206  MACRO_ADDR;
extern f32 D_L01_00161208 MACRO_ADDR;
extern f32 D_L01_0016120C MACRO_ADDR;
extern f32 D_L01_00161210 MACRO_ADDR;
extern f32 D_L01_00161214 MACRO_ADDR;
void update_pool_tiles_and_effects(struct Moby *moby) __asm__("FUN_L01_002fd0e8");

void update_pool_tiles_and_effects(struct Moby *moby) {
    enum {
        POOL_INITIALIZE = 0,
        POOL_UPDATE_TILES_AND_EFFECTS = 1
    };
    s32      *volumes;
    s32       found;
    s32       i;
    s32       j;
    PoolVec   drip_position_or_bubble_vector;
    PoolVec   bubble_position;
    s32       k;
    f32       bubble_path_fraction;
    f32       zero;
    u8       *tile_binding_output;
    s32       ripple_tile_index;
    s32       bind_tile_index;
    PoolTile *tiles;

    volumes = (s32 *)moby->pvars;
    switch (moby->state) {
    case POOL_INITIALIZE:
        moby->state = 1;
        moby->update_dist = 0xFF;
        moby->pos.z = moby->pos.z + 0.5f;
        pool_init_tiles(pool_tiles, 21);
        zero = 0.0f;
        pool_tiles[15].i44 = 8;
        pool_tiles[16].f28 = -0.0016f;
        pool_tiles[15].f28 = -0.0016f;
        pool_tiles[14].f28 = -0.0016f;
        pool_tiles[16].f2C = zero;
        pool_tiles[13].f28 = -0.0016f;
        pool_tiles[14].f2C = zero;
        pool_tiles[15].f2C = zero;
        pool_tiles[13].f2C = zero;
        pool_set_scale(1.0f);
        {
            PoolLight *pool_lighting;
            f32        sun_angle;

            pool_lighting = &pool_light;
            pool_lighting->f00 = 16.0f;
            pool_lighting->f04 = 16.0f;
            pool_lighting->f08 = -8.0f;
            pool_lighting->f0C = -8.0f;
            pool_lighting->f10 = 1.0f;
            pool_lighting->f14 = 1.0f;
            pool_lighting->f18 = 0.9f;
            pool_lighting->f20 = 0.1f;
            sun_angle = fast_arc_tan(pool_sun[4], pool_sun[5]);
            pool_lighting->f30 = fast_cos(sun_angle) * 0.57735f;
            pool_lighting->f34 = fast_sin(sun_angle) * 0.57735f;
            pool_lighting->f38 = -0.57735f;
            pool_lighting->i28 = 0x29;
            pool_lighting->i2C = 0x2A;
            pool_lighting->c3C = 0x1C;
            pool_lighting->c3D = 0x40;
        }
        pool_draw_count = 21;
        D_L01_00161208 = zero;
        tile_binding_output = pool_tile_tex;
        D_L01_0016120C = 32768.0f;
        D_L01_00161210 = 255.0f;
        D_L01_00161214 = 48.0f;
        pool_draw_tiles = pool_tiles;
        pool_draw_tex = pool_tile_tex;
        for (bind_tile_index = 0; bind_tile_index < 21; bind_tile_index++) {
            pool_bind_tile(&pool_tiles[bind_tile_index],
                           tile_binding_output + bind_tile_index * 16);
        }
        for (ripple_tile_index = 0; ripple_tile_index < 21; ripple_tile_index++) {
            PoolTile *tile = &pool_tiles[ripple_tile_index];
            tiles = pool_tiles;

            for (j = 0; j < 2; j++) {
                f32 x = tile->x + random_float_between(-6.0f, 6.0f);
                f32 y = tile->y + random_float_between(-6.0f, 6.0f);

                pool_ripple(x, y, 2.0f, 0.2f, tiles, 21);
            }
        }
        break;
    case POOL_UPDATE_TILES_AND_EFFECTS:
        found = -1;
        for (i = 0; i < 7; i++) {
            if (*(s32 *)((u8 *)volumes + (i << 2)) == -1 ||
                is_point_inside_clip_volume(camera_pos, *(s32 *)((u8 *)volumes + (i << 2))) == 0) {
                for (k = 0; k < pool_count[i]; k++) {
                    pool_tiles[pool_first[i] + k].color = 0;
                }
            } else {
                found = i;
                for (j = 0; j < pool_count[i]; j++) {
                    tiles = pool_tiles;
                    tiles[pool_first[i] + j].color = pool_colors[pool_first[i] + j];
                    if (random_integer_below(pool_odds[i]) == 0) {
                        f32 x = tiles[pool_first[i] + j].x + random_float_between(-4.0f, 4.0f);
                        f32 y = tiles[pool_first[i] + j].y + random_float_between(-4.0f, 4.0f);

                        pool_splash(x, y, 1.0f, -0.05f, pool_draw_tiles + pool_first[i] + j, 1, 1);
                    }
                }
            }
        }
        if (found >= 0) {
            if (found > 0 && found < 4) {
                D_L01_00161200 = 0;
                D_L01_00161201 = 0x28;
                D_L01_00161202 = 0x30;
                water_tint = 0x40;
                D_L01_00161204 = 0;
                D_L01_00161205 = 0x28;
                D_L01_00161206 = 0x30;
            } else {
                D_L01_00161200 = 0x18;
                D_L01_00161201 = 0x30;
                D_L01_00161202 = 0x90;
                water_tint = 0x30;
                D_L01_00161204 = 0x18;
                D_L01_00161205 = 0x30;
                D_L01_00161206 = 0x90;
            }
        }
        pool_flush(pool_tiles, 21);
        add_draw_callback(render_level_water_grids, moby);
        if (found == 0 || found == 6) {
            pool_drip_timer--;
            if (pool_drip_timer <= 0) {
                qcopy(&drip_position_or_bubble_vector, &pool_drips[random_integer_below(5)]);
                drip_position_or_bubble_vector.x =
                    drip_position_or_bubble_vector.x + random_float_between(-0.15f, 0.15f);
                drip_position_or_bubble_vector.y =
                    drip_position_or_bubble_vector.y + random_float_between(-0.15f, 0.15f);
                pool_drip(&drip_position_or_bubble_vector, 0.2f);
                pool_drip_timer = random_int_between(300, 1200);
            }
        }
        if (found == 5) {
            bubble_path_fraction = pool_wave[level_frame_count % 20] / 20.0f;
            drip_position_or_bubble_vector.x = 0.02f;
            drip_position_or_bubble_vector.y = 0.0f;
            drip_position_or_bubble_vector.z = 0.0f;
            bubble_position.x = random_float_between(-0.6f, 0.6f) + 177.0f;
            bubble_position.y =
                bubble_path_fraction * -11.0f + 176.0f + random_float_between(-0.1f, 0.1f);
            bubble_position.z = 39.0f;
            pool_bubbles(&bubble_position,
                         &drip_position_or_bubble_vector,
                         1.0f,
                         random_float_between(0.4f, 0.5f));
            drip_position_or_bubble_vector.x = 0.0f;
            drip_position_or_bubble_vector.y = 0.0f;
            drip_position_or_bubble_vector.z = 0.0f;
            for (bubble_path_fraction = drip_position_or_bubble_vector.x;
                 bubble_path_fraction < 1.0f;
                 bubble_path_fraction += 0.05f) {
                if (random_integer_below(32) == 0) {
                    bubble_position.x = random_float_between(-0.6f, 0.6f) + 178.0f;
                    bubble_position.y =
                        bubble_path_fraction * -11.0f + 176.0f + random_float_between(-0.1f, 0.1f);
                    bubble_position.z = random_float_between(-0.2f, 0.0f) + 39.0f;
                    pool_bubble(&bubble_position,
                                &drip_position_or_bubble_vector,
                                random_float_between(4.0f, 10.0f));
                }
            }
        }
        break;
    }
}

typedef struct {
    f32 x, y, z, w;
} SparkVec;

typedef struct {
    u8  pad0[0x24];
    f32 radius;
} SparkClass;

typedef struct {
    u8          pad0[0x10];
    SparkVec    pos;
    u8          state;
    u8          pad21[3];
    SparkClass *pclass;
    u8          pad28[4];
    f32         scale;
} SparkMoby;

typedef struct {
    u8  pad0[0x2C];
    f32 f2c;
} SparkHit;

extern s32   D_L01_0015F580;
extern void  spawn_proximity_gated_bolts() __asm__("FUN_L01_002787a0");
extern void *FUN_L00_00263fd8(void *, s32, void *, void *, s32, s32, void *, void *, f32, void *);
extern void  spawn_moby_from_placement() __asm__("FUN_L01_00278e20");

void green_mushroom_update(SparkMoby *m) __asm__("FUN_L01_002fd9a0");

void green_mushroom_update(SparkMoby *mushroom_moby) {
    enum {
        GREEN_MUSHROOM_INITIALIZE = 0,
        GREEN_MUSHROOM_WAIT_FOR_POSITIVE_HIT = 1,
        GREEN_MUSHROOM_SPAWN_PIECES_AND_REMOVE = 2
    };
    SparkHit *hit;
    s32       flag;
    s32       i;
    f32       fragment_placement_angle;
    f32       scale_ratio;
    SparkVec  fragment_spawn_position;
    SparkVec  fragment_rotation;

    flag = 0;
    hit = find_moby_hit(mushroom_moby, 0x10000, 0);
    switch (mushroom_moby->state) {
    case GREEN_MUSHROOM_INITIALIZE:
        mushroom_moby->state = 1;
        break;
    case GREEN_MUSHROOM_WAIT_FOR_POSITIVE_HIT:
        if (hit != 0 && hit->f2c > 0.0f) {
            flag = 1;
        }
        if (flag) {
            mushroom_moby->state = 2;
        }
        break;
    case GREEN_MUSHROOM_SPAWN_PIECES_AND_REMOVE:
        FUN_0022da68(0, 0, mushroom_moby);
        spawn_proximity_gated_bolts(mushroom_moby);
        scale_ratio = mushroom_moby->scale / mushroom_moby->pclass->radius;
        for (i = 0; i < 4; i++) {
            fragment_placement_angle = (f32)i * 1.5707964f;
            clear_u64_value(&fragment_rotation);
            fragment_rotation.z = fast_add_rotations(fragment_placement_angle, 0.87266463f);
            fragment_spawn_position.x = fast_cos(fragment_placement_angle) * -0.57f * scale_ratio;
            fragment_spawn_position.y = fast_sin(fragment_placement_angle) * -0.57f * scale_ratio;
            fragment_spawn_position.z = 0.0f;
            fast_vec_add(&fragment_spawn_position, &fragment_spawn_position, &mushroom_moby->pos);
            fragment_spawn_position.z += scale_ratio * 1.2f;
            FUN_L00_00263fd8(mushroom_moby,
                             0x719,
                             &fragment_spawn_position,
                             &fragment_rotation,
                             0,
                             0,
                             &D_L01_0015F580,
                             &D_L01_0015F580,
                             0.0f,
                             &D_L01_0015F580);
        }
        spawn_moby_from_placement(mushroom_moby, 0x718);
        mark_moby_for_removal(mushroom_moby);
        break;
    }
}

/* The four scrolling layers FUN_L01_002feb28 draws (0x60 bytes each, the ScrollLayer layout of birdbot.c). */
typedef struct {
    Vec4 *verts;
    u8    pad4[8];
    s32   count;
    u8    pad10[0x4C];
    f32   glow;
} WaterLayer;

extern WaterLayer D_L01_001FBC80[];
s32               D_L01_00161CE0 = 0;
extern s32        level_frame_count __asm__("D_L01_0015F5CC");
extern void       draw_four_scroll_layers(void) __asm__("FUN_L01_002feb28");

/* On first use, lowers and ripples the layers' vertices; every frame, pulses their glow and queues FUN_L01_002feb28. */
void update_scroll_layers_and_queue_draw(void *callback_argument) __asm__("FUN_L01_002feb58");

void update_scroll_layers_and_queue_draw(void *callback_argument) {
    s32   i;
    s32   glow_layer_index;
    s32   j;
    s32   k;
    s32   ripple_layer_index;
    s32   vertex_pair_start_index;
    Vec4 *lowered_vertex;
    Vec4 *ripple_vertex_pair;
    f32   glow_factor;

    if (D_L01_00161CE0 == 0) {
        D_L01_00161CE0 = 1;
        for (i = 0; i < 4; i++) {
            lowered_vertex = (D_L01_001FBC80 + i)->verts;
            for (j = 0; j < (D_L01_001FBC80 + i)->count; j++) {
                lowered_vertex->f[3] = lowered_vertex->f[2] = lowered_vertex->f[2] - 0.45f;
                lowered_vertex++;
            }
        }
        k = 1;
        for (ripple_layer_index = 0; ripple_layer_index < 4; ripple_layer_index++) {
            k--;
            ripple_vertex_pair = (D_L01_001FBC80 + ripple_layer_index)->verts;
            for (vertex_pair_start_index = 0;
                 vertex_pair_start_index < (D_L01_001FBC80 + ripple_layer_index)->count;
                 vertex_pair_start_index += 2) {
                if (k & 1) {
                    ripple_vertex_pair[0].f[2] += 0.05f;
                    ripple_vertex_pair[0].f[3] -= 0.05f;
                    ripple_vertex_pair[1].f[2] -= 0.05f;
                    ripple_vertex_pair[1].f[3] += 0.05f;
                } else {
                    ripple_vertex_pair[0].f[2] -= 0.05f;
                    ripple_vertex_pair[0].f[3] += 0.05f;
                    ripple_vertex_pair[1].f[2] += 0.05f;
                    ripple_vertex_pair[1].f[3] -= 0.05f;
                }
                ripple_vertex_pair += 2;
                k++;
            }
        }
    }
    glow_factor =
        fast_sin((convert_integer_to_float(level_frame_count & 0x3F) - 32.0f) * 0.09817477f);
    glow_factor = glow_factor * 0.5f + 0.5f;
    for (glow_layer_index = 3; glow_layer_index >= 0; glow_layer_index--) {
        D_L01_001FBC80[glow_layer_index].glow = glow_factor;
    }
    add_draw_callback(draw_four_scroll_layers, callback_argument);
}

typedef struct {
    Vec4 home;
    f32  t;
    f32  radius;
} HoverVars;

typedef struct {
    u8         pad0[0x10];
    Vec4       pos;
    u8         state;
    u8         pad21[0x27];
    f32        rot_z;
    u8         pad4C[0x2C];
    HoverVars *vars;
    u8         pad7C[0x2A];
    s16        oclass;
} HoverMoby;

void door_left_update(HoverMoby *m) __asm__("FUN_L01_002fed68");

void door_left_update(HoverMoby *door_moby) {
    enum {
        DOOR_INITIALIZE = 0,
        DOOR_CLOSED = 1,
        DOOR_OPENING = 2,
        DOOR_OPEN = 3,
        DOOR_CLOSING = 4
    };
    HoverVars *door_vars;
    Vec4      *player;
    Vec4       door_translation_offset;
    f32        k;

    door_vars = door_moby->vars;
    switch (door_moby->state) {
    case DOOR_INITIALIZE:
        qcopy(&door_vars->home, &door_moby->pos);
        door_vars->t = 0.0f;
        door_moby->state = 1;
        break;
    case DOOR_CLOSED:
        player = &hero.motion.pos;
        if (vector_distance_xy(door_vars, player) < door_vars->radius &&
            absolute_float(door_moby->pos.f[2] - player->f[2]) < 2.0f) {
            door_moby->state = 2;
            if (door_moby->oclass == 0x300) {
                FUN_0022da68(0, 0, door_moby);
            }
        }
        break;
    case DOOR_OPENING:
        door_vars->t += 1.0f / scale_time(20.0f);
        if (door_vars->t >= 1.0f) {
            door_vars->t = 1.0f;
            door_moby->state = 3;
        }
        break;
    case DOOR_OPEN:
        k = 1.1f;
        if (vector_distance_xy(door_vars, &hero.motion.pos) > door_vars->radius * k &&
            vector_distance_xy(door_vars, D_L01_00167240) > door_vars->radius * k) {
            door_moby->state = 4;
            if (door_moby->oclass == 0x300) {
                FUN_0022da68(0, 0, door_moby);
            }
        }
        break;
    case DOOR_CLOSING:
        player = &hero.motion.pos;
        if (vector_distance_xy(door_vars, player) < door_vars->radius &&
            absolute_float(door_moby->pos.f[2] - player->f[2]) < 2.0f) {
            door_moby->state = 2;
        } else {
            door_vars->t -= 1.0f / scale_time(20.0f);
            if (door_vars->t <= 0.0f) {
                door_vars->t = 0.0f;
                door_moby->state = 1;
            }
        }
        break;
    }
    door_translation_offset.f[0] =
        fast_cos(door_moby->rot_z) * door_vars->t * (door_moby->oclass == 0x300 ? 2.0f : -2.0f);
    door_translation_offset.f[1] =
        fast_sin(door_moby->rot_z) * door_vars->t * (door_moby->oclass == 0x300 ? 2.0f : -2.0f);
    door_translation_offset.f[2] = 0.0f;
    door_moby->pos.f[0] = door_vars->home.f[0] + door_translation_offset.f[0];
    door_moby->pos.f[1] = door_vars->home.f[1] + door_translation_offset.f[1];
}

/* Ported from rac1-decomp (src/overlays/l01_novalis/vendor_002FABE8.c: func_L01_00300C38), where it is exact; names translated to the US level program. */

extern char *FUN_L01_002ffa90_c(void *owner) __asm__("FUN_L01_002ffa90");
extern float fast_cos(float);
extern float fast_sin(float);
extern float random_float_between(float, float);
extern int   allocate_voice_for_target_entry(int, int, int);
extern int   scale_game_frames(int);

void pipe_update(struct Moby *m) __asm__("FUN_L01_002ff860");

void pipe_update(struct Moby *moby) {
    char *pipe_vars = (char *)moby->pvars;
    char *hit = find_moby_hit(moby, 0x10000, 0);
    moby->hit_slot = 0xFF;
    if (hit != 0 && 0.0f < *(float *)(hit + 0x2C)) {
        float particle_position[4];
        float particle_velocity[4];
        int   random_countdown =
            random_integer_below(*(int *)(pipe_vars + 4) - *(int *)pipe_vars + 1) +
            *(int *)pipe_vars;
        int k;
        if (random_countdown > 0) {
            do {
                random_countdown--;
            } while (random_countdown != 0);
        }
        allocate_voice_for_target_entry(0, 0, (int)moby);
        for (k = 0x95; k >= 0;) {
            float velocity_component_or_size, horizontal_speed_sample;
            *(OvlQuad *)particle_position = 0;
            particle_position[0] = -0.2f;
            particle_position[2] = 1.2f;
            k--;
            fast_vec_add(particle_position, particle_position, &moby->pos);
            velocity_component_or_size = fast_cos(random_angle_radians());
            horizontal_speed_sample = random_float_between(1.5f, 3.0f);
            velocity_component_or_size =
                velocity_component_or_size * (horizontal_speed_sample * frame_time);
            particle_velocity[0] = velocity_component_or_size;
            velocity_component_or_size = fast_sin(random_angle_radians());
            horizontal_speed_sample = random_float_between(1.5f, 3.0f);
            velocity_component_or_size =
                velocity_component_or_size * (horizontal_speed_sample * frame_time);
            particle_velocity[1] = velocity_component_or_size;
            particle_velocity[2] = 0;
            particle_velocity[2] = random_float_between(particle_velocity[2], 3.0f) * frame_time;
            velocity_component_or_size = random_float_between(125000.0f, 175000.0f);
            spawn_effect_particle_with_target_scalar(
                particle_position,
                particle_velocity,
                0x207F7F7F,
                0x272727,
                velocity_component_or_size,
                scale_game_frames(FUN_L00_00257b90(0x2D, 0x3C)));
        }
        FUN_L01_002ffa90_c(moby);
        mark_moby_for_removal(moby);
    }
}

typedef struct {
    u8   pad0[0x10];
    u128 pos;
    u8   state;
    u8   pad21[0xF];
    u8   b30;
    u8   b31;
    s16  h32;
    u8   pad34[4];
    u64  d38;
    u128 rot;
    u8   pad50[0x28];
    s32 *pvars;
} SpawnedMoby;

SpawnedMoby *
spawn_pipe_replacement_moby(SpawnedMoby *transform_source_moby) __asm__("FUN_L01_002ffa90");

SpawnedMoby *spawn_pipe_replacement_moby(SpawnedMoby *transform_source_moby) {
    SpawnedMoby *spawned_moby;
    s32         *spawned_moby_vars;

    spawned_moby = create_moby(0x30B);
    if (spawned_moby != 0) {
        spawned_moby->b30 = 0x20;
        spawned_moby->h32 = 0xFF;
        spawned_moby->b31 = 1;
        spawned_moby->state = 0;
        spawned_moby_vars = spawned_moby->pvars;
        spawned_moby->d38 = transform_source_moby->d38;
        qcopy(&spawned_moby->pos, &transform_source_moby->pos);
        qcopy(&spawned_moby->rot, &transform_source_moby->rot);
        *spawned_moby_vars = -1;
        update_entity(spawned_moby, -1);
    }
    return spawned_moby;
}

typedef struct {
    s32 sound;
} SmokeVars;

typedef struct {
    u8         pad0[0x10];
    u128       pos;
    u8         pad20[0x58];
    SmokeVars *vars;
    u8         pad7c[0x44];
    u128       mtx[4];
} SmokeMoby;

void update_smoke_emitter(SmokeMoby *self) __asm__("FUN_L01_002ffb28");

void update_smoke_emitter(SmokeMoby *self) {
    SmokeVars *smoke_vars;
    Vec4       pos;
    Vec4       particle_velocity;
    Vec4       local_velocity;
    f32        size;
    s32        i;

    smoke_vars = self->vars;
    if (voice_is_playing(self, smoke_vars->sound) == 0) {
        smoke_vars->sound = FUN_0022da68(0, 4, self);
    }
    for (i = 0; i < 2; i++) {
        pos.q = 0;
        local_velocity.q = 0;
        pos.f[0] = -0.2f;
        pos.f[2] = 1.2f;
        local_velocity.f[0] = random_float_between(0.8f, 1.2f) * (frame_time * 3.0f);
        local_velocity.f[2] = random_float_between(0.8f, 1.2f) * (frame_time + frame_time);
        particle_velocity.q = local_velocity.q;
        transform_vector_by_basis(&particle_velocity, &particle_velocity, self->mtx);
        fast_vec_add(&pos, &pos, &self->pos);
        size = random_float_between(125000.0f, 175000.0f);
        spawn_effect_particle_with_target_scalar(&pos,
                                                 &particle_velocity,
                                                 0x207F7F7F,
                                                 0x272727,
                                                 size,
                                                 scale_game_frames(FUN_L00_00257b90(0x2D, 0x3C)));
    }
}

typedef struct {
    u8  pad0[0x8];
    f32 f8;
    u8  padC[4];
    f32 f10;
    s32 timer;
    u8  pad18[4];
    f32 spin;
} OrbVars;

typedef struct {
    u8       pad0[0x10];
    u128     pos;
    u8       pad20[0xC];
    f32      scale;
    u8       b30;
    u8       b31;
    s16      h32;
    u8       pad34[0xC];
    s32      w40;
    u8       pad44[4];
    f32      rotz;
    u8       pad4C[0x2C];
    OrbVars *pvars;
} OrbMoby;

extern void copy_hero_moby_field_38(OrbMoby *) __asm__("FUN_L00_0025d1b8");

OrbMoby *FUN_L01_002ffcd0(u128 *pos, f32 scale) {
    OrbMoby *spawned_moby;
    OrbVars *spawned_moby_vars;

    spawned_moby = create_moby(0x313);
    if (spawned_moby != 0) {
        spawned_moby_vars = spawned_moby->pvars;
        copy_hero_moby_field_38(spawned_moby);
        spawned_moby->w40 = 0;
        spawned_moby->h32 = 0x40;
        spawned_moby->b30 = 0xFF;
        spawned_moby->scale = spawned_moby->scale * scale;
        spawned_moby->rotz = random_angle_radians();
        qcopy(&spawned_moby->pos, pos);
        qzero(spawned_moby_vars);
        spawned_moby_vars->f8 = -frame_time;
        spawned_moby_vars->timer = 0x78;
        spawned_moby_vars->f10 = frame_time_sq * 15.0f;
        spawned_moby_vars->spin = random_float_between(0.0031415929f, 0.062831856f);
        update_entity(spawned_moby);
    }
    return spawned_moby;
}

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/l01_novalis/vendor_002FABE8.c: func_L01_00301198), where it is exact; names translated to the US level program. */

extern char                *spawn_burst_moby(void *, float) __asm__("FUN_L00_002d7e90");
extern int                  pool_draw_count __asm__("D_L01_001612D8"); /* no foreign declaration */
extern int                  D_L01_001612D0;                            /* no foreign declaration */
extern float D_L01_001742E8 NOT_SDA;                                   /* no foreign declaration */
extern int  collision_line(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern int  collision_hit_material(void) __asm__("FUN_001f0b58");
extern int  allocate_voice_for_target_entry_alt(int, int, int) __asm__("FUN_0022da68");
extern int  tick_countdown_32_alt(int *) __asm__("FUN_001f9740");
extern void FUN_L00_0026f548(void *, void *, int, int);
extern void FUN_L00_002a3ec8(int, int, int, float, float, float, float);

/* Per-frame update of a falling debris moby: moves it, bursts into particles when it hits the ground, and removes it when its timer runs out. */
void update_falling_splash_moby(struct Moby *moby) __asm__("FUN_L01_002ffdc0");

void update_falling_splash_moby(struct Moby *moby) {
    float next_pos[4];
    float effect_pos[4];
    float particle_vel[4];
    char *data = (char *)moby->pvars;
    *(float *)(data + 8) -= *(float *)(data + 0x10);
    moby->rot.x += *(float *)(data + 0x1C);
    fast_vec_add(next_pos, &moby->pos, data);
    if (collision_line(&moby->pos, next_pos, 2, 0, 0)) {
        int   i;
        char *effect;
        if (!collision_hit_material()) {
            if (*(int *)(data + 0x18) == 0) {
                allocate_voice_for_target_entry_alt(0, 0, (int)moby);
            }
            FUN_L00_002a3ec8(
                D_L01_001612D0, pool_draw_count, 1, moby->pos.x, moby->pos.y, 0.5f, -0.35f);
            {
                float pos_unit = frame_time;
                float pos_unit2 = frame_time_sq;
                *(int *)(data + 0x18) = 1;
                *(int *)(data + 0x14) = 0x78;
                *(int *)(data + 0x1C) = 0;
                *(float *)(data + 8) = pos_unit * -1.5f;
                *(float *)(data + 0x10) = pos_unit2 * 0.8f;
            }
            effect_pos[0] = moby->pos.x;
            effect_pos[1] = moby->pos.y;
            effect_pos[2] = D_L01_001742E8;
            effect = spawn_burst_moby(effect_pos, 2.0f);
            if (effect != 0) {
                effect[0x23] = 0x70;
            }
            for (i = 0; i < 16; i++) {
                float particle_angle = random_angle_radians();
                float horizontal_velocity_magnitude =
                    random_float_between(frame_time * 0.0f, frame_time * 3.0f);
                particle_vel[0] = fast_cos(particle_angle) * horizontal_velocity_magnitude;
                particle_vel[1] = fast_sin(particle_angle) * horizontal_velocity_magnitude;
                particle_vel[2] = random_float_between(frame_time * 3.0f, frame_time * 6.5f);
                {
                    int lifetime = FUN_L00_00257b90(0x5A, 0x78);
                    FUN_L00_0026f548(effect_pos, particle_vel, random_integer_below(2), lifetime);
                }
            }
        } else {
            mark_moby_for_removal(moby);
            return;
        }
    }
    qcopy(&moby->pos, next_pos);
    if (*(int *)(data + 0x18) != 0) {
        moby->scale *= 0.99f;
    }
    if (tick_countdown_32_alt((int *)(data + 0x14))) {
        mark_moby_for_removal(moby);
    }
}

typedef struct {
    u8  pad0[0x10];
    s32 w10;
    u8  pad14[0x10];
    f32 f24;
} MobyClass;

typedef struct {
    u8  pad0[0x20];
    f32 f20;
    u8  pad24[0x14C];
    s32 w170;
    u8  pad174[0xB0];
    f32 f224;
    u8  pad228[0x28];
    f32 f250;
} ClonedVars;

typedef struct {
    u8         pad0[0x10];
    u128       pos;
    u8         pad20[4];
    MobyClass *mclass;
    u8         pad28[4];
    f32        scale;
    u8         b30;
    u8         b31;
    s16        h32;
    u16        flags;
    u8         pad36[2];
    u64        d38;
    u128       rot;
    u8         pad50[0x28];
    void      *pvars;
    u8         pad7C[0x18];
    s32        w94;
} ClonedMoby;

extern void get_first_moby_in_group(ClonedMoby **, s32, s32, s32) __asm__("FUN_L00_002591d0");

typedef struct {
    s32 src_id;
    u8  pad4[4];
    s32 count;
} SpawnerVars;

typedef struct {
    u8  pad0[0x60];
    u8  path[0x18];
    s32 w78;
    s32 w7C;
    s32 w80;
    s32 w84;
    f32 f88;
    u8  pad8C[0x24];
    f32 fB0;
    f32 fB4;
    u8  padB8[0x198];
    f32 f250;
} CloneAiVars;

typedef struct {
    u8    pad0[0x10];
    f32   x;
    f32   y;
    u8    pad18[8];
    u8    state;
    u8    pad21[0x57];
    void *pvars;
} SpawnerMoby;

typedef struct {
    u8  pad0[0x80];
    f32 x;
    f32 y;
} PlayerPosView;

extern s32         count_active_moby_group_members(s32, s32) __asm__("FUN_L01_0026e008");
extern ClonedMoby *initialize_beehive_spawn_moby(ClonedMoby *) __asm__("FUN_L01_00302328");
extern void trooper_start_knockback(void *, void *, s32, s32, s32, f32) __asm__("FUN_L00_0025c558");
s32         D_L01_00161E90 = 5;

void beehive_update(SpawnerMoby *m) __asm__("FUN_L01_003021b8");

void beehive_update(SpawnerMoby *beehive_moby) {
    enum {
        BEEHIVE_INITIALIZE = 0,
        BEEHIVE_TRY_SPAWN_CLONE = 1
    };
    SpawnerVars *beehive_vars = beehive_moby->pvars;
    SpawnerMoby *spawned_clone;
    CloneAiVars *nv;
    f32          k;

    switch (beehive_moby->state) {
    case BEEHIVE_INITIALIZE:
        beehive_moby->state = 1;
        break;
    case BEEHIVE_TRY_SPAWN_CLONE:
        if (beehive_vars->src_id < 0) {
            break;
        }
        if (count_active_moby_group_members(beehive_vars->src_id, 0xC) != 0) {
            break;
        }
        if (beehive_vars->count >= D_L01_00161E90) {
            break;
        }
        beehive_vars->count++;
        spawned_clone = (SpawnerMoby *)initialize_beehive_spawn_moby((ClonedMoby *)beehive_moby);
        if (spawned_clone != 0) {
            nv = spawned_clone->pvars;
            k = 0.8f;
            nv->w84 = 8;
            nv->w80 = truncate_float_to_s32(nv->f250 * k * 1024.0f);
            nv->f88 = nv->f250 * k;
            nv->fB0 = 7.0f;
            nv->fB4 = 13.0f;
            nv->w78 = 0;
            nv->w7C = 0;
            trooper_start_knockback(
                spawned_clone,
                nv->path,
                5,
                8,
                0,
                fast_add_rotations(fast_arc_tan(hero.motion.pos.f[0] - beehive_moby->x,
                                                hero.motion.pos.f[1] - beehive_moby->y),
                                   3.1415927f));
            spawned_clone->state = 9;
        }
        break;
    }
}

ClonedMoby *
initialize_beehive_spawn_moby(ClonedMoby *clone_spawner_moby) __asm__("FUN_L01_00302328");

ClonedMoby *initialize_beehive_spawn_moby(ClonedMoby *clone_spawner_moby) {
    ClonedMoby *destination_moby;
    ClonedVars *destination_moby_vars;

    destination_moby = 0;
    get_first_moby_in_group(&destination_moby, *(s32 *)clone_spawner_moby->pvars, 0, 0);
    destination_moby_vars = destination_moby->pvars;
    destination_moby->b30 = 0x80;
    destination_moby->h32 = 0x80;
    destination_moby->d38 = clone_spawner_moby->d38;
    destination_moby->b31 = 1;
    destination_moby->w94 = destination_moby->mclass->w10;
    destination_moby->flags |= 0x1020;
    destination_moby->scale = destination_moby->mclass->f24;
    {
        ClonedMoby *transform_destination = destination_moby;
        qcopy(&transform_destination->pos, &clone_spawner_moby->pos);
        qcopy(&transform_destination->rot, &clone_spawner_moby->rot);
    }
    destination_moby_vars->f20 = 3.0f;
    destination_moby_vars->f250 = 1.0f;
    destination_moby_vars->w170 = truncate_float_to_s32(819.2f);
    destination_moby_vars->f224 = 30.0f;
    update_entity(destination_moby);
    return destination_moby;
}

void mushroom_underside_update(void *self) __asm__("FUN_L01_00307c48");

void mushroom_underside_update(void *self) {
    u8 *hit;

    hit = find_moby_hit(self, 0x10000, 0);
    if (hit != NULL && *(f32 *)(hit + 0x2C) > 0.0f) {
        mark_moby_for_removal(self);
    }
}

BurstPalette D_L01_0020B4F0 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

BurstPalette D_L01_0020B508 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

s32 D_L01_001CAD28[1] __attribute__((section(".data"))) = {0};

s32 D_L01_001CAD2C[1] __attribute__((section(".data"))) = {0};

f32 D_L01_001CAD30[1] __attribute__((section(".data"))) = {0};

f32 D_L01_001CAD34[1] __attribute__((section(".data"))) = {0};

f32 D_L01_001CAD38[1] __attribute__((section(".data"))) = {0};
