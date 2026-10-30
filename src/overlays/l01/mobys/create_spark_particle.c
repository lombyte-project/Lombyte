/* Level 01 spark particle: create_spark_particle, which spark_particle_emitter spawns its sparks with. */
#include "types.h"
#include "asm.h"

#include "eetypes.h"
#include "qcopy.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/random.h"
#include "rnc/overlay/shared/rendering/effects/create_part.h"

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    s32 id;
    f32 f10;
    f32 f14;
    u8  r;
    u8  g;
    u8  b;
    u8  b1B;
} SparkBody;

typedef struct {
    u8        b0;
    u8        b1;
    u8        b2;
    u8        b3;
    s32       w4;
    u8        b8;
    u8        b9;
    s16       hA;
    f32       fC;
    u128      pos;
    SparkBody body;
} Spark;

extern s32 FUN_L00_0025bfe0(f32, f32, f32, f32);
extern u8 *spark_texture_table[] __asm__("D_L01_001B2500");

void create_spark_particle(s32   id,
                           u128 *pos,
                           s32   life,
                           u8    kind,
                           u8    texture_table_index,
                           u8    mode,
                           f32   x,
                           f32   y,
                           f32   z,
                           f32   w,
                           f32   alpha,
                           f32   r,
                           f32   g,
                           f32   b,
                           f32   size) __asm__("FUN_L01_0027e900");

void create_spark_particle(s32   id,
                           u128 *pos,
                           s32   life,
                           u8    kind,
                           u8    texture_table_index,
                           u8    mode,
                           f32   x,
                           f32   y,
                           f32   z,
                           f32   w,
                           f32   alpha,
                           f32   r,
                           f32   g,
                           f32   b,

                           f32 size) {
    Spark     *spark;
    SparkBody *spark_body;
    u128       position_snapshot = *pos;
    u128      *position_copy_source = &position_snapshot;

    if (id == 0) {
        return;
    }
    spark = create_part(6);
    if (spark == 0) {
        return;
    }
    spark_body = &spark->body;
    qcopy(&spark->pos, position_copy_source);
    if (mode == 0xFF) {
        spark->b9 = (kind >> 5) * 16 + truncate_float_to_s32(4.0f);
        if (kind & 1) {
            spark->b3 = 0x48;
        } else {
            spark->b3 = 0x44;
        }
    } else {
        spark->b9 = kind;
        spark->b3 = mode;
    }
    spark->b1 = 0;
    spark->b2 = *spark_texture_table[texture_table_index];
    spark_body->x = x;
    spark_body->y = y;
    spark_body->z = z;
    spark_body->f14 = alpha;
    spark_body->r = truncate_float_to_s32(r * 65535.0f);
    spark_body->g = truncate_float_to_s32(g * 65535.0f);
    spark_body->b = truncate_float_to_s32(b * 65535.0f);
    spark_body->f10 = size;
    spark_body->id = id;
    spark->fC = w;
    spark->b8 = alpha * 255.0f;
    spark->hA = scale_game_frames(life);
    spark_body->b1B = 0;
    spark->w4 = FUN_L00_0025bfe0(r, g, b, size);
}
