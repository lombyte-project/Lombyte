/* Level 12 moby class miner. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/game/options.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/overlay/watch.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/mobys/initialize_moby_grounding.h"
#include "rnc/overlay/l12/mobys/watcher.h"
#include "rnc/overlay/l12/mobys/miner.h"
#include "rnc/mobys/animation/build_moby_bone_transform.h"
#include "rnc/mobys/refresh_moby_spatial_bounds_from_basis.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/math/vectors/normalize_vector_triplet.h"

#include "qcopy.h"

extern int scale_game_frames(int);

#include "rnc/game/item_state.h"

extern float scale_time(float) __asm__("FUN_001f96b0");

extern float fast_subtract_rotations(float, float);
extern float vector_length_xy(void *);
extern void  begin_dialogue_camera_transition(void *, float) __asm__("FUN_L01_002783a8");
extern int   D_L12_0015F5C4 __asm__("D_L12_0015F5C4");
extern void  FUN_L00_00250df8_x(void *) __asm__("FUN_L00_00250df8");
extern void  queue_dialogue_message(int, int) __asm__("FUN_L00_00263d40");
extern int   update_npc_dialogue(void *, void *) __asm__("FUN_L00_00266448");
extern int   initialize_npc_dialogue(void *, void *) __asm__("FUN_L00_002668a0");
extern int   memcard_save_data(int, int) __asm__("FUN_0020b178");
extern void  jet_clear_persistent(int) __asm__("FUN_L00_002502a0");
extern void  update_joint_animation(float, float, void *, void *, int) __asm__("FUN_L00_002628d8");
extern void  jet_reset_camera(void *, void *) __asm__("FUN_L00_00284e50");
extern void  apply_npc_big_head_cheat(char *) __asm__("FUN_L12_002e6b60");
extern void  build_spherical_offset(void *, float, float, float) __asm__("FUN_00214db0");
extern L16WatchPlayer D_0013E633_watch __asm__("D_0013E533");

extern char *D_L12_001600EC;
extern void  advance_moby_animation(void *) __asm__("FUN_0020d580");

/* Ported from rac1-decomp (src/overlays/l12_hoven/mobyutil_00272D90.c: func_L12_0027C9B8), where it is exact; names translated to the US level program. */
void update_moby_bone_attachment(struct Moby   *parent,
                                 unsigned char *child,
                                 int            mode) __asm__("FUN_L12_0027b9c0");

void update_moby_bone_attachment(struct Moby *parent, unsigned char *child, int mode) {
    char workspace[0x40];
    build_moby_bone_transform(parent, mode, workspace);
    qcopy(child + 0x10, workspace + 0x30);
    advance_moby_animation(child);
    update_entity(child);
    copy_matrix3x4(child + 0xC0, workspace);
    normalize_vector_triplet(child + 0xC0);
    refresh_moby_spatial_bounds_from_basis(child);
    if (parent->flags & 1)
        *(unsigned short *)(child + 0x34) |= 0x41;
    else
        *(unsigned short *)(child + 0x34) &= 0xFFBE;
    *(unsigned short *)(child + 0x34) |= 6;
}

/* Activate the watcher and steer its body and head toward the player. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E45F8), where it is exact; names translated to the US level program. */
void miner_update(L16WatchMoby *m) __asm__("FUN_L12_002e6c48");

void miner_update(L16WatchMoby *moby) {
    enum {
        MINER_INITIALIZE = 0,
        MINER_UPDATE_DIALOGUE = 1,
        MINER_APPLY_DIALOGUE_COMPLETION = 2
    };
    L16WatchScratch scratch;
    L16WatchData   *moby_data = moby->data;
    float           rate;
    float           head_rate;
    int             tracking;
    apply_npc_big_head_cheat((char *)moby);
    switch (moby->state) {
    case MINER_INITIALIZE:
        if (alternate_item_available[1] != 0) {
            mark_moby_for_removal(moby);
            return;
        }
        moby->state = 1;
        initialize_npc_dialogue(moby, &moby_data->activation);
        initialize_moby_grounding(moby);
        break;
    case MINER_UPDATE_DIALOGUE:
        if (alternate_item_available[1] == 0 && update_npc_dialogue(moby, &moby_data->activation)) {
            begin_dialogue_camera_transition(moby, 2.5f);
            *(int *)((char *)moby + 0x58) = 0;
            moby->state = 2;
        }
        break;
    case MINER_APPLY_DIALOGUE_COMPLETION:
        if (D_L12_0015F5C4 != 2) {
            *(float *)((char *)moby + 0x58) = 1.0f;
            alternate_item_available[1] = 1;
            queue_dialogue_message(0x2EE7, scale_game_frames(300));
            alternate_item_available[0x61] = 1;
            jet_clear_persistent(moby->widget);
            if (moby_data->region != -1) {
                char *p = D_L12_001600EC + (moby_data->region << 7);
                jet_reset_camera(p + 0x30, p + 0x70);
            }
            memcard_save_data(0, -1);
            moby->state = 1;
        }
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
                moby_data->moving_timer = scale_game_frames(120);
            else
                fast_dec_timer(&moby_data->moving_timer);
        } else if (moby_data->moving_timer) {
            moby_data->moving_timer = 0;
            qcopy(moby_data->target, (char *)&D_0013E633_watch + 0xEED);
        }
        if (fast_dec_timer(&moby_data->target_timer)) {
            float heading;
            moby_data->target_timer =
                truncate_float_to_s32(scale_time(random_float_between(180.0f, 300.0f)));
            heading =
                fast_add_rotations(moby->yaw, random_float_between(-90.0f, 90.0f) * DEG_TO_RAD);
            build_spherical_offset(
                moby_data->target, 6.0f, heading, random_float_between(0.0f, 30.0f) * DEG_TO_RAD);
            fast_vec_add(moby_data->target, moby_data->target, moby->position);
        }
        if (moby_data->moving_timer) {
            qcopy(scratch.target, (char *)&D_0013E633_watch + 0xEED);
            rate = 0.04f;
            head_rate = 0.3f;
        } else {
            qcopy(scratch.target, moby_data->target);
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
        moby_data->pitch = pitch;
        moby_data->yaw = yaw * 0.3f;
        moby_data->head_yaw = yaw * 0.7f;
    }
    if (big_head_cheat_enabled)
        moby_data->height = 2.75f;
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, moby, moby_data->body, 1);
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, moby, (char *)moby_data + 0xE0, 0);
    if (*(int *)((char *)moby_data + 0x180) == 0) {
        char *created_moby = create_moby(0x11F);
        *(char **)((char *)moby_data + 0x180) = created_moby;
        if (created_moby) {
            MOBY(created_moby)->draw_dist = *(unsigned short *)((char *)moby + 0x32);
            (*(unsigned char **)((char *)moby_data + 0x180))[0x31] = 1;
            {
                char *p = *(char **)((char *)moby_data + 0x180);
                qcopy(p + 0x10, (char *)moby + 0x10);
                qcopy(p + 0x40, (char *)moby + 0x40);
            }
            FUN_L00_00250df8_x(moby);
        }
    } else {
        update_moby_bone_attachment(moby, *(void **)((char *)moby_data + 0x180), 2);
    }
}
