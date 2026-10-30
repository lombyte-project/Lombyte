/* Level 09 moby classes 0x505, 0x506, 0x507, 0x508: barrier. */
#include "types.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/fast_math.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/math/random.h"
#include "rnc/game/item_state.h"
#include "rnc/game/game_globals.h"
#include "qcopy.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/overlay/shared/hero/copy_hero_moby_field_38.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/mobys/moby.h"

typedef struct Moby Moby;

#include "rnc/overlay/l09/mobys/barrier_globals.h"

/* spawn a moby copying position, rotation and data word from owner */
/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_003095D0), where it is exact; names translated to the US level program. */
char *spawn_barrier_replacement(struct Moby *owner,
                                int          spawned_moby_class) __asm__("FUN_L09_00308220");

/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_00309688), where it is exact; names translated to the US level program. */
typedef struct BarrierData {
    int idx;
} BarrierData;

typedef struct Hit_309688 {
    char  pad0[0x10];
    float pos[4];
} Hit_309688;

typedef struct Flags_309688 {
    char          pad0[0x43];
    unsigned char f43;
} Flags_309688;

#include "rnc/overlay/l09/mobys/barrier_data/barrier_update.h"

struct Moby *spawn_tumbling_debris(
    struct Moby *src, Vec4 *pos, Vec4 *vel, s32 kind, f32 scale) __asm__("FUN_L09_002ef868");
extern int   func_001F9850_309688(int) __asm__("FUN_001f96f8");
extern float func_001F9F90_309688(float) __asm__("FUN_001f9dc8");
extern float func_001F9FA8_309688(float) __asm__("FUN_001f9de0");
extern float func_001F9B88_309688(float) __asm__("FUN_001f99c0");
extern void  func_L00_001FF4B0_309688(void *, void *, float) __asm__("FUN_001f9bf8");
extern void  func_L00_0025F4A8_309688(void *,
                                      void *,
                                      void *,
                                      float,
                                      float,
                                      int,
                                      int,
                                      int,
                                      float,
                                      float,
                                      float,
                                      float,
                                      int,
                                      float,
                                      int,
                                      int,
                                      int,
                                      int) __asm__("FUN_L00_0025e450");
void         barrier_update(Moby *moby) __asm__("FUN_L09_003082d8");

char *spawn_barrier_replacement(struct Moby *owner, int spawned_moby_class) {
    char *moby = create_moby(spawned_moby_class);
    if (moby != 0) {
        unsigned char state;
        ((unsigned char *)moby)[0x30] = 0xFF;
        state = 0xFF;
        *(short *)(moby + 0x32) = state;
        moby[0x31] = 1;
        moby[0x20] = 0;
        qcopy(moby + 0x10, &owner->pos);
        qcopy(moby + 0x40, &owner->rot);
        moby[0xBC] = scale_game_frames(10);
        update_entity(moby);
        copy_hero_moby_field_38(moby);
        *(float *)(moby + 0x2C) = owner->scale;
        *(unsigned short *)(moby + 0x34) = owner->flags;
        **(int **)(moby + 0x78) = *(int *)owner->pvars;
    }
    return moby;
}

void barrier_update(Moby *moby) {
    BarrierData *data = (*(BarrierData **)&moby->pvars);
    float        origin[4], center[4], piece_pos[4], piece_velocity[4], push[4];
    Hit_309688  *hit;
    int          has_next;
    int          count;
    int          i;
    float        angle;
    float        scale;

    if (moby->state == 0) {
        if (data->idx != -1)
            ((Flags_309688 *)(alternate_item_available + data->idx))->f43 = 0;
        moby->state = 1;
    }
    if (moby->mission != 0xFF) {
        int row = current_level_index * 16;
        if (visited52[moby->mission + row] == 0xFF) {
            if (data->idx != -1)
                ((Flags_309688 *)(alternate_item_available + data->idx))->f43 = 1;
            mark_moby_for_removal(moby);
            return;
        }
    }
    has_next = 0;
    hit = find_moby_hit(moby, 0x40000, 0);
    if (fast_subtract_rotations_cf(&moby->command) == 0)
        return;
    if (hit == 0)
        return;
    if (moby->oclass < 0x508)
        has_next = spawn_barrier_replacement(moby, moby->oclass + 1) != 0;
    moby->command = func_001F9850_309688(10);
    qcopy(origin, ((f32 *)&moby->pos));
    qcopy(center, ((f32 *)&moby->pos));
    if (moby->oclass < 0x508) {
        allocate_voice_for_target_entry(0, 0, moby);
    } else {
        allocate_voice_for_target_entry(0, 0, moby);
    }
    moby->hit_slot = 0xFF;
    count = 40;
    if (0.85f < radial_effect_count_reduction_control ||
        0.85f < barrier_radial_effect_count_reduction_control)
        count = 30;
    angle = -3.1415927f;
    for (i = 0; i < count; i++) {
        *(u128 *)piece_velocity = 0;
        piece_velocity[0] = random_float_between(-0.4f, 0.4f);
        piece_velocity[1] = random_float_between(-0.6f, 0.6f);
        piece_velocity[2] = random_float_between(-0.4f, 0.4f);
        *(u128 *)piece_pos = *(u128 *)piece_velocity;
        scale = random_float_between(0.3f, 2.6f);
        piece_pos[0] = piece_pos[0] + scale * func_001F9F90_309688(angle);
        scale = scale * func_001F9FA8_309688(angle);
        piece_pos[2] = piece_pos[2] + scale;
        transform_vector_by_basis(piece_pos, piece_pos, ((f32 *)&moby->rot_row0));
        fast_vec_add(piece_pos, piece_pos, center);
        fast_vec_sub(piece_velocity, piece_pos, origin);
        scale = func_001F9B88_309688(6.0f - fast_vec_length(piece_velocity)) / 6.0f;
        func_L00_001FF4B0_309688(push, hit->pos, scale * scale * 8.0f);
        fast_vec_add(piece_velocity, piece_velocity, push);
        func_L00_001FF4B0_309688(piece_velocity,
                                 piece_velocity,
                                 scale * (random_float_between(5.0f, 8.0f) * frame_time));
        if (moby->oclass == 0x508) {
            int odd = i & 1;
            spawn_tumbling_debris(
                moby, piece_pos, piece_velocity, odd, random_float_between(1.0f, 1.3f));
        }
        if (i % 3 == 0) {
            cart_burst(moby, piece_pos, -1, random_float_between(0.3f, 0.6f), 0.0f);
        }
        angle = angle + 6.2831855f / convert_integer_to_float(count);
    }
    fast_vec_scale(piece_pos, hit->pos, 0.5f);
    func_L00_0025F4A8_309688(moby,
                             hit->pos,
                             origin,
                             0.0f,
                             0.0f,
                             10,
                             3,
                             16,
                             3.0f,
                             2.0f,
                             1.0f,
                             1.0f,
                             -1,
                             15.0f,
                             1,
                             1,
                             -1,
                             0);
    if (moby->oclass == 0x508) {
        if (data->idx != -1)
            ((Flags_309688 *)(alternate_item_available + data->idx))->f43 = 1;
    }
    if (moby->oclass == 0x508 || has_next)
        mark_moby_for_removal(moby);
}
