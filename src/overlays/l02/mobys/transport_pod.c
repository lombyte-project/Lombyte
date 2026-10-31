/* Level 02 moby class transport_pod: its update, rail movement, and the camera mode setter it uses. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/game_globals.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/camera/scripted_camera.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/camera/follow_camera_set_up_spring.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"

extern float D_0015ED6C;
extern int   allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");

extern int D_L02_0015FFD8; /* no foreign declaration */

#include "sda.h"
#include "rnc/overlay/quad.h"

#include "rnc/overlay/cuboid.h"

extern char D_L02_001673C0[];

extern f32 advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("func_00213F38");

#include "qcopy.h"
#include "rnc/overlay/level_camera.h"

#include "rnc/game/usage_stats.h"

/* pvars of the class transport_pod_update updates (the lift: two rail mobys 0x421/0x422 ride with it). */
typedef struct {
    char  pad0[0x20];
    char  riders[0x40]; /* 0x20: passed to update_entity_matrix */
    int   node_a;       /* 0x60: l02_cuboids node of the lower stop */
    int   node_b;       /* 0x64: node of the upper stop */
    int   path;         /* 0x68: D_L02_001B0AB0 camera path */
    int   pad6C;
    int   gear[4];      /* 0x70: moby indices pushed along their axis in state 2 */
    int   timer;        /* 0x80 */
    char *rail_a;       /* 0x84: class 0x421 moby */
    char *rail_b;       /* 0x88: class 0x422 moby */
    float rail_a_speed; /* 0x8C */
    float rail_b_speed; /* 0x90 */
    float rail_a_off;   /* 0x94 */
    float rail_b_off;   /* 0x98 */
    float speed;        /* 0x9C */
    int   cam_goal;     /* 0xA0 */
    float cam_pos;      /* 0xA4 */
    float cam_speed;    /* 0xA8 */
    int   ride_timer;   /* 0xAC */
    float cam_accel;    /* 0xB0 */
    float cam_max;      /* 0xB4 */
} Varsdc88;

#include "rnc/overlay/l02/mobys/transport_pod_data/transport_pod_update.h"

extern float                   D_L02_00161C48 __attribute__((sda));
extern float                   D_L02_00161C4C __attribute__((sda));
extern float                   D_L02_00161C50 __attribute__((sda));
extern float                   D_L02_00161C54 __attribute__((sda));
extern float                   D_L02_00161C58 __attribute__((sda));
extern float                   D_L02_00161C60 __attribute__((sda));
extern float                   D_L02_00161C30_gp __asm__("D_L02_00161C30") __attribute__((sda));
extern float                   D_L02_00161C34_gp __asm__("D_L02_00161C34") __attribute__((sda));
extern int                    *D_L02_001B0AB0_dc88[] __asm__("D_L02_001B0AB0");
extern unsigned int D_00141BAC NOT_SDA;
extern float                   FUN_00213f38(float *, float *, float, float, float, float);
extern void sample_camera_path(void *, int, void *, void *, int, float) __asm__("FUN_00214e58");
extern void help_request(int, int) __asm__("FUN_L00_00203908");
extern void set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern void request_hero_target_pose(void *, int, float) __asm__("FUN_L00_00233ee8");
extern int  get_hero_target_pose_status() __asm__("FUN_L00_00233f38");
extern void draw_number(void *, void *, int, int, int, int, int, float) __asm__("FUN_L01_00285768");
extern void hero_set_state(int, int) __asm__("FUN_L02_0022b728");
extern void enter_hero_idle_or_stop(void) __asm__("FUN_L02_00237478");
void        advance_transport_pod_rails_to_offsets(struct Moby *moby) __asm__("FUN_L02_002def80");

extern void advance_transport_pod_rails_to_offsets(struct Moby *moby) __asm__("FUN_L02_002def80");
extern void snap_transport_pod_rails_to_offsets(struct Moby *moby) __asm__("FUN_L02_002df0b0");
void        set_active_camera_mode(int mode, int camera_argument_value) __asm__("FUN_L02_002f8a18");
extern void set_selected_object_float_triplets(float, float, float, float, float, float) __asm__("FUN_L02_002f8aa0");

extern int D_0015EEA4_gp __asm__("D_0015EEA4") __attribute__((sda));
/* Aridia lift: carries the hero between two stops, with its two rail mobys, gears and a camera path. */
void transport_pod_update(unsigned char *moby) __asm__("FUN_L02_002ddc88");

/* Moby class 713 */
void transport_pod_update(unsigned char *moby) {
    enum {
        TRANSPORT_POD_INITIALIZE = 0,
        TRANSPORT_POD_WAIT_FOR_GEAR_MOVEMENT_TRIGGER = 1,
        TRANSPORT_POD_TIMED_GEAR_MOVEMENT = 2,
        TRANSPORT_POD_RAISE_FOR_BOARDING = 3,
        TRANSPORT_POD_POSITION_RAILS_FOR_BOARDING = 4,
        TRANSPORT_POD_WAIT_FOR_RIDER_OR_RECALL = 5,
        TRANSPORT_POD_WAIT_FOR_RIDER_READY_FOR_DEPARTURE = 6,
        TRANSPORT_POD_POSITION_RAILS_FOR_DEPARTURE = 7,
        TRANSPORT_POD_LOWER_AND_TRANSFER_TO_OTHER_STOP = 8,
        TRANSPORT_POD_ADVANCE_RIDE_CAMERA = 9,
        TRANSPORT_POD_RAISE_FOR_DISEMBARKING = 10,
        TRANSPORT_POD_POSITION_RAILS_FOR_DISEMBARKING = 11,
        TRANSPORT_POD_RAISE_TO_STOP_HEIGHT = 12,
        TRANSPORT_POD_WAIT_FOR_RIDER_TO_LEAVE = 13,
        TRANSPORT_POD_CREATE_RAILS = 14,
        TRANSPORT_POD_POSITION_RAILS_FROM_CONFIGURED_OFFSETS = 15,
        TRANSPORT_POD_CYCLE_RAILS_TO_SPLIT_HEIGHTS = 16,
        TRANSPORT_POD_CYCLE_RAILS_TO_SHARED_LOWER_HEIGHT = 17
    };
    Varsdc88 *transport_pod_vars = ((Varsdc88 *)MOBY(moby)->pvars);
    int       i;
    int      *path;
    int       help_request_threshold_ticks;
    int       adjusted_help_ticks;
    float     s;
    OvlVec4   delta;
    OvlVec4   previous_rotation;
    OvlVec4   path_point_or_gear_step_or_stop_look_or_transfer_delta_or_camera_target;
    OvlVec4   camera_target_or_hero_position_or_rotation;
    OvlVec4   camera_position_or_stop_look;
    OvlVec4   initial_camera_rotation;

    fast_vec_scale(&delta, moby + 0x10, -1.0f);
    qcopy(&previous_rotation, moby + 0x40);
    if (transport_pod_path_markers_enabled_gp != 0.0f) {
        for (i = 0; i < *D_L02_001B0AB0_dc88[transport_pod_vars->path]; i++) {
            qcopy(&path_point_or_gear_step_or_stop_look_or_transfer_delta_or_camera_target,
                  (char *)D_L02_001B0AB0_dc88[transport_pod_vars->path] + i * 0x10 + 0x10);
            draw_number(&path_point_or_gear_step_or_stop_look_or_transfer_delta_or_camera_target,
                        moby,
                        4,
                        0x800000FF,
                        0x7F,
                        2,
                        0xFF,
                        0.1f);
        }
    }
    switch (moby[0x20]) {
    case TRANSPORT_POD_INITIALIZE:
        moby[0x30] = 0xFF;
        moby[0xBC] = 1;
        qcopy(moby + 0x10, &l02_cuboids[transport_pod_vars->node_b].pos);
        moby[0x20] = 1;
        ((struct Moby *)moby)->pos.z -= 6.0f;
        transport_pod_vars->rail_a = create_moby(0x421);
        *(short *)(transport_pod_vars->rail_a + 0x32) = 0x40;
        transport_pod_vars->rail_a[0x31] = 1;
        *(u64 *)(transport_pod_vars->rail_a + 0x38) = MOBY(moby)->spawn_frame;
        *(unsigned short *)(transport_pod_vars->rail_a + 0x34) = MOBY(moby)->flags;
        qcopy_nc(transport_pod_vars->rail_a + 0x10, moby + 0x10);
        qcopy_nc(transport_pod_vars->rail_a + 0x40, moby + 0x40);
        transport_pod_vars->rail_b = create_moby(0x422);
        *(short *)(transport_pod_vars->rail_b + 0x32) = 0x40;
        transport_pod_vars->rail_b[0x31] = 1;
        *(u64 *)(transport_pod_vars->rail_b + 0x38) = MOBY(moby)->spawn_frame;
        *(unsigned short *)(transport_pod_vars->rail_b + 0x34) = MOBY(moby)->flags;
        qcopy_nc(transport_pod_vars->rail_b + 0x10, moby + 0x10);
        qcopy_nc(transport_pod_vars->rail_b + 0x40, moby + 0x40);
        transport_pod_vars->rail_a_off = -1.0f;
        transport_pod_vars->rail_b_off = -1.0f;
        transport_pod_vars->cam_max =
            frame_time_sq * 0.1f * (float)*D_L02_001B0AB0_dc88[transport_pod_vars->path];
        transport_pod_vars->cam_accel =
            D_0015ED6C * 0.3f * (float)*D_L02_001B0AB0_dc88[transport_pod_vars->path];
        snap_transport_pod_rails_to_offsets((struct Moby *)moby);
        break;
    case TRANSPORT_POD_WAIT_FOR_GEAR_MOVEMENT_TRIGGER:
        if (visited52[moby[0xB0] + current_level_index * 16] == 0xFF) {
            transport_pod_vars->timer = scale_game_frames(30);
            moby[0x20] = 2;
        }
        break;
    case TRANSPORT_POD_TIMED_GEAR_MOVEMENT:
        s = D_0015ED6C * 5.0f;
        fast_vec_normalize(&path_point_or_gear_step_or_stop_look_or_transfer_delta_or_camera_target,
                           (char *)D_L02_0015FFD8 + (transport_pod_vars->gear[0] << 8) + 0xC0,
                           -s);
        fast_vec_add((char *)D_L02_0015FFD8 + (transport_pod_vars->gear[0] << 8) + 0x10,
                     (char *)D_L02_0015FFD8 + (transport_pod_vars->gear[0] << 8) + 0x10,
                     &path_point_or_gear_step_or_stop_look_or_transfer_delta_or_camera_target);
        fast_vec_normalize(&path_point_or_gear_step_or_stop_look_or_transfer_delta_or_camera_target,
                           (char *)D_L02_0015FFD8 + (transport_pod_vars->gear[1] << 8) + 0xC0,
                           s);
        fast_vec_add((char *)D_L02_0015FFD8 + (transport_pod_vars->gear[1] << 8) + 0x10,
                     (char *)D_L02_0015FFD8 + (transport_pod_vars->gear[1] << 8) + 0x10,
                     &path_point_or_gear_step_or_stop_look_or_transfer_delta_or_camera_target);
        fast_vec_normalize(&path_point_or_gear_step_or_stop_look_or_transfer_delta_or_camera_target,
                           (char *)D_L02_0015FFD8 + (transport_pod_vars->gear[2] << 8) + 0xC0,
                           -s);
        fast_vec_add((char *)D_L02_0015FFD8 + (transport_pod_vars->gear[2] << 8) + 0x10,
                     (char *)D_L02_0015FFD8 + (transport_pod_vars->gear[2] << 8) + 0x10,
                     &path_point_or_gear_step_or_stop_look_or_transfer_delta_or_camera_target);
        fast_vec_normalize(&path_point_or_gear_step_or_stop_look_or_transfer_delta_or_camera_target,
                           (char *)D_L02_0015FFD8 + (transport_pod_vars->gear[3] << 8) + 0xC0,
                           s);
        fast_vec_add((char *)D_L02_0015FFD8 + (transport_pod_vars->gear[3] << 8) + 0x10,
                     (char *)D_L02_0015FFD8 + (transport_pod_vars->gear[3] << 8) + 0x10,
                     &path_point_or_gear_step_or_stop_look_or_transfer_delta_or_camera_target);
        update_entity((char *)D_L02_0015FFD8 + (transport_pod_vars->gear[0] << 8));
        update_entity((char *)D_L02_0015FFD8 + (transport_pod_vars->gear[1] << 8));
        update_entity((char *)D_L02_0015FFD8 + (transport_pod_vars->gear[2] << 8));
        update_entity((char *)D_L02_0015FFD8 + (transport_pod_vars->gear[3] << 8));
        if (fast_dec_timer(&transport_pod_vars->timer)) {
            moby[0x20] = 3;
            allocate_voice_for_target_entry(1, 0, moby);
        }
        break;
    case TRANSPORT_POD_RAISE_FOR_BOARDING:
        FUN_00213f38((float *)&MOBY(moby)->pos.z,
                     &transport_pod_vars->speed,
                     l02_cuboids[transport_pod_vars->node_a].pos.z,
                     D_L02_00161C34_gp * frame_time_sq,
                     D_L02_00161C34_gp * frame_time_sq,
                     D_L02_00161C30_gp * D_0015ED6C);
        snap_transport_pod_rails_to_offsets((struct Moby *)moby);
        if (l02_cuboids[transport_pod_vars->node_a].pos.z - 0.0001f <=
            ((struct Moby *)moby)->pos.z) {
            moby[0x20] = 4;
            allocate_voice_for_target_entry(0, 0, moby);
        }
        break;
    case TRANSPORT_POD_POSITION_RAILS_FOR_BOARDING:
        transport_pod_vars->rail_a_off = 2.0f;
        transport_pod_vars->rail_b_off = -4.0f;
        advance_transport_pod_rails_to_offsets((struct Moby *)moby);
        if (absolute_float(*(float *)(transport_pod_vars->rail_a + 0x18) -
                           ((struct Moby *)moby)->pos.z - 2.0f) < 0.0001f &&
            absolute_float(*(float *)(transport_pod_vars->rail_b + 0x18) -
                           ((struct Moby *)moby)->pos.z + 4.0f) < 0.0001f) {
            moby[0x20] = 5;
        }
        break;
    case TRANSPORT_POD_WAIT_FOR_RIDER_OR_RECALL:
        if (moby[0xBC] == 0 &&
            vector_distance_xy(&hero.motion.pos, &l02_cuboids[transport_pod_vars->node_b].pos) <
                16.0f) {
            qcopy(moby + 0x10, &l02_cuboids[transport_pod_vars->node_b].pos);
            ((struct Moby *)moby)->pos.z -= 6.0f;
            transport_pod_vars->rail_a_off = -1.0f;
            transport_pod_vars->rail_b_off = -1.0f;
            snap_transport_pod_rails_to_offsets((struct Moby *)moby);
            moby[0x20] = 3;
            moby[0xBC] = 1;
            allocate_voice_for_target_entry(1, 0, moby);
        } else if (moby[0xBC] == 1 &&
                   vector_distance_xy(&hero.motion.pos,
                                      &l02_cuboids[transport_pod_vars->node_a].pos) < 16.0f) {
            qcopy(moby + 0x10, &l02_cuboids[transport_pod_vars->node_a].pos);
            ((struct Moby *)moby)->pos.z -= 6.0f;
            transport_pod_vars->rail_a_off = -1.0f;
            transport_pod_vars->rail_b_off = -1.0f;
            snap_transport_pod_rails_to_offsets((struct Moby *)moby);
            moby[0xBC] = 0;
            moby[0x20] = 3;
            allocate_voice_for_target_entry(1, 0, moby);
        } else if ((unsigned char *)hero.ground_moby == (struct Moby *)moby &&
                   hero.air_frames.s == 0 && hero.vehicle_mode == 0) {
            qcopy(&camera_target_or_hero_position_or_rotation, moby + 0x10);
            camera_target_or_hero_position_or_rotation.f[2] += 2.0f;
            if (moby[0xBC] == 0) {
                transport_pod_vars->cam_goal = 0;
                transport_pod_vars->cam_pos =
                    (float)(*D_L02_001B0AB0_dc88[transport_pod_vars->path] - 1);
                qcopy_nc(&path_point_or_gear_step_or_stop_look_or_transfer_delta_or_camera_target,
                         &l02_cuboids[transport_pod_vars->node_a].rot);
                qcopy(&camera_position_or_stop_look,
                      (char *)D_L02_001B0AB0_dc88[transport_pod_vars->path] +
                          truncate_float_to_s32(transport_pod_vars->cam_pos) * 0x10 + 0x10);
                clear_u64_value(&initial_camera_rotation);
                initial_camera_rotation.f[2] =
                    fast_arc_tan(camera_target_or_hero_position_or_rotation.f[0] -
                                     camera_position_or_stop_look.f[0],
                                 camera_target_or_hero_position_or_rotation.f[1] -
                                     camera_position_or_stop_look.f[1]);
                initial_camera_rotation.f[1] =
                    -fast_arc_tan(vector_distance_xy(&camera_position_or_stop_look,
                                                     &camera_target_or_hero_position_or_rotation),
                                  camera_target_or_hero_position_or_rotation.f[2] -
                                      camera_position_or_stop_look.f[2]);
            } else {
                transport_pod_vars->cam_goal = *D_L02_001B0AB0_dc88[transport_pod_vars->path] - 1;
                transport_pod_vars->cam_pos = 0.0f;
                qcopy_nc(&path_point_or_gear_step_or_stop_look_or_transfer_delta_or_camera_target,
                         &l02_cuboids[transport_pod_vars->node_b].rot);
                qcopy(&camera_position_or_stop_look,
                      (char *)D_L02_001B0AB0_dc88[transport_pod_vars->path] +
                          truncate_float_to_s32(transport_pod_vars->cam_pos) * 0x10 + 0x10);
                clear_u64_value(&initial_camera_rotation);
                initial_camera_rotation.f[2] =
                    fast_arc_tan(camera_target_or_hero_position_or_rotation.f[0] -
                                     camera_position_or_stop_look.f[0],
                                 camera_target_or_hero_position_or_rotation.f[1] -
                                     camera_position_or_stop_look.f[1]);
                initial_camera_rotation.f[1] =
                    -fast_arc_tan(vector_distance_xy(&camera_position_or_stop_look,
                                                     &camera_target_or_hero_position_or_rotation),
                                  camera_target_or_hero_position_or_rotation.f[2] -
                                      camera_position_or_stop_look.f[2]);
            }
            moby[0x20] = 6;
            transport_pod_vars->ride_timer = scale_game_frames(0xB4);
            request_hero_target_pose(
                moby + 0x10,
                0,
                path_point_or_gear_step_or_stop_look_or_transfer_delta_or_camera_target.f[2]);
            jet_mount(D_L02_001673C0, D_L02_001673C0 + 0x10, 3, scale_game_frames(0xF0), 0);
            set_selected_object_float_triplets(D_L02_00161C48,
                                               D_L02_00161C4C,
                                               D_L02_00161C50,
                                               D_L02_00161C54,
                                               D_L02_00161C58,
                                               D_L02_00161C60);
            set_camera_position(&camera_position_or_stop_look);
            set_camera_rotation(&initial_camera_rotation);
        }
        break;
    case TRANSPORT_POD_WAIT_FOR_RIDER_READY_FOR_DEPARTURE: {
        int rider_readiness_status = get_hero_target_pose_status();
        if (rider_readiness_status == 2) {
            moby[0x20] = 7;
            allocate_voice_for_target_entry(2, 0, moby);
            hero_set_state(0x72, 0);
        } else if (rider_readiness_status == 0) {
            jet_end_mount(2);
            moby[0x20] = 0xD;
        } else if (fast_dec_timer(&transport_pod_vars->ride_timer)) {
            jet_end_mount(2);
            moby[0x20] = 0xD;
            enter_hero_idle_or_stop();
        }
        break;
    }
    case TRANSPORT_POD_POSITION_RAILS_FOR_DEPARTURE:
        transport_pod_vars->rail_a_off = -1.0f;
        transport_pod_vars->rail_b_off = -1.0f;
        advance_transport_pod_rails_to_offsets((struct Moby *)moby);
        if (absolute_float(*(float *)(transport_pod_vars->rail_a + 0x18) -
                           ((struct Moby *)moby)->pos.z + 1.0f) < 0.01f &&
            absolute_float(*(float *)(transport_pod_vars->rail_b + 0x18) -
                           ((struct Moby *)moby)->pos.z + 1.0f) < 0.01f) {
            moby[0x20] = 8;
        }
        break;
    case TRANSPORT_POD_LOWER_AND_TRANSFER_TO_OTHER_STOP:
        FUN_00213f38((float *)&MOBY(moby)->pos.z,
                     &transport_pod_vars->speed,
                     l02_cuboids[transport_pod_vars->node_a].pos.z - 6.0f,
                     D_L02_00161C34_gp * frame_time_sq,
                     D_L02_00161C34_gp * frame_time_sq,
                     D_L02_00161C30_gp * D_0015ED6C);
        snap_transport_pod_rails_to_offsets((struct Moby *)moby);
        if (((struct Moby *)moby)->pos.z < l02_cuboids[transport_pod_vars->node_a].pos.z - 5.999f) {
            if (moby[0xBC] == 0) {
                moby[0xBC] = 1;
                qcopy_nc(moby + 0x10, &l02_cuboids[transport_pod_vars->node_b].pos);
                qcopy_nc(&camera_position_or_stop_look,
                         &l02_cuboids[transport_pod_vars->node_b].rot);
                fast_vec_sub(
                    &path_point_or_gear_step_or_stop_look_or_transfer_delta_or_camera_target,
                    &l02_cuboids[transport_pod_vars->node_b].pos,
                    &l02_cuboids[transport_pod_vars->node_a].pos);
            } else {
                moby[0xBC] = 0;
                qcopy_nc(moby + 0x10, &l02_cuboids[transport_pod_vars->node_a].pos);
                qcopy_nc(&camera_position_or_stop_look,
                         &l02_cuboids[transport_pod_vars->node_a].rot);
                fast_vec_sub(
                    &path_point_or_gear_step_or_stop_look_or_transfer_delta_or_camera_target,
                    &l02_cuboids[transport_pod_vars->node_a].pos,
                    &l02_cuboids[transport_pod_vars->node_b].pos);
            }
            ((struct Moby *)moby)->pos.z -= 6.0f;
            fast_vec_add(&camera_target_or_hero_position_or_rotation,
                         &hero.motion.pos,
                         &path_point_or_gear_step_or_stop_look_or_transfer_delta_or_camera_target);
            set_race_camera(&camera_target_or_hero_position_or_rotation,
                            &camera_position_or_stop_look,
                            0x72,
                            0);
            transport_pod_vars->speed = -transport_pod_vars->speed;
            moby[0x20] = 9;
            set_active_camera_mode(2, scale_game_frames(0xB4));
        }
        break;
    case TRANSPORT_POD_ADVANCE_RIDE_CAMERA:
        FUN_00213f38(&transport_pod_vars->cam_pos,
                     &transport_pod_vars->cam_speed,
                     (float)transport_pod_vars->cam_goal,
                     transport_pod_vars->cam_max,
                     transport_pod_vars->cam_max,
                     transport_pod_vars->cam_accel);
        if (transport_pod_vars->cam_goal != 0
                ? (float)transport_pod_vars->cam_goal <= transport_pod_vars->cam_pos
                : transport_pod_vars->cam_pos <= (float)transport_pod_vars->cam_goal) {
            moby[0x20] = 10;
        }
        sample_camera_path(D_L02_001B0AB0_dc88[transport_pod_vars->path],
                           0,
                           &camera_position_or_stop_look,
                           &camera_target_or_hero_position_or_rotation,
                           0,
                           transport_pod_vars->cam_pos);
        qcopy(&path_point_or_gear_step_or_stop_look_or_transfer_delta_or_camera_target,
              moby + 0x10);
        path_point_or_gear_step_or_stop_look_or_transfer_delta_or_camera_target.f[2] += 8.0f;
        clear_u64_value(&camera_target_or_hero_position_or_rotation);
        camera_target_or_hero_position_or_rotation.f[2] = fast_arc_tan(
            path_point_or_gear_step_or_stop_look_or_transfer_delta_or_camera_target.f[0] -
                camera_position_or_stop_look.f[0],
            path_point_or_gear_step_or_stop_look_or_transfer_delta_or_camera_target.f[1] -
                camera_position_or_stop_look.f[1]);
        camera_target_or_hero_position_or_rotation.f[1] = -fast_arc_tan(
            vector_distance_xy(
                &camera_position_or_stop_look,
                &path_point_or_gear_step_or_stop_look_or_transfer_delta_or_camera_target),
            path_point_or_gear_step_or_stop_look_or_transfer_delta_or_camera_target.f[2] -
                camera_position_or_stop_look.f[2]);
        set_camera_position(&camera_position_or_stop_look);
        set_camera_rotation(&camera_target_or_hero_position_or_rotation);
        break;
    case TRANSPORT_POD_RAISE_FOR_DISEMBARKING:
        FUN_00213f38((float *)&MOBY(moby)->pos.z,
                     &transport_pod_vars->speed,
                     l02_cuboids[transport_pod_vars->node_a].pos.z,
                     D_L02_00161C34_gp * frame_time_sq,
                     D_L02_00161C34_gp * frame_time_sq,
                     D_L02_00161C30_gp * D_0015ED6C);
        snap_transport_pod_rails_to_offsets((struct Moby *)moby);
        if (l02_cuboids[transport_pod_vars->node_a].pos.z <= ((struct Moby *)moby)->pos.z) {
            moby[0x20] = 0xB;
            allocate_voice_for_target_entry(0, 0, moby);
        }
        break;
    case TRANSPORT_POD_POSITION_RAILS_FOR_DISEMBARKING:
        transport_pod_vars->rail_a_off = 2.0f;
        transport_pod_vars->rail_b_off = -4.0f;
        advance_transport_pod_rails_to_offsets((struct Moby *)moby);
        if (absolute_float(*(float *)(transport_pod_vars->rail_a + 0x18) -
                           ((struct Moby *)moby)->pos.z - 2.0f) < 0.01f &&
            absolute_float(*(float *)(transport_pod_vars->rail_b + 0x18) -
                           ((struct Moby *)moby)->pos.z + 4.0f) < 0.01f) {
            {
                adjusted_help_ticks =
                    scale_game_frames(D_0015EEA4_gp) - usage_stats.stat[9].unk2 * 600;
            }
            help_request_threshold_ticks = scale_game_frames(3600) * 60.0f;
            if (help_request_threshold_ticks < adjusted_help_ticks) {
                extern int current_level_index_gp __asm__("D_0015ED84") __attribute__((sda));
                if (!(D_00141BAC & (1 << current_level_index_gp))) {
                    help_request(0x7DD, 0x48);
                }
            }
            moby[0x20] = 0xD;
            jet_end_mount(2);
            hero_set_state(0, 1);
        }
        break;
    case TRANSPORT_POD_RAISE_TO_STOP_HEIGHT:
        ((struct Moby *)moby)->pos.z += D_0015ED6C * 4.0f;
        if (l02_cuboids[transport_pod_vars->node_a].pos.z <= ((struct Moby *)moby)->pos.z) {
            moby[0x20] = 5;
        }
        break;
    case TRANSPORT_POD_WAIT_FOR_RIDER_TO_LEAVE:
        if ((unsigned char *)hero.ground_moby != (struct Moby *)moby) {
            moby[0x20] = 5;
        }
        break;
    case TRANSPORT_POD_CREATE_RAILS:
        transport_pod_vars->rail_a = create_moby(0x421);
        *(short *)(transport_pod_vars->rail_a + 0x32) = 0x40;
        transport_pod_vars->rail_a[0x31] = 1;
        *(u64 *)(transport_pod_vars->rail_a + 0x38) = MOBY(moby)->spawn_frame;
        *(unsigned short *)(transport_pod_vars->rail_a + 0x34) = MOBY(moby)->flags;
        qcopy_nc(transport_pod_vars->rail_a + 0x10, moby + 0x10);
        qcopy_nc(transport_pod_vars->rail_a + 0x40, moby + 0x40);
        transport_pod_vars->rail_b = create_moby(0x422);
        *(short *)(transport_pod_vars->rail_b + 0x32) = 0x40;
        transport_pod_vars->rail_b[0x31] = 1;
        *(u64 *)(transport_pod_vars->rail_b + 0x38) = MOBY(moby)->spawn_frame;
        *(unsigned short *)(transport_pod_vars->rail_b + 0x34) = MOBY(moby)->flags;
        qcopy_nc(transport_pod_vars->rail_b + 0x10, moby + 0x10);
        qcopy_nc(transport_pod_vars->rail_b + 0x40, moby + 0x40);
        moby[0x20] = 0xF;
        break;
    case TRANSPORT_POD_POSITION_RAILS_FROM_CONFIGURED_OFFSETS:
        transport_pod_vars->rail_a_off = transport_pod_configured_rail_a_height_offset_gp;
        transport_pod_vars->rail_b_off = transport_pod_configured_rail_b_height_offset_gp;
        advance_transport_pod_rails_to_offsets((struct Moby *)moby);
        if (transport_pod_rail_cycle_enabled_gp != 0.0f) {
            moby[0x20] = 0x10;
        }
        break;
    case TRANSPORT_POD_CYCLE_RAILS_TO_SPLIT_HEIGHTS:
        transport_pod_vars->rail_a_off = 2.0f;
        transport_pod_vars->rail_b_off = -4.0f;
        advance_transport_pod_rails_to_offsets((struct Moby *)moby);
        if (transport_pod_rail_cycle_enabled_gp == 0.0f) {
            moby[0x20] = 0xF;
        } else if (absolute_float(*(float *)(transport_pod_vars->rail_a + 0x18) -
                                  ((struct Moby *)moby)->pos.z - 2.0f) < 0.01f &&
                   absolute_float(*(float *)(transport_pod_vars->rail_b + 0x18) -
                                  ((struct Moby *)moby)->pos.z + 4.0f) < 0.01f) {
            moby[0x20] = 0x11;
        }
        break;
    case TRANSPORT_POD_CYCLE_RAILS_TO_SHARED_LOWER_HEIGHT:
        transport_pod_vars->rail_a_off = -1.0f;
        transport_pod_vars->rail_b_off = -1.0f;
        advance_transport_pod_rails_to_offsets((struct Moby *)moby);
        if (transport_pod_rail_cycle_enabled_gp == 0.0f) {
            moby[0x20] = 0xF;
        } else if (absolute_float(*(float *)(transport_pod_vars->rail_a + 0x18) -
                                  ((struct Moby *)moby)->pos.z + 1.0f) < 0.01f &&
                   absolute_float(*(float *)(transport_pod_vars->rail_b + 0x18) -
                                  ((struct Moby *)moby)->pos.z + 1.0f) < 0.01f) {
            moby[0x20] = 0x10;
        }
        break;
    }
    fast_vec_add(&delta, &delta, moby + 0x10);
    if (fast_vec_length(&delta) > 4.0f) {
        clear_u64_value(&delta);
    }
    update_entity_matrix(transport_pod_vars->riders, &delta, &previous_rotation, moby + 0x40);
}
/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002E03B8), where it is exact; names translated to the US level program. */

extern f32 D_L02_00161C30_gp __asm__("D_L02_00161C30") __attribute__((sda));
extern f32 D_L02_00161C34_gp __asm__("D_L02_00161C34") __attribute__((sda));

extern short transport_pod_rail_vertical_deceleration_gp __asm__("D_L02_00161C38")
    __attribute__((sda));
extern short transport_pod_rail_rotation_degrees_per_height_unit_gp __asm__("D_L02_00161C3C")
    __attribute__((sda));

void advance_transport_pod_rails_to_offsets(struct Moby *moby) {
    char *transport_pod_vars = (char *)moby->pvars;
    advance_accelerated_scalar((float *)(*(char **)(transport_pod_vars + 0x84) + 0x18),
                               (float *)(transport_pod_vars + 0x8C),
                               moby->pos.z + *(float *)(transport_pod_vars + 0x94),
                               *(float *)&D_L02_00161C34_gp * frame_time_sq,
                               *(float *)&transport_pod_rail_vertical_deceleration_gp *
                                   frame_time_sq,
                               *(float *)&D_L02_00161C30_gp * D_0015ED6C);
    advance_accelerated_scalar((float *)(*(char **)(transport_pod_vars + 0x88) + 0x18),
                               (float *)(transport_pod_vars + 0x90),
                               moby->pos.z + *(float *)(transport_pod_vars + 0x98),
                               *(float *)&D_L02_00161C34_gp * frame_time_sq,
                               *(float *)&transport_pod_rail_vertical_deceleration_gp *
                                   frame_time_sq,
                               *(float *)&D_L02_00161C30_gp * D_0015ED6C);
    {
        float r0 = fast_add_rotations(
            moby->rot.z,
            (*(float *)(*(char **)(transport_pod_vars + 0x84) + 0x18) - moby->pos.z) *
                *(float *)&transport_pod_rail_rotation_degrees_per_height_unit_gp * DEG_TO_RAD);
        float k = *(float *)&transport_pod_rail_rotation_degrees_per_height_unit_gp;
        *(float *)(*(char **)(transport_pod_vars + 0x84) + 0x48) = r0;
        *(float *)(*(char **)(transport_pod_vars + 0x88) + 0x48) = fast_add_rotations(
            moby->rot.z,
            (*(float *)(*(char **)(transport_pod_vars + 0x88) + 0x18) - moby->pos.z) * k *
                DEG_TO_RAD);
    }
    update_entity(*(void **)(transport_pod_vars + 0x84));
    update_entity(*(void **)(transport_pod_vars + 0x88));
}
/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002E04E8), where it is exact; names translated to the US level program. */

void snap_transport_pod_rails_to_offsets(struct Moby *moby) __asm__("FUN_L02_002df0b0");

void snap_transport_pod_rails_to_offsets(struct Moby *moby) {
    char *transport_pod_vars = (char *)moby->pvars;
    char *rail_a = *(char **)(transport_pod_vars + 0x84);
    float k, k2;
    float rail_a_rotation_z;
    qcopy(rail_a + 0x10, &moby->pos);
    qcopy(*(char **)(transport_pod_vars + 0x88) + 0x10, &moby->pos);
    k = *(float *)&transport_pod_rail_rotation_degrees_per_height_unit_gp;
    *(float *)(rail_a + 0x18) = moby->pos.z + *(float *)(transport_pod_vars + 0x94);
    *(int *)(transport_pod_vars + 0x8C) = 0;
    *(float *)(*(char **)(transport_pod_vars + 0x88) + 0x18) =
        moby->pos.z + *(float *)(transport_pod_vars + 0x98);
    *(int *)(transport_pod_vars + 0x90) = 0;
    rail_a_rotation_z = fast_add_rotations(
        moby->rot.z,
        (*(float *)(*(char **)(transport_pod_vars + 0x84) + 0x18) - moby->pos.z) * k * DEG_TO_RAD);
    k2 = *(float *)&transport_pod_rail_rotation_degrees_per_height_unit_gp;
    *(float *)(*(char **)(transport_pod_vars + 0x84) + 0x48) = rail_a_rotation_z;
    *(float *)(*(char **)(transport_pod_vars + 0x88) + 0x48) = fast_add_rotations(
        moby->rot.z,
        (*(float *)(*(char **)(transport_pod_vars + 0x88) + 0x18) - moby->pos.z) * k2 * DEG_TO_RAD);
    update_entity(*(char **)(transport_pod_vars + 0x84));
    update_entity(*(char **)(transport_pod_vars + 0x88));
}

extern void
compute_spherical_offset_from_positions(void *, void *, void *) __asm__("FUN_L00_001ed630");

/* Sets the mode byte of the active camera moby (the one the camera system keeps at 0x180); mode 3 also aims it from the hero's position. */
void set_active_camera_mode(int mode, int camera_argument_value) {
    enum {
        CAMERA_MODE_SET_ARGUMENT_PAIR = 2,
        CAMERA_MODE_CAPTURE_HERO_POSITION = 3
    };
    char *camera_context = (char *)&l02_camera;
    char *active_camera = *(char **)(camera_context + 0x180);
    char *camera_data = *(char **)(active_camera + 0x70);
    char *camera_argument_pair = camera_data + 0xE0;

    active_camera[0x88] = mode;
    switch (mode) {
    case CAMERA_MODE_CAPTURE_HERO_POSITION:
        qcopy(camera_data + 0x110, &hero.motion.pos);
        compute_spherical_offset_from_positions(
            camera_data + 0xFC, camera_context + 0x140, &hero.motion.pos);
    case CAMERA_MODE_SET_ARGUMENT_PAIR:
        *(int *)(camera_argument_pair + 4) = camera_argument_value;
        *(int *)(camera_data + 0xE0) = camera_argument_value;
        break;
    }
}
