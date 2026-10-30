/* Level 05 moby class hoverboard_girl: her update, her boobs animation and her cutscene instances. */
#include "types.h"
#include "rnc/game/level_object_states.h"
#include "rnc/hero/hero.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "sda.h"
#include "eetypes.h"
#include "qcopy.h"
#include "rnc/data_labels.h"
#include "rnc/game/options.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/cutscenes/update_npc_dialogue.h"
#include "rnc/overlay/shared/mobys/initialize_moby_grounding.h"
#include "rnc/mobys/attach_manipulator.h"
#include "rnc/mobys/moby.h"

/* Class 918 (hoverboard_girl), Rilgar / Blackwater City.
 * The update counts player action 11 at frame 15 when she faces the player.
 * Its two breast manipulators also carry the effect into cutscene instances. */
typedef union {
    u128 quadword;
    f32  component[4];
} HoverboardGirlVector;

struct HoverboardGirlMoby;

typedef struct {
    u8                         attached_flags[2];
    u8                         pad02[0x5E];
    f32                        target_rotation[3];
    u8                         pad6C[4];
    f32                        target_scale;
    u8                         pad74[4];
    struct HoverboardGirlMoby *owner;
    u8                         pad7C[4];
} HoverboardGirlJoint;

typedef struct {
    u8                   pad00[4];
    s16                  previous_dialogue_state;
    u8                   pad06[2];
    u8                   race_return_pending;
    u8                   pad09[0x17];
    const char          *dialogue_name;
    u8                   pad24[0x12];
    s16                  dialogue_state;
    s32                  dialogue_state_frame;
    u8                   pad3C[4];
    HoverboardGirlJoint  joints[4];
    u8                   pad240[4];
    s32                  idle_animation_timer;
    s32                  player_look_timer;
    s32                  random_look_timer;
    HoverboardGirlVector look_target;
    s32                  breast_growth_count_gp;
} HoverboardGirlState;

typedef struct HoverboardGirlMoby {
    u8                   pad00[0x10];
    HoverboardGirlVector position;
    u8                   state;
    u8                   pad21[0x27];
    f32                  yaw;
    u8                   pad4C[7];
    u8                   animation;
    u8                   pad54[0x1C];
    u8                   animation_flags;
    u8                   pad71[7];
    HoverboardGirlState *data;
    u8                   pad7C[0x2A];
    s16                  class_id;
    u8                   padA8[0x14];
    u8                   dialogue_event;
} HoverboardGirlMoby;

typedef struct {
    u8  pad00[4];
    s16 state;
} HoverboardRaceEntry;

/* Partial resident player-data view: addresses verified in the retail loads. */

typedef struct {
    f32 delta;
    f32 frame_scale;
    f32 movement_sampling_scale;
} FrameTiming;

extern s32                  breast_growth_count_gp __asm__("D_L05_00161ED0") __attribute__((sda));
extern s32                  game_mode __asm__("D_L05_0015F5C4");
extern s32                  game_mode __asm__("D_L05_0015F5C4");
extern s32                  frame_number __asm__("D_L05_0015F5CC");
extern u8                   alternate_item_available __asm__("D_0013D388") NOT_SDA;
extern HoverboardRaceEntry *race_entries[] __asm__("D_L05_001B1AF8");
extern const char           race_girl_name[] __asm__("D_L05_00161ED8");
extern HoverboardGirlVector race_camera_vectors[] __asm__("D_L05_00215BC0");
extern HoverboardGirlVector saved_camera_vectors[] __asm__("D_00141050");

extern f32  scale_time(f32) __asm__("FUN_001f96b0");
extern f32  probe_ground_height(f32, HoverboardGirlVector *, s32) __asm__("FUN_00213508");
extern void begin_dialogue_camera_transition(f32, HoverboardGirlMoby *) __asm__("FUN_L01_002783a8");
extern void set_race_camera(HoverboardGirlVector *,
                            HoverboardGirlVector *,
                            s32,
                            s32) __asm__("FUN_L00_00216f90");
extern void request_music_track(s32, s32) __asm__("FUN_L01_0027a248");
extern void memcard_save_data(s32, s64) __asm__("FUN_0020b178");
extern void build_spherical_offset(f32, f32, f32, void *) __asm__("FUN_00214db0");
extern void update_joint_animation(
    HoverboardGirlMoby *, HoverboardGirlJoint *, s32, f32, f32) __asm__("FUN_L00_002628d8");
void update_hoverboard_girl_cutscene_instances(HoverboardGirlMoby *) __asm__("FUN_L05_00316990");
void update_hoverboard_girl_boobs_animation(HoverboardGirlMoby *,
                                            HoverboardGirlState *) __asm__("FUN_L05_00316810");
void update_hoverboard_girl(HoverboardGirlMoby *) __asm__("FUN_L05_00316ab8");

extern f32 frame_scale_sq __asm__("D_0015ED64");

void update_hoverboard_girl_boobs_animation(HoverboardGirlMoby  *moby,
                                            HoverboardGirlState *state) __asm__("FUN_L05_00316810");

void update_hoverboard_girl_boobs_animation(HoverboardGirlMoby *moby, HoverboardGirlState *state) {
    s32                  joint_index = 0;
    HoverboardGirlJoint *joint = &state->joints[2];
    HoverboardGirlState *state_window = state;
    f32                  animation_value;

    do {
        if (breast_growth_count_gp != 0) {
            animation_value = (f32)breast_growth_count_gp * 0.15f + 1.0f;
            if (3.7f < animation_value) {
                animation_value = 3.7f;
            }
            state_window->joints[2].target_scale = animation_value;
            animation_value = (f32)(frame_number & 0x7F) * 0.0078125f;
            state_window->joints[2].target_rotation[1] =
                fast_sin(animation_value * 3.1415927f) * 0.61086524f;
        }
        update_joint_animation(
            moby, joint, joint_index + 2, frame_scale_sq * 0.05f, frame_scale_sq * 0.3f);
        joint_index++;
        joint++;
        state_window = (HoverboardGirlState *)((u8 *)state_window + 0x80);
    } while (joint_index < 2);
}

/* The level's moby list: count at 0x44, moby pointers from 0x178. */
typedef struct {
    char  pad0[0x44];
    short count;
    char  pad46[0x132];
    char *mobys[1];
} MobyList;

extern MobyList      D_L05_0016CC60;
extern unsigned char D_0015EDB0[] MACRO_ADDR;
extern unsigned char D_L05_0017C8A0[];

/* Applies breast growth (breast_growth_count) and big-head manipulators to the cutscene instances
 * of the girl's class. */
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_0030EB68.c: func_L05_00317E58), where it is exact; names translated to the US level program. */
void update_hoverboard_girl_cutscene_instances(HoverboardGirlMoby *girl) {
    char *reference_moby = (char *)girl;

    if (game_mode == 2 && (D_0015EDB0[0] != 0 || breast_growth_count_gp != 0)) {
        int i;
        for (i = 0; i < D_L05_0016CC60.count; i++) {
            char *moby = D_L05_0016CC60.mobys[i];
            if (MOBY(moby)->oclass == MOBY(reference_moby)->oclass) {
                if (D_0015EDB0[0] != 0) {
                    unsigned char *manipulator = D_L05_0017C8A0;
                    if (manipulator[1] == 0) {
                        attach_manipulator(moby, 0, manipulator);
                        *(float *)(manipulator + 0x20) = 2.75f;
                        *(float *)(manipulator + 0x24) = 2.75f;
                        *(float *)(manipulator + 0x28) = 2.75f;
                    }
                }
                if (breast_growth_count_gp != 0) {
                    update_hoverboard_girl_boobs_animation(
                        (HoverboardGirlMoby *)moby,
                        *(HoverboardGirlState **)(reference_moby + 0x78));
                }
            }
        }
    }
}

void update_hoverboard_girl(HoverboardGirlMoby *moby) __asm__("FUN_L05_00316ab8");

void update_hoverboard_girl(HoverboardGirlMoby *moby) {
    enum {
        HOVERBOARD_GIRL_STATE_INITIALIZE = 0,
        HOVERBOARD_GIRL_STATE_UPDATE_DIALOGUE = 1,
        HOVERBOARD_GIRL_STATE_WAIT_TO_RESUME_DIALOGUE = 3
    };
    HoverboardGirlState *state = moby->data;
    HoverboardGirlVector target_position;
    HoverboardGirlVector head_position;
    HoverboardGirlVector direction;
    s32                  animation;
    s32                  look_enabled;
    f32                  rotation_step;
    f32                  rotation_limit;
    f32                  heading;
    f32                  yaw;
    f32                  pitch;
    f32                  delta;

    update_hoverboard_girl_cutscene_instances(moby);
    switch (moby->state) {
    case HOVERBOARD_GIRL_STATE_INITIALIZE:
        if (moby->animation != 0) {
            blend_moby_animation(moby, 0, 0, scale_game_frames(20));
        }
        moby->position.component[2] = probe_ground_height(0.5f, &moby->position, 0);
        if (level_object_states[16].state == 0 || level_object_states[17].state == 0) {
            HoverboardRaceEntry **entries = race_entries;
            s32                   entry = get_dialogue_entry(moby);
            entries[entry]->state = 13;
        }
        state->dialogue_name = race_girl_name;
        moby->state = 1;
        initialize_npc_dialogue(moby, state);
        initialize_moby_grounding(moby);
        break;
    case HOVERBOARD_GIRL_STATE_UPDATE_DIALOGUE:
        if (update_npc_dialogue(moby, state) != 0) {
            begin_dialogue_camera_transition(2.0f, moby);
            moby->state = 3;
        }
        if (state->dialogue_state == 3) {
            state->previous_dialogue_state = state->dialogue_state;
            state->dialogue_state = 2;
            state->dialogue_state_frame = frame_number;
            moby->dialogue_event = 0;
            set_race_camera(&race_camera_vectors[0], &race_camera_vectors[1], 0, 1);
            qcopy(&saved_camera_vectors[0], &race_camera_vectors[2]);
            qcopy(&saved_camera_vectors[1], &race_camera_vectors[3]);
            request_music_track(2, 6);
        }
        if (moby->dialogue_event == 1) {
            set_race_camera(&race_camera_vectors[2], &race_camera_vectors[3], 0, 1);
            request_music_track(0, 8);
            if (alternate_item_available == 0) {
                state->previous_dialogue_state = state->dialogue_state;
                state->dialogue_state = 4;
                state->dialogue_state_frame = frame_number;
                state->race_return_pending = 1;
            }
            moby->dialogue_event = 0;
        }
        if (moby->dialogue_event == 2) {
            request_music_track(0, 8);
            set_race_camera(&race_camera_vectors[2], &race_camera_vectors[3], 0, 1);
            moby->dialogue_event = 0;
        }
        if (moby->dialogue_event == 3) {
            moby->dialogue_event = 0;
            set_race_camera(&race_camera_vectors[0], &race_camera_vectors[1], 0, 1);
            qcopy(&saved_camera_vectors[0], &race_camera_vectors[2]);
            qcopy(&saved_camera_vectors[1], &race_camera_vectors[3]);
        }
        if ((moby->animation == 0 || moby->animation == 2) &&
            fast_dec_timer(&state->idle_animation_timer)) {
            state->idle_animation_timer =
                truncate_float_to_s32(scale_time(random_float_between(1200.0f, 2400.0f)));
            if (moby->animation != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            }
        } else if (moby->animation_flags & 2) {
            animation = random_integer_below(2) != 0 ? 0 : 2;
            if (moby->animation != animation) {
                blend_moby_animation(moby, animation, 0, scale_game_frames(10));
            }
        }
        break;
    case HOVERBOARD_GIRL_STATE_WAIT_TO_RESUME_DIALOGUE:
        if (game_mode != 2) {
            moby->state = 1;
            if (state->previous_dialogue_state == 4) {
                alternate_item_available = 1;
                queue_dialogue_message(0x1395, scale_game_frames(300));
                memcard_save_data(0, -1);
            }
        }
        break;
    default:
        if (moby->dialogue_event != 0) {
            set_race_camera(&race_camera_vectors[2], &race_camera_vectors[3], 0, 1);
            moby->dialogue_event = 0;
        }
        break;
    }

    rotation_step = 0.02f;
    rotation_limit = 0.3f;
    look_enabled = 0;
    if (moby->animation == 0 || moby->animation == 2) {
        look_enabled = 1;
        if (vector_distance_xy(&moby->position, &hero.motion.pos) < 8.0f &&
            fast_difference_between_rotations(
                moby->yaw,
                fast_arc_tan(hero.motion.body_center.f[0] - moby->position.component[0],
                             hero.motion.body_center.f[1] - moby->position.component[1])) <
                1.5707964f) {
            if (fast_vec_length(&hero.motion.displacement) > 0.01f) {
                state->player_look_timer = scale_game_frames(120);
            } else {
                fast_dec_timer(&state->player_look_timer);
            }
        } else if (state->player_look_timer != 0) {
            state->player_look_timer = 0;
            qcopy(&state->look_target, &hero.motion.body_center);
        }
        if (fast_dec_timer(&state->random_look_timer)) {
            state->random_look_timer =
                truncate_float_to_s32(scale_time(random_float_between(180.0f, 300.0f)));
            heading =
                fast_add_rotations(moby->yaw, random_float_between(-90.0f, 90.0f) * DEG_TO_RAD);
            pitch = random_float_between(0.0f, 30.0f) * DEG_TO_RAD;
            build_spherical_offset(6.0f, heading, pitch, &state->look_target);
            fast_vec_add(&state->look_target, &state->look_target, &moby->position);
        }
        if (state->player_look_timer != 0) {
            qcopy(&target_position, &hero.motion.body_center);
            rotation_step = 0.03f;
            rotation_limit = 0.3f;
        } else {
            qcopy(&target_position, &state->look_target);
        }
    }
    if (look_enabled) {
        qcopy(&head_position, &moby->position);
        head_position.component[2] += 1.0f;
        fast_vec_sub(&direction, &target_position, &head_position);
        yaw = fast_subtract_rotations(fast_arc_tan(direction.component[0], direction.component[1]),
                                      moby->yaw);
        pitch = -fast_arc_tan(vector_length_xy(&direction), direction.component[2]);
        if (yaw > 1.5707964f)
            yaw = 1.5707964f;
        else if (yaw < -1.5707964f)
            yaw = -1.5707964f;
        if (pitch > 0.5235988f)
            pitch = 0.5235988f;
        else if (pitch < -0.5235988f)
            pitch = -0.5235988f;
        state->joints[0].target_rotation[1] = pitch;
        state->joints[0].target_rotation[2] = yaw * 0.6f;
        state->joints[1].target_rotation[2] = yaw * 0.4f;
    }

    /* The easter egg only counts flips within 15 units and a 70-degree
     * facing cone. Retail increments at action 11, frame 15; no debounce. */
    if (vector_distance_xy(&moby->position, &hero.motion.pos) < 15.0f &&
        fast_difference_between_rotations(
            moby->yaw,
            fast_arc_tan(hero.motion.pos.f[0] - moby->position.component[0],
                         hero.motion.pos.f[1] - moby->position.component[1])) < 1.2217305f) {
        if (hero.state.current == HERO_STATE_HIGH_JUMP && hero.state_timer == 15) {
            state->breast_growth_count_gp++;
            if (state->breast_growth_count_gp > 20)
                state->breast_growth_count_gp = 20;
            breast_growth_count_gp = state->breast_growth_count_gp;
        }
        /* Action 4 eventually reduces the effect, one step
         * every tenth game frame. Keep the scaled-frame threshold. */
        if (hero.state.current == HERO_STATE_CROUCH && scale_game_frames(20) < hero.state_timer &&
            frame_number % 10 == 0) {
            state->breast_growth_count_gp--;
            if (state->breast_growth_count_gp < 0)
                state->breast_growth_count_gp = 0;
            breast_growth_count_gp = state->breast_growth_count_gp;
        }
    }
    if (game_mode != 2) {
        update_hoverboard_girl_boobs_animation(moby, state);
        if (big_head_cheat_enabled != 0)
            state->joints[0].target_scale = 2.75f;
        update_joint_animation(moby,
                               &state->joints[0],
                               0,
                               rotation_step * frame_scale_sq,
                               rotation_limit * frame_scale_sq);
        update_joint_animation(moby,
                               &state->joints[1],
                               1,
                               rotation_step * frame_scale_sq,
                               rotation_limit * frame_scale_sq);
    }
}
