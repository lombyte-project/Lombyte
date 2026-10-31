/* Level 12 moby class homing_mine_spawner: spawns linked homing mines toward its target. */
#include "rnc/hero/hero.h"
#include "types.h"
#include "rnc/overlay/level_globals.h"
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

typedef struct Moby Moby;

/* Spawns the next linked moby at this one's spot, sets it up and gives it a random spin. */
extern void
spawn_jittered_effect_particle_with_optional_basis(char *, int, float) __asm__("FUN_L00_0026de90");

void spawn_next_linked_moby(struct Moby *spawner) __asm__("FUN_L12_003076e0");

/* Door/switch moby: clears its linked mobys, waits for the hero, opens, fires its list one by one, closes. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_00308C20), where it is exact; names translated to the US level program. */

void spawn_next_linked_moby(struct Moby *m) __asm__("FUN_L12_003076e0");

void spawn_next_linked_moby(struct Moby *spawner) {
    char *spawner_data = (char *)spawner->pvars;
    int  *indexed_spawner_data = (int *)(spawner_data + (*(int *)(spawner_data + 0xF0) << 2));
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
    spawned_moby->unk72 = 0xFF;
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
    enum {
        HOMING_MINE_SPAWNER_CLEAR_LINKED_MOBYS = 0,
        HOMING_MINE_SPAWNER_WAIT_FOR_PROXIMITY = 1,
        HOMING_MINE_SPAWNER_WAIT_FOR_PRESPAWN_ANIMATION = 2,
        HOMING_MINE_SPAWNER_SPAWN_LINKED_MOBYS = 3,
        HOMING_MINE_SPAWNER_WAIT_FOR_POSTSPAWN_ANIMATION = 4,
        HOMING_MINE_SPAWNER_SPAWN_SEQUENCE_COMPLETE = 5
    };
    char *spawner_data = (char *)spawner->pvars;
    int  *slots;
    int   i;
    update_homing_mine_spawner_target(spawner);
    slots = (int *)(spawner_data + 0xC0);
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
        spawner->state = 1;
        break;
    case HOMING_MINE_SPAWNER_WAIT_FOR_PROXIMITY:
        if (*(int *)(spawner_data + 0xB4) != 2) {
            if (vector_distance_xy(&spawner->pos, spawner_data + 0x70) <
                *(float *)(spawner_data + 0xF8)) {
                spawner->state = 2;
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
            spawner->state = 3;
        }
        break;
    case HOMING_MINE_SPAWNER_SPAWN_LINKED_MOBYS:
        if (fast_dec_timer_s16(spawner_data + 0xFC)) {
            if (slots[*(int *)(spawner_data + 0xF0)] != -1) {
                spawn_next_linked_moby(spawner);
                spawner->command = spawner->command == 0;
                *(short *)(spawner_data + 0xFC) = scale_game_frames(0x28);
            }
            *(int *)(spawner_data + 0xF0) += 1;
        }
        if (*(int *)(spawner_data + 0xF0) == 12 || slots[*(int *)(spawner_data + 0xF0)] == -1) {
            spawner->state = 4;
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
            spawner->state = 5;
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
    char *spawner_data = (char *)spawner->pvars;
    char *pad;
    if (spawner->state != 0) {
        if (*(int *)(spawner_data + 0x38) != 0) {
            *(short *)(spawner_data + 0xFE) = scale_game_frames(0xB4);
        }
        *(int *)(spawner_data + 0x38) = 0;
        if (fast_dec_timer_s16(spawner_data + 0xFE)) {
            *(float *)(spawner_data + 0xF8) = *(float *)(spawner_data + 0xF4);
        } else {
            *(float *)(spawner_data + 0xF8) = *(float *)(spawner_data + 0xF4) + 5.0f;
        }
        if (select_moby_combat_target(
                spawner, spawner_data + 0x70, *(float *)(spawner_data + 0xF8)) != 2) {
            if (absolute_float(spawner->pos.z - *(float *)(spawner_data + 0x78)) > 3.0f) {
                *(int *)(spawner_data + 0xB4) = 2;
            }
        }
        if (*(int *)(spawner_data + 0xB0) == 0) {
            pad = D_0013E533 + 0xE1D;
            *(int *)(spawner_data + 0xB0) = *(int *)(pad + 0x2080);
            qcopy(spawner_data + 0x70, pad + 0x80);
        }
    }
}
