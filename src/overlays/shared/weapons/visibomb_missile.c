/* Shared moby class visibomb_missile: the missile the hero steers from the visibomb gun, and the function that spawns it. */

#include "types.h"
#include "rnc/globals.h"
#include "rnc/hero/hero.h"
#include "rnc/data_labels.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/input/pad_state.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/overlay/shared/audio/voices/play_voice.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/ui/help/force_help_message.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/collision.h"
#include "rnc/overlay/shared/weapons/visibomb_missile_data/moby_save_globals_state.h"
#include "rnc/overlay/shared/weapons/visibomb_missile_data/visibomb_missile_update.h"

typedef struct {
    u8  p0[0x2084];
    s32 i2084;
} G;

typedef union {
    OvlQuad q;
    float   f[4];
} V_2b58d8;

typedef unsigned int u128_2b5ba0 __attribute__((mode(TI), aligned(16)));

typedef union {
    u128_2b5ba0 q;
    float       f[4];
} V_2b5ba0;

typedef struct {
    u128_2b5ba0 r[4];
} M_2b5ba0;

typedef struct {
    int v[6];
} V6;

typedef int u128_2b6140 __attribute__((mode(TI)));

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) V_2b6140;

typedef struct Ev {
    V_2b6140       pos;
    struct Moby   *moby;
    int            flags;
    unsigned char  b18;
    unsigned char  b19;
    unsigned short h1A;
    float          f1C;
    int            f20;
} Ev;

typedef struct Part {
    char     p0[0x10];
    V_2b6140 pos;
} Part;

typedef struct Tgt {
    char  p0[0x46];
    short h46;
    char  p48[0x5E];
    short hA6;
} Tgt;

typedef struct D_2b6140 {
    int   f0;
    float f4;
    char  p8[8];
    int   f10;
    void *f14;
    float f18;
    float f1C;
    char  p20[4];
    Part *f24;
    Part *f28;
    int   f2C;
    char  p30[0x14];
    float f44;
    float f48;
} D_2b6140;

typedef struct Hit_2b6140 {
    char         p0[0x18];
    struct Moby *f18;
    int          f1C;
    V_2b6140     v20;
    char         p30[0x10];
    V_2b6140     v40;
    V_2b6140     v50;
    V_2b6140     v60;
} Hit_2b6140;

void         jet_camera_mode(void) __asm__("FUN_L00_00203b18");
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern s32   random_integer_below(s32) __asm__("FUN_00213260");
extern char *create_moby(int);
extern int   FUN_L00_00257b90(int, int);
extern int   hero_set_state(int, int) __asm__("FUN_L00_002223f8");
extern void  PackDmaTag(s32 arg0, unsigned long arg1, unsigned long arg2);
extern void  set_background_color(int, int, int) __asm__("FUN_001fb280");
int          FUN_L00_002603d0_2b58d8(void *) __asm__("FUN_L00_002603d0");
void         FUN_L00_002b56d0_2b58d8(void *) __asm__("FUN_L00_002b56d0");
void         FUN_L00_00250df8_2b58d8(void *) __asm__("FUN_L00_00250df8");
int          initialize_mode6_orbit_camera(void *) __asm__("FUN_L00_002eb930");
void         fill_transfer_words(void *, int, int) __asm__("FUN_001f97e8");
void         attach_manipulator(void *, int, void *) __asm__("FUN_0020cb10");
int          FUN_L00_002bb360_2b58d8(void *) __asm__("FUN_L00_002bb360");
void         copy_hero_moby_field_38(void *) __asm__("FUN_L00_0025d1b8");
int          FUN_001efa68_2b58d8(void *, void *, int, void *, int) __asm__("FUN_001efa68");
void         select_camera_mode_by_hero_alignment_and_distance() __asm__("FUN_L00_002eba18");
void         FUN_L00_002b57f0_u() __asm__("FUN_L00_002b57f0");
void         clear_jet_camera_mode_latch() __asm__("FUN_L00_00203b78");
void         mark_moby_for_removal() __asm__("FUN_0020c828");
extern void  refresh_moby_spatial_bounds_from_basis(void *) __asm__("FUN_0020e098");
extern unsigned char             *
spawn_spinning_fade_in_out_effect_particle(void *pos,
                                                       int   spin,
                                                       int   col,
                                                       float range,
                                                       float f1,
                                                       float f2,
                                                       float f3,
                                                       float scale) __asm__("FUN_L00_0026b790");
extern void spawn_particle(void *, void *, int, int, int, int, int, int, float, float) __asm__("FUN_L00_0026a9f0");
extern void spawn_glow_particle(void *, void *, void *, int, int, int, int, int, float) __asm__("FUN_L00_002ac910");
extern int   update_orbit_camera_from_target_moby(void *, void *) __asm__("FUN_L00_002eb648");
extern void  func_L00_002B6AE0_2b6140(void *, int) __asm__("FUN_L00_002b57f0");
extern void  func_L00_002B6E90_2b6140(void *, void *) __asm__("FUN_L00_002b5ba0");
extern void  func_L00_002B6E10_2b6140(void *, void *) __asm__("FUN_L00_002b5b20");
extern void  func_L00_002B6FF0_2b6140(void *, int) __asm__("FUN_L00_002b5d00");
extern void  jet_animate(void *, float) __asm__("FUN_L00_00259fe8");
extern float approach_value(float *, float, float) __asm__("FUN_00213ed8");
extern int   collision_line_2b6140(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern void  reflect_vector(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern void  spawn_explosion(void *,
                             void *,
                             void *,
                             float,
                             float,
                             int,
                             int,
                             int,
                             float,
                             float,
                             float,
                             float,
                             int,
                             float,
                             int,
                             int,
                             int,
                             int) __asm__("FUN_L00_0025e450");
extern int
collect_mobies_in_sphere_2b6140(void *, float, int, void *, void *) __asm__("FUN_L00_001f2868");
extern void
send_blast_hits(void *, void *, void *, int, int, int, int, int, float, float, float) __asm__("FUN_L00_0025a9f8");
extern void add_random_offset_to_vector_xyz(void *, float) __asm__("FUN_L00_0025f8e0");

extern unsigned char D_L00_0015F444_e[] __asm__("D_L00_0015F444")
    __attribute__((section(".sdata")));
extern float              D_L00_0015F448;
extern float              D_L00_0015F448_macro[] __asm__("D_L00_0015F448") MACRO_ADDR;
extern int D_L00_00169FF8 NOT_SDA;
extern char               D_L00_0016CA40[] NOT_SDA;
extern unsigned char      D_L00_0015F444;
extern OvlQuad            D_L00_00173E60_lui __asm__("D_L00_00173E60") NOT_SDA;
extern int D_L00_0017E508 NOT_SDA;
extern int                D_L00_0015F608;
extern float visibomb_missile_default_speed_gp __asm__("D_L00_0016149C") __attribute__((sda));
extern int   D_00141330[];
extern V6 D_L00_001E9A58 NOT_SDA;
extern V6 D_L00_001E9A70 NOT_SDA;
extern char              D_L00_00166DC0[] NOT_SDA;
extern float             frame_time_macro __asm__("D_0015ED6C") MACRO_ADDR;
extern char              D_L00_00173E40_2b6140[] __asm__("D_L00_00173E40");
extern char              D_L00_00177F00_2b6140[] __asm__("D_L00_00177F00");
extern float             frame_scale_macro __asm__("D_0015ED60") MACRO_ADDR;
extern int               D_L00_0015F458 __attribute__((sda));
extern float visibomb_missile_max_speed_gp __asm__("D_L00_001614A0") __attribute__((sda));
extern float visibomb_missile_speed_target_blend_factor_gp __asm__("D_L00_001614A4")
    __attribute__((sda));
extern float visibomb_missile_speed_blend_factor_gp __asm__("D_L00_001614A8") __attribute__((sda));
extern int   D_L00_0015F608_macro __asm__("D_L00_0015F608") MACRO_ADDR;

float              D_L01_0015F448 = 0;
unsigned char      D_L01_0015F444 = 0;
int D_L01_0017E988 NOT_SDA = 0;
int                D_L01_0015F608 = 0;
float              D_L01_001614C4 = 24.0f;

V6 D_L01_0020AAB8 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

V6 D_L01_0020AAD0 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

int           D_L01_0015F458 = 0;
float         D_L01_001614C8 = 34.0f;
float         D_L01_001614CC = 0.1f;
float         D_L01_001614D0 = 0.01f;
float         D_L02_0015F448 = 0;
unsigned char D_L02_0015F444 = 0;
float         D_L02_0016146C = 24.0f;

V6 D_L02_001FA358 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

V6 D_L02_001FA370 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

float               D_L02_00161470 = 34.0f;
float               D_L02_00161474 = 0.1f;
float               D_L02_00161478 = 0.01f;
float               D_L03_0015F448 = 0;
char D_L03_0016CB40 NOT_SDA = 0;
unsigned char       D_L03_0015F444 = 0;
int D_L03_0017E608  NOT_SDA = 0;
int                 D_L03_0015F608 = 0;
float               D_L03_0016147C = 24.0f;

V6 D_L03_001E2BE8 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

V6 D_L03_001E2C00 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

int                 D_L03_0015F458 = 0;
float               D_L03_00161480 = 34.0f;
float               D_L03_00161484 = 0.1f;
float               D_L03_00161488 = 0.01f;
float               D_L04_0015F448 = 0;
char D_L04_0016CBC0 NOT_SDA = 0;
unsigned char       D_L04_0015F444 = 0;
int D_L04_0017E688  NOT_SDA = 0;
int                 D_L04_0015F608 = 0;
float               D_L04_0016146C = 24.0f;

V6 D_L04_001DB7F8 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

V6 D_L04_001DB810 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

int                 D_L04_0015F458 = 0;
float               D_L04_00161470 = 34.0f;
float               D_L04_00161474 = 0.1f;
float               D_L04_00161478 = 0.01f;
char D_L05_0016CE40 NOT_SDA = 0;
int D_L05_0017E988  NOT_SDA = 0;
int                 D_L05_0015F608 = 0;
float               D_L05_001614BC = 24.0f;

V6 D_L05_00218168 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

V6 D_L05_00218180 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

int                 D_L05_0015F458 = 0;
float               D_L05_001614C0 = 34.0f;
float               D_L05_001614C4 = 0.1f;
float               D_L05_001614C8 = 0.01f;
float               D_L06_0015F448 = 0;
char D_L06_0016D1C0 NOT_SDA = 0;
unsigned char       D_L06_0015F444 = 0;
int D_L06_0017EC88  NOT_SDA = 0;
int                 D_L06_0015F608 = 0;
float               D_L06_00161494 = 24.0f;

V6 D_L06_00200E58 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

V6 D_L06_00200E70 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

int                 D_L06_0015F458 = 0;
float               D_L06_00161498 = 34.0f;
float               D_L06_0016149C = 0.1f;
float               D_L06_001614A0 = 0.01f;
float               D_L07_0015F448 = 0;
char D_L07_0016CAC0 NOT_SDA = 0;
unsigned char       D_L07_0015F444 = 0;
int D_L07_0017E588  NOT_SDA = 0;
int                 D_L07_0015F608 = 0;
float               D_L07_0016146C = 24.0f;

V6 D_L07_00210CF8 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

V6 D_L07_00210D10 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

int                D_L07_0015F458 = 0;
float              D_L07_00161470 = 34.0f;
float              D_L07_00161474 = 0.1f;
float              D_L07_00161478 = 0.01f;
float              D_L08_0015F448 = 0;
unsigned char      D_L08_0015F444 = 0;
int D_L08_0017ED08 NOT_SDA = 0;
int                D_L08_0015F608 = 0;
float              D_L08_001614BC = 24.0f;

V6 D_L08_001FB9B8 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

V6 D_L08_001FB9D0 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

int                D_L08_0015F458 = 0;
float              D_L08_001614C0 = 34.0f;
float              D_L08_001614C4 = 0.1f;
float              D_L08_001614C8 = 0.01f;
float              D_L09_0015F448 = 0;
unsigned char      D_L09_0015F444 = 0;
int D_L09_0017E688 NOT_SDA = 0;
int                D_L09_0015F608 = 0;
float              D_L09_00161524 = 24.0f;

V6 D_L09_00208648 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

V6 D_L09_00208660 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

float              D_L09_00161528 = 34.0f;
float              D_L09_0016152C = 0.1f;
float              D_L09_00161530 = 0.01f;
float              D_L10_0015F448 = 0;
unsigned char      D_L10_0015F444 = 0;
int D_L10_0017E988 NOT_SDA = 0;
int                D_L10_0015F608 = 0;
float              D_L10_0016147C = 24.0f;

V6 D_L10_001DC848 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

V6 D_L10_001DC860 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

int                D_L10_0015F458 = 0;
float              D_L10_00161480 = 34.0f;
float              D_L10_00161484 = 0.1f;
float              D_L10_00161488 = 0.01f;
float              D_L11_0015F448 = 0;
unsigned char      D_L11_0015F444 = 0;
int D_L11_0017EF08 NOT_SDA = 0;
float              D_L11_001614FC = 24.0f;

V6 D_L11_0021ACD8 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

V6 D_L11_0021ACF0 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

int                 D_L11_0015F458 = 0;
float               D_L11_00161500 = 34.0f;
float               D_L11_00161504 = 0.1f;
float               D_L11_00161508 = 0.01f;
float               D_L12_0015F448 = 0;
char D_L12_0016CEC0 NOT_SDA = 0;
unsigned char       D_L12_0015F444 = 0;
int D_L12_0017E988  NOT_SDA = 0;
int                 D_L12_0015F608 = 0;
float               D_L12_0016147C = 24.0f;

V6 D_L12_0020BFA8 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

V6 D_L12_0020BFC0 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

int                 D_L12_0015F458 = 0;
float               D_L12_00161480 = 34.0f;
float               D_L12_00161484 = 0.1f;
float               D_L12_00161488 = 0.01f;
float               D_L13_0015F448 = 0;
char D_L13_0016CD40 NOT_SDA = 0;
unsigned char       D_L13_0015F444 = 0;
int D_L13_0017E808  NOT_SDA = 0;
float               D_L13_0016154C = 24.0f;

V6 D_L13_001F4A00 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

V6 D_L13_001F4A18 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

int                D_L13_0015F458 = 0;
float              D_L13_00161550 = 34.0f;
float              D_L13_00161554 = 0.1f;
float              D_L13_00161558 = 0.01f;
float              D_L14_0015F448 = 0;
unsigned char      D_L14_0015F444 = 0;
int D_L14_0017EC08 NOT_SDA = 0;
int                D_L14_0015F608 = 0;
float              D_L14_00161684 = 24.0f;

V6 D_L14_001FC288 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

V6 D_L14_001FC2A0 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

int                 D_L14_0015F458 = 0;
float               D_L14_00161688 = 34.0f;
float               D_L14_0016168C = 0.1f;
float               D_L14_00161690 = 0.01f;
char D_L15_0016D040 NOT_SDA = 0;
int D_L15_0017EB08  NOT_SDA = 0;
int                 D_L15_0015F608 = 0;
float               D_L15_00161624 = 24.0f;

V6 D_L15_001E2918 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

V6 D_L15_001E2930 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

int                 D_L15_0015F458 = 0;
float               D_L15_00161628 = 34.0f;
float               D_L15_0016162C = 0.1f;
float               D_L15_00161630 = 0.01f;
float               D_L16_0015F448 = 0;
char D_L16_0016CE40 NOT_SDA = 0;
unsigned char       D_L16_0015F444 = 0;
int D_L16_0017E988  NOT_SDA = 0;
int                 D_L16_0015F608 = 0;
float               D_L16_0016152C = 24.0f;

V6 D_L16_001E7DA8 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

V6 D_L16_001E7DC0 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

int                 D_L16_0015F458 = 0;
float               D_L16_00161530 = 34.0f;
float               D_L16_00161534 = 0.1f;
float               D_L16_00161538 = 0.01f;
float               D_L17_0015F448 = 0;
char D_L17_0016D340 NOT_SDA = 0;
unsigned char       D_L17_0015F444 = 0;
int D_L17_0017EE08  NOT_SDA = 0;
float               D_L17_0016157C = 24.0f;

V6 D_L17_001E64E8 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

V6 D_L17_001E6500 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

int                 D_L17_0015F458 = 0;
float               D_L17_00161580 = 34.0f;
float               D_L17_00161584 = 0.1f;
float               D_L17_00161588 = 0.01f;
float               D_L18_0015F448 = 0;
char D_L18_0016D440 NOT_SDA = 0;
unsigned char       D_L18_0015F444 = 0;
int                 D_L18_0015F608 = 0;
float               D_L18_0016150C = 24.0f;

V6 D_L18_001F2188 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

V6 D_L18_001F21A0 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

int   D_L18_0015F458 = 0;
float D_L18_00161510 = 34.0f;
float D_L18_00161514 = 0.1f;
float D_L18_00161518 = 0.01f;

/* Ported from rac1-decomp (src/overlays/shared/vendor_002B33E8.c: func_L00_002B69C0), where it is exact; names translated to the US level program. */
void moby_save_globals_state(void *m_) __asm__("FUN_L00_002b56d0");

void moby_save_globals_state(void *m_) {
    char *render_state_owner_moby = (char *)m_;
    char *render_state_backup_vars;
    float f0, f1, f2, f3;

    render_state_backup_vars = ((char *)MOBY(render_state_owner_moby)->pvars);
    f0 = D_L00_0015F448;
    f1 = D_L00_0015F44C;
    f2 = D_L00_0015F450;
    f3 = D_L00_0015F448_macro[3];
    visibomb_render_override_active = 1;
    *(float *)(render_state_backup_vars + 0x30) = f0;
    *(float *)(render_state_backup_vars + 0x34) = f1;
    *(float *)(render_state_backup_vars + 0x38) = f2;
    *(float *)(render_state_backup_vars + 0x3C) = f3;
    render_state_backup_vars[0x40] = D_L00_0015F444_e[0];
    render_state_backup_vars[0x41] = D_L00_0015F444_e[1];
    render_state_backup_vars[0x42] = D_L00_0015F444_e[2];
    D_L00_0015F444_e[0] = 0x40;
    D_L00_0015F448 = 0;
    D_L00_0015F444_e[1] = 0x60;
    D_L00_0015F454 = 0;
    D_L00_0015F444_e[2] = 0x40;
    D_L00_00160FE0 = 144.0f;
    D_L00_0015F44C = 131072.0f;
    D_L00_0015F450 = 255.0f;
    D_L00_00169FF8 = 0;
    D_L00_00160F80 = 147456.0f;
    D_L00_001604A4 = 144.0f;
    D_L00_0015FFF0 = 0x90;
    D_L00_0016017C = 0x50000;
    PackDmaTag(0x40, 0x60, 0x40);
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002B33E8.c: func_L00_002B6AE0), where it is exact; names translated to the US level program. */
void moby_restore_globals_state(int m_, int flag) __asm__("FUN_L00_002b57f0");

void moby_restore_globals_state(int m_, int flag) {
    struct Moby *render_restore_moby = (char *)m_;
    char        *render_restore_moby_vars;
    float        f0, f1, f2, f3;
    char        *background_color_source;

    render_restore_moby_vars = ((char *)render_restore_moby->pvars);
    visibomb_render_override_active = 0;
    f0 = *(float *)(render_restore_moby_vars + 0x30);
    f1 = *(float *)(render_restore_moby_vars + 0x34);
    f2 = *(float *)(render_restore_moby_vars + 0x38);
    f3 = *(float *)(render_restore_moby_vars + 0x3C);
    D_L00_0015F448 = f0;
    D_L00_0015F44C = f1;
    D_L00_0015F450 = f2;
    D_L00_0015F448_macro[3] = f3;
    D_L00_0015F444 = render_restore_moby_vars[0x40];
    D_L00_0015F445 = render_restore_moby_vars[0x41];
    D_L00_0015F446 = render_restore_moby_vars[0x42];
    D_L00_00169FF8 = 1;
    if (flag != 0) {
        D_L00_00160F80 = 512000.0f;
        D_L00_00160FE0 = 720.0f;
        D_L00_001604A4 = 500.0f;
        D_L00_0015FFF0 = 500;
        D_L00_0016017C = 0x1F4000;
    }
    background_color_source = D_L00_0016CA40;
    set_background_color(*(int *)(background_color_source + 0x23C),
                         *(int *)(background_color_source + 0x240),
                         *(int *)(background_color_source + 0x244));
}

unsigned char *spawn_visibomb_missile(unsigned char *source_moby,
                                      V_2b58d8      *pos,
                                      float          fa,
                                      float          fb) __asm__("FUN_L00_002b58d8");

unsigned char *

spawn_visibomb_missile(unsigned char *source_moby, V_2b58d8 *launch_position, float fa, float fb) {
    unsigned char *spawned_moby = create_moby(0xAC);
    if (spawned_moby) {
        unsigned char *spawned_moby_vars = ((unsigned char *)MOBY(spawned_moby)->pvars);
        struct Hero   *G = &hero;
        V_2b58d8       collision_start;
        G->unk1FE0 = spawned_moby;
        D_L00_0017E508 = 1;
        spawned_moby[0x30] = 0xFF;
        MOBY(spawned_moby)->draw_dist = 0xFF;
        spawned_moby[0x31] = 1;
        spawned_moby[0x20] = 1;
        MOBY(spawned_moby)->rot.z = fa;
        MOBY(spawned_moby)->rot.y = fb;
        *(unsigned char **)(spawned_moby_vars + 0x24) = G->unk1F60;
        *(unsigned char **)(spawned_moby_vars + 0x28) = G->unk1FA0;
        *(unsigned char **)(spawned_moby_vars + 0x0) = source_moby;
        qcopy(spawned_moby + 0x10, launch_position);
        *(int *)(spawned_moby_vars + 0x8) = 0;
        *(int *)(spawned_moby_vars + 0xC) = 0;
        if ((void *)G->ground_moby && FUN_L00_002603d0_2b58d8((void *)G->ground_moby))
            *(float *)(spawned_moby_vars + 0x4) = fast_vec_length(&G->motion.displacement);
        else
            *(int *)(spawned_moby_vars + 0x4) = 0;
        *(int *)(spawned_moby_vars + 0x10) = scale_game_frames(0xBB8);
        *(int *)(spawned_moby_vars + 0x18) = 0;
        *(int *)(spawned_moby_vars + 0x1C) = 0;
        *(int *)(spawned_moby_vars + 0x14) = 0;
        *(float *)(spawned_moby_vars + 0x44) = 0.4f;
        FUN_L00_002b56d0_2b58d8(spawned_moby);
        FUN_L00_00250df8_2b58d8(spawned_moby);
        *(int *)(spawned_moby_vars + 0x14) = initialize_mode6_orbit_camera(spawned_moby);
        hero_set_state(0x1D, 1);
        fill_transfer_words(*(void **)(spawned_moby_vars + 0x24), 0, 0x40);
        fill_transfer_words(*(void **)(spawned_moby_vars + 0x28), 0, 0x40);
        attach_manipulator(spawned_moby, 0, *(void **)(spawned_moby_vars + 0x24));
        attach_manipulator(spawned_moby, 1, *(void **)(spawned_moby_vars + 0x28));
        {
            int   missile_attachment_moby = FUN_L00_002bb360_2b58d8(spawned_moby);
            float k = visibomb_missile_default_speed_gp * frame_time;
            *(int *)(spawned_moby_vars + 0x20) = missile_attachment_moby;
            *(float *)(spawned_moby_vars + 0x48) = k;
        }
        copy_hero_moby_field_38(spawned_moby);
        *(int *)(spawned_moby_vars + 0x2C) = allocate_voice_for_target_entry(0, 4, spawned_moby);
        collision_start.q = *(OvlQuad *)&MOBY(source_moby)->pos;
        collision_start.f[2] = launch_position->f[2];
        if (FUN_001efa68_2b58d8(&collision_start,
                                launch_position,
                                0,
                                ({
                                    struct Hero *collision_ignored_hero = &hero;
                                    (void *)collision_ignored_hero->moby;
                                }),
                                0)) {
            spawned_moby[0xBC] = 1;
            qcopy(spawned_moby + 0x10, &D_L00_00173E60_lui);
        }
        D_L00_0015F608 = 1;
        force_help_message(6, 0);
    }
    return spawned_moby;
}

/* Clears the moby state word 4 and the 0x14 handle, calls release helpers, marks a for removal. */
void terminate_visibomb_missile(void  *missile_moby,
                                char **missile_vars) __asm__("FUN_L00_002b5b20");

void terminate_visibomb_missile(void *missile_moby, char **missile_vars) {
    char *x = *(char **)(*missile_vars + 0x78);
    if (x)
        *(int *)(x + 4) = 0;
    D_00141330[0] = 0;
    select_camera_mode_by_hero_alignment_and_distance(*(int *)((char *)missile_vars + 0x14));
    *(int *)((char *)missile_vars + 0x14) = 0;
    hero_set_state(HERO_STATE_IDLE, 1);
    FUN_L00_002b57f0_u(missile_moby, 1);
    clear_jet_camera_mode_latch();
    mark_moby_for_removal(missile_moby);
    D_L00_00167014[0] = 0;
}

void update_attached_moby_transform(unsigned char *source_moby,
                                    unsigned char *attachment_state) __asm__("FUN_L00_002b5ba0");

void update_attached_moby_transform(unsigned char *source_moby, unsigned char *attachment_state) {
    M_2b5ba0       combined_rotation, attachment_rotation, source_rotation;
    V_2b5ba0       k, rotated_attachment_offset;
    struct Moby   *attached_moby = *(unsigned char **)(attachment_state + 0x20);
    unsigned char *attached_moby_vars;
    if (attached_moby == 0)
        return;
    k.q = 0;
    k.f[1] = 0.11f;
    attached_moby_vars = ((unsigned char *)attached_moby->pvars);
    if (0.45f < *(float *)(attached_moby_vars + 0xC)) {
        *(float *)(attached_moby_vars + 0xC) =
            *(float *)(attached_moby_vars + 0xC) - D_0015ED60_gp * 0.05f;
    } else if (*(float *)(attached_moby_vars + 0xC) < 0.45f) {
        *(float *)(attached_moby_vars + 0xC) =
            *(float *)(attached_moby_vars + 0xC) + D_0015ED60_gp * 0.05f;
    }
    *(float *)(*(unsigned char **)(attachment_state + 0x20) + 0x48) = frame_scale * -1.5707964f;
    euler_rotation_matrix(&attachment_rotation,
                          *(unsigned char **)(attachment_state + 0x20) + 0x40);
    copy_matrix_rotation(&source_rotation, source_moby + 0xC0);
    multiply_matrix(&combined_rotation, &source_rotation, &attachment_rotation);
    copy_matrix3x4(*(unsigned char **)(attachment_state + 0x20) + 0xC0, &combined_rotation);
    transform_vector(&rotated_attachment_offset, &k, &combined_rotation);
    fast_vec_add(*(unsigned char **)(attachment_state + 0x20) + 0x10,
                 &rotated_attachment_offset,
                 source_moby + 0x10);
    refresh_moby_spatial_bounds_from_basis(*(unsigned char **)(attachment_state + 0x20));
}

/* spawns a burst of sparks and glow flashes at a moby, scaled by its distance from the origin marker */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B33E8.c: func_L00_002B6FF0), where it is exact; names translated to the US level program. */
void spawn_moby_particle_and_glow_burst(char *source_moby, int flag) __asm__("FUN_L00_002b5d00");

void spawn_moby_particle_and_glow_burst(char *source_moby, int flag) {
    float elevated_spawn_position[4];
    float view_origin_delta[4];
    float particle_velocity[4];
    V6    A;
    V6    B;
    float view_origin_distance, extra, sc;
    int   count, i, j;

    qcopy(elevated_spawn_position, source_moby + 0x10);
    elevated_spawn_position[2] += 0.3f;
    spawn_spinning_fade_in_out_effect_particle(
        elevated_spawn_position, 1, 0x40808080, 0.3f, 1.01f, 1.075f, 0.05f, 50000.0f);
    for (j = 4; j >= 0; j--) {
        spawn_spinning_fade_in_out_effect_particle(
            elevated_spawn_position, 1, 0x40808080, 0.3f, 1.01f, 1.07f, 0.05f, 50000.0f);
    }
    if (flag != 0) {
        fast_vec_sub(view_origin_delta, D_L00_00166DC0, source_moby + 0x10);
        clear_u64_value(particle_velocity);
        view_origin_distance = fast_vec_length(view_origin_delta);
        if (view_origin_distance < 8.0f) {
            count = truncate_float_to_s32(view_origin_distance) + 2;
        } else {
            count = 10;
        }
        extra = 0.0f;
        if (view_origin_distance < 7.0f)
            extra = 7.0f - view_origin_distance;
        if (count > 0) {
            i = count;
            do {
                int *pa, *pb;
                int  particle_frame_count, particle_random_frame_value;
                sc = random_float_between(8.0f, 10.0f) * frame_time - extra * frame_time;
                A = D_L00_001E9A58;
                B = D_L00_001E9A70;
                pa = A.v + random_integer_below(6);
                pb = B.v + random_integer_below(6);
                particle_frame_count =
                    FUN_L00_00257b90(scale_game_frames(15), scale_game_frames(20));
                particle_random_frame_value =
                    FUN_L00_00257b90(scale_game_frames(25), scale_game_frames(30));
                spawn_particle(source_moby + 0x10,
                               particle_velocity,
                               *pa,
                               *pb,
                               particle_frame_count,
                               particle_random_frame_value,
                               0,
                               0,
                               400000.0f,
                               sc);
            } while (--i);
        }
        if (D_L00_0015F5D0 < 0.95f) {
            spawn_glow_particle(source_moby,
                                source_moby + 0x10,
                                particle_velocity,
                                scale_game_frames(15),
                                0x7F,
                                0x7F,
                                0x7F,
                                0x20,
                                3.0f);
            spawn_glow_particle(source_moby,
                                source_moby + 0x10,
                                particle_velocity,
                                scale_game_frames(24),
                                0x7F,
                                0x20,
                                0,
                                0x20,
                                2.0f);
        }
        spawn_glow_particle(source_moby,
                            source_moby + 0x10,
                            particle_velocity,
                            scale_game_frames(20),
                            0x7F,
                            0x40,
                            0,
                            0x30,
                            3.0f);
        spawn_glow_particle(source_moby,
                            source_moby + 0x10,
                            particle_velocity,
                            scale_game_frames(27),
                            0x60,
                            0x10,
                            0,
                            0x40,
                            2.0f);
        spawn_glow_particle(source_moby,
                            source_moby + 0x10,
                            particle_velocity,
                            scale_game_frames(29),
                            0x20,
                            0,
                            0,
                            0x20,
                            1.0f);
    }
}

/* visibomb_missile update (moby class 172): drift, homing toward the target, arming countdown. */
void visibomb_missile_update(struct Moby *m) __asm__("FUN_L00_002b6140");

void visibomb_missile_update(struct Moby *missile_moby) {
    V_2b6140  v20;
    V_2b6140  k30;
    V_2b6140  k40;
    V_2b6140  v50;
    V_2b6140  v60;
    Ev        ev;
    V_2b6140  vA0;
    D_2b6140 *missile_vars = (D_2b6140 *)missile_moby->pvars;
    int       direct = 0;

    *(u128_2b6140 *)&k40 = 0;
    k40.z = frame_time_macro * 8.0f;
    *(u128_2b6140 *)&k30 = *(u128_2b6140 *)&k40;
    *(u128_2b6140 *)&k40 = 0;
    k40.z = 1.0f;
    D_L00_0015F608_macro = 1;
    D_L00_0015F458 = 1;
    jet_camera_mode();
    D_L00_0017E508 = 1;
    force_help_message(6, 0);
    missile_vars->f10--;
    if (!(-scale_game_frames(100) < missile_vars->f10) || hero.state.current == 0x16 ||
        hero.state.current == 0x3D || hero.state.current == 0x72 || hero.state.control_mode == 2 ||
        hero.state.control_mode == 4 || hero.state.control_mode == 3 ||
        hero.state.control_mode > 12 || hero.state.control_mode == 7 ||
        hero.state.control_mode == 10 || hero.state.control_mode == 16 ||
        hero.state.control_mode == 5 || hero.state.current == 0x65 || hero.grounded_frames == 0 ||
        (((Tgt *)hero.ground_moby) != 0 &&
         (((Tgt *)hero.ground_moby)->hA6 == 0x13E || ((Tgt *)hero.ground_moby)->hA6 == 0x46F))) {
        func_L00_002B6E10_2b6140(missile_moby, missile_vars);
        return;
    }
    if (missile_vars->f10 < 0) {
        if (missile_vars->f14 != 0)
            update_orbit_camera_from_target_moby(missile_vars->f14, missile_moby);
        func_L00_002B6AE0_2b6140(missile_moby, 0);
        func_L00_002B6E90_2b6140(missile_moby, missile_vars);
        return;
    }
    {
        float t = visibomb_missile_default_speed_gp * frame_time_macro;
        if (controller_state.held & 0x40)
            t = visibomb_missile_max_speed_gp * frame_time_macro;
        missile_vars->f48 +=
            (t - missile_vars->f48) * visibomb_missile_speed_target_blend_factor_gp;
        missile_vars->f4 +=
            (missile_vars->f48 - missile_vars->f4) * visibomb_missile_speed_blend_factor_gp;
        if (visibomb_missile_max_speed_gp * frame_time_macro < missile_vars->f4)
            missile_vars->f4 = visibomb_missile_max_speed_gp * frame_time_macro;
    }
    engine_voice_set_pitch(
        missile_vars->f2C,
        truncate_float_to_s32(
            (missile_vars->f48 - visibomb_missile_default_speed_gp * frame_time_macro /
                                     (visibomb_missile_max_speed_gp * frame_time_macro -
                                      visibomb_missile_default_speed_gp * frame_time_macro)) *
            visibomb_missile_speed_voice_control_scale_gp));
    if (missile_vars->f10 > 0) {
        if (missile_vars->f10 < scale_game_frames(0xBB8) - scale_game_frames(5)) {
            float steered_pitch;
            float dx = hero.stick_x - missile_vars->f18;
            float dy = hero.stick_y - missile_vars->f1C;
            float ka = frame_scale_macro * 0.1f;
            float kb = frame_scale_macro * 0.06f;
            float kc = frame_scale_macro * 0.0122718466f;
            missile_vars->f18 += dx * ka;
            missile_vars->f1C += dy * ka;
            missile_moby->rot.x += (missile_vars->f18 - missile_moby->rot.x) * kb;
            steered_pitch = fast_subtract_rotations(missile_moby->rot.y, missile_vars->f1C * kc);
            missile_moby->rot.y = steered_pitch;
            if (1.57079637f <= steered_pitch) {
                missile_moby->rot.y = 1.56979632f;
            } else if (steered_pitch <= -1.57079637f) {
                missile_moby->rot.y = -1.56979632f;
            }
            missile_moby->rot.z = fast_subtract_rotations(
                missile_moby->rot.z, missile_vars->f18 * (frame_scale_macro * 0.0122718466f));
            jet_animate(missile_moby, 0.2f);
        }
    }
    build_spherical_offset(&v50, missile_vars->f4, missile_moby->rot.z, -missile_moby->rot.y);
    qcopy(&v20, &missile_moby->pos);
    fast_vec_add(&missile_moby->pos, &missile_moby->pos, &v50);
    if (scale_game_frames(0xBB8) - scale_game_frames(0x3C) < missile_vars->f10) {
        float t = probe_ground_height(&missile_moby->pos, 0, 0.5f) + missile_vars->f44;
        if (missile_moby->pos.z < t) {
            if (coll_hit_moby == 0 ||
                (coll_hit_moby->pclass != 0 && coll_hit_moby->pclass->type == 5)) {
                float ground_clearance_pitch_adjustment =
                    fast_arc_tan(missile_vars->f4,
                                 approach_value(&missile_moby->pos.z, t, frame_time_macro * 6.0f));
                if (frame_time_macro * 1.57079637f < ground_clearance_pitch_adjustment)
                    ground_clearance_pitch_adjustment = frame_time_macro * 1.57079637f;
                missile_moby->rot.y -= ground_clearance_pitch_adjustment;
            }
        }
    }
    if (missile_moby->pos.x < 4.5f || 1018.5f < missile_moby->pos.x || missile_moby->pos.y < 4.5f ||
        1018.5f < missile_moby->pos.y || missile_moby->pos.z < 4.5f ||
        1018.5f < missile_moby->pos.z) {
        func_L00_002B6E10_2b6140(missile_moby, missile_vars);
        return;
    }
    func_L00_002B6E90_2b6140(missile_moby, missile_vars);
    {
        struct Moby *skip = missile_moby;
        rotation_2D5FA8(&missile_vars->f24->pos,
                        1,
                        -(missile_vars->f1C * 0.5f + absolute_float(missile_vars->f18 / 3.0f) +
                          (missile_vars->f18 - missile_moby->rot.x) * 1.1f) *
                            0.7f);
        rotation_2D5FA8(&missile_vars->f28->pos,
                        1,
                        -(missile_vars->f1C * 0.5f + absolute_float(missile_vars->f18 / 3.0f) -
                          (missile_vars->f18 - missile_moby->rot.x) * 1.1f) *
                            0.7f);
        ev.moby = missile_moby;
        ev.flags = 0x830000;
        ev.f1C = 6.0f;
        ev.f20 = 1;
        *(u128_2b6140 *)&ev.pos = *(u128_2b6140 *)&v50;
        set_vector_length_xy(&ev, &ev, 1.0f);
        ev.b18 = 3;
        ev.pos.w = 5627.9248f;
        ev.pos.z = 1.0f;
        ev.b19 = 2;
        ev.h1A = (*(unsigned short *)&missile_moby->oclass);
        if (scale_game_frames(0xBB8) - 40 < missile_vars->f10)
            skip = hero.moby;
        if (collision_line_2b6140(&v20, &missile_moby->pos, 0, skip, &ev)) {
            if (((Hit_2b6140 *)D_L00_00173E40_2b6140)->f18 != 0) {
                direct = 0;
                if (((Hit_2b6140 *)D_L00_00173E40_2b6140)->f18 == hero.moby)
                    direct = 1;
                missile_moby->command = 2;
                qcopy(&missile_moby->pos, &((Hit_2b6140 *)D_L00_00173E40_2b6140)->v20);
                fast_vec_sub(&v60,
                             &((Hit_2b6140 *)D_L00_00173E40_2b6140)->v50,
                             &((Hit_2b6140 *)D_L00_00173E40_2b6140)->v60);
                fast_vec_normalize(&v60, &v60, 1.0f);
                reflect_vector(&k30, &v50, &((Hit_2b6140 *)D_L00_00173E40_2b6140)->v40);
                fast_vec_normalize(&k30, &k30, frame_time_macro + frame_time_macro);
                fast_vec_normalize(&k40, &((Hit_2b6140 *)D_L00_00173E40_2b6140)->v40, 1.0f);
                if (missile_vars->f2C != -1) {
                    u8 *e = (u8 *)&voice_pool + missile_vars->f2C * 0x70;
                    if (*(struct Moby **)(e + 0x88) == missile_moby) {
                        if (e[0x74] != 0)
                            release_voice_slot(missile_vars->f2C);
                    }
                }
                missile_vars->f2C = -1;
            } else if (((Hit_2b6140 *)D_L00_00173E40_2b6140)->f1C > 0) {
                missile_moby->command = 1;
                qcopy(&missile_moby->pos, &((Hit_2b6140 *)D_L00_00173E40_2b6140)->v20);
                fast_vec_sub(&v60,
                             &((Hit_2b6140 *)D_L00_00173E40_2b6140)->v50,
                             &((Hit_2b6140 *)D_L00_00173E40_2b6140)->v60);
                fast_vec_normalize(&v60, &v60, 1.0f);
                reflect_vector(&k30, &v50, &((Hit_2b6140 *)D_L00_00173E40_2b6140)->v40);
                fast_vec_normalize(&k30, &k30, frame_time_macro + frame_time_macro);
                fast_vec_normalize(&k40, &((Hit_2b6140 *)D_L00_00173E40_2b6140)->v40, 1.0f);
                if (missile_vars->f2C != -1) {
                    u8 *e = (u8 *)&voice_pool + missile_vars->f2C * 0x70;
                    if (*(struct Moby **)(e + 0x88) == missile_moby) {
                        if (e[0x74] != 0)
                            release_voice_slot(missile_vars->f2C);
                    }
                }
                missile_vars->f2C = -1;
            }
        } else if (missile_vars->f10 == 0) {
            func_L00_002B6E10_2b6140(missile_moby, missile_vars);
            return;
        }
    }
    if (missile_vars->f10 < scale_game_frames(0xBB8) - scale_game_frames(0x3C)) {
        if (controller_state.pressed & 0x20) {
            missile_moby->command = 1;
            if (missile_vars->f2C != -1) {
                u8 *e = (u8 *)&voice_pool + missile_vars->f2C * 0x70;
                if (*(struct Moby **)(e + 0x88) == missile_moby) {
                    if (e[0x74] != 0)
                        release_voice_slot(missile_vars->f2C);
                }
            }
            missile_vars->f2C = -1;
        }
    }
    if (missile_moby->command != 0) {
        int k;
        int rr;
        if (direct == 0) {
            spawn_explosion(missile_moby,
                            &k30,
                            0,
                            0.0f,
                            0.0f,
                            10,
                            3,
                            16,
                            4.0f,
                            2.0f,
                            0.0f,
                            1.0f,
                            1,
                            20.0f,
                            1,
                            1,
                            -1,
                            0);
            rr = collect_mobies_in_sphere_2b6140(&missile_moby->pos, 4.0f, 16, missile_moby, 0);
            *(u128_2b6140 *)&vA0 = *(u128_2b6140 *)((V_2b6140 *)&missile_moby->pos);
            send_blast_hits(
                missile_moby, &vA0, D_L00_00177F00_2b6140, rr, 0, 0x830000, 3, 2, 6.0f, 1.0f, 1.0f);
        }
        missile_moby->flags |= 1;
        missile_vars->f4 = frame_scale_macro * 0.0199999996f;
        missile_vars->f18 = missile_vars->f18 * 0.00999999978f;
        if (scale_game_frames(0xBB8) - scale_game_frames(0x1E) < missile_vars->f10) {
            missile_vars->f10 = -scale_game_frames(100);
        } else {
            missile_vars->f10 = -1;
        }
        qcopy(&vA0, &missile_moby->pos);
        for (k = 4; k >= 0; k--) {
            qcopy(&missile_moby->pos, &vA0);
            add_random_offset_to_vector_xyz(&missile_moby->pos, 1.5f);
            func_L00_002B6FF0_2b6140(missile_moby, 1);
            qcopy(&missile_moby->pos, &vA0);
        }
        v50.x = 0.0f;
        missile_moby->command = 0;
    }
    if (missile_vars->f14 != 0)
        update_orbit_camera_from_target_moby(missile_vars->f14, missile_moby);
}
