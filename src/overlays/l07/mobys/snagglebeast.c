/* Level 07 moby class 0x452: snagglebeast. */
#include "types.h"
#include "rnc/overlay/shared/mobys/hit_color_flash.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/mobys/moby.h"
#include "rnc/data_labels.h"
#include "rnc/game/options.h"
#include "qcopy.h"
#include "rnc/rendering/geometry_quad.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/overlay/shared/world/level_labels.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/rendering/effect_texture.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/mobys/test_path_segment_strict_crossing.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/rendering/view/build_look_at_matrix.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/animation/set_animation_parameter.h"
#include "rnc/mobys/attach_manipulator.h"
#include "rnc/math/rotations/matrix_to_euler.h"
#include "rnc/overlay/shared/game/restore_or_clear_level_state.h"
#include "rnc/overlay/shared/rendering/effects/create_part.h"
#include "rnc/overlay/shared/rendering/effects/glow_quads_draw.h"
#include "rnc/overlay/shared/rendering/effects/spawn_type_10_particle.h"
#include "rnc/world/collision/collision_line.h"
#include "rnc/mobys/animation/get_moby_joint_matrices.h"
#include "rnc/overlay/shared/cutscenes/update_npc_dialogue.h"
#include "rnc/overlay/shared/ui/hud/hud_update_counter.h"
#include "rnc/overlay/shared/camera/init_camera_system.h"
#include "rnc/overlay/shared/mobys/build_symmetric_adjacency_masks.h"
#include "rnc/overlay/shared/math/split_float_fraction.h"

/* Ported from rac1-decomp (src/overlays/l07_umbris/effect_00313D28.c: func_L07_00313D28). */
enum {
    SNAGGLEBEAST_INITIALIZE = 0,
    SNAGGLEBEAST_IDLE = 1,
    SNAGGLEBEAST_ALTERNATING_JOINT_EMISSION_BURST = 2,
    SNAGGLEBEAST_TRIPLE_SPAWN_ATTACK = 3,
    SNAGGLEBEAST_SWEEP_BEAM_ATTACK = 4,
    SNAGGLEBEAST_CHASE_TARGET = 5,
    SNAGGLEBEAST_CHASE_AFTER_TRANSFORM_RESTORE = 6,
    SNAGGLEBEAST_TURN_TOWARD_TARGET = 7,
    SNAGGLEBEAST_CAPTURE_HERO_AND_DRAIN_AMMO = 8,
    SNAGGLEBEAST_ROPE_ATTACK = 9,
    SNAGGLEBEAST_DEATH_EXPLOSION = 10,
    SNAGGLEBEAST_WAIT_AFTER_HEALTH_THRESHOLD_HIT = 11,
    SNAGGLEBEAST_START_TIMED_DAMAGE_SEQUENCE = 12,
    SNAGGLEBEAST_WAIT_BEFORE_TIMED_DAMAGE = 13,
    SNAGGLEBEAST_APPLY_TIMED_DAMAGE_AND_SELECT_PATH = 14,
    SNAGGLEBEAST_RESTORE_TRANSFORM_AND_RESUME_CHASE = 15,
    SNAGGLEBEAST_POST_DEATH_CLEANUP = 16
};

extern s32 level_frame_count __asm__("D_L07_0015F5CC");

/* [4] is depth_buffer_address. */
extern s32   gs_texture_allocation_start_macro[6] __asm__("D_0015EE78") MACRO_ADDR;
extern void  vu1_add_g_sregister(s32, u64) __asm__("FUN_00233980");
extern void  draw_geometry_quad(void *, int, int) __asm__("FUN_001f7d30");
extern float normalize_angle(float) __asm__("FUN_L00_0025e310");

typedef struct SnagglebeastVars {
    u8                   pad_0[0x20];
    f32                  health;
    s16                  max_health;
    s16                  unk26;
    u8                   pad_28[0x1];
    u8                   unk29;
    u8                   pad_2A[0x2E];
    u8                   unk58;
    u8                   pad_59[0x1];
    u8                   unk5A;
    u8                   pad_5B[0x5];
    struct HitColorFlash hit_flash;
    u8                   pad_70[0x14];
    s32                  unk84;
    u8                   pad_88[0x10];
    f32                  turn_start_angle;
    f32                  unk9C;
    u8                   pad_A0[0x8];
    s32                  unkA8;
    u8                   pad_AC[0x14];
    f32                  turn_target[4];
    u8                   pad_D0[0x30];
    f32                  velocity[4];
    f32                  rope_start_position[4];
    f32                  rope_end_position[4];
    f32                  rope_tip_velocity[4];
    f32                  rope_tip_position[4];
    f32                  rope_target[4]; /* 3 above the position of hero.ground_moby */
    s32                  timer;
    s32                  substate;
    s32                  turn_controller_state;
    f32                  rope_extension_length;
    s32                  navigation_ids[6];
    s32                  nav_path;
    s32                  measured_path;
    f32                  measured_path_length;
    s16                  timer_194;
    s16                  timer_196;
    s16                  timer_198;
    s16                  timer_19A;
    u8                   navigation_mask;
    u8                   path_select;
    s16                  ammo_drain_step;
    s32                  path_1A0;
    s32                  hero_volume; /* clip volume: updates only while the hero is inside */
    s32                  target_nav_point;
    f32                  drop_path_distance;
    s32                  health_hud_handle;
    s32                  health_display;
    f32                  ground_z;
    u8                   pad_1BC[0x4];
    s32                  nav_point;
    u8                   hero_captured;
    u8                   sphere_fade_direction;
    u8                   sphere_shade;
    u8                   hero_capture_attack_enabled;
    f32                  sphere_fade;
    s32                  death_effect_moby;
    s32                  death_raised_moby; /* raised to z 34.6 after the death */
    u8                   pad_1D4[0x1C];
    s32                  cuboid;
    u8                   turn_saved_state;
    u8                   turn_fixed_target;
    u8                   turn_resume_animation;
    u8                   timer_1F7;
    s32                  voice;
    f32                  damage_reaction_amount;
    u8                   joint_animation[0x64];
    f32                  joint_damage_reaction_target;
    f32                  target_yaw_offset;
    u8                   pad_26C[0x14];
    s16                  point_light;
    u8                   pad_282[0xE];
    s32                  chase_path;
    s32                  capture_turn_done; /* 1 once the capture turn toward the hero ran */
    s32                  chase_frames;
    s32                  path_1A0_volume; /* the chase uses path_1A0 while the hero is inside */
    u8                   manipulator[1];
} SnagglebeastVars;

static inline int tint_color(int color) {
    int tinted = color | 0x404040;
    return tinted;
}

/* Draws a textured sphere around point 9 of the moby, one ring of latitude strips per
   longitude step, its alpha ramped from alpha to end_alpha. */
void draw_snagglebeast_sphere_quad_strips(char *moby,
                                          int   phase,
                                          float speed,
                                          float radius,
                                          float latitude_span,
                                          float alpha,
                                          float end_alpha) __asm__("FUN_L07_00312948");

void draw_snagglebeast_sphere_quad_strips(char *moby,
                                          int   phase,
                                          float speed,
                                          float radius,
                                          float latitude_span,
                                          float alpha,

                                          float end_alpha) {
    SnagglebeastVars   *sphere_moby_vars = (SnagglebeastVars *)MOBY(moby)->pvars;
    int                 shade = sphere_moby_vars->sphere_shade;
    int                 grey = (shade << 24) | (shade << 16) | (shade << 8) | shade;
    u64                 texture_state = 0xFF9000000260L;
    u64                 primitive = 0xA000000029L;
    struct GeometryQuad quad;
    Vec4                origin;
    Vec4                center;
    float               whole_part;
    u64                 texture;
    int                 color;
    float alpha_step, strip_start_alpha_scale, spin, latitude_step, angle_step, angle, next_angle,
        longitude;
    float latitude, row_alpha;

    vu1_add_g_sregister(0x4E, (u64)(depth_buffer_address >> 13) | 0x1000000L | (0x8000L << 17));
    texture = get_effect_texture(0x3C);
    row_alpha = alpha;
    color = truncate_float_to_s32(convert_integer_to_float(0) * row_alpha) << 24;
    color |= tint_color(grey);
    quad.colors[3] = color;
    quad.colors[2] = color;
    quad.colors[1] = color;
    quad.colors[0] = color;
    quad.texture = texture;
    quad.texture_state = texture_state;
    quad.primitive = primitive;
    quad.reserved.value = 0;
    spin = convert_integer_to_float(level_frame_count + phase) * speed;
    moby_point_position(moby, 9, &origin);
    qcopy(&center, &origin);
    center.f[2] = center.f[2] - (radius - 0.8f);
    quad.texture_coordinates[0][0] = split_float_fraction(&whole_part, spin * -0.01f);
    quad.texture_coordinates[2][0] = quad.texture_coordinates[0][0];
    quad.texture_coordinates[1][0] = quad.texture_coordinates[0][0] + 1.0f;
    quad.texture_coordinates[3][0] = quad.texture_coordinates[1][0];
    quad.texture_coordinates[0][1] = split_float_fraction(&whole_part, spin * -0.02f);
    quad.texture_coordinates[1][1] = quad.texture_coordinates[0][1];
    quad.texture_coordinates[2][1] = quad.texture_coordinates[0][1] + 1.0f;
    quad.texture_coordinates[3][1] = quad.texture_coordinates[2][1];
    alpha_step = (end_alpha - row_alpha) * 0.0625f;
    latitude_step = latitude_span * 0.0625f;
    angle_step = 0.3926991f;
    for (angle = -2.7488937f; angle < 3.1415927f; angle = next_angle) {
        qcopy(&quad.positions[2], &center);
        qcopy(&quad.positions[3], &center);
        next_angle = angle + angle_step;
        strip_start_alpha_scale = convert_integer_to_float(0);
        row_alpha = alpha;
        color = truncate_float_to_s32(strip_start_alpha_scale * row_alpha) << 24;
        color |= tint_color(grey);
        quad.colors[3] = color;
        quad.colors[2] = color;
        for (latitude = latitude_step + -1.5707964f; latitude <= latitude_span - 1.5707964f;
             latitude += latitude_step) {
            qcopy(&quad.positions[0], &quad.positions[2]);
            qcopy(&quad.positions[1], &quad.positions[3]);
            row_alpha += alpha_step;
            quad.colors[0] = quad.colors[2];
            quad.colors[1] = quad.colors[3];
            color = truncate_float_to_s32(convert_integer_to_float(0) * row_alpha) << 24;
            color |= tint_color(grey);
            quad.colors[3] = color;
            quad.colors[2] = color;
            longitude = normalize_angle(angle + spin * 0.02f);
            build_spherical_offset(&quad.positions[2], radius, longitude, latitude);
            build_spherical_offset(
                &quad.positions[3], radius, fast_add_rotations(longitude, angle_step), latitude);
            if (0.0f < latitude) {
                quad.positions[2].f[2] = quad.positions[2].f[2] + 0.6f;
                quad.positions[3].f[2] = quad.positions[3].f[2] + 0.6f;
            } else {
                quad.positions[2].f[2] = quad.positions[2].f[2] - 0.375f;
                quad.positions[3].f[2] = quad.positions[3].f[2] - 0.375f;
            }
            fast_vec_add(&quad.positions[2], &quad.positions[2], &origin);
            fast_vec_add(&quad.positions[3], &quad.positions[3], &origin);
            draw_geometry_quad(&quad, 0, 1);
        }
    }
    vu1_add_g_sregister(0x4E, (u64)(gs_texture_allocation_start_macro[4] >> 13) | 0x1000000L);
}

/* Starts the effect with an angle ramped from the moby's timer, capped at pi. */
/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_00313D28.c: func_L07_003141A8), where it is exact; names translated to the US level program. */
void draw_snagglebeast_sphere_effect(char *moby) __asm__("FUN_L07_00312dc8");

void draw_snagglebeast_sphere_effect(char *moby) {
    SnagglebeastVars *snagglebeast_vars = (SnagglebeastVars *)MOBY(moby)->pvars;
    float             latitude_span;
    if (snagglebeast_vars->sphere_fade < 0.8f) {
        latitude_span = snagglebeast_vars->sphere_fade * 3.0434179f / 0.8f + 0.09817477f;
        if (latitude_span > 3.14159274f)
            latitude_span = 3.14159274f;
    } else {
        latitude_span = 3.14159274f;
    }
    draw_snagglebeast_sphere_quad_strips(
        moby, 0, 1.0f, 5.8f, latitude_span, 1.0f, snagglebeast_vars->sphere_fade);
}

void draw_snagglebeast_rope_attack_layer(
    char *moby, float radius, int phase, float speed, int color) __asm__("FUN_L07_00312e70");

void draw_snagglebeast_rope_attack_layer(

    char *moby, float radius, int phase, float speed, int color) {
    SnagglebeastVars   *rope_layer_moby_vars = (SnagglebeastVars *)MOBY(moby)->pvars;
    struct GeometryQuad quad;
    struct GeometryQuad inner;
    float               offset[4];
    float               spin;
    float               step;
    float               angle;
    float               wrapped;
    u64                 texture;

    spin = convert_integer_to_float(level_frame_count + phase) * speed;
    if (rope_layer_moby_vars->substate == 4) {
        float alpha = convert_integer_to_float(color >> 24);
        float fade = convert_integer_to_float(rope_layer_moby_vars->timer);
        fade = fade / scale_game_frames(20);
        alpha = alpha * fade;
        if (alpha < 0.0f)
            alpha = 0.0f;
        color &= 0xFFFFFF;
        color |= truncate_float_to_s32(alpha) << 24;
    } else if (rope_layer_moby_vars->substate > 4) {
        return;
    }
    step = 0.5235988f;
    texture = get_effect_texture(0x3D);
    {
        float zero = 0.0f;
        float one = 1.0f;
        float half = 0.5f;

        quad.colors[3] = color;
        quad.colors[2] = color;
        quad.colors[1] = color;
        quad.colors[0] = color;
        quad.texture = texture;
        quad.texture_state = 0xFF9000000260LL;
        quad.reserved.value = 4;
        quad.primitive = 0x8000000048LL;
        quad.texture_coordinates[0][0] = zero;
        quad.texture_coordinates[2][0] = zero;
        quad.texture_coordinates[1][0] = one;
        quad.texture_coordinates[3][0] = one;
        quad.texture_coordinates[0][1] = zero;
        quad.texture_coordinates[1][1] = zero;
        quad.texture_coordinates[2][1] = half;
        quad.texture_coordinates[3][1] = half;
    }
    qcopy(&quad.positions[1], rope_layer_moby_vars->rope_start_position);
    qcopy(&quad.positions[3], rope_layer_moby_vars->rope_tip_position);
    inner = quad;
    qcopy(&inner.positions[0], rope_layer_moby_vars->rope_end_position);
    qcopy(&inner.positions[1], rope_layer_moby_vars->rope_end_position);
    for (angle = -3.1415927f; angle < step + 3.1415927f; angle += step) {
        qcopy(&quad.positions[0], &quad.positions[1]);
        qcopy(&quad.positions[2], &quad.positions[3]);
        wrapped = normalize_angle(angle + spin * 0.02f);
        offset[1] = radius * fast_cos(wrapped);
        offset[2] = radius * fast_sin(wrapped);
        offset[0] = 0.0f;
        transform_vector_by_basis(offset, offset, moby + 0xC0);
        fast_vec_add(&quad.positions[3], rope_layer_moby_vars->rope_tip_position, offset);
        if (angle != -3.1415927f) {
            draw_geometry_quad(&quad, 0, 1);
            qcopy(&inner.positions[2], &quad.positions[2]);
            qcopy(&inner.positions[3], &quad.positions[3]);
            draw_geometry_quad(&inner, 0, 0);
        }
    }
}

#include "qzero.h"

typedef union {
    unsigned int q __attribute__((mode(TI)));
    float        f[4];
} RopeQuad;

typedef float RopeVec[4] __attribute__((aligned(16)));
extern float  random_angle_radians(void) __asm__("FUN_00213308");
extern s32  collect_mobies_in_sphere(f32, void *, s32, void *, void *) __asm__("FUN_L00_001f2868");
extern void FUN_L00_00258278(struct Moby *, f32, void *, f32, f32, f32);
extern void
draw_snagglebeast_rope_attack_layer(char *, float, int, float, int) __asm__("FUN_L07_00312e70");

/* Rope draw: moves the two end points along the rope direction, draws three strands, then casts a ray between the ends. */
void draw_and_probe_snagglebeast_rope_attack(char *moby) __asm__("FUN_L07_003131b0");

void draw_and_probe_snagglebeast_rope_attack(char *moby) {
    RopeQuad          rope_offset;
    char              blk[0x30];
    RopeVec           rope_direction;
    SnagglebeastVars *rope_attack_moby_vars;
    float             one;

    rope_attack_moby_vars = (SnagglebeastVars *)MOBY(moby)->pvars;
    qzero(&rope_offset);
    one = 1.0f;
    fast_vec_normalize(&rope_offset,
                       rope_attack_moby_vars->rope_tip_velocity,
                       rope_attack_moby_vars->rope_extension_length * 0.8f);
    fast_vec_add(rope_attack_moby_vars->rope_tip_position,
                 &rope_offset,
                 rope_attack_moby_vars->rope_start_position);
    fast_vec_normalize(&rope_offset, &rope_offset, rope_attack_moby_vars->rope_extension_length);
    fast_vec_add(rope_attack_moby_vars->rope_end_position,
                 &rope_offset,
                 rope_attack_moby_vars->rope_start_position);
    draw_snagglebeast_rope_attack_layer(moby, one, 0, one, 0x20000080);
    draw_snagglebeast_rope_attack_layer(moby, 0.7f, 10, 2.0f, 0x28008080);
    draw_snagglebeast_rope_attack_layer(moby, 0.4f, 20, 3.0f, 0x30B0FFFF);
    fast_vec_sub(rope_direction,
                 rope_attack_moby_vars->rope_end_position,
                 rope_attack_moby_vars->rope_start_position);
    fast_vec_normalize(rope_direction, rope_direction, one);
    make_hit_descriptor(blk, moby, 0x30000, 1.00012302f, rope_direction);
    if (collision_line(rope_attack_moby_vars->rope_start_position,
                       rope_attack_moby_vars->rope_end_position,
                       0,
                       moby,
                       blk) == 0) {
        collect_mobies_in_sphere(0.5f, rope_attack_moby_vars->rope_end_position, 0, moby, blk);
    }
}

/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_00313D28.c: func_L07_00314730), where it is exact; names translated to the US level program. */
void *
collect_snagglebeast_navigation_ids(void *unused, void *dp, void *op) __asm__("FUN_L07_00313350");

void *collect_snagglebeast_navigation_ids(void *unused, void *dp, void *op) {
    SnagglebeastVars *snagglebeast_moby_vars = dp;
    int              *collected_navigation_ids = op;
    int               output_count = 0;
    int               output_candidate_0 = snagglebeast_moby_vars->navigation_ids[0];
    int output_candidate_1, output_candidate_2, output_candidate_3, output_candidate_4,
        output_candidate_5;
    if (output_candidate_0 != -1)
        collected_navigation_ids[output_count++] = output_candidate_0;
    output_candidate_1 = snagglebeast_moby_vars->navigation_ids[1];
    if (output_candidate_1 != -1)
        collected_navigation_ids[output_count++] = output_candidate_1;
    output_candidate_2 = snagglebeast_moby_vars->navigation_ids[2];
    if (output_candidate_2 != -1)
        collected_navigation_ids[output_count++] = output_candidate_2;
    if ((((unsigned char)snagglebeast_moby_vars->navigation_mask ^ 1) & 1)) {
        output_candidate_3 = snagglebeast_moby_vars->navigation_ids[3];
        if (output_candidate_3 != -1)
            collected_navigation_ids[output_count++] = output_candidate_3;
    }
    if (!((unsigned char)snagglebeast_moby_vars->navigation_mask & 2)) {
        output_candidate_4 = snagglebeast_moby_vars->navigation_ids[4];
        if (output_candidate_4 != -1)
            collected_navigation_ids[output_count++] = output_candidate_4;
    }
    if (!((unsigned char)snagglebeast_moby_vars->navigation_mask & 4)) {
        output_candidate_5 = snagglebeast_moby_vars->navigation_ids[5];
        if (output_candidate_5 != -1)
            collected_navigation_ids[output_count++] = output_candidate_5;
    }
    return (void *)output_count;
}

extern int resolve_moby_hit_response(void *, void *, void *, int, int *, int, int, int) __asm__("FUN_00213928");
extern void FUN_L00_00257470(void *, int, int);
extern void start_hit_color_flash(void *, void *) __asm__("FUN_L00_0025d458");
extern void update_hit_color_flash(void *, void *) __asm__("FUN_L00_0025d538");
extern void FUN_00212f90(void *, int, int, int);
extern void hero_set_state(int, int) __asm__("FUN_L00_002223f8");
extern void FUN_L00_00200228(float, void *);
extern int  FUN_0022da68(int, int, void *);
extern int  camera_secondary_mode_lui __asm__("D_001413D4") NOT_SDA;

#include "rnc/overlay/shared/world/cuboid.h"

extern char voice_slot_state_base[] __asm__("D_0013E5C4");
void        process_snagglebeast_damage(char             *moby,
                                        SnagglebeastVars *data,
                                        float            *health) __asm__("FUN_L07_00313420");

void process_snagglebeast_damage(char             *moby,
                                 SnagglebeastVars *snagglebeast_instance_vars,

                                 float *health) {
    enum {
        SNAGGLEBEAST_HIT_KIND_FORCE_UNIT_DAMAGE = 0x79,
        SNAGGLEBEAST_HIT_KIND_UNIT_DAMAGE_OVERRIDE = 0xba,
        SNAGGLEBEAST_HIT_KIND_REDUCED_DAMAGE = 0x131
    };
    char *hit;
    char *owner;
    struct {
        int   contact;
        float threshold;
    } scratch;
    int   voice;
    float damage;
    float frame;
    float wide;
    float narrow;
    float remaining;
    float output;
    float original_health;

    if ((u8)moby[0x20] != 10) {
        hit = find_moby_hit(moby, 0x330000, 0);
        resolve_moby_hit_response(moby, hit, health, 0, &scratch.contact, 0, 0, 4);
        if (hit != 0) {
            char *owner;
            if (0.0f < *(float *)(hit + 0x2c)) {
                unsigned short kind = *(unsigned short *)(hit + 0x2a);
                switch (kind) {
                case SNAGGLEBEAST_HIT_KIND_REDUCED_DAMAGE:
                    *(float *)(hit + 0x2c) = 0.2f;
                    break;
                case SNAGGLEBEAST_HIT_KIND_FORCE_UNIT_DAMAGE:
                case SNAGGLEBEAST_HIT_KIND_UNIT_DAMAGE_OVERRIDE:
                    *(float *)(hit + 0x2c) = 1.0f;
                    break;
                }
            }
            owner = *(char **)(hit + 0x20);
            if (owner != 0) {
                if (*(short *)(owner + 0xa6) == 0x419 || *(short *)(owner + 0xa6) == 0x416)
                    scratch.contact = 1;
            } else
                scratch.contact = 1;
        }
        if (snagglebeast_instance_vars->sphere_fade > 0.0f ||
            snagglebeast_instance_vars->sphere_fade_direction != 0)
            goto blocked_hit;
        if (scratch.contact >= 2) {
            original_health = *health;
            damage = *(float *)(hit + 0x2c);
            if (original_health <= damage) {
                char *owner;
                *health = 0.0f;
                MOBY(moby)->flags &= 0xefff;
                FUN_L00_00257470(moby, 0, -1);
                snagglebeast_instance_vars->hit_flash.flash_red = 0x78;
                start_hit_color_flash(moby, &snagglebeast_instance_vars->hit_flash);
                if ((u8)moby[0x53] != 2)
                    FUN_00212f90(moby, 2, 0, random_int_between(10, 13));
                if (camera_secondary_mode_lui == 0x78)
                    hero_set_state(0, 1);
                if (snagglebeast_instance_vars->cuboid != -1) {
                    set_moby_dialogue_record_value(moby, 1);
                    FUN_L00_00257470(moby, 0, -1);
                    jet_clear_persistent((u8)moby[0xb0]);
                    owner = (char *)l07_cuboids + (snagglebeast_instance_vars->cuboid << 7);
                    jet_reset_camera(owner + 0x30, owner + 0x70);
                }
                moby[0x20] = 10;
            } else {
                frame = (float)*(short *)(health + 1) / 6.0f;
                wide = frame * 1.75f;
                narrow = frame * 1.25f;
                remaining = -2.0f;
                if (!(2.0f < damage + damage))
                    remaining = -(damage + damage);
                snagglebeast_instance_vars->damage_reaction_amount = remaining;
                FUN_L00_00200228(original_health / frame, (void *)((unsigned)&scratch | 4));
                if ((float)snagglebeast_instance_vars->max_health - 1.0f < original_health)
                    scratch.threshold = 5.0f;
                scratch.threshold *= frame;
                *health -= *(float *)(hit + 0x2c);
                snagglebeast_instance_vars->hit_flash.flash_red = 0xfa;
                snagglebeast_instance_vars->unk26 = scale_game_frames(0);
                start_hit_color_flash(moby, &snagglebeast_instance_vars->hit_flash);
                if ((u8)moby[0x20] < 10 && *health < scratch.threshold) {
                    int slot;
                    if (camera_secondary_mode_lui == 0x78)
                        hero_set_state(0, 1);
                    if ((u8)moby[0x53] != 2) {
                        int hit_animation_start_frame = random_int_between(0, 2);
                        FUN_00212f90(
                            moby, 2, hit_animation_start_frame, random_int_between(20, 22));
                    }
                    slot = snagglebeast_instance_vars->voice;
                    snagglebeast_instance_vars->sphere_fade = 0.0f;
                    snagglebeast_instance_vars->sphere_fade_direction = 1;
                    if (slot != -1) {
                        owner = voice_slot_state_base - 0x74 + slot * 0x70;
                        if (*(char **)(owner + 0x88) == moby && *(u8 *)(owner + 0x74) != 0)
                            release_voice_slot(slot);
                    }
                    snagglebeast_instance_vars->voice = -1;
                    voice = FUN_0022da68(15, 4, moby);
                    snagglebeast_instance_vars->voice = voice;
                    moby[0x20] = 11;
                    moby[0xbc]++;
                }
                if ((wide < original_health && *health <= wide) ||
                    (narrow < original_health && *health <= narrow))
                    snagglebeast_instance_vars->hero_capture_attack_enabled = 1;
            }
        }
        goto decay_hit;
    blocked_hit:
        if (scratch.contact >= 2)
            snagglebeast_instance_vars->sphere_shade = 0xff;
    decay_hit:
        if ((u8)snagglebeast_instance_vars->sphere_shade >= 21)
            snagglebeast_instance_vars->sphere_shade -= 20;
        else
            snagglebeast_instance_vars->sphere_shade = 0;
        MOBY(moby)->hit_slot = 0xff;
    }
    update_hit_color_flash(moby, &snagglebeast_instance_vars->hit_flash);
    output = snagglebeast_instance_vars->damage_reaction_amount + 0.3f;
    snagglebeast_instance_vars->damage_reaction_amount = output;
    if (output > 0.0f)
        snagglebeast_instance_vars->damage_reaction_amount = 0.0f;
}

extern float random_float_between(float, float) __asm__("FUN_002132a8");

/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_00313D28.c: func_L07_00314D00), where it is exact; names translated to the US level program. */
void add_random_xy_offset(float *out, float value) __asm__("FUN_L07_00313920");

void add_random_xy_offset(float *position_to_offset, float offset_magnitude) {
    float neg = -offset_magnitude;
    position_to_offset[0] += random_float_between(neg, offset_magnitude);
    position_to_offset[1] += random_float_between(neg, offset_magnitude);
}

/* Finds where a path leaves the object's current segment and copies the point out. */
/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_00313D28.c: func_L07_00314D68), where it is exact; names translated to the US level program. */
extern char *D_L07_001B0530[];
extern int
follow_path(void *, void *, int, float, void *, void *, void *) __asm__("FUN_L01_00276fe8");
extern void      *
collect_snagglebeast_navigation_ids(void *, void *, void *) __asm__("FUN_L07_00313350");
void select_snagglebeast_chase_waypoint(char             *moby,
                                        SnagglebeastVars *moby_vars,
                                        void             *target_position,
                                        void             *out_position) __asm__("FUN_L07_00313988");

void select_snagglebeast_chase_waypoint(char             *moby,
                                        SnagglebeastVars *moby_vars,
                                        void             *target_position,

                                        void *out_position) {
    float navigation_ids[8];
    void *navigation_id_count =
        collect_snagglebeast_navigation_ids(moby, moby_vars, navigation_ids);
    if (follow_path(navigation_ids,
                    navigation_id_count,
                    moby_vars->nav_path,
                    0.0f,
                    moby + 0x10,
                    target_position,
                    out_position))
        return;
    {
        int i = select_point_index_by_distance(
            target_position, D_L07_001B0530[moby_vars->chase_path], 0.0f);
        int j =
            select_point_index_by_distance(D_L07_001B0530[moby_vars->chase_path] + (i * 16 + 0x10),
                                           D_L07_001B0530[moby_vars->nav_path],
                                           0.0f);
        if (follow_path(navigation_ids,
                        navigation_id_count,
                        moby_vars->nav_path,
                        0.0f,
                        moby + 0x10,
                        D_L07_001B0530[moby_vars->nav_path] + (j * 16 + 0x10),
                        out_position))
            return;
        i = select_point_index_by_distance(moby + 0x10, D_L07_001B0530[moby_vars->nav_path], 0.0f);
        qcopy(out_position, D_L07_001B0530[moby_vars->nav_path] + i * 16 + 0x10);
    }
}

/* Chaser step: follows its path while the goal is out of range (pulling back onto the path
 * when lost), else turns toward the hero; every fourth frame it may get nudged toward the
 * shared target moby. Returns the path-follow result, or 4. */
struct ChaserVars {
    u8  pad0[0x70];
    u8  nav[0x24]; /* 0x70 */
    f32 pullSpeed; /* 0x94 */
    u8  pad98[0x68];
    u8  navOut[0x68]; /* 0x100 */
    u8  turn[0x128];  /* 0x168 */
    s32 path;         /* 0x290 */
};

struct ChaserTargetVars {
    u8 pad0[0xBE];
    u8 sticky; /* 0xBE */
};

struct ChaserTarget {
    u8           pad0[0x18];
    struct Moby *moby;  /* 0x18 */
    s32          count; /* 0x1C */
};

extern s32                 level_frame_count_lui __asm__("D_L07_0015F5CC");
extern float               D_0015ED6C;
extern struct ChaserTarget D_L07_00173EC0;
extern int step_mover_2e3f20(struct Moby *, void *, int, void *) __asm__("FUN_L00_00259028");
extern int probe_vertical_ground_collision(void *, struct Moby *, int, float, float) __asm__("FUN_L00_002583f0");
int update_snagglebeast_chase_movement(struct Moby       *chaser_moby,
                                       struct ChaserVars *chaser_vars,
                                       Vec4              *goal,
                                       int                mode,
                                       float              range) __asm__("FUN_L07_00313af0");

int update_snagglebeast_chase_movement(

    struct Moby *chaser_moby, struct ChaserVars *chaser_vars, Vec4 *goal, int mode, float range) {
    float        position_delta[4];
    char        *path;
    struct Moby *target_moby;
    int          navigation_result;
    float        goal_distance;

    goal_distance = vector_distance_xy(&chaser_moby->pos, goal);
    navigation_result = 4;
    path = D_L07_001B0530[chaser_vars->path];
    if (range < goal_distance) {
        navigation_result =
            step_mover_2e3f20(chaser_moby, chaser_vars->nav, mode, chaser_vars->navOut);
        if (navigation_result == 2) {
            fast_vec_sub(
                position_delta,
                path + (select_point_index_by_distance(&chaser_moby->pos, path, 0.0f) * 16 + 0x10),
                &chaser_moby->pos);
            clamp_vector_length(position_delta, position_delta, chaser_vars->pullSpeed);
            fast_vec_add(&chaser_moby->pos, &chaser_moby->pos, position_delta);
        }
    } else {
        FUN_L00_00258278(chaser_moby,
                         fast_arc_tan(hero.motion.pos.f[0] - chaser_moby->pos.x,
                                      hero.motion.pos.f[1] - chaser_moby->pos.y),
                         chaser_vars->turn,
                         frame_time_sq * 2.0943952f,
                         frame_time_sq * 0.69813168f,
                         D_0015ED6C * 2.0943952f);
    }
    if ((level_frame_count & 3) == 3 &&
        probe_vertical_ground_collision(&chaser_moby->pos, chaser_moby, 0, 0.1f, 1.0f)) {
        target_moby = D_L07_00173EC0.moby;
        if (target_moby != 0 && D_L07_00173EC0.count > 0 && target_moby->oclass == 0x42D &&
            target_moby->command == 0) {
            if (((struct ChaserTargetVars *)target_moby->pvars)->sticky) {
                fast_vec_sub(position_delta, &target_moby->pos, &chaser_moby->pos);
                *(s32 *)&position_delta[2] = 0;
                fast_vec_scale(position_delta, position_delta, 0.5f);
                fast_vec_add(&chaser_moby->pos, &chaser_moby->pos, position_delta);
            }
            D_L07_00173EC0.moby->command = 1;
        }
    }
    return navigation_result;
}

/* eases the chaser speed up with the gap to its target */
struct ChaserSpeed {
    u8  pad0[0x80];
    f32 base;
    u8  pad84[0x10];
    f32 rate;
    u8  pad98[0x124];
    f32 rate2;
};

struct ChaserAnim {
    u8  pad0[0x58];
    f32 speed;
};

float update_snagglebeast_chase_speed(struct ChaserAnim  *chaser_anim,
                                      struct ChaserSpeed *chaser_speed,
                                      float               x,
                                      float               y,
                                      float               k) __asm__("FUN_L07_00313d30");

float update_snagglebeast_chase_speed(

    struct ChaserAnim *chaser_anim, struct ChaserSpeed *chaser_speed, float x, float y, float k) {
    float eased_rate = frame_scale * 0.06f;
    if (y < x) {
        eased_rate = x - y;
        if (50.0f < eased_rate)
            eased_rate = 50.0f;
        if (eased_rate < 0.0f)
            eased_rate = 0.0f;
        eased_rate = eased_rate / 50.0f;
        eased_rate = eased_rate * eased_rate;
        eased_rate = eased_rate * (frame_scale * 0.1f - frame_scale * 0.06f);
        eased_rate = eased_rate + frame_scale * 0.06f;
    }
    chaser_speed->rate2 = eased_rate;
    chaser_speed->rate = eased_rate;
    chaser_anim->speed = chaser_speed->base / (frame_scale * 0.06f) * k;
    return eased_rate;
}

/* latches onto a target when it is turned toward enough, then picks a turn animation */
/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_00313D28.c: func_L07_003151C0), where it is exact; names translated to the US level program. */
int start_snagglebeast_turn_toward_target(
    unsigned char    *moby,
    SnagglebeastVars *moby_vars,
    float            *target_position,
    int               resume_animation,
    int               use_fixed_target_position) __asm__("FUN_L07_00313de8");

int start_snagglebeast_turn_toward_target(unsigned char    *moby,
                                          SnagglebeastVars *moby_vars,
                                          float            *target_position,
                                          int               resume_animation,

                                          int use_fixed_target_position) {
    enum {
        SNAGGLEBEAST_TURN_CONTROLLER_RESET = 0
    };
    float target_heading = fast_arc_tan(target_position[0] - MOBY(moby)->pos.x,
                                        target_position[1] - MOBY(moby)->pos.y);
    if (moby_vars->turn_start_angle <
        fast_difference_between_rotations(MOBY(moby)->rot.z, target_heading)) {
        moby_vars->turn_controller_state = SNAGGLEBEAST_TURN_CONTROLLER_RESET;
        MOBY(moby)->anim_speed = 1.0f;
        moby_vars->turn_saved_state = MOBY(moby)->state;
        moby_vars->turn_resume_animation = resume_animation;
        moby_vars->turn_fixed_target = use_fixed_target_position;
        qcopy(moby_vars->turn_target, target_position);
        if (fast_arc_tan(target_position[0] - MOBY(moby)->pos.x,
                         target_position[1] - MOBY(moby)->pos.y) < 0.0f) {
            if (MOBY(moby)->prev_seq != 3) {
                int r = random_int_between(0, 1);
                blend_moby_animation(moby, 3, r, random_int_between(0xF, 0x14));
            }
        } else {
            if (MOBY(moby)->prev_seq != 0xE) {
                int r = random_int_between(0, 1);
                blend_moby_animation(moby, 0xE, r, random_int_between(0xF, 0x14));
            }
        }
        MOBY(moby)->state = SNAGGLEBEAST_TURN_TOWARD_TARGET;
        return 1;
    }
    return 0;
}

/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_00313D28.c: func_L07_00315338), where it is exact; names translated to the US level program. */
void update_snagglebeast_target_joint_animation(char             *moby,
                                                SnagglebeastVars *moby_vars,
                                                float *target_position) __asm__("FUN_L07_00313f60");

void update_snagglebeast_target_joint_animation(char             *moby,
                                                SnagglebeastVars *moby_vars,

                                                float *target_position) {
    float target_heading;
    float k;
    if (((unsigned char *)moby)[0x20] < 10) {
        target_heading = fast_arc_tan(target_position[0] - MOBY(moby)->pos.x,
                                      target_position[1] - MOBY(moby)->pos.y);
        moby_vars->target_yaw_offset = fast_subtract_rotations(target_heading, MOBY(moby)->rot.z);
    } else {
        *(int *)&moby_vars->target_yaw_offset = 0;
    }
    if (moby_vars->target_yaw_offset > 1.3962634f) {
        moby_vars->target_yaw_offset = 1.3962634f;
    } else if (moby_vars->target_yaw_offset < -1.3962634f) {
        moby_vars->target_yaw_offset = -1.3962634f;
    }
    k = frame_scale_sq;
    moby_vars->joint_damage_reaction_target = moby_vars->damage_reaction_amount;
    update_joint_animation(moby, moby_vars->joint_animation, 0xB, k * 0.030f, k * 0.3f);
}

typedef struct {
    char  pad0[0x44];
    short count;
    char  pad46[0x132];
    char *mobys[1];
} L07MobyList;

extern L07MobyList   D_L07_0016C8E0;
extern unsigned char l07_snagglebeast_big_head_manipulator[] __asm__("D_L07_0020C6C0");
void                 apply_snagglebeast_big_head_cheat(char *moby) __asm__("FUN_L07_00314058");

void apply_snagglebeast_big_head_cheat(char *moby) {
    int i;

    if (scene_state == 2 && big_head_cheat_enabled != 0) {
        for (i = 0; i < D_L07_0016C8E0.count; i++) {
            char *candidate_moby = D_L07_0016C8E0.mobys[i];

            if (MOBY(candidate_moby)->oclass == MOBY(moby)->oclass) {
                unsigned char *big_head_manipulator = l07_snagglebeast_big_head_manipulator;

                if (big_head_cheat_enabled != 0 && big_head_manipulator[1] == 0) {
                    attach_manipulator(candidate_moby, 0xB, big_head_manipulator);
                    *(float *)(big_head_manipulator + 0x20) = 2.75f;
                    *(float *)(big_head_manipulator + 0x24) = 2.75f;
                    *(float *)(big_head_manipulator + 0x28) = 2.75f;
                }
            }
        }
    }
}

/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_00313D28.c: func_L07_00315528). */
/* Target sensing, fade timers, and state transitions; only navigation result 4 takes the stop transition. */
typedef struct {
    Vec4  pos;
    Vec4  rotation;
    Vec4  position20; /* copied from hero +0xC0 or the selected position */
    Vec4  position30; /* copied from hero +0xD0 or the selected position */
    void *moby;
    int   kind;
} ChaserSight;

#include "rnc/overlay/shared/world/swarm_path.h"

typedef struct {
    char          pad[0x1D4];
    unsigned char path_points[4];
    int           linked_mobys[6];
} ChaserPathPoints;

typedef struct {
    char  pad[0x3C];
    short count;
    char  pad2[0xE];
} ChaserSpawnRecord;

extern Vec4 D_L07_00166E40;

int D_L07_00161B64 = 0;

extern ChaserSpawnRecord l07_ammo_transfer_records[] __asm__("D_L07_00179B40");
extern int               snagglebeast_ammo_drain_entries[][4] __asm__("D_L07_0020C600");

#include "rnc/game/item_state.h"

extern void FUN_L00_00263ac8(float, int, int, void *);
extern void spawn_explosion(void *,
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
extern void FUN_L00_0025a120(struct Moby *);
extern int  get_target_sight(void *, void *, float) __asm__("FUN_L07_002886d8");
extern int  FUN_L00_001fefc8(void *);
extern void suck_beam_queue_draw(int) __asm__("FUN_L00_002defe0");
extern int  queue_animation_update(int, int, int, int, int, int, int) __asm__("FUN_001ff308");
extern void reset_numeric_hud_element(void) __asm__("FUN_L00_00235d80");
extern void FUN_L06_00249738(void);
extern void suck_reset(void *) __asm__("FUN_L00_002ddca0");
extern int  rand(void) __asm__("FUN_001160d8");
extern int  FUN_00213260(int);
extern void spawn_moby_point_offset_particle(char *, int, float, float) __asm__("FUN_L07_0029b8d8");
extern int
update_alternating_joint_emission_burst(void *, char *, float *) __asm__("FUN_L07_003122b0");
extern int   is_value_within_interpolated_window(void *, float) __asm__("FUN_00214cc8");
extern void *spawn_fading_ring_effect(int, void *, int, float, float, float, float, float) __asm__("FUN_L07_0030dbb0");
extern void FUN_L00_0024f7c8(void *, int, void *);
extern void configure_timed_point_light(
    void *, void *, int, void *, short, unsigned int, int, float) __asm__("FUN_L07_00311eb8");
extern void  FUN_00214db0(void *, float, float, float);
extern void  spawn_white_particle_with_payload(void *, void *) __asm__("FUN_L07_0029c4e8");
extern void  rumble_by_distance(char *) __asm__("FUN_L07_003120d8");
extern void  draw_collision_clipped_moby_beam(void) __asm__("FUN_L07_00312420");
extern void *spawn_moving_resizing_height_snapped_alpha_fade_particle(
    void *, int, int, float, float) __asm__("FUN_L00_002715e8");
extern float update_snagglebeast_rope(void *, void *) __asm__("FUN_L07_00317e60");
void         detach_snagglebeast_rope_particles(int target_entry) __asm__("FUN_L07_00318e98");
extern void  detach_snagglebeast_rope_particles(int) __asm__("FUN_L07_00318e98");
extern void  FUN_L00_00257d78(void *, float, float);
extern int   sample_path(void *, void *, int *, float *, float, int) __asm__("FUN_L00_0025d808");
extern float FUN_L00_0025abf0(void *, void *, void *, float, float);
extern char *spawn_on_path(void *, int, int, int) __asm__("FUN_L00_002c52a8");
extern void  initialize_moby_particle_effect_state(char *) __asm__("FUN_L06_002e6148");
extern void  mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern void  jet_animate(struct Moby *, float) __asm__("FUN_L00_00259fe8");
extern void  update_timed_point_light_color(char *) __asm__("FUN_L07_00312150");
void         snagglebeast_update(unsigned char *moby) __asm__("FUN_L07_00314150");

/* Moby class 1106 */
void snagglebeast_update(unsigned char *moby) {
    enum {
        SNAGGLEBEAST_ROPE_START_ATTACK_ANIMATION = 0,
        SNAGGLEBEAST_ROPE_WAIT_FOR_ORIGIN_FRAME = 1,
        SNAGGLEBEAST_ROPE_WAIT_FOR_EXTENSION_FRAME = 2,
        SNAGGLEBEAST_ROPE_EXTEND = 3,
        SNAGGLEBEAST_ROPE_RETRACT = 4,
        SNAGGLEBEAST_ROPE_RETURN_TO_IDLE = 5
    };
    enum {
        SNAGGLEBEAST_PATH_SELECT_FIRST_POINT = 0,
        SNAGGLEBEAST_PATH_SELECT_SECOND_POINT = 1,
        SNAGGLEBEAST_PATH_SELECT_FARTHER_PAIRED_POINT = 2
    };
    enum {
        SNAGGLEBEAST_PHASE_BEFORE_FIRST_HEALTH_THRESHOLD = 0,
        SNAGGLEBEAST_PHASE_AFTER_FIRST_HEALTH_THRESHOLD = 1,
        SNAGGLEBEAST_PHASE_FIRST_TIMED_DAMAGE_SEQUENCE = 2,
        SNAGGLEBEAST_PHASE_AFTER_FIRST_TIMED_DAMAGE = 3,
        SNAGGLEBEAST_PHASE_AFTER_SECOND_HEALTH_THRESHOLD = 4,
        SNAGGLEBEAST_PHASE_SECOND_TIMED_DAMAGE_SEQUENCE = 5,
        SNAGGLEBEAST_PHASE_AFTER_SECOND_TIMED_DAMAGE = 6,
        SNAGGLEBEAST_PHASE_AFTER_THIRD_HEALTH_THRESHOLD = 7,
        SNAGGLEBEAST_PHASE_FINAL_TIMED_DAMAGE_SEQUENCE = 8
    };
    Vec4              oldpos;
    ChaserSight       sight;
    Vec4              position;
    Vec4              velocity;
    Vec4              direction;
    Vec4              start;
    Vec4              offset;
    Vec4              step;
    Vec4              projection;
    SnagglebeastVars *snagglebeast_instance_state = (SnagglebeastVars *)MOBY(moby)->pvars;
    int               follow_target;
    int               timer194;
    int               timer19a;
    int               sight_ready;
    int               target_close;
    int               path_point_ready;
    int               target_above;
    float             target_distance;
    float             target_angle;
    float             hero_distance;
    float             hero_angle;
    int               i;
    float            *health;

    FUN_L00_00263ac8(2.5f, (int)moby, 0xB, snagglebeast_instance_state->manipulator);
    apply_snagglebeast_big_head_cheat((char *)moby);
    if (scene_state == 2)
        return;
    if (snagglebeast_instance_state->hero_volume != -1 &&
        !is_point_inside_clip_volume(((char *)&hero) + 0x80,
                                     snagglebeast_instance_state->hero_volume))
        return;
    if (MOBY(moby)->visible != 0) {
        if (fast_vec_dist(moby + 0x10, &D_L07_00166E40) < 35.0f) {
            FUN_L00_0025a120((struct Moby *)moby);
            MOBY(moby)->shadow_range = 0x1C;
        }
    }
    qcopy(&oldpos, moby + 0x10);
    health = &snagglebeast_instance_state->health;
    process_snagglebeast_damage((char *)moby, snagglebeast_instance_state, health);
    follow_target = 0;
    get_target_sight((struct Moby *)moby, &sight, 30.0f);
    target_distance = vector_distance_xy(&sight, moby + 0x10);
    target_angle =
        fast_arc_tan(sight.pos.f[0] - MOBY(moby)->pos.x, sight.pos.f[1] - MOBY(moby)->pos.y);
    hero_distance = vector_distance_xy(moby + 0x10, ((char *)&hero) + 0x80);
    {
        char *g = ((char *)&hero);
        hero_angle = fast_arc_tan(*(float *)(g + 0x80) - MOBY(moby)->pos.x,
                                  *(float *)(g + 0x84) - MOBY(moby)->pos.y);
    }
    if (FUN_L00_001fefc8(&snagglebeast_instance_state->timer_1F7)) {
        if ((vector_distance_xy(moby + 0x10, ((char *)&hero) + 0x80) > 8.0f &&
             MOBY(moby)->state != SNAGGLEBEAST_CHASE_TARGET) ||
            (vector_distance_xy(moby + 0x10, ((char *)&hero) + 0x80) > 6.0f &&
             (MOBY(moby)->state == SNAGGLEBEAST_CHASE_TARGET || MOBY(moby)->command % 3U == 1))) {
            follow_target = 1;
        }
    }
    timer19a = 0;
    timer194 = fast_dec_timer_s16(&snagglebeast_instance_state->timer_194) != 0;
    if (fast_dec_timer_s16(&snagglebeast_instance_state->timer_19A) && target_distance > 9.0f)
        timer19a = 1;
    sight_ready = 0;
    if (snagglebeast_instance_state->hero_capture_attack_enabled != 0) {
        char *g = ((char *)&hero);
        if (sight.moby == *(void **)(g + 0x2080) && target_distance < 30.0f &&
            absolute_float(fast_subtract_rotations(target_angle, MOBY(moby)->rot.z)) < 1.3962634f) {
            sight_ready = 1;
        }
    }
    target_close = 0;
    if (fast_dec_timer_s16(&snagglebeast_instance_state->timer_198)) {
        if (((MOBY(moby)->state == SNAGGLEBEAST_CHASE_TARGET || MOBY(moby)->command % 3U == 1) &&
             target_distance < 14.0f) ||
            (MOBY(moby)->state != SNAGGLEBEAST_CHASE_TARGET && target_distance < 10.0f))
            target_close = 1;
    }
    {
        char *g = ((char *)&hero);
        path_point_ready = 0;
        target_above = 0;
        if (MOBY(moby)->pos.z + 1.0f < *(float *)(g + 0x2D8)) {
            if (*(char **)(g + 0x2FC) != 0 && *(short *)(*(char **)(g + 0x2FC) + 0xA6) >= 0x459 &&
                *(short *)(*(char **)(g + 0x2FC) + 0xA6) < 0x45D) {
                target_above = 1;
            }
        }
    }
    switch (MOBY(moby)->command) {
    case SNAGGLEBEAST_PHASE_BEFORE_FIRST_HEALTH_THRESHOLD:
        follow_target = 0;
        sight_ready = 0;
        break;
    case SNAGGLEBEAST_PHASE_AFTER_FIRST_TIMED_DAMAGE:
        if (snagglebeast_instance_state->target_nav_point != -1) {
            follow_target = 1;
            path_point_ready = 1;
            timer194 = 0;
            target_close = 0;
        } else {
            follow_target = 0;
        }
        sight_ready = 0;
        break;
    case SNAGGLEBEAST_PHASE_AFTER_SECOND_TIMED_DAMAGE:
        if (snagglebeast_instance_state->target_nav_point != -1) {
            follow_target = 1;
            path_point_ready = 1;
            timer194 = 0;
            target_close = 0;
        } else {
            follow_target = 0;
        }
        break;
    case SNAGGLEBEAST_PHASE_AFTER_FIRST_HEALTH_THRESHOLD:
    case SNAGGLEBEAST_PHASE_AFTER_SECOND_HEALTH_THRESHOLD:
    case SNAGGLEBEAST_PHASE_AFTER_THIRD_HEALTH_THRESHOLD:
        timer194 = 0;
        sight_ready = 0;
        if (MOBY(moby)->state != SNAGGLEBEAST_IDLE)
            timer19a = 0;
        break;
    case SNAGGLEBEAST_PHASE_FIRST_TIMED_DAMAGE_SEQUENCE:
    case SNAGGLEBEAST_PHASE_SECOND_TIMED_DAMAGE_SEQUENCE:
    case SNAGGLEBEAST_PHASE_FINAL_TIMED_DAMAGE_SEQUENCE:
        target_close = 0;
        timer194 = 0;
        sight_ready = 0;
        timer19a = 0;
        break;
    }
    snagglebeast_instance_state->health_display =
        truncate_float_to_s32(snagglebeast_instance_state->health * 16.0f) & 0xFFF0;
    if (snagglebeast_instance_state->health_display >=
        truncate_float_to_s32(((float)snagglebeast_instance_state->max_health - 0.5f) * 16.0f)) {
        snagglebeast_instance_state->health_display |= 0xF;
    }
    if (MOBY(moby)->command % 3U == 0) {
        if (snagglebeast_instance_state->health > 0.5f) {
            int r = ((truncate_float_to_s32(snagglebeast_instance_state->health) %
                      (snagglebeast_instance_state->max_health / 6)) *
                     16) /
                    (snagglebeast_instance_state->max_health / 6);
            if (r < 0)
                r = 0;
            snagglebeast_instance_state->health_display += r & 0xF;
        } else {
            snagglebeast_instance_state->health_display = 0;
        }
    }
    if (MOBY(moby)->command % 3U == 2)
        snagglebeast_instance_state->health_display += 0xF;
    if (MOBY(moby)->state != SNAGGLEBEAST_CAPTURE_HERO_AND_DRAIN_AMMO &&
        MOBY(moby)->state != SNAGGLEBEAST_INITIALIZE)
        suck_beam_queue_draw((int)moby);
    if ((int)snagglebeast_instance_state->sphere_fade_direction - 1 == 0) {
        if (snagglebeast_instance_state->voice == -1)
            snagglebeast_instance_state->voice = FUN_0022da68(0xF, 4, (int)moby);
        snagglebeast_instance_state->sphere_fade += frame_scale * 0.01f;
        if (snagglebeast_instance_state->sphere_fade >= 1.0f)
            snagglebeast_instance_state->sphere_fade_direction = 0;
        if (snagglebeast_instance_state->sphere_fade > 1.0f)
            snagglebeast_instance_state->sphere_fade = 1.0f;
    }
    if (snagglebeast_instance_state->sphere_fade_direction == 2) {
        if (snagglebeast_instance_state->voice == -1)
            snagglebeast_instance_state->voice = FUN_0022da68(0xF, 4, (int)moby);
        snagglebeast_instance_state->sphere_fade -= frame_scale * 0.01f;
        if (snagglebeast_instance_state->sphere_fade <= 0.0f) {
            if (snagglebeast_instance_state->voice != -1) {
                char *sl =
                    (voice_slot_state_base - 0x74) + snagglebeast_instance_state->voice * 0x70;
                if (*(void **)(sl + 0x88) == moby && *(unsigned char *)(sl + 0x74) != 0) {
                    release_voice_slot(snagglebeast_instance_state->voice);
                }
            }
            snagglebeast_instance_state->sphere_fade_direction = 0;
            snagglebeast_instance_state->voice = -1;
        }
        if (snagglebeast_instance_state->sphere_fade < 0.0f)
            snagglebeast_instance_state->sphere_fade = 0.0f;
    }
    if (snagglebeast_instance_state->sphere_fade > 0.0f)
        add_draw_callback(draw_snagglebeast_sphere_effect, moby);
    update_snagglebeast_target_joint_animation(
        (char *)moby, snagglebeast_instance_state, hero.motion.pos.f);
    if (MOBY(moby)->state != SNAGGLEBEAST_INITIALIZE &&
        MOBY(moby)->state != SNAGGLEBEAST_DEATH_EXPLOSION &&
        MOBY(moby)->state != SNAGGLEBEAST_POST_DEATH_CLEANUP) {
        snagglebeast_instance_state->health_hud_handle =
            queue_animation_update(0x16,
                                   0xFFFF,
                                   (int)reset_numeric_hud_element,
                                   (int)hud_update_counter,
                                   (int)FUN_L06_00249738,
                                   (int)(&snagglebeast_instance_state->health_display),
                                   (snagglebeast_instance_state->max_health << 4) | 0xF);
    }
    switch (MOBY(moby)->state) {
    case SNAGGLEBEAST_INITIALIZE:
        *(int *)&D_L07_00161B64 = 0;
        MOBY(moby)->state = SNAGGLEBEAST_IDLE;
        MOBY(moby)->command = 0;
        snagglebeast_instance_state->point_light = -1;
        snagglebeast_instance_state->measured_path_length = 0.0f;
        if (snagglebeast_instance_state->measured_path != -1) {
            int i;
            for (i = 0; i < *(int *)D_L07_001B0530[snagglebeast_instance_state->measured_path];
                 i++) {
                ((LevelPath *)D_L07_001B0530[snagglebeast_instance_state->measured_path])
                    ->points[i]
                    .f[3] = fast_vec_dist(
                    D_L07_001B0530[snagglebeast_instance_state->measured_path] + (i * 16 + 0x10),
                    D_L07_001B0530[snagglebeast_instance_state->measured_path] +
                        (((i + 1) %
                          *(int *)D_L07_001B0530[snagglebeast_instance_state->measured_path]) *
                             16 +
                         0x10));
                snagglebeast_instance_state->measured_path_length +=
                    ((LevelPath *)D_L07_001B0530[snagglebeast_instance_state->measured_path])
                        ->points[i]
                        .f[3];
            }
        }
        if (snagglebeast_instance_state->chase_path != -1) {
            char *chase_path = D_L07_001B0530[snagglebeast_instance_state->chase_path];
            int   i;
            for (i = 0; i < *(int *)chase_path; i++) {
                ((LevelPath *)chase_path)->points[i].f[3] =
                    fast_vec_dist(chase_path + (i * 16 + 0x10),
                                  chase_path + (((i + 1) % *(int *)chase_path) * 16 + 0x10));
            }
        }
        if (snagglebeast_instance_state->nav_path != -1) {
            int i;
            for (i = 0; i < *(int *)D_L07_001B0530[snagglebeast_instance_state->nav_path]; i++) {
                int k = truncate_float_to_s32(
                    ((LevelPath *)D_L07_001B0530[snagglebeast_instance_state->nav_path])
                        ->points[i]
                        .f[3]);
                if ((unsigned int)k < 4)
                    ((ChaserPathPoints *)snagglebeast_instance_state)->path_points[k] = i;
            }
        }
        snagglebeast_instance_state->unkA8 &= ~8;
        MOBY(moby)->flags |= 0x1000;
        build_symmetric_adjacency_masks(
            snagglebeast_instance_state->navigation_ids, 3, snagglebeast_instance_state->nav_path);
        snagglebeast_instance_state->navigation_mask = 7;
        MOBY(moby)->update_dist = 0xFF;
        MOBY(moby)->draw_dist = 0xFF;
        snagglebeast_instance_state->target_nav_point = -1;
        snagglebeast_instance_state->health_hud_handle = -1;
        snagglebeast_instance_state->nav_point = select_point_index_by_distance(
            moby + 0x10, D_L07_001B0530[snagglebeast_instance_state->nav_path], 0.0f);
        suck_reset(moby);
        MOBY(moby)->scale *= 1.75f;
        snagglebeast_instance_state->hero_capture_attack_enabled = 0;
        snagglebeast_instance_state->turn_start_angle = 1.7453293f;
        snagglebeast_instance_state->unk29 = 1;
        snagglebeast_instance_state->unk58 = truncate_float_to_s32(30.0f);
        snagglebeast_instance_state->unk5A = truncate_float_to_s32(40.0f);
        break;
    case SNAGGLEBEAST_IDLE:
        if (MOBY(moby)->anim_flags & 2) {
            if (MOBY(moby)->prev_seq != (rand() & 1)) {
                int r1 = rand() & 1;
                int r2 = random_int_between(0, 0xF);
                FUN_00212f90(moby, r1, r2, random_int_between(0x1E, 0x2A));
            }
        }
        if (fast_dec_timer(&snagglebeast_instance_state->timer)) {
            if (timer19a) {
                snagglebeast_instance_state->substate = 0;
                snagglebeast_instance_state->timer = 0;
                if (MOBY(moby)->prev_seq != 5) {
                    int r = random_int_between(0, 2);
                    FUN_00212f90(moby, 5, r, random_int_between(0x12, 0x14));
                }
                MOBY(moby)->state = SNAGGLEBEAST_SWEEP_BEAM_ATTACK;
            }
            if (timer194) {
                if (MOBY(moby)->state != SNAGGLEBEAST_CHASE_TARGET || FUN_00213260(4) == 0) {
                    snagglebeast_instance_state->substate = 0;
                    if (MOBY(moby)->prev_seq != 4) {
                        int r = random_int_between(0, 9);
                        FUN_00212f90(moby, 4, r, random_int_between(0x14, 0x28));
                    }
                    MOBY(moby)->state = SNAGGLEBEAST_TRIPLE_SPAWN_ATTACK;
                    MOBY(moby)->anim_speed = 1.0f;
                }
            }
            if (sight_ready) {
                if (MOBY(moby)->prev_seq != 6) {
                    int r = random_int_between(0, 2);
                    FUN_00212f90(moby, 6, r, random_int_between(0xF, 0x19));
                }
                snagglebeast_instance_state->substate = 0;
                snagglebeast_instance_state->rope_extension_length = 0.0f;
                snagglebeast_instance_state->hero_captured = 0;
                MOBY(moby)->state = SNAGGLEBEAST_CAPTURE_HERO_AND_DRAIN_AMMO;
                snagglebeast_instance_state->capture_turn_done = 0;
                snagglebeast_instance_state->voice = FUN_0022da68(9, 4, (int)moby);
            }
            if (target_close) {
                if (MOBY(moby)->prev_seq != 9) {
                    int r = random_int_between(0, 2);
                    FUN_00212f90(moby, 9, r, random_int_between(0x11, 0x19));
                }
                MOBY(moby)->state = SNAGGLEBEAST_ALTERNATING_JOINT_EMISSION_BURST;
            }
        }
        if (follow_target && MOBY(moby)->state != SNAGGLEBEAST_CAPTURE_HERO_AND_DRAIN_AMMO) {
            if (MOBY(moby)->prev_seq != 0xA) {
                int r = random_int_between(0, 4);
                FUN_00212f90(moby, 0xA, r, random_int_between(0x14, 0x1E));
            }
            MOBY(moby)->anim_speed = 1.5f;
            snagglebeast_instance_state->chase_frames = 0;
            MOBY(moby)->state = SNAGGLEBEAST_CHASE_TARGET;
        }
        break;
    case SNAGGLEBEAST_ALTERNATING_JOINT_EMISSION_BURST: {
        float rot;
        rot = MOBY(moby)->rot.z;
        if (level_frame_count % 6 == 0)
            spawn_moby_point_offset_particle((char *)moby, 0, 600000.0f, 5.0f);
        if ((level_frame_count_lui - 2) % 6 == 0)
            spawn_moby_point_offset_particle((char *)moby, 1, 600000.0f, 5.0f);
        if (start_snagglebeast_turn_toward_target(
                moby, snagglebeast_instance_state, (float *)(((char *)&hero) + 0x80), 9, 0))
            break;
        FUN_L00_00258278((struct Moby *)moby,
                         target_angle,
                         (float *)&snagglebeast_instance_state->turn_controller_state,
                         frame_time_sq * 2.0943952f,
                         frame_time_sq * 0.6981317f,
                         frame_time * 2.0943952f);
        if (fast_difference_between_rotations(MOBY(moby)->rot.z, rot) > frame_time * 0.17453292f) {
            if (MOBY(moby)->prev_seq != 0xD) {
                int r = random_int_between(0, 1);
                FUN_00212f90(moby, 0xD, r, random_int_between(0xF, 0x16));
            }
        } else {
            if (MOBY(moby)->prev_seq != 9) {
                int r = random_int_between(0, 1);
                FUN_00212f90(moby, 9, r, random_int_between(0xF, 0x16));
            }
        }
        if (MOBY(moby)->seq == 9 || MOBY(moby)->seq == 0xD)
            MOBY(moby)->state = update_alternating_joint_emission_burst(
                moby, (char *)snagglebeast_instance_state, (float *)&sight);
        if ((MOBY(moby)->command % 3U == 0 && vector_distance_xy(&sight, moby + 0x10) > 13.0f) ||
            (MOBY(moby)->command % 3U == 1 && vector_distance_xy(&sight, moby + 0x10) > 20.0f)) {
            snagglebeast_instance_state->timer_198 = scale_game_frames(0x23);
            snagglebeast_instance_state->timer = scale_game_frames(0x1E);
            MOBY(moby)->state = SNAGGLEBEAST_IDLE;
        }
        break;
    }
    case SNAGGLEBEAST_TRIPLE_SPAWN_ATTACK:
        FUN_L00_00258278((struct Moby *)moby,
                         target_angle,
                         (float *)&snagglebeast_instance_state->turn_controller_state,
                         frame_time_sq * 1.5707964f,
                         frame_time_sq * 0.6981317f,
                         frame_time * 1.0471976f);
        if (is_value_within_interpolated_window(moby, 31.0f) ||
            is_value_within_interpolated_window(moby, 67.0f) ||
            is_value_within_interpolated_window(moby, 101.0f)) {
            float speed;
            snagglebeast_instance_state->substate += 1;
            qcopy(&position, moby + 0x10);
            speed = frame_time * 8.0f;
            snagglebeast_instance_state->turn_controller_state = 0;
            spawn_fading_ring_effect((int)moby, &position, 0x3A, 1.0f, 25.0f, 2.0f, speed, 1.0f);
            FUN_L00_0024f7c8(moby, (snagglebeast_instance_state->substate & 1) + 5, &position);
            configure_timed_point_light(moby,
                                        snagglebeast_instance_state,
                                        1,
                                        &position,
                                        scale_game_frames(0x1E),
                                        0x800060FF,
                                        0,
                                        10.0f);
            position.f[2] = MOBY(moby)->pos.z;
            for (i = 0; i < 55; i++) {
                float r1 = random_angle_radians();
                float r2 = random_float_between(0.3926991f, 0.78539819f);
                FUN_00214db0(&velocity, frame_time * 5.0f, r1, r2);
                fast_vec_normalize(&direction, &velocity, 0.75f);
                direction.f[2] += 0.1f;
                fast_vec_add(&direction, &direction, &position);
                *(int *)&velocity.f[3] = 0;
                spawn_white_particle_with_payload(&direction, &velocity);
            }
            rumble_by_distance((char *)moby);
        }
        if (snagglebeast_instance_state->substate >= 3 &&
            is_value_within_interpolated_window(moby, 51.0f)) {
            if (timer19a) {
                snagglebeast_instance_state->timer_194 = 0xC0;
                snagglebeast_instance_state->timer = 0xAA;
            } else {
                snagglebeast_instance_state->timer_194 = scale_game_frames(0x3C);
                snagglebeast_instance_state->timer = scale_game_frames(0x37);
                if (snagglebeast_instance_state->timer_19A <= scale_game_frames(0x37))
                    snagglebeast_instance_state->timer_19A += scale_game_frames(0x3C);
            }
            MOBY(moby)->anim_speed = 1.0f;
            if (MOBY(moby)->prev_seq != (rand() & 1)) {
                int r1 = rand() & 1;
                int r2 = random_int_between(0, 0xF);
                FUN_00212f90(moby, r1, r2, random_int_between(0x1E, 0x2A));
            }
            MOBY(moby)->state = SNAGGLEBEAST_IDLE;
        }
        break;
    case SNAGGLEBEAST_SWEEP_BEAM_ATTACK:
        if (start_snagglebeast_turn_toward_target(
                moby, snagglebeast_instance_state, (float *)(((char *)&hero) + 0x80), 5, 0))
            break;
        if (snagglebeast_instance_state->timer < scale_game_frames(0x3C) / 2) {
            FUN_L00_0024f7c8(moby, 2, &position);
            configure_timed_point_light(moby,
                                        snagglebeast_instance_state,
                                        2,
                                        &position,
                                        scale_game_frames(0x1E),
                                        0x800060FF,
                                        0,
                                        10.0f);
            if ((level_frame_count & 7) == 0)
                spawn_moby_point_offset_particle((char *)moby, 2, 0.0f, 0.2f);
            if (((level_frame_count - 4) & 7) == 0)
                spawn_moby_point_offset_particle((char *)moby, 3, 0.0f, 0.2f);
        }
        if (MOBY(moby)->seq == 5) {
            qcopy(&position, &sight);
            position.f[2] = probe_ground_height(&position, 0, 0.5f);
            snagglebeast_instance_state->timer += 1;
            if (snagglebeast_instance_state->substate == 0) {
                fast_vec_sub(&velocity, moby + 0x10, &position);
                velocity.f[2] = 0.0f;
                fast_vec_normalize(&velocity, &velocity, 1.0f);
                fast_vec_normalize(&step, &velocity, 0.075f);
                direction.f[0] = fast_cos(sight.rotation.f[2]);
                direction.f[1] = fast_sin(sight.rotation.f[2]);
                direction.f[2] = 0.0f;
                {
                    float direction_velocity_dot = fast_vec_dot(&direction, &velocity);
                    fast_vec_normalize(&projection, &velocity, direction_velocity_dot);
                }
                fast_vec_sub(&direction, &direction, &projection);
                direction.f[2] = 0.0f;
                fast_vec_normalize(&start, &velocity, 8.0f);
                fast_vec_normalize(&direction, &direction, 4.0f);
                fast_vec_add(&start, &start, &position);
                fast_vec_sub(&start, &start, &direction);
                fast_vec_normalize(&direction, &direction, 8.0f);
                fast_vec_add(&direction, &direction, &position);
                fast_vec_normalize(&velocity, &velocity, 2.0f);
                fast_vec_sub(&direction, &direction, &velocity);
                qcopy(&projection, ((char *)&hero) + 0x100);
                projection.f[2] = 0.0f;
                fast_vec_scale(&offset, &projection, (float)scale_game_frames(0x28));
                fast_vec_sub(&velocity, &direction, &start);
                fast_vec_scale(&velocity, &velocity, 0.5f);
                fast_vec_add(&velocity, &velocity, &start);
                fast_vec_sub(&velocity, &velocity, &position);
                fast_vec_sub(&velocity, &velocity, &step);
                fast_vec_sub(&velocity, &offset, &velocity);
                fast_vec_add(&start, &start, &velocity);
                fast_vec_add(&direction, &direction, &velocity);
                qcopy(snagglebeast_instance_state->rope_start_position, &start);
                qcopy(snagglebeast_instance_state->rope_end_position, &direction);
                qcopy(snagglebeast_instance_state->rope_tip_position, &start);
                fast_vec_sub(snagglebeast_instance_state->rope_tip_velocity, &direction, &start);
                fast_vec_scale(snagglebeast_instance_state->rope_tip_velocity,
                               snagglebeast_instance_state->rope_tip_velocity,
                               1.0f / (float)scale_game_frames(0x3C));
                qcopy(snagglebeast_instance_state->rope_target,
                      snagglebeast_instance_state->rope_tip_position);
                *(int *)&snagglebeast_instance_state->rope_target[3] = 0;
                snagglebeast_instance_state->substate = 1;
            } else {
                if (snagglebeast_instance_state->timer < scale_game_frames(0x3C)) {
                    fast_vec_add(snagglebeast_instance_state->rope_tip_position,
                                 snagglebeast_instance_state->rope_tip_position,
                                 snagglebeast_instance_state->rope_tip_velocity);
                    add_draw_callback(draw_collision_clipped_moby_beam, moby);
                    snagglebeast_instance_state->substate = 2;
                } else {
                    snagglebeast_instance_state->substate = 0;
                    snagglebeast_instance_state->timer_19A = scale_game_frames(0xF0);
                    snagglebeast_instance_state->timer = scale_game_frames(0xC8);
                    MOBY(moby)->state = SNAGGLEBEAST_IDLE;
                }
            }
        }
        break;
    case SNAGGLEBEAST_CHASE_TARGET:
    case SNAGGLEBEAST_CHASE_AFTER_TRANSFORM_RESTORE: {
        float thr = 2.0f;
        float rot = MOBY(moby)->rot.z;
        float dd;
        collect_snagglebeast_navigation_ids(moby, snagglebeast_instance_state, &direction);
        if (!path_point_ready) {
            thr = 6.0f;
            if (snagglebeast_instance_state->path_1A0_volume != -1 &&
                (snagglebeast_instance_state->navigation_mask & 3) == 0 &&
                is_point_inside_clip_volume(&sight, snagglebeast_instance_state->path_1A0_volume)) {
                qcopy(&velocity,
                      D_L07_001B0530[snagglebeast_instance_state->path_1A0] +
                          select_point_index_by_distance(
                              &sight, D_L07_001B0530[snagglebeast_instance_state->path_1A0], 0.0f) *
                              16 +
                          0x10);
                dd = vector_distance_xy(moby + 0x10, &velocity);
            } else {
                qcopy(&velocity, &sight);
                qcopy(&velocity,
                      D_L07_001B0530[snagglebeast_instance_state->chase_path] +
                          select_point_index_by_distance(
                              &velocity,
                              D_L07_001B0530[snagglebeast_instance_state->chase_path],
                              0.0f) *
                              16 +
                          0x10);
                dd = vector_distance_xy(moby + 0x10, &velocity);
                if (dd < snagglebeast_instance_state->unk9C + 1.0f)
                    qcopy(&velocity, &sight);
            }
        } else {
            qcopy(&velocity,
                  D_L07_001B0530[snagglebeast_instance_state->nav_path] +
                      snagglebeast_instance_state->target_nav_point * 16 + 0x10);
            dd = vector_distance_xy(moby + 0x10, &velocity);
        }
        update_snagglebeast_chase_speed((struct ChaserAnim *)moby,
                                        (struct ChaserSpeed *)snagglebeast_instance_state,
                                        dd,
                                        thr,
                                        1.5f);
        if ((snagglebeast_instance_state->chase_frames & 3) == 0)
            select_snagglebeast_chase_waypoint((char *)moby,
                                               snagglebeast_instance_state,
                                               &velocity,
                                               snagglebeast_instance_state->rope_tip_position);
        qcopy(&position, snagglebeast_instance_state->rope_tip_position);
        snagglebeast_instance_state->chase_frames += 1;
        if (snagglebeast_instance_state->turn_start_angle <
            fast_difference_between_rotations(MOBY(moby)->rot.z,
                                              fast_arc_tan(position.f[0] - MOBY(moby)->pos.x,
                                                           position.f[1] - MOBY(moby)->pos.y))) {
            MOBY(moby)->anim_speed = 1.0f;
            snagglebeast_instance_state->turn_saved_state = MOBY(moby)->state;
            snagglebeast_instance_state->turn_resume_animation = 0xA;
            snagglebeast_instance_state->turn_fixed_target = 1;
            qcopy(snagglebeast_instance_state->turn_target, &position);
            if (MOBY(moby)->prev_seq != 3) {
                int r = random_int_between(0, 1);
                FUN_00212f90(moby, 3, r, random_int_between(0xF, 0x16));
            }
            MOBY(moby)->state = SNAGGLEBEAST_TURN_TOWARD_TARGET;
        } else {
            int hit =
                update_snagglebeast_chase_movement((struct Moby *)moby,
                                                   (struct ChaserVars *)snagglebeast_instance_state,
                                                   &velocity,
                                                   (int)&position,
                                                   thr);
            float spd;
            int   near;
            int   moving;
            int   four;
            fast_difference_between_rotations(MOBY(moby)->rot.z, rot);
            spd = fast_vec_length(snagglebeast_instance_state->velocity);
            near = 0;
            if (!path_point_ready && target_distance < 20.0f)
                near = 1;
            /* Fakematch: four set on both paths hides the 4 from cse, and the no-op near == 2
             * split is cross-jumped after reload, leaving retail's dead `li v0,2` before the test. */
            if (!(frame_time * 0.01f < spd)) {
                moving = 0;
                four = 4;
            } else {
                moving = 1;
                four = 4;
            }
            if (hit != four && near == 2) {
                if (hit == 4 || !moving)
                    goto stop;
            } else if (hit == 4 || !moving) {
            stop:
                snagglebeast_instance_state->timer_1F7 = scale_game_frames(0x5A);
                if (MOBY(moby)->prev_seq != (rand() & 1)) {
                    int r1 = rand() & 1;
                    int r2 = random_int_between(0, 9);
                    FUN_00212f90(moby, r1, r2, random_int_between(0x14, 0x28));
                }
                snagglebeast_instance_state->target_nav_point = -1;
                MOBY(moby)->state = SNAGGLEBEAST_IDLE;
                break;
            }
            if (MOBY(moby)->prev_seq != 0xA) {
                if (!near && (MOBY(moby)->anim_flags & 2))
                    FUN_00212f90(moby, 0xA, 5, random_int_between(0xA, 0xD));
            } else {
                if (near && (MOBY(moby)->anim_flags & 2))
                    FUN_00212f90(moby, 0xC, 5, random_int_between(0xA, 0xD));
            }
            if (fast_dec_timer(&snagglebeast_instance_state->timer)) {
                if (timer19a) {
                    snagglebeast_instance_state->substate = 0;
                    snagglebeast_instance_state->timer = 0;
                    MOBY(moby)->anim_speed = 1.0f;
                    if (MOBY(moby)->prev_seq != 5) {
                        int r = random_int_between(0, 2);
                        FUN_00212f90(moby, 5, r, random_int_between(0x14, 0x19));
                    }
                    MOBY(moby)->state = SNAGGLEBEAST_SWEEP_BEAM_ATTACK;
                }
                if (timer194) {
                    snagglebeast_instance_state->substate = 0;
                    if (MOBY(moby)->prev_seq != 4) {
                        int r = random_int_between(0, 5);
                        FUN_00212f90(moby, 4, r, random_int_between(0x14, 0x28));
                    }
                    MOBY(moby)->state = SNAGGLEBEAST_TRIPLE_SPAWN_ATTACK;
                    MOBY(moby)->anim_speed = 1.0f;
                }
                if (target_above) {
                    snagglebeast_instance_state->substate = 0;
                    {
                        char *g = ((char *)&hero);
                        qcopy(snagglebeast_instance_state->rope_target,
                              *(char **)(g + 0x2FC) + 0x10);
                    }
                    snagglebeast_instance_state->rope_target[2] += 3.0f;
                    MOBY(moby)->state = SNAGGLEBEAST_ROPE_ATTACK;
                }
                if (sight_ready) {
                    MOBY(moby)->anim_speed = 1.0f;
                    if (MOBY(moby)->prev_seq != 6) {
                        int r = random_int_between(0, 2);
                        FUN_00212f90(moby, 6, r, random_int_between(0x14, 0x19));
                    }
                    snagglebeast_instance_state->substate = 0;
                    snagglebeast_instance_state->rope_extension_length = 0.0f;
                    snagglebeast_instance_state->hero_captured = 0;
                    snagglebeast_instance_state->capture_turn_done = 0;
                    MOBY(moby)->state = SNAGGLEBEAST_CAPTURE_HERO_AND_DRAIN_AMMO;
                    snagglebeast_instance_state->voice = FUN_0022da68(9, 4, (int)moby);
                }
            }
            if (!follow_target) {
                MOBY(moby)->anim_speed = 1.0f;
                if (MOBY(moby)->prev_seq != (rand() & 1)) {
                    int r1 = rand() & 1;
                    int r2 = random_int_between(0, 2);
                    FUN_00212f90(moby, r1, r2, random_int_between(0x1C, 0x28));
                }
                MOBY(moby)->state = SNAGGLEBEAST_IDLE;
            }
            if (MOBY(moby)->seq == 0xC)
                update_alternating_joint_emission_burst(
                    moby, (char *)snagglebeast_instance_state, (float *)&sight);
        }
        if (MOBY(moby)->seq == 0xA || MOBY(moby)->seq == 0xC) {
            if (is_value_within_interpolated_window(moby, 27.5f))
                rumble_by_distance((char *)moby);
            if (is_value_within_interpolated_window(moby, 73.0f))
                rumble_by_distance((char *)moby);
        }
        break;
    }
    case SNAGGLEBEAST_TURN_TOWARD_TARGET: {
        float target_heading_delta;
        if (snagglebeast_instance_state->turn_fixed_target == 0)
            qcopy(snagglebeast_instance_state->turn_target, ((char *)&hero) + 0x80);
        target_heading_delta = fast_subtract_rotations(
            fast_arc_tan(snagglebeast_instance_state->turn_target[0] - MOBY(moby)->pos.x,
                         snagglebeast_instance_state->turn_target[1] - MOBY(moby)->pos.y),
            MOBY(moby)->rot.z);
        if (!target_close && (MOBY(moby)->anim_flags & 2)) {
            if (target_heading_delta < 0.0001f) {
                if (MOBY(moby)->prev_seq != 3)
                    FUN_00212f90(moby, 3, 5, random_int_between(0xA, 0xD));
            } else {
                if (MOBY(moby)->prev_seq != 0xE)
                    FUN_00212f90(moby, 0xE, 5, random_int_between(0xA, 0xD));
            }
        }
        if (MOBY(moby)->seq == 3 || MOBY(moby)->seq == 0xE || MOBY(moby)->seq == 0xD) {
            float dz = compute_interpolated_record_value(moby);
            if (dz < 14.5f || (dz > 19.0f && dz < 35.5f) || dz > 40.5f) {
                FUN_L00_00258278(
                    (struct Moby *)moby,
                    fast_arc_tan(snagglebeast_instance_state->turn_target[0] - MOBY(moby)->pos.x,
                                 snagglebeast_instance_state->turn_target[1] - MOBY(moby)->pos.y),
                    (float *)&snagglebeast_instance_state->turn_controller_state,
                    frame_time_sq * 2.0943952f,
                    frame_time_sq * 0.6981317f,
                    frame_time * 2.0943952f);
            }
            if (target_heading_delta > 0.001f && MOBY(moby)->prev_seq == 3) {
                FUN_00212f90(moby, 0xE, 5, random_int_between(0xA, 0xD));
            } else if (target_heading_delta < 0.001f && MOBY(moby)->prev_seq == 0xE) {
                FUN_00212f90(moby, 3, 5, random_int_between(0xA, 0xD));
            }
        } else {
            snagglebeast_instance_state->unk84 = 0;
        }
        if (fast_difference_between_rotations(
                MOBY(moby)->rot.z,
                fast_arc_tan(snagglebeast_instance_state->turn_target[0] - MOBY(moby)->pos.x,
                             snagglebeast_instance_state->turn_target[1] - MOBY(moby)->pos.y)) <
            snagglebeast_instance_state->turn_start_angle * 0.25f) {
            qzero(snagglebeast_instance_state->velocity);
            if (MOBY(moby)->prev_seq != snagglebeast_instance_state->turn_resume_animation) {
                int anim = snagglebeast_instance_state->turn_resume_animation;
                int r = random_int_between(0, 2);
                FUN_00212f90(moby, anim, r, random_int_between(0xF, 0x1B));
            }
            MOBY(moby)->state = snagglebeast_instance_state->turn_saved_state;
        }
        if (MOBY(moby)->seq == 3 || MOBY(moby)->seq == 0xE) {
            if (is_value_within_interpolated_window(moby, 16.0f))
                rumble_by_distance((char *)moby);
            if (is_value_within_interpolated_window(moby, 37.0f))
                rumble_by_distance((char *)moby);
        } else if (MOBY(moby)->seq == 0xD) {
            if (is_value_within_interpolated_window(moby, 13.0f))
                rumble_by_distance((char *)moby);
            if (is_value_within_interpolated_window(moby, 34.0f))
                rumble_by_distance((char *)moby);
        }
        break;
    }
    case SNAGGLEBEAST_START_TIMED_DAMAGE_SEQUENCE: {
        int k;
        if (snagglebeast_instance_state->health <=
            convert_integer_to_float(snagglebeast_instance_state->max_health / 6)) {
            if (MOBY(moby)->prev_seq != 8)
                FUN_00212f90(moby, 8, 0x35, random_int_between(0xA, 0xD));
        } else {
            if (MOBY(moby)->prev_seq != 0xB)
                FUN_00212f90(moby, 0xB, 0x3D, 1);
        }
        MOBY(moby)->state = SNAGGLEBEAST_WAIT_BEFORE_TIMED_DAMAGE;
        snagglebeast_instance_state->velocity[2] = 0.0f;
        if (MOBY(moby)->command % 3U < 2)
            MOBY(moby)->command += 1;
        if (MOBY(moby)->command % 3U < 2)
            MOBY(moby)->command += 1;
        if (MOBY(moby)->command >= 8)
            MOBY(moby)->command = 8;
        if (((ChaserPathPoints *)snagglebeast_instance_state)
                ->linked_mobys[snagglebeast_instance_state->path_select] != -1) {
            int k2;
            k = snagglebeast_instance_state->path_select;
            k2 = k + 3;
            if (((ChaserPathPoints *)snagglebeast_instance_state)->linked_mobys[k2] != -1) {
                char *linked_moby_vars =
                    *(char **)(level_mobys +
                               (((ChaserPathPoints *)snagglebeast_instance_state)->linked_mobys[k]
                                << 8) +
                               0x78);
                char *paired_linked_moby_vars =
                    *(char **)(level_mobys +
                               (((ChaserPathPoints *)snagglebeast_instance_state)->linked_mobys[k2]
                                << 8) +
                               0x78);
                if (*(int *)(linked_moby_vars + 0x28) != -1 &&
                    *(int *)(paired_linked_moby_vars + 0x28) != -1) {
                    float hero_to_linked_cuboid_distance = vector_distance_xy(
                        ((char *)&hero) + 0x80,
                        (char *)l07_cuboids + (*(int *)(linked_moby_vars + 0x28) << 7) + 0x30);
                    int sel;
                    if (vector_distance_xy(((char *)&hero) + 0x80,
                                           (char *)l07_cuboids +
                                               (*(int *)(paired_linked_moby_vars + 0x28) << 7) +
                                               0x30) < hero_to_linked_cuboid_distance) {
                        sel = *(int *)(paired_linked_moby_vars + 0x28);
                        k = k2;
                    } else {
                        sel = *(int *)(linked_moby_vars + 0x28);
                    }
                    MOBY(moby)->rot.z = fast_arc_tan(
                        *(float *)((char *)l07_cuboids + sel * 0x80 + 0x30) - MOBY(moby)->pos.x,
                        *(float *)((char *)l07_cuboids + sel * 0x80 + 0x34) - MOBY(moby)->pos.y);
                }
            }
            *(unsigned char *)(level_mobys +
                               ((ChaserPathPoints *)snagglebeast_instance_state)->linked_mobys[k] *
                                   0x100 +
                               0xBC) = 1;
        }
        break;
    }
    case SNAGGLEBEAST_WAIT_BEFORE_TIMED_DAMAGE: {
        int done = 0;
        if (compute_interpolated_record_value(moby) > 10.0f) {
            FUN_L00_0024f7c8(moby, 0xA, &position);
            position.f[2] = 34.6f;
            if (FUN_00213260(0x1A) == 0)
                spawn_moving_resizing_height_snapped_alpha_fade_particle(
                    &position, 0, 0x40103080, 1.6f, 14700.0f);
        }
        if (MOBY(moby)->command == 8) {
            float z = snagglebeast_instance_state->velocity[2];
            fast_vec_scale(
                snagglebeast_instance_state->velocity, snagglebeast_instance_state->velocity, 0.7f);
            (snagglebeast_instance_state->velocity)[2] = z;
            (snagglebeast_instance_state->velocity)[2] -= frame_time * 9.8f;
            fast_vec_add(moby + 0x10, moby + 0x10, snagglebeast_instance_state->velocity);
            if (MOBY(moby)->pos.z < snagglebeast_instance_state->ground_z)
                done = 1;
        } else {
            if (compute_interpolated_record_value(moby) > 79.0f)
                done = 1;
        }
        if (done) {
            snagglebeast_instance_state->timer = scale_game_frames(0xA);
            snagglebeast_instance_state->sphere_fade_direction = 2;
            MOBY(moby)->state = SNAGGLEBEAST_APPLY_TIMED_DAMAGE_AND_SELECT_PATH;
            snagglebeast_instance_state->substate = snagglebeast_instance_state->max_health / 6;
        }
        break;
    }
    case SNAGGLEBEAST_APPLY_TIMED_DAMAGE_AND_SELECT_PATH:
        fast_vec_scale(
            snagglebeast_instance_state->velocity, snagglebeast_instance_state->velocity, 0.4f);
        fast_vec_add(moby + 0x10, moby + 0x10, snagglebeast_instance_state->velocity);
        if (compute_interpolated_record_value(moby) < 168.0f) {
            FUN_L00_0024f7c8(moby, 0xA, &position);
            position.f[2] = 34.6f;
            if (FUN_00213260(0x1A) == 0)
                spawn_moving_resizing_height_snapped_alpha_fade_particle(
                    &position, 0, 0x40103080, 1.6f, 14700.0f);
        }
        if (is_value_within_interpolated_window(moby, 168.0f)) {
            FUN_L00_0024f7c8(moby, 5, &position);
            spawn_fading_ring_effect(
                (int)moby, &position, 0x3A, 1.0f, 25.0f, 2.0f, frame_time * 8.0f, 1.0f);
        }
        if (is_value_within_interpolated_window(moby, 168.5f)) {
            FUN_L00_0024f7c8(moby, 6, &position);
            spawn_fading_ring_effect(
                (int)moby, &position, 0x3A, 1.0f, 25.0f, 2.0f, frame_time * 8.0f, 1.0f);
            rumble_by_distance((char *)moby);
        }
        if (snagglebeast_instance_state->substate > 0) {
            if (fast_dec_timer(&snagglebeast_instance_state->timer)) {
                snagglebeast_instance_state->substate -= 1;
                *health -= 1.0f;
                snagglebeast_instance_state->hit_flash.flash_red = 0xFA;
                start_hit_color_flash(moby, &snagglebeast_instance_state->hit_flash);
                if (*health <= 0.0f) {
                    *health = 0.0f;
                    if (MOBY(moby)->prev_seq != 8)
                        FUN_00212f90(moby, 8, 0, random_int_between(0xA, 0xD));
                    if (snagglebeast_instance_state->cuboid != -1) {
                        set_moby_dialogue_record_value(moby, 1);
                        FUN_L00_00257470(moby, 0, -1);
                        jet_clear_persistent(MOBY(moby)->mission);
                        jet_reset_camera((char *)l07_cuboids +
                                             (snagglebeast_instance_state->cuboid << 7) + 0x30,
                                         (char *)l07_cuboids +
                                             (snagglebeast_instance_state->cuboid << 7) + 0x70);
                    }
                    MOBY(moby)->state = SNAGGLEBEAST_DEATH_EXPLOSION;
                } else {
                    if (snagglebeast_instance_state->substate > 0) {
                        snagglebeast_instance_state->timer = scale_game_frames(0xA);
                    } else {
                        snagglebeast_instance_state->timer = scale_game_frames(0x14);
                    }
                }
            }
        } else {
            if (fast_dec_timer(&snagglebeast_instance_state->timer)) {
                switch (snagglebeast_instance_state->path_select) {
                case SNAGGLEBEAST_PATH_SELECT_FIRST_POINT:
                    qcopy(snagglebeast_instance_state->rope_end_position,
                          D_L07_001B0530[snagglebeast_instance_state->path_1A0] + 0x10);
                    snagglebeast_instance_state->navigation_mask &= 0xFE;
                    break;
                case SNAGGLEBEAST_PATH_SELECT_SECOND_POINT:
                    qcopy(snagglebeast_instance_state->rope_end_position,
                          D_L07_001B0530[snagglebeast_instance_state->path_1A0] + 0x20);
                    snagglebeast_instance_state->navigation_mask &= 0xFD;
                    break;
                case SNAGGLEBEAST_PATH_SELECT_FARTHER_PAIRED_POINT: {
                    float hero_to_first_paired_path_point_distance = vector_distance_xy(
                        ((char *)&hero) + 0x80,
                        D_L07_001B0530[snagglebeast_instance_state->path_1A0] + 0x30);
                    if (vector_distance_xy(((char *)&hero) + 0x80,
                                           D_L07_001B0530[snagglebeast_instance_state->path_1A0] +
                                               0x40) < hero_to_first_paired_path_point_distance) {
                        qcopy(snagglebeast_instance_state->rope_end_position,
                              D_L07_001B0530[snagglebeast_instance_state->path_1A0] + 0x30);
                    } else {
                        qcopy(snagglebeast_instance_state->rope_end_position,
                              D_L07_001B0530[snagglebeast_instance_state->path_1A0] + 0x40);
                    }
                    snagglebeast_instance_state->navigation_mask &= 0xFB;
                    break;
                }
                }
                snagglebeast_instance_state->target_nav_point =
                    ((ChaserPathPoints *)snagglebeast_instance_state)
                        ->path_points[snagglebeast_instance_state->path_select];
                build_symmetric_adjacency_masks((int *)&position,
                                                (int)collect_snagglebeast_navigation_ids(
                                                    moby, snagglebeast_instance_state, &position),
                                                snagglebeast_instance_state->nav_path);
                MOBY(moby)->state = SNAGGLEBEAST_RESTORE_TRANSFORM_AND_RESUME_CHASE;
            }
        }
        break;
    case SNAGGLEBEAST_RESTORE_TRANSFORM_AND_RESUME_CHASE: {
        int point;
        if (compute_interpolated_record_value(moby) > 198.0f) {
            if (MOBY(moby)->prev_seq != 0xA)
                FUN_00212f90(moby, 0xA, 0, 0);
            FUN_L00_0024f7c8(moby, 0xA, &position);
            qcopy(snagglebeast_instance_state->turn_target, &position);
            snagglebeast_instance_state->turn_target[2] =
                snagglebeast_instance_state->rope_end_position[2];
            point = 0xA;
            get_moby_joint_matrices(moby, 1, &point, &velocity);
            matrix_to_euler(&velocity, &step);
            snagglebeast_instance_state->turn_target[3] =
                fast_add_rotations(MOBY(moby)->rot.z, step.f[2]);
        }
        if (MOBY(moby)->seq == 0xA) {
            qcopy(moby + 0x10, snagglebeast_instance_state->turn_target);
            MOBY(moby)->rot.z = snagglebeast_instance_state->turn_target[3];
            if (((ChaserPathPoints *)snagglebeast_instance_state)
                    ->linked_mobys[snagglebeast_instance_state->path_select] != -1) {
                *(unsigned char
                      *)(level_mobys +
                         ((ChaserPathPoints *)snagglebeast_instance_state)
                                 ->linked_mobys[snagglebeast_instance_state->path_select] *
                             0x100 +
                         0xBC) = 0;
            }
            MOBY(moby)->anim_speed = 1.5f;
            MOBY(moby)->command += 1;
            snagglebeast_instance_state->chase_frames = 0;
            MOBY(moby)->state = SNAGGLEBEAST_CHASE_AFTER_TRANSFORM_RESTORE;
            snagglebeast_instance_state->timer = scale_game_frames(0x37);
        }
        break;
    }
    case SNAGGLEBEAST_CAPTURE_HERO_AND_DRAIN_AMMO: {
        float fr;
        if (snagglebeast_instance_state->capture_turn_done <= 0) {
            if (start_snagglebeast_turn_toward_target(
                    moby, snagglebeast_instance_state, (float *)(((char *)&hero) + 0x80), 6, 0))
                break;
        }
        ((int *)snagglebeast_instance_state)[0xA5] = 1;
        FUN_L00_00258278((struct Moby *)moby,
                         hero_angle,
                         (float *)&snagglebeast_instance_state->turn_controller_state,
                         frame_time_sq * 2.0943952f,
                         frame_time_sq * 0.6981317f,
                         frame_time * 2.0943952f);
        FUN_L00_0024f7c8(moby, 4, &position);
        fr = update_snagglebeast_rope(moby, &position);
        if (MOBY(moby)->seq != 6)
            break;
        qcopy(snagglebeast_instance_state->rope_tip_position, ((char *)&hero) + 0x80);
        snagglebeast_instance_state->rope_tip_position[2] += 0.6f;
        if (snagglebeast_instance_state->rope_extension_length < hero_distance) {
            float step_time = frame_time * 15.0f;
            snagglebeast_instance_state->timer = 0;
            snagglebeast_instance_state->rope_extension_length += step_time;
        } else {
            if (snagglebeast_instance_state->hero_captured != 0 ||
                (fast_dec_timer(&snagglebeast_instance_state->timer) && fr < 0.6f)) {
                int   slot;
                char *g = ((char *)&hero);
                snagglebeast_instance_state->hero_captured = 1;
                if (*(int *)(g + 0x2084) != 0x78) {
                    qcopy(snagglebeast_instance_state->rope_start_position, ((char *)&hero) + 0x80);
                    qcopy(snagglebeast_instance_state->rope_end_position, ((char *)&hero) + 0x80);
                    if (MOBY(moby)->pos.z < *(float *)(g + 0x88)) {
                        snagglebeast_instance_state->rope_end_position[2] =
                            *(float *)(g + 0x88) + 2.0f;
                    } else {
                        snagglebeast_instance_state->rope_end_position[2] =
                            MOBY(moby)->pos.z + 2.0f;
                    }
                    hero_set_state(0x78, 1);
                    snagglebeast_instance_state->substate = 0;
                    snagglebeast_instance_state->drop_path_distance = 20.0f;
                    snagglebeast_instance_state->ammo_drain_step = 0;
                }
                fast_vec_sub(&velocity,
                             snagglebeast_instance_state->rope_end_position,
                             snagglebeast_instance_state->rope_start_position);
                fast_vec_scale(&velocity, &velocity, frame_scale * 0.1f);
                fast_vec_add(snagglebeast_instance_state->rope_start_position,
                             snagglebeast_instance_state->rope_start_position,
                             &velocity);
                qcopy(((char *)&hero) + 0x1D30, snagglebeast_instance_state->rope_start_position);
                qcopy(((char *)&hero) + 0x80, snagglebeast_instance_state->rope_start_position);
                {
                    char *captured_hero = ((char *)&hero);
                    *(float *)(captured_hero + 0x1D48) = fast_add_rotations(
                        *(float *)(captured_hero + 0x1D48), frame_time * 25.132742f);
                    *(float *)(captured_hero + 0x98) = fast_add_rotations(
                        *(float *)(captured_hero + 0x1D48), frame_time * 25.132742f);
                }
                snagglebeast_instance_state->rope_extension_length = hero_distance;
                slot = snagglebeast_ammo_drain_entries[snagglebeast_instance_state->substate][0];
                if (slot == -1) {
                    snagglebeast_instance_state->substate = 0;
                    snagglebeast_instance_state->timer_196 = scale_game_frames(0x320);
                    snagglebeast_instance_state->timer = scale_game_frames(0xF0);
                    detach_snagglebeast_rope_particles((int)moby);
                    {
                        char *g3 = ((char *)&hero);
                        if (*(int *)(g3 + 0x2084) == 0x78)
                            hero_set_state(0, 1);
                    }
                    snagglebeast_instance_state->hero_capture_attack_enabled = 0;
                    if (snagglebeast_instance_state->voice != -1) {
                        char *sl = (voice_slot_state_base - 0x74) +
                                   snagglebeast_instance_state->voice * 0x70;
                        if (*(void **)(sl + 0x88) == moby && *(unsigned char *)(sl + 0x74) != 0) {
                            release_voice_slot(snagglebeast_instance_state->voice);
                        }
                    }
                    snagglebeast_instance_state->voice = -1;
                    MOBY(moby)->state = SNAGGLEBEAST_IDLE;
                } else {
                    int ammo_transfer_count = (l07_ammo_transfer_records + slot)->count;
                    int rem;
                    int pos;
                    if (snagglebeast_instance_state->ammo_drain_step == 0) {
                        snagglebeast_instance_state->ammo_drain_step =
                            ((int *)&weapon_ammo_counts)[slot] / 8;
                        if (snagglebeast_instance_state->ammo_drain_step < ammo_transfer_count)
                            snagglebeast_instance_state->ammo_drain_step = ammo_transfer_count;
                        snagglebeast_instance_state->drop_path_distance = random_float_between(
                            snagglebeast_instance_state->drop_path_distance + 20.0f,
                            snagglebeast_instance_state->drop_path_distance + 100.0f);
                    }
                    ammo_transfer_count = snagglebeast_instance_state->ammo_drain_step;
                    if (((int *)&weapon_ammo_counts)[slot] < ammo_transfer_count) {
                        ammo_transfer_count = ((int *)&weapon_ammo_counts)[slot];
                        snagglebeast_instance_state->ammo_drain_step = 0;
                        snagglebeast_instance_state->substate += 1;
                    }
                    rem = ((int *)&weapon_ammo_counts)[slot] - ammo_transfer_count * 8;
                    pos = rem > 0;
                    ammo_transfer_count = ammo_transfer_count + pos * rem;
                    if (ammo_transfer_count > 0) {
                        float zv = 0.0f;
                        if (snagglebeast_instance_state->measured_path == -1) {
                            FUN_L00_00257d78(
                                (float *)&velocity, frame_time * 4.0f, frame_time * 10.0f);
                            if (velocity.f[2] < frame_time * 2.0f)
                                velocity.f[2] =
                                    random_float_between(frame_time * 3.0f, frame_time * 6.0f);
                            fast_vec_normalize(&direction, &velocity, 0.9f);
                            fast_vec_add(&direction, &direction, ((char *)&hero) + 0x80);
                        } else {
                            char *path = D_L07_001B0530[snagglebeast_instance_state->measured_path];
                            int   pidx = 0;
                            float pfrac = 0.0f;
                            float hero_to_sample_distance;
                            float moby_to_sample_distance;
                            pidx =
                                select_point_index_by_distance(((char *)&hero) + 0x80, path, 0.0f);
                            do {
                                snagglebeast_instance_state->drop_path_distance += 10.0f;
                                sample_path(path,
                                            &start,
                                            &pidx,
                                            &pfrac,
                                            snagglebeast_instance_state->drop_path_distance,
                                            1);
                                hero_to_sample_distance =
                                    vector_distance_xy(((char *)&hero) + 0x80, &start);
                                moby_to_sample_distance = vector_distance_xy(moby + 0x10, &start);
                            } while (hero_to_sample_distance < 8.0f ||
                                     moby_to_sample_distance < 12.0f);
                            add_random_xy_offset((float *)&start, 0.5f);
                            fast_vec_sub(&direction, &start, ((char *)&hero) + 0x80);
                            zv = start.f[2];
                            fast_vec_normalize(&direction, &direction, 0.9f);
                            qcopy(&velocity, &direction);
                            velocity.f[2] = 0.0f;
                            fast_vec_normalize(&velocity, &velocity, frame_time * 15.0f);
                            fast_vec_add(&direction, &direction, ((char *)&hero) + 0x80);
                            velocity.f[2] = FUN_L00_0025abf0(
                                &direction, &start, 0, frame_time * 15.0f, frame_time_sq * -9.8f);
                            snagglebeast_instance_state->drop_path_distance +=
                                random_float_between(0.7f, 2.0f);
                        }
                        FUN_0022da68(0xB, 0, (int)moby);
                        {
                            char *spawned_moby =
                                spawn_on_path(&direction, slot, ammo_transfer_count, 0);
                            if (spawned_moby != 0) {
                                qcopy_nc(((char *)MOBY(spawned_moby)->pvars) + 0x10, &velocity);
                                *(float *)(((char *)MOBY(spawned_moby)->pvars) + 0x28) = zv;
                                ((int *)&weapon_ammo_counts)[slot] -= ammo_transfer_count;
                                snagglebeast_instance_state->timer = scale_game_frames(0);
                            }
                        }
                    }
                }
            }
        }
        if (!sight_ready) {
            snagglebeast_instance_state->substate = 0;
            snagglebeast_instance_state->timer_196 = scale_game_frames(0xF);
            detach_snagglebeast_rope_particles((int)moby);
            {
                char *g3 = ((char *)&hero);
                if (*(int *)(g3 + 0x2084) == 0x78)
                    hero_set_state(0, 1);
            }
            snagglebeast_instance_state->timer = scale_game_frames(0x46);
            snagglebeast_instance_state->hero_capture_attack_enabled = 0;
            if (snagglebeast_instance_state->voice != -1) {
                char *sl =
                    (voice_slot_state_base - 0x74) + snagglebeast_instance_state->voice * 0x70;
                if (*(void **)(sl + 0x88) == moby && *(unsigned char *)(sl + 0x74) != 0) {
                    release_voice_slot(snagglebeast_instance_state->voice);
                }
            }
            snagglebeast_instance_state->voice = -1;
            MOBY(moby)->state = SNAGGLEBEAST_IDLE;
        }
        if (target_close) {
            snagglebeast_instance_state->timer_196 = scale_game_frames(0x1E);
            if (MOBY(moby)->prev_seq != 9) {
                int r = random_int_between(0, 2);
                FUN_00212f90(moby, 9, r, random_int_between(0x10, 0x19));
            }
            detach_snagglebeast_rope_particles((int)moby);
            {
                char *g3 = ((char *)&hero);
                if (*(int *)(g3 + 0x2084) == 0x78)
                    hero_set_state(0, 1);
            }
            if (snagglebeast_instance_state->voice != -1) {
                char *sl =
                    (voice_slot_state_base - 0x74) + snagglebeast_instance_state->voice * 0x70;
                if (*(void **)(sl + 0x88) == moby && *(unsigned char *)(sl + 0x74) != 0) {
                    release_voice_slot(snagglebeast_instance_state->voice);
                }
            }
            snagglebeast_instance_state->voice = -1;
            MOBY(moby)->state = SNAGGLEBEAST_ALTERNATING_JOINT_EMISSION_BURST;
        }
        break;
    }
    case SNAGGLEBEAST_ROPE_ATTACK:
        if (snagglebeast_instance_state->substate <= 0) {
            if (start_snagglebeast_turn_toward_target(moby,
                                                      snagglebeast_instance_state,
                                                      snagglebeast_instance_state->rope_target,
                                                      7,
                                                      1))
                break;
        }
        FUN_L00_00258278(
            (struct Moby *)moby,
            fast_arc_tan(snagglebeast_instance_state->rope_target[0] - MOBY(moby)->pos.x,
                         snagglebeast_instance_state->rope_target[1] - MOBY(moby)->pos.y),
            (float *)&snagglebeast_instance_state->turn_controller_state,
            frame_time_sq * 2.0943952f,
            frame_time_sq * 0.6981317f,
            frame_time * 2.0943952f);
        switch (snagglebeast_instance_state->substate) {
        case SNAGGLEBEAST_ROPE_START_ATTACK_ANIMATION:
            if (MOBY(moby)->prev_seq != 7)
                FUN_00212f90(moby, 7, 0, random_int_between(0xA, 0xD));
            snagglebeast_instance_state->timer = 0;
            snagglebeast_instance_state->substate = 1;
            break;
        case SNAGGLEBEAST_ROPE_WAIT_FOR_ORIGIN_FRAME:
            if ((*(u64 *)(moby + 0x50) & 0xFFFF0000) != 0x07070000 && MOBY(moby)->prev_seq != 7) {
                FUN_00212f90(moby, 7, 0, random_int_between(0xA, 0xD));
            }
            if (MOBY(moby)->seq == 7) {
                if (compute_interpolated_record_value(moby) >= 9.5f) {
                    FUN_L00_0024f7c8(moby, 0, &velocity);
                    FUN_L00_0024f7c8(moby, 1, &direction);
                    fast_vec_add(&position, &velocity, &direction);
                    fast_vec_scale(&position, &position, 0.5f);
                    qcopy(snagglebeast_instance_state->rope_start_position, &position);
                    fast_vec_sub(snagglebeast_instance_state->rope_tip_velocity,
                                 snagglebeast_instance_state->rope_target,
                                 snagglebeast_instance_state->rope_start_position);
                    fast_vec_normalize(snagglebeast_instance_state->rope_tip_velocity,
                                       snagglebeast_instance_state->rope_tip_velocity,
                                       frame_scale);
                    snagglebeast_instance_state->substate = 2;
                }
            }
            break;
        case SNAGGLEBEAST_ROPE_WAIT_FOR_EXTENSION_FRAME:
            if ((*(u64 *)(moby + 0x50) & 0xFFFF0000) != 0x07070000 && MOBY(moby)->prev_seq != 7) {
                FUN_00212f90(moby, 7, 0, random_int_between(7, 0xA));
            }
            if (fast_dec_timer(&snagglebeast_instance_state->timer)) {
                snagglebeast_instance_state->timer = scale_game_frames(0xA);
                spawn_moby_point_offset_particle((char *)moby, 0, 800000.0f, 5.0f);
                spawn_moby_point_offset_particle((char *)moby, 1, 800000.0f, 5.0f);
            }
            if (compute_interpolated_record_value(moby) >= 31.5f) {
                snagglebeast_instance_state->rope_extension_length = 0.0f;
                snagglebeast_instance_state->substate = 3;
                *(int *)&snagglebeast_instance_state->rope_start_position[3] = 0;
            }
            break;
        case SNAGGLEBEAST_ROPE_EXTEND: {
            float r = vector_distance_xy(moby + 0x10, &sight.position30);
            snagglebeast_instance_state->rope_extension_length += frame_scale;
            if (r <= snagglebeast_instance_state->rope_extension_length)
                snagglebeast_instance_state->substate = 4;
            if (fast_dec_timer(&snagglebeast_instance_state->timer))
                snagglebeast_instance_state->timer = scale_game_frames(0x14);
            add_draw_callback(draw_and_probe_snagglebeast_rope_attack, moby);
            break;
        }
        case SNAGGLEBEAST_ROPE_RETRACT:
            fast_vec_sub(&position,
                         snagglebeast_instance_state->rope_end_position,
                         snagglebeast_instance_state->rope_start_position);
            fast_vec_normalize(&position, &position, frame_scale);
            fast_vec_add(snagglebeast_instance_state->rope_start_position,
                         snagglebeast_instance_state->rope_start_position,
                         &position);
            configure_timed_point_light(moby,
                                        snagglebeast_instance_state,
                                        3,
                                        snagglebeast_instance_state->rope_start_position,
                                        scale_game_frames(0x1E),
                                        0x800060FF,
                                        0,
                                        10.0f);
            if (snagglebeast_instance_state->substate != 0) {
                if (fast_dec_timer(&snagglebeast_instance_state->timer))
                    snagglebeast_instance_state->substate = 5;
            }
            add_draw_callback(draw_and_probe_snagglebeast_rope_attack, moby);
            break;
        case SNAGGLEBEAST_ROPE_RETURN_TO_IDLE:
            MOBY(moby)->anim_speed = 1.0f;
            snagglebeast_instance_state->substate = 0;
            snagglebeast_instance_state->timer = 0;
            if (MOBY(moby)->prev_seq != (rand() & 1)) {
                int r1 = rand() & 1;
                int r2 = random_int_between(0, 7);
                FUN_00212f90(moby, r1, r2, random_int_between(0x1B, 0x2D));
            }
            MOBY(moby)->state = SNAGGLEBEAST_IDLE;
            break;
        }
        break;
    case SNAGGLEBEAST_DEATH_EXPLOSION:
        if (snagglebeast_instance_state->health_hud_handle != -1) {
            set_animation_parameter(snagglebeast_instance_state->health_hud_handle, 0);
            snagglebeast_instance_state->health_hud_handle = -1;
        }
        if (MOBY(moby)->anim_flags & 2) {
            fast_vec_normalize(
                snagglebeast_instance_state->velocity, snagglebeast_instance_state->velocity, 1.0f);
            snagglebeast_instance_state->velocity[2] = 1.0f;
            qcopy(&position, moby + 0x10);
            position.f[2] = snagglebeast_instance_state->ground_z + 1.0f;
            spawn_explosion(moby,
                            snagglebeast_instance_state->velocity,
                            &position,
                            0.0f,
                            0.0f,
                            0x14,
                            0xC,
                            0x40,
                            7.0f,
                            5.0f,
                            10.0f,
                            1.0f,
                            -1,
                            50.0f,
                            1,
                            1,
                            -1,
                            0);
            MOBY(moby)->flags |= 0x41;
            if (snagglebeast_instance_state->death_effect_moby != -1)
                initialize_moby_particle_effect_state(
                    level_mobys + (snagglebeast_instance_state->death_effect_moby << 8));
            MOBY(moby)->state = SNAGGLEBEAST_POST_DEATH_CLEANUP;
            MOBY(moby)->pos.z = 20.0f;
        }
        break;
    case SNAGGLEBEAST_WAIT_AFTER_HEALTH_THRESHOLD_HIT:
        if (MOBY(moby)->anim_flags & 2) {
            if (target_above) {
                snagglebeast_instance_state->substate = 0;
                {
                    char *g = ((char *)&hero);
                    qcopy(snagglebeast_instance_state->rope_target, *(char **)(g + 0x2FC) + 0x10);
                }
                snagglebeast_instance_state->rope_target[2] += 3.0f;
                MOBY(moby)->state = SNAGGLEBEAST_ROPE_ATTACK;
            } else {
                MOBY(moby)->state = SNAGGLEBEAST_IDLE;
            }
        }
        break;
    case SNAGGLEBEAST_POST_DEATH_CLEANUP:
        if (snagglebeast_instance_state->point_light != -1)
            release_point_light_slot(snagglebeast_instance_state->point_light);
        if (snagglebeast_instance_state->death_raised_moby == -1 ||
            !(*(float *)(level_mobys + (snagglebeast_instance_state->death_raised_moby << 8) +
                         0x18) < 34.5f)) {
            mark_moby_for_removal(moby);
            return;
        }
        *(float *)(level_mobys + (snagglebeast_instance_state->death_raised_moby << 8) + 0x18) +=
            (34.6f - *(float *)(level_mobys +
                                (snagglebeast_instance_state->death_raised_moby << 8) + 0x18)) *
            0.1f;
        break;
    case 17:
        break;
    }
    fast_vec_sub(snagglebeast_instance_state->velocity, moby + 0x10, &oldpos);
    if (MOBY(moby)->state < SNAGGLEBEAST_DEATH_EXPLOSION)
        jet_animate((struct Moby *)moby, 2.5f);
    update_timed_point_light_color((char *)snagglebeast_instance_state);
}

float scroll_phase __asm__("D_L07_00161B74") = 0;

float row_start_phase __asm__("D_L07_00161B78") = 0;

float row_phase_step_gp __asm__("D_L07_00161B7C") = 0.006f;

extern short rope_row_alpha[] __asm__("D_L07_0020DBC8");

typedef struct {
    RopeVec pos[4];
    u32     color[4];
    float   uv[8];
    u64     header[4];
} L07RopeDrawPacket;

extern RopeVec rope_grid[][20] __asm__("D_L07_0020DBE0");

/* Draws the rope grid as textured quads between neighbouring rows, each row faded by its level. */
void draw_rope_strips(void) __asm__("FUN_L07_00317910");

void draw_rope_strips(void) {
    L07RopeDrawPacket packet;
    float             phase;
    float             row_phase;
    float             end_phase;
    int               row;
    int               column;
    int               k;
    int               color;
    float             phase_step;
    float             column_step;

    packet.header[1] = get_effect_texture(0x15);
    packet.header[2] = 0xff9000000260ULL;
    packet.header[3] = 0x8000000048ULL;
    packet.header[0] = 0;
    phase = scroll_phase;
    scroll_phase += 0.001f;
    if (scroll_phase > 7.0f)
        scroll_phase -= 7.0f;
    vu1_add_g_sregister(0x47, 0x5301b);
    phase_step = 0.3f;
    phase += phase_step;
    column_step = 0.05f;
    vu1_add_g_sregister(0x4e, (depth_buffer_address >> 13) | 0x1000000 | ((u64)0x8000 << 17));
    for (row = 1; row < 10; row++) {
        row_phase = row_start_phase;
        row_start_phase += row_phase_step_gp;
        if (row_start_phase > 7.0f)
            row_start_phase -= 7.0f;
        for (column = 0; column < 20; column++) {
            qcopy(packet.pos[0], rope_grid[row][column]);
            qcopy(packet.pos[1], rope_grid[row][(column + 1) % 20]);
            qcopy(packet.pos[2], rope_grid[row - 1][column]);
            qcopy(packet.pos[3], rope_grid[row - 1][(column + 1) % 20]);
            end_phase = row_phase + column_step;
            color = rope_row_alpha[row];
            for (k = 0; k < 2; k++) {
                if (color < 0)
                    color = 0;
                if (color > 64)
                    color = 64;
                color = (color << 24) | 0x404040;
                packet.color[k * 2 + 1] = packet.color[k * 2] = color;
                color = rope_row_alpha[row - 1];
            }
            if (row == 1) {
                packet.color[3] = 0x20404040;
                packet.color[2] = 0x20404040;
            } else if (row == 9) {
                packet.color[1] = 0x404040;
                packet.color[0] = 0x404040;
            }
            packet.uv[0] = phase;
            packet.uv[1] = row_phase;
            packet.uv[2] = phase;
            packet.uv[3] = end_phase;
            packet.uv[4] = phase - phase_step;
            packet.uv[5] = row_phase;
            packet.uv[6] = phase - phase_step;
            packet.uv[7] = end_phase;
            row_phase = end_phase;
            draw_geometry_quad(&packet, 0, 0);
        }
        phase += phase_step;
    }
    vu1_add_g_sregister(0x4e, (s64)(depth_buffer_address >> 13) | 0x1000000);
    vu1_add_g_sregister(0x47, 0x5360b);
}

extern int snagglebeast_rope_reset_pending_gp __asm__("D_L07_00161B6C") __attribute__((sda));
extern int snagglebeast_rope_particles_reset_pending_gp __asm__("D_L07_00161B70")
    __attribute__((sda));
extern float row_phase_step_gp __asm__("D_L07_00161B7C") __attribute__((sda));

int D_L07_00161B80 = 0;

extern int D_L07_00161B84;

#include "rnc/overlay/l07/mobys/snagglebeast_globals.h"

extern char    D_L07_00161B90[] MACRO_ADDR;
extern char    D_L07_00161BA0[] MACRO_ADDR;
extern RopeVec rope_previous_points[] __asm__("D_L07_0020DAF0");
extern RopeVec rope_points[] __asm__("D_L07_0020DB00");

/* A particle travelling down the rope: it moves along segment toward the anchor, and between
 * ring columns by blend. */
typedef struct {
    short segment;
    short column;
    float distance;
    float blend;
    float blend_speed;
    float speed;
    char *particle;
} RopeParticle;

extern RopeParticle rope_particles[] __asm__("D_L07_0020C840");

/* Starts the rope when one is pending: lays nine points out from the anchor along the moby's
 * axis, then resets the swing state and, once, clears every entry's link. */
void initialize_snagglebeast_rope(char *moby, void *anchor) __asm__("FUN_L07_00317cb0");

void initialize_snagglebeast_rope(char *moby, void *anchor) {
    RopeVec   step;
    RopeVec   up;
    RopeQuad  rotation_input_vector;
    RopeQuad  rotation_axis;
    float     initial_swing_angle;
    int       i;
    int       j;
    RopeQuad *pc;
    RopeQuad *pd;

    if (snagglebeast_rope_reset_pending_gp == 0) {
        return;
    }
    snagglebeast_rope_reset_pending_gp = 0;
    fast_vec_normalize(step, moby + 0xC0, 2.8f);
    fast_vec_normalize(up, moby + 0xE0, 1.0f);
    qcopy(rope_points, anchor);
    pc = &rotation_input_vector;
    pd = &rotation_axis;
    rope_row_alpha[0] = 0;
    for (i = 1; i < 10; i++) {
        fast_vec_add(rope_points[i], rope_previous_points[i], step);
        rope_row_alpha[i] = 0;
    }
    clear_u64_value(D_L07_00161B90);
    rotation_input_vector.q = 0;
    rotation_axis.q = 0;
    rotation_input_vector.f[2] = 0.1f;
    rotation_input_vector.f[3] = 1.0f;
    rotation_axis.f[0] = 1.0f;
    rotation_axis.f[3] = 1.0f;
    initial_swing_angle = random_angle_radians();
    build_look_at_matrix(D_L07_00161BA0, pc, pd, initial_swing_angle);
    rope_swing_target_angle = initial_swing_angle;
    row_phase_step_gp = 0.006f;
    D_L07_00161B80 = 0;
    D_L07_00161B84 = 0;
    if (snagglebeast_rope_particles_reset_pending_gp != 0) {
        snagglebeast_rope_particles_reset_pending_gp = 0;
        for (j = 199; j >= 0; j--) {
            rope_particles[j].particle = 0;
        }
    }
}

/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_00313D28.c: func_L07_00319230). */
typedef int RopeU128 __attribute__((mode(TI)));

typedef struct {
    char          pad0[4];
    int           mode;
    char          pad8[2];
    unsigned char tint;
    char          fade;
    char          padC[4];
    float         vel[4];
} RopeTail;

typedef struct {
    char     pad0[0xA];
    short    life;
    char     padC[0x14];
    RopeTail tail;
} RopePuff;

extern RopeVec rope_points_lui[] __asm__("D_L07_0020DB00");
extern RopePuff *
spawn_puff(void *, float, float, float, int, void *, float, int) __asm__("FUN_L00_0026d000");
extern RopeVec l07_rope_ring_profile_vectors[] __asm__("D_L07_0020C700");
extern RopeVec l07_rope_previous_ring_rows[][20] __asm__("D_L07_0020DAA0");
extern RopeVec l07_rope_previous_segment_start_points[] __asm__("D_L07_0020DAE0");
extern float   rope_segment_angular_velocities[] __asm__("D_L07_0020DBA0");
extern float   select_segment_point_near_sphere_surface(
      void *, void *, void *, void *, void *, float) __asm__("FUN_L07_0029e6d8");
extern int  random_integer_below(int) __asm__("FUN_00213260");
extern void interpolate_vector(void *, void *, void *, float) __asm__("FUN_L00_0025b6b8");

int D_L07_00161B84_gp __asm__("D_L07_00161B84") = 0;

/* Updates the rope points, its ring mesh and the particles travelling along it. */
float update_snagglebeast_rope(void *moby, void *anchor) {
    RopeVec center;
    RopeVec step;
    RopeVec axis;
    RopeVec start;
    RopeVec end;
    RopeVec up;
    RopeVec delta;
    RopeVec basis;
    struct {
        RopeVec matrix;
        RopeVec cross;
        RopeVec reference;
    } ring_basis;
    RopeVec             velocity;
    RopeVec             extension;
    RopeVec             direction;
    RopeVec             tangent;
    RopeVec             side;
    RopeVec             perpendicular;
    RopeVec             normal;
    RopeVec             offset;
    RopeVec             previous;
    RopeVec             previous_cross;
    extern RopeParticle rope_particles[];
    float               minimum;

    minimum = 1000.0f;
    initialize_snagglebeast_rope(moby, anchor);
    qcopy(center, D_0013F3D0);
    center[2] += 0.6f;
    rope_row_alpha[0] += 4;
    {
        int row;
        for (row = 1; row < 10; row++) {
            rope_row_alpha[row] = rope_row_alpha[row - 1] - 0x20;
        }
    }
    fast_vec_sub(step, D_0013F3D0, anchor);
    step[2] += 0.6f;
    fast_vec_normalize(step, step, 2.8f);
    fast_vec_cross(axis, step, (char *)moby + 0xD0);
    fast_vec_normalize(axis, axis, 1.0f);
    qcopy(rope_points, anchor);
    transform_vector_by_basis(start, D_L07_00161B90, (char *)moby + 0xC0);
    fast_vec_add(start, start, step);
    transform_vector_by_basis(end, D_L07_00161BA0, (char *)moby + 0xC0);
    fast_vec_add(end, end, step);
    {
        float swing_fraction =
            (float)D_L07_00161B80 / convert_integer_to_float(scale_game_frames(60));
        D_L07_00161B80++;
        step[0] = start[0] + (end[0] - start[0]) * swing_fraction;
        step[1] = start[1] + (end[1] - start[1]) * swing_fraction;
        step[2] = start[2] + (end[2] - start[2]) * swing_fraction;
    }
    if (D_L07_00161B80 >= scale_game_frames(60)) {
        qcopy(D_L07_00161B90, D_L07_00161BA0);
        fast_vec_normalize(up, (char *)moby + 0xC0, 2.8f);
        *(RopeU128 *)delta = 0;
        *(RopeU128 *)basis = 0;
        delta[2] = 0.1f;
        delta[3] = 1.0f;
        basis[0] = 1.0f;
        basis[3] = 1.0f;
        {
            float turn = fast_add_rotations(rope_swing_target_angle,
                                            random_float_between(45.0f, 315.0f) * 0.017453292f);
            build_look_at_matrix(D_L07_00161BA0, delta, basis, turn);
            rope_swing_target_angle = turn;
        }
        D_L07_00161B80 = 0;
    }
    (*(float *)&D_L07_00161B84_gp) = fast_add_rotations((*(float *)&D_L07_00161B84_gp), 0.1f);
    fast_vec_normalize(up, (D_0013F3D0 + 0x210), -1.0f);
    qcopy(ring_basis.reference, up);
    fast_vec_cross(ring_basis.cross, step, ring_basis.reference);
    fast_vec_normalize(ring_basis.cross, ring_basis.cross, 1.0f);
    fast_vec_cross(ring_basis.matrix, ring_basis.reference, ring_basis.cross);
    {
        int column;
        for (column = 0; column < 20; column++) {
            fast_vec_scale(delta, l07_rope_ring_profile_vectors[column], 0.02f);
            transform_vector_by_basis(delta, delta, ring_basis.matrix);
            fast_vec_add(rope_grid[0][column], rope_points[0], delta);
        }
    }
    {
        float dot;
        float length;
        float angle;
        float turn;
        float difference;
        float index;
        int   i;
        for (i = 1; i < 10; i++) {
            fast_vec_sub(delta, rope_points[i], rope_previous_points[i]);
            qcopy(basis, step);
            fast_vec_cross(velocity, basis, delta);
            dot = fast_vec_dot(delta, basis);
            length = fast_vec_length(delta);
            angle = 1.5707964f - fast_arc_sin(dot / (length * 2.8f));
            turn = rope_spring_turn(
                &rope_segment_angular_velocities[i], 0.0f, angle, 0.06f, 0.2f, 0.0f);
            difference = fast_subtract_rotations(angle, turn);
            if (absolute_float(difference) > 0.31415927f) {
                turn = fast_add_rotations(turn, fast_subtract_rotations(difference, 0.31415927f));
            }
            build_look_at_matrix(extension, delta, velocity, turn);
            fast_vec_normalize(direction, extension, 2.8f);
            lerp_vector(extension, extension, direction, 1.0f);
            {
                float extension_length = fast_vec_length(extension);
                if (extension_length > 3.9f) {
                    fast_vec_scale(extension, extension, 3.9f / extension_length);
                }
            }
            if (i >= 6) {
                lerp_vector(
                    extension, extension, delta, (float)i / convert_integer_to_float(10) + 0.0f);
            }
            index = i;
            fast_vec_add(rope_points[i], rope_previous_points[i], extension);
            {
                float distance = select_segment_point_near_sphere_surface(
                    tangent, center, center, rope_previous_points[i], rope_points[i], 0.0f);
                if (distance < minimum) {
                    minimum = distance;
                }
            }
            qcopy(tangent, step);
            if (absolute_float(step[2]) > 2.1f) {
                qcopy(normal, up);
                fast_vec_cross(offset, normal, step);
                fast_vec_sub(previous_cross,
                             l07_rope_previous_segment_start_points[i],
                             rope_previous_points[i]);
                fast_vec_cross(previous, normal, previous_cross);
                {
                    float facing = fast_vec_dot(offset, previous);
                    if (facing > 0.0f) {
                        fast_vec_scale(normal, normal, -1.0f);
                    }
                }
            } else {
                fast_vec_normalize(normal, (D_0013F3D0 + 0x210), -1.0f);
            }
            fast_vec_normalize(side, tangent, 1.0f);
            fast_vec_cross(perpendicular, side, normal);
            fast_vec_normalize(perpendicular, perpendicular, 1.0f);
            fast_vec_cross(normal, perpendicular, side);
            qcopy(up, normal);
            {
                int   column;
                float radius;
                for (column = 0; column < 20; column++) {
                    radius = index * 0.2f + 0.02f;
                    radius += radius * 0.25f *
                              fast_sin(fast_add_rotations(
                                  (*(float *)&D_L07_00161B84_gp) + index * 0.7f, 0.0f));
                    fast_vec_scale(offset, l07_rope_ring_profile_vectors[column], radius);
                    transform_vector_by_basis(offset, offset, side);
                    fast_vec_add(rope_grid[i][column], rope_points[i], offset);
                }
            }
            fast_vec_normalize(step, extension, 2.8f);
        }
    }
    add_draw_callback(draw_rope_strips, moby);
    {
        int       created;
        int       extras;
        int       segment;
        float     fraction;
        float     length;
        float     blend;
        float     lifetime;
        int       random_column;
        int       column;
        RopePuff *puff;
        RopePuff *particle;
        RopeVec  *points;
        int       k;
        short     life;
        life = 200;
        created = 0;
        extras = 0;
        for (k = 0; k < 200; k++) {
            if (rope_particles[k].particle == 0 && created < 5) {
                for (segment = 9; segment >= 0; segment--) {
                    if (rope_row_alpha[segment] >= 0x40) {
                        break;
                    }
                }
                if (segment > 0) {
                    if (segment >= 2 && extras < 2) {
                        random_column = random_integer_below(20);
                        blend = random_float_between(0.2f, 1.0f);
                        lerp_vector(delta,
                                    rope_grid[0][random_column],
                                    rope_grid[segment][random_column],
                                    blend);
                        /* Fakematch: a second label and a dead store keep this address out of the
                     * loop-hoisted constant, so it is built in a1 next to the call. */
                        points = rope_points_lui;
                        lerp_vector(velocity, points[0], points[segment], blend);
                        points = 0;
                        lifetime = random_float_between(60000.0f, 180000.0f);
                        puff = spawn_puff(delta,
                                          0.0f,
                                          1.0f,
                                          1.03f,
                                          random_integer_below(5) + 1,
                                          basis,
                                          lifetime,
                                          0x404040);
                        if (puff != 0) {
                            RopeTail *tail = &puff->tail;
                            fast_vec_sub(extension, delta, velocity);
                            extras++;
                            fast_vec_normalize(tail->vel, extension, 0.01f);
                            tail->vel[2] += 0.02f;
                        }
                    }
                    qcopy(delta, rope_points[segment]);
                    clear_u64_value(basis);
                    particle = spawn_puff(delta,
                                          0.0f,
                                          1.0f,
                                          1.0f,
                                          6,
                                          basis,
                                          random_float_between(60000.0f, 180000.0f),
                                          0x7F404040);
                    rope_particles[k].particle = (char *)particle;
                    if (particle != 0) {
                        RopeTail *tail;
                        particle->life = life;
                        tail = &particle->tail;
                        tail->mode = 2;
                        created++;
                        tail->tint = 0x7F;
                        tail->fade = life;
                    }
                    rope_particles[k].segment = segment;
                    rope_particles[k].distance = 0.0f;
                    rope_particles[k].blend = 0.0f;
                    rope_particles[k].column = random_integer_below(20);
                    rope_particles[k].blend_speed = random_float_between(0.1f, 0.8f);
                    rope_particles[k].speed = random_float_between(0.1f, 0.4f);
                }
            }
            if (rope_particles[k].particle != 0) {
                fast_vec_sub(delta,
                             rope_previous_points[rope_particles[k].segment],
                             rope_points[rope_particles[k].segment]);
                length = fast_vec_length(delta);
                rope_particles[k].distance += rope_particles[k].speed;
                *(short *)(rope_particles[k].particle + 0xA) = life;
                if (length < rope_particles[k].distance) {
                    if (--rope_particles[k].segment <= 0) {
                        free_effect_particle(rope_particles[k].particle);
                        rope_particles[k].particle = 0;
                    } else {
                        rope_particles[k].distance -= length;
                        fast_vec_sub(delta,
                                     rope_previous_points[rope_particles[k].segment],
                                     rope_points[rope_particles[k].segment]);
                        length = fast_vec_length(delta);
                    }
                }
                if (rope_particles[k].particle != 0) {
                    fraction = rope_particles[k].distance / length;
                    if (rope_particles[k].segment == 2) {
                        *(short *)(rope_particles[k].particle + 0xA) = 100;
                    } else if (rope_particles[k].segment < 2) {
                        *(short *)(rope_particles[k].particle + 0xA) =
                            truncate_float_to_s32((1.0f - fraction) * 100.0f);
                        if (*(short *)(rope_particles[k].particle + 0xA) <= 60) {
                            *(short *)(rope_particles[k].particle + 0xA) = 60;
                        }
                    }
                    if (rope_particles[k].segment == 1) {
                        fast_vec_sub(basis, anchor, rope_particles[k].particle + 0x10);
                        fast_vec_normalize(basis, basis, rope_particles[k].speed);
                        fast_vec_add(rope_particles[k].particle + 0x10,
                                     rope_particles[k].particle + 0x10,
                                     basis);
                        *(float *)(rope_particles[k].particle + 0xC) *=
                            frame_scale * -0.050000012f + 1.0f;
                    } else {
                        fast_vec_scale(delta, delta, fraction);
                        fast_vec_add(rope_particles[k].particle + 0x10,
                                     rope_points[rope_particles[k].segment],
                                     delta);
                        column = rope_particles[k].column;
                        fast_vec_sub(basis,
                                     rope_grid[rope_particles[k].segment][column],
                                     rope_points[rope_particles[k].segment]);
                        fast_vec_sub(velocity,
                                     l07_rope_previous_ring_rows[rope_particles[k].segment][column],
                                     rope_previous_points[rope_particles[k].segment]);
                        interpolate_vector(extension, basis, velocity, fraction);
                        column = (column + 19) % 20;
                        fast_vec_sub(direction,
                                     rope_grid[rope_particles[k].segment][column],
                                     rope_points[rope_particles[k].segment]);
                        fast_vec_sub(tangent,
                                     l07_rope_previous_ring_rows[rope_particles[k].segment][column],
                                     rope_previous_points[rope_particles[k].segment]);
                        interpolate_vector(side, direction, tangent, fraction);
                        interpolate_vector(extension, extension, side, rope_particles[k].blend);
                        rope_particles[k].blend += rope_particles[k].blend_speed;
                        if (rope_particles[k].blend >= 1.0f) {
                            rope_particles[k].blend -= 1.0f;
                            rope_particles[k].column = column;
                        }
                        fast_vec_add(rope_particles[k].particle + 0x10,
                                     rope_particles[k].particle + 0x10,
                                     extension);
                    }
                }
            }
        }
    }
    return minimum;
}

/* Pulls each pending moby toward its target point and smooths its velocity. */
/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_00313D28.c: func_L07_0031A268), where it is exact; names translated to the US level program. */
extern void suck_beam_queue_draw(int draw_callback_context) __asm__("FUN_L00_002defe0");

void detach_snagglebeast_rope_particles(int target_entry) {
    float         segment_offset[4];
    float         interpolated_segment_position[4];
    float         particle_offset[4];
    RopeParticle *e;
    char         *particle;
    float         segment_length;
    int           off;
    int           i;
    char         *rope_particle_array_start;

    snagglebeast_rope_reset_pending_gp = 1;
    snagglebeast_rope_particles_reset_pending_gp = 1;
    row_phase_step_gp = row_phase_step_gp * 0.5f;
    allocate_voice_for_target_entry(10, 0, target_entry);
    rope_particle_array_start = (char *)rope_particles;
    for (i = 0; i < 200; i++) {
        particle = rope_particles[i].particle;
        if (particle != 0) {
            rope_particles[i].particle = 0;
            fast_vec_sub(segment_offset,
                         (char *)rope_previous_points + rope_particles[i].segment * 16,
                         (char *)rope_points + rope_particles[i].segment * 16);
            segment_length = fast_vec_length(segment_offset);
            fast_vec_scale(
                segment_offset, segment_offset, rope_particles[i].distance / segment_length);
            fast_vec_add(interpolated_segment_position,
                         (char *)rope_points + rope_particles[i].segment * 16,
                         segment_offset);
            fast_vec_sub(particle_offset, particle + 0x10, interpolated_segment_position);
            fast_vec_normalize(particle_offset, particle_offset, 0.03f);
            fast_vec_scale(segment_offset, segment_offset, 0.1f / segment_length);
            fast_vec_add(particle + 0x30, particle_offset, segment_offset);
            *(short *)(particle + 0xA) = 0x46;
        }
    }
    suck_beam_queue_draw(target_entry);
}

extern f32              D_0015ED6C;
extern void             spawn_explosion(void *,
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
extern void             jet_animate(struct Moby *, f32) __asm__("FUN_L00_00259fe8");
extern void             spawn_explosion(void *,
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
extern float D_0015ED6C MACRO_ADDR;

float rope_swing_target_angle = 0;
