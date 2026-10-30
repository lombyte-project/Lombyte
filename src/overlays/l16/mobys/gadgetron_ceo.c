/* Level 16 gadgetron_ceo and the challenge layout parameter. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"
#include "rnc/game/options.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/mobys/is_hero_on_moby.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/math/interpolation/spring_turn.h"
#include "rnc/overlay/shared/rendering/effects/spawn_type_10_particle.h"
#include "rnc/world/collision/collision_line.h"
#include "rnc/audio/voice_pool.h"

#include "qcopy.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/entities.h"
#include "rnc/overlay/watch.h"
#include "rnc/game/item_state.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voices/allocate_voice_for_bank_entry.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/interpolation/compute_directed_angle_delta.h"
#include "rnc/overlay/shared/vendor/vendor_spring.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/overlay/shared/mobys/step_moby_motion.h"
#include "rnc/overlay/shared/ui/help/help_request.h"
#include "rnc/save/memcard_save_data.h"
#include "rnc/mobys/animation/set_moby_animation.h"
#include "rnc/overlay/shared/cutscenes/update_npc_dialogue.h"
#include "rnc/overlay/shared/game/restore_or_clear_level_state.h"
#include "rnc/ui/text/font/font_queue_vu_state.h"
#include "rnc/overlay/shared/mobys/attach_big_head_cheat_manipulator.h"
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002DF780), where it is exact; names translated to the US level program. */

extern float challenge_layout_param __asm__("D_L16_00161C74");

void challenge_set_layout_param(float new_value) __asm__("FUN_L16_002de3b8");

void challenge_set_layout_param(float new_value) {
    challenge_layout_param = new_value;
}

extern float scale_time(float) __asm__("FUN_001f96b0");
extern float fast_add_rotations(float, float);
extern float fast_subtract_rotations(float, float);
extern float vector_length_xy(void *);
extern int   scale_game_frames(int);
extern s32   D_L16_0015F5C4;
extern void  set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern void  update_joint_animation(float, float, void *, void *, int) __asm__("FUN_L00_002628d8");
extern void  build_spherical_offset(void *, float, float, float) __asm__("FUN_00214db0");

#include "sda.h"

extern int D_L16_0015F5CC;

extern char D_L16_001671C0[];
extern void FUN_L00_0025a120(void *);

/* Update the challenge NPC's interaction state and gaze. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002E7C70), where it is exact; names translated to the US level program. */

typedef struct {
    char          pad0[4];
    short         previous;
    char          pad6[2];
    unsigned char flag;
    char          pad9[0x2D];
    short         mode;
    int           pending;
} L16ChallengeActivation;

typedef struct {
    char  pad00[0x20];
    char  activation[0x40];
    char  pad60[0x64];
    float initial_yaw;
    int   moving_timer, target_timer;
    float target[4];
    char  body[0x60];
    char  pad140[4];
    float pitch, yaw;
    char  pad14C[4];
    float height;
    char  pad154[0xC];
    char  head[0x60];
    char  pad1C0[8];
    float head_yaw;
} L16ChallengeData;

typedef struct {
    char              pad00[0x10];
    float             position[4];
    unsigned char     state;
    char              pad21[0xF];
    unsigned char     opacity, visible;
    char              pad32[0x16];
    float             yaw;
    char              pad4C[7];
    unsigned char     animation;
    char              pad54[0x24];
    L16ChallengeData *data;
    char              pad7C[3];
    unsigned char     distance_class;
    char              pad80[0x3C];
    unsigned char     event;
} L16ChallengeMoby;

typedef struct {
    char  pad00[0x80];
    float position[4];
    char  pad90[0x40];
    float aim[4];
    char  padE0[0x20];
    float velocity[4];
    char  pad110[0x798];
    int   points;
    char  pad8AC[0x17D8];
    int   state;
    char  pad2088[4];
    int   mode;
} L16ChallengePlayer;

typedef struct {
    float target[4];
    float eye[4];
    float delta[4];
} L16ChallengeScratch;

extern char                     gadgetron_ceo_challenge_start_pose[] __asm__("D_L16_001D9840");
extern char                     gadgetron_ceo_challenge_return_pose[] __asm__("D_L16_001D9860");
extern int                      freeze_stats_score_ref_b[] __asm__("D_0015EE6C") MACRO_ADDR;
extern volatile int             D_0015EE6C_p __asm__("D_0015EE6C");
extern float                    D_L16_0015F638;
extern int                      D_0015EEA4;
extern unsigned char D_0013D4DE NOT_SDA;
extern unsigned char D_0013D507 NOT_SDA;
extern unsigned char            D_0013D408[];
extern void                     FUN_L00_00263d40(int, int);
extern void                     FUN_L16_002923d0(int);
extern void begin_dialogue_camera_transition(float, void *) __asm__("FUN_L01_002783a8");
void activate_gadgetron_ceo_challenge_environment(struct Moby *moby) __asm__("FUN_L16_002e70c0");

extern void activate_gadgetron_ceo_challenge_environment(struct Moby *) __asm__("FUN_L16_002e70c0");
void restore_gadgetron_ceo_non_challenge_environment(struct Moby *moby) __asm__("FUN_L16_002e7168");

extern void
restore_gadgetron_ceo_non_challenge_environment(struct Moby *) __asm__("FUN_L16_002e7168");
extern char               D_00141968[];
extern L16ChallengePlayer D_0013E633_challenge __asm__("D_0013E533");

void gadgetron_ceo_update(L16ChallengeMoby *m) __asm__("FUN_L16_002e6808");

void gadgetron_ceo_update(L16ChallengeMoby *ceo_moby) {
    enum {
        GADGETRON_CEO_INITIALIZE_DIALOGUE = 0,
        GADGETRON_CEO_POLL_DIALOGUE = 1,
        GADGETRON_CEO_WAIT_AFTER_DIALOGUE = 2,
        GADGETRON_CEO_MONITOR_CHALLENGE_EVENTS = 3,
        GADGETRON_CEO_WAIT_TO_SAVE = 4
    };
    L16ChallengeScratch     scratch;
    L16ChallengeData       *ceo_data = ceo_moby->data;
    L16ChallengeActivation *activation = (L16ChallengeActivation *)ceo_data->activation;
    float                   rate, head_rate;
    int                     tracking;
    if (ceo_moby->visible && fast_vec_dist(ceo_moby->position, D_L16_001671C0) < 28.0f) {
        FUN_L00_0025a120(ceo_moby);
        ceo_moby->distance_class = 22;
    }
    attach_big_head_cheat_manipulator_to_matching_moby(ceo_moby);
    switch (ceo_moby->state) {
    case GADGETRON_CEO_INITIALIZE_DIALOGUE:
        ceo_moby->opacity = 255;
        ceo_moby->state = 1;
        initialize_npc_dialogue(ceo_moby, activation);
        *(float *)((char *)ceo_data + 0x2C) = 3.7f;
        ceo_data->initial_yaw = ceo_moby->yaw;
        if (*(int *)(D_0013D408 + 0x814) == 0)
            ceo_moby->yaw = fast_add_rotations(ceo_moby->yaw, 3.14159265f);
        restore_gadgetron_ceo_non_challenge_environment((char *)ceo_moby);
        break;
    case GADGETRON_CEO_POLL_DIALOGUE: {
        if (((L16ChallengePlayer *)((char *)&D_0013E633_challenge + 0xE1D))->mode != 15)
            D_L16_0015F638 = 115.0f;
        else
            D_L16_0015F638 = 100.0f;
        if (update_npc_dialogue(ceo_moby, (char *)ceo_data + 0x20)) {
            ceo_moby->yaw = ceo_data->initial_yaw;
            begin_dialogue_camera_transition(3.5f, ceo_moby);
            ceo_moby->state = 2;
        }
        if (!D_0013D4DE && vector_distance_xy(&hero.motion.pos, ceo_moby->position) < 3.5f) {
            int   elapsed, period;
            char *statistics;
            elapsed = scale_game_frames(freeze_stats_score_ref_b[14]);
            period = 600;
            statistics = ((char *)&D_00141968);
            elapsed -= *(unsigned short *)(statistics + 0x43A) * period;
            if (elapsed > (int)((float)scale_game_frames(18) * 60.0f) ||
                *(unsigned short *)(statistics + 0x43A) * period == 0) {
                help_request(0x3E83, 0x87);
            } else if (scale_game_frames(D_0015EEA4) / 600 >
                       *(unsigned short *)(statistics + 0x43A)) {
                *(unsigned short *)(statistics + 0x43A) = scale_game_frames(D_0015EEA4) / 600;
            }
        }
        if (activation->mode == 3) {
            char *camera;
            char *pose;
            int   pending;
            pending = D_L16_0015F5CC;
            activation->previous = (unsigned short)activation->mode;
            activation->mode = 2;
            activation->pending = pending;
            ceo_moby->event = 0;
            pose = gadgetron_ceo_challenge_start_pose;
            set_race_camera(pose, pose + 0x10, 0, 1);
            camera = (char *)hero.saved_pos_rot;
            qcopy(camera, pose + 0x20);
            qcopy(camera + 0x10, pose + 0x30);
            ceo_moby->state = 3;
            activate_gadgetron_ceo_challenge_environment((char *)ceo_moby);
        }
        if (((L16ChallengePlayer *)((char *)&D_0013E633_challenge + 0xE1D))->mode == 22) {
            ceo_moby->state = 3;
            activate_gadgetron_ceo_challenge_environment((char *)ceo_moby);
        }
        break;
    }
    case GADGETRON_CEO_WAIT_AFTER_DIALOGUE:
        if (*(int *)&D_L16_0015F5C4 != 2) {
            int previous;
            ceo_moby->state = 1;
            previous = activation->previous;
            if (previous == 4) {
                FUN_L16_002923d0(0);
                ceo_moby->state = previous;
            }
        }
        break;
    case GADGETRON_CEO_WAIT_TO_SAVE:
        if (*(int *)&D_L16_0015F5C4 == 0) {
            ceo_moby->state = 1;
            grant_inventory_item(31, 1);
            memcard_save_data(0, -1);
        }
        break;
    case GADGETRON_CEO_MONITOR_CHALLENGE_EVENTS:
    default: {
        L16ChallengePlayer *player = (L16ChallengePlayer *)((char *)&D_0013E633_challenge + 0xE1D);
        unsigned char       event;
        if (player->points >= 4500) {
            unsigned char *flags = D_0013D408;
            if (!flags[0x1A]) {
                flags[0x1A] = 1;
                allocate_voice_for_bank_entry(1, 0, 0);
                FUN_L00_00263d40(0x53D6, -1);
            }
            if (D_0015EE6C_p < 4500)
                D_0015EE6C_p = player->points;
        }
        if (((L16ChallengePlayer *)((char *)&D_0013E633_challenge + 0xE1D))->mode == 0) {
            ceo_moby->state = 1;
            restore_gadgetron_ceo_non_challenge_environment((char *)ceo_moby);
        }
        event = ceo_moby->event;
        if (event == 1) {
            ceo_moby->state = event;
            set_race_camera(gadgetron_ceo_challenge_return_pose,
                            gadgetron_ceo_challenge_return_pose + 0x10,
                            0,
                            1);
            ceo_moby->event = 0;
            if (!D_0013D507) {
                int            pending;
                unsigned short previous;
                previous = activation->mode;
                pending = D_L16_0015F5CC;
                activation->previous = previous;
                activation->mode = 4;
                activation->pending = pending;
                activation->flag = event;
            }
            restore_gadgetron_ceo_non_challenge_environment((char *)ceo_moby);
        }
        if (ceo_moby->event == 2) {
            ceo_moby->state = 1;
            set_race_camera(gadgetron_ceo_challenge_return_pose,
                            gadgetron_ceo_challenge_return_pose + 0x10,
                            0,
                            1);
            ceo_moby->event = 0;
            restore_gadgetron_ceo_non_challenge_environment((char *)ceo_moby);
        }
        if (ceo_moby->event == 3) {
            char *camera;
            char *pose = gadgetron_ceo_challenge_start_pose;
            ceo_moby->event = 0;
            set_race_camera(pose, pose + 0x10, 0, 1);
            camera = (char *)hero.saved_pos_rot;
            qcopy(camera, pose + 0x20);
            qcopy(camera + 0x10, pose + 0x30);
        }
        break;
    }
    }
    rate = 0.02f;
    head_rate = 0.3f;
    tracking = 0;
    if (ceo_moby->animation == 0) {
        char *player = (char *)&D_0013E633_challenge + 0xE9D;
        tracking = 1;
        if (vector_distance_xy(ceo_moby->position, player) < 8.0f &&
            fast_difference_between_rotations(
                ceo_moby->yaw,
                fast_arc_tan(
                    ((L16ChallengePlayer *)((char *)&D_0013E633_challenge + 0xE1D))->aim[0] -
                        ceo_moby->position[0],
                    ((L16ChallengePlayer *)((char *)&D_0013E633_challenge + 0xE1D))->aim[1] -
                        ceo_moby->position[1])) < 1.5707964f) {
            if (fast_vec_length(player + 0x80) > 0.01f)
                ceo_data->moving_timer = scale_game_frames(120);
            else
                fast_dec_timer(&ceo_data->moving_timer);
        } else if (ceo_data->moving_timer) {
            ceo_data->moving_timer = 0;
            qcopy(ceo_data->target, (char *)&D_0013E633_challenge + 0xEED);
        }
        if (fast_dec_timer(&ceo_data->target_timer)) {
            float heading;
            ceo_data->target_timer =
                truncate_float_to_s32(scale_time(random_float_between(180.0f, 300.0f)));
            heading =
                fast_add_rotations(ceo_moby->yaw, random_float_between(-90.0f, 90.0f) * DEG_TO_RAD);
            build_spherical_offset(
                ceo_data->target, 6.0f, heading, random_float_between(0.0f, 30.0f) * DEG_TO_RAD);
            fast_vec_add(ceo_data->target, ceo_data->target, ceo_moby->position);
        }
        if (ceo_data->moving_timer) {
            qcopy(scratch.target, (char *)&D_0013E633_challenge + 0xEED);
            rate = 0.04f;
            head_rate = 0.3f;
        } else {
            qcopy(scratch.target, ceo_data->target);
        }
    }
    if (tracking) {
        float yaw;
        float pitch;
        qcopy(scratch.eye, ceo_moby->position);
        scratch.eye[2] += 1.0f;
        fast_vec_sub(scratch.delta, scratch.target, scratch.eye);
        yaw = fast_subtract_rotations(fast_arc_tan(scratch.delta[0], scratch.delta[1]),
                                      ceo_moby->yaw);
        pitch = -fast_arc_tan(vector_length_xy(scratch.delta), scratch.delta[2]);
        if (yaw > 1.5707964f)
            yaw = 1.5707964f;
        else if (yaw < -1.5707964f)
            yaw = -1.5707964f;
        if (pitch > 0.5235988f)
            pitch = 0.5235988f;
        else if (pitch < -0.5235988f)
            pitch = -0.5235988f;
        ceo_data->pitch = pitch;
        ceo_data->yaw = yaw * 0.7f;
        ceo_data->head_yaw = yaw * 0.3f;
    }
    if (big_head_cheat_enabled)
        *(float *)((char *)ceo_data + 0x150) = 2.75f;
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, ceo_moby, ceo_data->body, 0);
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, ceo_moby, ceo_data->head, 1);
}
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002E8538), where it is exact; names translated to the US level program. */

extern f32  D_L16_0015F638;
extern void request_music_track(int, int) __asm__("FUN_L01_0027a248");
extern void jet_group_state(int, int, int, int) __asm__("FUN_L13_002650a8");

void activate_gadgetron_ceo_challenge_environment(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    int  *group_index_entry = (int *)(data + 0x60);
    int   i;
    for (i = 15; i >= 0; i--) {
        jet_group_state(*group_index_entry, 1, 1, 1);
        group_index_entry++;
    }
    group_index_entry = (int *)(data + 0xA0);
    for (i = 7; i >= 0; i--) {
        jet_group_state(*group_index_entry, 0, 0, 0);
        group_index_entry++;
    }
    *(float *)&D_L16_0015F638 = 74.0f;
    challenge_set_layout_param(76.0f);
    request_music_track(2, 6);
}
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002E85E0), where it is exact; names translated to the US level program. */

void restore_gadgetron_ceo_non_challenge_environment(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    int  *group_index_entry;
    int   i;
    group_index_entry = (int *)(data + 0x60);
    for (i = 15; i >= 0; i--) {
        jet_group_state(group_index_entry[0], 0, 0, 0);
        group_index_entry++;
    }
    group_index_entry = (int *)(data + 0xA0);
    for (i = 7; i >= 0; i--) {
        jet_group_state(group_index_entry[0], 1, 1, 0);
        group_index_entry++;
    }
    challenge_set_layout_param(100.0f);
    request_music_track(0, 8);
}
