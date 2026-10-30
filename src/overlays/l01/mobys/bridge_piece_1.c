/* Level 01 moby class bridge_piece_1. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/game/item_state.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/level_labels.h"

extern int FUN_L00_00257b90(int lo, int hi);

#include "rnc/math/vector.h"

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
    u8   pad0[0x10];
    Vec4 pos;
    u8   pad20[0x84];
    u8   bA4;
    u8   padA5[0xD];
    s16  id;
} BurstMoby;

extern s32 pal_mode_macro[] __asm__("D_0015ED80") MACRO_ADDR;

#include "rnc/overlay/level_save_state.h"

extern BurstColors l01_bridge_piece_debris_oclass_choices __asm__("D_L01_00161C60");
extern void        FUN_L01_002f8530(void *, void *, u32, s32, f32, f32, f32, f32, s32);

void bridge_piece_1_update(BurstMoby *self) __asm__("FUN_L01_002f9d80");

void bridge_piece_1_update(BurstMoby *self) {
    BurstTarget *hit_result;
    BurstColors  colors;
    Vec4         debris_velocity;
    Vec4         pos;
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

extern float fast_cos(float);
extern float fast_sin(float);
extern float random_float_between(float, float);

#include "qcopy.h"
