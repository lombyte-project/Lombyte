/* Level 05 moby class shady_salesman. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/game/options.h"
#include "rnc/math_consts.h"
#include "rnc/overlay/watch.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/cutscenes/update_npc_dialogue.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/mobys/attach_big_head_cheat_manipulator.h"

extern char  D_L05_001671C0[];
extern float scale_time(float) __asm__("FUN_001f96b0");
extern void  begin_dialogue_camera_transition(void *, float) __asm__("FUN_L01_002783a8");
extern void  memcard_save_data(int, int) __asm__("FUN_0020b178");
extern void  update_joint_animation(float, float, void *, void *, int) __asm__("FUN_L00_002628d8");

/* Activate the watcher and steer its body and head toward the player. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E45F8), where it is exact; names translated to the US level program. */

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

extern unsigned char D_0013D4FF NOT_SDA;
extern void                     FUN_L00_0025a120(void *);
extern float                    fast_add_rotations(float, float);
extern float                    fast_subtract_rotations(float, float);
extern float                    vector_length_xy(void *);
extern int                      scale_game_frames(int);
extern char                     shady_salesman_dialogue_name[] __asm__("D_L05_00161EF8");
extern void                     queue_dialogue_message(int, int) __asm__("FUN_L00_00263d40");
extern L16WatchPlayer           D_0013E633_watch __asm__("D_0013E533");

void shady_salesman_update(WM *m) __asm__("FUN_L05_003180a0");

void shady_salesman_update(WM *moby) {
    enum {
        SHADY_SALESMAN_INITIALIZE = 0,
        SHADY_SALESMAN_UPDATE_DIALOGUE = 1
    };
    L16WatchScratch scratch;
    WD             *npc_data = moby->data;
    float           rate;
    float           head_rate;
    int             tracking;
    attach_big_head_cheat_manipulator_to_matching_moby((char *)moby);
    if (moby->opacity) {
        if (fast_vec_dist(moby->position, D_L05_001671C0) < 40.0f) {
            FUN_L00_0025a120(moby);
            moby->b7F = 0x20;
        }
    }
    switch (moby->state) {
    case SHADY_SALESMAN_INITIALIZE:
        if (D_0013D4FF) {
            mark_moby_for_removal(moby);
            return;
        }
        moby->position[2] += 0.5f;
        moby->position[2] = probe_ground_height(moby->position, 0, 0.5f);
        npc_data->ptr20 = (void *)shady_salesman_dialogue_name;
        moby->state = 1;
        initialize_npc_dialogue(moby, npc_data);
        break;
    case SHADY_SALESMAN_UPDATE_DIALOGUE:
        if (update_npc_dialogue(moby, npc_data))
            begin_dialogue_camera_transition(moby, 2.0f);
        if ((unsigned)(npc_data->sub - 3) < 2) {
            grant_inventory_item(0x17, 1);
            queue_dialogue_message(0x1396, -1);
            memcard_save_data(0, -1);
            mark_moby_for_removal(moby);
            return;
        }
        if (moby->animation == 0 && fast_dec_timer((char *)npc_data + 0x154)) {
            npc_data->idle_timer =
                truncate_float_to_s32(scale_time(random_float_between(1200.0f, 2400.0f)));
            if (moby->animation != 1)
                blend_moby_animation(moby, 1, 0, scale_game_frames(10));
        } else if (moby->animation == 1 && (moby->flags70 & 2)) {
            blend_moby_animation(moby, 0, 0, scale_game_frames(10));
        }
        break;
    }
    rate = 0.02f;
    head_rate = 0.3f;
    tracking = 0;
    if (moby->animation == 0) {
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
                npc_data->moving_timer = scale_game_frames(120);
            else
                fast_dec_timer(&npc_data->moving_timer);
        } else if (npc_data->moving_timer) {
            npc_data->moving_timer = 0;
            qcopy(npc_data->target, (char *)&D_0013E633_watch + 0xEED);
        }
        if (fast_dec_timer(&npc_data->target_timer)) {
            float heading;
            npc_data->target_timer =
                truncate_float_to_s32(scale_time(random_float_between(180.0f, 300.0f)));
            heading =
                fast_add_rotations(moby->yaw, random_float_between(-90.0f, 90.0f) * DEG_TO_RAD);
            build_spherical_offset(
                npc_data->target, 6.0f, heading, random_float_between(0.0f, 30.0f) * DEG_TO_RAD);
            fast_vec_add(npc_data->target, npc_data->target, moby->position);
        }
        if (npc_data->moving_timer) {
            qcopy(scratch.target, (char *)&D_0013E633_watch + 0xEED);
            rate = 0.04f;
            head_rate = 0.3f;
        } else {
            qcopy(scratch.target, npc_data->target);
        }
    }
    if (tracking) {
        float yaw;
        float pitch;
        qcopy(scratch.eye, moby->position);
        scratch.eye[2] += 1.0f;
        fast_vec_sub(scratch.delta, scratch.target, scratch.eye);
        yaw = fast_subtract_rotations(fast_arc_tan(scratch.delta[0], scratch.delta[1]), moby->yaw);
        pitch = -fast_arc_tan(vector_length_xy(scratch.delta), scratch.delta[2]);
        if (yaw > 1.5707964f)
            yaw = 1.5707964f;
        else if (yaw < -1.5707964f)
            yaw = -1.5707964f;
        if (pitch > 0.5235988f)
            pitch = 0.5235988f;
        else if (pitch < -0.5235988f)
            pitch = -0.5235988f;
        npc_data->pitch = pitch;
        npc_data->yaw = yaw * 0.6f;
        npc_data->head_yaw = yaw * 0.4f;
    }
    if (big_head_cheat_enabled)
        npc_data->height = 2.75f;
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, moby, npc_data->body, 0);
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, moby, npc_data->head, 1);
}
