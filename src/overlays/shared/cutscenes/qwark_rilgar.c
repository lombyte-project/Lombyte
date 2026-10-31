/* Shared moby class qwark_rilgar (and qwark_umbris): turns its body and head toward the hero. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/game/options.h"
#include "rnc/math_consts.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/overlay/shared/cutscenes/update_npc_dialogue.h"
#include "qcopy.h"

typedef struct {
    float target[4];
    float eye[4];
    float delta[4];
} L03WatchScratch;

typedef struct {
    char          pad00[0x10];
    float         position[4];
    unsigned char state;
    char          pad21[0xF];
    unsigned char opacity;
    unsigned char near_flag;
    char          pad32[0x16];
    float         yaw;
    char          pad4C[7];
    unsigned char animation;
    char          pad54[0x24];
    char         *data;
    char          pad7C[3];
    unsigned char flag7F;
} L03WatchMoby;

typedef struct {
    char  pad00[0x64];
    float pitch;
    float yaw;
    char  pad6C[4];
    float height;
    char  pad74[0x74];
    float head_yaw;
    char  padEC[0x14];
    float target[4];
    char  pad110[8];
    int   moving_timer;
    int   target_timer;
    int   region_a;
    int   region_b;
    int   region_c;
} L03WatchData;

typedef struct {
    char  pad00[0xD0];
    float aim[4];
} L03Player;

extern float random_float_between(float, float) __asm__("FUN_002132a8");

extern void  debug_print() __asm__("FUN_001e93b0");
extern char  D_L03_00166EC0[];
extern void  FUN_0020c828(void *);
extern float scale_time(float) __asm__("FUN_001f96b0");
extern void  FUN_L00_0025a120(void *);

extern unsigned char *D_L03_0015FFD8 __attribute__((sda));
extern char           D_L03_001E3628[], D_L03_001E3660[], D_L03_001E3690[];
extern void           debug_print(void *) __asm__("FUN_001e93b0");
extern void update_joint_animation(float, float, void *, void *, int) __asm__("FUN_L00_002628d8");
extern void attach_cutscene_fixed_scale_manipulator(void *) __asm__("FUN_L01_002fb4b8");

/* Activate the watcher and steer its body and head toward the player. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E45F8), where it is exact; names translated to the US level program. */

/* Activate the watcher and steer its body and head toward the player. */

void qwark_rilgar_update(L03WatchMoby *m) __asm__("FUN_L03_002dc478");

/* Moby classes 447, 920 */
void qwark_rilgar_update(L03WatchMoby *moby) {
    enum {
        QWARK_RILGAR_INITIALIZE = 0,
        QWARK_RILGAR_WAIT_FOR_PLAYER_IN_REGION = 1,
        QWARK_RILGAR_POST_REGION_TRIGGER = 2
    };
    L03WatchScratch look_tracking_vectors;
    L03WatchData   *watch_data = (L03WatchData *)moby->data;
    float           rate;
    float           head_rate;
    int             tracking;
    attach_cutscene_fixed_scale_manipulator(moby);
    if (moby->near_flag) {
        if (fast_vec_dist(moby->position, D_L03_00166EC0) < 30.0f) {
            FUN_L00_0025a120(moby);
            moby->flag7F = 24;
        }
    }
    switch (moby->state) {
    case QWARK_RILGAR_INITIALIZE:
        moby->opacity = 0xFF;
        if (watch_data->region_a == -1)
            debug_print(D_L03_001E3628);
        if (watch_data->region_c == -1)
            debug_print(D_L03_001E3660);
        if (watch_data->region_b == -1) {
            debug_print(D_L03_001E3690);
        } else {
            unsigned char *moby_pool = D_L03_0015FFD8;
            if (moby_pool[watch_data->region_b * 256 + 0x20] != 0) {
                if (watch_data->region_c != -1)
                    moby_pool[watch_data->region_c * 256 + 0x20] = 3;
                FUN_0020c828(moby);
                return;
            }
        }
        moby->state = 1;
        break;
    case QWARK_RILGAR_WAIT_FOR_PLAYER_IN_REGION:
        if (watch_data->region_a != -1 &&
            is_point_inside_clip_volume(&hero.motion.pos, watch_data->region_a)) {
            set_moby_dialogue_record_value(moby, 1);
            moby->state = 2;
        }
        break;
    case QWARK_RILGAR_POST_REGION_TRIGGER:
        moby->state = 3;
        break;
    }
    rate = 0.02f;
    head_rate = 0.3f;
    tracking = 0;
    if (moby->animation == 0) {
        char      *hero_position = (char *)&hero.motion.pos;
        L03Player *player_aim_view;
        tracking = 1;
        if (vector_distance_xy(moby->position, hero_position) < 8.0f &&
            (player_aim_view = (L03Player *)(hero_position - 0x80),
             fast_difference_between_rotations(
                 moby->yaw,
                 fast_arc_tan(player_aim_view->aim[0] - moby->position[0],
                              player_aim_view->aim[1] - moby->position[1]))) < 1.5707964f) {
            if (fast_vec_length(hero_position + 0x80) > 0.01f)
                watch_data->moving_timer = scale_game_frames(120);
            else
                fast_dec_timer(&watch_data->moving_timer);
        } else if (watch_data->moving_timer) {
            watch_data->moving_timer = 0;
            qcopy(watch_data->target, hero.motion.body_center.f);
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
            qcopy(look_tracking_vectors.target, hero.motion.body_center.f);
            rate = 0.04f;
            head_rate = 0.3f;
        } else {
            qcopy(look_tracking_vectors.target, watch_data->target);
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
        watch_data->pitch = pitch;
        watch_data->yaw = yaw * 0.6f;
        watch_data->head_yaw = yaw * 0.4f;
    }
    if (big_head_cheat_enabled)
        watch_data->height = 2.75f;
    update_joint_animation(rate * frame_scale_sq, head_rate * frame_scale_sq, moby, watch_data, 0);
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, moby, (char *)watch_data + 0x80, 1);
}
