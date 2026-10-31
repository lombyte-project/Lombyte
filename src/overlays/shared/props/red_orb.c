/* Shared moby class red_orb: an orb that pulses its colour and draws a glow quad per orb of its group. */

#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/fast_math.h"

typedef struct {
    char  pad[8];
    s32 **p;
} S09;

typedef struct {
    char pad[0x78];
    S09 *s;
} M09;

typedef void (*CB09)(M09 *);

int         get_first_moby_in_group(void *, s32, s32, s32) __asm__("FUN_L00_002591d0");
int         get_next_moby_in_group(void *, s32, s32, s32) __asm__("FUN_L00_002592b8");
void        draw_offset_textured_effect_quad(void *, void *, f32, f32) __asm__("FUN_L00_00263618");
void        FUN_L00_002df3d8_u(M09 *) __asm__("FUN_L00_002df3d8");
void        FUN_001f4600(CB09, M09 *);
extern int  fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern void set_moby_color(void *, int, int, int) __asm__("FUN_L00_002502f0");
extern void FUN_L00_002df4a8_u(void *) __asm__("FUN_L00_002df4a8");

extern s32 D_L00_0015F5CC;
int        red_orb_pulse_color_start_gp __asm__("D_L00_00161B90") = 0x404070;
int        red_orb_pulse_color_end_gp __asm__("D_L00_00161B94") = 0x202020;
float      red_orb_pulse_angular_speed_degrees_gp __asm__("D_L00_00161B98") = 1.8e+02f;

int   D_L02_00161EC0 = 0x404070;
int   D_L02_00161EC4 = 0x202020;
float D_L02_00161EC8 = 1.8e+02f;
int   D_L05_00161F24 = 0x404070;
int   D_L05_00161F28 = 0x202020;
float D_L05_00161F2C = 1.8e+02f;
int   D_L18_00162100 = 0x404070;
int   D_L18_00162104 = 0x202020;
float D_L18_00162108 = 1.8e+02f;

void draw_red_orb_group_effect_quads(struct Moby *reference_moby) __asm__("FUN_L00_002df3d8");

void draw_red_orb_group_effect_quads(struct Moby *reference_moby) {
    u8 *it[2] __attribute__((aligned(16)));
    f32 quad_scale = 1.2f;
    if (current_level_index == 0 || current_level_index == 0x12) {
        quad_scale = 1.0f;
    }
    get_first_moby_in_group(it, reference_moby->group, 0, 0);
    while (it[0] != 0) {
        u8 *candidate_moby = it[0];
        if (candidate_moby[0x31] != 0) {
            void **moby_data = ((void **)MOBY(candidate_moby)->pvars);
            if (MOBY(candidate_moby)->oclass == reference_moby->oclass) {
                draw_offset_textured_effect_quad(
                    candidate_moby + 0x10, moby_data[1], quad_scale, 0.5f);
            }
        }
        get_next_moby_in_group(it, it[0], 0, 0);
    }
}

void enqueue_red_orb_group_draw_if_cache_changed(M09 *moby) __asm__("FUN_L00_002df4a8");

void enqueue_red_orb_group_draw_if_cache_changed(M09 *moby) {
    u8   moby_selection_key;
    s32  current_cache_value;
    S09 *s;
    moby_selection_key = moby->pad[0x21];
    if (moby_selection_key == 0xFF)
        return;
    s = moby->s;
    current_cache_value = D_L00_0015F5CC;
    if (current_cache_value == 0)
        return;
    if (current_cache_value != *s->p) {
        *s->p = current_cache_value;
        FUN_001f4600(FUN_L00_002df3d8_u, moby);
    }
}

/* update for the pulsing colour object */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9438.c: func_L00_002E09A8), where it is exact; names translated to the US level program. */
void red_orb_update(struct Moby *m) __asm__("FUN_L00_002df4f8");

/* Moby class 1060 */
void red_orb_update(struct Moby *moby) {
    enum {
        RED_ORB_STATE_INITIALIZE_PHASE = 0,
        RED_ORB_STATE_PULSE_COLOR = 1
    };
    char         *orb_data = ((char *)moby->pvars);
    unsigned char state = ((char)moby->state);
    float         pulse_phase_or_sine;
    int           tweened_color, hi, r, g, b, packed_color_without_red;
    switch (state) {
    case RED_ORB_STATE_INITIALIZE_PHASE:
        *(float *)orb_data = *(float *)orb_data * DEG_TO_RAD;
        moby->state = 1;
        break;
    case RED_ORB_STATE_PULSE_COLOR:
        pulse_phase_or_sine = fast_add_rotations(
            *(float *)orb_data, red_orb_pulse_angular_speed_degrees_gp * DEG_TO_RAD * frame_time);
        *(float *)orb_data = pulse_phase_or_sine;
        pulse_phase_or_sine = fast_sin(pulse_phase_or_sine);
        tweened_color = fast_tween_color(red_orb_pulse_color_start_gp,
                                         red_orb_pulse_color_end_gp,
                                         pulse_phase_or_sine * 0.5f + 0.5f);
        b = (tweened_color >> 16) & 0xFF;
        g = (tweened_color >> 8) & 0xFF;
        r = tweened_color & 0xFF;
        hi = 0x38;
        if (current_level_index == 0 || current_level_index == 0x12)
            hi = 0x20;
        packed_color_without_red = hi << 24 | b << 16;
        packed_color_without_red |= g << 8;
        *(int *)(orb_data + 4) = packed_color_without_red | r;
        FUN_L00_002df4a8_u(moby);
        set_moby_color(moby,
                       tweened_color & 0xFF,
                       (tweened_color >> 16) & 0xFF,
                       (unsigned)tweened_color >> 24);
        break;
    }
}
