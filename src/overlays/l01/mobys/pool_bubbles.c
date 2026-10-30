/* Level 01 pool bubble particles: pool_bubble and pool_bubbles, which pool_tiles_and_effects spawns. */
#include "types.h"
#include "asm.h"

#include "eetypes.h"
#include "qcopy.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/random.h"
#include "rnc/overlay/shared/rendering/effects/create_part.h"

typedef struct {
    u128 pos;
    f32  size;
    f32  speed;
} PartMotion;

typedef struct {
    u8         pad0;
    u8         active;
    u8         tex;
    u8         kind;
    u32        color;
    u8         spin;
    u8         alpha;
    s16        life;
    f32        scale;
    u128       pos;
    PartMotion m;
} Particle;

extern u8 *pool_bubble_texture_table[] __asm__("D_L01_001B25E0");

Particle *pool_bubble(u128 *pos, u128 *vel, f32 f) __asm__("FUN_L01_00287b00");

Particle *pool_bubble(u128 *pos, u128 *motion_position_source, f32 scale) {
    Particle *particle;
    s32       gray_channel;
    u32       col;

    particle = create_part(0x38);
    if (particle != 0) {
        qcopy(&particle->pos, pos);
        gray_channel = random_integer_below(0x10) * 2 + 0x60;
        col = (gray_channel << 8) | 0x40000000;
        particle->color = (gray_channel << 16) | col | gray_channel;
        particle->alpha = truncate_float_to_s32(4.0f) + 0x20;
        particle->kind = 0x48;
        particle->active = 0;
        particle->tex = *pool_bubble_texture_table[0];
        particle->scale = scale;
        qcopy(&particle->m.pos, motion_position_source);
        particle->spin = random_integer_below(0x100);
    }
    return particle;
}

extern u8 *pool_bubbles_texture_table[] __asm__("D_L01_001B25E4");

Particle *pool_bubbles(u128 *pos, u128 *dir, f32 scale, f32 speed) __asm__("FUN_L01_00287c80");

Particle *pool_bubbles(u128 *pos, u128 *motion_position_source, f32 scale, f32 speed) {
    Particle   *particle;
    PartMotion *m;
    f32         random_spin;

    particle = create_part(0x39);
    if (particle != NULL) {
        qcopy(&particle->pos, pos);
        m = &particle->m;
        particle->color = (random_integer_below(0x10) << 25) | 0x808080;
        particle->alpha = truncate_float_to_s32(12.0f) + 0x20;
        particle->kind = 0x48;
        particle->active = 1;
        particle->tex = *pool_bubbles_texture_table[0];
        particle->life = 0x80;
        particle->scale = scale * 210000.0f;
        qcopy(&m->pos, motion_position_source);
        random_spin = random_float_between(0.0f, 256.0f);
        m->size = random_spin;
        particle->spin = truncate_float_to_s32(random_spin);
        m->speed = speed;
    }
    return particle;
}
