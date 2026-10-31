/* Level 17 moby class fleet_rotating_radial_effect (0x29d): spins up, then sprays particles from points on a rotating ring, and draws its fanned scrolling quads. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"
#include "rnc/game/game_globals.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/world/collision/collision_line.h"
#include "rnc/math/normalize_vector_triplet.h"
#include "rnc/overlay/quad.h"
#include "rnc/overlay/l17/mobys/fleet_rotating_radial_effect_globals.h"

/* Fleet moby 669 update: spins up, then sprays particles from points on a rotating ring. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002AA068.c: func_L17_002D8CC8), where it is exact; names translated to the US level program. */

struct Blk {
    int   a0;
    int   a1;
    float f8;
    float fC;
    void *p10;
    int   v14;
    char  c18;
    char  c19;
    short h1A;
    float f1C;
    int   i20;
    int   pad[3];
};

extern float scale_time(float) __asm__("FUN_001f96b0");
extern int   get_effect_texture(int) __asm__("FUN_001f44b8");
extern int   hero_is_in_special_state(void) __asm__("FUN_L00_0020d568");
extern int   rand(void);
extern int   random_integer_below(int);
extern short fleet_radial_rotation_rate_degrees_factor __asm__("D_L17_00161B78")
    __attribute__((sda));
extern short fleet_radial_base_elevation_degrees __asm__("D_L17_00161B7C") __attribute__((sda));
extern f32   D_L17_00161B80_gp __asm__("D_L17_00161B80") __attribute__((sda));
extern short fleet_radial_radius __asm__("D_L17_00161B84") __attribute__((sda));
extern short fleet_radial_particles_per_emission __asm__("D_L17_00161B88") __attribute__((sda));
extern short fleet_radial_particle_scale __asm__("D_L17_00161B8C") __attribute__((sda));
extern short fleet_radial_particle_lifetime_range_end __asm__("D_L17_00161B90")
    __attribute__((sda));
extern short fleet_radial_particle_color __asm__("D_L17_00161B94") __attribute__((sda));
extern unsigned char      *
spawn_moving_hero_relative_alpha_fade_particle(float        *pos,
                                                    char         *vel,
                                                    int           color,
                                                    unsigned char life,
                                                    unsigned char b,
                                                    int           mode,
                                                    float         scale) __asm__("FUN_L00_002730e0");
void draw_fleet_scrolling_radial_quads(struct Moby *moby) __asm__("FUN_L17_002d7cf0");

void fleet_rotating_radial_effect_update(char *moby) __asm__("FUN_L17_002d77f0");

/* Moby class 669 */
void fleet_rotating_radial_effect_update(char *moby) {
    enum {
        RING_INITIALIZE = 0,
        RING_ROTATE_AND_DRAW = 1
    };
    OvlQuad    moby_to_hero_vector[1];
    OvlQuad    hero_facing_unit_axis[1];
    struct Blk blk;
    OvlQuad    normalized_moby_rotation_basis[3];
    OvlQuad    moby_rotation_matrix[4];
    OvlQuad    spC0[1];
    OvlQuad    spD0[1];
    OvlQuad    spE0[1];
    OvlQuad    spF0[1];
    OvlQuad    particle_vector_scratch[1];
    int        j;
    int        k;
    float     *ring_vars;
    float      particle_direction_length;
    float      base_elevation_angle;
    float      elevation_angle_offset;
    float      particle_direction_sign_or_minimum_speed;
    int        particle_lifetime;
    char      *particle_vector_scratch_destination;

    ring_vars = ((float *)MOBY(moby)->pvars);
    if (hero_is_in_special_state() != 0) {
        switch (MOBY(moby)->state) {
        case RING_INITIALIZE:
            MOBY(moby)->update_dist = 0xFF;
            MOBY(moby)->state = 1;
            ring_vars[1] = ring_vars[0] * *(float *)&fleet_radial_rotation_rate_degrees_factor *
                           DEG_TO_RAD * frame_time;
            break;
        case RING_ROTATE_AND_DRAW:
            MOBY(moby)->rot.x = fast_add_rotations(MOBY(moby)->rot.x, ring_vars[1]);
            add_draw_callback(draw_fleet_scrolling_radial_quads, moby);
            break;
        }
        fast_vec_sub(moby_to_hero_vector, &hero.motion.pos, (moby + 0x10));
        if (fast_vec_dot(moby_to_hero_vector, (moby + 0xC0)) > 0.0f) {
            fast_vec_normalize(hero_facing_unit_axis, (moby + 0xC0), 1.0f);
        } else {
            fast_vec_normalize(hero_facing_unit_axis, (moby + 0xC0), -1.0f);
        }
        {
            int i;
            blk.p10 = moby;
            blk.v14 = 0x10001;
            blk.i20 = 1;
            blk.c18 = 1;
            blk.f1C = 1.0f;
            blk.f8 = 1.0f;
            blk.fC = 5627.9248046875f;
            blk.c19 = 1;
            blk.h1A = ((unsigned short)MOBY(moby)->oclass);
            euler_rotation_basis(normalized_moby_rotation_basis, moby + 0x40);
            normalize_vector_triplet(normalized_moby_rotation_basis);
            copy_matrix_rotation(moby_rotation_matrix, normalized_moby_rotation_basis);
            i = 0;
            do {
                j = 0;
                do {
                    elevation_angle_offset =
                        fast_normalize_angle(*(float *)&D_L17_00161B80_gp * DEG_TO_RAD *
                                             convert_integer_to_float(j) * 0.5f) -
                        *(float *)&D_L17_00161B80_gp * DEG_TO_RAD;
                    base_elevation_angle =
                        *(float *)&fleet_radial_base_elevation_degrees * DEG_TO_RAD;
                    build_spherical_offset(
                        spC0,
                        *(float *)&fleet_radial_radius,
                        1.5707964f,
                        fast_add_rotations(
                            fast_add_rotations(
                                base_elevation_angle,
                                fast_normalize_angle(convert_integer_to_float(i) * 2.0943952f)),
                            elevation_angle_offset));
                    transform_vector(spC0, spC0, moby_rotation_matrix);
                    add_vectors(spD0, spC0, (moby + 0x10));
                    if (fast_vec_dist((moby + 0x10), &hero.motion.pos) < 30.0f) {
                        collision_line((moby + 0x10), spC0, 9, moby, &blk);
                    }
                    spD0[0] = *(OvlQuad *)&MOBY(moby)->pos;
                    *(float *)((char *)spD0 + 0xC) = *(float *)&fleet_radial_radius;
                    if (fast_bsphere_check((char *)spD0, 20.0f) != -1 && (j == 0 || j == 4)) {
                        spF0[0] = 0;
                        *(float *)((char *)spF0 + 8) = 12.0f;
                        transform_vector_by_basis(spF0, spF0, (moby + 0xC0));
                        {
                            for (k = 0; k < *(int *)&fleet_radial_particles_per_emission; k++) {
                                particle_direction_sign_or_minimum_speed = -1.0f;
                                if (rand() & 1)
                                    particle_direction_sign_or_minimum_speed = 1.0f;
                                particle_lifetime =
                                    truncate_float_to_s32(scale_time(random_float_between(
                                        *(float *)&fleet_radial_particle_lifetime_range_end * 0.5f,
                                        *(float *)&fleet_radial_particle_lifetime_range_end)));
                                if (particle_lifetime <= 0)
                                    particle_lifetime = 1;
                                fast_vec_sub(particle_vector_scratch, spC0, (moby + 0x10));
                                spF0[0] = particle_vector_scratch[0];
                                fast_vec_scale(
                                    spF0, spF0, particle_direction_sign_or_minimum_speed);
                                particle_direction_length = fast_vec_length(spF0);
                                if (particle_direction_sign_or_minimum_speed < 0.0f)
                                    spE0[0] = spC0[0];
                                else
                                    spE0[0] = *(OvlQuad *)&MOBY(moby)->pos;
                                fast_vec_scale(spF0, spF0, random_float_between(0.0f, 0.9f));
                                particle_vector_scratch_destination =
                                    (char *)particle_vector_scratch;
                                add_vectors(particle_vector_scratch_destination, spE0, spF0);
                                particle_direction_sign_or_minimum_speed = frame_time;
                                particle_direction_sign_or_minimum_speed *= 10.0f;
                                fast_vec_normalize(
                                    spF0,
                                    spF0,
                                    random_float_between(
                                        particle_direction_sign_or_minimum_speed,
                                        particle_direction_length /
                                            convert_integer_to_float(particle_lifetime * 4)));
                                spawn_moving_hero_relative_alpha_fade_particle(
                                    (float *)spE0,
                                    (char *)spF0,
                                    *(int *)&fleet_radial_particle_color,
                                    particle_lifetime & 0xFF,
                                    random_integer_below(0xFF) & 0xFF,
                                    0,
                                    *(float *)&fleet_radial_particle_scale);
                            }
                        }
                    }
                    j++;
                } while (j < 5);
                i++;
            } while (i < 3);
        }
    }
}

/* Draw callback of fleet moby 669: two passes of three textured quads fanned around the moby, with scrolling texture coordinates. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002AA068.c: func_L17_002D91C8), where it is exact; names translated to the US level program. */

typedef struct {
    float v[2];
} RingStep;

typedef struct {
    float x, y;
} RingUV;

typedef struct {
    float v[4][4];
    int   rgba[4];
    struct {
        float x, y;
    } uv[4];
    long z70;
    long tex;
    long q80;
    long alpha;
} RingPrim;

extern float  D_L17_00161B80_gp __asm__("D_L17_00161B80") __attribute__((sda));
extern RingUV l17_fleet_radial_quad_base_uvs[4] __asm__("D_L17_001D3F70");

extern float fleet_radial_uv_scroll_rate_gp __asm__("D_L17_00161BBC") __attribute__((sda));
extern float FUN_L00_00200260(float, float);
extern int   fleet_radial_alpha_base_value_gp __asm__("D_L17_00161B98") __attribute__((sda));

#include "rnc/overlay/l17/mobys/fleet_rotating_radial_effect_data/draw_fleet_scrolling_radial_quads.h"

extern int fleet_radial_quad_rgba_gp __asm__("D_L17_00161BB8") __attribute__((sda));
extern RingStep D_L17_00161BC0 NOT_SDA;
extern RingStep D_L17_00161BC8 NOT_SDA;
extern void                    draw_geometry_quad(void *, void *, int);

void draw_fleet_scrolling_radial_quads(struct Moby *moby) {
    RingPrim quad_primitive;
    float    moby_transform_matrix[16];
    float    scroll;
    int      pass;
    int      i;
    int      j;
    int      k;

    scroll =
        convert_integer_to_float(D_L17_0015F5CC) * (fleet_radial_uv_scroll_rate_gp * frame_time);
    scroll = FUN_L00_00200260(scroll, 1.0f);
    euler_rotation_matrix(moby_transform_matrix, &moby->rot.x);
    *(OvlQuad *)&moby_transform_matrix[12] = *(OvlQuad *)&moby->pos;
    moby_transform_matrix[15] = 1.0f;
    quad_primitive.tex = get_effect_texture(0x28);
    quad_primitive.q80 = 0xFF9000000260L;
    quad_primitive.alpha = (long)fleet_radial_alpha_base_value_gp |
                           ((long)fleet_radial_blend_subtract_color_selector << 2) |
                           ((long)fleet_radial_blend_alpha_factor_selector << 4) |
                           ((long)fleet_radial_blend_add_color_selector << 6) | 0x8000000000L;
    quad_primitive.z70 = 0;
    for (pass = 0; pass < 2; pass++) {
        for (k = 0; k < 4; k++) {
            RingStep step = D_L17_00161BC0;

            quad_primitive.uv[k].y = l17_fleet_radial_quad_base_uvs[k].y + scroll * step.v[pass];
        }
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 4; j++) {
                RingStep step = D_L17_00161BC8;
                float    spread[4] = {
                    0.0f, -D_L17_00161B80_gp * DEG_TO_RAD, D_L17_00161B80_gp * DEG_TO_RAD, 0.0f};
                float *vj;
                float  radius;

                quad_primitive.uv[j].x =
                    l17_fleet_radial_quad_base_uvs[j].x + scroll * step.v[pass];
                radius = 0.0f;
                if (j != 0) {
                    radius = *(float *)&fleet_radial_radius;
                }
                vj = quad_primitive.v[j];
                build_spherical_offset(
                    vj,
                    radius,
                    1.5707964f,
                    fast_normalize_angle(*(float *)&fleet_radial_base_elevation_degrees *
                                             DEG_TO_RAD +
                                         convert_integer_to_float(i) * 2.0943952f + spread[j]));
                vj[3] = 1.0f;
            }
            quad_primitive.rgba[3] = fleet_radial_quad_rgba_gp;
            quad_primitive.rgba[2] = fleet_radial_quad_rgba_gp;
            quad_primitive.rgba[1] = fleet_radial_quad_rgba_gp;
            quad_primitive.rgba[0] = fleet_radial_quad_rgba_gp;
            draw_geometry_quad(&quad_primitive, moby_transform_matrix, 0);
        }
    }
}

RingUV l17_fleet_radial_quad_base_uvs[4] __asm__("D_L17_001D3F70") = {
    {0},
    {0.0f, 1.0f},
    {0.25f, 0.0f},
    {0.25f, 1.0f},
};

RingStep D_L17_00161BC8 = {
    {1.0f, 0.62831855f}
};
