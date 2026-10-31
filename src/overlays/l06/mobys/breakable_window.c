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

#include "rnc/overlay/l06/mobys/breakable_window_data/breakable_window_update.h"

extern float camera_sequence_rotation_x_degrees_gp __asm__("D_L06_00162050") __attribute__((sda));
void         breakable_window_update(char *moby) __asm__("FUN_L06_002ffcb0");
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
    { 0, 398},
    { 1, 399},
    { 3, 396},
    { 2, 397},
    { 1, 394},
    { 4, 395},
    { 2, 392},
    { 5, 393},
    { 4, 390},
    { 6, 391},
    { 5, 388},
    { 7, 389},
    { 6, 386},
    { 8, 387},
    { 7, 384},
    { 9, 385},
    { 8, 382},
    {10, 383},
    { 9, 380},
    {11, 381},
    {10, 378},
    {12, 379},
    {11, 376},
    {13, 377},
    {12, 374},
    {14, 375},
    {13, 372},
    {15, 373},
    {14, 370},
    {16, 371},
    {15, 368},
    {17, 369},
    {16, 366},
    {18, 367},
    {17, 364},
    {19, 365},
    {18, 362},
    {20, 363},
    {19, 360},
    {21, 361},
    {20, 358},
    {22, 359},
    {21, 356},
    {23, 357},
    {22, 354},
    {24, 355},
    {23, 352},
    {25, 353},
    {24, 350},
    {26, 351},
    {25, 348},
    {27, 349},
    {26, 346},
    {28, 347},
    {27, 344},
    {29, 345},
    {28, 342},
    {30, 343},
    {29, 340},
    {31, 341},
    {30, 338},
    { 0, 339},
    {31, 336},
    { 3, 337},
    {32, 270},
    {33, 271},
    {35, 268},
    {34, 269},
    {33, 266},
    {36, 267},
    {34, 264},
    {37, 265},
    {36, 262},
    {38, 263},
    {37, 260},
    {39, 261},
    {38, 258},
    {40, 259},
    {39, 256},
    {41, 257},
    {40, 254},
    {42, 255},
    {41, 252},
    {43, 253},
    {42, 250},
    {44, 251},
    {43, 248},
    {45, 249},
    {44, 246},
    {46, 247},
    {45, 244},
    {47, 245},
    {46, 242},
    {48, 243},
    {47, 240},
    {49, 241},
    {48, 238},
    {50, 239},
    {49, 236},
    {51, 237},
    {50, 234},
    {52, 235},
    {51, 232},
    {53, 233},
    {52, 230},
    {54, 231},
    {53, 228},
    {55, 229},
    {54, 226},
    {56, 227},
    {55, 224},
    {57, 225},
    {56, 222},
    {58, 223},
    {57, 220},
    {59, 221},
    {58, 218},
    {60, 219},
    {59, 216},
    {61, 217},
    {60, 214},
    {62, 215},
    {61, 212},
    {63, 213},
    {62, 210},
    {32, 211},
    {63, 208},
    {35, 209},
    {35, 202},
    {34, 203},
    {65, 200},
    {64, 201},
    {34, 198},
    {37, 199},
    {64, 196},
    {66, 197},
    {37, 194},
    {39, 195},
    {66, 192},
    {67, 193},
    {39, 190},
    {41, 191},
    {67, 188},
    {68, 189},
    {41, 186},
    {43, 187},
    {68, 184},
    {69, 185},
    {43, 182},
    {45, 183},
    {69, 180},
    {70, 181},
    {45, 178},
    {47, 179},
    {70, 176},
    {71, 177},
    {47, 174},
    {49, 175},
    {71, 172},
    {72, 173},
    {49, 170},
    {51, 171},
    {72, 168},
    {73, 169},
    {51, 166},
    {53, 167},
    {73, 164},
    {74, 165},
    {53, 162},
    {55, 163},
    {74, 160},
    {75, 161},
    {55, 158},
    {57, 159},
    {75, 156},
    {76, 157},
    {57, 154},
    {59, 155},
    {76, 152},
    {77, 153},
    {59, 150},
    {61, 151},
    {77, 148},
    {78, 149},
    {61, 146},
    {63, 147},
    {78, 144},
    {79, 145},
    {63, 206},
    {35, 207},
    {79, 204},
    {65, 205},
    {31, 334},
    { 3, 335},
    {81, 332},
    {80, 333},
    { 3, 330},
    { 2, 331},
    {80, 328},
    {82, 329},
    { 2, 326},
    { 5, 327},
    {82, 324},
    {83, 325},
    { 5, 322},
    { 7, 323},
    {83, 320},
    {84, 321},
    { 7, 318},
    { 9, 319},
    {84, 316},
    {85, 317},
    { 9, 314},
    {11, 315},
    {85, 312},
    {86, 313},
    {11, 310},
    {13, 311},
    {86, 308},
    {87, 309},
    {13, 306},
    {15, 307},
    {87, 304},
    {88, 305},
    {15, 302},
    {17, 303},
    {88, 300},
    {89, 301},
    {17, 298},
    {19, 299},
    {89, 296},
    {90, 297},
    {19, 294},
    {21, 295},
    {90, 292},
    {91, 293},
    {21, 290},
    {23, 291},
    {91, 288},
    {92, 289},
    {23, 286},
    {25, 287},
    {92, 284},
    {93, 285},
    {25, 282},
    {27, 283},
    {93, 280},
    {94, 281},
    {27, 278},
    {29, 279},
    {94, 276},
    {95, 277},
    {29, 274},
    {31, 275},
    {95, 272},
    {81, 273},
};

extern UV3003f0 l06_backdrop_mesh_texture_coordinates[] __asm__("D_L06_001F38B0");

/* Draws the 96 quads of the second backdrop mesh, skipping those that face away from D_L06_00167540. */
typedef float FVec4_l06[4] __attribute__((aligned(16)));

typedef struct {
    float v[4];
} __attribute__((aligned(16))) FRow_l06;

/* One quad centre and one quad normal for each of the 96 quads effect_quad_draw_single draws. */
FRow_l06 l06_second_backdrop_mesh_quad_centres[96] __asm__("D_L06_001FAFF0") = {
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

FRow_l06 l06_second_backdrop_mesh_quad_normals[96] __asm__("D_L06_001FA2F0") = {
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
short l06_second_backdrop_mesh_quad_corner_indices[96][4][2] __asm__("D_L06_001F7EB0") = {
    { {0, 954},  {1, 955},   {3, 952},   {2, 953}},
    { {1, 950},  {4, 951},   {2, 948},   {5, 949}},
    { {4, 946},  {6, 947},   {5, 944},   {7, 945}},
    { {6, 942},  {8, 943},   {7, 940},   {9, 941}},
    { {8, 938}, {10, 939},   {9, 936},  {11, 937}},
    {{10, 934}, {12, 935},  {11, 932},  {13, 933}},
    {{12, 930}, {14, 931},  {13, 928},  {15, 929}},
    {{14, 926}, {16, 927},  {15, 924},  {17, 925}},
    {{16, 922}, {18, 923},  {17, 920},  {19, 921}},
    {{18, 918}, {20, 919},  {19, 916},  {21, 917}},
    {{20, 914}, {22, 915},  {21, 912},  {23, 913}},
    {{22, 910}, {24, 911},  {23, 908},  {25, 909}},
    {{24, 906}, {26, 907},  {25, 904},  {27, 905}},
    {{26, 958}, {28, 959},  {27, 956},  {29, 957}},
    {{28, 962}, {30, 963},  {29, 960},  {31, 961}},
    {{30, 966},  {0, 967},  {31, 964},   {3, 965}},
    {{32, 818}, {33, 819},  {35, 816},  {34, 817}},
    {{33, 814}, {36, 815},  {34, 812},  {37, 813}},
    {{36, 810}, {38, 811},  {37, 808},  {39, 809}},
    {{38, 806}, {40, 807},  {39, 804},  {41, 805}},
    {{40, 802}, {42, 803},  {41, 800},  {43, 801}},
    {{42, 798}, {44, 799},  {43, 796},  {45, 797}},
    {{44, 794}, {46, 795},  {45, 792},  {47, 793}},
    {{46, 790}, {48, 791},  {47, 788},  {49, 789}},
    {{48, 786}, {50, 787},  {49, 784},  {51, 785}},
    {{50, 782}, {52, 783},  {51, 780},  {53, 781}},
    {{52, 778}, {54, 779},  {53, 776},  {55, 777}},
    {{54, 838}, {56, 839},  {55, 836},  {57, 837}},
    {{56, 834}, {58, 835},  {57, 832},  {59, 833}},
    {{58, 830}, {60, 831},  {59, 828},  {61, 829}},
    {{60, 826}, {62, 827},  {61, 824},  {63, 825}},
    {{62, 822}, {32, 823},  {63, 820},  {35, 821}},
    {{64, 682}, {65, 683},  {67, 680},  {66, 681}},
    {{65, 678}, {68, 679},  {66, 676},  {69, 677}},
    {{68, 674}, {70, 675},  {69, 672},  {71, 673}},
    {{70, 670}, {72, 671},  {71, 668},  {73, 669}},
    {{72, 666}, {74, 667},  {73, 664},  {75, 665}},
    {{74, 662}, {76, 663},  {75, 660},  {77, 661}},
    {{76, 658}, {78, 659},  {77, 656},  {79, 657}},
    {{78, 654}, {80, 655},  {79, 652},  {81, 653}},
    {{80, 650}, {82, 651},  {81, 648},  {83, 649}},
    {{82, 710}, {84, 711},  {83, 708},  {85, 709}},
    {{84, 706}, {86, 707},  {85, 704},  {87, 705}},
    {{86, 702}, {88, 703},  {87, 700},  {89, 701}},
    {{88, 698}, {90, 699},  {89, 696},  {91, 697}},
    {{90, 694}, {92, 695},  {91, 692},  {93, 693}},
    {{92, 690}, {94, 691},  {93, 688},  {95, 689}},
    {{94, 686}, {64, 687},  {95, 684},  {67, 685}},
    {{67, 614}, {66, 615},  {97, 612},  {96, 613}},
    {{66, 610}, {69, 611},  {96, 608},  {98, 609}},
    {{69, 606}, {71, 607},  {98, 604},  {99, 605}},
    {{71, 602}, {73, 603},  {99, 600}, {100, 601}},
    {{73, 598}, {75, 599}, {100, 596}, {101, 597}},
    {{75, 594}, {77, 595}, {101, 592}, {102, 593}},
    {{77, 590}, {79, 591}, {102, 588}, {103, 589}},
    {{79, 586}, {81, 587}, {103, 584}, {104, 585}},
    {{81, 646}, {83, 647}, {104, 644}, {105, 645}},
    {{83, 642}, {85, 643}, {105, 640}, {106, 641}},
    {{85, 638}, {87, 639}, {106, 636}, {107, 637}},
    {{87, 634}, {89, 635}, {107, 632}, {108, 633}},
    {{89, 630}, {91, 631}, {108, 628}, {109, 629}},
    {{91, 626}, {93, 627}, {109, 624}, {110, 625}},
    {{93, 622}, {95, 623}, {110, 620}, {111, 621}},
    {{95, 618}, {67, 619}, {111, 616},  {97, 617}},
    {{31, 890},  {3, 891},  {62, 888},  {32, 889}},
    { {3, 886},  {2, 887},  {32, 884},  {33, 885}},
    { {2, 882},  {5, 883},  {33, 880},  {36, 881}},
    { {5, 878},  {7, 879},  {36, 876},  {38, 877}},
    { {7, 874},  {9, 875},  {38, 872},  {40, 873}},
    { {9, 870}, {11, 871},  {40, 868},  {42, 869}},
    {{11, 866}, {13, 867},  {42, 864},  {44, 865}},
    {{13, 862}, {15, 863},  {44, 860},  {46, 861}},
    {{15, 858}, {17, 859},  {46, 856},  {48, 857}},
    {{17, 854}, {19, 855},  {48, 852},  {50, 853}},
    {{19, 850}, {21, 851},  {50, 848},  {52, 849}},
    {{21, 846}, {23, 847},  {52, 844},  {54, 845}},
    {{23, 842}, {25, 843},  {54, 840},  {56, 841}},
    {{25, 902}, {27, 903},  {56, 900},  {58, 901}},
    {{27, 898}, {29, 899},  {58, 896},  {60, 897}},
    {{29, 894}, {31, 895},  {60, 892},  {62, 893}},
    {{63, 754}, {35, 755},  {94, 752},  {64, 753}},
    {{35, 750}, {34, 751},  {64, 748},  {65, 749}},
    {{34, 746}, {37, 747},  {65, 744},  {68, 745}},
    {{37, 742}, {39, 743},  {68, 740},  {70, 741}},
    {{39, 738}, {41, 739},  {70, 736},  {72, 737}},
    {{41, 734}, {43, 735},  {72, 732},  {74, 733}},
    {{43, 730}, {45, 731},  {74, 728},  {76, 729}},
    {{45, 726}, {47, 727},  {76, 724},  {78, 725}},
    {{47, 722}, {49, 723},  {78, 720},  {80, 721}},
    {{49, 718}, {51, 719},  {80, 716},  {82, 717}},
    {{51, 714}, {53, 715},  {82, 712},  {84, 713}},
    {{53, 774}, {55, 775},  {84, 772},  {86, 773}},
    {{55, 770}, {57, 771},  {86, 768},  {88, 769}},
    {{57, 766}, {59, 767},  {88, 764},  {90, 765}},
    {{59, 762}, {61, 763},  {90, 760},  {92, 761}},
    {{61, 758}, {63, 759},  {92, 756},  {94, 757}},
};

extern char D_0013F3D0[] NOT_SDA;

/* Moby class 1083 */
void breakable_window_update(char *moby) {
    enum {
        INITIALIZE_DRAW_SELECTION = 0,
        WAIT_FOR_LINKED_MOBY_TRIGGER = 1,
        WAIT_CAMERA_START_DELAY = 2,
        BLEND_CAMERA_TO_HERO = 3,
        WAIT_FOR_NEARBY_HERO = 4
    };
    int  *camera_sequence_and_draw_vars = ((int *)MOBY(moby)->pvars);
    float camera_position[4], camera_setup_rotation_or_position_target[4], from[4], target[4],
        offset[4];
    int base;
    int state;
    MOBY(moby)->scale =
        *(float *)(((char *)MOBY(moby)->pclass) + 0x24) * camera_sequence_moby_scale_multiplier_gp;
    switch ((unsigned char)moby[0x20]) {
    case INITIALIZE_DRAW_SELECTION:
        camera_sequence_and_draw_vars[1] = 1;
        moby[0x20] = 1;
        break;
    case WAIT_FOR_LINKED_MOBY_TRIGGER:
        if ((camera_sequence_and_draw_vars[2] < 0 ||
             (unsigned char)D_L06_0015FFD8_ffcb0[(camera_sequence_and_draw_vars[2] << 8) + 0xbc] ==
                 0) &&
            camera_sequence_and_draw_vars[8] != -1) {
            break;
        }
        if (vector_distance_xy(moby + 0x10, D_0013F3D0) < 32.0f) {
            moby[0x20] = 2;
            camera_sequence_and_draw_vars[9] = scale_game_frames(30);
        } else {
            spawn_moby_breakup_effect(moby);
            moby[0x20] = 4;
        }
        break;
    case WAIT_CAMERA_START_DELAY:
        if (fast_dec_timer(camera_sequence_and_draw_vars + 9)) {
            {
                int state;
                int base;
                moby[0x20] = 3;
                scripted_camera_sequence_active = 1;
                D_L06_0015F408 = 0x12;
                state = camera_sequence_and_draw_vars[5];
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
                camera_sequence_and_draw_vars[6] = 0;
                {
                    int base;
                    base = camera_sequence_and_draw_vars[4];
                    base <<= 7;
                    base += (int)l06_cuboids;
                    facing = fast_arc_tan(MOBY(moby)->pos.x - *(float *)(base + 0x30),
                                          MOBY(moby)->pos.y - *(float *)(base + 0x34));
                }
                {
                    int state;
                    state = camera_sequence_and_draw_vars[4];
                    state <<= 7;
                    base = (int)l06_cuboids;
                    *(float *)(state + base + 0x78) = facing;
                }
                state = camera_sequence_and_draw_vars[4];
                state <<= 7;
                set_race_camera(
                    (void *)(base + state + 0x30), (void *)(base + state + 0x70), 0x72, 0);
            }
        }
        if (camera_sequence_and_draw_vars[9] < camera_sequence_scalar_ramp_duration_frames_gp)
            D_L06_0015F3FC = 1.0f - convert_integer_to_float(camera_sequence_and_draw_vars[9]) /
                                        (float)camera_sequence_scalar_ramp_duration_frames_gp;
        break;
    case BLEND_CAMERA_TO_HERO:
        if (camera_sequence_and_draw_vars[9] < camera_sequence_scalar_ramp_duration_frames_gp) {
            camera_sequence_and_draw_vars[9]++;
            D_L06_0015F3FC = 1.0f - convert_integer_to_float(camera_sequence_and_draw_vars[9]) /
                                        (float)camera_sequence_scalar_ramp_duration_frames_gp;
        }
        /* This field holds float bits; retail completes only on an ordered >= comparison. */
        if (*(float *)(camera_sequence_and_draw_vars + 6) >= 0.99999f) {
            moby[0x20] = 5;
            hero_set_state(0, 0);
            FUN_L06_00312ce8(2);
            scripted_camera_sequence_active = 0;
        } else {
            if (*(float *)(camera_sequence_and_draw_vars + 6) >= 0.05f &&
                camera_sequence_and_draw_vars[3] >= 0) {
                {
                    int state;
                    int base;
                    state = camera_sequence_and_draw_vars[3];
                    state <<= 7;
                    base = (int)l06_cuboids;
                    qcopy((void *)(state + base + 0x30), D_0013F3D0);
                    *(float *)(state + base + 0x38) -= 0.5f;
                }
                camera_sequence_and_draw_vars[3] = -1;
            }
            if (camera_sequence_and_draw_vars[1] == 1 &&
                vector_distance_xy(moby + 0x10,
                                   D_L06_0015FFD8_ffcb0 + (camera_sequence_and_draw_vars[8] << 8) +
                                       0x10) > 0.1f) {
                allocate_voice_for_target_entry(0, 0, moby);
                spawn_moby_breakup_effect(moby);
            }
            advance_accelerated_scalar((float *)(camera_sequence_and_draw_vars + 6),
                                       (float *)(camera_sequence_and_draw_vars + 7),
                                       1.0f,
                                       frame_time_sq,
                                       frame_time_sq,
                                       frame_time * 1.5f);
            {
                int state;
                int base;
                state = camera_sequence_and_draw_vars[5];
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
                        *(float *)(camera_sequence_and_draw_vars + 6));
            lerp_vector(from, from, target, *(float *)(camera_sequence_and_draw_vars + 6));
            set_camera_position(camera_position);
            set_camera_rotation(from);
        }
        break;
    case WAIT_FOR_NEARBY_HERO:
        if (vector_distance_xy(moby + 0x10, D_0013F3D0) < 16.0f) {
            state = camera_sequence_and_draw_vars[3];
            state <<= 7;
            base = (int)l06_cuboids;
            qcopy((void *)(state + base + 0x30), D_0013F3D0);
            *(float *)(state + base + 0x38) -= 0.5f;
            camera_sequence_and_draw_vars[3] = -1;
            moby[0x20] = 5;
        }
        break;
    }
    if (camera_sequence_and_draw_vars[1] != 0) {
        if (camera_sequence_and_draw_vars[0] != 0)
            FUN_L06_00210fc8((int)effect_quad_draw_single, moby);
        else
            FUN_L06_00210fc8((int)FUN_L06_00300958, moby);
    } else {
        if (camera_sequence_and_draw_vars[0] != 0)
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
        fast_vec_scale(center,
                       &l06_second_backdrop_mesh_quad_centres[i],
                       backdrop_mesh_vertex_position_scale_gp);
        transform_vector(center, center, backdrop_quad_transform_matrix);
        fast_vec_sub(reference_to_backdrop_quad_unit_direction, center, D_L06_00167540);
        fast_vec_normalize(reference_to_backdrop_quad_unit_direction,
                           reference_to_backdrop_quad_unit_direction,
                           1.0f);
        transform_vector_by_basis(center, &l06_second_backdrop_mesh_quad_normals[i], mp);
        if (fast_vec_dot(reference_to_backdrop_quad_unit_direction, center) > 0.0f)
            continue;
        for (j = 0; j < 4; j++) {
            int quad_source_vertex_index = l06_second_backdrop_mesh_quad_corner_indices[i][j][0];
            int quad_source_uv_index = l06_second_backdrop_mesh_quad_corner_indices[i][j][1];
            fast_vec_scale(&quad_corner_positions[j],
                           &D_L06_001F77B0[quad_source_vertex_index],
                           backdrop_mesh_vertex_position_scale_gp);
            uv[j][0] = D_L06_001F84B0[quad_source_uv_index][0];
            uv[j][1] = D_L06_001F84B0[quad_source_uv_index][1];
        }
        draw_geometry_quad(quad_corner_positions, backdrop_quad_transform_matrix, 0);
    }
}
