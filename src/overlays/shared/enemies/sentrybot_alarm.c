/* Shared moby class sentrybot_alarm: pulses, runs the alarm effect and releases group mobys. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "rnc/math/asm_helpers.h"
#include "qcopy.h"
#include "rnc/math/random.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/world/collision/collision_line.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/enemies.h"
#include "rnc/overlay/shared/rendering.h"

typedef struct {
    int w[20];
} __attribute__((aligned(16))) Tbl_29E8A0;

typedef struct {
    int c[6];
} Cols;

extern float        fast_sin(float);
extern int          D_L15_0015F5C4;
extern int          D_L15_0015F5CC;
extern struct Moby *l15_moby_array __asm__("D_L15_0015FFD8");
extern int          scale_game_frames(int);
extern void         add_draw_callback(void (*)(void), void *) __asm__("FUN_001f4600");

extern f32 approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");

extern int D_L15_001ABE40[];
extern int D_0015ED84;

extern int interpolate_color_2a3138(float, int, int) __asm__("FUN_001fa6e0");

extern int   l15_sentrybot_alarm_effect_update_stamp __asm__("D_L15_00161AB4") __attribute__((sda));
extern int   D_L15_00161AB8 __attribute__((sda));
extern short D_L15_00161ABC_gp __asm__("D_L15_00161ABC") __attribute__((sda));
void         update_sentrybot_alarm_effect(void *moby) __asm__("FUN_L15_002cbc30");

extern void update_sentrybot_alarm_effect(void *) __asm__("FUN_L15_002cbc30");

extern int FUN_L15_002cbac0_u() __asm__("FUN_L15_002cbac0");
extern s32 D_L15_00161AB8;
extern s32 D_L15_00161AC0_gp __asm__("D_L15_00161AC0") __attribute__((sda));

int          sentrybot_alarm_voice_slot_index_gp __asm__("D_L15_00161AC4") = -1;
extern short sentrybot_alarm_voice_owner_gp __asm__("D_L15_00161AC8") __attribute__((sda));

/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_002A3A48), where it is exact; names translated to the US level program. */

int activate_first_inactive_group_moby(int moby_group_index, void *pos) __asm__("FUN_L15_002a2868");

int activate_first_inactive_group_moby(int moby_group_index, void *pos) {
    enum {
        GROUP_MOBY_STATE_INACTIVE = 1
    };
    enum {
        GROUP_MOBY_STATE_ACTIVATING = 2
    };
    short       *group_moby_entry = (short *)D_L15_001ABE40[moby_group_index];
    struct Hero *hero_data = &hero;
    do {
        unsigned char *listed_moby =
            (unsigned char *)l15_moby_array + ((*(unsigned short *)group_moby_entry & 0x7FFF) << 8);
        unsigned char state = MOBY(listed_moby)->state;
        if (state == GROUP_MOBY_STATE_INACTIVE) {
            char *activated_moby_data;
            MOBY(listed_moby)->state = GROUP_MOBY_STATE_ACTIVATING;
            if (MOBY(listed_moby)->prev_seq != 0)
                blend_moby_animation(listed_moby, 0, 0, 0);
            MOBY(listed_moby)->flags |= 1;
            MOBY(listed_moby)->visible = 0;
            activated_moby_data = ((char *)MOBY(listed_moby)->pvars);
            MOBY(listed_moby)->collision = *(int *)(((char *)MOBY(listed_moby)->pclass) + 0x10);
            *(float *)(activated_moby_data + 0x64) =
                random_float_between(-15.0f, 15.0f) * DEG_TO_RAD;
            *(int *)(activated_moby_data + 0x68) = scale_game_frames(0x3C);
            qcopy(&MOBY(listed_moby)->pos, pos);
            MOBY(listed_moby)->pos.z -= 1.0f;
            MOBY(listed_moby)->rot.z =
                fast_arc_tan(hero_data->motion.pos.f[0] - MOBY(listed_moby)->pos.x,
                             hero_data->motion.pos.f[1] - MOBY(listed_moby)->pos.y);
            return 1;
        }
    } while (*group_moby_entry++ > -1);
    return 0;
}

/* Advances the animation state, pulses the color, and updates the shared effect. */

void sentrybot_alarm_update(unsigned char *moby) __asm__("FUN_L15_002cb4c8");

/* Moby class 408 */
void sentrybot_alarm_update(unsigned char *moby) {
    enum {
        SENTRYBOT_ALARM_STATE_INITIALIZE = 0,
        SENTRYBOT_ALARM_STATE_STANDBY_PULSE = 1,
        SENTRYBOT_ALARM_STATE_STANDBY_RETURN_TRANSITION = 2,
        SENTRYBOT_ALARM_STATE_WAIT_FOR_ALARM_TRIGGER = 3,
        SENTRYBOT_ALARM_STATE_ALARM_START_TRANSITION = 4,
        SENTRYBOT_ALARM_STATE_ACTIVE_PULSE = 5,
        SENTRYBOT_ALARM_STATE_WAIT_FOR_GROUP_RELEASE_TRIGGER = 6,
        SENTRYBOT_ALARM_STATE_GROUP_RELEASE_START_TRANSITION = 7,
        SENTRYBOT_ALARM_STATE_RELEASE_GROUP_MOBYS = 8,
        SENTRYBOT_ALARM_STATE_GROUP_RELEASE_STOP_TRANSITION = 9,
        SENTRYBOT_ALARM_STATE_ALARM_STOP_TRANSITION = 10
    };
    enum {
        SENTRYBOT_ALARM_MODE_GROUP_RELEASE_ONLY = 2
    };
    float position[4];
    float step;
    int   color;
    int   configuration_mode;
    int  *alarm_state = ((int *)MOBY(moby)->pvars);
    switch (MOBY(moby)->state) {
    case SENTRYBOT_ALARM_STATE_INITIALIZE:
        configuration_mode = alarm_state[0];
        l15_sentrybot_alarm_effect_update_stamp = 0;
        D_L15_00161AB8 = 0;
        *(int *)&D_L15_00161ABC_gp = 0;
        if (configuration_mode == SENTRYBOT_ALARM_MODE_GROUP_RELEASE_ONLY) {
            MOBY(moby)->state = SENTRYBOT_ALARM_STATE_WAIT_FOR_GROUP_RELEASE_TRIGGER;
            if (MOBY(moby)->prev_seq != 4)
                blend_moby_animation(moby, 4, 0, 0);
        } else {
            MOBY(moby)->state = SENTRYBOT_ALARM_STATE_WAIT_FOR_ALARM_TRIGGER;
            if (MOBY(moby)->prev_seq != 2)
                blend_moby_animation(moby, 2, 0, 0);
        }
        break;
    case SENTRYBOT_ALARM_STATE_STANDBY_PULSE:
        color = interpolate_color_2a3138(
            (fast_sin(*(float *)(alarm_state + 5)) + 1.0f) * 0.5f, 0x801414bc, 0x801414dc);
        step = frame_time;
        MOBY(moby)->glow_color = color;
        *(float *)(alarm_state + 5) =
            fast_add_rotations(*(float *)(alarm_state + 5), step * 6.2831855f);
        if (alarm_state[2] == 0)
            goto update;
        if (D_L15_0015F5C4 != 2) {
            if (alarm_state[0] == 1) {
                MOBY(moby)->state = SENTRYBOT_ALARM_STATE_WAIT_FOR_GROUP_RELEASE_TRIGGER;
                if (MOBY(moby)->prev_seq != 4)
                    blend_moby_animation(moby, 4, 0, scale_game_frames(30));
            } else {
                MOBY(moby)->state = SENTRYBOT_ALARM_STATE_ACTIVE_PULSE;
            }
        }
        break;
    case SENTRYBOT_ALARM_STATE_STANDBY_RETURN_TRANSITION:
        if (MOBY(moby)->anim_flags & 2) {
            MOBY(moby)->state = SENTRYBOT_ALARM_STATE_WAIT_FOR_ALARM_TRIGGER;
            if (MOBY(moby)->prev_seq != 2)
                blend_moby_animation(moby, 2, 0, scale_game_frames(10));
        }
        break;
    case SENTRYBOT_ALARM_STATE_WAIT_FOR_ALARM_TRIGGER:
        if (alarm_state[2] == 0)
            goto update;
        if (D_L15_0015F5C4 != 2) {
            MOBY(moby)->state = SENTRYBOT_ALARM_STATE_ALARM_START_TRANSITION;
            if (MOBY(moby)->prev_seq != 3)
                blend_moby_animation(moby, 3, 0, scale_game_frames(10));
        }
        break;
    case SENTRYBOT_ALARM_STATE_ALARM_START_TRANSITION:
        if (MOBY(moby)->anim_flags & 2) {
            if (alarm_state[2] == 0 || D_L15_0015F5C4 == 2)
                goto stop_animation;
            if (alarm_state[0] == 1) {
                MOBY(moby)->state = SENTRYBOT_ALARM_STATE_GROUP_RELEASE_START_TRANSITION;
                if (MOBY(moby)->prev_seq != 5)
                    blend_moby_animation(moby, 5, 0, scale_game_frames(10));
            } else {
                if (MOBY(moby)->prev_seq != 8)
                    blend_moby_animation(moby, 8, 0, scale_game_frames(10));
                MOBY(moby)->state = SENTRYBOT_ALARM_STATE_ACTIVE_PULSE;
            }
        }
        break;
    stop_animation:
        if (MOBY(moby)->prev_seq != 8)
            blend_moby_animation(moby, 8, 0, scale_game_frames(10));
        MOBY(moby)->state = SENTRYBOT_ALARM_STATE_ALARM_STOP_TRANSITION;
        break;
    case SENTRYBOT_ALARM_STATE_ACTIVE_PULSE:
        if ((MOBY(moby)->anim_flags & 2) && MOBY(moby)->prev_seq != 0)
            blend_moby_animation(moby, 0, 0, scale_game_frames(10));
        color = interpolate_color_2a3138(
            (fast_sin(*(float *)(alarm_state + 5)) + 1.0f) * 0.5f, 0x801414dc, 0x8004048c);
        step = frame_time;
        MOBY(moby)->glow_color = color;
        *(float *)(alarm_state + 5) =
            fast_add_rotations(*(float *)(alarm_state + 5), step * 12.566371f);
        if (fast_dec_timer(alarm_state + 6) || D_L15_0015F5C4 == 2) {
            set_sentrybot_alarm_duration((char *)moby, 0);
            alarm_state[2] = 0;
            if (MOBY(moby)->prev_seq != 8)
                blend_moby_animation(moby, 8, 0, scale_game_frames(10));
            MOBY(moby)->state = SENTRYBOT_ALARM_STATE_ALARM_STOP_TRANSITION;
        }
        break;
    case SENTRYBOT_ALARM_STATE_WAIT_FOR_GROUP_RELEASE_TRIGGER:
        if (alarm_state[2] == 0)
            goto update;
        MOBY(moby)->state = SENTRYBOT_ALARM_STATE_GROUP_RELEASE_START_TRANSITION;
        if (MOBY(moby)->prev_seq != 5)
            blend_moby_animation(moby, 5, 0, scale_game_frames(10));
        break;
    case SENTRYBOT_ALARM_STATE_GROUP_RELEASE_START_TRANSITION:
        if (MOBY(moby)->anim_flags & 2) {
            MOBY(moby)->state = SENTRYBOT_ALARM_STATE_RELEASE_GROUP_MOBYS;
            if (MOBY(moby)->prev_seq != 6)
                blend_moby_animation(moby, 6, 0, scale_game_frames(10));
        }
        break;
    case SENTRYBOT_ALARM_STATE_RELEASE_GROUP_MOBYS: {
        float *pos = (float *)&MOBY(moby)->pos;
        float *hero_pos = hero.motion.pos.f;
        if (vector_distance_xy(pos, hero_pos) < 16.0f && D_L15_0015F5C4 != 2 &&
            fast_dec_timer(alarm_state + 3)) {
            qcopy(position, pos);
            position[2] += 1.0f;
            if (collision_line(position, hero_pos + 20, 2, 0, 0) == 0 &&
                activate_first_inactive_group_moby(alarm_state[1], pos))
                alarm_state[3] = alarm_state[4];
        }
        if (fast_dec_timer(alarm_state + 6) || D_L15_0015F5C4 == 2) {
            set_sentrybot_alarm_duration((char *)moby, 0);
            alarm_state[2] = 0;
            MOBY(moby)->state = SENTRYBOT_ALARM_STATE_GROUP_RELEASE_STOP_TRANSITION;
            if (MOBY(moby)->prev_seq != 7)
                blend_moby_animation(moby, 7, 0, scale_game_frames(10));
        }
        break;
    }
    case SENTRYBOT_ALARM_STATE_GROUP_RELEASE_STOP_TRANSITION:
        if (MOBY(moby)->anim_flags & 2) {
            if (alarm_state[0] == 2) {
                MOBY(moby)->state = SENTRYBOT_ALARM_STATE_WAIT_FOR_GROUP_RELEASE_TRIGGER;
                if (MOBY(moby)->prev_seq != 4)
                    blend_moby_animation(moby, 4, 0, scale_game_frames(10));
            } else {
                MOBY(moby)->state = SENTRYBOT_ALARM_STATE_ALARM_STOP_TRANSITION;
                if (MOBY(moby)->prev_seq != 8)
                    blend_moby_animation(moby, 8, 0, scale_game_frames(10));
            }
        }
        break;
    case SENTRYBOT_ALARM_STATE_ALARM_STOP_TRANSITION:
        if (MOBY(moby)->anim_flags & 2) {
            MOBY(moby)->state = SENTRYBOT_ALARM_STATE_STANDBY_RETURN_TRANSITION;
            if (MOBY(moby)->prev_seq != 1)
                blend_moby_animation(moby, 1, 0, scale_game_frames(10));
        }
        break;
    }
    if (alarm_state[2] != 0)
        D_L15_00161AB8 = 1;
update:
    if (l15_sentrybot_alarm_effect_update_stamp != D_L15_0015F5CC) {
        l15_sentrybot_alarm_effect_update_stamp = D_L15_0015F5CC;
        update_sentrybot_alarm_effect(moby);
        D_L15_00161AB8 = 0;
    }
}

/* per-frame update of the level's looping effect: ramp the gauge, then start, run or stop the draw callback */
/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_002CCFC0), where it is exact; names translated to the US level program. */

void update_sentrybot_alarm_effect(void *moby) {
    struct Hero *alarm_gating_hero = &hero;
    if (alarm_gating_hero->state.current == 0x65) {
        approach_value(&D_L15_00161ABC_gp, 0.0f, frame_time * 2);
        FUN_L15_002cbac0_u(moby, 0);
    } else if (*(int *)&D_L15_00161AB8 != 0 &&
               (D_0015ED84 != 0xF || 240.0f < alarm_gating_hero->motion.pos.f[1])) {
        approach_value(&D_L15_00161ABC_gp, 1.0f, frame_time * 2);
    } else {
        approach_value(&D_L15_00161ABC_gp, 0.0f, frame_time * 2);
    }
    if (*(float *)&D_L15_00161ABC_gp != 0.0f) {
        *(int *)&D_L15_00161AC0_gp = 1;
        if (voice_is_playing(*(void **)&sentrybot_alarm_voice_owner_gp,
                             sentrybot_alarm_voice_slot_index_gp) == 0) {
            sentrybot_alarm_voice_slot_index_gp =
                allocate_voice_for_target_entry(5, 0x11, (int)moby);
            *(void **)&sentrybot_alarm_voice_owner_gp = moby;
        }
        add_draw_callback(emit_sine_modulated_rgba_draw_packet, moby);
    } else {
        if (voice_is_playing(*(void **)&sentrybot_alarm_voice_owner_gp,
                             sentrybot_alarm_voice_slot_index_gp) != 0) {
            release_voice_slot(sentrybot_alarm_voice_slot_index_gp);
            *(int *)&sentrybot_alarm_voice_owner_gp = 0;
            sentrybot_alarm_voice_slot_index_gp = -1;
        }
        *(int *)&D_L15_00161AC0_gp = 0;
    }
}

int   D_L17_00161988 = 0;
s32   D_L17_00161994 = 0;
int   D_L17_00161998 = -1;
short D_L17_0016199C = 0;
