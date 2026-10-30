/* Moby class 0x79 (bomb_update): the bomb thrown by the bomb glove, with its landing prediction and marker, and the functions that spawn and launch it. */

#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/overlay/shared/props/grow_and_fade_out.h"
#include "rnc/overlay/shared/rendering/effects/explosion_light.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/overlay/collision.h"
#include "rnc/input/pad_globals.h"
#include "rnc/game/item_state.h"
#include "rnc/rendering/debug/debug_menu.h"
#include "rnc/overlay/shared/weapons/mine.h"
#include "rnc/overlay/shared/weapons/bomb_glove_bomb.h"

typedef struct {
    char           p0[0x80];
    float          pos[4];
    char           p90[0x2FC - 0x90];
    unsigned char *p2FC;
    char           p300[0x1090 - 0x300];
    M910          *hero;
    char           p1094[0x2080 - 0x1094];
    M910          *cam;
    char           p2084[0x20];
    unsigned char  b20A4;
    unsigned char  b20A5;
    char           p20A6[9];
    unsigned char  b20AF;
} G910;

typedef union {
    u128 q_002acbf0;
    f32  f[4];
} V_2acbf0_002acbf0;

typedef struct {
    V_2acbf0_002acbf0 corner[4];
    u32               color[4];
    f32               uv[8];
    s64               unk70;
    s64               tex;
    s64               unk80;
    s64               unk88;
} Q_2acbf0_002acbf0;

typedef struct {
    u8                pad0[0x10];
    V_2acbf0_002acbf0 pos;
    f32               f20, f24, f28;
} PV_2acbf0_002acbf0;

typedef struct {
    u8                  pad0[0x78];
    PV_2acbf0_002acbf0 *pv;
} M_2acbf0_002acbf0;

typedef unsigned int u128_2ace70 __attribute__((mode(TI), aligned(16)));

typedef union {
    u128_2ace70 q;
    float       f[4];
} V_2ace70;

typedef struct {
    U910  vel;
    U910  landing;
    U910  normal;
    U910  previous_hit;
    U910  difference;
    M910 *holder;
    short timer;
    short holder_flag;
    char  pad58[8];
    M910 *last_hit;
    int   hit_count;
} LandingVars;

/* Update of moby class 121, a thrown bomb: it flies and spins, bounces once on the ground, explodes on a hit or when its timer runs out, then its blast ring grows for 15 frames. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002AB910.c: func_L00_002AEDD0), where it is exact; names translated to the US level program. */
typedef union {
    u128  q;
    float f[4];
} Vec_2adb30 __attribute__((aligned(16)));

typedef struct {
    int c[6];
} Colors6;

typedef struct BombMoby BombMoby;

typedef struct {
    Vec_2adb30 v;
    char       pad10[0x40];
    M910      *owner;
    short      h54;
    short      h56;
    char       pad58[0x10];
    short      h68;
    short      h6a;
} BombData;

struct BombMoby {
    char           p0[0x10];
    Vec_2adb30     pos;
    unsigned char  state;
    char           p21[0x13];
    unsigned short flags;
    char           p36[0xA];
    float          rx;
    float          ry;
    char           p48[0x2C];
    int            f74;
    BombData      *d;
    char           p7C[0x14];
    int            color;
    char           p94[0x12];
    short          cls;
    char           pA8[0x14];
    unsigned char  bBC;
};

typedef struct {
    Vec_2adb30    dir;
    BombMoby     *moby;
    int           flags;
    unsigned char kind;
    unsigned char on;
    short         cls;
    float         fDC;
    int           iE0;
} BombHit;

extern float scale_time(float) __asm__("FUN_001f96b0");
extern float fast_vec_dot(void *a, void *b) __asm__("FUN_001f9ab0");
extern int   collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern int   collision_hit_material(void) __asm__("FUN_001f0b58");
extern int collect_mobies_in_sphere(void *, int, void *, void *, float) __asm__("FUN_L00_001f2868");
extern int mine_blend_color(int, int, float) __asm__("FUN_L00_002371e0");
extern int random_integer_below_cf(void *, void *, float, float) __asm__("FUN_L00_0026f080");
extern unsigned       swap_packed_color_channels(unsigned c, int mask) __asm__("FUN_L00_0025c0e8");
extern unsigned char *FUN_002141f8_cf(unsigned char *) __asm__("FUN_L00_002603d0");
extern void          *spawn_shrinking_effect_particle_with_gravity_and_secondary_spawns(
             void *, void *, int, int, float, int, int, int, int) __asm__("FUN_L00_0026bb70");
extern void reflect_vector(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern void
send_blast_hits(void *, void *, void *, int, int, int, int, int, float, float, float) __asm__("FUN_L00_0025a9f8");
extern void spawn_debris_chunk(void *, void *, int, int) __asm__("FUN_L00_0026f548");
extern void spawn_gravity_floor_burst_particle(
    void *, void *, int, float, float, float, int, int, int) __asm__("FUN_L00_002738e8");
extern void pool_splash(int, int, int, float, float, float, float) __asm__("FUN_L00_002a3ec8");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern void scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");
extern void fast_vec_sub(void *dst, void *a, void *b) __asm__("FUN_001f9a28");
void        update_entity(void *) __asm__("FUN_L00_00250df8");
int         FUN_001f96f8_002acbf0(int) __asm__("FUN_001f96f8");
extern s64  get_effect_texture(s32) __asm__("FUN_001f44b8");
extern void draw_geometry_quad(void *, void *, s32) __asm__("FUN_001f7d30");
/* Ported from rac1-decomp (src/overlays/shared/vendor_002AB910.c: func_L00_002AE278). */
extern f32   fast_difference_between_rotations(f32, f32) __asm__("FUN_001fa688");
extern void  aim_throw_arc(void *, void *, void *, f32, f32, f32) __asm__("FUN_L00_002c2488");
extern int   ray_hits(void *, void *, int, void *, int) __asm__("FUN_001efa68");
extern int   FUN_L00_002603d0_2ad410(M910 *) __asm__("FUN_L00_002603d0");
extern void  add_draw_callback(void *, void *) __asm__("FUN_001f4600");
extern int   FUN_001f96f8_2adb30(int) __asm__("FUN_001f96f8");
extern float FUN_001f96b0_2adb30(float) __asm__("FUN_001f96b0");
extern void  FUN_001f9a28_2adb30(void *, void *, void *) __asm__("FUN_001f9a28");
extern void  FUN_001f9a68_2adb30(void *, void *, float) __asm__("FUN_001f9a68");
extern float FUN_001f9ab0_2adb30(void *, void *) __asm__("FUN_001f9ab0");
extern float FUN_001f9af0_2adb30(void *) __asm__("FUN_001f9af0");
extern float FUN_001f9dc8_2adb30(float) __asm__("FUN_001f9dc8");
extern float FUN_001f9de0_2adb30(float) __asm__("FUN_001f9de0");
extern int   fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern float resolve_camera_surface_height(float *, void *) __asm__("FUN_002135f0");
extern char *
spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern void  FUN_001f9bf8_2adb30(void *, void *, float) __asm__("FUN_001f9bf8");
extern int   random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern void  random_vector(float *, float, float) __asm__("FUN_L00_00257d78");
extern float allocate_voice_for_target_entry_cf(void *) __asm__("FUN_L00_00259710");
extern float interpolate_scalar_linear(float, float, float) __asm__("FUN_L00_0025b6a8");
extern void  swap_color_channel_words(int *, int *, int *, int) __asm__("FUN_L00_0025c088");
extern unsigned char *FUN_L00_002603d0_2adb30(unsigned char *) __asm__("FUN_L00_002603d0");
extern float          normalize_angle(float) __asm__("FUN_L00_0025e310");
extern void           FUN_L00_0026a9f0_2adb30(
              void *, void *, int, int, float, int, int, int, int, float) __asm__("FUN_L00_0026a9f0");
extern int trooper_play_sound(int i, int voice_flags, int moby, int k) __asm__("FUN_L00_0028dc90");
extern int FUN_L00_002ad410_2adb30(void *, void *, int) __asm__("FUN_L00_002ad410");
extern char *FUN_L00_002af450_2adb30(char *, char *, int, int, int) __asm__("FUN_L00_002af450");
extern char *spawn_randomly_rotated_colored_moby(void *,
                                                 void *,
                                                 void *,
                                                 float,
                                                 int,
                                                 unsigned char,
                                                 unsigned char,
                                                 unsigned char,
                                                 int) __asm__("FUN_L00_002df808");

extern E              D_0013E520;
extern s32            D_L00_00166DC0[];
extern s32            D_L00_001612D0_gp __asm__("D_L00_001612D0") __attribute__((sda));
extern s32            D_L00_001612D8_gp __asm__("D_L00_001612D8") __attribute__((sda));
extern H910           coll_hit __asm__("D_L00_00173E40");
extern s32            D_L00_0015F5C4_002acbf0 __asm__("D_L00_0015F5C4");
extern s32            D_L00_0015F5CC_002acbf0 __asm__("D_L00_0015F5CC");
extern float          frame_time_macro __asm__("D_0015ED6C") MACRO_ADDR;
extern float          frame_time_sq_macro __asm__("D_0015ED70") MACRO_ADDR;
extern int            D_L00_0015F5CC_2adb30 __asm__("D_L00_0015F5CC");
extern int           *D_L00_00177F00_2adb30[] __asm__("D_L00_00177F00");
extern unsigned char *D_L00_001B2080_2adb30[] __asm__("D_L00_001B2080");
extern int            D_L00_00160770;
extern short   bomb_spark_vertical_acceleration __asm__("D_L00_001613F0") __attribute__((sda));
extern short   bomb_spark_velocity_scale __asm__("D_L00_001613F4") __attribute__((sda));
extern short   bomb_spark_random_velocity_radius __asm__("D_L00_001613F8") __attribute__((sda));
extern short   bomb_spark_random_frame_count_range __asm__("D_L00_001613FC") __attribute__((sda));
extern short   bomb_spark_vertical_offset_frame_base __asm__("D_L00_00161400") __attribute__((sda));
extern short   bomb_spark_jittered_frame_count_base __asm__("D_L00_00161404") __attribute__((sda));
extern short   bomb_spark_frame_count_jitter __asm__("D_L00_00161408") __attribute__((sda));
extern short   D_L00_0016140C __attribute__((sda));
extern short   D_L00_00161410 __attribute__((sda));
extern short   D_L00_00161414 __attribute__((sda));
extern short   D_L00_00161418 __attribute__((sda));
extern short   bomb_spark_primary_color_start __asm__("D_L00_0016141C") __attribute__((sda));
extern short   bomb_spark_primary_color_end __asm__("D_L00_00161420") __attribute__((sda));
extern short   bomb_spark_secondary_color_start __asm__("D_L00_00161424") __attribute__((sda));
extern short   bomb_spark_secondary_color_end __asm__("D_L00_00161428") __attribute__((sda));
extern short   bomb_sparks_per_emission __asm__("D_L00_0016142C") __attribute__((sda));
extern short   bomb_spark_texture_selector __asm__("D_L00_00161430") __attribute__((sda));
extern B910    l00_bomb_camera_blast_parameters __asm__("D_L00_001E98D0");
extern Colors6 l00_bomb_blast_effect_primary_palette __asm__("D_L00_001E9920");
extern Colors6 l00_bomb_blast_effect_secondary_palette __asm__("D_L00_001E9938");
extern s32     D_L00_00166DC0[] NOT_SDA;

short D_L01_001613F0 = 0;
short D_L01_001613F4 = 0;
short D_L01_001613F8 = 0;
short D_L01_001613FC = 1;
short D_L01_00161400 = 0x1e;
short D_L01_00161404 = 0xa;
short D_L01_00161408 = 0;
short D_L01_0016140C = 0;
short D_L01_00161410 = -26214;
short D_L01_00161414 = -26214;
short D_L01_00161418 = -13107;
short D_L01_0016141C = -32513;
short D_L01_00161420 = -1;
short D_L01_00161424 = 0x4040;
short D_L01_00161428 = 0x2080;
short D_L01_0016142C = 1;
short D_L01_00161430 = 0x19;

Colors6 D_L01_0020A980 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors6 D_L01_0020A998 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

int   D_L02_00160770 = -1;
short D_L02_001613C0 = 0;
short D_L02_001613C4 = 0;
short D_L02_001613C8 = 0;
short D_L02_001613CC = 1;
short D_L02_001613D0 = 0x1e;
short D_L02_001613D4 = 0xa;
short D_L02_001613D8 = 0;
short D_L02_001613DC = 0;
short D_L02_001613E0 = -26214;
short D_L02_001613E4 = -26214;
short D_L02_001613E8 = -13107;
short D_L02_001613EC = -32513;
short D_L02_001613F0 = -1;
short D_L02_001613F4 = 0x4040;
short D_L02_001613F8 = 0x2080;
short D_L02_001613FC = 1;
short D_L02_00161400 = 0x19;

Colors6 D_L02_001FA220 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors6 D_L02_001FA238 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

short D_L03_001613D0 = 0;
short D_L03_001613D4 = 0;
short D_L03_001613D8 = 0;
short D_L03_001613DC = 1;
short D_L03_001613E0 = 0x1e;
short D_L03_001613E4 = 0xa;
short D_L03_001613E8 = 0;
short D_L03_001613EC = 0;
short D_L03_001613F0 = -26214;
short D_L03_001613F4 = -26214;
short D_L03_001613F8 = -13107;
short D_L03_001613FC = -32513;
short D_L03_00161400 = -1;
short D_L03_00161404 = 0x4040;
short D_L03_00161408 = 0x2080;
short D_L03_0016140C = 1;
short D_L03_00161410 = 0x19;

Colors6 D_L03_001E2AB0 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors6 D_L03_001E2AC8 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

int   D_L04_00160770 = -1;
short D_L04_001613C4 = 0;
short D_L04_001613C8 = 0;
short D_L04_001613CC = 0;
short D_L04_001613D0 = 1;
short D_L04_001613D4 = 0x1e;
short D_L04_001613D8 = 0xa;
short D_L04_001613DC = 0;
short D_L04_001613E0 = 0;
short D_L04_001613E4 = -26214;
short D_L04_001613E8 = -26214;
short D_L04_001613EC = -13107;
short D_L04_001613F0 = -32513;
short D_L04_001613F4 = -1;
short D_L04_001613F8 = 0x4040;
short D_L04_001613FC = 0x2080;
short D_L04_00161400 = 1;
short D_L04_00161404 = 0x19;

Colors6 D_L04_001DB6C0 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors6 D_L04_001DB6D8 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

int   D_L05_00160770 = -1;
short D_L05_00161410 = 0;
short D_L05_00161414 = 0;
short D_L05_00161418 = 0;
short D_L05_0016141C = 1;
short D_L05_00161420 = 0x1e;
short D_L05_00161424 = 0xa;
short D_L05_00161428 = 0;
short D_L05_0016142C = 0;
short D_L05_00161430 = -26214;
short D_L05_00161434 = -26214;
short D_L05_00161438 = -13107;
short D_L05_0016143C = -32513;
short D_L05_00161440 = -1;
short D_L05_00161444 = 0x4040;
short D_L05_00161448 = 0x2080;
short D_L05_0016144C = 1;
short D_L05_00161450 = 0x19;

Colors6 D_L05_00218030 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors6 D_L05_00218048 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

int   D_L06_00160770 = -1;
short D_L06_001613C0 = 0;
short D_L06_001613C4 = 0;
short D_L06_001613C8 = 0;
short D_L06_001613CC = 1;
short D_L06_001613D0 = 0x1e;
short D_L06_001613D4 = 0xa;
short D_L06_001613D8 = 0;
short D_L06_001613DC = 0;
short D_L06_001613E0 = -26214;
short D_L06_001613E4 = -26214;
short D_L06_001613E8 = -13107;
short D_L06_001613EC = -32513;
short D_L06_001613F0 = -1;
short D_L06_001613F4 = 0x4040;
short D_L06_001613F8 = 0x2080;
short D_L06_001613FC = 1;
short D_L06_00161400 = 0x19;

Colors6 D_L06_00200D20 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors6 D_L06_00200D38 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

int   D_L07_00160770 = -1;
short D_L07_001613C0 = 0;
short D_L07_001613C4 = 0;
short D_L07_001613C8 = 0;
short D_L07_001613CC = 1;
short D_L07_001613D0 = 0x1e;
short D_L07_001613D4 = 0xa;
short D_L07_001613D8 = 0;
short D_L07_001613DC = 0;
short D_L07_001613E0 = -26214;
short D_L07_001613E4 = -26214;
short D_L07_001613E8 = -13107;
short D_L07_001613EC = -32513;
short D_L07_001613F0 = -1;
short D_L07_001613F4 = 0x4040;
short D_L07_001613F8 = 0x2080;
short D_L07_001613FC = 1;
short D_L07_00161400 = 0x19;

Colors6 D_L07_00210BC0 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors6 D_L07_00210BD8 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

int   D_L08_00160770 = -1;
short D_L08_001613C0 = 0;
short D_L08_001613C4 = 0;
short D_L08_001613C8 = 0;
short D_L08_001613CC = 1;
short D_L08_001613D0 = 0x1e;
short D_L08_001613D4 = 0xa;
short D_L08_001613D8 = 0;
short D_L08_001613DC = 0;
short D_L08_001613E0 = -26214;
short D_L08_001613E4 = -26214;
short D_L08_001613E8 = -13107;
short D_L08_001613EC = -32513;
short D_L08_001613F0 = -1;
short D_L08_001613F4 = 0x4040;
short D_L08_001613F8 = 0x2080;
short D_L08_001613FC = 1;
short D_L08_00161400 = 0x19;

Colors6 D_L08_001FB860 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors6 D_L08_001FB878 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

int   D_L09_00160770 = -1;
short D_L09_00161450 = 0;
short D_L09_00161454 = 0;
short D_L09_00161458 = 0;
short D_L09_0016145C = 1;
short D_L09_00161460 = 0x1e;
short D_L09_00161464 = 0xa;
short D_L09_00161468 = 0;
short D_L09_0016146C = 0;
short D_L09_00161470 = -26214;
short D_L09_00161474 = -26214;
short D_L09_00161478 = -13107;
short D_L09_0016147C = -32513;
short D_L09_00161480 = -1;
short D_L09_00161484 = 0x4040;
short D_L09_00161488 = 0x2080;
short D_L09_0016148C = 1;
short D_L09_00161490 = 0x19;

Colors6 D_L09_00208510 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors6 D_L09_00208528 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

int   D_L10_00160770 = -1;
short D_L10_001613D0 = 0;
short D_L10_001613D4 = 0;
short D_L10_001613D8 = 0;
short D_L10_001613DC = 1;
short D_L10_001613E0 = 0x1e;
short D_L10_001613E4 = 0xa;
short D_L10_001613E8 = 0;
short D_L10_001613EC = 0;
short D_L10_001613F0 = -26214;
short D_L10_001613F4 = -26214;
short D_L10_001613F8 = -13107;
short D_L10_001613FC = -32513;
short D_L10_00161400 = -1;
short D_L10_00161404 = 0x4040;
short D_L10_00161408 = 0x2080;
short D_L10_0016140C = 1;
short D_L10_00161410 = 0x19;

Colors6 D_L10_001DC710 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors6 D_L10_001DC728 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

int   D_L11_00160770 = -1;
short D_L11_00161450 = 0;
short D_L11_00161454 = 0;
short D_L11_00161458 = 0;
short D_L11_0016145C = 1;
short D_L11_00161460 = 0x1e;
short D_L11_00161464 = 0xa;
short D_L11_00161468 = 0;
short D_L11_0016146C = 0;
short D_L11_00161470 = -26214;
short D_L11_00161474 = -26214;
short D_L11_00161478 = -13107;
short D_L11_0016147C = -32513;
short D_L11_00161480 = -1;
short D_L11_00161484 = 0x4040;
short D_L11_00161488 = 0x2080;
short D_L11_0016148C = 1;
short D_L11_00161490 = 0x19;

Colors6 D_L11_0021ABA0 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors6 D_L11_0021ABB8 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

int   D_L12_00160770 = -1;
short D_L12_001613D0 = 0;
short D_L12_001613D4 = 0;
short D_L12_001613D8 = 0;
short D_L12_001613DC = 1;
short D_L12_001613E0 = 0x1e;
short D_L12_001613E4 = 0xa;
short D_L12_001613E8 = 0;
short D_L12_001613EC = 0;
short D_L12_001613F0 = -26214;
short D_L12_001613F4 = -26214;
short D_L12_001613F8 = -13107;
short D_L12_001613FC = -32513;
short D_L12_00161400 = -1;
short D_L12_00161404 = 0x4040;
short D_L12_00161408 = 0x2080;
short D_L12_0016140C = 1;
short D_L12_00161410 = 0x19;

Colors6 D_L12_0020BE70 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors6 D_L12_0020BE88 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

int   D_L13_00160770 = -1;
short D_L13_001614A0 = 0;
short D_L13_001614A4 = 0;
short D_L13_001614A8 = 0;
short D_L13_001614AC = 1;
short D_L13_001614B0 = 0x1e;
short D_L13_001614B4 = 0xa;
short D_L13_001614B8 = 0;
short D_L13_001614BC = 0;
short D_L13_001614C0 = -26214;
short D_L13_001614C4 = -26214;
short D_L13_001614C8 = -13107;
short D_L13_001614CC = -32513;
short D_L13_001614D0 = -1;
short D_L13_001614D4 = 0x4040;
short D_L13_001614D8 = 0x2080;
short D_L13_001614DC = 1;
short D_L13_001614E0 = 0x19;

Colors6 D_L13_001F47D0 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors6 D_L13_001F47E8 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

int   D_L14_00160770 = -1;
short D_L14_001615DC = 0;
short D_L14_001615E0 = 0;
short D_L14_001615E4 = 0;
short D_L14_001615E8 = 1;
short D_L14_001615EC = 0x1e;
short D_L14_001615F0 = 0xa;
short D_L14_001615F4 = 0;
short D_L14_001615F8 = 0;
short D_L14_001615FC = -26214;
short D_L14_00161600 = -26214;
short D_L14_00161604 = -13107;
short D_L14_00161608 = -32513;
short D_L14_0016160C = -1;
short D_L14_00161610 = 0x4040;
short D_L14_00161614 = 0x2080;
short D_L14_00161618 = 1;
short D_L14_0016161C = 0x19;

Colors6 D_L14_001FC150 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors6 D_L14_001FC168 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

int   D_L15_00160770 = -1;
short D_L15_00161504 = 0;
short D_L15_00161508 = 0;
short D_L15_0016150C = 0;
short D_L15_00161510 = 1;
short D_L15_00161514 = 0x1e;
short D_L15_00161518 = 0xa;
short D_L15_0016151C = 0;
short D_L15_00161520 = 0;
short D_L15_00161524 = -26214;
short D_L15_00161528 = -26214;
short D_L15_0016152C = -13107;
short D_L15_00161530 = -32513;
short D_L15_00161534 = -1;
short D_L15_00161538 = 0x4040;
short D_L15_0016153C = 0x2080;
short D_L15_00161540 = 1;
short D_L15_00161544 = 0x19;

Colors6 D_L15_001E27C0 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors6 D_L15_001E27D8 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

int   D_L16_00160770 = -1;
short D_L16_00161480 = 0;
short D_L16_00161484 = 0;
short D_L16_00161488 = 0;
short D_L16_0016148C = 1;
short D_L16_00161490 = 0x1e;
short D_L16_00161494 = 0xa;
short D_L16_00161498 = 0;
short D_L16_0016149C = 0;
short D_L16_001614A0 = -26214;
short D_L16_001614A4 = -26214;
short D_L16_001614A8 = -13107;
short D_L16_001614AC = -32513;
short D_L16_001614B0 = -1;
short D_L16_001614B4 = 0x4040;
short D_L16_001614B8 = 0x2080;
short D_L16_001614BC = 1;
short D_L16_001614C0 = 0x19;

Colors6 D_L16_001E7C70 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors6 D_L16_001E7C88 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

int   D_L17_00160770 = -1;
short D_L17_0016149C = 0;
short D_L17_001614A0 = 0;
short D_L17_001614A4 = 0;
short D_L17_001614A8 = 1;
short D_L17_001614AC = 0x1e;
short D_L17_001614B0 = 0xa;
short D_L17_001614B4 = 0;
short D_L17_001614B8 = 0;
short D_L17_001614BC = -26214;
short D_L17_001614C0 = -26214;
short D_L17_001614C4 = -13107;
short D_L17_001614C8 = -32513;
short D_L17_001614CC = -1;
short D_L17_001614D0 = 0x4040;
short D_L17_001614D4 = 0x2080;
short D_L17_001614D8 = 1;
short D_L17_001614DC = 0x19;

Colors6 D_L17_001E6390 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors6 D_L17_001E63A8 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

int   D_L18_00160770 = -1;
short D_L18_00161460 = 0;
short D_L18_00161464 = 0;
short D_L18_00161468 = 0;
short D_L18_0016146C = 1;
short D_L18_00161470 = 0x1e;
short D_L18_00161474 = 0xa;
short D_L18_00161478 = 0;
short D_L18_0016147C = 0;
short D_L18_00161480 = -26214;
short D_L18_00161484 = -26214;
short D_L18_00161488 = -13107;
short D_L18_0016148C = -32513;
short D_L18_00161490 = -1;
short D_L18_00161494 = 0x4040;
short D_L18_00161498 = 0x2080;
short D_L18_0016149C = 1;
short D_L18_001614A0 = 0x19;

Colors6 D_L18_001F2050 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors6 D_L18_001F2068 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

void draw_bomb_landing_marker(M_2acbf0_002acbf0 *moby) __asm__("FUN_L00_002acbf0");

void draw_bomb_landing_marker(M_2acbf0_002acbf0 *moby) {
    Q_2acbf0_002acbf0   quad;
    V_2acbf0_002acbf0   quad_transform[4];
    V_2acbf0_002acbf0   mz[4];
    V_2acbf0_002acbf0   quad_alignment_angles;
    V_2acbf0_002acbf0   rz;
    PV_2acbf0_002acbf0 *pv = moby->pv;
    s32                 i;
    s32                 odd;
    f32                 polar_angle, rotation_angle_numerator;
    if (D_L00_0015F5C4_002acbf0 == 2) {
        return;
    }
    for (i = 0; i < 2; i++) {
        odd = i & 1;
        polar_angle = fast_arc_tan(square_root(pv->f20 * pv->f20 + pv->f28 * pv->f28), pv->f24);
        quad_alignment_angles.f[0] = fast_add_rotations(-polar_angle, 1.57079637f);
        quad_alignment_angles.f[1] = fast_arc_tan(pv->f28, pv->f20);
        quad_alignment_angles.f[2] = 0.0f;
        quad_alignment_angles.f[3] = 0.0f;
        rz.f[0] = 0.0f;
        rotation_angle_numerator =
            convert_integer_to_float(D_L00_0015F5CC_002acbf0 % FUN_001f96f8_002acbf0(180));
        {
            f32 scaled_rotation_period = scale_time(180.0f);
            f32 rotation_angle;
            rotation_angle_numerator *= 6.28318548f;
            rotation_angle =
                fast_normalize_angle(rotation_angle_numerator / scaled_rotation_period);
            if (odd) {
                rotation_angle = -rotation_angle;
            }
            rz.f[1] = rotation_angle;
        }
        rz.f[2] = 0.0f;
        rz.f[3] = 0.0f;
        euler_rotation_matrix(mz, &rz);
        euler_rotation_matrix(quad_transform, &quad_alignment_angles);
        multiply_matrix(quad_transform, quad_transform, mz);
        qcopy(&quad_transform[3], &pv->pos);
        quad.color[0] = 0x80808080;
        quad.color[1] = 0x80808080;
        quad.color[2] = 0x80808080;
        quad.color[3] = 0x80808080;
        quad.tex = get_effect_texture(odd ? 0x12 : 0x11);
        quad.unk80 = 0xFF9000000260L;
        quad.unk88 = 0x8000000048L;
        quad.unk70 = 0;
        quad.corner[0].f[0] = -1.0f;
        quad.corner[0].f[1] = 0.0f;
        quad.corner[0].f[2] = 1.0f;
        quad.corner[0].f[3] = 1.0f;
        quad.corner[1].f[0] = 1.0f;
        quad.corner[1].f[1] = 0.0f;
        quad.corner[1].f[2] = 1.0f;
        quad.corner[1].f[3] = 1.0f;
        quad.corner[2].f[0] = -1.0f;
        quad.corner[2].f[1] = 0.0f;
        quad.corner[2].f[2] = -1.0f;
        quad.corner[2].f[3] = 1.0f;
        quad.corner[3].f[0] = 1.0f;
        quad.corner[3].f[1] = 0.0f;
        quad.corner[3].f[2] = -1.0f;
        quad.corner[3].f[3] = 1.0f;
        quad.uv[0] = 0.0f;
        quad.uv[1] = 1.0f;
        quad.uv[2] = 1.0f;
        quad.uv[3] = 1.0f;
        quad.uv[4] = 0.0f;
        quad.uv[5] = 0.0f;
        quad.uv[6] = 1.0f;
        quad.uv[7] = 0.0f;
        draw_geometry_quad(&quad, quad_transform, 1);
    }
}

unsigned char *spawn_bomb(int owner, void *pos) __asm__("FUN_L00_002ace70");

unsigned char *spawn_bomb(int owner, void *spawn_position) {
    unsigned char *bomb_moby = create_moby(0x79);
    if (bomb_moby) {
        V_2ace70       u, random_rotation;
        unsigned char *v = ((unsigned char *)MOBY(bomb_moby)->pvars);
        struct Hero   *hero_context;
        float          frame_scaled_random_angle;
        random_rotation.q = 0;
        random_rotation.f[0] = random_float_between(-3.1415927f, 3.1415927f);
        random_rotation.f[1] = random_float_between(-3.1415927f, 3.1415927f);
        random_rotation.f[2] = random_float_between(-3.1415927f, 3.1415927f);
        u.q = random_rotation.q;
        bomb_moby[0x30] = 0xFF;
        MOBY(bomb_moby)->draw_dist = 0xFF;
        bomb_moby[0x31] = 1;
        bomb_moby[0x20] = 0;
        *(int *)(v + 0x50) = owner;
        *(short *)(v + 0x56) = 0;
        *(short *)(v + 0x6A) = 0;
        *(int *)(v + 0x60) = 0;
        hero_context = &hero;
        *(float *)(v + 0x6C) = fast_arc_tan(hero_context->facing_x, hero_context->facing_y);
        *(int *)(v + 0x64) = 0;
        qcopy(bomb_moby + 0x10, spawn_position);
        clear_u64_value(v);
        qcopy(bomb_moby + 0x40, &u);
        frame_scaled_random_angle =
            random_float_between(frame_time * 1.0471976f, frame_time * 3.1415927f);
        {
            float frame_step = frame_time;
            *(float *)(v + 0x58) = frame_scaled_random_angle;
            *(float *)(v + 0x5C) =
                random_float_between(frame_step * 1.0471976f, frame_step * 3.1415927f);
        }
        update_entity(bomb_moby);
    }
    return bomb_moby;
}

void launch_bomb(float *pos, unsigned char *moby, float *velocity) __asm__("FUN_L00_002acfd8");

void launch_bomb(float *pos, unsigned char *moby, float *velocity) {
    float         *vars = ((float *)MOBY(moby)->pvars);
    float          difference[4] __attribute__((aligned(16)));
    float          direction[4] __attribute__((aligned(16)));
    float          result[4] __attribute__((aligned(16)));
    float          working[4] __attribute__((aligned(16)));
    float          desired[4] __attribute__((aligned(16)));
    unsigned char *mp;
    float         *rp;
    float          dot, length, distance, angle, height, total;

    *(short *)((char *)vars + 0x6a) = scale_game_frames(30);
    moby[0x20] = 1;
    qcopy(vars, velocity);
    if (hero.state.control_mode == 15) {
        fast_vec_add(vars, vars, &hero.motion.displacement);
    } else if (hero.ground_moby != 0 && FUN_002141f8_cf((unsigned char *)hero.ground_moby) != 0) {
        fast_vec_add(vars, vars, &hero.motion.displacement);
    } else if (*(float **)((char *)vars + 0x60) != 0) {
        short cls = *(short *)(*(char **)((char *)vars + 0x60) + 0xa6);
        if (cls == 0x56e || cls == 0x58e) {
            fast_vec_add(vars, vars, vars + 0x10);
        } else if (*(int *)((char *)vars + 0x64) != 0) {
            length = vector_length_xy(velocity);
            fast_vec_sub(difference, &hero.motion.pos, *(char **)((char *)vars + 0x60) + 0x10);
            fast_vec_normalize(difference, difference, 1.0f);
            dot = fast_vec_dot(difference, vars + 0x10);
            total = dot + length;
            distance = vector_distance_xy(pos, *(char **)((char *)vars + 0x60) + 0x10) / total;
            if (dot > 0.0f) {
                scale_vector_xyz(direction, difference, distance * dot);
                fast_vec_add(working, direction, *(char **)((char *)vars + 0x60) + 0x10);
                *(OvlQuad *)direction = *(OvlQuad *)working;
            } else {
                scale_vector_xyz(direction, vars + 0x10, distance);
                fast_vec_add(working, direction, *(char **)((char *)vars + 0x60) + 0x10);
                *(OvlQuad *)direction = *(OvlQuad *)working;
            }
            {
                float heading = fast_arc_tan(hero.facing_x, hero.facing_y);
                float toward = fast_arc_tan(direction[0] - hero.motion.pos.f[0],
                                            direction[1] - hero.motion.pos.f[1]);
                angle = fast_difference_between_rotations(heading, toward);
            }
            if (angle > 1.5707964f) {
                working[0] = fast_cos(fast_arc_tan(hero.facing_x, hero.facing_y)) *
                             vector_distance_xy(&hero.motion.pos, direction) * 0.5f;
                angle = fast_sin(fast_arc_tan(hero.facing_x, hero.facing_y));
                working[1] = angle * vector_distance_xy(&hero.motion.pos, direction) * 0.5f;
                working[2] = 0.0f;
                fast_vec_add(desired, working, &hero.motion.pos);
                *(OvlQuad *)direction = *(OvlQuad *)desired;
            }
            total = frame_time_sq * 11.0f;
            rp = result;
            if (pad_held[0] & 5) {
                height = convert_integer_to_float(D_0013E520.bA) * 2.5f + 8.5f;
            } else {
                height = 8.5f;
            }
            aim_throw_arc(pos, direction, rp, total, 0.00001f, height);
            scale_vector_xyz(result, result, 0.65f);
            scale_vector_xyz(vars, vars, 0.35f);
            fast_vec_add(working, vars, result);
            *(OvlQuad *)vars = *(OvlQuad *)working;
        }
    }
    mp = moby + 0x10;
    *(short *)((char *)vars + 0x54) = scale_game_frames(300);
    qcopy(mp, pos);
    *(OvlQuad *)difference = *(OvlQuad *)((char *)hero.moby + 0x10);
    difference[2] = pos[2];
    if (collision_line(difference, pos, 0, (int)hero.moby, 0) != 0) {
        moby[0xbc] = 1;
        qcopy(mp, &coll_hit.pos);
    }
    if (D_0013E520.bA != 0 && hero.state.current == HERO_STATE_LOOK) {
        float z = vars[2];
        scale_vector_xyz(vars, vars, 1.5f);
        vars[2] = z / 1.5f;
    }
}

int predict_throw_landing(LandingVars *v, M910 *thrown_moby, int mode) __asm__("FUN_L00_002ad410");

int predict_throw_landing(LandingVars *v, M910 *thrown_moby, int mode) {
    U910 position, velocity, previous, hero_position, offset, start;
    int  timer;
    int  object_hit;
    int  found = 0, i;
    object_hit = 0;

    if (hero.state.current == 0x72)
        return 0;
    if (D_L00_0015F5C4_002acbf0 != 0)
        return 0;
    qcopy(&previous, &thrown_moby->pos);
    if (thrown_moby->state == 0) {
        position.f[0] =
            fast_cos(fast_add_rotations(-0.361968f, fast_arc_tan(hero.facing_x, hero.facing_y))) *
            0.859039f;
        position.f[1] =
            fast_sin(fast_add_rotations(-0.361968f, fast_arc_tan(hero.facing_x, hero.facing_y))) *
            0.859039f;
        position.f[2] = 0.0f;
        fast_vec_add(&position, &position, &hero.motion.pos);
        position.f[2] += 0.49082f;
        qcopy(&velocity, &v->vel);
        if (hero.state.control_mode == 15)
            fast_vec_add(&velocity, &velocity, &hero.motion.displacement);
        if (hero.ground_moby && FUN_L00_002603d0_2ad410((M910 *)hero.ground_moby) && mode)
            fast_vec_add(&velocity, &velocity, &hero.motion.displacement);
        timer = scale_game_frames(300);
    } else {
        W910 *holder_vars = v->holder->v;
        int   holder_flag;
        qcopy(&position, &thrown_moby->pos);
        qcopy(&velocity, &v->vel);
        holder_flag = *(int *)((char *)holder_vars + 0x50);
        timer = v->timer;
        if (holder_flag)
            v->holder_flag = 1;
    }
    {
        float z = position.f[2];
        hero_position.q = ((M910 *)hero.moby)->pos.q;
        hero_position.f[2] = z;
    }
    if (ray_hits(&hero_position, &position, 0x10, hero.moby, 0) &&
        (collision_hit_material() || 0.0f < velocity.f[2])) {
        if (coll_hit.moby && coll_hit.i1C <= 0) {
            if ((coll_hit.moby != hero.moby && coll_hit.moby != v->holder) ||
                timer < scale_game_frames(300) - scale_game_frames(10)) {
                offset.q = 0;
                offset.f[2] = 0.5f;
                {
                    H910 *h = &coll_hit;
                    if (v->last_hit != h->moby) {
                        v->hit_count = 0;
                        v->last_hit = h->moby;
                        v->previous_hit.q = h->moby->pos.q;
                    } else {
                        v->hit_count++;
                        fast_vec_sub(&start, (char *)h->moby + 0x10, &v->previous_hit);
                        v->difference.q = start.q;
                        v->previous_hit.q = h->moby->pos.q;
                    }
                }
                object_hit = 1;
                qcopy(&v->landing, (char *)coll_hit.moby + 0x10);
                found = 1;
                fast_vec_add(&offset, &offset, &v->landing);
                probe_ground_height(&offset, 0, 0.5f);
            }
        } else if (coll_hit.i1C > 0) {
            qcopy(&v->landing, &coll_hit.pos);
            found = 1;
        }
    }
    if (item_text_variant[0xa] && hero.state.current == 1) {
        float z = velocity.f[2];
        fast_vec_scale(&velocity, &velocity, 1.5f);
        velocity.f[2] = z / 1.5f;
    }
    hero_position.q = position.q;
    while (!fast_dec_timer(&timer) && !found) {
        qcopy(&previous, &position);
        for (i = 9; i >= 0; i--) {
            fast_vec_add(&position, &position, &velocity);
            velocity.f[2] -= frame_time_sq * 11.0f;
        }
        if (!ray_hits(&previous, &position, 0x10, thrown_moby, 0))
            continue;
        if (!collision_hit_material() && 0.0f < velocity.f[2])
            continue;
        if (coll_hit.moby && coll_hit.i1C <= 0) {
            if (!(position.f[2] - hero_position.f[2] < 2.0f))
                goto check_surface;
            if ((coll_hit.moby == hero.moby || coll_hit.moby == v->holder) &&
                timer >= scale_game_frames(300) - scale_game_frames(10))
                continue;
            offset.q = 0;
            offset.f[2] = 0.5f;
            {
                H910 *h = &coll_hit;
                if (v->last_hit != h->moby) {
                    v->hit_count = 0;
                    v->last_hit = h->moby;
                    v->previous_hit.q = h->moby->pos.q;
                } else {
                    v->hit_count++;
                    fast_vec_sub(&start, (char *)h->moby + 0x10, &v->previous_hit);
                    v->difference.q = start.q;
                    v->previous_hit.q = h->moby->pos.q;
                }
            }
            object_hit = 1;
            qcopy(&v->landing, (char *)coll_hit.moby + 0x10);
            fast_vec_add(&offset, &offset, &v->landing);
            probe_ground_height(&offset, 0, 0.5f);
            break;
        }
    check_surface:
        if (coll_hit.i1C > 0) {
            qcopy(&v->landing, &coll_hit.pos);
            break;
        }
    }
    if (!timer)
        goto return_zero;
    if (!mode) {
        if (!coll_hit.moby)
            goto return_zero;
        if (!FUN_L00_002603d0_2ad410(coll_hit.moby))
            goto return_zero;
    }
    if (mode != 2 || !coll_hit.moby || !FUN_L00_002603d0_2ad410(coll_hit.moby))
        add_draw_callback(draw_bomb_landing_marker, thrown_moby);
    if (!object_hit && v->last_hit) {
        if (v->last_hit->state == 0xfe) {
            v->hit_count = 0;
        } else if (v->last_hit->state == 0xfd) {
            v->hit_count = 0;
        } else if (!(v->last_hit->flags & 0x1000)) {
            v->hit_count = 0;
        } else {
            if (v->hit_count <= scale_game_frames(30))
                goto keep_hit;
            v->hit_count = 0;
        }
        v->last_hit = 0;
    }
keep_hit:
    v->normal.q = coll_hit_normal.q;
    fast_vec_sub(&previous, &v->landing, D_L00_00166DC0);
    fast_vec_scale(&previous, &previous, 0.95f);
    fast_vec_add(&v->landing, &previous, D_L00_00166DC0);
    return 1;
return_zero:
    return 0;
}

void bomb_update(BombMoby *m) __asm__("FUN_L00_002adb30");

void bomb_update(BombMoby *bomb_moby) {
    B910       rec = l00_bomb_camera_blast_parameters;
    Vec_2adb30 delta;
    Vec_2adb30 old;
    Vec_2adb30 blast_velocity;
    Vec_2adb30 b;
    Vec_2adb30 proj;
    BombData  *d;
    b.q = 0;
    b.f[2] = frame_time_macro * 2.0f;
    blast_velocity.q = b.q;
    b.q = 0;
    b.f[2] = 1.0f;
    d = bomb_moby->d;
    if ((((G910 *)&hero)->hero == 0 || ((G910 *)&hero)->hero->cls != 0xC0) &&
        bomb_moby->state == 0) {
        mark_moby_for_removal(bomb_moby);
        return;
    }
    if (((G910 *)&hero)->b20A4 != 0 && bomb_moby->state == 0) {
        mark_moby_for_removal(bomb_moby);
        return;
    }
    if ((((G910 *)&hero)->b20A5 != 0 || ((G910 *)&hero)->b20AF != 0) && bomb_moby->state == 0) {
        bomb_moby->flags |= 0x41;
    } else {
        bomb_moby->flags &= ~0x41;
    }
    if (d->owner->cls == 0xC0 && d->h56 == 0 && FUN_001f9af0_2adb30(d) > 0.0f) {
        if (((G910 *)&hero)->p2FC == 0 || FUN_L00_002603d0_2adb30(((G910 *)&hero)->p2FC) == 0) {
            FUN_L00_002ad410_2adb30(d, bomb_moby, 1);
        } else if (FUN_L00_002ad410_2adb30(d, bomb_moby, 0) == 0) {
            FUN_L00_002ad410_2adb30(d, bomb_moby, 2);
        }
    }
    if (bomb_moby->state == 1) {
        Vec_2adb30 c;
        float      t;
        int        cr;
        int        cg;
        int        cb;
        c.q = 0;
        c.f[0] = 128.0f;
        c.f[1] = 128.0f;
        c.f[2] = 128.0f;
        c.f[3] = 255.0f;
        fast_dec_timer_s16(&d->h6a);
        t = convert_integer_to_float(d->h6a) / convert_integer_to_float(FUN_001f96f8_2adb30(0x1E));
        cr = truncate_float_to_s32((c.f[0] - 255.0f) * t + 255.0f);
        cg = truncate_float_to_s32((c.f[1] - 64.0f) * t + 64.0f);
        cb = truncate_float_to_s32((c.f[2] - 64.0f) * t + 64.0f);
        bomb_moby->color = (cb << 16) | (0xFF << 24) | (cg << 8) | cr;
        if (d->h68 == 0) {
            if ((D_L00_0015F5CC_2adb30 & 7) == 0) {
                int i;
                for (i = 0; i < *(int *)&bomb_sparks_per_emission; i++) {
                    Vec_2adb30 spark_velocity;
                    Vec_2adb30 v2;
                    Vec_2adb30 random_velocity_offset;
                    int        p0;
                    int        spark_secondary_color;
                    int        spark_random_frame_count;
                    int        spark_vertical_offset_scale;
                    int        spark_jittered_frame_count;
                    FUN_001f9a68_2adb30(&spark_velocity, d, *(float *)&bomb_spark_velocity_scale);
                    random_vector(random_velocity_offset.f,
                                  0.0f,
                                  *(float *)&bomb_spark_random_velocity_radius * frame_time_macro);
                    fast_vec_add(&spark_velocity, &random_velocity_offset, &spark_velocity);
                    clear_u64_value(v2.f);
                    spark_velocity.f[3] =
                        random_float_between(*(float *)&D_L00_0016140C, *(float *)&D_L00_00161410);
                    v2.f[3] =
                        random_float_between(*(float *)&D_L00_00161414, *(float *)&D_L00_00161418);
                    p0 = fast_tween_color(*(int *)&bomb_spark_primary_color_start,
                                          *(int *)&bomb_spark_primary_color_end,
                                          random_float_between(0.0f, 1.0f));
                    spark_secondary_color =
                        fast_tween_color(*(int *)&bomb_spark_secondary_color_start,
                                         *(int *)&bomb_spark_secondary_color_end,
                                         random_float_between(0.0f, 1.0f));
                    spark_random_frame_count = truncate_float_to_s32(
                        FUN_001f96b0_2adb30((float)*(int *)&bomb_spark_random_frame_count_range *
                                                random_float_between(0.0f, 1.0f) +
                                            1.0f));
                    spark_vertical_offset_scale = truncate_float_to_s32(FUN_001f96b0_2adb30(
                        (float)*(int *)&bomb_spark_vertical_offset_frame_base *
                        (random_float_between(-*(float *)&bomb_spark_frame_count_jitter,
                                              *(float *)&bomb_spark_frame_count_jitter) +
                         1.0f)));
                    spark_jittered_frame_count = truncate_float_to_s32(FUN_001f96b0_2adb30(
                        (float)*(int *)&bomb_spark_jittered_frame_count_base *
                        (random_float_between(-*(float *)&bomb_spark_frame_count_jitter,
                                              *(float *)&bomb_spark_frame_count_jitter) +
                         1.0f)));
                    v2.f[2] -= *(float *)&bomb_spark_vertical_acceleration * frame_time_sq_macro *
                               (float)spark_vertical_offset_scale;
                    spawn_spark(&bomb_moby->pos,
                                &spark_velocity,
                                &v2,
                                p0,
                                spark_secondary_color,
                                spark_random_frame_count,
                                spark_vertical_offset_scale,
                                spark_jittered_frame_count,
                                *(int *)&bomb_spark_texture_selector);
                }
            }
        }
        {
            BombHit q;
            bomb_moby->rx = fast_add_rotations(bomb_moby->rx, frame_time_macro * 2.0943952f);
            bomb_moby->ry = fast_add_rotations(bomb_moby->ry, frame_time_macro * 7.8539815f);
            qcopy_nc(&old, &bomb_moby->pos);
            fast_vec_add(&bomb_moby->pos, &bomb_moby->pos, d);
            if (d->h68 == 0) {
                d->v.f[2] -= frame_time_sq_macro * 11.0f;
            } else {
                d->v.f[2] -= frame_time_sq_macro * 11.0f / 10.0f;
                if (random_integer_below(FUN_001f96f8_2adb30(6) - 1) == 0) {
                    float h = resolve_camera_surface_height((float *)&bomb_moby->pos, &q);
                    FUN_001f9a68_2adb30(&q, d, 0.25f);
                    random_integer_below_cf(
                        &bomb_moby->pos, &q, random_float_between(0.05f, 0.1f) * 210000.0f, h);
                }
            }
            q.moby = bomb_moby;
            q.flags = 0x830000;
            if (d->h54 < FUN_001f96f8_2adb30(0x12C) - FUN_001f96f8_2adb30(0xA)) {
                q.flags |= 1;
            }
            q.iE0 = 1;
            q.fDC = 2.0f;
            qcopy(&q, d);
            set_vector_length_xy(&q, &q, 1.0f);
            q.dir.f[2] = 1.0f;
            q.dir.f[3] = 5627.9248f;
            q.kind = 2;
            q.cls = bomb_moby->cls;
            q.on = 1;
            if (d->h68 != 0) {
                d->h68++;
            }
            if (collision_line(&old, &bomb_moby->pos, 0x10, (int)bomb_moby, (int)&q)) {
                if (d->h68 == 0 && collision_hit_material() == 0 && d->v.f[2] < 0.0f) {
                    Vec_2adb30 p;
                    Vec_2adb30 splash_particle_velocity;
                    char      *s;
                    int        i;
                    pool_splash(*(int *)&D_L00_001612D0_gp,
                                *(int *)&D_L00_001612D8_gp,
                                1,
                                bomb_moby->pos.f[0],
                                bomb_moby->pos.f[1],
                                0.5f,
                                -0.35f);
                    d->h68 = 1;
                    clear_u64_value(d->v.f);
                    d->v.f[2] = frame_time_macro * -1.5f;
                    p.f[0] = bomb_moby->pos.f[0];
                    p.f[1] = bomb_moby->pos.f[1];
                    p.f[2] = coll_hit.pos.f[2];
                    s = spawn_burst_moby(&p, 2.0f);
                    if (s != 0) {
                        s[0x23] = 0x70;
                    }
                    for (i = 0; i < 16; i++) {
                        float ang = random_angle_radians();
                        float r =
                            random_float_between(frame_time_macro * 0.0f, frame_time_macro * 3.0f);
                        int n;
                        splash_particle_velocity.f[0] = FUN_001f9dc8_2adb30(ang) * r;
                        splash_particle_velocity.f[1] = FUN_001f9de0_2adb30(ang) * r;
                        splash_particle_velocity.f[2] =
                            random_float_between(frame_time_macro * 3.0f, frame_time_macro * 6.5f);
                        n = random_int_between(0x5A, 0x78);
                        spawn_debris_chunk(
                            &p, &splash_particle_velocity, random_integer_below(2), n);
                    }
                } else if (d->h68 != 0 || collision_hit_material() != 0) {
                    if (coll_hit.moby != 0) {
                        if ((coll_hit.moby != ((G910 *)&hero)->cam && coll_hit.moby != d->owner) ||
                            d->h54 < FUN_001f96f8_2adb30(0x12C) - FUN_001f96f8_2adb30(0xA)) {
                            if (((BombMoby *)coll_hit.moby)->f74 != D_L00_00160770 ||
                                coll_hit.moby->cls == 0x336) {
                                H910 *h = &coll_hit;
                                bomb_moby->bBC = 1;
                                qcopy(&bomb_moby->pos, &h->pos);
                                reflect_vector(&blast_velocity, d, &h->nrm);
                                FUN_001f9bf8_2adb30(
                                    &blast_velocity, &blast_velocity, frame_time_macro * 2.0f);
                                FUN_001f9bf8_2adb30(&b, &h->nrm, 1.0f);
                            }
                        }
                    } else if (coll_hit.i1C > 0) {
                        H910 *h = &coll_hit;
                        bomb_moby->bBC = 1;
                        qcopy(&bomb_moby->pos, &h->pos);
                        reflect_vector(&blast_velocity, d, &h->nrm);
                        FUN_001f9bf8_2adb30(
                            &blast_velocity, &blast_velocity, frame_time_macro * 2.0f);
                        FUN_001f9bf8_2adb30(&b, &h->nrm, 1.0f);
                    }
                }
            } else if (fast_dec_timer_s16(&d->h54)) {
                bomb_moby->bBC = 1;
                fast_vec_add(&blast_velocity, &blast_velocity, d);
            }
            if (bomb_moby->bBC != 0) {
                float s = convert_integer_to_float((unsigned char)item_text_variant[0xA]) + 1.0f;
                float camera_distance;
                Vec_2adb30 v;
                int        i;
                FUN_001f9a68_2adb30(&blast_velocity, &blast_velocity, s);
                FUN_001f9a28_2adb30(&delta, &g_debug_camera, &bomb_moby->pos);
                camera_distance = FUN_001f9af0_2adb30(&delta);
                if (d->h68 < FUN_001f96f8_2adb30(0x14)) {
                    int n;
                    n = collect_mobies_in_sphere(&bomb_moby->pos, 0x10, bomb_moby, 0, s * 0.5f);
                    v.q = bomb_moby->pos.q;
                    send_blast_hits(bomb_moby,
                                    &v,
                                    D_L00_00177F00_2adb30,
                                    n,
                                    0,
                                    0x830000,
                                    2,
                                    1,
                                    2.0f,
                                    1.0f,
                                    1.0f);
                    bomb_moby->state = 2;
                }
                {
                    int k;
                    for (k = 0; k < 10; k++) {
                        Vec_2adb30 t;
                        float      dp;
                        t.q = 0;
                        t.f[0] = random_float_between(-1.0f, 1.0f);
                        t.f[1] = random_float_between(-1.0f, 1.0f);
                        v.q = t.q;
                        dp = FUN_001f9ab0_2adb30(&v, &b);
                        FUN_001f9bf8_2adb30(&proj, &b, dp);
                        FUN_001f9a28_2adb30(&v, &v, &proj);
                        FUN_001f9bf8_2adb30(&proj, &b, random_float_between(0.0f, 1.0f));
                        fast_vec_add(&v, &v, &proj);
                        FUN_001f9bf8_2adb30(
                            &v, &v, s * random_float_between(3.5f, 6.5f) * frame_time_macro);
                        fast_vec_add(&v, &v, &blast_velocity);
                        FUN_L00_002af450_2adb30((char *)&bomb_moby->pos,
                                                (char *)&v,
                                                random_int_between(FUN_001f96f8_2adb30(0x3C),
                                                                   FUN_001f96f8_2adb30(0x78)),
                                                0,
                                                (unsigned char)item_text_variant[0xA]);
                    }
                }
                if (d->h68 < FUN_001f96f8_2adb30(0x14)) {
                    Vec_2adb30 t;
                    float      g;
                    int        r2;
                    int        g2;
                    int        b2;
                    int        blast_effect_count;
                    {
                        int k;
                        for (k = 0; k < 4; k++) {
                            float dp;
                            t.q = 0;
                            t.f[0] = random_float_between(-1.0f, 1.0f);
                            t.f[1] = random_float_between(-1.0f, 1.0f);
                            v.q = t.q;
                            dp = FUN_001f9ab0_2adb30(&v, &b);
                            FUN_001f9bf8_2adb30(&proj, &b, dp);
                            FUN_001f9a28_2adb30(&v, &v, &proj);
                            FUN_001f9bf8_2adb30(&proj, &b, random_float_between(0.0f, 1.0f));
                            fast_vec_add(&v, &v, &proj);
                            FUN_001f9bf8_2adb30(
                                &v, &v, s * random_float_between(6.5f, 10.0f) * frame_time_macro);
                            fast_vec_add(&v, &v, &blast_velocity);
                            FUN_L00_002af450_2adb30((char *)&bomb_moby->pos,
                                                    (char *)&v,
                                                    random_int_between(FUN_001f96f8_2adb30(0x3C),
                                                                       FUN_001f96f8_2adb30(0x5A)),
                                                    1,
                                                    (unsigned char)item_text_variant[0xA]);
                        }
                    }
                    t.q = 0;
                    t.f[0] = random_float_between(-1.0f, 1.0f);
                    t.f[1] = random_float_between(-1.0f, 1.0f);
                    t.f[2] = random_float_between(-1.0f, 1.0f);
                    v.q = t.q;
                    delta.f[2] += camera_distance * 0.5f;
                    FUN_001f9bf8_2adb30(&v, &v, camera_distance / 5.0f * frame_time_macro);
                    FUN_001f9bf8_2adb30(&delta, &delta, camera_distance * 2.0f * frame_time_macro);
                    fast_vec_add(&v, &v, &delta);
                    clamp_vector_length(&v, &v, frame_time_macro * 10.0f);
                    FUN_L00_002af450_2adb30(
                        (char *)&bomb_moby->pos,
                        (char *)&v,
                        random_int_between(FUN_001f96f8_2adb30(0x3C), FUN_001f96f8_2adb30(0x5A)),
                        1,
                        (unsigned char)item_text_variant[0xA]);
                    if (camera_distance < 6.0f) {
                        blast_effect_count = truncate_float_to_s32(camera_distance) + 1;
                    } else {
                        blast_effect_count = 4;
                    }
                    g = 0.0f;
                    if (camera_distance < 7.0f) {
                        g = 7.0f - camera_distance;
                    }
                    for (i = 0; i < blast_effect_count; i++) {
                        float spd = s * random_float_between(8.0f, 10.0f) * frame_time_macro -
                                    g * frame_time_macro;
                        Colors6 c1 = l00_bomb_blast_effect_primary_palette;
                        Colors6 blast_effect_secondary_palette =
                            l00_bomb_blast_effect_secondary_palette;
                        FUN_L00_0026a9f0_2adb30(
                            &bomb_moby->pos,
                            &blast_velocity,
                            swap_packed_color_channels(c1.c[random_integer_below(6)],
                                                       (unsigned char)item_text_variant[0xA]),
                            swap_packed_color_channels(
                                blast_effect_secondary_palette.c[random_integer_below(6)],
                                (unsigned char)item_text_variant[0xA]),
                            s * 400000.0f,
                            random_int_between(FUN_001f96f8_2adb30(0xF), FUN_001f96f8_2adb30(0x14)),
                            random_int_between(FUN_001f96f8_2adb30(0x19),
                                               FUN_001f96f8_2adb30(0x1E)),
                            0,
                            0,
                            spd);
                    }
                    if (camera_distance > 9.0f) {
                        int r;
                        int gg;
                        int bb;
                        spawn_randomly_rotated_colored_moby(bomb_moby,
                                                            &bomb_moby->pos,
                                                            &blast_velocity,
                                                            s * 4.0f,
                                                            FUN_001f96f8_2adb30(0xF),
                                                            0x7F,
                                                            0x7F,
                                                            0x7F,
                                                            0x20);
                        r = 0x7F;
                        gg = 0x20;
                        bb = 0;
                        swap_color_channel_words(
                            &r, &gg, &bb, (unsigned char)item_text_variant[0xA]);
                        spawn_randomly_rotated_colored_moby(bomb_moby,
                                                            &bomb_moby->pos,
                                                            &blast_velocity,
                                                            s * 4.0f,
                                                            FUN_001f96f8_2adb30(0x18),
                                                            r,
                                                            gg,
                                                            bb,
                                                            0x20);
                    }
                    r2 = 0x7F;
                    g2 = 0x3F;
                    b2 = 0;
                    swap_color_channel_words(&r2, &g2, &b2, (unsigned char)item_text_variant[0xA]);
                    spawn_randomly_rotated_colored_moby(bomb_moby,
                                                        &bomb_moby->pos,
                                                        &blast_velocity,
                                                        s * 4.0f,
                                                        FUN_001f96f8_2adb30(0x14),
                                                        r2,
                                                        g2,
                                                        b2,
                                                        0x30);
                    r2 = 0x60;
                    g2 = 0x10;
                    b2 = 0;
                    swap_color_channel_words(&r2, &g2, &b2, (unsigned char)item_text_variant[0xA]);
                    spawn_randomly_rotated_colored_moby(bomb_moby,
                                                        &bomb_moby->pos,
                                                        &blast_velocity,
                                                        s * 3.5f,
                                                        FUN_001f96f8_2adb30(0x1B),
                                                        r2,
                                                        g2,
                                                        b2,
                                                        0x40);
                    r2 = 0x20;
                    g2 = 0;
                    b2 = 0;
                    swap_color_channel_words(&r2, &g2, &b2, (unsigned char)item_text_variant[0xA]);
                    spawn_randomly_rotated_colored_moby(bomb_moby,
                                                        &bomb_moby->pos,
                                                        &blast_velocity,
                                                        s * 3.0f,
                                                        FUN_001f96f8_2adb30(0x1D),
                                                        r2,
                                                        g2,
                                                        b2,
                                                        0x20);
                }
                if (d->h68 >= FUN_001f96f8_2adb30(0x14)) {
                    float h;
                    bomb_moby->pos.f[2] += 1.0f;
                    h = resolve_camera_surface_height((float *)&bomb_moby->pos, &v);
                    bomb_moby->pos.f[2] -= 1.0f;
                    {
                        int i;
                        for (i = 0; i < 150; i++) {
                            Vec_2adb30 u;
                            Vec_2adb30 t;
                            t.q = 0;
                            t.f[0] = random_float_between(-1.0f, 1.0f);
                            t.f[1] = random_float_between(-1.0f, 1.0f);
                            t.f[2] = random_float_between(-0.5f, 2.0f);
                            u.q = t.q;
                            FUN_001f9bf8_2adb30(&u,
                                                &u,
                                                s * random_float_between(frame_time_macro * 4.0f,
                                                                         frame_time_macro * 8.0f));
                            random_integer_below_cf(&bomb_moby->pos,
                                                    &u,
                                                    random_float_between(0.05f, 0.1f) * 210000.0f,
                                                    h);
                        }
                    }
                }
                d->v.f[0] = 0.0f;
                bomb_moby->bBC = 0;
                if (d->h68 >= FUN_001f96f8_2adb30(0x14)) {
                    trooper_play_sound(0x16, 0, (int)bomb_moby, 0);
                } else if ((unsigned char)item_text_variant[0xA] != 0) {
                    allocate_voice_for_target_entry(1, 0, (int)bomb_moby);
                } else {
                    allocate_voice_for_target_entry(0, 0, (int)bomb_moby);
                }
                if (d->h68 != 0) {
                    float gz;
                    float wz;
                    int   cr = 0x60;
                    int   cg = 0x60;
                    int   cb = 0x7F;
                    int   delay = 0;
                    v.q = bomb_moby->pos.q;
                    v.f[2] += 3.5f;
                    gz = probe_ground_height(&v, 0, 0.5f);
                    wz = allocate_voice_for_target_entry_cf(&v);
                    if (gz != wz && gz - wz < 1.2f) {
                        Vec_2adb30 v2;
                        Vec_2adb30 particle_work_vector;
                        int        i;
                        int        j;
                        for (i = 0; i < 100; i++) {
                            float r;
                            float ang;
                            particle_work_vector.q = 0;
                            particle_work_vector.f[0] = random_float_between(
                                frame_time_macro * -0.75f, frame_time_macro * 0.75f);
                            particle_work_vector.f[1] = random_float_between(
                                frame_time_macro * -0.75f, frame_time_macro * 0.75f);
                            particle_work_vector.f[2] = random_float_between(
                                frame_time_macro * 4.0f, frame_time_macro * 14.0f);
                            v2.q = particle_work_vector.q;
                            r = random_float_between(0.0f, 0.35f);
                            ang = random_angle_radians();
                            particle_work_vector.f[0] = FUN_001f9dc8_2adb30(ang) * r;
                            particle_work_vector.f[1] = FUN_001f9de0_2adb30(ang) * r;
                            particle_work_vector.f[2] = 0.0f;
                            fast_vec_add(
                                &particle_work_vector, &particle_work_vector, &bomb_moby->pos);
                            particle_work_vector.f[2] = gz + 0.5f;
                            v2.f[2] -= r * frame_time_macro * 8.0f;
                            spawn_gravity_floor_burst_particle(&particle_work_vector,
                                                               &v2,
                                                               delay,
                                                               random_float_between(0.25f, 1.0f),
                                                               gz,
                                                               frame_time_sq_macro * 20.0f,
                                                               (0x16 << 24) | (cb << 16) |
                                                                   (cg << 8) | cr,
                                                               (cb << 16) | (cg << 8) | cr,
                                                               1);
                        }
                        for (j = 0; j < 20; j++) {
                            int shrinking_particle_primary_color =
                                mine_blend_color(0x7F << 24,
                                                 (0x7F << 24) | (cb << 16) | (cg << 8) | cr,
                                                 random_float_between(0.25f, 1.0f));
                            int   c1 = mine_blend_color(0,
                                                      ((cb / 2) << 16) | ((cg / 2) << 8) | (cr / 2),
                                                      random_float_between(0.5f, 1.0f));
                            float sz;
                            int   life;
                            particle_work_vector.f[0] = random_float_between(-3.0f, 3.0f);
                            particle_work_vector.f[1] = random_float_between(-3.0f, 3.0f);
                            particle_work_vector.f[2] = random_float_between(
                                frame_time_macro * 4.0f, frame_time_macro * 14.0f);
                            FUN_001f9bf8_2adb30(&particle_work_vector,
                                                &particle_work_vector,
                                                random_float_between(1.5f, 3.0f) *
                                                    frame_time_macro);
                            particle_work_vector.f[2] =
                                random_float_between(3.0f, 6.0f) * frame_time_macro;
                            v2.q = bomb_moby->pos.q;
                            v2.f[2] = gz;
                            sz = random_float_between(31500.002f, 52500.0f);
                            life = random_int_between(FUN_001f96f8_2adb30(0x5A),
                                                      FUN_001f96f8_2adb30(0x96));
                            spawn_shrinking_effect_particle_with_gravity_and_secondary_spawns(
                                &v2,
                                &particle_work_vector,
                                shrinking_particle_primary_color,
                                c1,
                                sz,
                                life,
                                1,
                                D_L00_001B2080_2adb30[11][0],
                                -1);
                        }
                    }
                }
                l00_camera.shake_amount = camera_distance < 20.0f ? 0.4f - camera_distance * 0.0175f
                                                                  : 0.4f - 20.0f * 0.0175f;
                l00_camera.shake_frames = FUN_001f96f8_2adb30(0x19);
                fast_vec_dist(&bomb_moby->pos, ((G910 *)&hero)->pos);
                mine_camera_blast((char *)&rec, (char *)&bomb_moby->pos, 0, 0);
                if (bomb_moby->state != 2) {
                    mark_moby_for_removal(bomb_moby);
                    return;
                }
            }
        }
    } else if (bomb_moby->state != 2) {
        int v;
        d->h6a++;
        v = truncate_float_to_s32(FUN_001f9de0_2adb30(normalize_angle((float)d->h6a * 0.068f)) *
                                  96.0f) +
            0x9F;
        bomb_moby->color = (v << 16) | (0xFF << 24) | (v << 8) | v;
    }
    if (bomb_moby->state == 2) {
        Vec_2adb30 p;
        float      r;
        int        n;
        bomb_moby->flags |= 0x41;
        r = interpolate_scalar_linear(
            0.5f,
            (convert_integer_to_float(item_text_variant[0xA]) + 1.0f) * 3.0f - 0.5f,
            convert_integer_to_float(bomb_moby->bBC) / (float)FUN_001f96f8_2adb30(0xF));
        n = collect_mobies_in_sphere(&bomb_moby->pos, 0x10, bomb_moby, 0, r);
        p.q = bomb_moby->pos.q;
        send_blast_hits(
            bomb_moby, &p, D_L00_00177F00_2adb30, n, 0, 0x830000, 2, 1, 2.0f, 1.0f, 1.0f);
        if (++bomb_moby->bBC > FUN_001f96f8_2adb30(0xF)) {
            mark_moby_for_removal(bomb_moby);
        }
    }
}

int   D_L00_00160770 = -1;
short bomb_spark_primary_color_end __asm__("D_L00_00161420") = -1;
short bomb_spark_secondary_color_end __asm__("D_L00_00161428") = 0x2080;

Colors6 l00_bomb_blast_effect_primary_palette __asm__("D_L00_001E9920") = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors6 l00_bomb_blast_effect_secondary_palette __asm__("D_L00_001E9938") = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};
