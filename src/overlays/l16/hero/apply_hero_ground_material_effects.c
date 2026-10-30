/* Level 16 hero movement: ground material effects, velocity composition and limits, ground contact, position collisions and motion integration. */
#include "types.h"
#include "rnc/hero/hero_state.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/overlay/hero.h"
#include "rnc/overlay/collision.h"
#include "rnc/data_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/mobys/get_moby_target_info.h"
#include "rnc/overlay/shared/hero/set_hero_animation.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/world/collision/collision_hit_material.h"
#include "rnc/mobys/moby.h"

/* Per-frame update of the pickup/hit state flags from the state code. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/help_00209D98.c: func_L16_00209D98), where it is exact; names translated to the US level program. */

extern int  coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern void exit_hero_vehicle_mode_if_mode_three(void) __asm__("FUN_L00_0020b930");
extern int  hero_set_state(int, int) __asm__("FUN_L16_0021e398");

void apply_hero_ground_material_effects(void) __asm__("FUN_L16_002097a0");

void apply_hero_ground_material_effects(void) {
    struct Hero *hero_data = &hero;
    int          keep = hero_data->ground_sound_class;
    int          saved_ground_type = hero_data->ground_type;
    int          level_index;
    fast_mem_zero16(&hero_data->ground_type, 0x10);
    hero_data->ground_sound_class = keep;
    hero_data->ground_type = -1;
    hero_data->wading = 0;
    hero_data->unk308 = 0;
    if (saved_ground_type == -1)
        return;
    if (saved_ground_type == 2) {
        if (hero_data->frames_off_ground.s == 0 || hero_data->ground_distance < 0.3f) {
            hero_data->on_magnet_ground = 1;
            if (hero_data->vehicle_mode == 0) {
                char *item_moby = (char *)hero_data->items[1].moby;
                if (item_moby != 0 && MOBY(item_moby)->oclass == 0xAD)
                    hero_data->unk308 = 1;
            }
        }
    }
    if (saved_ground_type == 0xE) {
        struct Hero *q = &hero;
        q->base_condition = 1;
        q->water_depth = 0.2f;
        q->water_surface_z = q->ground_z.f + 0.2f;
    }
    if (saved_ground_type == 0) {
        struct Hero *q = &hero;
        float        water_depth = q->water_surface_z - q->ground_z.f;
        q->water_depth = water_depth;
        if (water_depth < 0.85f && 0.25f < water_depth)
            q->wading = 1;
        {
            struct Hero *flag = &hero;
            flag->base_condition = 1;
        }
        level_index = current_level_index;
    } else {
        level_index = current_level_index;
    }
    if (level_index == 0xD) {
        struct Hero *q = &hero;
        if (q->state.current != 0x7B) {
            if (coll_sphere(q->capsule_radius + 0.03f, &q->motion.body_center, 2, 0)) {
                if (collision_hit_material() == 0xB) {
                    hero_set_state(0x7B, 1);
                    return;
                }
            }
        }
    }
    if (saved_ground_type == 0xB) {
        struct Hero *q = &hero;
        q->selector_11 = 1;
    }
    if (saved_ground_type == 0x5) {
        struct Hero *q = &hero;
        q->selector_5 = 1;
    }
    if (saved_ground_type == 0x6) {
        struct Hero *q = &hero;
        q->unk12E9 = 1;
    }
    if (saved_ground_type == 0x2) {
        struct Hero *q = &hero;
        q->on_magnet_ground = 1;
    }
    if (saved_ground_type == 0x4) {
        struct Hero *q = &hero;
        q->selector_4 = 1;
    }
    if (saved_ground_type == 0xD) {
        struct Hero *q = &hero;
        q->selector_13 = 1;
    }
    if (saved_ground_type == 0x8) {
        struct Hero *q = &hero;
        q->on_slip_ground = 1;
    }
    if (saved_ground_type == 0x9) {
        struct Hero *q = &hero;
        q->unk12EE = 1;
    }
    if (saved_ground_type == 0xC) {
        struct Hero *q = &hero;
        q->on_slip_ground = 1;
    }
    {
        struct Hero *q = &hero;
        struct Hero *hero_ptr;
        if (q->selector_4 != 0 && q->grounded_frames != 0) {
            int k = q->state.control_mode;
            if (k != 0x10 && k != 0x14 && k != 7) {
                hero_set_state(0x31, 1);
                return;
            }
        }
        hero_ptr = &hero;
        if (hero_ptr->selector_13 != 0 && hero_ptr->state.current != 0x7F) {
            if (absolute_float(hero_ptr->water_surface_z - (hero_ptr->motion.pos.f[2] + 0.25f)) <
                1.0f) {
                if (hero_ptr->water_surface_z - hero_ptr->motion.pos.f[2] > 0.0f) {
                    if (hero_ptr->motion.displacement.f[2] < 0.0f) {
                        exit_hero_vehicle_mode_if_mode_three();
                        hero_set_state(HERO_STATE_SINK, 1);
                    }
                }
            }
        }
    }
}

#include "qcopy.h"
#include "rnc/overlay/quad.h"

/* Ported from rac1-decomp (src/overlays/l16_kalebo3/help_00209D98.c: func_L16_0020FEC8), where it is exact; names translated to the US level program. */

extern void FUN_L00_0025f730(void *a, float x);

void compose_and_limit_hero_velocity(void) __asm__("FUN_L16_0020f8b0");

void compose_and_limit_hero_velocity(void) {
    char  normalized_vector[16];
    char *accumulated_vector = D_0013E533 + 0x158D;
    char *normalization_source;
    qcopy(accumulated_vector, accumulated_vector - 0x670);
    normalization_source = accumulated_vector - 0x770;
    fast_vec_normalize(
        normalized_vector, normalization_source, *(float *)(normalization_source + 0x194));
    fast_vec_add(accumulated_vector, accumulated_vector, normalized_vector);
    qcopy(accumulated_vector - 0x690, accumulated_vector);
    FUN_L00_0025f730(accumulated_vector - 0x690,
                     *(float *)(normalization_source + 0x85C) +
                         *(float *)(normalization_source + 0x868));
}

typedef union {
    OvlQuad q;
    f32     f[4];
} V_u;

extern u8 D_L16_00174240[] NOT_SDA;
extern u8 D_L16_00174260[] NOT_SDA;

void hero_get_collision_ray_endpoints(void *, void *, s32, f32, f32) __asm__("FUN_L00_002126b8");
s32  collision_line(void *, void *, s32, s32, s32) __asm__("FUN_001efa68");
s32  coll_sound_class(void) __asm__("FUN_L00_001f3608");
f32  resolve_camera_surface_height(void *, void *) __asm__("FUN_002135f0");
void sceVu0Normalize(void *, void *) __asm__("FUN_00125180");
f32  compute_gravity_relative_vector_inclination(void *) __asm__("FUN_L00_00233d30");
void compute_rotated_vector_angles(void *, void *) __asm__("FUN_L00_00216078");
void transform_hero_polar_offset_to_world(void *, f32, f32, f32) __asm__("FUN_L00_002335c8");
void FUN_L00_002334d0(void *, void *, f32);

extern CollisionHit HH __asm__("D_L16_00174240") NOT_SDA;
#define HI(o) (*(s32 *)(D_L16_00174240 + (o)))
#define HF(o) (*(f32 *)(D_L16_00174240 + (o)))

void update_hero_ground_contact(void) __asm__("FUN_L16_0020ffb0");

void update_hero_ground_contact(void) {
    V_u collision_ray_endpoint, ground_probe_endpoint, ground_offset_or_angles, side_probe_endpoint,
        unused_side_probe_endpoint, probe_surface_angles, normalized_hit_normal;
    f32 f;
    s32 collision_sound_class;
    s32 i;
    f32 side_probe_angle;
    u8 *hit_moby_record;

    s32 k;
    f32 ground_probe_start_offset;

    hero.ground_normal.f[2] = 1.0f;
    hero.air_frames.u++;
    hero.frames_off_ground.u++;
    hero.ground_normal.i[0] = 0;
    hero.ground_normal.i[1] = 0;
    hero.ground_slope.i = 0;
    hero.ground_z.i = 0;
    clear_u64_value((u8 *)&hero.ground_point);
    hero.on_flagged_ground_moby = 0;
    hero.ground_distance = 32.0f;
    hero.ground_moby = 0;
    if (hero.in_water_frames != 0) {
        hero.in_water_frames++;
    }
    k = 0;
    if (hero.gravity_mode)
        k = 1;
    ground_probe_start_offset = hero.capsule_radius * 2.2f;
    if (hero.state.control_mode == 2) {
        ground_probe_start_offset = hero.capsule_radius * 2.9f;
    }
    hero_get_collision_ray_endpoints(
        &ground_probe_endpoint, &collision_ray_endpoint, k, ground_probe_start_offset, -48.0f);
    if (collision_line(&ground_probe_endpoint, &collision_ray_endpoint, 2, (s32)hero.moby, 0) ==
        0) {
        goto tail;
    }
    collision_sound_class = coll_sound_class();
    if (collision_sound_class == -1) {
        collision_sound_class = 3;
    }
    hero.ground_sound_class = collision_sound_class;
    hero.ground_type = collision_hit_material();
    if (hero.ground_type == 0) {
        hero.water_surface_z =
            resolve_camera_surface_height(D_L16_00174260, (u8 *)&hero.water_surface_normal);
        if (hero.motion.pos.f[2] < hero.water_surface_z && hero.in_water_frames <= 0) {
            hero.in_water_frames = 1;
        }
        if (collision_line(
                &ground_probe_endpoint, &collision_ray_endpoint, 0x24, (s32)hero.moby, 0) == 0) {
            goto d1;
        }
        collision_sound_class = coll_sound_class();
        if (current_level_index == 1 || current_level_index == 0x12) {
            collision_sound_class = 3;
        }
        hero.ground_sound_class = collision_sound_class;
    }
    if (hero.ground_type == 0xD) {
        hero.water_surface_z = HH.point.f[2];
        ground_probe_endpoint.f[2] = HH.point.f[2] - 0.01f;
        if (collision_line(&ground_probe_endpoint, &collision_ray_endpoint, 4, (s32)hero.moby, 0) ==
            0) {
            goto d1;
        }
    }
    if (hero.ground_type == 0xB) {
        hero.sink_surface_z = HH.point.f[2];
        goto d1;
    }
    if (HH.moby != 0) {
        if (is_moby_in_class_range_500_to_540(HH.moby)) {
            hero.on_flagged_ground_moby = 1;
            if ((u32)(hero.state.control_mode - 0x15) < 2) {
                FUN_L00_002334d0((u8 *)&HH.point, (u8 *)&HH.point, 1.0f);
            }
        }
        hit_moby_record = get_moby_target_info(HH.moby);
        if (hit_moby_record != 0 && (*(u16 *)(hit_moby_record + 0x1E) & 8)) {
            hero.on_flagged_ground_moby = 1;
        }
    }
    if (HH.unk1C > 0) {
        qcopy((u8 *)&hero.ground_point, &HH.point);
        hero.ground_z.f = HH.point.f[2];
        hero.ground_distance = fast_vec_dist((u8 *)&hero.motion.pos, (u8 *)&hero.ground_point);
        hero.ground_moby = (struct Moby *)HH.moby;
        sceVu0Normalize((u8 *)&hero.ground_normal, (u8 *)&HH.normal_x);
        hero.ground_slope.f = compute_gravity_relative_vector_inclination((u8 *)&HH.normal_x);
        hero.downhill_yaw = fast_arc_tan(HH.normal_x, HH.normal_y);
        fast_vec_sub(&ground_offset_or_angles, (u8 *)&hero.ground_point, (u8 *)&hero.motion.pos);
        if (0.0f < fast_vec_dot(&ground_offset_or_angles, (u8 *)&hero.ground_normal)) {
            hero.ground_distance = -hero.ground_distance;
        }
        if (hero.ground_distance < 0.02f) {
            hero.frames_off_ground.s = 0;
            if (hero.ground_slope.f <= 0.87266463f || hero.gravity_mode == 1 ||
                hero.state.control_mode == 0x16) {
                hero.air_frames.s = 0;
                hero.last_landed_moby = HH.moby;
            }
        }
    }
tail:
    if (hero.air_frames.s != 0) {
        hero.grounded_frames = 0;
    } else {
        hero.grounded_frames++;
        qcopy((u8 *)&hero.last_ground_point, (u8 *)&hero.ground_point);
    }
    if (hero.motion.pos.f[2] > hero.water_surface_z) {
        hero.in_water_frames = 0;
    }
d1:
    clear_u64_value((u8 *)&hero.gravity_dir);
    switch (hero.gravity_mode) {
    case HERO_GRAVITY_WORLD_DOWN:
        hero.gravity_dir.f[2] = -1.0f;
        break;
    case HERO_GRAVITY_OPPOSITE_GROUND_NORMAL:
        qcopy((u8 *)&hero.gravity_dir, (u8 *)&hero.ground_normal);
        fast_vec_scale((u8 *)&hero.gravity_dir, (u8 *)&hero.gravity_dir, -1.0f);
        break;
    case HERO_GRAVITY_TOWARD_REFERENCE_POINT:
        fast_vec_sub(
            (u8 *)&hero.gravity_dir, (u8 *)&hero.motion.gravity_point, (u8 *)&hero.motion.pos);
        fast_vec_normalize((u8 *)&hero.gravity_dir, (u8 *)&hero.gravity_dir, 1.0f);
        break;
    }
    hero.ground_pitch.i = 0;
    hero.unk2E8.i = 0;
    hero.unk2D0.i[0] = 0;
    hero.unk2D0.i[1] = 0;
    hero.unk2C8.i[0] = 0;
    hero.unk2C8.i[1] = 0;
    hero.unk2C0.i[0] = 0;
    hero.unk2C0.i[1] = 0;
    if (hero.state.control_mode == 0xF || hero.state.control_mode == 0x15 ||
        hero.state.control_mode == 6 || hero.state.control_mode == 4 ||
        hero.state.control_mode == 5 || hero.state.control_mode == 3) {
        return;
    }
    if (!(hero.ground_distance < 0.25f)) {
        return;
    }
    compute_rotated_vector_angles((u8 *)&hero.ground_normal, &ground_offset_or_angles);
    if (-0.78539819f < ground_offset_or_angles.f[1] && ground_offset_or_angles.f[1] < 0.78539819f) {
        hero.ground_pitch.f = ground_offset_or_angles.f[1];
    }
    if (-0.78539819f < ground_offset_or_angles.f[0] && ground_offset_or_angles.f[0] < 0.78539819f) {
        hero.unk2E8.f = ground_offset_or_angles.f[0];
    }
    for (i = 0; i < 2; i++) {
        side_probe_angle = 1.5707964f;
        if (i == 1) {
            side_probe_angle = -side_probe_angle;
        }
        transform_hero_polar_offset_to_world(&side_probe_endpoint, 0.2f, side_probe_angle, 0.45f);
        transform_hero_polar_offset_to_world(
            &unused_side_probe_endpoint, 0.2f, side_probe_angle, -0.45f);
        if (collision_line(
                &side_probe_endpoint, &collision_ray_endpoint, 0x22, (s32)hero.moby, 0)) {
            f = HH.point.f[2] - hero.ground_z.f;
            hero.unk2C0.f[i] = f;
            if (0.4f < absolute_float(f)) {
                hero.unk2C0.f[i] = 0.0f;
            }
            sceVu0Normalize(&normalized_hit_normal, (u8 *)&HH.normal_x);
            compute_rotated_vector_angles(&normalized_hit_normal, &probe_surface_angles);
            if (-0.78539819f < probe_surface_angles.f[1] &&
                probe_surface_angles.f[1] < 0.78539819f) {
                hero.unk2C8.f[i] = probe_surface_angles.f[1];
            }
            if (-0.78539819f < probe_surface_angles.f[0] &&
                probe_surface_angles.f[0] < 0.78539819f) {
                hero.unk2D0.f[i] = probe_surface_angles.f[0];
            }
        } else {
            hero.unk2C0.i[i] = 0;
            hero.unk2C8.f[i] = hero.ground_pitch.f;
            hero.unk2D0.f[i] = hero.unk2E8.f;
        }
    }
}
/* Adapted from the exact FUN_L00_002133a8 (src/overlays/shared/hero/hero_check_damage.c): L16 adds the mode-0x16 v-offset branch, the 0x15/0x16 range test and a 0x5f collision arm, and its own level data. */

typedef unsigned int u128_2133a8 __attribute__((mode(TI), aligned(16)));

typedef union {
    u128_2133a8 q;
    f32         f[4];
} V_2133a8;

extern void FUN_L00_002347c0(void);
extern void transform_hero_local_offset_to_world(void *, f32, f32, f32) __asm__("FUN_L00_00233660");
extern void FUN_L00_00233810(void *, void *, f32);
extern void FUN_L00_002334d0(void *, void *, f32);
extern s32  coll1_2133a8(void *, s32, s32, f32) __asm__("FUN_L00_001f0d60");
extern s32  rig_probe(void *, s32, f32, s32, f32) __asm__("FUN_L00_001f19a0");
extern s32  FUN_L00_001f3170(void *, f32);
extern u128_2133a8 D_L16_00174270[] NOT_SDA;
s32                resolve_hero_position_collisions(s32 mode) __asm__("FUN_L16_00210b60");

s32 resolve_hero_position_collisions(s32 mode) {
    V_2133a8     pre_collision_position;
    V_2133a8     collision_position_offset;
    V_2133a8     resolved_position_delta;
    s32          mask;
    s32          i;
    u128_2133a8 *collision_output;
    f32          collision_extent_or_displacement_length;
    if (hero.collision_off_frames != 0) {
        return 1;
    }
    qcopy(&pre_collision_position, &hero.motion.pos.q);
    clear_u64_value(&collision_position_offset);
    if (hero.path_constrain_pending != 0) {
        FUN_L00_002347c0();
        hero.path_constrain_pending = 0;
    }
    if (hero.state.control_mode != 0x11) {
        if (hero.state.control_mode == 0x16) {
            transform_hero_local_offset_to_world(&collision_position_offset, 0.0f, 0.0f, 0.7f);
            fast_vec_sub(
                &collision_position_offset, &collision_position_offset, &hero.motion.pos.q);
        } else if (hero.gravity_mode != 0 || hero.unk1F8 != 0) {
            if (hero.gravity_mode == 1 || hero.unk1F8 != 0 ||
                (unsigned)(hero.state.control_mode - 0x15) < 2) {
                FUN_L00_00233810(&collision_position_offset, &collision_position_offset, 0.6f);
            } else {
                FUN_L00_002334d0(
                    &collision_position_offset, &collision_position_offset, -hero.capsule_bottom);
            }
        } else {
            collision_position_offset.f[2] = hero.capsule_bottom;
        }
    }
    fast_vec_add(&hero.motion.pos.q, &hero.motion.pos.q, &collision_position_offset);
    mask = 0x24;
    if (hero.state.current == HERO_STATE_SINK) {
        mask = 0xD24;
    }
    for (i = 0; i < 8; i++) {
        if (hero.gravity_mode != 0) {
            if (!coll1_2133a8(&hero.motion.pos.q, mask, ((s32)hero.moby), frame_scale * 0.4f))
                break;
        } else if (hero.state.control_mode == 0x11) {
            if (!coll1_2133a8(&hero.motion.pos.q, mask, ((s32)hero.moby), 0.6f))
                break;
        } else if (hero.state.control_mode == 0x16) {
            if (!coll1_2133a8(&hero.motion.pos.q, mask, ((s32)hero.moby), frame_scale * 0.5f))
                break;
        } else if (hero.state.control_mode == 0xF) {
            if (!coll1_2133a8(&hero.motion.pos.q, mask, ((s32)hero.moby), frame_scale * 0.45f))
                break;
        } else {
            s32 collision_result;
            collision_extent_or_displacement_length = hero.capsule_top - hero.capsule_bottom;
            if (collision_extent_or_displacement_length < 0.05f) {
                collision_extent_or_displacement_length = 0.05f;
            }
            collision_result = rig_probe(&hero.motion.pos.q,
                                         mask,
                                         hero.capsule_radius,
                                         ((s32)hero.moby),
                                         collision_extent_or_displacement_length);
            if (!(collision_result | FUN_L00_001f3170(&hero.motion.pos.q, hero.capsule_radius)))
                break;
        }
        collision_output = D_L16_00174270;
        qcopy(&hero.motion.pos.q, collision_output);
        qcopy(&hero.wall_contact_normal.q, collision_output + 1);
        qcopy(&hero.wall_contact_point.q, collision_output - 1);
        hero.wall_hit = 1;
        hero.coll_hit_moby = ((u8 **)collision_output)[-6];
    }
    fast_vec_sub(&hero.motion.pos.q, &hero.motion.pos.q, &collision_position_offset);
    fast_vec_sub(&resolved_position_delta, &hero.motion.pos.q, &pre_collision_position);
    collision_extent_or_displacement_length = fast_vec_length(&resolved_position_delta);
    if (hero.capsule_radius * 1.5f < collision_extent_or_displacement_length) {
        if (mode == 0xF) {
            if (512.0f < resolved_position_delta.f[0])
                resolved_position_delta.f[0] = 512.0f;
            else if (resolved_position_delta.f[0] < -512.0f)
                resolved_position_delta.f[0] = -512.0f;
            if (512.0f < resolved_position_delta.f[1])
                resolved_position_delta.f[1] = 512.0f;
            else if (resolved_position_delta.f[1] < -512.0f)
                resolved_position_delta.f[1] = -512.0f;
            if (512.0f < resolved_position_delta.f[2])
                resolved_position_delta.f[2] = 512.0f;
            else if (resolved_position_delta.f[2] < -512.0f)
                resolved_position_delta.f[2] = -512.0f;
            fast_vec_normalize(
                &resolved_position_delta, &resolved_position_delta, hero.capsule_radius);
            fast_vec_add(&hero.motion.pos.q, &pre_collision_position, &resolved_position_delta);
        }
        return -1;
    }
    return 1;
}

/* Integrate player motion, resolve contacts, and measure the resulting movement. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/help_00209D98.c: func_L16_002116B0), where it is exact; names translated to the US level program. */

typedef union {
    OvlQuad q;
    f32     f[4];
} V;

typedef struct {
    char          pad00[0x80];
    float         position[4];
    float         rotation[4];
    char          padA0[0x40];
    float         velocity[4];
    float         contact[4];
    float         external[4];
    float         delta[4];
    float         vertical_delta[4];
    float         horizontal_delta[4];
    float         frame_delta[4];
    char          pad150[0x10];
    float         speed;
    float         horizontal_speed;
    float         vertical_speed;
    float         slope;
    char          pad170[0x5C];
    int           unconstrained;
    char          pad1D0[0x64];
    float         speed_limit;
    char          pad238[4];
    int           contact_state;
    char          pad240[0x17];
    unsigned char contact_flag;
    char          pad258[0x84];
    float         gravity_limit;
    char          pad2E0[0x218];
    int           physics_mode;
    char          pad4FC[0x424];
    float         extra_step[4];
    char          pad930[0x1754];
    int           state;
    char          pad2088[4];
    int           mode;
} L16MovementPlayer;

extern char *D_L16_00174258[];
extern float forward_component(void *) __asm__("FUN_L00_00213350");
extern float gravity_length_xy(float *v) __asm__("FUN_L00_002339d0");
extern void  correct_hero_ground_penetration(void) __asm__("FUN_L00_00212ff0");
extern void  resolve_hero_collisions_with_history(void) __asm__("FUN_L00_002137a8");
extern void  gravity_flatten(float *, float *) __asm__("FUN_L00_002338d0");
extern void
set_gravity_relative_vector_length_xy(float *, float *, float) __asm__("FUN_L00_00233ba0");
extern void update_hero_attachment(int, void *, void *) __asm__("FUN_L00_00233f80");
extern char D_0013F430[] MACRO_ADDR;

void integrate_hero_motion(void) __asm__("FUN_L16_00211098");

void integrate_hero_motion(void) {
    V                  previous;
    V                  motion_reference_vector;
    V                  plane;
    L16MovementPlayer *contact_player;
    L16MovementPlayer *motion_player;
    L16MovementPlayer *cap_player;
    float             *position = (float *)(&hero.motion.pos);
    L16MovementPlayer *player;
    float             *delta;
    float             *vertical;
    float             *horizontal;
    float             *extra_step;
    float             *velocity;
    float             *new_position;
    float             *contact;
    float             *external;
    float              magnitude;

    qcopy(previous.f, position);
    player = (L16MovementPlayer *)((char *)position - 0x80);
    update_hero_attachment(player->physics_mode, (char *)position + 0x70, (char *)position + 0x10);
    /* Limit the velocity by state and mode. Inside an arm `position` moves on to the velocity; it is formed again below. */
    if (player->state == 34 || player->state == 20) {
        float  limit;
        float *limited_velocity;

        if (!player->unconstrained) {
            float length;

            position = (float *)((char *)position + 0x60);
            length = gravity_length_xy(position);
            limit = player->speed_limit - 0.02f;
            if (limit < length)
                fast_vec_normalize(position, position, limit);
        }
        limited_velocity = (float *)(&hero.motion.velocity);
        magnitude = gravity_z(limited_velocity);
        limit = -*(float *)((char *)limited_velocity + 0x1FC); /* the player's gravity_limit */
        if (magnitude < limit) {
            if (limit > 0.0f)
                limit = 0.0f;
            gravity_set_z(limited_velocity, limited_velocity, limit);
        }
    } else if (player->mode == 15) {
        /* The same vertical clamp as in mode 22 below, written out in both places: the compiler merges the two. */
        if (!player->unconstrained) {
            float length;
            float limit;

            position = (float *)(((char *)&D_0013F430));
            length = vector_length_xy(position);
            limit = player->speed_limit - 0.02f;
            if (limit < length)
                set_vector_length_xy(position, position, limit);
        }
    } else if (player->mode == 21) {
        float limit = 0.6f;

        if (!player->unconstrained) {
            float *limited_velocity = (float *)((char *)position + 0x60);

            if (fast_vec_length(limited_velocity) > limit)
                fast_vec_normalize(limited_velocity, limited_velocity, limit);
        }
    } else if (player->mode == 22) {
        qcopy(motion_reference_vector.f, position);
        motion_reference_vector.f[2] += 0.5f;
        fast_vec_add(plane.f, motion_reference_vector.f, (char *)position + 0x60);
        if (collision_line(position, plane.f, 2, 0, 0)) {
            char *hit = D_L16_00174258[0];

            if (!hit || *(short *)(hit + 0xA6) == 0x1F6 || *(short *)(hit + 0xA6) == 0x59F) {
                player = (L16MovementPlayer *)(((char *)((char *)&hero)));
                if (!player->unconstrained) {
                    float length;
                    float limit;

                    position = (float *)(((char *)&D_0013F430));
                    length = vector_length_xy(position);
                    limit = player->speed_limit - 0.02f;
                    if (limit < length)
                        set_vector_length_xy(position, position, limit);
                }
            }
        }
    } else if (player->mode != 13) {
        if (player->mode != 14) {
            if (!player->unconstrained) {
                float length;
                float limit;

                position = (float *)((char *)position + 0x60);
                length = fast_vec_length(position);
                limit = player->speed_limit - 0.02f;
                if (limit < length)
                    fast_vec_normalize(position, position, limit);
            }
        }
    }
    position = (float *)(&hero.motion.pos);
    fast_vec_add(position, position, (char *)position + 0x60);
    extra_step = (float *)((char *)position + 0x8A0);
    fast_vec_add(position, position, extra_step);
    clear_u64_value(extra_step);
    contact_player = (L16MovementPlayer *)((char *)position - 0x80);
    contact_player->contact_flag = 0;
    contact_player->contact_state = 0;
    if (fast_vec_length((char *)position + 0x70) <= 0.0001f) {
        resolve_hero_collisions_with_history();
        update_hero_ground_contact();
        fast_vec_sub((char *)position + 0x80, position, previous.f);
        correct_hero_ground_penetration();
    } else {
        update_hero_ground_contact();
    }
    delta = (float *)(&hero.motion.resolved_velocity);
    new_position = (float *)((char *)delta - 0x90);
    horizontal = (float *)((char *)delta + 0x20);
    fast_vec_sub(delta, new_position, previous.f);
    qcopy(horizontal, delta);
    vertical = (float *)((char *)delta + 0x10);
    qcopy(vertical, delta);
    velocity = (float *)((char *)delta - 0x30);
    fast_vec_normalize(delta, delta, 1.0f);
    magnitude = fast_vec_dot(delta, velocity);
    if (magnitude < 0.0f)
        magnitude = 0.0f;
    fast_vec_normalize(delta, velocity, magnitude);
    qcopy(motion_reference_vector.f, velocity);
    gravity_flatten(motion_reference_vector.f, motion_reference_vector.f);
    gravity_flatten(horizontal, horizontal);
    fast_vec_normalize(horizontal, horizontal, 1.0f);
    magnitude = fast_vec_dot(horizontal, motion_reference_vector.f);
    if (magnitude < 0.0f)
        magnitude = 0.0f;
    fast_vec_normalize(horizontal, motion_reference_vector.f, magnitude);
    qcopy(plane.f, velocity);
    set_gravity_relative_vector_length_xy(plane.f, plane.f, 0.0f);
    set_gravity_relative_vector_length_xy(vertical, vertical, 0.0f);
    fast_vec_normalize(vertical, vertical, 1.0f);
    magnitude = fast_vec_dot(vertical, plane.f);
    if (magnitude < 0.0f)
        magnitude = 0.0f;
    fast_vec_normalize(vertical, plane.f, magnitude);
    motion_player = (L16MovementPlayer *)((char *)delta - 0x110);
    motion_player->speed = fast_vec_length(delta);
    motion_player->horizontal_speed = vector_length_xy(delta);
    motion_reference_vector.q = *(OvlQuad *)delta;
    motion_player->vertical_speed = forward_component(motion_reference_vector.f);
    if (motion_player->vertical_speed < 0.0f)
        motion_player->vertical_speed = 0.0f;
    qcopy(motion_reference_vector.f, new_position);
    contact = (float *)((char *)delta - 0x20);
    external = (float *)((char *)delta - 0x10);
    if (fast_vec_length(contact) > 0.0001f) {
        float saved;

        fast_vec_add(new_position, new_position, contact);
        saved = motion_player->contact[3];
        clear_u64_value(contact);
        motion_player->contact[3] = saved;
        resolve_hero_collisions_with_history();
        update_hero_ground_contact();
        fast_vec_sub(external, new_position, previous.f);
        correct_hero_ground_penetration();
    }
    fast_vec_sub((char *)delta + 0x30, new_position, motion_reference_vector.f);
    motion_player->frame_delta[3] = motion_player->contact[3];
    motion_player->contact[3] = 0.0f;
    fast_vec_sub(external, new_position, previous.f);
    motion_player->slope = 0.0f;
    if (motion_player->horizontal_speed > 0.004f) {
        motion_player->slope = motion_player->external[2] / motion_player->horizontal_speed;
        if (motion_player->slope > 0.5f)
            motion_player->slope = 0.5f;
        else if (motion_player->slope < -0.5f)
            motion_player->slope = -0.5f;
    }
    cap_player = (L16MovementPlayer *)(((char *)((char *)&hero)));
    magnitude = frame_time * 52.0f;
    if (magnitude < cap_player->speed) {
        fast_vec_scale(cap_player->external, cap_player->external, magnitude / cap_player->speed);
        cap_player->speed = frame_time * 52.0f;
    }
}
