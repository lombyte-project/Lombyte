/* Level 01 moby class destroyable_cave_wall. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/game/item_state.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"

#include "rnc/math/vector.h"
#include "rnc/overlay/l01/mobys/debris_class_choices.h"
#include "rnc/overlay/level_labels.h"

extern s32  FUN_0022da68(s32, s32, void *);
extern int  FUN_L00_00257b90(int lo, int hi);
extern void spawn_effect_particle_with_target_scalar(void *, void *, int, int, float, int) __asm__("FUN_L00_0026ced0");
extern struct Moby *
spawn_debris_chunk(void *, void *, s32, f32, s32, f32, f32, f32, s32) __asm__("FUN_L01_002f8530");

#include "rnc/overlay/level_save_state.h"

DebrisClassChoices l01_cave_wall_debris_oclass_choices __asm__("D_L01_00161C70") = {
    {0x2b8, 0x2b9, 0x2ba}
};

extern u8 D_0013D388[];

/* Removed once collected; when hit, bursts into 500 smoke puffs and 100 chunks, marks itself collected and sets D_0013D388[0xE] (an item-availability flag). */
void destroyable_cave_wall_update(struct Moby *m) __asm__("FUN_L01_002fa800");

/* Moby class 729 */
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
                Vec4 vel;
                Vec4 pos;
                Vec4 off;

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
            DebrisClassChoices oclass_choices;
            Vec4               vel;
            Vec4               pos;
            Vec4               off;

            oclass_choices = l01_cave_wall_debris_oclass_choices;
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
                               oclass_choices.oclass[random_integer_below(3)],
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

extern float fast_cos(float);
extern float fast_sin(float);
extern float random_float_between(float, float);
extern int   scale_game_frames(int);

#include "qcopy.h"
