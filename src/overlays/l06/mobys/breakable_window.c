/* Level 06 moby class 0x43b: breakable window; shatters into fragments, runs its camera sequence and draws its backdrop mesh. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "asm.h"
#include "qcopy.h"
#include "sda.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/rendering/effect_texture.h"
#include "rnc/overlay/l06/hero/hero_set_state.h"
#include "rnc/overlay/l06/scripted_camera_sequence.h"

extern float          D_L06_00167540[];
extern void           draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");
extern char           D_0013F3D0[];
extern unsigned char *D_L06_0015FFD8_ffcb0 __asm__("D_L06_0015FFD8");

#include "rnc/overlay/cuboid.h"

extern float D_L06_0015F3FC __asm__("D_L06_0015F3FC");
extern void
advance_accelerated_scalar(float *, float *, float, float, float, float) __asm__("FUN_00213f38");
extern void  set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern void  jet_mount(void *, void *, int, int, int) __asm__("FUN_L00_002eaaa0");
extern void  set_camera_rotation(void *) __asm__("FUN_L00_002eaa30");
extern void  FUN_L06_00312b40(float, float, float, float, float, float);
extern void  FUN_L06_00312a78(void *);
extern void  FUN_L06_00312ad0(void *);
extern void  FUN_L06_00312ce8(int);
extern void  set_camera_position(void *) __asm__("FUN_L00_002ea9d8");
void         spawn_moby_breakup_effect(char *source_moby) __asm__("FUN_L06_00300278");
extern void  spawn_moby_breakup_effect(char *) __asm__("FUN_L06_00300278");
extern void  FUN_L06_00210fc8(int, char *);
extern void  FUN_L06_00300958(void);
extern void  effect_quad_draw_single(void) __asm__("FUN_L06_00300720");
extern void  FUN_L06_00300588(void);
extern void  draw_backdrop_mesh(void) __asm__("FUN_L06_003003f0");
extern int   D_L06_0015F408;
extern float camera_sequence_moby_scale_multiplier_gp __asm__("D_L06_00162048")
    __attribute__((sda));

#include "rnc/overlay/l06/mobys/breakable_window_data/update_breakable_window.h"

extern float camera_sequence_rotation_x_degrees_gp __asm__("D_L06_00162050") __attribute__((sda));
void         update_breakable_window(char *moby) __asm__("FUN_L06_002ffcb0");
extern void  update_entity(char *) __asm__("FUN_L00_00250df8");
extern unsigned char              *
spawn_spinning_moby_fragment(char *, float *, float *) __asm__("FUN_L06_00300c60");
extern float moby_breakup_z_offset_gp __asm__("D_L06_0016204C") __attribute__((sda));

typedef struct {
    float f[4];
} __attribute__((aligned(16))) V3003f0;

typedef struct {
    float u, v;
} UV3003f0;

typedef struct {
    V3003f0      corner[4];
    unsigned int color[4];
    UV3003f0     uv[4];
    long         unk70, tex, unk80, unk88;
} Q3003f0;

typedef struct {
    char  pad[0x38];
    float f38;
    int   pad3c;
} B3003f0;

typedef struct {
    short v, uv;
} I3003f0;

#include "rnc/overlay/l06/mobys/breakable_window_data/draw_backdrop_mesh_2.h"

extern unsigned int l06_backdrop_mesh_vertex_color __asm__("D_L06_0016206C") __attribute__((sda));

#include "rnc/overlay/l06/mobys/breakable_window_data/draw_backdrop_mesh.h"

extern V3003f0 l06_backdrop_mesh_vertex_positions[] __asm__("D_L06_001F2EB0");

/* 4 corners for each of the 64 quads draw_backdrop_mesh draws. */
I3003f0 l06_backdrop_mesh_quad_corner_index_pairs[256] __asm__("D_L06_001F34B0") = {
    {   0, 0x18e},
    {   1, 0x18f},
    {   3, 0x18c},
    {   2, 0x18d},
    {   1, 0x18a},
    {   4, 0x18b},
    {   2, 0x188},
    {   5, 0x189},
    {   4, 0x186},
    {   6, 0x187},
    {   5, 0x184},
    {   7, 0x185},
    {   6, 0x182},
    {   8, 0x183},
    {   7, 0x180},
    {   9, 0x181},
    {   8, 0x17e},
    { 0xa, 0x17f},
    {   9, 0x17c},
    { 0xb, 0x17d},
    { 0xa, 0x17a},
    { 0xc, 0x17b},
    { 0xb, 0x178},
    { 0xd, 0x179},
    { 0xc, 0x176},
    { 0xe, 0x177},
    { 0xd, 0x174},
    { 0xf, 0x175},
    { 0xe, 0x172},
    {0x10, 0x173},
    { 0xf, 0x170},
    {0x11, 0x171},
    {0x10, 0x16e},
    {0x12, 0x16f},
    {0x11, 0x16c},
    {0x13, 0x16d},
    {0x12, 0x16a},
    {0x14, 0x16b},
    {0x13, 0x168},
    {0x15, 0x169},
    {0x14, 0x166},
    {0x16, 0x167},
    {0x15, 0x164},
    {0x17, 0x165},
    {0x16, 0x162},
    {0x18, 0x163},
    {0x17, 0x160},
    {0x19, 0x161},
    {0x18, 0x15e},
    {0x1a, 0x15f},
    {0x19, 0x15c},
    {0x1b, 0x15d},
    {0x1a, 0x15a},
    {0x1c, 0x15b},
    {0x1b, 0x158},
    {0x1d, 0x159},
    {0x1c, 0x156},
    {0x1e, 0x157},
    {0x1d, 0x154},
    {0x1f, 0x155},
    {0x1e, 0x152},
    {   0, 0x153},
    {0x1f, 0x150},
    {   3, 0x151},
    {0x20, 0x10e},
    {0x21, 0x10f},
    {0x23, 0x10c},
    {0x22, 0x10d},
    {0x21, 0x10a},
    {0x24, 0x10b},
    {0x22, 0x108},
    {0x25, 0x109},
    {0x24, 0x106},
    {0x26, 0x107},
    {0x25, 0x104},
    {0x27, 0x105},
    {0x26, 0x102},
    {0x28, 0x103},
    {0x27, 0x100},
    {0x29, 0x101},
    {0x28,  0xfe},
    {0x2a,  0xff},
    {0x29,  0xfc},
    {0x2b,  0xfd},
    {0x2a,  0xfa},
    {0x2c,  0xfb},
    {0x2b,  0xf8},
    {0x2d,  0xf9},
    {0x2c,  0xf6},
    {0x2e,  0xf7},
    {0x2d,  0xf4},
    {0x2f,  0xf5},
    {0x2e,  0xf2},
    {0x30,  0xf3},
    {0x2f,  0xf0},
    {0x31,  0xf1},
    {0x30,  0xee},
    {0x32,  0xef},
    {0x31,  0xec},
    {0x33,  0xed},
    {0x32,  0xea},
    {0x34,  0xeb},
    {0x33,  0xe8},
    {0x35,  0xe9},
    {0x34,  0xe6},
    {0x36,  0xe7},
    {0x35,  0xe4},
    {0x37,  0xe5},
    {0x36,  0xe2},
    {0x38,  0xe3},
    {0x37,  0xe0},
    {0x39,  0xe1},
    {0x38,  0xde},
    {0x3a,  0xdf},
    {0x39,  0xdc},
    {0x3b,  0xdd},
    {0x3a,  0xda},
    {0x3c,  0xdb},
    {0x3b,  0xd8},
    {0x3d,  0xd9},
    {0x3c,  0xd6},
    {0x3e,  0xd7},
    {0x3d,  0xd4},
    {0x3f,  0xd5},
    {0x3e,  0xd2},
    {0x20,  0xd3},
    {0x3f,  0xd0},
    {0x23,  0xd1},
    {0x23,  0xca},
    {0x22,  0xcb},
    {0x41,  0xc8},
    {0x40,  0xc9},
    {0x22,  0xc6},
    {0x25,  0xc7},
    {0x40,  0xc4},
    {0x42,  0xc5},
    {0x25,  0xc2},
    {0x27,  0xc3},
    {0x42,  0xc0},
    {0x43,  0xc1},
    {0x27,  0xbe},
    {0x29,  0xbf},
    {0x43,  0xbc},
    {0x44,  0xbd},
    {0x29,  0xba},
    {0x2b,  0xbb},
    {0x44,  0xb8},
    {0x45,  0xb9},
    {0x2b,  0xb6},
    {0x2d,  0xb7},
    {0x45,  0xb4},
    {0x46,  0xb5},
    {0x2d,  0xb2},
    {0x2f,  0xb3},
    {0x46,  0xb0},
    {0x47,  0xb1},
    {0x2f,  0xae},
    {0x31,  0xaf},
    {0x47,  0xac},
    {0x48,  0xad},
    {0x31,  0xaa},
    {0x33,  0xab},
    {0x48,  0xa8},
    {0x49,  0xa9},
    {0x33,  0xa6},
    {0x35,  0xa7},
    {0x49,  0xa4},
    {0x4a,  0xa5},
    {0x35,  0xa2},
    {0x37,  0xa3},
    {0x4a,  0xa0},
    {0x4b,  0xa1},
    {0x37,  0x9e},
    {0x39,  0x9f},
    {0x4b,  0x9c},
    {0x4c,  0x9d},
    {0x39,  0x9a},
    {0x3b,  0x9b},
    {0x4c,  0x98},
    {0x4d,  0x99},
    {0x3b,  0x96},
    {0x3d,  0x97},
    {0x4d,  0x94},
    {0x4e,  0x95},
    {0x3d,  0x92},
    {0x3f,  0x93},
    {0x4e,  0x90},
    {0x4f,  0x91},
    {0x3f,  0xce},
    {0x23,  0xcf},
    {0x4f,  0xcc},
    {0x41,  0xcd},
    {0x1f, 0x14e},
    {   3, 0x14f},
    {0x51, 0x14c},
    {0x50, 0x14d},
    {   3, 0x14a},
    {   2, 0x14b},
    {0x50, 0x148},
    {0x52, 0x149},
    {   2, 0x146},
    {   5, 0x147},
    {0x52, 0x144},
    {0x53, 0x145},
    {   5, 0x142},
    {   7, 0x143},
    {0x53, 0x140},
    {0x54, 0x141},
    {   7, 0x13e},
    {   9, 0x13f},
    {0x54, 0x13c},
    {0x55, 0x13d},
    {   9, 0x13a},
    { 0xb, 0x13b},
    {0x55, 0x138},
    {0x56, 0x139},
    { 0xb, 0x136},
    { 0xd, 0x137},
    {0x56, 0x134},
    {0x57, 0x135},
    { 0xd, 0x132},
    { 0xf, 0x133},
    {0x57, 0x130},
    {0x58, 0x131},
    { 0xf, 0x12e},
    {0x11, 0x12f},
    {0x58, 0x12c},
    {0x59, 0x12d},
    {0x11, 0x12a},
    {0x13, 0x12b},
    {0x59, 0x128},
    {0x5a, 0x129},
    {0x13, 0x126},
    {0x15, 0x127},
    {0x5a, 0x124},
    {0x5b, 0x125},
    {0x15, 0x122},
    {0x17, 0x123},
    {0x5b, 0x120},
    {0x5c, 0x121},
    {0x17, 0x11e},
    {0x19, 0x11f},
    {0x5c, 0x11c},
    {0x5d, 0x11d},
    {0x19, 0x11a},
    {0x1b, 0x11b},
    {0x5d, 0x118},
    {0x5e, 0x119},
    {0x1b, 0x116},
    {0x1d, 0x117},
    {0x5e, 0x114},
    {0x5f, 0x115},
    {0x1d, 0x112},
    {0x1f, 0x113},
    {0x5f, 0x110},
    {0x51, 0x111},
};

extern UV3003f0 l06_backdrop_mesh_texture_coordinates[] __asm__("D_L06_001F38B0");

/* Draws the 96 quads of the second backdrop mesh, skipping those that face away from D_L06_00167540. */
typedef float FVec4_l06[4] __attribute__((aligned(16)));

typedef struct {
    float v[4];
} __attribute__((aligned(16))) FRow_l06;

/* One quad centre and one quad normal for each of the 96 quads effect_quad_draw_single draws. */
FRow_l06 D_L06_001FAFF0[96] = {
    {{37150.492f, 33258.63f, 13941.652f, 1.0f}},  {{37076.242f, 33289.383f, 13941.652f, 1.0f}},
    {{36995.883f, 33289.383f, 13941.652f, 1.0f}}, {{36921.633f, 33258.63f, 13941.652f, 1.0f}},
    {{36864.81f, 33201.805f, 13941.652f, 1.0f}},  {{36834.055f, 33127.555f, 13941.652f, 1.0f}},
    {{36834.055f, 33047.195f, 13941.652f, 1.0f}}, {{36864.81f, 32972.945f, 13941.652f, 1.0f}},
    {{36921.633f, 32916.12f, 13941.652f, 1.0f}},  {{36995.883f, 32885.367f, 13941.652f, 1.0f}},
    {{37076.242f, 32885.367f, 13941.652f, 1.0f}}, {{37150.492f, 32916.12f, 13941.652f, 1.0f}},
    {{37207.316f, 32972.945f, 13941.652f, 1.0f}}, {{37238.07f, 33047.195f, 13941.652f, 1.0f}},
    {{37238.07f, 33127.555f, 13941.652f, 1.0f}},  {{37207.316f, 33201.805f, 13941.652f, 1.0f}},
    {{37150.492f, 33258.63f, 14130.826f, 1.0f}},  {{37076.242f, 33289.383f, 14130.826f, 1.0f}},
    {{36995.883f, 33289.383f, 14130.826f, 1.0f}}, {{36921.633f, 33258.63f, 14130.826f, 1.0f}},
    {{36864.81f, 33201.805f, 14130.826f, 1.0f}},  {{36834.055f, 33127.555f, 14130.826f, 1.0f}},
    {{36834.055f, 33047.195f, 14130.826f, 1.0f}}, {{36864.81f, 32972.945f, 14130.826f, 1.0f}},
    {{36921.633f, 32916.12f, 14130.826f, 1.0f}},  {{36995.883f, 32885.367f, 14130.826f, 1.0f}},
    {{37076.242f, 32885.367f, 14130.826f, 1.0f}}, {{37150.492f, 32916.12f, 14130.826f, 1.0f}},
    {{37207.316f, 32972.945f, 14130.826f, 1.0f}}, {{37238.07f, 33047.195f, 14130.826f, 1.0f}},
    {{37238.07f, 33127.555f, 14130.826f, 1.0f}},  {{37207.316f, 33201.805f, 14130.826f, 1.0f}},
    {{37150.492f, 33258.63f, 14327.521f, 1.0f}},  {{37076.242f, 33289.383f, 14327.521f, 1.0f}},
    {{36995.883f, 33289.383f, 14327.521f, 1.0f}}, {{36921.633f, 33258.63f, 14327.521f, 1.0f}},
    {{36864.81f, 33201.805f, 14327.521f, 1.0f}},  {{36834.055f, 33127.555f, 14327.521f, 1.0f}},
    {{36834.055f, 33047.195f, 14327.521f, 1.0f}}, {{36864.81f, 32972.945f, 14327.521f, 1.0f}},
    {{36921.633f, 32916.12f, 14327.521f, 1.0f}},  {{36995.883f, 32885.367f, 14327.521f, 1.0f}},
    {{37076.242f, 32885.367f, 14327.521f, 1.0f}}, {{37150.492f, 32916.12f, 14327.521f, 1.0f}},
    {{37207.316f, 32972.945f, 14327.521f, 1.0f}}, {{37238.07f, 33047.195f, 14327.521f, 1.0f}},
    {{37238.07f, 33127.555f, 14327.521f, 1.0f}},  {{37207.316f, 33201.805f, 14327.521f, 1.0f}},
    {{37150.492f, 33258.63f, 14418.348f, 1.0f}},  {{37076.242f, 33289.383f, 14418.348f, 1.0f}},
    {{36995.883f, 33289.383f, 14418.348f, 1.0f}}, {{36921.633f, 33258.63f, 14418.348f, 1.0f}},
    {{36864.81f, 33201.805f, 14418.348f, 1.0f}},  {{36834.055f, 33127.555f, 14418.348f, 1.0f}},
    {{36834.055f, 33047.195f, 14418.348f, 1.0f}}, {{36864.81f, 32972.945f, 14418.348f, 1.0f}},
    {{36921.633f, 32916.12f, 14418.348f, 1.0f}},  {{36995.883f, 32885.367f, 14418.348f, 1.0f}},
    {{37076.242f, 32885.367f, 14418.348f, 1.0f}}, {{37150.492f, 32916.12f, 14418.348f, 1.0f}},
    {{37207.316f, 32972.945f, 14418.348f, 1.0f}}, {{37238.07f, 33047.195f, 14418.348f, 1.0f}},
    {{37238.07f, 33127.555f, 14418.348f, 1.0f}},  {{37207.316f, 33201.805f, 14418.348f, 1.0f}},
    {{37207.316f, 33201.805f, 14032.479f, 1.0f}}, {{37150.492f, 33258.63f, 14032.479f, 1.0f}},
    {{37076.242f, 33289.383f, 14032.479f, 1.0f}}, {{36995.883f, 33289.383f, 14032.479f, 1.0f}},
    {{36921.633f, 33258.63f, 14032.479f, 1.0f}},  {{36864.81f, 33201.805f, 14032.479f, 1.0f}},
    {{36834.055f, 33127.555f, 14032.479f, 1.0f}}, {{36834.055f, 33047.195f, 14032.479f, 1.0f}},
    {{36864.81f, 32972.945f, 14032.479f, 1.0f}},  {{36921.633f, 32916.12f, 14032.479f, 1.0f}},
    {{36995.883f, 32885.367f, 14032.479f, 1.0f}}, {{37076.242f, 32885.367f, 14032.479f, 1.0f}},
    {{37150.492f, 32916.12f, 14032.479f, 1.0f}},  {{37207.316f, 32972.945f, 14032.479f, 1.0f}},
    {{37238.07f, 33047.195f, 14032.479f, 1.0f}},  {{37238.07f, 33127.555f, 14032.479f, 1.0f}},
    {{37207.316f, 33201.805f, 14229.174f, 1.0f}}, {{37150.492f, 33258.63f, 14229.174f, 1.0f}},
    {{37076.242f, 33289.383f, 14229.174f, 1.0f}}, {{36995.883f, 33289.383f, 14229.174f, 1.0f}},
    {{36921.633f, 33258.63f, 14229.174f, 1.0f}},  {{36864.81f, 33201.805f, 14229.174f, 1.0f}},
    {{36834.055f, 33127.555f, 14229.174f, 1.0f}}, {{36834.055f, 33047.195f, 14229.174f, 1.0f}},
    {{36864.81f, 32972.945f, 14229.174f, 1.0f}},  {{36921.633f, 32916.12f, 14229.174f, 1.0f}},
    {{36995.883f, 32885.367f, 14229.174f, 1.0f}}, {{37076.242f, 32885.367f, 14229.174f, 1.0f}},
    {{37150.492f, 32916.12f, 14229.174f, 1.0f}},  {{37207.316f, 32972.945f, 14229.174f, 1.0f}},
    {{37238.07f, 33047.195f, 14229.174f, 1.0f}},  {{37238.07f, 33127.555f, 14229.174f, 1.0f}},
};

FRow_l06 D_L06_001FA2F0[96] = {
    {{0.55558f, 0.831463f, 0.0f, 1.0f}},   {{0.19508f, 0.980787f, 0.0f, 1.0f}},
    {{-0.19508f, 0.980787f, 0.0f, 1.0f}},  {{-0.55558f, 0.831463f, 0.0f, 1.0f}},
    {{-0.831463f, 0.55558f, 0.0f, 1.0f}},  {{-0.980787f, 0.19508f, 0.0f, 1.0f}},
    {{-0.980787f, -0.19508f, 0.0f, 1.0f}}, {{-0.831463f, -0.55558f, 0.0f, 1.0f}},
    {{-0.55558f, -0.831463f, 0.0f, 1.0f}}, {{-0.19508f, -0.980787f, 0.0f, 1.0f}},
    {{0.19508f, -0.980787f, 0.0f, 1.0f}},  {{0.55558f, -0.831463f, 0.0f, 1.0f}},
    {{0.831463f, -0.55558f, 0.0f, 1.0f}},  {{0.980787f, -0.19508f, 0.0f, 1.0f}},
    {{0.980787f, 0.19508f, 0.0f, 1.0f}},   {{0.831463f, 0.55558f, 0.0f, 1.0f}},
    {{0.55558f, 0.831463f, 0.0f, 1.0f}},   {{0.19508f, 0.980787f, 0.0f, 1.0f}},
    {{-0.19508f, 0.980787f, 0.0f, 1.0f}},  {{-0.55558f, 0.831463f, 0.0f, 1.0f}},
    {{-0.831463f, 0.55558f, 0.0f, 1.0f}},  {{-0.980787f, 0.19508f, 0.0f, 1.0f}},
    {{-0.980787f, -0.19508f, 0.0f, 1.0f}}, {{-0.831463f, -0.55558f, 0.0f, 1.0f}},
    {{-0.55558f, -0.831463f, 0.0f, 1.0f}}, {{-0.19508f, -0.980787f, 0.0f, 1.0f}},
    {{0.19508f, -0.980787f, 0.0f, 1.0f}},  {{0.55558f, -0.831463f, 0.0f, 1.0f}},
    {{0.831463f, -0.55558f, 0.0f, 1.0f}},  {{0.980787f, -0.19508f, 0.0f, 1.0f}},
    {{0.980787f, 0.19508f, 0.0f, 1.0f}},   {{0.831463f, 0.55558f, 0.0f, 1.0f}},
    {{0.55558f, 0.831463f, 0.0f, 1.0f}},   {{0.19508f, 0.980787f, 0.0f, 1.0f}},
    {{-0.19508f, 0.980787f, 0.0f, 1.0f}},  {{-0.55558f, 0.831463f, 0.0f, 1.0f}},
    {{-0.831463f, 0.55558f, 0.0f, 1.0f}},  {{-0.980787f, 0.19508f, 0.0f, 1.0f}},
    {{-0.980787f, -0.19508f, 0.0f, 1.0f}}, {{-0.831463f, -0.55558f, 0.0f, 1.0f}},
    {{-0.55558f, -0.831463f, 0.0f, 1.0f}}, {{-0.19508f, -0.980787f, 0.0f, 1.0f}},
    {{0.19508f, -0.980787f, 0.0f, 1.0f}},  {{0.55558f, -0.831463f, 0.0f, 1.0f}},
    {{0.831463f, -0.55558f, 0.0f, 1.0f}},  {{0.980787f, -0.19508f, 0.0f, 1.0f}},
    {{0.980787f, 0.19508f, 0.0f, 1.0f}},   {{0.831463f, 0.55558f, 0.0f, 1.0f}},
    {{0.55558f, 0.831463f, 0.0f, 1.0f}},   {{0.19508f, 0.980787f, 0.0f, 1.0f}},
    {{-0.19508f, 0.980787f, 0.0f, 1.0f}},  {{-0.55558f, 0.831463f, 0.0f, 1.0f}},
    {{-0.831463f, 0.55558f, 0.0f, 1.0f}},  {{-0.980787f, 0.19508f, 0.0f, 1.0f}},
    {{-0.980787f, -0.19508f, 0.0f, 1.0f}}, {{-0.831463f, -0.55558f, 0.0f, 1.0f}},
    {{-0.55558f, -0.831463f, 0.0f, 1.0f}}, {{-0.19508f, -0.980787f, 0.0f, 1.0f}},
    {{0.19508f, -0.980787f, 0.0f, 1.0f}},  {{0.55558f, -0.831463f, 0.0f, 1.0f}},
    {{0.831463f, -0.55558f, 0.0f, 1.0f}},  {{0.980787f, -0.19508f, 0.0f, 1.0f}},
    {{0.980787f, 0.19508f, 0.0f, 1.0f}},   {{0.831463f, 0.55558f, 0.0f, 1.0f}},
    {{0.831463f, 0.55558f, 0.0f, 1.0f}},   {{0.55558f, 0.831463f, 0.0f, 1.0f}},
    {{0.19508f, 0.980787f, 0.0f, 1.0f}},   {{-0.19508f, 0.980787f, 0.0f, 1.0f}},
    {{-0.55558f, 0.831463f, 0.0f, 1.0f}},  {{-0.831463f, 0.55558f, 0.0f, 1.0f}},
    {{-0.980787f, 0.19508f, 0.0f, 1.0f}},  {{-0.980787f, -0.19508f, 0.0f, 1.0f}},
    {{-0.831463f, -0.55558f, 0.0f, 1.0f}}, {{-0.55558f, -0.831463f, 0.0f, 1.0f}},
    {{-0.19508f, -0.980787f, 0.0f, 1.0f}}, {{0.19508f, -0.980787f, 0.0f, 1.0f}},
    {{0.55558f, -0.831463f, 0.0f, 1.0f}},  {{0.831463f, -0.55558f, 0.0f, 1.0f}},
    {{0.980787f, -0.19508f, 0.0f, 1.0f}},  {{0.980787f, 0.19508f, 0.0f, 1.0f}},
    {{0.831463f, 0.55558f, 0.0f, 1.0f}},   {{0.55558f, 0.831463f, 0.0f, 1.0f}},
    {{0.19508f, 0.980787f, 0.0f, 1.0f}},   {{-0.19508f, 0.980787f, 0.0f, 1.0f}},
    {{-0.55558f, 0.831463f, 0.0f, 1.0f}},  {{-0.831463f, 0.55558f, 0.0f, 1.0f}},
    {{-0.980787f, 0.19508f, 0.0f, 1.0f}},  {{-0.980787f, -0.19508f, 0.0f, 1.0f}},
    {{-0.831463f, -0.55558f, 0.0f, 1.0f}}, {{-0.55558f, -0.831463f, 0.0f, 1.0f}},
    {{-0.19508f, -0.980787f, 0.0f, 1.0f}}, {{0.19508f, -0.980787f, 0.0f, 1.0f}},
    {{0.55558f, -0.831463f, 0.0f, 1.0f}},  {{0.831463f, -0.55558f, 0.0f, 1.0f}},
    {{0.980787f, -0.19508f, 0.0f, 1.0f}},  {{0.980787f, 0.19508f, 0.0f, 1.0f}},
};

extern FRow_l06 D_L06_001F77B0[] NOT_SDA;
extern float    D_L06_001F84B0[][2] NOT_SDA;

/* (vertex, uv) index pair of each corner of the 96 quads. */
short D_L06_001F7EB0[96][4][2] = {
    {   {0, 0x3ba},    {1, 0x3bb},    {3, 0x3b8},    {2, 0x3b9}},
    {   {1, 0x3b6},    {4, 0x3b7},    {2, 0x3b4},    {5, 0x3b5}},
    {   {4, 0x3b2},    {6, 0x3b3},    {5, 0x3b0},    {7, 0x3b1}},
    {   {6, 0x3ae},    {8, 0x3af},    {7, 0x3ac},    {9, 0x3ad}},
    {   {8, 0x3aa},  {0xa, 0x3ab},    {9, 0x3a8},  {0xb, 0x3a9}},
    { {0xa, 0x3a6},  {0xc, 0x3a7},  {0xb, 0x3a4},  {0xd, 0x3a5}},
    { {0xc, 0x3a2},  {0xe, 0x3a3},  {0xd, 0x3a0},  {0xf, 0x3a1}},
    { {0xe, 0x39e}, {0x10, 0x39f},  {0xf, 0x39c}, {0x11, 0x39d}},
    {{0x10, 0x39a}, {0x12, 0x39b}, {0x11, 0x398}, {0x13, 0x399}},
    {{0x12, 0x396}, {0x14, 0x397}, {0x13, 0x394}, {0x15, 0x395}},
    {{0x14, 0x392}, {0x16, 0x393}, {0x15, 0x390}, {0x17, 0x391}},
    {{0x16, 0x38e}, {0x18, 0x38f}, {0x17, 0x38c}, {0x19, 0x38d}},
    {{0x18, 0x38a}, {0x1a, 0x38b}, {0x19, 0x388}, {0x1b, 0x389}},
    {{0x1a, 0x3be}, {0x1c, 0x3bf}, {0x1b, 0x3bc}, {0x1d, 0x3bd}},
    {{0x1c, 0x3c2}, {0x1e, 0x3c3}, {0x1d, 0x3c0}, {0x1f, 0x3c1}},
    {{0x1e, 0x3c6},    {0, 0x3c7}, {0x1f, 0x3c4},    {3, 0x3c5}},
    {{0x20, 0x332}, {0x21, 0x333}, {0x23, 0x330}, {0x22, 0x331}},
    {{0x21, 0x32e}, {0x24, 0x32f}, {0x22, 0x32c}, {0x25, 0x32d}},
    {{0x24, 0x32a}, {0x26, 0x32b}, {0x25, 0x328}, {0x27, 0x329}},
    {{0x26, 0x326}, {0x28, 0x327}, {0x27, 0x324}, {0x29, 0x325}},
    {{0x28, 0x322}, {0x2a, 0x323}, {0x29, 0x320}, {0x2b, 0x321}},
    {{0x2a, 0x31e}, {0x2c, 0x31f}, {0x2b, 0x31c}, {0x2d, 0x31d}},
    {{0x2c, 0x31a}, {0x2e, 0x31b}, {0x2d, 0x318}, {0x2f, 0x319}},
    {{0x2e, 0x316}, {0x30, 0x317}, {0x2f, 0x314}, {0x31, 0x315}},
    {{0x30, 0x312}, {0x32, 0x313}, {0x31, 0x310}, {0x33, 0x311}},
    {{0x32, 0x30e}, {0x34, 0x30f}, {0x33, 0x30c}, {0x35, 0x30d}},
    {{0x34, 0x30a}, {0x36, 0x30b}, {0x35, 0x308}, {0x37, 0x309}},
    {{0x36, 0x346}, {0x38, 0x347}, {0x37, 0x344}, {0x39, 0x345}},
    {{0x38, 0x342}, {0x3a, 0x343}, {0x39, 0x340}, {0x3b, 0x341}},
    {{0x3a, 0x33e}, {0x3c, 0x33f}, {0x3b, 0x33c}, {0x3d, 0x33d}},
    {{0x3c, 0x33a}, {0x3e, 0x33b}, {0x3d, 0x338}, {0x3f, 0x339}},
    {{0x3e, 0x336}, {0x20, 0x337}, {0x3f, 0x334}, {0x23, 0x335}},
    {{0x40, 0x2aa}, {0x41, 0x2ab}, {0x43, 0x2a8}, {0x42, 0x2a9}},
    {{0x41, 0x2a6}, {0x44, 0x2a7}, {0x42, 0x2a4}, {0x45, 0x2a5}},
    {{0x44, 0x2a2}, {0x46, 0x2a3}, {0x45, 0x2a0}, {0x47, 0x2a1}},
    {{0x46, 0x29e}, {0x48, 0x29f}, {0x47, 0x29c}, {0x49, 0x29d}},
    {{0x48, 0x29a}, {0x4a, 0x29b}, {0x49, 0x298}, {0x4b, 0x299}},
    {{0x4a, 0x296}, {0x4c, 0x297}, {0x4b, 0x294}, {0x4d, 0x295}},
    {{0x4c, 0x292}, {0x4e, 0x293}, {0x4d, 0x290}, {0x4f, 0x291}},
    {{0x4e, 0x28e}, {0x50, 0x28f}, {0x4f, 0x28c}, {0x51, 0x28d}},
    {{0x50, 0x28a}, {0x52, 0x28b}, {0x51, 0x288}, {0x53, 0x289}},
    {{0x52, 0x2c6}, {0x54, 0x2c7}, {0x53, 0x2c4}, {0x55, 0x2c5}},
    {{0x54, 0x2c2}, {0x56, 0x2c3}, {0x55, 0x2c0}, {0x57, 0x2c1}},
    {{0x56, 0x2be}, {0x58, 0x2bf}, {0x57, 0x2bc}, {0x59, 0x2bd}},
    {{0x58, 0x2ba}, {0x5a, 0x2bb}, {0x59, 0x2b8}, {0x5b, 0x2b9}},
    {{0x5a, 0x2b6}, {0x5c, 0x2b7}, {0x5b, 0x2b4}, {0x5d, 0x2b5}},
    {{0x5c, 0x2b2}, {0x5e, 0x2b3}, {0x5d, 0x2b0}, {0x5f, 0x2b1}},
    {{0x5e, 0x2ae}, {0x40, 0x2af}, {0x5f, 0x2ac}, {0x43, 0x2ad}},
    {{0x43, 0x266}, {0x42, 0x267}, {0x61, 0x264}, {0x60, 0x265}},
    {{0x42, 0x262}, {0x45, 0x263}, {0x60, 0x260}, {0x62, 0x261}},
    {{0x45, 0x25e}, {0x47, 0x25f}, {0x62, 0x25c}, {0x63, 0x25d}},
    {{0x47, 0x25a}, {0x49, 0x25b}, {0x63, 0x258}, {0x64, 0x259}},
    {{0x49, 0x256}, {0x4b, 0x257}, {0x64, 0x254}, {0x65, 0x255}},
    {{0x4b, 0x252}, {0x4d, 0x253}, {0x65, 0x250}, {0x66, 0x251}},
    {{0x4d, 0x24e}, {0x4f, 0x24f}, {0x66, 0x24c}, {0x67, 0x24d}},
    {{0x4f, 0x24a}, {0x51, 0x24b}, {0x67, 0x248}, {0x68, 0x249}},
    {{0x51, 0x286}, {0x53, 0x287}, {0x68, 0x284}, {0x69, 0x285}},
    {{0x53, 0x282}, {0x55, 0x283}, {0x69, 0x280}, {0x6a, 0x281}},
    {{0x55, 0x27e}, {0x57, 0x27f}, {0x6a, 0x27c}, {0x6b, 0x27d}},
    {{0x57, 0x27a}, {0x59, 0x27b}, {0x6b, 0x278}, {0x6c, 0x279}},
    {{0x59, 0x276}, {0x5b, 0x277}, {0x6c, 0x274}, {0x6d, 0x275}},
    {{0x5b, 0x272}, {0x5d, 0x273}, {0x6d, 0x270}, {0x6e, 0x271}},
    {{0x5d, 0x26e}, {0x5f, 0x26f}, {0x6e, 0x26c}, {0x6f, 0x26d}},
    {{0x5f, 0x26a}, {0x43, 0x26b}, {0x6f, 0x268}, {0x61, 0x269}},
    {{0x1f, 0x37a},    {3, 0x37b}, {0x3e, 0x378}, {0x20, 0x379}},
    {   {3, 0x376},    {2, 0x377}, {0x20, 0x374}, {0x21, 0x375}},
    {   {2, 0x372},    {5, 0x373}, {0x21, 0x370}, {0x24, 0x371}},
    {   {5, 0x36e},    {7, 0x36f}, {0x24, 0x36c}, {0x26, 0x36d}},
    {   {7, 0x36a},    {9, 0x36b}, {0x26, 0x368}, {0x28, 0x369}},
    {   {9, 0x366},  {0xb, 0x367}, {0x28, 0x364}, {0x2a, 0x365}},
    { {0xb, 0x362},  {0xd, 0x363}, {0x2a, 0x360}, {0x2c, 0x361}},
    { {0xd, 0x35e},  {0xf, 0x35f}, {0x2c, 0x35c}, {0x2e, 0x35d}},
    { {0xf, 0x35a}, {0x11, 0x35b}, {0x2e, 0x358}, {0x30, 0x359}},
    {{0x11, 0x356}, {0x13, 0x357}, {0x30, 0x354}, {0x32, 0x355}},
    {{0x13, 0x352}, {0x15, 0x353}, {0x32, 0x350}, {0x34, 0x351}},
    {{0x15, 0x34e}, {0x17, 0x34f}, {0x34, 0x34c}, {0x36, 0x34d}},
    {{0x17, 0x34a}, {0x19, 0x34b}, {0x36, 0x348}, {0x38, 0x349}},
    {{0x19, 0x386}, {0x1b, 0x387}, {0x38, 0x384}, {0x3a, 0x385}},
    {{0x1b, 0x382}, {0x1d, 0x383}, {0x3a, 0x380}, {0x3c, 0x381}},
    {{0x1d, 0x37e}, {0x1f, 0x37f}, {0x3c, 0x37c}, {0x3e, 0x37d}},
    {{0x3f, 0x2f2}, {0x23, 0x2f3}, {0x5e, 0x2f0}, {0x40, 0x2f1}},
    {{0x23, 0x2ee}, {0x22, 0x2ef}, {0x40, 0x2ec}, {0x41, 0x2ed}},
    {{0x22, 0x2ea}, {0x25, 0x2eb}, {0x41, 0x2e8}, {0x44, 0x2e9}},
    {{0x25, 0x2e6}, {0x27, 0x2e7}, {0x44, 0x2e4}, {0x46, 0x2e5}},
    {{0x27, 0x2e2}, {0x29, 0x2e3}, {0x46, 0x2e0}, {0x48, 0x2e1}},
    {{0x29, 0x2de}, {0x2b, 0x2df}, {0x48, 0x2dc}, {0x4a, 0x2dd}},
    {{0x2b, 0x2da}, {0x2d, 0x2db}, {0x4a, 0x2d8}, {0x4c, 0x2d9}},
    {{0x2d, 0x2d6}, {0x2f, 0x2d7}, {0x4c, 0x2d4}, {0x4e, 0x2d5}},
    {{0x2f, 0x2d2}, {0x31, 0x2d3}, {0x4e, 0x2d0}, {0x50, 0x2d1}},
    {{0x31, 0x2ce}, {0x33, 0x2cf}, {0x50, 0x2cc}, {0x52, 0x2cd}},
    {{0x33, 0x2ca}, {0x35, 0x2cb}, {0x52, 0x2c8}, {0x54, 0x2c9}},
    {{0x35, 0x306}, {0x37, 0x307}, {0x54, 0x304}, {0x56, 0x305}},
    {{0x37, 0x302}, {0x39, 0x303}, {0x56, 0x300}, {0x58, 0x301}},
    {{0x39, 0x2fe}, {0x3b, 0x2ff}, {0x58, 0x2fc}, {0x5a, 0x2fd}},
    {{0x3b, 0x2fa}, {0x3d, 0x2fb}, {0x5a, 0x2f8}, {0x5c, 0x2f9}},
    {{0x3d, 0x2f6}, {0x3f, 0x2f7}, {0x5c, 0x2f4}, {0x5e, 0x2f5}},
};

extern char D_0013F3D0[] NOT_SDA;

void update_breakable_window(char *moby) {
    enum {
        INITIALIZE_DRAW_SELECTION = 0,
        WAIT_FOR_LINKED_MOBY_TRIGGER = 1,
        WAIT_CAMERA_START_DELAY = 2,
        BLEND_CAMERA_TO_HERO = 3,
        WAIT_FOR_NEARBY_HERO = 4
    };
    int  *data = ((int *)MOBY(moby)->pvars);
    float camera_position[4], camera_setup_rotation_or_position_target[4], from[4], target[4],
        offset[4];
    int base;
    int state;
    MOBY(moby)->scale =
        *(float *)(((char *)MOBY(moby)->pclass) + 0x24) * camera_sequence_moby_scale_multiplier_gp;
    switch ((unsigned char)moby[0x20]) {
    case INITIALIZE_DRAW_SELECTION:
        data[1] = 1;
        moby[0x20] = 1;
        break;
    case WAIT_FOR_LINKED_MOBY_TRIGGER:
        if ((data[2] < 0 || (unsigned char)D_L06_0015FFD8_ffcb0[(data[2] << 8) + 0xbc] == 0) &&
            data[8] != -1) {
            break;
        }
        if (vector_distance_xy(moby + 0x10, D_0013F3D0) < 32.0f) {
            moby[0x20] = 2;
            data[9] = scale_game_frames(30);
        } else {
            spawn_moby_breakup_effect(moby);
            moby[0x20] = 4;
        }
        break;
    case WAIT_CAMERA_START_DELAY:
        if (fast_dec_timer(data + 9)) {
            {
                int state;
                int base;
                moby[0x20] = 3;
                scripted_camera_sequence_active = 1;
                D_L06_0015F408 = 0x12;
                state = data[5];
                state <<= 7;
                base = (int)l06_cuboids;
                qcopy(camera_position, (void *)(state + base + 0x30));
                qcopy(camera_setup_rotation_or_position_target, (void *)(state + base + 0x70));
            }
            jet_mount(camera_position,
                      camera_setup_rotation_or_position_target,
                      1,
                      scale_game_frames(300),
                      0);
            FUN_L06_00312b40(0.001f, 1.0f, 1.0f, 0.001f, 1.0f, 1.0f);
            FUN_L06_00312a78(camera_position);
            FUN_L06_00312ad0(camera_setup_rotation_or_position_target);
            {
                int   base;
                int   state;
                float facing;
                data[6] = 0;
                {
                    int base;
                    base = data[4];
                    base <<= 7;
                    base += (int)l06_cuboids;
                    facing = fast_arc_tan(MOBY(moby)->pos.x - *(float *)(base + 0x30),
                                          MOBY(moby)->pos.y - *(float *)(base + 0x34));
                }
                {
                    int state;
                    state = data[4];
                    state <<= 7;
                    base = (int)l06_cuboids;
                    *(float *)(state + base + 0x78) = facing;
                }
                state = data[4];
                state <<= 7;
                set_race_camera(
                    (void *)(base + state + 0x30), (void *)(base + state + 0x70), 0x72, 0);
            }
        }
        if (data[9] < camera_sequence_scalar_ramp_duration_frames_gp)
            D_L06_0015F3FC = 1.0f - convert_integer_to_float(data[9]) /
                                        (float)camera_sequence_scalar_ramp_duration_frames_gp;
        break;
    case BLEND_CAMERA_TO_HERO:
        if (data[9] < camera_sequence_scalar_ramp_duration_frames_gp) {
            data[9]++;
            D_L06_0015F3FC = 1.0f - convert_integer_to_float(data[9]) /
                                        (float)camera_sequence_scalar_ramp_duration_frames_gp;
        }
        /* This field holds float bits; retail completes only on an ordered >= comparison. */
        if (*(float *)(data + 6) >= 0.99999f) {
            moby[0x20] = 5;
            hero_set_state(0, 0);
            FUN_L06_00312ce8(2);
            scripted_camera_sequence_active = 0;
        } else {
            if (*(float *)(data + 6) >= 0.05f && data[3] >= 0) {
                {
                    int state;
                    int base;
                    state = data[3];
                    state <<= 7;
                    base = (int)l06_cuboids;
                    qcopy((void *)(state + base + 0x30), D_0013F3D0);
                    *(float *)(state + base + 0x38) -= 0.5f;
                }
                data[3] = -1;
            }
            if (data[1] == 1 &&
                vector_distance_xy(moby + 0x10, D_L06_0015FFD8_ffcb0 + (data[8] << 8) + 0x10) >
                    0.1f) {
                allocate_voice_for_target_entry(0, 0, moby);
                spawn_moby_breakup_effect(moby);
            }
            advance_accelerated_scalar((float *)(data + 6),
                                       (float *)(data + 7),
                                       1.0f,
                                       frame_time_sq,
                                       frame_time_sq,
                                       frame_time * 1.5f);
            {
                int state;
                int base;
                state = data[5];
                state <<= 7;
                base = (int)l06_cuboids;
                qcopy(camera_position, (void *)(state + base + 0x30));
                qcopy(from, (void *)(state + base + 0x70));
            }
            from[0] = camera_sequence_rotation_x_degrees_gp * 0.017453292f;
            qcopy(camera_setup_rotation_or_position_target, D_0013F3D0);
            qcopy(target, D_0013F3D0 + 16);
            /* Retail takes this component from the camera rotation, not the hero position. */
            target[1] = from[1];
            fast_vec_normalize(offset, D_0013F3D0 - 0x80, -2.0f);
            fast_vec_add(camera_setup_rotation_or_position_target,
                         camera_setup_rotation_or_position_target,
                         offset);
            camera_setup_rotation_or_position_target[2] += 0.85f;
            lerp_vector(camera_position,
                        camera_position,
                        camera_setup_rotation_or_position_target,
                        *(float *)(data + 6));
            lerp_vector(from, from, target, *(float *)(data + 6));
            set_camera_position(camera_position);
            set_camera_rotation(from);
        }
        break;
    case WAIT_FOR_NEARBY_HERO:
        if (vector_distance_xy(moby + 0x10, D_0013F3D0) < 16.0f) {
            state = data[3];
            state <<= 7;
            base = (int)l06_cuboids;
            qcopy((void *)(state + base + 0x30), D_0013F3D0);
            *(float *)(state + base + 0x38) -= 0.5f;
            data[3] = -1;
            moby[0x20] = 5;
        }
        break;
    }
    if (data[1] != 0) {
        if (data[0] != 0)
            FUN_L06_00210fc8((int)effect_quad_draw_single, moby);
        else
            FUN_L06_00210fc8((int)FUN_L06_00300958, moby);
    } else {
        if (data[0] != 0)
            FUN_L06_00210fc8((int)draw_backdrop_mesh, moby);
        else
            FUN_L06_00210fc8((int)FUN_L06_00300588, moby);
    }
}

/* Breaks the moby apart: spawns a copy (class 0x43C) and throws 50
 * fragments out in random directions. */
void spawn_moby_breakup_effect(char *source_moby) {
    float fragment_position[4];
    float fragment_velocity[4];
    char *spawned_moby;
    float fragment_step_length;
    float fragment_angle;
    int   i;

    *(int *)(((char *)MOBY(source_moby)->pvars) + 4) = 0;
    spawned_moby = create_moby(0x43C);
    MOBY(spawned_moby)->draw_dist = 0x40;
    spawned_moby[0x31] = 1;
    *(long long *)(spawned_moby + 0x38) = *(long long *)(source_moby + 0x38);
    MOBY(spawned_moby)->flags = MOBY(source_moby)->flags;
    qcopy(spawned_moby + 0x10, source_moby + 0x10);
    qcopy(spawned_moby + 0x40, source_moby + 0x40);
    MOBY(spawned_moby)->scale = MOBY(source_moby)->scale;
    update_entity(spawned_moby);
    MOBY(source_moby)->flags |= 1;
    for (i = 0; i < 50; i++) {
        fragment_step_length = random_float_between(1.0f, 6.0f) * frame_time;
        fragment_angle = random_angle_radians();
        fragment_velocity[0] = fast_cos(fragment_angle) * 2.0f;
        fragment_velocity[1] = fast_sin(fragment_angle) * 2.0f;
        fragment_velocity[2] = 0.0f;
        fast_vec_add(fragment_position, source_moby + 0x10, fragment_velocity);
        fragment_position[2] +=
            random_float_between(moby_breakup_z_offset_gp + 0.5f, moby_breakup_z_offset_gp + 4.5f);
        fast_vec_normalize(fragment_velocity, fragment_velocity, fragment_step_length);
        spawn_spinning_moby_fragment(source_moby, fragment_position, fragment_velocity);
    }
}

/* Draws the 64 quads of the backdrop mesh, each corner scaled from the shared vertex table. */
void draw_backdrop_mesh(void) {
    Q3003f0      quad;
    B3003f0      backdrop_draw_transform;
    unsigned int shared_quad_vertex_color;
    int          i, j, k;
    I3003f0     *quad_corner_index_pair;

    quad.tex = get_effect_texture(backdrop_mesh_effect_texture_index_gp);
    quad.unk88 = (long)backdrop_mesh_blend_color_a_selector_gp |
                 ((long)backdrop_mesh_blend_color_b_selector_gp << 2) |
                 ((long)backdrop_mesh_blend_alpha_c_selector_gp << 4) |
                 ((long)backdrop_mesh_blend_color_d_selector_gp << 6) |
                 ((long)backdrop_mesh_blend_fixed_alpha_gp << 32);
    quad.unk80 = 0xFF9000000260L;
    quad.unk70 = 0;
    set_identity_matrix(&backdrop_draw_transform);
    shared_quad_vertex_color = l06_backdrop_mesh_vertex_color;
    backdrop_draw_transform.f38 = moby_breakup_z_offset_gp;
    quad.color[3] = shared_quad_vertex_color;
    quad.color[2] = shared_quad_vertex_color;
    quad.color[1] = shared_quad_vertex_color;
    quad.color[0] = shared_quad_vertex_color;
    for (i = 0; i < 64; i++) {
        quad_corner_index_pair = &l06_backdrop_mesh_quad_corner_index_pairs[i * 4];
        for (j = 0; j < 4; j++) {
            k = quad_corner_index_pair->uv;
            fast_vec_scale(&quad.corner[j],
                           &l06_backdrop_mesh_vertex_positions[quad_corner_index_pair->v],
                           backdrop_mesh_vertex_position_scale_gp);
            quad_corner_index_pair++;
            quad.uv[j].u = l06_backdrop_mesh_texture_coordinates[k].u;
            quad.uv[j].v = l06_backdrop_mesh_texture_coordinates[k].v;
        }
        draw_geometry_quad(&quad, &backdrop_draw_transform, 0);
    }
}

void effect_quad_draw_single(void) {
    FRow_l06      quad_corner_positions[4];
    int           colors[4];
    float         uv[4][2];
    unsigned long pkt[4];
    float         backdrop_quad_transform_matrix[4][4];
    FVec4_l06     reference_to_backdrop_quad_unit_direction;
    FVec4_l06     center;
    float        *mp;
    int           i;
    int           j;

    pkt[1] = get_effect_texture(backdrop_mesh_effect_texture_index_gp);
    mp = backdrop_quad_transform_matrix[0];
    pkt[2] = 0xFF9000000260;
    pkt[3] = backdrop_mesh_blend_color_a_selector_gp | (long)backdrop_mesh_blend_color_b_selector_gp
                                                           << 2;
    pkt[3] |= (long)backdrop_mesh_blend_alpha_c_selector_gp << 4;
    pkt[3] |= (long)backdrop_mesh_blend_color_d_selector_gp << 6;
    pkt[3] |= (long)backdrop_mesh_blend_fixed_alpha_gp << 32;
    pkt[0] = 0;
    set_identity_matrix(mp);
    backdrop_quad_transform_matrix[3][2] = moby_breakup_z_offset_gp;
    colors[3] = l06_backdrop_mesh_vertex_color;
    colors[2] = l06_backdrop_mesh_vertex_color;
    colors[1] = l06_backdrop_mesh_vertex_color;
    colors[0] = l06_backdrop_mesh_vertex_color;
    for (i = 0; i < 96; i++) {
        fast_vec_scale(center, &D_L06_001FAFF0[i], backdrop_mesh_vertex_position_scale_gp);
        transform_vector(center, center, backdrop_quad_transform_matrix);
        fast_vec_sub(reference_to_backdrop_quad_unit_direction, center, D_L06_00167540);
        fast_vec_normalize(reference_to_backdrop_quad_unit_direction,
                           reference_to_backdrop_quad_unit_direction,
                           1.0f);
        transform_vector_by_basis(center, &D_L06_001FA2F0[i], mp);
        if (fast_vec_dot(reference_to_backdrop_quad_unit_direction, center) > 0.0f)
            continue;
        for (j = 0; j < 4; j++) {
            int quad_source_vertex_index = D_L06_001F7EB0[i][j][0];
            int quad_source_uv_index = D_L06_001F7EB0[i][j][1];
            fast_vec_scale(&quad_corner_positions[j],
                           &D_L06_001F77B0[quad_source_vertex_index],
                           backdrop_mesh_vertex_position_scale_gp);
            uv[j][0] = D_L06_001F84B0[quad_source_uv_index][0];
            uv[j][1] = D_L06_001F84B0[quad_source_uv_index][1];
        }
        draw_geometry_quad(quad_corner_positions, backdrop_quad_transform_matrix, 0);
    }
}
