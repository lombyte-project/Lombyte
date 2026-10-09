#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "sda.h"
#include "eetypes.h"
#include "qcopy.h"

/* Class 918 (hoverboard_girl), Rilgar / Blackwater City.
 * The update counts player action 11 at frame 15 when she faces the player.
 * Its two breast manipulators also carry the effect into cutscene instances. */
typedef union {
    u128 quadword;
    f32 component[4];
} HoverboardGirlVector;
struct HoverboardGirlMoby;

typedef struct {
    u8 attached_flags[2];
    u8 pad02[0x5E];
    f32 target_rotation[3];
    u8 pad6C[4];
    f32 target_scale;
    u8 pad74[4];
    struct HoverboardGirlMoby *owner;
    u8 pad7C[4];
} HoverboardGirlJoint;

typedef struct {
    u8 pad00[4];
    s16 previous_dialogue_state;
    u8 pad06[2];
    u8 race_return_pending;
    u8 pad09[0x17];
    const char *dialogue_name;
    u8 pad24[0x12];
    s16 dialogue_state;
    s32 dialogue_state_frame;
    u8 pad3C[4];
    HoverboardGirlJoint joints[4];
    u8 pad240[4];
    s32 idle_animation_timer;
    s32 player_look_timer;
    s32 random_look_timer;
    HoverboardGirlVector look_target;
    s32 breast_growth_count;
} HoverboardGirlState;

typedef struct HoverboardGirlMoby {
    u8 pad00[0x10];
    HoverboardGirlVector position;
    u8 state;
    u8 pad21[0x27];
    f32 yaw;
    u8 pad4C[7];
    u8 animation;
    u8 pad54[0x1C];
    u8 animation_flags;
    u8 pad71[7];
    HoverboardGirlState *data;
    u8 pad7C[0x2A];
    s16 class_id;
    u8 padA8[0x14];
    u8 dialogue_event;
} HoverboardGirlMoby;

typedef struct {
    u8 pad00[0x10C];
    s32 first_gate;
    u8 pad110[0xC];
    s32 second_gate;
} HoverboardRaceGates;

typedef struct {
    u8 pad00[4];
    s16 state;
} HoverboardRaceEntry;

/* Partial resident player-data view: addresses verified in the retail loads. */

typedef struct {
    f32 delta;
    f32 frame_scale;
    f32 movement_sampling_scale;
} FrameTiming;
extern FrameTiming frame_timing __asm__("D_0015ED64");
extern FrameTiming frame_timing_absolute __asm__("D_0015ED64");
extern u8 big_head_cheat_enabled __asm__("D_0015EDB0");
extern s32 breast_growth_count __asm__("D_L05_00161ED0") __attribute__((sda));
extern s32 game_mode __asm__("D_L05_0015F5C4");
extern s32 game_mode_absolute __asm__("D_L05_0015F5C4");
extern s32 frame_number __asm__("D_L05_0015F5CC");
extern s32 frame_number_absolute __asm__("D_L05_0015F5CC");
extern u8 race_completed __asm__("D_0013D388") NOT_SDA;
extern HoverboardRaceGates race_gates __asm__("D_0013D5B0");
extern HoverboardRaceEntry *race_entries[] __asm__("D_L05_001B1AF8");
extern const char race_girl_name[] __asm__("D_L05_00161ED8");
extern HoverboardGirlVector race_camera_vectors[] __asm__("D_L05_00215BC0");
extern HoverboardGirlVector saved_camera_vectors[] __asm__("D_00141050");

extern s32 scale_frame_count(s32) __asm__("FUN_001f96f8");
extern f32 scale_time(f32) __asm__("FUN_001f96b0");
extern s32 advance_timer(s32 *) __asm__("FUN_001f9740");
extern s32 truncate_time(f32) __asm__("FUN_001fa6d0");
extern f32 random_float(f32, f32) __asm__("FUN_002132a8");
extern s32 random_remainder(s32) __asm__("FUN_00213260");
extern void blend_moby_animation(HoverboardGirlMoby *, s32, s32, s32) __asm__("FUN_00212f90");
extern f32 find_ground_height(f32, HoverboardGirlVector *, s32) __asm__("FUN_00213508");
extern s32 get_dialogue_entry(HoverboardGirlMoby *) __asm__("FUN_L00_002667d0");
extern void initialize_npc_dialogue(HoverboardGirlMoby *,
                                    HoverboardGirlState *) __asm__("FUN_L00_002668a0");
extern void initialize_moby_grounding(HoverboardGirlMoby *) __asm__("FUN_L02_0025c758");
extern s32 update_npc_dialogue(HoverboardGirlMoby *,
                               HoverboardGirlState *) __asm__("FUN_L00_00266448");
extern void begin_dialogue_camera_transition(f32, HoverboardGirlMoby *) __asm__("FUN_L01_002783a8");
extern void set_race_camera(HoverboardGirlVector *, HoverboardGirlVector *, s32,
                            s32) __asm__("FUN_L00_00216f90");
extern void set_race_camera_mode(s32, s32) __asm__("FUN_L01_0027a248");
extern void queue_dialogue_message(s32, s32) __asm__("FUN_L00_00263d40");
extern void func_0020b178(s32, s64) __asm__("FUN_0020b178");
extern f32 vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern f32 angle_from_xy(f32, f32) __asm__("FUN_001f9e90");
extern f32 absolute_angle_difference(f32, f32) __asm__("FUN_001fa688");
extern f32 vector_length(void *) __asm__("FUN_001f9af0");
extern f32 add_angle(f32, f32) __asm__("FUN_001fa580");
extern void vector_from_angles(f32, f32, f32, void *) __asm__("FUN_00214db0");
extern void add_vector(void *, void *, void *) __asm__("FUN_001f9a10");
extern void subtract_vector(void *, void *, void *) __asm__("FUN_001f9a28");
extern f32 signed_angle_difference(f32, f32) __asm__("FUN_001fa5c8");
extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");
extern void update_joint_animation(HoverboardGirlMoby *, HoverboardGirlJoint *, s32, f32,
                                   f32) __asm__("FUN_L00_002628d8");
extern f32 fast_cos(f32) __asm__("FUN_001f9de0");
extern void
update_hoverboard_girl_cutscene_instances(HoverboardGirlMoby *) __asm__("FUN_L05_00316990");
void update_hoverboard_girl_boobs_animation(HoverboardGirlMoby *,
                                            HoverboardGirlState *) __asm__("FUN_L05_00316810");
void update_hoverboard_girl(HoverboardGirlMoby *) __asm__("FUN_L05_00316ab8");

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L05_00316810.s", FUN_L05_00316810);
#else
void update_hoverboard_girl_boobs_animation(HoverboardGirlMoby *moby, HoverboardGirlState *state) {
    extern FrameTiming frame_timing_gp __asm__("D_0015ED64") __attribute__((sda));
    s32 joint_index = 0;
    HoverboardGirlJoint *joint = &state->joints[2];
    HoverboardGirlState *state_window = state;
    f32 delta;
    f32 animation_value;
    do {
        delta = frame_timing_gp.delta;
        if (breast_growth_count != 0) {
            animation_value = (f32)breast_growth_count * 0.15f + 1.0f;
            if (3.7f < animation_value) {
                animation_value = 3.7f;
            }
            state_window->joints[2].target_scale = animation_value;
            animation_value = (f32)(frame_number & 0x7F) * 0.0078125f;
            state_window->joints[2].target_rotation[1] =
                fast_cos(animation_value * 3.1415927f) * 0.61086524f;
            delta = frame_timing_absolute.delta;
        }
        update_joint_animation(moby, joint, joint_index++ + 2, delta * 0.05f, delta * 0.3f);
        joint++;
        /* Retail advances both its state window and joint cursor by 0x80. */
        state_window = (HoverboardGirlState *)((u8 *)state_window + 0x80);
    } while (joint_index < 2);
}
#endif

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L05_00316ab8.s", FUN_L05_00316ab8);
#else
void update_hoverboard_girl(HoverboardGirlMoby *moby) {
    HoverboardGirlState *state = moby->data;
    HoverboardGirlVector target_position;
    HoverboardGirlVector head_position;
    HoverboardGirlVector direction;
    s32 animation;
    s32 look_enabled;
    f32 rotation_step;
    f32 rotation_limit;
    f32 heading;
    f32 yaw;
    f32 pitch;
    f32 delta;

    update_hoverboard_girl_cutscene_instances(moby);
    switch (moby->state) {
    case 0:
        if (moby->animation != 0) {
            blend_moby_animation(moby, 0, 0, scale_frame_count(20));
        }
        moby->position.component[2] = find_ground_height(0.5f, &moby->position, 0);
        if (race_gates.first_gate == 0 || race_gates.second_gate == 0) {
            HoverboardRaceEntry **entries = race_entries;
            s32 entry = get_dialogue_entry(moby);
            entries[entry]->state = 13;
        }
        state->dialogue_name = race_girl_name;
        moby->state = 1;
        initialize_npc_dialogue(moby, state);
        initialize_moby_grounding(moby);
        break;
    case 1:
        if (update_npc_dialogue(moby, state) != 0) {
            begin_dialogue_camera_transition(2.0f, moby);
            moby->state = 3;
        }
        if (state->dialogue_state == 3) {
            state->previous_dialogue_state = state->dialogue_state;
            state->dialogue_state_frame = frame_number_absolute;
            state->dialogue_state = 2;
            moby->dialogue_event = 0;
            set_race_camera(&race_camera_vectors[0], &race_camera_vectors[1], 0, 1);
            qcopy(&saved_camera_vectors[0], &race_camera_vectors[2]);
            qcopy(&saved_camera_vectors[1], &race_camera_vectors[3]);
            set_race_camera_mode(2, 6);
        }
        if (moby->dialogue_event == 1) {
            set_race_camera(&race_camera_vectors[2], &race_camera_vectors[3], 0, 1);
            set_race_camera_mode(0, 8);
            if (race_completed == 0) {
                state->previous_dialogue_state = state->dialogue_state;
                state->dialogue_state = 4;
                state->dialogue_state_frame = frame_number_absolute;
                state->race_return_pending = 1;
            }
            moby->dialogue_event = 0;
        }
        if (moby->dialogue_event == 2) {
            set_race_camera_mode(0, 8);
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
            advance_timer(&state->idle_animation_timer)) {
            state->idle_animation_timer = truncate_time(scale_time(random_float(1200.0f, 2400.0f)));
            if (moby->animation != 1) {
                blend_moby_animation(moby, 1, 0, scale_frame_count(10));
            }
        } else if (moby->animation_flags & 2) {
            animation = random_remainder(2) != 0 ? 0 : 2;
            if (moby->animation != animation) {
                blend_moby_animation(moby, animation, 0, scale_frame_count(10));
            }
        }
        break;
    case 3:
        if (game_mode != 2) {
            moby->state = 1;
            if (state->previous_dialogue_state == 4) {
                race_completed = 1;
                queue_dialogue_message(0x1395, scale_frame_count(300));
                func_0020b178(0, -1);
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
        if (vector_distance(&moby->position, &hero.motion.pos) < 8.0f &&
            absolute_angle_difference(
                moby->yaw,
                angle_from_xy(hero.motion.unkD0.component[0] - moby->position.component[0],
                              hero.motion.unkD0.component[1] -
                                  moby->position.component[1])) < 1.5707964f) {
            if (vector_length(&hero.motion.unk100) > 0.01f) {
                state->player_look_timer = scale_frame_count(120);
            } else {
                advance_timer(&state->player_look_timer);
            }
        } else if (state->player_look_timer != 0) {
            state->player_look_timer = 0;
            qcopy(&state->look_target, &hero.motion.unkD0);
        }
        if (advance_timer(&state->random_look_timer)) {
            state->random_look_timer = truncate_time(scale_time(random_float(180.0f, 300.0f)));
            heading = add_angle(moby->yaw, random_float(-90.0f, 90.0f) * DEG_TO_RAD);
            pitch = random_float(0.0f, 30.0f) * DEG_TO_RAD;
            vector_from_angles(6.0f, heading, pitch, &state->look_target);
            add_vector(&state->look_target, &state->look_target, &moby->position);
        }
        if (state->player_look_timer != 0) {
            qcopy(&target_position, &hero.motion.unkD0);
            rotation_step = 0.03f;
            rotation_limit = 0.3f;
        } else {
            qcopy(&target_position, &state->look_target);
        }
    }
    if (look_enabled) {
        qcopy(&head_position, &moby->position);
        head_position.component[2] += 1.0f;
        subtract_vector(&direction, &target_position, &head_position);
        yaw = signed_angle_difference(angle_from_xy(direction.component[0], direction.component[1]),
                                      moby->yaw);
        pitch = -angle_from_xy(vector_length_xy(&direction), direction.component[2]);
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
    if (vector_distance(&moby->position, &hero.motion.pos) < 15.0f &&
        absolute_angle_difference(
            moby->yaw,
            angle_from_xy(hero.motion.pos.component[0] - moby->position.component[0],
                          hero.motion.pos.component[1] - moby->position.component[1])) <
            1.2217305f) {
        if (hero.state.current == 11 && hero.state_timer == 15) {
            state->breast_growth_count++;
            if (state->breast_growth_count > 20)
                state->breast_growth_count = 20;
            breast_growth_count = state->breast_growth_count;
        }
        /* Action 4 eventually reduces the effect, one step
         * every tenth game frame. Keep the scaled-frame threshold. */
        if (hero.state.current == 4 && scale_frame_count(20) < hero.state_timer &&
            frame_number % 10 == 0) {
            state->breast_growth_count--;
            if (state->breast_growth_count < 0)
                state->breast_growth_count = 0;
            breast_growth_count = state->breast_growth_count;
        }
    }
    if (game_mode_absolute != 2) {
        update_hoverboard_girl_boobs_animation(moby, state);
        if (big_head_cheat_enabled != 0) {
            state->joints[0].target_scale = 2.75f;
            delta = frame_timing_absolute.delta;
        } else {
            delta = frame_timing.delta;
        }
        update_joint_animation(moby, &state->joints[0], 0, rotation_step * delta,
                               rotation_limit * delta);
        update_joint_animation(moby, &state->joints[1], 1,
                               rotation_step * frame_timing_absolute.delta,
                               rotation_limit * frame_timing_absolute.delta);
    }
}
#endif
