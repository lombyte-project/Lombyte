/* Level 03 moby class helga: the fitness instructor. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/mobys/initialize_moby_grounding.h"
#include "rnc/overlay/shared/mobys/record_moby_context_bytes.h"
#include "rnc/hero/hero.h"
#include "qcopy.h"
#include "rnc/overlay/level_save_state.h"
#include "rnc/game/options.h"
#include "rnc/game/game_globals.h"
#include "rnc/overlay/cuboid.h"
#include "rnc/overlay/l03/mobys/npc_dialogue_state.h"

#include "rnc/overlay/l03/mobys/helga_globals.h"

typedef struct {
    char  pad0[0x30];
    int   mode;
    int   t;
    char  pad38[0xC];
    short n;
    char  pad46[0x132];
    char *list[1];
} G_2da710;

extern int D_L03_0015F5C4;

extern G_2da710 D_L03_0016C960;
extern char     D_L03_0017C540[];
extern float    helga_big_head_attachment_uniform_value_gp __asm__("D_L03_00161BB4")
    __attribute__((sda));
extern void attach_manipulator(void *, int, void *) __asm__("FUN_0020cb10");

/* During the cutscene, nudges a camera value at two time windows and attaches
 * the shared manipulator to every listed moby of the same class as m. */
void apply_helga_big_head_cutscene_adjustments(struct Moby *class_reference_moby) __asm__("FUN_L03_002da710");

void apply_helga_big_head_cutscene_adjustments(struct Moby *class_reference_moby) {
    char *shared_attachment_record;
    int   i;

    if (D_L03_0015F5C4 == 2 && big_head_cheat_enabled != 0) {
        if (D_L03_0016C960.mode == 2 && D_L03_0016C960.t >= 0x1EA && D_L03_0016C960.t < 0x266) {
            helga_big_head_cutscene_adjustment = -0.2f;
        }
        if (D_L03_0016C960.mode == 2 && D_L03_0016C960.t >= 0x2EE && D_L03_0016C960.t < 0x321) {
            helga_big_head_cutscene_adjustment = 0.2f;
        }
        for (i = 0; i < D_L03_0016C960.n; i++) {
            if (*(short *)(D_L03_0016C960.list[i] + 0xA6) == class_reference_moby->oclass) {
                shared_attachment_record = D_L03_0017C540;
                if (((unsigned char *)shared_attachment_record)[1] == 0) {
                    attach_manipulator(D_L03_0016C960.list[i], 0, shared_attachment_record);
                    *(float *)(shared_attachment_record + 0x20) =
                        helga_big_head_attachment_uniform_value_gp;
                    *(float *)(shared_attachment_record + 0x24) =
                        helga_big_head_attachment_uniform_value_gp;
                    *(float *)(shared_attachment_record + 0x28) =
                        helga_big_head_attachment_uniform_value_gp;
                }
            }
        }
    }
}

/* Pvars of the Kerwan quest giver helga_update drives. */
struct QuestGiverVars {
    u8               pad0[0x80];
    struct TalkState talk; /* 0x80 */
    s32 reward; /* 0xD0: moby index removed (and recorded) when the deal is accepted, -1 for none */
    s32 link;   /* 0xD4: moby index of the class 0x3F4 helper it drives, -1 for none */
    s32 link_arg; /* 0xD8: stored into the helper when it is released */
    u8  padDC[4];
    u8  body[0x64]; /* 0xE0: body look-at driver (update_joint_animation) */
    f32 pitch;      /* 0x144 */
    f32 yaw;        /* 0x148 */
    u8  pad14C[4];
    f32 height; /* 0x150 */
    u8  pad154[0xC];
    u8  head[0x80];    /* 0x160: head look-at driver */
    Vec4 target;       /* 0x1E0: point it looks at while idle */
    s32  watch_timer;  /* 0x1F0: frames left watching the hero */
    s32  target_timer; /* 0x1F4: frames until a new idle target */
};

extern u8           npc_dialogue_suppression_flags[] __asm__("D_0013D4CC");
extern char        *D_001413D0[];
extern struct Moby *D_L03_0015FFD8;
extern s32          helga_helper_flag_context_records[] __asm__("D_L03_001BA720");
extern char         helga_dialogue_script[] __asm__("D_L03_00161BB8");

extern u8             mission_collection_recording_filter[] __asm__("D_L03_0015FC88");
extern LevelSaveState l03_moby_collection_state __asm__("D_L03_001BA6D0");

extern void  initialize_npc_dialogue(struct Moby *, struct TalkState *) __asm__("FUN_L00_002668a0");
extern int   update_npc_dialogue(struct Moby *, struct TalkState *) __asm__("FUN_L00_00266448");
extern void  begin_dialogue_camera_transition(struct Moby *, f32) __asm__("FUN_L01_002783a8");
extern void  jet_clear_persistent(int) __asm__("FUN_L00_002502a0");
extern void  jet_reset_camera(void *, void *) __asm__("FUN_L00_00284e50");
extern void  grant_inventory_item(int, int) __asm__("FUN_L00_00260860");
extern void  queue_dialogue_message(int, int) __asm__("FUN_L00_00263d40");
extern void  memcard_save_data(int, int) __asm__("FUN_0020b178");
extern float scale_time(float) __asm__("FUN_001f96b0");
extern void  build_spherical_offset(void *, float, float, float) __asm__("FUN_00214db0");
extern void  update_joint_animation(float, float, void *, void *, int) __asm__("FUN_L00_002628d8");

/* Quest giver: talks when approached (driving its class 0x3F4 helper), warps the
   hero or, once the deal is accepted, removes the reward moby, records it and saves.
   While idle it turns its head toward the hero or toward random points. */
void helga_update(struct Moby *m) __asm__("FUN_L03_002da870");

/* Moby class 890 */
void helga_update(struct Moby *helga_moby) {
    enum {
        HELGA_INITIALIZE = 0,
        HELGA_UPDATE_DIALOGUE = 1,
        HELGA_RESOLVE_DIALOGUE = 2
    };
    struct QuestGiverVars *helga_vars = (struct QuestGiverVars *)helga_moby->pvars;
    struct TalkState      *talk = &helga_vars->talk;
    struct Moby           *helper_moby;
    struct Moby           *reward_moby;
    u8                     group;
    s32                   *flag;
    Vec4                  *hero_position;
    struct Hero           *hero_state;
    float                  scratch_target[4];
    float                  eye[4];
    float                  delta[4];
    float                  rate;
    float                  head_rate;
    float                  yaw;
    float                  pitch;
    int                    tracking;

    apply_helga_big_head_cutscene_adjustments(helga_moby);
    switch (helga_moby->state) {
    case HELGA_INITIALIZE:
        if (npc_dialogue_suppression_flags[0] != 0) {
            if (helga_vars->link != -1) {
                helper_moby = (struct Moby *)((helga_vars->link << 8) + (s32)D_L03_0015FFD8);
                if (helper_moby->oclass == 0x3F4) {
                    flag = (s32 *)(helper_moby->pvars + 0x8C);
                    *flag = 1;
                    record_moby_context_bytes(
                        flag, 4, helga_moby, 2, helga_helper_flag_context_records);
                }
            }
            mark_moby_for_removal(helga_moby);
            return;
        }
        if (visited52[helga_moby->mission + current_level_index * 16] == 0xFF &&
            helga_vars->link != -1) {
            helper_moby = (struct Moby *)((helga_vars->link << 8) + (s32)D_L03_0015FFD8);
            if (helper_moby->oclass == 0x3F4) {
                flag = (s32 *)(helper_moby->pvars + 0x8C);
                *flag = 1;
                record_moby_context_bytes(
                    flag, 4, helga_moby, 2, helga_helper_flag_context_records);
            }
        }
        helga_moby->spawn_frame = *(u64 *)(D_001413D0[0] + 0x38);
        helga_moby->state = 1;
        talk->script = helga_dialogue_script;
        helga_vars->talk.unk48 = 5;
        initialize_npc_dialogue(helga_moby, talk);
        initialize_moby_grounding(helga_moby);
        break;
    case HELGA_UPDATE_DIALOGUE:
        if (update_npc_dialogue(helga_moby, talk)) {
            begin_dialogue_camera_transition(helga_moby, 2.3f);
            helga_moby->state = 2;
            if (helga_vars->link != -1) {
                helper_moby = (struct Moby *)((helga_vars->link << 8) + (s32)D_L03_0015FFD8);
                if (helper_moby->oclass == 0x3F4) {
                    flag = (s32 *)(helper_moby->pvars + 0x8C);
                    *flag = 1;
                    record_moby_context_bytes(
                        flag, 4, helga_moby, 2, helga_helper_flag_context_records);
                }
            }
        }
        break;
    case HELGA_RESOLVE_DIALOGUE:
        if (D_L03_0015F5C4 != 2) {
            if (helga_vars->talk.mode != 2) {
                jet_clear_persistent(helga_moby->mission);
                if (helga_vars->link != -1) {
                    helper_moby = (struct Moby *)((helga_vars->link << 8) + (s32)D_L03_0015FFD8);
                    if (helper_moby->oclass == 0x3F4) {
                        flag = (s32 *)(helper_moby->pvars + 0x8C);
                        *flag = 1;
                        record_moby_context_bytes(
                            flag, 4, helga_moby, 2, helga_helper_flag_context_records);
                    }
                }
                if (helga_vars->talk.warp != -1) {
                    jet_reset_camera(&l03_cuboids[helga_vars->talk.warp].pos,
                                     &l03_cuboids[helga_vars->talk.warp].rot);
                }
            }
            helga_moby->state = 1;
            if (helga_vars->talk.mode == 2) {
                grant_inventory_item(0xC, 1);
                queue_dialogue_message(0xBC8, -1);
                if (helga_vars->reward != -1) {
                    l03_moby_collection_state
                        .cleared[(s16)((struct Moby *)((helga_vars->reward << 8) +
                                                       (s32)D_L03_0015FFD8))
                                     ->save_id] =
                        ((struct Moby *)((helga_vars->reward << 8) + (s32)D_L03_0015FFD8))
                            ->mission +
                        2;
                    reward_moby = (struct Moby *)((helga_vars->reward << 8) + (s32)D_L03_0015FFD8);
                    /* Fakematch: same record as the || test in the level's other reward
                       code; the do/while (0) blocks keep the loop notes that make gcc
                       predict the 0xFF branch taken and keep the group load ahead of the
                       table load. */
                    if (reward_moby->mission == 0xFF)
                        goto record;
                    do {
                        group = reward_moby->mission;
                    } while (0);
                    if (mission_collection_recording_filter[group] != 0xFF &&
                        visited52[group + current_level_index * 16] == 0xFF) {
                        /* Fakematch: see above. */
                        do {
                        record:
                            l03_save_state.cleared[(s16)reward_moby->save_id] =
                                reward_moby->mission + 2;
                        } while (0);
                    }
                    mark_moby_for_removal(&D_L03_0015FFD8[helga_vars->reward]);
                }
                if (helga_vars->link != -1) {
                    helper_moby = (struct Moby *)((helga_vars->link << 8) + (s32)D_L03_0015FFD8);
                    if (helper_moby->oclass == 0x3F4) {
                        u8 *tv = helper_moby->pvars;
                        *(s32 *)(tv + 0x7C) = 0;
                        *(s32 *)(tv + 0x8C) = 0;
                        *(s32 *)(tv + 0xA0) = helga_vars->link_arg;
                    }
                }
                memcard_save_data(0, -1);
                mark_moby_for_removal(helga_moby);
                return;
            }
        } else if (helga_vars->talk.unk36 == 2 && helga_vars->link != -1) {
            helper_moby = (struct Moby *)((helga_vars->link << 8) + (s32)D_L03_0015FFD8);
            if (helper_moby->oclass == 0x3F4) {
                u8 *tv = helper_moby->pvars;
                *(s32 *)(tv + 0x7C) = 0;
                *(s32 *)(tv + 0x8C) = 0;
                *(s32 *)(tv + 0xA0) = helga_vars->link_arg;
            }
        }
        break;
    }
    rate = 0.02f;
    head_rate = 0.3f;
    tracking = 0;
    if (helga_moby->prev_seq == 0) {
        hero_position = &hero.motion.pos;
        tracking = 1;
        if (vector_distance_xy(&helga_moby->pos, hero_position) < 8.0f &&
            (hero_state = (struct Hero *)((u8 *)hero_position - 0x80),
             fast_difference_between_rotations(
                 helga_moby->rot.z,
                 fast_arc_tan(hero_state->motion.body_center.f[0] - helga_moby->pos.x,
                              hero_state->motion.body_center.f[1] - helga_moby->pos.y))) <
                1.5707964f) {
            if (fast_vec_length((u8 *)hero_position + 0x80) > 0.01f) {
                helga_vars->watch_timer = scale_game_frames(120);
            } else {
                fast_dec_timer(&helga_vars->watch_timer);
            }
        } else if (helga_vars->watch_timer) {
            helga_vars->watch_timer = 0;
            qcopy(&helga_vars->target, &hero.motion.body_center);
        }
        if (fast_dec_timer(&helga_vars->target_timer)) {
            float heading;
            helga_vars->target_timer =
                truncate_float_to_s32(scale_time(random_float_between(180.0f, 300.0f)));
            heading = fast_add_rotations(helga_moby->rot.z,
                                         random_float_between(-90.0f, 90.0f) * DEG_TO_RAD);
            build_spherical_offset(
                &helga_vars->target, 6.0f, heading, random_float_between(0.0f, 30.0f) * DEG_TO_RAD);
            fast_vec_add(&helga_vars->target, &helga_vars->target, &helga_moby->pos);
        }
        if (helga_vars->watch_timer) {
            qcopy(scratch_target, &hero.motion.body_center);
            rate = 0.04f;
            head_rate = 0.3f;
        } else {
            qcopy(scratch_target, &helga_vars->target);
        }
    }
    if (tracking) {
        qcopy(eye, &helga_moby->pos);
        eye[2] += 1.0f;
        fast_vec_sub(delta, scratch_target, eye);
        yaw = fast_subtract_rotations(fast_arc_tan(delta[0], delta[1]), helga_moby->rot.z);
        pitch = -fast_arc_tan(vector_length_xy(delta), delta[2]);
        if (yaw > 1.5707964f)
            yaw = 1.5707964f;
        else if (yaw < -1.5707964f)
            yaw = -1.5707964f;
        if (pitch > 0.5235988f)
            pitch = 0.5235988f;
        else if (pitch < -0.5235988f)
            pitch = -0.5235988f;
        helga_vars->pitch = pitch;
        helga_vars->yaw = yaw * 0.5f;
    }
    if (big_head_cheat_enabled)
        helga_vars->height = 2.75f;
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, helga_moby, helga_vars->body, 0);
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, helga_moby, helga_vars->head, 1);
}
