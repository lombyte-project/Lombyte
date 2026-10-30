/* Follow camera targeting: the hero target point, collision-aware target positions, transitions and the position springs. */

#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/camera/camera_modes.h"
#include "rnc/hero/hero.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/fast_math.h"
#include "qcopy.h"
#include "rnc/overlay/collision.h"
#include "rnc/camera/follow_cam.h"

typedef struct {
    char b[0x80];
} M_2e4180_002e4180;

typedef struct {
    u8    p0[0x2DC];
    f32   f2DC;
    u8    p1[0x30C - 0x2E0];
    s16   h30C;
    u8    p2[0x2080 - 0x30E];
    void *p2080;
} G_002e42c0;

typedef float V4_2e4430[4] __attribute__((aligned(16)));

typedef struct {
    int a[4];
} Vq4 __attribute__((aligned(16)));

typedef float V4[4] __attribute__((aligned(16)));

typedef struct {
    float         v00[4];
    float         v10[4];
    float         v20[4];
    float         v30[4];
    char          pad40[0x84];
    unsigned char state;
    unsigned char prev;
    short         timer;
    float         invTimer;
    float         invMove;
    float         dist;
    short         moveTimer;
    char          padD6;
    unsigned char hit;
} CamAttach;

typedef struct {
    f32 v[4];
} V002e5730 __attribute__((aligned(16)));

typedef struct {
    u8  p0[0x90];
    u8  x90[0x34];
    u8  xc4;
    u8  xc5;
    s16 xc6;
    f32 xc8;
} A002e5730;

typedef struct {
    u8  p[0x70];
    u8 *m;
} O002e5730;

/* lerp_vector with the blend factor first, as these callers pass it. */
void         lerp_vector_by(f32 t, void *out, void *a, void *b) __asm__("FUN_001f9a40");
extern int   coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern char *FUN_L00_001ee1b0_002e4180(void *) __asm__("FUN_L00_001ee1b0");
extern void  FUN_001f9a28_002e4180(void *, void *, void *) __asm__("FUN_001f9a28");
extern float FUN_001f9af0_002e4180(void *) __asm__("FUN_001f9af0");
extern void  FUN_001f9a68_002e4180(void *, void *, float) __asm__("FUN_001f9a68");
extern void  FUN_001f9a10_002e4180(void *, void *, void *) __asm__("FUN_001f9a10");
int          find_camera_collision_volume_for_point(OvlQuad *, float) __asm__("FUN_L00_001ee1b0");
void         FUN_001f9a68_002e42c0(OvlQuad *, void *, f32) __asm__("FUN_001f9a68");
void         FUN_001f9a10_002e42c0(OvlQuad *, OvlQuad *, OvlQuad *) __asm__("FUN_001f9a10");
s32          FUN_001efa68_002e42c0(OvlQuad *, OvlQuad *, s32, void *, s32) __asm__("FUN_001efa68");
void         FUN_001f9a68_002e4430(void *, void *, float) __asm__("FUN_001f9a68");
void         FUN_001f9a10_002e4430(void *, void *, void *) __asm__("FUN_001f9a10");
void         FUN_001f9a28_002e4430(void *, void *, void *) __asm__("FUN_001f9a28");
extern void  FUN_001f9a10_c2(void *, void *, void *) __asm__("FUN_001f9a10");
extern void  FUN_001f9a68_c3(void *, void *, float) __asm__("FUN_001f9a68");
extern int   FUN_001f96f8_c(int) __asm__("FUN_001f96f8");
extern void  FUN_001f9a10_c(void *, void *, void *) __asm__("FUN_001f9a10");
extern void  FUN_001f9a68_c(void *, void *, float) __asm__("FUN_001f9a68");
extern void  vcopy_2e48a0(void *, void *) __asm__("FUN_001f9a68");
extern int   FUN_001f9770_c2(void *) __asm__("FUN_001f9770");
extern void  FUN_L00_002e48a0_c(void *out, float f) __asm__("FUN_L00_002e48a0");
extern int   FUN_001f96f8_c2(int) __asm__("FUN_001f96f8");
extern int   FUN_001f9770_c(void *) __asm__("FUN_001f9770");
extern void  FUN_001f9a68_c2(void *, void *, float) __asm__("FUN_001f9a68");
f32          cam_interp_values(void *, f32, f32, f32, f32, f32) __asm__("FUN_001ebd78");
extern void  scale_vector_xyz(void *out, void *a, f32 s) __asm__("FUN_001f9a68");
extern s32
collision_line(void *from, void *to, s32 mask, s32 ignore, s32 flags) __asm__("FUN_001efa68");
extern void follow_cam_update_mode(void *cam) __asm__("FUN_L00_002e4930");
extern void follow_cam_focus_velocity(void *cam, f32 *out) __asm__("FUN_L00_002e4fe8");

extern M_2e4180_002e4180 *D_L00_001600F4_002e4180 __asm__("D_L00_001600F4");
extern u8                 D_L00_00166E10_lui[] __asm__("D_L00_00166E10") NOT_SDA;
extern OvlQuad D_L00_00173E60_002e42c0 __asm__("D_L00_00173E60") __attribute__((section(".data")));
extern char    D_L00_00166E10_002e4430[] __asm__("D_L00_00166E10") NOT_SDA;
extern char    D_L00_00166E10[];
extern char    D_0013E533[];
extern int     camera_secondary_mode_lui __asm__("D_001413D4") NOT_SDA;
extern int D_001413DC       NOT_SDA;
extern float D_L00_00166EEC NOT_SDA;
extern s32                  D_L00_00161D24_gp __asm__("D_L00_00161D24") __attribute__((sda));
extern short follow_camera_ground_aware_focus_transition_frames_gp __asm__("D_L00_00161D38")
    __attribute__((sda));
extern short D_L00_00161D3C_gp __asm__("D_L00_00161D3C") __attribute__((sda));
extern short D_L00_00161D40_gp __asm__("D_L00_00161D40") __attribute__((sda));
extern short follow_camera_blend_out_focus_offset_gp __asm__("D_L00_00161D48") __attribute__((sda));
extern short follow_camera_project_hero_focus_blend_base_frames_gp __asm__("D_L00_00161D4C")
    __attribute__((sda));
extern short follow_camera_focus_blend_transition_frames_gp __asm__("D_L00_00161D50")
    __attribute__((sda));
extern short D_L00_00161D54_gp __asm__("D_L00_00161D54") __attribute__((sda));
extern short follow_camera_project_hero_focus_offset_gp __asm__("D_L00_00161D58")
    __attribute__((sda));
extern short follow_camera_hold_focus_blend_base_frames_gp __asm__("D_L00_00161D5C")
    __attribute__((sda));
extern short follow_camera_hold_focus_transition_frames_gp __asm__("D_L00_00161D60")
    __attribute__((sda));
extern short follow_camera_hold_focus_exit_base_frames_gp __asm__("D_L00_00161D64")
    __attribute__((sda));
extern short follow_camera_hold_focus_blend_offset_gp __asm__("D_L00_00161D68")
    __attribute__((sda));
extern short D_L00_00161D6C_gp __asm__("D_L00_00161D6C") __attribute__((sda));
extern short follow_camera_continue_projected_focus_transition_frames_gp __asm__("D_L00_00161D78")
    __attribute__((sda));
extern short follow_camera_projected_focus_blend_offset_gp __asm__("D_L00_00161D7C")
    __attribute__((sda));
extern short follow_camera_ground_focus_angle_upper_bound_gp __asm__("D_L00_00161D0C")
    __attribute__((sda));
extern short follow_camera_ground_focus_angle_lower_bound_gp __asm__("D_L00_00161D10")
    __attribute__((sda));
extern short follow_camera_projected_focus_blend_fraction_gp __asm__("D_L00_00161D80")
    __attribute__((sda));
extern f32 follow_camera_position_default_stiffness_gp __asm__("D_L00_00161D28")
    __attribute__((sda));
extern f32 follow_camera_position_default_damping_gp __asm__("D_L00_00161D2C") __attribute__((sda));
extern f32 follow_camera_position_transition_end_stiffness_gp __asm__("D_L00_00161D30")
    __attribute__((sda));
extern f32 follow_camera_position_transition_end_damping_gp __asm__("D_L00_00161D34")
    __attribute__((sda));
/* The camera view: +0x20 its forward axis, +0x30 its up axis. */
extern u8  camera_view[] __asm__("D_L00_00166E10");
extern f32 follow_cam_min_distance_gp __asm__("D_L00_00161CD8") __attribute__((sda));
extern s32 follow_cam_blend_frames __asm__("D_L00_00161D20");
extern s32 camera_collision_mask __asm__("D_L00_0015EF58");
/* Point where the last collision line stopped (CollisionHit.point). */
extern f32 collision_hit_point[4] __asm__("D_L00_00173E60") __attribute__((aligned(16)));

f32 D_L01_0016736C[1] __attribute__((section(".data"))) = {0};

f32 D_L02_001674EC[1] __attribute__((section(".data"))) = {0};

f32 D_L03_00166FEC[1] __attribute__((section(".data"))) = {0};

f32 D_L04_0016706C[1] __attribute__((section(".data"))) = {0};

f32 D_L05_001672EC[1] __attribute__((section(".data"))) = {0};

f32 D_L06_0016766C[1] __attribute__((section(".data"))) = {0};

f32 D_L07_00166F6C[1] __attribute__((section(".data"))) = {0};

f32 D_L09_0016706C[1] __attribute__((section(".data"))) = {0};

f32 D_L10_0016736C[1] __attribute__((section(".data"))) = {0};

f32 D_L12_0016736C[1] __attribute__((section(".data"))) = {0};

f32 D_L13_001671EC[1] __attribute__((section(".data"))) = {0};

f32 D_L14_001675EC[1] __attribute__((section(".data"))) = {0};

f32 D_L15_001674EC[1] __attribute__((section(".data"))) = {0};

f32 D_L16_001672EC[1] __attribute__((section(".data"))) = {0};

short                D_L01_001621B0 = 0x1e;
short                D_L01_001621B4 = 0xa;
short                D_L01_001621B8 = 0x23;
short                D_L01_001621C0 = 0;
short                D_L01_001621C4 = 0x55;
short                D_L01_001621C8 = 0x1e;
short                D_L01_001621CC = 0x14;
short                D_L01_001621D0 = 0;
short                D_L01_001621D4 = 0x2d;
short                D_L01_001621D8 = 0x1e;
short                D_L01_001621DC = 0x14;
short                D_L01_001621E0 = 0;
short                D_L01_001621E4 = 0x1e;
short                D_L01_001621F0 = 0x1e;
short                D_L01_001621F4 = 0;
short                D_L01_00162184 = 0xfdb;
short                D_L01_00162188 = -10486;
short                D_L01_001621F8 = 0;
f32                  D_L01_001621A0 = 0.01f;
f32                  D_L01_001621A4 = 0.2f;
f32                  D_L01_001621A8 = 0.06f;
f32                  D_L01_001621AC = 0.4f;
s32                  D_L01_00162198 = 0x5a;
short                D_L02_001622E8 = 0x1e;
short                D_L02_001622EC = 0xa;
short                D_L02_001622F0 = 0x23;
short                D_L02_001622F8 = 0;
short                D_L02_001622FC = 0x55;
short                D_L02_00162300 = 0x1e;
short                D_L02_00162304 = 0x14;
short                D_L02_00162308 = 0;
short                D_L02_0016230C = 0x2d;
short                D_L02_00162310 = 0x1e;
short                D_L02_00162314 = 0x14;
short                D_L02_00162318 = 0;
short                D_L02_0016231C = 0x1e;
short                D_L02_00162328 = 0x1e;
short                D_L02_0016232C = 0;
short                D_L02_001622BC = 0xfdb;
short                D_L02_001622C0 = -10486;
short                D_L02_00162330 = 0;
f32                  D_L02_001622D8 = 0.01f;
f32                  D_L02_001622DC = 0.2f;
f32                  D_L02_001622E0 = 0.06f;
f32                  D_L02_001622E4 = 0.4f;
s32                  D_L02_001622D0 = 0x5a;
short                D_L03_00161DB0 = 0x1e;
short                D_L03_00161DB4 = 0xa;
short                D_L03_00161DB8 = 0x23;
short                D_L03_00161DC0 = 0;
short                D_L03_00161DC4 = 0x55;
short                D_L03_00161DC8 = 0x1e;
short                D_L03_00161DCC = 0x14;
short                D_L03_00161DD0 = 0;
short                D_L03_00161DD4 = 0x2d;
short                D_L03_00161DD8 = 0x1e;
short                D_L03_00161DDC = 0x14;
short                D_L03_00161DE0 = 0;
short                D_L03_00161DE4 = 0x1e;
short                D_L03_00161DF0 = 0x1e;
short                D_L03_00161DF4 = 0;
short                D_L03_00161D84 = 0xfdb;
short                D_L03_00161D88 = -10486;
short                D_L03_00161DF8 = 0;
f32                  D_L03_00161DA0 = 0.01f;
f32                  D_L03_00161DA4 = 0.2f;
f32                  D_L03_00161DA8 = 0.06f;
f32                  D_L03_00161DAC = 0.4f;
s32                  D_L03_00161D98 = 0x5a;
short                D_L04_00161EA8 = 0x1e;
short                D_L04_00161EAC = 0xa;
short                D_L04_00161EB0 = 0x23;
short                D_L04_00161EB8 = 0;
short                D_L04_00161EBC = 0x55;
short                D_L04_00161EC0 = 0x1e;
short                D_L04_00161EC4 = 0x14;
short                D_L04_00161EC8 = 0;
short                D_L04_00161ECC = 0x2d;
short                D_L04_00161ED0 = 0x1e;
short                D_L04_00161ED4 = 0x14;
short                D_L04_00161ED8 = 0;
short                D_L04_00161EDC = 0x1e;
short                D_L04_00161EE8 = 0x1e;
short                D_L04_00161EEC = 0;
short                D_L04_00161E7C = 0xfdb;
short                D_L04_00161E80 = -10486;
short                D_L04_00161EF0 = 0;
f32                  D_L04_00161E98 = 0.01f;
f32                  D_L04_00161E9C = 0.2f;
f32                  D_L04_00161EA0 = 0.06f;
f32                  D_L04_00161EA4 = 0.4f;
s32                  D_L04_00161E90 = 0x5a;
short                D_L05_00162100 = 0x1e;
short                D_L05_00162104 = 0xa;
short                D_L05_00162108 = 0x23;
short                D_L05_00162110 = 0;
short                D_L05_00162114 = 0x55;
short                D_L05_00162118 = 0x1e;
short                D_L05_0016211C = 0x14;
short                D_L05_00162120 = 0;
short                D_L05_00162124 = 0x2d;
short                D_L05_00162128 = 0x1e;
short                D_L05_0016212C = 0x14;
short                D_L05_00162130 = 0;
short                D_L05_00162134 = 0x1e;
short                D_L05_00162140 = 0x1e;
short                D_L05_00162144 = 0;
short                D_L05_001620D4 = 0xfdb;
short                D_L05_001620D8 = -10486;
short                D_L05_00162148 = 0;
f32                  D_L05_001620F0 = 0.01f;
f32                  D_L05_001620F4 = 0.2f;
f32                  D_L05_001620F8 = 0.06f;
f32                  D_L05_001620FC = 0.4f;
s32                  D_L05_001620E8 = 0x5a;
short                D_L06_001624C8 = 0x1e;
short                D_L06_001624CC = 0xa;
short                D_L06_001624D0 = 0x23;
short                D_L06_001624D8 = 0;
short                D_L06_001624DC = 0x55;
short                D_L06_001624E0 = 0x1e;
short                D_L06_001624E4 = 0x14;
short                D_L06_001624E8 = 0;
short                D_L06_001624EC = 0x2d;
short                D_L06_001624F0 = 0x1e;
short                D_L06_001624F4 = 0x14;
short                D_L06_001624F8 = 0;
short                D_L06_001624FC = 0x1e;
short                D_L06_00162508 = 0x1e;
short                D_L06_0016250C = 0;
short                D_L06_0016249C = 0xfdb;
short                D_L06_001624A0 = -10486;
short                D_L06_00162510 = 0;
f32                  D_L06_001624B8 = 0.01f;
f32                  D_L06_001624BC = 0.2f;
f32                  D_L06_001624C0 = 0.06f;
f32                  D_L06_001624C4 = 0.4f;
s32                  D_L06_001624B0 = 0x5a;
short                D_L07_00161D80 = 0x1e;
short                D_L07_00161D84 = 0xa;
short                D_L07_00161D88 = 0x23;
short                D_L07_00161D90 = 0;
short                D_L07_00161D94 = 0x55;
short                D_L07_00161D98 = 0x1e;
short                D_L07_00161D9C = 0x14;
short                D_L07_00161DA0 = 0;
short                D_L07_00161DA4 = 0x2d;
short                D_L07_00161DA8 = 0x1e;
short                D_L07_00161DAC = 0x14;
short                D_L07_00161DB0 = 0;
short                D_L07_00161DB4 = 0x1e;
short                D_L07_00161DC0 = 0x1e;
short                D_L07_00161DC4 = 0;
short                D_L07_00161D54 = 0xfdb;
short                D_L07_00161D58 = -10486;
short                D_L07_00161DC8 = 0;
f32                  D_L07_00161D70 = 0.01f;
f32                  D_L07_00161D74 = 0.2f;
f32                  D_L07_00161D78 = 0.06f;
f32                  D_L07_00161D7C = 0.4f;
s32                  D_L07_00161D68 = 0x5a;
float D_L08_001676EC NOT_SDA = 0;
short                D_L08_00162540 = 0x1e;
short                D_L08_00162544 = 0xa;
short                D_L08_00162548 = 0x23;
short                D_L08_00162550 = 0;
short                D_L08_00162554 = 0x55;
short                D_L08_00162558 = 0x1e;
short                D_L08_0016255C = 0x14;
short                D_L08_00162560 = 0;
short                D_L08_00162564 = 0x2d;
short                D_L08_00162568 = 0x1e;
short                D_L08_0016256C = 0x14;
short                D_L08_00162570 = 0;
short                D_L08_00162574 = 0x1e;
short                D_L08_00162580 = 0x1e;
short                D_L08_00162584 = 0;
short                D_L08_00162514 = 0xfdb;
short                D_L08_00162518 = -10486;
short                D_L08_00162588 = 0;
f32                  D_L08_00162530 = 0.01f;
f32                  D_L08_00162534 = 0.2f;
f32                  D_L08_00162538 = 0.06f;
f32                  D_L08_0016253C = 0.4f;
s32                  D_L08_00162528 = 0x5a;
short                D_L09_00161E98 = 0x1e;
short                D_L09_00161E9C = 0xa;
short                D_L09_00161EA0 = 0x23;
short                D_L09_00161EA8 = 0;
short                D_L09_00161EAC = 0x55;
short                D_L09_00161EB0 = 0x1e;
short                D_L09_00161EB4 = 0x14;
short                D_L09_00161EB8 = 0;
short                D_L09_00161EBC = 0x2d;
short                D_L09_00161EC0 = 0x1e;
short                D_L09_00161EC4 = 0x14;
short                D_L09_00161EC8 = 0;
short                D_L09_00161ECC = 0x1e;
short                D_L09_00161ED8 = 0x1e;
short                D_L09_00161EDC = 0;
short                D_L09_00161E6C = 0xfdb;
short                D_L09_00161E70 = -10486;
short                D_L09_00161EE0 = 0;
f32                  D_L09_00161E88 = 0.01f;
f32                  D_L09_00161E8C = 0.2f;
f32                  D_L09_00161E90 = 0.06f;
f32                  D_L09_00161E94 = 0.4f;
s32                  D_L09_00161E80 = 0x5a;
short                D_L10_00162168 = 0x1e;
short                D_L10_0016216C = 0xa;
short                D_L10_00162170 = 0x23;
short                D_L10_00162178 = 0;
short                D_L10_0016217C = 0x55;
short                D_L10_00162180 = 0x1e;
short                D_L10_00162184 = 0x14;
short                D_L10_00162188 = 0;
short                D_L10_0016218C = 0x2d;
short                D_L10_00162190 = 0x1e;
short                D_L10_00162194 = 0x14;
short                D_L10_00162198 = 0;
short                D_L10_0016219C = 0x1e;
short                D_L10_001621A8 = 0x1e;
short                D_L10_001621AC = 0;
short                D_L10_0016213C = 0xfdb;
short                D_L10_00162140 = -10486;
short                D_L10_001621B0 = 0;
f32                  D_L10_00162158 = 0.01f;
f32                  D_L10_0016215C = 0.2f;
f32                  D_L10_00162160 = 0.06f;
f32                  D_L10_00162164 = 0.4f;
s32                  D_L10_00162150 = 0x5a;
float D_L11_001678EC NOT_SDA = 0;
short                D_L11_001626D0 = 0x1e;
short                D_L11_001626D4 = 0xa;
short                D_L11_001626D8 = 0x23;
short                D_L11_001626E0 = 0;
short                D_L11_001626E4 = 0x55;
short                D_L11_001626E8 = 0x1e;
short                D_L11_001626EC = 0x14;
short                D_L11_001626F0 = 0;
short                D_L11_001626F4 = 0x2d;
short                D_L11_001626F8 = 0x1e;
short                D_L11_001626FC = 0x14;
short                D_L11_00162700 = 0;
short                D_L11_00162704 = 0x1e;
short                D_L11_00162710 = 0x1e;
short                D_L11_00162714 = 0;
short                D_L11_001626A4 = 0xfdb;
short                D_L11_001626A8 = -10486;
short                D_L11_00162718 = 0;
f32                  D_L11_001626C0 = 0.01f;
f32                  D_L11_001626C4 = 0.2f;
f32                  D_L11_001626C8 = 0.06f;
f32                  D_L11_001626CC = 0.4f;
s32                  D_L11_001626B8 = 0x5a;
short                D_L12_001621A8 = 0x1e;
short                D_L12_001621AC = 0xa;
short                D_L12_001621B0 = 0x23;
short                D_L12_001621B8 = 0;
short                D_L12_001621BC = 0x55;
short                D_L12_001621C0 = 0x1e;
short                D_L12_001621C4 = 0x14;
short                D_L12_001621C8 = 0;
short                D_L12_001621CC = 0x2d;
short                D_L12_001621D0 = 0x1e;
short                D_L12_001621D4 = 0x14;
short                D_L12_001621D8 = 0;
short                D_L12_001621DC = 0x1e;
short                D_L12_001621E8 = 0x1e;
short                D_L12_001621EC = 0;
short                D_L12_0016217C = 0xfdb;
short                D_L12_00162180 = -10486;
short                D_L12_001621F0 = 0;
f32                  D_L12_00162198 = 0.01f;
f32                  D_L12_0016219C = 0.2f;
f32                  D_L12_001621A0 = 0.06f;
f32                  D_L12_001621A4 = 0.4f;
s32                  D_L12_00162190 = 0x5a;
short                D_L13_00162020 = 0x1e;
short                D_L13_00162024 = 0xa;
short                D_L13_00162028 = 0x23;
short                D_L13_00162030 = 0;
short                D_L13_00162034 = 0x55;
short                D_L13_00162038 = 0x1e;
short                D_L13_0016203C = 0x14;
short                D_L13_00162040 = 0;
short                D_L13_00162044 = 0x2d;
short                D_L13_00162048 = 0x1e;
short                D_L13_0016204C = 0x14;
short                D_L13_00162050 = 0;
short                D_L13_00162054 = 0x1e;
short                D_L13_00162060 = 0x1e;
short                D_L13_00162064 = 0;
short                D_L13_00161FF4 = 0xfdb;
short                D_L13_00161FF8 = -10486;
short                D_L13_00162068 = 0;
f32                  D_L13_00162010 = 0.01f;
f32                  D_L13_00162014 = 0.2f;
f32                  D_L13_00162018 = 0.06f;
f32                  D_L13_0016201C = 0.4f;
s32                  D_L13_00162008 = 0x5a;
short                D_L14_001623D8 = 0x1e;
short                D_L14_001623DC = 0xa;
short                D_L14_001623E0 = 0x23;
short                D_L14_001623E8 = 0;
short                D_L14_001623EC = 0x55;
short                D_L14_001623F0 = 0x1e;
short                D_L14_001623F4 = 0x14;
short                D_L14_001623F8 = 0;
short                D_L14_001623FC = 0x2d;
short                D_L14_00162400 = 0x1e;
short                D_L14_00162404 = 0x14;
short                D_L14_00162408 = 0;
short                D_L14_0016240C = 0x1e;
short                D_L14_00162418 = 0x1e;
short                D_L14_0016241C = 0;
short                D_L14_001623AC = 0xfdb;
short                D_L14_001623B0 = -10486;
short                D_L14_00162420 = 0;
f32                  D_L14_001623C8 = 0.01f;
f32                  D_L14_001623CC = 0.2f;
f32                  D_L14_001623D0 = 0.06f;
f32                  D_L14_001623D4 = 0.4f;
s32                  D_L14_001623C0 = 0x5a;
short                D_L15_00162328 = 0x1e;
short                D_L15_0016232C = 0xa;
short                D_L15_00162330 = 0x23;
short                D_L15_00162338 = 0;
short                D_L15_0016233C = 0x55;
short                D_L15_00162340 = 0x1e;
short                D_L15_00162344 = 0x14;
short                D_L15_00162348 = 0;
short                D_L15_0016234C = 0x2d;
short                D_L15_00162350 = 0x1e;
short                D_L15_00162354 = 0x14;
short                D_L15_00162358 = 0;
short                D_L15_0016235C = 0x1e;
short                D_L15_00162368 = 0x1e;
short                D_L15_0016236C = 0;
short                D_L15_001622FC = 0xfdb;
short                D_L15_00162300 = -10486;
short                D_L15_00162370 = 0;
f32                  D_L15_00162318 = 0.01f;
f32                  D_L15_0016231C = 0.2f;
f32                  D_L15_00162320 = 0.06f;
f32                  D_L15_00162324 = 0.4f;
s32                  D_L15_00162310 = 0x5a;
short                D_L16_001620F0 = 0x1e;
short                D_L16_001620F4 = 0xa;
short                D_L16_001620F8 = 0x23;
short                D_L16_00162100 = 0;
short                D_L16_00162104 = 0x55;
short                D_L16_00162108 = 0x1e;
short                D_L16_0016210C = 0x14;
short                D_L16_00162110 = 0;
short                D_L16_00162114 = 0x2d;
short                D_L16_00162118 = 0x1e;
short                D_L16_0016211C = 0x14;
short                D_L16_00162120 = 0;
short                D_L16_00162124 = 0x1e;
short                D_L16_00162130 = 0x1e;
short                D_L16_00162134 = 0;
short                D_L16_001620C4 = 0xfdb;
short                D_L16_001620C8 = -10486;
short                D_L16_00162138 = 0;
f32                  D_L16_001620E0 = 0.01f;
f32                  D_L16_001620E4 = 0.2f;
f32                  D_L16_001620E8 = 0.06f;
f32                  D_L16_001620EC = 0.4f;
s32                  D_L16_001620D8 = 0x5a;
float D_L17_001677EC NOT_SDA = 0;
short                D_L17_001625D8 = 0x1e;
short                D_L17_001625DC = 0xa;
short                D_L17_001625E0 = 0x23;
short                D_L17_001625E8 = 0;
short                D_L17_001625EC = 0x55;
short                D_L17_001625F0 = 0x1e;
short                D_L17_001625F4 = 0x14;
short                D_L17_001625F8 = 0;
short                D_L17_001625FC = 0x2d;
short                D_L17_00162600 = 0x1e;
short                D_L17_00162604 = 0x14;
short                D_L17_00162608 = 0;
short                D_L17_0016260C = 0x1e;
short                D_L17_00162618 = 0x1e;
short                D_L17_0016261C = 0;
short                D_L17_001625AC = 0xfdb;
short                D_L17_001625B0 = -10486;
short                D_L17_00162620 = 0;
f32                  D_L17_001625C8 = 0.01f;
f32                  D_L17_001625CC = 0.2f;
f32                  D_L17_001625D0 = 0.06f;
f32                  D_L17_001625D4 = 0.4f;
s32                  D_L17_001625C0 = 0x5a;
float D_L18_001678EC NOT_SDA = 0;
short                D_L18_001626F0 = 0x1e;
short                D_L18_001626F4 = 0xa;
short                D_L18_001626F8 = 0x23;
short                D_L18_00162700 = 0;
short                D_L18_00162704 = 0x55;
short                D_L18_00162708 = 0x1e;
short                D_L18_0016270C = 0x14;
short                D_L18_00162710 = 0;
short                D_L18_00162714 = 0x2d;
short                D_L18_00162718 = 0x1e;
short                D_L18_0016271C = 0x14;
short                D_L18_00162720 = 0;
short                D_L18_00162724 = 0x1e;
short                D_L18_00162730 = 0x1e;
short                D_L18_00162734 = 0;
short                D_L18_001626C4 = 0xfdb;
short                D_L18_001626C8 = -10486;
short                D_L18_00162738 = 0;
f32                  D_L18_001626E0 = 0.01f;
f32                  D_L18_001626E4 = 0.2f;
f32                  D_L18_001626E8 = 0.06f;
f32                  D_L18_001626EC = 0.4f;
s32                  D_L18_001626D8 = 0x5a;

/* Copies the vector at D_0013F3D0 into b; a is unused. */
void follow_camera_hero_target(int a, void *b) __asm__("FUN_L00_002e4168");

void follow_camera_hero_target(int unused_camera, void *out_hero_position) {
    qcopy(out_hero_position, &hero.motion.pos);
}

int compute_follow_camera_radial_collision_positions(
    u128 *input_position, float radial_distance_extension) __asm__("FUN_L00_002e4180");

int compute_follow_camera_radial_collision_positions(u128 *input_position,

                                                     float radial_distance_extension) {
    u128  lookup_position_copy;
    u128  position_delta;
    char *selected_record;
    char *reference_position;
    char *extended_position;
    float selected_record_vector_length, inv;
    lookup_position_copy = *input_position;
    {
        int *lookup_result = (int *)FUN_L00_001ee1b0_002e4180(&lookup_position_copy);
        if (lookup_result == 0)
            goto fail;
        selected_record = (char *)&D_L00_001600F4_002e4180[lookup_result[5]];
    }
    reference_position = selected_record + 0x30;
    FUN_001f9a28_002e4180(&position_delta, input_position, reference_position);
    selected_record_vector_length = FUN_001f9af0_002e4180(selected_record);
    inv = 1.0f / FUN_001f9af0_002e4180(&position_delta);
    extended_position = (char *)&coll_hit_position;
    FUN_001f9a68_002e4180(extended_position,
                          &position_delta,
                          (selected_record_vector_length + radial_distance_extension) * inv);
    FUN_001f9a68_002e4180(
        extended_position - 0x10, &position_delta, selected_record_vector_length * inv);
    FUN_001f9a10_002e4180(extended_position, extended_position, reference_position);
    FUN_001f9a10_002e4180(extended_position - 0x10, extended_position - 0x10, reference_position);
    return 1;
fail:
    return 0;
}

/* True when FUN_L00_001ee1b0 returns non-zero for a copy of v and 0. */
int test_follow_camera_point_in_collision_volume(OvlQuad *input_quad) __asm__("FUN_L00_002e4290");

int test_follow_camera_point_in_collision_volume(OvlQuad *input_quad) {
    OvlQuad input_copy = *input_quad;
    return find_camera_collision_volume_for_point(&input_copy, 0.0f) != 0;
}

s32 compute_follow_camera_ground_aware_target_position(OvlQuad *out,
                                                       s32      flag) __asm__("FUN_L00_002e42c0");

s32 compute_follow_camera_ground_aware_target_position(OvlQuad *out_ground_aware_target_position, s32 flag) {
    OvlQuad      position_and_offset_vectors[4];
    struct Hero *player = &hero;
    u8          *camera_view = D_L00_00166E10_lui;
    f32          ground_distance;
    if (player->frames_off_ground.s == 0) {
        follow_camera_hero_target(*(s32 *)(camera_view - 0x10), out_ground_aware_target_position);
        return 1;
    }
    follow_camera_hero_target(*(s32 *)(camera_view - 0x10), &position_and_offset_vectors[0]);
    ground_distance = player->ground_distance;
    if (30.0f <= ground_distance || flag == 0) {
        FUN_001f9a68_002e42c0(&position_and_offset_vectors[3], camera_view + 0x30, 0.2f);
        FUN_001f9a10_002e42c0(&position_and_offset_vectors[1],
                              &position_and_offset_vectors[0],
                              &position_and_offset_vectors[3]);
        FUN_001f9a68_002e42c0(&position_and_offset_vectors[3], camera_view + 0x30, -30.0f);
        FUN_001f9a10_002e42c0(&position_and_offset_vectors[2],
                              &position_and_offset_vectors[0],
                              &position_and_offset_vectors[3]);
        if (FUN_001efa68_002e42c0(&position_and_offset_vectors[1],
                                  &position_and_offset_vectors[2],
                                  0x12,
                                  player->moby,
                                  0)) {
            qcopy(out_ground_aware_target_position, &D_L00_00173E60_002e42c0);
            return 0;
        }
        qcopy(out_ground_aware_target_position, &position_and_offset_vectors[0]);
        return -1;
    } else {
        FUN_001f9a68_002e42c0(
            &position_and_offset_vectors[1], camera_view + 0x30, -ground_distance);
        FUN_001f9a10_002e42c0(
            out_ground_aware_target_position, &position_and_offset_vectors[0], &position_and_offset_vectors[1]);
        return 1;
    }
}

void follow_camera_reset(char *o) __asm__("FUN_L00_002e4430");

void follow_camera_reset(char *camera) {
    V4_2e4430 target_position, target_up_projection, b;
    char     *g = D_L00_00166E10_002e4430;
    char     *camera_up_axis = g + 0x30;
    char     *camera_state = *(char **)(camera + 0x70);
    char     *attach_state, *r;
    compute_follow_camera_ground_aware_target_position(target_position, 0);
    attach_state = camera_state + 0x40;
    r = camera_state + 0x130;
    {
        float x = fast_vec_dot(target_position, camera_up_axis);
        FUN_001f9a68_002e4430(target_up_projection, camera_up_axis, x);
    }
    FUN_001f9a28_002e4430(attach_state, target_position, target_up_projection);
    qcopy(camera_state + 0x50, target_up_projection);
    FUN_001f9a10_002e4430(attach_state, attach_state, camera_state + 0x50);
    g += 0x20;
    FUN_001f9a68_002e4430(b, g, *(float *)(attach_state + 0xB0));
    FUN_001f9a10_002e4430(camera_state + 0x80, b, attach_state);
    FUN_001f9a10_002e4430(camera_state + 0xD0, b, attach_state);
    clear_u64_value(camera_state + 0xE0);
    attach_state[0xC4] = 0;
    *(float *)(attach_state + 0xC8) = 1.0f;
    attach_state[0xC5] = 0;
    *(s16 *)(attach_state + 0xC6) = 0;
    qcopy(camera_state + 0xA0, attach_state);
    clear_u64_value(camera_state + 0xB0);
    clear_u64_value(camera_state + 0xC0);
    FUN_001f9a68_002e4430(b, g, *(float *)(r + 0x30));
    FUN_001f9a10_002e4430(camera_state + 0x90, b, attach_state);
}

/* angle between a direction and an axis, signed */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002E5A30), where it is exact; names translated to the US level program. */
float compute_follow_camera_signed_target_angle(char *camera, float target_offset_scale) __asm__("FUN_L00_002e4580");

float compute_follow_camera_signed_target_angle(char *camera, float target_offset_scale) {
    float hero_position[4], offset_target_position[4], angle_direction[4], axis_projection[4];
    float reference_dot_or_signed_angle, axis_dot_or_direction_length;
    char *camera_view = D_L00_00166E10;
    follow_camera_hero_target(camera, hero_position);
    FUN_001f9a68_c3(offset_target_position, camera_view + 0x20, target_offset_scale);
    FUN_001f9a10_c2(offset_target_position, offset_target_position, hero_position);
    fast_vec_sub(angle_direction, offset_target_position, (float *)(camera + 0x30));
    axis_dot_or_direction_length = fast_vec_dot(angle_direction, camera + 0x10);
    FUN_001f9a68_c3(axis_projection, camera + 0x10, axis_dot_or_direction_length);
    fast_vec_sub(angle_direction, angle_direction, axis_projection);
    reference_dot_or_signed_angle = fast_vec_dot(angle_direction, camera);
    axis_dot_or_direction_length = fast_vec_length(angle_direction);
    if (axis_dot_or_direction_length == 0.0f) {
        return 0.0f;
    }
    reference_dot_or_signed_angle =
        1.5707964f - fast_arc_sin(reference_dot_or_signed_angle / axis_dot_or_direction_length);
    if (fast_vec_dot(angle_direction, camera + 0x20) < 0.0f) {
        reference_dot_or_signed_angle = -reference_dot_or_signed_angle;
    }
    return reference_dot_or_signed_angle;
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002E5B68), where it is exact; names translated to the US level program. */
void apply_follow_camera_translation(char *camera) __asm__("FUN_L00_002e46b8");

void apply_follow_camera_translation(char *camera) {
    Vq4   translation_delta;
    Vq4   camera_translation_adjustment, lateral_displacement;
    char *camera_view = D_L00_00166E10;
    char *camera_state = *(char **)(camera + 0x70);
    char *attach_state = camera_state + 0x40;
    char *ground_check_hero, *state_check_hero;
    qcopy(&translation_delta, D_0013E533 + 0xF5D);
    FUN_001f9a10_c(camera + 0x30, &translation_delta, camera + 0x30);
    FUN_001f9a10_c(camera_state + 0xA0, &translation_delta, camera_state + 0xA0);
    FUN_001f9a10_c(attach_state, &translation_delta, attach_state);
    FUN_001f9a10_c(camera_state + 0xD0, &translation_delta, camera_state + 0xD0);
    FUN_001f9a10_c(camera_state, &translation_delta, camera_state);
    ground_check_hero = D_0013E533 + 0xE1D;
    if (*(short *)(ground_check_hero + 0x308) == 0) {
        char *ground_moby = *(char **)(ground_check_hero + 0x2FC);
        char *camera_system_state;
        if (ground_moby == 0 || MOBY(ground_moby)->oclass != 0x22D ||
            ((camera_system_state = camera_view - 0x190), FUN_001f96f8_c(0x1E)) <
                *(int *)(camera_system_state + 0x398)) {
            char *translation_source_hero = D_0013E533 + 0xE1D;
            *(float *)(*(char **)(camera + 0x70) + 0x1C8) =
                *(float *)(translation_source_hero + 0x14C);
        }
    }
    state_check_hero = D_0013E533 + 0xE1D;
    if ((unsigned)(*(int *)(state_check_hero + 0x2084) - 0xB) < 2) {
        char *reloaded_camera_state = *(char **)(camera + 0x70);
        char *camera_up_axis = camera_view + 0x30;
        float translation_up_component;
        *(short *)(reloaded_camera_state + 0x10) = 0;
        FUN_001f9a68_c(
            &camera_translation_adjustment, camera_view + 0x80, *(float *)(camera_view + 0xA4));
        translation_up_component = fast_vec_dot(camera_up_axis, &translation_delta);
        FUN_001f9a68_c(&lateral_displacement, camera_up_axis, translation_up_component);
        fast_vec_sub((float *)&lateral_displacement,
                     (float *)&translation_delta,
                     (float *)&lateral_displacement);
        fast_vec_sub((float *)&camera_translation_adjustment,
                     (float *)&camera_translation_adjustment,
                     (float *)&lateral_displacement);
        FUN_001f9a10_c(camera + 0x30, &camera_translation_adjustment, camera + 0x30);
        FUN_001f9a10_c(attach_state + 0x60, &camera_translation_adjustment, attach_state + 0x60);
        FUN_001f9a10_c(attach_state, &camera_translation_adjustment, attach_state);
        FUN_001f9a10_c(attach_state + 0x90, &camera_translation_adjustment, attach_state + 0x90);
        FUN_001f9a10_c(
            reloaded_camera_state, &camera_translation_adjustment, reloaded_camera_state);
    }
}

void compute_follow_camera_offset_target_up_projection(void *out) __asm__("FUN_L00_002e48a0");

void compute_follow_camera_offset_target_up_projection(void *out_target_up_projection) {
    V4    offset_target_position, hero_position;
    char *camera_view = D_L00_00166E10;
    vcopy_2e48a0(offset_target_position, camera_view + 0x20);
    follow_camera_hero_target(*(int *)(camera_view - 0x10), hero_position);
    fast_vec_add(offset_target_position, offset_target_position, hero_position);
    fast_vec_scale(
        out_target_up_projection, camera_view + 0x30, fast_vec_dot(camera_view + 0x30, offset_target_position));
}

/* camera attach mode transitions: picks the next state from the player's state and arms its timers */
/* Migrated from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002E5DE0). */
void FUN_L00_002e4930(char *camera) {
    char      *camera_state = *(char **)(camera + 0x70);
    CamAttach *attach_state = (CamAttach *)(camera_state + 0x40);
    char      *timer_camera_state;
    float      inv, x, y;
    int        camera_timer_base_frames;

    if (attach_state->state == 0) {
        struct Hero *player = &hero;
        if (player->state.control_mode == 2) {
            attach_state->prev = attach_state->state;
            attach_state->state = 1;
            attach_state->timer = D_L00_00161D3C_gp;
            attach_state->invTimer = 1.0f / convert_integer_to_float(attach_state->timer);
        } else {
            struct Hero *current_player = &hero;
            if (current_player->state.current == 0xF && D_L00_00166EEC == 0.0f) {
                attach_state->prev = attach_state->state;
                attach_state->state = 3;
                attach_state->moveTimer = scale_game_frames(
                    *(int *)&follow_camera_project_hero_focus_blend_base_frames_gp);
                attach_state->invMove = 1.0f / convert_integer_to_float(attach_state->moveTimer);
                attach_state->timer = follow_camera_focus_blend_transition_frames_gp;
                inv = 1.0f / convert_integer_to_float(attach_state->timer);
                attach_state->dist = *(float *)&follow_camera_project_hero_focus_offset_gp;
                attach_state->hit = 0;
                x = *(float *)&follow_camera_project_hero_focus_offset_gp;
                attach_state->invTimer = inv;
                FUN_L00_002e48a0_c(attach_state->v30, x);
                qcopy(attach_state->v20, attach_state->v10);
            } else {
                struct Hero *state_check_player = &hero;
                if (state_check_player->state.current == 0xC && D_L00_00166EEC == 0.0f) {
                    attach_state->prev = attach_state->state;
                    attach_state->state = 8;
                    attach_state->timer = follow_camera_hold_focus_transition_frames_gp;
                    attach_state->invTimer = 1.0f / convert_integer_to_float(attach_state->timer);
                } else {
                    struct Hero *hero_state_player = &hero;
                    if (hero_state_player->state.current == 0xB && D_L00_00166EEC == 0.0f) {
                        attach_state->prev = attach_state->state;
                        attach_state->state = 6;
                        attach_state->moveTimer = scale_game_frames(*(int *)&D_L00_00161D40_gp);
                        attach_state->invMove =
                            1.0f / convert_integer_to_float(attach_state->moveTimer);
                        attach_state->timer = follow_camera_focus_blend_transition_frames_gp;
                        inv = 1.0f / convert_integer_to_float(attach_state->timer);
                        attach_state->dist = *(float *)&follow_camera_blend_out_focus_offset_gp;
                        attach_state->hit = 0;
                        x = *(float *)&follow_camera_blend_out_focus_offset_gp;
                        attach_state->invTimer = inv;
                        FUN_L00_002e48a0_c(attach_state->v30, x);
                        qcopy(attach_state->v20, attach_state->v10);
                    } else {
                        struct Hero *state_transition_player = &hero;
                        if ((state_transition_player->state.current == 0xD ||
                             state_transition_player->state.current == 0xE) &&
                            D_L00_00166EEC == 0.0f) {
                            attach_state->prev = attach_state->state;
                            attach_state->state = 5;
                            attach_state->moveTimer = scale_game_frames(
                                *(int *)&follow_camera_hold_focus_blend_base_frames_gp);
                            attach_state->invMove =
                                1.0f / convert_integer_to_float(attach_state->moveTimer);
                            attach_state->timer = follow_camera_hold_focus_transition_frames_gp;
                            inv = 1.0f / convert_integer_to_float(attach_state->timer);
                            attach_state->dist =
                                *(float *)&follow_camera_hold_focus_blend_offset_gp;
                            attach_state->hit = 0;
                            x = *(float *)&follow_camera_hold_focus_blend_offset_gp;
                            attach_state->invTimer = inv;
                            FUN_L00_002e48a0_c(attach_state->v30, x);
                            qcopy(attach_state->v20, attach_state->v10);
                        } else {
                            struct Hero *control_mode_player = &hero;
                            if (control_mode_player->state.control_mode == 4 &&
                                D_L00_00166EEC == 0.0f) {
                                attach_state->prev = attach_state->state;
                                attach_state->state = 2;
                                attach_state->timer =
                                    follow_camera_ground_aware_focus_transition_frames_gp;
                                attach_state->invTimer =
                                    1.0f / convert_integer_to_float(attach_state->timer);
                            }
                        }
                    }
                }
            }
        }
        return;
    }
    if (attach_state->state != 9 && attach_state->state != 1 && camera_secondary_mode_lui == 0x14) {
        float r;
        attach_state->prev = attach_state->state;
        attach_state->state = 9;
        attach_state->moveTimer = scale_game_frames(*(int *)&D_L00_00161D6C_gp);
        r = 1.0f / convert_integer_to_float(attach_state->moveTimer);
        y = *(float *)&follow_camera_projected_focus_blend_offset_gp;
        attach_state->hit = 0;
        attach_state->timer = 0;
        x = *(float *)&follow_camera_projected_focus_blend_offset_gp;
        attach_state->invTimer = 1.0f;
        attach_state->dist = y;
        attach_state->invMove = r;
        FUN_L00_002e48a0_c(attach_state->v30, x);
        qcopy(attach_state->v20, attach_state->v10);
        return;
    }
    if ((attach_state->state == 1 || attach_state->state == 2) && D_001413DC != 4 &&
        D_001413DC != 2) {
        attach_state->timer = 0;
        attach_state->invTimer = 1.0f;
        attach_state->state = 0;
        return;
    }
    if (attach_state->state == 7) {
        struct Hero *p = &hero;
        if (p->past_jump_apex == 0) {
            attach_state->prev = attach_state->state;
            attach_state->state = 6;
            attach_state->moveTimer = scale_game_frames(*(int *)&D_L00_00161D40_gp);
            attach_state->invMove = 1.0f / convert_integer_to_float(attach_state->moveTimer);
            attach_state->timer = follow_camera_focus_blend_transition_frames_gp;
            inv = 1.0f / convert_integer_to_float(attach_state->timer);
            attach_state->dist = *(float *)&follow_camera_blend_out_focus_offset_gp;
            attach_state->hit = 0;
            x = *(float *)&follow_camera_blend_out_focus_offset_gp;
            attach_state->invTimer = inv;
            FUN_L00_002e48a0_c(attach_state->v30, x);
            qcopy(attach_state->v20, attach_state->v10);
            return;
        }
        if (p->state.control_mode == 2) {
            attach_state->prev = attach_state->state;
            attach_state->state = 1;
            attach_state->timer = scale_game_frames(*(int *)&D_L00_00161D54_gp);
            attach_state->invTimer = 1.0f / convert_integer_to_float(attach_state->timer);
            return;
        }
    }
    if (attach_state->state == 4 && D_001413DC != 4) {
        if (D_001413DC == 2) {
            attach_state->prev = 4;
            attach_state->state = 1;
            attach_state->timer = scale_game_frames(*(int *)&D_L00_00161D54_gp);
            attach_state->invTimer = 1.0f / convert_integer_to_float(attach_state->timer);
        } else {
            attach_state->prev = 4;
            attach_state->state = 0;
            attach_state->timer = scale_game_frames(*(int *)&D_L00_00161D54_gp);
            attach_state->invTimer = 1.0f / convert_integer_to_float(attach_state->timer);
        }
        return;
    }
    if (attach_state->state == 3 && camera_secondary_mode_lui == 0xB) {
        attach_state->prev = attach_state->state;
        attach_state->state = 6;
        attach_state->moveTimer = scale_game_frames(*(int *)&D_L00_00161D40_gp);
        attach_state->invMove = 1.0f / convert_integer_to_float(attach_state->moveTimer);
        attach_state->timer = follow_camera_focus_blend_transition_frames_gp;
        inv = 1.0f / convert_integer_to_float(attach_state->timer);
        attach_state->dist = *(float *)&follow_camera_blend_out_focus_offset_gp;
        attach_state->hit = 0;
        x = *(float *)&follow_camera_blend_out_focus_offset_gp;
        attach_state->invTimer = inv;
        FUN_L00_002e48a0_c(attach_state->v30, x);
        qcopy(attach_state->v20, attach_state->v10);
        return;
    }
    if (attach_state->state == 5 && camera_secondary_mode_lui == 0xC) {
        attach_state->prev = attach_state->state;
        attach_state->state = 8;
        attach_state->timer = follow_camera_hold_focus_transition_frames_gp;
        attach_state->invTimer = 1.0f / convert_integer_to_float(attach_state->timer);
        return;
    }
    if (attach_state->state == 8 && camera_secondary_mode_lui != 0xC) {
        attach_state->prev = attach_state->state;
        attach_state->state = 0;
        attach_state->timer =
            scale_game_frames(*(int *)&follow_camera_hold_focus_exit_base_frames_gp);
        attach_state->invTimer = 1.0f / convert_integer_to_float(attach_state->timer);
        return;
    }
    if (attach_state->state == 6 && camera_secondary_mode_lui != 0xB) {
        attach_state->prev = attach_state->state;
        attach_state->state = 0;
        attach_state->timer = scale_game_frames(*(int *)&D_L00_00161D54_gp);
        attach_state->invTimer = 1.0f / convert_integer_to_float(attach_state->timer);
        return;
    }
    if ((attach_state->state == 3 || attach_state->state == 5) && D_001413DC != 4) {
        attach_state->prev = attach_state->state;
        attach_state->state = 0;
        attach_state->timer = scale_game_frames(*(int *)&D_L00_00161D54_gp);
        attach_state->invTimer = 1.0f / convert_integer_to_float(attach_state->timer);
        return;
    }
    if (attach_state->state == 9) {
        struct Hero *p = &hero;
        if (p->state.control_mode == 2 || p->frames_off_ground.s >= scale_game_frames(0x37)) {
            goto to_one;
        }
    }
    if (attach_state->state == 0xA) {
        struct Hero *p = &hero;
        if (p->state.control_mode == 2 || p->frames_off_ground.s >= scale_game_frames(0x37)) {
            float r;
        to_one:
            attach_state->prev = attach_state->state;
            attach_state->state = 1;
            attach_state->timer = scale_game_frames(*(int *)&D_L00_00161D54_gp);
            r = 1.0f / convert_integer_to_float(attach_state->timer);
            camera_timer_base_frames = *(int *)&D_L00_00161D24_gp;
            attach_state->invTimer = r;
            timer_camera_state = *(char **)(camera + 0x70);
            *(short *)(timer_camera_state + 0x20) = scale_game_frames(camera_timer_base_frames);
            qcopy(camera + 0x40, camera);
            return;
        }
    }
    if (attach_state->state == 9 && camera_secondary_mode_lui != 0x14) {
        attach_state->prev = attach_state->state;
        attach_state->state = 0xA;
        attach_state->timer = follow_camera_continue_projected_focus_transition_frames_gp;
        attach_state->invTimer = 1.0f / convert_integer_to_float(attach_state->timer);
        return;
    }
    if (attach_state->state == 0xA && FUN_001f9770_c2(&attach_state->moveTimer)) {
        attach_state->prev = attach_state->state;
        attach_state->state = 0;
        attach_state->timer = scale_game_frames(*(int *)&D_L00_00161D54_gp);
        attach_state->invTimer = 1.0f / convert_integer_to_float(attach_state->timer);
        return;
    }
    if (attach_state->state == 5 && camera_secondary_mode_lui == 0x10) {
        attach_state->prev = attach_state->state;
        attach_state->state = 0;
        attach_state->timer =
            scale_game_frames(*(int *)&follow_camera_hold_focus_exit_base_frames_gp);
        attach_state->invTimer = 1.0f / convert_integer_to_float(attach_state->timer);
    }
}

/* per-frame state machine that eases a moby's attach pose toward the camera block's vector */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002E6498), where it is exact; names translated to the US level program. */
void FUN_L00_002e4fe8(char *camera, float *out_focus_velocity) {
    float        hero_position[4];
    float        target_position_or_up_offset[4];
    char        *camera_state;
    char        *attach_state;
    char        *camera_view;
    struct Hero *pad;
    struct Hero *padb;
    struct Hero *padc;
    char        *timer_camera_state;
    float        x, y;
    int          flag;
    camera_view = D_L00_00166E10;
    camera_state = *(char **)(camera + 0x70);
    follow_camera_hero_target(camera, hero_position);
    pad = &hero;
    attach_state = camera_state + 0x40;
    if (pad->state.current == 0x77) {
        x = fast_vec_dot(attach_state, camera_view + 0x30);
        FUN_001f9a68_c2(out_focus_velocity, camera_view + 0x30, x);
        fast_vec_sub(attach_state, attach_state, out_focus_velocity);
        y = fast_vec_dot(camera_view + 0x30, camera_state + 0xA0);
        FUN_001f9a68_c2(out_focus_velocity, camera_view + 0x30, y);
        qcopy(camera_state + 0x50, out_focus_velocity);
        if (*(unsigned char *)(attach_state + 0xC4) != 0xB) {
            *(unsigned char *)(attach_state + 0xC4) = 0xB;
            qcopy(camera + 0x40, camera);
            timer_camera_state = *(char **)(camera + 0x70);
            *(short *)(timer_camera_state + 0x20) = FUN_001f96f8_c2(*(int *)&D_L00_00161D24_gp);
        }
        return;
    }
    switch (*(unsigned char *)(attach_state + 0xC4)) {
    case CAM_ATTACH_GROUND_AWARE_FOCUS_VELOCITY:
        padb = &hero;
        if (padb->state.current == 0x11 ||
            (padb->past_jump_apex != 0 && padb->ground_distance > 0.2f &&
             ((y = compute_follow_camera_signed_target_angle(camera,
                                                             *(float *)(attach_state + 0xB0))) <
                  *(float *)&follow_camera_ground_focus_angle_lower_bound_gp ||
              *(float *)&follow_camera_ground_focus_angle_upper_bound_gp < y))) {
            int v;
            *(unsigned char *)(attach_state + 0xC4) = 1;
            v = *(unsigned short *)&D_L00_00161D3C_gp;
            *(short *)(attach_state + 0xC6) = v;
            *(float *)(attach_state + 0xC8) = 1.0f / convert_integer_to_float((short)v);
        }
        padc = &hero;
        flag = 0;
        if (padc->frames_off_ground.s != 0 && *(float *)(camera_view + 0xDC) == 0.0f &&
            (padc->past_jump_apex == 0 || padc->frames_to_land >= 0x10)) {
            flag = 1;
        } else {
            if (compute_follow_camera_ground_aware_target_position(target_position_or_up_offset,
                                                                   1) == 1) {
                x = fast_vec_dot(target_position_or_up_offset, camera_view + 0x30);
                if (x < fast_vec_length(attach_state + 0x10))
                    flag = 1;
            } else {
                flag = 1;
            }
        }
        if (flag != 0) {
            if (absolute_float(*(float *)(camera_view + 0x30) - *(float *)(camera_view + 0x40)) >
                    0.001f ||
                absolute_float(*(float *)(camera_view + 0x34) - *(float *)(camera_view + 0x44)) >
                    0.001f ||
                absolute_float(*(float *)(camera_view + 0x38) - *(float *)(camera_view + 0x48)) >
                    0.001f) {
                x = fast_vec_dot(hero_position, camera_view + 0x30);
                FUN_001f9a68_c2(out_focus_velocity, camera_view + 0x30, x);
                fast_vec_sub(attach_state, hero_position, out_focus_velocity);
            } else {
                qcopy(out_focus_velocity, attach_state + 0x10);
                x = fast_vec_dot(hero_position, camera_view + 0x30);
                FUN_001f9a68_c2(target_position_or_up_offset, camera_view + 0x30, x);
                fast_vec_sub(attach_state, hero_position, target_position_or_up_offset);
            }
        } else {
            compute_follow_camera_ground_aware_target_position(target_position_or_up_offset, 1);
            x = fast_vec_dot(target_position_or_up_offset, camera_view + 0x30);
            FUN_001f9a68_c2(out_focus_velocity, camera_view + 0x30, x);
            fast_vec_sub(attach_state, target_position_or_up_offset, out_focus_velocity);
        }
        break;
    case CAM_ATTACH_BLEND_TO_PROJECT_HERO_FOCUS_VELOCITY:
    case CAM_ATTACH_BLEND_FOCUS_VELOCITY_WITH_HOLD_TRANSITION:
    case CAM_ATTACH_BLEND_OUT_FOCUS_VELOCITY:
    case CAM_ATTACH_BLEND_BACK_FOCUS_VELOCITY:
        lerp_vector_by((float)*(short *)(attach_state + 0xD4) * *(float *)(attach_state + 0xCC),
                       attach_state + 0x10,
                       attach_state + 0x30,
                       attach_state + 0x20);
        if (*(unsigned char *)(attach_state + 0xD7) == 0) {
            if (coll_sphere(0.2f, camera + 0x30, 0x12, 0)) {
                qcopy(attach_state + 0x30, attach_state + 0x10);
                *(unsigned char *)(attach_state + 0xD7) = 1;
            }
        }
        qcopy(out_focus_velocity, attach_state + 0x10);
        x = fast_vec_dot(hero_position, camera_view + 0x30);
        FUN_001f9a68_c2(target_position_or_up_offset, camera_view + 0x30, x);
        fast_vec_sub(attach_state, hero_position, target_position_or_up_offset);
        if (FUN_001f9770_c(attach_state + 0xD4)) {
            if (*(unsigned char *)(attach_state + 0xC4) == 9) {
                *(unsigned char *)(attach_state + 0xC4) = 0xA;
                *(short *)(attach_state + 0xD4) = FUN_001f96f8_c2(*(int *)&D_L00_00161D6C_gp);
                qcopy(attach_state + 0x20, attach_state + 0x10);
                FUN_001f9a68_c2(target_position_or_up_offset,
                                camera_view + 0x30,
                                *(float *)(attach_state + 0xD0));
                fast_vec_sub(
                    attach_state + 0x30, attach_state + 0x20, target_position_or_up_offset);
            } else if (*(unsigned char *)(attach_state + 0xC4) == 6) {
                *(unsigned char *)(attach_state + 0xC4) = 7;
                *(short *)(attach_state + 0xD4) = FUN_001f96f8_c2(*(int *)&D_L00_00161D40_gp);
                qcopy(attach_state + 0x30, attach_state + 0x20);
                qcopy(attach_state + 0x20, attach_state + 0x10);
            } else {
                int v;
                *(unsigned char *)(attach_state + 0xC4) = 4;
                v = *(unsigned short *)&D_L00_00161D54_gp;
                *(short *)(attach_state + 0xC6) = v;
                *(float *)(attach_state + 0xC8) = 1.0f / convert_integer_to_float((short)v);
            }
        }
        break;
    case CAM_ATTACH_HOLD_FOCUS_VELOCITY:
        qcopy(out_focus_velocity, attach_state + 0x10);
        x = fast_vec_dot(hero_position, camera_view + 0x30);
        FUN_001f9a68_c2(target_position_or_up_offset, camera_view + 0x30, x);
        fast_vec_sub(attach_state, hero_position, target_position_or_up_offset);
        break;
    case CAM_ATTACH_BLEND_PROJECTED_HERO_FOCUS_VELOCITY:
    case CAM_ATTACH_CONTINUE_BLEND_PROJECTED_HERO_FOCUS_VELOCITY:
        x = fast_vec_dot(hero_position, camera_view + 0x30);
        FUN_001f9a68_c2(out_focus_velocity, camera_view + 0x30, x);
        fast_vec_sub(attach_state, hero_position, out_focus_velocity);
        lerp_vector_by(*(float *)&follow_camera_projected_focus_blend_fraction_gp,
                       out_focus_velocity,
                       out_focus_velocity,
                       attach_state + 0x20);
        break;
    case CAM_ATTACH_PROJECT_HERO_FOCUS_VELOCITY:
    default:
        x = fast_vec_dot(hero_position, camera_view + 0x30);
        FUN_001f9a68_c2(out_focus_velocity, camera_view + 0x30, x);
        fast_vec_sub(attach_state, hero_position, out_focus_velocity);
        break;
    }
}

void interpolate_follow_camera_position(void *mm,
                                        void *vv,
                                        void *tt,
                                        f32   stiffness,
                                        f32   damping,
                                        f32   limit) __asm__("FUN_L00_002e55b8");

void interpolate_follow_camera_position(

    void *mm, void *vv, void *tt, f32 stiffness, f32 damping, f32 limit) {
    u8  *camera_state = *(u8 **)((u8 *)mm + 0x70);
    f32 *current_values = vv;
    f32 *target_values = tt;
    current_values[0] = cam_interp_values(
        camera_state + 0xE0, current_values[0], target_values[0], stiffness, damping, limit);
    current_values[1] = cam_interp_values(
        camera_state + 0xE4, current_values[1], target_values[1], stiffness, damping, limit);
    current_values[2] = cam_interp_values(
        camera_state + 0xE8, current_values[2], target_values[2], stiffness, damping, limit);
}

s32 compute_follow_camera_target_position(void *mm,
                                          void *input_offset,
                                          void *out_target_position,
                                          s32   k) __asm__("FUN_L00_002e5678");

s32 compute_follow_camera_target_position(void *mm,
                                          void *input_offset,
                                          void *out_target_position,

                                          s32 k) {
    u8 *camera = mm;
    s32 attach_mode_return_flag = 1;
    u8 *stored_focus_position = *(u8 **)(camera + 0x70) + 0x40;
    f32 hero_base_position[4] __attribute__((aligned(16)));
    switch (k) {
    case CAM_ATTACH_BLEND_TO_PROJECT_HERO_FOCUS_VELOCITY:
    case CAM_ATTACH_PROJECT_HERO_FOCUS_VELOCITY:
        fast_vec_add(out_target_position, input_offset, stored_focus_position);
        attach_mode_return_flag = 0;
        break;
    case CAM_ATTACH_BLEND_FOCUS_VELOCITY_WITH_HOLD_TRANSITION:
    case CAM_ATTACH_BLEND_OUT_FOCUS_VELOCITY:
    case CAM_ATTACH_BLEND_BACK_FOCUS_VELOCITY:
        fast_vec_add(out_target_position, input_offset, stored_focus_position);
        attach_mode_return_flag = 0;
        break;
    case CAM_ATTACH_HERO_POSITION_BASE:
        follow_camera_hero_target((int)camera, hero_base_position);
        fast_vec_add(out_target_position, input_offset, hero_base_position);
        break;
    case CAM_ATTACH_GROUND_AWARE_FOCUS_VELOCITY:
    default:
        fast_vec_add(out_target_position, input_offset, stored_focus_position);
        break;
    }
    return attach_mode_return_flag;
}

void follow_camera_transition(O002e5730 *o, void *a) __asm__("FUN_L00_002e5730");

void follow_camera_transition(O002e5730 *camera, void *input_offset) {
    V002e5730  target_position;
    u8        *m = camera->m;
    A002e5730 *attach_state = (A002e5730 *)(m + 0x40);
    f32        k, x, y;
    u8         attach_mode;
    if (fast_dec_timer_s16(m + 0x106))
        attach_state->xc5 = attach_state->xc4;
    k = (f32)attach_state->xc6 * attach_state->xc8;
    compute_follow_camera_target_position(
        camera, input_offset, &target_position, attach_state->xc4);
    attach_mode = attach_state->xc4;
    if (attach_mode == 3 || attach_mode == 5 || attach_mode == 6 || attach_mode == 7 ||
        attach_mode == 1) {
        x = follow_camera_position_transition_end_stiffness_gp +
            (follow_camera_position_default_stiffness_gp -
             follow_camera_position_transition_end_stiffness_gp) *
                k;
        y = follow_camera_position_transition_end_damping_gp +
            (follow_camera_position_default_damping_gp -
             follow_camera_position_transition_end_damping_gp) *
                k;
    } else {
        x = follow_camera_position_default_stiffness_gp;
        y = follow_camera_position_default_damping_gp;
    }
    interpolate_follow_camera_position(camera, attach_state->x90, &target_position, x, y, 0.0f);
}

/* Updates the follow camera: eases its focus and offset vectors, then places
   the eye behind the hero, pulled in where collision lines hit. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002E6CE0); names and data translated to the US level program. */
void follow_camera_springs(void *cam) __asm__("FUN_L00_002e5830");

void follow_camera_springs(void *camera_moby) {
    f32 velocity[4], hero_pos[4], focus[4], up[4], lateral[4], look[4], base[4], probe[4], left[4],
        collision_resolution_delta[4], mid[4];
    u8             *view = camera_view;
    FollowCamState *camera_state = FOLLOW_CAM_STATE((struct UpdateCam *)camera_moby);
    FollowCamTail  *camera_tail = &camera_state->tail;
    FollowCamBlock *follow_block = &camera_state->follow;
    f32             up_projection_or_collision_clearance, back;

    follow_camera_hero_target(camera_moby, hero_pos);
    follow_cam_update_mode(camera_moby);
    follow_cam_focus_velocity(camera_moby, velocity);
    qcopy(&camera_state->follow.velocity, velocity);
    qcopy(focus, follow_block);
    up_projection_or_collision_clearance = fast_vec_dot(view + 0x30, camera_state->follow.offset);
    scale_vector_xyz(up, view + 0x30, up_projection_or_collision_clearance);
    fast_vec_sub(lateral, camera_state->follow.offset, up);
    up[0] = cam_interp_values(&camera_state->follow.up_ease[0],
                              up[0],
                              follow_block->velocity[0],
                              follow_block->up_stiffness,
                              follow_block->up_damping,
                              0.0f);
    up[1] = cam_interp_values(&camera_state->follow.up_ease[1],
                              up[1],
                              follow_block->velocity[1],
                              follow_block->up_stiffness,
                              follow_block->up_damping,
                              0.0f);
    up[2] = cam_interp_values(&camera_state->follow.up_ease[2],
                              up[2],
                              follow_block->velocity[2],
                              follow_block->up_stiffness,
                              follow_block->up_damping,
                              0.0f);
    lateral[0] = cam_interp_values(&camera_state->follow.lateral_ease[0],
                                   lateral[0],
                                   follow_block->focus[0],
                                   follow_block->lateral_stiffness,
                                   follow_block->lateral_damping,
                                   0.0f);
    lateral[1] = cam_interp_values(&camera_state->follow.lateral_ease[1],
                                   lateral[1],
                                   follow_block->focus[1],
                                   follow_block->lateral_stiffness,
                                   follow_block->lateral_damping,
                                   0.0f);
    lateral[2] = cam_interp_values(&camera_state->follow.lateral_ease[2],
                                   lateral[2],
                                   follow_block->focus[2],
                                   follow_block->lateral_stiffness,
                                   follow_block->lateral_damping,
                                   0.0f);
    fast_vec_add(follow_block, follow_block, camera_state->follow.velocity);
    fast_vec_add(camera_state->follow.offset, lateral, up);
    scale_vector_xyz(look, view + 0x20, follow_block->look_height);
    if (follow_block->blending_in) {
        if (++follow_block->blend_timer > scale_game_frames(follow_cam_blend_frames)) {
            follow_block->blend_timer = scale_game_frames(follow_cam_blend_frames);
        }
        lerp_vector_by((f32)follow_block->blend_timer /
                           convert_integer_to_float(scale_game_frames(follow_cam_blend_frames)),
                       base,
                       follow_block,
                       camera_state->follow.offset);
        fast_vec_add(camera_state->follow.look_target, look, base);
    } else {
        fast_dec_timer_s16(&camera_state->follow.blend_timer);
        if (follow_block->blend_timer < 0) {
            follow_block->blend_timer = 0;
        }
        lerp_vector_by((f32)follow_block->blend_timer /
                           convert_integer_to_float(scale_game_frames(follow_cam_blend_frames)),
                       base,
                       follow_block,
                       camera_state->follow.offset);
        fast_vec_add(camera_state->follow.look_target, look, base);
    }
    follow_camera_transition(camera_moby, look);
    if (hero.vehicle_mode == 2) {
        scale_vector_xyz(look, view + 0x20, 3.0f);
    } else if (hero.vehicle_mode == 1) {
        scale_vector_xyz(look, view + 0x20, 0.4f);
    } else {
        scale_vector_xyz(look, view + 0x20, 0.5f);
    }
    fast_vec_add(base, hero_pos, look);
    scale_vector_xyz(look, view + 0x20, camera_tail->distance);
    fast_vec_add(follow_block->eye, look, follow_block);
    up_projection_or_collision_clearance =
        follow_cam_min_distance_gp + FOLLOW_CAM_STATE(current_follow_cam)->tail.zoom_range *
                                         FOLLOW_CAM_STATE(current_follow_cam)->tail.zoom_scale *
                                         (FOLLOW_CAM_STATE(current_follow_cam)->tail.zoom - 1.0f);
    scale_vector_xyz(probe, view + 0x20, 20.0f);
    fast_vec_add(probe, base, probe);
    if (collision_line(base, probe, camera_collision_mask, (s32)hero.moby, 0)) {
        qcopy(left, collision_hit_point);
    } else {
        qcopy(left, probe);
    }
    scale_vector_xyz(probe, view + 0x20, 20.0f);
    fast_vec_sub(probe, base, probe);
    if (collision_line(base, probe, camera_collision_mask, (s32)hero.moby, 0)) {
        qcopy(probe, collision_hit_point);
    }
    fast_vec_sub(collision_resolution_delta, probe, left);
    if (fast_vec_length(collision_resolution_delta) <
        up_projection_or_collision_clearance + up_projection_or_collision_clearance) {
        fast_vec_add(mid, left, probe);
        scale_vector_xyz(mid, mid, 0.5f);
        qcopy(follow_block->eye, mid);
        fast_vec_sub(collision_resolution_delta, mid, follow_block);
        follow_camera_set_height(fast_vec_dot(collision_resolution_delta, view + 0x20), 0.003f);
    } else {
        f32 reach;
        fast_vec_sub(collision_resolution_delta, left, follow_block);
        back = fast_vec_length(collision_resolution_delta);
        reach = camera_tail->distance;
        if (back <= reach + up_projection_or_collision_clearance) {
            scale_vector_xyz(mid, view + 0x20, up_projection_or_collision_clearance);
            fast_vec_sub(mid, left, mid);
            qcopy(follow_block->eye, mid);
            fast_vec_sub(collision_resolution_delta, mid, follow_block);
            follow_camera_set_height(fast_vec_dot(collision_resolution_delta, view + 0x20), 0.003f);
        } else {
            up_projection_or_collision_clearance = fast_vec_dot(follow_block, view + 0x30);
            if (fast_vec_dot(probe, view + 0x30) < up_projection_or_collision_clearance) {
                qcopy(probe, follow_block);
            }
            scale_vector_xyz(look, view + 0x20, reach);
            fast_vec_add(follow_block->eye, look, probe);
        }
    }
}

float D_L00_00166EEC NOT_SDA = 0;
short follow_camera_ground_aware_focus_transition_frames_gp __asm__("D_L00_00161D38") = 0x1e;
short D_L00_00161D40_gp __asm__("D_L00_00161D40") = 0x23;
short follow_camera_project_hero_focus_blend_base_frames_gp __asm__("D_L00_00161D4C") = 0x55;
short follow_camera_hold_focus_blend_base_frames_gp __asm__("D_L00_00161D5C") = 0x2d;
short D_L00_00161D6C_gp __asm__("D_L00_00161D6C") = 0x1e;
short follow_camera_continue_projected_focus_transition_frames_gp __asm__("D_L00_00161D78") = 0x1e;
s32   follow_cam_blend_frames __asm__("D_L00_00161D20") = 0x5a;
