/* Level 08 moby class 0x1a9: bomber tank bomb, fired by the bomber tank, and its spawner. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "rnc/math/random.h"
#include "qcopy.h"
#include "rnc/overlay/l08/mobys/bomber_tank_bomb.h"
#include "rnc/overlay/level_camera.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"

/* Ported from rac1-decomp src/overlays/l08_batalia/vendor_002B9438.c (func_L08_002B94B0) */

extern float D_0015ED70;

extern void mark_moby_for_removal(void *) __asm__("func_0020C828");

#include "rnc/overlay/collision.h"

extern CollisionHit D_L08_00174640;
extern float        D_0015ED6C_lui __asm__("D_0015ED6C");

extern int
FUN_L08_002dc9a8_collision(void *, float, int, void *, void *) __asm__("FUN_L00_001f2868");
extern int   FUN_L08_002dc9a8_trace(void *, void *, int, void *, int) __asm__("FUN_001efa68");
extern void  FUN_L08_002dc9a8_add(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern void  FUN_L08_002dc9a8_scale(float, void *, void *) __asm__("FUN_001f9bf8");
extern float FUN_L08_002dc9a8_sin(float) __asm__("FUN_001f9dc8");
extern float FUN_L08_002dc9a8_cos(float) __asm__("FUN_001f9de0");
extern int   FUN_L08_002dc9a8_random_color(int) __asm__("FUN_001f96f8");
extern int   FUN_L08_002dc9a8_color(float, int, int) __asm__("FUN_L08_00264930");
extern void
FUN_L08_002dc9a8_emit(void *, void *, int, int, float, int, int) __asm__("FUN_L00_0026bed0");
extern int D_L08_00174658_2DC9A8 __asm__("D_L08_00174658") NOT_SDA;

typedef struct __attribute__((packed)) EffectVectorCopy {
    unsigned long long values[2];
} EffectVectorCopy;

typedef struct EffectRecord {
    float pos[4];
    float velocity[4];
    void *owner;
    int   flags;
    int   unknown;
    float scale;
    int   state;
} EffectRecord;

void update_bomber_tank_bomb(struct Moby *moby) __asm__("FUN_L08_002dc9a8");

extern char *create_moby(int);
extern void  update_entity(void *) __asm__("FUN_L00_00250df8");

/* Spawns a moby of class 0x1A9 at a position, with a random spin and a heading from a vector. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002DDC18), where it is exact; names translated to the US level program. */
char *spawn_bomber_tank_bomb(char *source_moby,
                             char *spawn_position,
                             char *initial_velocity) __asm__("FUN_L08_002dc8a0");

char *spawn_bomber_tank_bomb(char *source_moby, char *spawn_position, char *initial_velocity) {
    char *moby = create_moby(0x1A9);
    char *data;
    if (moby) {
        data = ((char *)MOBY(moby)->pvars);
        MOBY(moby)->update_dist = 0xFF;
        MOBY(moby)->draw_dist = 0xFF;
        moby[0x31] = 1;
        moby[0x20] = 1;
        *(char **)(data + 0x20) = source_moby;
        *(int *)(data + 0x10) = scale_game_frames(0x12C);
        *(float *)(data + 0x1C) = 1.0f;
        *(int *)(data + 0x14) = 0;
        qcopy(moby + 0x10, spawn_position);
        qcopy(data, initial_velocity);
        MOBY(moby)->rot.x = random_float_between(-180.0f, 180.0f) * DEG_TO_RAD;
        MOBY(moby)->rot.z =
            fast_arc_tan(*(float *)initial_velocity, *(float *)(initial_velocity + 4)) + 1.5707964f;
        update_entity(moby);
    }
    return moby;
}

void update_bomber_tank_bomb(struct Moby *moby) {
    enum {
        PROJECTILE_STATE_FLYING = 1,
        PROJECTILE_STATE_DETONATE = 99
    };
    enum {
        DETONATION_EFFECT_FIXED_COLORS = 0,
        DETONATION_EFFECT_NO_EMISSION = 1,
        DETONATION_EFFECT_TWO_COLOR_CHOICE = 2,
        DETONATION_EFFECT_SPHERICAL_VELOCITY = 3
    };
    char        *moby_bytes = (char *)moby;
    char        *data = *(char **)(moby_bytes + 0x78);
    float        original[4];
    EffectRecord record;
    float       *position;
    float        effect[4];
    float        velocity[4];
    float        frame_time;
    float        radius;
    int          i, axis;
    float        angle;

    if (FUN_L08_002dc9a8_collision(moby_bytes + 0x10, 0.5f, 0, moby_bytes, 0) != 0 &&
        D_L08_00174658_2DC9A8 != 0 &&
        (D_L08_00174658_2DC9A8 != *(int *)(data + 0x20) || *(int *)(data + 0x14) != 0)) {
        moby_bytes[0x20] = 99;
    }
    switch ((unsigned char)moby_bytes[0x20]) {
    case PROJECTILE_STATE_FLYING: {
        position = (float *)(moby_bytes + 0x10);
        qcopy(original, position);
        fast_vec_add(position, position, data);
        *(float *)(data + 8) -= D_0015ED70 * 10.8f;
        qcopy(record.pos, position);
        record.owner = moby_bytes;
        record.flags = 0x810000;
        record.state = 1;
        record.scale = 1.0f;
        qcopy(record.velocity, data);
        if (FUN_L08_002dc9a8_trace(original, &record, 0, moby_bytes, 0) != 0) {
            if (*(int *)(data + 0x20) != 0 && D_L08_00174640.moby != *(int *)(data + 0x20)) {
                moby_bytes[0x20] = 99;
                qcopy(position, D_L08_00174640.point.f);
                FUN_L08_002dc9a8_add(data, data, &D_L08_00174640.normal_x);
                FUN_L08_002dc9a8_scale(D_0015ED6C_lui + D_0015ED6C_lui, data, data);
            }
        } else {
            if (fast_dec_timer(data + 0x10) != 0)
                moby_bytes[0x20] = 99;
        }
    } break;
    case PROJECTILE_STATE_DETONATE: {
        record.pos[0] = D_0015ED6C_lui * 14.0f;
        record.pos[1] = D_0015ED6C_lui * 22.0f;
        record.pos[2] = D_0015ED6C_lui * 12.0f;
        record.pos[3] = 0.0f;
        *(EffectVectorCopy *)original = *(EffectVectorCopy *)record.pos;
        record.owner = moby_bytes;
        record.flags = 0x810001;
        record.scale = 1.0f;
        clear_u64_value(record.velocity);
        record.state = 0;
        FUN_L08_002dc9a8_collision(moby_bytes + 0x10, 1.0f, 9, moby_bytes, record.velocity);
        allocate_voice_for_target_entry(0, 0, moby_bytes);
        for (i = 0; i < 50; i++) {
            axis = i % 4;
            *(u128 *)velocity = 0;
            velocity[0] = random_float_between(D_0015ED6C_lui * -0.5f, D_0015ED6C_lui * 0.5f);
            velocity[1] = random_float_between(D_0015ED6C_lui * -0.5f, D_0015ED6C_lui * 0.5f);
            velocity[2] = random_float_between(D_0015ED6C_lui * 4.0f, original[axis]);
            *(u128 *)effect = *(u128 *)velocity;
            radius = random_float_between(0.0f, 0.25f);
            angle = random_angle_radians();
            record.pos[0] = FUN_L08_002dc9a8_sin(angle) * radius;
            record.pos[1] = FUN_L08_002dc9a8_cos(angle) * radius;
            record.pos[2] = 0.0f;
            fast_vec_add(record.pos, record.pos, moby_bytes + 0x10);
            frame_time = radius * D_0015ED6C_lui;
            radius *= 0.717f;
            record.pos[2] -= radius;
            effect[2] -= frame_time * 8.0f;
            position = effect;
            switch (axis) {
            case DETONATION_EFFECT_FIXED_COLORS: {
                int   color, color2, alpha;
                float size = random_float_between(200000.0f, 300000.0f);
                color = FUN_L08_002dc9a8_random_color(60);
                color2 = FUN_L08_002dc9a8_random_color(120);
                alpha = random_int_between(color, color2);
                FUN_L08_002dc9a8_emit(record.pos, position, 0x1f101820, 0x101010, size, alpha, 0);
            } break;
            case DETONATION_EFFECT_TWO_COLOR_CHOICE: {
                int color, color2, alpha;
                color = random_integer_below(100) < 40 ? 0x5ff8f8f8 : 0x2f486078;
                color2 = FUN_L08_002dc9a8_random_color(30);
                alpha = FUN_L08_002dc9a8_random_color(45);
                FUN_L08_002dc9a8_emit(record.pos,
                                      position,
                                      color,
                                      0x0f000020,
                                      150000.0f,
                                      random_int_between(color2, alpha),
                                      2);
            } break;
            case DETONATION_EFFECT_SPHERICAL_VELOCITY: {
                int color, color2, alpha;
                color = FUN_L08_002dc9a8_color(
                    random_float_between(0.25f, 1.0f), 0x7f000000, 0x7f182030);
                color2 = FUN_L08_002dc9a8_color(random_float_between(0.5f, 1.0f), 0, 0x5f5f5f);
                radius = random_float_between(0.0f, 1.0f) * D_0015ED6C_lui;
                build_spherical_offset(position, radius, angle, random_angle_radians());
                radius = random_float_between(150000.0f, 200000.0f);
                alpha = FUN_L08_002dc9a8_random_color(60);
                alpha = random_int_between(alpha, FUN_L08_002dc9a8_random_color(120));
                FUN_L08_002dc9a8_emit(record.pos, position, color, color2, radius, alpha, 3);
            } break;
            case DETONATION_EFFECT_NO_EMISSION:
                break;
            }
        }
        mark_moby_for_removal(moby_bytes);
    } break;
    }
}
