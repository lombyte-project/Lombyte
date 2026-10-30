/* Tesla claw beam: builds the beam and arc points toward the targets and draws the beam, arcs and anchor quads. */

#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/overlay/beam_vector.h"
#include "rnc/math/random.h"
#include "rnc/rendering/vu1_registers.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/overlay/collision.h"
#include "rnc/overlay/shared/weapons/tesla_claw_beam_globals.h"
#include "rnc/overlay/shared/weapons/tesla_claw_beam_data/draw_beam_and_arc_two_pass_quads.h"
#include "rnc/overlay/shared/weapons/tesla_claw_beam.h"
#include "rnc/rendering/debug/debug_menu.h"

typedef struct {
    Vec4  direction;
    int   words[2];
    u8    kind, mode;
    short id;
    float amount;
    int   flag;
} HitDescriptor;

typedef struct {
    Vec4         origin;
    Vec4         tip;
    struct Moby *target;
    u8           pad24[0x14];
    struct Moby *second_target;
    u8           pad3c[2];
    short        count3e;
    short        target_timer;
    short        second_timer;
    short        unk44;
    short        unk46;
} WalloperBeam;

typedef struct {
    u8  pad[0x3E];
    s16 h3E;
} S2ba970;

typedef float RW_2baae0[4] __attribute__((aligned(16)));

typedef struct {
    float f[4];
} __attribute__((aligned(16))) V_2bb0b0;

typedef struct {
    float u, v;
} UV;

typedef struct {
    V_2bb0b0     corner[4];
    unsigned int color[4];
    UV           uv[4];
    long         unk70, tex, unk80, unk88;
} Q_2bb0b0;

extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float fast_subtract_rotations(float, float) __asm__("FUN_001fa5c8");
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern float vector_length_xy(void *) __asm__("FUN_001f9b20");
extern float fast_vec_dist(void *, void *) __asm__("FUN_001f9b48");
extern void  fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void  fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void  fast_vec_cross(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void  fast_vec_scale(float, void *, void *) __asm__("FUN_001f9a68");
extern void  fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern void  clear_u64_value() __asm__("FUN_001f99f8");
extern void  blend_vector_2b94d0(float, void *, void *, void *) __asm__("FUN_001f9a40");
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float convert_integer_to_float(int) __asm__("FUN_001fa6c0");
extern int   truncate_float_to_s32(float) __asm__("FUN_001fa6d0");
extern int   scale_game_frames(int) __asm__("FUN_001f96f8");
extern int   fast_dec_timer_s16() __asm__("FUN_001f9770");
extern int   get_moby_target_info() __asm__("FUN_002141f8");
extern float normalize_angle(float) __asm__("FUN_L00_0025e310");
extern int   FUN_L00_00257b90(int, int), FUN_00213260(int);
extern void  make_hit_descriptor(void *, void *, int, void *, float) __asm__("FUN_L00_00259888");
extern void  record_moby_hit(void *, void *) __asm__("FUN_L00_00259a88");
extern int   test_moby_linked_record_word8(void *) __asm__("FUN_L00_00259830");
extern int   collision_line(Vec4f *, Vec4f *, int, void *, int) __asm__("FUN_001efa68");
extern void
collect_mobies_in_sphere(float, void *, int, void *, void *) __asm__("FUN_L00_001f2868");
extern void           FUN_0022da68(int, int, void *);
extern unsigned char *FUN_L00_00272060(
    void *, int, float, float, int, int, int, float *, float) __asm__("FUN_L00_00272060");
void draw_beam_and_arc_two_pass_quads(void *, s32, s32, s32, f32, f32, f32) __asm__("FUN_L00_002baae0");
void        draw_beam_anchor_camera_facing_quads(void *mv) __asm__("FUN_L00_002bb0b0");
extern long get_effect_texture(int) __asm__("FUN_001f44b8");
extern void draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");
void        transform_vector(void *, void *, void *) __asm__("FUN_001f9d20");
extern void
build_look_at_matrix(void *dst, void *vec, void *axis, float angle) __asm__("FUN_00214890");
extern void  draw_geometry_quad(void *, void *, int);
extern float fast_vec_dist(void *, void *);
unsigned char *
FUN_L00_00272060(void *pos, int s, float x, float y, int color, int mode, int b, float *v, float z);

extern float        D_L00_001615AC_gp __asm__("D_L00_001615AC") __attribute__((sda));
extern float        D_L00_001615B4_gp __asm__("D_L00_001615B4") __attribute__((sda));
extern float        D_L00_001615C8_gp __asm__("D_L00_001615C8") __attribute__((sda));
extern char         D_0013E533[];
extern char        *active_mobies[] __asm__("D_L00_001ABA00");
extern u8           D_L00_001DB5C0[];
extern CollisionHit coll_hit __asm__("D_L00_00173E40");
extern float        D_L00_00161594 __attribute__((sda));
extern float beam_target_hero_heading_error_limit __asm__("D_L00_00161598") __attribute__((sda));
extern float beam_target_camera_heading_error_limit __asm__("D_L00_0016159C") __attribute__((sda));
extern float D_L00_001615A0 __attribute__((sda));
extern float primary_beam_wave_start_amplitude __asm__("D_L00_001615A4") __attribute__((sda));
extern float primary_beam_wave_end_amplitude __asm__("D_L00_001615A8") __attribute__((sda));
extern float primary_beam_wave_phase_advance __asm__("D_L00_001615B0") __attribute__((sda));
extern float secondary_beam_wave_phase_retreat __asm__("D_L00_001615B8") __attribute__((sda));
extern float D_L00_001615BC __attribute__((sda));
extern float secondary_beam_vertical_wave_envelope_amplitude __asm__("D_L00_001615C0")
    __attribute__((sda));
extern float secondary_beam_horizontal_wave_amplitude __asm__("D_L00_001615C4")
    __attribute__((sda));
extern float secondary_beam_vertical_envelope_phase_advance __asm__("D_L00_001615CC")
    __attribute__((sda));
extern float D_L00_001615D0 __attribute__((sda));
extern float primary_beam_vertical_jitter_range_start __asm__("D_L00_001615E4")
    __attribute__((sda));
extern float primary_beam_vertical_jitter_range_end __asm__("D_L00_001615E8") __attribute__((sda));
extern int   D_L00_001615F8 __attribute__((sda));
extern float beam_origin_particle_direction_scale __asm__("D_L00_00161628") __attribute__((sda));
extern float beam_target_camera_alignment_score_weight __asm__("D_L00_0016162C")
    __attribute__((sda));
extern short D_L00_00161658[] MACRO_ADDR;
extern short D_L00_00161668[] MACRO_ADDR;
extern short D_L00_00161670[] MACRO_ADDR;
extern s16   D_L00_00161660;
extern Vec4  arc_points[4][5] __asm__("D_L00_001DBA20");
extern Vec4  zero_vector_macro __asm__("D_L00_0015F580") MACRO_ADDR;
extern f32   beam_first_pass_texture_u_offset_gp __asm__("D_L00_001615EC") __attribute__((sda));
extern f32 beam_first_pass_texture_u_scroll_speed_gp __asm__("D_L00_001615F0") __attribute__((sda));
extern s32 primary_beam_first_pass_alpha_gp __asm__("D_L00_001615D4") __attribute__((sda));
extern s32 primary_beam_second_pass_alpha_gp __asm__("D_L00_001615DC") __attribute__((sda));
extern f32 beam_and_arc_first_pass_half_width_gp __asm__("D_L00_001615FC") __attribute__((sda));
extern f32 primary_beam_second_pass_half_width_gp __asm__("D_L00_00161600") __attribute__((sda));
extern f32 primary_beam_second_pass_segment_extension_factor_gp __asm__("D_L00_00161630")
    __attribute__((sda));
extern s32 secondary_beam_first_pass_alpha_gp __asm__("D_L00_00161604") __attribute__((sda));
extern s32 secondary_beam_second_pass_alpha_gp __asm__("D_L00_00161608") __attribute__((sda));
extern f32 D_L00_0016160C __attribute__((sda));
extern f32 secondary_beam_and_arc_second_pass_half_width_gp __asm__("D_L00_00161610")
    __attribute__((sda));
extern f32 secondary_beam_second_pass_segment_extension_factor_gp __asm__("D_L00_00161634")
    __attribute__((sda));
extern unsigned int D_L00_001615F8_gp __asm__("D_L00_001615F8") __attribute__((sda));
extern char         D_0013F5E0[];
extern unsigned int l00_beam_anchor_first_pass_vertex_color __asm__("D_L00_00161640")
    __attribute__((sda));
extern float beam_anchor_first_pass_base_corner_scale_gp __asm__("D_L00_00161648")
    __attribute__((sda));
extern float beam_anchor_second_pass_base_corner_scale_gp __asm__("D_L00_0016164C")
    __attribute__((sda));
extern V_2bb0b0 D_L00_001DB560[] NOT_SDA;
extern UV       D_L00_001DB5A0[] NOT_SDA;
extern float    frame_scale_macro __asm__("D_0015ED60") MACRO_ADDR;
extern char     D_0013F3D0[] NOT_SDA;

float D_L01_001615C8 = 0.7853982f;
float D_L01_001615CC = 0.55850536f;
float D_L01_001615D0 = 0.017453292f;
float D_L01_001615D4 = 0.03f;
float D_L01_001615D8 = 0.03f;
float D_L01_001615E0 = 0.34906584f;
float D_L01_001615E8 = 0.80285144f;
float D_L01_001615EC = 0.99483764f;
float D_L01_001615F0 = 0.8f;
float D_L01_001615F4 = 0.5f;
float D_L01_001615FC = 0.05236f;
float D_L01_00161600 = 0.20943952f;
float D_L01_00161614 = 0;
float D_L01_00161618 = 0.1f;
float D_L01_00161658 = 0.09f;
float D_L01_0016165C = 1.5f;
s16   D_L01_00161690 = 0;
Vec4  D_L01_001DBEA0[4][5] = {0};

f32          D_L01_0016161C = 0;
f32          D_L01_00161620 = 0.3f;
s32          D_L01_00161604 = 0x80;
s32          D_L01_0016160C = 0x40;
f32          D_L01_0016162C = 0.05f;
f32          D_L01_00161630 = 0.4f;
f32          D_L01_00161660 = 0.9f;
s32          D_L01_00161634 = 0x10;
s32          D_L01_00161638 = 0x20;
f32          D_L01_0016163C = 0.1f;
f32          D_L01_00161640 = 0.3f;
f32          D_L01_00161664 = 0.1f;
unsigned int D_L01_00161670 = 0x307f7f7f;
float        D_L01_00161678 = 0.1f;
float        D_L01_0016167C = 0.35f;

V_2bb0b0 D_L01_001DB9E0 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

UV D_L01_001DBA20 NOT_SDA = {0};

float D_L02_00161568 = 0.7853982f;
float D_L02_0016156C = 0.55850536f;
float D_L02_00161570 = 0.017453292f;
float D_L02_00161574 = 0.03f;
float D_L02_00161578 = 0.03f;
float D_L02_00161580 = 0.34906584f;
float D_L02_00161588 = 0.80285144f;
float D_L02_0016158C = 0.99483764f;
float D_L02_00161590 = 0.8f;
float D_L02_00161594 = 0.5f;
float D_L02_0016159C = 0.05236f;
float D_L02_001615A0 = 0.20943952f;
float D_L02_001615B4 = 0;
float D_L02_001615B8 = 0.1f;
float D_L02_001615F8 = 0.09f;
float D_L02_001615FC = 1.5f;
s16   D_L02_00161630 = 0;
Vec4  D_L02_001D04A0[4][5] = {0};

f32          D_L02_001615BC = 0;
f32          D_L02_001615C0 = 0.3f;
s32          D_L02_001615A4 = 0x80;
s32          D_L02_001615AC = 0x40;
f32          D_L02_001615CC = 0.05f;
f32          D_L02_001615D0 = 0.4f;
f32          D_L02_00161600 = 0.9f;
s32          D_L02_001615D4 = 0x10;
s32          D_L02_001615D8 = 0x20;
f32          D_L02_001615DC = 0.1f;
f32          D_L02_001615E0 = 0.3f;
f32          D_L02_00161604 = 0.1f;
unsigned int D_L02_00161610 = 0x307f7f7f;
float        D_L02_00161618 = 0.1f;
float        D_L02_0016161C = 0.35f;

V_2bb0b0 D_L02_001CFFE0 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

UV D_L02_001D0020 NOT_SDA = {0};

float D_L03_00161578 = 0.7853982f;
float D_L03_0016157C = 0.55850536f;
float D_L03_00161580 = 0.017453292f;
float D_L03_00161584 = 0.03f;
float D_L03_00161588 = 0.03f;
float D_L03_00161590 = 0.34906584f;
float D_L03_00161598 = 0.80285144f;
float D_L03_0016159C = 0.99483764f;
float D_L03_001615A0 = 0.8f;
float D_L03_001615A4 = 0.5f;
float D_L03_001615AC = 0.05236f;
float D_L03_001615B0 = 0.20943952f;
float D_L03_001615C4 = 0;
float D_L03_001615C8 = 0.1f;
float D_L03_00161608 = 0.09f;
float D_L03_0016160C = 1.5f;
s16   D_L03_00161640 = 0;
Vec4  D_L03_001CFFA0[4][5] = {0};

f32          D_L03_001615CC = 0;
f32          D_L03_001615D0 = 0.3f;
s32          D_L03_001615B4 = 0x80;
s32          D_L03_001615BC = 0x40;
f32          D_L03_001615DC = 0.05f;
f32          D_L03_001615E0 = 0.4f;
f32          D_L03_00161610 = 0.9f;
s32          D_L03_001615E4 = 0x10;
s32          D_L03_001615E8 = 0x20;
f32          D_L03_001615EC = 0.1f;
f32          D_L03_001615F0 = 0.3f;
f32          D_L03_00161614 = 0.1f;
unsigned int D_L03_00161620 = 0x307f7f7f;
float        D_L03_00161628 = 0.1f;
float        D_L03_0016162C = 0.35f;

V_2bb0b0 D_L03_001CFAE0 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

UV D_L03_001CFB20 NOT_SDA = {0};

float D_L04_00161568 = 0.7853982f;
float D_L04_0016156C = 0.55850536f;
float D_L04_00161570 = 0.017453292f;
float D_L04_00161574 = 0.03f;
float D_L04_00161578 = 0.03f;
float D_L04_00161580 = 0.34906584f;
float D_L04_00161588 = 0.80285144f;
float D_L04_0016158C = 0.99483764f;
float D_L04_00161590 = 0.8f;
float D_L04_00161594 = 0.5f;
float D_L04_0016159C = 0.05236f;
float D_L04_001615A0 = 0.20943952f;
float D_L04_001615B4 = 0;
float D_L04_001615B8 = 0.1f;
float D_L04_001615F8 = 0.09f;
float D_L04_001615FC = 1.5f;
s16   D_L04_00161630 = 0;
Vec4  D_L04_001D0020[4][5] = {0};

f32          D_L04_001615BC = 0;
f32          D_L04_001615C0 = 0.3f;
s32          D_L04_001615A4 = 0x80;
s32          D_L04_001615AC = 0x40;
f32          D_L04_001615CC = 0.05f;
f32          D_L04_001615D0 = 0.4f;
f32          D_L04_00161600 = 0.9f;
s32          D_L04_001615D4 = 0x10;
s32          D_L04_001615D8 = 0x20;
f32          D_L04_001615DC = 0.1f;
f32          D_L04_001615E0 = 0.3f;
f32          D_L04_00161604 = 0.1f;
unsigned int D_L04_00161610 = 0x307f7f7f;
float        D_L04_00161618 = 0.1f;
float        D_L04_0016161C = 0.35f;

V_2bb0b0 D_L04_001CFB60 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

UV D_L04_001CFBA0 NOT_SDA = {0};

float D_L05_001615B8 = 0.7853982f;
float D_L05_001615BC = 0.55850536f;
float D_L05_001615C0 = 0.017453292f;
float D_L05_001615C4 = 0.03f;
float D_L05_001615C8 = 0.03f;
float D_L05_001615D0 = 0.34906584f;
float D_L05_001615D8 = 0.80285144f;
float D_L05_001615DC = 0.99483764f;
float D_L05_001615E0 = 0.8f;
float D_L05_001615E4 = 0.5f;
float D_L05_001615EC = 0.05236f;
float D_L05_001615F0 = 0.20943952f;
float D_L05_00161604 = 0;
float D_L05_00161608 = 0.1f;
float D_L05_00161648 = 0.09f;
float D_L05_0016164C = 1.5f;
s16   D_L05_00161680 = 0;
Vec4  D_L05_001D0340[4][5] = {0};

f32          D_L05_0016160C = 0;
f32          D_L05_00161610 = 0.3f;
s32          D_L05_001615F4 = 0x80;
s32          D_L05_001615FC = 0x40;
f32          D_L05_0016161C = 0.05f;
f32          D_L05_00161620 = 0.4f;
f32          D_L05_00161650 = 0.9f;
s32          D_L05_00161624 = 0x10;
s32          D_L05_00161628 = 0x20;
f32          D_L05_0016162C = 0.1f;
f32          D_L05_00161630 = 0.3f;
f32          D_L05_00161654 = 0.1f;
unsigned int D_L05_00161660 = 0x307f7f7f;
float        D_L05_00161668 = 0.1f;
float        D_L05_0016166C = 0.35f;

V_2bb0b0 D_L05_001CFE80 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

UV D_L05_001CFEC0 NOT_SDA = {0};

float D_L06_00161598 = 0.7853982f;
float D_L06_0016159C = 0.55850536f;
float D_L06_001615A0 = 0.017453292f;
float D_L06_001615A4 = 0.03f;
float D_L06_001615A8 = 0.03f;
float D_L06_001615B0 = 0.34906584f;
float D_L06_001615B8 = 0.80285144f;
float D_L06_001615BC = 0.99483764f;
float D_L06_001615C0 = 0.8f;
float D_L06_001615C4 = 0.5f;
float D_L06_001615CC = 0.05236f;
float D_L06_001615D0 = 0.20943952f;
float D_L06_001615E4 = 0;
float D_L06_001615E8 = 0.1f;
float D_L06_00161628 = 0.09f;
float D_L06_0016162C = 1.5f;
s16   D_L06_00161660 = 0;
Vec4  D_L06_001D0620[4][5] = {0};

f32          D_L06_001615EC = 0;
f32          D_L06_001615F0 = 0.3f;
s32          D_L06_001615D4 = 0x80;
s32          D_L06_001615DC = 0x40;
f32          D_L06_001615FC = 0.05f;
f32          D_L06_00161600 = 0.4f;
f32          D_L06_00161630 = 0.9f;
s32          D_L06_00161604 = 0x10;
s32          D_L06_00161608 = 0x20;
f32          D_L06_0016160C = 0.1f;
f32          D_L06_00161610 = 0.3f;
f32          D_L06_00161634 = 0.1f;
unsigned int D_L06_00161640 = 0x307f7f7f;
float        D_L06_00161648 = 0.1f;
float        D_L06_0016164C = 0.35f;

V_2bb0b0 D_L06_001D0160 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

UV D_L06_001D01A0 NOT_SDA = {0};

float D_L07_00161568 = 0.7853982f;
float D_L07_0016156C = 0.55850536f;
float D_L07_00161570 = 0.017453292f;
float D_L07_00161574 = 0.03f;
float D_L07_00161578 = 0.03f;
float D_L07_00161580 = 0.34906584f;
float D_L07_00161588 = 0.80285144f;
float D_L07_0016158C = 0.99483764f;
float D_L07_00161590 = 0.8f;
float D_L07_00161594 = 0.5f;
float D_L07_0016159C = 0.05236f;
float D_L07_001615A0 = 0.20943952f;
float D_L07_001615B4 = 0;
float D_L07_001615B8 = 0.1f;
float D_L07_001615F8 = 0.09f;
float D_L07_001615FC = 1.5f;
s16   D_L07_00161630 = 0;
Vec4  D_L07_001CFF20[4][5] = {0};

f32          D_L07_001615BC = 0;
f32          D_L07_001615C0 = 0.3f;
s32          D_L07_001615A4 = 0x80;
s32          D_L07_001615AC = 0x40;
f32          D_L07_001615CC = 0.05f;
f32          D_L07_001615D0 = 0.4f;
f32          D_L07_00161600 = 0.9f;
s32          D_L07_001615D4 = 0x10;
s32          D_L07_001615D8 = 0x20;
f32          D_L07_001615DC = 0.1f;
f32          D_L07_001615E0 = 0.3f;
f32          D_L07_00161604 = 0.1f;
unsigned int D_L07_00161610 = 0x307f7f7f;
float        D_L07_00161618 = 0.1f;
float        D_L07_0016161C = 0.35f;

V_2bb0b0 D_L07_001CFA60 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

UV D_L07_001CFAA0 NOT_SDA = {0};

float D_L08_001615B8 = 0.7853982f;
float D_L08_001615BC = 0.55850536f;
float D_L08_001615C0 = 0.017453292f;
float D_L08_001615C4 = 0.03f;
float D_L08_001615C8 = 0.03f;
float D_L08_001615D0 = 0.34906584f;
float D_L08_001615D8 = 0.80285144f;
float D_L08_001615DC = 0.99483764f;
float D_L08_001615E0 = 0.8f;
float D_L08_001615E4 = 0.5f;
float D_L08_001615EC = 0.05236f;
float D_L08_001615F0 = 0.20943952f;
float D_L08_00161604 = 0;
float D_L08_00161608 = 0.1f;
float D_L08_00161648 = 0.09f;
float D_L08_0016164C = 1.5f;
s16   D_L08_00161680 = 0;
Vec4  D_L08_001D06A0[4][5] = {0};

f32          D_L08_0016160C = 0;
f32          D_L08_00161610 = 0.3f;
s32          D_L08_001615F4 = 0x80;
s32          D_L08_001615FC = 0x40;
f32          D_L08_0016161C = 0.05f;
f32          D_L08_00161620 = 0.4f;
f32          D_L08_00161650 = 0.9f;
s32          D_L08_00161624 = 0x10;
s32          D_L08_00161628 = 0x20;
f32          D_L08_0016162C = 0.1f;
f32          D_L08_00161630 = 0.3f;
f32          D_L08_00161654 = 0.1f;
unsigned int D_L08_00161660 = 0x307f7f7f;
float        D_L08_00161668 = 0.1f;
float        D_L08_0016166C = 0.35f;

V_2bb0b0 D_L08_001D01E0 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

UV D_L08_001D0220 NOT_SDA = {0};

float D_L09_00161628 = 0.7853982f;
float D_L09_0016162C = 0.55850536f;
float D_L09_00161630 = 0.017453292f;
float D_L09_00161634 = 0.03f;
float D_L09_00161638 = 0.03f;
float D_L09_00161640 = 0.34906584f;
float D_L09_00161648 = 0.80285144f;
float D_L09_0016164C = 0.99483764f;
float D_L09_00161650 = 0.8f;
float D_L09_00161654 = 0.5f;
float D_L09_0016165C = 0.05236f;
float D_L09_00161660 = 0.20943952f;
float D_L09_00161674 = 0;
float D_L09_00161678 = 0.1f;
float D_L09_001616B8 = 0.09f;
float D_L09_001616BC = 1.5f;
s16   D_L09_001616F0 = 0;
Vec4  D_L09_001D0020[4][5] = {0};

f32          D_L09_0016167C = 0;
f32          D_L09_00161680 = 0.3f;
s32          D_L09_00161664 = 0x80;
s32          D_L09_0016166C = 0x40;
f32          D_L09_0016168C = 0.05f;
f32          D_L09_00161690 = 0.4f;
f32          D_L09_001616C0 = 0.9f;
s32          D_L09_00161694 = 0x10;
s32          D_L09_00161698 = 0x20;
f32          D_L09_0016169C = 0.1f;
f32          D_L09_001616A0 = 0.3f;
f32          D_L09_001616C4 = 0.1f;
unsigned int D_L09_001616D0 = 0x307f7f7f;
float        D_L09_001616D8 = 0.1f;
float        D_L09_001616DC = 0.35f;

V_2bb0b0 D_L09_001CFB60 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

UV D_L09_001CFBA0 NOT_SDA = {0};

float D_L10_00161578 = 0.7853982f;
float D_L10_0016157C = 0.55850536f;
float D_L10_00161580 = 0.017453292f;
float D_L10_00161584 = 0.03f;
float D_L10_00161588 = 0.03f;
float D_L10_00161590 = 0.34906584f;
float D_L10_00161598 = 0.80285144f;
float D_L10_0016159C = 0.99483764f;
float D_L10_001615A0 = 0.8f;
float D_L10_001615A4 = 0.5f;
float D_L10_001615AC = 0.05236f;
float D_L10_001615B0 = 0.20943952f;
float D_L10_001615C4 = 0;
float D_L10_001615C8 = 0.1f;
float D_L10_00161608 = 0.09f;
float D_L10_0016160C = 1.5f;
s16   D_L10_00161640 = 0;
Vec4  D_L10_001D0320[4][5] = {0};

f32          D_L10_001615CC = 0;
f32          D_L10_001615D0 = 0.3f;
s32          D_L10_001615B4 = 0x80;
s32          D_L10_001615BC = 0x40;
f32          D_L10_001615DC = 0.05f;
f32          D_L10_001615E0 = 0.4f;
f32          D_L10_00161610 = 0.9f;
s32          D_L10_001615E4 = 0x10;
s32          D_L10_001615E8 = 0x20;
f32          D_L10_001615EC = 0.1f;
f32          D_L10_001615F0 = 0.3f;
f32          D_L10_00161614 = 0.1f;
unsigned int D_L10_00161620 = 0x307f7f7f;
float        D_L10_00161628 = 0.1f;
float        D_L10_0016162C = 0.35f;

V_2bb0b0 D_L10_001CFE60 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

UV D_L10_001CFEA0 NOT_SDA = {0};

float D_L11_001615F8 = 0.7853982f;
float D_L11_001615FC = 0.55850536f;
float D_L11_00161600 = 0.017453292f;
float D_L11_00161604 = 0.03f;
float D_L11_00161608 = 0.03f;
float D_L11_00161610 = 0.34906584f;
float D_L11_00161618 = 0.80285144f;
float D_L11_0016161C = 0.99483764f;
float D_L11_00161620 = 0.8f;
float D_L11_00161624 = 0.5f;
float D_L11_0016162C = 0.05236f;
float D_L11_00161630 = 0.20943952f;
float D_L11_00161644 = 0;
float D_L11_00161648 = 0.1f;
float D_L11_00161688 = 0.09f;
float D_L11_0016168C = 1.5f;
s16   D_L11_001616C0 = 0;
Vec4  D_L11_001D08A0[4][5] = {0};

f32          D_L11_0016164C = 0;
f32          D_L11_00161650 = 0.3f;
s32          D_L11_00161634 = 0x80;
s32          D_L11_0016163C = 0x40;
f32          D_L11_0016165C = 0.05f;
f32          D_L11_00161660 = 0.4f;
f32          D_L11_00161690 = 0.9f;
s32          D_L11_00161664 = 0x10;
s32          D_L11_00161668 = 0x20;
f32          D_L11_0016166C = 0.1f;
f32          D_L11_00161670 = 0.3f;
f32          D_L11_00161694 = 0.1f;
unsigned int D_L11_001616A0 = 0x307f7f7f;
float        D_L11_001616A8 = 0.1f;
float        D_L11_001616AC = 0.35f;

V_2bb0b0 D_L11_001D03E0 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

UV D_L11_001D0420 NOT_SDA = {0};

float D_L12_00161578 = 0.7853982f;
float D_L12_0016157C = 0.55850536f;
float D_L12_00161580 = 0.017453292f;
float D_L12_00161584 = 0.03f;
float D_L12_00161588 = 0.03f;
float D_L12_00161590 = 0.34906584f;
float D_L12_00161598 = 0.80285144f;
float D_L12_0016159C = 0.99483764f;
float D_L12_001615A0 = 0.8f;
float D_L12_001615A4 = 0.5f;
float D_L12_001615AC = 0.05236f;
float D_L12_001615B0 = 0.20943952f;
float D_L12_001615C4 = 0;
float D_L12_001615C8 = 0.1f;
float D_L12_00161608 = 0.09f;
float D_L12_0016160C = 1.5f;
s16   D_L12_00161640 = 0;
Vec4  D_L12_001E2F40[4][5] = {0};

f32          D_L12_001615CC = 0;
f32          D_L12_001615D0 = 0.3f;
s32          D_L12_001615B4 = 0x80;
s32          D_L12_001615BC = 0x40;
f32          D_L12_001615DC = 0.05f;
f32          D_L12_001615E0 = 0.4f;
f32          D_L12_00161610 = 0.9f;
s32          D_L12_001615E4 = 0x10;
s32          D_L12_001615E8 = 0x20;
f32          D_L12_001615EC = 0.1f;
f32          D_L12_001615F0 = 0.3f;
f32          D_L12_00161614 = 0.1f;
unsigned int D_L12_00161620 = 0x307f7f7f;
float        D_L12_00161628 = 0.1f;
float        D_L12_0016162C = 0.35f;

V_2bb0b0 D_L12_001E2A80 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

UV D_L12_001E2AC0 NOT_SDA = {0};

float D_L13_00161648 = 0.7853982f;
float D_L13_0016164C = 0.55850536f;
float D_L13_00161650 = 0.017453292f;
float D_L13_00161654 = 0.03f;
float D_L13_00161658 = 0.03f;
float D_L13_00161660 = 0.34906584f;
float D_L13_00161668 = 0.80285144f;
float D_L13_0016166C = 0.99483764f;
float D_L13_00161670 = 0.8f;
float D_L13_00161674 = 0.5f;
float D_L13_0016167C = 0.05236f;
float D_L13_00161680 = 0.20943952f;
float D_L13_00161694 = 0;
float D_L13_00161698 = 0.1f;
float D_L13_001616D8 = 0.09f;
float D_L13_001616DC = 1.5f;
s16   D_L13_00161710 = 0;
Vec4  D_L13_001D07B0[4][5] = {0};

f32          D_L13_0016169C = 0;
f32          D_L13_001616A0 = 0.3f;
s32          D_L13_00161684 = 0x80;
s32          D_L13_0016168C = 0x40;
f32          D_L13_001616AC = 0.05f;
f32          D_L13_001616B0 = 0.4f;
f32          D_L13_001616E0 = 0.9f;
s32          D_L13_001616B4 = 0x10;
s32          D_L13_001616B8 = 0x20;
f32          D_L13_001616BC = 0.1f;
f32          D_L13_001616C0 = 0.3f;
f32          D_L13_001616E4 = 0.1f;
unsigned int D_L13_001616F0 = 0x307f7f7f;
float        D_L13_001616F8 = 0.1f;
float        D_L13_001616FC = 0.35f;

V_2bb0b0 D_L13_001D02F0 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

UV D_L13_001D0330 NOT_SDA = {0};

float D_L14_00161788 = 0.7853982f;
float D_L14_0016178C = 0.55850536f;
float D_L14_00161790 = 0.017453292f;
float D_L14_00161794 = 0.03f;
float D_L14_00161798 = 0.03f;
float D_L14_001617A0 = 0.34906584f;
float D_L14_001617A8 = 0.80285144f;
float D_L14_001617AC = 0.99483764f;
float D_L14_001617B0 = 0.8f;
float D_L14_001617B4 = 0.5f;
float D_L14_001617BC = 0.05236f;
float D_L14_001617C0 = 0.20943952f;
float D_L14_001617D4 = 0;
float D_L14_001617D8 = 0.1f;
float D_L14_00161818 = 0.09f;
float D_L14_0016181C = 1.5f;
s16   D_L14_00161850 = 0;
Vec4  D_L14_001DCE60[4][5] = {0};

f32          D_L14_001617DC = 0;
f32          D_L14_001617E0 = 0.3f;
s32          D_L14_001617C4 = 0x80;
s32          D_L14_001617CC = 0x40;
f32          D_L14_001617EC = 0.05f;
f32          D_L14_001617F0 = 0.4f;
f32          D_L14_00161820 = 0.9f;
s32          D_L14_001617F4 = 0x10;
s32          D_L14_001617F8 = 0x20;
f32          D_L14_001617FC = 0.1f;
f32          D_L14_00161800 = 0.3f;
f32          D_L14_00161824 = 0.1f;
unsigned int D_L14_00161830 = 0x307f7f7f;
float        D_L14_00161838 = 0.1f;
float        D_L14_0016183C = 0.35f;

V_2bb0b0 D_L14_001DC9A0 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

UV D_L14_001DC9E0 NOT_SDA = {0};

float D_L15_00161728 = 0.7853982f;
float D_L15_0016172C = 0.55850536f;
float D_L15_00161730 = 0.017453292f;
float D_L15_00161734 = 0.03f;
float D_L15_00161738 = 0.03f;
float D_L15_00161740 = 0.34906584f;
float D_L15_00161748 = 0.80285144f;
float D_L15_0016174C = 0.99483764f;
float D_L15_00161750 = 0.8f;
float D_L15_00161754 = 0.5f;
float D_L15_0016175C = 0.05236f;
float D_L15_00161760 = 0.20943952f;
float D_L15_00161774 = 0;
float D_L15_00161778 = 0.1f;
float D_L15_001617B8 = 0.09f;
float D_L15_001617BC = 1.5f;
s16   D_L15_001617F0 = 0;
Vec4  D_L15_001D0920[4][5] = {0};

f32          D_L15_00161780 = 0.3f;
s32          D_L15_00161764 = 0x80;
s32          D_L15_0016176C = 0x40;
f32          D_L15_00161790 = 0.4f;
f32          D_L15_001617C0 = 0.9f;
s32          D_L15_00161794 = 0x10;
s32          D_L15_00161798 = 0x20;
f32          D_L15_0016179C = 0.1f;
f32          D_L15_001617A0 = 0.3f;
f32          D_L15_001617C4 = 0.1f;
unsigned int D_L15_001617D0 = 0x307f7f7f;
float        D_L15_001617D8 = 0.1f;
float        D_L15_001617DC = 0.35f;

V_2bb0b0 D_L15_001D0460 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

UV D_L15_001D04A0 NOT_SDA = {0};

float D_L16_00161628 = 0.7853982f;
float D_L16_0016162C = 0.55850536f;
float D_L16_00161630 = 0.017453292f;
float D_L16_00161634 = 0.03f;
float D_L16_00161638 = 0.03f;
float D_L16_00161640 = 0.34906584f;
float D_L16_00161648 = 0.80285144f;
float D_L16_0016164C = 0.99483764f;
float D_L16_00161650 = 0.8f;
float D_L16_00161654 = 0.5f;
float D_L16_0016165C = 0.05236f;
float D_L16_00161660 = 0.20943952f;
float D_L16_00161674 = 0;
float D_L16_00161678 = 0.1f;
float D_L16_001616B8 = 0.09f;
float D_L16_001616BC = 1.5f;
s16   D_L16_001616F0 = 0;
Vec4  D_L16_001D0320[4][5] = {0};

f32          D_L16_0016167C = 0;
f32          D_L16_00161680 = 0.3f;
s32          D_L16_00161664 = 0x80;
s32          D_L16_0016166C = 0x40;
f32          D_L16_0016168C = 0.05f;
f32          D_L16_00161690 = 0.4f;
f32          D_L16_001616C0 = 0.9f;
s32          D_L16_00161694 = 0x10;
s32          D_L16_00161698 = 0x20;
f32          D_L16_0016169C = 0.1f;
f32          D_L16_001616A0 = 0.3f;
f32          D_L16_001616C4 = 0.1f;
unsigned int D_L16_001616D0 = 0x307f7f7f;
float        D_L16_001616D8 = 0.1f;
float        D_L16_001616DC = 0.35f;

V_2bb0b0 D_L16_001CFE60 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

UV D_L16_001CFEA0 NOT_SDA = {0};

float D_L17_00161678 = 0.7853982f;
float D_L17_0016167C = 0.55850536f;
float D_L17_00161680 = 0.017453292f;
float D_L17_00161684 = 0.03f;
float D_L17_00161688 = 0.03f;
float D_L17_00161690 = 0.34906584f;
float D_L17_00161698 = 0.80285144f;
float D_L17_0016169C = 0.99483764f;
float D_L17_001616A0 = 0.8f;
float D_L17_001616A4 = 0.5f;
float D_L17_001616AC = 0.05236f;
float D_L17_001616B0 = 0.20943952f;
float D_L17_001616C4 = 0;
float D_L17_001616C8 = 0.1f;
float D_L17_00161708 = 0.09f;
float D_L17_0016170C = 1.5f;
s16   D_L17_00161740 = 0;
Vec4  D_L17_001D0BA0[4][5] = {0};

f32          D_L17_001616D0 = 0.3f;
s32          D_L17_001616B4 = 0x80;
s32          D_L17_001616BC = 0x40;
f32          D_L17_001616E0 = 0.4f;
f32          D_L17_00161710 = 0.9f;
s32          D_L17_001616E4 = 0x10;
s32          D_L17_001616E8 = 0x20;
f32          D_L17_001616EC = 0.1f;
f32          D_L17_001616F0 = 0.3f;
f32          D_L17_00161714 = 0.1f;
unsigned int D_L17_00161720 = 0x307f7f7f;
float        D_L17_00161728 = 0.1f;
float        D_L17_0016172C = 0.35f;

V_2bb0b0 D_L17_001D06E0 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

UV D_L17_001D0720 NOT_SDA = {0};

float D_L18_00161608 = 0.7853982f;
float D_L18_0016160C = 0.55850536f;
float D_L18_00161610 = 0.017453292f;
float D_L18_00161614 = 0.03f;
float D_L18_00161618 = 0.03f;
float D_L18_00161620 = 0.34906584f;
float D_L18_00161628 = 0.80285144f;
float D_L18_0016162C = 0.99483764f;
float D_L18_00161630 = 0.8f;
float D_L18_00161634 = 0.5f;
float D_L18_0016163C = 0.05236f;
float D_L18_00161640 = 0.20943952f;
float D_L18_00161654 = 0;
float D_L18_00161658 = 0.1f;
float D_L18_00161698 = 0.09f;
float D_L18_0016169C = 1.5f;
s16   D_L18_001616D0 = 0;
Vec4  D_L18_001D08A0[4][5] = {0};

f32          D_L18_00161660 = 0.3f;
s32          D_L18_00161644 = 0x80;
s32          D_L18_0016164C = 0x40;
f32          D_L18_00161670 = 0.4f;
f32          D_L18_001616A0 = 0.9f;
s32          D_L18_00161674 = 0x10;
s32          D_L18_00161678 = 0x20;
f32          D_L18_0016167C = 0.1f;
f32          D_L18_00161680 = 0.3f;
f32          D_L18_001616A4 = 0.1f;
unsigned int D_L18_001616B0 = 0x307f7f7f;
float        D_L18_001616B8 = 0.1f;
float        D_L18_001616BC = 0.35f;

V_2bb0b0 D_L18_001D03E0 = {
    {0.0f, -1.0f, 1.0f, 1.0f}
};

UV D_L18_001D0420 NOT_SDA = {0};

void build_beam_and_arc_points(struct Moby *o) __asm__("FUN_L00_002b94d0");

void build_beam_and_arc_points(struct Moby *beam_owner_moby) {
    Vec4 target0, target1;
    struct {
        Vec4  direction;
        int   words[2];
        u8    kind, mode;
        short id;
        float amount;
        int   flag;
    } hit;
    Vec4         camera_direction, direction, delta, blend, step, offset;
    struct Moby *collided[128];
    Vec4         target_delta;
    union {
        struct {
            Vec4          origin;
            HitDescriptor hit0;
        } first;
        HitDescriptor hit1;
    } effects;
    /* Retail uses a five-vector scratch span with differences beginning at slot 1. */
    Vec4          differences[5];
    WalloperBeam *beam_state = ((WalloperBeam *)beam_owner_moby->pvars);
    int           i;
    struct Moby  *best = 0, *secondary_beam_target_candidate = 0, *candidate, **list;
    char         *beam_target_aim_metadata;
    float         best_score = 0.0f, second_score = 0.0f;
    int           enabled = (u8)D_0013E533[0];
    float         spacing = D_L00_00161594 / 20.0f;
    float distance, angle, score, beam_wave_sample_index, amplitude, wave, length, factor, phase,
        side_wave;
    float        side_phase;
    float        x_s, x_c, x_a, x_m;
    struct Hero *player;
    LevelCamera *camera;
    struct Hero *aim_hero;
    float        primary_distance, sign;
    float        distance_w17;
    float        distance_w16;
    float        factor_w15;
    float        factor_w13;
    float        length_w4;
    float        length_w3;
    int          k;
    int          arc_point_index;
    int          mirror;
    int          seg;
    int          j, hit_result, collided_count, spin;
    char        *previous, *side;
    Vec4        *beam;
    float       *side_waves;
    D_L00_001615AC_gp = fast_add_rotations(D_L00_001615AC_gp, primary_beam_wave_phase_advance);
    D_L00_001615B4_gp =
        fast_subtract_rotations(D_L00_001615B4_gp, secondary_beam_wave_phase_retreat);
    D_L00_001615C8_gp =
        fast_add_rotations(D_L00_001615C8_gp, secondary_beam_vertical_envelope_phase_advance);
    if (!beam_state->target || (enabled && !beam_state->second_target)) {
        if (beam_state->second_target) {
            beam_state->target = beam_state->second_target;
            beam_state->target_timer = beam_state->second_timer;
            beam_state->second_target = 0;
            beam_state->second_timer = 0;
        } else {
            for (k = 0; (candidate = ((struct Moby **)active_mobies)[k]) != 0; k++) {
                camera = &l00_camera;
                aim_hero = &hero;
                if (candidate == beam_state->target || candidate == beam_state->second_target ||
                    !candidate || !candidate->pclass ||
                    *(short *)((char *)candidate->pclass + 0x46) != 5)
                    continue;
                if (candidate->state == 0xfe)
                    goto next_candidate;
                if (candidate->state == 0xfd)
                    goto next_candidate;
                if (!get_moby_target_info(candidate))
                    continue;
                fast_vec_sub(&target0, &candidate->pos, beam_state);
                distance_w16 = vector_length_xy(&target0);
                if (D_L00_00161594 < distance_w16)
                    continue;
                length_w3 = fast_difference_between_rotations(
                    camera->yaw,
                    fast_arc_tan(candidate->pos.x - camera->pos.f[0],
                                 candidate->pos.y - camera->pos.f[1]));
                if (beam_target_camera_heading_error_limit < length_w3)
                    continue;
                score = fast_difference_between_rotations(
                    fast_arc_tan(candidate->pos.x - aim_hero->motion.pos.f[0],
                                 candidate->pos.y - aim_hero->motion.pos.f[1]),
                    aim_hero->motion.rot.f[2]);
                if (beam_target_hero_heading_error_limit < score)
                    continue;
                score = ((beam_target_camera_heading_error_limit - length_w3) /
                         beam_target_camera_heading_error_limit) *
                            beam_target_camera_alignment_score_weight +
                        (D_L00_00161594 - distance_w16) / D_L00_00161594;
                if (best_score < score) {
                    second_score = best_score;
                    secondary_beam_target_candidate = best;
                    best_score = score;
                    best = candidate;
                } else if (second_score < score) {
                    second_score = score;
                    secondary_beam_target_candidate = candidate;
                }
            next_candidate:;
            }
        }
    }
    if (best) {
        if (beam_state->target)
            secondary_beam_target_candidate = best;
        else {
            beam_state->target = best;
            beam_state->target_timer = 0;
            FUN_0022da68(1, 0, beam_owner_moby);
        }
    }
    if (secondary_beam_target_candidate && enabled) {
        distance_w17 = fast_vec_dist(beam_state, &best->pos);
        if (distance_w17 < D_L00_00161594) {
            angle = fast_vec_dist(&best->pos, &secondary_beam_target_candidate->pos);
            if (distance_w17 + angle < D_L00_00161594) {
                beam_state->second_target = secondary_beam_target_candidate;
                beam_state->second_timer = 0;
            }
        }
    }
    primary_distance = 0.0f;
    if (beam_state->target) {
        beam_target_aim_metadata = (char *)get_moby_target_info(beam_state->target);
        qcopy(&target0, &beam_state->target->pos);
        target0.f[2] += *(float *)(beam_target_aim_metadata + 0x10);
        primary_distance = fast_vec_dist(beam_state, &target0);
        spacing = primary_distance * 0.05f;
    }
    if (beam_state->second_target) {
        beam_target_aim_metadata = (char *)get_moby_target_info(beam_state->second_target);
        qcopy(&target1, &beam_state->second_target->pos);
        target1.f[2] += *(float *)(beam_target_aim_metadata + 0x10);
        spacing = (fast_vec_dist(&target0, &target1) + primary_distance) * 0.05f;
    }
    if (enabled)
        D_L00_001615F8 = 0x207f20;
    else
        D_L00_001615F8 = 0x7f2020;
    clear_u64_value(&camera_direction);
    make_hit_descriptor(&hit, beam_owner_moby, 0x210000, &camera_direction, enabled + 2.0f);
    hit.kind = 5;
    hit.mode = 1;
    hit.id = beam_owner_moby->oclass;
    beam_state->count3e = 20;
    player = &hero;
    collect_mobies_in_sphere(0.3f, beam_state, 0x10, player->moby, &hit);
    fast_vec_sub(&direction, &beam_state->tip, beam_state);
    fast_vec_normalize(&direction, &direction, 1.0f);
    if (beam_state->target || beam_state->second_target) {
        fast_vec_cross(&delta, &direction, &player->gravity_dir);
        build_look_at_matrix(&direction, &direction, &delta, 0.17453292f);
    }
    hit_result = 0;
    collided_count = 0;
    fast_vec_scale(spacing, &step, &direction);
    qcopy(D_L00_001DB5C0, beam_state);
    qcopy(D_L00_001DB5C0 + 0x140, beam_state);
    beam = (Vec4 *)D_L00_001DB5C0;
    side_waves = (float *)D_L00_001DB9D0;
    for (seg = 1; seg < 20; seg++) {
        previous = (char *)&beam[seg - 1];
        side = (char *)&beam[seg + 20];
        beam_wave_sample_index = (float)seg;
        factor = primary_beam_wave_start_amplitude +
                 (primary_beam_wave_end_amplitude - primary_beam_wave_start_amplitude) *
                     (convert_integer_to_float(seg) * 0.05f);
        phase = normalize_angle(D_L00_001615AC_gp + beam_wave_sample_index * D_L00_001615A0);
        wave = factor * fast_sin(phase);
        wave += randf_sym(primary_beam_vertical_jitter_range_start,
                          primary_beam_vertical_jitter_range_end);

        beam[seg].f[2] -= ((float *)D_L00_001DB840)[seg];
        ((float *)D_L00_001DB840)[seg] = wave;
        fast_vec_sub(&delta, &beam[seg], (char *)D_L00_001DB5C0 - 16 + seg * 16);
        blend_vector_2b94d0(0.85f, &blend, &delta, &step);
        {
            float blend_length = fast_vec_length(&blend);
            if (blend_length == 0.0f)
                qcopy(&beam[seg], previous);
            else {
                fast_vec_scale(spacing / blend_length, &blend, &blend);
                fast_vec_add(&beam[seg], (char *)D_L00_001DB5C0 - 16 + seg * 16, &blend);
                qcopy(&step, &blend);
            }
        }
        beam[seg].f[2] += wave;
        fast_vec_sub(&delta, &beam[seg], (char *)D_L00_001DB5C0 - 16 + seg * 16);
        if (beam_state->target) {
            if (!enabled || !beam_state->second_target) {
                fast_vec_sub(&target_delta, &target0, &beam[seg]);
                length_w4 = fast_vec_length(&target_delta);
                if (length_w4 != 0.0f) {
                    if (seg == 19) {
                        factor_w13 = 0.5f;
                        spacing = length_w4;
                    } else if (seg >= 16) {
                        factor_w13 = 0.5f;
                        spacing = length_w4 / convert_integer_to_float(19 - seg);
                    } else
                        factor_w13 = 0.1f;
                    fast_vec_scale(spacing / length_w4, &target_delta, &target_delta);
                    blend_vector_2b94d0(factor_w13, &step, &step, &target_delta);
                }
            } else {
                if (seg >= 10)
                    fast_vec_sub(&target_delta, &target1, &beam[seg]);
                else
                    fast_vec_sub(&target_delta, &target0, &beam[seg]);
                length_w4 = fast_vec_length(&target_delta);
                if (length_w4 != 0.0f) {
                    if (seg == 9 || seg == 19) {
                        factor_w13 = 0.7f;
                        spacing = length_w4;
                    } else if ((unsigned)(seg - 5) < 5) {
                        factor_w13 = 0.7f;
                        spacing = length_w4 / convert_integer_to_float(9 - seg);
                    } else if (seg > 14) {
                        factor_w13 = 0.7f;
                        spacing = length_w4 / convert_integer_to_float(19 - seg);
                    } else
                        factor_w13 = 0.2f;
                    fast_vec_scale(spacing / length_w4, &target_delta, &target_delta);
                    blend_vector_2b94d0(factor_w13, &step, &step, &target_delta);
                }
            }
        }
        if (seg < 10) {
            side_phase = normalize_angle(D_L00_001615B4_gp + (float)seg * D_L00_001615BC);
            angle = normalize_angle(D_L00_001615C8_gp + (float)seg * D_L00_001615D0);
            factor = secondary_beam_vertical_wave_envelope_amplitude * fast_sin(angle);
            qcopy(side, &beam[seg]);
            x_s = fast_sin(side_phase);
            side_wave = randf_sym(0.0f, 0.1f);
            x_c = factor * x_s + side_wave;
            side_waves[seg] = x_c;
            *(float *)((char *)&beam[seg] + 0x148) += x_c;
            x_a = fast_add_rotations(fast_arc_tan(delta.f[0], delta.f[1]), 1.5707964f);
            x_m = secondary_beam_horizontal_wave_amplitude * fast_cos(side_phase);
            offset.f[0] = fast_cos(x_a) * x_m;
            offset.f[1] = fast_sin(x_a) * x_m;
            offset.f[2] = 0.0f;
            qcopy_nc(&D_L00_001DB890[seg], &offset);
            fast_vec_add(side, side, &offset);
        } else {
            mirror = 21 - seg;
            qcopy(side, &beam[seg]);
            *(float *)((char *)&beam[seg] + 0x148) += side_waves[mirror];
            fast_vec_add(side, side, &D_L00_001DB890[mirror]);
            qcopy(&offset, &D_L00_001DB890[seg]);
        }
        if (seg == 1) {
            struct Hero *collision_source_hero = &hero;
            fast_vec_scale(0.5f, &effects.first.origin, (char *)collision_source_hero->moby + 0xe0);
            fast_vec_add(&target_delta, &collision_source_hero->motion.pos, &effects.first.origin);
            hit_result = collision_line(
                (Vec4f *)&target_delta, (Vec4f *)beam_state, 0x10, collision_source_hero->moby, 0);
        } else if (seg == 19 || !(seg & 1)) {
            hit_result = collision_line((Vec4f *)((char *)D_L00_001DB5C0 - 32 + seg * 16),
                                        (Vec4f *)&beam[seg],
                                        0x10,
                                        beam_owner_moby,
                                        0);
        } else
            continue;
        fast_vec_sub(&camera_direction, &beam[seg], &g_debug_camera);
        camera_direction.f[2] = 0.0f;
        length = fast_vec_length(&camera_direction);
        if (length == 0.0f)
            camera_direction.f[2] = 1.0f;
        else
            fast_vec_scale(1.0f / length, &camera_direction, &camera_direction);
        camera_direction.f[3] = 5627.9248046875f;
        qcopy(&hit.direction, &camera_direction);
        if (hit_result) {
            candidate = (struct Moby *)coll_hit.moby;
            if (candidate) {
                if (!get_moby_target_info(candidate) || !candidate->pclass ||
                    *(short *)((char *)candidate->pclass + 0x46) != 5)
                    record_moby_hit(candidate, &hit);
                else {
                    hit_result = 0;
                    collided[collided_count++] = candidate;
                    candidate->collision = 0;
                }
            }
            if (hit_result) {
                qcopy(&beam[seg], &coll_hit.point);
                beam_state->count3e = seg;
                qcopy(&beam_state->tip, &coll_hit.point);
                if (!coll_hit.moby || test_moby_linked_record_word8(coll_hit.moby)) {
                    beam_state->target = 0;
                    beam_state->target_timer = 0;
                }
                break;
            }
        }
    }
    for (k = 0; k < collided_count; k++)
        collided[k]->collision = *(int *)((char *)collided[k]->pclass + 0x10);
    if (!hit_result)
        qcopy(&beam_state->tip, D_L00_001DB5C0 + 0x130);
    if (beam_state->target && !(*(unsigned long *)&beam_state->target_timer & 0xffff0000ffffUL)) {
        fast_vec_sub(&target_delta, &beam_state->target->pos, &g_debug_camera);
        target_delta.f[2] = 0.0f;
        fast_vec_normalize(&target_delta, &target_delta, 1.0f);
        target_delta.f[3] = 5627.9248046875f;
        make_hit_descriptor(
            &effects.first.hit0, beam_owner_moby, 0x210000, &target_delta, enabled + 2.0f);
        effects.first.hit0.kind = 5;
        effects.first.hit0.mode = 3;
        effects.first.hit0.id = beam_owner_moby->oclass;
        record_moby_hit(beam_state->target, &effects.first.hit0);
        {
            char *aim;
            int   frames = 5;
            aim = (char *)get_moby_target_info(beam_state->target);
            if (aim && *(short *)(aim + 4) == 1)
                frames = 1;
            beam_state->unk44 = scale_game_frames(frames);
        }
    }
    if (beam_state->second_target &&
        !(*(unsigned long *)&beam_state->target_timer & 0xffff0000ffff0000UL)) {
        fast_vec_sub(&target_delta, &beam_state->second_target->pos, &g_debug_camera);
        target_delta.f[2] = 0.0f;
        fast_vec_normalize(&target_delta, &target_delta, 1.0f);
        target_delta.f[3] = 5627.9248046875f;
        make_hit_descriptor(
            &effects.hit1, beam_owner_moby, 0x210000, &target_delta, enabled + 2.0f);
        effects.hit1.kind = 5;
        effects.hit1.mode = 3;
        effects.hit1.id = beam_owner_moby->oclass;
        record_moby_hit(beam_state->second_target, &effects.hit1);
        {
            char *aim;
            int   frames = 5;
            aim = (char *)get_moby_target_info(beam_state->second_target);
            if (aim && *(short *)(aim + 4) == 1)
                frames = 1;
            beam_state->unk46 = scale_game_frames(frames);
        }
    }
    for (i = 0; i < 4; i++) {
        Vec4 *rows;
        if (i < 2) {
            qcopy(&effects.first.origin, beam_state);
            factor_w15 = 0.4f;
        } else {
            qcopy(&effects.first.origin, &beam_state->tip);
            factor_w15 = 0.5f;
        }
        if (fast_dec_timer_s16(&D_L00_00161658[i])) {
            D_L00_00161658[i] = scale_game_frames(D_L00_00161668[i]);
            (&D_L00_00161660)[i] = 0;
            angle = randf_sym(0.5235988f, 1.0471976f);
            phase = random_float_between(-1.0471976f, 0.17453292f);
            fast_vec_scale(factor_w15, &target_delta, &direction);
            rows = (Vec4 *)(D_0013F3D0 + 0x5d0);
            build_look_at_matrix(&target_delta, &target_delta, rows, phase);
            build_look_at_matrix(&target_delta, &target_delta, rows + 1, angle);
            qcopy(&arc_points[i][0], &effects.first.origin);
            fast_vec_add(&arc_points[i][1], &effects.first.origin, &target_delta);
            sign = 1.0f;
            for (arc_point_index = 2; arc_point_index < 5; arc_point_index++) {
                fast_vec_sub(
                    &effects.first.hit0, &arc_points[i][-1] + arc_point_index, &g_debug_camera);
                phase = random_float_between(0.17453292f, 0.9599311f) * sign;
                sign = -sign;
                build_look_at_matrix(&target_delta, &target_delta, &effects.first.hit0, phase);
                fast_vec_add(&arc_points[i][arc_point_index],
                             &arc_points[i][-1] + arc_point_index,
                             &target_delta);
            }
        } else {
            int arc_alpha_timer_delta, arc_countdown_frames;
            for (j = 1; j < 5; j++) {
                fast_vec_sub(&differences[j], &arc_points[i][j], &arc_points[i][-1] + j);
                differences[j].f[0] += randf_sym(0.0f, 0.1f);
                differences[j].f[1] += randf_sym(0.0f, 0.1f);
                differences[j].f[2] += randf_sym(0.0f, 0.1f);
            }
            qcopy(&arc_points[i][0], &effects.first.origin);
            for (j = 1; j < 5; j++)
                fast_vec_add(&arc_points[i][j], &arc_points[i][-1] + j, &differences[j]);
            arc_alpha_timer_delta = D_L00_00161670[i];
            arc_countdown_frames = D_L00_00161658[i];
            if (arc_alpha_timer_delta < arc_countdown_frames)
                arc_alpha_timer_delta = arc_countdown_frames - arc_alpha_timer_delta;
            else
                arc_alpha_timer_delta = arc_alpha_timer_delta - arc_countdown_frames;
            distance = convert_integer_to_float(arc_alpha_timer_delta) /
                       convert_integer_to_float(scale_game_frames(15));
            (&D_L00_00161660)[i] = truncate_float_to_s32((1.0f - distance) * 32.0f);
        }
    }
    angle = randf_sym(0.5235988f, 1.0471976f);
    phase = random_float_between(-0.5235988f, 1.0471976f);
    fast_vec_scale(
        beam_origin_particle_direction_scale * frame_scale_macro, &target_delta, &direction);
    build_look_at_matrix(&target_delta, &target_delta, D_0013F3D0 + 0x5d0, phase);
    build_look_at_matrix(&target_delta, &target_delta, D_0013F3D0 + 0x5e0, angle);
    spin = FUN_L00_00257b90(1, 8);
    if (FUN_00213260(2))
        spin = -spin;
    amplitude = random_float_between(0.2f, 0.6f);
    FUN_L00_00272060(beam_state,
                     scale_game_frames(15),
                     amplitude * 0.3f,
                     amplitude,
                     D_L00_001615F8 | 0x7f000000,
                     0,
                     spin,
                     (float *)&target_delta,
                     0.0f);
    spin = -spin;
    FUN_L00_00272060(beam_state,
                     scale_game_frames(15),
                     amplitude * 0.15f,
                     amplitude * 0.5f,
                     0x307f7f7f,
                     0,
                     spin,
                     (float *)&target_delta,
                     0.0f);
    spin = FUN_L00_00257b90(1, 8);
    if (FUN_00213260(2))
        spin = -spin;
    amplitude = random_float_between(0.15f, 3.0f);
    {
        /* Fakematch: a never-taken branch (cprop proves dead == 0, then jump deletes it). Retail
           had about 30 more insns before gcse that vanish later; they size the PRE hash table,
           which orders the &target_delta and &effects spill slots. */
        int dead = 0;
        if (amplitude < 0.0f)
            dead = 0;
        if (dead) {
            FUN_L00_00272060(beam_state,
                             scale_game_frames(15),
                             amplitude,
                             amplitude,
                             0,
                             0,
                             spin,
                             (float *)&target_delta,
                             0.0f);
            FUN_L00_00272060(beam_state,
                             scale_game_frames(15),
                             amplitude,
                             amplitude,
                             0,
                             0,
                             spin,
                             (float *)&target_delta,
                             0.0f);
            FUN_L00_00272060(beam_state,
                             scale_game_frames(15),
                             amplitude,
                             amplitude,
                             0,
                             0,
                             spin,
                             (float *)&target_delta,
                             0.0f);
        }
    }
    FUN_L00_00272060(&beam_state->tip,
                     scale_game_frames(12),
                     amplitude * 0.1f,
                     amplitude,
                     D_L00_001615F8 | 0x7f000000,
                     0,
                     spin,
                     zero_vector_macro.f,
                     0.0f);
}

void draw_beam_and_arc_effects(struct Moby *moby) __asm__("FUN_L00_002ba970");

void draw_beam_and_arc_effects(struct Moby *moby) {
    S2ba970 *moby_vars = ((S2ba970 *)moby->pvars);
    beam_first_pass_texture_u_offset_gp -=
        beam_first_pass_texture_u_scroll_speed_gp * frame_scale_macro;
    if (beam_first_pass_texture_u_offset_gp <= -8.0f) {
        beam_first_pass_texture_u_offset_gp += 8.0f;
    }
    draw_beam_and_arc_two_pass_quads(D_L00_001DB5C0,
                                     primary_beam_first_pass_alpha_gp,
                                     primary_beam_second_pass_alpha_gp,
                                     moby_vars->h3E,
                                     beam_and_arc_first_pass_half_width_gp,
                                     primary_beam_second_pass_half_width_gp,
                                     primary_beam_second_pass_segment_extension_factor_gp);
    draw_beam_and_arc_two_pass_quads(D_L00_001DB5C0 + 0x140,
                                     secondary_beam_first_pass_alpha_gp,
                                     secondary_beam_second_pass_alpha_gp,
                                     moby_vars->h3E,
                                     D_L00_0016160C,
                                     secondary_beam_and_arc_second_pass_half_width_gp,
                                     secondary_beam_second_pass_segment_extension_factor_gp);
    draw_beam_and_arc_two_pass_quads(arc_points[0],
                                     D_L00_00161660,
                                     D_L00_00161660,
                                     5,
                                     D_L00_0016160C,
                                     secondary_beam_and_arc_second_pass_half_width_gp,
                                     0.2f);
    draw_beam_and_arc_two_pass_quads(arc_points[1],
                                     beam_arc_second_row_alpha,
                                     beam_arc_second_row_alpha,
                                     5,
                                     D_L00_0016160C,
                                     secondary_beam_and_arc_second_pass_half_width_gp,
                                     0.2f);
    draw_beam_and_arc_two_pass_quads(arc_points[2],
                                     beam_arc_third_row_alpha,
                                     beam_arc_third_row_alpha,
                                     5,
                                     D_L00_0016160C,
                                     secondary_beam_and_arc_second_pass_half_width_gp,
                                     0.2f);
    draw_beam_and_arc_two_pass_quads(arc_points[3],
                                     beam_arc_fourth_row_alpha,
                                     beam_arc_fourth_row_alpha,
                                     5,
                                     D_L00_0016160C,
                                     secondary_beam_and_arc_second_pass_half_width_gp,
                                     0.2f);
    draw_beam_anchor_camera_facing_quads(moby);
}

void draw_beam_and_arc_two_pass_quads(void *mv,
                                      int   first_pass_alpha,
                                      int   second_pass_alpha,
                                      int   point_count,
                                      float sx,
                                      float sy,

                                      float sz) {
    char      *point_bytes = mv;
    RW_2baae0 *v = mv;
    RW_2baae0  first_pass_corners[4];
    RT_2baae0  first_pass_render_attributes;
    RW_2baae0  q2[4];
    RT_2baae0  u;
    RW_2baae0  camera_facing_side_vector, second_pass_width_offset, first_pass_point_camera_offset,
        segment_delta, adjusted_segment_start, adjusted_segment_end, segment_start_camera_offset,
        segment_end_camera_offset, s1A0, s1B0;
    int   i, col0, col1;
    float first_pass_side_normalization_factor, initial_first_pass_side_normalization_factor,
        segment_start_camera_distance, segment_end_camera_distance,
        second_pass_side_normalization_factor;
    first_pass_render_attributes.q0 = get_effect_texture(beam_and_arc_first_pass_texture_index_gp);
    col0 = beam_and_arc_first_pass_base_vertex_color_gp | (first_pass_alpha << 24);
    first_pass_render_attributes.q2 = 0x8000000048L;
    first_pass_render_attributes.q1 = 0xFF9000000260L;
    first_pass_render_attributes.z4 = 0;
    first_pass_render_attributes.c[3] = col0;
    first_pass_render_attributes.c[2] = col0;
    first_pass_render_attributes.c[1] = col0;
    first_pass_render_attributes.c[0] = col0;
    u.q0 = get_effect_texture(beam_and_arc_second_pass_texture_index_gp);
    col1 = D_L00_001615F8 | (second_pass_alpha << 24);
    u.q2 = first_pass_render_attributes.q2;
    u.q1 = first_pass_render_attributes.q1;
    u.z4 = first_pass_render_attributes.z4;
    u.c[3] = col1;
    u.c[2] = col1;
    u.c[1] = col1;
    u.c[0] = col1;
    u.f[0] = 0;
    u.f[1] = 0;
    u.f[2] = 0;
    u.f[3] = 1.0f;
    u.f[4] = 1.0f;
    u.f[5] = 0;
    u.f[6] = 1.0f;
    u.f[7] = 1.0f;
    first_pass_render_attributes.f[0] = beam_first_pass_texture_u_offset_gp + 0;
    first_pass_render_attributes.f[1] = 0;
    first_pass_render_attributes.f[2] = beam_first_pass_texture_u_offset_gp + 0;
    first_pass_render_attributes.f[3] = 1.0f;
    first_pass_render_attributes.f[4] = beam_first_pass_texture_u_offset_gp + 1.0f;
    first_pass_render_attributes.f[5] = 0;
    first_pass_render_attributes.f[6] = beam_first_pass_texture_u_offset_gp + 1.0f;
    first_pass_render_attributes.f[7] = 1.0f;
    fast_vec_sub(first_pass_point_camera_offset, point_bytes, &g_debug_camera);
    fast_vec_sub(segment_delta, point_bytes + 0x10, point_bytes);
    fast_vec_cross(camera_facing_side_vector, segment_delta, first_pass_point_camera_offset);
    initial_first_pass_side_normalization_factor = fast_vec_length(camera_facing_side_vector);
    if (initial_first_pass_side_normalization_factor != 0)
        initial_first_pass_side_normalization_factor =
            1.0f / initial_first_pass_side_normalization_factor;
    fast_vec_scale(beam_and_arc_first_pass_half_width_gp *
                       initial_first_pass_side_normalization_factor,
                   camera_facing_side_vector,
                   camera_facing_side_vector);
    fast_vec_add(first_pass_corners[0], point_bytes, camera_facing_side_vector);
    fast_vec_sub(first_pass_corners[1], point_bytes, camera_facing_side_vector);
    for (i = 1; i < point_count - 1; i++) {
        fast_vec_sub(first_pass_point_camera_offset, v[i], &g_debug_camera);
        fast_vec_sub(segment_delta, v[i + 1], v[i]);
        fast_vec_cross(camera_facing_side_vector, segment_delta, first_pass_point_camera_offset);
        first_pass_side_normalization_factor = fast_vec_length(camera_facing_side_vector);
        if (first_pass_side_normalization_factor != 0)
            first_pass_side_normalization_factor = 1.0f / first_pass_side_normalization_factor;
        fast_vec_scale(beam_and_arc_first_pass_half_width_gp * first_pass_side_normalization_factor,
                       camera_facing_side_vector,
                       camera_facing_side_vector);
        fast_vec_add(first_pass_corners[2], v[i], camera_facing_side_vector);
        fast_vec_sub(first_pass_corners[3], v[i], camera_facing_side_vector);
        if (i == 1) {
            first_pass_render_attributes.c[1] = beam_and_arc_first_pass_base_vertex_color_gp;
            first_pass_render_attributes.c[0] = beam_and_arc_first_pass_base_vertex_color_gp;
        } else if (i == point_count - 2) {
            first_pass_render_attributes.c[3] = beam_and_arc_first_pass_base_vertex_color_gp;
            first_pass_render_attributes.c[2] = beam_and_arc_first_pass_base_vertex_color_gp;
        } else if (i == 2) {
            first_pass_render_attributes.c[1] = col0;
            first_pass_render_attributes.c[0] = col0;
        }
        draw_geometry_quad(first_pass_corners, 0, 0);
        qcopy(first_pass_corners[0], first_pass_corners[2]);
        qcopy(first_pass_corners[1], first_pass_corners[3]);
    }
    for (i = 0; i < point_count - 1; i++) {
        fast_vec_sub(segment_start_camera_offset, point_bytes + i * 16, &g_debug_camera);
        fast_vec_sub(segment_end_camera_offset, point_bytes + i * 16 + 16, &g_debug_camera);
        segment_start_camera_distance = fast_vec_length(segment_start_camera_offset);
        segment_end_camera_distance = fast_vec_length(segment_end_camera_offset);
        if (segment_end_camera_distance < segment_start_camera_distance) {
            if (segment_end_camera_distance == 0)
                qcopy(segment_end_camera_offset, segment_start_camera_offset);
            else
                fast_vec_scale(segment_start_camera_distance / segment_end_camera_distance,
                               segment_end_camera_offset,
                               segment_end_camera_offset);
        } else {
            if (segment_start_camera_distance == 0)
                qcopy(segment_start_camera_offset, segment_end_camera_offset);
            else
                fast_vec_scale(segment_end_camera_distance / segment_start_camera_distance,
                               segment_start_camera_offset,
                               segment_start_camera_offset);
        }
        fast_vec_add(adjusted_segment_start, &g_debug_camera, segment_start_camera_offset);
        fast_vec_add(adjusted_segment_end, &g_debug_camera, segment_end_camera_offset);
        fast_vec_sub(segment_delta, adjusted_segment_end, adjusted_segment_start);
        fast_vec_scale(sz, s1B0, segment_delta);
        fast_vec_sub(adjusted_segment_start, adjusted_segment_start, s1B0);
        fast_vec_add(adjusted_segment_end, adjusted_segment_end, s1B0);
        fast_vec_sub(s1A0, &g_debug_camera, adjusted_segment_start);
        fast_vec_cross(camera_facing_side_vector, s1A0, segment_delta);
        second_pass_side_normalization_factor = fast_vec_length(camera_facing_side_vector);
        if (second_pass_side_normalization_factor != 0)
            second_pass_side_normalization_factor = 1.0f / second_pass_side_normalization_factor;
        fast_vec_scale(sy * second_pass_side_normalization_factor,
                       second_pass_width_offset,
                       camera_facing_side_vector);
        fast_vec_add(q2[0], adjusted_segment_start, second_pass_width_offset);
        fast_vec_sub(q2[1], adjusted_segment_start, second_pass_width_offset);
        fast_vec_add(q2[2], adjusted_segment_end, second_pass_width_offset);
        fast_vec_sub(q2[3], adjusted_segment_end, second_pass_width_offset);
        if (i == 0) {
            u.c[1] = D_L00_001615F8;
            u.c[0] = D_L00_001615F8;
        } else if (i == point_count - 2) {
            u.c[3] = D_L00_001615F8;
            u.c[2] = D_L00_001615F8;
        } else if (i == 1) {
            u.c[1] = col1;
            u.c[0] = col1;
        }
        draw_geometry_quad(q2, 0, 0);
    }
    vu1_add_g_sregister(0x47, 0x5360B);
}

void draw_beam_anchor_camera_facing_quads(void *mv) {
    Q_2bb0b0     quad;
    V_2bb0b0     camera_facing_transform[4];
    V_2bb0b0    *quad_anchor_position = *(V_2bb0b0 **)((char *)mv + 0x78);
    unsigned int quad_color;
    float        corner_scale;
    int          i;
    quad.tex = get_effect_texture(0xB);
    quad.unk88 = 0x8000000048L;
    quad.unk80 = 0xFF9000000260L;
    quad.unk70 = 0;
    fast_vec_sub(
        &camera_facing_transform[3], &(*(V_2bb0b0 *)&g_debug_camera), quad_anchor_position);
    fast_vec_normalize(&camera_facing_transform[3], &camera_facing_transform[3], 0.3f);
    fast_vec_add(&camera_facing_transform[3], &camera_facing_transform[3], quad_anchor_position);
    camera_facing_transform[3].f[3] = 1.0f;
    fast_vec_sub(
        &camera_facing_transform[0], &(*(V_2bb0b0 *)&g_debug_camera), &camera_facing_transform[3]);
    fast_vec_normalize(&camera_facing_transform[0], &camera_facing_transform[0], 1.0f);
    fast_vec_cross(
        &camera_facing_transform[1], &camera_facing_transform[0], &(*(V_2bb0b0 *)D_0013F5E0));
    fast_vec_normalize(&camera_facing_transform[1], &camera_facing_transform[1], -1.0f);
    fast_vec_cross(
        &camera_facing_transform[2], &camera_facing_transform[1], &camera_facing_transform[0]);
    quad_color = l00_beam_anchor_first_pass_vertex_color;
    quad.color[3] = quad_color;
    quad.color[2] = quad_color;
    quad.color[1] = quad_color;
    quad.color[0] = quad_color;
    corner_scale = beam_anchor_first_pass_base_corner_scale_gp + randf_sym(0.0f, 0.025f);
    for (i = 0; i < 4; i++) {
        fast_vec_scale(corner_scale, &quad.corner[i], &D_L00_001DB560[i]);
        transform_vector(&quad.corner[i], &quad.corner[i], &camera_facing_transform[0]);
        quad.uv[i].u = D_L00_001DB5A0[i].u;
        quad.uv[i].v = D_L00_001DB5A0[i].v;
    }
    draw_geometry_quad(&quad, 0, 0);
    quad_color = D_L00_001615F8_gp;
    quad.color[3] = quad_color;
    quad.color[2] = quad_color;
    quad.color[1] = quad_color;
    quad.color[0] = quad_color;
    corner_scale = beam_anchor_second_pass_base_corner_scale_gp + randf_sym(0.0f, 0.05f);
    for (i = 0; i < 4; i++) {
        fast_vec_scale(corner_scale, &quad.corner[i], &D_L00_001DB560[i]);
        transform_vector(&quad.corner[i], &quad.corner[i], &camera_facing_transform[0]);
        quad.uv[i].u = D_L00_001DB5A0[i].u;
        quad.uv[i].v = D_L00_001DB5A0[i].v;
    }
    draw_geometry_quad(&quad, 0, 0);
}
