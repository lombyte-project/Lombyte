/* Level 12 moby class edwina. */
#include "types.h"
#include "rnc/game/level_object_states.h"
#include "rnc/game/options.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/overlay/watch.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/mobys/initialize_moby_grounding.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/l12/mobys/watcher.h"

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
extern void  grant_inventory_item(int, int) __asm__("FUN_L00_00260860");
extern void  update_joint_animation(float, float, void *, void *, int) __asm__("FUN_L00_002628d8");
extern void  jet_reset_camera(void *, void *) __asm__("FUN_L00_00284e50");
extern void  build_spherical_offset(void *, float, float, float) __asm__("FUN_00214db0");
extern L16WatchPlayer D_0013E633_watch __asm__("D_0013E533");

extern char *D_L12_001600EC;

/* Data of the second watcher (edwina_update): the L16 watcher layout with
 * the turret and base mobys it spawns at 0x164/0x168. */
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
    int                region;
    char              *turret;
    char              *base;
    char               pad16C[4];
    float              target[4];
    char               pad180[4];
    int                moving_timer;
    int                target_timer;
} L12WatchData;

extern void attach_shared_manipulator_to_matching_class_moby(void *) __asm__("FUN_L03_002db480");

void edwina_update(L16WatchMoby *m) __asm__("FUN_L12_002eb570");

void edwina_update(L16WatchMoby *moby) {
    enum {
        EDWINA_INITIALIZE = 0,
        EDWINA_UPDATE_DIALOGUE = 1,
        EDWINA_APPLY_DIALOGUE_COMPLETION = 2
    };
    L16WatchScratch scratch;
    L12WatchData   *moby_data = (L12WatchData *)moby->data;
    float           rate;
    float           head_rate;
    int             tracking;
    attach_shared_manipulator_to_matching_class_moby(moby);
    if (level_object_states[70].state == 0) {
        if (moby->animation != 1)
            blend_moby_animation(moby, 1, 0, 2);
    } else if (moby->animation != 0) {
        blend_moby_animation(moby, 0, 0, 2);
    }
    switch (moby->state) {
    case EDWINA_INITIALIZE:
        moby->state = 1;
        initialize_npc_dialogue(moby, &moby_data->activation);
        moby_data->activation.radius = 1.7f;
        initialize_moby_grounding(moby);
        if (alternate_item_available[0x13C])
            *(short *)((char *)moby_data + 0x56) = -1;
        break;
    case EDWINA_UPDATE_DIALOGUE:
        if (alternate_item_available[0x13C] == 0 &&
            update_npc_dialogue(moby, &moby_data->activation)) {
            begin_dialogue_camera_transition(moby, 2.52f);
            *(int *)((char *)moby + 0x58) = 0;
            moby->state = 2;
        }
        break;
    case EDWINA_APPLY_DIALOGUE_COMPLETION:
        if (D_L12_0015F5C4 != 2) {
            *(float *)((char *)moby + 0x58) = 1.0f;
            jet_clear_persistent(moby->widget);
            if (moby_data->region != -1) {
                char *p = D_L12_001600EC + (moby_data->region << 7);
                jet_reset_camera(p + 0x30, p + 0x70);
            }
            moby->state = 1;
            if (*(short *)((char *)moby_data + 0x24) == 2) {
                grant_inventory_item(4, 0);
                queue_dialogue_message(0x2EE6, -1);
                if (moby_data->region != -1) {
                    char *p = D_L12_001600EC + (moby_data->region << 7);
                    jet_reset_camera(p + 0x30, p + 0x70);
                }
                memcard_save_data(0, -1);
            }
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
    if (level_object_states[70].state == 0) {
        if (moby_data->turret == 0) {
            char *o = create_moby(0x4C1);
            moby_data->turret = o;
            if (o) {
                *(unsigned short *)(o + 0x32) = *(unsigned short *)((char *)moby + 0x32);
                moby_data->turret[0x31] = 1;
                {
                    char *p = moby_data->turret;
                    qcopy(p + 0x10, (char *)moby + 0x10);
                    qcopy(p + 0x40, (char *)moby + 0x40);
                    FUN_L00_00250df8_x(p);
                }
                *(unsigned short *)(moby_data->turret + 0x34) = 0;
                blend_moby_animation(moby_data->turret, 1, 0, 2);
            }
        } else if (D_L12_0015F5C4 == 2) {
            *(unsigned short *)(moby_data->turret + 0x34) |= 1;
            moby_data->turret[0x31] = 0;
        } else {
            *(unsigned short *)(moby_data->turret + 0x34) &= ~1;
            moby_data->turret[0x31] = 1;
        }
        if (moby_data->base == 0) {
            char *o = create_moby(0x196);
            moby_data->base = o;
            if (o) {
                *(unsigned short *)(o + 0x32) = *(unsigned short *)((char *)moby + 0x32);
                moby_data->base[0x31] = 1;
                {
                    char *p = moby_data->base;
                    qcopy(p + 0x10, (char *)moby + 0x10);
                    qcopy(p + 0x40, (char *)moby + 0x40);
                    FUN_L00_00250df8_x(p);
                }
            }
        } else if (D_L12_0015F5C4 == 2) {
            *(unsigned short *)(moby_data->turret + 0x34) |= 1;
            moby_data->turret[0x31] = 0;
            *(unsigned short *)(moby_data->base + 0x34) |= 1;
            moby_data->base[0x31] = 0;
        } else {
            *(unsigned short *)(moby_data->turret + 0x34) &= ~1;
            moby_data->turret[0x31] = 1;
            *(unsigned short *)(moby_data->base + 0x34) &= ~1;
            moby_data->base[0x31] = 1;
        }
    } else {
        if (moby_data->base) {
            mark_moby_for_removal(moby_data->base);
            moby_data->base = 0;
        }
        if (moby_data->turret) {
            mark_moby_for_removal(moby_data->turret);
            moby_data->turret = 0;
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
        moby_data->head_yaw = moby_data->yaw = yaw * 0.5f;
    }
    if (big_head_cheat_enabled)
        moby_data->height = 2.75f;
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, moby, moby_data->body, 1);
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, moby, moby_data->head, 0);
}
