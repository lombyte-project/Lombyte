/* Shared moby class vendor: the Gadgetron vendor and its effects. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/attach_manipulator.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/ui/help/try_set_help_message.h"
#include "rnc/overlay/shared/vendor/vendor_menu.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "qcopy.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/overlay/shared/vendor/vendor_data/vendor_update.h"
#include "rnc/overlay/collision.h"

typedef unsigned int Q_2A62D0 __attribute__((mode(TI), aligned(16)));

typedef union {
    Q_2A62D0 q;
    float    f[4];
} V_2A62D0;

typedef struct {
    float v[4][4];
    int   col[4];
    float uv[4][2];
} Quad;

typedef struct {
    f32 f[4];
} V_2a57a8;

typedef struct {
    u8  p0[0x24];
    f32 f24;
} N_2a57a8;

typedef struct {
    u8        p0[0x10];
    f32       f10, f14, f18;
    u8        p1C[0x24 - 0x1C];
    N_2a57a8 *p24;
    u8        p28[4];
    f32       f2C;
    u8        p30[2];
    u16       h32;
    u16       h34;
    u8        p36[0x73 - 0x36];
    u8        b73;
} M_2a57a8;

typedef struct {
    s32       i0, i4;
    u8        p8[4];
    M_2a57a8 *mC;
    u8        p10[0x90 - 0x10];
    f32       f90;
} Q_2a57a8;

typedef struct {
    u8        p0[0x10];
    f32       f10, f14, f18;
    u8        p1C[4];
    u8        b20;
    u8        p21[0x52 - 0x21];
    u8        b52, b53;
    u8        p54[0x78 - 0x54];
    Q_2a57a8 *q;
    u8        p7C[0x90 - 0x7C];
    s32       i90;
} P_2a57a8;

typedef struct {
    u8  p0[0x80];
    f32 f80, f84;
    u8  p88[0x98 - 0x88];
    f32 f98;
    u8  p9C[0x2084 - 0x9C];
    s32 i2084;
    u8  p2088[4];
    s32 i208C;
    u8  p2090[0x20A4 - 0x2090];
    u8  b20A4;
} G_2a57a8;

extern LevelCamera l00_camera_lui __asm__("D_L00_00166C80") NOT_SDA;

extern float D_L00_001D7360[][4];
extern float D_L00_001D73F0[][2];
extern int   vendor_sparkle_colors[] __asm__("D_L00_001D7438") NOT_SDA;
extern int   vendor_sparkle_quad_corners[][4] __asm__("D_L00_001D7460");
extern s64   get_effect_texture(s32) __asm__("FUN_001f44b8");
extern float vendor_sparkle_uv_scroll __asm__("D_L00_00161394") __attribute__((sda));
extern float vendor_glow_phase __asm__("D_L00_00161398") __attribute__((sda));
void        draw_offset_textured_effect_quad(void *, int, float, float) __asm__("FUN_L00_00263618");
extern void draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");

extern V_2a57a8 hero_pos __asm__("D_0013F3D0") NOT_SDA;
extern s32      D_0013CAE4_lui __asm__("D_0013CAE4") NOT_SDA;
extern s32      D_L00_0015F5CC_2a57a8 __asm__("D_L00_0015F5CC");
void            FUN_L00_002a5040_2a57a8(void) __asm__("FUN_L00_002a5040");

/* Ported from rac1-decomp (src/overlays/shared/bolts_and_wrench.c: func_L00_002A62D0). */

void draw_vendor_effects(char *m) __asm__("FUN_L00_002a5040");

void draw_vendor_effects(char *m) {
    char    *p = *(char **)(m + 0x78);
    Quad     qd;
    long     pk[4];
    float    q[4][4];
    float    d0[4];
    float    mtx[3][4];
    V_2A62D0 M[4];
    V_2A62D0 T[4];
    float    zero;
    char    *g, *h;
    float(*uv_v)[2];
    float(*uvs)[2];
    int   izero;
    float off;
    /* Fakematch: the sparkle radius 1.1 lives in $f22, copied from the glow block's 1.1. */
    register float sparkle_radius __asm__("$f22");
    /* Fakematch: d0's address for the second euler_rotation_basis call. */
    register float *d0_arg __asm__("$5");
    int             k, i, i0, i1, i2, i3, col;

    h = (char *)&l00_camera_lui;
    d0[2] = *(float *)(h + 0x158);
    d0[0] = 0.0f;
    d0[1] = 0.0f;
    euler_rotation_basis(mtx, d0);

    vendor_sparkle_uv_scroll = vendor_sparkle_uv_scroll + 0.01f;
    if (1.0f < vendor_sparkle_uv_scroll) {
        vendor_sparkle_uv_scroll = vendor_sparkle_uv_scroll - 1.0f;
    }

    pk[2] = 1;
    pk[3] = 0x4000000044L;
    pk[0] = 0;
    pk[1] = get_effect_texture(0x18);

    for (k = 0; k < 4; k++) {
        transform_vector_by_basis(q[0], D_L00_001D7360[vendor_sparkle_quad_corners[k][0]], mtx);
        transform_vector_by_basis(q[1], D_L00_001D7360[vendor_sparkle_quad_corners[k][1]], mtx);
        transform_vector_by_basis(q[2], D_L00_001D7360[vendor_sparkle_quad_corners[k][2]], mtx);
        transform_vector_by_basis(q[3], D_L00_001D7360[vendor_sparkle_quad_corners[k][3]], mtx);

        qd.v[0][0] = *(float *)(m + 0x10) + q[0][0] * *(float *)(p + 0x90);
        qd.v[0][1] = *(float *)(m + 0x14) + q[0][1] * *(float *)(p + 0x90);
        qd.v[0][2] = *(float *)(m + 0x18) + q[0][2];
        qd.v[1][0] = *(float *)(m + 0x10) + q[1][0] * *(float *)(p + 0x90);
        qd.v[1][1] = *(float *)(m + 0x14) + q[1][1] * *(float *)(p + 0x90);
        qd.v[1][2] = *(float *)(m + 0x18) + q[1][2];
        qd.v[2][0] = *(float *)(m + 0x10) + q[2][0] * *(float *)(p + 0x90);
        qd.v[2][1] = *(float *)(m + 0x14) + q[2][1] * *(float *)(p + 0x90);
        qd.v[2][2] = *(float *)(m + 0x18) + q[2][2];
        qd.v[3][0] = *(float *)(m + 0x10) + q[3][0] * *(float *)(p + 0x90);
        qd.v[3][1] = *(float *)(m + 0x14) + q[3][1] * *(float *)(p + 0x90);
        qd.v[3][2] = *(float *)(m + 0x18) + q[3][2];

        i0 = vendor_sparkle_quad_corners[k][0];
        i1 = vendor_sparkle_quad_corners[k][1];
        i2 = vendor_sparkle_quad_corners[k][2];
        i3 = vendor_sparkle_quad_corners[k][3];
        off = vendor_sparkle_uv_scroll;
        qd.col[0] = vendor_sparkle_colors[i0];
        qd.col[1] = vendor_sparkle_colors[i1];
        qd.col[2] = vendor_sparkle_colors[i2];
        qd.col[3] = vendor_sparkle_colors[i3];
        uvs = D_L00_001D73F0;
        uv_v = (float(*)[2]) & uvs[0][1];
        qd.uv[0][0] = uvs[i0][0];
        qd.uv[0][1] = uv_v[i0][0] + off;
        qd.uv[1][0] = uvs[i1][0];
        qd.uv[1][1] = uv_v[i1][0] + off;
        qd.uv[2][0] = uvs[i2][0];
        qd.uv[2][1] = uv_v[i2][0] + off;
        qd.uv[3][0] = uvs[i3][0];
        qd.uv[3][1] = uv_v[i3][0] + off;
        draw_geometry_quad(&qd, 0, 1);
    }

    pk[1] = get_effect_texture(0x1B);

    vendor_glow_phase = vendor_glow_phase + 0.01f;
    if (1.0f < vendor_glow_phase) {
        vendor_glow_phase = 0.0f;
    }

    {
        float g = vendor_glow_phase;
        float n = -g * 1.2f;
        float pp = g * 1.2f;
        float z = g * 1.4f + 1.1f;
        float zz = z - 0.2f;
        sparkle_radius = 1.1f;

        T[0].q = 0;
        T[0].f[0] = n * *(float *)(p + 0x90);
        T[0].f[1] = n * *(float *)(p + 0x90);
        T[0].f[2] = z;
        T[1].q = 0;
        T[1].f[0] = pp * *(float *)(p + 0x90);
        T[1].f[1] = n * *(float *)(p + 0x90);
        T[1].f[2] = zz;
        T[2].q = 0;
        T[2].f[0] = n * *(float *)(p + 0x90);
        T[2].f[1] = pp * *(float *)(p + 0x90);
        T[2].f[2] = z;
        T[3].q = 0;
        T[3].f[0] = pp * *(float *)(p + 0x90);
        T[3].f[1] = pp * *(float *)(p + 0x90);
        T[3].f[2] = zz;
        M[0].q = T[0].q;
        T[0].q = M[0].q;
        M[1].q = T[1].q;
        T[1].q = M[1].q;
        M[2].q = T[2].q;
        M[3].q = T[3].q;
    }

    g = (char *)&l00_camera_lui;
    off = *(float *)(g + 0x158);
    d0[0] = 0.0f;
    d0[2] = off;
    d0[1] = 0.0f;
    /* Fakematch: d0_arg = d0 (frame slot 0xD0). The sparkle_radius input puts it after retail's mov.s $f22,$f5;
     * the $4 clobber keeps addiu $a0 (mtx) after it. */
    __asm__("addiu %0,$sp,0xD0" : "=r"(d0_arg) : "f"(sparkle_radius) : "$4");
    euler_rotation_basis(mtx, d0_arg);
    transform_vector_by_basis(q[0], M[0].f, mtx);
    transform_vector_by_basis(q[1], M[1].f, mtx);
    transform_vector_by_basis(q[2], M[2].f, mtx);
    transform_vector_by_basis(q[3], M[3].f, mtx);

    col = ((int)((1.0f - vendor_glow_phase) * 128.0f) << 24) | 0x808080;
    qd.v[0][0] = *(float *)(m + 0x10) + q[0][0];
    qd.v[0][1] = *(float *)(m + 0x14) + q[0][1];
    qd.v[0][2] = *(float *)(m + 0x18) + q[0][2];
    qd.v[1][0] = *(float *)(m + 0x10) + q[1][0];
    qd.v[1][1] = *(float *)(m + 0x14) + q[1][1];
    qd.v[1][2] = *(float *)(m + 0x18) + q[1][2];
    qd.v[2][0] = *(float *)(m + 0x10) + q[2][0];
    qd.v[2][1] = *(float *)(m + 0x14) + q[2][1];
    qd.v[2][2] = *(float *)(m + 0x18) + q[2][2];
    qd.v[3][0] = *(float *)(m + 0x10) + q[3][0];
    qd.v[3][1] = *(float *)(m + 0x14) + q[3][1];
    qd.v[3][2] = *(float *)(m + 0x18) + q[3][2];
    qd.col[0] = col;
    qd.col[1] = col;
    qd.col[2] = col;
    qd.col[3] = col;
    qd.uv[0][0] = 0.0f;
    qd.uv[0][1] = 0.0f;
    qd.uv[1][0] = 1.0f;
    qd.uv[1][1] = 0.0f;
    qd.uv[2][0] = 0.0f;
    qd.uv[2][1] = 1.0f;
    qd.uv[3][0] = 1.0f;
    qd.uv[3][1] = 1.0f;
    draw_geometry_quad(&qd, 0, 1);

    /* the sparkle vector reuses the first matrix row's slot in retail; its w is that row's 0 */
    izero = 0;
    zero = *(float *)&izero;
    for (i = 0; i < 4; i++) {
        float ang = (float)i * 3.14159f * 0.5f - 3.14159f;
        T[0].f[0] = fast_cos(ang) * sparkle_radius;
        T[0].f[1] = fast_sin(ang) * sparkle_radius;
        T[0].f[2] = 0.59f;
        transform_vector_by_basis(T[0].f, T[0].f, m + 0xC0);
        fast_vec_add(T[0].f, T[0].f, m + 0x10);
        draw_offset_textured_effect_quad(T[0].f, *(int *)(m + 0x90) & 0xFFFF0000, 0.1333f, zero);
    }
}

void vendor_update(P_2a57a8 *p) __asm__("FUN_L00_002a57a8");

void vendor_update(P_2a57a8 *vendor_moby) {
    enum {
        VENDOR_INITIALIZE = 0,
        VENDOR_WAIT_FOR_HERO = 1,
        VENDOR_HERO_NEARBY = 2,
        VENDOR_ZERO_AUXILIARY_SCALE = 3
    };
    Q_2a57a8 *q = vendor_moby->q;
    s32       i, interaction_allowed, help_message_allowed, help_message_succeeded;
    u8       *v;
    f32       hero_distance, z;

    vendor_color_pulse_phase += 0.05f;
    if (vendor_color_pulse_phase > 3.14f)
        vendor_color_pulse_phase = -3.14f;
    vendor_moby->i90 =
        (((s32)(fast_sin(vendor_color_pulse_phase) * 48.0f) + 0x60) * 0x10101) | 0x80000000;
    switch (vendor_moby->b20) {
    case VENDOR_INITIALIZE:
        vendor_moby->b20 = 1;
        q->i0 = 0;
        q->i4 = 0;
        q->mC = create_moby(0x477);
        q->mC->h34 |= 2;
        q->mC->h32 = 0x40;
        q->mC->f10 = vendor_moby->f10;
        q->mC->f14 = vendor_moby->f14;
        q->mC->f18 = vendor_moby->f18 + 2.95f;
        q->mC->b73 = 0x20;
        q->f90 = 0.0f;
        for (i = 0; i < 2; i++) {
            attach_manipulator(q->mC, i, (u8 *)q + 0x10 + i * 0x40);
        }
        update_entity(q->mC);
        break;
    case VENDOR_WAIT_FOR_HERO:
        if (q->f90 > 0.0f)
            q->f90 -= 0.1f;
        if (q->f90 <= 0.0f)
            q->mC->h34 |= 1;
        if (!(D_L00_0015F5CC_2a57a8 & 7) &&
            vector_distance_xy(&vendor_moby->f10, &hero_pos) <= 16.0f &&
            absolute_float(vendor_moby->f18 - hero_pos.f[2]) <= 8.0f) {
            vendor_moby->b20 = 2;
            if (vendor_moby->b53 != 1)
                blend_moby_animation(vendor_moby, 1, 0, 10);
            q->mC->h34 &= 0xFFFE;
        }
        add_draw_callback(FUN_L00_002a5040_2a57a8, vendor_moby);
        rotation_2D5FA8((u8 *)q + 0x20, 2, vendor_manipulator_rotation_phase);
        rotation_2D5FA8((u8 *)q + 0x60, 2, vendor_manipulator_counter_rotation_phase);
        vendor_manipulator_rotation_phase =
            fast_add_rotations(vendor_manipulator_rotation_phase, 0.01f);
        vendor_manipulator_counter_rotation_phase =
            fast_add_rotations(vendor_manipulator_counter_rotation_phase, -0.02f);
        q->mC->f2C = q->f90 * q->mC->p24->f24 * 2.5f;
        update_entity(q->mC);
        break;
    case VENDOR_HERO_NEARBY:
        if (q->f90 < 1.0f)
            q->f90 += 0.1f;
        hero_distance = vector_distance_xy(&vendor_moby->f10, &hero_pos);
        z = absolute_float(vendor_moby->f18 - hero_pos.f[2]);
        if (!(D_L00_0015F5CC_2a57a8 & 7) && (hero_distance > 18.0f || z > 10.0f)) {
            vendor_moby->b20 = 1;
            if (vendor_moby->b53)
                blend_moby_animation(vendor_moby, 0, 0, 10);
            break;
        }
        add_draw_callback(FUN_L00_002a5040_2a57a8, vendor_moby);
        interaction_allowed = 1;
        help_message_allowed = 1;
        rotation_2D5FA8((u8 *)q + 0x20, 2, vendor_manipulator_rotation_phase);
        rotation_2D5FA8((u8 *)q + 0x60, 2, vendor_manipulator_counter_rotation_phase);
        vendor_manipulator_rotation_phase =
            fast_add_rotations(vendor_manipulator_rotation_phase, 0.01f);
        vendor_manipulator_counter_rotation_phase =
            fast_add_rotations(vendor_manipulator_counter_rotation_phase, -0.02f);
        q->mC->f2C = q->f90 * q->mC->p24->f24 * 2.5f;
        update_entity(q->mC);
        if (hero.state.control_mode != 0 && hero.state.current != 3 &&
            hero.state.control_mode != 1) {
            interaction_allowed = 0;
            help_message_allowed = 0;
        }
        if (hero.state.current == 0x1D || hero.state.current == 0x32) {
            interaction_allowed = 0;
            help_message_allowed = 0;
        }
        if (hero.vehicle_mode) {
            interaction_allowed = 0;
            help_message_allowed = 0;
        }
        if (vendor_moby->b52 != 1)
            interaction_allowed = 0;
        if (hero_distance > 4.0f || z > 2.0f) {
            interaction_allowed = 0;
            help_message_allowed = 0;
        }
        if (fast_difference_between_rotations(
                hero.motion.rot.f[2],
                fast_arc_tan(vendor_moby->f10 - hero.motion.pos.f[0],
                             vendor_moby->f14 - hero.motion.pos.f[1])) > 1.5707964f) {
            interaction_allowed = 0;
            help_message_allowed = 0;
        }
        help_message_succeeded = 0;
        if (help_message_allowed)
            help_message_succeeded = try_set_help_message(1, 0x53E3) != 0;
        if ((D_0013CAE4_lui & 0x10) && interaction_allowed && help_message_succeeded) {
            open_vendor_menu(vendor_moby);
            vendor_moby->b20 = 3;
        }
        break;
    case VENDOR_ZERO_AUXILIARY_SCALE:
        if (q->mC) {
            q->f90 = 0.0f;
            q->mC->f2C = q->mC->p24->f24 * q->f90 * 2.5f;
            update_entity(q->mC);
        }
        break;
    }
}

s32 D_L00_001D7460[1] __attribute__((section(".data"))) = {0};

s32 D_L01_001D78E0[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001CBEE0[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001CB9E0[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001CBA60[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001CBD60[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001CC060[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001CB960[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001CC0E0[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001CBA60[1] __attribute__((section(".data"))) = {0};

s32 D_L10_001CBD60[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001CC2E0[1] __attribute__((section(".data"))) = {0};

s32 D_L12_001CBD60[1] __attribute__((section(".data"))) = {0};

s32 D_L13_001CBBE0[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001D87D0[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001CBEE0[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001CBD60[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001CC1E0[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001CC2E0[1] __attribute__((section(".data"))) = {0};

float D_L01_001D77E0 NOT_SDA = 0;
float D_L01_001D7870 NOT_SDA = 0;
float                D_L01_00161394 = 0;
float                D_L01_00161398 = 0;

float D_L02_001CBDE0 NOT_SDA = 0;
float D_L02_001CBE70 NOT_SDA = 0;
float                D_L02_00161368 = 0;
float                D_L02_0016136C = 0;

float D_L03_001CB8E0 NOT_SDA = 0;
float D_L03_001CB970 NOT_SDA = 0;
float                D_L03_00161370 = 0;
float                D_L03_00161374 = 0;
float D_L04_001CB960 NOT_SDA = 0;
float D_L04_001CB9F0 NOT_SDA = 0;
float                D_L04_00161368 = 0;
float                D_L04_0016136C = 0;

float D_L05_001CBC60 NOT_SDA = 0;
float D_L05_001CBCF0 NOT_SDA = 0;
float                D_L05_00161368 = 0;
float                D_L05_0016136C = 0;

float D_L06_001CBF60 NOT_SDA = 0;
float D_L06_001CBFF0 NOT_SDA = 0;
float                D_L06_00161368 = 0;
float                D_L06_0016136C = 0;

float D_L07_001CB860 NOT_SDA = 0;
float D_L07_001CB8F0 NOT_SDA = 0;
float                D_L07_00161368 = 0;
float                D_L07_0016136C = 0;

float D_L08_001CBFE0 NOT_SDA = 0;
float D_L08_001CC070 NOT_SDA = 0;
float                D_L08_00161368 = 0;
float                D_L08_0016136C = 0;

float D_L09_001CB960 NOT_SDA = 0;
float D_L09_001CB9F0 NOT_SDA = 0;
float                D_L09_001613D8 = 0;
float                D_L09_001613DC = 0;

float D_L10_001CBC60 NOT_SDA = 0;
float D_L10_001CBCF0 NOT_SDA = 0;
float                D_L10_00161368 = 0;
float                D_L10_0016136C = 0;

float D_L11_001CC1E0 NOT_SDA = 0;
float D_L11_001CC270 NOT_SDA = 0;
float                D_L11_00161368 = 0;
float                D_L11_0016136C = 0;

float D_L12_001CBC60 NOT_SDA = 0;
float D_L12_001CBCF0 NOT_SDA = 0;
float                D_L12_00161368 = 0;
float                D_L12_0016136C = 0;

float D_L13_001CBAE0 NOT_SDA = 0;
float D_L13_001CBB70 NOT_SDA = 0;
float                D_L13_00161368 = 0;
float                D_L13_0016136C = 0;
float D_L14_001D86D0 NOT_SDA = 0;
float D_L14_001D8760 NOT_SDA = 0;
float                D_L14_00161488 = 0;
float                D_L14_0016148C = 0;

float D_L15_001CBDE0 NOT_SDA = 0;
float D_L15_001CBE70 NOT_SDA = 0;
float                D_L15_00161368 = 0;
float                D_L15_0016136C = 0;

float D_L16_001CBC60 NOT_SDA = 0;
float D_L16_001CBCF0 NOT_SDA = 0;
float                D_L16_00161368 = 0;
float                D_L16_0016136C = 0;

float D_L17_001CC0E0 NOT_SDA = 0;
float D_L17_001CC170 NOT_SDA = 0;
float                D_L17_00161368 = 0;
float                D_L17_0016136C = 0;

float D_L18_001CC1E0 NOT_SDA = 0;
float D_L18_001CC270 NOT_SDA = 0;
float                D_L18_00161368 = 0;
float                D_L18_0016136C = 0;
