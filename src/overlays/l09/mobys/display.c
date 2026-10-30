/* Level 09 moby classes 0x114, 0x512, 0x513: display. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/overlay/shared/hero/copy_hero_moby_field_38.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "qcopy.h"
#include "sda.h"
#include "rnc/game/game_globals.h"

extern int FUN_L00_00257b90(int, int);

/* spawns a moby of the given class at a source moby's position */
/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_002EEEB0), where it is exact; names translated to the US level program. */
extern s32   rand();
char        *spawn_breakable_stack_replacement(char *source_moby,
                                               int   spawned_moby_class) __asm__("FUN_L09_002edb60");
extern float D_L09_00166F40_edc30[] __asm__("D_L09_00166F40");
extern int   FUN_L00_001fefc8(u8 *);
extern void  mark_moby_for_removal_cf(struct Moby *, int, int) __asm__("FUN_L00_00257470");
extern void  add_random_offset_to_vector_xyz(void *, float) __asm__("FUN_L00_0025f8e0");
extern void  scale_vector_edc30(void *, void *, float) __asm__("FUN_001f9a68");
extern struct Moby *
spawn_tumbling_debris(struct Moby *, Vec4 *, Vec4 *, s32, f32) __asm__("FUN_L09_002ef868");
extern void spawn_explosion(struct Moby *,
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

/* Level 9 breakable stack: when hit, swaps in the next damaged class (0x512 -> 0x513 -> 0x514), bursts debris and smoke and removes itself once gone. */
void display_update(struct Moby *moby) __asm__("FUN_L09_002edc30");

struct DebrisVars {
    Vec4         vel;    /* 0x00 */
    f32          spin;   /* 0x10 */
    f32          tumble; /* 0x14 */
    struct Moby *src;    /* 0x18 */
    s32          life;   /* 0x1C */
};

char *spawn_breakable_stack_replacement(char *source_moby, int spawned_moby_class) {
    char *moby = create_moby(spawned_moby_class);
    if (moby != 0) {
        ((unsigned char *)moby)[0x30] = 0xFF;
        MOBY(moby)->draw_dist = 0xFF;
        moby[0x31] = 1;
        moby[0x20] = 0;
        qcopy(moby + 0x10, source_moby + 0x10);
        qcopy(moby + 0x40, source_moby + 0x40);
        moby[0xBC] = scale_game_frames(10);
        update_entity(moby);
        copy_hero_moby_field_38(moby);
        MOBY(moby)->flags = MOBY(source_moby)->flags;
        if (spawned_moby_class == 0x514) {
            if (rand() & 1) {
                MOBY(moby)->flags |= 0x8000;
            }
        }
    }
    return moby;
}

void display_update(struct Moby *moby) {
    enum {
        BREAKABLE_STACK_ANTEPENULTIMATE_CLASS = 0x512,
        BREAKABLE_STACK_PENULTIMATE_CLASS = 0x513
    };
    float center[4];
    float top[4];
    float push[4];
    float pos[4];
    float debris_offset_or_velocity[4];
    float pull[4];
    char *hit;
    int   replaced;
    int   next;
    int   i;
    float spread;
    float falloff;
    if (moby->mission != 0xFF && visited52[moby->mission + current_level_index * 16] == 0xFF) {
        mark_moby_for_removal(moby);
        return;
    }
    replaced = 0;
    hit = find_moby_hit(moby, 0x40000, 0);
    if (moby->state == 0) {
        moby->state = 1;
    }
    if (FUN_L00_001fefc8(&moby->command) == 0 || hit == 0) {
        return;
    }
    if (moby->oclass < 0x514) {
        next = 0x512;
        switch (moby->oclass) {
        case BREAKABLE_STACK_ANTEPENULTIMATE_CLASS:
            next = 0x513;
            break;
        case BREAKABLE_STACK_PENULTIMATE_CLASS:
            qcopy(center, &moby->pos);
            moby->pos.z += 4.0f;
            mark_moby_for_removal_cf(moby, 0, -1);
            qcopy(&moby->pos, center);
            next = 0x514;
            break;
        }
        replaced = spawn_breakable_stack_replacement((char *)moby, next) != 0;
    }
    moby->command = scale_game_frames(10);
    qcopy(top, &moby->pos);
    top[2] += 1.25f;
    qcopy(center, top);
    add_random_offset_to_vector_xyz(center, 0.4f);
    if (moby->oclass < 0x513) {
        allocate_voice_for_target_entry(0, 0, moby);
    } else {
        allocate_voice_for_target_entry(0, 0, moby);
    }
    moby->hit_slot = 0xFF;
    fast_vec_sub(push, D_L09_00166F40_edc30, center);
    fast_vec_normalize(push, push, 2.0f);
    spread = 4.0f;
    if (moby->oclass != 0x114) {
        spread = 2.0f;
    }
    for (i = 0; i < 20; i++) {
        *(u128 *)debris_offset_or_velocity = 0;
        debris_offset_or_velocity[0] = random_float_between(-0.5f, 0.5f);
        debris_offset_or_velocity[1] = random_float_between(-2.0f, 2.0f);
        debris_offset_or_velocity[2] = random_float_between(-spread, spread);
        *(u128 *)pos = *(u128 *)debris_offset_or_velocity;
        transform_vector_by_basis(pos, pos, &moby->rot_row0);
        fast_vec_add(pos, pos, top);
        fast_vec_sub(debris_offset_or_velocity, pos, center);
        falloff = absolute_float(3.0f - fast_vec_length(debris_offset_or_velocity)) / 3.0f;
        fast_vec_normalize(pull, hit + 0x10, falloff * falloff * 8.0f);
        fast_vec_add(debris_offset_or_velocity, debris_offset_or_velocity, pull);
        fast_vec_normalize(debris_offset_or_velocity,
                           debris_offset_or_velocity,
                           falloff * (random_float_between(6.0f, 10.0f) * frame_time));
        fast_vec_add(pos, pos, push);
        if (moby->oclass == 0x514 || random_integer_below(4) == 0) {
            spawn_tumbling_debris(moby,
                                  (Vec4 *)pos,
                                  (Vec4 *)debris_offset_or_velocity,
                                  i & 1,
                                  random_float_between(1.0f, 2.0f));
        }
        if (i % 3 == 0) {
            cart_burst(moby, pos, -1, random_float_between(1.0f, 1.6f), 0.0f);
        }
    }
    scale_vector_edc30(pos, hit + 0x10, 0.5f);
    fast_vec_add(center, center, push);
    spawn_explosion(moby,
                    hit + 0x10,
                    center,
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
    if (moby->oclass == 0x514 || replaced) {
        mark_moby_for_removal(moby);
    }
}

/* spawns a debris moby of class 0x140 + kind at pos with velocity vel and a random spin */
struct Moby *spawn_tumbling_debris(struct Moby *src, Vec4 *pos, Vec4 *vel, s32 kind, f32 scale) {
    Vec4               position_copy;
    Vec4               velocity_copy;
    Vec4              *pp = &position_copy;
    Vec4              *velocity_copy_source = &velocity_copy;
    struct Moby       *spawned_moby;
    struct DebrisVars *debris_vars;

    position_copy.q = pos->q;
    velocity_copy.q = vel->q;
    spawned_moby = create_moby(kind + 0x140);
    if (spawned_moby != 0) {
        debris_vars = (struct DebrisVars *)spawned_moby->pvars;
        debris_vars->src = src;
        spawned_moby->update_dist = 0x7F;
        spawned_moby->draw_dist = 0x7F;
        spawned_moby->visible = 1;
        spawned_moby->spawn_frame = src->spawn_frame;
        spawned_moby->rot.x = random_angle_radians();
        spawned_moby->rot.y = random_angle_radians();
        spawned_moby->rot.z = random_angle_radians();
        qcopy(&spawned_moby->pos, pp);
        qcopy(&debris_vars->vel, velocity_copy_source);
        spawned_moby->scale = spawned_moby->pclass->scale * scale;
        debris_vars->spin = random_float_between(-(frame_time * 9.075712f), frame_time * 9.075712f);
        debris_vars->tumble =
            random_float_between(-(frame_time * 6.2831855f), frame_time * 6.2831855f);
        debris_vars->life = scale_game_frames(FUN_L00_00257b90(0x5A, 0x6E));
        update_entity(spawned_moby);
    }
    return spawned_moby;
}
