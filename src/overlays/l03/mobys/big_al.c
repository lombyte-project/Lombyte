/* Level 03 moby class big_al. */
#include "types.h"
#include "rnc/game/level_object_states.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/math/random.h"
#include "rnc/overlay/shared/mobys/initialize_moby_grounding.h"
#include "rnc/hero/hero.h"
#include "qcopy.h"
#include "rnc/overlay/level_save_state.h"
#include "rnc/game/options.h"
#include "rnc/game/game_globals.h"
#include "rnc/overlay/cuboid.h"
#include "rnc/overlay/l03/mobys/npc_dialogue_state.h"
#include "rnc/save/memcard_save_data.h"

extern int            D_L03_0015F5C4;
extern s32            D_L03_0015FFD8_i __asm__("D_L03_0015FFD8");
extern u8             mission_collection_recording_filter[] __asm__("D_L03_0015FC88");
extern LevelSaveState l03_moby_collection_state __asm__("D_L03_001BA6D0");
extern void  initialize_npc_dialogue(struct Moby *, struct TalkState *) __asm__("FUN_L00_002668a0");
extern int   update_npc_dialogue(struct Moby *, struct TalkState *) __asm__("FUN_L00_00266448");
extern void  begin_dialogue_camera_transition(struct Moby *, f32) __asm__("FUN_L01_002783a8");
extern void  jet_clear_persistent(int) __asm__("FUN_L00_002502a0");
extern void  jet_reset_camera(void *, void *) __asm__("FUN_L00_00284e50");
extern void  grant_inventory_item(int, int) __asm__("FUN_L00_00260860");
extern void  queue_dialogue_message(int, int) __asm__("FUN_L00_00263d40");
extern float scale_time(float) __asm__("FUN_001f96b0");
extern void  build_spherical_offset(void *, float, float, float) __asm__("FUN_00214db0");
extern void  update_joint_animation(float, float, void *, void *, int) __asm__("FUN_L00_002628d8");

/* Dialogue record run by initialize_npc_dialogue / update_npc_dialogue (only the fields read here). */
struct TalkRecord {
    u8  pad0[4];
    s16 mode; /* 0x04: 2 when the talk was accepted */
    u8  pad6[0x30];
    s16 unk36;
    u8  pad38[8];
};

/* Look-at driver run by update_joint_animation. */
struct LookAt {
    u8  pad0[0x64];
    f32 pitch; /* 0x64 */
    f32 yaw;   /* 0x68 */
    u8  pad6C[4];
    f32 height; /* 0x70 */
    u8  pad74[0xC];
};

/* Pvars of the Kerwan guide big_al_update drives. */
struct GuideVars {
    struct TalkRecord talk;
    struct LookAt     head;   /* 0x40 */
    struct LookAt     body;   /* 0xC0 */
    s32               turned; /* 0x140: set once it has turned around to face the hero */
    s32  reward; /* 0x144: moby index of the class 0x3E5 reward it releases, -1 for none */
    s32  link;   /* 0x148: moby index of its class 0x3F4 helper, -1 for none */
    s32  warp;   /* 0x14C: index into l03_cuboids, -1 for none */
    Vec4 target; /* 0x150: point it looks at while idle */
    u8   pad160[4];
    s32  watch_timer;  /* 0x164 */
    s32  target_timer; /* 0x168 */
};

extern u8    D_0013D4C2[];
extern char *D_001413D0_db558[] __asm__("D_001413D0");

extern void
attach_shared_manipulator_to_matching_class_moby(struct Moby *) __asm__("FUN_L03_002db480");
extern void set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");

/* Kerwan guide: talks when approached (turning round to face the hero first),
   releases its reward moby once the item is owned, can warp the hero, and turns
   its head toward the hero or random points while idle. */
void big_al_update(struct Moby *m) __asm__("FUN_L03_002db558");

/* Moby class 909 */
void big_al_update(struct Moby *guide_moby) {
    enum {
        BIG_AL_INITIALIZE = 0,
        BIG_AL_UPDATE_DIALOGUE = 1,
        BIG_AL_RESOLVE_DIALOGUE = 2
    };
    struct GuideVars *v = (struct GuideVars *)guide_moby->pvars;
    struct Moby      *referenced_moby;
    struct Moby      *reward_moby;
    Vec4             *hero_position;
    struct Hero      *hero_state;
    float             scratch_target[4];
    float             eye[4];
    float             delta[4];
    float             rate;
    float             head_rate;
    float             yaw;
    float             pitch;
    int               tracking;
    u8                group;

    attach_shared_manipulator_to_matching_class_moby(guide_moby);
    switch (guide_moby->state) {
    case BIG_AL_INITIALIZE:
        if (level_object_states[20].state == 0) {
            guide_moby->rot.z = fast_add_rotations(guide_moby->rot.z, 3.1415927f);
            v->turned = 0;
        } else {
            v->turned = 1;
        }
        if (visited52[guide_moby->mission + current_level_index * 16] == 0xFF && v->link != -1) {
            referenced_moby = (struct Moby *)((v->link << 8) + D_L03_0015FFD8_i);
            if (referenced_moby->oclass == 0x3F4) {
                *(s32 *)(referenced_moby->pvars + 0x8C) = 1;
            }
        }
        guide_moby->spawn_frame = *(u64 *)(D_001413D0_db558[0] + 0x38);
        guide_moby->state = 1;
        initialize_npc_dialogue(guide_moby, (struct TalkState *)&v->talk);
        initialize_moby_grounding(guide_moby);
        if (D_0013D4C2[0] != 0) {
            v->talk.unk36 = -1;
        }
        break;
    case BIG_AL_UPDATE_DIALOGUE:
        if (item_available[2] != 0 && v->reward != -1) {
            referenced_moby = (struct Moby *)((v->reward << 8) + D_L03_0015FFD8_i);
            if (referenced_moby->oclass == 0x3E5 && referenced_moby->state == 0) {
                referenced_moby->state = 1;
                l03_moby_collection_state
                    .cleared[(s16)((struct Moby *)((v->reward << 8) + D_L03_0015FFD8_i))->save_id] =
                    ((struct Moby *)((v->reward << 8) + D_L03_0015FFD8_i))->mission + 2;
                reward_moby = (struct Moby *)((v->reward << 8) + D_L03_0015FFD8_i);
                {
                    s32 k = reward_moby->mission;
                    u32 u = k & 0xFF;
                    if (k == 0xFF || (mission_collection_recording_filter[u] != 0xFF &&
                                      visited52[u + current_level_index * 16] == 0xFF)) {
                        l03_save_state.cleared[(s16)reward_moby->save_id] = k + 2;
                    }
                }
            }
        }
        if (item_available[2] != 0 &&
            visited52[guide_moby->mission + current_level_index * 16] == 0xFF) {
            break;
        }
        if (update_npc_dialogue(guide_moby, (struct TalkState *)&v->talk)) {
            if (v->turned == 0) {
                guide_moby->rot.z = fast_add_rotations(guide_moby->rot.z, 3.1415927f);
                v->turned = 1;
            }
            begin_dialogue_camera_transition(guide_moby, 2.7f);
            guide_moby->anim_speed = 0.0f;
            guide_moby->state = 2;
        }
        break;
    case BIG_AL_RESOLVE_DIALOGUE:
        if (D_L03_0015F5C4 == 2) {
            break;
        }
        guide_moby->anim_speed = 1.0f;
        if (v->talk.mode != 2) {
            jet_clear_persistent(guide_moby->mission);
            if (v->link != -1) {
                referenced_moby = (struct Moby *)((v->link << 8) + D_L03_0015FFD8_i);
                if (referenced_moby->oclass == 0x3F4) {
                    *(s32 *)(referenced_moby->pvars + 0x8C) = 1;
                }
            }
            if (v->warp != -1) {
                jet_reset_camera(&l03_cuboids[v->warp].pos, &l03_cuboids[v->warp].rot);
            }
        }
        guide_moby->state = 1;
        if (v->talk.mode != 2) {
            break;
        }
        grant_inventory_item(2, 1);
        queue_dialogue_message(0xBC7, -1);
        if (v->reward != -1) {
            referenced_moby = (struct Moby *)((v->reward << 8) + D_L03_0015FFD8_i);
            if (referenced_moby->oclass == 0x3E5 && referenced_moby->state == 0) {
                referenced_moby->state = 1;
                l03_moby_collection_state
                    .cleared[(s16)((struct Moby *)((v->reward << 8) + D_L03_0015FFD8_i))->save_id] =
                    ((struct Moby *)((v->reward << 8) + D_L03_0015FFD8_i))->mission + 2;
                reward_moby = (struct Moby *)((v->reward << 8) + D_L03_0015FFD8_i);
                /* Fakematch: same record test as above; the goto and the do/while (0)
                   loop note keep the group byte's zero-extend ahead of the table address. */
                if (reward_moby->mission == 0xFF)
                    goto rec2;
                do {
                    group = reward_moby->mission;
                } while (0);
                if (mission_collection_recording_filter[group] != 0xFF &&
                    visited52[group + current_level_index * 16] == 0xFF) {
                rec2:
                    l03_save_state.cleared[(s16)reward_moby->save_id] = reward_moby->mission + 2;
                }
            }
        }
        if (v->warp != -1) {
            jet_reset_camera(&l03_cuboids[v->warp].pos, &l03_cuboids[v->warp].rot);
        }
        memcard_save_data(0, -1);
        set_race_camera(&l03_cuboids[v->warp].pos, &l03_cuboids[v->warp].rot, 0, 1);
        break;
    }
    rate = 0.02f;
    head_rate = 0.3f;
    tracking = 0;
    if (guide_moby->prev_seq == 0) {
        hero_position = &hero.motion.pos;
        tracking = 1;
        if (vector_distance_xy(&guide_moby->pos, hero_position) < 8.0f &&
            (hero_state = (struct Hero *)((u8 *)hero_position - 0x80),
             fast_difference_between_rotations(
                 guide_moby->rot.z,
                 fast_arc_tan(hero_state->motion.body_center.f[0] - guide_moby->pos.x,
                              hero_state->motion.body_center.f[1] - guide_moby->pos.y))) <
                1.5707964f) {
            if (fast_vec_length((u8 *)hero_position + 0x80) > 0.01f) {
                v->watch_timer = scale_game_frames(120);
            } else {
                fast_dec_timer(&v->watch_timer);
            }
        } else if (v->watch_timer) {
            v->watch_timer = 0;
            qcopy(&v->target, &hero.motion.body_center);
        }
        if (fast_dec_timer(&v->target_timer)) {
            float heading;
            v->target_timer =
                truncate_float_to_s32(scale_time(random_float_between(180.0f, 300.0f)));
            heading = fast_add_rotations(guide_moby->rot.z,
                                         random_float_between(-90.0f, 90.0f) * DEG_TO_RAD);
            build_spherical_offset(
                &v->target, 6.0f, heading, random_float_between(0.0f, 30.0f) * DEG_TO_RAD);
            fast_vec_add(&v->target, &v->target, &guide_moby->pos);
        }
        if (v->watch_timer) {
            qcopy(scratch_target, &hero.motion.body_center);
            rate = 0.04f;
            head_rate = 0.3f;
        } else {
            qcopy(scratch_target, &v->target);
        }
    }
    if (tracking) {
        qcopy(eye, &guide_moby->pos);
        eye[2] += 1.0f;
        fast_vec_sub(delta, scratch_target, eye);
        yaw = fast_subtract_rotations(fast_arc_tan(delta[0], delta[1]), guide_moby->rot.z);
        pitch = -fast_arc_tan(vector_length_xy(delta), delta[2]);
        if (yaw > 1.5707964f)
            yaw = 1.5707964f;
        else if (yaw < -1.5707964f)
            yaw = -1.5707964f;
        if (pitch > 0.5235988f)
            pitch = 0.5235988f;
        else if (pitch < -0.5235988f)
            pitch = -0.5235988f;
        v->head.pitch = pitch;
        v->body.yaw = v->head.yaw = yaw * 0.5f;
    }
    if (big_head_cheat_enabled)
        v->head.height = 2.75f;
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, guide_moby, &v->head, 1);
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, guide_moby, &v->body, 0);
}
