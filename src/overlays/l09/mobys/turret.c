/* Level 09 moby class 0x4b1: turret. */
#include "types.h"
#include "rnc/game/options.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/random.h"
#include "rnc/math/spherical_offset.h"
#include "qcopy.h"
#include "rnc/overlay/level_globals.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/hero/hero_set_state.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/attach_manipulator.h"
#include "rnc/overlay/shared/camera/scripted_camera.h"
#include "rnc/overlay/shared/camera/follow_camera_set_up_spring.h"
#include "rnc/overlay/shared/rendering/effects/glow_quads_draw.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/mobys/moby.h"
#include "rnc/input/pad_state.h"

/* Gaspar turret the hero mounts: aims with the stick, fires its two barrels in turn,
   and while idle swings toward the hero with its beam light on. */
typedef struct {
    u8   pad0[0x10];
    Vec4 aim; /* 0x10: FUN_L00_001fff28 target */
    u8   pad20[0x20];
    s16  unk40;
    s16  mount_delay; /* 0x42: frames before the hero can mount again */
    f32  pitch;       /* 0x44 */
    s32  unk48;
    s32  unk4C;
    u8   pad50[2];
    s16  dismount_delay; /* 0x52 */
    s16  barrel;         /* 0x54: 0 fires barrel 1 next, 1 barrel 2 */
    u8   pad56[6];
    f32  rest_z;      /* 0x5C */
    f32  yaw_speed;   /* 0x60 */
    f32  pitch_speed; /* 0x64 */
    s32  beam;        /* 0x68: D_L09_00180440 slot, -1: none */
    s32  beam_frames; /* 0x6C */
} TurretVars;

typedef struct {
    f32 size;
    u8  pad4[0xC];
    f32 pos[3];
    f32 alpha;
} TurretBeam;

typedef struct {
    u8  pad0[0x160];
    f32 unk160;
    u8  pad164[4];
    s32 unk168;
    u8  pad16C[4];
    f32 unk170;
    u8  pad174[4];
    s32 unk178;
} TurretShake;

/* moby the sound collision probe ignores (clamp_voice_position_to_collision) */
extern struct Moby *sound_probe_ignore[] __asm__("D_0013E5BC");
extern f32          turret_camera_offset_length_gp __asm__("D_L09_00161CB0") __attribute__((sda));
extern f32          turret_camera_z_offset_gp __asm__("D_L09_00161CB4") __attribute__((sda));
extern f32          turret_beam_local_offset_gp[2] __asm__("D_L09_00161CC0") __attribute__((sda));

/* a vector; declared small so Ps2EeAs reaches it through $gp */
extern TurretShake l09_turret_shot_response_state __asm__("D_L09_00166E00");
extern TurretBeam  l09_turret_beam_light_slots[] __asm__("D_L09_00180440");
/* turret_base.c defines it with a third float argument that it never reads. */
extern void         spawn_turret_base(struct Moby *, Vec4 *) __asm__("FUN_L09_002efda8");
extern struct Moby *spawn_turret_shot(void *, void *, void *, int) __asm__("FUN_L09_00307ba8");
extern void         snap_scripted_camera_to_pose(void) __asm__("FUN_L08_00313ba8");
extern int
allocate_colored_effect_slot(void *, float, float, float, float, float) __asm__("FUN_L00_0023e738");
extern void FUN_L00_0026dd20(void *, void *, int, int, float);
extern void divide_vector_xyz(void *, void *, float) __asm__("FUN_L00_001ff378");
extern void suck_draw_reticle(void *, int, void *, int, int, int, float, float, float) __asm__("FUN_L00_001edf60");
extern float compute_interpolated_record_value(void *) __asm__("FUN_0020c9e0");
extern void  FUN_L00_001fff28(void *, int, float);
void         turret_update(struct Moby *moby) __asm__("FUN_L09_00304c80");

void turret_update(struct Moby *moby) {
    enum {
        TURRET_STATE_INITIALIZE = 0,
        TURRET_STATE_IDLE = 4,
        TURRET_STATE_MOUNTED = 5
    };
    Vec4        seat;
    Vec4        offset;
    Vec4        work_vector;
    Vec4        shot;
    Vec4        muzzle;
    Vec4        firing_vector_or_camera_rotation;
    Vec4        rnd;
    TurretVars *vars;
    TurretBeam *beam;
    float       stick_magnitude;
    float       stick_y;
    int         beam_frame_count;
    int         i;

    vars = (TurretVars *)moby->pvars;
    moby->flags &= ~4;
    moby_point_position(moby, 3, &seat);
    seat.f[2] += 0.4f;
    if (moby->state >= 4) {
        if (fast_dec_timer_s16(&vars->mount_delay) != 0 && moby->command == 1 &&
            hero.state.control_mode != 0x14 && hero.state.control_mode != 7 &&
            hero.state.current != 0x3D) {
            qcopy(&hero.motion.pos, &seat);
            qcopy(&hero.motion.rot, &moby->rot);
            hero.motion.rot.f[2] = fast_add_rotations(1.5707964f, hero.motion.rot.f[2]);
            offset.f[0] = fast_cos(hero.motion.rot.f[2]) * -0.7f;
            offset.f[1] = fast_sin(hero.motion.rot.f[2]) * -0.7f;
            offset.f[2] = 0.0f;
            fast_vec_add(&hero.motion.pos, &hero.motion.pos, &offset);
            fast_mem_zero16(&hero.motion.velocity, 0x90);
            if (D_0013CAE4 & 0x10) {
                jet_end_mount(0);
                hero_set_state(0, 1);
                moby->command = 2;
                moby->state = 4;
                blend_moby_animation(moby, 0, 0, 1);
            } else {
                if (moby->state != 5) {
                    vars->dismount_delay = 2;
                    jet_mount(&moby->pos, &moby->rot, 0, 0, 0);
                    if (moby->prev_seq != 5) {
                        blend_moby_animation(moby, 5, 0, scale_game_frames(5));
                    }
                }
                moby->state = 5;
            }
        } else if (moby->command == 2) {
            if (fast_vec_dist(&seat, &hero.moby->pos) > 1.2f || hero.ground_moby != moby) {
                moby->command = 0;
                vars->mount_delay = scale_game_frames(60);
            }
        } else if (hero.state.control_mode != 0x14 && hero.state.control_mode != 7 &&
                   hero.state.current != 0x3D && hero.ground_moby == moby &&
                   fast_vec_dist(&seat, &hero.moby->pos) < 1.2f && vars->mount_delay == 0) {
            moby->command = 1;
            hero_set_state(0x32, 1);
        }
    }
    switch (moby->state) {
    case TURRET_STATE_INITIALIZE:
        attach_manipulator(moby, 0, vars);
        work_vector.q = *(u128 *)&moby->pos;
        spawn_turret_base(moby, &work_vector);
        moby->state = 4;
        vars->rest_z = moby->pos.z;
        vars->pitch = 0.0f;
        moby->rot.z = 0.0f;
        vars->beam = -1;
        vars->mount_delay = 0;
        vars->unk48 = 0;
        vars->unk4C = 0;
        vars->unk40 = 0;
        sound_probe_ignore[0] = moby;
        return;
    case TURRET_STATE_IDLE:
        if (vector_distance_xy(&moby->pos, &hero.motion.pos) < 4.0f || hero.ground_moby == moby) {
            moby->pos.z += ((vars->rest_z - 1.0f) - moby->pos.z) * 0.11f;
            vars->yaw_speed =
                fast_add_rotations(
                    vars->yaw_speed,
                    fast_add_rotations(
                        fast_subtract_rotations(fast_arc_tan(hero.motion.pos.f[0] - moby->pos.x,
                                                             hero.motion.pos.f[1] - moby->pos.y),
                                                moby->rot.z),
                        1.5707964f) *
                        0.01f) *
                0.9f;
            moby->rot.z = fast_add_rotations(moby->rot.z, vars->yaw_speed);
            transform_vector_by_basis(&work_vector, turret_beam_local_offset_gp, &moby->rot_row0);
            fast_vec_add(&work_vector, &work_vector, &seat);
            if (vars->beam == -1) {
                vars->beam_frames = 0;
                vars->beam =
                    allocate_colored_effect_slot(&work_vector, 0.4f, 0.0f, 4.0f, 4.0f, 4.0f);
                allocate_voice_for_target_entry(1, 0, moby);
            } else {
                beam = &l09_turret_beam_light_slots[vars->beam];
                beam->pos[0] = work_vector.f[0];
                beam->pos[1] = work_vector.f[1];
                beam->pos[2] = work_vector.f[2];
                beam_frame_count = ++vars->beam_frames;
                if (beam_frame_count > 40 || !(beam_frame_count & 4)) {
                    beam->size = 4.0f;
                    beam->alpha = 0.4f;
                } else if (beam_frame_count >= 25 && (beam_frame_count & 0xA) == 0xA) {
                    beam->size = 4.0f;
                    beam->alpha = 0.4f;
                } else {
                    beam->alpha = 0.0f;
                }
            }
        } else {
            if (moby->pos.z < vars->rest_z) {
                moby->pos.z += frame_time;
            } else {
                moby->pos.z = vars->rest_z;
            }
            if (hero.ground_moby != moby && vars->beam != -1) {
                allocate_voice_for_target_entry(2, 0, moby);
                release_point_light_slot(vars->beam);
                vars->beam = -1;
            }
        }
        break;
    case TURRET_STATE_MOUNTED:
        sound_probe_ignore[0] = moby;
        if (moby->pos.z < vars->rest_z) {
            moby->pos.z += frame_time;
        } else {
            moby->pos.z = vars->rest_z;
        }
        work_vector.f[0] = hero.stick_x;
        stick_y = -hero.stick_y;
        if (!invert52) {
            stick_y = hero.stick_y;
        }
        work_vector.f[1] = stick_y;
        stick_magnitude = vector_length_xy(&work_vector);
        if (stick_magnitude > 1.0f) {
            divide_vector_xyz(&work_vector, &work_vector, stick_magnitude);
        }
        vars->yaw_speed += frame_time_sq * 3.1415927f * -work_vector.f[0];
        vars->pitch_speed += frame_time_sq * 0.7853982f * -work_vector.f[1];
        moby->rot.z = fast_add_rotations(moby->rot.z, vars->yaw_speed);
        vars->pitch = fast_add_rotations(vars->pitch, vars->pitch_speed);
        if (vars->pitch < -0.5235988f) {
            vars->pitch = -0.5235988f;
            vars->pitch_speed = 0.0f;
        } else if (vars->pitch > 0.5235988f) {
            vars->pitch = 0.5235988f;
            vars->pitch_speed = 0.0f;
        }
        vars->yaw_speed *= 0.97f;
        vars->pitch_speed *= 0.97f;
        FUN_L00_001fff28(&vars->aim, 1, -vars->pitch);
        if (controller_state.held & 0x20) {
            if (controller_state.pressed & 0x20) {
                blend_moby_animation(moby, vars->barrel == 0 ? 4 : 3, 0, 1);
            } else if (moby->seq == 3) {
                if (absolute_float(compute_interpolated_record_value(moby) - 1.0f) <= 0.25f) {
                    build_spherical_offset(&shot,
                                           frame_time * 60.0f,
                                           fast_add_rotations(1.5707964f, moby->rot.z),
                                           vars->pitch);
                    moby_point_position(moby, 1, &muzzle);
                    spawn_turret_shot(moby, &muzzle, &shot, scale_game_frames(90));
                    allocate_voice_for_target_entry(0, 0, moby);
                    l09_turret_shot_response_state.unk160 = 0.03f;
                    l09_turret_shot_response_state.unk168 = scale_game_frames(10);
                    l09_turret_shot_response_state.unk170 = 0.03f;
                    l09_turret_shot_response_state.unk178 = scale_game_frames(7);
                    vars->barrel = 0;
                    for (i = 0; i < 5; i++) {
                        rnd.q = 0;
                        rnd.f[0] = random_float_between(-1.0f, 1.0f);
                        rnd.f[1] = random_float_between(-1.0f, 1.0f);
                        rnd.f[2] = random_float_between(-1.0f, 1.0f);
                        firing_vector_or_camera_rotation.q = rnd.q;
                        fast_vec_normalize(&firing_vector_or_camera_rotation,
                                           &firing_vector_or_camera_rotation,
                                           fast_vec_length(&shot) *
                                               (frame_scale * (0.65f - 1.0f) + 1.0f));
                        fast_vec_add(&firing_vector_or_camera_rotation,
                                     &shot,
                                     &firing_vector_or_camera_rotation);
                        fast_vec_normalize(
                            &firing_vector_or_camera_rotation,
                            &firing_vector_or_camera_rotation,
                            random_float_between(frame_time * 3.0f, frame_time * 6.0f));
                        FUN_L00_0026dd20(
                            &muzzle,
                            &firing_vector_or_camera_rotation,
                            0x5F2F4F6F,
                            random_int_between(scale_game_frames(5), scale_game_frames(10)),
                            50000.0f);
                    }
                    fast_vec_normalize(&shot, &shot, frame_time * 0.5f);
                    FUN_L00_0026dd20(&muzzle,
                                     &shot,
                                     0x2F4F7F7F,
                                     random_int_between(scale_game_frames(4), scale_game_frames(7)),
                                     500000.0f);
                } else if (moby->anim_flags & 2) {
                    blend_moby_animation(moby, 4, 0, 1);
                }
            } else if (moby->seq == 4) {
                if (absolute_float(compute_interpolated_record_value(moby) - 1.0f) <= 0.25f) {
                    build_spherical_offset(&shot,
                                           frame_time * 60.0f,
                                           fast_add_rotations(1.5707964f, moby->rot.z),
                                           vars->pitch);
                    moby_point_position(moby, 2, &muzzle);
                    spawn_turret_shot(moby, &muzzle, &shot, scale_game_frames(90));
                    allocate_voice_for_target_entry(0, 0, moby);
                    l09_turret_shot_response_state.unk160 = 0.03f;
                    l09_turret_shot_response_state.unk168 = scale_game_frames(10);
                    l09_turret_shot_response_state.unk170 = 0.03f;
                    l09_turret_shot_response_state.unk178 = scale_game_frames(7);
                    vars->barrel = 1;
                    for (i = 0; i < 5; i++) {
                        rnd.q = 0;
                        rnd.f[0] = random_float_between(-1.0f, 1.0f);
                        rnd.f[1] = random_float_between(-1.0f, 1.0f);
                        rnd.f[2] = random_float_between(-1.0f, 1.0f);
                        firing_vector_or_camera_rotation.q = rnd.q;
                        fast_vec_normalize(&firing_vector_or_camera_rotation,
                                           &firing_vector_or_camera_rotation,
                                           fast_vec_length(&shot) *
                                               (frame_scale * (0.65f - 1.0f) + 1.0f));
                        fast_vec_add(&firing_vector_or_camera_rotation,
                                     &shot,
                                     &firing_vector_or_camera_rotation);
                        fast_vec_normalize(
                            &firing_vector_or_camera_rotation,
                            &firing_vector_or_camera_rotation,
                            random_float_between(frame_time * 3.0f, frame_time * 6.0f));
                        FUN_L00_0026dd20(
                            &muzzle,
                            &firing_vector_or_camera_rotation,
                            0x5F2F4F6F,
                            random_int_between(scale_game_frames(5), scale_game_frames(10)),
                            50000.0f);
                    }
                    fast_vec_normalize(&shot, &shot, frame_time * 0.5f);
                    FUN_L00_0026dd20(&muzzle,
                                     &shot,
                                     0x2F4F7F7F,
                                     random_int_between(scale_game_frames(4), scale_game_frames(7)),
                                     500000.0f);
                } else if (moby->anim_flags & 2) {
                    blend_moby_animation(moby, 3, 0, 1);
                }
            }
        } else if (moby->prev_seq != 5 && (moby->anim_flags & 2)) {
            blend_moby_animation(moby, 5, 0, 1);
        }
        if (fast_dec_timer_s16(&vars->dismount_delay) != 0) {
            update_entity(moby);
            moby->flags |= 4;
            snap_scripted_camera_to_pose();
            moby_point_position(moby, 0, &work_vector);
            build_spherical_offset(&shot,
                                   turret_camera_offset_length_gp,
                                   fast_add_rotations(1.5707964f, moby->rot.z),
                                   vars->pitch);
            shot.f[2] += turret_camera_z_offset_gp;
            fast_vec_add(&muzzle, &work_vector, &shot);
            set_camera_position(&muzzle);
            firing_vector_or_camera_rotation.f[0] = 0.0f;
            firing_vector_or_camera_rotation.f[1] = -vars->pitch;
            firing_vector_or_camera_rotation.f[2] = fast_add_rotations(1.5707964f, moby->rot.z);
            set_camera_rotation(&firing_vector_or_camera_rotation);
        }
        suck_draw_reticle(moby, 0xFF0F0FFF, 0, 0xD, -1, 4, 1.0f, 0.0f, 90.0f);
        break;
    }
}
