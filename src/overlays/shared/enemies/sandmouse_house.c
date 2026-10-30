/* Shared moby class sandmouse_house. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/math/fast_math.h"
#include "qcopy.h"
#include "sda.h"

extern void                     blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
extern unsigned char D_0013D4C5 NOT_SDA;

extern float spring_turn(float *vel, float cur, float target, float k, float d, float max) __asm__("FUN_L00_00258110");
extern void  blend_moby_animation(void *m, int a, int b, int c) __asm__("FUN_00212f90");
extern float D_0015ED70_lui __asm__("D_0015ED70");

void sandmouse_house_update(unsigned char *m) __asm__("FUN_L01_002f2b68");

void sandmouse_house_update(unsigned char *moby) {
    enum {
        SANDMOUSE_HOUSE_INITIALIZE = 0,
        SANDMOUSE_HOUSE_WAIT_FOR_INITIAL_COMMAND = 1,
        SANDMOUSE_HOUSE_WAIT_FOR_INITIAL_COMMAND_ANIMATION_END = 2,
        SANDMOUSE_HOUSE_WAIT_FOR_FOLLOWUP_COMMAND = 3,
        SANDMOUSE_HOUSE_WAIT_FOR_FOLLOWUP_COMMAND_ANIMATION_END = 4,
        SANDMOUSE_HOUSE_WAIT_FOR_POST_FOLLOWUP_COMMAND = 5,
        SANDMOUSE_HOUSE_WAIT_FOR_POST_FOLLOWUP_COMMAND_ANIMATION_END = 6,
        SANDMOUSE_HOUSE_RESTORE_INITIAL_ANIMATION = 7
    };
    char *house_vars;
    if (moby == 0)
        return;
    house_vars = ((char *)MOBY(moby)->pvars);
    if (house_vars != 0) {
        if (moby[0x20] != 0) {
            if (D_0013D4C5 != 0) {
                if (hero.items[2].item_id == 5) {
                    float yaw_response_scale = D_0015ED70_lui;
                    MOBY(moby)->rot.z = spring_turn((float *)(house_vars + 4),
                                                    MOBY(moby)->rot.z,
                                                    *(float *)house_vars,
                                                    yaw_response_scale * 6.2831855f,
                                                    yaw_response_scale * 5.7595863f,
                                                    0.0f);
                }
            }
        }
    }
    switch (moby[0x20]) {
    case SANDMOUSE_HOUSE_INITIALIZE:
        if (moby[0x53] != 0) {
            blend_moby_animation(moby, 0, 0, scale_game_frames(0));
        }
        moby[0x20] = 1;
        *(float *)house_vars = MOBY(moby)->rot.z;
        break;
    case SANDMOUSE_HOUSE_WAIT_FOR_INITIAL_COMMAND:
        if ((unsigned char)(moby[0xBC] - 3) < 3) {
            if (moby[0x53] != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            }
            moby[0x20] = 2;
        }
        break;
    case SANDMOUSE_HOUSE_WAIT_FOR_INITIAL_COMMAND_ANIMATION_END:
        if (moby[0x70] & 2) {
            if (moby[0x53] != 2) {
                blend_moby_animation(moby, 2, 0, scale_game_frames(3));
            }
            moby[0x20] = 3;
        }
        break;
    case SANDMOUSE_HOUSE_WAIT_FOR_FOLLOWUP_COMMAND:
        if ((unsigned char)(moby[0xBC] - 4) < 2) {
            if (moby[0x53] != 3) {
                blend_moby_animation(moby, 3, 0, scale_game_frames(3));
            }
            moby[0x20] = 4;
        } else if (moby[0xBC] == 7 || moby[0xBC] == 1) {
            if (moby[0x53] != 6) {
                blend_moby_animation(moby, 6, 0, scale_game_frames(3));
            }
            moby[0x20] = 7;
        }
        break;
    case SANDMOUSE_HOUSE_WAIT_FOR_FOLLOWUP_COMMAND_ANIMATION_END:
        if (moby[0x70] & 2) {
            if (moby[0x53] != 4) {
                blend_moby_animation(moby, 4, 0, scale_game_frames(3));
            }
            moby[0x20] = 5;
        }
        break;
    case SANDMOUSE_HOUSE_WAIT_FOR_POST_FOLLOWUP_COMMAND:
        if (moby[0xBC] == 3 || moby[0xBC] == 6 || moby[0xBC] == 7 || moby[0xBC] == 1) {
            if (moby[0x53] != 5) {
                blend_moby_animation(moby, 5, 0, scale_game_frames(3));
            }
            moby[0x20] = 6;
        }
        break;
    case SANDMOUSE_HOUSE_WAIT_FOR_POST_FOLLOWUP_COMMAND_ANIMATION_END:
        if (moby[0x70] & 2) {
            if (moby[0xBC] == 7 || moby[0xBC] == 1) {
                if (moby[0x53] != 6) {
                    blend_moby_animation(moby, 6, 0, scale_game_frames(3));
                }
                moby[0x20] = 7;
            } else {
                if (moby[0x53] != 2) {
                    blend_moby_animation(moby, 2, 0, scale_game_frames(3));
                }
                moby[0x20] = 3;
            }
        }
        break;
    case SANDMOUSE_HOUSE_RESTORE_INITIAL_ANIMATION:
        if (moby[0x70] & 2) {
            if (moby[0x53] != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(3));
            }
            moby[0x20] = 1;
        }
        break;
    case 8:
        break;
    }
}
