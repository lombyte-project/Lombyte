/* Level 08 moby class 0x46a: commando. */
#include "types.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/overlay/watch.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"

#include "rnc/data_labels.h"
#include "rnc/game/options.h"
#include "rnc/game/game_globals.h"
#include "sda.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/cutscenes/update_npc_dialogue.h"
#include "rnc/overlay/shared/game/restore_or_clear_level_state.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/game/queue_planet_unlock_message.h"
#include "rnc/overlay/shared/mobys/attach_big_head_cheat_manipulator.h"
#include "qcopy.h"

typedef struct {
    char           pad00[4];
    unsigned short sub;
    char           pad06[0x1A];
    void          *ptr20;
    char           pad24[0x1C];
    char           body[0x64];
    float          pitch;
    float          yaw;
    char           padAC[4];
    float          height;
    char           padB4[0xC];
    char           head[0x68];
    float          head_yaw;
    char           pad12C[0x14];
    float          target[4];
    char           pad150[4];
    int            idle_timer;
    int            moving_timer;
    int            target_timer;
} WD;

typedef struct {
    char          pad00[0x10];
    float         position[4];
    unsigned char state;
    char          pad21[0x10];
    unsigned char opacity;
    char          pad32[0x16];
    float         yaw;
    char          pad4C[7];
    unsigned char animation;
    char          pad54[0x1C];
    unsigned char flags70;
    char          pad71[7];
    WD           *data;
    char          pad7C[3];
    unsigned char b7F;
} WM;

extern int   level_game_mode __asm__("D_L08_0015F5C4");
extern void  begin_dialogue_camera_transition(void *, float) __asm__("FUN_L01_002783a8");
extern void  memcard_save_data(int, int) __asm__("FUN_0020b178");
extern float scale_time(float) __asm__("FUN_001f96b0");
extern float vector_length_xy(void *);
extern void  update_joint_animation(float, float, void *, void *, int) __asm__("FUN_L00_002628d8");
extern L16WatchPlayer D_0013E633_watch __asm__("D_0013E533");

typedef struct {
    char          pad00[0x24];
    short         h24;
    char          pad26[2];
    unsigned char b28;
    char          pad29[3];
    float         f2C;
    char          pad30[0x10];
    void         *p40;
    char          pad44[0x1C];
    char          body[0x64];
    float         pitch;
    float         yaw;
    char          padCC[4];
    float         height;
    char          padD4[0xC];
    char          head[0x68];
    float         head_yaw;
    char          pad14C[0x14];
    float         target[4];
    char          pad170[0x10];
    int           joint;
    char          pad184[4];
    int           moving_timer;
    int           target_timer;
} WD8;

extern unsigned char D_0013D3B4 NOT_SDA;
extern char                     commando_name_string[] __asm__("D_L08_001621E0");
int          commando_height_probe_distance_limit_gp __asm__("D_L08_001621D8") = 0x18;
extern char *D_L08_001600EC;
extern float D_L08_001675C0_c[] __asm__("D_L08_001675C0");
extern void  FUN_L00_0025a120(void *);

/* Watcher update, same routine as batalia_deserter_update with two more states and the joint pose copy. */
void commando_update(WM *m) __asm__("FUN_L08_00302ce8");

/* Moby class 1130 */
void commando_update(WM *moby) {
    enum {
        COMMANDO_INITIALIZE = 0,
        COMMANDO_DIALOGUE_BEFORE_REPOSITION = 1,
        COMMANDO_DIALOGUE_AFTER_REPOSITION = 2,
        COMMANDO_POST_DIALOGUE = 3
    };
    L16WatchScratch look_tracking_vectors;
    WD8            *commando_vars = (WD8 *)moby->data;
    float           rate;
    float           head_rate;
    int             tracking;
    attach_big_head_cheat_manipulator_to_matching_moby(moby);
    if (moby->opacity) {
        if (fast_vec_dist(moby->position, D_L08_001675C0_c) <
            (float)commando_height_probe_distance_limit_gp) {
            FUN_L00_0025a120(moby);
            moby->b7F = (commando_height_probe_distance_limit_gp * 13) >> 4;
        }
    }
    switch (moby->state) {
    case COMMANDO_INITIALIZE:
        ((unsigned char *)moby)[0x30] = 0xFF;
        if (moby->animation != 1)
            blend_moby_animation(moby, 1, 0, scale_game_frames(10));
        if (D_0013DD4A &&
            visited52[((unsigned char *)moby)[0xB0] + current_level_index * 16] == 0xFF) {
            mark_moby_for_removal(moby);
            return;
        }
        commando_vars->f2C = 3.0f;
        commando_vars->p40 = commando_name_string;
        if (!D_0013D3B4) {
            moby->state = 1;
        } else {
            char *b = D_L08_001600EC;
            int   j = commando_vars->joint;
            qcopy(moby->position, b + j * 0x80 + 0x30);
            qcopy((char *)moby + 0x40, b + j * 0x80 + 0x70);
            moby->state = 2;
        }
        initialize_npc_dialogue(moby, (char *)commando_vars + 0x20);
        break;
    case COMMANDO_DIALOGUE_BEFORE_REPOSITION:
        if (update_npc_dialogue(moby, (char *)commando_vars + 0x20)) {
            char *b = D_L08_001600EC;
            int   j = commando_vars->joint;
            qcopy(moby->position, b + j * 0x80 + 0x30);
            qcopy((char *)moby + 0x40, b + j * 0x80 + 0x70);
            moby->state = 2;
            D_0013D3B4 = 1;
        }
        break;
    case COMMANDO_DIALOGUE_AFTER_REPOSITION:
        if (moby->flags70 & 2) {
            if (moby->animation != random_integer_below(2) + 1)
                blend_moby_animation(moby, random_integer_below(2) + 1, 0, scale_game_frames(20));
        }
        if (update_npc_dialogue(moby, (char *)commando_vars + 0x20)) {
            moby->state = 3;
            begin_dialogue_camera_transition(moby, 2.2f);
        }
        break;
    case COMMANDO_POST_DIALOGUE:
        if (level_game_mode != 2) {
            commando_vars->b28 = 1;
            if (commando_vars->h24 == 3) {
                unlock_planet(10);
                queue_planet_unlock_message(10);
                jet_clear_persistent(((unsigned char *)moby)[0xB0]);
                jet_reset_camera((char *)&D_0013E633_watch + 0xE9D,
                                 (char *)&D_0013E633_watch + 0xEAD);
                memcard_save_data(0, -1);
                mark_moby_for_removal(moby);
                return;
            }
            moby->state = 2;
        }
        break;
    }
    rate = 0.02f;
    head_rate = 0.3f;
    tracking = 0;
    if (moby->animation == 1) {
        char *player = (char *)&D_0013E633_watch + 0xE9D;
        tracking = 1;
        if (vector_distance_xy(moby->position, player) < 8.0f &&
            fast_difference_between_rotations(
                moby->yaw,
                fast_arc_tan(((L16WatchPlayer *)((char *)&D_0013E633_watch + 0xE1D))->aim[0] -
                                 moby->position[0],
                             ((L16WatchPlayer *)((char *)&D_0013E633_watch + 0xE1D))->aim[1] -
                                 moby->position[1])) < 1.5707964f) {
            if (fast_vec_length(player + 0x80) > 0.01f)
                commando_vars->moving_timer = scale_game_frames(120);
            else
                fast_dec_timer(&commando_vars->moving_timer);
        } else if (commando_vars->moving_timer) {
            commando_vars->moving_timer = 0;
            qcopy(commando_vars->target, (char *)&D_0013E633_watch + 0xEED);
        }
        if (fast_dec_timer(&commando_vars->target_timer)) {
            float heading;
            commando_vars->target_timer =
                truncate_float_to_s32(scale_time(random_float_between(180.0f, 300.0f)));
            heading =
                fast_add_rotations(moby->yaw, random_float_between(-90.0f, 90.0f) * DEG_TO_RAD);
            build_spherical_offset(commando_vars->target,
                                   6.0f,
                                   heading,
                                   random_float_between(0.0f, 30.0f) * DEG_TO_RAD);
            fast_vec_add(commando_vars->target, commando_vars->target, moby->position);
        }
        if (commando_vars->moving_timer) {
            qcopy(look_tracking_vectors.target, (char *)&D_0013E633_watch + 0xEED);
            rate = 0.04f;
            head_rate = 0.3f;
        } else {
            qcopy(look_tracking_vectors.target, commando_vars->target);
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
        commando_vars->pitch = pitch;
        commando_vars->yaw = yaw * 0.6f;
        commando_vars->head_yaw = yaw * 0.4f;
    }
    if (big_head_cheat_enabled)
        commando_vars->height = 2.75f;
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, moby, commando_vars->body, 0);
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, moby, commando_vars->head, 1);
}
