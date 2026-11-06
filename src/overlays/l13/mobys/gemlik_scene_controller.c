/* Level 13 Gemlik scene controller moby class. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/save/memcard_save_data.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/world/cuboid.h"
#include "rnc/game/item_state.h"
#include "rnc/overlay/shared/world/level_labels.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/game.h"

extern int D_L13_0015F5C4;

extern unsigned char D_001413F5 NOT_SDA;

extern float D_L13_0015F3FC;

extern int D_L13_0015F404;

extern struct {
    char  pad0[0x60];
    float hero_start_x, hero_start_y, hero_start_z; /* LevelRenderState.hero_start_pos */
    char  pad6C[0xC];
    float hero_start_yaw; /* LevelRenderState.hero_start_rot z */
} D_0013E030;

extern unsigned char D_L13_0015FC88[] MACRO_ADDR;

extern char D_0013F3D0[];

extern void set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");

extern void request_music_track(int, int) __asm__("FUN_L01_0027a248");

extern void start_scene(int) __asm__("FUN_L00_00298840");

extern void queue_scene_movie_with_game_language(int) __asm__("FUN_L00_00299460");

void gemlik_scene_controller_update(struct Moby *moby) __asm__("FUN_L13_0030b628");

/* Moby class 1353 */
void gemlik_scene_controller_update(struct Moby *moby) {
    enum {
        WAIT_TO_START_INITIAL_SCENE = 0,
        START_INITIAL_SCENE = 1,
        WAIT_INITIAL_SCENE = 2,
        ENTER_CLIP_VOLUME_CHECK = 3,
        CHECK_CLIP_VOLUME_FOR_SCENE = 4,
        WAIT_CLIP_VOLUME_SCENE = 5,
        ENTER_LINKED_MOBY_REMOVAL_WAIT = 6,
        WAIT_LINKED_MOBY_REMOVAL_BEFORE_SCENE = 7,
        WAIT_SCENE_BEFORE_FOLLOWUP = 8,
        START_FOLLOWUP_SCENE = 9,
        WAIT_FOLLOWUP_SCENE = 10,
        WAIT_LINKED_MOBY_REMOVAL_BEFORE_SAVE = 11,
        ENTER_LINKED_MOBY_CLEANUP_WAIT = 12,
        WAIT_TO_REMOVE_LINKED_MOBY = 13,
        FINALIZE_LINKED_MOBY_CLEANUP = 14
    };
    enum {
        MOBY_STATE_REMOVAL_BELOW_INSTANCE_END = 0xfd,
        MOBY_STATE_REMOVAL_AT_OR_ABOVE_INSTANCE_END = 0xfe
    };
    int *scene_controller_vars = (int *)moby->pvars;
    switch (moby->state) {
    case WAIT_TO_START_INITIAL_SCENE:
        if (D_L13_0015F5C4 == 0)
            moby->state = START_INITIAL_SCENE;
        break;
    case START_INITIAL_SCENE:
        if (alternate_item_available[0x64] == 0) {
            alternate_item_available[0x64] = 1;
            moby->state = WAIT_INITIAL_SCENE;
            start_scene(0);
            return;
        }
        moby->state = ENTER_CLIP_VOLUME_CHECK;
        break;
    case WAIT_INITIAL_SCENE:
        if (D_L13_0015F5C4 != 2)
            moby->state = ENTER_CLIP_VOLUME_CHECK;
        break;
    case ENTER_CLIP_VOLUME_CHECK:
        moby->state = CHECK_CLIP_VOLUME_FOR_SCENE;
        break;
    case CHECK_CLIP_VOLUME_FOR_SCENE: {
        if (alternate_item_available[0x65] == 0) {
            if (scene_controller_vars[0] == -1)
                goto set_state_4;
            if (is_point_inside_clip_volume(D_0013F3D0, scene_controller_vars[0]) == 0)
                goto after_state_4;
            alternate_item_available[0x65] = 1;
            if (scene_controller_vars[5] != -1) {
                struct Moby *other = (struct Moby *)(moby_base + scene_controller_vars[5] * 0x100);
                other->flags |= 0x41;
            }
            moby->state = WAIT_CLIP_VOLUME_SCENE;
            start_scene(1);
        } else {
            moby->state = ENTER_LINKED_MOBY_REMOVAL_WAIT;
            goto after_state_4;
        set_state_4:
            moby->state = WAIT_LINKED_MOBY_REMOVAL_BEFORE_SCENE;
        }
    after_state_4:
        if (moby->state != CHECK_CLIP_VOLUME_FOR_SCENE && scene_controller_vars[3] != -1) {
            mark_moby_for_removal((char *)(moby_base + scene_controller_vars[3] * 0x100));
        }
        break;
    }
    case WAIT_CLIP_VOLUME_SCENE: {
        char *object;
        char *entity;
        int   index;
        if (D_L13_0015F5C4 != 2) {
            if (scene_controller_vars[5] != -1) {
                struct Moby *other = (struct Moby *)(moby_base + scene_controller_vars[5] * 0x100);
                other->flags &= 0xffbe;
            }
            index = scene_controller_vars[2];
            if (index != -1 && scene_controller_vars[4] != -1) {
                entity = (char *)(moby_base + scene_controller_vars[4] * 0x100);
                object = (char *)l13_cuboids;
                index *= 0x80;
                qcopy(entity + 0x10, &object[index + 0x30]);
                object += index;
                set_race_camera(object + 0x30, object + 0x70, 0, 1);
                D_001413F5 = 1;
                *(float *)(*(char **)(entity + 0x78) + 0x7c) = 0.99f;
                D_L13_0015F3FC = 0.99f;
                entity[0x20] = 2;
            }
            moby->state = ENTER_LINKED_MOBY_REMOVAL_WAIT;
        }
        break;
    }
    case ENTER_LINKED_MOBY_REMOVAL_WAIT:
        moby->state = WAIT_LINKED_MOBY_REMOVAL_BEFORE_SCENE;
        break;
    case WAIT_LINKED_MOBY_REMOVAL_BEFORE_SCENE: {
        char *object;
        if (alternate_item_available[0x66] == 0) {
            struct Moby *other;
            if (scene_controller_vars[5] == -1) {
                moby->state = WAIT_TO_REMOVE_LINKED_MOBY;
                break;
            }
            other = (struct Moby *)(moby_base + scene_controller_vars[5] * 0x100);
            if (other != 0 && other->oclass == 0x184 &&
                other->state != MOBY_STATE_REMOVAL_AT_OR_ABOVE_INSTANCE_END &&
                other->state != MOBY_STATE_REMOVAL_BELOW_INSTANCE_END)
                return;
            if (scene_controller_vars[4] != -1) {
                struct Moby *other = (struct Moby *)(moby_base + scene_controller_vars[4] * 0x100);
                other->flags |= 2;
            }
            object = (char *)l13_cuboids + scene_controller_vars[2] * 0x80;
            set_race_camera(object + 0x30, object + 0x70, 0x72, 1);
            alternate_item_available[0x66] = 1;
            if (scene_controller_vars[8] != -1) {
                struct Moby *other = (struct Moby *)(moby_base + scene_controller_vars[8] * 0x100);
                other->flags |= 0x41;
            }
            request_music_track(0, 5);
            moby->state = WAIT_SCENE_BEFORE_FOLLOWUP;
            start_scene(2);
            return;
        }
        moby->state = ENTER_LINKED_MOBY_CLEANUP_WAIT;
        break;
    }
    case WAIT_SCENE_BEFORE_FOLLOWUP:
        if (D_L13_0015F5C4 != 2) {
            moby->state = START_FOLLOWUP_SCENE;
            queue_scene_movie_with_game_language(14);
        }
        break;
    case START_FOLLOWUP_SCENE:
        if (D_L13_0015F5C4 != 2) {
            start_scene(3);
            moby->state = WAIT_FOLLOWUP_SCENE;
        }
        break;
    case WAIT_FOLLOWUP_SCENE: {
        if (D_L13_0015F5C4 != 2) {
            if (scene_controller_vars[4] != -1) {
                struct Moby *other = (struct Moby *)(moby_base + scene_controller_vars[4] * 0x100);
                other->flags &= 0xfffd;
            }
            unlock_planet(14);
            if (scene_controller_vars[8] != -1) {
                struct Moby *other = (struct Moby *)(moby_base + scene_controller_vars[8] * 0x100);
                other->flags &= 0xffbe;
                initialize_moby_state_record(other);
                D_0013E030.hero_start_x = 464.66f;
                D_0013E030.hero_start_y = 580.68f;
                D_0013E030.hero_start_z = 316.72f;
                D_0013E030.hero_start_yaw = 2.77f;
            }
            D_L13_0015F404 = 0;
            moby->state = WAIT_LINKED_MOBY_REMOVAL_BEFORE_SAVE;
        }
        break;
    }
    case WAIT_LINKED_MOBY_REMOVAL_BEFORE_SAVE: {
        if (scene_controller_vars[4] != -1) {
            struct Moby *other = (struct Moby *)(moby_base + scene_controller_vars[4] * 0x100);
            if (other != 0 && other->oclass == 0x45 &&
                other->state != MOBY_STATE_REMOVAL_AT_OR_ABOVE_INSTANCE_END &&
                other->state != MOBY_STATE_REMOVAL_BELOW_INSTANCE_END)
                return;
            memcard_save_data(0, -1);
            moby->state = ENTER_LINKED_MOBY_CLEANUP_WAIT;
            return;
        }
        moby->state = ENTER_LINKED_MOBY_CLEANUP_WAIT;
        break;
    }
    case ENTER_LINKED_MOBY_CLEANUP_WAIT:
        moby->state = WAIT_TO_REMOVE_LINKED_MOBY;
        break;
    case WAIT_TO_REMOVE_LINKED_MOBY:
        if (scene_controller_vars[4] != -1) {
            if (D_L13_0015FC88[*(unsigned char *)(moby_base + scene_controller_vars[4] * 0x100 +
                                                  0xb0)] == 0xff) {
                if (scene_controller_vars[8] != -1)
                    mark_moby_for_removal((char *)(moby_base + scene_controller_vars[8] * 0x100));
                moby->state = FINALIZE_LINKED_MOBY_CLEANUP;
            }
        } else {
            mark_moby_for_removal(moby);
        }
        break;
    case FINALIZE_LINKED_MOBY_CLEANUP:
        moby->flags |= 2;
        break;
    }
}
