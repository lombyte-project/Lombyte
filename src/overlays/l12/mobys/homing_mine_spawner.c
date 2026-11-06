/* Level 12 moby class homing_mine_spawner: spawns linked homing mines toward its target. */
#include "rnc/hero/hero.h"
#include "types.h"
#include "rnc/overlay/shared/world/level_globals.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"

#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/mobys/get_moby_target_info.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/overlay/shared/rendering/effects/update_three_phase_effect_particle.h"

enum {
    HOMING_MINE_SPAWNER_CLEAR_LINKED_MOBYS = 0,
    HOMING_MINE_SPAWNER_WAIT_FOR_PROXIMITY = 1,
    HOMING_MINE_SPAWNER_WAIT_FOR_PRESPAWN_ANIMATION = 2,
    HOMING_MINE_SPAWNER_SPAWN_LINKED_MOBYS = 3,
    HOMING_MINE_SPAWNER_WAIT_FOR_POSTSPAWN_ANIMATION = 4,
    HOMING_MINE_SPAWNER_SPAWN_SEQUENCE_COMPLETE = 5
};

typedef struct Moby Moby;

/* Spawns the next linked moby at this one's spot, sets it up and gives it a random spin. */

void spawn_next_linked_moby(struct Moby *spawner) __asm__("FUN_L12_003076e0");

/* Door/switch moby: clears its linked mobys, waits for the hero, opens, fires its list one by one, closes. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_00308C20), where it is exact; names translated to the US level program. */

void spawn_next_linked_moby(struct Moby *m) __asm__("FUN_L12_003076e0");

typedef struct HomingMineSpawnerVars {
    u8  pad_0[0x38];
    s32 unk38;
    u8  pad_3C[0x34];
    u8  unk70;
    u8  pad_71[0x7];
    f32 unk78;
    u8  pad_7C[0x34];
    s32 unkB0;
    s32 unkB4;
    u8  pad_B8[0x8];
    s32 unkC0;
    u8  pad_C4[0x2C];
    s32 unkF0;
    f32 unkF4;
    f32 unkF8;
    s16 timer_FC;
    s16 timer_FE;
} HomingMineSpawnerVars;

void spawn_next_linked_moby(struct Moby *spawner) {
    HomingMineSpawnerVars *spawner_data = (HomingMineSpawnerVars *)spawner->pvars;
    int         *indexed_spawner_data = (int *)((u8 *)spawner_data + (spawner_data->unkF0 << 2));
    struct Moby *spawned_moby =
        (struct Moby *)((unsigned char *)D_L12_0015FFD8_p + (indexed_spawner_data[0x30] << 8));
    struct MobyClass *moby_class;
    short            *aim_height_data;
    int               i;
    qcopy(&spawned_moby->pos, &spawner->pos);
    spawned_moby->pos.z -= 0.35f;
    spawned_moby->state = 0;
    spawned_moby->command = 0;
    moby_class = spawned_moby->pclass;
    spawned_moby->flags = moby_class->flags;
    spawned_moby->scale = moby_class->scale;
    spawned_moby->update_dist = 0xFF;
    spawned_moby->visible = 1;
    spawned_moby->occlusion = 0x7F80;
    spawned_moby->draw_dist = 0xFF;
    spawned_moby->unk71 = 0xFF;
    spawned_moby->lod_dist = 0xFF;
    spawned_moby->hit_slot = 0xFF;
    spawned_moby->collision = moby_class->collision;
    spawned_moby->spawn_frame = spawner->spawn_frame;
    update_entity(spawned_moby);
    spawned_moby->parent = spawner;
    for (i = 0; i < 14; i++) {
        spawn_jittered_effect_particle_with_optional_basis(
            &spawner->pos, 0, random_float_between(frame_time * 3.0f, frame_time * 5.0f));
    }
    aim_height_data = get_moby_target_info(spawned_moby);
    if (aim_height_data != 0) {
        *(float *)aim_height_data = aim_height_data[2];
    }
}

void update_homing_mine_spawner_target(struct Moby *spawner) __asm__("FUN_L12_00307ab8");

extern void update_homing_mine_spawner_target(struct Moby *m) __asm__("FUN_L12_00307ab8");

void homing_mine_spawner_update(struct Moby *m) __asm__("FUN_L12_00307840");

/* Moby class 1281 */
void homing_mine_spawner_update(struct Moby *spawner) {
    HomingMineSpawnerVars *spawner_data = (HomingMineSpawnerVars *)spawner->pvars;
    int                   *slots;
    int                    i;
    update_homing_mine_spawner_target(spawner);
    slots = &spawner_data->unkC0;
    switch (spawner->state) {
    case HOMING_MINE_SPAWNER_CLEAR_LINKED_MOBYS:
        for (i = 0; i < 12; i++) {
            if (slots[i] != -1) {
                char *linked_moby = (D_L12_0015FFD8_p + (slots[i] << 8));
                if (linked_moby != 0 && ((unsigned char *)linked_moby)[0x20] != 0xFE &&
                    ((unsigned char *)linked_moby)[0x20] != 0xFD) {
                    mark_moby_for_removal(linked_moby);
                }
            }
        }
        spawner->state = HOMING_MINE_SPAWNER_WAIT_FOR_PROXIMITY;
        break;
    case HOMING_MINE_SPAWNER_WAIT_FOR_PROXIMITY:
        if (spawner_data->unkB4 != 2) {
            if (vector_distance_xy(&spawner->pos, &spawner_data->unk70) < spawner_data->unkF8) {
                spawner->state = HOMING_MINE_SPAWNER_WAIT_FOR_PRESPAWN_ANIMATION;
            }
        }
        break;
    case HOMING_MINE_SPAWNER_WAIT_FOR_PRESPAWN_ANIMATION:
        if (spawner->prev_seq != 1) {
            blend_moby_animation(spawner, 1, 0, scale_game_frames(2));
        }
        if (spawner->anim_flags & 2) {
            if (spawner->prev_seq != 2) {
                blend_moby_animation(spawner, 2, 0, scale_game_frames(2));
            }
            spawner->state = HOMING_MINE_SPAWNER_SPAWN_LINKED_MOBYS;
        }
        break;
    case HOMING_MINE_SPAWNER_SPAWN_LINKED_MOBYS:
        if (fast_dec_timer_s16(&spawner_data->timer_FC)) {
            if (slots[spawner_data->unkF0] != -1) {
                spawn_next_linked_moby(spawner);
                spawner->command = spawner->command == 0;
                spawner_data->timer_FC = scale_game_frames(0x28);
            }
            spawner_data->unkF0 += 1;
        }
        if (spawner_data->unkF0 == 12 || slots[spawner_data->unkF0] == -1) {
            spawner->state = HOMING_MINE_SPAWNER_WAIT_FOR_POSTSPAWN_ANIMATION;
        }
        break;
    case HOMING_MINE_SPAWNER_WAIT_FOR_POSTSPAWN_ANIMATION:
        spawner->flags &= 0xEFFF;
        if (spawner->prev_seq != 3) {
            blend_moby_animation(spawner, 3, 0, scale_game_frames(2));
        }
        if (spawner->anim_flags & 2) {
            if (spawner->prev_seq != 0) {
                blend_moby_animation(spawner, 0, 0, scale_game_frames(2));
            }
            spawner->state = HOMING_MINE_SPAWNER_SPAWN_SEQUENCE_COMPLETE;
        }
        break;
    case HOMING_MINE_SPAWNER_SPAWN_SEQUENCE_COMPLETE:
        break;
    }
}

/* per-frame update of a moby's timer and approach state */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_00308E98), where it is exact; names translated to the US level program. */

extern char D_0013E533[];
extern int  select_moby_combat_target(char *, char *, float) __asm__("FUN_L00_0025fcb8");

void update_homing_mine_spawner_target(struct Moby *spawner) {
    HomingMineSpawnerVars *spawner_data = (HomingMineSpawnerVars *)spawner->pvars;
    char                  *pad;
    if (spawner->state != HOMING_MINE_SPAWNER_CLEAR_LINKED_MOBYS) {
        if (spawner_data->unk38 != 0) {
            spawner_data->timer_FE = scale_game_frames(0xB4);
        }
        spawner_data->unk38 = 0;
        if (fast_dec_timer_s16(&spawner_data->timer_FE)) {
            spawner_data->unkF8 = spawner_data->unkF4;
        } else {
            spawner_data->unkF8 = spawner_data->unkF4 + 5.0f;
        }
        if (select_moby_combat_target(spawner, &spawner_data->unk70, spawner_data->unkF8) != 2) {
            if (absolute_float(spawner->pos.z - spawner_data->unk78) > 3.0f) {
                spawner_data->unkB4 = 2;
            }
        }
        if (spawner_data->unkB0 == 0) {
            pad = D_0013E533 + 0xE1D;
            spawner_data->unkB0 = *(int *)(pad + 0x2080);
            qcopy(&spawner_data->unk70, pad + 0x80);
        }
    }
}
