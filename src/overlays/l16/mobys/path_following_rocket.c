/* Level 16 moby class path_following_rocket (0x101): a rocket that follows a path. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/mobys/is_hero_on_moby.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/math/interpolation/spring_turn.h"
#include "rnc/overlay/shared/rendering/effects/spawn_type_10_particle.h"
#include "rnc/world/collision/collision_line.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/mobys/moby_spawn.h"

#include "qcopy.h"
#include "rnc/overlay/entities.h"

extern float D_0015ED6C;
extern int   voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern float D_0015ED6C, frame_time;
/* Migrated from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002C5030). */

extern char *D_L16_001B0930[];
extern s32   allocate_voice_for_target_entry_u(s32, s32, void *) __asm__("func_0022DA68");
extern void  sample_camera_path(void *, s32, void *, void *, s32, f32) __asm__("func_00214E58");
s32          is_point_inside_clip_volume(s32 arg0, s32 arg1) __asm__("FUN_00214720");

void path_following_rocket_update(struct Moby *moby) __asm__("FUN_L16_002c3d38");

/* Moby class 257 */
void path_following_rocket_update(struct Moby *moby) {
    enum {
        ROCKET_STATE_INITIALIZE = 0,
        ROCKET_STATE_WAIT_FOR_TRIGGER = 1,
        ROCKET_STATE_FOLLOW_PATH = 2,
        ROCKET_STATE_WAIT_FOR_RESET = 3
    };
    char *rocket_path_data = (char *)moby->pvars;
    char *path = D_L16_001B0930[*(int *)(rocket_path_data + 4)];
    switch (moby->state) {
    case ROCKET_STATE_INITIALIZE:
        if (*(int *)rocket_path_data == -1 || *(int *)(rocket_path_data + 4) == -1) {
            mark_moby_for_removal(moby);
            break;
        }
        *(float *)(rocket_path_data + 0xC) =
            *(float *)(rocket_path_data + 8) * D_0015ED6C / fast_vec_dist(path + 0x10, path + 0x20);
        moby->state = 1;
        moby->update_dist = 100;
        moby->command = 0;
        break;
    case ROCKET_STATE_WAIT_FOR_TRIGGER: {
        struct Hero *player = &hero;
        if (player->state.control_mode != 15)
            break;
        if (is_point_inside_clip_volume(&player->motion.body_center, *(int *)rocket_path_data)) {
            moby->state = 2;
            *(short *)(rocket_path_data + 0x16) = 0;
        }
        break;
    }
    case ROCKET_STATE_FOLLOW_PATH:
        *(float *)(rocket_path_data + 0x10) += *(float *)(rocket_path_data + 0xC);
        if ((float)(*(int *)path - 1) <= *(float *)(rocket_path_data + 0x10)) {
            moby->update_dist = 0xFF;
            moby->state = 3;
        } else {
            sample_camera_path(
                (int *)path, 0, &moby->pos, &moby->rot.x, 0, *(float *)(rocket_path_data + 0x10));
        }
        if (moby->command)
            moby->collision = 0;
        if (!voice_is_playing(moby, *(short *)(rocket_path_data + 0x14))) {
            *(short *)(rocket_path_data + 0x14) = allocate_voice_for_target_entry(0, 4, moby);
        }
        if (*(short *)(rocket_path_data + 0x16) == 0 &&
            vector_distance_xy(&moby->pos, (char *)(&hero.motion.pos)) < 7.0f) {
            *(short *)(rocket_path_data + 0x16) = 1;
            allocate_voice_for_target_entry_u(1, 0, moby);
        }
        break;
    case ROCKET_STATE_WAIT_FOR_RESET:
        if (moby->visible == 0) {
            moby->visible = 0;
            moby->collision = 0;
            moby->flags |= 1;
        }
        if (*(int *)((char *)(&hero.state.control_mode)) != 15) {
            float pos[4];
            qcopy(pos, path + 0x10);
            pos[3] = 3.0f;
            if (fast_bsphere_check((char *)pos, 64.0f) == -1) {
                moby->command = 0;
                *(int *)(rocket_path_data + 0x10) = 0;
                sample_camera_path((int *)path,
                                   0,
                                   &moby->pos,
                                   &moby->rot.x,
                                   0,
                                   *(float *)(rocket_path_data + 0x10));
                moby->visible = 1;
                moby->flags &= ~1;
                moby->collision = moby->pclass->collision;
                *(short *)(rocket_path_data + 0x16) = 0;
                moby->state = 1;
            }
        }
        break;
    }
}
