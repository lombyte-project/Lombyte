/* Shared moby class pyrocitor: the flamethrower held by the hero, and its flame effect slots. */

#include "types.h"
#include "rnc/overlay/point_light.h"
#include "rnc/globals.h"
#include "rnc/hero/hero.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/input/pad_state.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/mobys/moby.h"
#include "rnc/mobys/moby_spawn.h"
#include "qcopy.h"
#include "rnc/game/usage_stats.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/overlay/shared/weapons/devastator.h"

/* Ported from rac1-decomp (nonmatching/shared/func_L00_002B8AE8.c: func_L00_002B8AE8). */
typedef struct PyrocitorVars {
    struct Moby *other;
    char         pad4[0x40];
    s16          light;
    char         pad46[4];
    s16          voice;
    s16          timer;
    s16          spark;
} PyrocitorVars;

typedef struct FlameHit {
    Vec_2b2100   point;
    struct Moby *moby;
    s32          flags;
    u8           type;
    u8           pad[3];
    f32          scale;
    s32          mode;
} FlameHit;

typedef Vec_2b2100 Vec;

/* 0x78 points at a table of 15 s32 slots; index 0 is skipped, so the first
   free slot from 1 to 15 takes val. */
typedef struct {
    u8   pad0[0x78];
    s32 *slots;
} ListSlots8;

extern int   collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern float scale_time(float) __asm__("FUN_001f96b0");
extern int   get_ammo(int) __asm__("FUN_L00_00233e98");
extern void  blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
extern int   use_ammo(int, int) __asm__("FUN_L00_00233db8");
extern void  moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void  start_equipped_item_animation(void) __asm__("FUN_L00_0020e4c8");
extern void  release_point_light_slot(int) __asm__("FUN_L00_0023e838");
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern void
     collect_mobies_in_sphere(void *, float, int, void *, void *) __asm__("FUN_L00_001f2868");
void cancel_hero_item_animation(void) __asm__("FUN_L00_0020e698");
void update_pyrocitor_usage_and_cleanup(struct Moby *) __asm__("FUN_L00_002b8230");
void update_pyrocitor_flame_effect_slots(char *) __asm__("FUN_L00_002b8438");
extern struct Moby *spawn_linked_moby_from_basis(struct Moby *) __asm__("FUN_L00_002bb360");
extern s16          FUN_L00_0023e5e0(void *, f32, f32, s32) __asm__("FUN_L00_0023e5e0");
extern s16          FUN_0022da68(s32, s32, struct Moby *);
extern s32          voice_is_playing(struct Moby *, s32) __asm__("FUN_L00_0028d8c0");
extern f32  advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("FUN_00213f38");
extern void random_vector(void *, f32, f32) __asm__("FUN_L00_00257d78");
extern void *
spawn_spinning_and_fading_effect_particle(void *, void *, s32, f32) __asm__("FUN_L00_0026b230");
extern void FUN_L00_002b8798(struct Moby *, void *) __asm__("FUN_L00_002b8798");
extern void spawn_color_decaying_effect_particle(void *, void *, s32) __asm__("FUN_L00_0026d598");
extern void update_entity(struct Moby *) __asm__("FUN_L00_00250df8");
void        help_request(int, int) __asm__("FUN_L00_00203908");
extern char *
spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern int      fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern unsigned swap_packed_color_channels(unsigned c, int mask) __asm__("FUN_L00_0025c0e8");
extern void     FUN_L00_00257d78(float *, float, float);
extern void
make_hit_descriptor(char *arg, int a, int b, void *src, float scale) __asm__("FUN_L00_00259888");

extern char coll_hit[] __asm__("D_L00_00173E40");
extern int  D_0015EEA4;
extern s32  D_0015EEA4;
extern s32  D_L00_0015F5CC;
extern Vec4 zero_vector __asm__("D_L00_0015F580") MACRO_ADDR;
extern f32  pyrocitor_light_color_start_x_gp __asm__("D_L00_001614F0") __attribute__((sda));
extern f32  pyrocitor_light_color_end_x_gp __asm__("D_L00_00161500") __attribute__((sda));
extern f32  D_L00_00161510 __attribute__((sda));
extern f32  pyrocitor_flame_maximum_speed_gp __asm__("D_L00_00161514") __attribute__((sda));
extern s32 pyrocitor_flame_primary_particle_count_gp __asm__("D_L00_00161518") __attribute__((sda));
extern s32 pyrocitor_flame_extra_particle_count_gp __asm__("D_L00_0016151C") __attribute__((sda));
extern f32 pyrocitor_flame_base_range_gp __asm__("D_L00_00161520") __attribute__((sda));
extern f32 pyrocitor_flame_full_length_random_range_gp __asm__("D_L00_00161524")
    __attribute__((sda));
extern f32 pyrocitor_flame_zero_length_random_range_gp __asm__("D_L00_00161528")
    __attribute__((sda));
extern s32 pyrocitor_ammo_use_interval_frames_gp __asm__("D_L00_00161530") __attribute__((sda));
extern f32 D_L00_001601E8;
/* Fakematch: a second name for the pad state makes the button tests rebuild its address as retail does. */
extern struct PadState           controller_state_lui __asm__("D_0013C940");
extern unsigned short D_00141BD8 NOT_SDA;
extern short                     pyrocitor_spark_lookup_index_and_flags_gp __asm__("D_L00_0016152C")
    __attribute__((sda));
extern short pyrocitor_spark_random_vector_magnitude_scale_gp __asm__("D_L00_00161538")
    __attribute__((sda));
extern short pyrocitor_spark_direction_vector_w_gp __asm__("D_L00_0016153C") __attribute__((sda));
extern short pyrocitor_spark_random_vector_w_gp __asm__("D_L00_00161540") __attribute__((sda));
extern short pyrocitor_spark_initial_color_start_gp __asm__("D_L00_00161544") __attribute__((sda));
extern short pyrocitor_spark_initial_color_end_gp __asm__("D_L00_00161548") __attribute__((sda));
extern short pyrocitor_spark_second_color_start_gp __asm__("D_L00_0016154C") __attribute__((sda));
extern short pyrocitor_spark_second_color_end_gp __asm__("D_L00_00161550") __attribute__((sda));
extern short pyrocitor_spark_random_frame_count_min_gp __asm__("D_L00_00161554")
    __attribute__((sda));
extern short pyrocitor_spark_random_frame_count_max_gp __asm__("D_L00_00161558")
    __attribute__((sda));
extern short pyrocitor_spark_burst_count_gp __asm__("D_L00_0016155C") __attribute__((sda));

f32   D_L01_00161520 = 1.0f;
f32   D_L01_00161530 = 1.0f;
f32   D_L01_00161540 = 0.5f;
f32   D_L01_00161544 = 16.0f;
s32   D_L01_00161548 = 2;
s32   D_L01_0016154C = 2;
f32   D_L01_00161550 = 12.0f;
f32   D_L01_00161554 = 1.0f;
f32   D_L01_00161558 = 3.0f;
s32   D_L01_00161560 = 0xa;
f32   D_L01_001601E8 = 48.0f;
short D_L01_0016155C = 0xc;
short D_L01_00161568 = 0;
short D_L01_0016156C = -13107;
short D_L01_00161570 = -13107;
short D_L01_00161574 = 0x2040;
short D_L01_00161578 = 0x4040;
short D_L01_0016157C = 0x2040;
short D_L01_00161580 = 0x4040;
short D_L01_00161584 = 0xa;
short D_L01_00161588 = 0x14;
short D_L01_0016158C = 2;
f32   D_L02_001614C0 = 1.0f;
f32   D_L02_001614D0 = 1.0f;
f32   D_L02_001614E0 = 0.5f;
f32   D_L02_001614E4 = 16.0f;
s32   D_L02_001614E8 = 2;
s32   D_L02_001614EC = 2;
f32   D_L02_001614F0 = 12.0f;
f32   D_L02_001614F4 = 1.0f;
f32   D_L02_001614F8 = 3.0f;
s32   D_L02_00161500 = 0xa;
f32   D_L02_001601E8 = 48.0f;
short D_L02_001614FC = 0xc;
short D_L02_00161508 = 0;
short D_L02_0016150C = -13107;
short D_L02_00161510 = -13107;
short D_L02_00161514 = 0x2040;
short D_L02_00161518 = 0x4040;
short D_L02_0016151C = 0x2040;
short D_L02_00161520 = 0x4040;
short D_L02_00161524 = 0xa;
short D_L02_00161528 = 0x14;
short D_L02_0016152C = 2;
f32   D_L03_001614D0 = 1.0f;
f32   D_L03_001614E0 = 1.0f;
f32   D_L03_001614F0 = 0.5f;
f32   D_L03_001614F4 = 16.0f;
s32   D_L03_001614F8 = 2;
s32   D_L03_001614FC = 2;
f32   D_L03_00161500 = 12.0f;
f32   D_L03_00161504 = 1.0f;
f32   D_L03_00161508 = 3.0f;
s32   D_L03_00161510 = 0xa;
f32   D_L03_001601E8 = 48.0f;
short D_L03_0016150C = 0xc;
short D_L03_00161518 = 0;
short D_L03_0016151C = -13107;
short D_L03_00161520 = -13107;
short D_L03_00161524 = 0x2040;
short D_L03_00161528 = 0x4040;
short D_L03_0016152C = 0x2040;
short D_L03_00161530 = 0x4040;
short D_L03_00161534 = 0xa;
short D_L03_00161538 = 0x14;
short D_L03_0016153C = 2;
f32   D_L04_001614C0 = 1.0f;
f32   D_L04_001614D0 = 1.0f;
f32   D_L04_001614E0 = 0.5f;
f32   D_L04_001614E4 = 16.0f;
s32   D_L04_001614E8 = 2;
s32   D_L04_001614EC = 2;
f32   D_L04_001614F0 = 12.0f;
f32   D_L04_001614F4 = 1.0f;
f32   D_L04_001614F8 = 3.0f;
s32   D_L04_00161500 = 0xa;
f32   D_L04_001601E8 = 48.0f;
short D_L04_001614FC = 0xc;
short D_L04_00161508 = 0;
short D_L04_0016150C = -13107;
short D_L04_00161510 = -13107;
short D_L04_00161514 = 0x2040;
short D_L04_00161518 = 0x4040;
short D_L04_0016151C = 0x2040;
short D_L04_00161520 = 0x4040;
short D_L04_00161524 = 0xa;
short D_L04_00161528 = 0x14;
short D_L04_0016152C = 2;
f32   D_L05_00161510 = 1.0f;
f32   D_L05_00161520 = 1.0f;
f32   D_L05_00161530 = 0.5f;
f32   D_L05_00161534 = 16.0f;
s32   D_L05_00161538 = 2;
s32   D_L05_0016153C = 2;
f32   D_L05_00161540 = 12.0f;
f32   D_L05_00161544 = 1.0f;
f32   D_L05_00161548 = 3.0f;
s32   D_L05_00161550 = 0xa;
f32   D_L05_001601E8 = 48.0f;
short D_L05_0016154C = 0xc;
short D_L05_00161558 = 0;
short D_L05_0016155C = -13107;
short D_L05_00161560 = -13107;
short D_L05_00161564 = 0x2040;
short D_L05_00161568 = 0x4040;
short D_L05_0016156C = 0x2040;
short D_L05_00161570 = 0x4040;
short D_L05_00161574 = 0xa;
short D_L05_00161578 = 0x14;
short D_L05_0016157C = 2;
f32   D_L06_001614F0 = 1.0f;
f32   D_L06_00161500 = 1.0f;
f32   D_L06_00161510 = 0.5f;
f32   D_L06_00161514 = 16.0f;
s32   D_L06_00161518 = 2;
s32   D_L06_0016151C = 2;
f32   D_L06_00161520 = 12.0f;
f32   D_L06_00161524 = 1.0f;
f32   D_L06_00161528 = 3.0f;
s32   D_L06_00161530 = 0xa;
f32   D_L06_001601E8 = 48.0f;
short D_L06_0016152C = 0xc;
short D_L06_00161538 = 0;
short D_L06_0016153C = -13107;
short D_L06_00161540 = -13107;
short D_L06_00161544 = 0x2040;
short D_L06_00161548 = 0x4040;
short D_L06_0016154C = 0x2040;
short D_L06_00161550 = 0x4040;
short D_L06_00161554 = 0xa;
short D_L06_00161558 = 0x14;
short D_L06_0016155C = 2;
f32   D_L07_001614C0 = 1.0f;
f32   D_L07_001614D0 = 1.0f;
f32   D_L07_001614E0 = 0.5f;
f32   D_L07_001614E4 = 16.0f;
s32   D_L07_001614E8 = 2;
s32   D_L07_001614EC = 2;
f32   D_L07_001614F0 = 12.0f;
f32   D_L07_001614F4 = 1.0f;
f32   D_L07_001614F8 = 3.0f;
s32   D_L07_00161500 = 0xa;
f32   D_L07_001601E8 = 48.0f;
short D_L07_001614FC = 0xc;
short D_L07_00161508 = 0;
short D_L07_0016150C = -13107;
short D_L07_00161510 = -13107;
short D_L07_00161514 = 0x2040;
short D_L07_00161518 = 0x4040;
short D_L07_0016151C = 0x2040;
short D_L07_00161520 = 0x4040;
short D_L07_00161524 = 0xa;
short D_L07_00161528 = 0x14;
short D_L07_0016152C = 2;
f32   D_L08_00161510 = 1.0f;
f32   D_L08_00161520 = 1.0f;
f32   D_L08_00161530 = 0.5f;
f32   D_L08_00161534 = 16.0f;
s32   D_L08_00161538 = 2;
s32   D_L08_0016153C = 2;
f32   D_L08_00161540 = 12.0f;
f32   D_L08_00161544 = 1.0f;
f32   D_L08_00161548 = 3.0f;
s32   D_L08_00161550 = 0xa;
f32   D_L08_001601E8 = 48.0f;
short D_L08_0016154C = 0xc;
short D_L08_00161558 = 0;
short D_L08_0016155C = -13107;
short D_L08_00161560 = -13107;
short D_L08_00161564 = 0x2040;
short D_L08_00161568 = 0x4040;
short D_L08_0016156C = 0x2040;
short D_L08_00161570 = 0x4040;
short D_L08_00161574 = 0xa;
short D_L08_00161578 = 0x14;
short D_L08_0016157C = 2;
f32   D_L09_00161580 = 1.0f;
f32   D_L09_00161590 = 1.0f;
f32   D_L09_001615A0 = 0.5f;
f32   D_L09_001615A4 = 16.0f;
s32   D_L09_001615A8 = 2;
s32   D_L09_001615AC = 2;
f32   D_L09_001615B0 = 12.0f;
f32   D_L09_001615B4 = 1.0f;
f32   D_L09_001615B8 = 3.0f;
s32   D_L09_001615C0 = 0xa;
f32   D_L09_001601E8 = 48.0f;
short D_L09_001615BC = 0xc;
short D_L09_001615C8 = 0;
short D_L09_001615CC = -13107;
short D_L09_001615D0 = -13107;
short D_L09_001615D4 = 0x2040;
short D_L09_001615D8 = 0x4040;
short D_L09_001615DC = 0x2040;
short D_L09_001615E0 = 0x4040;
short D_L09_001615E4 = 0xa;
short D_L09_001615E8 = 0x14;
short D_L09_001615EC = 2;
f32   D_L10_001614D0 = 1.0f;
f32   D_L10_001614E0 = 1.0f;
f32   D_L10_001614F0 = 0.5f;
f32   D_L10_001614F4 = 16.0f;
s32   D_L10_001614F8 = 2;
s32   D_L10_001614FC = 2;
f32   D_L10_00161500 = 12.0f;
f32   D_L10_00161504 = 1.0f;
f32   D_L10_00161508 = 3.0f;
s32   D_L10_00161510 = 0xa;
f32   D_L10_001601E8 = 48.0f;
short D_L10_0016150C = 0xc;
short D_L10_00161518 = 0;
short D_L10_0016151C = -13107;
short D_L10_00161520 = -13107;
short D_L10_00161524 = 0x2040;
short D_L10_00161528 = 0x4040;
short D_L10_0016152C = 0x2040;
short D_L10_00161530 = 0x4040;
short D_L10_00161534 = 0xa;
short D_L10_00161538 = 0x14;
short D_L10_0016153C = 2;
f32   D_L11_00161550 = 1.0f;
f32   D_L11_00161560 = 1.0f;
f32   D_L11_00161570 = 0.5f;
f32   D_L11_00161574 = 16.0f;
s32   D_L11_00161578 = 2;
s32   D_L11_0016157C = 2;
f32   D_L11_00161580 = 12.0f;
f32   D_L11_00161584 = 1.0f;
f32   D_L11_00161588 = 3.0f;
s32   D_L11_00161590 = 0xa;
f32   D_L11_001601E8 = 48.0f;
short D_L11_0016158C = 0xc;
short D_L11_00161598 = 0;
short D_L11_0016159C = -13107;
short D_L11_001615A0 = -13107;
short D_L11_001615A4 = 0x2040;
short D_L11_001615A8 = 0x4040;
short D_L11_001615AC = 0x2040;
short D_L11_001615B0 = 0x4040;
short D_L11_001615B4 = 0xa;
short D_L11_001615B8 = 0x14;
short D_L11_001615BC = 2;
f32   D_L12_001614D0 = 1.0f;
f32   D_L12_001614E0 = 1.0f;
f32   D_L12_001614F0 = 0.5f;
f32   D_L12_001614F4 = 16.0f;
s32   D_L12_001614F8 = 2;
s32   D_L12_001614FC = 2;
f32   D_L12_00161500 = 12.0f;
f32   D_L12_00161504 = 1.0f;
f32   D_L12_00161508 = 3.0f;
s32   D_L12_00161510 = 0xa;
f32   D_L12_001601E8 = 48.0f;
short D_L12_0016150C = 0xc;
short D_L12_00161518 = 0;
short D_L12_0016151C = -13107;
short D_L12_00161520 = -13107;
short D_L12_00161524 = 0x2040;
short D_L12_00161528 = 0x4040;
short D_L12_0016152C = 0x2040;
short D_L12_00161530 = 0x4040;
short D_L12_00161534 = 0xa;
short D_L12_00161538 = 0x14;
short D_L12_0016153C = 2;
f32   D_L13_001615A0 = 1.0f;
f32   D_L13_001615B0 = 1.0f;
f32   D_L13_001615C0 = 0.5f;
f32   D_L13_001615C4 = 16.0f;
s32   D_L13_001615C8 = 2;
s32   D_L13_001615CC = 2;
f32   D_L13_001615D0 = 12.0f;
f32   D_L13_001615D4 = 1.0f;
f32   D_L13_001615D8 = 3.0f;
s32   D_L13_001615E0 = 0xa;
f32   D_L13_001601E8 = 48.0f;
short D_L13_001615DC = 0xc;
short D_L13_001615E8 = 0;
short D_L13_001615EC = -13107;
short D_L13_001615F0 = -13107;
short D_L13_001615F4 = 0x2040;
short D_L13_001615F8 = 0x4040;
short D_L13_001615FC = 0x2040;
short D_L13_00161600 = 0x4040;
short D_L13_00161604 = 0xa;
short D_L13_00161608 = 0x14;
short D_L13_0016160C = 2;
f32   D_L14_001616E0 = 1.0f;
f32   D_L14_001616F0 = 1.0f;
f32   D_L14_00161700 = 0.5f;
f32   D_L14_00161704 = 16.0f;
s32   D_L14_00161708 = 2;
s32   D_L14_0016170C = 2;
f32   D_L14_00161710 = 12.0f;
f32   D_L14_00161714 = 1.0f;
f32   D_L14_00161718 = 3.0f;
s32   D_L14_00161720 = 0xa;
f32   D_L14_001601E8 = 48.0f;
short D_L14_0016171C = 0xc;
short D_L14_00161728 = 0;
short D_L14_0016172C = -13107;
short D_L14_00161730 = -13107;
short D_L14_00161734 = 0x2040;
short D_L14_00161738 = 0x4040;
short D_L14_0016173C = 0x2040;
short D_L14_00161740 = 0x4040;
short D_L14_00161744 = 0xa;
short D_L14_00161748 = 0x14;
short D_L14_0016174C = 2;
f32   D_L15_00161680 = 1.0f;
f32   D_L15_00161690 = 1.0f;
f32   D_L15_001616A0 = 0.5f;
f32   D_L15_001616A4 = 16.0f;
s32   D_L15_001616A8 = 2;
s32   D_L15_001616AC = 2;
f32   D_L15_001616B0 = 12.0f;
f32   D_L15_001616B4 = 1.0f;
f32   D_L15_001616B8 = 3.0f;
s32   D_L15_001616C0 = 0xa;
f32   D_L15_001601E8 = 48.0f;
short D_L15_001616BC = 0xc;
short D_L15_001616C8 = 0;
short D_L15_001616CC = -13107;
short D_L15_001616D0 = -13107;
short D_L15_001616D4 = 0x2040;
short D_L15_001616D8 = 0x4040;
short D_L15_001616DC = 0x2040;
short D_L15_001616E0 = 0x4040;
short D_L15_001616E4 = 0xa;
short D_L15_001616E8 = 0x14;
short D_L15_001616EC = 2;
f32   D_L16_00161580 = 1.0f;
f32   D_L16_00161590 = 1.0f;
f32   D_L16_001615A0 = 0.5f;
f32   D_L16_001615A4 = 16.0f;
s32   D_L16_001615A8 = 2;
s32   D_L16_001615AC = 2;
f32   D_L16_001615B0 = 12.0f;
f32   D_L16_001615B4 = 1.0f;
f32   D_L16_001615B8 = 3.0f;
s32   D_L16_001615C0 = 0xa;
f32   D_L16_001601E8 = 48.0f;
short D_L16_001615BC = 0xc;
short D_L16_001615C8 = 0;
short D_L16_001615CC = -13107;
short D_L16_001615D0 = -13107;
short D_L16_001615D4 = 0x2040;
short D_L16_001615D8 = 0x4040;
short D_L16_001615DC = 0x2040;
short D_L16_001615E0 = 0x4040;
short D_L16_001615E4 = 0xa;
short D_L16_001615E8 = 0x14;
short D_L16_001615EC = 2;
f32   D_L17_001615D0 = 1.0f;
f32   D_L17_001615E0 = 1.0f;
f32   D_L17_001615F0 = 0.5f;
f32   D_L17_001615F4 = 16.0f;
s32   D_L17_001615F8 = 2;
s32   D_L17_001615FC = 2;
f32   D_L17_00161600 = 12.0f;
f32   D_L17_00161604 = 1.0f;
f32   D_L17_00161608 = 3.0f;
s32   D_L17_00161610 = 0xa;
f32   D_L17_001601E8 = 48.0f;
short D_L17_0016160C = 0xc;
short D_L17_00161618 = 0;
short D_L17_0016161C = -13107;
short D_L17_00161620 = -13107;
short D_L17_00161624 = 0x2040;
short D_L17_00161628 = 0x4040;
short D_L17_0016162C = 0x2040;
short D_L17_00161630 = 0x4040;
short D_L17_00161634 = 0xa;
short D_L17_00161638 = 0x14;
short D_L17_0016163C = 2;
f32   D_L18_00161560 = 1.0f;
f32   D_L18_00161570 = 1.0f;
f32   D_L18_00161580 = 0.5f;
f32   D_L18_00161584 = 16.0f;
s32   D_L18_00161588 = 2;
s32   D_L18_0016158C = 2;
f32   D_L18_00161590 = 12.0f;
f32   D_L18_00161594 = 1.0f;
f32   D_L18_00161598 = 3.0f;
s32   D_L18_001615A0 = 0xa;
f32   D_L18_001601E8 = 48.0f;
short D_L18_0016159C = 0xc;
short D_L18_001615A8 = 0;
short D_L18_001615AC = -13107;
short D_L18_001615B0 = -13107;
short D_L18_001615B4 = 0x2040;
short D_L18_001615B8 = 0x4040;
short D_L18_001615BC = 0x2040;
short D_L18_001615C0 = 0x4040;
short D_L18_001615C4 = 0xa;
short D_L18_001615C8 = 0x14;
short D_L18_001615CC = 2;

void pyrocitor_update(struct Moby *moby) __asm__("FUN_L00_002b77f0");

/* Moby class 176 */
void pyrocitor_update(struct Moby *moby) {
    enum {
        PYROCITOR_STATE_INITIALIZE = 0,
        PYROCITOR_STATE_WAIT_FOR_READY = 1,
        PYROCITOR_STATE_READY = 2,
        PYROCITOR_STATE_FIRING = 3
    };
    PyrocitorVars *pyrocitor_vars = (PyrocitorVars *)moby->pvars;
    Vec_2b2100     pos, light_offset_and_flame_velocity, light_color_and_flame_origin, endpoint,
        particle_pos, random_vec;
    FlameHit     hit;
    f32          pos_out, vel_out;
    f32          speed, length, flame_braking_step, damping;
    s32          gate;
    s32          i;
    s32          fl;
    void        *effect;
    s32          ready;
    s32          firing_voice_slot_index;
    struct Moby *other;

    update_pyrocitor_usage_and_cleanup((unsigned char *)moby);
    moby_point_position(moby, 0, &pos);
    switch (moby->state) {
    case PYROCITOR_STATE_INITIALIZE:
        pyrocitor_vars->light = -1;
        pyrocitor_vars->voice = -1;
        moby->state = 1;
        break;
    case PYROCITOR_STATE_WAIT_FOR_READY:
        if (moby->seq == 1)
            moby->state = 2;
        if (moby->seq != 0)
            break;
        if (compute_interpolated_record_value(moby) > 10.0f &&
            (controller_state.held & hero.items[0].button_mask) && moby->prev_seq != 1)
            blend_moby_animation(moby, 1, 0, scale_game_frames(5));
        break;
    case PYROCITOR_STATE_READY:
        if (pyrocitor_vars->other == 0) {
            pyrocitor_vars->other = spawn_linked_moby_from_basis(moby);
            if (pyrocitor_vars->other != 0) {
                pyrocitor_vars->other->flags |= 0x100;
                *(f32 *)(pyrocitor_vars->other->pvars + 0xC) = D_L00_00161510;
            }
        }
        if (pyrocitor_vars->light != -1) {
            lerp_vector(&point_lights[pyrocitor_vars->light],
                        &point_lights[pyrocitor_vars->light],
                        &zero_vector,
                        0.1f);
            if (fast_vec_length(&point_lights[pyrocitor_vars->light]) < 0.001f) {
                release_point_light_slot(pyrocitor_vars->light);
                /* Fakematch: the dead test on moby keeps the s6/s7 assignment of retail. */
                if (moby)
                    pyrocitor_vars->light = -1;
                else
                    pyrocitor_vars->light = -1;
            }
        }
        if ((controller_state_lui.held & hero.items[0].button_mask) && !hero.item_use_locked &&
            get_ammo(-1)) {
            moby->state = 3;
            start_equipped_item_animation();
            pyrocitor_vars->timer = scale_game_frames(10);
            if (pyrocitor_vars->light == -1)
                pyrocitor_vars->light = FUN_L00_0023e5e0(&moby->pos, 7.5f, 0.0f, 0);
        } else if ((controller_state_lui.pressed & hero.items[0].button_mask) && !get_ammo(-1)) {
            FUN_0022da68(0, 0, moby);
        }
        break;
    case PYROCITOR_STATE_FIRING:
        if (!voice_is_playing(moby, pyrocitor_vars->voice))
            pyrocitor_vars->voice = FUN_0022da68(1, 4, moby);
        if (pyrocitor_vars->light != -1 && moby->state == 3) {
            light_offset_and_flame_velocity.q = 0;
            light_offset_and_flame_velocity.f.x = 2.0f;
            light_offset_and_flame_velocity.f.z = 2.0f;
            transform_vector_by_basis(
                &light_offset_and_flame_velocity, &light_offset_and_flame_velocity, hero.basis);
            fast_vec_add(&point_lights[pyrocitor_vars->light].position,
                         &hero.motion.pos,
                         &light_offset_and_flame_velocity);
            point_lights[pyrocitor_vars->light].position.f[3] = 7.5f;
            lerp_vector(&light_color_and_flame_origin,
                        &pyrocitor_light_color_start_x_gp,
                        &pyrocitor_light_color_end_x_gp,
                        fast_sin(((f32)(D_L00_0015F5CC % 120) / 120.0f) * 6.28318f - 3.14159f) *
                                0.5f +
                            0.5f);
            lerp_vector(&point_lights[pyrocitor_vars->light],
                        &point_lights[pyrocitor_vars->light],
                        &light_color_and_flame_origin,
                        0.1f);
        }
        pos_out = vel_out = 0.0f;
        speed = convert_integer_to_float(item_text_variant[16]) * 4.0f;
        if (!(D_L00_0015F5CC & 3))
            gate = 1;
        else
            gate = 0;
        if (hero.state.current == 1 && (moby->flags & 1)) {
            fast_vec_normalize(&light_color_and_flame_origin, &l00_camera.unk350[8], -1.0f);
            fast_vec_add(
                &light_color_and_flame_origin, &light_color_and_flame_origin, &l00_camera.pos);
            fast_vec_normalize(&light_offset_and_flame_velocity,
                               l00_camera.unk350,
                               pyrocitor_flame_base_range_gp + speed + 0.75f);
            fast_vec_add(
                &endpoint, &light_offset_and_flame_velocity, &light_color_and_flame_origin);
        } else {
            qcopy(&light_color_and_flame_origin, &pos);
            fast_vec_normalize(&light_offset_and_flame_velocity,
                               &hero.facing_x,
                               pyrocitor_flame_base_range_gp + speed + 0.75f);
            fast_vec_add(&endpoint, &light_offset_and_flame_velocity, &pos);
        }
        if (collision_line(&hero.motion.body_center, &light_color_and_flame_origin, 2, 0, 0)) {
            length = 0.0f;
        } else if (collision_line(&light_color_and_flame_origin, &endpoint, 2, 0, 0)) {
            length = fast_vec_dist(&light_color_and_flame_origin, coll_hit + 0x20) - 0.75f;
            if (length < 0.0f)
                length = 0.0f;
        } else {
            length = pyrocitor_flame_base_range_gp + speed;
        }
        flame_braking_step = D_L00_001601E8 * frame_time_sq;
        damping = pyrocitor_flame_maximum_speed_gp * frame_time;
        advance_accelerated_scalar(&pos_out, &vel_out, length, 1.0f, flame_braking_step, damping);
        fast_vec_normalize(
            &light_offset_and_flame_velocity, &light_offset_and_flame_velocity, vel_out);
        {
            s32 initial_count, extra;
            initial_count = pyrocitor_flame_primary_particle_count_gp;
            extra = pyrocitor_flame_extra_particle_count_gp;
            if (length < 4.0f || (moby->flags & 1))
                initial_count = 1;
            for (i = 0; i < extra + initial_count; i++) {
                random_vector(&random_vec,
                              0.0f,
                              ((pyrocitor_flame_full_length_random_range_gp -
                                pyrocitor_flame_zero_length_random_range_gp) *
                                   (length / (pyrocitor_flame_base_range_gp + speed)) +
                               pyrocitor_flame_zero_length_random_range_gp) *
                                  frame_time);
                fast_vec_add(&random_vec, &random_vec, &light_offset_and_flame_velocity);
                fast_vec_scale(&particle_pos,
                               &light_offset_and_flame_velocity,
                               random_float_between(0.0f, 1.0f));
                fast_vec_add(&particle_pos, &particle_pos, &light_color_and_flame_origin);
                if (i < initial_count)
                    fl = 1;
                else
                    fl = 0;
                effect = spawn_spinning_and_fading_effect_particle(
                    &particle_pos, &random_vec, fl | 4, length);
                if (i == 2 && gate)
                    FUN_L00_002b8798(moby, effect);
            }
        }
        particle_pos.q = 0;
        particle_pos.f.x = -0.3f;
        transform_vector_by_basis(&random_vec, &particle_pos, hero.basis);
        fast_vec_add(&random_vec, &random_vec, &pos);
        hit.moby = moby;
        hit.flags = 0x10000;
        hit.scale = convert_integer_to_float(item_text_variant[16]) + 1.0f;
        hit.point.q = light_offset_and_flame_velocity.q;
        hit.mode = 1;
        set_vector_length_xy(&hit.point, &hit.point, 1.0f);
        hit.point.f.z = 1.0f;
        hit.point.f.w = 5627.9248f;
        hit.mode = 1;
        hit.type = 5;
        collect_mobies_in_sphere(&random_vec, 0.42f, 5, hero.moby, &hit);
        particle_pos.f.x = -0.8f;
        transform_vector_by_basis(&random_vec, &particle_pos, hero.basis);
        fast_vec_add(&random_vec, &random_vec, &pos);
        collect_mobies_in_sphere(&random_vec, 0.42f, 5, hero.moby, &hit);
        if (fast_dec_timer_s16(&pyrocitor_vars->spark)) {
            pyrocitor_vars->spark =
                truncate_float_to_s32(scale_time(random_float_between(10.0f, 30.0f)));
            random_vector(&random_vec, 0.0f, frame_time + frame_time);
            fast_vec_scale(
                &particle_pos, &light_offset_and_flame_velocity, random_float_between(0.5f, 0.1f));
            fast_vec_add(&particle_pos, &particle_pos, &random_vec);
            spawn_color_decaying_effect_particle(
                &light_color_and_flame_origin, &particle_pos, item_text_variant[16]);
        }
        if (D_L00_0015F5CC % scale_game_frames(pyrocitor_ammo_use_interval_frames_gp) == 0)
            use_ammo(-1, 1);
        ready = get_ammo(-1);
        if (fast_dec_timer_s16(&pyrocitor_vars->timer)) {
            if ((controller_state.held & hero.items[0].button_mask) && !hero.item_use_locked &&
                ready)
                break;
            moby->state = 2;
            cancel_hero_item_animation();
            firing_voice_slot_index = pyrocitor_vars->voice;
            if (firing_voice_slot_index != -1) {
                u8 *slot = (u8 *)&voice_pool + firing_voice_slot_index * 0x70;
                if (*(struct Moby **)(slot + 0x88) == moby && slot[0x74])
                    release_voice_slot(firing_voice_slot_index);
            }
            pyrocitor_vars->voice = -1;
            if (!ready)
                FUN_0022da68(0, 0, moby);
        }
        break;
    }
    if (pyrocitor_vars->other) {
        euler_rotation_basis(&pyrocitor_vars->other->rot_row0, &pyrocitor_vars->other->rot);
        multiply_matrix3x3(
            &pyrocitor_vars->other->rot_row0, &moby->rot_row0, &pyrocitor_vars->other->rot_row0);
        other = pyrocitor_vars->other;
        qcopy(&other->pos, &pos);
        update_entity(other);
    }
    update_pyrocitor_flame_effect_slots((char *)moby);
}

void update_pyrocitor_usage_and_cleanup(struct Moby *pyrocitor_moby) __asm__("FUN_L00_002b8230");

void update_pyrocitor_usage_and_cleanup(struct Moby *pyrocitor_moby) {
    unsigned char     *pyrocitor_vars = ((unsigned char *)pyrocitor_moby->pvars);
    struct UsageStats *P;
    unsigned char     *voice;
    int                voice_slot_or_time_stat, firing_frame_count;
    if (pyrocitor_moby->prev_seq == 2) {
        pyrocitor_moby->state = 4;
        if (*(void **)pyrocitor_vars)
            mark_moby_for_removal(*(void **)pyrocitor_vars);
        if (*(short *)(pyrocitor_vars + 0x44) != -1) {
            release_point_light_slot(*(short *)(pyrocitor_vars + 0x44));
            *(short *)(pyrocitor_vars + 0x44) = -1;
        }
        voice_slot_or_time_stat = *(short *)(pyrocitor_vars + 0x4A);
        if (voice_slot_or_time_stat != -1) {
            voice = (unsigned char *)&voice_pool + voice_slot_or_time_stat * 0x70;
            if (*(unsigned char **)(voice + 0x88) == pyrocitor_moby && voice[0x74])
                release_voice_slot(voice_slot_or_time_stat);
        }
        *(short *)(pyrocitor_vars + 0x4A) = -1;
        return;
    }
    if (D_00141BD8)
        return;
    P = &usage_stats;
    if (P->stat[10].count)
        return;
    if (pyrocitor_moby->state == 3) {
        (*(unsigned short *)(pyrocitor_vars + 0x50))++;
        return;
    }
    firing_frame_count = *(short *)(pyrocitor_vars + 0x50);
    if (firing_frame_count == 0)
        return;
    if (scale_game_frames(0x78) < firing_frame_count) {
        if (P->stat[10].count <= 0xFFFE)
            (P->stat[10].count)++;
        voice_slot_or_time_stat = scale_game_frames(D_0015EEA4) / 600;
        if (P->stat[10].unk2 < voice_slot_or_time_stat)
            P->stat[10].unk2 = scale_game_frames(D_0015EEA4) / 600;
        P->stat[10].level_mask = P->stat[10].level_mask | (1 << current_level_index) | 0x80000000;
    } else if (*(short *)(pyrocitor_vars + 0x50) < scale_game_frames(0x1E)) {
        hero.unk20B4++;
    }
    *(short *)(pyrocitor_vars + 0x50) = 0;
    if (hero.unk20B4 >= 3)
        help_request(0x4E24, 0x4E);
}

/* Walks the 15 entry pointers in the moby's data; for each live type-0xC entry builds a hit record and, for sub-type 10,
   spawns a burst of particles. Entries that are invalid or have sub-type < 2 are cleared. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B33E8.c: func_L00_002B9730), where it is exact; names translated to the US level program. */
void update_pyrocitor_flame_effect_slots(char *moby) __asm__("FUN_L00_002b8438");

void update_pyrocitor_flame_effect_slots(char *moby) {
    float          hit_direction_vector[4];
    char           hit_descriptor[0x30];
    char           qv[16];
    float          spark_random_vector[4];
    float          spark_position[4];
    char          *slots_base = ((char *)MOBY(moby)->pvars) + 4;
    unsigned char *item_variant_bytes = item_text_variant;
    int            i;

    for (i = 0; i < 15; i++) {
        char *flame_effect = *(char **)(slots_base - (-(i * 4)));
        if (flame_effect == 0) {
            continue;
        }
        if (*(unsigned char *)flame_effect == 0xC && flame_effect[1] >= 0) {
            float hit_sphere_radius = *(float *)(flame_effect + 0xC) / 210000.0f * 0.5f;
            char *entry_direction_vector = flame_effect + 0x20;
            int   sub_type;
            set_vector_length_xy(hit_direction_vector, entry_direction_vector, 1.0f);
            hit_direction_vector[2] = 1.0f;
            hit_direction_vector[3] = 5627.9248046875f;
            make_hit_descriptor(hit_descriptor,
                                (int)moby,
                                0x10000,
                                hit_direction_vector,
                                convert_integer_to_float(item_variant_bytes[0x10]) + 1.0f);
            *(unsigned char *)(hit_descriptor + 0x18) = 5;
            *(unsigned char *)(hit_descriptor + 0x19) = 1;
            *(unsigned short *)(hit_descriptor + 0x1A) = ((unsigned short)MOBY(moby)->oclass);
            collect_mobies_in_sphere(
                (flame_effect + 0x10), hit_sphere_radius, 0, moby, hit_descriptor);
            sub_type = *(short *)(flame_effect + 0xA);
            if (sub_type == 10) {
                int j;
                for (j = 0; j < *(int *)&pyrocitor_spark_burst_count_gp; j++) {
                    int spark_initial_color;
                    int spark_interpolated_color;
                    int spark_second_frame_count;
                    int spark_third_frame_count;
                    qcopy(qv, entry_direction_vector);
                    FUN_L00_00257d78(spark_random_vector,
                                     0.0f,
                                     *(float *)&pyrocitor_spark_random_vector_magnitude_scale_gp *
                                         frame_time);
                    *(float *)(qv + 0xC) = *(float *)&pyrocitor_spark_direction_vector_w_gp;
                    spark_random_vector[2] = frame_time * 4.0f;
                    spark_random_vector[3] = *(float *)&pyrocitor_spark_random_vector_w_gp;
                    spark_initial_color =
                        fast_tween_color(*(int *)&pyrocitor_spark_initial_color_start_gp,
                                         *(int *)&pyrocitor_spark_initial_color_end_gp,
                                         random_float_between(0.0f, 1.0f));
                    swap_packed_color_channels(spark_initial_color, item_variant_bytes[0x10]);
                    spark_interpolated_color =
                        fast_tween_color(*(int *)&pyrocitor_spark_second_color_start_gp,
                                         *(int *)&pyrocitor_spark_second_color_end_gp,
                                         random_float_between(0.0f, 1.0f));
                    swap_packed_color_channels(spark_interpolated_color, item_variant_bytes[0x10]);
                    spark_second_frame_count =
                        truncate_float_to_s32(scale_time(random_float_between(
                            (float)*(int *)&pyrocitor_spark_random_frame_count_min_gp,
                            (float)*(int *)&pyrocitor_spark_random_frame_count_max_gp)));
                    spark_third_frame_count = truncate_float_to_s32(scale_time(random_float_between(
                        (float)*(int *)&pyrocitor_spark_random_frame_count_min_gp,
                        (float)*(int *)&pyrocitor_spark_random_frame_count_max_gp)));
                    FUN_L00_00257d78(spark_position, 0.0f, hit_sphere_radius + hit_sphere_radius);
                    fast_vec_add(spark_position, spark_position, (flame_effect + 0x10));
                    spawn_spark(spark_position,
                                qv,
                                spark_random_vector,
                                spark_initial_color,
                                spark_interpolated_color,
                                0xA,
                                spark_second_frame_count,
                                spark_third_frame_count,
                                *(int *)&pyrocitor_spark_lookup_index_and_flags_gp);
                }
            } else if (sub_type < 2) {
                *(int *)(slots_base - (-(i * 4))) = 0;
            }
        } else {
            *(int *)(slots_base - (-(i * 4))) = 0;
        }
    }
}

/* Stores val in the first empty of 15 slots after the list head. */
void register_pyrocitor_flame_effect(ListSlots8 *list, s32 val) __asm__("FUN_L00_002b8798");

void register_pyrocitor_flame_effect(ListSlots8 *list, s32 val) {
    s32 *list_data = list->slots;
    s32  i = 0;
    s32 *first_slot;
    s32  slot_byte_offset;

    while (i < 15) {
        slot_byte_offset = i * 4;
        first_slot = (s32 *)((u8 *)list_data + 4);
        if (*(s32 *)((u8 *)first_slot + slot_byte_offset) == 0) {
            *(s32 *)((u8 *)first_slot + slot_byte_offset) = val;
            break;
        }
        i++;
    }
}
