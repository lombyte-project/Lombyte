/* Level 14 Sam moby class. */
#include "types.h"
#include "rnc/overlay/level_save_state.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "qcopy.h"
#include "rnc/game/game_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/cutscenes/update_npc_dialogue.h"
#include "rnc/overlay/shared/game/restore_or_clear_level_state.h"
#include "rnc/overlay/shared/game/queue_planet_unlock_message.h"
#include "rnc/overlay/shared/mobys/attach_big_head_cheat_manipulator.h"
#include "rnc/overlay/cuboid.h"
#include "rnc/game/options.h"

extern int unlock_planet() __asm__("FUN_L00_002607d0");

typedef struct {
    float target[4];
    float eye[4];
    float delta[4];
} L14WatchScratch;

typedef struct {
    char           pad00[0x10];
    float          position[4];
    unsigned char  state;
    char           pad21[0xF];
    unsigned char  opacity;
    unsigned char  near_flag;
    char           pad32[0x16];
    float          yaw;
    char           pad4C[7];
    unsigned char  animation;
    char           pad54[0x24];
    char          *data;
    char           pad7C[3];
    unsigned char  flag7F;
    char           pad80[0x30];
    unsigned char  bB0;
    char           padB1;
    unsigned short hB2;
} L14WatchMoby;

typedef struct {
    char           pad00[4];
    unsigned short sub;
    char           pad06[2];
    unsigned char  b8;
    char           pad09[3];
    float          fC;
    char           pad10[0x10];
    void          *p20;
    char           pad24[0x1C];
    int            i40;
    int            i44;
    int            i48;
    char           pad4C[4];
    char           body[0x64];
    float          pitch;
    float          yaw;
    char           padBC[4];
    float          height;
    char           padC4[0xC];
    char           head[0x68];
    float          head_yaw;
    char           pad13C[0x14];
    float          target[4];
    int            moving_timer;
    int            target_timer;
} L14WatchData;

typedef struct {
    char  pad00[0xD0];
    float aim[4];
} L14Player;

extern float scale_time(float) __asm__("FUN_001f96b0");

extern void update_joint_animation(float, float, void *, void *, int) __asm__("FUN_L00_002628d8");

extern LevelSaveState D_L14_001BB930;

extern int D_L14_001BABD0[];

extern char sam_dialogue_name_text[] __asm__("D_L14_00161FE8");

extern int D_L14_0015F5C4;

extern unsigned char D_0013DD4F NOT_SDA;

extern struct {
    char         pad[0x10];
    unsigned int q10[4] __attribute__((aligned(16)));
    char         pad20[8];
    float        f28;
    char         pad2C[0x1A];
    short        h46;
} D_L14_0016CF60;

extern void memcard_save_data(int, int) __asm__("FUN_0020b178");

void sam_update(L14WatchMoby *m) __asm__("FUN_L14_002fefe0");

void sam_update(L14WatchMoby *moby) {
    enum {
        SAM_STATE_INITIALIZE = 0,
        SAM_STATE_DIALOGUE = 1,
        SAM_STATE_POST_DIALOGUE_WAIT = 2,
        SAM_STATE_REMOVE = 3
    };
    L14WatchScratch scratch;
    L14WatchData   *moby_data = (L14WatchData *)moby->data;
    float           rate;
    float           head_rate;
    int             tracking;
    attach_big_head_cheat_manipulator_to_matching_moby(moby);
    switch (moby->state) {
    case SAM_STATE_INITIALIZE: {
        unsigned short id;
        if (D_0013DD4F) {
            mark_moby_for_removal(moby);
            return;
        }
        id = moby->hB2;
        if (D_L14_001BB930.cleared[(short)id] != 0 ||
            (D_0014C190_257218[current_level_index][(short)id >> 5] >> (id & 0x1F)) & 1) {
            mark_moby_for_removal(moby);
            return;
        }
        if (moby_data->i44 == -1 || moby_data->i48 == -1) {
            mark_moby_for_removal(moby);
            return;
        }
        moby_data->p20 = (void *)sam_dialogue_name_text;
        initialize_npc_dialogue(moby, moby_data);
        moby->state = 1;
        moby_data->b8 = 1;
        break;
    }
    case SAM_STATE_DIALOGUE:
        if (moby_data->b8 == 1 && is_point_inside_clip_volume(&hero.motion.pos, moby_data->i40)) {
            if (visited52[moby->bB0 + current_level_index * 16] != 0xFF) {
                char *t;
                jet_clear_persistent(moby->bB0);
                t = (char *)l14_cuboids + moby_data->i44 * 128;
                jet_reset_camera(t + 0x30, t + 0x70);
            }
            moby_data->fC = 255.0f;
        } else {
            moby_data->fC = 3.0f;
        }
        if (update_npc_dialogue(moby, moby_data)) {
            char *t = (char *)l14_cuboids + moby_data->i48 * 128;
            D_L14_0016CF60.h46 = 1;
            qcopy(D_L14_0016CF60.q10, t + 0x30);
            clear_u64_value((char *)&D_L14_0016CF60 + 0x20);
            D_L14_0016CF60.f28 = *(float *)(t + 0x78);
            moby->state = 2;
        }
        break;
    case SAM_STATE_POST_DIALOGUE_WAIT:
        if (D_L14_0015F5C4 != 2) {
            moby->state = 1;
            if ((short)moby_data->sub == 4) {
                unlock_planet(0xF);
                queue_planet_unlock_message(0xF);
                D_0014C190_257218[current_level_index][(short)moby->hB2 >> 5] |=
                    1 << (moby->hB2 & 0x1F);
                D_L14_001BABD0[(short)moby->hB2 >> 5] |= 1 << (moby->hB2 & 0x1F);
                memcard_save_data(0, -1);
                mark_moby_for_removal(moby);
                return;
            }
        }
        break;
    case SAM_STATE_REMOVE:
        mark_moby_for_removal(moby);
        return;
    }
    rate = 0.02f;
    head_rate = 0.3f;
    tracking = 0;
    if (moby->animation == 0) {
        char      *hero_position = (char *)&hero.motion.pos;
        L14Player *hero_aim_view;
        tracking = 1;
        if (vector_distance_xy(moby->position, hero_position) < 8.0f &&
            (hero_aim_view = (L14Player *)(hero_position - 0x80),
             fast_difference_between_rotations(
                 moby->yaw,
                 fast_arc_tan(hero_aim_view->aim[0] - moby->position[0],
                              hero_aim_view->aim[1] - moby->position[1]))) < 1.5707964f) {
            if (fast_vec_length(hero_position + 0x80) > 0.01f)
                moby_data->moving_timer = scale_game_frames(120);
            else
                fast_dec_timer(&moby_data->moving_timer);
        } else if (moby_data->moving_timer) {
            moby_data->moving_timer = 0;
            qcopy(moby_data->target, hero.motion.body_center.f);
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
            qcopy(scratch.target, hero.motion.body_center.f);
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
        moby_data->yaw = yaw * 0.6f;
        moby_data->head_yaw = yaw * 0.4f;
    }
    if (big_head_cheat_enabled)
        moby_data->height = 2.75f;
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, moby, moby_data->body, 0);
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, moby, moby_data->head, 1);
}
