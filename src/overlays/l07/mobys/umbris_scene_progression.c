/* Level 07 moby class 0x1b4: Umbris scene progression; starts the level's scenes as the hero enters its clip volumes and unlocks the next planet. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/overlay/shared/world/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/l07.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/save/memcard_save_data.h"

/* Update for moby class 436 on level 07: steps through its scenes as the hero enters its clip volumes. */
/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_002F6F20), where it is exact; names translated to the US level program. */
typedef struct {
    int open_volume;
    int finish_volume;
    int removed_moby;
    int moby_0c;
    int moby_10;
    int moby_14;
    int points_18;
    int points_1c;
} Class436Vars;

extern u8 umbris_open_volume_triggered[] __asm__("D_0013D3B1");

#include "rnc/overlay/shared/world/cuboid.h"

extern void start_scene(int) __asm__("FUN_L00_00298840");
extern void set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern void request_music_track(int, int) __asm__("FUN_L01_0027a248");
extern void queue_scene_movie_with_game_language(int) __asm__("FUN_L00_00299460");
extern void request_space_load(int) __asm__("FUN_0022e188");
void        umbris_scene_progression_update(struct Moby *moby) __asm__("FUN_L07_002f5ba0");

/* Moby class 436 */
void umbris_scene_progression_update(struct Moby *moby) {
    enum {
        CONTROLLER_WAIT_FOR_SCENE_IDLE = 0,
        CONTROLLER_START_INITIAL_SCENE = 1,
        CONTROLLER_WAIT_FOR_INITIAL_SCENE = 2,
        CONTROLLER_ADVANCE_TO_OPEN_VOLUME_WAIT = 3,
        CONTROLLER_WAIT_FOR_OPEN_VOLUME = 4,
        CONTROLLER_WAIT_FOR_FIRST_OPEN_SCENE = 5,
        CONTROLLER_WAIT_FOR_SECOND_OPEN_SCENE = 6,
        CONTROLLER_CLEAR_LINKED_MOBY_FLAGS = 7,
        CONTROLLER_WAIT_FOR_FINISH_VOLUME = 8,
        CONTROLLER_WAIT_FOR_FINISH_VOLUME_SCENE = 9,
        CONTROLLER_WAIT_TO_START_FINAL_SCENE = 10,
        CONTROLLER_WAIT_FOR_FINAL_SCENE_AND_UNLOCK_PLANET = 11,
        CONTROLLER_CLEANUP_LINKED_MOBY = 12,
        CONTROLLER_DELETE_SELF = 13
    };
    enum {
        SCENE_STATE_IDLE = 0,
        SCENE_STATE_ACTIVE = 2,
        MOBY_STATE_REMOVAL_BELOW_INSTANCE_END = 0xFD,
        MOBY_STATE_REMOVAL_AT_OR_ABOVE_INSTANCE_END = 0xFE
    };
    Class436Vars *scene_controller_vars = (Class436Vars *)moby->pvars;
    float         linked_moby_to_hero_offset[4];

    switch (moby->state) {
    case CONTROLLER_WAIT_FOR_SCENE_IDLE:
        if (umbris_open_volume_triggered[0] != 0 && scene_controller_vars->removed_moby != -1) {
            mark_moby_for_removal(
                &((struct Moby *)level_mobys)[scene_controller_vars->removed_moby]);
        }
        if (scene_state == SCENE_STATE_IDLE)
            moby->state = CONTROLLER_START_INITIAL_SCENE;
        break;
    case CONTROLLER_START_INITIAL_SCENE:
        if (alternate_item_available[0x28] == 0) {
            alternate_item_available[0x28] = 1;
            moby->state = CONTROLLER_WAIT_FOR_INITIAL_SCENE;
            start_scene(0);
        } else {
            moby->state = CONTROLLER_ADVANCE_TO_OPEN_VOLUME_WAIT;
        }
        break;
    case CONTROLLER_WAIT_FOR_INITIAL_SCENE:
        if (scene_state != SCENE_STATE_ACTIVE)
            moby->state = CONTROLLER_ADVANCE_TO_OPEN_VOLUME_WAIT;
        break;
    case CONTROLLER_ADVANCE_TO_OPEN_VOLUME_WAIT:
        moby->state = CONTROLLER_WAIT_FOR_OPEN_VOLUME;
        break;
    case CONTROLLER_WAIT_FOR_OPEN_VOLUME:
        if (alternate_item_available[0x29] == 0) {
            if (scene_controller_vars->open_volume != -1) {
                if (is_point_inside_clip_volume(&hero.motion.pos,
                                                scene_controller_vars->open_volume) != 0) {
                    alternate_item_available[0x29] = 1;
                    if (scene_controller_vars->moby_0c != -1) {
                        struct Moby *e =
                            &((struct Moby *)level_mobys)[scene_controller_vars->moby_0c];
                        e->flags |= 0x41;
                    }
                    if (scene_controller_vars->moby_14 != -1) {
                        struct Moby *e =
                            &((struct Moby *)level_mobys)[scene_controller_vars->moby_14];
                        e->flags |= 0x41;
                    }
                    moby->state = CONTROLLER_WAIT_FOR_FIRST_OPEN_SCENE;
                    start_scene(1);
                }
            } else {
                moby->state = CONTROLLER_WAIT_FOR_FINISH_VOLUME;
            }
        } else {
            moby->state = CONTROLLER_CLEAR_LINKED_MOBY_FLAGS;
        }
        if (moby->state != CONTROLLER_WAIT_FOR_OPEN_VOLUME &&
            scene_controller_vars->removed_moby != -1) {
            mark_moby_for_removal(
                &((struct Moby *)level_mobys)[scene_controller_vars->removed_moby]);
        }
        break;
    case CONTROLLER_WAIT_FOR_FIRST_OPEN_SCENE:
        if (scene_state == SCENE_STATE_ACTIVE)
            break;
        if (scene_controller_vars->moby_0c != -1) {
            struct Moby *e = &((struct Moby *)level_mobys)[scene_controller_vars->moby_0c];
            e->flags |= 0x41;
        }
        moby->state = CONTROLLER_WAIT_FOR_SECOND_OPEN_SCENE;
        start_scene(2);
        break;
    case CONTROLLER_WAIT_FOR_SECOND_OPEN_SCENE:
        if (scene_state == SCENE_STATE_ACTIVE)
            break;
        if (scene_controller_vars->points_18 != -1) {
            set_race_camera(&l07_cuboids[scene_controller_vars->points_18].pos,
                            &l07_cuboids[scene_controller_vars->points_18].rot,
                            0,
                            1);
        }
        moby->state = CONTROLLER_CLEAR_LINKED_MOBY_FLAGS;
        break;
    case CONTROLLER_CLEAR_LINKED_MOBY_FLAGS:
        if (scene_controller_vars->moby_0c != -1) {
            struct Moby *e = &((struct Moby *)level_mobys)[scene_controller_vars->moby_0c];
            e->flags &= 0xFFBE;
        }
        if (scene_controller_vars->moby_14 != -1) {
            struct Moby *e = &((struct Moby *)level_mobys)[scene_controller_vars->moby_14];
            e->flags &= 0xFFBE;
        }
        request_music_track(2, 4);
        moby->state = CONTROLLER_WAIT_FOR_FINISH_VOLUME;
        break;
    case CONTROLLER_WAIT_FOR_FINISH_VOLUME:
        if (alternate_item_available[0x2A] == 0) {
            struct Moby *e;
            if (scene_controller_vars->moby_0c == -1 ||
                (e = &((struct Moby *)level_mobys)[scene_controller_vars->moby_0c]) == 0 ||
                e->state == MOBY_STATE_REMOVAL_AT_OR_ABOVE_INSTANCE_END ||
                e->state == MOBY_STATE_REMOVAL_BELOW_INSTANCE_END) {
                if (scene_controller_vars->finish_volume != -1) {
                    if (is_point_inside_clip_volume(&hero.motion.pos,
                                                    scene_controller_vars->finish_volume) != 0) {
                        set_race_camera(&l07_cuboids[scene_controller_vars->points_1c].pos,
                                        &l07_cuboids[scene_controller_vars->points_1c].rot,
                                        0x72,
                                        1);
                        alternate_item_available[0x2A] = 1;
                        moby->state = CONTROLLER_WAIT_FOR_FINISH_VOLUME_SCENE;
                        start_scene(3);
                    }
                } else {
                    moby->state = CONTROLLER_DELETE_SELF;
                }
            }
        } else {
            moby->state = CONTROLLER_CLEANUP_LINKED_MOBY;
        }
        if (moby->state != CONTROLLER_WAIT_FOR_FINISH_VOLUME &&
            scene_controller_vars->moby_10 != -1) {
            struct Moby *e = &((struct Moby *)level_mobys)[scene_controller_vars->moby_10];
            e->flags |= 0x41;
        }
        break;
    case CONTROLLER_WAIT_FOR_FINISH_VOLUME_SCENE:
        if (scene_state == SCENE_STATE_ACTIVE)
            break;
        moby->state = CONTROLLER_WAIT_TO_START_FINAL_SCENE;
        queue_scene_movie_with_game_language(8);
        break;
    case CONTROLLER_WAIT_TO_START_FINAL_SCENE:
        if (scene_state == SCENE_STATE_ACTIVE)
            break;
        moby->state = CONTROLLER_WAIT_FOR_FINAL_SCENE_AND_UNLOCK_PLANET;
        start_scene(4);
    case CONTROLLER_WAIT_FOR_FINAL_SCENE_AND_UNLOCK_PLANET:
        if (scene_state == SCENE_STATE_ACTIVE)
            break;
        if (scene_controller_vars->points_1c != -1) {
            set_race_camera(&l07_cuboids[scene_controller_vars->points_1c].pos,
                            &l07_cuboids[scene_controller_vars->points_1c].rot,
                            0,
                            1);
        }
        if (scene_controller_vars->moby_10 != -1) {
            struct Moby *e = &((struct Moby *)level_mobys)[scene_controller_vars->moby_10];
            fast_vec_sub(linked_moby_to_hero_offset, &hero.motion.pos, &e->pos);
            clamp_vector_length(linked_moby_to_hero_offset, linked_moby_to_hero_offset, 0.9f);
            fast_vec_add(&e->pos, &hero.motion.pos, linked_moby_to_hero_offset);
            mark_moby_for_removal(e);
        }
        clear_bit_grid_rectangle(0x87, 0xC, 0x56, 0x3C);
        unlock_planet(8);
        memcard_save_data(0, 8);
        request_space_load(8);
        moby->state = CONTROLLER_CLEANUP_LINKED_MOBY;
        break;
    case CONTROLLER_CLEANUP_LINKED_MOBY:
        if (scene_controller_vars->moby_10 != -1) {
            struct Moby *e = &((struct Moby *)level_mobys)[scene_controller_vars->moby_10];
            if (e != 0 && e->state != MOBY_STATE_REMOVAL_AT_OR_ABOVE_INSTANCE_END &&
                e->state != MOBY_STATE_REMOVAL_BELOW_INSTANCE_END) {
                mark_moby_for_removal(e);
            }
        }
        moby->state = CONTROLLER_DELETE_SELF;
        break;
    case CONTROLLER_DELETE_SELF:
        mark_moby_for_removal(moby);
        break;
    }
}
