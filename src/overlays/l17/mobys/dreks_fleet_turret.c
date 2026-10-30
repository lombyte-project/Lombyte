/* Level 17 moby classes: dreks_fleet_turret. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/game/game_globals.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/test_path_segment_strict_crossing.h"
#include "rnc/overlay/shared/math/interpolation/compute_directed_angle_delta.h"
#include "rnc/overlay/shared/math/rotations/clip_negative_segment_endpoint_xyz.h"
#include "rnc/overlay/shared/math/vectors/add_vectors.h"
#include "rnc/world/collision/collision_line.h"
#include "rnc/math/vectors/normalize_vector_triplet.h"

/* Steps a moby's countdown from a helper's output and retires it when it runs out. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002AA068.c: func_L17_002CD088), where it is exact; names translated to the US level program. */

extern int
hit_reaction(void *, void *, void *, int, void *, void *, int, int) __asm__("FUN_L00_0025a478");
extern void trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");

void process_dreks_fleet_turret_hits(struct Moby *turret_moby) __asm__("FUN_L17_002cbc10");

void process_dreks_fleet_turret_hits(struct Moby *turret_moby) {
    char *turret_vars;
    char *hit_animation_state;
    char *hit_record;
    int   hit_reaction_status;
    float fv;
    int   depletion_status;
    turret_vars = ((char *)turret_moby->pvars);
    if (turret_moby->state != 0) {
        fv = 0.0f;
        hit_record = find_moby_hit(turret_moby, 0x330000, 0);
        if (hit_record != 0) {
            if (*(int *)(hit_record + 0x20) != 0) {
                int hit_moby_class_id = *(short *)(*(char **)(hit_record + 0x20) + 0xA6);
                if (hit_moby_class_id != 0x3F1) {
                    if (hit_moby_class_id != 0x127)
                        hit_record = 0;
                }
            }
        }
        hit_animation_state = turret_vars + 0x60;
        hit_reaction(
            turret_moby, hit_record, turret_vars + 0x20, 0, &hit_reaction_status, &fv, 0, 4);
        if (hit_reaction_status != 1 && turret_moby->state != 4) {
            float remaining_value = *(float *)(turret_vars + 0x20) - fv;
            *(float *)(turret_vars + 0x20) = remaining_value;
            depletion_status = 0xB;
            if (remaining_value <= 0.0f)
                depletion_status = 1;
            if (depletion_status == 1) {
                turret_moby->state = 4;
                turret_moby->flags &= 0xEFFF;
            }
            *(unsigned char *)(turret_vars + 0x67) = 0xF0;
            trooper_anim_update(turret_moby, (short *)hit_animation_state);
        }
        turret_moby->hit_slot = 0xFF;
        trooper_anim_finish(turret_moby, hit_animation_state);
    }
}

#include "qcopy.h"
#include "rnc/overlay/quad.h"

/* Updates moby 99: a beam swinging between two waypoints that sprays particles while the hero is in its zone. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002AA068.c: func_L17_002AA068), where it is exact; names translated to the US level program. */

extern char *D_L17_0015FFD8;
extern float animation_start(int a, unsigned int b) __asm__("FUN_L04_002418b0");
extern float animation_end(int a, unsigned int b) __asm__("FUN_L04_00241910");
extern float fast_cos(float);
extern int   D_L17_0015FFD8_t; /* no foreign declaration */
extern short fleet_beam_endpoint_axis_offset __asm__("D_L17_00161498") __attribute__((sda));
extern unsigned char      *
spawn_trail_particle(char *parent, void *pos, float *vec) __asm__("FUN_L17_0026fb60");
void draw_fleet_beam_quad(struct Moby *moby) __asm__("FUN_L17_002a95b8");

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
                spawn_trail_particle(
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
                spawn_trail_particle(
                    beam_moby, beam_endpoint_and_offset_scratch, beam_vector_scratch);
            }
            fast_vec_normalize(beam_vector_scratch,
                               beam_vars + 0x30,
                               -random_float_between(frame_time * 0.5f, frame_time));
            beam_vector_scratch[0] += random_float_between(-0.01f, 0.01f);
            beam_vector_scratch[1] += random_float_between(-0.01f, 0.01f);
            beam_vector_scratch[2] += random_float_between(-0.01f, 0.01f);
            spawn_trail_particle(0, beam_vars + 0x20, beam_vector_scratch);
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

#include "qcopy.h"

#include "qzero.h"

/* Updates moby 347, a turret of Drek's fleet: waits for the hero, aims at its zone or at the hero's ship and fires. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002AA068.c: func_L17_002CC788), where it is exact; names translated to the US level program. */

typedef struct {
    char  pad0[0x20];
    float f20;
    short s24;
    char  pad26[0x4A];
    int   zone;
    int   timer;
    float yawvel;
    int   list;
    short group;
    char  pad82[2];
    int   path;
    char  pad88[8];
    char  a90[0x44];
    float fD4;
    char  padD8[0x1C];
    float fF4;
} Vars347;

typedef struct {
    float pos[4];
    float rot[4];
    float v20[4];
    float v30[4];
    char *moby;
    int   pad[3];
} Aim347;

extern char *FUN_L00_0026dd20(char *pos, char *vel, int c, int d, float f);
extern char *spawn_class_558_moby_toward_target(char  *src,
                                                char  *pos,
                                                float *v,
                                                float  f) __asm__("FUN_L17_002e94d8");
extern char *try_activate_blarg_ship_from_list(int list) __asm__("FUN_L17_002f4fb8");
extern float vector_length_xy(void *);
extern char  fleet_turret_aim_vertex_indices[4] __asm__("D_L17_00161980") __attribute__((sda));
extern void *FUN_L00_00263fd8(char  *src,
                              int    cls,
                              float *pos,
                              void  *mat,
                              int    a8,
                              int    a9,
                              float  scale,
                              float *v10,
                              float *v11,
                              float *v12);
extern void  select_moby_combat_target(void *, void *, float) __asm__("FUN_L00_0025fcb8");
extern void  process_dreks_fleet_turret_hits(struct Moby *m) __asm__("FUN_L17_002cbc10");
void         distribute_class_733_group_mobys_along_path(struct Moby *turret_moby) __asm__("FUN_L17_002cbd38");

extern void distribute_class_733_group_mobys_along_path(struct Moby *m) __asm__("FUN_L17_002cbd38");
extern void
transform_scaled_vertex_batch(char *arg0, int arg1, void *arg2, char *arg3) __asm__("FUN_0020cd48");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

void dreks_fleet_turret_update(struct Moby *m) __asm__("FUN_L17_002cb310");

void dreks_fleet_turret_update(struct Moby *turret_moby) {
    enum {
        FLEET_TURRET_INITIALIZE = 0,
        FLEET_TURRET_WAIT_FOR_TARGET = 1,
        FLEET_TURRET_AIM_AND_FIRE_AT_ZONE = 2,
        FLEET_TURRET_AIM_AND_FIRE_AT_HERO_SHIP = 3,
        FLEET_TURRET_DESTROY = 4
    };
    float    targeting_vector_scratch[4];
    float    from[4];
    Vars347 *turret_vars = (Vars347 *)turret_moby->pvars;

    process_dreks_fleet_turret_hits(turret_moby);
    switch (turret_moby->state) {
    case FLEET_TURRET_INITIALIZE:
        turret_moby->state = 1;
        turret_vars->f20 = 15.0f;
        turret_vars->s24 = 0xF;
        distribute_class_733_group_mobys_along_path(turret_moby);
        if (visited52[turret_moby->mission + (current_level_index << 4)] == 0xFF) {
            mark_moby_for_removal(turret_moby);
        }
        break;
    case FLEET_TURRET_WAIT_FOR_TARGET: {
        struct Hero *g = &hero;
        if (g->unk308 != 0) {
            qcopy(targeting_vector_scratch, &turret_moby->pos);
            targeting_vector_scratch[2] += 10.0f;
            fast_vec_sub(
                from, D_L17_001600EC + (turret_vars->zone << 7) + 0x30, targeting_vector_scratch);
            fast_vec_scale(from, from, 0.9f);
            fast_vec_add(from, from, targeting_vector_scratch);
            from[2] += 0.5f;
            if (collision_line(targeting_vector_scratch, from, 2, turret_moby, 0) == 0) {
                turret_moby->state = 2;
            }
        } else if (g->state.current == 0x32) {
            char *hero_ship_moby = ((char *)g->ship_moby);
            if (hero_ship_moby != 0 && MOBY(hero_ship_moby)->oclass == 0x563) {
                if (vector_distance_xy(&turret_moby->pos, &g->motion.pos) < 254.0f) {
                    turret_moby->state = 3;
                }
            }
        }
        break;
    }
    case FLEET_TURRET_AIM_AND_FIRE_AT_ZONE: {
        if (hero.unk308 == 0) {
            turret_moby->state = 1;
        }
        {
            float  j[8];
            float  acc[4];
            float  tmp[4];
            float  zero[4];
            float *yaw;
            float  ang;
            float  turn;
            float  off;

            transform_scaled_vertex_batch(
                turret_moby, 2, fleet_turret_aim_vertex_indices, (char *)j);
            fast_vec_scale(
                tmp, D_L17_001600EC + (turret_vars->zone << 7), random_float_between(0.0f, 1.0f));
            fast_vec_add(acc, tmp, D_L17_001600EC + (turret_vars->zone << 7) + 0x30);
            fast_vec_scale(tmp,
                           D_L17_001600EC + (turret_vars->zone << 7) + 0x10,
                           random_float_between(0.0f, 1.0f));
            fast_vec_add(acc, acc, tmp);
            fast_vec_scale(tmp,
                           D_L17_001600EC + (turret_vars->zone << 7) + 0x20,
                           random_float_between(0.0f, 1.0f));
            fast_vec_add(acc, acc, tmp);
            fast_vec_sub(targeting_vector_scratch, acc, j);
            qcopy(from, j + 4);
            yaw = &turret_moby->rot.z;
            ang = fast_arc_tan(targeting_vector_scratch[0], targeting_vector_scratch[1]);
            turn = frame_time_sq * 0.5235988f;
            approach_angle(yaw, &turret_vars->yawvel, ang, turn, turn, frame_time * 1.0471976f);
            turret_vars->fF4 = -fast_arc_tan(vector_length_xy(targeting_vector_scratch),
                                             targeting_vector_scratch[2]);
            update_joint_animation(
                turret_moby, turret_vars->a90, 0, frame_scale_sq * 0.02f, frame_scale_sq * 0.3f);
            off = fast_difference_between_rotations(
                turret_vars->fD4,
                -fast_arc_tan(vector_length_xy(targeting_vector_scratch),
                              targeting_vector_scratch[2]));
            if (D_L17_0015F5CC % scale_game_frames(0x5A) == turret_vars->timer &&
                off < 0.034906585f) {
                if (fast_difference_between_rotations(
                        turret_moby->rot.z,
                        fast_arc_tan(*(float *)(D_L17_001600EC + turret_vars->zone * 0x80 + 0x30) -
                                         turret_moby->pos.x,
                                     *(float *)(D_L17_001600EC + turret_vars->zone * 0x80 + 0x34) -
                                         turret_moby->pos.y)) < 0.034906585f) {
                    char *p;
                    fast_vec_normalize(j, targeting_vector_scratch, frame_time * 60.0f);
                    fast_vec_add(j + 4, j, j + 4);
                    qzero(zero);
                    p = FUN_L00_0026dd20((char *)(j + 4), (char *)zero, 0x80FF8F6F, 3, 1260000.0f);
                    if (p != 0) {
                        p[9] = func_001FA898_r(4.0f) + 0x70;
                    }
                    allocate_voice_for_target_entry(0, 0, (int)turret_moby);
                    spawn_class_558_moby_toward_target(
                        turret_moby,
                        (char *)from,
                        (float *)(D_L17_001600EC + (turret_vars->zone << 7) + 0x30),
                        frame_time * 60.0f);
                }
            }
        }
        break;
    }
    case FLEET_TURRET_AIM_AND_FIRE_AT_HERO_SHIP: {
        struct Hero *g = &hero;
        if (g->state.current != 0x32 || ((char *)g->ship_moby) == 0 ||
            *(short *)(((char *)g->ship_moby) + 0xA6) != 0x563) {
            turret_moby->state = 1;
        } else {
            Aim347 aim;
            float  j[8];
            float  zero[4];
            float  tmp[4];
            float *yaw;
            float  ang;
            float  turn;
            float  off;

            transform_scaled_vertex_batch(
                turret_moby, 2, fleet_turret_aim_vertex_indices, (char *)j);
            select_moby_combat_target(turret_moby, &aim, 512.0f);
            fast_vec_sub(targeting_vector_scratch, aim.pos, j);
            qcopy(from, j + 4);
            yaw = &turret_moby->rot.z;
            ang = fast_arc_tan(targeting_vector_scratch[0], targeting_vector_scratch[1]);
            turn = frame_time_sq * 0.5235988f;
            approach_angle(yaw, &turret_vars->yawvel, ang, turn, turn, frame_time * 1.0471976f);
            turret_vars->fF4 = -fast_arc_tan(vector_length_xy(targeting_vector_scratch),
                                             targeting_vector_scratch[2]);
            update_joint_animation(
                turret_moby, turret_vars->a90, 0, frame_scale_sq * 0.02f, frame_scale_sq * 0.3f);
            off = fast_difference_between_rotations(
                turret_vars->fD4,
                -fast_arc_tan(vector_length_xy(targeting_vector_scratch),
                              targeting_vector_scratch[2]));
            if (fast_dec_timer(&turret_vars->timer) && off < 0.034906585f) {
                if (fast_difference_between_rotations(
                        turret_moby->rot.z,
                        fast_arc_tan(aim.pos[0] - turret_moby->pos.x,
                                     aim.pos[1] - turret_moby->pos.y)) < 0.034906585f) {
                    if (collision_line(from, aim.pos, 2, ((char *)g->ship_moby), 0) == 0) {
                        char *p;
                        qzero(zero);
                        fast_vec_sub(tmp, j + 4, j);
                        fast_vec_normalize(tmp, tmp, 250.0f);
                        fast_vec_add(tmp, tmp, j + 4);
                        p = FUN_L00_0026dd20(
                            (char *)(j + 4), (char *)zero, 0x80FF8F6F, 3, 1260000.0f);
                        if (p != 0) {
                            p[9] = func_001FA898_r(4.0f) + 0x70;
                        }
                        turret_vars->timer = scale_game_frames(0x5A) / 2;
                        allocate_voice_for_target_entry(0, 0, (int)turret_moby);
                        spawn_class_558_moby_toward_target(
                            turret_moby, (char *)from, tmp, frame_time * 60.0f);
                    }
                }
            }
            if (turret_vars->list != 0 && random_integer_below(0x1F4) == 0) {
                try_activate_blarg_ship_from_list(turret_vars->list);
            }
        }
        break;
    }
    case FLEET_TURRET_DESTROY:
        FUN_L00_00263fd8(turret_moby,
                         0x63A,
                         &turret_moby->pos.x,
                         &turret_moby->rot,
                         0,
                         0,
                         0.0f,
                         D_L17_0015F580,
                         D_L17_0015F580,
                         D_L17_0015F580);
        FUN_L00_00263fd8(turret_moby,
                         0x63B,
                         &turret_moby->pos.x,
                         &turret_moby->rot,
                         0,
                         0,
                         0.0f,
                         D_L17_0015F580,
                         D_L17_0015F580,
                         D_L17_0015F580);
        cart_burst(turret_moby, &turret_moby->pos, -1, 10.0f, 13.0f);
        mark_moby_for_removal(turret_moby);
        break;
    }
}

/* Second, %hi/%lo read of the moby array (retail reloads it for the placement pass). */
extern char *D_L17_0015FFD8_macro __asm__("D_L17_0015FFD8") MACRO_ADDR;

/* Spreads the class-0x733 mobies of this moby's group evenly along its path. */
void distribute_class_733_group_mobys_along_path(struct Moby *turret_moby) {
    char  *turret_vars;
    char  *path;
    char  *mobys;
    char  *matched_moby;
    char  *od;
    short *group_entry;
    int    i;
    short  class_733_moby_count;
    short  k;
    short  step;

    turret_vars = ((char *)turret_moby->pvars);
    class_733_moby_count = 0;
    k = 0;
    i = *(short *)(turret_vars + 0x80);
    if (i == -1)
        return;
    group_entry = D_L17_001AC140[i];
    if (group_entry == 0)
        return;
    do {
        if (*(short *)(((*group_entry & 0x7FFF) << 8) + 0xA6 + D_L17_0015FFD8) == 0x733)
            class_733_moby_count++;
    } while (*group_entry++ >= 0);
    path = ((char **)D_L17_001B0DB0)[*(int *)(turret_vars + 0x84)];
    step = *(int *)path / class_733_moby_count;
    group_entry = D_L17_001AC140[i];
    mobys = D_L17_0015FFD8_macro;
    do {
        i = (*group_entry & 0x7FFF) << 8;
        if (*(short *)(i + 0xA6 + mobys) == 0x733) {
            matched_moby = i + mobys;
            od = ((char *)MOBY(matched_moby)->pvars);
            qcopy(matched_moby + 0x10, path + 0x10 + k * 0x10);
            *(int *)(od + 0x60) = *(int *)(turret_vars + 0x84);
            *(int *)(od + 0x64) = k;
            *(int *)(od + 0x68) = 0;
            k += step;
        }
    } while (*group_entry++ >= 0);
}

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

void update_fleet_rotating_radial_effect(char *moby) __asm__("FUN_L17_002d77f0");

void update_fleet_rotating_radial_effect(char *moby) {
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
                                particle_lifetime = func_001FA898_r(scale_time(random_float_between(
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

#include "rnc/overlay/l17/mobys/dreks_fleet_turret_globals.h"

extern float fleet_radial_uv_scroll_rate_gp __asm__("D_L17_00161BBC") __attribute__((sda));
extern float FUN_L00_00200260(float, float);
extern int   fleet_radial_alpha_base_value_gp __asm__("D_L17_00161B98") __attribute__((sda));

#include "rnc/overlay/l17/mobys/dreks_fleet_turret_data/draw_fleet_scrolling_radial_quads.h"

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

#include "qcopy.h"

/* Updates moby 835: a timed effect that spawns a spray of particles and deletes itself. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002AA068.c: func_L17_002DD8D0), where it is exact; names translated to the US level program. */

extern char *D_L17_00174758;
extern char *FUN_L00_0026f080(char *vec, float *pos, float sc, float vy);
extern int   collect_mobies_in_sphere() __asm__("FUN_L00_001f2868");
extern void  spawn_explosion(void *,
                             void *,
                             void *,
                             float,
                             float,
                             int,
                             int,
                             int,
                             float,
                             float,
                             float,
                             float,
                             int,
                             float,
                             int,
                             int,
                             int,
                             int) __asm__("FUN_L00_0025e450");
extern short D_L17_00161D2C __attribute__((sda));
extern short fleet_bobbing_phase_rate_degrees __asm__("D_L17_00161D30") __attribute__((sda));
extern void  update_moby_vertical_oscillation(float, float, char *, float *, float *) __asm__("FUN_L00_00262b00");
f32        resolve_camera_surface_height(struct CameraProbePosition *position,
                                         s32 optional_output) __asm__("FUN_002135f0");
extern int func_L00_001F2BE8_alt(float, void *, int, void *, void *) __asm__("FUN_L00_001f2868");

void update_fleet_bobbing_moby(struct Moby *moby) __asm__("FUN_L17_002dc3f8");

void update_fleet_bobbing_moby(struct Moby *moby) {
    enum {
        BOBBING_INITIALIZE = 0,
        BOBBING_UPDATE_AND_CHECK_CONTACT = 1
    };
    float        zero_vector[4];
    float        hit_descriptor[12];
    float        pos[4];
    float        surface_query_scratch[4];
    float        scaled_particle_vector[4];
    float        random_direction[4];
    int          out;
    char        *data = (char *)moby->pvars;
    char        *hit_record;
    struct Hero *hero_contact_context;
    float        yy;
    int          i;
    clear_u64_value(zero_vector);
    hit_record = find_moby_hit(moby, 0x230000, 0);
    hit_reaction(moby, hit_record, data + 0x20, 0, &out, 0, 0, 4);
    if (out >= 2 && hit_record != 0 && *(short *)(*(char **)(hit_record + 0x20) + 0xA6) != 0x343) {
        spawn_explosion(moby,
                        zero_vector,
                        0,
                        0.0f,
                        0.0f,
                        3,
                        3,
                        5,
                        2.0f,
                        1.0f,
                        4.0f,
                        1.0f,
                        1,
                        7.0f,
                        0,
                        1,
                        -1,
                        0);
        mark_moby_for_removal(moby);
        return;
    }
    switch (moby->state) {
    case BOBBING_INITIALIZE:
        *(float *)(data + 0x60) = random_angle_radians();
        *(int *)(data + 0x64) = 0;
        moby->state = 1;
        break;
    case BOBBING_UPDATE_AND_CHECK_CONTACT:
        update_moby_vertical_oscillation(*(float *)&D_L17_00161D2C,
                                         *(float *)&fleet_bobbing_phase_rate_degrees * DEG_TO_RAD *
                                             frame_time,
                                         moby,
                                         (float *)(data + 0x60),
                                         (float *)(data + 0x64));
        make_hit_descriptor(hit_descriptor, (int)moby, 0x10001, 1.0f, zero_vector);
        qcopy(pos, &moby->pos);
        pos[2] = pos[2] + 0.75f;
        hero_contact_context = &hero;
        if (((char *)hero_contact_context->coll_hit_moby) == moby ||
            ((char *)hero_contact_context->unk240) == moby ||
            func_L00_001F2BE8_alt(1.0f, pos, 0x10, moby, hit_descriptor)) {
            if (D_L17_00174758 != 0 && *(short *)(D_L17_00174758 + 0xA6) != 0x343) {
                spawn_explosion(moby,
                                zero_vector,
                                0,
                                0.0f,
                                0.0f,
                                3,
                                3,
                                5,
                                2.0f,
                                1.0f,
                                4.0f,
                                1.0f,
                                1,
                                7.0f,
                                0,
                                1,
                                -1,
                                0);
                moby->pos.z = moby->pos.z + 1.0f;
                yy = resolve_camera_surface_height(&moby->pos.x, surface_query_scratch);
                moby->pos.z = moby->pos.z - 1.0f;
                i = 0x95;
                do {
                    *(OvlQuad *)random_direction = 0;
                    random_direction[0] = random_float_between(-1.0f, 1.0f);
                    random_direction[1] = random_float_between(-1.0f, 1.0f);
                    random_direction[2] = random_float_between(-0.5f, 2.0f);
                    *(OvlQuad *)scaled_particle_vector = *(OvlQuad *)random_direction;
                    fast_vec_normalize(scaled_particle_vector,
                                       scaled_particle_vector,
                                       random_float_between(frame_time * 4.0f, frame_time * 8.0f));
                    FUN_L00_0026f080(&moby->pos,
                                     scaled_particle_vector,
                                     random_float_between(0.05f, 0.1f) * 210000.0f,
                                     yy);
                } while (--i >= 0);
                mark_moby_for_removal(moby);
            }
        }
        break;
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
