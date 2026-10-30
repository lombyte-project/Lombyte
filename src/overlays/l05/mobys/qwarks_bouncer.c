/* Level 05 moby class qwarks_bouncer. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/game/options.h"
#include "rnc/math_consts.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/cutscenes/update_npc_dialogue.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/mobys/initialize_moby_grounding.h"
#include "rnc/overlay/shared/mobys/attach_big_head_cheat_manipulator.h"

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
} WD_c;

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
    WD_c         *data;
    char          pad7C[3];
    unsigned char b7F;
} WM_c;

typedef struct {
    char  pad00[0x80];
    float position[4];
    char  pad90[0x40];
    float aim[4];
    char  padE0[0x22E];
    short disabled;
    char  pad310[0x1D7C];
    int   mode;
} L16WatchPlayer_c;

typedef struct {
    float target[4];
    float eye[4];
    float delta[4];
} L16WatchScratch_c;

extern void  begin_dialogue_camera_transition(void *, float) __asm__("FUN_L01_002783a8");
extern void  memcard_save_data(int, int) __asm__("FUN_0020b178");
extern float FUN_001f96b0_c(float) __asm__("FUN_001f96b0");
extern float fast_add_rotations_c(float, float) __asm__("fast_add_rotations");
extern float fast_subtract_rotations_c(float, float) __asm__("fast_subtract_rotations");
extern float vector_length_xy_c(void *) __asm__("vector_length_xy");
extern int   scale_game_frames_c(int) __asm__("scale_game_frames");
extern void  update_joint_animation(float, float, void *, void *, int) __asm__("FUN_L00_002628d8");
extern L16WatchPlayer_c D_0013E633_watch_c __asm__("D_0013E533");

typedef struct {
    char  pad00[4];
    short sub;
    char  pad06[6];
    float fC;
    char  pad10[0x10];
    void *ptr20;
    char  pad24[0x1C];
    char  body[0x64];
    float pitch;
    float yaw;
    char  padAC[4];
    float height;
    char  padB4[0xC];
    char  head[0x68];
    float head_yaw;
    char  pad12C[0x18];
    int   idle_timer;
    int   moving_timer;
    int   target_timer;
    float target[4];
} WD2;

extern char qwarks_bouncer_dialogue_name[] __asm__("D_L05_00161EF0");

extern unsigned char D_0013DD47 NOT_SDA;
/* Watcher moby update (variant of shady_salesman_update): turns its head toward the hero or a random target. */
void       qwarks_bouncer_update(WM_c *m) __asm__("FUN_L05_00317470");
extern int scale_game_frames(int);

void qwarks_bouncer_update(WM_c *bouncer_moby) {
    enum {
        QWARKS_BOUNCER_INITIALIZE = 0,
        QWARKS_BOUNCER_UPDATE_DIALOGUE = 1
    };
    L16WatchScratch_c scratch;
    WD2              *bouncer_data = (WD2 *)bouncer_moby->data;
    float             rate;
    float             head_rate;
    int               tracking;
    attach_big_head_cheat_manipulator(bouncer_moby);
    if (!bouncer_data)
        return;
    switch (bouncer_moby->state) {
    case QWARKS_BOUNCER_INITIALIZE:
        if (D_0013DD47) {
            mark_moby_for_removal(bouncer_moby);
            return;
        }
        bouncer_data->ptr20 = (void *)qwarks_bouncer_dialogue_name;
        bouncer_moby->state = 1;
        initialize_npc_dialogue(bouncer_moby, bouncer_data);
        bouncer_data->fC = 3.7f;
        initialize_moby_grounding(bouncer_moby);
        break;
    case QWARKS_BOUNCER_UPDATE_DIALOGUE:
        if (update_npc_dialogue(bouncer_moby, bouncer_data))
            begin_dialogue_camera_transition(bouncer_moby, 2.5f);
        if (bouncer_data->sub == 5) {
            unlock_planet(7);
            memcard_save_data(0, -1);
            mark_moby_for_removal(bouncer_moby);
            return;
        }
        if (bouncer_moby->animation == 0 && fast_dec_timer(&bouncer_data->idle_timer)) {
            int idle_animation = 2;
            bouncer_data->idle_timer =
                truncate_float_to_s32(FUN_001f96b0_c(random_float_between(1200.0f, 2400.0f)));
            if (random_integer_below(2))
                idle_animation = 1;
            if (bouncer_moby->animation != idle_animation)
                blend_moby_animation(bouncer_moby, idle_animation, 0, scale_game_frames(10));
        } else if ((bouncer_moby->flags70 & 2) && bouncer_moby->animation) {
            blend_moby_animation(bouncer_moby, 0, 0, scale_game_frames(10));
        }
        break;
    }
    rate = 0.02f;
    head_rate = 0.3f;
    tracking = 0;
    if (bouncer_moby->animation == 0) {
        char *player = (char *)&D_0013E633_watch_c + 0xE9D;
        tracking = 1;
        if (vector_distance_xy(bouncer_moby->position, player) < 8.0f &&
            fast_difference_between_rotations(
                bouncer_moby->yaw,
                fast_arc_tan(((L16WatchPlayer_c *)((char *)&D_0013E633_watch_c + 0xE1D))->aim[0] -
                                 bouncer_moby->position[0],
                             ((L16WatchPlayer_c *)((char *)&D_0013E633_watch_c + 0xE1D))->aim[1] -
                                 bouncer_moby->position[1])) < 1.5707964f) {
            if (fast_vec_length(player + 0x80) > 0.01f)
                bouncer_data->moving_timer = scale_game_frames_c(120);
            else
                fast_dec_timer(&bouncer_data->moving_timer);
        } else if (bouncer_data->moving_timer) {
            bouncer_data->moving_timer = 0;
            qcopy(bouncer_data->target, (char *)&D_0013E633_watch_c + 0xEED);
        }
        if (fast_dec_timer(&bouncer_data->target_timer)) {
            float heading;
            bouncer_data->target_timer =
                truncate_float_to_s32(FUN_001f96b0_c(random_float_between(180.0f, 300.0f)));
            heading = fast_add_rotations_c(bouncer_moby->yaw,
                                           random_float_between(-90.0f, 90.0f) * DEG_TO_RAD);
            build_spherical_offset(bouncer_data->target,
                                   6.0f,
                                   heading,
                                   random_float_between(0.0f, 30.0f) * DEG_TO_RAD);
            fast_vec_add(bouncer_data->target, bouncer_data->target, bouncer_moby->position);
        }
        if (bouncer_data->moving_timer) {
            qcopy(scratch.target, (char *)&D_0013E633_watch_c + 0xEED);
            rate = 0.04f;
            head_rate = 0.3f;
        } else {
            qcopy(scratch.target, bouncer_data->target);
        }
    }
    if (tracking) {
        float yaw;
        float pitch;
        qcopy(scratch.eye, bouncer_moby->position);
        scratch.eye[2] += 1.0f;
        fast_vec_sub(scratch.delta, scratch.target, scratch.eye);
        yaw = fast_subtract_rotations_c(fast_arc_tan(scratch.delta[0], scratch.delta[1]),
                                        bouncer_moby->yaw);
        pitch = -fast_arc_tan(vector_length_xy_c(scratch.delta), scratch.delta[2]);
        if (yaw > 1.5707964f)
            yaw = 1.5707964f;
        else if (yaw < -1.5707964f)
            yaw = -1.5707964f;
        if (pitch > 0.5235988f)
            pitch = 0.5235988f;
        else if (pitch < -0.5235988f)
            pitch = -0.5235988f;
        bouncer_data->pitch = pitch;
        bouncer_data->head_yaw = bouncer_data->yaw = yaw * 0.5f;
    }
    if (big_head_cheat_enabled)
        bouncer_data->height = 2.75f;
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, bouncer_moby, bouncer_data->body, 0);
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, bouncer_moby, bouncer_data->head, 1);
}
