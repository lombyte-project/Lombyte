/* Shared moby class morphoray: the morph-o-ray held by the hero, its target selection, and the ring, rail and pulse beam effects. */

#include "types.h"
#include "rnc/game/item_state.h"
#include "rnc/overlay/point_light.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/math/random.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/rendering/vu1_registers.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/audio/voices/allocate_voice_for_bank_entry.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/hero/hero_switch_item.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/rendering/effects/glow_quads_draw.h"
#include "rnc/ui/hud/remove_hud_item.h"
#include "rnc/data_labels.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/overlay/shared/weapons/morphoray_globals.h"
#include "rnc/overlay/shared/weapons/morphoray_data/select_morphoray_target_and_adjust_aim.h"
#include "rnc/overlay/hud.h"
#include "rnc/overlay/shared/weapons/morphoray_data/update_beam_ring_rail_and_pulse_effects.h"
#include "rnc/overlay/shared/weapons/morphoray_data/draw_beam_ring_and_rail_effects_4.h"
#include "rnc/overlay/shared/weapons/morphoray_data/draw_beam_ring_and_rail_effects_3.h"
#include "rnc/overlay/shared/weapons/morphoray_data/draw_beam_ring_and_rail_effects_2.h"
#include "rnc/overlay/shared/weapons/morphoray_data/draw_beam_ring_and_rail_effects.h"
#include "rnc/rendering/geometry_quad.h"
#include "rnc/overlay/shared/weapons/tesla_claw_beam.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/game/game_globals.h"
#include "rnc/input/pad_globals.h"
#include "rnc/rendering/debug/debug_menu.h"

typedef float RW_2baae0[4] __attribute__((aligned(16)));

typedef u32 VecQuad __attribute__((mode(TI), aligned(16)));

typedef struct Hero Hero;

typedef struct Moby Moby;

typedef int u128_2BDAF0 __attribute__((mode(TI)));

typedef struct Vq {
    float x, y, z, w;
} __attribute__((aligned(16))) Vq;

typedef struct Ray {
    float pos[4];
    Moby *target;
    float total;
    float left;
    short timer;
    short seen;
    char  p0[0x34 - 0x20];
    int   snd;
    char  p1[0x44 - 0x38];
    int   delay;
    float yaw;
    float pitch;
} Ray;

typedef struct Stats {
    char           p0[0xA8];
    unsigned short kills;
    unsigned short best;
    int            levels;
} Stats;

typedef struct Voice {
    char          p0[0x74];
    unsigned char on;
    char          p1[0x88 - 0x75];
    Moby         *owner;
} Voice;

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) Vec_2BE3A8;

typedef struct {
    Vec_2BE3A8   pos;
    struct Moby *target;
    char         pad14[0x34];
    float        f48;
    float        f4C;
} Vars;

typedef struct {
    char  pad0[0x10];
    float height;
} Info;

typedef struct {
    int c[5];
} Tab;

typedef struct {
    float f[4];
} __attribute__((aligned(16))) V_t;

typedef struct {
    OvlQuad corner[4];
    int     color[4];
    float   uv[8];
    long    unk70, tex, unk80, unk88;
} Quad;

typedef struct {
    float f[4];
} __attribute__((aligned(16))) V_2be890;

typedef struct {
    V_2be890 corner[4];
    int      color[4];
    int      unk50, unk54, unk58;
    float    unk5C, unk60;
    int      unk64;
    float    unk68, unk6C;
    long     unk70, tex, unk80, unk88;
} Q_2be890;

extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern float fast_vec_dist(void *, void *) __asm__("FUN_001f9b48");
extern void  fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void  fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void  fast_vec_cross(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void  fast_vec_scale(float, void *, void *) __asm__("FUN_001f9a68");
extern void  fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float convert_integer_to_float(int) __asm__("FUN_001fa6c0");
extern int   truncate_float_to_s32(float) __asm__("FUN_001fa6d0");
extern int   get_moby_target_info() __asm__("FUN_002141f8");
extern float normalize_angle(float) __asm__("FUN_L00_0025e310");
extern int   collision_line(Vec4f *, Vec4f *, int, void *, int) __asm__("FUN_001efa68");
extern long  get_effect_texture(int) __asm__("FUN_001f44b8");
extern void  draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");
void         transform_vector(void *, void *, void *) __asm__("FUN_001f9d20");
extern void
build_look_at_matrix(void *dst, void *vec, void *axis, float angle) __asm__("FUN_00214890");
extern void  draw_geometry_quad(void *, void *, int);
extern f32   fast_arc_sin(f32) __asm__("func_001F9DF8");
extern float fast_vec_dist(void *, void *);
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
float        absolute_float(float input) __asm__("FUN_001f99c0");
extern int   draw_uniform_size_hud_stars() __asm__("FUN_L00_00236e50");
int          allocate_packed_color_point_light_slot(void *v,
                                                    float a,
                                                    float b,
                                                    int   col) __asm__("FUN_L00_0023e5e0");
struct Moby *spawn_morphoray_chicken(void *a) __asm__("FUN_L00_002c9348");
void         start_equipped_item_animation(void) __asm__("FUN_L00_0020e4c8");
void         hud_init_particles(HudElem *e) __asm__("FUN_L00_00236268");
void         hud_update_particles(void) __asm__("FUN_L00_00236458");
extern int   func_001F9938_2BDAF0(void *) __asm__("FUN_001f9770");
extern int   func_001F9850_2BDAF0(int) __asm__("FUN_001f96f8");
extern void  hero_set_state(int, int) __asm__("FUN_L00_002223f8");
extern void  copy_matrix3x4(void *, void *) __asm__("FUN_001fa2b8");
extern void  func_001FA4A0_2BDAF0(void *, void *) __asm__("FUN_001fa2d8");
extern void  func_001F9EE8_2BDAF0(void *, void *, void *) __asm__("FUN_001f9d20");
extern void  func_001F9C30_2BDAF0(void *, void *, float) __asm__("FUN_001f9a68");
extern void  transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
extern float func_001F9CE8_2BDAF0(void *) __asm__("FUN_001f9b20");
extern int   func_001F9908_2BDAF0(void *) __asm__("FUN_001f9740");
extern float func_001FA888_2BDAF0(int) __asm__("FUN_001fa6c0");
extern int
queue_animation_update(int, int, void *, void *, void *, int *, int) __asm__("FUN_001ff308");
extern int  func_001FA898_2BDAF0(float) __asm__("FUN_001fa6d0");
extern void func_L00_002BE3A8_2BDAF0(void *) __asm__("FUN_L00_002bd0a0");
extern void func_L00_001FF4B0_2BDAF0(void *, void *, float) __asm__("FUN_001f9bf8");
extern void func_001F9BD8_2BDAF0(void *, void *, void *) __asm__("FUN_001f9a10");
extern void scale_vector_2bcf98(float, void *, void *) __asm__("FUN_001f9a68");
unsigned char              *
spawn_proximity_corrected_size_alpha_tween_particle(void  *pos,
                                                                 int    s,
                                                                 int    col,
                                                                 int    mode,
                                                                 int    b,
                                                                 float *vec,
                                                                 int    h,
                                                                 float  x,
                                                                 float  y) __asm__("FUN_L00_00275910");
void         draw_beam_ring_and_rail_effects(void *o) __asm__("FUN_L00_002bdc50");
extern void  func_L00_002BE2A0_2BE3A8(void) __asm__("FUN_L00_002bcf98");
extern void  func_001F9BD8_2BE3A8(void *, void *, void *) __asm__("FUN_001f9a10");
extern int   func_L00_001EFFF0_2BE3A8(void *, void *, int, void *, int) __asm__("FUN_001efa68");
extern void  func_001F9BF0_2BE3A8(void *, void *, void *) __asm__("FUN_001f9a28");
extern float func_001FA888_2BE3A8(int) __asm__("FUN_001fa6c0");
extern void  func_L00_001FF4B0_2BE3A8(void *, void *, float) __asm__("FUN_001f9bf8");
extern void  func_001F9C08_2BE3A8(void *, void *, void *, float) __asm__("FUN_001f9a40");
extern void  func_001F9C30_2BE3A8(void *, void *, float) __asm__("FUN_001f9a68");
extern void  func_001F9CA0_2BE3A8(void *, void *, void *) __asm__("FUN_001f9ad8");
extern float func_001F9FA8_2BE3A8(float) __asm__("FUN_001f9de0");
extern float func_001F9F90_2BE3A8(float) __asm__("FUN_001f9dc8");
extern void  func_001F9EE8_2BE3A8(void *, void *, void *) __asm__("FUN_001f9d20");
extern int   func_001F9850_2BE3A8(int) __asm__("FUN_001f96f8");
extern void  func_L00_00272F00_2BE3A8(
     void *, int, int, int, int, void *, float, float, float) __asm__("FUN_L00_00272060");
extern int func_001FA898_2BE3A8(float) __asm__("FUN_001fa6d0");
extern int func_001F9908_2BE3A8(int *) __asm__("FUN_001f9740");
void       draw_scrolling_ribbon_colored(void *, int, int, float, float, float, int, int) __asm__("FUN_L00_002be078");
void  draw_beam_pulse_quads(void *unused) __asm__("FUN_L00_002be650");
void  draw_beam_origin_camera_facing_quads(void *mv) __asm__("FUN_L00_002be890");
long  FUN_001f44b8(int);
float fast_vec_dot(void *, void *) __asm__("FUN_001f9ab0");
void  FUN_001f9a68_2be890(float, void *, void *) __asm__("FUN_001f9a68");
void  FUN_001f9a10_2be890(void *, void *, void *) __asm__("FUN_001f9a10");
void  FUN_001f9a28_2be890(void *, void *, void *) __asm__("FUN_001f9a28");
void  FUN_001f9bf8_2be890(void *, void *, float) __asm__("FUN_001f9bf8");
void  FUN_001f9ad8_2be890(void *, void *, void *) __asm__("FUN_001f9ad8");
void  FUN_001f9d20_2be890(void *, void *, void *) __asm__("FUN_001f9d20");
void  FUN_001f7d30_2be890(void *, void *, int) __asm__("FUN_001f7d30");

extern char         *active_mobies[] __asm__("D_L00_001ABA00");
extern Vec4          zero_vector_macro __asm__("D_L00_0015F580") MACRO_ADDR;
extern char          D_0013F5E0[];
extern Vec4          collision_hit_point __asm__("D_L00_00173E60");
extern int           D_L00_00173E40[];
extern float         D_L00_001616DC __attribute__((sda));
extern float         D_0015ED6C;
extern int           D_0015EEA4;
extern float         morphoray_base_drain_rate_gp __asm__("D_L00_001616E0") __attribute__((sda));
extern int           D_L00_001616EC __attribute__((sda));
extern int           D_L00_001617BC;
extern int           D_L00_001617C8;
extern unsigned char D_0013E633_2BDAF0[] __asm__("D_0013E533");
extern f32           D_L00_001616DC_gp __asm__("D_L00_001616DC") __attribute__((sda));
extern f32           D_L00_001617CC_macro __asm__("D_L00_001617CC") MACRO_ADDR;
extern s32           D_L00_00161718_gp __asm__("D_L00_00161718") __attribute__((sda));
extern s32           D_L00_00161720_gp __asm__("D_L00_00161720") __attribute__((sda));
extern s32           D_L00_00161724_gp __asm__("D_L00_00161724") __attribute__((sda));
extern float D_L00_001617D0 MACRO_ADDR;
extern s32                  D_L00_00161824;
extern s32                  D_L00_00161818;
extern Vec_2BE3A8           beam_prev_nodes[12] __asm__("D_L00_001DBF50");
extern Vec_2BE3A8           beam_nodes[] __asm__("D_L00_001DBF60") NOT_SDA;
extern Vec_2BE3A8           beam_pulse_directions[3] __asm__("D_L00_001DD0D0");
extern unsigned char        D_0013F5E0_r[] __asm__("D_0013F5E0");
extern float                D_L00_001616DC_gp __asm__("D_L00_001616DC") __attribute__((sda));
extern int                  D_L00_00161818;
extern int                  D_L00_00161824;
extern float                D_L00_001616F0_gp __asm__("D_L00_001616F0") __attribute__((sda));
extern int                  D_L00_001616F4_gp __asm__("D_L00_001616F4") __attribute__((sda));
extern float                D_L00_001616F8_gp __asm__("D_L00_001616F8") __attribute__((sda));
extern float                D_L00_001616FC_gp __asm__("D_L00_001616FC") __attribute__((sda));
extern float                D_L00_00161704_gp __asm__("D_L00_00161704") __attribute__((sda));
extern float                D_L00_00161708_gp __asm__("D_L00_00161708") __attribute__((sda));
extern float                D_L00_0016170C_gp __asm__("D_L00_0016170C") __attribute__((sda));
extern float                D_L00_0016172C_gp __asm__("D_L00_0016172C") __attribute__((sda));
extern float                D_L00_00161730_gp __asm__("D_L00_00161730") __attribute__((sda));
extern float                D_L00_00161734_gp __asm__("D_L00_00161734") __attribute__((sda));
extern float                D_L00_0016173C_gp __asm__("D_L00_0016173C") __attribute__((sda));
extern float                D_L00_00161740_gp __asm__("D_L00_00161740") __attribute__((sda));
extern float                D_L00_00161748_gp __asm__("D_L00_00161748") __attribute__((sda));
extern int                  D_L00_0016174C_gp __asm__("D_L00_0016174C") __attribute__((sda));
extern int                  D_L00_00161750_gp __asm__("D_L00_00161750") __attribute__((sda));
extern float                D_L00_00161758_gp __asm__("D_L00_00161758") __attribute__((sda));
extern float                D_L00_0016175C_gp __asm__("D_L00_0016175C") __attribute__((sda));
extern float                D_L00_00161768_gp __asm__("D_L00_00161768") __attribute__((sda));
extern signed char          D_L00_00161763_macro[] __asm__("D_L00_00161763") MACRO_ADDR;
extern float                D_L00_001617CC_macro __asm__("D_L00_001617CC") MACRO_ADDR;
extern int                  D_L00_00161824_macro __asm__("D_L00_00161824") MACRO_ADDR;
extern float                D_0015ED6C_gp __asm__("D_0015ED6C") MACRO_ADDR;
extern int                  D_L00_001617D8_macro[3] __asm__("D_L00_001617D8") MACRO_ADDR;
extern float                D_L00_001618B8_2BE3A8[3] __asm__("D_L00_001617E8") MACRO_ADDR;
extern float                D_L00_001617F8_macro[3] __asm__("D_L00_001617F8") MACRO_ADDR;
extern float                D_L00_001618D8_2BE3A8[3] __asm__("D_L00_00161808") MACRO_ADDR;
extern int                  D_L00_001618E8_2BE3A8[3] __asm__("D_L00_00161818") MACRO_ADDR;
extern Vec_2BE3A8           beam_rail_a_points[12] __asm__("D_L00_001DC020");
extern Vec_2BE3A8           beam_rail_b_points[12] __asm__("D_L00_001DC0E0");
extern Vec_2BE3A8           beam_pulse_positions[3] __asm__("D_L00_001DD0A0");
extern Vec_2BE3A8           D_L00_00173F60_2BE3A8 __asm__("D_L00_00173E60");
extern Tab                  l00_beam_origin_particle_color_palette __asm__("D_L00_001E9AB0");
extern u8 D_0013E535        NOT_SDA;
extern float                D_L00_00161718 __attribute__((sda));
extern float                D_L00_00161720 __attribute__((sda));
extern float                D_L00_00161724 __attribute__((sda));
extern int                  D_L00_00161798 __attribute__((sda));
extern OvlQuad              D_L00_001DC060[][20] NOT_SDA;
extern OvlQuad              D_L00_001DC1A0[][20] NOT_SDA;
extern int beam_rail_first_pass_base_vertex_color_gp __asm__("D_L00_00161780") __attribute__((sda));
extern int beam_rail_first_pass_texture_index_gp __asm__("D_L00_001617A4") __attribute__((sda));
extern int beam_rail_second_pass_texture_index_gp __asm__("D_L00_001617A8") __attribute__((sda));
extern int beam_pulse_default_rgb_gp __asm__("D_L00_00161760") __attribute__((sda));
extern int D_L00_00161764 __attribute__((sda));
extern float   D_L00_001617E8_macro[3] __asm__("D_L00_001617E8") MACRO_ADDR;
extern float   D_L00_00161808_macro[3] __asm__("D_L00_00161808") MACRO_ADDR;
extern int     D_L00_00161818_macro[3] __asm__("D_L00_00161818") MACRO_ADDR;
extern OvlVec4 D_L00_001DBF20[] NOT_SDA;
extern float beam_origin_quad_basis_offset_length_gp __asm__("D_L00_001617B4") __attribute__((sda));
extern float beam_origin_quad_first_pass_corner_scale_gp __asm__("D_L00_001617AC")
    __attribute__((sda));
extern float beam_origin_quad_second_pass_corner_scale_gp __asm__("D_L00_001617B0")
    __attribute__((sda));
extern int beam_origin_quad_first_pass_vertex_color_gp __asm__("D_L00_001617B8")
    __attribute__((sda));
extern int D_L00_00173E40[] NOT_SDA;

Vec_2BE3A8 D_L00_001DC1A0_2BE3A8[12][20] __asm__("D_L00_001DC1A0") = {0};

Vec_2BE3A8 D_L01_001DC620[12][20] = {0};

Vec_2BE3A8 D_L02_001D0C20[12][20] = {0};

Vec_2BE3A8 D_L03_001D0720[12][20] = {0};

Vec_2BE3A8 D_L04_001D07A0[12][20] = {0};

Vec_2BE3A8 D_L05_001D0AC0[12][20] = {0};

Vec_2BE3A8 D_L06_001D0DA0[12][20] = {0};

Vec_2BE3A8 D_L07_001D06A0[12][20] = {0};

Vec_2BE3A8 D_L08_001D0E20[12][20] = {0};

Vec_2BE3A8 D_L09_001D07A0[12][20] = {0};

Vec_2BE3A8 D_L10_001D0AA0[12][20] = {0};

Vec_2BE3A8 D_L11_001D1020[12][20] = {0};

Vec_2BE3A8 D_L12_001E36C0[12][20] = {0};

Vec_2BE3A8 D_L13_001D0F30[12][20] = {0};

Vec_2BE3A8 D_L14_001DD5E0[12][20] = {0};

Vec_2BE3A8 D_L15_001D10A0[12][20] = {0};

Vec_2BE3A8 D_L16_001D0AA0[12][20] = {0};

Vec_2BE3A8 D_L17_001D1320[12][20] = {0};

Vec_2BE3A8 D_L18_001D1020[12][20] = {0};

float              D_L01_00161710 = 3.0f;
int                D_L01_0016171C = 0;
int                D_L01_001617EC = 0;
int                D_L01_001617F8 = 0;
f32                D_L01_001617FC = 0;
s32                D_L01_00161748 = 0;
s32                D_L01_00161750 = 0;
s32                D_L01_00161754 = 0;
float              D_L01_00161800 = 0;
s32                D_L01_00161854 = 0;
s32                D_L01_00161848 = 0;
s32 D_L01_001DC3D0 NOT_SDA = 0;
s32 D_L01_001DD550 NOT_SDA = 0;
signed char        D_L01_00161793 = 0x40;
int                D_L01_00161818 = 0;
int                D_L01_00161838 = 0;
int D_L01_001DC3F0 NOT_SDA = 0;
int D_L01_001DD520 NOT_SDA = 0;
float              D_L01_00161720 = 0.08333f;
int                D_L01_00161724 = 6;
float              D_L01_00161728 = 0.20071286f;
float              D_L01_0016172C = 0.39968038f;
float              D_L01_00161734 = 0.3141592f;
float              D_L01_00161738 = 0.05f;
float              D_L01_0016173C = 1.234f;
float              D_L01_0016175C = 0.01f;
float              D_L01_00161760 = 0.5f;
float              D_L01_00161764 = 0.2f;
float              D_L01_0016176C = 0.2617994f;
float              D_L01_00161770 = -0.25f;
float              D_L01_00161778 = 8.0f;
int                D_L01_0016177C = 0x3c;
int                D_L01_00161780 = 0x1e;
float              D_L01_00161788 = 1.5f;
float              D_L01_0016178C = 0.1f;
float              D_L01_00161798 = 2.13333f;
int                D_L01_00161808[3] = {0};

float D_L01_00161828[3] = {0};

Vec_2BE3A8 D_L01_001DC4A0[12] = {0};

Vec_2BE3A8 D_L01_001DC560[12] = {0};

Tab D_L01_0020AB10 = {
    {0x7f207f7f, 0x7f7f2020, 0x7f20207f, 0x7f7f207f, 0x7f7f7f20}
};

int                D_L01_001617C8 = 0x60007f00;
int                D_L01_001617B0 = 0x7f7f7f;
int                D_L01_001617D4 = 0xe;
int                D_L01_001617D8 = 0x10;
int                D_L01_00161790 = 0x407f207f;
int                D_L01_00161794 = 0x60007f00;
float              D_L01_001617E4 = -0.5f;
float              D_L01_001617DC = 0.4f;
float              D_L01_001617E0 = 0.1f;
int                D_L01_001617E8 = 0x7f207f7f;
float              D_L02_001616B0 = 3.0f;
int                D_L02_001616BC = 0;
int                D_L02_0016178C = 0;
int                D_L02_00161798 = 0;
f32                D_L02_0016179C = 0;
s32                D_L02_001616E8 = 0;
s32                D_L02_001616F0 = 0;
s32                D_L02_001616F4 = 0;
float              D_L02_001617A0 = 0;
s32                D_L02_001617F4 = 0;
s32                D_L02_001617E8 = 0;
s32 D_L02_001D09D0 NOT_SDA = 0;
s32 D_L02_001D1B50 NOT_SDA = 0;
signed char        D_L02_00161733 = 0x40;
int                D_L02_001617B8 = 0;
int                D_L02_001617D8 = 0;
int D_L02_001D09F0 NOT_SDA = 0;
int D_L02_001D1B20 NOT_SDA = 0;
float              D_L02_001616C0 = 0.08333f;
int                D_L02_001616C4 = 6;
float              D_L02_001616C8 = 0.20071286f;
float              D_L02_001616CC = 0.39968038f;
float              D_L02_001616D4 = 0.3141592f;
float              D_L02_001616D8 = 0.05f;
float              D_L02_001616DC = 1.234f;
float              D_L02_001616FC = 0.01f;
float              D_L02_00161700 = 0.5f;
float              D_L02_00161704 = 0.2f;
float              D_L02_0016170C = 0.2617994f;
float              D_L02_00161710 = -0.25f;
float              D_L02_00161718 = 8.0f;
int                D_L02_0016171C = 0x3c;
int                D_L02_00161720 = 0x1e;
float              D_L02_00161728 = 1.5f;
float              D_L02_0016172C = 0.1f;
float              D_L02_00161738 = 2.13333f;
int                D_L02_001617A8[3] = {0};

float D_L02_001617C8[3] = {0};

Vec_2BE3A8 D_L02_001D0AA0[12] = {0};

Vec_2BE3A8 D_L02_001D0B60[12] = {0};

Tab D_L02_001FA3B0 = {
    {0x7f207f7f, 0x7f7f2020, 0x7f20207f, 0x7f7f207f, 0x7f7f7f20}
};

int                D_L02_00161768 = 0x60007f00;
int                D_L02_00161750 = 0x7f7f7f;
int                D_L02_00161774 = 0xe;
int                D_L02_00161778 = 0x10;
int                D_L02_00161730 = 0x407f207f;
int                D_L02_00161734 = 0x60007f00;
float              D_L02_00161784 = -0.5f;
float              D_L02_0016177C = 0.4f;
float              D_L02_00161780 = 0.1f;
int                D_L02_00161788 = 0x7f207f7f;
float              D_L03_001616C0 = 3.0f;
int                D_L03_001616CC = 0;
int                D_L03_0016179C = 0;
int                D_L03_001617A8 = 0;
f32                D_L03_001617AC = 0;
s32                D_L03_001616F8 = 0;
s32                D_L03_00161700 = 0;
s32                D_L03_00161704 = 0;
float              D_L03_001617B0 = 0;
s32                D_L03_00161804 = 0;
s32                D_L03_001617F8 = 0;
s32 D_L03_001D04D0 NOT_SDA = 0;
s32 D_L03_001D1650 NOT_SDA = 0;
signed char        D_L03_00161743 = 0x40;
int                D_L03_001617C8 = 0;
int                D_L03_001617E8 = 0;
int D_L03_001D04F0 NOT_SDA = 0;
int D_L03_001D1620 NOT_SDA = 0;
float              D_L03_001616D0 = 0.08333f;
int                D_L03_001616D4 = 6;
float              D_L03_001616D8 = 0.20071286f;
float              D_L03_001616DC = 0.39968038f;
float              D_L03_001616E4 = 0.3141592f;
float              D_L03_001616E8 = 0.05f;
float              D_L03_001616EC = 1.234f;
float              D_L03_0016170C = 0.01f;
float              D_L03_00161710 = 0.5f;
float              D_L03_00161714 = 0.2f;
float              D_L03_0016171C = 0.2617994f;
float              D_L03_00161720 = -0.25f;
float              D_L03_00161728 = 8.0f;
int                D_L03_0016172C = 0x3c;
int                D_L03_00161730 = 0x1e;
float              D_L03_00161738 = 1.5f;
float              D_L03_0016173C = 0.1f;
float              D_L03_00161748 = 2.13333f;
int                D_L03_001617B8[3] = {0};

float D_L03_001617D8[3] = {0};

Vec_2BE3A8 D_L03_001D05A0[12] = {0};

Vec_2BE3A8 D_L03_001D0660[12] = {0};

Tab D_L03_001E2C40 = {
    {0x7f207f7f, 0x7f7f2020, 0x7f20207f, 0x7f7f207f, 0x7f7f7f20}
};

int                D_L03_00161778 = 0x60007f00;
int                D_L03_00161760 = 0x7f7f7f;
int                D_L03_00161784 = 0xe;
int                D_L03_00161788 = 0x10;
int                D_L03_00161740 = 0x407f207f;
int                D_L03_00161744 = 0x60007f00;
float              D_L03_00161794 = -0.5f;
float              D_L03_0016178C = 0.4f;
float              D_L03_00161790 = 0.1f;
int                D_L03_00161798 = 0x7f207f7f;
float              D_L04_001616B0 = 3.0f;
int                D_L04_001616BC = 0;
int                D_L04_0016178C = 0;
int                D_L04_00161798 = 0;
f32                D_L04_0016179C = 0;
s32                D_L04_001616E8 = 0;
s32                D_L04_001616F0 = 0;
s32                D_L04_001616F4 = 0;
float              D_L04_001617A0 = 0;
s32                D_L04_001617F4 = 0;
s32                D_L04_001617E8 = 0;
s32 D_L04_001D0550 NOT_SDA = 0;
s32 D_L04_001D16D0 NOT_SDA = 0;
signed char        D_L04_00161733 = 0x40;
int                D_L04_001617B8 = 0;
int                D_L04_001617D8 = 0;
int D_L04_001D0570 NOT_SDA = 0;
int D_L04_001D16A0 NOT_SDA = 0;
float              D_L04_001616C0 = 0.08333f;
int                D_L04_001616C4 = 6;
float              D_L04_001616C8 = 0.20071286f;
float              D_L04_001616CC = 0.39968038f;
float              D_L04_001616D4 = 0.3141592f;
float              D_L04_001616D8 = 0.05f;
float              D_L04_001616DC = 1.234f;
float              D_L04_001616FC = 0.01f;
float              D_L04_00161700 = 0.5f;
float              D_L04_00161704 = 0.2f;
float              D_L04_0016170C = 0.2617994f;
float              D_L04_00161710 = -0.25f;
float              D_L04_00161718 = 8.0f;
int                D_L04_0016171C = 0x3c;
int                D_L04_00161720 = 0x1e;
float              D_L04_00161728 = 1.5f;
float              D_L04_0016172C = 0.1f;
float              D_L04_00161738 = 2.13333f;
int                D_L04_001617A8[3] = {0};

float D_L04_001617C8[3] = {0};

Vec_2BE3A8 D_L04_001D0620[12] = {0};

Vec_2BE3A8 D_L04_001D06E0[12] = {0};

Tab D_L04_001DB850 = {
    {0x7f207f7f, 0x7f7f2020, 0x7f20207f, 0x7f7f207f, 0x7f7f7f20}
};

int                D_L04_00161768 = 0x60007f00;
int                D_L04_00161750 = 0x7f7f7f;
int                D_L04_00161774 = 0xe;
int                D_L04_00161778 = 0x10;
int                D_L04_00161730 = 0x407f207f;
int                D_L04_00161734 = 0x60007f00;
float              D_L04_00161784 = -0.5f;
float              D_L04_0016177C = 0.4f;
float              D_L04_00161780 = 0.1f;
int                D_L04_00161788 = 0x7f207f7f;
float              D_L05_00161700 = 3.0f;
int                D_L05_0016170C = 0;
int                D_L05_001617DC = 0;
int                D_L05_001617E8 = 0;
f32                D_L05_001617EC = 0;
s32                D_L05_00161738 = 0;
s32                D_L05_00161740 = 0;
s32                D_L05_00161744 = 0;
float              D_L05_001617F0 = 0;
s32                D_L05_00161844 = 0;
s32                D_L05_00161838 = 0;
s32 D_L05_001D0870 NOT_SDA = 0;
s32 D_L05_001D19F0 NOT_SDA = 0;
signed char        D_L05_00161783 = 0x40;
int                D_L05_00161808 = 0;
int                D_L05_00161828 = 0;
int D_L05_001D0890 NOT_SDA = 0;
int D_L05_001D19C0 NOT_SDA = 0;
float              D_L05_00161710 = 0.08333f;
int                D_L05_00161714 = 6;
float              D_L05_00161718 = 0.20071286f;
float              D_L05_0016171C = 0.39968038f;
float              D_L05_00161724 = 0.3141592f;
float              D_L05_00161728 = 0.05f;
float              D_L05_0016172C = 1.234f;
float              D_L05_0016174C = 0.01f;
float              D_L05_00161750 = 0.5f;
float              D_L05_00161754 = 0.2f;
float              D_L05_0016175C = 0.2617994f;
float              D_L05_00161760 = -0.25f;
float              D_L05_00161768 = 8.0f;
int                D_L05_0016176C = 0x3c;
int                D_L05_00161770 = 0x1e;
float              D_L05_00161778 = 1.5f;
float              D_L05_0016177C = 0.1f;
float              D_L05_00161788 = 2.13333f;
int                D_L05_001617F8[3] = {0};

float D_L05_00161818[3] = {0};

Vec_2BE3A8 D_L05_001D0940[12] = {0};

Vec_2BE3A8 D_L05_001D0A00[12] = {0};

Tab D_L05_002181C0 = {
    {0x7f207f7f, 0x7f7f2020, 0x7f20207f, 0x7f7f207f, 0x7f7f7f20}
};

int                D_L05_001617B8 = 0x60007f00;
int                D_L05_001617A0 = 0x7f7f7f;
int                D_L05_001617C4 = 0xe;
int                D_L05_001617C8 = 0x10;
int                D_L05_00161780 = 0x407f207f;
int                D_L05_00161784 = 0x60007f00;
float              D_L05_001617D4 = -0.5f;
float              D_L05_001617CC = 0.4f;
float              D_L05_001617D0 = 0.1f;
int                D_L05_001617D8 = 0x7f207f7f;
float              D_L06_001616E0 = 3.0f;
int                D_L06_001616EC = 0;
int                D_L06_001617BC = 0;
int                D_L06_001617C8 = 0;
f32                D_L06_001617CC = 0;
s32                D_L06_00161718 = 0;
s32                D_L06_00161720 = 0;
s32                D_L06_00161724 = 0;
float              D_L06_001617D0 = 0;
s32                D_L06_00161824 = 0;
s32                D_L06_00161818 = 0;
s32 D_L06_001D0B50 NOT_SDA = 0;
s32 D_L06_001D1CD0 NOT_SDA = 0;
signed char        D_L06_00161763 = 0x40;
int                D_L06_001617E8 = 0;
int                D_L06_00161808 = 0;
int D_L06_001D0B70 NOT_SDA = 0;
int D_L06_001D1CA0 NOT_SDA = 0;
float              D_L06_001616F0 = 0.08333f;
int                D_L06_001616F4 = 6;
float              D_L06_001616F8 = 0.20071286f;
float              D_L06_001616FC = 0.39968038f;
float              D_L06_00161704 = 0.3141592f;
float              D_L06_00161708 = 0.05f;
float              D_L06_0016170C = 1.234f;
float              D_L06_0016172C = 0.01f;
float              D_L06_00161730 = 0.5f;
float              D_L06_00161734 = 0.2f;
float              D_L06_0016173C = 0.2617994f;
float              D_L06_00161740 = -0.25f;
float              D_L06_00161748 = 8.0f;
int                D_L06_0016174C = 0x3c;
int                D_L06_00161750 = 0x1e;
float              D_L06_00161758 = 1.5f;
float              D_L06_0016175C = 0.1f;
float              D_L06_00161768 = 2.13333f;
int                D_L06_001617D8[3] = {0};

float D_L06_001617F8[3] = {0};

Vec_2BE3A8 D_L06_001D0C20[12] = {0};

Vec_2BE3A8 D_L06_001D0CE0[12] = {0};

Tab D_L06_00200EB0 = {
    {0x7f207f7f, 0x7f7f2020, 0x7f20207f, 0x7f7f207f, 0x7f7f7f20}
};

int                D_L06_00161798 = 0x60007f00;
int                D_L06_00161780 = 0x7f7f7f;
int                D_L06_001617A4 = 0xe;
int                D_L06_001617A8 = 0x10;
int                D_L06_00161760 = 0x407f207f;
int                D_L06_00161764 = 0x60007f00;
float              D_L06_001617B4 = -0.5f;
float              D_L06_001617AC = 0.4f;
float              D_L06_001617B0 = 0.1f;
int                D_L06_001617B8 = 0x7f207f7f;
float              D_L07_001616B0 = 3.0f;
int                D_L07_001616BC = 0;
int                D_L07_0016178C = 0;
int                D_L07_00161798 = 0;
f32                D_L07_0016179C = 0;
s32                D_L07_001616E8 = 0;
s32                D_L07_001616F0 = 0;
s32                D_L07_001616F4 = 0;
float              D_L07_001617A0 = 0;
s32                D_L07_001617F4 = 0;
s32                D_L07_001617E8 = 0;
s32 D_L07_001D0450 NOT_SDA = 0;
s32 D_L07_001D15D0 NOT_SDA = 0;
signed char        D_L07_00161733 = 0x40;
int                D_L07_001617B8 = 0;
int                D_L07_001617D8 = 0;
int D_L07_001D0470 NOT_SDA = 0;
int D_L07_001D15A0 NOT_SDA = 0;
float              D_L07_001616C0 = 0.08333f;
int                D_L07_001616C4 = 6;
float              D_L07_001616C8 = 0.20071286f;
float              D_L07_001616CC = 0.39968038f;
float              D_L07_001616D4 = 0.3141592f;
float              D_L07_001616D8 = 0.05f;
float              D_L07_001616DC = 1.234f;
float              D_L07_001616FC = 0.01f;
float              D_L07_00161700 = 0.5f;
float              D_L07_00161704 = 0.2f;
float              D_L07_0016170C = 0.2617994f;
float              D_L07_00161710 = -0.25f;
float              D_L07_00161718 = 8.0f;
int                D_L07_0016171C = 0x3c;
int                D_L07_00161720 = 0x1e;
float              D_L07_00161728 = 1.5f;
float              D_L07_0016172C = 0.1f;
float              D_L07_00161738 = 2.13333f;
int                D_L07_001617A8[3] = {0};

float D_L07_001617C8[3] = {0};

Vec_2BE3A8 D_L07_001D0520[12] = {0};

Vec_2BE3A8 D_L07_001D05E0[12] = {0};

Tab D_L07_00210D50 = {
    {0x7f207f7f, 0x7f7f2020, 0x7f20207f, 0x7f7f207f, 0x7f7f7f20}
};

int                D_L07_00161768 = 0x60007f00;
int                D_L07_00161750 = 0x7f7f7f;
int                D_L07_00161774 = 0xe;
int                D_L07_00161778 = 0x10;
int                D_L07_00161730 = 0x407f207f;
int                D_L07_00161734 = 0x60007f00;
float              D_L07_00161784 = -0.5f;
float              D_L07_0016177C = 0.4f;
float              D_L07_00161780 = 0.1f;
int                D_L07_00161788 = 0x7f207f7f;
float              D_L08_00161700 = 3.0f;
int                D_L08_0016170C = 0;
int                D_L08_001617DC = 0;
int                D_L08_001617E8 = 0;
f32                D_L08_001617EC = 0;
s32                D_L08_00161738 = 0;
s32                D_L08_00161740 = 0;
s32                D_L08_00161744 = 0;
float              D_L08_001617F0 = 0;
s32                D_L08_00161844 = 0;
s32                D_L08_00161838 = 0;
s32 D_L08_001D0BD0 NOT_SDA = 0;
s32 D_L08_001D1D50 NOT_SDA = 0;
signed char        D_L08_00161783 = 0x40;
int                D_L08_00161808 = 0;
int                D_L08_00161828 = 0;
int D_L08_001D0BF0 NOT_SDA = 0;
int D_L08_001D1D20 NOT_SDA = 0;
float              D_L08_00161710 = 0.08333f;
int                D_L08_00161714 = 6;
float              D_L08_00161718 = 0.20071286f;
float              D_L08_0016171C = 0.39968038f;
float              D_L08_00161724 = 0.3141592f;
float              D_L08_00161728 = 0.05f;
float              D_L08_0016172C = 1.234f;
float              D_L08_0016174C = 0.01f;
float              D_L08_00161750 = 0.5f;
float              D_L08_00161754 = 0.2f;
float              D_L08_0016175C = 0.2617994f;
float              D_L08_00161760 = -0.25f;
float              D_L08_00161768 = 8.0f;
int                D_L08_0016176C = 0x3c;
int                D_L08_00161770 = 0x1e;
float              D_L08_00161778 = 1.5f;
float              D_L08_0016177C = 0.1f;
float              D_L08_00161788 = 2.13333f;
int                D_L08_001617F8[3] = {0};

float D_L08_00161818[3] = {0};

Vec_2BE3A8 D_L08_001D0CA0[12] = {0};

Vec_2BE3A8 D_L08_001D0D60[12] = {0};

Tab D_L08_001FBA10 = {
    {0x7f207f7f, 0x7f7f2020, 0x7f20207f, 0x7f7f207f, 0x7f7f7f20}
};

int                D_L08_001617B8 = 0x60007f00;
int                D_L08_001617A0 = 0x7f7f7f;
int                D_L08_001617C4 = 0xe;
int                D_L08_001617C8 = 0x10;
int                D_L08_00161780 = 0x407f207f;
int                D_L08_00161784 = 0x60007f00;
float              D_L08_001617D4 = -0.5f;
float              D_L08_001617CC = 0.4f;
float              D_L08_001617D0 = 0.1f;
int                D_L08_001617D8 = 0x7f207f7f;
float              D_L09_00161770 = 3.0f;
int                D_L09_0016177C = 0;
int                D_L09_0016184C = 0;
int                D_L09_00161858 = 0;
f32                D_L09_0016185C = 0;
s32                D_L09_001617A8 = 0;
s32                D_L09_001617B0 = 0;
s32                D_L09_001617B4 = 0;
float              D_L09_00161860 = 0;
s32                D_L09_001618B4 = 0;
s32                D_L09_001618A8 = 0;
s32 D_L09_001D0550 NOT_SDA = 0;
s32 D_L09_001D16D0 NOT_SDA = 0;
signed char        D_L09_001617F3 = 0x40;
int                D_L09_00161878 = 0;
int                D_L09_00161898 = 0;
int D_L09_001D0570 NOT_SDA = 0;
int D_L09_001D16A0 NOT_SDA = 0;
float              D_L09_00161780 = 0.08333f;
int                D_L09_00161784 = 6;
float              D_L09_00161788 = 0.20071286f;
float              D_L09_0016178C = 0.39968038f;
float              D_L09_00161794 = 0.3141592f;
float              D_L09_00161798 = 0.05f;
float              D_L09_0016179C = 1.234f;
float              D_L09_001617BC = 0.01f;
float              D_L09_001617C0 = 0.5f;
float              D_L09_001617C4 = 0.2f;
float              D_L09_001617CC = 0.2617994f;
float              D_L09_001617D0 = -0.25f;
float              D_L09_001617D8 = 8.0f;
int                D_L09_001617DC = 0x3c;
int                D_L09_001617E0 = 0x1e;
float              D_L09_001617E8 = 1.5f;
float              D_L09_001617EC = 0.1f;
float              D_L09_001617F8 = 2.13333f;
int                D_L09_00161868[3] = {0};

float D_L09_00161888[3] = {0};

Vec_2BE3A8 D_L09_001D0620[12] = {0};

Vec_2BE3A8 D_L09_001D06E0[12] = {0};

Tab D_L09_002086A0 = {
    {0x7f207f7f, 0x7f7f2020, 0x7f20207f, 0x7f7f207f, 0x7f7f7f20}
};

int                D_L09_00161828 = 0x60007f00;
int                D_L09_00161810 = 0x7f7f7f;
int                D_L09_00161834 = 0xe;
int                D_L09_00161838 = 0x10;
int                D_L09_001617F0 = 0x407f207f;
int                D_L09_001617F4 = 0x60007f00;
float              D_L09_00161844 = -0.5f;
float              D_L09_0016183C = 0.4f;
float              D_L09_00161840 = 0.1f;
int                D_L09_00161848 = 0x7f207f7f;
float              D_L10_001616C0 = 3.0f;
int                D_L10_001616CC = 0;
int                D_L10_0016179C = 0;
int                D_L10_001617A8 = 0;
f32                D_L10_001617AC = 0;
s32                D_L10_001616F8 = 0;
s32                D_L10_00161700 = 0;
s32                D_L10_00161704 = 0;
float              D_L10_001617B0 = 0;
s32                D_L10_00161804 = 0;
s32                D_L10_001617F8 = 0;
s32 D_L10_001D0850 NOT_SDA = 0;
s32 D_L10_001D19D0 NOT_SDA = 0;
signed char        D_L10_00161743 = 0x40;
int                D_L10_001617C8 = 0;
int                D_L10_001617E8 = 0;
int D_L10_001D0870 NOT_SDA = 0;
int D_L10_001D19A0 NOT_SDA = 0;
float              D_L10_001616D0 = 0.08333f;
int                D_L10_001616D4 = 6;
float              D_L10_001616D8 = 0.20071286f;
float              D_L10_001616DC = 0.39968038f;
float              D_L10_001616E4 = 0.3141592f;
float              D_L10_001616E8 = 0.05f;
float              D_L10_001616EC = 1.234f;
float              D_L10_0016170C = 0.01f;
float              D_L10_00161710 = 0.5f;
float              D_L10_00161714 = 0.2f;
float              D_L10_0016171C = 0.2617994f;
float              D_L10_00161720 = -0.25f;
float              D_L10_00161728 = 8.0f;
int                D_L10_0016172C = 0x3c;
int                D_L10_00161730 = 0x1e;
float              D_L10_00161738 = 1.5f;
float              D_L10_0016173C = 0.1f;
float              D_L10_00161748 = 2.13333f;
int                D_L10_001617B8[3] = {0};

float D_L10_001617D8[3] = {0};

Vec_2BE3A8 D_L10_001D0920[12] = {0};

Vec_2BE3A8 D_L10_001D09E0[12] = {0};

Tab D_L10_001DC8A0 = {
    {0x7f207f7f, 0x7f7f2020, 0x7f20207f, 0x7f7f207f, 0x7f7f7f20}
};

int                D_L10_00161778 = 0x60007f00;
int                D_L10_00161760 = 0x7f7f7f;
int                D_L10_00161784 = 0xe;
int                D_L10_00161788 = 0x10;
int                D_L10_00161740 = 0x407f207f;
int                D_L10_00161744 = 0x60007f00;
float              D_L10_00161794 = -0.5f;
float              D_L10_0016178C = 0.4f;
float              D_L10_00161790 = 0.1f;
int                D_L10_00161798 = 0x7f207f7f;
float              D_L11_00161740 = 3.0f;
int                D_L11_0016174C = 0;
int                D_L11_0016181C = 0;
int                D_L11_00161828 = 0;
f32                D_L11_0016182C = 0;
s32                D_L11_00161778 = 0;
s32                D_L11_00161780 = 0;
s32                D_L11_00161784 = 0;
float              D_L11_00161830 = 0;
s32                D_L11_00161884 = 0;
s32                D_L11_00161878 = 0;
s32 D_L11_001D0DD0 NOT_SDA = 0;
s32 D_L11_001D1F50 NOT_SDA = 0;
signed char        D_L11_001617C3 = 0x40;
int                D_L11_00161848 = 0;
int                D_L11_00161868 = 0;
int D_L11_001D0DF0 NOT_SDA = 0;
int D_L11_001D1F20 NOT_SDA = 0;
float              D_L11_00161750 = 0.08333f;
int                D_L11_00161754 = 6;
float              D_L11_00161758 = 0.20071286f;
float              D_L11_0016175C = 0.39968038f;
float              D_L11_00161764 = 0.3141592f;
float              D_L11_00161768 = 0.05f;
float              D_L11_0016176C = 1.234f;
float              D_L11_0016178C = 0.01f;
float              D_L11_00161790 = 0.5f;
float              D_L11_00161794 = 0.2f;
float              D_L11_0016179C = 0.2617994f;
float              D_L11_001617A0 = -0.25f;
float              D_L11_001617A8 = 8.0f;
int                D_L11_001617AC = 0x3c;
int                D_L11_001617B0 = 0x1e;
float              D_L11_001617B8 = 1.5f;
float              D_L11_001617BC = 0.1f;
float              D_L11_001617C8 = 2.13333f;
int                D_L11_00161838[3] = {0};

float D_L11_00161858[3] = {0};

Vec_2BE3A8 D_L11_001D0EA0[12] = {0};

Vec_2BE3A8 D_L11_001D0F60[12] = {0};

Tab D_L11_0021AD30 = {
    {0x7f207f7f, 0x7f7f2020, 0x7f20207f, 0x7f7f207f, 0x7f7f7f20}
};

int                D_L11_001617F8 = 0x60007f00;
int                D_L11_001617E0 = 0x7f7f7f;
int                D_L11_00161804 = 0xe;
int                D_L11_00161808 = 0x10;
int                D_L11_001617C0 = 0x407f207f;
int                D_L11_001617C4 = 0x60007f00;
float              D_L11_00161814 = -0.5f;
float              D_L11_0016180C = 0.4f;
float              D_L11_00161810 = 0.1f;
int                D_L11_00161818 = 0x7f207f7f;
float              D_L12_001616C0 = 3.0f;
int                D_L12_001616CC = 0;
int                D_L12_0016179C = 0;
int                D_L12_001617A8 = 0;
f32                D_L12_001617AC = 0;
s32                D_L12_001616F8 = 0;
s32                D_L12_00161700 = 0;
s32                D_L12_00161704 = 0;
float              D_L12_001617B0 = 0;
s32                D_L12_00161804 = 0;
s32                D_L12_001617F8 = 0;
s32 D_L12_001E3470 NOT_SDA = 0;
s32 D_L12_001E45F0 NOT_SDA = 0;
signed char        D_L12_00161743 = 0x40;
int                D_L12_001617C8 = 0;
int                D_L12_001617E8 = 0;
int D_L12_001E3490 NOT_SDA = 0;
int D_L12_001E45C0 NOT_SDA = 0;
float              D_L12_001616D0 = 0.08333f;
int                D_L12_001616D4 = 6;
float              D_L12_001616D8 = 0.20071286f;
float              D_L12_001616DC = 0.39968038f;
float              D_L12_001616E4 = 0.3141592f;
float              D_L12_001616E8 = 0.05f;
float              D_L12_001616EC = 1.234f;
float              D_L12_0016170C = 0.01f;
float              D_L12_00161710 = 0.5f;
float              D_L12_00161714 = 0.2f;
float              D_L12_0016171C = 0.2617994f;
float              D_L12_00161720 = -0.25f;
float              D_L12_00161728 = 8.0f;
int                D_L12_0016172C = 0x3c;
int                D_L12_00161730 = 0x1e;
float              D_L12_00161738 = 1.5f;
float              D_L12_0016173C = 0.1f;
float              D_L12_00161748 = 2.13333f;
int                D_L12_001617B8[3] = {0};

float D_L12_001617D8[3] = {0};

Vec_2BE3A8 D_L12_001E3540[12] = {0};

Vec_2BE3A8 D_L12_001E3600[12] = {0};

Tab D_L12_0020C000 = {
    {0x7f207f7f, 0x7f7f2020, 0x7f20207f, 0x7f7f207f, 0x7f7f7f20}
};

int                D_L12_00161778 = 0x60007f00;
int                D_L12_00161760 = 0x7f7f7f;
int                D_L12_00161784 = 0xe;
int                D_L12_00161788 = 0x10;
int                D_L12_00161740 = 0x407f207f;
int                D_L12_00161744 = 0x60007f00;
float              D_L12_00161794 = -0.5f;
float              D_L12_0016178C = 0.4f;
float              D_L12_00161790 = 0.1f;
int                D_L12_00161798 = 0x7f207f7f;
float              D_L13_00161790 = 3.0f;
int                D_L13_0016179C = 0;
int                D_L13_0016186C = 0;
int                D_L13_00161878 = 0;
f32                D_L13_0016187C = 0;
s32                D_L13_001617C8 = 0;
s32                D_L13_001617D0 = 0;
s32                D_L13_001617D4 = 0;
float              D_L13_00161880 = 0;
s32                D_L13_001618D4 = 0;
s32                D_L13_001618C8 = 0;
s32 D_L13_001D0CE0 NOT_SDA = 0;
s32 D_L13_001D1E60 NOT_SDA = 0;
signed char        D_L13_00161813 = 0x40;
int                D_L13_00161898 = 0;
int                D_L13_001618B8 = 0;
int D_L13_001D0D00 NOT_SDA = 0;
int D_L13_001D1E30 NOT_SDA = 0;
float              D_L13_001617A0 = 0.08333f;
int                D_L13_001617A4 = 6;
float              D_L13_001617A8 = 0.20071286f;
float              D_L13_001617AC = 0.39968038f;
float              D_L13_001617B4 = 0.3141592f;
float              D_L13_001617B8 = 0.05f;
float              D_L13_001617BC = 1.234f;
float              D_L13_001617DC = 0.01f;
float              D_L13_001617E0 = 0.5f;
float              D_L13_001617E4 = 0.2f;
float              D_L13_001617EC = 0.2617994f;
float              D_L13_001617F0 = -0.25f;
float              D_L13_001617F8 = 8.0f;
int                D_L13_001617FC = 0x3c;
int                D_L13_00161800 = 0x1e;
float              D_L13_00161808 = 1.5f;
float              D_L13_0016180C = 0.1f;
float              D_L13_00161818 = 2.13333f;
int                D_L13_00161888[3] = {0};

float D_L13_001618A8[3] = {0};

Vec_2BE3A8 D_L13_001D0DB0[12] = {0};

Vec_2BE3A8 D_L13_001D0E70[12] = {0};

Tab D_L13_001F4A50 = {
    {0x7f207f7f, 0x7f7f2020, 0x7f20207f, 0x7f7f207f, 0x7f7f7f20}
};

int                D_L13_00161848 = 0x60007f00;
int                D_L13_00161830 = 0x7f7f7f;
int                D_L13_00161854 = 0xe;
int                D_L13_00161858 = 0x10;
int                D_L13_00161810 = 0x407f207f;
int                D_L13_00161814 = 0x60007f00;
float              D_L13_00161864 = -0.5f;
float              D_L13_0016185C = 0.4f;
float              D_L13_00161860 = 0.1f;
int                D_L13_00161868 = 0x7f207f7f;
float              D_L14_001618D0 = 3.0f;
int                D_L14_001618DC = 0;
int                D_L14_001619AC = 0;
int                D_L14_001619B8 = 0;
f32                D_L14_001619BC = 0;
s32                D_L14_00161908 = 0;
s32                D_L14_00161910 = 0;
s32                D_L14_00161914 = 0;
float              D_L14_001619C0 = 0;
s32                D_L14_00161A14 = 0;
s32                D_L14_00161A08 = 0;
s32 D_L14_001DD390 NOT_SDA = 0;
s32 D_L14_001DE510 NOT_SDA = 0;
signed char        D_L14_00161953 = 0x40;
int                D_L14_001619D8 = 0;
int                D_L14_001619F8 = 0;
int D_L14_001DD3B0 NOT_SDA = 0;
int D_L14_001DE4E0 NOT_SDA = 0;
float              D_L14_001618E0 = 0.08333f;
int                D_L14_001618E4 = 6;
float              D_L14_001618E8 = 0.20071286f;
float              D_L14_001618EC = 0.39968038f;
float              D_L14_001618F4 = 0.3141592f;
float              D_L14_001618F8 = 0.05f;
float              D_L14_001618FC = 1.234f;
float              D_L14_0016191C = 0.01f;
float              D_L14_00161920 = 0.5f;
float              D_L14_00161924 = 0.2f;
float              D_L14_0016192C = 0.2617994f;
float              D_L14_00161930 = -0.25f;
float              D_L14_00161938 = 8.0f;
int                D_L14_0016193C = 0x3c;
int                D_L14_00161940 = 0x1e;
float              D_L14_00161948 = 1.5f;
float              D_L14_0016194C = 0.1f;
float              D_L14_00161958 = 2.13333f;
int                D_L14_001619C8[3] = {0};

float D_L14_001619E8[3] = {0};

Vec_2BE3A8 D_L14_001DD460[12] = {0};

Vec_2BE3A8 D_L14_001DD520[12] = {0};

Tab D_L14_001FC2E0 = {
    {0x7f207f7f, 0x7f7f2020, 0x7f20207f, 0x7f7f207f, 0x7f7f7f20}
};

int                D_L14_00161988 = 0x60007f00;
int                D_L14_00161970 = 0x7f7f7f;
int                D_L14_00161994 = 0xe;
int                D_L14_00161998 = 0x10;
int                D_L14_00161950 = 0x407f207f;
int                D_L14_00161954 = 0x60007f00;
float              D_L14_001619A4 = -0.5f;
float              D_L14_0016199C = 0.4f;
float              D_L14_001619A0 = 0.1f;
int                D_L14_001619A8 = 0x7f207f7f;
float              D_L15_00161870 = 3.0f;
int                D_L15_0016187C = 0;
int                D_L15_0016194C = 0;
int                D_L15_00161958 = 0;
f32                D_L15_0016195C = 0;
s32                D_L15_001618A8 = 0;
s32                D_L15_001618B0 = 0;
s32                D_L15_001618B4 = 0;
float              D_L15_00161960 = 0;
s32                D_L15_001619B4 = 0;
s32                D_L15_001619A8 = 0;
s32 D_L15_001D0E50 NOT_SDA = 0;
s32 D_L15_001D1FD0 NOT_SDA = 0;
signed char        D_L15_001618F3 = 0x40;
int                D_L15_00161978 = 0;
int                D_L15_00161998 = 0;
int D_L15_001D0E70 NOT_SDA = 0;
int D_L15_001D1FA0 NOT_SDA = 0;
float              D_L15_00161880 = 0.08333f;
int                D_L15_00161884 = 6;
float              D_L15_00161888 = 0.20071286f;
float              D_L15_0016188C = 0.39968038f;
float              D_L15_00161894 = 0.3141592f;
float              D_L15_00161898 = 0.05f;
float              D_L15_0016189C = 1.234f;
float              D_L15_001618BC = 0.01f;
float              D_L15_001618C0 = 0.5f;
float              D_L15_001618C4 = 0.2f;
float              D_L15_001618CC = 0.2617994f;
float              D_L15_001618D0 = -0.25f;
float              D_L15_001618D8 = 8.0f;
int                D_L15_001618DC = 0x3c;
int                D_L15_001618E0 = 0x1e;
float              D_L15_001618E8 = 1.5f;
float              D_L15_001618EC = 0.1f;
float              D_L15_001618F8 = 2.13333f;
int                D_L15_00161968[3] = {0};

float D_L15_00161988[3] = {0};

Vec_2BE3A8 D_L15_001D0F20[12] = {0};

Vec_2BE3A8 D_L15_001D0FE0[12] = {0};

Tab D_L15_001E2970 = {
    {0x7f207f7f, 0x7f7f2020, 0x7f20207f, 0x7f7f207f, 0x7f7f7f20}
};

int                D_L15_00161928 = 0x60007f00;
int                D_L15_00161910 = 0x7f7f7f;
int                D_L15_00161934 = 0xe;
int                D_L15_00161938 = 0x10;
int                D_L15_001618F0 = 0x407f207f;
int                D_L15_001618F4 = 0x60007f00;
float              D_L15_00161944 = -0.5f;
float              D_L15_0016193C = 0.4f;
float              D_L15_00161940 = 0.1f;
int                D_L15_00161948 = 0x7f207f7f;
float              D_L16_00161770 = 3.0f;
int                D_L16_0016177C = 0;
int                D_L16_0016184C = 0;
int                D_L16_00161858 = 0;
f32                D_L16_0016185C = 0;
s32                D_L16_001617A8 = 0;
s32                D_L16_001617B0 = 0;
s32                D_L16_001617B4 = 0;
float              D_L16_00161860 = 0;
s32                D_L16_001618B4 = 0;
s32                D_L16_001618A8 = 0;
s32 D_L16_001D0850 NOT_SDA = 0;
s32 D_L16_001D19D0 NOT_SDA = 0;
signed char        D_L16_001617F3 = 0x40;
int                D_L16_00161878 = 0;
int                D_L16_00161898 = 0;
int D_L16_001D0870 NOT_SDA = 0;
int D_L16_001D19A0 NOT_SDA = 0;
float              D_L16_00161780 = 0.08333f;
int                D_L16_00161784 = 6;
float              D_L16_00161788 = 0.20071286f;
float              D_L16_0016178C = 0.39968038f;
float              D_L16_00161794 = 0.3141592f;
float              D_L16_00161798 = 0.05f;
float              D_L16_0016179C = 1.234f;
float              D_L16_001617BC = 0.01f;
float              D_L16_001617C0 = 0.5f;
float              D_L16_001617C4 = 0.2f;
float              D_L16_001617CC = 0.2617994f;
float              D_L16_001617D0 = -0.25f;
float              D_L16_001617D8 = 8.0f;
int                D_L16_001617DC = 0x3c;
int                D_L16_001617E0 = 0x1e;
float              D_L16_001617E8 = 1.5f;
float              D_L16_001617EC = 0.1f;
float              D_L16_001617F8 = 2.13333f;
int                D_L16_00161868[3] = {0};

float D_L16_00161888[3] = {0};

Vec_2BE3A8 D_L16_001D0920[12] = {0};

Vec_2BE3A8 D_L16_001D09E0[12] = {0};

Tab D_L16_001E7E00 = {
    {0x7f207f7f, 0x7f7f2020, 0x7f20207f, 0x7f7f207f, 0x7f7f7f20}
};

int                D_L16_00161828 = 0x60007f00;
int                D_L16_00161810 = 0x7f7f7f;
int                D_L16_00161834 = 0xe;
int                D_L16_00161838 = 0x10;
int                D_L16_001617F0 = 0x407f207f;
int                D_L16_001617F4 = 0x60007f00;
float              D_L16_00161844 = -0.5f;
float              D_L16_0016183C = 0.4f;
float              D_L16_00161840 = 0.1f;
int                D_L16_00161848 = 0x7f207f7f;
float              D_L17_001617C0 = 3.0f;
int                D_L17_001617CC = 0;
int                D_L17_0016189C = 0;
int                D_L17_001618A8 = 0;
f32                D_L17_001618AC = 0;
s32                D_L17_001617F8 = 0;
s32                D_L17_00161800 = 0;
s32                D_L17_00161804 = 0;
float              D_L17_001618B0 = 0;
s32                D_L17_00161904 = 0;
s32                D_L17_001618F8 = 0;
s32 D_L17_001D10D0 NOT_SDA = 0;
s32 D_L17_001D2250 NOT_SDA = 0;
signed char        D_L17_00161843 = 0x40;
int                D_L17_001618C8 = 0;
int                D_L17_001618E8 = 0;
int D_L17_001D10F0 NOT_SDA = 0;
int D_L17_001D2220 NOT_SDA = 0;
float              D_L17_001617D0 = 0.08333f;
int                D_L17_001617D4 = 6;
float              D_L17_001617D8 = 0.20071286f;
float              D_L17_001617DC = 0.39968038f;
float              D_L17_001617E4 = 0.3141592f;
float              D_L17_001617E8 = 0.05f;
float              D_L17_001617EC = 1.234f;
float              D_L17_0016180C = 0.01f;
float              D_L17_00161810 = 0.5f;
float              D_L17_00161814 = 0.2f;
float              D_L17_0016181C = 0.2617994f;
float              D_L17_00161820 = -0.25f;
float              D_L17_00161828 = 8.0f;
int                D_L17_0016182C = 0x3c;
int                D_L17_00161830 = 0x1e;
float              D_L17_00161838 = 1.5f;
float              D_L17_0016183C = 0.1f;
float              D_L17_00161848 = 2.13333f;
int                D_L17_001618B8[3] = {0};

float D_L17_001618D8[3] = {0};

Vec_2BE3A8 D_L17_001D11A0[12] = {0};

Vec_2BE3A8 D_L17_001D1260[12] = {0};

Tab D_L17_001E6540 = {
    {0x7f207f7f, 0x7f7f2020, 0x7f20207f, 0x7f7f207f, 0x7f7f7f20}
};

int                D_L17_00161878 = 0x60007f00;
int                D_L17_00161860 = 0x7f7f7f;
int                D_L17_00161884 = 0xe;
int                D_L17_00161888 = 0x10;
int                D_L17_00161840 = 0x407f207f;
int                D_L17_00161844 = 0x60007f00;
float              D_L17_00161894 = -0.5f;
float              D_L17_0016188C = 0.4f;
float              D_L17_00161890 = 0.1f;
int                D_L17_00161898 = 0x7f207f7f;
float              D_L18_00161750 = 3.0f;
int                D_L18_0016175C = 0;
int                D_L18_0016182C = 0;
int                D_L18_00161838 = 0;
f32                D_L18_0016183C = 0;
s32                D_L18_00161788 = 0;
s32                D_L18_00161790 = 0;
s32                D_L18_00161794 = 0;
float              D_L18_00161840 = 0;
s32                D_L18_00161894 = 0;
s32                D_L18_00161888 = 0;
s32 D_L18_001D0DD0 NOT_SDA = 0;
s32 D_L18_001D1F50 NOT_SDA = 0;
signed char        D_L18_001617D3 = 0x40;
int                D_L18_00161858 = 0;
int                D_L18_00161878 = 0;
int D_L18_001D0DF0 NOT_SDA = 0;
int D_L18_001D1F20 NOT_SDA = 0;
float              D_L18_00161760 = 0.08333f;
int                D_L18_00161764 = 6;
float              D_L18_00161768 = 0.20071286f;
float              D_L18_0016176C = 0.39968038f;
float              D_L18_00161774 = 0.3141592f;
float              D_L18_00161778 = 0.05f;
float              D_L18_0016177C = 1.234f;
float              D_L18_0016179C = 0.01f;
float              D_L18_001617A0 = 0.5f;
float              D_L18_001617A4 = 0.2f;
float              D_L18_001617AC = 0.2617994f;
float              D_L18_001617B0 = -0.25f;
float              D_L18_001617B8 = 8.0f;
int                D_L18_001617BC = 0x3c;
int                D_L18_001617C0 = 0x1e;
float              D_L18_001617C8 = 1.5f;
float              D_L18_001617CC = 0.1f;
float              D_L18_001617D8 = 2.13333f;
int                D_L18_00161848[3] = {0};

float D_L18_00161868[3] = {0};

Vec_2BE3A8 D_L18_001D0EA0[12] = {0};

Vec_2BE3A8 D_L18_001D0F60[12] = {0};

Tab D_L18_001F21E0 = {
    {0x7f207f7f, 0x7f7f2020, 0x7f20207f, 0x7f7f207f, 0x7f7f7f20}
};

int                D_L18_00161808 = 0x60007f00;
int                D_L18_001617F0 = 0x7f7f7f;
int                D_L18_00161814 = 0xe;
int                D_L18_00161818 = 0x10;
int                D_L18_001617D0 = 0x407f207f;
int                D_L18_001617D4 = 0x60007f00;
float              D_L18_00161824 = -0.5f;
float              D_L18_0016181C = 0.4f;
float              D_L18_00161820 = 0.1f;
int                D_L18_00161828 = 0x7f207f7f;
int D_L00_001DBF70 NOT_SDA = 0;

/* Ported from rac1-decomp (src/overlays/shared/vendor_002BA7C8.c: func_L00_002BD6B8), where it is exact; names translated to the US level program. */
char *select_morphoray_target_and_adjust_aim(char  *self,
                                             float *position,
                                             float *heading,
                                             float *pitch) __asm__("FUN_L00_002bc3b0");

char *

select_morphoray_target_and_adjust_aim(char *self, float *position, float *heading, float *pitch) {
    float        start[4] __attribute__((aligned(16)));
    float        end[4] __attribute__((aligned(16)));
    float        target[4] __attribute__((aligned(16)));
    float        offset[4] __attribute__((aligned(16)));
    char        *candidate;
    char        *data;
    struct Moby *moby;
    int          collision;
    int          a0s[1];
    int          selected_target[1];
    float        distance, angle, elevation, headingError, limit, apparentAngle, radius, reduction;
    int          i;
    char        *hero_base;
    char        *hero_moby;

    limit = D_L00_001616DC;
    a0s[0] = (int)self;
    *(VecQuad *)start = *(VecQuad *)position;
    hero_base = ((char *)&hero);
    hero_moby = *(char **)(hero_base + 0x2080);
    *(VecQuad *)end = *(VecQuad *)&MOBY(hero_moby)->pos;
    position = start;
    end[2] = position[2];
    collision = collision_line((Vec4f *)end, (Vec4f *)position, 6, hero_moby, 0);
    selected_target[0] = 0;
    if (collision) {
        qcopy(position, &collision_hit_point);
        return 0;
    }
    i = 0;
    if ((candidate = active_mobies[i]) != 0)
        do {
            if (*(signed char *)(candidate + 0x20) < 0 || *(short *)(candidate + 0xA6) == 0x58E ||
                *(short *)(candidate + 0xA6) == 0x452)
                continue;
            qcopy(target, candidate + 0x10);
            data = (char *)get_moby_target_info(candidate);
            if (!data)
                continue;
            target[2] += *(float *)(data + 0x10);
            if (!candidate || !*(char **)(candidate + 0x24) ||
                *(short *)(*(char **)(candidate + 0x24) + 0x46) != 5)
                continue;
            angle = fast_arc_tan(target[0] - position[0], target[1] - position[1]);
            distance = vector_distance(position, target);
            elevation = fast_arc_tan(distance, target[2] - position[2]);
            headingError = fast_difference_between_rotations(*heading, angle);
            distance = fast_vec_dist(position, target);
            if (distance < 2.5f) {
                float toward;
                char *close_range_hero_base = ((char *)&hero);
                moby = *(char **)(close_range_hero_base + 0x2080);
                toward = fast_arc_tan(
                    *(float *)(candidate + 0x10) - *(float *)(close_range_hero_base + 0x80),
                    *(float *)(candidate + 0x14) - *(float *)(close_range_hero_base + 0x84));
                if (fast_difference_between_rotations(moby->rot.z, toward) < 1.0471976f &&
                    absolute_float(position[2] - target[2]) < 2.0f &&
                    !collision_line((Vec4f *)position,
                                    (Vec4f *)target,
                                    6,
                                    *(char **)(close_range_hero_base + 0x2080),
                                    0)) {
                    selected_target[0] = (int)candidate;
                    *pitch = -elevation;
                    *heading = angle;
                    goto done;
                }
            }
            if (distance > limit)
                continue;
            {
                char *aim_offset_hero_base = ((char *)&hero);
                build_spherical_offset(offset,
                                       distance,
                                       *heading,
                                       *pitch + *(float *)(aim_offset_hero_base + 0x2E4) * 0.5f);
            }
            fast_vec_add(offset, offset, position);
            apparentAngle = fast_arc_sin(fast_vec_dist(offset, target) / (distance + distance));
            apparentAngle += apparentAngle;
            if (morphoray_target_angle_threshold_degrees_gp * 0.017453292f < apparentAngle) {
                data = (char *)get_moby_target_info(candidate);
                if (data) {
                    radius = convert_integer_to_float(*(unsigned char *)(data + 10)) * 0.125f;
                    reduction = fast_arc_sin(radius / distance);
                    if (radius < distance) {
                        apparentAngle -= reduction;
                        if (apparentAngle < 0.0f)
                            apparentAngle = 0.0f;
                    }
                }
            }
            if (apparentAngle < morphoray_target_angle_threshold_degrees_gp * 0.017453292f) {
                int valid = 1;
                if (collision_line((Vec4f *)position, (Vec4f *)target, 6, self, 0)) {
                    if (!D_L00_00173E40[6] || (char *)D_L00_00173E40[6] != candidate)
                        valid = 0;
                }
                if (valid) {
                    *pitch = -elevation;
                    limit = distance;
                    if (headingError < 0.17453292f)
                        *heading = angle;
                    selected_target[0] = (int)candidate;
                }
            }
        } while ((candidate = active_mobies[++i]) != 0);
done:
    return (char *)selected_target[0];
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002BA7C8.c: func_L00_002BDAF0), where it is exact; names translated to the US level program. */
void morphoray_update(Moby *moby) __asm__("FUN_L00_002bc7e8");

void morphoray_update(Moby *moby) {
    Vq    direction;
    Vq    points[3];
    Vq    matrix[4];
    Ray  *data;
    int   busy;
    int   in_range;
    Moby *hit;
    int   now;

    busy = 0;
    if (((Hero *)((char *)&hero))->items[0].state == 3) {
        busy = 1;
    }
    data = (struct Ray *)moby->pvars;
    if (moby->state == 0) {
        moby->state = 1;
        morphoray_point_light_slot = -1;
        D_L00_001617C8 = -1;
        morphoray_light_fade_countdown = 0;
    }
    in_range = 1;
    if (moby->prev_seq == 0 && compute_interpolated_record_value(moby) < 6.0f) {
        in_range = 0;
    }
    func_001F9938_2BDAF0(&data->timer);
    if ((((unsigned int)((Hero *)((char *)&hero))->state.control_mode < 2 ||
          ((Hero *)((char *)&hero))->state.control_mode == 12 ||
          ((Hero *)((char *)&hero))->state.control_mode == 4 ||
          ((Hero *)((char *)&hero))->state.control_mode == 5)) &&
        (*(int *)pad_held & ((Hero *)((char *)&hero))->items[0].button_mask) != 0 && !busy &&
        in_range) {
        if (((Hero *)((char *)&hero))->item_anim_active == 0) {
            start_equipped_item_animation();
        }
        if (data->seen == 0) {
            data->seen = 1;
            if (((Stats *)(item_usage))->kills <= 0xFFFE) {
                ((Stats *)(item_usage))->kills++;
            }
            now = func_001F9850_2BDAF0(D_0015EEA4) / 600;
            if (((Stats *)(item_usage))->best < now) {
                ((Stats *)(item_usage))->best = func_001F9850_2BDAF0(D_0015EEA4) / 600;
            }
            ((Stats *)(item_usage))->levels =
                ((Stats *)(item_usage))->levels | (1 << current_level_index) | 0x80000000;
        }
        if (data->timer == 0) {
            data->timer = func_001F9850_2BDAF0(15);
        }
        moby_point_position(moby, 0, data);
        if (((Hero *)((char *)&hero))->state.current == 1) {
            hero_set_state(30, 1);
        }
        *(u128_2BDAF0 *)&direction = 0;
        if (((Hero *)((char *)&hero))->state.current == 30 &&
            (*(unsigned short *)((char *)((Hero *)((char *)&hero))->moby + 0x34) & 1)) {
            copy_matrix3x4(points, l00_camera.unk0);
            func_001FA4A0_2BDAF0(matrix, points);
            direction.z = 1.0f;
            func_001F9EE8_2BDAF0(&direction, &direction, matrix);
        } else if (((Hero *)((char *)&hero))->unk22C8 == 1) {
            func_001F9C30_2BDAF0(&direction, &moby->rot_row1, -1.0f);
        } else {
            direction.x = 1.0f;
            transform_vector_by_basis(
                &direction, &direction, ((Hero *)((char *)&hero))->basis[0].f);
        }
        data->yaw = fast_arc_tan(direction.x, direction.y);
        data->pitch = fast_arc_tan(func_001F9CE8_2BDAF0(&direction), direction.z);
        *(u128_2BDAF0 *)&points[0] = *(u128_2BDAF0 *)data->pos;
        hit = select_morphoray_target_and_adjust_aim(moby, &points[0], &data->yaw, &data->pitch);
        func_001F9908_2BDAF0(&data->delay);
        if (data->target != 0) {
            if (data->target->state == 0xFE) {
                data->target = 0;
                data->delay = 0;
            } else if (data->target->state == 0xFD) {
                data->target = 0;
                data->delay = 0;
            }
        }
        if (hit != 0 || data->delay != 0) {
            if (hit != 0) {
                float *target_health = get_moby_target_info(hit);
                if (hit != data->target && data->delay == 0) {
                    data->target = hit;
                    data->total = func_001FA888_2BDAF0(((short *)target_health)[2]);
                    data->left = target_health[0];
                    data->delay = func_001F9850_2BDAF0(30);
                    if (data->total <= 1.0f) {
                        data->left = 0.0f;
                    }
                    data->snd = queue_animation_update(4,
                                                       0x7533,
                                                       hud_init_particles,
                                                       hud_update_particles,
                                                       draw_uniform_size_hud_stars,
                                                       &D_L00_001616EC,
                                                       10000);
                }
            }
            {
                float level_bonus = func_001FA888_2BDAF0(D_0013E633_2BDAF0[2]) * 0.33f + 1.0f;
                float remaining =
                    data->left - morphoray_base_drain_rate_gp * D_0015ED6C * level_bonus;
                data->left = remaining;
                if (remaining < 0.0f) {
                    data->left = 0.0f;
                }
            }
            D_L00_001616EC = 10000 - func_001FA898_2BDAF0(data->left * 10000.0f / data->total);
        }
        if (data->target != 0 && data->left <= 0.0f) {
            if (current_level_index == 5 && data->target->oclass == 0x271 &&
                item_unlocked[8] == 0) {
                item_unlocked[8] = 1;
                allocate_voice_for_bank_entry(1, 0, 0);
                queue_dialogue_message(0x53D6, -1);
            }
            spawn_morphoray_chicken(data->target);
            data->target = 0;
        }
        func_L00_002BE3A8_2BDAF0(moby);
        if (morphoray_point_light_slot == -1) {
            morphoray_point_light_slot =
                allocate_packed_color_point_light_slot((float *)&moby->pos, 7.5f, 0.0f, 0x207F7F);
        }
        if (morphoray_point_light_slot >= 0) {
            morphoray_light_fade_countdown = func_001F9850_2BDAF0(20);
            func_L00_001FF4B0_2BDAF0(&points[0], &moby->rot_row1, -0.5f);
            func_001F9BD8_2BDAF0(&points[1], data, &points[0]);
            point_lights[morphoray_point_light_slot].position.f[0] = points[1].x;
            point_lights[morphoray_point_light_slot].position.f[1] = points[1].y;
            point_lights[morphoray_point_light_slot].position.f[2] = points[1].z;
            point_lights[morphoray_point_light_slot].position.f[3] = 7.5f;
        }
        if (voice_is_playing(moby, D_L00_001617C8) == 0) {
            D_L00_001617C8 = allocate_voice_for_target_entry(0, 4, moby);
        }
    } else {
        data->target = 0;
        data->seen = 0;
        data->delay = 0;
        if (data->snd != -1) {
            remove_hud_item(data->snd);
            data->snd = -1;
        }
        D_L00_001617BC = 1;
        if (morphoray_point_light_slot >= 0) {
            float brightness;
            func_L00_001FF4B0_2BDAF0(&direction, &moby->rot_row1, -0.5f);
            func_001F9BD8_2BDAF0(&points[0], data, &direction);
            point_lights[morphoray_point_light_slot].position.f[0] = points[0].x;
            point_lights[morphoray_point_light_slot].position.f[1] = points[0].y;
            point_lights[morphoray_point_light_slot].position.f[2] = points[0].z;
            point_lights[morphoray_point_light_slot].position.f[3] = 7.5f;
            brightness = func_001FA888_2BDAF0(morphoray_light_fade_countdown) * 0.05f;
            point_lights[morphoray_point_light_slot].color[0] = brightness;
            point_lights[morphoray_point_light_slot].color[1] = brightness;
            point_lights[morphoray_point_light_slot].color[2] = brightness * 0.25f;
            if (func_001F9908_2BDAF0(&morphoray_light_fade_countdown) != 0) {
                release_point_light_slot(morphoray_point_light_slot);
                morphoray_point_light_slot = -1;
            }
        }
        if (morphoray_point_light_slot >= 0 && morphoray_light_fade_countdown == 0) {
            release_point_light_slot(morphoray_point_light_slot);
            morphoray_point_light_slot = -1;
        }
        if (D_L00_001617C8 >= 0) {
            if (D_L00_001617C8 != -1 &&
                ((Voice *)((char *)&voice_pool + D_L00_001617C8 * 0x70))->owner == moby &&
                ((Voice *)((char *)&voice_pool + D_L00_001617C8 * 0x70))->on != 0) {
                release_voice_slot(D_L00_001617C8);
            }
            D_L00_001617C8 = -1;
        }
        cancel_hero_item_animation();
    }
    if (busy && morphoray_point_light_slot >= 0) {
        release_point_light_slot(morphoray_point_light_slot);
        morphoray_point_light_slot = -1;
    }
}

void reset_beam_chain_and_pulse_state(u8 *moby) __asm__("FUN_L00_002bcf98");

void reset_beam_chain_and_pulse_state(u8 *moby) {
    u128  step;
    u128 *source = *(u128 **)(moby + 0x78);
    s32   i;
    s32   j;
    f32   scale;

    if (D_L00_001617BC != 0) {
        D_L00_001617BC = 0;
        scale = D_L00_001616DC_gp / convert_integer_to_float(12);
        D_L00_001617CC_macro = scale;
        scale_vector_2bcf98(-scale, &step, moby + 0xD0);
        qcopy(&beam_nodes[0], source);
        for (i = 1; i < 12; i++) {
            fast_vec_add(&beam_nodes[i], (u128 *)beam_prev_nodes + i, &step);
        }
        D_L00_00161718_gp = 0;
        D_L00_00161720_gp = 0;
        D_L00_00161724_gp = 0;
        D_L00_001617D0 = 0.0f;
        D_L00_00161824 = 0;
        for (j = 0; j < 3; j++) {
            (&D_L00_00161818)[j] = 0;
            qcopy((u128 *)beam_pulse_directions + j, &step);
        }
    }
}

/* Tesla claw arc: bends the twelve-point beam toward the target, builds the two side rails and the ring of
 * twenty points around each node, queues the draw callback, sparks at the tip, and runs the three travelling
 * pulses along the beam. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002BA7C8.c: func_L00_002BE3A8), where it is exact; names translated to the US level program. */
void update_beam_ring_rail_and_pulse_effects(struct Moby *moby) __asm__("FUN_L00_002bd0a0");

void update_beam_ring_rail_and_pulse_effects(struct Moby *moby) {
    Vec_2BE3A8 aim;
    Vec_2BE3A8 segment;
    Vec_2BE3A8 wave_offset;
    Vec_2BE3A8 beam_end;
    Vec_2BE3A8 point;
    Vec_2BE3A8 beam_segment_direction;
    Vec_2BE3A8 side;
    Vars      *data;
    Info      *info;
    int        ring_index;
    int        slot;
    int        pulse;
    int        sign;
    int        node_index;
    int        color;
    float      beam_length;
    float      progress;
    float      factor;
    float      wave_width;
    float      spark_size;

    data = (Vars *)moby->pvars;
    wave_offset.w = 1.0f;
    wave_offset.x = 0.0f;
    func_L00_002BE2A0_2BE3A8();
    D_L00_001617D0 = normalize_angle(D_L00_001617D0 + D_L00_001616F8_gp);
    build_spherical_offset(&aim, D_L00_001617CC_macro, data->f48, data->f4C);
    if (data->target != 0 && (info = get_moby_target_info(data->target)) != 0) {
        qcopy(&beam_end, &data->target->pos);
        beam_end.z = beam_end.z + info->height;
    } else {
        build_spherical_offset(&beam_end, D_L00_001616DC_gp, data->f48, data->f4C);
        func_001F9BD8_2BE3A8(&beam_end, &beam_end, data);
    }
    beam_length = D_L00_001616DC_gp;
    if (data->target == 0) {
        if (func_L00_001EFFF0_2BE3A8(data, &beam_end, 20, *(void **)((char *)&D_001413D0), 0) !=
            0) {
            beam_length = vector_distance(&D_L00_00173F60_2BE3A8, data);
        }
    } else {
        beam_length = vector_distance(&data->target->pos, data);
    }
    {
        int i;

        D_L00_001617CC_macro = beam_length * D_L00_001616F0_gp;
        qcopy(&beam_nodes[0], data);
        qcopy(&beam_rail_a_points[0], data);
        qcopy(&beam_rail_b_points[0], data);
        for (i = 1; i < 12; i++) {
            float seg_length;

            func_001F9BF0_2BE3A8(&segment, &beam_nodes[i], &beam_nodes[i - 1]);
            progress = func_001FA888_2BE3A8(i) * D_L00_001616F0_gp;
            factor = progress * -0.49999997f + 0.9f;
            if (D_L00_001616F4_gp < i && data->target != 0) {
                func_001F9BF0_2BE3A8(&point, &data->target->pos, &beam_nodes[i]);
                func_L00_001FF4B0_2BE3A8(&point, &point, D_L00_001617CC_macro);
                func_001F9C08_2BE3A8(&aim, &aim, &point, 0.2f);
            }
            func_001F9C08_2BE3A8(&point, &segment, &aim, factor);
            seg_length = fast_vec_length(&point);
            if (seg_length == 0.0f) {
                qcopy(&beam_nodes[i], &beam_nodes[i - 1]);
            } else {
                func_001F9C30_2BE3A8(&point, &point, D_L00_001617CC_macro / seg_length);
                func_001F9BD8_2BE3A8(&beam_nodes[i], &beam_nodes[i - 1], &point);
            }
            func_001F9BF0_2BE3A8(&beam_segment_direction, &beam_nodes[i], &beam_nodes[i - 1]);
            func_001F9CA0_2BE3A8(&side, &beam_segment_direction, D_0013F5E0_r);
            func_L00_001FF4B0_2BE3A8(&side,
                                     &side,
                                     D_L00_00161708_gp +
                                         (D_L00_0016170C_gp - D_L00_00161708_gp) * progress);
            {
                Vec_2BE3A8 ring;
                Vec_2BE3A8 fwd;
                Vec_2BE3A8 out;
                Vec_2BE3A8 up;
                Vec_2BE3A8 node;

                for (ring_index = 0; ring_index < 20; ring_index++) {
                    build_look_at_matrix(&ring,
                                         &side,
                                         &beam_segment_direction,
                                         (float)ring_index * D_L00_00161704_gp);
                    func_001F9BD8_2BE3A8(
                        &D_L00_001DC1A0_2BE3A8[i][ring_index], &ring, &beam_nodes[i]);
                }
                qcopy(&node, &beam_nodes[i]);
                func_L00_001FF4B0_2BE3A8(&fwd, &beam_segment_direction, 1.0f);
                func_L00_001FF4B0_2BE3A8(&out, &side, 1.0f);
                func_001F9CA0_2BE3A8(&up, &out, &fwd);
                wave_width = D_L00_0016172C_gp;
                wave_width = wave_width + (D_L00_00161730_gp - wave_width) * progress;
                factor =
                    normalize_angle(D_L00_001617D0 + D_L00_001616FC_gp * func_001FA888_2BE3A8(i));
                wave_offset.y = wave_width * func_001F9FA8_2BE3A8(factor);
                wave_width = wave_width * func_001F9F90_2BE3A8(factor);
                wave_offset.z = wave_width;
                func_001F9EE8_2BE3A8(&beam_rail_a_points[i], &wave_offset, &fwd);
                wave_offset.z = -wave_offset.z;
                func_001F9EE8_2BE3A8(&beam_rail_b_points[i], &wave_offset, &fwd);
            }
        }
    }
    add_draw_callback(draw_beam_ring_and_rail_effects, moby);
    spark_size = random_float_between(0.1f, 2.0f);
    sign = random_integer_below(2);
    if (sign == 0) {
        sign = sign - 1;
    }
    if (random_integer_below(2) == 0) {
        qcopy(&point, &beam_rail_a_points[11]);
        func_L00_00272F00_2BE3A8(&point,
                                 func_001F9850_2BE3A8(30),
                                 0x7F7F2020,
                                 0,
                                 sign,
                                 &zero_vector_macro,
                                 spark_size * 0.1f,
                                 spark_size,
                                 0.003f);
        sign = -sign;
        func_L00_00272F00_2BE3A8(&point,
                                 func_001F9850_2BE3A8(30),
                                 0x7F7F7F7F,
                                 1,
                                 sign,
                                 &zero_vector_macro,
                                 spark_size * 0.07f,
                                 spark_size * 0.7f,
                                 0.003f);
    }
    if (random_integer_below(2) == 0) {
        qcopy(&point, &beam_rail_b_points[11]);
        func_L00_00272F00_2BE3A8(&point,
                                 func_001F9850_2BE3A8(30),
                                 0x7F20207F,
                                 0,
                                 sign,
                                 &zero_vector_macro,
                                 spark_size * 0.1f,
                                 spark_size,
                                 0.003f);
        sign = -sign;
        func_L00_00272F00_2BE3A8(&point,
                                 func_001F9850_2BE3A8(30),
                                 0x7F7F7F7F,
                                 1,
                                 sign,
                                 &zero_vector_macro,
                                 spark_size * 0.07f,
                                 spark_size * 0.7f,
                                 0.003f);
    }
    func_001F9BF0_2BE3A8(&beam_segment_direction, &beam_nodes[1], &beam_nodes[0]);
    func_L00_001FF4B0_2BE3A8(
        &beam_segment_direction, &beam_segment_direction, D_L00_00161734_gp * frame_scale);
    if (random_integer_below(2) == 0) {
        func_L00_001FF4B0_2BE3A8(&side, &moby->rot_row1, D_L00_00161740_gp);
        func_001F9BD8_2BE3A8(&point, &beam_nodes[0], &side);
        {
            Tab        colors;
            Vec_2BE3A8 out;
            Vec_2BE3A8 up;

            colors = l00_beam_origin_particle_color_palette;
            color = colors.c[random_integer_below(5)];
            wave_width = randf_sym(0.0f, D_L00_0016173C_gp);
            build_look_at_matrix(
                &out, &beam_segment_direction, D_0013F5E0_r, randf_sym(0.0f, D_L00_0016173C_gp));
            func_001F9CA0_2BE3A8(&up, &beam_segment_direction, D_0013F5E0_r);
            build_look_at_matrix(&out, &out, &up, wave_width);
            node_index =
                func_001FA898_2BE3A8(beam_length * 1.2f / (D_L00_00161734_gp * frame_scale));
            if (data->target != 0) {
                node_index = node_index * 3;
            }
            spawn_proximity_corrected_size_alpha_tween_particle(&point,
                                                                node_index,
                                                                color,
                                                                0,
                                                                sign,
                                                                &out,
                                                                data->target,
                                                                spark_size * 0.1f,
                                                                spark_size);
            spawn_proximity_corrected_size_alpha_tween_particle(&point,
                                                                node_index,
                                                                0x7F7F7F7F,
                                                                1,
                                                                -sign,
                                                                &out,
                                                                data->target,
                                                                spark_size * 0.07f,
                                                                spark_size * 0.7f);
        }
    }
    if (func_001F9908_2BE3A8(&D_L00_00161824_macro) != 0) {
        D_L00_00161824_macro = func_001F9850_2BE3A8(beam_pulse_spawn_interval_frames_gp);
        for (slot = 0; slot < 3; slot++) {
            if (D_L00_001618E8_2BE3A8[slot] == 0) {
                D_L00_001618E8_2BE3A8[slot] = 1;
                D_L00_001617D8_macro[slot] = func_001F9850_2BE3A8(D_L00_0016174C_gp);
                D_L00_001617F8_macro[slot] = 0.0f;
                D_L00_001618D8_2BE3A8[slot] = D_L00_0016175C_gp;
                D_L00_001618B8_2BE3A8[slot] = func_001FA888_2BE3A8(D_L00_00161763_macro[0]);
                qcopy(&beam_pulse_positions[slot], data);
                func_001F9BF0_2BE3A8(&side, &beam_nodes[1], &beam_nodes[0]);
                func_L00_001FF4B0_2BE3A8(&beam_pulse_directions[slot], &side, 1.0f);
                break;
            }
        }
    }
    for (pulse = 0; pulse < 3; pulse++) {
        Vec_2BE3A8 step;
        float      dt;
        float      travelled;
        float      max_length;
        float      ratio;
        float      remainder;
        float      inv_length;

        if (D_L00_001618E8_2BE3A8[pulse] == 0) {
            continue;
        }
        if (func_001F9908_2BE3A8(&D_L00_001617D8_macro[pulse]) != 0) {
            D_L00_001618E8_2BE3A8[pulse] = 0;
            continue;
        }
        if (D_L00_001617D8_macro[pulse] < D_L00_00161750_gp) {
            D_L00_001618B8_2BE3A8[pulse] = D_L00_001618B8_2BE3A8[pulse] - D_L00_00161768_gp;
        }
        dt = D_L00_00161748_gp * D_0015ED6C_gp;
        travelled = D_L00_001617F8_macro[pulse] + dt;
        max_length = D_L00_001616DC_gp;
        ratio = travelled / max_length;
        D_L00_001617F8_macro[pulse] = travelled;
        if (1.0f < ratio) {
            ratio = 1.0f;
        }
        D_L00_001618D8_2BE3A8[pulse] =
            D_L00_0016175C_gp + (D_L00_00161758_gp - D_L00_0016175C_gp) * ratio;
        if (max_length <= travelled) {
            func_001F9C30_2BE3A8(&side, &beam_pulse_directions[pulse], dt);
            func_001F9BD8_2BE3A8(&beam_pulse_positions[pulse], &beam_pulse_positions[pulse], &side);
            continue;
        }
        node_index = func_001FA898_2BE3A8(travelled / D_L00_001617CC_macro);
        remainder =
            D_L00_001617F8_macro[pulse] - func_001FA888_2BE3A8(node_index) * D_L00_001617CC_macro;
        qcopy(&beam_pulse_positions[pulse], &beam_nodes[node_index]);
        if (node_index == 11) {
            func_001F9BF0_2BE3A8(&side, &beam_nodes[node_index], &beam_prev_nodes[node_index]);
        } else {
            func_001F9BF0_2BE3A8(&side, &beam_nodes[node_index + 1], &beam_nodes[node_index]);
        }
        inv_length = fast_vec_length(&side);
        if (inv_length == 0.0f) {
            continue;
        }
        inv_length = 1.0f / inv_length;
        func_001F9C30_2BE3A8(&step, &side, remainder * inv_length);
        func_001F9BD8_2BE3A8(&beam_pulse_positions[pulse], &beam_pulse_positions[pulse], &step);
        func_L00_001FF4B0_2BE3A8(&step, &side, inv_length);
        func_001F9C08_2BE3A8(
            &beam_pulse_directions[pulse], &beam_pulse_directions[pulse], &step, 0.1f);
        func_L00_001FF4B0_2BE3A8(
            &beam_pulse_directions[pulse], &beam_pulse_directions[pulse], 1.0f);
    }
}

/* scrolls the two texture-offset accumulators and draws the 10x20 band of
   textured quads between the D_L00_001DC060/001DC1A0 rings, skipping
   back-facing ones */
void draw_beam_ring_and_rail_effects(void *moby) {
    Quad    quad;
    OvlVec4 edge_to_corner0, edge_to_corner2, face_normal, camera_to_corner1;
    float   y, first_pass_v, second_pass_v, texture_scroll_step;
    int     c, i, j, k, off, jo;

    beam_rail_first_pass_texture_u_offset_gp =
        beam_rail_first_pass_texture_u_offset_gp -
        beam_rail_first_pass_texture_u_scroll_speed_gp * frame_scale;
    if (beam_rail_first_pass_texture_u_offset_gp <= -8.0f)
        beam_rail_first_pass_texture_u_offset_gp += 8.0f;
    draw_scrolling_ribbon_colored(beam_rail_a_points,
                                  beam_rail_first_pass_alpha_gp,
                                  beam_rail_second_pass_alpha_gp,
                                  beam_rail_first_pass_half_width_gp,
                                  beam_rail_second_pass_half_width_gp,
                                  beam_rail_second_pass_segment_extension_factor_gp,
                                  12,
                                  beam_first_rail_second_pass_base_vertex_color_gp);
    draw_scrolling_ribbon_colored(beam_rail_b_points,
                                  beam_rail_first_pass_alpha_gp,
                                  beam_rail_second_pass_alpha_gp,
                                  beam_rail_first_pass_half_width_gp,
                                  beam_rail_second_pass_half_width_gp,
                                  beam_rail_second_pass_segment_extension_factor_gp,
                                  12,
                                  beam_second_rail_second_pass_base_vertex_color_gp);
    draw_beam_pulse_quads(moby);
    draw_beam_origin_camera_facing_quads(moby);
    k = D_0013E535;
    c = k ? D_L00_00161798 : beam_ring_default_vertex_color_gp;
    quad.tex = FUN_001f44b8(beam_ring_texture_index_gp);
    quad.unk80 = 0xFF9000000260L;
    quad.unk88 = 0x8000000048L;
    quad.unk70 = 0;
    quad.color[3] = c;
    quad.color[2] = c;
    quad.color[1] = c;
    quad.color[0] = c;
    y = D_L00_00161718;
    D_L00_00161718 = y - beam_ring_texture_u_scroll_speed_gp * frame_scale;
    if (D_L00_00161718 <= -8.0f)
        D_L00_00161718 += 8.0f;
    texture_scroll_step = beam_ring_texture_v_scroll_speed_gp * frame_scale;
    D_L00_00161720 = D_L00_00161720 - texture_scroll_step;
    if (D_L00_00161720 <= -7.0f)
        D_L00_00161720 += 7.0f;
    D_L00_00161724 = D_L00_00161724 + texture_scroll_step;
    if (D_L00_00161724 >= 5.0f)
        D_L00_00161724 -= 5.0f;
    for (i = 2; i < 12; i++) {
        first_pass_v = D_L00_00161720;
        second_pass_v = D_L00_00161724;
        for (j = 0; j < 20; j++) {
            qcopy(&quad.corner[0], &D_L00_001DC1A0[i][j]);
            qcopy(&quad.corner[1], &D_L00_001DC1A0[i][(j + 1) % 20]);
            qcopy(&quad.corner[2], &D_L00_001DC060[i][j]);
            qcopy(&quad.corner[3], &D_L00_001DC060[i][(j + 1) % 20]);
            fast_vec_sub(&edge_to_corner0, &quad.corner[0], &quad.corner[1]);
            fast_vec_sub(&edge_to_corner2, &quad.corner[2], &quad.corner[1]);
            fast_vec_cross(&face_normal, &edge_to_corner2, &edge_to_corner0);
            fast_vec_sub(
                &camera_to_corner1, &quad.corner[1], &(*(OvlVec4 *)g_debug_camera.position));
            if (fast_vec_dot(&face_normal, &camera_to_corner1) >= 0.0f) {
                first_pass_v += beam_ring_face_texture_v_step_gp;
                second_pass_v += beam_ring_face_texture_v_step_gp;
            } else {
                if (i == 11) {
                    quad.color[0] = quad.color[1] = beam_ring_default_vertex_color_gp & 0xFFFFFF;
                }
                quad.uv[5] = first_pass_v;
                quad.uv[1] = first_pass_v;
                quad.uv[3] = first_pass_v + beam_ring_face_texture_v_step_gp;
                quad.uv[4] = y - beam_ring_band_texture_u_step_gp;
                quad.uv[0] = y;
                quad.uv[2] = y;
                quad.uv[7] = first_pass_v + beam_ring_face_texture_v_step_gp;
                quad.uv[6] = y - beam_ring_band_texture_u_step_gp;
                first_pass_v = first_pass_v + beam_ring_face_texture_v_step_gp;
                draw_geometry_quad(&quad, 0, 0);
                quad.uv[0] = y;
                quad.uv[1] = second_pass_v;
                quad.uv[2] = y;
                quad.uv[3] = second_pass_v + beam_ring_face_texture_v_step_gp;
                quad.uv[5] = second_pass_v;
                quad.uv[4] = y - beam_ring_band_texture_u_step_gp;
                quad.uv[6] = y - beam_ring_band_texture_u_step_gp;
                quad.uv[7] = second_pass_v + beam_ring_face_texture_v_step_gp;
                second_pass_v = second_pass_v + beam_ring_face_texture_v_step_gp;
                draw_geometry_quad(&quad, 0, 0);
            }
        }
        y += beam_ring_band_texture_u_step_gp;
    }
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002BA7C8.c: func_L00_002BF380). */
void draw_scrolling_ribbon_colored(void *mv,
                                   int   ribbon_first_pass_alpha,
                                   int   ribbon_second_pass_alpha,
                                   float sx,
                                   float sy,
                                   float sz,
                                   int   ribbon_point_count,

                                   int col) {
    RW_2baae0 *ribbon_points = mv;
    RW_2baae0  ribbon_first_pass_corners[4];
    RT_2baae0  ribbon_first_pass_render_attributes;
    RW_2baae0  q2[4];
    RT_2baae0  ribbon_second_pass_render_attributes;
    RW_2baae0  ribbon_camera_facing_side_vector, ribbon_second_pass_width_offset,
        ribbon_first_pass_point_camera_offset, ribbon_segment_delta, ribbon_adjusted_segment_start,
        ribbon_adjusted_segment_end, ribbon_segment_start_camera_offset,
        ribbon_segment_end_camera_offset, s1A0, s1B0;
    int   i, col0, col1;
    float ribbon_first_pass_side_normalization_factor, ribbon_initial_side_normalization_factor,
        ribbon_second_pass_side_normalization_factor, ribbon_segment_start_camera_distance,
        ribbon_segment_end_camera_distance;
    ribbon_first_pass_render_attributes.q0 =
        get_effect_texture(beam_rail_first_pass_texture_index_gp);
    col0 = beam_rail_first_pass_base_vertex_color_gp | (ribbon_first_pass_alpha << 24);
    ribbon_first_pass_render_attributes.q2 = 0x8000000048L;
    ribbon_first_pass_render_attributes.q1 = 0xFF9000000260L;
    ribbon_first_pass_render_attributes.z4 = 0;
    ribbon_first_pass_render_attributes.c[3] = col0;
    ribbon_first_pass_render_attributes.c[2] = col0;
    ribbon_first_pass_render_attributes.c[1] = col0;
    ribbon_first_pass_render_attributes.c[0] = col0;
    ribbon_second_pass_render_attributes.q0 =
        get_effect_texture(beam_rail_second_pass_texture_index_gp);
    col1 = col | (ribbon_second_pass_alpha << 24);
    ribbon_second_pass_render_attributes.q2 = ribbon_first_pass_render_attributes.q2;
    ribbon_second_pass_render_attributes.q1 = ribbon_first_pass_render_attributes.q1;
    ribbon_second_pass_render_attributes.z4 = ribbon_first_pass_render_attributes.z4;
    ribbon_second_pass_render_attributes.c[3] = col1;
    ribbon_second_pass_render_attributes.c[2] = col1;
    ribbon_second_pass_render_attributes.c[1] = col1;
    ribbon_second_pass_render_attributes.c[0] = col1;
    ribbon_second_pass_render_attributes.f[0] = 0;
    ribbon_second_pass_render_attributes.f[1] = 0;
    ribbon_second_pass_render_attributes.f[2] = 0;
    ribbon_second_pass_render_attributes.f[3] = 1.0f;
    ribbon_second_pass_render_attributes.f[4] = 1.0f;
    ribbon_second_pass_render_attributes.f[5] = 0;
    ribbon_second_pass_render_attributes.f[6] = 1.0f;
    ribbon_second_pass_render_attributes.f[7] = 1.0f;
    ribbon_first_pass_render_attributes.f[0] =
        ribbon_second_pass_render_attributes.f[0] + beam_rail_first_pass_texture_u_offset_gp;
    ribbon_first_pass_render_attributes.f[1] = ribbon_second_pass_render_attributes.f[1];
    ribbon_first_pass_render_attributes.f[2] =
        ribbon_second_pass_render_attributes.f[2] + beam_rail_first_pass_texture_u_offset_gp;
    ribbon_first_pass_render_attributes.f[3] = ribbon_second_pass_render_attributes.f[3];
    ribbon_first_pass_render_attributes.f[4] =
        ribbon_second_pass_render_attributes.f[4] + beam_rail_first_pass_texture_u_offset_gp;
    ribbon_first_pass_render_attributes.f[5] = ribbon_second_pass_render_attributes.f[5];
    ribbon_first_pass_render_attributes.f[6] =
        ribbon_second_pass_render_attributes.f[6] + beam_rail_first_pass_texture_u_offset_gp;
    ribbon_first_pass_render_attributes.f[7] = ribbon_second_pass_render_attributes.f[7];
    fast_vec_sub(ribbon_first_pass_point_camera_offset, ribbon_points, g_debug_camera.position);
    fast_vec_sub(ribbon_segment_delta, &ribbon_points[1], ribbon_points);
    fast_vec_cross(ribbon_camera_facing_side_vector,
                   ribbon_segment_delta,
                   ribbon_first_pass_point_camera_offset);
    ribbon_initial_side_normalization_factor = fast_vec_length(ribbon_camera_facing_side_vector);
    if (ribbon_initial_side_normalization_factor != 0)
        ribbon_initial_side_normalization_factor = 1.0f / ribbon_initial_side_normalization_factor;
    fast_vec_scale(beam_rail_first_pass_half_width_gp * ribbon_initial_side_normalization_factor,
                   ribbon_camera_facing_side_vector,
                   ribbon_camera_facing_side_vector);
    fast_vec_add(ribbon_first_pass_corners[0], ribbon_points, ribbon_camera_facing_side_vector);
    fast_vec_sub(ribbon_first_pass_corners[1], ribbon_points, ribbon_camera_facing_side_vector);
    for (i = 1; i < ribbon_point_count - 1; i++) {
        fast_vec_sub(
            ribbon_first_pass_point_camera_offset, &ribbon_points[i], g_debug_camera.position);
        fast_vec_sub(ribbon_segment_delta, &ribbon_points[i + 1], &ribbon_points[i]);
        fast_vec_cross(ribbon_camera_facing_side_vector,
                       ribbon_segment_delta,
                       ribbon_first_pass_point_camera_offset);
        ribbon_first_pass_side_normalization_factor =
            fast_vec_length(ribbon_camera_facing_side_vector);
        if (ribbon_first_pass_side_normalization_factor != 0)
            ribbon_first_pass_side_normalization_factor =
                1.0f / ribbon_first_pass_side_normalization_factor;
        fast_vec_scale(beam_rail_first_pass_half_width_gp *
                           ribbon_first_pass_side_normalization_factor,
                       ribbon_camera_facing_side_vector,
                       ribbon_camera_facing_side_vector);
        fast_vec_add(
            ribbon_first_pass_corners[2], &ribbon_points[i], ribbon_camera_facing_side_vector);
        fast_vec_sub(
            ribbon_first_pass_corners[3], &ribbon_points[i], ribbon_camera_facing_side_vector);
        if (i == 1) {
            ribbon_first_pass_render_attributes.c[1] = beam_rail_first_pass_base_vertex_color_gp;
            ribbon_first_pass_render_attributes.c[0] = beam_rail_first_pass_base_vertex_color_gp;
        } else if (i == ribbon_point_count - 2) {
            ribbon_first_pass_render_attributes.c[3] = beam_rail_first_pass_base_vertex_color_gp;
            ribbon_first_pass_render_attributes.c[2] = beam_rail_first_pass_base_vertex_color_gp;
        } else if (i == 2) {
            ribbon_first_pass_render_attributes.c[1] = col0;
            ribbon_first_pass_render_attributes.c[0] = col0;
        }
        draw_geometry_quad(ribbon_first_pass_corners, 0, 0);
        qcopy(ribbon_first_pass_corners[0], ribbon_first_pass_corners[2]);
        qcopy(ribbon_first_pass_corners[1], ribbon_first_pass_corners[3]);
    }
    for (i = 0; i < ribbon_point_count - 1; i++) {
        fast_vec_sub(
            ribbon_segment_start_camera_offset, &ribbon_points[i], g_debug_camera.position);
        fast_vec_sub(
            ribbon_segment_end_camera_offset, &ribbon_points[i + 1], g_debug_camera.position);
        ribbon_segment_start_camera_distance = fast_vec_length(ribbon_segment_start_camera_offset);
        ribbon_segment_end_camera_distance = fast_vec_length(ribbon_segment_end_camera_offset);
        if (ribbon_segment_end_camera_distance < ribbon_segment_start_camera_distance) {
            if (ribbon_segment_end_camera_distance == 0)
                qcopy(ribbon_segment_end_camera_offset, ribbon_segment_start_camera_offset);
            else
                fast_vec_scale(ribbon_segment_start_camera_distance /
                                   ribbon_segment_end_camera_distance,
                               ribbon_segment_end_camera_offset,
                               ribbon_segment_end_camera_offset);
        } else {
            if (ribbon_segment_start_camera_distance == 0)
                qcopy(ribbon_segment_start_camera_offset, ribbon_segment_end_camera_offset);
            else
                fast_vec_scale(ribbon_segment_end_camera_distance /
                                   ribbon_segment_start_camera_distance,
                               ribbon_segment_start_camera_offset,
                               ribbon_segment_start_camera_offset);
        }
        fast_vec_add(ribbon_adjusted_segment_start,
                     g_debug_camera.position,
                     ribbon_segment_start_camera_offset);
        fast_vec_add(
            ribbon_adjusted_segment_end, g_debug_camera.position, ribbon_segment_end_camera_offset);
        fast_vec_sub(
            ribbon_segment_delta, ribbon_adjusted_segment_end, ribbon_adjusted_segment_start);
        fast_vec_scale(sz, s1B0, ribbon_segment_delta);
        fast_vec_sub(ribbon_adjusted_segment_start, ribbon_adjusted_segment_start, s1B0);
        fast_vec_add(ribbon_adjusted_segment_end, ribbon_adjusted_segment_end, s1B0);
        fast_vec_sub(s1A0, g_debug_camera.position, ribbon_adjusted_segment_start);
        fast_vec_cross(ribbon_camera_facing_side_vector, s1A0, ribbon_segment_delta);
        ribbon_second_pass_side_normalization_factor =
            fast_vec_length(ribbon_camera_facing_side_vector);
        if (ribbon_second_pass_side_normalization_factor != 0)
            ribbon_second_pass_side_normalization_factor =
                1.0f / ribbon_second_pass_side_normalization_factor;
        fast_vec_scale(sy * ribbon_second_pass_side_normalization_factor,
                       ribbon_second_pass_width_offset,
                       ribbon_camera_facing_side_vector);
        fast_vec_add(q2[0], ribbon_adjusted_segment_start, ribbon_second_pass_width_offset);
        fast_vec_sub(q2[1], ribbon_adjusted_segment_start, ribbon_second_pass_width_offset);
        fast_vec_add(q2[2], ribbon_adjusted_segment_end, ribbon_second_pass_width_offset);
        fast_vec_sub(q2[3], ribbon_adjusted_segment_end, ribbon_second_pass_width_offset);
        if (i == 0) {
            ribbon_second_pass_render_attributes.c[1] = col;
            ribbon_second_pass_render_attributes.c[0] = col;
        } else if (i == ribbon_point_count - 2) {
            ribbon_second_pass_render_attributes.c[3] = col;
            ribbon_second_pass_render_attributes.c[2] = col;
        } else if (i == 1) {
            ribbon_second_pass_render_attributes.c[1] = col1;
            ribbon_second_pass_render_attributes.c[0] = col1;
        }
        draw_geometry_quad(q2, 0, 0);
    }
    vu1_add_g_sregister(0x47, 0x5360B);
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002BA7C8.c: func_L00_002BF958). */
void draw_beam_pulse_quads(void *unused) {
    struct GeometryQuad quad;
    Vec4                basis[3], position, corner;
    int                 color;
    Vec4               *origin;
    int                 i;
    int                 j;

    quad.texture = get_effect_texture(8);
    quad.primitive = 0x8000000048;
    quad.texture_state = 0xFF9000000260;
    quad.reserved.value = 0;
    quad.texture_coordinates[0][0] = 0;
    quad.texture_coordinates[0][1] = 0;
    quad.texture_coordinates[1][0] = 0;
    quad.texture_coordinates[1][1] = 1.0f;
    quad.texture_coordinates[2][0] = 1.0f;
    quad.texture_coordinates[2][1] = 0;
    quad.texture_coordinates[3][0] = 1.0f;
    quad.texture_coordinates[3][1] = 1.0f;
    {
        int selected_color = D_0013E535 != 0 ? D_L00_00161764 : beam_pulse_default_rgb_gp;
        color = selected_color & 0xFFFFFF;
    }
    origin = &position;
    for (i = 0; i < 3; i++) {
        int rgba;
        if (D_L00_00161818_macro[i] != 0) {
            rgba = color | (truncate_float_to_s32(D_L00_001617E8_macro[i]) << 24);
            quad.colors[3] = rgba;
            quad.colors[2] = rgba;
            quad.colors[1] = rgba;
            quad.colors[0] = rgba;
            qcopy(origin, &beam_pulse_positions[i]);
            qcopy(&basis[0], &beam_pulse_directions[i]);
            fast_vec_scale(-1.0f, &basis[2], D_0013F5E0);
            fast_vec_cross(&basis[1], &basis[0], &basis[2]);
            fast_vec_normalize(&basis[1], &basis[1], 1.0f);
            fast_vec_cross(&basis[2], &basis[1], &basis[0]);
            for (j = 0; j < 4; j++) {
                fast_vec_scale(D_L00_00161808_macro[i], &corner, &D_L00_001DBF20[j]);
                transform_vector(&quad.positions[j], &corner, &basis[0]);
            }
            draw_geometry_quad(&quad, 0, 0);
        }
    }
}

void draw_beam_origin_camera_facing_quads(void *mv) {
    char    *moby_bytes = mv;
    Q_2be890 quad;
    V_2be890 camera_facing_transform[4];
    V_2be890 scaled_corner;
    V_2be890 offset_vector;
    int      first_pass_vertex_color;
    int      i, j;
    quad.tex = get_effect_texture(11);
    first_pass_vertex_color = beam_origin_quad_first_pass_vertex_color_gp;
    quad.unk88 = 0x8000000048L;
    quad.unk80 = 0xFF9000000260L;
    quad.unk70 = 0;
    quad.unk50 = 0;
    quad.unk54 = 0;
    quad.color[3] = first_pass_vertex_color;
    quad.color[2] = first_pass_vertex_color;
    quad.color[1] = first_pass_vertex_color;
    quad.color[0] = first_pass_vertex_color;
    quad.unk58 = 0;
    quad.unk5C = 1.0f;
    quad.unk60 = 1.0f;
    quad.unk64 = 0;
    quad.unk68 = 1.0f;
    quad.unk6C = 1.0f;
    FUN_001f9bf8_2be890(&offset_vector, moby_bytes + 0xD0, beam_origin_quad_basis_offset_length_gp);
    FUN_001f9a10_2be890(&camera_facing_transform[3], &beam_nodes[0], &offset_vector);
    FUN_001f9a28_2be890(
        &offset_vector, &(*(V_2be890 *)g_debug_camera.position), &camera_facing_transform[3]);
    FUN_001f9bf8_2be890(&camera_facing_transform[0], &offset_vector, 1.0f);
    FUN_001f9ad8_2be890(
        &camera_facing_transform[1], &camera_facing_transform[0], &(*(V_2be890 *)D_0013F5E0));
    FUN_001f9bf8_2be890(&camera_facing_transform[1], &camera_facing_transform[1], -1.0f);
    FUN_001f9ad8_2be890(
        &camera_facing_transform[2], &camera_facing_transform[1], &camera_facing_transform[0]);
    for (i = 3; i >= 0; i--) {
        FUN_001f9a68_2be890(
            beam_origin_quad_first_pass_corner_scale_gp, &scaled_corner, &D_L00_001DBF20[3 - i]);
        FUN_001f9d20_2be890(&quad.corner[3 - i], &scaled_corner, &camera_facing_transform[0]);
    }
    FUN_001f7d30_2be890(&quad, 0, 0);
    quad.color[3] = -1;
    quad.color[2] = -1;
    quad.color[1] = -1;
    quad.color[0] = -1;
    for (j = 3; j >= 0; j--) {
        FUN_001f9a68_2be890(
            beam_origin_quad_second_pass_corner_scale_gp, &scaled_corner, &D_L00_001DBF20[3 - j]);
        FUN_001f9d20_2be890(&quad.corner[3 - j], &scaled_corner, &camera_facing_transform[0]);
    }
    FUN_001f7d30_2be890(&quad, 0, 0);
}

int   D_L00_001617BC = 0;
int   D_L00_001617C8 = 0;
float D_L00_001617CC_macro __asm__("D_L00_001617CC") = 0;
s32   D_L00_00161718_gp __asm__("D_L00_00161718") = 0;
s32   D_L00_00161720_gp __asm__("D_L00_00161720") = 0;
s32   D_L00_00161724_gp __asm__("D_L00_00161724") = 0;
float D_L00_001617D0 = 0;
int   D_L00_00161824_macro __asm__("D_L00_00161824") = 0;
int   D_L00_001617D8_macro[3] __asm__("D_L00_001617D8") = {0};

float D_L00_001617F8_macro[3] __asm__("D_L00_001617F8") = {0};

Vec_2BE3A8 beam_prev_nodes[12] __asm__("D_L00_001DBF50") = {
    {0.0f, 1.0f, -1.0f, 1.0f},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
};

Vec_2BE3A8 beam_rail_a_points[12] __asm__("D_L00_001DC020") = {0};

Vec_2BE3A8 beam_rail_b_points[12] __asm__("D_L00_001DC0E0") = {0};

Vec_2BE3A8 beam_pulse_positions[3] __asm__("D_L00_001DD0A0") = {0};

Vec_2BE3A8 beam_pulse_directions[3] __asm__("D_L00_001DD0D0") = {0};

Tab l00_beam_origin_particle_color_palette __asm__("D_L00_001E9AB0") = {
    {0x7f207f7f, 0x7f7f2020, 0x7f20207f, 0x7f7f207f, 0x7f7f7f20}
};

float D_L00_001617E8_macro[3] __asm__("D_L00_001617E8") = {0};

float D_L00_00161808_macro[3] __asm__("D_L00_00161808") = {0};

int D_L00_00161818_macro[3] __asm__("D_L00_00161818") = {0};

Vec_2BE3A8 beam_nodes[1] __asm__("D_L00_001DBF60") = {0};
