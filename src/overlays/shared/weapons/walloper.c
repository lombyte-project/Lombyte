/* Shared moby class walloper: the punching glove held by the hero, and its arc and quad effects. */

#include "types.h"
#include "rnc/game/item_state.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/math/random.h"
#include "rnc/rendering/vu1_registers.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/math/random/random_spherical_offset.h"
#include "rnc/overlay/shared/weapons/walloper_data/walloper_update_2.h"
#include "rnc/overlay/shared/weapons/walloper_data/walloper_update.h"

typedef struct {
    float previous[4];
    float point[4][4];
} SparkleRowC860;

/* One arc: its start point and four bent points toward the D_L00_001DBB90 row. */
typedef struct {
    float point[5][4];
} SparkleArc;

typedef float RW[4] __attribute__((aligned(16)));

typedef struct {
    int   c[4];
    float f[8];
    long  z4, q0, q1, q2;
} RT;

typedef struct {
    float f[4];
} __attribute__((aligned(16))) V_2bc1a0;

typedef struct {
    V_2bc1a0     corner[4];
    unsigned int color[4];
    int          unk50, unk54, unk58;
    float        unk5C, unk60;
    int          unk64;
    float        unk68, unk6C;
    long         unk70, tex, unk80, unk88;
} Q_2bc1a0;

extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern void  fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void  fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void  fast_vec_cross(void *, void *, void *) __asm__("FUN_001f9ad8");
extern float convert_integer_to_float(int) __asm__("FUN_001fa6c0");
extern int   truncate_float_to_s32(float) __asm__("FUN_001fa6d0");
extern int   scale_game_frames(int) __asm__("FUN_001f96f8");
extern long  get_effect_texture(int) __asm__("FUN_001f44b8");
extern void  draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");
void         transform_vector(void *, void *, void *) __asm__("FUN_001f9d20");
extern float FUN_L00_00257c48(float lo, float hi);
extern void  FUN_L00_002bbb00_c(int) __asm__("FUN_L00_002bbb00");
extern void
build_look_at_matrix(void *dst, void *vec, void *axis, float angle) __asm__("FUN_00214890");
extern void scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");
extern int  func_001F9908_r(void *) __asm__("FUN_001f9740");
void        draw_walloper_arc_two_pass_quads(void *, s32, s32, s32, f32, f32, f32) __asm__("FUN_L00_002bbbc8");
void        draw_walloper_camera_facing_quad(void *mv) __asm__("FUN_L00_002bc1a0");
extern void draw_geometry_quad(void *, void *, int);
void        FUN_001f9a68_2bc1a0(float, void *, void *) __asm__("FUN_001f9a68");
void        FUN_001f9bf8_2bc1a0(void *, void *, float) __asm__("FUN_001f9bf8");
float       FUN_001fa6c0_2bc1a0(int) __asm__("FUN_001fa6c0");
int         FUN_001fa6d0_2bc1a0(float) __asm__("FUN_001fa6d0");

extern float                g_debug_camera[] __asm__("D_L00_00166DC0");
extern float                g_debug_camera[] __asm__("D_L00_00166DC0") NOT_SDA;
extern char                 D_0013F5E0[];
extern float                D_L00_00166DC0[];
extern float D_L00_0017AEBC NOT_SDA;
extern int                  D_L00_001616CC;
extern float l00_walloper_arc_step_sample_bound __asm__("D_L00_001616A4") __attribute__((sda));
extern short D_L00_00161660_macro[] __asm__("D_L00_00161660") MACRO_ADDR;
extern char  walloper_effect_fade_in_duration_frames[4] __asm__("D_L00_001616A8")
    __attribute__((sda));
extern char walloper_effect_fade_out_duration_frames[4] __asm__("D_L00_001616AC")
    __attribute__((sda));
extern short          D_L00_001DBEF0[];
extern short          D_L00_001DBF08[];
extern SparkleRowC860 l00_walloper_arc_predecessor_rows[] __asm__("D_L00_001DBB90");
extern f32            D_L00_00161660_f[] __asm__("D_L00_00161660") MACRO_ADDR;
extern f32  walloper_arc_first_pass_half_width_gp __asm__("D_L00_00161690") __attribute__((sda));
extern f32  walloper_arc_second_pass_half_width_gp __asm__("D_L00_00161694") __attribute__((sda));
extern char walloper_arc_first_pass_base_vertex_color_gp[4] __asm__("D_L00_00161680")
    __attribute__((sda));
extern char walloper_arc_second_pass_base_vertex_color_gp[4] __asm__("D_L00_00161684")
    __attribute__((sda));
extern char walloper_arc_first_pass_texture_index_gp[4] __asm__("D_L00_001616B0")
    __attribute__((sda));
extern char walloper_arc_second_pass_texture_index_gp[4] __asm__("D_L00_001616B4")
    __attribute__((sda));
extern float walloper_quad_basis_offset_scale_gp __asm__("D_L00_001616BC") __attribute__((sda));
extern float walloper_quad_corner_scale_gp __asm__("D_L00_001616B8") __attribute__((sda));
extern int   walloper_quad_texture_index_gp __asm__("D_L00_001616C8") __attribute__((sda));
extern unsigned int l00_walloper_quad_common_base_vertex_color __asm__("D_L00_001616C0")
    __attribute__((sda));
extern unsigned int l00_walloper_quad_rare_base_vertex_color __asm__("D_L00_001616C4")
    __attribute__((sda));
extern V_2bc1a0 D_L00_001DBB60[] NOT_SDA;

SparkleArc D_L00_001DBBA0[10] = {0};

SparkleArc D_L01_001DC020[10] = {0};

SparkleArc D_L02_001D0620[10] = {0};

SparkleArc D_L03_001D0120[10] = {0};

SparkleArc D_L04_001D01A0[10] = {0};

SparkleArc D_L05_001D04C0[10] = {0};

SparkleArc D_L06_001D07A0[10] = {0};

SparkleArc D_L07_001D00A0[10] = {0};

SparkleArc D_L08_001D0820[10] = {0};

SparkleArc D_L09_001D01A0[10] = {0};

SparkleArc D_L10_001D04A0[10] = {0};

SparkleArc D_L11_001D0A20[10] = {0};

SparkleArc D_L12_001E30C0[10] = {0};

SparkleArc D_L13_001D0930[10] = {0};

SparkleArc D_L14_001DCFE0[10] = {0};

SparkleArc D_L15_001D0AA0[10] = {0};

SparkleArc D_L16_001D04A0[10] = {0};

SparkleArc D_L17_001D0D20[10] = {0};

SparkleArc D_L18_001D0A20[10] = {0};

short D_L00_001DBED8[10] = {0};

short D_L01_001DC358[10] = {0};

short D_L02_001D0958[10] = {0};

short D_L03_001D0458[10] = {0};

short D_L04_001D04D8[10] = {0};

short D_L05_001D07F8[10] = {0};

short D_L06_001D0AD8[10] = {0};

short D_L07_001D03D8[10] = {0};

short D_L08_001D0B58[10] = {0};

short D_L09_001D04D8[10] = {0};

short D_L10_001D07D8[10] = {0};

short D_L11_001D0D58[10] = {0};

short D_L12_001E33F8[10] = {0};

short D_L13_001D0C68[10] = {0};

short D_L14_001DD318[10] = {0};

short D_L15_001D0DD8[10] = {0};

short D_L16_001D07D8[10] = {0};

short D_L17_001D1058[10] = {0};

short D_L18_001D0D58[10] = {0};

short D_L00_001DBEC0[10] = {0};

short D_L01_001DC340[10] = {0};

short D_L02_001D0940[10] = {0};

short D_L03_001D0440[10] = {0};

short D_L04_001D04C0[10] = {0};

short D_L05_001D07E0[10] = {0};

short D_L06_001D0AC0[10] = {0};

short D_L07_001D03C0[10] = {0};

short D_L08_001D0B40[10] = {0};

short D_L09_001D04C0[10] = {0};

short D_L10_001D07C0[10] = {0};

short D_L11_001D0D40[10] = {0};

short D_L12_001E33E0[10] = {0};

short D_L13_001D0C50[10] = {0};

short D_L14_001DD300[10] = {0};

short D_L15_001D0DC0[10] = {0};

short D_L16_001D07C0[10] = {0};

short D_L17_001D1040[10] = {0};

short D_L18_001D0D40[10] = {0};

int   D_L01_001616FC = 0;
float D_L01_001616D4 = 0.2f;
char  D_L01_001616D8[4] = {5, 0, 0, 0};

char D_L01_001616DC[4] = {0x14, 0, 0, 0};

short D_L01_001DC370 NOT_SDA = 0;
short D_L01_001DC388 NOT_SDA = 0;
f32                  D_L01_001616C0 = 0.05f;
f32                  D_L01_001616C4 = 0.5f;
char                 D_L01_001616B0[4] = {-1, -1, -1, 0};

char D_L01_001616B4[4] = {0x20, 0x20, 0x7f, 0};

char D_L01_001616E0[4] = {0xe, 0, 0, 0};

char D_L01_001616E4[4] = {0x10, 0, 0, 0};

float        D_L01_001616EC = -0.5f;
float        D_L01_001616E8 = 0.75f;
int          D_L01_001616F8 = 8;
unsigned int D_L01_001616F0 = 0x307f4040;
unsigned int D_L01_001616F4 = 0x7f7f4040;

V_2bc1a0 D_L01_001DBFE0 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

int   D_L02_0016169C = 0;
float D_L02_00161674 = 0.2f;
char  D_L02_00161678[4] = {5, 0, 0, 0};

char D_L02_0016167C[4] = {0x14, 0, 0, 0};

short D_L02_001D0970 NOT_SDA = 0;
short D_L02_001D0988 NOT_SDA = 0;
f32                  D_L02_00161660 = 0.05f;
f32                  D_L02_00161664 = 0.5f;
char                 D_L02_00161650[4] = {-1, -1, -1, 0};

char D_L02_00161654[4] = {0x20, 0x20, 0x7f, 0};

char D_L02_00161680[4] = {0xe, 0, 0, 0};

char D_L02_00161684[4] = {0x10, 0, 0, 0};

float        D_L02_0016168C = -0.5f;
float        D_L02_00161688 = 0.75f;
int          D_L02_00161698 = 8;
unsigned int D_L02_00161690 = 0x307f4040;
unsigned int D_L02_00161694 = 0x7f7f4040;

V_2bc1a0 D_L02_001D05E0 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

int   D_L03_001616AC = 0;
float D_L03_00161684 = 0.2f;
char  D_L03_00161688[4] = {5, 0, 0, 0};

char D_L03_0016168C[4] = {0x14, 0, 0, 0};

short D_L03_001D0470 NOT_SDA = 0;
short D_L03_001D0488 NOT_SDA = 0;
f32                  D_L03_00161670 = 0.05f;
f32                  D_L03_00161674 = 0.5f;
char                 D_L03_00161660[4] = {-1, -1, -1, 0};

char D_L03_00161664[4] = {0x20, 0x20, 0x7f, 0};

char D_L03_00161690[4] = {0xe, 0, 0, 0};

char D_L03_00161694[4] = {0x10, 0, 0, 0};

float        D_L03_0016169C = -0.5f;
float        D_L03_00161698 = 0.75f;
int          D_L03_001616A8 = 8;
unsigned int D_L03_001616A0 = 0x307f4040;
unsigned int D_L03_001616A4 = 0x7f7f4040;

V_2bc1a0 D_L03_001D00E0 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

int   D_L04_0016169C = 0;
float D_L04_00161674 = 0.2f;
char  D_L04_00161678[4] = {5, 0, 0, 0};

char D_L04_0016167C[4] = {0x14, 0, 0, 0};

short D_L04_001D04F0 NOT_SDA = 0;
short D_L04_001D0508 NOT_SDA = 0;
f32                  D_L04_00161660 = 0.05f;
f32                  D_L04_00161664 = 0.5f;
char                 D_L04_00161650[4] = {-1, -1, -1, 0};

char D_L04_00161654[4] = {0x20, 0x20, 0x7f, 0};

char D_L04_00161680[4] = {0xe, 0, 0, 0};

char D_L04_00161684[4] = {0x10, 0, 0, 0};

float        D_L04_0016168C = -0.5f;
float        D_L04_00161688 = 0.75f;
int          D_L04_00161698 = 8;
unsigned int D_L04_00161690 = 0x307f4040;
unsigned int D_L04_00161694 = 0x7f7f4040;

V_2bc1a0 D_L04_001D0160 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

int   D_L05_001616EC = 0;
float D_L05_001616C4 = 0.2f;
char  D_L05_001616C8[4] = {5, 0, 0, 0};

char D_L05_001616CC[4] = {0x14, 0, 0, 0};

short D_L05_001D0810 NOT_SDA = 0;
short D_L05_001D0828 NOT_SDA = 0;
f32                  D_L05_001616B0 = 0.05f;
f32                  D_L05_001616B4 = 0.5f;
char                 D_L05_001616A0[4] = {-1, -1, -1, 0};

char D_L05_001616A4[4] = {0x20, 0x20, 0x7f, 0};

char D_L05_001616D0[4] = {0xe, 0, 0, 0};

char D_L05_001616D4[4] = {0x10, 0, 0, 0};

float        D_L05_001616DC = -0.5f;
float        D_L05_001616D8 = 0.75f;
int          D_L05_001616E8 = 8;
unsigned int D_L05_001616E0 = 0x307f4040;
unsigned int D_L05_001616E4 = 0x7f7f4040;

V_2bc1a0 D_L05_001D0480 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

int   D_L06_001616CC = 0;
float D_L06_001616A4 = 0.2f;
char  D_L06_001616A8[4] = {5, 0, 0, 0};

char D_L06_001616AC[4] = {0x14, 0, 0, 0};

short D_L06_001D0AF0 NOT_SDA = 0;
short D_L06_001D0B08 NOT_SDA = 0;
f32                  D_L06_00161690 = 0.05f;
f32                  D_L06_00161694 = 0.5f;
char                 D_L06_00161680[4] = {-1, -1, -1, 0};

char D_L06_00161684[4] = {0x20, 0x20, 0x7f, 0};

char D_L06_001616B0[4] = {0xe, 0, 0, 0};

char D_L06_001616B4[4] = {0x10, 0, 0, 0};

float        D_L06_001616BC = -0.5f;
float        D_L06_001616B8 = 0.75f;
int          D_L06_001616C8 = 8;
unsigned int D_L06_001616C0 = 0x307f4040;
unsigned int D_L06_001616C4 = 0x7f7f4040;

V_2bc1a0 D_L06_001D0760 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

int   D_L07_0016169C = 0;
float D_L07_00161674 = 0.2f;
char  D_L07_00161678[4] = {5, 0, 0, 0};

char D_L07_0016167C[4] = {0x14, 0, 0, 0};

short D_L07_001D03F0 NOT_SDA = 0;
short D_L07_001D0408 NOT_SDA = 0;
f32                  D_L07_00161660 = 0.05f;
f32                  D_L07_00161664 = 0.5f;
char                 D_L07_00161650[4] = {-1, -1, -1, 0};

char D_L07_00161654[4] = {0x20, 0x20, 0x7f, 0};

char D_L07_00161680[4] = {0xe, 0, 0, 0};

char D_L07_00161684[4] = {0x10, 0, 0, 0};

float        D_L07_0016168C = -0.5f;
float        D_L07_00161688 = 0.75f;
int          D_L07_00161698 = 8;
unsigned int D_L07_00161690 = 0x307f4040;
unsigned int D_L07_00161694 = 0x7f7f4040;

V_2bc1a0 D_L07_001D0060 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

int   D_L08_001616EC = 0;
float D_L08_001616C4 = 0.2f;
char  D_L08_001616C8[4] = {5, 0, 0, 0};

char D_L08_001616CC[4] = {0x14, 0, 0, 0};

short D_L08_001D0B70 NOT_SDA = 0;
short D_L08_001D0B88 NOT_SDA = 0;
f32                  D_L08_001616B0 = 0.05f;
f32                  D_L08_001616B4 = 0.5f;
char                 D_L08_001616A0[4] = {-1, -1, -1, 0};

char D_L08_001616A4[4] = {0x20, 0x20, 0x7f, 0};

char D_L08_001616D0[4] = {0xe, 0, 0, 0};

char D_L08_001616D4[4] = {0x10, 0, 0, 0};

float        D_L08_001616DC = -0.5f;
float        D_L08_001616D8 = 0.75f;
int          D_L08_001616E8 = 8;
unsigned int D_L08_001616E0 = 0x307f4040;
unsigned int D_L08_001616E4 = 0x7f7f4040;

V_2bc1a0 D_L08_001D07E0 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

int   D_L09_0016175C = 0;
float D_L09_00161734 = 0.2f;
char  D_L09_00161738[4] = {5, 0, 0, 0};

char D_L09_0016173C[4] = {0x14, 0, 0, 0};

short D_L09_001D04F0 NOT_SDA = 0;
short D_L09_001D0508 NOT_SDA = 0;
f32                  D_L09_00161720 = 0.05f;
f32                  D_L09_00161724 = 0.5f;
char                 D_L09_00161710[4] = {-1, -1, -1, 0};

char D_L09_00161714[4] = {0x20, 0x20, 0x7f, 0};

char D_L09_00161740[4] = {0xe, 0, 0, 0};

char D_L09_00161744[4] = {0x10, 0, 0, 0};

float        D_L09_0016174C = -0.5f;
float        D_L09_00161748 = 0.75f;
int          D_L09_00161758 = 8;
unsigned int D_L09_00161750 = 0x307f4040;
unsigned int D_L09_00161754 = 0x7f7f4040;

V_2bc1a0 D_L09_001D0160 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

int   D_L10_001616AC = 0;
float D_L10_00161684 = 0.2f;
char  D_L10_00161688[4] = {5, 0, 0, 0};

char D_L10_0016168C[4] = {0x14, 0, 0, 0};

short D_L10_001D07F0 NOT_SDA = 0;
short D_L10_001D0808 NOT_SDA = 0;
f32                  D_L10_00161670 = 0.05f;
f32                  D_L10_00161674 = 0.5f;
char                 D_L10_00161660[4] = {-1, -1, -1, 0};

char D_L10_00161664[4] = {0x20, 0x20, 0x7f, 0};

char D_L10_00161690[4] = {0xe, 0, 0, 0};

char D_L10_00161694[4] = {0x10, 0, 0, 0};

float        D_L10_0016169C = -0.5f;
float        D_L10_00161698 = 0.75f;
int          D_L10_001616A8 = 8;
unsigned int D_L10_001616A0 = 0x307f4040;
unsigned int D_L10_001616A4 = 0x7f7f4040;

V_2bc1a0 D_L10_001D0460 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

int   D_L11_0016172C = 0;
float D_L11_00161704 = 0.2f;
char  D_L11_00161708[4] = {5, 0, 0, 0};

char D_L11_0016170C[4] = {0x14, 0, 0, 0};

short D_L11_001D0D70 NOT_SDA = 0;
short D_L11_001D0D88 NOT_SDA = 0;
f32                  D_L11_001616F0 = 0.05f;
f32                  D_L11_001616F4 = 0.5f;
char                 D_L11_001616E0[4] = {-1, -1, -1, 0};

char D_L11_001616E4[4] = {0x20, 0x20, 0x7f, 0};

char D_L11_00161710[4] = {0xe, 0, 0, 0};

char D_L11_00161714[4] = {0x10, 0, 0, 0};

float        D_L11_0016171C = -0.5f;
float        D_L11_00161718 = 0.75f;
int          D_L11_00161728 = 8;
unsigned int D_L11_00161720 = 0x307f4040;
unsigned int D_L11_00161724 = 0x7f7f4040;

V_2bc1a0 D_L11_001D09E0 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

int   D_L12_001616AC = 0;
float D_L12_00161684 = 0.2f;
char  D_L12_00161688[4] = {5, 0, 0, 0};

char D_L12_0016168C[4] = {0x14, 0, 0, 0};

short D_L12_001E3410 NOT_SDA = 0;
short D_L12_001E3428 NOT_SDA = 0;
f32                  D_L12_00161670 = 0.05f;
f32                  D_L12_00161674 = 0.5f;
char                 D_L12_00161660[4] = {-1, -1, -1, 0};

char D_L12_00161664[4] = {0x20, 0x20, 0x7f, 0};

char D_L12_00161690[4] = {0xe, 0, 0, 0};

char D_L12_00161694[4] = {0x10, 0, 0, 0};

float        D_L12_0016169C = -0.5f;
float        D_L12_00161698 = 0.75f;
int          D_L12_001616A8 = 8;
unsigned int D_L12_001616A0 = 0x307f4040;
unsigned int D_L12_001616A4 = 0x7f7f4040;

V_2bc1a0 D_L12_001E3080 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

int   D_L13_0016177C = 0;
float D_L13_00161754 = 0.2f;
char  D_L13_00161758[4] = {5, 0, 0, 0};

char D_L13_0016175C[4] = {0x14, 0, 0, 0};

short D_L13_001D0C80 NOT_SDA = 0;
short D_L13_001D0C98 NOT_SDA = 0;
f32                  D_L13_00161740 = 0.05f;
f32                  D_L13_00161744 = 0.5f;
char                 D_L13_00161730[4] = {-1, -1, -1, 0};

char D_L13_00161734[4] = {0x20, 0x20, 0x7f, 0};

char D_L13_00161760[4] = {0xe, 0, 0, 0};

char D_L13_00161764[4] = {0x10, 0, 0, 0};

float        D_L13_0016176C = -0.5f;
float        D_L13_00161768 = 0.75f;
int          D_L13_00161778 = 8;
unsigned int D_L13_00161770 = 0x307f4040;
unsigned int D_L13_00161774 = 0x7f7f4040;

V_2bc1a0 D_L13_001D08F0 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

int   D_L14_001618BC = 0;
float D_L14_00161894 = 0.2f;
char  D_L14_00161898[4] = {5, 0, 0, 0};

char D_L14_0016189C[4] = {0x14, 0, 0, 0};

short D_L14_001DD330 NOT_SDA = 0;
short D_L14_001DD348 NOT_SDA = 0;
f32                  D_L14_00161880 = 0.05f;
f32                  D_L14_00161884 = 0.5f;
char                 D_L14_00161870[4] = {-1, -1, -1, 0};

char D_L14_00161874[4] = {0x20, 0x20, 0x7f, 0};

char D_L14_001618A0[4] = {0xe, 0, 0, 0};

char D_L14_001618A4[4] = {0x10, 0, 0, 0};

float        D_L14_001618AC = -0.5f;
float        D_L14_001618A8 = 0.75f;
int          D_L14_001618B8 = 8;
unsigned int D_L14_001618B0 = 0x307f4040;
unsigned int D_L14_001618B4 = 0x7f7f4040;

V_2bc1a0 D_L14_001DCFA0 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

int   D_L15_0016185C = 0;
float D_L15_00161834 = 0.2f;
char  D_L15_00161838[4] = {5, 0, 0, 0};

char D_L15_0016183C[4] = {0x14, 0, 0, 0};

short D_L15_001D0DF0 NOT_SDA = 0;
short D_L15_001D0E08 NOT_SDA = 0;
f32                  D_L15_00161820 = 0.05f;
f32                  D_L15_00161824 = 0.5f;
char                 D_L15_00161810[4] = {-1, -1, -1, 0};

char D_L15_00161814[4] = {0x20, 0x20, 0x7f, 0};

char D_L15_00161840[4] = {0xe, 0, 0, 0};

char D_L15_00161844[4] = {0x10, 0, 0, 0};

float        D_L15_0016184C = -0.5f;
float        D_L15_00161848 = 0.75f;
int          D_L15_00161858 = 8;
unsigned int D_L15_00161850 = 0x307f4040;
unsigned int D_L15_00161854 = 0x7f7f4040;

V_2bc1a0 D_L15_001D0A60 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

int   D_L16_0016175C = 0;
float D_L16_00161734 = 0.2f;
char  D_L16_00161738[4] = {5, 0, 0, 0};

char D_L16_0016173C[4] = {0x14, 0, 0, 0};

short D_L16_001D07F0 NOT_SDA = 0;
short D_L16_001D0808 NOT_SDA = 0;
f32                  D_L16_00161720 = 0.05f;
f32                  D_L16_00161724 = 0.5f;
char                 D_L16_00161710[4] = {-1, -1, -1, 0};

char D_L16_00161714[4] = {0x20, 0x20, 0x7f, 0};

char D_L16_00161740[4] = {0xe, 0, 0, 0};

char D_L16_00161744[4] = {0x10, 0, 0, 0};

float        D_L16_0016174C = -0.5f;
float        D_L16_00161748 = 0.75f;
int          D_L16_00161758 = 8;
unsigned int D_L16_00161750 = 0x307f4040;
unsigned int D_L16_00161754 = 0x7f7f4040;

V_2bc1a0 D_L16_001D0460 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

int   D_L17_001617AC = 0;
float D_L17_00161784 = 0.2f;
char  D_L17_00161788[4] = {5, 0, 0, 0};

char D_L17_0016178C[4] = {0x14, 0, 0, 0};

short D_L17_001D1070 NOT_SDA = 0;
short D_L17_001D1088 NOT_SDA = 0;
f32                  D_L17_00161770 = 0.05f;
f32                  D_L17_00161774 = 0.5f;
char                 D_L17_00161760[4] = {-1, -1, -1, 0};

char D_L17_00161764[4] = {0x20, 0x20, 0x7f, 0};

char D_L17_00161790[4] = {0xe, 0, 0, 0};

char D_L17_00161794[4] = {0x10, 0, 0, 0};

float        D_L17_0016179C = -0.5f;
float        D_L17_00161798 = 0.75f;
int          D_L17_001617A8 = 8;
unsigned int D_L17_001617A0 = 0x307f4040;
unsigned int D_L17_001617A4 = 0x7f7f4040;

V_2bc1a0 D_L17_001D0CE0 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

int   D_L18_0016173C = 0;
float D_L18_00161714 = 0.2f;
char  D_L18_00161718[4] = {5, 0, 0, 0};

char D_L18_0016171C[4] = {0x14, 0, 0, 0};

short D_L18_001D0D70 NOT_SDA = 0;
short D_L18_001D0D88 NOT_SDA = 0;
f32                  D_L18_00161700 = 0.05f;
f32                  D_L18_00161704 = 0.5f;
char                 D_L18_001616F0[4] = {-1, -1, -1, 0};

char D_L18_001616F4[4] = {0x20, 0x20, 0x7f, 0};

char D_L18_00161720[4] = {0xe, 0, 0, 0};

char D_L18_00161724[4] = {0x10, 0, 0, 0};

float        D_L18_0016172C = -0.5f;
float        D_L18_00161728 = 0.75f;
int          D_L18_00161738 = 8;
unsigned int D_L18_00161730 = 0x307f4040;
unsigned int D_L18_00161734 = 0x7f7f4040;

V_2bc1a0 D_L18_001D09E0 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

/* Ported from rac1-decomp (src/overlays/shared/vendor_002BA7C8.c: func_L00_002BC860), where it is exact; names translated to the US level program. */
void walloper_update(char *m) __asm__("FUN_L00_002bb558");

/* Moby class 180 */
void walloper_update(char *walloper_moby) {
    enum {
        WALLOPER_STATE_INITIALIZE = 0,
        WALLOPER_STATE_ANIMATION_DRIVEN_EFFECTS = 2
    };
    int          flag = 0;
    float        arc_origin[4] __attribute__((aligned(16)));
    float        arc_step[4] __attribute__((aligned(16)));
    float        arc_point_camera_offset[4] __attribute__((aligned(16)));
    float        arc_point_offsets[5][4] __attribute__((aligned(16)));
    struct Hero *walloper_hero_state = &hero;
    int          i;
    int          next_arc_index;
    int          j;
    int          arc_init_index;
    int          any;

    if (*(unsigned char *)(((char *)walloper_hero_state->moby) + 0x53) != 0x82) {
        D_L00_0017AEBC = 0.1f;
    }
    switch (MOBY(walloper_moby)->state) {
    case WALLOPER_STATE_INITIALIZE:
        walloper_moby[0x20] = 2;
        for (arc_init_index = 0; arc_init_index < 10; arc_init_index++) {
            D_L00_001DBEC0[arc_init_index] = 0;
            D_L00_001DBEF0[arc_init_index] = 2;
            D_L00_001DBF08[arc_init_index] = 1;
        }
        D_L00_001616CC = scale_game_frames(*(int *)&walloper_effect_fade_in_duration_frames);
        walloper_effect_fade_out_countdown =
            scale_game_frames(*(int *)&walloper_effect_fade_out_duration_frames);
        break;
    case WALLOPER_STATE_ANIMATION_DRIVEN_EFFECTS:
        if (walloper_hero_state->state.current == 0x20) {
            float interpolated_record_value = compute_interpolated_record_value(walloper_moby);
            if (6.0f <= interpolated_record_value) {
                flag = 1;
                if (func_001F9908_r(&D_L00_00161660_macro[54]) == 0) {
                    float a = convert_integer_to_float(D_L00_001616CC);
                    a = a / convert_integer_to_float(scale_game_frames(
                                *(int *)&walloper_effect_fade_in_duration_frames));
                    walloper_effect_alpha_scale = 1.0f - a;
                } else if (12.0f <= interpolated_record_value) {
                    if (func_001F9908_r(&D_L00_00161660_macro[56]) == 0) {
                        float a = convert_integer_to_float(walloper_effect_fade_out_countdown);
                        a = a / convert_integer_to_float(scale_game_frames(
                                    *(int *)&walloper_effect_fade_out_duration_frames));
                        walloper_effect_alpha_scale = a;
                    } else {
                        walloper_effect_alpha_scale = 0.0f;
                    }
                } else {
                    walloper_effect_alpha_scale = 1.0f;
                }
            }
        } else {
            D_L00_001616CC = scale_game_frames(*(int *)&walloper_effect_fade_in_duration_frames);
            walloper_effect_fade_out_countdown =
                scale_game_frames(*(int *)&walloper_effect_fade_out_duration_frames);
            blend_moby_animation(walloper_moby, 1, 0, scale_game_frames(12));
        }
    }
    scale_vector_xyz(arc_origin, walloper_moby + 0xD0, -0.5f);
    fast_vec_add(arc_origin, arc_origin, walloper_moby + 0x10);
    any = 0;
    for (j = 0; j < 10; j++) {
        if (D_L00_001DBEC0[j] != 0) {
            any = 1;
            break;
        }
    }
    if (!flag && !any)
        return;
    for (i = 0; i < 10; i = next_arc_index) {
        D_L00_001DBEC0[i]--;
        if (D_L00_001DBEC0[i] < 0)
            D_L00_001DBEC0[i] = 0;
        if (D_L00_001DBEC0[i] == 0) {
            if (flag) {
                float sign;
                int   k;
                D_L00_001DBEC0[i] = 2;
                D_L00_001DBED8[i] = 0x40;
                random_spherical_offset(arc_step,
                                        l00_walloper_arc_step_sample_bound,
                                        l00_walloper_arc_step_sample_bound);
                qcopy(D_L00_001DBBA0[i].point[0], arc_origin);
                next_arc_index = i + 1;
                sign = 1.0f;
                for (k = 1; k < 5; k++) {
                    float bend_angle;
                    fast_vec_sub(arc_point_camera_offset,
                                 l00_walloper_arc_predecessor_rows[i].point[k - 1],
                                 D_L00_00166DC0);
                    bend_angle = random_float_between(0.17453292f, 0.7853982f) * sign;
                    sign = -sign;
                    build_look_at_matrix(arc_step, arc_step, arc_point_camera_offset, bend_angle);
                    fast_vec_add(D_L00_001DBBA0[i].point[k],
                                 l00_walloper_arc_predecessor_rows[i].point[k - 1],
                                 arc_step);
                }
            } else {
                next_arc_index = i + 1;
            }
        } else {
            int k;
            float(*workspace)[4] = arc_point_offsets;
            for (k = 1; k < 5; k++) {
                float *point_offset = workspace[k];
                fast_vec_sub(point_offset,
                             D_L00_001DBBA0[i].point[k],
                             l00_walloper_arc_predecessor_rows[i].point[k - 1]);
                point_offset[0] += FUN_L00_00257c48(0.0f, 0.1f);
                point_offset[1] += FUN_L00_00257c48(0.0f, 0.1f);
                point_offset[2] += FUN_L00_00257c48(0.0f, 0.1f);
            }
            next_arc_index = i + 1;
            qcopy(D_L00_001DBBA0[i].point[0], arc_origin);
            for (k = 1; k < 5; k++) {
                fast_vec_add(D_L00_001DBBA0[i].point[k],
                             l00_walloper_arc_predecessor_rows[i].point[k - 1],
                             workspace[k]);
            }
            if (D_L00_001DBEC0[i] > 0)
                D_L00_001DBED8[i] = 0x20;
            else
                D_L00_001DBED8[i] = 0;
        }
    }
    add_draw_callback(FUN_L00_002bbb00_c, walloper_moby);
}

void draw_walloper_arc_and_quad_effects(void *moby) __asm__("FUN_L00_002bbb00");

void draw_walloper_arc_and_quad_effects(void *moby) {
    s32 i;
    s32 arc_alpha;
    for (i = 0; i < 10; i++) {
        if (D_L00_001DBEC0[i] != 0) {
            arc_alpha = truncate_float_to_s32(convert_integer_to_float(D_L00_001DBED8[i]) *
                                              D_L00_00161660_f[29]);
            draw_walloper_arc_two_pass_quads(&D_L00_001DBBA0[i],
                                             arc_alpha,
                                             arc_alpha,
                                             5,
                                             walloper_arc_first_pass_half_width_gp,
                                             walloper_arc_second_pass_half_width_gp,
                                             0.2f);
        }
    }
    draw_walloper_camera_facing_quad(moby);
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002BA7C8.c: func_L00_002BCED0), where it is exact; names translated to the US level program. */
void draw_walloper_arc_two_pass_quads(void *mv,
                                      int   first_pass_alpha,
                                      int   second_pass_alpha,
                                      int   point_count,
                                      float sx,
                                      float sy,

                                      float sz) {
    char *point_bytes = mv;
    RW    first_pass_corners[4];
    RT    first_pass_render_attributes;
    RW    q2[4];
    RT    second_pass_render_attributes;
    RW    arc_camera_facing_side_vector, second_pass_width_offset, first_pass_point_camera_offset,
        segment_delta, adjusted_segment_start, adjusted_segment_end, segment_start_camera_offset,
        segment_end_camera_offset, s1A0, s1B0;
    int   i, col0, col1;
    float side_vector_normalization_factor, initial_side_vector_normalization_factor,
        segment_start_camera_distance, segment_end_camera_distance;
    first_pass_render_attributes.q0 =
        get_effect_texture(*(int *)&walloper_arc_first_pass_texture_index_gp);
    col0 = *(int *)&walloper_arc_first_pass_base_vertex_color_gp | (first_pass_alpha << 24);
    first_pass_render_attributes.q2 = 0x8000000048L;
    first_pass_render_attributes.q1 = 0xFF9000000260L;
    first_pass_render_attributes.z4 = 0;
    first_pass_render_attributes.c[3] = col0;
    first_pass_render_attributes.c[2] = col0;
    first_pass_render_attributes.c[1] = col0;
    first_pass_render_attributes.c[0] = col0;
    second_pass_render_attributes.q0 =
        get_effect_texture(*(int *)&walloper_arc_second_pass_texture_index_gp);
    col1 = *(int *)&walloper_arc_second_pass_base_vertex_color_gp | (second_pass_alpha << 24);
    second_pass_render_attributes.q2 = first_pass_render_attributes.q2;
    second_pass_render_attributes.q1 = first_pass_render_attributes.q1;
    second_pass_render_attributes.z4 = first_pass_render_attributes.z4;
    second_pass_render_attributes.c[3] = col1;
    second_pass_render_attributes.c[2] = col1;
    second_pass_render_attributes.c[1] = col1;
    second_pass_render_attributes.c[0] = col1;
    second_pass_render_attributes.f[0] = 0;
    second_pass_render_attributes.f[1] = 0;
    second_pass_render_attributes.f[2] = 0;
    second_pass_render_attributes.f[3] = 1.0f;
    second_pass_render_attributes.f[4] = 1.0f;
    second_pass_render_attributes.f[5] = 0;
    second_pass_render_attributes.f[6] = 1.0f;
    second_pass_render_attributes.f[7] = 1.0f;
    first_pass_render_attributes.f[0] = 0;
    first_pass_render_attributes.f[1] = 0;
    first_pass_render_attributes.f[2] = 0;
    first_pass_render_attributes.f[3] = 1.0f;
    first_pass_render_attributes.f[4] = 1.0f;
    first_pass_render_attributes.f[5] = 0;
    first_pass_render_attributes.f[6] = 1.0f;
    first_pass_render_attributes.f[7] = 1.0f;
    fast_vec_sub(first_pass_point_camera_offset, point_bytes, D_L00_00166DC0);
    fast_vec_sub(segment_delta, point_bytes + 0x10, point_bytes);
    fast_vec_cross(arc_camera_facing_side_vector, segment_delta, first_pass_point_camera_offset);
    initial_side_vector_normalization_factor = fast_vec_length(arc_camera_facing_side_vector);
    if (initial_side_vector_normalization_factor != 0)
        initial_side_vector_normalization_factor = 1.0f / initial_side_vector_normalization_factor;
    scale_vector_xyz(arc_camera_facing_side_vector,
                     arc_camera_facing_side_vector,
                     sx * initial_side_vector_normalization_factor);
    fast_vec_add(first_pass_corners[0], point_bytes, arc_camera_facing_side_vector);
    fast_vec_sub(first_pass_corners[1], point_bytes, arc_camera_facing_side_vector);
    for (i = 1; i < point_count - 1; i++) {
        fast_vec_sub(first_pass_point_camera_offset, point_bytes + i * 16, D_L00_00166DC0);
        fast_vec_sub(segment_delta, point_bytes + i * 16 + 16, point_bytes + i * 16);
        fast_vec_cross(
            arc_camera_facing_side_vector, segment_delta, first_pass_point_camera_offset);
        side_vector_normalization_factor = fast_vec_length(arc_camera_facing_side_vector);
        if (side_vector_normalization_factor != 0)
            side_vector_normalization_factor = 1.0f / side_vector_normalization_factor;
        scale_vector_xyz(arc_camera_facing_side_vector,
                         arc_camera_facing_side_vector,
                         sx * side_vector_normalization_factor);
        fast_vec_add(first_pass_corners[2], point_bytes + i * 16, arc_camera_facing_side_vector);
        fast_vec_sub(first_pass_corners[3], point_bytes + i * 16, arc_camera_facing_side_vector);
        if (i == 1) {
            first_pass_render_attributes.c[1] =
                *(int *)&walloper_arc_first_pass_base_vertex_color_gp;
            first_pass_render_attributes.c[0] =
                *(int *)&walloper_arc_first_pass_base_vertex_color_gp;
        } else if (i == point_count - 2) {
            first_pass_render_attributes.c[3] =
                *(int *)&walloper_arc_first_pass_base_vertex_color_gp;
            first_pass_render_attributes.c[2] =
                *(int *)&walloper_arc_first_pass_base_vertex_color_gp;
        } else if (i == 2) {
            first_pass_render_attributes.c[1] = col0;
            first_pass_render_attributes.c[0] = col0;
        }
        draw_geometry_quad(first_pass_corners, 0, 0);
        qcopy(first_pass_corners[0], first_pass_corners[2]);
        qcopy(first_pass_corners[1], first_pass_corners[3]);
    }
    for (i = 0; i < point_count - 1; i++) {
        fast_vec_sub(segment_start_camera_offset, point_bytes + i * 16, D_L00_00166DC0);
        fast_vec_sub(segment_end_camera_offset, point_bytes + i * 16 + 16, D_L00_00166DC0);
        segment_start_camera_distance = fast_vec_length(segment_start_camera_offset);
        segment_end_camera_distance = fast_vec_length(segment_end_camera_offset);
        if (segment_end_camera_distance < segment_start_camera_distance) {
            if (segment_end_camera_distance == 0)
                qcopy(segment_end_camera_offset, segment_start_camera_offset);
            else
                scale_vector_xyz(segment_end_camera_offset,
                                 segment_end_camera_offset,
                                 segment_start_camera_distance / segment_end_camera_distance);
        } else {
            if (segment_start_camera_distance == 0)
                qcopy(segment_start_camera_offset, segment_end_camera_offset);
            else
                scale_vector_xyz(segment_start_camera_offset,
                                 segment_start_camera_offset,
                                 segment_end_camera_distance / segment_start_camera_distance);
        }
        fast_vec_add(adjusted_segment_start, D_L00_00166DC0, segment_start_camera_offset);
        fast_vec_add(adjusted_segment_end, D_L00_00166DC0, segment_end_camera_offset);
        fast_vec_sub(segment_delta, adjusted_segment_end, adjusted_segment_start);
        scale_vector_xyz(s1B0, segment_delta, sz);
        fast_vec_sub(adjusted_segment_start, adjusted_segment_start, s1B0);
        fast_vec_add(adjusted_segment_end, adjusted_segment_end, s1B0);
        fast_vec_sub(s1A0, D_L00_00166DC0, adjusted_segment_start);
        fast_vec_cross(arc_camera_facing_side_vector, s1A0, segment_delta);
        side_vector_normalization_factor = fast_vec_length(arc_camera_facing_side_vector);
        if (side_vector_normalization_factor != 0)
            side_vector_normalization_factor = 1.0f / side_vector_normalization_factor;
        scale_vector_xyz(second_pass_width_offset,
                         arc_camera_facing_side_vector,
                         sy * side_vector_normalization_factor);
        fast_vec_add(q2[0], adjusted_segment_start, second_pass_width_offset);
        fast_vec_sub(q2[1], adjusted_segment_start, second_pass_width_offset);
        fast_vec_add(q2[2], adjusted_segment_end, second_pass_width_offset);
        fast_vec_sub(q2[3], adjusted_segment_end, second_pass_width_offset);
        if (i == 0) {
            second_pass_render_attributes.c[1] =
                *(int *)&walloper_arc_second_pass_base_vertex_color_gp;
            second_pass_render_attributes.c[0] =
                *(int *)&walloper_arc_second_pass_base_vertex_color_gp;
        } else if (i == point_count - 2) {
            second_pass_render_attributes.c[3] =
                *(int *)&walloper_arc_second_pass_base_vertex_color_gp;
            second_pass_render_attributes.c[2] =
                *(int *)&walloper_arc_second_pass_base_vertex_color_gp;
        } else if (i == 1) {
            second_pass_render_attributes.c[1] = col1;
            second_pass_render_attributes.c[0] = col1;
        }
        draw_geometry_quad(q2, 0, 0);
    }
    vu1_add_g_sregister(0x47, 0x5360B);
}

void draw_walloper_camera_facing_quad(void *mv) {
    char        *moby_bytes = mv;
    Q_2bc1a0     quad;
    V_2bc1a0     camera_facing_transform[4];
    V_2bc1a0     scaled_corner;
    V_2bc1a0     scaled_basis_offset;
    V_2bc1a0     camera_facing_offset;
    unsigned int uniform_vertex_color;
    int          i;
    FUN_001f9a68_2bc1a0(
        walloper_quad_basis_offset_scale_gp, &scaled_basis_offset, moby_bytes + 0xD0);
    fast_vec_add(&camera_facing_transform[3], &scaled_basis_offset, moby_bytes + 0x10);
    quad.tex = get_effect_texture(walloper_quad_texture_index_gp);
    quad.unk88 = 0x8000000048L;
    quad.unk80 = 0xFF9000000260L;
    quad.unk70 = 0;
    quad.unk50 = 0;
    quad.unk54 = 0;
    quad.unk58 = 0;
    quad.unk5C = 1.0f;
    quad.unk60 = 1.0f;
    quad.unk64 = 0;
    quad.unk68 = 1.0f;
    quad.unk6C = 1.0f;
    if (!random_integer_below(4))
        uniform_vertex_color = l00_walloper_quad_rare_base_vertex_color;
    else
        uniform_vertex_color = l00_walloper_quad_common_base_vertex_color;
    uniform_vertex_color =
        (uniform_vertex_color & 0xFFFFFF) |
        (FUN_001fa6d0_2bc1a0(FUN_001fa6c0_2bc1a0((int)uniform_vertex_color >> 24) *
                             D_L00_00161660_f[29])
         << 24);
    quad.color[3] = uniform_vertex_color;
    quad.color[2] = uniform_vertex_color;
    quad.color[1] = uniform_vertex_color;
    quad.color[0] = uniform_vertex_color;
    fast_vec_sub(
        &camera_facing_offset, &(*(V_2bc1a0 *)g_debug_camera), &camera_facing_transform[3]);
    FUN_001f9bf8_2bc1a0(&camera_facing_transform[0], &camera_facing_offset, 1.0f);
    fast_vec_cross(
        &camera_facing_transform[1], &camera_facing_transform[0], &(*(V_2bc1a0 *)D_0013F5E0));
    FUN_001f9bf8_2bc1a0(&camera_facing_transform[1], &camera_facing_transform[1], -1.0f);
    fast_vec_cross(
        &camera_facing_transform[2], &camera_facing_transform[1], &camera_facing_transform[0]);
    FUN_001f9a68_2bc1a0(0.2f, &camera_facing_offset, &camera_facing_transform[0]);
    fast_vec_add(&camera_facing_transform[3], &camera_facing_transform[3], &camera_facing_offset);
    for (i = 3; i >= 0; i--) {
        FUN_001f9a68_2bc1a0(walloper_quad_corner_scale_gp, &scaled_corner, &D_L00_001DBB60[3 - i]);
        transform_vector(&quad.corner[3 - i], &scaled_corner, &camera_facing_transform[0]);
    }
    draw_geometry_quad(&quad, 0, 0);
}

float D_L00_0017AEBC NOT_SDA = 1.0f;
int                  D_L00_001616CC = 0;
int                  walloper_quad_texture_index_gp __asm__("D_L00_001616C8") = 8;
unsigned int l00_walloper_quad_common_base_vertex_color __asm__("D_L00_001616C0") = 0x307f4040;
