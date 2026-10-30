/* Level 17 moby class fleet_beam (0x63): a beam swinging between two waypoints that sprays particles while the hero is in its zone, and its draw. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "rnc/game/game_globals.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/world/collision/collision_line.h"
#include "qcopy.h"
#include "rnc/overlay/shared/rendering/effects/create_part.h"
#include "rnc/overlay/quad.h"

/* Spawns a particle (part type 0x4F) at pos moving by record_vector_source, following the parent moby when one is given. */
extern unsigned char *D_L17_001B2A54 NOT_SDA;
unsigned char                       *spawn_fleet_beam_particle(struct Moby *parent,
                                                               void        *pos,
                                                               float *record_vector_source) __asm__("FUN_L17_0026fb60");

unsigned char *

spawn_fleet_beam_particle(struct Moby *parent, void *pos, float *record_vector_source) {
    int            header_value_input_base = random_integer_below(5);
    unsigned char  unscaled_header_value = scale_game_frames(header_value_input_base + 2);
    unsigned char  header_byte_value = random_integer_below(0xFF);
    unsigned char *record = create_part(0x4F);
    if (record != 0) {
        char *record_data = (char *)record + 0x20;
        qcopy(record + 0x10, pos);
        *(float *)(record_data + 0x10) = record_vector_source[0];
        *(float *)(record_data + 0x14) = record_vector_source[1];
        *(float *)(record_data + 0x18) = record_vector_source[2];
        if (parent != 0) {
            fast_vec_sub(record_data, record + 0x10, &parent->pos);
            *(short *)(record + 0xA) = unscaled_header_value;
        } else {
            *(short *)(record + 0xA) = unscaled_header_value << 2;
        }
        *(char **)(record_data + 0x1C) = parent;
        *(int *)(record + 4) = 0x604040FF;
        record[9] = truncate_float_to_s32(1.0f) + 0x20;
        record[3] = 0x48;
        *(float *)(record + 0xC) = 42000.0f;
        record[8] = header_byte_value;
        record[1] = 0;
        record[2] = *D_L17_001B2A54;
    }
    return record;
}

/* Updates moby 99: a beam swinging between two waypoints that sprays particles while the hero is in its zone. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002AA068.c: func_L17_002AA068), where it is exact; names translated to the US level program. */

extern char *D_L17_0015FFD8;
extern float animation_start(int a, unsigned int b) __asm__("FUN_L04_002418b0");
extern float animation_end(int a, unsigned int b) __asm__("FUN_L04_00241910");
extern float fast_cos(float);
extern int   D_L17_0015FFD8_t; /* no foreign declaration */
extern short fleet_beam_endpoint_axis_offset __asm__("D_L17_00161498") __attribute__((sda));
void         draw_fleet_beam_quad(struct Moby *moby) __asm__("FUN_L17_002a95b8");

extern void draw_fleet_beam_quad(struct Moby *moby) __asm__("FUN_L17_002a95b8");
extern void transform_vector_by_basis(void *, void *, void *);

extern char *D_L17_0015FFD0[] MACRO_ADDR;
void         update_fleet_beam(struct Moby *beam_moby) __asm__("FUN_L17_002a8da0");

void update_fleet_beam(struct Moby *beam_moby) {
    enum {
        BEAM_INITIALIZE = 0,
        BEAM_UPDATE_STATIONARY = 1,
        BEAM_UPDATE_OSCILLATING = 2
    };
    float beam_endpoint_and_offset_scratch[4];
    float beam_vector_scratch[4];
    float beam_geometry_scratch[4];
    float hero_offset[4];
    float axis_aligned_hero_offset[4];
    float beam_endpoint_position_scratch[4];
    float hit_descriptor_and_vector_scratch[12];
    char *beam_vars;
    char *beam_endpoint_record;
    int   flag;
    int   i;
    int   k;
    int   j;
    float signed_beam_length;

    if (beam_moby->state == 3)
        return;
    flag = 0;
    beam_vars = (char *)beam_moby->pvars;
    switch (beam_moby->state) {
    case BEAM_INITIALIZE:
        *(float *)(beam_vars + 0x58) = animation_start(beam_moby->class_slot, 0);
        *(float *)(beam_vars + 0x5C) =
            (animation_end(beam_moby->class_slot, 0) - *(float *)(beam_vars + 0x58)) / 6.2831855f;
        k = *(int *)(beam_vars + 0x44);
        if (k != -1) {
            qcopy(beam_vars, &beam_moby->pos);
            *(float *)(beam_vars + 0x54) =
                *(float *)(beam_vars + 0x48) * frame_time /
                fast_vec_dist(beam_vars, D_L17_001600EC + (k << 7) + 0x30);
            beam_moby->state = 2;
        } else if ((j = *(int *)(beam_vars + 0x68)) != -1) {
            qcopy(beam_vars, &beam_moby->pos);
            *(float *)(beam_vars + 0x54) =
                *(float *)(beam_vars + 0x48) * frame_time /
                fast_vec_dist(beam_vars, D_L17_0015FFD0[2] + (j << 8) + 0x10);
            beam_moby->state = 2;
        } else {
            beam_moby->state = 1;
        }
        *(int *)(beam_vars + 0x60) = 0;
        *(float *)(beam_vars + 0x4C) = 0.1f;
        *(OvlQuad *)beam_vector_scratch = 0;
        beam_vector_scratch[0] = 13.0f;
        transform_vector_by_basis(beam_vector_scratch, beam_vector_scratch, &beam_moby->rot_row0);
        fast_vec_add(beam_geometry_scratch, &beam_moby->pos, beam_vector_scratch);
        *(OvlQuad *)beam_endpoint_and_offset_scratch = *(OvlQuad *)beam_geometry_scratch;
        if (collision_line(&beam_moby->pos, beam_endpoint_and_offset_scratch, 2, 0, 0)) {
            *(OvlQuad *)beam_endpoint_and_offset_scratch = *(OvlQuad *)D_L17_00174760;
        }
        fast_vec_sub(beam_geometry_scratch, beam_endpoint_and_offset_scratch, &beam_moby->pos);
        *(OvlQuad *)(beam_vars + 0x30) = *(OvlQuad *)beam_geometry_scratch;
        break;
    case BEAM_UPDATE_STATIONARY:
        if (*(int *)(beam_vars + 0x40) == -1 ||
            is_point_inside_clip_volume(&hero.motion.pos, *(int *)(beam_vars + 0x40))) {
            flag = 1;
        }
        break;
    case BEAM_UPDATE_OSCILLATING:
        *(float *)(beam_vars + 0x50) += *(float *)(beam_vars + 0x54);
        if (*(float *)(beam_vars + 0x50) > 1.0f) {
            *(float *)(beam_vars + 0x50) = 1.0f;
            *(float *)(beam_vars + 0x54) = -*(float *)(beam_vars + 0x54);
        } else if (*(float *)(beam_vars + 0x50) < 0.0f) {
            *(float *)(beam_vars + 0x50) = 0.0f;
            *(float *)(beam_vars + 0x54) = -*(float *)(beam_vars + 0x54);
        }
        if (*(int *)(beam_vars + 0x68) != -1) {
            lerp_vector(&beam_moby->pos,
                        beam_vars,
                        ((char *(*)[64])D_L17_0015FFD8)[*(int *)(beam_vars + 0x68)][0x1E],
                        *(float *)(beam_vars + 0x50));
        } else {
            lerp_vector(&beam_moby->pos,
                        beam_vars,
                        D_L17_001600EC + (*(int *)(beam_vars + 0x44) << 7) + 0x30,
                        *(float *)(beam_vars + 0x50));
        }
        if (*(int *)(beam_vars + 0x40) == -1 ||
            is_point_inside_clip_volume(&hero.motion.pos, *(int *)(beam_vars + 0x40))) {
            *(OvlQuad *)beam_vector_scratch = 0;
            beam_vector_scratch[0] = 13.0f;
            transform_vector_by_basis(
                beam_vector_scratch, beam_vector_scratch, &beam_moby->rot_row0);
            fast_vec_add(beam_geometry_scratch, &beam_moby->pos, beam_vector_scratch);
            *(OvlQuad *)beam_endpoint_and_offset_scratch = *(OvlQuad *)beam_geometry_scratch;
            if (collision_line(&beam_moby->pos, beam_endpoint_and_offset_scratch, 2, 0, 0)) {
                *(OvlQuad *)beam_endpoint_and_offset_scratch = *(OvlQuad *)D_L17_00174760;
            }
            flag = 1;
            fast_vec_sub(beam_geometry_scratch, beam_endpoint_and_offset_scratch, &beam_moby->pos);
            *(OvlQuad *)(beam_vars + 0x30) = *(OvlQuad *)beam_geometry_scratch;
        }
        break;
    }
    *(OvlQuad *)beam_vector_scratch = 0;
    beam_vector_scratch[0] = 12.0f;
    transform_vector_by_basis(beam_vector_scratch, beam_vector_scratch, &beam_moby->rot_row0);
    fast_vec_add(beam_geometry_scratch, &beam_moby->pos, beam_vars + 0x30);
    *(OvlQuad *)beam_endpoint_and_offset_scratch = *(OvlQuad *)beam_geometry_scratch;
    if (*(int *)(beam_vars + 0x60) != 0) {
        fast_vec_normalize(beam_vars + 0x20,
                           beam_vector_scratch,
                           -(*(float *)&fleet_beam_endpoint_axis_offset - 0.2f));
        fast_vec_normalize(*(char **)(beam_vars + 0x60) + 0x10,
                           beam_vector_scratch,
                           -*(float *)&fleet_beam_endpoint_axis_offset);
        fast_vec_add(beam_geometry_scratch, beam_vars + 0x20, beam_endpoint_and_offset_scratch);
        beam_endpoint_record = *(char **)(beam_vars + 0x60);
        *(OvlQuad *)(beam_vars + 0x20) = *(OvlQuad *)beam_geometry_scratch;
        fast_vec_add(
            beam_geometry_scratch, beam_endpoint_record + 0x10, beam_endpoint_and_offset_scratch);
        *(OvlQuad *)(beam_endpoint_record + 0x10) = *(OvlQuad *)beam_geometry_scratch;
    } else {
        *(OvlQuad *)(beam_vars + 0x20) = *(OvlQuad *)beam_endpoint_and_offset_scratch;
    }
    if (flag) {
        *(float *)(beam_vars + 0x4C) =
            fast_cos(fast_add_rotations(
                3.1415927f,
                (compute_interpolated_record_value(beam_moby) - *(float *)(beam_vars + 0x58)) /
                    *(float *)(beam_vars + 0x5C))) *
                0.02f +
            0.09f;
        fast_vec_normalize(beam_vars + 0x10, beam_vector_scratch, 0.2f);
        fast_vec_sub(beam_endpoint_position_scratch, &beam_moby->pos, beam_vars + 0x10);
        *(OvlQuad *)(beam_vars + 0x10) = *(OvlQuad *)beam_endpoint_position_scratch;
        signed_beam_length = fast_vec_length(beam_vars + 0x30);
        lerp_vector(beam_geometry_scratch, beam_vars + 0x10, beam_vars + 0x20, 0.5f);
        beam_geometry_scratch[3] = signed_beam_length * 0.5f;
        if (fast_bsphere_check((char *)beam_geometry_scratch, 32.0f) != -1) {
            fast_vec_sub(hero_offset, &hero.motion.pos, &beam_moby->pos);
            fast_vec_normalize(axis_aligned_hero_offset,
                               &beam_moby->rot_row0,
                               fast_vec_dot(&beam_moby->rot_row0, hero_offset));
            fast_vec_sub(hero_offset, hero_offset, axis_aligned_hero_offset);
            make_hit_descriptor((char *)hit_descriptor_and_vector_scratch,
                                (int)beam_moby,
                                0x10001,
                                1.0f,
                                hero_offset);
            collision_line(
                &beam_moby->pos, beam_vars + 0x20, 9, beam_moby, hit_descriptor_and_vector_scratch);
            signed_beam_length = fast_vec_length(beam_vars + 0x30);
            *(OvlQuad *)beam_endpoint_position_scratch = *(OvlQuad *)(beam_vars + 0x10);
            for (i = 0; i < 7; i++) {
                fast_vec_scale(beam_endpoint_and_offset_scratch,
                               beam_vars + 0x30,
                               random_float_between(0.0f, 0.9f));
                add_vectors(hit_descriptor_and_vector_scratch,
                            beam_endpoint_and_offset_scratch,
                            beam_endpoint_position_scratch);
                fast_vec_normalize(
                    beam_vector_scratch,
                    beam_vars + 0x30,
                    random_float_between(frame_time * 10.0f, signed_beam_length / 20.0f));
                spawn_fleet_beam_particle(
                    beam_moby, beam_endpoint_and_offset_scratch, beam_vector_scratch);
            }
            signed_beam_length = -signed_beam_length;
            *(OvlQuad *)beam_endpoint_position_scratch = *(OvlQuad *)(beam_vars + 0x20);
            for (i = 0; i < 8; i++) {
                fast_vec_scale(beam_endpoint_and_offset_scratch,
                               beam_vars + 0x30,
                               random_float_between(0.0f, 0.9f));
                fast_vec_sub(hit_descriptor_and_vector_scratch,
                             beam_endpoint_position_scratch,
                             beam_endpoint_and_offset_scratch);
                *(OvlQuad *)beam_endpoint_and_offset_scratch =
                    *(OvlQuad *)hit_descriptor_and_vector_scratch;
                fast_vec_normalize(
                    beam_vector_scratch,
                    beam_vars + 0x30,
                    random_float_between(frame_time * 10.0f, signed_beam_length / 20.0f));
                spawn_fleet_beam_particle(
                    beam_moby, beam_endpoint_and_offset_scratch, beam_vector_scratch);
            }
            fast_vec_normalize(beam_vector_scratch,
                               beam_vars + 0x30,
                               -random_float_between(frame_time * 0.5f, frame_time));
            beam_vector_scratch[0] += random_float_between(-0.01f, 0.01f);
            beam_vector_scratch[1] += random_float_between(-0.01f, 0.01f);
            beam_vector_scratch[2] += random_float_between(-0.01f, 0.01f);
            spawn_fleet_beam_particle(0, beam_vars + 0x20, beam_vector_scratch);
        }
        add_draw_callback(draw_fleet_beam_quad, beam_moby);
    }
}

typedef struct {
    char         pad[0x40];
    unsigned int w[4];
    struct {
        float a, b;
    } f[4];
    long z70;
    long z78;
    long q80;
    long q88;
} S_a95b8;

extern int  get_effect_texture(int) __asm__("FUN_001f44b8");
extern void func_L00_001FD1D8_a95b8(void *, void *, int) __asm__("FUN_001f7d30");

extern struct {
    float a, b;
} D_L17_001E6620_a95b8[4] __asm__("D_L17_001E6320");

void draw_fleet_beam_quad(struct Moby *moby) {
    char   *moby_vars = (char *)moby->pvars;
    S_a95b8 quad_primitive;
    char    beam_start_vertex_offset[16];
    char    beam_end_vertex_offset[16];
    char    beam_end_to_start_vector[16];
    char    quad_vertex_scratch[16];
    int     i;
    fast_vec_sub(beam_end_to_start_vector, moby_vars + 0x10, moby_vars + 0x20);
    fast_vec_sub(beam_start_vertex_offset, moby_vars + 0x10, D_L17_001676C0);
    fast_vec_sub(beam_end_vertex_offset, moby_vars + 0x20, D_L17_001676C0);
    fast_vec_cross(beam_start_vertex_offset, beam_start_vertex_offset, beam_end_to_start_vector);
    fast_vec_normalize(
        beam_start_vertex_offset, beam_start_vertex_offset, *(float *)(moby_vars + 0x4C));
    fast_vec_cross(beam_end_vertex_offset, beam_end_vertex_offset, beam_end_to_start_vector);
    fast_vec_normalize(
        beam_end_vertex_offset, beam_end_vertex_offset, *(float *)(moby_vars + 0x4C));
    quad_primitive.z78 = get_effect_texture(0xE);
    quad_primitive.q80 = 0xFF9000000260L;
    quad_primitive.q88 = 0x8000000048L;
    quad_primitive.z70 = 0;
    quad_primitive.w[3] = 0x80243278;
    quad_primitive.w[2] = 0x80243278;
    quad_primitive.w[1] = 0x80243278;
    quad_primitive.w[0] = 0x80243278;
    for (i = 0; i < 4; i++) {
        quad_primitive.f[i].a = D_L17_001E6620_a95b8[i].a;
        quad_primitive.f[i].b = D_L17_001E6620_a95b8[i].b;
    }
    fast_vec_sub(quad_vertex_scratch, moby_vars + 0x10, beam_start_vertex_offset);
    *(OvlQuad *)(quad_primitive.pad + 0) = *(OvlQuad *)quad_vertex_scratch;
    fast_vec_add(quad_vertex_scratch, moby_vars + 0x10, beam_start_vertex_offset);
    *(OvlQuad *)(quad_primitive.pad + 0x10) = *(OvlQuad *)quad_vertex_scratch;
    fast_vec_sub(quad_vertex_scratch, moby_vars + 0x20, beam_end_vertex_offset);
    *(OvlQuad *)(quad_primitive.pad + 0x20) = *(OvlQuad *)quad_vertex_scratch;
    fast_vec_add(quad_vertex_scratch, moby_vars + 0x20, beam_end_vertex_offset);
    *(OvlQuad *)(quad_primitive.pad + 0x30) = *(OvlQuad *)quad_vertex_scratch;
    func_L00_001FD1D8_a95b8(&quad_primitive, 0, 0);
}
