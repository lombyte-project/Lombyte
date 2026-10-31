/* Level 16 help_desk_lady. */
#include "types.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"

#include "qcopy.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/entities.h"
#include "rnc/overlay/watch.h"
#include "rnc/game/item_state.h"
#include "rnc/game/options.h"
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
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/animation/set_moby_animation.h"
#include "rnc/overlay/shared/cutscenes/update_npc_dialogue.h"
#include "rnc/overlay/shared/game/restore_or_clear_level_state.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/ui/text/font/font_queue_vu_state.h"

/* Activate the watcher and steer its body and head toward the player. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E45F8), where it is exact; names translated to the US level program. */

typedef struct {
    char  pad00[0xC];
    float radius;
    char  pad10[0x30];
} L16WatchActivation;

typedef struct {
    char               pad00[0x20];
    L16WatchActivation activation;
    char               body[0x60];
    float              padC0;
    float              pitch;
    float              yaw;
    float              padCC;
    float              height;
    char               padD4[0xC];
    char               head[0x60];
    char               pad140[8];
    float              head_yaw;
    char               pad14C[0x14];
    int                activation_region;
    int                entry_region;
    int                moving_timer;
    int                target_timer;
    float              target[4];
} L16WatchData;

typedef struct {
    char          pad00[0x10];
    float         position[4];
    unsigned char state;
    char          pad21[0xF];
    unsigned char opacity;
    char          pad31[0x17];
    float         yaw;
    char          pad4C[7];
    unsigned char animation;
    char          pad54[0x24];
    L16WatchData *data;
    char          pad7C[0x34];
    unsigned char widget;
} L16WatchMoby;

typedef struct {
    char          pad00[0x70];
    unsigned char entered;
    unsigned char mission;
} L16WatchFlags;

float D_L16_001D96C0[8] = {300.73f, 206.04f, 123.22f, 0.0f, 0.0f, 0.11f, 0.0f, 0.0f};

extern float scale_time(float) __asm__("FUN_001f96b0");
extern float fast_add_rotations(float, float);
extern float fast_subtract_rotations(float, float);
extern float vector_length_xy(void *);
extern int   is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");
extern int   scale_game_frames(int);
extern s32   D_L16_0015F5C4;
extern void  set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern void  update_joint_animation(float, float, void *, void *, int) __asm__("FUN_L00_002628d8");
extern void  apply_npc_big_head_cheat(char *) __asm__("FUN_L12_002e6b60");
extern void  build_spherical_offset(void *, float, float, float) __asm__("FUN_00214db0");
extern L16WatchPlayer D_0013E533;
extern L16WatchFlags  D_0013D355_watch __asm__("D_0013D24D");

void help_desk_lady_update(L16WatchMoby *m) __asm__("FUN_L16_002e3190");

/* Moby class 1377 */
void help_desk_lady_update(L16WatchMoby *moby) {
    enum {
        HELP_DESK_INITIALIZE_DIALOGUE = 0,
        HELP_DESK_POLL_DIALOGUE = 1,
        HELP_DESK_REQUEST_HELP_AND_SAVE = 2
    };
    L16WatchScratch gaze_vectors;
    L16WatchData   *watch_data = moby->data;
    L16WatchFlags  *flags;
    float           rate;
    float           head_rate;
    int             tracking;
    void           *activation;
    apply_npc_big_head_cheat((char *)moby);
    activation = &watch_data->activation;
    switch (moby->state) {
    case HELP_DESK_INITIALIZE_DIALOGUE:
        initialize_npc_dialogue(moby, activation);
        moby->state = 1;
        watch_data->activation.radius = 20.0f;
        moby->opacity = 0xFF;
        break;
    case HELP_DESK_POLL_DIALOGUE: {
        L16WatchPlayer *player = (L16WatchPlayer *)((char *)&D_0013E533 + 0xE1D);
        if (player->disabled == 0 && player->mode != 15 && player->mode != 22) {
            if (is_point_inside_clip_volume((char *)player + 0x80, watch_data->activation_region) &&
                update_npc_dialogue(moby, activation))
                moby->state = 2;
        }
        break;
    }
    case HELP_DESK_REQUEST_HELP_AND_SAVE:
        if (*(int *)&D_L16_0015F5C4 != 2) {
            char *player;
            moby->state = 1;
            set_race_camera(D_L16_001D96C0, D_L16_001D96C0 + 4, 0, 1);
            help_request(0x3E82, 0x84);
            grant_inventory_item(0x21, 1);
            jet_clear_persistent(moby->widget);
            player = (char *)&D_0013E533 + 0xE9D;
            jet_reset_camera(player, player + 0x10);
            memcard_save_data(0, -1);
            moby->state = 3;
        }
        break;
    }
    flags = (L16WatchFlags *)((char *)&D_0013D355_watch + 0x13B);
    if (!flags->entered &&
        is_point_inside_clip_volume((char *)&D_0013E533 + 0xE9D, watch_data->entry_region))
        flags->entered = 1;
    if (!((L16WatchFlags *)((char *)&D_0013D355_watch + 0x13B))->mission &&
        hero.state.control_mode == 15)
        ((L16WatchFlags *)((char *)&D_0013D355_watch + 0x13B))->mission = 1;
    rate = 0.02f;
    head_rate = 0.3f;
    tracking = 0;
    if (moby->animation == 0) {
        char *player = (char *)&D_0013E533 + 0xE9D;
        tracking = 1;
        if (vector_distance_xy(moby->position, player) < 8.0f &&
            fast_difference_between_rotations(
                moby->yaw,
                fast_arc_tan(((L16WatchPlayer *)((char *)&D_0013E533 + 0xE1D))->aim[0] -
                                 moby->position[0],
                             ((L16WatchPlayer *)((char *)&D_0013E533 + 0xE1D))->aim[1] -
                                 moby->position[1])) < 1.5707964f) {
            if (fast_vec_length(player + 0x80) > 0.01f)
                watch_data->moving_timer = scale_game_frames(120);
            else
                fast_dec_timer(&watch_data->moving_timer);
        } else if (watch_data->moving_timer) {
            watch_data->moving_timer = 0;
            qcopy(watch_data->target, (char *)&D_0013E533 + 0xEED);
        }
        if (fast_dec_timer(&watch_data->target_timer)) {
            float heading;
            watch_data->target_timer =
                truncate_float_to_s32(scale_time(random_float_between(180.0f, 300.0f)));
            heading =
                fast_add_rotations(moby->yaw, random_float_between(-90.0f, 90.0f) * DEG_TO_RAD);
            build_spherical_offset(
                watch_data->target, 6.0f, heading, random_float_between(0.0f, 30.0f) * DEG_TO_RAD);
            fast_vec_add(watch_data->target, watch_data->target, moby->position);
        }
        if (watch_data->moving_timer) {
            qcopy(gaze_vectors.target, (char *)&D_0013E533 + 0xEED);
            rate = 0.04f;
            head_rate = 0.3f;
        } else {
            qcopy(gaze_vectors.target, watch_data->target);
        }
    }
    if (tracking) {
        float yaw;
        float pitch;
        qcopy(gaze_vectors.eye, moby->position);
        gaze_vectors.eye[2] += 1.5f;
        fast_vec_sub(gaze_vectors.delta, gaze_vectors.target, gaze_vectors.eye);
        yaw = fast_subtract_rotations(fast_arc_tan(gaze_vectors.delta[0], gaze_vectors.delta[1]),
                                      moby->yaw);
        pitch = -fast_arc_tan(vector_length_xy(gaze_vectors.delta), gaze_vectors.delta[2]);
        if (yaw > 1.5707964f)
            yaw = 1.5707964f;
        else if (yaw < -1.5707964f)
            yaw = -1.5707964f;
        if (pitch > 0.5235988f)
            pitch = 0.5235988f;
        else if (pitch < -0.5235988f)
            pitch = -0.5235988f;
        watch_data->pitch = pitch;
        watch_data->yaw = yaw * 0.7f;
        watch_data->head_yaw = yaw * 0.3f;
    }
    if (big_head_cheat_enabled)
        watch_data->height = 2.75f;
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, moby, watch_data->body, 0);
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, moby, watch_data->head, 2);
}
