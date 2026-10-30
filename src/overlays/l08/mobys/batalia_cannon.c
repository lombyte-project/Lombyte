/* Level 08 moby class 0x1b8: batalia cannon. */
#include "types.h"
#include "rnc/input/pad_globals.h"
#include "rnc/game/game_globals.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/mobys/moby.h"
#include "sda.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/game/options.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/hero/hero_switch_item.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/camera/scripted_camera.h"
#include "rnc/overlay/shared/camera/follow_camera_set_up_spring.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/overlay/l08/hero/hero_set_state.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/mobys/moby_spawn.h"

/* finds the first of eight linked entries whose direction lines up with the moby */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E15A0), where it is exact; names translated to the US level program. */
extern char D_L08_001675C0[];
int         find_batalia_cannon_aim_target(struct Moby *moby) __asm__("FUN_L08_002e0228");

/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E2A10), where it is exact; names translated to the US level program. */
/* Sets the moby's byte 0xBC and calls hero_set_state(0x32, 1). */
void request_batalia_cannon_mount(struct Moby *command_target_moby) __asm__("FUN_L08_002e1698");

#include "qcopy.h"

/* Re-aims the twelve child mobys of a parent, adjusts the parent's spin, then runs the children's follow-up. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E9F78), where it is exact; names translated to the US level program. */
extern float D_0015ED6C;

#include "rnc/hero/hero.h"

/* Draws the Batalia cannon's HUD: a four-part frame, markers for the eight linked and four indexed targets
   (relative to the cannon's heading), the inner frame, a 32-segment health dial and two rotating needles. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E0258), where it is exact; names translated to the US level program. */
typedef struct Moby    Moby;
typedef struct RData51 RData51;

struct RData51 {
    char  pad0[0x58];
    int   health, pad5C;
    Moby *linked[8];
    int   indices[4];
    char  pad90[0x10];
    int   timer;
};

/* no foreign declaration */
extern f32 D_L08_00161AC0_gp __asm__("D_L08_00161AC0") __attribute__((sda));

/* no foreign declaration */
extern int D_L08_00161AC8_gp __asm__("D_L08_00161AC8") __attribute__((sda));

/* no foreign declaration */
extern int D_L08_00161AD4_gp __asm__("D_L08_00161AD4") __attribute__((sda));

/* no foreign declaration */
extern int D_L08_00161AD8_gp __asm__("D_L08_00161AD8") __attribute__((sda));

/* no foreign declaration */
extern int D_L08_00161ADC_gp __asm__("D_L08_00161ADC") __attribute__((sda));

/* no foreign declaration */
extern f32 D_L08_00161B60_gp __asm__("D_L08_00161B60") __attribute__((sda));

/* no foreign declaration */
extern f32 D_L08_00161B64_gp __asm__("D_L08_00161B64") __attribute__((sda));

/* no foreign declaration */
extern f32 D_L08_00161B68_gp __asm__("D_L08_00161B68") __attribute__((sda));

/* no foreign declaration */
extern f32 D_L08_00161B6C_gp __asm__("D_L08_00161B6C") __attribute__((sda));

/* no foreign declaration */
extern f32 D_L08_00161B70_gp __asm__("D_L08_00161B70") __attribute__((sda));

/* no foreign declaration */
extern f32 D_L08_00161B74_gp __asm__("D_L08_00161B74") __attribute__((sda));

/* no foreign declaration */
extern f32 D_L08_00161B78_gp __asm__("D_L08_00161B78") __attribute__((sda));

/* no foreign declaration */
extern f32 D_L08_00161B7C_gp __asm__("D_L08_00161B7C") __attribute__((sda));

/* no foreign declaration */
extern f32 D_L08_00161B80_gp __asm__("D_L08_00161B80") __attribute__((sda));

/* no foreign declaration */
extern f32 D_L08_00161B84_gp __asm__("D_L08_00161B84") __attribute__((sda));

/* no foreign declaration */
extern f32 D_L08_00161B88_gp __asm__("D_L08_00161B88") __attribute__((sda));

/* no foreign declaration */
extern f32 D_L08_00161B8C_gp __asm__("D_L08_00161B8C") __attribute__((sda));

/* no foreign declaration */
extern f32 D_L08_00161B90_gp __asm__("D_L08_00161B90") __attribute__((sda));

/* no foreign declaration */
extern f32 D_L08_00161B94_gp __asm__("D_L08_00161B94") __attribute__((sda));

/* no foreign declaration */
extern f32 D_L08_00161B98_gp __asm__("D_L08_00161B98") __attribute__((sda));

/* no foreign declaration */
extern f32 D_L08_00161B9C_gp __asm__("D_L08_00161B9C") __attribute__((sda));

/* no foreign declaration */
extern int D_L08_00161BA0_gp __asm__("D_L08_00161BA0") __attribute__((sda));

/* no foreign declaration */
extern int D_L08_00161BA4_gp __asm__("D_L08_00161BA4") __attribute__((sda));

/* no foreign declaration */
extern int D_L08_00161BA8_gp __asm__("D_L08_00161BA8") __attribute__((sda));

/* no foreign declaration */
extern int D_L08_00161BAC_gp __asm__("D_L08_00161BAC") __attribute__((sda));

/* no foreign declaration */
extern int D_L08_00161BB0_gp __asm__("D_L08_00161BB0") __attribute__((sda));

/* no foreign declaration */
extern int D_L08_00161BB4_gp __asm__("D_L08_00161BB4") __attribute__((sda));

/* no foreign declaration */
extern int D_L08_00161BB8_gp __asm__("D_L08_00161BB8") __attribute__((sda));

/* no foreign declaration */
extern int D_L08_00161BBC_gp __asm__("D_L08_00161BBC") __attribute__((sda));

/* no foreign declaration */
extern int D_L08_00161BC0_gp __asm__("D_L08_00161BC0") __attribute__((sda));

/* no foreign declaration */
extern int D_L08_00161BC4_gp __asm__("D_L08_00161BC4") __attribute__((sda));

/* no foreign declaration */
extern int D_L08_00161BC8_gp __asm__("D_L08_00161BC8") __attribute__((sda));

/* no foreign declaration */
extern int D_L08_00161BCC_gp __asm__("D_L08_00161BCC") __attribute__((sda));

/* no foreign declaration */
extern int D_L08_00161BD0_gp __asm__("D_L08_00161BD0") __attribute__((sda));

/* no foreign declaration */
extern int D_L08_00161BD4_gp __asm__("D_L08_00161BD4") __attribute__((sda));

/* no foreign declaration */
extern int D_L08_00161BD8_gp __asm__("D_L08_00161BD8") __attribute__((sda));

/* no foreign declaration */
extern int D_L08_00161BDC_gp __asm__("D_L08_00161BDC") __attribute__((sda));

/* no foreign declaration */
extern int D_L08_00161BE0_gp __asm__("D_L08_00161BE0") __attribute__((sda));

/* no foreign declaration */
extern int D_L08_00161BE4_gp __asm__("D_L08_00161BE4") __attribute__((sda));

/* no foreign declaration */
extern int D_L08_00161BE8_gp __asm__("D_L08_00161BE8") __attribute__((sda));

/* no foreign declaration */
extern int   D_L08_00161BEC_gp __asm__("D_L08_00161BEC") __attribute__((sda));
extern s64   get_effect_texture(s32) __asm__("func_001F44B8");
extern void  vu1_add_g_sregister(s32, u64) __asm__("func_00233980");
extern void  FUN_L02_0020bc88_c(void *, void *, void *, long, int) __asm__("FUN_L02_0020bc88");
extern int   func_001FA8A8_2E0258(int, int, float) __asm__("FUN_001fa6e0");
extern char *D_L08_0015FFD8;
extern int   D_L08_0015F5CC __asm__("D_L08_0015F5CC");
extern unsigned char draw_map51[4] __asm__("D_0015EDB4");
extern float         D_L08_00161AC0 __asm__("D_L08_00161AC0");
extern float         D_L08_00161B60 __asm__("D_L08_00161B60");
extern float         D_L08_00161B64 __asm__("D_L08_00161B64");
extern float         D_L08_00161B68 __asm__("D_L08_00161B68");
extern float         D_L08_00161B6C __asm__("D_L08_00161B6C");
extern float         D_L08_00161B70 __asm__("D_L08_00161B70");
extern float         D_L08_00161B74 __asm__("D_L08_00161B74");
extern float         D_L08_00161B78 __asm__("D_L08_00161B78");
extern float         D_L08_00161B7C __asm__("D_L08_00161B7C");
extern float         D_L08_00161B80 __asm__("D_L08_00161B80");
extern float         D_L08_00161B84 __asm__("D_L08_00161B84");
extern float         D_L08_00161B88 __asm__("D_L08_00161B88");
extern float         D_L08_00161B8C __asm__("D_L08_00161B8C");
extern float         D_L08_00161B90 __asm__("D_L08_00161B90");
extern float         D_L08_00161B94 __asm__("D_L08_00161B94");
extern float         D_L08_00161B98 __asm__("D_L08_00161B98");
extern float         D_L08_00161B9C __asm__("D_L08_00161B9C");
extern int           D_L08_00161AC8 __asm__("D_L08_00161AC8");
extern int           D_L08_00161AD4 __asm__("D_L08_00161AD4");
extern int           D_L08_00161AD8 __asm__("D_L08_00161AD8");
extern int           D_L08_00161ADC __asm__("D_L08_00161ADC");
extern int           D_L08_00161BA0 __asm__("D_L08_00161BA0");
extern int           D_L08_00161BA4 __asm__("D_L08_00161BA4");
extern int           D_L08_00161BA8 __asm__("D_L08_00161BA8");
extern int           D_L08_00161BAC __asm__("D_L08_00161BAC");
extern int           D_L08_00161BB0 __asm__("D_L08_00161BB0");
extern int           D_L08_00161BB4 __asm__("D_L08_00161BB4");
extern int           D_L08_00161BB8 __asm__("D_L08_00161BB8");
extern int           D_L08_00161BBC __asm__("D_L08_00161BBC");
extern int           D_L08_00161BC0 __asm__("D_L08_00161BC0");
extern int           D_L08_00161BC4 __asm__("D_L08_00161BC4");
extern int           D_L08_00161BC8 __asm__("D_L08_00161BC8");
extern int           D_L08_00161BCC __asm__("D_L08_00161BCC");
extern int           D_L08_00161BD0 __asm__("D_L08_00161BD0");
extern int           D_L08_00161BD4 __asm__("D_L08_00161BD4");
extern int           D_L08_00161BD8 __asm__("D_L08_00161BD8");
extern int           D_L08_00161BDC __asm__("D_L08_00161BDC");
extern int           D_L08_00161BE0 __asm__("D_L08_00161BE0");
extern int           D_L08_00161BE4 __asm__("D_L08_00161BE4");
extern int           D_L08_00161BE8 __asm__("D_L08_00161BE8");
extern int           D_L08_00161BEC __asm__("D_L08_00161BEC");
void                 draw_batalia_cannon_hud(Moby *moby) __asm__("FUN_L08_002deee0");

/* Updates the mounted Batalia cannon: entry/exit, aiming, alternating barrels, waves, and attached visuals. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E16A0), where it is exact; names translated to the US level program. */
typedef struct CannonData52 CannonData52;

struct CannonData52 {
    char          pad0[0x40];
    short         fire_timer, exit_timer;
    int           pad44;
    float         pitch_step, yaw_step;
    short         pad50, camera_timer, barrel, pad56;
    int           health;
    short         wave_timer;
    unsigned char target_index, target_timer;
    Moby         *linked[8];
    int           indices[4];
    Moby         *guns[2];
    float         range;
    int           platform_index, lock_timer;
    Moby         *visual;
    float         visual_angle;
};

char        *spawn_turret_rocket(int    owner,
                                 float *position,
                                 float *rotation,
                                 int    value) __asm__("FUN_L08_002e7eb0");
extern float D_0015ED70;
extern float D_L08_00161AE8_gp __asm__("D_L08_00161AE8") __attribute__((sda));
extern float D_L08_00161AEC_gp __asm__("D_L08_00161AEC") __attribute__((sda));
extern float D_L08_00161AF0_gp __asm__("D_L08_00161AF0") __attribute__((sda));
extern float D_L08_00161AF4_gp __asm__("D_L08_00161AF4") __attribute__((sda));
extern float D_L08_00161AF8_gp __asm__("D_L08_00161AF8") __attribute__((sda));
extern float D_L08_00161AFC_gp __asm__("D_L08_00161AFC") __attribute__((sda));
extern float D_L08_00161B00_gp __asm__("D_L08_00161B00") __attribute__((sda));
extern s32   D_0015ED84;
extern char  D_0013A5E0_2E16A0[] __asm__("D_0013CAE4");
extern int   D_0015ED84;
extern short D_L08_00161A90_gp __asm__("D_L08_00161A90") __attribute__((sda));
extern short D_L08_00161B10_gp __asm__("D_L08_00161B10") __attribute__((sda));
extern short D_L08_00161B20_gp __asm__("D_L08_00161B20") __attribute__((sda));
extern short D_L08_00161B30_gp __asm__("D_L08_00161B30") __attribute__((sda));
extern short D_L08_00161B40_gp __asm__("D_L08_00161B40") __attribute__((sda));
extern short D_L08_00161B50_gp __asm__("D_L08_00161B50") __attribute__((sda));
extern short D_L08_00161BF0_gp __asm__("D_L08_00161BF0") __attribute__((sda));
extern void  func_L00_001FF328_2E16A0(void *, void *, float) __asm__("FUN_L00_001ff378");
extern void  func_L00_00217718_2E16A0(void *, void *, int, int) __asm__("FUN_L00_00216f90");
void         set_batalia_cannon_platform_mode(u8 *platform, s32 mode) __asm__("FUN_L08_00306b30");
extern float approach_value(float *, float, float) __asm__("FUN_00213ed8");
extern char *func_L00_0026EBC0_2E16A0(char *, char *, int, int, float) __asm__("FUN_L00_0026dd20");
extern void  snap_scripted_camera_to_pose(void) __asm__("FUN_L08_00313ba8");
extern unsigned char    l08_cannon_hero_interaction_flags[] __asm__("D_0014095F");
void                    update_batalia_cannon(Moby *moby) __asm__("FUN_L08_002e0328");
extern float D_0015ED70 MACRO_ADDR;

int D_L08_00161AD4 = 0x600000ff;

int D_L08_00161AD8 = 0x600080ff;

int D_L08_00161ADC = 0x60ffffff;

void draw_batalia_cannon_hud(Moby *moby) {
    long     xy[4];
    int      color_words[4], uv_words[4];
    float    relative[4];
    RData51 *data = (*(RData51 **)&moby->pvars);
    long     texture;
    int      segments;
    int      i, j, k, indexed_target_slot, sign, color;
    int     *indices;
    float    sign_x, sign_y, angle, radius, fraction, next_angle;
    int      outer_x, outer_y, inner_x, inner_y;
    int      color_a, color_b;
    long     base;
    vu1_add_g_sregister(0x42, 0x8000000044L);
    texture = get_effect_texture(D_L08_00161BA0 + 0x28);
    uv_words[0] = D_L08_00161BB0;
    uv_words[1] = D_L08_00161BB4;
    uv_words[2] = D_L08_00161BB8;
    uv_words[3] = D_L08_00161BBC;
    color_words[0] = color_words[1] = color_words[2] = color_words[3] = D_L08_00161AC8;
    for (i = 0; i < 4; i++) {
        sign_x = (i & 1) ? -1.0f : 1.0f;
        sign_y = (i >= 2) ? -1.0f : 1.0f;
        xy[3] = 0xFFFFF000000000L;
        xy[2] = 0xFFFFF000000000L;
        xy[1] = 0xFFFFF000000000L;
        xy[0] = 0xFFFFF000000000L;
        xy[0] += (unsigned int)((truncate_float_to_s32((D_L08_00161B84 + D_L08_00161B8C * sign_y) *
                                                       16.0f)
                                 << 16) -
                                (int)0x80000000);
        xy[0] += (truncate_float_to_s32(D_L08_00161B80 + D_L08_00161B88 * sign_x) << 4) + 0x8000;
        xy[1] +=
            (unsigned int)((truncate_float_to_s32(D_L08_00161B84 * 16.0f) << 16) - (int)0x80000000);
        xy[1] += (truncate_float_to_s32(D_L08_00161B80 + D_L08_00161B88 * sign_x) << 4) + 0x8000;
        xy[2] += (unsigned int)((truncate_float_to_s32((D_L08_00161B84 + D_L08_00161B8C * sign_y) *
                                                       16.0f)
                                 << 16) -
                                (int)0x80000000);
        xy[2] += (truncate_float_to_s32(D_L08_00161B80) << 4) + 0x8000;
        xy[3] +=
            (unsigned int)((truncate_float_to_s32(D_L08_00161B84 * 16.0f) << 16) - (int)0x80000000);
        xy[3] += (truncate_float_to_s32(D_L08_00161B80) << 4) + 0x8000;
        FUN_L02_0020bc88_c(xy, uv_words, color_words, texture, 1);
    }
    texture = get_effect_texture(D_L08_00161BA4);
    uv_words[0] = D_L08_00161BC0;
    uv_words[1] = D_L08_00161BC4;
    uv_words[2] = D_L08_00161BC8;
    uv_words[3] = D_L08_00161BCC;
    for (k = 0; k < 8; k++) {
        float angle, radius;
        if (data->linked[k]) {
            if ((signed char)data->linked[k]->state >= 0) {
                if (data->linked[k]->state == 2) {
                    if (D_L08_0015F5CC & 0x10) {
                        color_words[0] = color_words[1] = color_words[2] = color_words[3] =
                            D_L08_00161AD4;
                    } else {
                        color_words[0] = color_words[1] = color_words[2] = color_words[3] =
                            0x60000000;
                    }
                } else {
                    color_words[0] = color_words[1] = color_words[2] = color_words[3] =
                        D_L08_00161ADC;
                }
                angle = fast_subtract_rotations(
                    fast_arc_tan((*(float(*)[4]) & data->linked[k]->pos)[0] -
                                     (*(float(*)[4]) & moby->pos)[0],
                                 (*(float(*)[4]) & data->linked[k]->pos)[1] -
                                     (*(float(*)[4]) & moby->pos)[1]),
                    (*(float(*)[4]) & moby->rot)[2]);
                radius = fast_vec_dist((*(float(*)[4]) & data->linked[k]->pos),
                                       (*(float(*)[4]) & moby->pos)) *
                         D_L08_00161AC0;
                sign = draw_map51[0] ? -1 : 1;
                relative[0] = (float)-sign * fast_sin(angle) * radius;
                relative[1] = -fast_cos(angle) * radius;
                xy[3] = 0xFFFFF000000000L;
                xy[2] = 0xFFFFF000000000L;
                xy[1] = 0xFFFFF000000000L;
                xy[0] = 0xFFFFF000000000L;
                xy[0] +=
                    (unsigned int)((truncate_float_to_s32(
                                        (D_L08_00161B84 + D_L08_00161B94) * 16.0f + relative[1])
                                    << 16) -
                                   (int)0x80000000);
                xy[0] +=
                    truncate_float_to_s32((D_L08_00161B80 + D_L08_00161B90) * 16.0f + relative[0]) +
                    0x8000;
                xy[1] +=
                    (unsigned int)((truncate_float_to_s32(
                                        (D_L08_00161B84 - D_L08_00161B94) * 16.0f + relative[1])
                                    << 16) -
                                   (int)0x80000000);
                xy[1] +=
                    truncate_float_to_s32((D_L08_00161B80 + D_L08_00161B90) * 16.0f + relative[0]) +
                    0x8000;
                xy[2] +=
                    (unsigned int)((truncate_float_to_s32(
                                        (D_L08_00161B84 + D_L08_00161B94) * 16.0f + relative[1])
                                    << 16) -
                                   (int)0x80000000);
                xy[2] +=
                    truncate_float_to_s32((D_L08_00161B80 - D_L08_00161B90) * 16.0f + relative[0]) +
                    0x8000;
                xy[3] +=
                    (unsigned int)((truncate_float_to_s32(
                                        (D_L08_00161B84 - D_L08_00161B94) * 16.0f + relative[1])
                                    << 16) -
                                   (int)0x80000000);
                xy[3] +=
                    truncate_float_to_s32((D_L08_00161B80 - D_L08_00161B90) * 16.0f + relative[0]) +
                    0x8000;
                FUN_L02_0020bc88_c(xy, uv_words, color_words, texture, 1);
            } else
                data->linked[k] = 0;
        }
    }
    indices = data->indices;
    for (indexed_target_slot = 0; indexed_target_slot < 4; indexed_target_slot++) {
        float angle, radius;
        if (((Moby *)D_L08_0015FFD8)[*indices].state < 0x7F) {
            color_words[0] = color_words[1] = color_words[2] = color_words[3] = D_L08_00161AD8;
            angle = fast_subtract_rotations(
                fast_arc_tan((*(float(*)[4]) & ((Moby *)D_L08_0015FFD8)[*indices].pos)[0] -
                                 (*(float(*)[4]) & moby->pos)[0],
                             (*(float(*)[4]) & ((Moby *)D_L08_0015FFD8)[*indices].pos)[1] -
                                 (*(float(*)[4]) & moby->pos)[1]),
                (*(float(*)[4]) & moby->rot)[2]);
            radius = fast_vec_dist((*(float(*)[4]) & ((Moby *)D_L08_0015FFD8)[*indices].pos),
                                   (*(float(*)[4]) & moby->pos)) *
                     D_L08_00161AC0;
            relative[0] = -fast_sin(angle) * radius;
            relative[1] = -fast_cos(angle) * radius;
            xy[3] = 0xFFFFF000000000L;
            xy[2] = 0xFFFFF000000000L;
            xy[1] = 0xFFFFF000000000L;
            xy[0] = 0xFFFFF000000000L;
            xy[0] += (unsigned int)((truncate_float_to_s32(
                                         (D_L08_00161B84 + D_L08_00161B9C) * 16.0f + relative[1])
                                     << 16) -
                                    (int)0x80000000);
            xy[0] +=
                truncate_float_to_s32((D_L08_00161B80 + D_L08_00161B98) * 16.0f + relative[0]) +
                0x8000;
            xy[1] += (unsigned int)((truncate_float_to_s32(
                                         (D_L08_00161B84 - D_L08_00161B9C) * 16.0f + relative[1])
                                     << 16) -
                                    (int)0x80000000);
            xy[1] +=
                truncate_float_to_s32((D_L08_00161B80 + D_L08_00161B98) * 16.0f + relative[0]) +
                0x8000;
            xy[2] += (unsigned int)((truncate_float_to_s32(
                                         (D_L08_00161B84 + D_L08_00161B9C) * 16.0f + relative[1])
                                     << 16) -
                                    (int)0x80000000);
            xy[2] +=
                truncate_float_to_s32((D_L08_00161B80 - D_L08_00161B98) * 16.0f + relative[0]) +
                0x8000;
            xy[3] += (unsigned int)((truncate_float_to_s32(
                                         (D_L08_00161B84 - D_L08_00161B9C) * 16.0f + relative[1])
                                     << 16) -
                                    (int)0x80000000);
            xy[3] +=
                truncate_float_to_s32((D_L08_00161B80 - D_L08_00161B98) * 16.0f + relative[0]) +
                0x8000;
            FUN_L02_0020bc88_c(xy, uv_words, color_words, texture, 1);
        }
        indices++;
    }
    uv_words[0] = D_L08_00161BB0;
    uv_words[1] = D_L08_00161BB4;
    uv_words[2] = D_L08_00161BB8;
    uv_words[3] = D_L08_00161BBC;
    color_words[0] = color_words[1] = color_words[2] = color_words[3] = D_L08_00161AC8;
    texture = get_effect_texture(D_L08_00161BA0 + 0x28);
    for (i = 0; i < 4; i++) {
        sign_x = (i & 1) ? -1.0f : 1.0f;
        sign_y = (i >= 2) ? -1.0f : 1.0f;
        xy[3] = 0xFFFFF000000000L;
        xy[2] = 0xFFFFF000000000L;
        xy[1] = 0xFFFFF000000000L;
        xy[0] = 0xFFFFF000000000L;
        xy[0] += (unsigned int)((truncate_float_to_s32((D_L08_00161B64 + D_L08_00161B6C * sign_y) *
                                                       16.0f)
                                 << 16) -
                                (int)0x80000000);
        xy[0] += (truncate_float_to_s32(D_L08_00161B60 + D_L08_00161B68 * sign_x) << 4) + 0x8000;
        xy[1] +=
            (unsigned int)((truncate_float_to_s32(D_L08_00161B64 * 16.0f) << 16) - (int)0x80000000);
        xy[1] += (truncate_float_to_s32(D_L08_00161B60 + D_L08_00161B68 * sign_x) << 4) + 0x8000;
        xy[2] += (unsigned int)((truncate_float_to_s32((D_L08_00161B64 + D_L08_00161B6C * sign_y) *
                                                       16.0f)
                                 << 16) -
                                (int)0x80000000);
        xy[2] += (truncate_float_to_s32(D_L08_00161B60) << 4) + 0x8000;
        xy[3] +=
            (unsigned int)((truncate_float_to_s32(D_L08_00161B64 * 16.0f) << 16) - (int)0x80000000);
        xy[3] += (truncate_float_to_s32(D_L08_00161B60) << 4) + 0x8000;
        FUN_L02_0020bc88_c(xy, uv_words, color_words, texture, 1);
    }
    texture = get_effect_texture(D_L08_00161BA4);
    uv_words[0] = D_L08_00161BD0;
    uv_words[1] = D_L08_00161BD4;
    uv_words[2] = D_L08_00161BD8;
    uv_words[3] = D_L08_00161BDC;
    if ((float)data->health > 100.0f)
        color =
            func_001FA8A8_2E0258(0x6000FFFF, 0x6000FF00, ((float)data->health - 100.0f) / 100.0f);
    else
        color = func_001FA8A8_2E0258(0x600000FF, 0x6000FFFF, (float)data->health / 100.0f);
    segments = truncate_float_to_s32((1.0f - (float)data->health / 200.0f) * 32.0f);
    for (j = 0; j < 32; j++) {
        float angle, next_angle;
        if (j >= segments) {
            color_words[0] = color_words[1] = color_words[2] = color_words[3] = color;
        } else {
            color_words[0] = color_words[1] = color_words[2] = color_words[3] =
                (color & 0xFFFFFF) + 0x20000000;
        }
        angle = fast_normalize_angle((float)j * 0.19634955f);
        next_angle = fast_add_rotations(angle, 0.19634955f);
        outer_x = truncate_float_to_s32(D_L08_00161B70 * fast_sin(angle));
        outer_y = truncate_float_to_s32(D_L08_00161B74 * -fast_cos(angle));
        inner_x = truncate_float_to_s32(D_L08_00161B70 * fast_sin(next_angle));
        inner_y = truncate_float_to_s32(D_L08_00161B74 * -fast_cos(next_angle));
        xy[3] = 0xFFFFF000000000L;
        xy[2] = 0xFFFFF000000000L;
        xy[1] = 0xFFFFF000000000L;
        xy[0] = 0xFFFFF000000000L;
        xy[0] += (unsigned int)((truncate_float_to_s32((D_L08_00161B64 + (float)inner_y) * 16.0f)
                                 << 16) -
                                (int)0x80000000);
        xy[0] += (truncate_float_to_s32(D_L08_00161B60 + (float)inner_x) << 4) + 0x8000;
        xy[1] +=
            (unsigned int)((truncate_float_to_s32(D_L08_00161B64 * 16.0f) << 16) - (int)0x80000000);
        xy[1] += (truncate_float_to_s32(D_L08_00161B60) << 4) + 0x8000;
        xy[2] += (unsigned int)((truncate_float_to_s32((D_L08_00161B64 + (float)outer_y) * 16.0f)
                                 << 16) -
                                (int)0x80000000);
        xy[2] += (truncate_float_to_s32(D_L08_00161B60 + (float)outer_x) << 4) + 0x8000;
        xy[3] +=
            (unsigned int)((truncate_float_to_s32(D_L08_00161B64 * 16.0f) << 16) - (int)0x80000000);
        xy[3] += (truncate_float_to_s32(D_L08_00161B60) << 4) + 0x8000;
        FUN_L02_0020bc88_c(xy, uv_words, color_words, texture, 1);
    }
    if (scale_game_frames(15) < data->timer) {
        color = func_001FA8A8_2E0258(0x5000FFFF,
                                     0x500000FF,
                                     convert_integer_to_float(data->timer - scale_game_frames(15)) /
                                         (float)scale_game_frames(15));
    } else {
        color = func_001FA8A8_2E0258(0x5000FF00,
                                     0x5000FFFF,
                                     convert_integer_to_float(data->timer) /
                                         (float)scale_game_frames(15));
    }
    uv_words[0] = D_L08_00161BE0;
    uv_words[1] = D_L08_00161BE4;
    uv_words[2] = D_L08_00161BE8;
    uv_words[3] = D_L08_00161BEC;
    color_words[0] = color_words[1] = color_words[2] = color_words[3] = color;
    xy[3] = 0xFFFFF000000000L;
    xy[2] = 0xFFFFF000000000L;
    xy[1] = 0xFFFFF000000000L;
    xy[0] = 0xFFFFF000000000L;
    angle = fast_normalize_angle(fast_add_rotations((*(float(*)[4]) & moby->rot)[2] * 4.0f, 0.0f));
    xy[0] +=
        (unsigned int)((truncate_float_to_s32(fast_sin(angle) * D_L08_00161B7C * 16.0f) << 16) -
                       (int)0x80000000);
    xy[0] += truncate_float_to_s32(fast_cos(angle) * D_L08_00161B78 * 16.0f) + 0x8000;
    angle = fast_normalize_angle(
        fast_add_rotations((*(float(*)[4]) & moby->rot)[2] * 4.0f, 1.5707964f));
    xy[1] +=
        (unsigned int)((truncate_float_to_s32(fast_sin(angle) * D_L08_00161B7C * 16.0f) << 16) -
                       (int)0x80000000);
    xy[1] += truncate_float_to_s32(fast_cos(angle) * D_L08_00161B78 * 16.0f) + 0x8000;
    angle = fast_normalize_angle(
        fast_add_rotations((*(float(*)[4]) & moby->rot)[2] * 4.0f, -1.5707964f));
    xy[2] +=
        (unsigned int)((truncate_float_to_s32(fast_sin(angle) * D_L08_00161B7C * 16.0f) << 16) -
                       (int)0x80000000);
    xy[2] += truncate_float_to_s32(fast_cos(angle) * D_L08_00161B78 * 16.0f) + 0x8000;
    angle = fast_normalize_angle(
        fast_add_rotations((*(float(*)[4]) & moby->rot)[2] * 4.0f, 3.1415927f));
    xy[3] +=
        (unsigned int)((truncate_float_to_s32(fast_sin(angle) * D_L08_00161B7C * 16.0f) << 16) -
                       (int)0x80000000);
    xy[3] += truncate_float_to_s32(fast_cos(angle) * D_L08_00161B78 * 16.0f) + 0x8000;
    FUN_L02_0020bc88_c(xy, uv_words, color_words, get_effect_texture(D_L08_00161BA8), 1);
    xy[3] = 0xFFFFF000000000L;
    xy[2] = 0xFFFFF000000000L;
    xy[1] = 0xFFFFF000000000L;
    xy[0] = 0xFFFFF000000000L;
    angle = fast_normalize_angle(fast_add_rotations((*(float(*)[4]) & moby->rot)[1] * 8.0f, 0.0f));
    xy[0] +=
        (unsigned int)((truncate_float_to_s32(fast_sin(angle) * D_L08_00161B7C * 16.0f) << 16) -
                       (int)0x80000000);
    xy[0] += truncate_float_to_s32(fast_cos(angle) * D_L08_00161B78 * 16.0f) + 0x8000;
    angle = fast_normalize_angle(
        fast_add_rotations((*(float(*)[4]) & moby->rot)[1] * 8.0f, 1.5707964f));
    xy[1] +=
        (unsigned int)((truncate_float_to_s32(fast_sin(angle) * D_L08_00161B7C * 16.0f) << 16) -
                       (int)0x80000000);
    xy[1] += truncate_float_to_s32(fast_cos(angle) * D_L08_00161B78 * 16.0f) + 0x8000;
    angle = fast_normalize_angle(
        fast_add_rotations((*(float(*)[4]) & moby->rot)[1] * 8.0f, -1.5707964f));
    xy[2] +=
        (unsigned int)((truncate_float_to_s32(fast_sin(angle) * D_L08_00161B7C * 16.0f) << 16) -
                       (int)0x80000000);
    xy[2] += truncate_float_to_s32(fast_cos(angle) * D_L08_00161B78 * 16.0f) + 0x8000;
    angle = fast_normalize_angle(
        fast_add_rotations((*(float(*)[4]) & moby->rot)[1] * 8.0f, 3.1415927f));
    xy[3] +=
        (unsigned int)((truncate_float_to_s32(fast_sin(angle) * D_L08_00161B7C * 16.0f) << 16) -
                       (int)0x80000000);
    xy[3] += truncate_float_to_s32(fast_cos(angle) * D_L08_00161B78 * 16.0f) + 0x8000;
    FUN_L02_0020bc88_c(xy, uv_words, color_words, get_effect_texture(D_L08_00161BAC), 1);
}

int find_batalia_cannon_aim_target(struct Moby *moby) {
    float facing_direction[4];
    float target_direction[4];
    int  *linked_targets = (int *)((char *)moby->pvars + 0x60);
    int   i;
    for (i = 0; i < 8; i++) {
        if (linked_targets[i] != 0) {
            float direction_dot;
            build_spherical_offset(facing_direction, 1.0f, moby->rot.z, -moby->rot.y);
            fast_vec_sub(target_direction, (char *)linked_targets[i] + 0x10, D_L08_001675C0);
            fast_vec_normalize(target_direction, target_direction, 1.0f);
            direction_dot = fast_vec_dot(target_direction, facing_direction);
            if (fast_cos(0.06981317f) < direction_dot)
                return linked_targets[i];
        }
    }
    return 0;
}

void update_batalia_cannon(Moby *moby) {
    enum {
        CANNON_INITIALIZE = 0,
        CANNON_UNMOUNTED_IDLE = 4,
        CANNON_MOUNTED_CONTROL = 5,
        CANNON_REMOVE = 6
    };
    enum {
        CANNON_NO_INACTIVE_INDEXED_TARGETS = 0,
        CANNON_ONE_INACTIVE_INDEXED_TARGET = 1,
        CANNON_TWO_INACTIVE_INDEXED_TARGETS = 2,
        CANNON_THREE_INACTIVE_INDEXED_TARGETS = 3,
        CANNON_ALL_INDEXED_TARGETS_INACTIVE = 4
    };
    float position[4], offset[4], work[4], direction[4], muzzle[4], scatter[4], random_vec[4];
    CannonData52 *data = (*(CannonData52 **)&moby->pvars);
    int           i, target, all_done, dead, best, min_life;
    Moby        **links;
    float         yaw, pitch, magnitude, best_angle;
    qcopy(position, (*(float(*)[4]) & moby->pos));
    position[2] += 0.4f;
    if (fast_dec_timer_s16(&data->exit_timer) && moby->command == 1) {
        qcopy(hero.motion.pos.f, position);
        qcopy(hero.motion.rot.f, (*(float(*)[4]) & moby->rot));
        offset[0] = fast_cos(hero.motion.rot.f[2]) * -0.7f;
        offset[1] = fast_sin(hero.motion.rot.f[2]) * -0.7f;
        offset[2] = 0.0f;
        fast_vec_add(hero.motion.pos.f, hero.motion.pos.f, offset);
        fast_mem_zero16((char *)hero.motion.pos.f + 0x60, 0x90);
        if (data->health == 0) {
            Moby *node;
            (*(float(*)[4]) & moby->rot)[1] = 0.0f;
            moby->command = 2;
            jet_end_mount(0);
            moby->state = 4;
            node = (Moby *)(D_L08_0015FFD8 + (data->platform_index << 8));
            qcopy(direction, (*(float(*)[4]) & node->rot));
            direction[2] = fast_add_rotations(direction[2], 3.1415927f);
            fast_vec_normalize(work, (*(float(*)[16]) & node->rot_row0), 2.5f);
            fast_vec_add(work, work, (*(float(*)[4]) & node->pos));
            func_L00_00217718_2E16A0(work, direction, 0, 1);
            set_batalia_cannon_platform_mode((char *)node, 0);
            return;
        }
        if (visited52[moby->mission + D_0015ED84 * 16] != 0xFF && (hero.ship_flags & 1))
            hero_respawn();
        if (((*(int *)D_0013A5E0_2E16A0 & 0x10) ||
             (*(unsigned char *)(l08_cannon_hero_interaction_flags) & 1)) &&
            visited52[moby->mission + D_0015ED84 * 16] == 0xFF) {
            hero.ship_flags = 0;
            hero_set_state(0, 1);
            moby->command = 2;
            (*(float(*)[4]) & moby->rot)[1] = 0.0f;
            euler_rotation_basis((*(float(*)[16]) & moby->rot_row0), (*(float(*)[4]) & moby->rot));
            clear_u64_value(direction);
            direction[2] = (*(float(*)[4]) & moby->rot)[2];
            transform_vector_by_basis(work, &D_L08_00161A90_gp, (*(float(*)[16]) & moby->rot_row0));
            fast_vec_add(work, work, (*(float(*)[4]) & moby->pos));
            func_L00_00217718_2E16A0(work, direction, 0, 1);
            jet_end_mount(0);
            hero.motion.rot.f[2] = fast_add_rotations(hero.motion.rot.f[2], 3.1415927f);
            moby->state = 4;
        } else {
            int *indices;
            int  count;
            if (moby->state != 5) {
                data->camera_timer = 2;
                jet_mount((*(float(*)[4]) & moby->pos), (*(float(*)[4]) & moby->rot), 1, 0, 0);
            }
            moby->state = 5;
            {
                int waiting = 2, activated = 3;
                indices = data->indices;
                for (count = 3; count >= 0; count--) {
                    if (((Moby *)D_L08_0015FFD8)[*indices].state == waiting)
                        ((Moby *)D_L08_0015FFD8)[*indices].state = activated;
                    indices++;
                }
            }
        }
    } else if (moby->command == 2) {
        if (vector_distance_xy(position, (*(float(*)[4]) & hero.moby->pos)) > 1.0f) {
            moby->command = 0;
            data->exit_timer = scale_game_frames(60);
        }
    } else if (visited52[moby->mission + D_0015ED84 * 16] == 0xFF) {
        if (vector_distance_xy(position, (*(float(*)[4]) & hero.moby->pos)) < 1.0f &&
            hero.motion.pos.f[2] - (*(float(*)[4]) & moby->pos)[2] < 7.0f &&
            data->exit_timer == 0 && data->health > 0) {
            moby->command = 1;
            hero_set_state(50, 1);
        }
    }
    switch (moby->state) {
    case CANNON_INITIALIZE: {
        Moby *node;
        int  *indices;
        int   count;
        moby->state = 4;
        data->health = 200;
        data->range = 10.0f;
        node = create_moby(0x710);
        data->visual = node;
        if (node) {
            qcopy((*(float(*)[4]) & node->pos), (*(float(*)[4]) & moby->pos));
            qcopy((*(float(*)[4]) & node->rot), (*(float(*)[4]) & moby->rot));
            node->draw_dist = 0x30;
            data->visual->visible = 1;
            (*(long *)&data->visual->spawn_frame) = (*(long *)&moby->spawn_frame);
        }
        (*(float(*)[4]) & moby->rot)[1] = 0.0f;
        data->fire_timer = scale_game_frames(30);
        data->exit_timer = 0;
        data->pitch_step = 0.0f;
        data->yaw_step = 0.0f;
        indices = (int *)&data->linked[7];
        count = 7;
        do {
            *indices-- = 0;
        } while (--count >= 0);
        data->wave_timer = scale_game_frames(180);
        data->target_timer = 0;
        data->target_index = 0;
        if (visited52[moby->mission + D_0015ED84 * 16] != 0xFF)
            data->visual_angle = 0.0f;
        else
            data->visual_angle = 1.5707964f;
        break;
    }
    case CANNON_MOUNTED_CONTROL:
        if (data->health < 0)
            data->health = 0;
        work[0] = hero.stick_x;
        work[1] = invert52 ? hero.stick_y : -hero.stick_y;
        magnitude = vector_length_xy(work);
        if (magnitude > 1.0f)
            func_L00_001FF328_2E16A0(work, work, magnitude);
        yaw = D_L08_00161AE8_gp * 0.017453292f * D_0015ED6C * -work[0];
        pitch = -(D_L08_00161AEC_gp * 0.017453292f * D_0015ED6C) * work[1];
        if ((data->yaw_step >= 0.0f && data->yaw_step < yaw) ||
            (data->yaw_step <= 0.0f && yaw < data->yaw_step))
            approach_value(&data->yaw_step, yaw, D_L08_00161AF0_gp * 0.017453292f * D_0015ED70);
        else
            approach_value(&data->yaw_step, yaw, D_L08_00161AF8_gp * 0.017453292f * D_0015ED70);
        if ((data->pitch_step >= 0.0f && data->pitch_step < pitch) ||
            (data->pitch_step <= 0.0f && pitch < data->pitch_step))
            approach_value(&data->pitch_step, pitch, D_L08_00161AF4_gp * 0.017453292f * D_0015ED70);
        else
            approach_value(&data->pitch_step, pitch, D_L08_00161AFC_gp * 0.017453292f * D_0015ED70);
        (*(float(*)[4]) & moby->rot)[2] =
            fast_add_rotations((*(float(*)[4]) & moby->rot)[2], data->yaw_step);
        (*(float(*)[4]) & moby->rot)[1] =
            fast_add_rotations((*(float(*)[4]) & moby->rot)[1], data->pitch_step);
        if ((*(float(*)[4]) & moby->rot)[1] < -0.7853982f)
            (*(float(*)[4]) & moby->rot)[1] = -0.7853982f;
        else if ((*(float(*)[4]) & moby->rot)[1] > 0.34906584f)
            (*(float(*)[4]) & moby->rot)[1] = 0.34906584f;
        target = find_batalia_cannon_aim_target((char *)moby);
        if (target) {
            if (data->lock_timer < scale_game_frames(30))
                data->lock_timer++;
        } else if (data->lock_timer)
            data->lock_timer--;
        fast_dec_timer_s16(&data->fire_timer);
        if ((pad_held[0] & 0x20) && data->fire_timer == 0) {
            if (data->barrel == 1) {
                int count;
                build_spherical_offset(direction,
                                       D_L08_00161B00_gp * D_0015ED6C,
                                       (*(float(*)[4]) & moby->rot)[2],
                                       -(*(float(*)[4]) & moby->rot)[1]);
                blend_moby_animation(data->guns[0], 1, 0, 0);
                transform_vector_by_basis(
                    muzzle, &D_L08_00161B30_gp, (*(float(*)[16]) & moby->rot_row0));
                fast_vec_add(muzzle, (*(float(*)[4]) & moby->pos), muzzle);
                spawn_turret_rocket(moby, muzzle, direction, target);
                allocate_voice_for_target_entry(0, 0, moby);
                l08_camera.shake_amount = 0.03f;
                l08_camera.shake_frames = scale_game_frames(10);
                l08_camera.shake2_amount = 0.03f;
                l08_camera.shake2_frames = scale_game_frames(7);
                data->barrel = 0;
                data->fire_timer = scale_game_frames(20);
                count = 4;
                do {
                    *(u128 *)random_vec = 0;
                    random_vec[0] = random_float_between(-1.0f, 1.0f);
                    random_vec[1] = random_float_between(-1.0f, 1.0f);
                    random_vec[2] = random_float_between(-1.0f, 1.0f);
                    *(u128 *)scatter = *(u128 *)random_vec;
                    fast_vec_normalize(scatter,
                                       scatter,
                                       fast_vec_length(direction) *
                                           (frame_scale * -0.35000002f + 1.0f));
                    fast_vec_add(scatter, direction, scatter);
                    fast_vec_normalize(scatter,
                                       scatter,
                                       random_float_between(D_0015ED6C * 3.0f, D_0015ED6C * 6.0f));
                    min_life = scale_game_frames(5);
                    func_L00_0026EBC0_2E16A0((char *)muzzle,
                                             (char *)scatter,
                                             0x5F2F4F6F,
                                             random_int_between(min_life, scale_game_frames(10)),
                                             50000.0f);
                } while (--count >= 0);
                fast_vec_normalize(direction, direction, D_0015ED6C * 0.5f);
                min_life = scale_game_frames(4);
                func_L00_0026EBC0_2E16A0((char *)muzzle,
                                         (char *)direction,
                                         0x2F4F7F7F,
                                         random_int_between(min_life, scale_game_frames(7)),
                                         500000.0f);
            } else {
                int count;
                build_spherical_offset(direction,
                                       D_L08_00161B00_gp * D_0015ED6C,
                                       (*(float(*)[4]) & moby->rot)[2],
                                       -(*(float(*)[4]) & moby->rot)[1]);
                blend_moby_animation(data->guns[1], 1, 0, 0);
                transform_vector_by_basis(
                    muzzle, &D_L08_00161B40_gp, (*(float(*)[16]) & moby->rot_row0));
                fast_vec_add(muzzle, (*(float(*)[4]) & moby->pos), muzzle);
                spawn_turret_rocket(
                    moby, muzzle, direction, find_batalia_cannon_aim_target((char *)moby));
                allocate_voice_for_target_entry(0, 0, moby);
                l08_camera.shake_amount = 0.03f;
                l08_camera.shake_frames = scale_game_frames(10);
                l08_camera.shake2_amount = 0.03f;
                l08_camera.shake2_frames = scale_game_frames(7);
                data->barrel = 1;
                data->fire_timer = scale_game_frames(20);
                count = 4;
                do {
                    *(u128 *)random_vec = 0;
                    random_vec[0] = random_float_between(-1.0f, 1.0f);
                    random_vec[1] = random_float_between(-1.0f, 1.0f);
                    random_vec[2] = random_float_between(-1.0f, 1.0f);
                    *(u128 *)scatter = *(u128 *)random_vec;
                    fast_vec_normalize(scatter,
                                       scatter,
                                       fast_vec_length(direction) *
                                           (frame_scale * -0.35000002f + 1.0f));
                    fast_vec_add(scatter, direction, scatter);
                    fast_vec_normalize(scatter,
                                       scatter,
                                       random_float_between(D_0015ED6C * 3.0f, D_0015ED6C * 6.0f));
                    min_life = scale_game_frames(5);
                    func_L00_0026EBC0_2E16A0((char *)muzzle,
                                             (char *)scatter,
                                             0x5F2F4F6F,
                                             random_int_between(min_life, scale_game_frames(10)),
                                             50000.0f);
                } while (--count >= 0);
                fast_vec_normalize(direction, direction, D_0015ED6C * 0.5f);
                min_life = scale_game_frames(4);
                func_L00_0026EBC0_2E16A0((char *)muzzle,
                                         (char *)direction,
                                         0x2F4F7F7F,
                                         random_int_between(min_life, scale_game_frames(7)),
                                         500000.0f);
            }
        }
        if (fast_dec_timer_s16(&data->camera_timer)) {
            snap_scripted_camera_to_pose();
            transform_vector_by_basis(
                direction, &D_L08_00161B50_gp, (*(float(*)[16]) & moby->rot_row0));
            fast_vec_add(direction, direction, (*(float(*)[4]) & moby->pos));
            set_camera_position(direction);
            set_camera_rotation((*(float(*)[4]) & moby->rot));
        }
        add_draw_callback(draw_batalia_cannon_hud, moby);
        if (fast_dec_timer_s16(&data->wave_timer)) {
            int count;
            dead = 0;
            all_done = 1;
            links = data->linked;
            count = 7;
            do {
                if (*links && (signed char)(*links)->state >= 0)
                    all_done = (*links)->state == 2 ? all_done : 0;
                links++;
            } while (--count >= 0);
            if (all_done) {
                int  cutoff = -1;
                int *indices = data->indices;
                int *walk = indices;
                count = 3;
                do {
                    Moby *node;
                    node = (Moby *)((*walk << 8) + (unsigned int)D_L08_0015FFD8);
                    if ((signed char)node->state <= cutoff)
                        dead++;
                    walk++;
                } while (--count >= 0);
                switch (dead) {
                case CANNON_NO_INACTIVE_INDEXED_TARGETS:
                    data->range = 10.0f;
                    data->wave_timer = scale_game_frames(285);
                    break;
                case CANNON_ONE_INACTIVE_INDEXED_TARGET:
                    data->range = 12.5f;
                    data->wave_timer = scale_game_frames(240);
                    break;
                case CANNON_TWO_INACTIVE_INDEXED_TARGETS:
                    data->range = 15.0f;
                    data->wave_timer = scale_game_frames(195);
                    break;
                case CANNON_THREE_INACTIVE_INDEXED_TARGETS:
                    data->range = 17.5f;
                    data->wave_timer = scale_game_frames(150);
                    break;
                case CANNON_ALL_INDEXED_TARGETS_INACTIVE:
                    if (visited52[moby->mission + D_0015ED84 * 16] != 0xFF) {
                        Moby *node;
                        (*(float(*)[4]) & moby->rot)[1] = 0.0f;
                        moby->command = 2;
                        jet_end_mount(0);
                        moby->state = 4;
                        node = (Moby *)(D_L08_0015FFD8 + (data->platform_index << 8));
                        qcopy(muzzle, (*(float(*)[4]) & node->rot));
                        muzzle[2] = fast_add_rotations(muzzle[2], 3.1415927f);
                        fast_vec_normalize(direction, (*(float(*)[16]) & node->rot_row0), 2.5f);
                        fast_vec_add(direction, direction, (*(float(*)[4]) & node->pos));
                        func_L00_00217718_2E16A0(direction, muzzle, 0, 1);
                        set_batalia_cannon_platform_mode((char *)node, 1);
                        data->visual_angle = 1.5707964f;
                        return;
                    }
                    break;
                }
                if (fast_subtract_rotations_cf(&data->target_timer) ||
                    (signed char)((Moby *)D_L08_0015FFD8)[*(int *)((unsigned int)indices +
                                                                   (data->target_index << 2))]
                            .state < 0) {
                    data->target_timer = 1;
                    best_angle = 3.1415927f;
                    best = -1;
                    for (i = 0; i < 4; i++) {
                        Moby *node;
                        node = (Moby *)(D_L08_0015FFD8 + (indices[i] << 8));
                        if (fast_difference_between_rotations(
                                fast_arc_tan((*(float(*)[4]) & node->pos)[0] - hero.motion.pos.f[0],
                                             (*(float(*)[4]) & node->pos)[1] -
                                                 hero.motion.pos.f[1]),
                                hero.motion.rot.f[2]) < best_angle &&
                            node->state < 0x7F) {
                            best = i;
                            best_angle = fast_difference_between_rotations(
                                fast_arc_tan((*(float(*)[4]) & node->pos)[0] - hero.motion.pos.f[0],
                                             (*(float(*)[4]) & node->pos)[1] -
                                                 hero.motion.pos.f[1]),
                                hero.motion.rot.f[2]);
                        }
                    }
                    data->target_index = best;
                }
                ((Moby *)
                     D_L08_0015FFD8)[*(int *)((unsigned int)indices + (data->target_index << 2))]
                    .command = 1;
            }
        }
        break;
    case CANNON_UNMOUNTED_IDLE:
        break;
    case CANNON_REMOVE:
        mark_moby_for_removal(data->guns[0]);
        mark_moby_for_removal(data->guns[1]);
        mark_moby_for_removal(moby);
        return;
    }
    if (data->guns[0] == 0) {
        Moby *node;
        node = create_moby(0x236);
        data->guns[0] = node;
        if (node) {
            node->update_dist = 0xFF;
            data->guns[0]->draw_dist = 0x7E;
            data->guns[0]->visible = 1;
            (*(long *)&data->guns[0]->spawn_frame) = (*(long *)&moby->spawn_frame);
            transform_vector_by_basis(work, &D_L08_00161B10_gp, (*(float(*)[16]) & moby->rot_row0));
            fast_vec_add(work, (*(float(*)[4]) & moby->pos), work);
            {
                Moby *gun = data->guns[0];
                qcopy((*(float(*)[4]) & gun->pos), work);
                qcopy((*(float(*)[4]) & gun->rot), (*(float(*)[4]) & moby->rot));
                update_entity(gun);
            }
        }
    } else {
        transform_vector_by_basis(work, &D_L08_00161B10_gp, (*(float(*)[16]) & moby->rot_row0));
        fast_vec_add(work, (*(float(*)[4]) & moby->pos), work);
        {
            Moby *gun = data->guns[0];
            qcopy((*(float(*)[4]) & gun->pos), work);
            qcopy((*(float(*)[4]) & gun->rot), (*(float(*)[4]) & moby->rot));
            if ((gun->anim_flags & 2) && gun->prev_seq == 1)
                blend_moby_animation(gun, 0, 0, 0);
        }
    }

    if (data->guns[1] == 0) {
        Moby *node;
        node = create_moby(0x236);
        data->guns[1] = node;
        if (node) {
            node->update_dist = 0xFF;
            data->guns[1]->draw_dist = 0x7E;
            data->guns[1]->visible = 1;
            (*(long *)&data->guns[1]->spawn_frame) = (*(long *)&moby->spawn_frame);
            transform_vector_by_basis(work, &D_L08_00161B20_gp, (*(float(*)[16]) & moby->rot_row0));
            fast_vec_add(work, (*(float(*)[4]) & moby->pos), work);
            {
                Moby *gun = data->guns[1];
                qcopy((*(float(*)[4]) & gun->pos), work);
                qcopy((*(float(*)[4]) & gun->rot), (*(float(*)[4]) & moby->rot));
                update_entity(gun);
            }
        }
    } else {
        transform_vector_by_basis(work, &D_L08_00161B20_gp, (*(float(*)[16]) & moby->rot_row0));
        fast_vec_add(work, (*(float(*)[4]) & moby->pos), work);
        {
            Moby *gun = data->guns[1];
            qcopy((*(float(*)[4]) & gun->pos), work);
            qcopy((*(float(*)[4]) & gun->rot), (*(float(*)[4]) & moby->rot));
            if ((gun->anim_flags & 2) && gun->prev_seq == 1)
                blend_moby_animation(gun, 0, 0, 0);
        }
    }

    if (data->visual) {
        transform_vector_by_basis(work, &D_L08_00161BF0_gp, (*(float(*)[16]) & moby->rot_row0));
        fast_vec_add((*(float(*)[4]) & data->visual->pos), work, (*(float(*)[4]) & moby->pos));
        {
            Moby *visual = data->visual;
            qcopy((*(float(*)[4]) & visual->rot), (*(float(*)[4]) & moby->rot));
            (*(float(*)[4]) & data->visual->rot)[1] =
                fast_add_rotations((*(float(*)[4]) & visual->rot)[1], data->visual_angle);
        }
        update_entity(data->visual);
    }
}

void request_batalia_cannon_mount(struct Moby *command_target_moby) {
    command_target_moby->command = 1;
    hero_set_state(0x32, 1);
}

/* Puts the cannon's platform moby back in state 1, leaving with mode 3 (targets cleared, mode 1)
 * or 4 (cannon destroyed, mode 0); mode -1 respawns the hero instead. */
void set_batalia_cannon_platform_mode(u8 *platform, s32 mode) {
    u8 *payload;

    platform[0x20] = 1;
    payload = *(u8 **)(platform + 0x78);
    switch (mode) {
    case 1:
        payload[8] = 1;
        *(s16 *)(payload + 0x36) = 3;
        break;
    case 0:
        payload[8] = 1;
        *(s16 *)(payload + 0x36) = 4;
        break;
    case -1:
        hero_respawn();
        break;
    }
}
