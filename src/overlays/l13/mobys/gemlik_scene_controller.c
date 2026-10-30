/* Level 13 Gemlik scene controller moby class. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/save/memcard_save_data.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/cuboid.h"
#include "rnc/game/item_state.h"
#include "rnc/overlay/level_labels.h"

extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");

extern int D_L13_0015F5C4;

extern unsigned char D_001413F5 NOT_SDA;

extern float D_L13_0015F3FC;

extern int D_L13_0015F404;

extern struct {
    char  pad0[0x60];
    float unk60, unk64, unk68;
    char  pad6C[0xC];
    float unk78;
} D_0013E030;

extern unsigned char D_L13_0015FC88[] MACRO_ADDR;

extern char D_0013F3D0[];

extern void set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");

extern void request_music_track(int, int) __asm__("FUN_L01_0027a248");

extern void start_scene(int) __asm__("FUN_L00_00298840");

extern void queue_scene_movie_with_game_language(int) __asm__("FUN_L00_00299460");

extern void initialize_moby_state_record(void *) __asm__("FUN_L00_0028e8a0");

void update_gemlik_scene_controller(struct Moby *moby) __asm__("FUN_L13_0030b628");

void update_gemlik_scene_controller(struct Moby *moby) {
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
    int *data = (int *)moby->pvars;
    switch (moby->state) {
    case WAIT_TO_START_INITIAL_SCENE:
        if (D_L13_0015F5C4 == 0)
            moby->state = 1;
        break;
    case START_INITIAL_SCENE:
        if (alternate_item_available[0x64] == 0) {
            alternate_item_available[0x64] = 1;
            moby->state = 2;
            start_scene(0);
            return;
        }
        moby->state = 3;
        break;
    case WAIT_INITIAL_SCENE:
        if (D_L13_0015F5C4 != 2)
            moby->state = 3;
        break;
    case ENTER_CLIP_VOLUME_CHECK:
        moby->state = 4;
        break;
    case CHECK_CLIP_VOLUME_FOR_SCENE: {
        if (alternate_item_available[0x65] == 0) {
            if (data[0] == -1)
                goto set_state_4;
            if (is_point_inside_clip_volume(D_0013F3D0, data[0]) == 0)
                goto after_state_4;
            alternate_item_available[0x65] = 1;
            if (data[5] != -1) {
                struct Moby *other = (struct Moby *)(moby_base + data[5] * 0x100);
                other->flags |= 0x41;
            }
            moby->state = 5;
            start_scene(1);
        } else {
            moby->state = 6;
            goto after_state_4;
        set_state_4:
            moby->state = 7;
        }
    after_state_4:
        if (moby->state != 4 && data[3] != -1) {
            mark_moby_for_removal((char *)(moby_base + data[3] * 0x100));
        }
        break;
    }
    case WAIT_CLIP_VOLUME_SCENE: {
        char *object;
        char *entity;
        int   index;
        if (D_L13_0015F5C4 != 2) {
            if (data[5] != -1) {
                struct Moby *other = (struct Moby *)(moby_base + data[5] * 0x100);
                other->flags &= 0xffbe;
            }
            index = data[2];
            if (index != -1 && data[4] != -1) {
                entity = (char *)(moby_base + data[4] * 0x100);
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
            moby->state = 6;
        }
        break;
    }
    case ENTER_LINKED_MOBY_REMOVAL_WAIT:
        moby->state = 7;
        break;
    case WAIT_LINKED_MOBY_REMOVAL_BEFORE_SCENE: {
        char *object;
        if (alternate_item_available[0x66] == 0) {
            struct Moby *other;
            if (data[5] == -1) {
                moby->state = 13;
                break;
            }
            other = (struct Moby *)(moby_base + data[5] * 0x100);
            if (other != 0 && other->oclass == 0x184 && other->state != 0xfe &&
                other->state != 0xfd)
                return;
            if (data[4] != -1) {
                struct Moby *other = (struct Moby *)(moby_base + data[4] * 0x100);
                other->flags |= 2;
            }
            object = (char *)l13_cuboids + data[2] * 0x80;
            set_race_camera(object + 0x30, object + 0x70, 0x72, 1);
            alternate_item_available[0x66] = 1;
            if (data[8] != -1) {
                struct Moby *other = (struct Moby *)(moby_base + data[8] * 0x100);
                other->flags |= 0x41;
            }
            request_music_track(0, 5);
            moby->state = 8;
            start_scene(2);
            return;
        }
        moby->state = 12;
        break;
    }
    case WAIT_SCENE_BEFORE_FOLLOWUP:
        if (D_L13_0015F5C4 != 2) {
            moby->state = 9;
            queue_scene_movie_with_game_language(14);
        }
        break;
    case START_FOLLOWUP_SCENE:
        if (D_L13_0015F5C4 != 2) {
            start_scene(3);
            moby->state = 10;
        }
        break;
    case WAIT_FOLLOWUP_SCENE: {
        if (D_L13_0015F5C4 != 2) {
            if (data[4] != -1) {
                struct Moby *other = (struct Moby *)(moby_base + data[4] * 0x100);
                other->flags &= 0xfffd;
            }
            unlock_planet(14);
            if (data[8] != -1) {
                struct Moby *other = (struct Moby *)(moby_base + data[8] * 0x100);
                other->flags &= 0xffbe;
                initialize_moby_state_record(other);
                D_0013E030.unk60 = 464.66f;
                D_0013E030.unk64 = 580.68f;
                D_0013E030.unk68 = 316.72f;
                D_0013E030.unk78 = 2.77f;
            }
            D_L13_0015F404 = 0;
            moby->state = 11;
        }
        break;
    }
    case WAIT_LINKED_MOBY_REMOVAL_BEFORE_SAVE: {
        if (data[4] != -1) {
            struct Moby *other = (struct Moby *)(moby_base + data[4] * 0x100);
            if (other != 0 && other->oclass == 0x45 && other->state != 0xfe && other->state != 0xfd)
                return;
            memcard_save_data(0, -1);
            moby->state = 12;
            return;
        }
        moby->state = 12;
        break;
    }
    case ENTER_LINKED_MOBY_CLEANUP_WAIT:
        moby->state = 13;
        break;
    case WAIT_TO_REMOVE_LINKED_MOBY:
        if (data[4] != -1) {
            if (D_L13_0015FC88[*(unsigned char *)(moby_base + data[4] * 0x100 + 0xb0)] == 0xff) {
                if (data[8] != -1)
                    mark_moby_for_removal((char *)(moby_base + data[8] * 0x100));
                moby->state = 14;
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
