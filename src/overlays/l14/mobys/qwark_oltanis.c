/* Level 14 Qwark (Oltanis) moby class and the elevator state it sets. */
#include "types.h"
#include "rnc/overlay/level_save_state.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "qcopy.h"
#include "rnc/game/game_globals.h"
#include "rnc/game/options.h"
#include "rnc/math/fast_math.h"
#include "rnc/game/item_state.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/cutscenes/update_npc_dialogue.h"
#include "rnc/overlay/shared/game/restore_or_clear_level_state.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/mobys/attach_big_head_cheat_manipulator.h"
#include "rnc/math/random.h"
#include "rnc/overlay/cuboid.h"

extern char D_L14_001674C0[];

extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");

extern int D_L14_0015F5C4;

typedef struct {
    char  pad0[0xAC];
    float value;
    char  padB0[0x74];
    short field124;
} Level14VendorData;

typedef struct {
    char               pad0[0x78];
    Level14VendorData *data;
    char               pad7C[0x40];
    unsigned char      state;
} Level14VendorMoby;

void enable_elevator_alternate_route(Level14VendorMoby *moby) __asm__("FUN_L14_002ef578");

void enable_elevator_alternate_route(Level14VendorMoby *moby) {
    Level14VendorData *data = moby->data;
    moby->state = data->value < 0.0f ? 2 : 1;
    data->field124 = 1;
}

extern int D_L14_001BABD0[];

void mark_elevator_collected(Level14VendorMoby *vendor_moby) __asm__("FUN_L14_002ef5a8");

void mark_elevator_collected(Level14VendorMoby *vendor_moby) {
    vendor_moby->state = 0;
    vendor_moby->data->field124 = 0;
    D_0014C190_257218[current_level_index]
                     [(short)*(unsigned short *)((char *)vendor_moby + 0xB2) >> 5] |=
        1 << (*(unsigned short *)((char *)vendor_moby + 0xB2) & 0x1F);
    D_L14_001BABD0[(short)*(unsigned short *)((char *)vendor_moby + 0xB2) >> 5] |=
        1 << (*(unsigned short *)((char *)vendor_moby + 0xB2) & 0x1F);
}

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
    char           pad4C[0x34];
    char           body[0x64];
    float          pitch;
    float          yaw;
    char           padEC[4];
    float          height;
    char           padF4[0xC];
    char           head[0x68];
    float          head_yaw;
    char           pad16C[0x14];
    float          target[4];
    char           pad190[4];
    int            moving_timer;
    int            target_timer;
} L14WatchData;

typedef struct {
    char  pad00[0xD0];
    float aim[4];
} L14Player;

extern float scale_time(float) __asm__("FUN_001f96b0");

extern int FUN_001fa6d0(float);

extern void FUN_L00_0025a120(void *);

extern void update_joint_animation(float, float, void *, void *, int) __asm__("FUN_L00_002628d8");

extern LevelSaveState D_L14_001BB930;

extern char captain_qwark_name[] __asm__("D_L14_00161F20");

extern unsigned char *D_L14_0015FFD8;

extern void begin_dialogue_camera_transition(void *, float) __asm__("FUN_L01_002783a8");

void mark_elevator_collected(Level14VendorMoby *m) __asm__("FUN_L14_002ef5a8");

extern void FUN_L00_00263d40(int, int);

extern void memcard_save_data(int, int) __asm__("FUN_0020b178");

void qwark_oltanis_update(L14WatchMoby *m) __asm__("FUN_L14_002fba20");

void qwark_oltanis_update(L14WatchMoby *moby) {
    enum {
        QWARK_OLTANIS_STATE_INITIALIZE_DIALOGUE = 0,
        QWARK_OLTANIS_STATE_UPDATE_DIALOGUE = 1,
        QWARK_OLTANIS_STATE_POST_DIALOGUE = 2,
        QWARK_OLTANIS_STATE_REMOVE = 3
    };
    L14WatchScratch scratch;
    L14WatchData   *moby_vars = (L14WatchData *)moby->data;
    float           rate;
    float           head_rate;
    int             tracking;
    attach_big_head_cheat_manipulator_to_matching_moby(moby);
    if (moby->near_flag) {
        if (fast_vec_dist(moby->position, D_L14_001674C0) < 28.0f) {
            FUN_L00_0025a120(moby);
            moby->flag7F = 0x16;
        }
    }
    switch (moby->state) {
    case QWARK_OLTANIS_STATE_INITIALIZE_DIALOGUE: {
        unsigned short id = moby->hB2;
        if (D_L14_001BB930.cleared[(short)id] != 0 ||
            (D_0014C190_257218[current_level_index][(short)id >> 5] >> (id & 0x1F)) & 1) {
            mark_moby_for_removal(moby);
            return;
        }
        if (moby_vars->i44 == -1 || moby_vars->i48 == -1) {
            mark_moby_for_removal(moby);
            return;
        }
        moby_vars->p20 = (void *)captain_qwark_name;
        initialize_npc_dialogue(moby, moby_vars);
        moby_vars->b8 = 1;
        moby->state = 1;
        break;
    }
    case QWARK_OLTANIS_STATE_UPDATE_DIALOGUE:
        if (moby_vars->b8 == 1 && is_point_inside_clip_volume(&hero.motion.pos, moby_vars->i40)) {
            if (visited52[moby->bB0 + current_level_index * 16] != 0xFF) {
                char *camera_point;
                jet_clear_persistent(moby->bB0);
                camera_point = (char *)l14_cuboids + moby_vars->i48 * 128;
                jet_reset_camera(camera_point + 0x30, camera_point + 0x70);
            }
            enable_elevator_alternate_route(D_L14_0015FFD8 + moby_vars->i44 * 256);
            moby_vars->fC = 255.0f;
        } else {
            moby_vars->fC = 1.7f;
        }
        if (update_npc_dialogue(moby, moby_vars)) {
            begin_dialogue_camera_transition(moby, 3.0f);
            moby->state = 2;
        }
        break;
    case QWARK_OLTANIS_STATE_POST_DIALOGUE:
        if (D_L14_0015F5C4 != 2) {
            moby->state = 1;
            if ((unsigned)(moby_vars->sub - 3) < 2) {
                if ((short)moby_vars->sub == 4)
                    FUN_L00_00263d40(0x53DD, -1);
                else
                    FUN_L00_00263d40(0x36B3, -1);
                grant_inventory_item(0x20, 1);
                mark_elevator_collected(D_L14_0015FFD8 + moby_vars->i44 * 256);
                D_0014C190_257218[current_level_index][(short)moby->hB2 >> 5] |=
                    1 << (moby->hB2 & 0x1F);
                D_L14_001BABD0[(short)moby->hB2 >> 5] |= 1 << (moby->hB2 & 0x1F);
                memcard_save_data(0, -1);
                mark_moby_for_removal(moby);
                return;
            }
        }
        break;
    case QWARK_OLTANIS_STATE_REMOVE:
        mark_moby_for_removal(moby);
        return;
    }
    rate = 0.02f;
    head_rate = 0.3f;
    tracking = 0;
    if (moby->animation == 0) {
        char      *hero_position = (char *)&hero.motion.pos;
        L14Player *hero_data;
        tracking = 1;
        if (vector_distance_xy(moby->position, hero_position) < 8.0f &&
            (hero_data = (L14Player *)(hero_position - 0x80),
             fast_difference_between_rotations(
                 moby->yaw,
                 fast_arc_tan(hero_data->aim[0] - moby->position[0],
                              hero_data->aim[1] - moby->position[1]))) < 1.5707964f) {
            if (fast_vec_length(hero_position + 0x80) > 0.01f)
                moby_vars->moving_timer = scale_game_frames(120);
            else
                fast_dec_timer(&moby_vars->moving_timer);
        } else if (moby_vars->moving_timer) {
            moby_vars->moving_timer = 0;
            qcopy(moby_vars->target, hero.motion.body_center.f);
        }
        if (fast_dec_timer(&moby_vars->target_timer)) {
            float heading;
            moby_vars->target_timer =
                FUN_001fa6d0(scale_time(random_float_between(180.0f, 300.0f)));
            heading =
                fast_add_rotations(moby->yaw, random_float_between(-90.0f, 90.0f) * DEG_TO_RAD);
            build_spherical_offset(
                moby_vars->target, 6.0f, heading, random_float_between(0.0f, 30.0f) * DEG_TO_RAD);
            fast_vec_add(moby_vars->target, moby_vars->target, moby->position);
        }
        if (moby_vars->moving_timer) {
            qcopy(scratch.target, hero.motion.body_center.f);
            rate = 0.04f;
            head_rate = 0.3f;
        } else {
            qcopy(scratch.target, moby_vars->target);
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
        moby_vars->pitch = pitch;
        moby_vars->yaw = yaw * 0.6f;
        moby_vars->head_yaw = yaw * 0.4f;
    }
    if (big_head_cheat_enabled)
        moby_vars->height = 2.75f;
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, moby, moby_vars->body, 0);
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, moby, moby_vars->head, 1);
}

extern float vector_length_xy(void *);
