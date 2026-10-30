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
extern void         set_sentrybot_alarm_duration(char *m, int a) __asm__("FUN_L15_002cbac0");
extern void         add_draw_callback(void (*)(void), void *) __asm__("FUN_001f4600");

extern f32 approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");

extern int D_L15_001ABE40[];
extern int D_0015ED84;

extern int interpolate_color_2a3138(float, int, int) __asm__("FUN_001fa6e0");

extern int   D_L15_00161AB4 __attribute__((sda));
extern int   D_L15_00161AB8 __attribute__((sda));
extern short D_L15_00161ABC_gp __asm__("D_L15_00161ABC") __attribute__((sda));
void         update_sentrybot_alarm_effect(void *moby) __asm__("FUN_L15_002cbc30");

extern void update_sentrybot_alarm_effect(void *) __asm__("FUN_L15_002cbc30");

extern int  voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern int  FUN_L15_002cbac0_u() __asm__("FUN_L15_002cbac0");
extern s32  D_L15_00161AB8;
extern s32  D_L15_00161AC0_gp __asm__("D_L15_00161AC0") __attribute__((sda));
extern void emit_sine_modulated_rgba_draw_packet(void) __asm__("FUN_L15_002cbd88");

extern int   sentrybot_alarm_voice_slot_index_gp __asm__("D_L15_00161AC4") __attribute__((sda));
extern short sentrybot_alarm_voice_owner_gp __asm__("D_L15_00161AC8") __attribute__((sda));

/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_002A3A48), where it is exact; names translated to the US level program. */

int activate_first_inactive_group_moby(int moby_group_index, void *pos) __asm__("FUN_L15_002a2868");

int activate_first_inactive_group_moby(int moby_group_index, void *pos) {
    short       *group_moby_entry = (short *)D_L15_001ABE40[moby_group_index];
    struct Hero *hero_data = &hero;
    do {
        unsigned char *listed_moby =
            (unsigned char *)l15_moby_array + ((*(unsigned short *)group_moby_entry & 0x7FFF) << 8);
        unsigned char state = listed_moby[0x20];
        if (state == 1) {
            char *data;
            listed_moby[0x20] = 2;
            if (listed_moby[0x53] != 0)
                blend_moby_animation(listed_moby, 0, 0, 0);
            MOBY(listed_moby)->flags |= 1;
            listed_moby[0x31] = 0;
            data = ((char *)MOBY(listed_moby)->pvars);
            MOBY(listed_moby)->collision = *(int *)(((char *)MOBY(listed_moby)->pclass) + 0x10);
            *(float *)(data + 0x64) = random_float_between(-15.0f, 15.0f) * DEG_TO_RAD;
            *(int *)(data + 0x68) = scale_game_frames(0x3C);
            qcopy(listed_moby + 0x10, pos);
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

void sentrybot_alarm_update(unsigned char *moby) {
    float position[4];
    float step;
    int   color;
    int   mode;
    int  *data = ((int *)MOBY(moby)->pvars);
    switch (moby[0x20]) {
    case 0:
        mode = data[0];
        D_L15_00161AB4 = 0;
        D_L15_00161AB8 = 0;
        *(int *)&D_L15_00161ABC_gp = 0;
        if (mode == 2) {
            moby[0x20] = 6;
            if (moby[0x53] != 4)
                blend_moby_animation(moby, 4, 0, 0);
        } else {
            moby[0x20] = 3;
            if (moby[0x53] != 2)
                blend_moby_animation(moby, 2, 0, 0);
        }
        break;
    case 1:
        color = interpolate_color_2a3138(
            (fast_sin(*(float *)(data + 5)) + 1.0f) * 0.5f, 0x801414bc, 0x801414dc);
        step = frame_time;
        MOBY(moby)->glow_color = color;
        *(float *)(data + 5) = fast_add_rotations(*(float *)(data + 5), step * 6.2831855f);
        if (data[2] == 0)
            goto update;
        if (D_L15_0015F5C4 != 2) {
            if (data[0] == 1) {
                moby[0x20] = 6;
                if (moby[0x53] != 4)
                    blend_moby_animation(moby, 4, 0, scale_game_frames(30));
            } else {
                moby[0x20] = 5;
            }
        }
        break;
    case 2:
        if (moby[0x70] & 2) {
            moby[0x20] = 3;
            if (moby[0x53] != 2)
                blend_moby_animation(moby, 2, 0, scale_game_frames(10));
        }
        break;
    case 3:
        if (data[2] == 0)
            goto update;
        if (D_L15_0015F5C4 != 2) {
            moby[0x20] = 4;
            if (moby[0x53] != 3)
                blend_moby_animation(moby, 3, 0, scale_game_frames(10));
        }
        break;
    case 4:
        if (moby[0x70] & 2) {
            if (data[2] == 0 || D_L15_0015F5C4 == 2)
                goto stop_animation;
            if (data[0] == 1) {
                moby[0x20] = 7;
                if (moby[0x53] != 5)
                    blend_moby_animation(moby, 5, 0, scale_game_frames(10));
            } else {
                if (moby[0x53] != 8)
                    blend_moby_animation(moby, 8, 0, scale_game_frames(10));
                moby[0x20] = 5;
            }
        }
        break;
    stop_animation:
        if (moby[0x53] != 8)
            blend_moby_animation(moby, 8, 0, scale_game_frames(10));
        moby[0x20] = 10;
        break;
    case 5:
        if ((moby[0x70] & 2) && moby[0x53] != 0)
            blend_moby_animation(moby, 0, 0, scale_game_frames(10));
        color = interpolate_color_2a3138(
            (fast_sin(*(float *)(data + 5)) + 1.0f) * 0.5f, 0x801414dc, 0x8004048c);
        step = frame_time;
        MOBY(moby)->glow_color = color;
        *(float *)(data + 5) = fast_add_rotations(*(float *)(data + 5), step * 12.566371f);
        if (fast_dec_timer(data + 6) || D_L15_0015F5C4 == 2) {
            set_sentrybot_alarm_duration((char *)moby, 0);
            data[2] = 0;
            if (moby[0x53] != 8)
                blend_moby_animation(moby, 8, 0, scale_game_frames(10));
            moby[0x20] = 10;
        }
        break;
    case 6:
        if (data[2] == 0)
            goto update;
        moby[0x20] = 7;
        if (moby[0x53] != 5)
            blend_moby_animation(moby, 5, 0, scale_game_frames(10));
        break;
    case 7:
        if (moby[0x70] & 2) {
            moby[0x20] = 8;
            if (moby[0x53] != 6)
                blend_moby_animation(moby, 6, 0, scale_game_frames(10));
        }
        break;
    case 8: {
        float *pos = (float *)&MOBY(moby)->pos;
        float *hero_pos = hero.motion.pos.f;
        if (vector_distance_xy(pos, hero_pos) < 16.0f && D_L15_0015F5C4 != 2 &&
            fast_dec_timer(data + 3)) {
            qcopy(position, pos);
            position[2] += 1.0f;
            if (collision_line(position, hero_pos + 20, 2, 0, 0) == 0 &&
                activate_first_inactive_group_moby(data[1], pos))
                data[3] = data[4];
        }
        if (fast_dec_timer(data + 6) || D_L15_0015F5C4 == 2) {
            set_sentrybot_alarm_duration((char *)moby, 0);
            data[2] = 0;
            moby[0x20] = 9;
            if (moby[0x53] != 7)
                blend_moby_animation(moby, 7, 0, scale_game_frames(10));
        }
        break;
    }
    case 9:
        if (moby[0x70] & 2) {
            if (data[0] == 2) {
                moby[0x20] = 6;
                if (moby[0x53] != 4)
                    blend_moby_animation(moby, 4, 0, scale_game_frames(10));
            } else {
                moby[0x20] = 10;
                if (moby[0x53] != 8)
                    blend_moby_animation(moby, 8, 0, scale_game_frames(10));
            }
        }
        break;
    case 10:
        if (moby[0x70] & 2) {
            moby[0x20] = 2;
            if (moby[0x53] != 1)
                blend_moby_animation(moby, 1, 0, scale_game_frames(10));
        }
        break;
    }
    if (data[2] != 0)
        D_L15_00161AB8 = 1;
update:
    if (D_L15_00161AB4 != D_L15_0015F5CC) {
        D_L15_00161AB4 = D_L15_0015F5CC;
        update_sentrybot_alarm_effect(moby);
        D_L15_00161AB8 = 0;
    }
}

/* per-frame update of the level's looping effect: ramp the gauge, then start, run or stop the draw callback */
/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_002CCFC0), where it is exact; names translated to the US level program. */

void update_sentrybot_alarm_effect(void *moby) {
    struct Hero *base = &hero;
    if (base->state.current == 0x65) {
        approach_value(&D_L15_00161ABC_gp, 0.0f, frame_time * 2);
        FUN_L15_002cbac0_u(moby, 0);
    } else if (*(int *)&D_L15_00161AB8 != 0 &&
               (D_0015ED84 != 0xF || 240.0f < base->motion.pos.f[1])) {
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
