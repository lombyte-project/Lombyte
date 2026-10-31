/* Plasmabot death debris (class 0x145): a part the plasmabot (or racer) throws off when it dies, spinning with a particle trail until it lands; its spawner and its draw callback, a camera-facing textured quad that l01 burning_debris also draws. */
#include "types.h"
#include "rnc/overlay/shared/enemies/plasmabot_death_debris.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "qcopy.h"
#include "rnc/overlay/quad.h"

extern float D_0015ED60;
extern u8    D_L14_0015F580[] MACRO_ADDR;
#define DE670_THRESHOLD D_L14_00161B20
#define DE670_DAMPING   D_L14_00161B1C
#define DE670_GRAVITY   D_L14_00161B18
extern void *D_001413D0_lui __asm__("D_001413D0") NOT_SDA;
extern void  FUN_L14_002dec40(struct Moby *);
extern s32   FUN_L00_00257b90(s32, s32);
extern s32   coll_sphere(f32, void *, s32, void *) __asm__("FUN_L00_001f0d60");
extern void  spawn_explosion(void *,
                             void *,
                             void *,
                             f32,
                             f32,
                             s32,
                             s32,
                             s32,
                             f32,
                             f32,
                             f32,
                             f32,
                             s32,
                             f32,
                             s32,
                             s32,
                             s32,
                             s32) __asm__("FUN_L00_0025e450");
extern void
FUN_L00_0026cbb0_de670(void *, void *, s32, s32, s32, s32, f32) __asm__("FUN_L00_0026cbb0");
extern u8 *
FUN_L00_0026d000_de670(void *, f32, f32, f32, s32, void *, f32, s32) __asm__("FUN_L00_0026d000");

extern f32 D_L14_00161B18 __attribute__((sda));
extern f32 D_L14_00161B1C __attribute__((sda));
extern f32 D_L14_00161B20 __attribute__((sda));
void       plasmabot_death_debris_update(struct Moby *moby) __asm__("FUN_L14_002de670");

/* Moby class 325 */
void plasmabot_death_debris_update(struct Moby *moby) {
    f32 *velocity = (f32 *)moby->pvars;
    s32  i;
    s32  signed_particle_variation;
    s32  alpha;
    s32  color;
    f32  size;
    f32  absolute_velocity;
    u8  *particle;
    u8  *fade;

    if (random_integer_below(2) != 0) {
        add_draw_callback(FUN_L14_002dec40, moby);
    }
    fast_vec_add(&moby->pos, &moby->pos, velocity);
    absolute_velocity = absolute_float(velocity[0]);
    if (DE670_THRESHOLD * D_0015ED60 < absolute_velocity) {
        velocity[0] *= (DE670_DAMPING - 1.0f) * D_0015ED60 + 1.0f;
    }
    absolute_velocity = absolute_float(velocity[1]);
    if (DE670_THRESHOLD * D_0015ED60 < absolute_velocity) {
        velocity[1] *= (DE670_DAMPING - 1.0f) * D_0015ED60 + 1.0f;
    }
    if (velocity[2] > 0.0f) {
        velocity[2] *= (DE670_DAMPING - 1.0f) * D_0015ED60 + 1.0f;
    }
    velocity[2] -= DE670_GRAVITY * frame_scale_sq;
    moby->rot.x = fast_add_rotations(moby->rot.x, velocity[4]);
    moby->rot.y = fast_add_rotations(moby->rot.y, velocity[5]);
    moby->rot.z = fast_add_rotations(moby->rot.z, velocity[6]);

    if (moby->pos.x < 2.0f || 1021.0f < moby->pos.x || moby->pos.y < 2.0f ||
        1021.0f < moby->pos.y || moby->pos.z < 2.0f || 1021.0f < moby->pos.z) {
        mark_moby_for_removal(moby);
        return;
    }
    if (fast_dec_timer(velocity + 8) != 0 ||
        coll_sphere(0.5f, &moby->pos, 0, *(void **)(velocity + 9)) != 0) {
        spawn_explosion(moby,
                        velocity,
                        &moby->pos,
                        0.0f,
                        0.0f,
                        5,
                        2,
                        4,
                        1.0f,
                        0.5f,
                        9.0f,
                        1.0f,
                        0,
                        15.0f,
                        1,
                        5,
                        -1,
                        0);
        coll_sphere(1.0f, &moby->pos, 0, D_001413D0_lui);
        mark_moby_for_removal(moby);
        return;
    }
    FUN_L00_0026cbb0_de670(&moby->pos,
                           D_L14_0015F580,
                           0x4F007FFF,
                           0x1FFFFFFF,
                           FUN_L00_00257b90(scale_game_frames(10), scale_game_frames(20)),
                           1,
                           60000.0f);
    for (i = 0; i < 2; i++) {
        signed_particle_variation = random_integer_below(6);
        if (random_integer_below(2) != 0)
            signed_particle_variation = -signed_particle_variation;
        size = random_float_between(40000.0f, 100000.0f);
        color = FUN_L00_00257b90(0x30, 0xFF);
        alpha = 0x60000000;
        particle = FUN_L00_0026d000_de670(&moby->pos,
                                          0.025f,
                                          1.0f,
                                          1.0f,
                                          signed_particle_variation,
                                          D_L14_0015F580,
                                          size,
                                          color | ((color << 16) | ((color << 8) | alpha)));
        if (particle != 0) {
            particle[3] = 0x44;
            fade = particle + 0x20;
            *(s16 *)(particle + 0xA) = scale_game_frames(0x3C);
            *(s32 *)(fade + 4) = 2;
            fade[0xA] = 0x60;
            fade[0xB] = particle[0xA];
        }
    }
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_002DFE98), where it is exact; names translated to the US level program. */

extern float FUN_L00_00257c48(float lo, float hi);
extern void
build_look_at_matrix(void *dst, void *vec, void *axis, float angle) __asm__("FUN_00214890");

extern float plasmabot_debris_launch_magnitude_range_start_gp __asm__("D_L14_00161B08")
    __attribute__((sda));
extern float plasmabot_debris_launch_magnitude_range_end_gp __asm__("D_L14_00161B0C")
    __attribute__((sda));
extern float plasmabot_debris_z_velocity_range_start_gp __asm__("D_L14_00161B10")
    __attribute__((sda));
extern float plasmabot_debris_z_velocity_range_end_gp __asm__("D_L14_00161B14")
    __attribute__((sda));

unsigned char *spawn_plasmabot_death_debris(char *owner, char *pos) __asm__("FUN_L14_002dea98");

unsigned char *spawn_plasmabot_death_debris(char *debris_source_moby, char *pos) {
    unsigned char *spawned_moby = create_moby(0x145);
    char          *spawned_vars;
    char          *hero_launch_reference;
    float          launch_vector_scratch[8];
    if (spawned_moby != 0) {
        spawned_vars = ((char *)MOBY(spawned_moby)->pvars);
        *(char **)(spawned_vars + 0x24) = debris_source_moby;
        qcopy(spawned_moby + 0x10, pos);
        MOBY(spawned_moby)->rot.z = *(float *)(debris_source_moby + 0x48);
        spawned_moby[0x20] = 0;
        spawned_moby[0x30] = 0xFF;
        MOBY(spawned_moby)->draw_dist = 0xFF;
        spawned_moby[0x31] = 1;
        hero_launch_reference = (char *)&hero.motion.pos;
        fast_vec_sub(launch_vector_scratch, spawned_moby + 0x10, hero_launch_reference);
        hero_launch_reference = hero_launch_reference + 0x210;
        launch_vector_scratch[2] = 0.0f;
        fast_vec_normalize(
            launch_vector_scratch,
            launch_vector_scratch,
            random_float_between(plasmabot_debris_launch_magnitude_range_start_gp * frame_scale,
                                 plasmabot_debris_launch_magnitude_range_end_gp * frame_scale));
        fast_vec_cross(launch_vector_scratch + 4, hero_launch_reference, launch_vector_scratch);
        build_look_at_matrix(spawned_vars,
                             launch_vector_scratch,
                             hero_launch_reference,
                             FUN_L00_00257c48(0.34906584f, 1.0471976f));
        *(float *)(spawned_vars + 8) =
            random_float_between(plasmabot_debris_z_velocity_range_start_gp * frame_scale,
                                 plasmabot_debris_z_velocity_range_end_gp * frame_scale);
        *(float *)(spawned_vars + 0x10) = FUN_L00_00257c48(0.0f, 0.008726646f);
        *(float *)(spawned_vars + 0x14) = FUN_L00_00257c48(0.05235988f, 0.13962634f);
        *(float *)(spawned_vars + 0x18) = FUN_L00_00257c48(0.0f, DEG_TO_RAD);
        *(int *)(spawned_vars + 0x20) = scale_game_frames(200);
        update_entity(spawned_moby);
    }
    return spawned_moby;
}

typedef union {
    OvlQuad q;
    f32     f[4];
} GlowVec_bfa8;

typedef struct {
    GlowVec_bfa8 v[4];
} GlowCorners_bfa8;

typedef struct {
    GlowVec_bfa8 corner[4];
    u32          color[4];
    s32          unk50, unk54, unk58;
    f32          unk5C, unk60;
    s32          unk64;
    f32          unk68, unk6C;
    s64          unk70, tex, unk80, unk88;
} GlowQuad_bfa8;

extern GlowVec_bfa8 l01_moby_offset_quad_source_corners[4] __asm__("D_L01_0020B9D0");
extern char         D_L01_00167240[];
extern GlowVec_bfa8 D_0013F5E0[];

extern f32  moby_quad_c0_offset_scale_gp __asm__("D_L01_00162064") __attribute__((sda));
extern f32  moby_quad_e0_offset_scale_gp __asm__("D_L01_0016206C") __attribute__((sda));
extern s64  get_effect_texture(s32) __asm__("FUN_001f44b8");
extern void draw_geometry_quad(void *, void *, s32) __asm__("FUN_001f7d30");

void draw_moby_offset_textured_quad(char *moby) __asm__("FUN_L01_0030bfa8");

void draw_moby_offset_textured_quad(char *moby) {
    struct {
        GlowQuad_bfa8 quad;
        GlowVec_bfa8  src_corner[4];
        struct {
            GlowVec_bfa8 mat0, mat1, mat2, point, partial, offset;
        } basis;
    } local;
    GlowVec_bfa8 *basis_axis_or_quad_corner;
    GlowVec_bfa8 *basis_axis_or_source_corner;
    GlowVec_bfa8 *basis;
    s32           i;
    f32           one;
    OvlQuad       source_corner_0, source_corner_1, source_corner_2, source_corner_3;

    i = 3;
    one = 1.0f;
    source_corner_0 = l01_moby_offset_quad_source_corners[0].q;
    source_corner_1 = l01_moby_offset_quad_source_corners[1].q;
    source_corner_2 = l01_moby_offset_quad_source_corners[2].q;
    source_corner_3 = l01_moby_offset_quad_source_corners[3].q;
    local.src_corner[0].q = source_corner_0;
    local.src_corner[1].q = source_corner_1;
    local.src_corner[2].q = source_corner_2;
    local.src_corner[3].q = source_corner_3;
    local.quad.tex = get_effect_texture(0xB);
    local.quad.unk88 = 0x8000000048L;
    local.quad.unk80 = 0xFF9000000260L;
    local.quad.unk70 = 5;
    local.quad.unk50 = 0;
    local.quad.unk54 = 0;
    local.quad.unk58 = 0;
    local.quad.unk5C = one;
    local.quad.unk60 = one;
    local.quad.unk64 = 0;
    local.quad.unk68 = one;
    local.quad.unk6C = one;

    fast_vec_scale(&local.basis.offset, moby + 0xc0, moby_quad_c0_offset_scale_gp);
    fast_vec_scale(&local.basis.partial, moby + 0xe0, moby_quad_e0_offset_scale_gp);
    fast_vec_add(&local.basis.offset, &local.basis.offset, &local.basis.partial);
    basis_axis_or_source_corner = &local.basis.mat0;
    fast_vec_add(&local.basis.point, &local.basis.offset, moby + 0x10);
    fast_vec_sub(basis_axis_or_source_corner, D_L01_00167240, &local.basis.point);
    fast_vec_normalize(basis_axis_or_source_corner, basis_axis_or_source_corner, one);
    basis_axis_or_quad_corner = &local.basis.mat1;
    fast_vec_cross(basis_axis_or_quad_corner, basis_axis_or_source_corner, D_0013F5E0);
    fast_vec_normalize(basis_axis_or_quad_corner, basis_axis_or_quad_corner, -1.0f);
    fast_vec_cross(&local.basis.mat2, basis_axis_or_quad_corner, basis_axis_or_source_corner);
    basis = basis_axis_or_source_corner;

    local.quad.color[3] = 0x202020FF;
    local.quad.color[2] = 0x202020FF;
    local.quad.color[1] = 0x202020FF;
    local.quad.color[0] = 0x202020FF;
    basis_axis_or_source_corner = local.src_corner;
    basis_axis_or_quad_corner = local.quad.corner;
    do {
        fast_vec_scale(basis_axis_or_quad_corner, basis_axis_or_source_corner, 0.4f);
        basis_axis_or_source_corner++;
        transform_vector(basis_axis_or_quad_corner, basis_axis_or_quad_corner, basis);
        basis_axis_or_quad_corner++;
        i--;
    } while (i >= 0);
    draw_geometry_quad(&local.quad, 0, 0);
}

f32   D_L15_00161A80 = 0.003f;
f32   D_L15_00161A84 = 0.97f;
f32   D_L15_00161A88 = 0.05f;
float D_L15_00161A70 = 0.075f;
float D_L15_00161A74 = 0.2f;
float D_L15_00161A78 = 0.2f;
float D_L15_00161A7C = 0.3f;
f32   D_L14_00161B24 = -0.258f;
f32   D_L14_00161B2C = 0.448f;
f32   D_L15_00161A8C = -0.258f;
f32   D_L15_00161A94 = 0.448f;
