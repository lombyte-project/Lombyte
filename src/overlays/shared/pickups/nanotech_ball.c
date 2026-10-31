/* Shared moby class nanotech_ball: the health orb that falls, flies to the hero and is collected, and the function that spawns it. */

#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/rendering/view/build_look_at_matrix.h"
#include "rnc/overlay/collision.h"
#include "rnc/overlay/shared/pickups/nanotech_ball_data/spawn_nanotech_ball_collection_effect_pair.h"
#include "rnc/overlay/shared/pickups/nanotech_ball_data/update_nanotech_ball_collection_effects.h"
#include "rnc/overlay/shared/pickups/nanotech_ball_data/nanotech_ball_update_3.h"
#include "rnc/overlay/shared/pickups/nanotech_ball_data/nanotech_ball_update_2.h"
#include "rnc/overlay/shared/pickups/nanotech_ball_data/nanotech_ball_update.h"

typedef struct {
    char        pad[0x10];
    OvlQuad     q;
    signed char b20;
} P2d8138;

typedef struct {
    char     pad[0xC];
    P2d8138 *p;
} T2d8138;

typedef struct {
    char           pad[0x10];
    OvlQuad        q;
    char           b20;
    char           pad2[0xF];
    char           b30;
    char           pad3[3];
    unsigned short h34;
    char           pad4[0x42];
    T2d8138       *t78;
    char           pad5[0x18];
    int            f94;
} O2d8138;

typedef unsigned int u128_2d81b8 __attribute__((mode(TI), aligned(16)));

typedef union {
    u128_2d81b8 q;
    float       f[4];
} V_2d81b8;

typedef struct {
    char  p0[0x10];
    float x, y, z;
} O_2d81b8;

typedef struct {
    char  p0[0xA];
    short hA;
    char  pC[4];
    float f10, f14, f18;
    char  p1C[0x50 - 0x1C];
    float f50;
    char  p54[0x70 - 0x54];
    float f70, f74, f78;
} D_2d81b8;

typedef struct {
    char      p0[0x18];
    O_2d81b8 *o;
    char      p1C[0x28 - 0x1C];
    float     f28;
} G_2d81b8;

typedef struct {
    float x, y, z, w_002d8418;
} __attribute__((aligned(16))) Vec4_002d8418;

typedef struct {
    f32 x, y, z, w;
} __attribute__((aligned(16))) V_2d8510;

typedef struct {
    u8  p0[0x58];
    s16 h58[4];
    f32 f60[4];
} O_2d8510;

typedef struct {
    u8        p0[0x10];
    V_2d8510  v10;
    u8        p20[0x58];
    O_2d8510 *p78;
} M_2d8510;

typedef struct {
    u8  p0[0xF8];
    u8 *pF8;
} T_2d8510;

typedef union {
    u128  q_002d8850;
    float f[4];
} Vec4_002d8850;

typedef struct {
    float         f0_002d8850;
    char          pad[0xc];
    u128          p10;
    Vec4_002d8850 p20;
    char         *e[4];
} M_002d8850;

typedef f32 V4[4] __attribute__((aligned(16)));

typedef struct Hero Hero;

typedef struct Moby Moby;

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) V_2D9EA0;

typedef struct {
    float x, y, z;
} V3;

typedef struct Part {
    int           f0;
    int           f4;
    unsigned char b8;
    char          p9[3];
    float         fC;
    V_2D9EA0      pos;
} Part;

typedef struct Ext {
    int f0;
    union {
        struct {
            struct Moby *owner;
            float        scale;
            float        ang;
        } s;
        V3 a;
    } u;
    V3    b;
    short h1C;
    short h1E;
} Ext;

typedef struct D_2D9EA0 {
    float        bob_offset;
    float        bob_phase;
    short        h8;
    short        hA;
    struct Moby *owner;
    V_2D9EA0     pos;
    V_2D9EA0     v20;
    Part        *slots[8];
    int          f50;
    short        h54;
    short        h56;
    short        h58[4];
    float        f60[4];
    char         p70[0xC];
    int          f7C;
} D_2D9EA0;

/* Ported from rac1-decomp (nonmatching/shared/func_L00_002DACC0.c: func_L00_002DACC0), a near miss there; names translated to the US level program. */
typedef struct {
    u128  points[4];
    u32   colors[4];
    float texcoords[8];
    u64   texture[4];
} __attribute__((aligned(16))) GeometryQuad;

int  FUN_001efa68_2d81b8(void *, void *, int, void *, int) __asm__("FUN_001efa68");
void FUN_00214890_002d8418(Vec4_002d8418 *,
                           Vec4_002d8418 *,
                           Vec4_002d8418 *,
                           float) __asm__("FUN_00214890");
void spawn_moving_hero_relative_alpha_fade_particle(
    void *, Vec4_002d8418 *, int, int, int, int, float) __asm__("FUN_L00_002730e0");
s32   FUN_001f96f8_2d8510(s32) __asm__("FUN_001f96f8");
s32   FUN_001fa6e0_2d8510(s32, s32, f32) __asm__("FUN_001fa6e0");
u8   *FUN_L00_00272f68_2d8510(void *, s32, u8, s32, s32, s32, s32, f32) __asm__("FUN_L00_00272f68");
void  FUN_001f9bf8_002d8850(void *, void *, float) __asm__("FUN_001f9bf8");
float cubic_hermite_scalar(float a, float b, float c, float d, float t) __asm__("FUN_L00_00257e20");
void  draw_nanotech_ball(Moby *moby) __asm__("FUN_L00_002d9810");
extern int   func_001F9850_2D9EA0(int) __asm__("FUN_001f96f8");
extern void  func_L00_001FF4B0_2D9EA0(void *, void *, float) __asm__("FUN_001f9bf8");
extern void  func_001F9BD8_2D9EA0(void *, void *, void *) __asm__("FUN_001f9a10");
extern void  func_001F9BF0_2D9EA0(void *, void *, void *) __asm__("FUN_001f9a28");
extern float func_001F9FA8_2D9EA0(float) __asm__("FUN_001f9de0");
extern int   func_L00_002D9E30_2D9EA0(void *) __asm__("FUN_L00_002d8980");
extern void  func_001F9C30_2D9EA0(void *, void *, float) __asm__("FUN_001f9a68");
extern void  func_002156E0_2D9EA0(void *, void *, float, void *) __asm__("FUN_00214890");
extern void  func_L00_002D9D00_2D9EA0(void *, void *, int) __asm__("FUN_L00_002d8850");
extern Part *func_L00_002745A8_2D9EA0(void *, int, int) __asm__("FUN_L00_00273708");
extern void  func_L00_002D98C8_2D9EA0(void *) __asm__("FUN_L00_002d8418");
extern int   func_L00_002D99C0_2D9EA0(void *) __asm__("FUN_L00_002d8510");
extern void  func_L00_002D9668_2D9EA0(void *) __asm__("FUN_L00_002d81b8");
extern void  func_L00_002688A8_2D9EA0(void *) __asm__("FUN_L00_00267a08");
extern int   func_L00_0028EF68_2D9EA0(int, int, void *, int) __asm__("FUN_L00_0028dc90");
extern s64   get_effect_texture(s32) __asm__("func_001F44B8");
extern void  vu1_add_g_sregister(s32, u64) __asm__("func_00233980");
void         font_queue_vu_state(void) __asm__("FUN_001f76a0");
void         FUN_L00_001fde98(int, void *, void *, void *, int);
extern void  draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");
float        FUN_L00_00200260(float, float);
int          collision_line(Vec4 *, Vec4 *, int, int, int) __asm__("FUN_001efa68");

extern int   D_L00_00173E40_lui[] __asm__("D_L00_00173E40") NOT_SDA;
extern float frame_time_sq __asm__("D_0015ED70");
extern char  g_debug_camera[] __asm__("D_L00_00166DC0");
extern float nanotech_ball_collection_gravity_scale_range_start_gp __asm__("D_L00_00161A60")
    __attribute__((sda));
extern float nanotech_ball_collection_gravity_scale_range_end_gp __asm__("D_L00_00161A64")
    __attribute__((sda));
extern unsigned char l00_nanotech_ball_collection_particle_lifetime_frames __asm__("D_L00_00161A68")
    __attribute__((sda));
extern float nanotech_ball_collection_first_particle_scale_factor_gp __asm__("D_L00_00161A74")
    __attribute__((sda));
extern float nanotech_ball_collection_second_particle_scale_factor_gp __asm__("D_L00_00161A78")
    __attribute__((sda));
extern char     g_debug_camera[] __asm__("D_L00_00166DC0") NOT_SDA;
extern T_2d8510 D_L00_001B2080_lui __asm__("D_L00_001B2080") NOT_SDA;
extern s32      D_L00_00161A58_gp __asm__("D_L00_00161A58") __attribute__((sda));
extern s32      nanotech_ball_collection_color_range_start_gp __asm__("D_L00_00161A7C")
    __attribute__((sda));
extern s32 nanotech_ball_collection_color_range_end_gp __asm__("D_L00_00161A80")
    __attribute__((sda));
extern f32 nanotech_ball_collection_particle_rotation_steps[1] __asm__("D_L00_00161A90")
    __attribute__((sda));
extern f32 nanotech_ball_collection_particle_scale_factors[1] __asm__("D_L00_00161AA0")
    __attribute__((sda));
extern f32 nanotech_ball_collection_particle_z_offsets[1] __asm__("D_L00_00161AB0")
    __attribute__((sda));
extern u128 D_L00_001E3CA0_002d8850[] __asm__("D_L00_001E3CA0");
extern V4   D_L00_001E3CA0[];
extern V4   D_L00_001E3D20[];
extern f32  nanotech_ball_slot_offset_transform_amount_gp __asm__("D_L00_001619F0")
    __attribute__((sda));
extern f32           D_L00_00161A14_gp __asm__("D_L00_00161A14") __attribute__((sda));
extern s32           D_001413D0_lui[] __asm__("D_001413D0") NOT_SDA;
extern Moby         *D_L00_0015F63C;
extern char          D_L00_00166DC0[];
extern int           hero_max_health __asm__("D_0015EDA0");
extern unsigned char D_L00_001619F4 __attribute__((sda));
extern float         D_L00_001619F8 __attribute__((sda));
extern float         D_L00_001619FC __attribute__((sda));
extern float         nanotech_ball_bob_phase_step_gp __asm__("D_L00_00161A0C") __attribute__((sda));
extern float         l00_nanotech_ball_bob_amplitude __asm__("D_L00_00161A10") __attribute__((sda));
extern float         D_L00_00161A14_gp __asm__("D_L00_00161A14") __attribute__((sda));
extern int           nanotech_ball_collection_lifetime_frames_gp __asm__("D_L00_00161A3C")
    __attribute__((sda));
extern int nanotech_ball_slot_collection_min_frames_gp __asm__("D_L00_00161A44")
    __attribute__((sda));
extern int nanotech_ball_slot_collection_max_frames_gp __asm__("D_L00_00161A48")
    __attribute__((sda));
extern int   D_L00_00161A58_gp __asm__("D_L00_00161A58") __attribute__((sda));
extern float nanotech_ball_slot_scale_pulse_amplitude_gp __asm__("D_L00_00161A5C")
    __attribute__((sda));
extern unsigned char          D_0013F5E0[];
extern volatile unsigned char nanotech_ball_invulnerability_pickup_enabled __asm__("D_0015EDB6");
extern V_2D9EA0               D_L00_001E4070_2D9EA0[] __asm__("D_L00_001E3D20");
extern float                  D_L00_001E17F0_2D9EA0[] __asm__("D_L00_001E14A0");
extern int                    D_L00_00161B10_macro __asm__("D_L00_00161B10") MACRO_ADDR;
extern int l00_nanotech_ball_blend_minuend_color_selector __asm__("D_L00_00161AC0")
    __attribute__((sda));
extern int l00_nanotech_ball_blend_subtrahend_color_selector __asm__("D_L00_00161AC4")
    __attribute__((sda));
extern int l00_nanotech_ball_blend_alpha_selector __asm__("D_L00_00161AC8") __attribute__((sda));
extern int l00_nanotech_ball_blend_addend_color_selector __asm__("D_L00_00161ACC")
    __attribute__((sda));
extern int   l00_nanotech_ball_blend_fixed_alpha __asm__("D_L00_00161AD0") __attribute__((sda));
extern int   l00_nanotech_ball_mesh_texture_index __asm__("D_L00_00161AD4") __attribute__((sda));
extern u32   l00_nanotech_ball_initial_quad_color __asm__("D_L00_00161AD8") __attribute__((sda));
extern float l00_nanotech_ball_pulse_quad_base_scale __asm__("D_L00_00161ADC") __attribute__((sda));
extern u32   l00_nanotech_ball_pulse_quad_base_color __asm__("D_L00_00161AE0") __attribute__((sda));
extern int   l00_nanotech_ball_state_one_quad_texture_index __asm__("D_L00_00161AE4")
    __attribute__((sda));
extern float l00_nanotech_ball_state_one_quad_uv_vector_length __asm__("D_L00_00161AE8")
    __attribute__((sda));
extern float D_L00_00161AEC_gp __asm__("D_L00_00161AEC") __attribute__((sda));
extern char  D_L00_00161AF0_macro[] __asm__("D_L00_00161AF0") MACRO_ADDR;
extern char  nanotech_ball_mesh_source_vertices[] __asm__("D_L00_001E14C0");
extern char  nanotech_ball_mesh_texcoords[] __asm__("D_L00_001E26E0");
extern char  nanotech_ball_mesh_vertex_colors[] __asm__("D_L00_001E2FF0");
extern char  nanotech_ball_pulse_quad_source_vertices[] __asm__("D_L00_001E3480");
extern char  nanotech_ball_pulse_quad_vertex_indices[] __asm__("D_L00_001E3A80");
extern char  nanotech_ball_state_one_quad_source_vertices[] __asm__("D_L00_001E3B80");
extern char  nanotech_ball_state_one_quad_vertex_indices[] __asm__("D_L00_001E3C80");
extern char  nanotech_ball_transformed_mesh_vertices[] __asm__("D_L00_001E3DA0");
extern int   D_L00_00173E40_lui[];

float         D_L01_00161DD0 = 1.0f;
float         D_L01_00161DD4 = 2.0f;
unsigned char D_L01_00161DD8 = 0x3c;
float         D_L01_00161DE4 = 0.4f;
float         D_L01_00161DE8 = 0.2f;
s32           D_L01_00161DC8 = 0x5a;
s32           D_L01_00161DEC = 0x407f2020;
s32           D_L01_00161DF0 = 0x107f2020;
f32           D_L01_00161E00[1] = {-2.0f};

f32 D_L01_00161E10[1] = {4.0f};

f32 D_L01_00161E20[1] = {-0.25f};

f32           D_L01_00161D60 = 0.06981317f;
f32           D_L01_00161D84 = 0.25f;
Moby         *D_L01_0015F63C = 0;
unsigned char D_L01_00161D64 = 4;
float         D_L01_00161D68 = 1e+04f;
float         D_L01_00161D6C = 6e+04f;
float         D_L01_00161D7C = 0.034906585f;
float         D_L01_00161D80 = 0.06f;
int           D_L01_00161DAC = 0x12c;
int           D_L01_00161DB4 = 0x14;
int           D_L01_00161DB8 = 0x50;
float         D_L01_00161DCC = 0.15f;
int           D_L01_00161E80 = 0;
int           D_L01_00161E30 = 0;
int           D_L01_00161E34 = 1;
int           D_L01_00161E38 = 0;
int           D_L01_00161E3C = 1;
int           D_L01_00161E40 = 0x10;
int           D_L01_00161E44 = 0x15;
u32           D_L01_00161E48 = 0x50804040;
float         D_L01_00161E4C = 0.3f;
u32           D_L01_00161E50 = 0x802020;
int           D_L01_00161E54 = 0x15;
float         D_L01_00161E58 = 2.0f;
float         D_L01_00161E5C = 1.125f;
float         D_L02_00161D80 = 1.0f;
float         D_L02_00161D84 = 2.0f;
unsigned char D_L02_00161D88 = 0x3c;
float         D_L02_00161D94 = 0.4f;
float         D_L02_00161D98 = 0.2f;
s32           D_L02_00161D78 = 0x5a;
s32           D_L02_00161D9C = 0x407f2020;
s32           D_L02_00161DA0 = 0x107f2020;
f32           D_L02_00161DB0[1] = {-2.0f};

f32 D_L02_00161DC0[1] = {4.0f};

f32 D_L02_00161DD0[1] = {-0.25f};

f32           D_L02_00161D10 = 0.06981317f;
f32           D_L02_00161D34 = 0.25f;
Moby         *D_L02_0015F63C = 0;
unsigned char D_L02_00161D14 = 4;
float         D_L02_00161D18 = 1e+04f;
float         D_L02_00161D1C = 6e+04f;
float         D_L02_00161D2C = 0.034906585f;
float         D_L02_00161D30 = 0.06f;
int           D_L02_00161D5C = 0x12c;
int           D_L02_00161D64 = 0x14;
int           D_L02_00161D68 = 0x50;
float         D_L02_00161D7C = 0.15f;
int           D_L02_00161E30 = 0;
int           D_L02_00161DE0 = 0;
int           D_L02_00161DE4 = 1;
int           D_L02_00161DE8 = 0;
int           D_L02_00161DEC = 1;
int           D_L02_00161DF0 = 0x10;
int           D_L02_00161DF4 = 0x15;
u32           D_L02_00161DF8 = 0x50804040;
float         D_L02_00161DFC = 0.3f;
u32           D_L02_00161E00 = 0x802020;
int           D_L02_00161E04 = 0x15;
float         D_L02_00161E08 = 2.0f;
float         D_L02_00161E0C = 1.125f;
float         D_L03_00161A30 = 1.0f;
float         D_L03_00161A34 = 2.0f;
unsigned char D_L03_00161A38 = 0x3c;
float         D_L03_00161A44 = 0.4f;
float         D_L03_00161A48 = 0.2f;
s32           D_L03_00161A28 = 0x5a;
s32           D_L03_00161A4C = 0x407f2020;
s32           D_L03_00161A50 = 0x107f2020;
f32           D_L03_00161A60[1] = {-2.0f};

f32 D_L03_00161A70[1] = {4.0f};

f32 D_L03_00161A80[1] = {-0.25f};

f32           D_L03_001619C0 = 0.06981317f;
f32           D_L03_001619E4 = 0.25f;
Moby         *D_L03_0015F63C = 0;
unsigned char D_L03_001619C4 = 4;
float         D_L03_001619C8 = 1e+04f;
float         D_L03_001619CC = 6e+04f;
float         D_L03_001619DC = 0.034906585f;
float         D_L03_001619E0 = 0.06f;
int           D_L03_00161A0C = 0x12c;
int           D_L03_00161A14 = 0x14;
int           D_L03_00161A18 = 0x50;
float         D_L03_00161A2C = 0.15f;
int           D_L03_00161AE0 = 0;
int           D_L03_00161A90 = 0;
int           D_L03_00161A94 = 1;
int           D_L03_00161A98 = 0;
int           D_L03_00161A9C = 1;
int           D_L03_00161AA0 = 0x10;
int           D_L03_00161AA4 = 0x15;
u32           D_L03_00161AA8 = 0x50804040;
float         D_L03_00161AAC = 0.3f;
u32           D_L03_00161AB0 = 0x802020;
int           D_L03_00161AB4 = 0x15;
float         D_L03_00161AB8 = 2.0f;
float         D_L03_00161ABC = 1.125f;
float         D_L04_00161BB0 = 1.0f;
float         D_L04_00161BB4 = 2.0f;
unsigned char D_L04_00161BB8 = 0x3c;
float         D_L04_00161BC4 = 0.4f;
float         D_L04_00161BC8 = 0.2f;
s32           D_L04_00161BA8 = 0x5a;
s32           D_L04_00161BCC = 0x407f2020;
s32           D_L04_00161BD0 = 0x107f2020;
f32           D_L04_00161BE0[1] = {-2.0f};

f32 D_L04_00161BF0[1] = {4.0f};

f32 D_L04_00161C00[1] = {-0.25f};

f32           D_L04_00161B40 = 0.06981317f;
f32           D_L04_00161B64 = 0.25f;
Moby         *D_L04_0015F63C = 0;
unsigned char D_L04_00161B44 = 4;
float         D_L04_00161B48 = 1e+04f;
float         D_L04_00161B4C = 6e+04f;
float         D_L04_00161B5C = 0.034906585f;
float         D_L04_00161B60 = 0.06f;
int           D_L04_00161B8C = 0x12c;
int           D_L04_00161B94 = 0x14;
int           D_L04_00161B98 = 0x50;
float         D_L04_00161BAC = 0.15f;
int           D_L04_00161C60 = 0;
int           D_L04_00161C10 = 0;
int           D_L04_00161C14 = 1;
int           D_L04_00161C18 = 0;
int           D_L04_00161C1C = 1;
int           D_L04_00161C20 = 0x10;
int           D_L04_00161C24 = 0x15;
u32           D_L04_00161C28 = 0x50804040;
float         D_L04_00161C2C = 0.3f;
u32           D_L04_00161C30 = 0x802020;
int           D_L04_00161C34 = 0x15;
float         D_L04_00161C38 = 2.0f;
float         D_L04_00161C3C = 1.125f;
float         D_L05_00161C10 = 1.0f;
float         D_L05_00161C14 = 2.0f;
unsigned char D_L05_00161C18 = 0x3c;
float         D_L05_00161C24 = 0.4f;
float         D_L05_00161C28 = 0.2f;
s32           D_L05_00161C08 = 0x5a;
s32           D_L05_00161C2C = 0x407f2020;
s32           D_L05_00161C30 = 0x107f2020;
f32           D_L05_00161C40[1] = {-2.0f};

f32 D_L05_00161C50[1] = {4.0f};

f32 D_L05_00161C60[1] = {-0.25f};

f32           D_L05_00161BA0 = 0.06981317f;
f32           D_L05_00161BC4 = 0.25f;
Moby         *D_L05_0015F63C = 0;
unsigned char D_L05_00161BA4 = 4;
float         D_L05_00161BA8 = 1e+04f;
float         D_L05_00161BAC = 6e+04f;
float         D_L05_00161BBC = 0.034906585f;
float         D_L05_00161BC0 = 0.06f;
int           D_L05_00161BEC = 0x12c;
int           D_L05_00161BF4 = 0x14;
int           D_L05_00161BF8 = 0x50;
float         D_L05_00161C0C = 0.15f;
int           D_L05_00161CC0 = 0;
int           D_L05_00161C70 = 0;
int           D_L05_00161C74 = 1;
int           D_L05_00161C78 = 0;
int           D_L05_00161C7C = 1;
int           D_L05_00161C80 = 0x10;
int           D_L05_00161C84 = 0x15;
u32           D_L05_00161C88 = 0x50804040;
float         D_L05_00161C8C = 0.3f;
u32           D_L05_00161C90 = 0x802020;
int           D_L05_00161C94 = 0x15;
float         D_L05_00161C98 = 2.0f;
float         D_L05_00161C9C = 1.125f;
float         D_L06_00161B30 = 1.0f;
float         D_L06_00161B34 = 2.0f;
unsigned char D_L06_00161B38 = 0x3c;
float         D_L06_00161B44 = 0.4f;
float         D_L06_00161B48 = 0.2f;
s32           D_L06_00161B28 = 0x5a;
s32           D_L06_00161B4C = 0x407f2020;
s32           D_L06_00161B50 = 0x107f2020;
f32           D_L06_00161B60[1] = {-2.0f};

f32 D_L06_00161B70[1] = {4.0f};

f32 D_L06_00161B80[1] = {-0.25f};

f32           D_L06_00161AC0 = 0.06981317f;
f32           D_L06_00161AE4 = 0.25f;
Moby         *D_L06_0015F63C = 0;
unsigned char D_L06_00161AC4 = 4;
float         D_L06_00161AC8 = 1e+04f;
float         D_L06_00161ACC = 6e+04f;
float         D_L06_00161ADC = 0.034906585f;
float         D_L06_00161AE0 = 0.06f;
int           D_L06_00161B0C = 0x12c;
int           D_L06_00161B14 = 0x14;
int           D_L06_00161B18 = 0x50;
float         D_L06_00161B2C = 0.15f;
int           D_L06_00161BE0 = 0;
int           D_L06_00161B90 = 0;
int           D_L06_00161B94 = 1;
int           D_L06_00161B98 = 0;
int           D_L06_00161B9C = 1;
int           D_L06_00161BA0 = 0x10;
int           D_L06_00161BA4 = 0x15;
u32           D_L06_00161BA8 = 0x50804040;
float         D_L06_00161BAC = 0.3f;
u32           D_L06_00161BB0 = 0x802020;
int           D_L06_00161BB4 = 0x15;
float         D_L06_00161BB8 = 2.0f;
float         D_L06_00161BBC = 1.125f;
float         D_L07_001619D0 = 1.0f;
float         D_L07_001619D4 = 2.0f;
unsigned char D_L07_001619D8 = 0x3c;
float         D_L07_001619E4 = 0.4f;
float         D_L07_001619E8 = 0.2f;
s32           D_L07_001619C8 = 0x5a;
s32           D_L07_001619EC = 0x407f2020;
s32           D_L07_001619F0 = 0x107f2020;
f32           D_L07_00161A00[1] = {-2.0f};

f32 D_L07_00161A10[1] = {4.0f};

f32 D_L07_00161A20[1] = {-0.25f};

f32           D_L07_00161960 = 0.06981317f;
f32           D_L07_00161984 = 0.25f;
Moby         *D_L07_0015F63C = 0;
unsigned char D_L07_00161964 = 4;
float         D_L07_00161968 = 1e+04f;
float         D_L07_0016196C = 6e+04f;
float         D_L07_0016197C = 0.034906585f;
float         D_L07_00161980 = 0.06f;
int           D_L07_001619AC = 0x12c;
int           D_L07_001619B4 = 0x14;
int           D_L07_001619B8 = 0x50;
float         D_L07_001619CC = 0.15f;
int           D_L07_00161A80 = 0;
int           D_L07_00161A30 = 0;
int           D_L07_00161A34 = 1;
int           D_L07_00161A38 = 0;
int           D_L07_00161A3C = 1;
int           D_L07_00161A40 = 0x10;
int           D_L07_00161A44 = 0x15;
u32           D_L07_00161A48 = 0x50804040;
float         D_L07_00161A4C = 0.3f;
u32           D_L07_00161A50 = 0x802020;
int           D_L07_00161A54 = 0x15;
float         D_L07_00161A58 = 2.0f;
float         D_L07_00161A5C = 1.125f;
float         D_L08_00162050 = 1.0f;
float         D_L08_00162054 = 2.0f;
unsigned char D_L08_00162058 = 0x3c;
float         D_L08_00162064 = 0.4f;
float         D_L08_00162068 = 0.2f;
s32           D_L08_00162048 = 0x5a;
s32           D_L08_0016206C = 0x407f2020;
s32           D_L08_00162070 = 0x107f2020;
f32           D_L08_00162080[1] = {-2.0f};

f32 D_L08_00162090[1] = {4.0f};

f32 D_L08_001620A0[1] = {-0.25f};

f32           D_L08_00161FE0 = 0.06981317f;
f32           D_L08_00162004 = 0.25f;
Moby         *D_L08_0015F63C = 0;
unsigned char D_L08_00161FE4 = 4;
float         D_L08_00161FE8 = 1e+04f;
float         D_L08_00161FEC = 6e+04f;
float         D_L08_00161FFC = 0.034906585f;
float         D_L08_00162000 = 0.06f;
int           D_L08_0016202C = 0x12c;
int           D_L08_00162034 = 0x14;
int           D_L08_00162038 = 0x50;
float         D_L08_0016204C = 0.15f;
int           D_L08_00162100 = 0;
int           D_L08_001620B0 = 0;
int           D_L08_001620B4 = 1;
int           D_L08_001620B8 = 0;
int           D_L08_001620BC = 1;
int           D_L08_001620C0 = 0x10;
int           D_L08_001620C4 = 0x15;
u32           D_L08_001620C8 = 0x50804040;
float         D_L08_001620CC = 0.3f;
u32           D_L08_001620D0 = 0x802020;
int           D_L08_001620D4 = 0x15;
float         D_L08_001620D8 = 2.0f;
float         D_L08_001620DC = 1.125f;
float         D_L09_00161AA0 = 1.0f;
float         D_L09_00161AA4 = 2.0f;
unsigned char D_L09_00161AA8 = 0x3c;
float         D_L09_00161AB4 = 0.4f;
float         D_L09_00161AB8 = 0.2f;
s32           D_L09_00161A98 = 0x5a;
s32           D_L09_00161ABC = 0x407f2020;
s32           D_L09_00161AC0 = 0x107f2020;
f32           D_L09_00161AD0[1] = {-2.0f};

f32 D_L09_00161AE0[1] = {4.0f};

f32 D_L09_00161AF0[1] = {-0.25f};

f32           D_L09_00161A30 = 0.06981317f;
f32           D_L09_00161A54 = 0.25f;
Moby         *D_L09_0015F63C = 0;
unsigned char D_L09_00161A34 = 4;
float         D_L09_00161A38 = 1e+04f;
float         D_L09_00161A3C = 6e+04f;
float         D_L09_00161A4C = 0.034906585f;
float         D_L09_00161A50 = 0.06f;
int           D_L09_00161A7C = 0x12c;
int           D_L09_00161A84 = 0x14;
int           D_L09_00161A88 = 0x50;
float         D_L09_00161A9C = 0.15f;
int           D_L09_00161B50 = 0;
int           D_L09_00161B00 = 0;
int           D_L09_00161B04 = 1;
int           D_L09_00161B08 = 0;
int           D_L09_00161B0C = 1;
int           D_L09_00161B10 = 0x10;
int           D_L09_00161B14 = 0x15;
u32           D_L09_00161B18 = 0x50804040;
float         D_L09_00161B1C = 0.3f;
u32           D_L09_00161B20 = 0x802020;
int           D_L09_00161B24 = 0x15;
float         D_L09_00161B28 = 2.0f;
float         D_L09_00161B2C = 1.125f;
float         D_L10_00161A10 = 1.0f;
float         D_L10_00161A14 = 2.0f;
unsigned char D_L10_00161A18 = 0x3c;
float         D_L10_00161A24 = 0.4f;
float         D_L10_00161A28 = 0.2f;
s32           D_L10_00161A08 = 0x5a;
s32           D_L10_00161A2C = 0x407f2020;
s32           D_L10_00161A30 = 0x107f2020;
f32           D_L10_00161A40[1] = {-2.0f};

f32 D_L10_00161A50[1] = {4.0f};

f32 D_L10_00161A60[1] = {-0.25f};

f32           D_L10_001619A0 = 0.06981317f;
f32           D_L10_001619C4 = 0.25f;
Moby         *D_L10_0015F63C = 0;
unsigned char D_L10_001619A4 = 4;
float         D_L10_001619A8 = 1e+04f;
float         D_L10_001619AC = 6e+04f;
float         D_L10_001619BC = 0.034906585f;
float         D_L10_001619C0 = 0.06f;
int           D_L10_001619EC = 0x12c;
int           D_L10_001619F4 = 0x14;
int           D_L10_001619F8 = 0x50;
float         D_L10_00161A0C = 0.15f;
int           D_L10_00161AC0 = 0;
int           D_L10_00161A70 = 0;
int           D_L10_00161A74 = 1;
int           D_L10_00161A78 = 0;
int           D_L10_00161A7C = 1;
int           D_L10_00161A80 = 0x10;
int           D_L10_00161A84 = 0x15;
u32           D_L10_00161A88 = 0x50804040;
float         D_L10_00161A8C = 0.3f;
u32           D_L10_00161A90 = 0x802020;
int           D_L10_00161A94 = 0x15;
float         D_L10_00161A98 = 2.0f;
float         D_L10_00161A9C = 1.125f;
float         D_L11_00161C80 = 1.0f;
float         D_L11_00161C84 = 2.0f;
unsigned char D_L11_00161C88 = 0x3c;
float         D_L11_00161C94 = 0.4f;
float         D_L11_00161C98 = 0.2f;
s32           D_L11_00161C78 = 0x5a;
s32           D_L11_00161C9C = 0x407f2020;
s32           D_L11_00161CA0 = 0x107f2020;
f32           D_L11_00161CB0[1] = {-2.0f};

f32 D_L11_00161CC0[1] = {4.0f};

f32 D_L11_00161CD0[1] = {-0.25f};

f32           D_L11_00161C10 = 0.06981317f;
f32           D_L11_00161C34 = 0.25f;
Moby         *D_L11_0015F63C = 0;
unsigned char D_L11_00161C14 = 4;
float         D_L11_00161C18 = 1e+04f;
float         D_L11_00161C1C = 6e+04f;
float         D_L11_00161C2C = 0.034906585f;
float         D_L11_00161C30 = 0.06f;
int           D_L11_00161C5C = 0x12c;
int           D_L11_00161C64 = 0x14;
int           D_L11_00161C68 = 0x50;
float         D_L11_00161C7C = 0.15f;
int           D_L11_00161D30 = 0;
int           D_L11_00161CE0 = 0;
int           D_L11_00161CE4 = 1;
int           D_L11_00161CE8 = 0;
int           D_L11_00161CEC = 1;
int           D_L11_00161CF0 = 0x10;
int           D_L11_00161CF4 = 0x15;
u32           D_L11_00161CF8 = 0x50804040;
float         D_L11_00161CFC = 0.3f;
u32           D_L11_00161D00 = 0x802020;
int           D_L11_00161D04 = 0x15;
float         D_L11_00161D08 = 2.0f;
float         D_L11_00161D0C = 1.125f;
float         D_L12_00161BB0 = 1.0f;
float         D_L12_00161BB4 = 2.0f;
unsigned char D_L12_00161BB8 = 0x3c;
float         D_L12_00161BC4 = 0.4f;
float         D_L12_00161BC8 = 0.2f;
s32           D_L12_00161BA8 = 0x5a;
s32           D_L12_00161BCC = 0x407f2020;
s32           D_L12_00161BD0 = 0x107f2020;
f32           D_L12_00161BE0[1] = {-2.0f};

f32 D_L12_00161BF0[1] = {4.0f};

f32 D_L12_00161C00[1] = {-0.25f};

f32           D_L12_00161B40 = 0.06981317f;
f32           D_L12_00161B64 = 0.25f;
Moby         *D_L12_0015F63C = 0;
unsigned char D_L12_00161B44 = 4;
float         D_L12_00161B48 = 1e+04f;
float         D_L12_00161B4C = 6e+04f;
float         D_L12_00161B5C = 0.034906585f;
float         D_L12_00161B60 = 0.06f;
int           D_L12_00161B8C = 0x12c;
int           D_L12_00161B94 = 0x14;
int           D_L12_00161B98 = 0x50;
float         D_L12_00161BAC = 0.15f;
int           D_L12_00161C60 = 0;
int           D_L12_00161C10 = 0;
int           D_L12_00161C14 = 1;
int           D_L12_00161C18 = 0;
int           D_L12_00161C1C = 1;
int           D_L12_00161C20 = 0x10;
int           D_L12_00161C24 = 0x15;
u32           D_L12_00161C28 = 0x50804040;
float         D_L12_00161C2C = 0.3f;
u32           D_L12_00161C30 = 0x802020;
int           D_L12_00161C34 = 0x15;
float         D_L12_00161C38 = 2.0f;
float         D_L12_00161C3C = 1.125f;
float         D_L13_00161BF0 = 1.0f;
float         D_L13_00161BF4 = 2.0f;
unsigned char D_L13_00161BF8 = 0x3c;
float         D_L13_00161C04 = 0.4f;
float         D_L13_00161C08 = 0.2f;
s32           D_L13_00161BE8 = 0x5a;
s32           D_L13_00161C0C = 0x407f2020;
s32           D_L13_00161C10 = 0x107f2020;
f32           D_L13_00161C20[1] = {-2.0f};

f32 D_L13_00161C30[1] = {4.0f};

f32 D_L13_00161C40[1] = {-0.25f};

f32           D_L13_00161B80 = 0.06981317f;
f32           D_L13_00161BA4 = 0.25f;
Moby         *D_L13_0015F63C = 0;
unsigned char D_L13_00161B84 = 4;
float         D_L13_00161B88 = 1e+04f;
float         D_L13_00161B8C = 6e+04f;
float         D_L13_00161B9C = 0.034906585f;
float         D_L13_00161BA0 = 0.06f;
int           D_L13_00161BCC = 0x12c;
int           D_L13_00161BD4 = 0x14;
int           D_L13_00161BD8 = 0x50;
float         D_L13_00161BEC = 0.15f;
int           D_L13_00161CA0 = 0;
int           D_L13_00161C50 = 0;
int           D_L13_00161C54 = 1;
int           D_L13_00161C58 = 0;
int           D_L13_00161C5C = 1;
int           D_L13_00161C60 = 0x10;
int           D_L13_00161C64 = 0x15;
u32           D_L13_00161C68 = 0x50804040;
float         D_L13_00161C6C = 0.3f;
u32           D_L13_00161C70 = 0x802020;
int           D_L13_00161C74 = 0x15;
float         D_L13_00161C78 = 2.0f;
float         D_L13_00161C7C = 1.125f;
float         D_L14_00161DF0 = 1.0f;
float         D_L14_00161DF4 = 2.0f;
unsigned char D_L14_00161DF8 = 0x3c;
float         D_L14_00161E04 = 0.4f;
float         D_L14_00161E08 = 0.2f;
s32           D_L14_00161DE8 = 0x5a;
s32           D_L14_00161E0C = 0x407f2020;
s32           D_L14_00161E10 = 0x107f2020;
f32           D_L14_00161E20[1] = {-2.0f};

f32 D_L14_00161E30[1] = {4.0f};

f32 D_L14_00161E40[1] = {-0.25f};

f32           D_L14_00161D80 = 0.06981317f;
f32           D_L14_00161DA4 = 0.25f;
Moby         *D_L14_0015F63C = 0;
unsigned char D_L14_00161D84 = 4;
float         D_L14_00161D88 = 1e+04f;
float         D_L14_00161D8C = 6e+04f;
float         D_L14_00161D9C = 0.034906585f;
float         D_L14_00161DA0 = 0.06f;
int           D_L14_00161DCC = 0x12c;
int           D_L14_00161DD4 = 0x14;
int           D_L14_00161DD8 = 0x50;
float         D_L14_00161DEC = 0.15f;
int           D_L14_00161EA0 = 0;
int           D_L14_00161E50 = 0;
int           D_L14_00161E54 = 1;
int           D_L14_00161E58 = 0;
int           D_L14_00161E5C = 1;
int           D_L14_00161E60 = 0x10;
int           D_L14_00161E64 = 0x15;
u32           D_L14_00161E68 = 0x50804040;
float         D_L14_00161E6C = 0.3f;
u32           D_L14_00161E70 = 0x802020;
int           D_L14_00161E74 = 0x15;
float         D_L14_00161E78 = 2.0f;
float         D_L14_00161E7C = 1.125f;
float         D_L15_00161D70 = 1.0f;
float         D_L15_00161D74 = 2.0f;
unsigned char D_L15_00161D78 = 0x3c;
float         D_L15_00161D84 = 0.4f;
float         D_L15_00161D88 = 0.2f;
s32           D_L15_00161D68 = 0x5a;
s32           D_L15_00161D8C = 0x407f2020;
s32           D_L15_00161D90 = 0x107f2020;
f32           D_L15_00161DA0[1] = {-2.0f};

f32 D_L15_00161DB0[1] = {4.0f};

f32 D_L15_00161DC0[1] = {-0.25f};

f32           D_L15_00161D00 = 0.06981317f;
f32           D_L15_00161D24 = 0.25f;
Moby         *D_L15_0015F63C = 0;
unsigned char D_L15_00161D04 = 4;
float         D_L15_00161D08 = 1e+04f;
float         D_L15_00161D0C = 6e+04f;
float         D_L15_00161D1C = 0.034906585f;
float         D_L15_00161D20 = 0.06f;
int           D_L15_00161D4C = 0x12c;
int           D_L15_00161D54 = 0x14;
int           D_L15_00161D58 = 0x50;
float         D_L15_00161D6C = 0.15f;
int           D_L15_00161E20 = 0;
int           D_L15_00161DD0 = 0;
int           D_L15_00161DD4 = 1;
int           D_L15_00161DD8 = 0;
int           D_L15_00161DDC = 1;
int           D_L15_00161DE0 = 0x10;
int           D_L15_00161DE4 = 0x15;
u32           D_L15_00161DE8 = 0x50804040;
float         D_L15_00161DEC = 0.3f;
u32           D_L15_00161DF0 = 0x802020;
int           D_L15_00161DF4 = 0x15;
float         D_L15_00161DF8 = 2.0f;
float         D_L15_00161DFC = 1.125f;
float         D_L16_00161B10 = 1.0f;
float         D_L16_00161B14 = 2.0f;
unsigned char D_L16_00161B18 = 0x3c;
float         D_L16_00161B24 = 0.4f;
float         D_L16_00161B28 = 0.2f;
s32           D_L16_00161B08 = 0x5a;
s32           D_L16_00161B2C = 0x407f2020;
s32           D_L16_00161B30 = 0x107f2020;
f32           D_L16_00161B40[1] = {-2.0f};

f32 D_L16_00161B50[1] = {4.0f};

f32 D_L16_00161B60[1] = {-0.25f};

f32           D_L16_00161AA0 = 0.06981317f;
f32           D_L16_00161AC4 = 0.25f;
Moby         *D_L16_0015F63C = 0;
unsigned char D_L16_00161AA4 = 4;
float         D_L16_00161AA8 = 1e+04f;
float         D_L16_00161AAC = 6e+04f;
float         D_L16_00161ABC = 0.034906585f;
float         D_L16_00161AC0 = 0.06f;
int           D_L16_00161AEC = 0x12c;
int           D_L16_00161AF4 = 0x14;
int           D_L16_00161AF8 = 0x50;
float         D_L16_00161B0C = 0.15f;
int           D_L16_00161BC0 = 0;
int           D_L16_00161B70 = 0;
int           D_L16_00161B74 = 1;
int           D_L16_00161B78 = 0;
int           D_L16_00161B7C = 1;
int           D_L16_00161B80 = 0x10;
int           D_L16_00161B84 = 0x15;
u32           D_L16_00161B88 = 0x50804040;
float         D_L16_00161B8C = 0.3f;
u32           D_L16_00161B90 = 0x802020;
int           D_L16_00161B94 = 0x15;
float         D_L16_00161B98 = 2.0f;
float         D_L16_00161B9C = 1.125f;
float         D_L17_00161C60 = 1.0f;
float         D_L17_00161C64 = 2.0f;
unsigned char D_L17_00161C68 = 0x3c;
float         D_L17_00161C74 = 0.4f;
float         D_L17_00161C78 = 0.2f;
s32           D_L17_00161C58 = 0x5a;
s32           D_L17_00161C7C = 0x407f2020;
s32           D_L17_00161C80 = 0x107f2020;
f32           D_L17_00161C90[1] = {-2.0f};

f32 D_L17_00161CA0[1] = {4.0f};

f32 D_L17_00161CB0[1] = {-0.25f};

f32           D_L17_00161BF0 = 0.06981317f;
f32           D_L17_00161C14 = 0.25f;
Moby         *D_L17_0015F63C = 0;
unsigned char D_L17_00161BF4 = 4;
float         D_L17_00161BF8 = 1e+04f;
float         D_L17_00161BFC = 6e+04f;
float         D_L17_00161C0C = 0.034906585f;
float         D_L17_00161C10 = 0.06f;
int           D_L17_00161C3C = 0x12c;
int           D_L17_00161C44 = 0x14;
int           D_L17_00161C48 = 0x50;
float         D_L17_00161C5C = 0.15f;
int           D_L17_00161D10 = 0;
int           D_L17_00161CC0 = 0;
int           D_L17_00161CC4 = 1;
int           D_L17_00161CC8 = 0;
int           D_L17_00161CCC = 1;
int           D_L17_00161CD0 = 0x10;
int           D_L17_00161CD4 = 0x15;
u32           D_L17_00161CD8 = 0x50804040;
float         D_L17_00161CDC = 0.3f;
u32           D_L17_00161CE0 = 0x802020;
int           D_L17_00161CE4 = 0x15;
float         D_L17_00161CE8 = 2.0f;
float         D_L17_00161CEC = 1.125f;
float         D_L18_00161DA0 = 1.0f;
float         D_L18_00161DA4 = 2.0f;
unsigned char D_L18_00161DA8 = 0x3c;
float         D_L18_00161DB4 = 0.4f;
float         D_L18_00161DB8 = 0.2f;
s32           D_L18_00161D98 = 0x5a;
s32           D_L18_00161DBC = 0x407f2020;
s32           D_L18_00161DC0 = 0x107f2020;
f32           D_L18_00161DD0[1] = {-2.0f};

f32 D_L18_00161DE0[1] = {4.0f};

f32 D_L18_00161DF0[1] = {-0.25f};

f32           D_L18_00161D30 = 0.06981317f;
f32           D_L18_00161D54 = 0.25f;
Moby         *D_L18_0015F63C = 0;
unsigned char D_L18_00161D34 = 4;
float         D_L18_00161D38 = 1e+04f;
float         D_L18_00161D3C = 6e+04f;
float         D_L18_00161D4C = 0.034906585f;
float         D_L18_00161D50 = 0.06f;
int           D_L18_00161D7C = 0x12c;
int           D_L18_00161D84 = 0x14;
int           D_L18_00161D88 = 0x50;
float         D_L18_00161D9C = 0.15f;
int           D_L18_00161E50 = 0;
int           D_L18_00161E00 = 0;
int           D_L18_00161E04 = 1;
int           D_L18_00161E08 = 0;
int           D_L18_00161E0C = 1;
int           D_L18_00161E10 = 0x10;
int           D_L18_00161E14 = 0x15;
u32           D_L18_00161E18 = 0x50804040;
float         D_L18_00161E1C = 0.3f;
u32           D_L18_00161E20 = 0x802020;
int           D_L18_00161E24 = 0x15;
float         D_L18_00161E28 = 2.0f;
float         D_L18_00161E2C = 1.125f;

/* True when the moby state byte is 1. */
int is_moby_in_state_one(unsigned char *moby) __asm__("FUN_L00_002d8128");

int is_moby_in_state_one(unsigned char *moby) {
    return moby[0x20] == 1;
}

/* Creates a class 0x326 moby linked to p at p's position; 0 when p's state is negative. */
O2d8138 *create_linked_class_0326_moby(P2d8138 *p) __asm__("FUN_L00_002d8138");

O2d8138 *create_linked_class_0326_moby(P2d8138 *p) {
    O2d8138 *spawned_moby;
    if (p->b20 < 0)
        return 0;
    spawned_moby = create_moby(0x326);
    if (spawned_moby) {
        spawned_moby->b30 = 0x40;
        spawned_moby->b20 = 0;
        spawned_moby->t78->p = p;
        spawned_moby->f94 = 0;
        spawned_moby->h34 |= 0x41;
        qcopy(&spawned_moby->q, &p->q);
    }
    return spawned_moby;
}

void update_nanotech_ball_grounding_and_fall(unsigned char *moby) __asm__("FUN_L00_002d81b8");

void update_nanotech_ball_grounding_and_fall(unsigned char *moby) {
    V_2d81b8  probe_start_position, probe_end_position;
    D_2d81b8 *moby_data = ((D_2d81b8 *)MOBY(moby)->pvars);
    int       hit = 0;
    float     z, f;
    qcopy(&probe_start_position, moby + 0x10);
    probe_start_position.f[2] += 0.5f;
    qcopy(&probe_end_position, &probe_start_position);
    probe_end_position.f[2] -= 1.5f;
    if (FUN_001efa68_2d81b8(&probe_start_position, &probe_end_position, 2, moby, 0)) {
        z = collision_hit_z;
        hit = 1;
    } else {
        z = probe_end_position.f[2];
    }
    if (hit) {
        G_2d81b8 *collision_result = &(*(G_2d81b8 *)D_L00_00173E40_lui);
        if (collision_result->o) {
            if (is_moby_in_class_range_500_to_540(collision_result->o)) {
                MOBY(moby)->pos.x = moby_data->f10 = collision_result->o->x;
                MOBY(moby)->pos.y = moby_data->f14 = collision_result->o->y;
                {
                    float t = collision_result->f28;
                    MOBY(moby)->pos.z = t;
                    moby_data->f18 = t + 0.5f;
                }
            } else if (moby_data->hA) {
                MOBY(moby)->pos.x = moby_data->f10 = collision_result->o->x + moby_data->f70;
                MOBY(moby)->pos.y = moby_data->f14 = collision_result->o->y + moby_data->f74;
                {
                    float t = collision_result->o->z + moby_data->f78;
                    MOBY(moby)->pos.z = t;
                    moby_data->f18 = t + 0.5f;
                }
            } else {
                moby_data->hA = 1;
                moby_data->f70 = MOBY(moby)->pos.x - collision_result->o->x;
                moby_data->f74 = MOBY(moby)->pos.y - collision_result->o->y;
                moby_data->f78 = MOBY(moby)->pos.z - collision_result->o->z;
            }
        }
    }
    f = MOBY(moby)->pos.z;
    if (f <= z) {
        moby[0xBC] = 0;
        if (moby_data->hA)
            moby_data->f78 += z - f;
        MOBY(moby)->pos.z = z;
        *(int *)&moby_data->f50 = 0;
    } else {
        float capped_fall_step, acc;
        moby[0xBC] = 1;
        acc = frame_time_sq * 9.8f;
        MOBY(moby)->pos.z = f - moby_data->f50;
        moby_data->f50 += acc;
        if (0.3f < moby_data->f50)
            capped_fall_step = 0.3f;
        else
            capped_fall_step = moby_data->f50;
        moby_data->f50 = capped_fall_step;
        if (moby_data->hA)
            moby_data->f78 -= capped_fall_step;
    }
}

void spawn_nanotech_ball_collection_effect_pair(char *moby) __asm__("FUN_L00_002d8418");

void spawn_nanotech_ball_collection_effect_pair(char *moby) {
    Vec4_002d8418 to_camera;
    Vec4_002d8418 scaled_gravity_vector;
    Vec4_002d8418 effect_vector;
    char         *moby_position = moby + 0x10;
    fast_vec_sub(&to_camera, g_debug_camera, moby_position);
    fast_vec_scale(
        &scaled_gravity_vector,
        &hero.gravity_dir,
        random_float_between(nanotech_ball_collection_gravity_scale_range_start_gp * frame_time,
                             nanotech_ball_collection_gravity_scale_range_end_gp * frame_time));
    FUN_00214890_002d8418(
        &effect_vector, &scaled_gravity_vector, &to_camera, random_angle_radians());
    spawn_moving_hero_relative_alpha_fade_particle(
        moby_position,
        &effect_vector,
        nanotech_ball_collection_first_particle_color,
        l00_nanotech_ball_collection_particle_lifetime_frames,
        random_integer_below(0xFF) & 0xFF,
        1,
        nanotech_ball_collection_first_particle_scale_factor_gp);
    spawn_moving_hero_relative_alpha_fade_particle(
        moby_position,
        &effect_vector,
        nanotech_ball_collection_second_particle_color,
        l00_nanotech_ball_collection_particle_lifetime_frames,
        random_integer_below(0xFF) & 0xFF,
        1,
        nanotech_ball_collection_second_particle_scale_factor_gp);
}

s32 update_nanotech_ball_collection_effects(M_2d8510 *moby) __asm__("FUN_L00_002d8510");

s32 update_nanotech_ball_collection_effects(M_2d8510 *moby) {
    O_2d8510 *effect_data;
    V_2d8510  effect_position, effect_position_step;
    s32       i, any_effect_timer_above_threshold, c, hi, al;
    f32       z, color_progress_or_size_factor, unwrapped_effect_value;
    u8       *effect_record;
    effect_data = moby->p78;
    fast_vec_sub(&effect_position, &(*(V_2d8510 *)g_debug_camera), &moby->v10);
    any_effect_timer_above_threshold = 0;
    fast_vec_normalize(&effect_position, &effect_position, -0.3f);
    fast_vec_normalize(&effect_position_step, &effect_position, 0.1f);
    fast_vec_add(&effect_position, &effect_position, &moby->v10);
    z = effect_position.z;
    for (i = 0; i < 4; i++) {
        if (fast_dec_timer_s16(&effect_data->h58[i]))
            continue;
        if (effect_data->h58[i] > FUN_001f96f8_2d8510(0x30))
            any_effect_timer_above_threshold = 1;
        unwrapped_effect_value = effect_data->f60[i] +=
            nanotech_ball_collection_particle_rotation_steps[i];
        if (255.0f <= unwrapped_effect_value)
            effect_data->f60[i] = unwrapped_effect_value - 255.0f;
        else if (unwrapped_effect_value <= 0.0f)
            effect_data->f60[i] = unwrapped_effect_value + 255.0f;
        color_progress_or_size_factor =
            convert_integer_to_float(FUN_001f96f8_2d8510(0xFF) - effect_data->h58[i]);
        color_progress_or_size_factor =
            color_progress_or_size_factor / (f32)FUN_001f96f8_2d8510(0xFF);
        c = FUN_001fa6e0_2d8510(nanotech_ball_collection_color_range_start_gp,
                                nanotech_ball_collection_color_range_end_gp,
                                absolute_float(0.5f - color_progress_or_size_factor));
        hi = c >> 24;
        color_progress_or_size_factor =
            1.0f -
            effect_data->h58[i] / convert_integer_to_float(FUN_001f96f8_2d8510(D_L00_00161A58_gp));
        al = truncate_float_to_s32(convert_integer_to_float(hi) *
                                   (1.0f - color_progress_or_size_factor))
             << 24;
        c = al | (c & 0xFFFFFF);
        effect_position.z = z + nanotech_ball_collection_particle_z_offsets[i];
        effect_record = FUN_L00_00272f68_2d8510(&effect_position,
                                                c,
                                                truncate_float_to_s32(effect_data->f60[i]),
                                                nanotech_ball_collection_particle_byte_source_index,
                                                0,
                                                2,
                                                1,
                                                nanotech_ball_collection_particle_scale_factors[i] *
                                                    color_progress_or_size_factor);
        if (effect_record)
            effect_record[2] = *D_L00_001B2080_lui.pF8;
        effect_record =
            FUN_L00_00272f68_2d8510(&effect_position,
                                    al | 0xFFFFFF,
                                    truncate_float_to_s32(effect_data->f60[i]),
                                    nanotech_ball_collection_particle_byte_source_index,
                                    nanotech_ball_collection_second_particle_mode_selector,
                                    2,
                                    1,
                                    nanotech_ball_collection_particle_scale_factors[i] *
                                        (color_progress_or_size_factor * 0.7f));
        if (effect_record)
            effect_record[2] = *D_L00_001B2080_lui.pF8;
        fast_vec_add(&effect_position, &effect_position, &effect_position_step);
    }
    return any_effect_timer_above_threshold;
}

void compute_nanotech_ball_slot_position(unsigned char *moby,
                                         float         *out,
                                         int            i) __asm__("FUN_L00_002d8850");

void compute_nanotech_ball_slot_position(unsigned char *moby, float *out_slot_position, int i) {
    u128        slot_offset;
    M_002d8850 *moby_data = ((M_002d8850 *)MOBY(moby)->pvars);
    char       *e;
    build_look_at_matrix(
        &slot_offset, &D_L00_001E3CA0_002d8850[i], &moby_data->p20, moby_data->p20.f[3]);
    e = moby_data->e[i];
    if (e && moby[0x20] == 2)
        FUN_001f9bf8_002d8850(&slot_offset, &slot_offset, *(float *)(e + 0x28));
    fast_vec_add(out_slot_position, &moby_data->p10, &slot_offset);
    out_slot_position[2] += moby_data->f0_002d8850;
}

/* Rebuilds the eight D_L00_001E3CA0 entries with build_look_at_matrix and normalizes them. */
void update_nanotech_ball_slot_offsets(void) __asm__("FUN_L00_002d8908");

void update_nanotech_ball_slot_offsets(void) {
    s32 i;
    for (i = 0; i < 8; i++) {
        build_look_at_matrix(&D_L00_001E3CA0[i],
                             &D_L00_001E3CA0[i],
                             &D_L00_001E3D20[i],
                             nanotech_ball_slot_offset_transform_amount_gp);
        fast_vec_normalize(&D_L00_001E3CA0[i], &D_L00_001E3CA0[i], D_L00_00161A14_gp);
    }
}

/* True when coll_line_fix returns 0 for the moby and (*(char * *)&D_001413D0_lui) positions, both raised by 0.5. */
int is_nanotech_ball_target_line_clear(char *moby) __asm__("FUN_L00_002d8980");

int is_nanotech_ball_target_line_clear(char *moby) {
    Vec4 raised_moby_position, raised_target_position;
    qcopy(&raised_moby_position, moby + 0x10);
    raised_moby_position.f[2] += 0.5f;
    qcopy(&raised_target_position, (*(char **)&D_001413D0_lui) + 0x10);
    raised_target_position.f[2] += 0.5f;
    return collision_line(&raised_moby_position, &raised_target_position, 0x12, 0, 0) == 0;
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9438.c: func_L00_002D9EA0), where it is exact; names translated to the US level program. */
void nanotech_ball_update(Moby *moby) __asm__("FUN_L00_002d89f0");

/* Moby class 806 */
void nanotech_ball_update(Moby *moby) {
    enum {
        NANOTECH_BALL_STATE_INITIALIZE = 0,
        NANOTECH_BALL_STATE_FOLLOW_OWNER = 1,
        NANOTECH_BALL_STATE_DETACHED_PICKUP = 2,
        NANOTECH_BALL_STATE_COLLECTING = 3,
        NANOTECH_BALL_STATE_BASIS_ONLY = 4
    };
    V_2D9EA0 slot_rotation_basis_or_cull_position_or_start_tangent,
        rotated_slot_offset_or_particle_position, v20, collection_target_or_end_tangent,
        cross_product_or_particle_midpoint, half_displacement_or_spark_position, slot_curve_offset;
    D_2D9EA0 *nanotech_ball_state = (D_2D9EA0 *)moby->pvars;

    switch (moby->state) {
    case NANOTECH_BALL_STATE_INITIALIZE: {
        Moby *owner;
        int   i;
        moby->state = 1;
        moby->flags |= 0x41;
        moby->collision = 0;
        moby->command = 0;
        owner = nanotech_ball_state->owner;
        qcopy(&moby->pos, &owner->pos);
        qcopy(&moby->rot, &owner->rot);
        qcopy(&nanotech_ball_state->pos, &moby->pos);
        *(int *)&nanotech_ball_state->bob_offset = 0;
        nanotech_ball_state->h8 = 0;
        nanotech_ball_state->pos.z += 0.5f;
        nanotech_ball_state->bob_phase =
            random_float_between(nanotech_ball_state->bob_offset, 3.1415927f);
        nanotech_ball_state->h56 = 0;
        nanotech_ball_state->hA = 0;
        for (i = 0; i < 4; i++)
            nanotech_ball_state->f60[i] = (float)random_integer_below(0xFF);
        for (i = 0; i < 4; i++)
            nanotech_ball_state->h58[i] = func_001F9850_2D9EA0(D_L00_00161A58_gp);
        for (i = 7; i >= 0; i--)
            nanotech_ball_state->slots[i] = 0;
        if (D_L00_0015F63C == 0) {
            euler_rotation_basis(&moby->rot_row0, &moby->rot);
            D_L00_0015F63C = moby;
            moby->update_dist = 0xFF;
            nanotech_ball_state->f7C = 0;
            func_L00_001FF4B0_2D9EA0(&D_L00_001E3CA0[0], &moby->rot_row0, D_L00_00161A14_gp);
            qcopy(&D_L00_001E4070_2D9EA0[0], &moby->rot_row2);
            func_L00_001FF4B0_2D9EA0(&D_L00_001E3CA0[1], &moby->rot_row1, D_L00_00161A14_gp);
            qcopy(&D_L00_001E4070_2D9EA0[1], &moby->rot_row0);
            func_L00_001FF4B0_2D9EA0(&D_L00_001E3CA0[2], &moby->rot_row2, D_L00_00161A14_gp);
            qcopy(&D_L00_001E4070_2D9EA0[2], &moby->rot_row1);
            func_001F9BD8_2D9EA0(&D_L00_001E3CA0[3], &moby->rot_row0, &moby->rot_row2);
            func_L00_001FF4B0_2D9EA0(&D_L00_001E3CA0[3], &D_L00_001E3CA0[3], D_L00_00161A14_gp);
            fast_vec_cross(&D_L00_001E4070_2D9EA0[3], &D_L00_001E3CA0[3], &moby->rot_row1);
            func_001F9BD8_2D9EA0(&D_L00_001E3CA0[4], &moby->rot_row0, &moby->rot_row1);
            func_L00_001FF4B0_2D9EA0(&D_L00_001E3CA0[4], &D_L00_001E3CA0[4], D_L00_00161A14_gp);
            fast_vec_cross(&D_L00_001E4070_2D9EA0[4], &D_L00_001E3CA0[4], &moby->rot_row2);
            func_001F9BF0_2D9EA0(&D_L00_001E3CA0[5], &moby->rot_row1, &moby->rot_row2);
            func_L00_001FF4B0_2D9EA0(&D_L00_001E3CA0[5], &D_L00_001E3CA0[5], D_L00_00161A14_gp);
            fast_vec_cross(&D_L00_001E4070_2D9EA0[5], &D_L00_001E3CA0[5], &moby->rot_row0);
            func_001F9BF0_2D9EA0(&D_L00_001E3CA0[6], &moby->rot_row0, &moby->rot_row2);
            func_L00_001FF4B0_2D9EA0(&D_L00_001E3CA0[6], &D_L00_001E3CA0[6], D_L00_00161A14_gp);
            fast_vec_cross(&D_L00_001E4070_2D9EA0[6], &D_L00_001E3CA0[6], &moby->rot_row1);
            func_001F9BF0_2D9EA0(&D_L00_001E3CA0[7], &moby->rot_row0, &moby->rot_row1);
            func_L00_001FF4B0_2D9EA0(&D_L00_001E3CA0[7], &D_L00_001E3CA0[7], D_L00_00161A14_gp);
            fast_vec_cross(&D_L00_001E4070_2D9EA0[7], &D_L00_001E3CA0[7], &moby->rot_row2);
        }
        {
            float yaw = random_float_between(0.0f, 6.2831855f);
            build_spherical_offset(&nanotech_ball_state->v20,
                                   1.0f,
                                   yaw,
                                   random_float_between(-1.5707964f, 1.5707964f));
        }
        nanotech_ball_state->v20.w = randf_sym(0.785398006f, 1.5707964f);
    }
    case NANOTECH_BALL_STATE_FOLLOW_OWNER: {
        Moby *owner = nanotech_ball_state->owner;
        qcopy(&moby->pos, &owner->pos);
        qcopy(&nanotech_ball_state->pos, &owner->pos);
        nanotech_ball_state->pos.z += 0.5f;
        if ((signed char)owner->state < 0 || owner == moby) {
            moby->state = 2;
            nanotech_ball_state->f50 = 0;
        }
    }
    case NANOTECH_BALL_STATE_DETACHED_PICKUP:
        if (moby->state == 2) {
            int i;
            qcopy(&nanotech_ball_state->pos, &moby->pos);
            nanotech_ball_state->pos.z += 0.5f;
            func_L00_002D9668_2D9EA0(moby);
            for (i = 0; i < 8; i++) {
                if (nanotech_ball_state->slots[i] != 0) {
                    Ext *ext = (Ext *)(nanotech_ball_state->slots[i] + 1);
                    ext->u.s.scale = nanotech_ball_slot_scale_pulse_amplitude_gp *
                                         func_001F9FA8_2D9EA0(ext->u.s.ang) +
                                     D_L00_00161A14_gp;
                    ext->u.s.ang = fast_add_rotations(ext->u.s.ang, D_L00_001E17F0_2D9EA0[i]);
                }
            }
        }
        {
            Hero *player = &hero;
            int   max_hp = hero_max_health;
            if (player->vehicle_mode == 1)
                max_hp = 4;
            if (moby->state != 1 && D_L00_0015F63C != 0 &&
                ((D_2D9EA0 *)D_L00_0015F63C->pvars) != 0 && player->health.hp != 0 &&
                (player->health.hp + ((D_2D9EA0 *)D_L00_0015F63C->pvars)->f7C < max_hp ||
                 (nanotech_ball_invulnerability_pickup_enabled != 0 &&
                  player->invulnerable_frames == 0)) &&
                fast_vec_dist(&moby->pos, &player->motion.pos) < 10.0f &&
                func_L00_002D9E30_2D9EA0(moby)) {
                int i;
                moby->state = 3;
                ((D_2D9EA0 *)D_L00_0015F63C->pvars)->f7C++;
                if (nanotech_ball_invulnerability_pickup_enabled != 0)
                    player->invulnerable_frames = func_001F9850_2D9EA0(0x258);
                nanotech_ball_state->h54 =
                    func_001F9850_2D9EA0(nanotech_ball_collection_lifetime_frames_gp);
                func_001F9C30_2D9EA0(&collection_target_or_end_tangent,
                                     &player->gravity_dir,
                                     -nanotech_ball_collection_target_gravity_offset_gp);
                func_001F9BD8_2D9EA0(&collection_target_or_end_tangent,
                                     &player->motion.pos,
                                     &collection_target_or_end_tangent);
                func_001F9C30_2D9EA0(&slot_rotation_basis_or_cull_position_or_start_tangent,
                                     &player->gravity_dir,
                                     -nanotech_ball_collection_curve_radius_gp);
                func_001F9BF0_2D9EA0(&v20, &collection_target_or_end_tangent, &moby->pos);
                v20.z -= 0.5f;
                fast_vec_cross(&cross_product_or_particle_midpoint,
                               &v20,
                               &slot_rotation_basis_or_cull_position_or_start_tangent);
                fast_vec_cross(&slot_rotation_basis_or_cull_position_or_start_tangent,
                               &cross_product_or_particle_midpoint,
                               &v20);
                func_002156E0_2D9EA0(&slot_rotation_basis_or_cull_position_or_start_tangent,
                                     &slot_rotation_basis_or_cull_position_or_start_tangent,
                                     -1.5707964f,
                                     &v20);
                func_L00_001FF4B0_2D9EA0(&slot_rotation_basis_or_cull_position_or_start_tangent,
                                         &slot_rotation_basis_or_cull_position_or_start_tangent,
                                         nanotech_ball_collection_curve_radius_gp);
                func_001F9C30_2D9EA0(&half_displacement_or_spark_position, &v20, 0.5f);
                for (i = 0; i < 8; i++) {
                    if (nanotech_ball_state->slots[i] != 0) {
                        Ext *ext = (Ext *)(nanotech_ball_state->slots[i] + 1);
                        func_002156E0_2D9EA0(&rotated_slot_offset_or_particle_position,
                                             &slot_rotation_basis_or_cull_position_or_start_tangent,
                                             (float)i * 0.448549628f,
                                             &v20);
                        func_001F9BD8_2D9EA0(&slot_curve_offset,
                                             &half_displacement_or_spark_position,
                                             &rotated_slot_offset_or_particle_position);
                        {
                            V3 *dst = &ext->u.a;
                            dst->x = slot_curve_offset.x;
                            dst->y = slot_curve_offset.y;
                            dst->z = slot_curve_offset.z;
                        }
                        {
                            V3 *dst = &ext->b;
                            V3 *src = (V3 *)&nanotech_ball_state->slots[i]->pos;
                            dst->x = src->x;
                            dst->y = src->y;
                            dst->z = src->z;
                        }
                        ext->h1E = ext->h1C = func_001F9850_2D9EA0(
                            random_int_between(nanotech_ball_slot_collection_min_frames_gp,
                                               nanotech_ball_slot_collection_max_frames_gp));
                    }
                }
            }
        }
        nanotech_ball_state->bob_offset =
            l00_nanotech_ball_bob_amplitude * func_001F9FA8_2D9EA0(nanotech_ball_state->bob_phase);
        nanotech_ball_state->bob_phase =
            fast_add_rotations(nanotech_ball_state->bob_phase, nanotech_ball_bob_phase_step_gp);
        if (D_L00_0015F63C == moby)
            update_nanotech_ball_slot_offsets();
        qcopy(&slot_rotation_basis_or_cull_position_or_start_tangent, &moby->pos);
        slot_rotation_basis_or_cull_position_or_start_tangent.z += 0.5f;
        slot_rotation_basis_or_cull_position_or_start_tangent.w = 1.0f;
        if (fast_bsphere_check(&slot_rotation_basis_or_cull_position_or_start_tangent, 64.0f) >=
            0) {
            int i;
            int lifetime;
            add_draw_callback((void (*)(void))draw_nanotech_ball, (int)moby);
            if (10.0f < vector_distance_xy(&moby->pos, D_L00_00166DC0))
                lifetime = func_001F9850_2D9EA0(5);
            else
                lifetime = func_001F9850_2D9EA0(40);
            for (i = 0; i < 8; i++) {
                Part *part;
                Part *slot;
                func_L00_002D9D00_2D9EA0(moby, &rotated_slot_offset_or_particle_position, i);
                part = func_L00_002745A8_2D9EA0(
                    &rotated_slot_offset_or_particle_position, lifetime, 2);
                if (part != 0) {
                    Ext *ext = (Ext *)(part + 1);
                    ext->b.x = part->pos.x - moby->pos.x;
                    ext->b.y = part->pos.y - moby->pos.y;
                    ext->b.z = part->pos.z - moby->pos.z;
                    if (moby->state == 2)
                        ext->u.s.owner = moby;
                    else
                        ext->u.s.owner = nanotech_ball_state->owner;
                    part->f4 = nanotech_ball_particle_color_gp;
                    part->fC = D_L00_001619F8;
                }
                slot = nanotech_ball_state->slots[i];
                if (slot == 0) {
                    nanotech_ball_state->slots[i] =
                        func_L00_002745A8_2D9EA0(&rotated_slot_offset_or_particle_position, 0, 1);
                    if (nanotech_ball_state->slots[i] != 0) {
                        Ext *ext = (Ext *)(nanotech_ball_state->slots[i] + 1);
                        ext->u.s.scale = D_L00_00161A14_gp;
                        ext->u.s.ang = 0.0f;
                        nanotech_ball_state->slots[i]->fC = D_L00_001619FC;
                        nanotech_ball_state->slots[i]->f4 = nanotech_ball_particle_color_gp;
                    }
                } else {
                    qcopy(&slot->pos, &rotated_slot_offset_or_particle_position);
                    slot->b8 += D_L00_001619F4;
                }
            }
        }
        break;
    case NANOTECH_BALL_STATE_COLLECTING:
        if (D_L00_0015F63C == moby)
            update_nanotech_ball_slot_offsets();
        if (nanotech_ball_state->h56 != 0) {
            func_001F9C30_2D9EA0(
                &moby->pos, D_0013F5E0, -nanotech_ball_collection_target_gravity_offset_gp);
            func_001F9BD8_2D9EA0(&moby->pos, D_0013F5E0 - 0x210, &moby->pos);
            moby->rot.w = 0.52359879f;
            if (func_L00_002D99C0_2D9EA0(moby) != 0 && random_integer_below(5) == 0)
                func_L00_002D98C8_2D9EA0(moby);
        }
        {
            Hero *player = &hero;
            int   max_hp = hero_max_health;
            if (player->vehicle_mode == 1)
                max_hp = 4;
            if (player->health.hp == max_hp && player->invulnerable_frames != 0 &&
                nanotech_ball_invulnerability_pickup_enabled != 0) {
                int duration = func_001F9850_2D9EA0(
                    nanotech_ball_full_health_invulnerable_collection_min_frames_gp);
                if (nanotech_ball_state->h54 < duration) {
                    int i;
                    nanotech_ball_state->h54 = duration;
                    for (i = 0; i < 4; i++)
                        nanotech_ball_state->h58[i]++;
                }
            }
        }
        if (fast_dec_timer_s16(&nanotech_ball_state->h54)) {
            int i;
            for (i = 0; i < 8; i++) {
                if (nanotech_ball_state->slots[i] != 0) {
                    func_L00_002688A8_2D9EA0(nanotech_ball_state->slots[i]);
                    nanotech_ball_state->slots[i] = 0;
                }
            }
            if (D_L00_0015F63C == moby)
                moby->state = 4;
            else
                mark_moby_for_removal(moby);
        } else {
            int i;
            func_001F9C30_2D9EA0(
                &v20, D_0013F5E0, -nanotech_ball_collection_target_gravity_offset_gp);
            func_001F9BD8_2D9EA0(&v20, D_0013F5E0 - 0x210, &v20);
            for (i = 0; i < 8; i++) {
                Part *part = nanotech_ball_state->slots[i];
                Ext  *ext;
                if (part == 0)
                    continue;
                ext = (Ext *)(part + 1);
                if (fast_dec_timer_s16(&ext->h1E)) {
                    if (nanotech_ball_state->h56 == 0) {
                        Hero *player = &hero;
                        int   max_hp = hero_max_health;
                        if (player->vehicle_mode == 1)
                            max_hp = 4;
                        if (player->health.hp != 0) {
                            int new_hp = player->health.hp + 1;
                            player->health.hp = (max_hp < new_hp) ? max_hp : new_hp;
                        }
                        {
                            D_2D9EA0 *leader_data = (D_2D9EA0 *)D_L00_0015F63C->pvars;
                            leader_data->f7C--;
                            if (leader_data->f7C < 0)
                                leader_data->f7C = 0;
                        }
                        func_L00_0028EF68_2D9EA0(1, 0, moby, 0x1F5);
                        nanotech_ball_state->h56 = 1;
                    }
                    func_L00_002688A8_2D9EA0(part);
                    nanotech_ball_state->slots[i] = 0;
                } else {
                    float progress;
                    float remaining;
                    Part *spark;
                    qcopy(&cross_product_or_particle_midpoint, &part->pos);
                    remaining = convert_integer_to_float(ext->h1C);
                    remaining = (float)ext->h1E / remaining;
                    {
                        V3 *src = &ext->b;
                        rotated_slot_offset_or_particle_position.x = src->x;
                        rotated_slot_offset_or_particle_position.y = src->y;
                        rotated_slot_offset_or_particle_position.z = src->z;
                    }
                    rotated_slot_offset_or_particle_position.w = 0.0f;
                    progress = 1.0f - remaining;
                    slot_rotation_basis_or_cull_position_or_start_tangent.x =
                        rotated_slot_offset_or_particle_position.x - ext->u.a.x;
                    slot_rotation_basis_or_cull_position_or_start_tangent.y =
                        rotated_slot_offset_or_particle_position.y - ext->u.a.y;
                    slot_rotation_basis_or_cull_position_or_start_tangent.z =
                        rotated_slot_offset_or_particle_position.z - ext->u.a.z;
                    collection_target_or_end_tangent.x = v20.x - ext->u.a.x;
                    collection_target_or_end_tangent.y = v20.y - ext->u.a.y;
                    collection_target_or_end_tangent.z = v20.z - ext->u.a.z;
                    part->pos.x = cubic_hermite_scalar(
                        slot_rotation_basis_or_cull_position_or_start_tangent.x,
                        rotated_slot_offset_or_particle_position.x,
                        v20.x,
                        collection_target_or_end_tangent.x,
                        progress);
                    part->pos.y = cubic_hermite_scalar(
                        slot_rotation_basis_or_cull_position_or_start_tangent.y,
                        rotated_slot_offset_or_particle_position.y,
                        v20.y,
                        collection_target_or_end_tangent.y,
                        progress);
                    part->pos.z = cubic_hermite_scalar(
                        slot_rotation_basis_or_cull_position_or_start_tangent.z,
                        rotated_slot_offset_or_particle_position.z,
                        v20.z,
                        collection_target_or_end_tangent.z,
                        progress);
                    half_displacement_or_spark_position.x =
                        randf_sym(0.0f, 0.00999999978f) + part->pos.x;
                    half_displacement_or_spark_position.y =
                        randf_sym(0.0f, 0.00999999978f) + part->pos.y;
                    half_displacement_or_spark_position.z =
                        randf_sym(0.0f, 0.00999999978f) + part->pos.z;
                    spark = func_L00_002745A8_2D9EA0(
                        &half_displacement_or_spark_position, func_001F9850_2D9EA0(0x14), 0);
                    if (spark != 0) {
                        spark->f4 = nanotech_ball_particle_color_gp;
                        spark->fC = D_L00_001619F8;
                    }
                    func_001F9BD8_2D9EA0(&cross_product_or_particle_midpoint,
                                         &part->pos,
                                         &cross_product_or_particle_midpoint);
                    func_001F9C30_2D9EA0(&cross_product_or_particle_midpoint,
                                         &cross_product_or_particle_midpoint,
                                         0.5f);
                    half_displacement_or_spark_position.x =
                        randf_sym(0.0f, 0.00999999978f) + cross_product_or_particle_midpoint.x;
                    half_displacement_or_spark_position.y =
                        randf_sym(0.0f, 0.00999999978f) + cross_product_or_particle_midpoint.y;
                    half_displacement_or_spark_position.z =
                        randf_sym(0.0f, 0.00999999978f) + cross_product_or_particle_midpoint.z;
                    spark = func_L00_002745A8_2D9EA0(
                        &half_displacement_or_spark_position, func_001F9850_2D9EA0(0x14), 0);
                    if (spark != 0) {
                        spark->f4 = nanotech_ball_particle_color_gp;
                        spark->fC = D_L00_001619F8;
                    }
                }
            }
        }
        break;
    case NANOTECH_BALL_STATE_BASIS_ONLY:
        if (D_L00_0015F63C == moby)
            update_nanotech_ball_slot_offsets();
        break;
    }
}

void draw_nanotech_ball(Moby *moby) {
    GeometryQuad quad;
    float        basis[4] __attribute__((aligned(16)));
    float       *forward;
    float        side[4] __attribute__((aligned(16)));
    float        up[4] __attribute__((aligned(16)));
    float        center[4] __attribute__((aligned(16)));
    float        view_dir[4] __attribute__((aligned(16)));
    float        to_camera[4] __attribute__((aligned(16)));
    float        vertex[4] __attribute__((aligned(16)));
    D_2D9EA0    *nanotech_ball_draw_state = (D_2D9EA0 *)moby->pvars;
    Moby        *owner;
    float        pulse = nanotech_ball_draw_state->bob_offset / l00_nanotech_ball_bob_amplitude;
    int          i, j, color;
    short       *indices;
    short       *row;
    short       *rows;
    char        *corner;
    char        *outer, *inner;
    float       *u, *v;
    float       *v_base, *u_base;

    quad.texture[1] = get_effect_texture(l00_nanotech_ball_mesh_texture_index);
    quad.texture[2] = 0xff9000000260ULL;
    quad.texture[3] = (u64)l00_nanotech_ball_blend_minuend_color_selector |
                      ((u64)l00_nanotech_ball_blend_subtrahend_color_selector << 2) |
                      ((u64)l00_nanotech_ball_blend_alpha_selector << 4) |
                      ((u64)l00_nanotech_ball_blend_addend_color_selector << 6) |
                      ((u64)l00_nanotech_ball_blend_fixed_alpha << 32);
    quad.texture[0] = 0;
    vu1_add_g_sregister(0x47, 0x53001);
    forward = basis;
    center[0] = moby->pos.x;
    center[1] = moby->pos.y;
    center[2] = moby->pos.z + 0.5f;
    center[2] += nanotech_ball_draw_state->bob_offset;
    center[3] = 1.0f;
    fast_vec_add(&center, &center, D_L00_00161AF0_macro);
    fast_vec_sub(&to_camera, g_debug_camera, &center);
    fast_vec_normalize(forward, &to_camera, 1.0f);
    owner = nanotech_ball_draw_state->owner;
    fast_vec_cross(&side, forward, &owner->rot_row2);
    fast_vec_normalize(&side, &side, 1.0f);
    fast_vec_cross(&up, &side, forward);
    quad.colors[3] = l00_nanotech_ball_initial_quad_color;
    quad.colors[2] = l00_nanotech_ball_initial_quad_color;
    quad.colors[1] = l00_nanotech_ball_initial_quad_color;
    quad.colors[0] = l00_nanotech_ball_initial_quad_color;
    vu1_add_g_sregister(6, get_effect_texture(l00_nanotech_ball_mesh_texture_index));
    vu1_add_g_sregister(0x42, 0x8000000044ULL);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xff9000000260ULL);
    vu1_add_g_sregister(0x4a, 0);
    font_queue_vu_state();
    for (j = 0; j < 0x122; j++) {
        transform_vector(&vertex, nanotech_ball_mesh_source_vertices + j * 16, basis);
        {
            float x = vertex[0], y = vertex[1], z = vertex[2];
            ((Vec3 *)nanotech_ball_transformed_mesh_vertices)[j].x = x;
            ((Vec3 *)nanotech_ball_transformed_mesh_vertices)[j].y = y;
            ((Vec3 *)nanotech_ball_transformed_mesh_vertices)[j].z = z;
        }
    }
    FUN_L00_001fde98(0x94,
                     nanotech_ball_transformed_mesh_vertices,
                     nanotech_ball_mesh_vertex_colors,
                     nanotech_ball_mesh_texcoords,
                     1);
    FUN_L00_001fde98(0x6d,
                     nanotech_ball_transformed_mesh_vertices + 0x6d8,
                     nanotech_ball_mesh_vertex_colors + 0x248,
                     nanotech_ball_mesh_texcoords + 0x490,
                     1);
    FUN_L00_001fde98(0x24,
                     nanotech_ball_transformed_mesh_vertices + 0xbe8,
                     nanotech_ball_mesh_vertex_colors + 0x3f8,
                     nanotech_ball_mesh_texcoords + 0x7f0,
                     1);
    quad.texture[1] = get_effect_texture(0xb);
    quad.texture[3] = 0x8000000048ULL;
    color = truncate_float_to_s32(pulse * 64.0f + 128.0f);
    color = ((u32)(color + D_L00_00161B10_macro) << 24) | l00_nanotech_ball_pulse_quad_base_color;
    quad.colors[3] = 0;
    quad.colors[2] = 0;
    quad.colors[1] = color;
    quad.colors[0] = color;
    for (i = 0; i < 32; i++) {
        indices = (short *)(nanotech_ball_pulse_quad_vertex_indices + i * 8);
        j = 0;
        inner = (char *)quad.points;
        outer = (char *)quad.points;
        for (; j < 4; j++) {
            short index = *indices;
            if (j >= 2)
                fast_vec_scale(outer,
                               nanotech_ball_pulse_quad_source_vertices + index * 16,
                               l00_nanotech_ball_pulse_quad_base_scale * D_L00_00161AEC_gp);
            else
                fast_vec_scale(inner,
                               nanotech_ball_pulse_quad_source_vertices + index * 16,
                               l00_nanotech_ball_pulse_quad_base_scale);
            inner += 16;
            outer += 16;
            indices++;
        }
        quad.texcoords[0] = 0.5f;
        quad.texcoords[1] = 0.5f;
        quad.texcoords[2] = 0.5f;
        quad.texcoords[3] = 0.5f;
        quad.texcoords[4] = 0.5f;
        quad.texcoords[5] = 1.0f;
        quad.texcoords[6] = 0.5f;
        quad.texcoords[7] = 1.0f;
        draw_geometry_quad(&quad, basis, 0);
    }
    if (is_moby_in_state_one((u8 *)moby)) {
        quad.texture[1] = get_effect_texture(l00_nanotech_ball_state_one_quad_texture_index);
        quad.colors[3] = 0x107f7f7f;
        quad.colors[2] = 0x107f7f7f;
        quad.colors[1] = 0x107f7f7f;
        quad.colors[0] = 0x107f7f7f;
        quad.texture[3] = 0x8000000044ULL;
        {
            Moby *owner = nanotech_ball_draw_state->owner;
            qcopy(basis, &owner->rot_row0);
            qcopy(&side, &owner->rot_row1);
            qcopy(&up, &owner->rot_row2);
        }
        center[2] = moby->pos.z;
        v_base = &quad.texcoords[0];
        u_base = &quad.texcoords[1];
        rows = (short *)nanotech_ball_state_one_quad_vertex_indices;
        for (i = 0; i < 4; i++) {
            u = u_base;
            v = v_base;
            row = (short *)(i * 8 + (u32)rows);
            corner = (char *)quad.points;
            for (j = 3; j >= 0; j--) {
                transform_vector(
                    corner, nanotech_ball_state_one_quad_source_vertices + (*row++) * 16, basis);
                fast_vec_sub(view_dir, corner, g_debug_camera);
                corner += 16;
                view_dir[1] = view_dir[2];
                set_vector_length_xy(
                    view_dir, view_dir, l00_nanotech_ball_state_one_quad_uv_vector_length);
                *v = view_dir[0] * 0.5f + 0.5f;
                *v = FUN_L00_00200260(*v, 8.0f);
                v += 2;
                *u = view_dir[1] * 0.5f + 0.5f;
                *u = FUN_L00_00200260(*u, 8.0f);
                u += 2;
            }
            draw_geometry_quad(&quad, 0, 0);
        }
    }
    vu1_add_g_sregister(0x47, 0x5360b);
}
