/* Hero: ground contact update. */
#include "types.h"
#include "rnc/hero/hero_state.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/overlay/hero.h"
#include "rnc/overlay/collision.h"
#include "sda.h"
#include "rnc/mobys/get_moby_target_info.h"
#include "rnc/overlay/shared/hero/hero_switch_item.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/world/collision/collision_hit_material.h"
#include "rnc/overlay/shared/camera/find_camera_collision_volume_for_point.h"
#include "rnc/sdk/library/sceVu0Normalize.h"
#include "rnc/world/collision/collision_line.h"

typedef struct {
    char pad[0x24];
    int  v;
    char pad2[0x24];
} Rec;

extern Rec D_L15_0017A0C0[];

/* Same routine as FUN_L06_0021d0a0, with case 2 in place of case 1. */
int FUN_L15_001fe778(int allow_item_lookup) {
    struct Hero *vehicle_check_hero = &hero;
    switch (vehicle_check_hero->vehicle_mode) {
    case HERO_VEHICLE_MODE_GIANT_CLANK:
        return 0;
    case 3:
        return 0;
    }
    {
        struct Hero *status_check_hero = &hero;
        if (status_check_hero->health.hp == 1) {
            return 0x54;
        }
        if (allow_item_lookup == 0) {
            return 0;
        }
        if (status_check_hero->item_anim_active == 0) {
            return 0;
        }
        if (status_check_hero->unk20AA == 0) {
            return 0;
        }
        if (status_check_hero->unk22C8 != 0) {
            return 0;
        }
    }
    {
        int i = hero_slot_item_id(0);
        return D_L15_0017A0C0[i].v;
    }
}

#include "qcopy.h"
#include "rnc/overlay/quad.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#define GI(o) (*(s32 *)(hero_lui + (o)))
#define GF(o) (*(f32 *)(hero_lui + (o)))
#define GH(o) (*(s16 *)(hero_lui + (o)))
#define GU(o) (*(u16 *)(hero_lui + (o)))
#define GB(o) (hero_lui[o])
#define HI(o) (*(s32 *)(D_L15_00174440 + (o)))
#define HF(o) (*(f32 *)(D_L15_00174440 + (o)))
typedef union {
    OvlQuad q;
    f32     f[4];
} V;

extern u8 D_L15_00174440[] NOT_SDA;
extern u8 D_L15_00174460[] NOT_SDA;

void hero_get_collision_ray_endpoints(void *, void *, s32, f32, f32) __asm__("FUN_L00_002126b8");
f32  resolve_camera_surface_height(void *, void *) __asm__("FUN_002135f0");
f32  compute_gravity_relative_vector_inclination(void *) __asm__("FUN_L00_00233d30");
void compute_rotated_vector_angles(void *, void *) __asm__("FUN_L00_00216078");
void transform_hero_polar_offset_to_world(void *, f32, f32, f32) __asm__("FUN_L00_002335c8");

#define GI(o) (*(s32 *)(hero_lui + (o)))
#define GF(o) (*(f32 *)(hero_lui + (o)))
#define GH(o) (*(s16 *)(hero_lui + (o)))
#define GU(o) (*(u16 *)(hero_lui + (o)))
#define GB(o) (hero_lui[o])
extern CollisionHit HH __asm__("D_L15_00174440") NOT_SDA;
#define HI(o) (*(s32 *)(D_L15_00174440 + (o)))
#define HF(o) (*(f32 *)(D_L15_00174440 + (o)))

extern struct Hero hero_lui __asm__("D_0013F350") NOT_SDA;
void               update_hero_ground_contact(void) __asm__("FUN_L15_0020a7a0");

void update_hero_ground_contact(void) {
    V shared_ray_endpoint, ground_ray_endpoint, ground_displacement_or_angles,
        surface_probe_endpoint, unused_probe_vector, probe_surface_angles, probe_surface_normal;
    f32 f;
    s32 collision_sound_class;
    s32 i;
    f32 surface_probe_angle;
    u8 *aim_height_data;

    s32 k;
    f32 ground_ray_endpoint_offset;

    hero.ground_normal.f[2] = 1.0f;
    hero.air_frames.s++;
    hero.frames_off_ground.s++;
    hero.ground_normal.i[0] = 0;
    hero.ground_normal.i[1] = 0;
    *(s32 *)&hero.ground_slope = 0;
    *(s32 *)&hero.ground_z = 0;
    clear_u64_value(&hero.ground_point);
    hero.on_flagged_ground_moby = 0;
    hero.ground_distance = 32.0f;
    hero.ground_moby = 0;
    if (hero.in_water_frames != 0) {
        hero.in_water_frames++;
    }
    k = 0;
    if (hero.gravity_mode)
        k = 1;
    ground_ray_endpoint_offset = hero.capsule_radius * 2.2f;
    if (hero.state.control_mode == 2) {
        ground_ray_endpoint_offset = hero.capsule_radius * 2.9f;
    }
    hero_get_collision_ray_endpoints(
        &ground_ray_endpoint, &shared_ray_endpoint, k, ground_ray_endpoint_offset, -48.0f);
    if (collision_line(&ground_ray_endpoint, &shared_ray_endpoint, 2, hero.moby, 0) == 0) {
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
            resolve_camera_surface_height(D_L15_00174460, &hero.water_surface_normal);
        if (hero.motion.pos.f[2] < hero.water_surface_z && hero.in_water_frames <= 0) {
            hero.in_water_frames = 1;
        }
        if (collision_line(&ground_ray_endpoint, &shared_ray_endpoint, 0x24, hero.moby, 0) == 0) {
            goto d1;
        }
        collision_sound_class = coll_sound_class();
        if (current_level_index == 1 || current_level_index == 0x12) {
            collision_sound_class = 3;
        }
        hero.ground_sound_class = collision_sound_class;
    }
    if (hero.ground_type == 0x3) {
        hero.sink_surface_z = HH.point.f[2];
        ground_ray_endpoint.f[2] = HH.point.f[2] - 0.01f;
        if (collision_line(&ground_ray_endpoint, &shared_ray_endpoint, 4, hero.moby, 0) == 0) {
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
        }
        aim_height_data = get_moby_target_info(HH.moby);
        if (aim_height_data != 0 && (*(u16 *)(aim_height_data + 0x1E) & 8)) {
            hero.on_flagged_ground_moby = 1;
        }
    }
    if (HH.unk1C > 0) {
        qcopy(&hero.ground_point, &HH.point);
        hero.ground_z.f = HH.point.f[2];
        hero.ground_distance = fast_vec_dist(&hero.motion.pos, &hero.ground_point);
        hero.ground_moby = (struct Moby *)HH.moby;
        sceVu0Normalize(&hero.ground_normal, (u8 *)&HH.normal_x);
        hero.ground_slope.f = compute_gravity_relative_vector_inclination((u8 *)&HH.normal_x);
        hero.downhill_yaw = fast_arc_tan(HH.normal_x, HH.normal_y);
        fast_vec_sub(&ground_displacement_or_angles, &hero.ground_point, &hero.motion.pos);
        if (0.0f < fast_vec_dot(&ground_displacement_or_angles, &hero.ground_normal)) {
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
        qcopy(&hero.last_ground_point, &hero.ground_point);
    }
    if (hero.motion.pos.f[2] > hero.water_surface_z) {
        hero.in_water_frames = 0;
    }
d1:
    clear_u64_value(&hero.gravity_dir);
    switch (hero.gravity_mode) {
    case HERO_GRAVITY_WORLD_DOWN:
        hero.gravity_dir.f[2] = -1.0f;
        break;
    case HERO_GRAVITY_OPPOSITE_GROUND_NORMAL:
        qcopy(&hero.gravity_dir, &hero.ground_normal);
        fast_vec_scale(&hero.gravity_dir, &hero.gravity_dir, -1.0f);
        break;
    case HERO_GRAVITY_TOWARD_REFERENCE_POINT:
        fast_vec_sub(&hero.gravity_dir, &hero.motion.gravity_point, &hero.motion.pos);
        fast_vec_normalize(&hero.gravity_dir, &hero.gravity_dir, 1.0f);
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
    compute_rotated_vector_angles(&hero.ground_normal, &ground_displacement_or_angles);
    if (-0.78539819f < ground_displacement_or_angles.f[1] &&
        ground_displacement_or_angles.f[1] < 0.78539819f) {
        hero.ground_pitch.f = ground_displacement_or_angles.f[1];
    }
    if (-0.78539819f < ground_displacement_or_angles.f[0] &&
        ground_displacement_or_angles.f[0] < 0.78539819f) {
        hero.unk2E8.f = ground_displacement_or_angles.f[0];
    }
    for (i = 0; i < 2; i++) {
        surface_probe_angle = 1.5707964f;
        if (i == 1) {
            surface_probe_angle = -surface_probe_angle;
        }
        transform_hero_polar_offset_to_world(
            &surface_probe_endpoint, 0.2f, surface_probe_angle, 0.45f);
        transform_hero_polar_offset_to_world(
            &unused_probe_vector, 0.2f, surface_probe_angle, -0.45f);
        if (collision_line(&surface_probe_endpoint, &shared_ray_endpoint, 0x22, hero.moby, 0)) {
            f = HH.point.f[2] - hero.ground_z.f;
            hero.unk2C0.f[i] = f;
            if (0.4f < absolute_float(f)) {
                hero.unk2C0.f[i] = 0.0f;
            }
            sceVu0Normalize(&probe_surface_normal, (u8 *)&HH.normal_x);
            compute_rotated_vector_angles(&probe_surface_normal, &probe_surface_angles);
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
