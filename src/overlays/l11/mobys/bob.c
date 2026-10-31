/* Level 11 moby class bob. */
#include "types.h"

#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/game/item_state.h"
#include "rnc/data_labels.h"
#include "rnc/game/options.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/ui/help/help_request.h"
#include "rnc/save/memcard_save_data.h"
#include "rnc/overlay/shared/cutscenes/update_npc_dialogue.h"
#include "rnc/overlay/shared/game/restore_or_clear_level_state.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/watch.h"
#include "rnc/overlay/l11/mobys/watcher.h"

#include "qcopy.h"

typedef struct {
    char  pad00[4];
    short h04;
    char  pad06[6];
    float f0C;
    char  pad10[0x30];
    int   region;
    char  pad44[0x2C];
    char  body[0x64];
    float pitch;
    float yaw;
    char  padDC[4];
    float height;
    char  padE4[0xC];
    char  head[0x68];
    float head_yaw;
    char  pad15C[0x14];
    float target[4];
    char  pad180[4];
    int   moving_timer;
    int   target_timer;
} WD11;

extern void attach_big_head_cheat_manipulator(void *) __asm__("FUN_L05_00317398");
extern int  FUN_00214720(void *, int);
extern void begin_dialogue_camera_transition(void *, float) __asm__("FUN_L01_002783a8");
extern void clear_active_help_record(void) __asm__("FUN_L00_002039a0");
extern unsigned char D_0013D4C3 NOT_SDA;
extern int                      D_L11_0015F5C4;
extern float                    random_float_between(float, float) __asm__("FUN_002132a8");
extern float                    scale_time(float) __asm__("FUN_001f96b0");
extern void                     FUN_00214db0(void *, float, float, float);
extern void update_joint_animation(float, float, void *, void *, int) __asm__("FUN_L00_002628d8");

/* Watcher update, same routine as commando_update with this level's states and data layout. */
void bob_update(WatcherMoby *m) __asm__("FUN_L11_002d0710");

/* Moby class 90 */
void bob_update(WatcherMoby *moby) {
    enum {
        BOB_INITIALIZE = 0,
        BOB_CHECK_DIALOGUE = 1,
        BOB_WAIT_FOR_DIALOGUE_END = 2
    };
    L16WatchScratch look_tracking_vectors;
    WD11           *moby_vars = (WD11 *)moby->data;
    float           rate;
    float           head_rate;
    int             tracking;
    attach_big_head_cheat_manipulator(moby);
    switch (moby->state) {
    case BOB_INITIALIZE:
        if (D_0013D4C3) {
            moby->state = 3;
        } else {
            moby->state = 1;
        }
        initialize_npc_dialogue(moby, moby_vars);
        moby_vars->f0C = 3.7f;
        break;
    case BOB_CHECK_DIALOGUE:
        if (FUN_00214720(hero.motion.pos.f, moby_vars->region)) {
            if (update_npc_dialogue(moby, moby_vars)) {
                begin_dialogue_camera_transition(moby, 2.7f);
                moby->state = 2;
            }
        }
        break;
    case BOB_WAIT_FOR_DIALOGUE_END:
        if (D_L11_0015F5C4 != 2) {
            moby->state = 1;
            if (moby_vars->h04 == 2) {
                grant_inventory_item(3, 1);
                jet_reset_camera(hero.motion.pos.f, hero.motion.pos.f + 4);
                memcard_save_data(0, -1);
                clear_active_help_record();
                help_request(0x2AFB, 0x3D);
            }
        }
        break;
    }
    rate = 0.02f;
    head_rate = 0.3f;
    tracking = 0;
    if (moby->animation == 0) {
        char *player = D_0013E533 + 0xE9D;
        tracking = 1;
        if (vector_distance_xy(moby->position, player) < 8.0f &&
            fast_difference_between_rotations(
                moby->yaw,
                fast_arc_tan(((L16WatchPlayer *)(D_0013E533 + 0xE1D))->aim[0] - moby->position[0],
                             ((L16WatchPlayer *)(D_0013E533 + 0xE1D))->aim[1] -
                                 moby->position[1])) < 1.5707964f) {
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
                fast_add_rotations(moby->yaw, random_float_between(-90.0f, 90.0f) * DEG_TO_RAD);
            FUN_00214db0(
                moby_vars->target, 6.0f, heading, random_float_between(0.0f, 30.0f) * DEG_TO_RAD);
            fast_vec_add(moby_vars->target, moby_vars->target, moby->position);
        }
        if (moby_vars->moving_timer) {
            qcopy(look_tracking_vectors.target, hero.motion.body_center.f);
            rate = 0.04f;
            head_rate = 0.3f;
        } else {
            qcopy(look_tracking_vectors.target, moby_vars->target);
        }
    }
    if (tracking) {
        float yaw;
        float pitch;
        qcopy(look_tracking_vectors.eye, moby->position);
        look_tracking_vectors.eye[2] += 1.0f;
        fast_vec_sub(
            look_tracking_vectors.delta, look_tracking_vectors.target, look_tracking_vectors.eye);
        yaw = fast_subtract_rotations(
            fast_arc_tan(look_tracking_vectors.delta[0], look_tracking_vectors.delta[1]),
            moby->yaw);
        pitch = -fast_arc_tan(vector_length_xy(look_tracking_vectors.delta),
                              look_tracking_vectors.delta[2]);
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
