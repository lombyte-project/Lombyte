/* Level 11 moby class fred_pokitaru: the Pokitaru sentry. */
#include "types.h"

#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/mobys/moby.h"
#include "rnc/game/item_state.h"
#include "rnc/game/usage_stats.h"
#include "rnc/data_labels.h"
#include "rnc/game/options.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/ui/help/help_request.h"
#include "rnc/save/memcard_save_data.h"
#include "rnc/overlay/shared/cutscenes/update_npc_dialogue.h"
#include "rnc/overlay/shared/game/restore_or_clear_level_state.h"
#include "rnc/overlay/shared/mobys/attach_big_head_cheat_manipulator.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/watch.h"

void mark_moby_for_removal(struct Moby *obj) __asm__("FUN_0020c828");

#include "qcopy.h"

extern void  begin_dialogue_camera_transition(void *, float) __asm__("FUN_L01_002783a8");
extern int   D_L11_0015F5C4;
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern float scale_time(float) __asm__("FUN_001f96b0");
extern void  FUN_00214db0(void *, float, float, float);
extern void  update_joint_animation(float, float, void *, void *, int) __asm__("FUN_L00_002628d8");
extern float camera_pos_b358[] __asm__("D_L11_001677C0");
extern void  FUN_L00_0025a120(void *);
extern void  FUN_00212f90(void *, int, int, int);
extern int   D_0015ED84;
extern unsigned char D_0014C050[];

#include "rnc/overlay/cuboid.h"

/* Pokitaru sentry pvars (class of fred_pokitaru_update). */
struct PokSentryVars {
    char  pad00[4];
    short h04;
    char  pad06[6];
    float f0C;
    char  pad10[0x38];
    int   slot;
    char  pad4C[4];
    char  body[0x64];
    float pitch;
    float yaw;
    char  padBC[4];
    float height;
    char  padC4[0xC];
    char  head[0x68];
    float head_yaw;
    char  pad13C[0x14];
    float target[4];
    int   moving_timer;
    int   target_timer;
};

extern unsigned char D_0013D50B NOT_SDA;
extern struct UsageStat         help_records[] __asm__("D_00141968") NOT_SDA;

void fred_pokitaru_update(struct Moby *moby) __asm__("FUN_L11_002f0e40");

/* Moby class 298 */
void fred_pokitaru_update(struct Moby *moby) {
    enum {
        FRED_POKITARU_INITIALIZE = 0,
        FRED_POKITARU_CHECK_DIALOGUE = 1,
        FRED_POKITARU_WAIT_FOR_DIALOGUE_END = 2
    };
    L16WatchScratch       sentry_look_tracking_vectors;
    struct PokSentryVars *moby_vars = (struct PokSentryVars *)moby->pvars;
    float                 rate;
    float                 head_rate;
    int                   tracking;

    attach_big_head_cheat_manipulator_to_matching_moby(moby);
    if (moby->visible && fast_vec_dist(&moby->pos, camera_pos_b358) < 30.0f) {
        FUN_L00_0025a120(moby);
        moby->shadow_range = 24;
    }
    switch (moby->state) {
    case FRED_POKITARU_INITIALIZE:
        moby->state = 1;
        if (moby->prev_seq != 1)
            FUN_00212f90(moby, 1, 0, scale_game_frames(20));
        if (D_0013D50B) {
            mark_moby_for_removal(moby);
            return;
        }
        initialize_npc_dialogue(moby, moby_vars);
        moby_vars->f0C = 4.0f;
        break;
    case FRED_POKITARU_CHECK_DIALOGUE:
        if (update_npc_dialogue(moby, moby_vars)) {
            begin_dialogue_camera_transition(moby, 3.0f);
            moby->state = 2;
        }
        if (alternate_item_available[1] == 0 && discount_purchase_pricing[0] == 0) {
            if (fast_vec_dist(&moby->pos, hero.motion.pos.f) < 4.0f) {
                int help_frame_delta =
                    scale_game_frames(D_0015EEA4) - help_records[0x86].unk2 * 600;
                if ((int)(scale_game_frames(18) * 60.0f) < help_frame_delta ||
                    help_records[0x86].unk2 * 600 == 0) {
                    help_request(0x2B03, 0x86);
                } else if (scale_game_frames(D_0015EEA4) / 600 > help_records[0x86].unk2) {
                    help_records[0x86].unk2 = scale_game_frames(D_0015EEA4) / 600;
                }
            }
        }
        if (D_0014C050[moby->mission + D_0015ED84 * 16] != 0xFF &&
            fast_vec_dist(hero.motion.pos.f, &moby->pos) < 4.0f &&
            absolute_float(hero.motion.pos.f[2] - moby->pos.z) < 4.0f) {
            char *camera_reset_path_point;
            jet_clear_persistent(moby->mission);
            camera_reset_path_point = (char *)l11_cuboids + moby_vars->slot * 128;
            jet_reset_camera(camera_reset_path_point + 0x30, camera_reset_path_point + 0x70);
        }
        break;
    case FRED_POKITARU_WAIT_FOR_DIALOGUE_END:
        if (D_L11_0015F5C4 != 2) {
            moby->state = 1;
            if (moby_vars->h04 == 2) {
                alternate_item_available[1] = 0;
                discount_purchase_pricing[0] = 1;
                D_0013D50B = 1;
                queue_dialogue_message(0x2B06, scale_game_frames(300));
                memcard_save_data(0, -1);
                help_request(0x2AF9, 0x3B);
            }
        }
        break;
    }
    rate = 0.02f;
    head_rate = 0.3f;
    tracking = 0;
    if (moby->prev_seq == 1) {
        char *player = D_0013E533 + 0xE9D;
        tracking = 1;
        if (vector_distance_xy(&moby->pos, player) < 8.0f &&
            fast_difference_between_rotations(
                moby->rot.z,
                fast_arc_tan(((L16WatchPlayer *)(D_0013E533 + 0xE1D))->aim[0] - moby->pos.x,
                             ((L16WatchPlayer *)(D_0013E533 + 0xE1D))->aim[1] - moby->pos.y)) <
                1.5707964f) {
            if (fast_vec_length(player + 0x80) > 0.01f)
                moby_vars->moving_timer = scale_game_frames(120);
            else
                fast_dec_timer(&moby_vars->moving_timer);
        } else if (moby_vars->moving_timer) {
            moby_vars->moving_timer = 0;
            qcopy(moby_vars->target, hero.motion.body_center.f);
        }
        if (fast_dec_timer(&moby_vars->target_timer)) {
            float heading;
            moby_vars->target_timer =
                truncate_float_to_s32(scale_time(random_float_between(180.0f, 300.0f)));
            heading =
                fast_add_rotations(moby->rot.z, random_float_between(-90.0f, 90.0f) * DEG_TO_RAD);
            FUN_00214db0(
                moby_vars->target, 6.0f, heading, random_float_between(0.0f, 30.0f) * DEG_TO_RAD);
            fast_vec_add(moby_vars->target, moby_vars->target, &moby->pos);
        }
        if (moby_vars->moving_timer) {
            qcopy(sentry_look_tracking_vectors.target, hero.motion.body_center.f);
            rate = 0.04f;
            head_rate = 0.3f;
        } else {
            qcopy(sentry_look_tracking_vectors.target, moby_vars->target);
        }
    }
    if (tracking) {
        float yaw;
        float pitch;
        qcopy(sentry_look_tracking_vectors.eye, &moby->pos);
        sentry_look_tracking_vectors.eye[2] += 1.0f;
        fast_vec_sub(sentry_look_tracking_vectors.delta,
                     sentry_look_tracking_vectors.target,
                     sentry_look_tracking_vectors.eye);
        yaw = fast_subtract_rotations(fast_arc_tan(sentry_look_tracking_vectors.delta[0],
                                                   sentry_look_tracking_vectors.delta[1]),
                                      moby->rot.z);
        pitch = -fast_arc_tan(vector_length_xy(sentry_look_tracking_vectors.delta),
                              sentry_look_tracking_vectors.delta[2]);
        if (yaw > 1.5707964f)
            yaw = 1.5707964f;
        else if (yaw < -1.5707964f)
            yaw = -1.5707964f;
        if (pitch > 0.5235988f)
            pitch = 0.5235988f;
        else if (pitch < -0.5235988f)
            pitch = -0.5235988f;
        moby_vars->pitch = pitch;
        moby_vars->head_yaw = moby_vars->yaw = yaw * 0.5f;
    }
    if (big_head_cheat_enabled)
        moby_vars->height = 2.75f;
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, moby, moby_vars->body, 0);
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, moby, moby_vars->head, 1);
}
