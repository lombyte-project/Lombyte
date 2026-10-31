/* Level 14 racer helpers (callers in racer.c and path_camera.c): path tracking and start, group reset, path following and weapon fire, glow quads and joint effects. */
#include "types.h"
#include "rnc/overlay/level_spline.h"
#include "rnc/overlay/level_path.h"
#include "rnc/overlay/level_globals.h"
#include "rnc/math/vector.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "asm.h"
#include "qcopy.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/entities.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/rendering/effect_texture.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/mobys/animation/build_moby_bone_transform.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/rendering/view/build_look_at_matrix.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/rendering/effects/glow_quads_draw.h"
#include "rnc/world/collision/collision_line.h"

/* Finds the path segments nearest to the two target points and sums the segment lengths between them. */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002D7668), where it is exact; names translated to the US level program. */

typedef struct {
    float v[4];
    int   i;
    float t;
} PathPos;

extern int FUN_L00_0025df68(void *, void *, float *, int *, float *, int, float, float, float);
float      racer_path_lead_distance_gp __asm__("D_L14_00161A38") = 9.0f;

void initialize_racer_path_tracking(struct Moby *racer_moby) __asm__("FUN_L14_002d6358");

void initialize_racer_path_tracking(struct Moby *racer_moby) {
    char          *moby_data = (char *)racer_moby->pvars;
    char          *path = (char *)l14_splines[*(int *)(moby_data + 0x60)].path;
    char          *tg;
    PathPos        nearest_path_position;
    int            k;
    float          z;
    unsigned short second_point_segment;
    *(float *)(moby_data + 0x74) = racer_path_lead_distance_gp;
    *(int *)(moby_data + 0x68) = 0;
    *(int *)(moby_data + 0x64) = 0;
    z = *(float *)(moby_data + 0x68);
    tg = (char *)l14_paths[*(int *)(moby_data + 0x88)] + 0x30;
    nearest_path_position.t = z;
    nearest_path_position.i = 0;
    FUN_L00_0025df68(path,
                     tg,
                     nearest_path_position.v,
                     &nearest_path_position.i,
                     &nearest_path_position.t,
                     0,
                     20.0f,
                     5.0f,
                     z);
    *(short *)(moby_data + 0x8C) = nearest_path_position.i;
    nearest_path_position.t = z;
    nearest_path_position.i = 0;
    FUN_L00_0025df68(path,
                     (char *)l14_paths[*(int *)(moby_data + 0x88)] + 0x20,
                     nearest_path_position.v,
                     &nearest_path_position.i,
                     &nearest_path_position.t,
                     0,
                     20.0f,
                     5.0f,
                     z);
    second_point_segment = nearest_path_position.i;
    *(short *)(moby_data + 0x8E) = second_point_segment;
    *(float *)(moby_data + 0x78) = z;
    for (k = (short)second_point_segment; k < *(short *)(moby_data + 0x8C); k++) {
        *(float *)(moby_data + 0x78) += *(float *)(path + k * 16 + 0x1C);
    }
}

/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002D77E0), where it is exact; names translated to the US level program. */

int interpolate_racer_to_path_start(struct Moby *racer_moby) __asm__("FUN_L14_002d64d0");

int interpolate_racer_to_path_start(struct Moby *racer_moby) {
    char *moby_data = (char *)racer_moby->pvars;
    char *ent = (char *)l14_splines[*(int *)(moby_data + 0x60)].path;
    int   ok;
    float remaining_frames;
    ok = fast_dec_timer(moby_data + 0x6C) != 0;
    remaining_frames = convert_integer_to_float(*(int *)(moby_data + 0x6C));
    lerp_vector(&racer_moby->pos,
                ent + 0x10,
                ((char *)l14_paths[*(int *)(moby_data + 0x84)]) + 0x10,
                remaining_frames * *(float *)(moby_data + 0x70));
    return ok;
}

/* Lines up the three listed racers on the hero's track at the start point and resets their race state. */
extern unsigned short *D_L14_001ABF40_6570[] __asm__("D_L14_001ABF40");
extern char           *D_L14_0015FFD8_6570 __asm__("D_L14_0015FFD8");

typedef struct {
    char  p[0x48];
    float f48;
} M_6570;

typedef struct {
    char  p0[0x80];
    float f80;
    float f84;
    char  p1[0x560 - 0x88];
    char *f560;
    int   f564;
    float f568;
} G_6570;

extern int FUN_L00_0025df68_c(
    void *, void *, void *, int *, float *, int, float, float, float) __asm__("FUN_L00_0025df68");
extern void set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern void
FUN_L00_0025d808_6570(void *, void *, void *, void *, int, float) __asm__("FUN_L00_0025d808");

void reset_racer_group_to_hero_path_position(struct Moby *group_anchor_moby) __asm__("FUN_L14_002d6570");

void reset_racer_group_to_hero_path_position(struct Moby *group_anchor_moby) {
    char           *list[4];
    float           near[4];
    float           path_heading_delta[4];
    float           hero_path_euler_rotation[4];
    int             path_segment_index;
    float           nearest_segment_distance;
    unsigned short *moby_index_cursor = D_L14_001ABF40_6570[group_anchor_moby->group];
    char          **racer_list_write_cursor;
    int             i;
    float           path_step_distance;
    if (moby_index_cursor == 0) {
        return;
    }
    racer_list_write_cursor = list;
    do {
        char *o = D_L14_0015FFD8_6570 + ((*moby_index_cursor & 0x7FFF) << 8);
        char *od = *(char **)(o + 0x78);
        char *path = hero.rail_spline;
        if (path == (char *)l14_splines[*(int *)(od + 0x60)].path) {
            char *tp = ((char *)l14_paths[*(int *)(od + 0x88)]);
            nearest_segment_distance = 0.0f;
            FUN_L00_0025df68_c(path,
                               tp + 0x10,
                               near,
                               &path_segment_index,
                               &nearest_segment_distance,
                               0,
                               20.0f,
                               5.0f,
                               0.0f);
            fast_vec_sub(path_heading_delta,
                         path + (path_segment_index * 16 + 0x40),
                         path + (path_segment_index * 16 + 0x10));
            hero_path_euler_rotation[0] = 0.0f;
            hero_path_euler_rotation[1] = 0.0f;
            hero_path_euler_rotation[2] = fast_add_rotations(
                fast_arc_tan(path_heading_delta[0], path_heading_delta[1]), -1.5707964f);
            hero_path_euler_rotation[3] = 0.0f;
            hero.rail_segment = path_segment_index;
            hero.rail_seg_frac = 0.0f;
            set_race_camera(
                path + (path_segment_index * 16 + 0x10), hero_path_euler_rotation, 0x28, 0);
        }
        *racer_list_write_cursor++ = o;
    } while ((short)*moby_index_cursor++ >= 0);
    for (i = 0; i < 3; i++) {
        char *o = list[i];
        char *od = *(char **)(o + 0x78);
        char *path = (char *)l14_splines[*(int *)(od + 0x60)].path;
        o[0x20] = 6;
        ((M_6570 *)o)->f48 = fast_arc_tan(hero.motion.pos.f[0] - group_anchor_moby->pos.x,
                                          hero.motion.pos.f[1] - group_anchor_moby->pos.y);
        path_step_distance = -racer_path_lead_distance_gp;
        *(int *)(od + 0x64) = hero.rail_segment;
        *(float *)(od + 0x68) = hero.rail_seg_frac;
        FUN_L00_0025d808_6570(path, o + 0x10, od + 0x64, od + 0x68, 0, path_step_distance);
        clear_u64_value(od + 0x90);
        *(int *)(od + 0xF8) = i * 2;
        *(int *)(od + 0xFC) = i * 2 + 1;
        if (*(int *)(od + 0xF0) != -1) {
            release_point_light_slot(*(int *)(od + 0xF0));
            *(int *)(od + 0xF0) = -1;
        }
    }
}

/* Racer, racing: measures how far along its stretch of track it is (sets the catch-up speed), snaps to the
 * track point nearest the hero and runs along the track, springing its position towards it, and faces along the
 * track. While its gun is active (states 2-4) it swings the gun joint to aim at the track ahead, blending in and
 * out over the aim timers, then fires a ray (3) or shows the muzzle flash and its light (2, 4). Last it turns
 * its head towards the hero. */
typedef int u128_67e8 __attribute__((mode(TI)));

typedef struct {
    char  pad00[0x64];
    int   seg;
    float frac;
} RacerSeg_67e8;

typedef struct {
    float x, y, z, w;
} V_67e8;

extern V_67e8 D_0013F3D0_67e8 __asm__("D_0013F3D0");

typedef struct {
    V_67e8        dir;   /* 0x00 */
    struct Moby  *self;  /* 0x10 */
    int           flags; /* 0x14 */
    unsigned char b18;
    unsigned char b19;
    short         oclass; /* 0x1A */
    float         f1C;
    int           i20;
    int           pad[3];
} Ray_67e8;

int D_L14_00161A3C = 0x8c;

#include "rnc/overlay/l14/mobys/racer_path_and_weapon_data/update_racer_path_and_weapon.h"

extern char D_L14_001809C0[];
extern char D_L14_0015F580_macro[] __asm__("D_L14_0015F580") MACRO_ADDR;
extern void track_67e8(void *, void *, void *, void *, int, float) __asm__("FUN_L00_0025d808");
extern void vendor_spring(void *, void *, float, float, float, float) __asm__("FUN_L00_0025b8c0");
extern void matrix_to_quaternion(void *, void *) __asm__("FUN_00214260");
extern void FUN_L00_001fff28(void *, int, float);
extern void register_beam_slot(struct Moby *, int, int) __asm__("FUN_L00_002e1f28");
extern int
set_beam_origin_transform_and_endpoint(struct Moby *, void *, void *) __asm__("FUN_L00_002e2190");
extern char *spawn_mode_controlled_rotating_alpha_fade_particle(
    void *, void *, int, struct Moby *) __asm__("FUN_L00_00274948");
extern int
allocate_colored_effect_slot(void *, float, float, float, float, float) __asm__("FUN_L00_0023e738");

void update_racer_path_and_weapon(struct Moby *racer_moby) __asm__("FUN_L14_002d67e8");

void update_racer_path_and_weapon(struct Moby *racer_moby) {
    char    *moby_data = (char *)racer_moby->pvars;
    char    *path = (char *)l14_splines[*(int *)(moby_data + 0x60)].path;
    V_67e8   near;
    V_67e8   joint_transform_and_aim_vectors[4];
    V_67e8   ma[4];
    V_67e8   mb[4];
    V_67e8   mc[4];
    V_67e8   md[4];
    V_67e8   scl;
    V_67e8   rot;
    V_67e8   one;
    Ray_67e8 ray;
    int      ti;
    float    tf;
    float    z;
    float    yaw;
    float    blend;
    float    target_yaw_offset;

    if (*(int *)(moby_data + 0x64) >= *(short *)(moby_data + 0x8E)) {
        float sum = 0.0f;
        int   k;
        for (k = *(short *)(moby_data + 0x8E); k < *(int *)(moby_data + 0x64); k++) {
            sum += *(float *)(path + k * 16 + 0x1C);
        }
        sum += *(float *)(moby_data + 0x68);
        sum /= *(float *)(moby_data + 0x78);
        if (sum > 1.0f) {
            sum = 1.0f;
        }
        *(float *)(moby_data + 0x74) =
            racer_path_lead_distance_gp + (-3.0f - racer_path_lead_distance_gp) * sum;
    }
    *(int *)(moby_data + 0x68) = 0;
    *(int *)(moby_data + 0x64) = 0;
    z = *(float *)(moby_data + 0x68);
    FUN_L00_0025df68(path,
                     &D_0013F3D0_67e8,
                     &near.x,
                     (int *)(moby_data + 0x64),
                     (float *)(moby_data + 0x68),
                     0,
                     20.0f,
                     5.0f,
                     z);
    track_67e8(path, &near, moby_data + 0x64, moby_data + 0x68, 0, -*(float *)(moby_data + 0x74));
    vendor_spring(&racer_moby->pos.x, moby_data + 0x90, near.x, 0.01f, 0.2f, z);
    vendor_spring(&racer_moby->pos.y, moby_data + 0x94, near.y, 0.01f, 0.2f, z);
    vendor_spring(&racer_moby->pos.z, moby_data + 0x98, near.z, 0.01f, 0.2f, z);
    {
        int   i = *(int *)(moby_data + 0x64);
        char *current_path_record = path + i * 16;
        char *lookahead_path_record = path + (i + 5) * 16;
        racer_moby->rot.z = fast_arc_tan(
            *(float *)(lookahead_path_record + 0x10) - *(float *)(current_path_record + 0x10),
            *(float *)(lookahead_path_record + 0x14) - *(float *)(current_path_record + 0x14));
    }
    if (racer_moby->command == 5 && fast_dec_timer(moby_data + 0x1C8) != 0) {
        racer_moby->command = 1;
    }
    if (racer_moby->command == 1) {
        *(int *)(moby_data + 0x1C0) = scale_game_frames(D_L14_00161A3C);
        *(int *)(moby_data + 0x1C4) = 0;
        racer_moby->command = 2;
        build_moby_bone_transform(racer_moby, 4, joint_transform_and_aim_vectors);
        build_moby_bone_transform(racer_moby, 5, ma);
        fast_vec_normalize(&ma[0], &ma[0], 1.0f);
        fast_vec_normalize(&ma[1], &ma[1], 1.0f);
        fast_vec_normalize(&ma[2], &ma[2], 1.0f);
        transpose_rotation_matrix(mb, ma);
        mb[3].w = 1.0f;
        mb[3].x = mb[3].y = mb[3].z = 0.0f;
        multiply_matrix(mc, mb, joint_transform_and_aim_vectors);
        matrix_to_quaternion(moby_data + 0x1D0, mc);
    }
    yaw = racer_moby->rot.z;
    if ((unsigned char)(racer_moby->command - 2) < 3) {
        float turn;
        float aim_timer_fraction;
        float k;
        build_moby_bone_transform(racer_moby, 0, moby_data + 0xA0);
        blend = 1.0f;
        aim_timer_fraction = convert_integer_to_float(*(int *)(moby_data + 0x1C0)) /
                             convert_integer_to_float(scale_game_frames(D_L14_00161A3C));
        k = weapon_aim_final_path_step;
        ti = ((RacerSeg_67e8 *)moby_data)->seg;
        tf = ((RacerSeg_67e8 *)moby_data)->frac;
        track_67e8(path,
                   &joint_transform_and_aim_vectors[0],
                   &ti,
                   &tf,
                   0,
                   k + (1.0f - k) * aim_timer_fraction);
        qcopy(moby_data + 0x1E0, &joint_transform_and_aim_vectors[0]);
        fast_vec_sub(&joint_transform_and_aim_vectors[1],
                     &joint_transform_and_aim_vectors[0],
                     &racer_moby->pos);
        yaw = fast_arc_tan(joint_transform_and_aim_vectors[1].x,
                           joint_transform_and_aim_vectors[1].y);
        turn = fast_subtract_rotations(yaw, racer_moby->rot.z);
        if (turn > 1.2217305f) {
            turn = 1.2217305f;
        } else if (turn < -1.2217305f) {
            turn = -1.2217305f;
        }
        (*(int *)(moby_data + 0x1C4))++;
        if (scale_game_frames(30) < *(int *)(moby_data + 0x1C4)) {
            if (racer_moby->command == 4) {
                racer_moby->command = 0;
            } else {
                unsigned char firing_command = 3;
                register_beam_slot(racer_moby, firing_command, 1);
                racer_moby->command = firing_command;
            }
            *(int *)(moby_data + 0x1C4) = scale_game_frames(30);
        }
        if (racer_moby->command == 2 || racer_moby->command == 4) {
            blend = convert_integer_to_float(*(int *)(moby_data + 0x1C4)) /
                    convert_integer_to_float(scale_game_frames(30));
            if (racer_moby->command == 4) {
                blend = 1.0f - blend;
            }
            turn = (turn - 0.0f) * blend + 0.0f;
        }
        FUN_L00_001fff28(moby_data + 0x150, 2, turn);
        moby_point_position(racer_moby, 4, &joint_transform_and_aim_vectors[2]);
        *(u128_67e8 *)&joint_transform_and_aim_vectors[3] = 0;
        joint_transform_and_aim_vectors[3].z = 1.0f;
        joint_transform_and_aim_vectors[3].w = 1.0f;
        fast_vec_normalize(&ma[1], &joint_transform_and_aim_vectors[1], 1.0f);
        fast_vec_cross(&ma[2], &ma[1], &joint_transform_and_aim_vectors[3]);
        fast_vec_normalize(&ma[2], &ma[2], 1.0f);
        fast_vec_cross(&ma[0], &ma[2], &ma[1]);
        ma[3].x = ma[3].y = ma[3].z = 0.0f;
        ma[3].w = 1.0f;
        build_moby_bone_transform(racer_moby, 5, mb);
        fast_vec_normalize(&mb[0], &mb[0], 1.0f);
        fast_vec_normalize(&mb[1], &mb[1], 1.0f);
        fast_vec_normalize(&mb[2], &mb[2], 1.0f);
        transpose_rotation_matrix(mc, mb);
        mc[3].x = mc[3].y = mc[3].z = 0.0f;
        mc[3].w = 1.0f;
        multiply_matrix(md, mc, ma);
        *(u128_67e8 *)&scl = 0;
        scl.w = 1.0f;
        scl.y = 512.0f / racer_moby->scale;
        matrix_to_quaternion(&rot, md);
        if (racer_moby->command == 2 || racer_moby->command == 4) {
            quaternion_nlerp(moby_data + 0x190, moby_data + 0x1D0, &rot, blend);
        } else {
            qcopy(moby_data + 0x190, &rot);
        }
        *(char *)(moby_data + 0x183) = 1;
        *(float *)(moby_data + 0x18C) = 1.0f;
        *(u128_67e8 *)&one = 0;
        one.x = 1.0f;
        one.y = 1.0f;
        one.z = 1.0f;
        one.w = 1.0f;
        qcopy(moby_data + 0x1A0, &one);
        qcopy(moby_data + 0x1B0, &scl);
        if (fast_dec_timer(moby_data + 0x1C0) != 0) {
            racer_moby->command = 4;
            *(int *)(moby_data + 0x1C4) = 0;
        }
        if (racer_moby->command == 3) {
            if (set_beam_origin_transform_and_endpoint(
                    racer_moby, moby_data + 0xA0, moby_data + 0x1E0) == 0) {
                register_beam_slot(racer_moby, 3, 1);
            }
            ray.flags = 0x10001;
            ray.self = racer_moby;
            ray.i20 = 1;
            ray.f1C = 1.0f;
            ray.dir.x = fast_cos(racer_moby->rot.z);
            ray.dir.y = fast_sin(racer_moby->rot.z);
            ray.dir.z = 1.0f;
            ray.dir.w = 5627.925f;
            ray.b19 = 1;
            ray.oclass = racer_moby->oclass;
            ray.b18 = 0;
            collision_line(moby_data + 0xD0, moby_data + 0x1E0, 9, racer_moby, &ray);
        } else if (racer_moby->command == 2 || racer_moby->command == 4) {
            char *muzzle_flash_particle;
            float muzzle_light_position_w;
            char *muzzle = moby_data + 0xD0;
            fast_vec_scale(&ray, moby_data + 0xA0, muzzle_flash_joint_axis_offset);
            fast_vec_add(&ray, &ray, muzzle);
            muzzle_flash_particle = spawn_mode_controlled_rotating_alpha_fade_particle(
                &ray, D_L14_0015F580_macro, 0x7F, racer_moby);
            if (muzzle_flash_particle != 0) {
                char *muzzle_flash_particle_state = muzzle_flash_particle + 0x20;
                int   lifetime_frames;
                *(float *)(muzzle_flash_particle + 0xC) =
                    (muzzle_flash_particle_full_blend_endpoint - 1000.0f) * blend + 1000.0f;
                *(short *)(muzzle_flash_particle_state + 0x14) =
                    truncate_float_to_s32(blend * 111.0f + 16.0f);
                *(short *)(muzzle_flash_particle_state + 0x16) = 1;
                lifetime_frames = scale_game_frames(4);
                *(short *)(muzzle_flash_particle + 0xA) = lifetime_frames;
                *(float *)(muzzle_flash_particle_state + 0x10) =
                    1.0f / convert_integer_to_float((short)lifetime_frames);
                *(int *)(muzzle_flash_particle_state + 0x18) = 0x7F7F7F;
            }
            qcopy(&ray.self, muzzle);
            ((float *)&ray.self)[2] -= 0.5f;
            muzzle_light_position_w = (muzzle_light_full_blend_position_w - 0.1f) * blend + 0.1f;
            if (*(int *)(moby_data + 0xF0) == -1) {
                *(int *)(moby_data + 0xF0) =
                    allocate_colored_effect_slot(&ray.self,
                                                 muzzle_light_position_w,
                                                 0.0f,
                                                 muzzle_light_color_component_x,
                                                 muzzle_light_color_component_y,
                                                 muzzle_light_color_component_z);
            }
            if (*(int *)(moby_data + 0xF0) >= 0) {
                float *muzzle_light_entry =
                    (float *)(D_L14_001809C0 + (*(int *)(moby_data + 0xF0) << 5));
                qcopy(muzzle_light_entry + 4, &ray.self);
                muzzle_light_entry[7] = muzzle_light_position_w;
                muzzle_light_entry[0] = muzzle_light_color_component_x;
                muzzle_light_entry[1] = muzzle_light_color_component_y;
                muzzle_light_entry[2] = muzzle_light_color_component_z;
            }
        } else if (*(int *)(moby_data + 0xF0) != -1) {
            release_point_light_slot(*(int *)(moby_data + 0xF0));
            *(int *)(moby_data + 0xF0) = -1;
        }
    } else if (*(int *)(moby_data + 0xF0) != -1) {
        release_point_light_slot(*(int *)(moby_data + 0xF0));
        *(int *)(moby_data + 0xF0) = -1;
    }
    fast_vec_sub(&joint_transform_and_aim_vectors[0], &D_0013F3D0_67e8, &racer_moby->pos);
    target_yaw_offset = fast_subtract_rotations(
        fast_arc_tan(joint_transform_and_aim_vectors[0].x, joint_transform_and_aim_vectors[0].y),
        yaw);
    if (target_yaw_offset > 1.2217305f) {
        target_yaw_offset = 1.2217305f;
    } else if (target_yaw_offset < -1.2217305f) {
        target_yaw_offset = -1.2217305f;
    }
    FUN_L00_001fff28(moby_data + 0x110, 2, target_yaw_offset);
}
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002D84A8), where it is exact; names translated to the US level program. */

extern void
sample_path(int a, char *pos, char *b, char *c, int d, float f) __asm__("FUN_L00_0025d808");

extern float D_L14_00161A30 __attribute__((sda));
void         advance_moby_along_path(struct Moby *moby) __asm__("FUN_L14_002d7198");

void advance_moby_along_path(struct Moby *moby) {
    char *path_tracking_state = (char *)moby->pvars;
    int   path_index = *(int *)(path_tracking_state + 0x60);
    sample_path((int)l14_splines[path_index].path,
                &moby->pos,
                path_tracking_state + 0x64,
                path_tracking_state + 0x68,
                0,
                D_L14_00161A30 * frame_time);
}

/* Draws the racer's two glow quads, at its lamp (0xE0) and its tail (0x200), turned to face the camera. */
typedef struct {
    float         m[4][4];
    u32           col[4];
    float         uv[8];
    unsigned long a, b, c, d;
} Pk;

float D_L14_001DE540[4][4] = {
    {0.0f, -1.0f,  1.0f, 1.0f},
    {0.0f, -1.0f, -1.0f, 1.0f},
    {0.0f,  1.0f,  1.0f, 1.0f},
    {0.0f,  1.0f, -1.0f, 1.0f},
};

extern void draw_geometry_quad(void *, int, int) __asm__("FUN_001f7d30");

void draw_racer_glow_quads(struct Moby *m) __asm__("FUN_L14_002d71f0");

void draw_racer_glow_quads(struct Moby *m) {
    char *d = (char *)m->pvars;
    Pk    pk;
    float glow_quad_transform[4][4];
    int   i;
    int   j;
    qcopy(glow_quad_transform[3], d + 0xE0);
    glow_quad_transform[3][3] = 1.0f;
    fast_vec_sub(glow_quad_transform[0], D_L14_001674C0, glow_quad_transform[3]);
    fast_vec_normalize(glow_quad_transform[0], glow_quad_transform[0], 1.0f);
    fast_vec_cross(glow_quad_transform[1], glow_quad_transform[0], &hero.gravity_dir);
    fast_vec_normalize(glow_quad_transform[1], glow_quad_transform[1], -1.0f);
    fast_vec_cross(glow_quad_transform[2], glow_quad_transform[1], glow_quad_transform[0]);
    pk.b = get_effect_texture(0xB);
    pk.d = 0x8000000048ULL;
    pk.c = 0x0000FF9000000260ULL;
    pk.a = 5;
    pk.col[3] = 0x80FFFFFF;
    pk.col[2] = 0x80FFFFFF;
    pk.col[1] = 0x80FFFFFF;
    pk.col[0] = 0x80FFFFFF;
    pk.uv[0] = 0.0f;
    pk.uv[1] = 0.0f;
    pk.uv[2] = 0.0f;
    pk.uv[3] = 1.0f;
    pk.uv[4] = 1.0f;
    pk.uv[5] = 0.0f;
    pk.uv[6] = 1.0f;
    pk.uv[7] = 1.0f;
    for (i = 0; i < 4; i++) {
        fast_vec_scale(pk.m[i], D_L14_001DE540[i], 0.4f);
        transform_vector(pk.m[i], pk.m[i], glow_quad_transform);
    }
    draw_geometry_quad(&pk, 0, 0);
    qcopy(glow_quad_transform[3], d + 0x200);
    fast_vec_sub(glow_quad_transform[0], D_L14_001674C0, glow_quad_transform[3]);
    fast_vec_normalize(glow_quad_transform[0], glow_quad_transform[0], 1.0f);
    fast_vec_cross(glow_quad_transform[1], glow_quad_transform[0], &hero.gravity_dir);
    fast_vec_normalize(glow_quad_transform[1], glow_quad_transform[1], -1.0f);
    fast_vec_cross(glow_quad_transform[2], glow_quad_transform[1], glow_quad_transform[0]);
    pk.col[3] = 0x202020FF;
    pk.col[2] = 0x202020FF;
    pk.col[1] = 0x202020FF;
    pk.col[0] = 0x202020FF;
    for (j = 0; j < 4; j++) {
        fast_vec_scale(pk.m[j], D_L14_001DE540[j], 0.2f);
        transform_vector(pk.m[j], pk.m[j], glow_quad_transform);
    }
    draw_geometry_quad(&pk, 0, 0);
}

/* Puffs smoke from the moby's exhaust joint: keeps its light in place while active, then spawns four puffs. */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002D87A0), where it is exact; names translated to the US level program. */

extern float D_L14_0015F580[] MACRO_ADDR;

#include "rnc/overlay/l14/mobys/racer_path_and_weapon_globals.h"

float       joint_light_position_w_gp __asm__("D_L14_00161A60") = 6.0f;
extern void random_spherical_offset(void *, float, float) __asm__("FUN_00213358");
extern void draw_racer_glow_quads(struct Moby *) __asm__("FUN_L14_002d71f0");

void update_moby_joint_visual_effects(struct Moby *moby) __asm__("FUN_L14_002d7490");

void update_moby_joint_visual_effects(struct Moby *moby) {
    char *moby_data = (char *)moby->pvars;
    float joint_position[4];
    float glow_position_offset[4];
    float random_particle_vector[4];
    char *particle;
    int   i;
    moby_point_position(moby, 1, joint_position);
    fast_vec_sub(glow_position_offset, joint_position, D_L14_001674C0);
    fast_vec_normalize(glow_position_offset, glow_position_offset, -0.5f);
    fast_vec_add(moby_data + 0xE0, joint_position, glow_position_offset);
    if ((unsigned int)(moby->state - 3) < 3) {
        joint_position[2] += 1.0f;
        if (*(int *)(moby_data + 0xF0) == -1) {
            *(int *)(moby_data + 0xF0) = allocate_colored_effect_slot(
                joint_position, joint_light_position_w_gp, 0.0f, 0.5f, 0.5f, 1.0f);
        }
        if (*(int *)(moby_data + 0xF0) >= 0) {
            char *e = D_L14_001809C0 + *(int *)(moby_data + 0xF0) * 32;
            qcopy(e + 0x10, joint_position);
            *(float *)(e + 0x1C) = joint_light_position_w_gp;
        }
        joint_position[2] -= 1.0f;
    }
    random_spherical_offset(random_particle_vector, 0.005f, 0.03f);
    particle = spawn_mode_controlled_rotating_alpha_fade_particle(
        joint_position, random_particle_vector, 0x7F, moby);
    if (particle != 0) {
        *(float *)(particle + 0xC) = random_float_between(6000.0f, 32000.0f);
    }
    for (i = 0; i < 3; i++) {
        char *e;
        int   lifetime_frames;
        particle = spawn_mode_controlled_rotating_alpha_fade_particle(
            joint_position, D_L14_0015F580, 0x7F, moby);
        if (particle == 0)
            continue;
        e = particle + 0x20;
        if (i == 2 && random_integer_below(8) == 0) {
            *(float *)(particle + 0xC) = 180000.0f;
        } else {
            *(float *)(particle + 0xC) = random_float_between(80000.0f, 120000.0f);
        }
        lifetime_frames = scale_game_frames(2);
        *(short *)(particle + 0xA) = lifetime_frames;
        *(float *)(e + 0x10) = 1.0f / convert_integer_to_float((short)lifetime_frames);
        *(int *)(e + 0x18) = 0x7F7F7F;
        *(short *)(e + 0x16) = 3;
    }
    moby_point_position(moby, 6, moby_data + 0x200);
    add_draw_callback(draw_racer_glow_quads, moby);
}
