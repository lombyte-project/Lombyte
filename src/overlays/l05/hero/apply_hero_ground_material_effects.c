/* Level 05 hero ground handling: apply_hero_ground_material_effects, update_hero_ground_contact, resolve_hero_position_collisions and integrate_hero_motion_with_collisions. */
#include "types.h"
#include "rnc/hero/hero_state.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/overlay/hero.h"
#include "rnc/overlay/collision.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/weapons/suck_aim.h"
#include "rnc/overlay/shared/hero/set_hero_animation.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/world/collision/collision_hit_material.h"

int        FUN_L00_001f0d60_239fc0(void *, int, int, f32) __asm__("FUN_L00_001f0d60");
f32        absolute_float(f32) __asm__("func_001F99C0");
void       exit_hero_vehicle_mode_if_mode_three(void) __asm__("FUN_L00_0020b930");
extern int hero_set_state(int, int) __asm__("FUN_L05_0024cee8");
void       apply_hero_ground_material_effects(void) __asm__("FUN_L05_00239fc0");

void apply_hero_ground_material_effects(void) {
    u8  save = hero.ground_sound_class;
    s16 saved_ground_type = hero.ground_type;
    f32 water_depth;
    int s;
    fast_mem_zero16(&hero.ground_type, 0x10);
    hero.ground_sound_class = save;
    hero.ground_type = -1;
    hero.wading = 0;
    hero.unk308 = 0;
    if (saved_ground_type == -1)
        return;
    if (saved_ground_type == 2) {
        if (hero.frames_off_ground.s == 0 || hero.ground_distance < 0.3f) {
            hero.on_magnet_ground = 1;
            if (hero.vehicle_mode == 0 && ((u8 *)hero.items[1].moby) != 0 &&
                *(s16 *)(((u8 *)hero.items[1].moby) + 0xA6) == 0xAD)
                hero.unk308 = 1;
        }
    }
    if (saved_ground_type == 14) {
        hero.base_condition = 1;
        hero.water_depth = 0.2f;
        hero.water_surface_z = hero.ground_z.f + 0.2f;
    }
    if (saved_ground_type == 0) {
        water_depth = hero.water_surface_z - hero.ground_z.f;
        hero.water_depth = water_depth;
        if (water_depth < 0.85f && 0.25f < water_depth)
            hero.wading = 1;
        hero.base_condition = 1;
    }
    if (saved_ground_type == 3)
        hero.selector_3 = 1;
    if (current_level_index == 13 && hero.state.current != 0x7B &&
        FUN_L00_001f0d60_239fc0(
            (u8 *)&hero.motion.body_center, 2, 0, hero.capsule_radius + 0.03f) &&
        collision_hit_material() == 11) {
        goto s7B;
    }
    if (saved_ground_type == 11)
        hero.selector_11 = 1;
    if (saved_ground_type == 5)
        hero.selector_5 = 1;
    if (saved_ground_type == 6)
        hero.unk12E9 = 1;
    if (saved_ground_type == 4)
        hero.selector_4 = 1;
    if (saved_ground_type == 13)
        hero.selector_13 = 1;
    if (saved_ground_type == 1)
        hero.selector_1 = 1;
    if (saved_ground_type == 8)
        hero.on_slip_ground = 1;
    if (saved_ground_type == 9)
        hero.unk12EE = 1;
    if (saved_ground_type == 12)
        hero.on_slip_ground = 1;
    if (hero.selector_4 && hero.grounded_frames && hero.state.control_mode != 0x10 &&
        hero.state.control_mode != 0x14 && hero.state.control_mode != 7) {
        s = 0x31;
        goto call;
    }
    if (hero.selector_1 && hero.grounded_frames &&
        (hero.state.current != 0x3C || hero.past_jump_apex != 0)) {
        exit_hero_vehicle_mode_if_mode_three();
        if (hero.health.hp)
            goto call3c;
        s = 0x7C;
        goto call;
    }
    if (hero.selector_13 && hero.state.current != HERO_STATE_SINK &&
        absolute_float(hero.water_surface_z - (hero.motion.pos.f[2] + 0.25f)) < 1.0f &&
        0.0f < hero.water_surface_z - hero.motion.pos.f[2] &&
        hero.motion.displacement.f[2] < 0.0f) {
        exit_hero_vehicle_mode_if_mode_three();
        s = 0x7F;
        goto call;
    }
    if (hero.selector_3 && hero.state.current != 0x68 && hero.state.current != 0x7B &&
        absolute_float(hero.sink_surface_z - (hero.motion.pos.f[2] + 0.25f)) < 1.0f &&
        0.0f < hero.sink_surface_z - hero.motion.pos.f[2] &&
        (hero.state.current != 0x69 || hero.past_jump_apex != 0) &&
        hero.motion.displacement.f[2] < 0.0f) {
        exit_hero_vehicle_mode_if_mode_three();
        if (hero.health.hp) {
            s = 0x68;
            goto call;
        }
    s7B:
        s = 0x7B;
    call:
        hero_set_state(s, 1);
        return;
    call3c:
        s = 0x3C;
        goto call;
    }
    if (hero.selector_11 && hero.state.current != 0x7B &&
        absolute_float(hero.sink_surface_z - (hero.motion.pos.f[2] + 0.25f)) < 1.0f &&
        0.0f < hero.sink_surface_z - hero.motion.pos.f[2] && hero.motion.displacement.f[2] < 0.0f) {
        exit_hero_vehicle_mode_if_mode_three();
        hero_set_state(0x7B, 1);
    }
}

#include "qcopy.h"
#include "rnc/overlay/quad.h"

typedef union {
    OvlQuad q;
    f32     f[4];
} V;

extern u8                 D_L05_00174240[] NOT_SDA;
extern u8                 D_L05_00174260[] NOT_SDA;
extern f32 D_L05_00174268 NOT_SDA;

void hero_get_collision_ray_endpoints(void *, void *, s32, f32, f32) __asm__("FUN_L00_002126b8");
s32  collision_line(void *, void *, s32, s32, s32) __asm__("FUN_001efa68");
s32  coll_sound_class(void) __asm__("FUN_L00_001f3608");
f32  resolve_camera_surface_height(void *, void *) __asm__("FUN_002135f0");
void sceVu0Normalize(void *, void *) __asm__("FUN_00125180");
f32  compute_gravity_relative_vector_inclination(void *) __asm__("FUN_L00_00233d30");
void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
f32  FUN_001f9ab0_c(void *, void *) __asm__("FUN_001f9ab0");
void FUN_001f9a68_c(void *, void *, f32) __asm__("FUN_001f9a68");
void FUN_001f9bf8_c(void *, void *, f32) __asm__("FUN_001f9bf8");
void compute_rotated_vector_angles(void *, void *) __asm__("FUN_L00_00216078");
void transform_hero_polar_offset_to_world(void *, f32, f32, f32) __asm__("FUN_L00_002335c8");
void FUN_L00_002334d0(void *, void *, f32);

extern CollisionHit HH __asm__("D_L05_00174240") NOT_SDA;
#define HI(o) (*(s32 *)(D_L05_00174240 + (o)))
#define HF(o) (*(f32 *)(D_L05_00174240 + (o)))

void update_hero_ground_contact(void) __asm__("FUN_L05_002402a8");

void update_hero_ground_contact(void) {
    V shared_probe_endpoint, ground_probe_endpoint, ground_vector_scratch, side_probe_endpoint,
        unused_probe_vector, hit_normal_angles, normalized_hit_normal;
    f32 f;
    s32 ground_sound_class;
    s32 i;
    f32 side_probe_angle;
    u8 *hit_moby_record;

    s32 k;
    f32 ground_probe_endpoint_offset;

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
    ground_probe_endpoint_offset = hero.capsule_radius * 2.2f;
    if (hero.state.control_mode == 2) {
        ground_probe_endpoint_offset = hero.capsule_radius * 2.9f;
    }
    hero_get_collision_ray_endpoints(
        &ground_probe_endpoint, &shared_probe_endpoint, k, ground_probe_endpoint_offset, -48.0f);
    if (collision_line(&ground_probe_endpoint, &shared_probe_endpoint, 2, (s32)hero.moby, 0) == 0) {
        goto tail;
    }
    ground_sound_class = coll_sound_class();
    if (ground_sound_class == -1) {
        ground_sound_class = 3;
    }
    hero.ground_sound_class = ground_sound_class;
    hero.ground_type = collision_hit_material();
    if (hero.ground_type == 0) {
        hero.water_surface_z =
            resolve_camera_surface_height(D_L05_00174260, (u8 *)&hero.water_surface_normal);
        if (hero.motion.pos.f[2] < hero.water_surface_z && hero.in_water_frames <= 0) {
            hero.in_water_frames = 1;
        }
        if (collision_line(
                &ground_probe_endpoint, &shared_probe_endpoint, 0x24, (s32)hero.moby, 0) == 0) {
            goto d1;
        }
        ground_sound_class = coll_sound_class();
        if (current_level_index == 1 || current_level_index == 0x12) {
            ground_sound_class = 3;
        }
        hero.ground_sound_class = ground_sound_class;
    }
    if (hero.ground_type == 0xD) {
        hero.water_surface_z = D_L05_00174268;
        ground_probe_endpoint.f[2] = D_L05_00174268 - 0.01f;
        if (collision_line(&ground_probe_endpoint, &shared_probe_endpoint, 4, (s32)hero.moby, 0) ==
            0) {
            goto d1;
        }
    }
    if (hero.ground_type == 3) {
        hero.sink_surface_z = D_L05_00174268;
        ground_probe_endpoint.f[2] = D_L05_00174268 - 0.01f;
        if (collision_line(&ground_probe_endpoint, &shared_probe_endpoint, 4, (s32)hero.moby, 0) ==
            0) {
            goto d1;
        }
    }
    if (hero.ground_type == 0xB) {
        hero.sink_surface_z = D_L05_00174268;
        goto d1;
    }
    if (hero.ground_type == 1 && (u32)(hero.state.current - 0x7C) < 2) {
        goto d1;
    }
    if (HH.moby != 0) {
        if (is_moby_in_class_range_500_to_540(HH.moby)) {
            hero.on_flagged_ground_moby = 1;
            if ((u32)(hero.state.control_mode - 0x15) < 2) {
                FUN_L00_002334d0((u8 *)&HH.point, (u8 *)&HH.point, 1.0f);
            }
        }
        hit_moby_record = suck_get_aim_height(HH.moby);
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
        fast_vec_sub(&ground_vector_scratch, (u8 *)&hero.ground_point, (u8 *)&hero.motion.pos);
        if (0.0f < FUN_001f9ab0_c(&ground_vector_scratch, (u8 *)&hero.ground_normal)) {
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
        FUN_001f9a68_c((u8 *)&hero.gravity_dir, (u8 *)&hero.gravity_dir, -1.0f);
        break;
    case HERO_GRAVITY_TOWARD_REFERENCE_POINT:
        fast_vec_sub(
            (u8 *)&hero.gravity_dir, (u8 *)&hero.motion.gravity_point, (u8 *)&hero.motion.pos);
        FUN_001f9bf8_c((u8 *)&hero.gravity_dir, (u8 *)&hero.gravity_dir, 1.0f);
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
    compute_rotated_vector_angles((u8 *)&hero.ground_normal, &ground_vector_scratch);
    if (-0.78539819f < ground_vector_scratch.f[1] && ground_vector_scratch.f[1] < 0.78539819f) {
        hero.ground_pitch.f = ground_vector_scratch.f[1];
    }
    if (-0.78539819f < ground_vector_scratch.f[0] && ground_vector_scratch.f[0] < 0.78539819f) {
        hero.unk2E8.f = ground_vector_scratch.f[0];
    }
    for (i = 0; i < 2; i++) {
        side_probe_angle = 1.5707964f;
        if (i == 1) {
            side_probe_angle = -side_probe_angle;
        }
        transform_hero_polar_offset_to_world(&side_probe_endpoint, 0.2f, side_probe_angle, 0.45f);
        transform_hero_polar_offset_to_world(&unused_probe_vector, 0.2f, side_probe_angle, -0.45f);
        if (collision_line(&side_probe_endpoint, &shared_probe_endpoint, 0x22, (s32)hero.moby, 0)) {
            f = HH.point.f[2] - hero.ground_z.f;
            hero.unk2C0.f[i] = f;
            if (0.4f < absolute_float(f)) {
                hero.unk2C0.f[i] = 0.0f;
            }
            sceVu0Normalize(&normalized_hit_normal, (u8 *)&HH.normal_x);
            compute_rotated_vector_angles(&normalized_hit_normal, &hit_normal_angles);
            if (-0.78539819f < hit_normal_angles.f[1] && hit_normal_angles.f[1] < 0.78539819f) {
                hero.unk2C8.f[i] = hit_normal_angles.f[1];
            }
            if (-0.78539819f < hit_normal_angles.f[0] && hit_normal_angles.f[0] < 0.78539819f) {
                hero.unk2D0.f[i] = hit_normal_angles.f[0];
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

#include "rnc/math/fast_math.h"

typedef union {
    u128_2133a8 q;
    f32         f[4];
} V_2133a8;

extern void FUN_L00_002347c0(void);
extern void transform_hero_local_offset_to_world(void *, f32, f32, f32) __asm__("FUN_L00_00233660");
extern void FUN_L00_00233810(void *, void *, f32);
extern void FUN_L00_002334d0(void *, void *, f32);
extern s32  coll_sphere(void *, s32, s32, f32) __asm__("FUN_L00_001f0d60");
extern s32  rig_probe(void *, s32, f32, s32, f32) __asm__("FUN_L00_001f19a0");
extern s32  FUN_L00_001f3170(void *, f32);
extern u128_2133a8 D_L05_00174270[] NOT_SDA;
s32                resolve_hero_position_collisions(s32 mode) __asm__("FUN_L05_00240ed0");

s32 resolve_hero_position_collisions(s32 mode) {
    V_2133a8     old;
    V_2133a8     collision_query_offset;
    V_2133a8     resolved_displacement;
    s32          mask;
    s32          i;
    u128_2133a8 *collision_result_base;
    f32          query_height_or_displacement_length;
    if (hero.collision_off_frames != 0) {
        return 1;
    }
    qcopy(&old, &hero.motion.pos.q);
    clear_u64_value(&collision_query_offset);
    if (hero.path_constrain_pending != 0) {
        FUN_L00_002347c0();
        hero.path_constrain_pending = 0;
    }
    if (hero.state.control_mode != 0x11) {
        if (hero.state.control_mode == 0x16) {
            transform_hero_local_offset_to_world(&collision_query_offset, 0.0f, 0.0f, 0.7f);
            fast_vec_sub(&collision_query_offset, &collision_query_offset, &hero.motion.pos.q);
        } else if (hero.gravity_mode != 0 || hero.unk1F8 != 0) {
            if (hero.gravity_mode == 1 || hero.unk1F8 != 0 ||
                (unsigned)(hero.state.control_mode - 0x15) < 2) {
                FUN_L00_00233810(&collision_query_offset, &collision_query_offset, 0.6f);
            } else {
                FUN_L00_002334d0(
                    &collision_query_offset, &collision_query_offset, -hero.capsule_bottom);
            }
        } else {
            collision_query_offset.f[2] = hero.capsule_bottom;
        }
    }
    fast_vec_add(&hero.motion.pos.q, &hero.motion.pos.q, &collision_query_offset);
    mask = 0x24;
    if (hero.state.current == HERO_STATE_SINK) {
        mask = 0xD24;
    }
    for (i = 0; i < 8; i++) {
        if (hero.gravity_mode != 0) {
            if (!coll_sphere(&hero.motion.pos.q, mask, ((s32)hero.moby), frame_scale * 0.4f))
                break;
        } else if (hero.state.control_mode == 0x11) {
            if (!coll_sphere(&hero.motion.pos.q, mask, ((s32)hero.moby), 0.6f))
                break;
        } else if (hero.state.control_mode == 0x16) {
            if (!coll_sphere(&hero.motion.pos.q, mask, ((s32)hero.moby), frame_scale * 0.5f))
                break;
        } else {
            s32 collision_query_result;
            query_height_or_displacement_length = hero.capsule_top - hero.capsule_bottom;
            if (query_height_or_displacement_length < 0.05f) {
                query_height_or_displacement_length = 0.05f;
            }
            collision_query_result = rig_probe(&hero.motion.pos.q,
                                               mask,
                                               hero.capsule_radius,
                                               ((s32)hero.moby),
                                               query_height_or_displacement_length);
            if (!(collision_query_result |
                  FUN_L00_001f3170(&hero.motion.pos.q, hero.capsule_radius)))
                break;
        }
        collision_result_base = D_L05_00174270;
        qcopy(&hero.motion.pos.q, collision_result_base);
        qcopy(&hero.wall_contact_normal.q, collision_result_base + 1);
        qcopy(&hero.wall_contact_point.q, collision_result_base - 1);
        hero.wall_hit = 1;
        hero.coll_hit_moby = ((u8 **)collision_result_base)[-6];
    }
    fast_vec_sub(&hero.motion.pos.q, &hero.motion.pos.q, &collision_query_offset);
    fast_vec_sub(&resolved_displacement, &hero.motion.pos.q, &old);
    query_height_or_displacement_length = fast_vec_length(&resolved_displacement);
    if (hero.capsule_radius * 1.5f < query_height_or_displacement_length) {
        if (mode == 0xF) {
            if (512.0f < resolved_displacement.f[0])
                resolved_displacement.f[0] = 512.0f;
            else if (resolved_displacement.f[0] < -512.0f)
                resolved_displacement.f[0] = -512.0f;
            if (512.0f < resolved_displacement.f[1])
                resolved_displacement.f[1] = 512.0f;
            else if (resolved_displacement.f[1] < -512.0f)
                resolved_displacement.f[1] = -512.0f;
            if (512.0f < resolved_displacement.f[2])
                resolved_displacement.f[2] = 512.0f;
            else if (resolved_displacement.f[2] < -512.0f)
                resolved_displacement.f[2] = -512.0f;
            fast_vec_normalize(&resolved_displacement, &resolved_displacement, hero.capsule_radius);
            fast_vec_add(&hero.motion.pos.q, &old, &resolved_displacement);
        }
        return -1;
    }
    return 1;
}

typedef union {
    OvlQuad q;
    f32     f[4];
} V_c413e0;

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

extern char *collision_hit_moby[] __asm__("D_L05_00174258");
extern float forward_component(void *) __asm__("FUN_L00_00213350");
extern float gravity_length_xy(float *v) __asm__("FUN_L00_002339d0");
extern void  correct_hero_ground_penetration(void) __asm__("FUN_L00_00212ff0");
extern void  resolve_hero_collisions_with_history(void) __asm__("FUN_L00_002137a8");
extern void  gravity_flatten(float *, float *) __asm__("FUN_L00_002338d0");
extern void
set_gravity_relative_vector_length_xy(float *, float *, float) __asm__("FUN_L00_00233ba0");
extern void update_hero_attachment(int, void *, void *) __asm__("FUN_L00_00233f80");

/* Same routine as FUN_L16_00211098, without the mode 15 and 21 cases. */
void integrate_hero_motion_with_collisions(void) __asm__("FUN_L05_002413e0");

void integrate_hero_motion_with_collisions(void) {
    V_c413e0           previous;
    V_c413e0           work;
    V_c413e0           plane;
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
    } else if (player->mode == 22) {
        qcopy(work.f, position);
        work.f[2] += 0.5f;
        fast_vec_add(plane.f, work.f, (char *)position + 0x60);
        if (collision_line(position, plane.f, 2, 0, 0)) {
            char *hit = collision_hit_moby[0];

            if (!hit || *(short *)(hit + 0xA6) == 0x1F6 || *(short *)(hit + 0xA6) == 0x59F) {
                player = (L16MovementPlayer *)(((char *)((char *)&hero)));
                if (!player->unconstrained) {
                    float length;
                    float limit;

                    position = (float *)((char *)player + 0xE0);
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
    qcopy(work.f, velocity);
    gravity_flatten(work.f, work.f);
    gravity_flatten(horizontal, horizontal);
    fast_vec_normalize(horizontal, horizontal, 1.0f);
    magnitude = fast_vec_dot(horizontal, work.f);
    if (magnitude < 0.0f)
        magnitude = 0.0f;
    fast_vec_normalize(horizontal, work.f, magnitude);
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
    work.q = *(OvlQuad *)delta;
    motion_player->vertical_speed = forward_component(work.f);
    if (motion_player->vertical_speed < 0.0f)
        motion_player->vertical_speed = 0.0f;
    qcopy(work.f, new_position);
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
    fast_vec_sub((char *)delta + 0x30, new_position, work.f);
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
