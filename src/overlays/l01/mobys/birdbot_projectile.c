/* Level 01 moby class birdbot_projectile (class 0x2D2): its spawn helper, explosion effects and update. */
#include "types.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"

/* Spawns a birdbot projectile (class 0x2D2) at pos with starting velocity vel, at half scale and randomly turned. */
typedef struct {
    u128 vel;
    s32  owner;
    s32  color;
} ProjectileSpawnVars;

typedef struct {
    u8                   pad0[0x10];
    u128                 pos;
    u8                   pad20[0xC];
    f32                  scale;
    u8                   b30;
    u8                   b31;
    s16                  h32;
    u8                   pad34[0xC];
    f32                  rx;
    f32                  ry;
    f32                  rz;
    u8                   pad4c[0x2C];
    ProjectileSpawnVars *pvars;
} ProjectileSpawnMoby;

void set_moby_color(void *, s32, s32, s32) __asm__("FUN_L00_002502f0");
void spawn_source_attached_fading_effect_particle(void *, u32, s32, s32, f32) __asm__("FUN_L00_0026daa0");
extern void update_entity() __asm__("FUN_L00_00250df8");

#include "rnc/overlay/l01/mobys/birdbot_projectile.h"

/* yaw and pitch are passed by birdbot_update but not read. */
void *spawn_birdbot_projectile(
    void *owner, void *pos, void *vel, f32 yaw, f32 pitch, s32 color) __asm__("FUN_L01_002fa068");

void *

spawn_birdbot_projectile(void *owner, void *pos, void *vel, f32 yaw, f32 pitch, s32 color) {
    u128                 position_copy;
    u128                 velocity_copy;
    ProjectileSpawnMoby *spawned_moby;
    ProjectileSpawnVars *projectile_vars;
    u128                *pp;
    u128                *tp;

    pp = &position_copy;
    tp = &velocity_copy;
    position_copy = *(u128 *)pos;
    velocity_copy = *(u128 *)vel;
    spawned_moby = create_moby(0x2D2);
    if (spawned_moby != 0) {
        projectile_vars = spawned_moby->pvars;
        projectile_vars->owner = (s32)owner;
        spawned_moby->b30 = 0x7F;
        spawned_moby->h32 = 0x7F;
        spawned_moby->b31 = 1;
        spawned_moby->scale *= 0.5f;
        set_moby_color(spawned_moby, 0x7F, 0x7F, 0x7F);
        spawned_moby->rx = random_angle_radians();
        spawned_moby->ry = random_angle_radians();
        spawned_moby->rz = random_angle_radians();
        qcopy(&spawned_moby->pos, pp);
        qcopy(&projectile_vars->vel, tp);
        projectile_vars->color = color;
        spawn_source_attached_fading_effect_particle(
            spawned_moby, 0x2F00007F, color, -1, 210000.0f);
        spawn_source_attached_fading_effect_particle(
            spawned_moby, 0x4F004F7F, projectile_vars->color, -1, 115500.0f);
        update_entity(spawned_moby);
    }
    return spawned_moby;
}

extern s32 FUN_0022da68(s32, s32, void *);

#include "rnc/math/vector.h"

/*
 * Explosion burst at `where`: three particles in
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
    Vec4          burst_position;
    Vec4          particle_base_velocity;
    BurstPalette  inner;
    BurstPalette  outer;
    BurstPalette *inner_p;
    Vec4         *pos_p;
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

void birdbot_projectile_update(DropMoby *) __asm__("FUN_L01_002fa4c0");

/* Moby class 722 */
void birdbot_projectile_update(DropMoby *projectile) {
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

extern float random_float_between(float, float);
extern int   scale_game_frames(int);

BurstPalette D_L01_0020B4F0 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

BurstPalette D_L01_0020B508 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};
