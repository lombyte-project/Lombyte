/* Level 16 wall_button. */
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
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/mobys/is_hero_on_moby.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/math/interpolation/spring_turn.h"
#include "rnc/overlay/shared/rendering/effects/spawn_type_10_particle.h"
#include "rnc/world/collision/collision_line.h"

extern float D_0015ED6C;

extern char *D_L16_0015FFD8;

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");

#include "sda.h"

extern float D_0015ED6C, frame_time;
extern float random_angle_radians(void) __asm__("FUN_00213308");

extern float D_0015ED6C, D_0015EE70;

/* Pulse a linked control object and toggle its child when hit or its timer expires. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D0560), where it is exact; names translated to the US level program. */

extern float         fast_add_rotations(float, float);
extern float         fast_sin(float);
extern int           scale_game_frames(int);
extern int           truncate_float_to_s32(float);
extern unsigned char visited52_lui[] __asm__("D_0014C050") NOT_SDA;
extern void          set_wall_button_blocker_collision(void *, int) __asm__("FUN_L16_002e3770");

void wall_button_update(struct Moby *m) __asm__("FUN_L16_002cf198");

/* Moby class 546 */
void wall_button_update(struct Moby *moby) {
    enum {
        WALL_BUTTON_STATE_INITIALIZE = 0,
        WALL_BUTTON_STATE_WAIT_FOR_HIT = 1,
        WALL_BUTTON_STATE_ACTIVATED = 2
    };
    char *wall_button_data = (char *)moby->pvars;
    switch ((*(unsigned char *)((char *)(moby) + (0x20)))) {
    case WALL_BUTTON_STATE_INITIALIZE:
        (*(float *)((char *)(wall_button_data) + (0x68))) = random_angle_radians();
        (*(float *)((char *)(wall_button_data) + (0x30))) = 0.001f;
        if ((*(unsigned char *)((char *)(((char *)&(*(short *)&visited52_lui)) +
                                         (current_level_index << 4)) +
                                ((*(unsigned char *)((char *)(moby) + (0xB0)))))) == 255 &&
            (*(int *)((char *)(wall_button_data) + (0x64))) == -1) {
            (*(unsigned char *)((char *)(moby) + (0xBC))) = 1;
            (*(unsigned char *)((char *)(moby) + (0x20))) = 2;
            if ((*(int *)((char *)(wall_button_data) + (0x60))) != -1) {
                set_wall_button_blocker_collision(
                    D_L16_0015FFD8 + ((*(int *)((char *)(wall_button_data) + (0x60))) << 8), 0);
            }
        } else
            (*(unsigned char *)((char *)(moby) + (0x20))) = 1;
        break;
    case WALL_BUTTON_STATE_WAIT_FOR_HIT: {
        int color;
        (*(float *)((char *)(wall_button_data) + (0x68))) = fast_add_rotations(
            (*(float *)((char *)(wall_button_data) + (0x68))), D_0015ED6C * 6.2831855f);
        color = truncate_float_to_s32(
            (fast_sin((*(float *)((char *)(wall_button_data) + (0x68)))) * 4.0f - 3.0f) * 128.0f);
        if (color > 128)
            color = 128;
        else if (color < 32)
            color = 32;
        (*(int *)((char *)(moby) + (0x90))) = 0x80000000 | (color << 16) | (color << 8) | color;
        if (find_moby_hit(moby, 0x330000, 0)) {
            (*(unsigned char *)((char *)(moby) + (0x20))) = 2;
            (*(int *)((char *)(moby) + (0x90))) = 0x80208020;
            (*(unsigned char *)((char *)(moby) + (0xBC))) = 1;
            (*(float *)((char *)(wall_button_data) + (0x68))) = 1.5707964f;
            allocate_voice_for_target_entry(0, 0, moby);
            if ((*(int *)((char *)(wall_button_data) + (0x60))) != -1)
                set_wall_button_blocker_collision(
                    D_L16_0015FFD8 + ((*(int *)((char *)(wall_button_data) + (0x60))) << 8), 0);
            if ((*(int *)((char *)(wall_button_data) + (0x64))) > 0)
                (*(int *)((char *)(wall_button_data) + (0x6C))) =
                    scale_game_frames((*(int *)((char *)(wall_button_data) + (0x64))));
        }
        break;
    }
    case WALL_BUTTON_STATE_ACTIVATED: {
        int color = truncate_float_to_s32(
            (fast_sin((*(float *)((char *)(wall_button_data) + (0x68)))) * 0.5f + 0.5f) * 128.0f);
        if (color > 128)
            color = 128;
        else if (color < 32)
            color = 32;
        (*(int *)((char *)(moby) + (0x90))) = 0x80200020 | (color << 8);
        if ((*(int *)((char *)(wall_button_data) + (0x64))) > 0) {
            if (fast_dec_timer((int *)(wall_button_data + 0x6C))) {
                (*(unsigned char *)((char *)(moby) + (0xBC))) = 0;
                (*(unsigned char *)((char *)(moby) + (0x20))) = 1;
                (*(float *)((char *)(wall_button_data) + (0x68))) = 1.5707964f;
                if ((*(int *)((char *)(wall_button_data) + (0x60))) != -1) {
                    set_wall_button_blocker_collision(
                        D_L16_0015FFD8 + ((*(int *)((char *)(wall_button_data) + (0x60))) << 8), 1);
                }
            } else if ((*(int *)((char *)(wall_button_data) + (0x6C))) < scale_game_frames(90))
                (*(float *)((char *)(wall_button_data) + (0x68))) = fast_add_rotations(
                    (*(float *)((char *)(wall_button_data) + (0x68))), D_0015ED6C * 25.132742f);
        }
        break;
    }
    }
    (*(unsigned char *)((char *)(moby) + (0xA4))) = 255;
}
