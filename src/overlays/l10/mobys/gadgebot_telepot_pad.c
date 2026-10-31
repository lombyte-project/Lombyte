/* Level 10 moby class gadgebot_telepot_pad. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/camera/follow_cam.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"

#include "rnc/overlay/l10/mobys/gadgebot_telepot_pad_globals.h"

#include "qcopy.h"

extern void set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");

/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_00296BD8.c: func_L10_002BFBB8), where it is exact; names translated to the US level program. */

typedef unsigned int Q_2BFBB8 __attribute__((mode(TI), aligned(16)));

extern struct Moby *D_L10_0015FFD8;
extern char         D_0013F350[];
extern char         D_L10_00167100[];
extern float        D_0015ED6C;
extern float        D_0015ED70;
extern float        vector_distance_xy_alt(void *, void *) __asm__("FUN_001f9b80");
extern int          D_L10_0015F5CC;
extern int          gadgebot_telepot_camera_effect_duration_frames_gp __asm__("D_L10_001618A0")
    __attribute__((sda));
extern int gadgebot_telepot_camera_spring_decay_frames_gp __asm__("D_L10_001618A4")
    __attribute__((sda));
extern int   FUN_L00_00257b90(int, int);
extern int   tick_countdown_32_alt(int *) __asm__("FUN_001f9740");
extern s32   func_001FA6E0(s32, s32, f32);
extern void *spawn_moving_resizing_height_snapped_alpha_fade_particle(
    void *, int, int, float, float) __asm__("FUN_L00_002715e8");
extern void *FUN_L00_00272060(void *, int, float, float, int, int, int, void *, float);
extern void  add_vectors(void *, void *, void *) __asm__("FUN_L00_001ff290");
extern void  set_follow_camera_orbit_clamp_mode(int) __asm__("FUN_L00_002e8568");
extern void  set_follow_camera_blend_output_flags(void) __asm__("FUN_L00_002e8680");

#define func_001F9850_2BFBB8     ((int (*)(int))scale_game_frames)
#define func_001F9D48_2BFBB8     ((float (*)(void *, void *))vector_distance_xy_alt)
#define func_002140F8_2BFBB8     ((float (*)(float, float))random_float_between)
#define func_00214158_2BFBB8     ((float (*)(void))random_angle_radians)
#define func_002140B0_2BFBB8     ((int (*)(int))random_integer_below)
#define func_L00_00258BC8_2BFBB8 ((int (*)(int, int))FUN_L00_00257b90)
#define func_001F9F90_2BFBB8     ((float (*)(float))fast_cos)
#define func_001F9FA8_2BFBB8     ((float (*)(float))fast_sin)
#define func_L00_001FF4B0_2BFBB8 ((void (*)(void *, void *, float))fast_vec_normalize)
#define func_L00_00272F00_2BFBB8 FUN_L00_00272060
#define func_L00_00272488_2BFBB8 spawn_moving_resizing_height_snapped_alpha_fade_particle
#define func_0022ED80_2BFBB8     ((int (*)(int, int, int))allocate_voice_for_target_entry)
#define func_L00_00217718_2BFBB8 set_race_camera
#define func_001FA888_2BFBB8     ((float (*)(int))convert_integer_to_float)
#define func_001FA898_2BFBB8     ((int (*)(float))truncate_float_to_s32)
#define func_001FA8A8_2BFBB8     func_001FA6E0
#define func_001F9908_2BFBB8     ((int (*)(int *))tick_countdown_32_alt)
#define func_L00_002E9A40_2BFBB8 follow_camera_set_lateral_spring
#define func_L00_002E9B30_2BFBB8 set_follow_camera_blend_output_flags
#define func_L00_002E9AF8_2BFBB8 follow_camera_start_slide
#define func_L00_002E9A18_2BFBB8 set_follow_camera_orbit_clamp_mode

void gadgebot_telepot_pad_update(char *moby) __asm__("FUN_L10_002be858");

/* Moby classes 351, 1301 */
void gadgebot_telepot_pad_update(char *moby) {
    int   *telepot_pad_vars = ((int *)MOBY(moby)->pvars);
    float  position[4];
    float  velocity[4];
    float  offset_buffer[4];
    float  angle;
    float *offset;
    int    i;
    char  *hero_bytes;
    int    life;
    if (MOBY(moby)->state == 0) {
        gadgebot_telepot_camera_effect_remaining_frames = 0;
        gadgebot_telepot_camera_spring_decay_frames_gp =
            func_001F9850_2BFBB8(gadgebot_telepot_camera_spring_decay_frames_gp);
        gadgebot_telepot_camera_effect_duration_frames_gp =
            func_001F9850_2BFBB8(gadgebot_telepot_camera_effect_duration_frames_gp);
        MOBY(moby)->state = 1;
    }
    hero_bytes = D_0013F350;
    if ((unsigned char)hero_bytes[0x20A4] == 1 && *telepot_pad_vars != -1 &&
        func_001F9D48_2BFBB8(moby + 0x10, hero_bytes + 0x80) < 0.5f) {
        offset = offset_buffer;
        i = 9;
        do {
            float size, size_scaled;
            int   red, green, blue, color, spin, green_alpha;
            i--;
            size = func_002140F8_2BFBB8(0.1f, 1.0f);
            red = func_L00_00258BC8_2BFBB8(0x40, 0xff);
            green = func_L00_00258BC8_2BFBB8(0x40, 0xff);
            blue = func_L00_00258BC8_2BFBB8(0x40, 0xff);
            velocity[0] =
                func_001F9F90_2BFBB8(func_00214158_2BFBB8()) * func_002140F8_2BFBB8(0.0f, 0.5f);
            velocity[1] =
                func_001F9FA8_2BFBB8(func_00214158_2BFBB8()) * func_002140F8_2BFBB8(0.0f, 0.5f);
            velocity[2] = 0.0f;
            velocity[2] += func_002140F8_2BFBB8(0.0f, 0.5f);
            add_vectors(offset, velocity, D_0013F350 + 0x80);
            func_L00_001FF4B0_2BFBB8(position, position, func_002140F8_2BFBB8(0.0f, D_0015ED6C));
            position[2] = func_002140F8_2BFBB8(D_0015ED6C * 2.5f, D_0015ED6C * 5.0f);
            life = func_001F9850_2BFBB8(func_L00_00258BC8_2BFBB8(15, 30));
            green_alpha = (green << 8) | 0x7f000000;
            color = (blue << 16) | green_alpha | red;
            size_scaled = size * 0.07f;
            size *= 0.7f;
            spin = func_002140B0_2BFBB8(2) ? 1 : -1;
            func_L00_00272F00_2BFBB8(
                velocity, life, size_scaled, size, color, 0, spin, position, D_0015ED70 * 15.0f);
        } while (i >= 0);
        func_0022ED80_2BFBB8(0, 0, (int)moby);
        {
            int   moby_offset = *telepot_pad_vars << 8;
            char *effect = (*(char **)&D_L10_0015FFD8) + moby_offset;
            func_L00_00217718_2BFBB8(effect + 0x10, effect + 0x40, 0x43, 0);
        }
        {
            char *table = D_L10_00167100;
            char *effect = *(char **)(table + 0x180);
            gadgebot_telepot_camera_effect_remaining_frames =
                gadgebot_telepot_camera_effect_duration_frames_gp;
            if (*(short *)(effect + 0x86) == 0) {
                char  *current = *(char **)(table + 0x180);
                short *timer = (short *)(*(char **)(current + 0x70) + 0x20);
                *timer = func_001F9850_2BFBB8(0x5a);
                qcopy(current + 0x40, current);
            }
        }
        i = 9;
        do {
            float size, size_scaled;
            int   red, green, blue, color, spin, green_alpha;
            *(Q_2BFBB8 *)velocity = 0;
            velocity[0] = func_002140F8_2BFBB8(-1.0f, 1.0f);
            i--;
            velocity[1] = func_002140F8_2BFBB8(-1.0f, 1.0f);
            *(Q_2BFBB8 *)position = *(Q_2BFBB8 *)velocity;
            size = func_002140F8_2BFBB8(0.1f, 1.0f);
            red = func_L00_00258BC8_2BFBB8(0x40, 0xff);
            green = func_L00_00258BC8_2BFBB8(0x40, 0xff);
            blue = func_L00_00258BC8_2BFBB8(0x40, 0xff);
            velocity[0] =
                func_001F9F90_2BFBB8(func_00214158_2BFBB8()) * func_002140F8_2BFBB8(0.0f, 0.5f);
            velocity[1] =
                func_001F9FA8_2BFBB8(func_00214158_2BFBB8()) * func_002140F8_2BFBB8(0.0f, 0.5f);
            velocity[2] = 0.0f;
            velocity[2] += func_002140F8_2BFBB8(0.0f, 0.5f);
            add_vectors(
                offset, velocity, (*(char **)&D_L10_0015FFD8) + (*telepot_pad_vars << 8) + 0x10);
            func_L00_001FF4B0_2BFBB8(position, position, func_002140F8_2BFBB8(0.0f, D_0015ED6C));
            position[2] = func_002140F8_2BFBB8(D_0015ED6C * 2.5f, D_0015ED6C * 5.0f);
            life = func_001F9850_2BFBB8(func_L00_00258BC8_2BFBB8(15, 30));
            green_alpha = (green << 8) | 0x7f000000;
            color = (blue << 16) | green_alpha | red;
            size_scaled = size * 0.07f;
            size *= 0.7f;
            spin = func_002140B0_2BFBB8(2) ? 1 : -1;
            func_L00_00272F00_2BFBB8(
                velocity, life, size_scaled, size, color, 0, spin, position, D_0015ED70 * 15.0f);
        } while (i >= 0);
    }
    if (D_L10_0015F5CC % func_001F9850_2BFBB8(0x3c) == 0 && *telepot_pad_vars != -1) {
        for (i = 0; i < func_001FA898_2BFBB8(16.0f); i++) {
            char *effect;
            *(Q_2BFBB8 *)position = *(Q_2BFBB8 *)&MOBY(moby)->pos;
            position[2] += func_001FA888_2BFBB8(i) * 0.1f * 0.0625f + 1.8f;
            effect = (char *)func_L00_00272488_2BFBB8(
                position, 0, 0x1eff7f40, 0.0f, D_0015ED6C * 315000.0f);
            if (effect != 0) {
                float *periodic_effect_velocity = (float *)(effect + 0x20);
                float  z = D_0015ED6C * -1.5f;
                periodic_effect_velocity[0] = 0.0f;
                periodic_effect_velocity[1] = 0.0f;
                periodic_effect_velocity[2] = z;
            }
        }
    }
    {
        float phase = (float)(D_L10_0015F5CC % 150) / 150.0f;
        phase += phase;
        angle = phase * 3.1415927f;
    }
    if (3.1415927f < angle)
        angle -= 6.2831855f;
    MOBY(moby)->glow_color =
        func_001FA8A8_2BFBB8(0x80464646, 0x80828282, (func_001F9FA8_2BFBB8(angle) + 1.0f) * 0.5f);
    func_001F9908_2BFBB8(&gadgebot_telepot_camera_effect_remaining_frames);
    if (gadgebot_telepot_camera_effect_remaining_frames != 0) {
        if (gadgebot_telepot_camera_spring_decay_frames_gp <
            gadgebot_telepot_camera_effect_remaining_frames)
            angle = 0.002f;
        else
            angle = func_001FA888_2BFBB8(gadgebot_telepot_camera_effect_remaining_frames) /
                        func_001FA888_2BFBB8(gadgebot_telepot_camera_spring_decay_frames_gp) *
                        0.007999999f +
                    0.002f;
        func_L00_002E9A40_2BFBB8(angle, 0.2f);
        func_L00_002E9B30_2BFBB8();
        func_L00_002E9AF8_2BFBB8();
        func_L00_002E9A18_2BFBB8(0);
    }
}
