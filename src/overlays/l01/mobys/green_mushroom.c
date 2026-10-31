/* Level 01 moby class green_mushroom. */
#include "types.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"

extern s32 FUN_0022da68(s32, s32, void *);

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

/* Moby class 754 */
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

extern float fast_cos(float);
extern float fast_sin(float);

#include "qcopy.h"
