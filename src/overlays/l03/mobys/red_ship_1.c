/* Level 03 moby class red_ship_1 with its spline, emitter and wreck helpers. */
#include "types.h"
#include "qzero.h"
#include "rnc/mobys/moby_class_id.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/audio/voices/allocate_voice_for_bank_entry.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "qcopy.h"
#include "rnc/overlay/quad.h"
#include "rnc/overlay/level_path.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/game/item_state.h"

/* Ported from rac1-decomp (src/overlays/l03_kerwan/vendor_00293720.c: func_L03_0029DB88), where it is exact; names translated to the US level program. */

extern int D_L03_0015F5CC; /* no foreign declaration */

/* Builds two scaled offset vectors from the moby's matrix and adds them to out. */

void add_moby_rotation_based_offsets(struct Moby *moby, float *out) __asm__("FUN_L03_0029c8a8");

void add_moby_rotation_based_offsets(struct Moby *moby, float *out) {
    float unit_x_axis[4];
    float transformed_x_axis[4];
    float first_offset[4];
    float second_offset[4];
    float z_shifted_x_axis[4];
    float rotation_matrix[16];
    char *data;
    *(OvlQuad *)unit_x_axis = 0;
    unit_x_axis[0] = 1.0f;
    data = (char *)moby->pvars;
    if (*(float *)(data + 0x140) != 0.0f || *(float *)(data + 0x144) != 0.0f) {
        euler_rotation_matrix(rotation_matrix, (float *)&moby->rot);
        transform_vector(transformed_x_axis, unit_x_axis, rotation_matrix);
        *(OvlQuad *)z_shifted_x_axis = *(OvlQuad *)transformed_x_axis;
        z_shifted_x_axis[2] = z_shifted_x_axis[2] - 1.0f;
        fast_vec_cross(first_offset, transformed_x_axis, z_shifted_x_axis);
        fast_vec_cross(second_offset, transformed_x_axis, first_offset);
        fast_vec_scale(first_offset, first_offset, *(float *)(data + 0x140));
        fast_vec_scale(second_offset, second_offset, *(float *)(data + 0x144));
        fast_vec_add(out, out, first_offset);
        fast_vec_add(out, out, second_offset);
    }
}

void maintain_emitter_sound_voice(struct Moby *moby) __asm__("FUN_L03_0029c9d0");

void maintain_emitter_sound_voice(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    if (moby->state == 0) {
        *(int *)(data + 0x150) = -1;
        return;
    }
    if ((D_L03_0015F5CC & 7) != (((int)moby >> 8) & 7))
        return;
    if (*(short *)(data + 0x14A) != 0)
        return;
    if (*(int *)(data + 0x150) >= 0) {
        if (voice_is_playing(moby, *(int *)(data + 0x150)) != 0)
            return;
    }
    switch (moby->oclass) {
    case MOBY_CLASS_JOINT_ZERO_TEN_PARTICLE_EMITTER:
    case MOBY_CLASS_GRAY_JOINT_ZERO_TEN_PARTICLE_EMITTER:
        *(int *)(data + 0x150) = allocate_voice_for_target_entry(0, 4, (int)moby);
        break;
    case MOBY_CLASS_ALTERNATE_COLOR_CHILD_EFFECT_EMITTER:
    case MOBY_CLASS_TEN_PARTICLE_BURST_EMITTER:
    case MOBY_CLASS_TWO_JOINT_PARTICLE_EMITTER:
    case MOBY_CLASS_FIFTEEN_PARTICLE_BURST_EMITTER:
    case MOBY_CLASS_DEFAULT_COLOR_CHILD_EFFECT_EMITTER:
        *(int *)(data + 0x150) = allocate_voice_for_target_entry(1, 4, (int)moby);
        break;
    }
}

extern char D_L03_00166EC0[];

extern int  FUN_L00_001fefc8(void *);
extern void spawn_effect_particle_with_target_scalar(void *, void *, int, int, int, float) __asm__("FUN_L00_0026ced0");
char *spawn_emitter_child_effect(struct Moby *parent,
                                 int          unused,
                                 int          color,
                                 int          secondary_color,
                                 int          timer_frames,
                                 float        f,
                                 float        scale) __asm__("FUN_L03_002bad40");

/* Hit effects for the Kerwan train and turret classes: sparks or smoke puffs pushed along the hit velocity,
 * with countdowns in the moby data (0x12E, 0x12F) limiting how often they spawn. */
void update_moby_emitter_effects(char *moby, void *vel_in, int quiet) __asm__("FUN_L03_0029caa8");

void update_moby_emitter_effects(char *moby, void *vel_in, int quiet) {
    float  emitter_motion_delta[4];
    float *vp;
    float  joint_world_position[4];
    float  particle_velocity[4];
    char  *data;
    int    i;
    int    j;
    int    k;
    float  azimuth_angle;
    float  elevation_angle;
    int    color;
    int    color2;

    data = ((char *)MOBY(moby)->pvars);
    vp = emitter_motion_delta;
    *(OvlQuad *)vp = *(OvlQuad *)vel_in;
    if (MOBY(moby)->flags & 1) {
        return;
    }
    switch (MOBY(moby)->oclass) {
    case MOBY_CLASS_TWO_JOINT_PARTICLE_EMITTER:
        if (quiet == 0) {
            fast_vec_scale(vp, vp, 0.35f);
            for (i = 0; i < 2; i++) {
                moby_point_position(moby, i, joint_world_position);
                azimuth_angle = random_angle_radians();
                elevation_angle = random_angle_radians();
                build_spherical_offset(
                    particle_velocity, frame_time * 0.15f, azimuth_angle, elevation_angle);
                fast_vec_add(particle_velocity, particle_velocity, vp);
                spawn_effect_particle_with_target_scalar(joint_world_position,
                                                         particle_velocity,
                                                         0x8040C0F0,
                                                         0x808080,
                                                         scale_game_frames(20),
                                                         84000.0f);
            }
        }
        break;
    case MOBY_CLASS_TEN_PARTICLE_BURST_EMITTER:
    case MOBY_CLASS_JOINT_ZERO_TEN_PARTICLE_EMITTER:
    case MOBY_CLASS_GRAY_JOINT_ZERO_TEN_PARTICLE_EMITTER:
    case MOBY_CLASS_TEN_GRAY_PARTICLE_BURST_EMITTER:
        if (quiet == 0 && FUN_L00_001fefc8(data + 0x12E)) {
            if (fast_vec_length(vp) < frame_time * 40.0f &&
                fast_vec_dist(moby + 0x10, D_L03_00166EC0) < 75.0f) {
                fast_vec_scale(vp, vp, 0.7f);
                for (j = 0; j < 10; j++) {
                    moby_point_position(moby, 0, joint_world_position);
                    azimuth_angle = random_angle_radians();
                    elevation_angle = random_angle_radians();
                    build_spherical_offset(
                        particle_velocity, frame_time * 0.6f, azimuth_angle, elevation_angle);
                    fast_vec_add(particle_velocity, particle_velocity, vp);
                    spawn_effect_particle_with_target_scalar(joint_world_position,
                                                             particle_velocity,
                                                             0x80808080,
                                                             0x808080,
                                                             scale_game_frames(35),
                                                             105000.0f);
                }
            }
            data[0x12E] = random_int_between(scale_game_frames(7), scale_game_frames(20));
        }
        break;
    case MOBY_CLASS_FIFTEEN_PARTICLE_BURST_EMITTER:
        if (quiet == 0 && FUN_L00_001fefc8(data + 0x12E)) {
            if (fast_vec_length(vp) < frame_time * 40.0f &&
                fast_vec_dist(moby + 0x10, D_L03_00166EC0) < 90.0f) {
                fast_vec_scale(vp, vp, 0.5f);
                for (k = 0; k < 15; k++) {
                    moby_point_position(moby, 0, joint_world_position);
                    azimuth_angle = random_angle_radians();
                    elevation_angle = random_angle_radians();
                    build_spherical_offset(
                        particle_velocity, frame_time * 1.2f, azimuth_angle, elevation_angle);
                    fast_vec_add(particle_velocity, particle_velocity, vp);
                    spawn_effect_particle_with_target_scalar(joint_world_position,
                                                             particle_velocity,
                                                             0x80808080,
                                                             0x808080,
                                                             scale_game_frames(45),
                                                             168000.0f);
                }
            }
            data[0x12E] = random_int_between(scale_game_frames(7), scale_game_frames(20));
        }
        break;
    case MOBY_CLASS_ALTERNATE_COLOR_CHILD_EFFECT_EMITTER:
    case MOBY_CLASS_DEFAULT_COLOR_CHILD_EFFECT_EMITTER:
        FUN_L00_001fefc8(data + 0x12F);
        if (quiet != 0 && ((unsigned char *)data)[0x12F] == 0) {
            break;
        }
        FUN_L00_001fefc8(data + 0x12E);
        if (((unsigned char *)data)[0x12E] == 0 || ((unsigned char *)data)[0x12F] != 0) {
            moby_point_position(moby, 0, data + 0xF0);
        }
        if (((unsigned char *)data)[0x12E] != 0) {
            break;
        }
        if (fast_vec_length(vp) < frame_time * 40.0f &&
            fast_vec_dist(moby + 0x10, D_L03_00166EC0) < 90.0f) {
            color = 0xA040;
            color2 = 0xA080;
            if (MOBY(moby)->oclass == 0x4B) {
                color = 0x30A0;
                color2 = 0x90A0;
            }
            spawn_emitter_child_effect((struct Moby *)moby,
                                       (int)(data + 0xF0),
                                       color,
                                       color2,
                                       scale_game_frames(50),
                                       frame_time * 2.5f,
                                       0.15f);
            ((unsigned char *)data)[0x12F] = scale_game_frames(50);
        }
        data[0x12E] = scale_game_frames(13);
        break;
    }
}

extern float spring_turn(float *vel, float cur, float target, float k, float d, float max) __asm__("FUN_L00_00258110");
/* Ported from rac1-decomp (src/overlays/l03_kerwan/vendor_00293720.c: func_L03_0029E1A8), where it is exact; names translated to the US level program. */

extern float normalize_angle(float) __asm__("FUN_L00_0025e310");

void update_spline_moby_orientation(struct Moby *moby) __asm__("FUN_L03_0029cff0");

void update_spline_moby_orientation(struct Moby *moby) {
    char *moby_data = (char *)moby->pvars;
    float position_delta[4];
    float yaw_offset_radians = (float)*(short *)(moby_data + 0x132) * DEG_TO_RAD;
    float yaw_step, x, y, z, w;
    fast_subtract_rotations(moby->rot.z, yaw_offset_radians);
    x = fast_arc_tan(*(float *)(moby_data + 0xD0) - *(float *)(moby_data + 0xE0),
                     *(float *)(moby_data + 0xD4) - *(float *)(moby_data + 0xE4));
    x = fast_add_rotations(x, yaw_offset_radians);
    y = fast_subtract_rotations(x, moby->rot.z);
    yaw_step = y * (1.0f / *(float *)(moby_data + 0x120) * frame_scale);
    moby->rot.z = fast_add_rotations(yaw_step, moby->rot.z);
    fast_vec_sub(position_delta, moby_data + 0xD0, moby_data + 0xE0);
    z = fast_arc_tan(vector_length_xy(position_delta), position_delta[2]);
    moby->rot.y = spring_turn((float *)(moby_data + 0x11C),
                              moby->rot.y,
                              -z,
                              *(float *)(moby_data + 0x118) * 10.0f * DEG_TO_RAD * frame_time_sq,
                              *(float *)(moby_data + 0x118) * 5.0f * DEG_TO_RAD * frame_time_sq,
                              *(float *)(moby_data + 0x118) * DEG_TO_RAD * frame_time);
    w = normalize_angle(yaw_step * *(float *)(moby_data + 0x124));
    w = fast_subtract_rotations(moby->rot.x, w);
    w = normalize_angle(w * *(float *)(moby_data + 0x128));
    moby->rot.x = spring_turn((float *)(moby_data + 0x114),
                              moby->rot.x,
                              w,
                              *(float *)(moby_data + 0x110) * 10.0f * DEG_TO_RAD * frame_time_sq,
                              *(float *)(moby_data + 0x110) * 5.0f * DEG_TO_RAD * frame_time_sq,
                              *(float *)(moby_data + 0x110) * DEG_TO_RAD * frame_time);
}

/* Ported from rac1-decomp (src/overlays/l03_kerwan/vendor_00293720.c: func_L03_0029E370), where it is exact; names translated to the US level program. */

void update_emitter_intermittent_sound(struct Moby *moby) __asm__("FUN_L03_0029d1b8");

void update_emitter_intermittent_sound(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    int   unused_local;
    float distance_to_reference;
    switch (moby->oclass) {
    case 0x33:
    case 0x40:
    case MOBY_CLASS_ALTERNATE_COLOR_CHILD_EFFECT_EMITTER:
    case MOBY_CLASS_TEN_PARTICLE_BURST_EMITTER:
    case MOBY_CLASS_TWO_JOINT_PARTICLE_EMITTER:
    case MOBY_CLASS_FIFTEEN_PARTICLE_BURST_EMITTER:
    case MOBY_CLASS_DEFAULT_COLOR_CHILD_EFFECT_EMITTER:
        if (*(short *)(data + 0x148) == -1) {
            int lo = *(short *)(data + 0x14C);
            *(short *)(data + 0x148) =
                scale_game_frames(random_int_between(lo, lo + *(short *)(data + 0x14E)));
        } else {
            int x, y;
            if (!fast_dec_timer_s16(data + 0x148)) {
                break;
            }
            distance_to_reference = fast_vec_dist(&moby->pos, D_L03_00166EC0);
            if (distance_to_reference > 15.0f && distance_to_reference < 90.0f) {
                allocate_voice_for_target_entry(0, 0, (int)moby);
            }
            x = scale_game_frames(0x898);
            y = scale_game_frames(0x898);
            *(short *)(data + 0x148) =
                scale_game_frames(random_int_between(x, y + scale_game_frames(0x3E8)));
        }
        break;
    }
}

extern float D_L03_0015F580_macro[] __asm__("D_L03_0015F580") MACRO_ADDR;
extern void *D_L03_00160770;
extern void  allocate_voice_for_group_entry(int, int, void *) __asm__("FUN_0022dba0");
extern void  spawn_randomized_bolt_drop(void *, int, int, int, int) __asm__("FUN_L00_00260a88");
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
                             int,
                             float,
                             float,
                             int,
                             int,
                             int,
                             int) __asm__("FUN_L00_0025e450");
extern void  spawn_debris_29d2e0(char *,
                                 int,
                                 float *,
                                 void *,
                                 int,
                                 int,
                                 float,
                                 float *,
                                 float *,
                                 float *) __asm__("FUN_L00_00263fd8");
void         red_ship_1_update(char *moby) __asm__("FUN_L03_0029dba8");

#define SPAWN_DEBRIS(id)                           \
    spawn_debris_29d2e0(moby,                      \
                        (id),                      \
                        (float *)&MOBY(moby)->pos, \
                        moby + 0x40,               \
                        scale_game_frames(90),     \
                        0,                         \
                        frame_time_sq * 12.0f,     \
                        push,                      \
                        D_L03_0015F580_macro,      \
                        D_L03_0015F580_macro)

/* Wreck reaction for the Kerwan train and turret classes: when hit by a qualifying attack it bursts into
 * the class debris, starts the shake/smoke effects and arms the respawn countdown (0x14A); once that runs
 * out the moby is hidden again unless the caller keeps it, and switches its update to the car update. */
void update_spline_moby_destruction_and_respawn(char *moby,
                                                void *vel_in,
                                                int   flag) __asm__("FUN_L03_0029d2e0");

void update_spline_moby_destruction_and_respawn(char *moby, void *vel_in, int flag) {
    float          spot[4];
    float          halved_motion_delta[4];
    float          push[4];
    char          *data;
    char          *hit;
    int            voice_slot_index;
    unsigned char *voice_slot_entry;

    data = ((char *)MOBY(moby)->pvars);
    hit = find_moby_hit(moby, 0xA30000, 0);
    if (hit != 0 && *(unsigned short *)(hit + 0x28) != 0x102) {
        clear_u64_value(spot);
        if (MOBY(moby)->oclass == 0x4B) {
            spot[0] = fast_cos(MOBY(moby)->rot.z) * -2.747f;
            spot[1] = fast_sin(MOBY(moby)->rot.z) * -2.747f;
            spot[2] = 0.0f;
        }
        *(OvlQuad *)halved_motion_delta = *(OvlQuad *)vel_in;
        fast_vec_scale(halved_motion_delta, halved_motion_delta, 0.5f);
        fast_vec_add(spot, spot, moby + 0x10);
        if (*(int *)(hit + 0x24) & 0x800000) {
            spawn_randomized_bolt_drop(moby, 10, 20, 2, -1);
        } else {
            spawn_randomized_bolt_drop(moby, 3, 5, 2, -1);
        }
        *(short *)(data + 0x14A) = scale_game_frames(600);
        spawn_explosion(moby,
                        halved_motion_delta,
                        spot,
                        0.0f,
                        0.0f,
                        20,
                        3,
                        4,
                        4.0f,
                        2.0f,
                        100000.0f,
                        -1,
                        3.0f,
                        15.0f,
                        1,
                        1,
                        -1,
                        0);
        fast_vec_scale(push, moby + 0xC0, frame_scale * 0.075f);
        push[2] += frame_scale * 0.08f;
        switch (MOBY(moby)->oclass) {
        case MOBY_CLASS_TEN_PARTICLE_BURST_EMITTER:
            SPAWN_DEBRIS(0x6A9);
            SPAWN_DEBRIS(0x6AA);
            SPAWN_DEBRIS(0x6AB);
            break;
        case MOBY_CLASS_TWO_JOINT_PARTICLE_EMITTER:
            SPAWN_DEBRIS(0x6AC);
            SPAWN_DEBRIS(0x6AD);
            SPAWN_DEBRIS(0x6AE);
            break;
        case MOBY_CLASS_JOINT_ZERO_TEN_PARTICLE_EMITTER:
            SPAWN_DEBRIS(0x6AF);
            SPAWN_DEBRIS(0x6B0);
            SPAWN_DEBRIS(0x6B1);
            break;
        case MOBY_CLASS_FIFTEEN_PARTICLE_BURST_EMITTER:
            SPAWN_DEBRIS(0x6B2);
            SPAWN_DEBRIS(0x6B3);
            SPAWN_DEBRIS(0x6B4);
            break;
        case MOBY_CLASS_DEFAULT_COLOR_CHILD_EFFECT_EMITTER:
            SPAWN_DEBRIS(0x6B5);
            SPAWN_DEBRIS(0x6B6);
            SPAWN_DEBRIS(0x6B7);
            break;
        case MOBY_CLASS_GRAY_JOINT_ZERO_TEN_PARTICLE_EMITTER:
            SPAWN_DEBRIS(0x6B8);
            SPAWN_DEBRIS(0x6B9);
            SPAWN_DEBRIS(0x6BA);
            break;
        case MOBY_CLASS_TEN_GRAY_PARTICLE_BURST_EMITTER:
            SPAWN_DEBRIS(0x6BC);
            SPAWN_DEBRIS(0x6BE);
            SPAWN_DEBRIS(0x6BF);
            break;
        }
        if (MOBY(moby)->oclass == 0x31B && item_unlocked[4] == 0) {
            item_unlocked[4] = 1;
            allocate_voice_for_bank_entry(1, 0, 0);
            queue_dialogue_message(0x53D6, -1);
        }
        allocate_voice_for_group_entry(0, 0, moby);
    }
    D_L03_00160770 = (void *)red_ship_1_update;
    ((unsigned char *)moby)[0xA4] = 0xFF;
    if (*(short *)(data + 0x14A) != 0) {
        if (fast_dec_timer_s16(data + 0x14A) && flag) {
            MOBY(moby)->collision = *(int *)(((char *)MOBY(moby)->pclass) + 0x10);
            MOBY(moby)->flags = (MOBY(moby)->flags & 0xFFBE) | 0x1000;
        } else {
            MOBY(moby)->collision = 0;
            MOBY(moby)->flags |= 0x41;
            voice_slot_index = *(int *)(data + 0x150);
            if (voice_slot_index != -1) {
                voice_slot_entry = (unsigned char *)&voice_pool + voice_slot_index * 0x70;
                if (*(char **)(voice_slot_entry + 0x88) == moby && voice_slot_entry[0x74]) {
                    release_voice_slot(voice_slot_index);
                }
            }
            *(int *)(data + 0x150) = -1;
            MOBY(moby)->flags &= 0xEFFF;
        }
    }
}
#undef SPAWN_DEBRIS
extern char *D_L03_0015FFD8_29dba8 __asm__("D_L03_0015FFD8");
extern char  spline_moby_missing_path_warning_format[] __asm__("D_L03_001E2A28");
extern int   get_first_moby_in_group(int *, int, int, int) __asm__("FUN_L00_002591d0");
extern int   get_next_moby_in_group(int *, int, int, int) __asm__("FUN_L00_002592b8");
extern int   check_scaled_vector_bsphere(char *, float) __asm__("FUN_L01_00275690");
extern int   select_point_index_by_distance(void *, void *, float) __asm__("FUN_L01_0028b510");
extern int   resolve_path_node_index(void *, int) __asm__("FUN_L01_0028b828");
extern int   advance_path_node_index(char *) __asm__("FUN_L01_0028b878");
extern void  initialize_path_descriptor(char *) __asm__("FUN_L01_0028b410");
extern void
compute_path_node_tangent(void *out, int *l, int idx, float a, float b) __asm__("FUN_L01_0028b8c8");
extern void evaluate_cubic_hermite_segment(
    float t, void *out, void *p1, void *p2, void *p3, void *p4) __asm__("FUN_L01_0028ba80");
extern void
compute_sampled_spline_path_lengths(void *, int, void *, float, float) __asm__("FUN_L01_0028bb90");
extern void
update_spline_moby_destruction_and_respawn(char *, void *, int) __asm__("FUN_L03_0029d2e0");
extern void report_29dba8(void *, int, int) __asm__("FUN_001e93b0");

/* Train car update: keeps the cars of a group in step (stops them all when none can move), and on its
 * first frame snaps to the nearest path node and sets up the spline; then runs along the path spline,
 * steering, banking and spawning its hit/smoke effects; state 2 waits without a path, 3 waits a timer. */
void red_ship_1_update(char *moby) {
    enum {
        RED_SHIP_INITIALIZE = 0,
        RED_SHIP_FOLLOW_PATH_SPLINE = 1,
        RED_SHIP_WAIT_FOR_PATH = 2,
        RED_SHIP_WAIT_FOR_MOVEMENT_TIMER = 3
    };
    float          zero[4];
    float          old_rot[4];
    float          old_pos[4];
    float          culling_sphere_or_motion_delta[4];
    float          position_delta[4];
    char          *item;
    char          *data;
    char          *pm;
    char          *path_tangent_or_moby_position;
    char          *initial_path_tangent;
    char          *moby_position;
    char          *path_node_base;
    char          *group_member_data;
    int           *path;
    int            flag;
    int            found;
    int            advanced_path_node_index;
    int            initial_path_node_index;
    int            voice_slot_index;
    unsigned char *voice_slot_entry;
    float          initial_speed_oscillation_phase;
    float          step;

    *(OvlQuad *)zero = 0;
    flag = 0;
    data = ((char *)MOBY(moby)->pvars);
    if (((unsigned char *)moby)[0x31] == 0) {
        *(OvlQuad *)culling_sphere_or_motion_delta = *(OvlQuad *)&MOBY(moby)->pos;
        culling_sphere_or_motion_delta[3] = 7.0f;
        flag = fast_bsphere_check(culling_sphere_or_motion_delta, MOBY(moby)->draw_dist) == -1;
    }
    if (data == 0) {
        return;
    }
    pm = data + 0x60;
    if (((unsigned char *)moby)[0x21] != 0xFF && ((unsigned char *)moby)[0x20] != 0 &&
        ((unsigned char *)moby)[0x20] != 3) {
        found = 0;
        get_first_moby_in_group((int *)&item, ((unsigned char *)moby)[0x21], 0, 0);
        if (item == moby) {
            goto test;
            for (;;) {
                get_next_moby_in_group((int *)&item, (int)item, 0, 0);
            test:
                if (item == 0) {
                    break;
                }
                if (check_scaled_vector_bsphere(
                        item, convert_integer_to_float(*(short *)(item + 0x32))) != -1) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                get_first_moby_in_group((int *)&item, ((unsigned char *)moby)[0x21], 0, 0);
                while (item != 0) {
                    group_member_data = *(char **)(item + 0x78);
                    item[0x20] = 3;
                    *(short *)(group_member_data + 0x12C) = scale_game_frames(5);
                    get_next_moby_in_group((int *)&item, (int)item, 0, 0);
                }
            }
        }
    }
    maintain_emitter_sound_voice((unsigned char *)moby);
    update_emitter_intermittent_sound(moby);
    switch (((unsigned char *)moby)[0x20]) {
    case RED_SHIP_INITIALIZE:
        MOBY(moby)->flags |= 0x1000;
        if (*(int *)(pm + 0x14) != -1) {
            path = (int *)l03_paths[*(int *)(pm + 0x14)];
            if (fast_vec_dist((char *)path + 0x10, (char *)path + *path * 0x10) < 0.5f) {
                *path -= 1;
            }
        } else {
            report_29dba8(spline_moby_missing_path_warning_format,
                          MOBY(moby)->oclass,
                          truncate_float_to_s32((moby - D_L03_0015FFD8_29dba8) >> 8));
        }
        ((unsigned char *)moby)[0x30] = 0xFF;
        data[0x2B] = 1;
        data[0x2C] = 0x25;
        if (*(float *)(data + 0x140) > 500.0f) {
            *(float *)(data + 0x140) =
                random_float_between(-1.0f, 1.0f) * *(float *)(data + 0x140) / 1000.0f;
        }
        if (*(float *)(data + 0x144) > 500.0f) {
            *(float *)(data + 0x144) =
                random_float_between(-1.0f, 1.0f) * *(float *)(data + 0x144) / 1000.0f;
        }
        if (*(int *)(pm + 0x14) == -1) {
            moby[0x20] = 2;
            break;
        }
        moby_position = moby + 0x10;
        data[0x131] = ((moby - D_L03_0015FFD8_29dba8) >> 8) / 2 % 7;
        initialize_path_descriptor(pm);
        if (((unsigned char *)pm)[5] == 0) {
            if (*(int *)pm <= 0) {
                initial_path_node_index =
                    select_point_index_by_distance(moby_position, *(void **)(pm + 0x10), 0.0f);
                *(int *)pm = initial_path_node_index;
                if (pm[4] >= 0 ? initial_path_node_index == **(int **)(pm + 0x10) - 1
                               : initial_path_node_index == 0) {
                    *(int *)pm = 0;
                }
            }
            *(float *)(pm + 0xC) =
                fast_vec_dist(moby_position, *(char **)(pm + 0x10) + (*(int *)pm * 0x10 + 0x10));
            path_node_base = *(char **)(pm + 0x10) + *(int *)pm * 0x10;
            *(float *)(pm + 0x18) =
                fast_arc_tan(*(float *)(path_node_base + 0x10) - MOBY(moby)->pos.x,
                             *(float *)(path_node_base + 0x14) - MOBY(moby)->pos.y);
            pm[5] = 2;
            *(int *)(pm + 0x8) = 0;
            *(int *)(pm + 0x1C) = 0;
        }
        initial_path_tangent = data + 0xA0;
        *(OvlQuad *)(data + 0xE0) = *(OvlQuad *)(*(char **)(pm + 0x10) + *(int *)pm * 0x10 + 0x10);
        *(float *)(data + 0x100) = 1.0f;
        compute_path_node_tangent(
            initial_path_tangent, (int *)pm, 0, *(float *)(data + 0x104), *(float *)(data + 0x108));
        *(OvlQuad *)(data + 0xB0) = *(OvlQuad *)(data + 0xC0) =
            *(OvlQuad *)(*(char **)(pm + 0x10) + *(int *)pm * 0x10 + 0x10);
        MOBY(moby)->rot.x = 0.0f;
        MOBY(moby)->rot.y =
            -fast_arc_tan(vector_length_xy(initial_path_tangent), *(float *)(data + 0xA8));
        MOBY(moby)->rot.z =
            fast_arc_tan(*(float *)(data + 0xA0) - zero[0], *(float *)(data + 0xA4) - zero[1]);
        *(float *)(data + 0x114) = 0.0f;
        *(float *)(data + 0x11C) = 0.0f;
        *(OvlQuad *)&MOBY(moby)->pos = *(OvlQuad *)(data + 0xE0);
        add_moby_rotation_based_offsets(moby, moby_position);
        data[0x131] = 7;
        initial_speed_oscillation_phase = *(float *)(data + 0x13C);
        if (initial_speed_oscillation_phase > 6.2831855f) {
            *(float *)(data + 0x13C) = initial_speed_oscillation_phase * DEG_TO_RAD;
        } else if (*(float *)(data + 0x134) != 0.0f && initial_speed_oscillation_phase == -1.0f) {
            *(float *)(data + 0x13C) = random_angle_radians();
        }
        *(OvlQuad *)(data + 0xD0) = *(OvlQuad *)(data + 0xC0);
        compute_sampled_spline_path_lengths(
            pm, 0, moby, *(float *)(data + 0x104), *(float *)(data + 0x108));
        if (*(short *)(data + 0x12C) > 0) {
            moby[0x20] = 3;
        } else {
            *(short *)(data + 0x14A) = 0;
            moby[0x20] = 1;
        }
        break;
    case RED_SHIP_WAIT_FOR_PATH:
        if (*(int *)(pm + 0x14) != -1) {
            moby[0x20] = 0;
        }
        voice_slot_index = *(int *)(data + 0x150);
        if (voice_slot_index != -1) {
            voice_slot_entry = (unsigned char *)&voice_pool + voice_slot_index * 0x70;
            if (*(char **)(voice_slot_entry + 0x88) == moby && voice_slot_entry[0x74]) {
                release_voice_slot(voice_slot_index);
            }
        }
        *(int *)(data + 0x150) = -1;
        break;
    case RED_SHIP_WAIT_FOR_MOVEMENT_TIMER:
        if (fast_dec_timer_s16(data + 0x12C)) {
            moby[0x20] = 1;
        }
        break;
    case RED_SHIP_FOLLOW_PATH_SPLINE:
        *(OvlQuad *)old_rot = *(OvlQuad *)&MOBY(moby)->rot;
        *(OvlQuad *)old_pos = *(OvlQuad *)&MOBY(moby)->pos;
        path_tangent_or_moby_position = data + 0xA0;
        if (*(float *)(data + 0x100) >= 1.0f) {
            *(float *)(data + 0x100) -= 1.0f;
            advanced_path_node_index = advance_path_node_index(pm);
            *(OvlQuad *)(data + 0x90) = *(OvlQuad *)(data + 0xA0);
            compute_path_node_tangent(path_tangent_or_moby_position,
                                      (int *)pm,
                                      1,
                                      *(float *)(data + 0x104),
                                      *(float *)(data + 0x108));
            *(OvlQuad *)(data + 0xB0) = *(OvlQuad *)(data + 0xC0);
            *(OvlQuad *)(data + 0xC0) = *(
                OvlQuad *)(*(char **)(pm + 0x10) +
                           resolve_path_node_index(pm, advanced_path_node_index + 1) * 0x10 + 0x10);
        }
        step = (*(float *)(data + 0x10C) +
                *(float *)(data + 0x134) * fast_sin(*(float *)(data + 0x13C))) /
               *(float *)(data + 0xBC);
        *(float *)(data + 0x13C) =
            fast_add_rotations(*(float *)(data + 0x13C), *(float *)(data + 0x138) * DEG_TO_RAD);
        *(OvlQuad *)(data + 0xE0) = *(OvlQuad *)(data + 0xD0);
        evaluate_cubic_hermite_segment(*(float *)(data + 0x100) + step,
                                       data + 0xD0,
                                       data + 0xB0,
                                       data + 0xC0,
                                       data + 0x90,
                                       path_tangent_or_moby_position);
        if (!flag) {
            update_spline_moby_orientation(moby);
        } else {
            clear_u64_value(moby + 0x40);
            MOBY(moby)->rot.z = fast_arc_tan(*(float *)(data + 0xD0) - *(float *)(data + 0xE0),
                                             *(float *)(data + 0xD4) - *(float *)(data + 0xE4));
        }
        path_tangent_or_moby_position = moby + 0x10;
        *(float *)(data + 0x100) += step;
        *(OvlQuad *)&MOBY(moby)->pos = *(OvlQuad *)(data + 0xE0);
        if (!flag) {
            add_moby_rotation_based_offsets(moby, path_tangent_or_moby_position);
        }
        fast_vec_sub(position_delta, path_tangent_or_moby_position, old_pos);
        *(OvlQuad *)culling_sphere_or_motion_delta = *(OvlQuad *)position_delta;
        update_moby_emitter_effects(moby, culling_sphere_or_motion_delta, flag);
        update_spline_moby_destruction_and_respawn(moby, culling_sphere_or_motion_delta, flag);
        break;
    }
}

void set_moby_color(char *, int, int, int) __asm__("FUN_L00_002502f0");
void f250df8_2bad40(char *) __asm__("FUN_L00_00250df8");
/* Spawns a child effect (moby 0xEB) on the parent: copies its position and rotation, then sets the colour, timers and scale. */
char *spawn_emitter_child_effect(struct Moby *parent,
                                 int          unused,
                                 int          color,
                                 int          secondary_color,
                                 int          timer_frames,
                                 float        f,

                                 float scale) {
    char *child_moby = create_moby(0xEB);
    if (child_moby) {
        char *child_data = ((char *)MOBY(child_moby)->pvars);
        int   r, g, b;
        MOBY(child_moby)->update_dist = 0xFF;
        MOBY(child_moby)->draw_dist = 0x7F;
        child_moby[0x31] = 1;
        MOBY(child_moby)->scale *= scale;
        *(char **)child_data = parent;
        qcopy(child_moby + 0x10, (char *)parent->pvars + 0xF0);
        *(OvlQuad *)&MOBY(child_moby)->rot = *(OvlQuad *)&parent->rot;
        MOBY(child_moby)->rot.x = random_angle_radians();
        *(short *)(child_data + 0xC) = timer_frames;
        b = (color >> 16) & 0xFF;
        g = (color >> 8) & 0xFF;
        r = color & 0xFF;
        *(short *)(child_data + 0xE) = timer_frames;
        *(float *)(child_data + 0x14) = f;
        *(int *)(child_data + 0x4) = color;
        *(int *)(child_data + 0x8) = secondary_color;
        set_moby_color(child_moby, r, g, b);
        f250df8_2bad40(child_moby);
    }
    return child_moby;
}
