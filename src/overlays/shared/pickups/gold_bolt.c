/* Shared moby class gold_bolt: the gold bolt pickup, its glow rings, particles and pickup camera. */

#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/random/random_vector.h"
#include "rnc/rendering/effects/fade_to_black.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/camera/scripted_camera.h"
#include "rnc/overlay/shared/camera/follow_camera_set_up_spring.h"
#include "rnc/overlay/cuboid.h"
#include "rnc/game/options.h"

typedef struct {
    s32   id; /* collected flag index (D_0014BEC0 + level * 4), -1 none */
    u8    pad4[8];
    s16   unkC; /* nonzero: collect at once, no cutscene */
    u8    padE[2];
    Vec4f home; /* spawn position; w = spawn yaw */
    u8    pad20[0x40];
    f32   unk60;
    f32   unk64;
    f32   unk68;
    s32   timer; /* frames until the pickup finishes */
} Vars;

typedef struct {
    f32 x, y, z, w;
} V4;

extern void set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern void memcard_save_data(int, int) __asm__("FUN_0020b178");
extern void exit_hero_vehicle_mode_and_restore_moby(void) __asm__("FUN_L01_00231450");
extern void set_hero_animation(f32, s32, s32) __asm__("FUN_L00_002323b8");

typedef struct {
    u8  p0[0x20];
    f32 r[4];
    f32 a[4];
    s32 c[4];
    f32 x40, x44, x48, x4c;
} M_FUN_L01_00308470;

typedef struct {
    u8                  p0[0x78];
    M_FUN_L01_00308470 *m;
} O_FUN_L01_00308470;

void initialize_gold_bolt_data(O_FUN_L01_00308470 *moby) __asm__("FUN_L01_00308470");
/* the unit defines these with a char * parameter further down */
extern void           FUN_L01_00308550_f(struct Moby *) __asm__("FUN_L01_00308550");
extern void           update_gold_bolt_visuals(struct Moby *) __asm__("FUN_L01_00308380");
extern void           FUN_L01_003087e0_f(struct Moby *) __asm__("FUN_L01_003087e0");
extern void           FUN_L01_003089f0_f(struct Moby *) __asm__("FUN_L01_003089f0");
void                  update_gold_bolt_glow_rings(struct Moby *m) __asm__("FUN_L01_00308550");
extern void           FUN_L01_003089f0_u(char *) __asm__("FUN_L01_003089f0");
extern int            fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern unsigned char *FUN_L00_00272f68(void *, int, unsigned char, int, int, int, int, float);
extern void           spawn_moving_hero_relative_alpha_fade_particle(
              void *, void *, int, int, int, int, float) __asm__("FUN_L00_002730e0");

extern s32  D_0015ED84;
extern char D_L01_00167240[];
extern int  D_0015ED84; /* no foreign declaration */
extern int  cutscene_active __asm__("D_L01_0015F404");
extern u8   D_0014BEC0[];
/* MACRO_ADDR aliases: retail loads these with the one-register lui form */
extern f32 frame_time_macro __asm__("D_0015ED6C");
extern s32 current_level_index_macro __asm__("D_0015ED84");
extern s32 D_L01_0015F640;
extern s32 cutscene_active __asm__("D_L01_0015F404");
extern f32 D_L01_00161F10_gp __asm__("D_L01_00161F10") __attribute__((sda));
extern f32 D_L01_00161F14_gp __asm__("D_L01_00161F14") __attribute__((sda));
extern f32 D_L01_00161F18_gp __asm__("D_L01_00161F18") __attribute__((sda));
extern f32 D_L01_00161F2C_gp __asm__("D_L01_00161F2C") __attribute__((sda));
extern s32 D_L01_00161F50_gp __asm__("D_L01_00161F50") __attribute__((sda));
extern f32 gold_bolt_spin_x_degrees_per_second_gp __asm__("D_L01_00161F20") __attribute__((sda));
extern f32 gold_bolt_spin_y_degrees_per_second_gp __asm__("D_L01_00161F24") __attribute__((sda));
extern f32 gold_bolt_spin_z_degrees_per_second_gp __asm__("D_L01_00161F28") __attribute__((sda));
extern int gold_bolt_glow_ring_tween_start_color_gp __asm__("D_L01_00161F3C") __attribute__((sda));
extern int gold_bolt_glow_ring_tween_end_color_gp __asm__("D_L01_00161F40") __attribute__((sda));
extern int gold_bolt_glow_ring_particle_use_flags_44_gp __asm__("D_L01_00161F48")
    __attribute__((sda));
extern int gold_bolt_glow_ring_particle_texture_index_gp __asm__("D_L01_00161F4C")
    __attribute__((sda));
extern float gold_bolt_particle_initial_speed_min_gp __asm__("D_L01_00161F54") __attribute__((sda));
extern float gold_bolt_particle_initial_speed_max_gp __asm__("D_L01_00161F58") __attribute__((sda));
extern float gold_bolt_particle_velocity_jitter_speed_min_gp __asm__("D_L01_00161F5C")
    __attribute__((sda));
extern float gold_bolt_particle_velocity_jitter_speed_max_gp __asm__("D_L01_00161F60")
    __attribute__((sda));
extern short gold_bolt_particle_lifetime_gp __asm__("D_L01_00161F64") __attribute__((sda));
extern int   gold_bolt_particle_initial_color_gp __asm__("D_L01_00161F68") __attribute__((sda));
extern int   gold_bolt_particle_loop_color_gp __asm__("D_L01_00161F6C") __attribute__((sda));
extern float gold_bolt_particle_initial_scale_gp __asm__("D_L01_00161F70") __attribute__((sda));
extern float gold_bolt_particle_loop_scale_gp __asm__("D_L01_00161F74") __attribute__((sda));

f32   D_L02_00161ECC = 1.0f;
f32   D_L02_00161ED0 = 1.0f;
f32   D_L02_00161ED4 = 0.3f;
f32   D_L02_00161EE8 = 9e+01f;
s32   D_L02_00161F0C = 0x28;
f32   D_L02_00161EDC = 1.2e+02f;
f32   D_L02_00161EE0 = 4e+01f;
f32   D_L02_00161EE4 = 2e+01f;
int   D_L02_00161EF8 = 0x4040ffff;
int   D_L02_00161EFC = 0x1040ffff;
int   D_L02_00161F04 = 1;
int   D_L02_00161F08 = 0x35;
float D_L02_00161F10 = 1.0f;
float D_L02_00161F14 = 2.0f;
float D_L02_00161F18 = 0;
float D_L02_00161F1C = 0.333f;
short D_L02_00161F20 = 0x3c;
int   D_L02_00161F24 = 0x50408080;
int   D_L02_00161F28 = 0x40408080;
float D_L02_00161F2C = 0.5f;
float D_L02_00161F30 = 0.3f;
f32   D_L03_00161BEC = 1.0f;
f32   D_L03_00161BF0 = 1.0f;
f32   D_L03_00161BF4 = 0.3f;
f32   D_L03_00161C08 = 9e+01f;
s32   D_L03_00161C2C = 0x28;
f32   D_L03_00161BFC = 1.2e+02f;
f32   D_L03_00161C00 = 4e+01f;
f32   D_L03_00161C04 = 2e+01f;
int   D_L03_00161C18 = 0x4040ffff;
int   D_L03_00161C1C = 0x1040ffff;
int   D_L03_00161C24 = 1;
int   D_L03_00161C28 = 0x35;
float D_L03_00161C30 = 1.0f;
float D_L03_00161C34 = 2.0f;
float D_L03_00161C38 = 0;
float D_L03_00161C3C = 0.333f;
short D_L03_00161C40 = 0x3c;
int   D_L03_00161C44 = 0x50408080;
int   D_L03_00161C48 = 0x40408080;
float D_L03_00161C4C = 0.5f;
float D_L03_00161C50 = 0.3f;
f32   D_L04_00161CE0 = 1.0f;
f32   D_L04_00161CE4 = 1.0f;
f32   D_L04_00161CE8 = 0.3f;
f32   D_L04_00161CFC = 9e+01f;
s32   D_L04_00161D20 = 0x28;
f32   D_L04_00161CF0 = 1.2e+02f;
f32   D_L04_00161CF4 = 4e+01f;
f32   D_L04_00161CF8 = 2e+01f;
int   D_L04_00161D0C = 0x4040ffff;
int   D_L04_00161D10 = 0x1040ffff;
int   D_L04_00161D18 = 1;
int   D_L04_00161D1C = 0x35;
float D_L04_00161D24 = 1.0f;
float D_L04_00161D28 = 2.0f;
float D_L04_00161D2C = 0;
float D_L04_00161D30 = 0.333f;
short D_L04_00161D34 = 0x3c;
int   D_L04_00161D38 = 0x50408080;
int   D_L04_00161D3C = 0x40408080;
float D_L04_00161D40 = 0.5f;
float D_L04_00161D44 = 0.3f;
f32   D_L05_00161F30 = 1.0f;
f32   D_L05_00161F34 = 1.0f;
f32   D_L05_00161F38 = 0.3f;
f32   D_L05_00161F4C = 9e+01f;
s32   D_L05_00161F70 = 0x28;
f32   D_L05_00161F40 = 1.2e+02f;
f32   D_L05_00161F44 = 4e+01f;
f32   D_L05_00161F48 = 2e+01f;
int   D_L05_00161F5C = 0x4040ffff;
int   D_L05_00161F60 = 0x1040ffff;
int   D_L05_00161F68 = 1;
int   D_L05_00161F6C = 0x35;
float D_L05_00161F74 = 1.0f;
float D_L05_00161F78 = 2.0f;
float D_L05_00161F7C = 0;
float D_L05_00161F80 = 0.333f;
short D_L05_00161F84 = 0x3c;
int   D_L05_00161F88 = 0x50408080;
int   D_L05_00161F8C = 0x40408080;
float D_L05_00161F90 = 0.5f;
float D_L05_00161F94 = 0.3f;
f32   D_L06_0016223C = 1.0f;
f32   D_L06_00162240 = 1.0f;
f32   D_L06_00162244 = 0.3f;
f32   D_L06_00162258 = 9e+01f;
s32   D_L06_0016227C = 0x28;
f32   D_L06_0016224C = 1.2e+02f;
f32   D_L06_00162250 = 4e+01f;
f32   D_L06_00162254 = 2e+01f;
int   D_L06_00162268 = 0x4040ffff;
int   D_L06_0016226C = 0x1040ffff;
int   D_L06_00162274 = 1;
int   D_L06_00162278 = 0x35;
float D_L06_00162280 = 1.0f;
float D_L06_00162284 = 2.0f;
float D_L06_00162288 = 0;
float D_L06_0016228C = 0.333f;
short D_L06_00162290 = 0x3c;
int   D_L06_00162294 = 0x50408080;
int   D_L06_00162298 = 0x40408080;
float D_L06_0016229C = 0.5f;
float D_L06_001622A0 = 0.3f;
f32   D_L07_00161BC0 = 1.0f;
f32   D_L07_00161BC4 = 1.0f;
f32   D_L07_00161BC8 = 0.3f;
f32   D_L07_00161BDC = 9e+01f;
s32   D_L07_00161C00 = 0x28;
f32   D_L07_00161BD0 = 1.2e+02f;
f32   D_L07_00161BD4 = 4e+01f;
f32   D_L07_00161BD8 = 2e+01f;
int   D_L07_00161BEC = 0x4040ffff;
int   D_L07_00161BF0 = 0x1040ffff;
int   D_L07_00161BF8 = 1;
int   D_L07_00161BFC = 0x35;
float D_L07_00161C04 = 1.0f;
float D_L07_00161C08 = 2.0f;
float D_L07_00161C0C = 0;
float D_L07_00161C10 = 0.333f;
short D_L07_00161C14 = 0x3c;
int   D_L07_00161C18 = 0x50408080;
int   D_L07_00161C1C = 0x40408080;
float D_L07_00161C20 = 0.5f;
float D_L07_00161C24 = 0.3f;
f32   D_L08_001621F0 = 1.0f;
f32   D_L08_001621F4 = 1.0f;
f32   D_L08_001621F8 = 0.3f;
f32   D_L08_0016220C = 9e+01f;
s32   D_L08_00162230 = 0x28;
f32   D_L08_00162200 = 1.2e+02f;
f32   D_L08_00162204 = 4e+01f;
f32   D_L08_00162208 = 2e+01f;
int   D_L08_0016221C = 0x4040ffff;
int   D_L08_00162220 = 0x1040ffff;
int   D_L08_00162228 = 1;
int   D_L08_0016222C = 0x35;
float D_L08_00162234 = 1.0f;
float D_L08_00162238 = 2.0f;
float D_L08_0016223C = 0;
float D_L08_00162240 = 0.333f;
short D_L08_00162244 = 0x3c;
int   D_L08_00162248 = 0x50408080;
int   D_L08_0016224C = 0x40408080;
float D_L08_00162250 = 0.5f;
float D_L08_00162254 = 0.3f;
f32   D_L09_00161C24 = 1.0f;
f32   D_L09_00161C28 = 1.0f;
f32   D_L09_00161C2C = 0.3f;
f32   D_L09_00161C40 = 9e+01f;
s32   D_L09_00161C64 = 0x28;
f32   D_L09_00161C34 = 1.2e+02f;
f32   D_L09_00161C38 = 4e+01f;
f32   D_L09_00161C3C = 2e+01f;
int   D_L09_00161C50 = 0x4040ffff;
int   D_L09_00161C54 = 0x1040ffff;
int   D_L09_00161C5C = 1;
int   D_L09_00161C60 = 0x35;
float D_L09_00161C68 = 1.0f;
float D_L09_00161C6C = 2.0f;
float D_L09_00161C70 = 0;
float D_L09_00161C74 = 0.333f;
short D_L09_00161C78 = 0x3c;
int   D_L09_00161C7C = 0x50408080;
int   D_L09_00161C80 = 0x40408080;
float D_L09_00161C84 = 0.5f;
float D_L09_00161C88 = 0.3f;
f32   D_L10_00161D70 = 1.0f;
f32   D_L10_00161D74 = 1.0f;
f32   D_L10_00161D78 = 0.3f;
f32   D_L10_00161D8C = 9e+01f;
s32   D_L10_00161DB0 = 0x28;
f32   D_L10_00161D80 = 1.2e+02f;
f32   D_L10_00161D84 = 4e+01f;
f32   D_L10_00161D88 = 2e+01f;
int   D_L10_00161D9C = 0x4040ffff;
int   D_L10_00161DA0 = 0x1040ffff;
int   D_L10_00161DA8 = 1;
int   D_L10_00161DAC = 0x35;
float D_L10_00161DB4 = 1.0f;
float D_L10_00161DB8 = 2.0f;
float D_L10_00161DBC = 0;
float D_L10_00161DC0 = 0.333f;
short D_L10_00161DC4 = 0x3c;
int   D_L10_00161DC8 = 0x50408080;
int   D_L10_00161DCC = 0x40408080;
float D_L10_00161DD0 = 0.5f;
float D_L10_00161DD4 = 0.3f;
f32   D_L11_00161E20 = 1.0f;
f32   D_L11_00161E24 = 1.0f;
f32   D_L11_00161E28 = 0.3f;
f32   D_L11_00161E3C = 9e+01f;
s32   D_L11_00161E60 = 0x28;
f32   D_L11_00161E30 = 1.2e+02f;
f32   D_L11_00161E34 = 4e+01f;
f32   D_L11_00161E38 = 2e+01f;
int   D_L11_00161E4C = 0x4040ffff;
int   D_L11_00161E50 = 0x1040ffff;
int   D_L11_00161E58 = 1;
int   D_L11_00161E5C = 0x35;
float D_L11_00161E64 = 1.0f;
float D_L11_00161E68 = 2.0f;
float D_L11_00161E6C = 0;
float D_L11_00161E70 = 0.333f;
short D_L11_00161E74 = 0x3c;
int   D_L11_00161E78 = 0x50408080;
int   D_L11_00161E7C = 0x40408080;
float D_L11_00161E80 = 0.5f;
float D_L11_00161E84 = 0.3f;
f32   D_L12_00161CE0 = 1.0f;
f32   D_L12_00161CE4 = 1.0f;
f32   D_L12_00161CE8 = 0.3f;
f32   D_L12_00161CFC = 9e+01f;
s32   D_L12_00161D20 = 0x28;
f32   D_L12_00161CF0 = 1.2e+02f;
f32   D_L12_00161CF4 = 4e+01f;
f32   D_L12_00161CF8 = 2e+01f;
int   D_L12_00161D0C = 0x4040ffff;
int   D_L12_00161D10 = 0x1040ffff;
int   D_L12_00161D18 = 1;
int   D_L12_00161D1C = 0x35;
float D_L12_00161D24 = 1.0f;
float D_L12_00161D28 = 2.0f;
float D_L12_00161D2C = 0;
float D_L12_00161D30 = 0.333f;
short D_L12_00161D34 = 0x3c;
int   D_L12_00161D38 = 0x50408080;
int   D_L12_00161D3C = 0x40408080;
float D_L12_00161D40 = 0.5f;
float D_L12_00161D44 = 0.3f;
f32   D_L13_00161D38 = 1.0f;
f32   D_L13_00161D3C = 1.0f;
f32   D_L13_00161D40 = 0.3f;
f32   D_L13_00161D54 = 9e+01f;
s32   D_L13_00161D78 = 0x28;
f32   D_L13_00161D48 = 1.2e+02f;
f32   D_L13_00161D4C = 4e+01f;
f32   D_L13_00161D50 = 2e+01f;
int   D_L13_00161D64 = 0x4040ffff;
int   D_L13_00161D68 = 0x1040ffff;
int   D_L13_00161D70 = 1;
int   D_L13_00161D74 = 0x35;
float D_L13_00161D7C = 1.0f;
float D_L13_00161D80 = 2.0f;
float D_L13_00161D84 = 0;
float D_L13_00161D88 = 0.333f;
short D_L13_00161D8C = 0x3c;
int   D_L13_00161D90 = 0x50408080;
int   D_L13_00161D94 = 0x40408080;
float D_L13_00161D98 = 0.5f;
float D_L13_00161D9C = 0.3f;
f32   D_L14_00162004 = 1.0f;
f32   D_L14_00162008 = 1.0f;
f32   D_L14_0016200C = 0.3f;
f32   D_L14_00162020 = 9e+01f;
s32   D_L14_00162044 = 0x28;
f32   D_L14_00162014 = 1.2e+02f;
f32   D_L14_00162018 = 4e+01f;
f32   D_L14_0016201C = 2e+01f;
int   D_L14_00162030 = 0x4040ffff;
int   D_L14_00162034 = 0x1040ffff;
int   D_L14_0016203C = 1;
int   D_L14_00162040 = 0x35;
float D_L14_00162048 = 1.0f;
float D_L14_0016204C = 2.0f;
float D_L14_00162050 = 0;
float D_L14_00162054 = 0.333f;
short D_L14_00162058 = 0x3c;
int   D_L14_0016205C = 0x50408080;
int   D_L14_00162060 = 0x40408080;
float D_L14_00162064 = 0.5f;
float D_L14_00162068 = 0.3f;
f32   D_L15_00161EC4 = 1.0f;
f32   D_L15_00161EC8 = 1.0f;
f32   D_L15_00161ECC = 0.3f;
f32   D_L15_00161EE0 = 9e+01f;
s32   D_L15_00161F04 = 0x28;
f32   D_L15_00161ED4 = 1.2e+02f;
f32   D_L15_00161ED8 = 4e+01f;
f32   D_L15_00161EDC = 2e+01f;
int   D_L15_00161EF0 = 0x4040ffff;
int   D_L15_00161EF4 = 0x1040ffff;
int   D_L15_00161EFC = 1;
int   D_L15_00161F00 = 0x35;
float D_L15_00161F08 = 1.0f;
float D_L15_00161F0C = 2.0f;
float D_L15_00161F10 = 0;
float D_L15_00161F14 = 0.333f;
short D_L15_00161F18 = 0x3c;
int   D_L15_00161F1C = 0x50408080;
int   D_L15_00161F20 = 0x40408080;
float D_L15_00161F24 = 0.5f;
float D_L15_00161F28 = 0.3f;
f32   D_L16_00161C78 = 1.0f;
f32   D_L16_00161C7C = 1.0f;
f32   D_L16_00161C80 = 0.3f;
f32   D_L16_00161C94 = 9e+01f;
s32   D_L16_00161CB8 = 0x28;
f32   D_L16_00161C88 = 1.2e+02f;
f32   D_L16_00161C8C = 4e+01f;
f32   D_L16_00161C90 = 2e+01f;
int   D_L16_00161CA4 = 0x4040ffff;
int   D_L16_00161CA8 = 0x1040ffff;
int   D_L16_00161CB0 = 1;
int   D_L16_00161CB4 = 0x35;
float D_L16_00161CBC = 1.0f;
float D_L16_00161CC0 = 2.0f;
float D_L16_00161CC4 = 0;
float D_L16_00161CC8 = 0.333f;
short D_L16_00161CCC = 0x3c;
int   D_L16_00161CD0 = 0x50408080;
int   D_L16_00161CD4 = 0x40408080;
float D_L16_00161CD8 = 0.5f;
float D_L16_00161CDC = 0.3f;
f32   D_L17_00161E3C = 1.0f;
f32   D_L17_00161E40 = 1.0f;
f32   D_L17_00161E44 = 0.3f;
f32   D_L17_00161E58 = 9e+01f;
s32   D_L17_00161E7C = 0x28;
f32   D_L17_00161E4C = 1.2e+02f;
f32   D_L17_00161E50 = 4e+01f;
f32   D_L17_00161E54 = 2e+01f;
int   D_L17_00161E68 = 0x4040ffff;
int   D_L17_00161E6C = 0x1040ffff;
int   D_L17_00161E74 = 1;
int   D_L17_00161E78 = 0x35;
float D_L17_00161E80 = 1.0f;
float D_L17_00161E84 = 2.0f;
float D_L17_00161E88 = 0;
float D_L17_00161E8C = 0.333f;
short D_L17_00161E90 = 0x3c;
int   D_L17_00161E94 = 0x50408080;
int   D_L17_00161E98 = 0x40408080;
float D_L17_00161E9C = 0.5f;
float D_L17_00161EA0 = 0.3f;
f32   D_L18_0016210C = 1.0f;
f32   D_L18_00162110 = 1.0f;
f32   D_L18_00162114 = 0.3f;
f32   D_L18_00162128 = 9e+01f;
s32   D_L18_0016214C = 0x28;
f32   D_L18_0016211C = 1.2e+02f;
f32   D_L18_00162120 = 4e+01f;
f32   D_L18_00162124 = 2e+01f;
int   D_L18_00162138 = 0x4040ffff;
int   D_L18_0016213C = 0x1040ffff;
int   D_L18_00162144 = 1;
int   D_L18_00162148 = 0x35;
float D_L18_00162150 = 1.0f;
float D_L18_00162154 = 2.0f;
float D_L18_00162158 = 0;
float D_L18_0016215C = 0.333f;
short D_L18_00162160 = 0x3c;
int   D_L18_00162164 = 0x50408080;
int   D_L18_00162168 = 0x40408080;
float D_L18_0016216C = 0.5f;
float D_L18_00162170 = 0.3f;

void gold_bolt_update(struct Moby *moby) __asm__("FUN_L01_00307ca0");

void gold_bolt_update(struct Moby *moby) {
    enum {
        GOLD_BOLT_INITIALIZE = 0,
        GOLD_BOLT_WAIT_FOR_PICKUP = 1,
        GOLD_BOLT_PICKUP_CUTSCENE = 2
    };
    Vars        *vars = (Vars *)moby->pvars;
    struct Hero *hero_ptr;
    Vec4f        at;
    Vec4f        look;
    Vec4f        at2;
    Vec4f        look2;
    int          i;

    switch (moby->state) {
    case GOLD_BOLT_INITIALIZE:
        if (vars->id == -1 || D_0014BEC0[vars->id + D_0015ED84 * 4] != 0) {
            goto remove;
        }
        qcopy(&vars->home, &moby->pos);
        vars->home.w = moby->rot.z;
        moby->rot.x = random_angle_radians();
        moby->rot.y = random_angle_radians();
        moby->rot.z = random_angle_radians();
        moby->rot.w = random_angle_radians();
        vars->unk68 = gold_bolt_spin_x_degrees_per_second_gp * 0.017453292f * frame_time_macro;
        vars->unk64 = gold_bolt_spin_y_degrees_per_second_gp * 0.017453292f * frame_time_macro;
        vars->unk60 = gold_bolt_spin_z_degrees_per_second_gp * 0.017453292f * frame_time_macro;
        moby->state = 1;
        initialize_gold_bolt_data(moby);
        return;
    case GOLD_BOLT_WAIT_FOR_PICKUP:
        moby->scale = moby->pclass->scale * D_L01_00161F10_gp;
        moby->pos.z = vars->home.z + D_L01_00161F14_gp + D_L01_00161F18_gp * fast_sin(moby->rot.w);
        moby->rot.x = fast_add_rotations(
            moby->rot.x, gold_bolt_spin_x_degrees_per_second_gp * 0.017453292f * frame_time_macro);
        moby->rot.y = fast_add_rotations(
            moby->rot.y, gold_bolt_spin_y_degrees_per_second_gp * 0.017453292f * frame_time_macro);
        moby->rot.z = fast_add_rotations(
            moby->rot.z, gold_bolt_spin_z_degrees_per_second_gp * 0.017453292f * frame_time_macro);
        moby->rot.w =
            fast_add_rotations(moby->rot.w, D_L01_00161F2C_gp * 0.017453292f * frame_time_macro);
        FUN_L01_00308550_f(moby);
        if (!random_integer_below(D_L01_00161F50_gp)) {
            FUN_L01_003089f0_f(moby);
        }
        if (!(vector_distance_xy(&moby->pos, &hero.motion.pos) < 3.0f)) {
            return;
        }
        if (!(absolute_float(moby->pos.z - hero.motion.pos.f[2]) < 2.0f)) {
            return;
        }
        if (hero.health.hp == 0) {
            return;
        }
        if (hero.vehicle_mode != 3 && hero.vehicle_mode != 0) {
            return;
        }
        if (vars->unkC != 0) {
            D_L01_0015F640 = scale_game_frames(180);
            queue_dialogue_message(0x53B3, -1);
            D_0014BEC0[vars->id + current_level_index_macro * 4] = 1;
            memcard_save_data(0, -1);
        remove:
            mark_moby_for_removal(moby);
            return;
        }
        at.x = fast_cos(vars->home.w) * 2.5f;
        at.y = fast_sin(vars->home.w) * 2.5f;
        at.z = 0.0f;
        fast_vec_add(&at, &at, &vars->home);
        clear_u64_value(&look);
        look.z = fast_add_rotations(vars->home.w, 3.1415927f);
        fade_to_black(scale_game_frames(10));
        if (hero.vehicle_mode == 3) {
            exit_hero_vehicle_mode_and_restore_moby();
        }
        set_race_camera(&at, &look, 0x72, 0);
        allocate_voice_for_target_entry(0, 0, moby);
        at2.x = fast_cos(vars->home.w) * 1.25f;
        at2.y = fast_sin(vars->home.w) * 1.25f;
        at2.z = 0.0f;
        at2.x += fast_cos(fast_add_rotations(vars->home.w, 1.5707964f)) * 4.0f;
        at2.y += fast_sin(fast_add_rotations(vars->home.w, 1.5707964f)) * 4.0f;
        fast_vec_add(&at2, &at2, &vars->home);
        at2.z += 1.0f;
        clear_u64_value(&look2);
        look2.z = fast_subtract_rotations(vars->home.w, 1.5707964f);
        jet_mount(&at2, &look2, 1, 0, 0);
        set_camera_position(&at2);
        set_camera_rotation(&look2);
        qcopy(&moby->pos, &hero.motion.pos);
        qcopy(&moby->rot, &hero.motion.rot);
        if (moby->prev_seq != 1) {
            blend_moby_animation(moby, 1, 0, 0);
        }
        set_hero_animation(0.0f, 0x82, 0);
        if (opt_mirror_anim) {
            moby->flags |= 0x8000;
        }
        hero.unk20AF = 1;
        cutscene_active = 1;
        moby->state = 2;
        vars->timer = scale_game_frames(60);
        return;
    case GOLD_BOLT_PICKUP_CUTSCENE:
        FUN_L01_003087e0_f(moby);
        qcopy(&moby->pos, &hero.motion.pos);
        qcopy(&moby->rot, &hero.motion.rot);
        if (moby->seq == 1) {
            update_gold_bolt_visuals(moby);
            if (!random_integer_below(10)) {
                FUN_L01_003089f0_f(moby);
            }
            if (moby->anim_flags & 2) {
                if (moby->prev_seq != 0) {
                    blend_moby_animation(moby, 0, 0, 0);
                }
                moby->visible = 0;
                moby->flags |= 1;
                for (i = 0; i < 4; i++) {
                    FUN_L01_003089f0_f(moby);
                }
            }
        }
        hero_ptr = &hero;
        if (hero_ptr->anim_flags & 2) {
            set_hero_animation(scale_game_frames(30), 0, 0);
        }
        if (moby->seq != 0 || !fast_dec_timer(&vars->timer)) {
            return;
        }
        D_L01_0015F640 = scale_game_frames(180);
        queue_dialogue_message(0x53B3, -1);
        D_0014BEC0[vars->id + current_level_index_macro * 4] = 1;
        cutscene_active = 0;
        set_race_camera(&hero_ptr->motion.pos, &hero_ptr->motion.rot, 0, 0);
        jet_end_mount(0);
        memcard_save_data(0, -1);
        mark_moby_for_removal(moby);
        break;
    }
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002F7700.c: func_L01_00309758), where it is exact; names translated to the US level program. */
void update_gold_bolt_visuals(struct Moby *moby) __asm__("FUN_L01_00308380");

void update_gold_bolt_visuals(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    float updated_rotation_z;
    float bobbing_sine;
    float k;
    moby->scale = moby->pclass->scale * *(float *)&D_L01_00161F10_gp;
    if (moby->state == 1) {
        bobbing_sine = fast_sin(moby->rot.w);
        moby->pos.z = *(float *)(data + 0x18) + *(float *)&D_L01_00161F14_gp +
                      *(float *)&D_L01_00161F18_gp * bobbing_sine;
        moby->rot.x = fast_add_rotations(moby->rot.x, *(float *)(data + 0x68));
        moby->rot.y = fast_add_rotations(moby->rot.y, *(float *)(data + 0x64));
        updated_rotation_z = fast_add_rotations(moby->rot.z, *(float *)(data + 0x60));
        k = *(float *)&D_L01_00161F2C_gp * DEG_TO_RAD * frame_time;
        moby->rot.z = updated_rotation_z;
        moby->rot.w = fast_add_rotations(moby->rot.w, k);
    }
    update_gold_bolt_glow_rings(moby);
    if (random_integer_below(*(int *)&D_L01_00161F50_gp) == 0) {
        FUN_L01_003089f0_u(moby);
    }
}

/* Fills a moby data block: four random values, fixed constants, four frame-scaled counts. */

void initialize_gold_bolt_data(O_FUN_L01_00308470 *moby) {
    M_FUN_L01_00308470 *m = moby->m;
    s32                 i;
    for (i = 0; i < 4; i++)
        m->r[i] = random_integer_below(0xFF);
    m->a[0] = -1.0f;
    m->a[1] = -2.25f;
    m->a[2] = 1.25f;
    m->a[3] = 2.5f;
    for (i = 0; i < 4; i++)
        m->c[i] = scale_game_frames(i * 0x40 + 0x3F);
    m->x40 = 2.5f;
    m->x44 = 3.0f;
    m->x4c = 2.5f;
    m->x48 = 3.0f;
}

/* Draws the moby's four pulsing glow rings between it and the camera, fading with distance. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002F7700.c: func_L01_00309928), where it is exact; names translated to the US level program. */
void update_gold_bolt_glow_rings(struct Moby *moby) {
    char  *moby_vars = ((char *)moby->pvars);
    float  glow_particle_position[4];
    float  glow_origin_position[4];
    float  step[4];
    float  scale = 1.0f;
    int    i;
    float *ring_rotations;
    float *spd;
    int   *tm;
    float *sz;
    if (((unsigned char *)moby)[0x20] == 1) {
        fast_vec_scale(glow_origin_position, moby, 0.0009765625f);
    } else {
        moby_point_position(moby, 0, glow_origin_position);
        scale = (160.0f - compute_interpolated_record_value(moby)) / 160.0f * 16.0f;
        if (1.0f < scale)
            scale = 1.0f;
    }
    fast_vec_sub(glow_particle_position, D_L01_00167240, glow_origin_position);
    fast_vec_normalize(glow_particle_position, glow_particle_position, -0.3f);
    fast_vec_normalize(step, glow_particle_position, 0.1f);
    fast_vec_add(glow_particle_position, glow_particle_position, glow_origin_position);
    ring_rotations = (float *)(moby_vars + 0x20);
    spd = (float *)(moby_vars + 0x30);
    tm = (int *)(moby_vars + 0x40);
    sz = (float *)(moby_vars + 0x50);
    for (i = 0; i < 4; i++) {
        float unwrapped_angle = ring_rotations[i] + spd[i];
        float pulse_progress;
        int   packed_color;
        ring_rotations[i] = unwrapped_angle;
        if (255.0f <= unwrapped_angle) {
            ring_rotations[i] = unwrapped_angle - 255.0f;
        } else if (unwrapped_angle <= 0.0f) {
            ring_rotations[i] = unwrapped_angle + 255.0f;
        }
        if (fast_dec_timer((int *)(moby_vars + 0x40) + i)) {
            tm[i] = scale_game_frames(0xFF);
        }
        pulse_progress = convert_integer_to_float(scale_game_frames(0xFF) - tm[i]) /
                         (float)scale_game_frames(0xFF);
        packed_color = fast_tween_color(gold_bolt_glow_ring_tween_start_color_gp,
                                        gold_bolt_glow_ring_tween_end_color_gp,
                                        absolute_float(0.5f - pulse_progress));
        FUN_L00_00272f68(glow_particle_position,
                         packed_color,
                         truncate_float_to_s32(ring_rotations[i]),
                         gold_bolt_glow_ring_particle_texture_index_gp,
                         gold_bolt_glow_ring_particle_use_flags_44_gp,
                         2,
                         0,
                         sz[i] * scale);
        fast_vec_add(glow_particle_position, glow_particle_position, step);
    }
}

void update_gold_bolt_pickup_camera(struct Moby *moby) __asm__("FUN_L01_003087e0");

void update_gold_bolt_pickup_camera(struct Moby *moby) {
    char *data = ((char *)moby->pvars);
    V4    output_position;
    V4    output_rotation;
    V4    start_rotation;
    V4    end_rotation;
    f32   record_value;
    f32   interpolation_factor;

    if (moby->seq == 1) {
        record_value = compute_interpolated_record_value(moby);
    } else {
        record_value = 170.0f;
    }
    interpolation_factor = record_value / 170.0f;
    if (*(s32 *)(data + 4) == -1 || *(s32 *)(data + 8) == -1) {
        f32          position_offset_heading;
        f32          five = 5.0f;
        struct Hero *hero_ptr = &hero;
        position_offset_heading =
            fast_add_rotations(interpolation_factor * 0.6981316804885864f - 0.3490658402442932f,
                               hero_ptr->motion.rot.f[2]);
        output_position.x = fast_cos(position_offset_heading) * five;
        output_position.y = fast_sin(position_offset_heading) * five;
        output_position.z = 0.0f;
        fast_vec_add(&output_position.x, &output_position.x, (f32 *)(&hero_ptr->motion.pos));
        output_position.z += 1.0f;
        clear_u64_value(&output_rotation.x);
        output_rotation.z = fast_add_rotations(position_offset_heading, 3.141590118408203f);
        output_rotation.y = interpolation_factor * -0.17000000178813934f;
    } else {
        lerp_vector(&output_position.x,
                    &l01_cuboids[*(s32 *)(data + 4)].pos,
                    &l01_cuboids[*(s32 *)(data + 8)].pos,
                    interpolation_factor);
        {
            LevelCuboid *cuboids = l01_cuboids;

            qcopy(&start_rotation, &cuboids[*(s32 *)(data + 4)].rot);
            qcopy(&end_rotation, &cuboids[*(s32 *)(data + 8)].rot);
        }
        output_rotation.z = fast_add_rotations(
            fast_subtract_rotations(end_rotation.z, start_rotation.z) * interpolation_factor,
            start_rotation.z);
        output_rotation.y = fast_add_rotations(
            fast_subtract_rotations(end_rotation.y, start_rotation.y) * interpolation_factor,
            start_rotation.y);
        output_rotation.x = fast_add_rotations(
            fast_subtract_rotations(end_rotation.x, start_rotation.x) * interpolation_factor,
            start_rotation.x);
    }
    set_camera_position(&output_position.x);
    set_camera_rotation(&output_rotation.x);
}

/* builds three jittered offsets and fires the same effect at each accumulated point */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002F7700.c: func_L01_00309DC8), where it is exact; names translated to the US level program. */
void spawn_gold_bolt_particles(struct Moby *moby) __asm__("FUN_L01_003089f0");

void spawn_gold_bolt_particles(struct Moby *moby) {
    float v[4];
    float particle_velocity[4];
    float u[4];
    float frame_duration;
    int   i;
    int   particle_lifetime;
    int   loop_particle_lifetime;
    if (moby->state == 1) {
        fast_vec_scale(v, moby, 0.0009765625f);
    } else {
        moby_point_position(moby, 0, v);
    }
    frame_duration = frame_time;
    random_vector(particle_velocity,
                  gold_bolt_particle_initial_speed_min_gp * frame_duration,
                  gold_bolt_particle_initial_speed_max_gp * frame_duration);
    particle_lifetime = *(unsigned char *)&gold_bolt_particle_lifetime_gp;
    spawn_moving_hero_relative_alpha_fade_particle(v,
                                                   particle_velocity,
                                                   gold_bolt_particle_initial_color_gp,
                                                   particle_lifetime,
                                                   random_integer_below(0xFF) & 0xFF,
                                                   0,
                                                   gold_bolt_particle_initial_scale_gp);
    for (i = 2; i >= 0; i--) {
        frame_duration = frame_time;
        random_vector(u,
                      gold_bolt_particle_velocity_jitter_speed_min_gp * frame_duration,
                      gold_bolt_particle_velocity_jitter_speed_max_gp * frame_duration);
        fast_vec_add(particle_velocity, particle_velocity, u);
        loop_particle_lifetime = *(unsigned char *)&gold_bolt_particle_lifetime_gp;
        spawn_moving_hero_relative_alpha_fade_particle(v,
                                                       particle_velocity,
                                                       gold_bolt_particle_loop_color_gp,
                                                       loop_particle_lifetime,
                                                       random_integer_below(0xFF) & 0xFF,
                                                       0,
                                                       gold_bolt_particle_loop_scale_gp);
    }
}

s32 D_L01_00161F50_gp __asm__("D_L01_00161F50") = 0x28;
int gold_bolt_glow_ring_tween_end_color_gp __asm__("D_L01_00161F40") = 0x1040ffff;
