/* Shared moby class hoverboard. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/data_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/l05/hero/hero_set_state.h"
#include "rnc/game/item_state.h"
#include "rnc/overlay/shared/vehicles/hoverboard_data/hoverboard_update.h"

extern void help_request(int, int) __asm__("FUN_L00_00203908");
extern void spawn_effect(void *, void *, int, int, int, int, int, int) __asm__("FUN_L00_00269958");
extern void spawn_burst(void *, void *, int, int, int, int, float) __asm__("FUN_L00_0026cbb0");
extern int  frame_number_gp __asm__("D_L05_0015F5CC") __attribute__((sda));
extern unsigned short D_00141938 NOT_SDA;
extern unsigned short D_00141C60 NOT_SDA;
extern int                       D_L05_0015F6A8;

extern int D_L05_0016190C __attribute__((sda));
extern int D_L05_00161910 __attribute__((sda));

void hoverboard_update(unsigned char *moby) __asm__("FUN_L05_002f87a8");

/* Moby class 439 */
void hoverboard_update(unsigned char *moby) {
    enum {
        HOVERBOARD_STATE_WAIT_FOR_PLAYER_APPROACH = 0,
        HOVERBOARD_STATE_WAIT_FOR_CONTROL_MODE_EXIT = 1
    };
    char        *hoverboard_vars = ((char *)MOBY(moby)->pvars);
    float        delta[4], position[4], effect_position[4], offset[4], second_position[4];
    float        angle, distance_scale, delta_length;
    int          state = (u8)moby[0x20];
    struct Hero *player_hero;

    switch (state) {
    case HOVERBOARD_STATE_WAIT_FOR_PLAYER_APPROACH: {
        char        *player_position = D_0013F3D0;
        struct Hero *player;
        if (fast_vec_dist(player_position, moby + 0x10) < 5.5f && frame_number_gp > 3 &&
            (player = (struct Hero *)(player_position - 0x80))->state.control_mode != 0x15 &&
            player->state.control_mode != 0x16) {
            player->hoverboard_moby = moby;
            hero_set_state(0x6b, 1);
            if (*(int *)(hoverboard_vars + 0x50) == 0) {
                qcopy(hoverboard_vars + 0x10, moby + 0x10);
            }
            moby[0x20] = 1;
            MOBY(moby)->update_dist = 0xff;
        }
        break;
    }
    case HOVERBOARD_STATE_WAIT_FOR_CONTROL_MODE_EXIT:
        if (*(int *)(hoverboard_vars + 0x50) == 0 && alternate_item_available[0] != 0 &&
            D_00141938 == 0 && D_L05_0015F6A8 != 0 &&
            (player_hero = &hero, scale_game_frames(0x4b0) < player_hero->control_mode_timer) &&
            D_00141C60 == 0) {
            help_request(0x138f, 0x5f);
        }
        if (hero.state.control_mode != 0x16) {
            moby[0x30] = 0x10;
            if (*(int *)(hoverboard_vars + 0x50) == 0) {
                qcopy(moby + 0x10, hoverboard_vars + 0x10);
            }
            moby[0x20] = 0;
        }
        break;
    case 2:
        /* Fakematch: a dead test in state 2 gives retail's block layout. */
        if (*(int *)(hoverboard_vars + 0x50) != 0)
            break;
        break;
    }
    {
        unsigned char *position_ptr = moby + 0x10;
        fast_vec_sub(delta, position_ptr, hoverboard_vars);
        qcopy(hoverboard_vars, position_ptr);
    }
    delta_length = fast_vec_length(delta);
    if ((unsigned char)moby[0xbc] != 2 && delta_length < 2.0f) {
        moby_point_position(moby, frame_number_gp & 1, position);
        angle = random_angle_radians();
        distance_scale = frame_time * 0.25f;
        fast_vec_normalize(effect_position, moby + 0xe0, distance_scale * fast_cos(angle));
        fast_vec_normalize(offset, moby + 0xd0, distance_scale * fast_sin(angle));
        fast_vec_add(effect_position, effect_position, delta);
        fast_vec_add(effect_position, effect_position, offset);
        qcopy(effect_position, delta);
        effect_position[3] = (float)hoverboard_effect_displacement_w_gp;
        if ((unsigned char)moby[0xbc] == 0) {
            int size = random_int_between(scale_game_frames(10), scale_game_frames(13));
            spawn_effect(
                position, effect_position, D_L05_0016190C, D_L05_00161910, size, 15, 15, 1);
            moby[0xbc] = 0;
            return;
        }
        fast_vec_scale(second_position, delta, 0.920f);
        {
            int size = random_int_between(scale_game_frames(15), scale_game_frames(18));
            spawn_effect(position, second_position, 0x5032f0d2, D_L05_00161910, size, 30, 20, 1);
        }
        {
            int size = random_int_between(scale_game_frames(8), scale_game_frames(17));
            spawn_burst(
                position, effect_position, D_L05_0016190C, D_L05_00161910, size, 1, 15000.0f);
        }
    }
    moby[0xbc] = 0;
}

int D_L16_0015F6A8 = 0;
int D_L16_00161968 = 0x4f0090f0;
int D_L16_0016196C = 0x4f0000f0;
