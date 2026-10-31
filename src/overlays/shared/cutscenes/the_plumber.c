/* Shared moby class the_plumber: the plumber NPC, with a per-level dialogue script and head tracking. */

#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/mobys/attach_manipulator.h"
#include "rnc/overlay/shared/cutscenes/update_npc_dialogue.h"
#include "rnc/overlay/shared/game/restore_or_clear_level_state.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/game/queue_planet_unlock_message.h"
#include "rnc/overlay/shared/mobys/record_moby_context_bytes.h"
#include "rnc/overlay/cuboid.h"
#include "rnc/game/options.h"
#include "rnc/game/game_globals.h"
#include "rnc/overlay/level_labels.h"

typedef struct {
    char  pad0[0x44];
    short n;
    char  pad1[0x132];
    char *m[1];
} G_q;

typedef struct {
    float target[4];
    float eye[4];
    float delta[4];
} L01WatchScratch;

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
} L01WatchMoby;

typedef struct {
    char          pad00[4];
    short         sub;
    short         h6;
    unsigned char b8;
    char          pad09[0x17];
    void         *p20;
    char          pad24[0x20];
    int           i44;
    char          pad48[4];
    int           i4C;
    char          body[0x64];
    float         pitch;
    float         yaw;
    char          padBC[4];
    float         height;
    char          padC4[0xC];
    char          head[0x68];
    float         head_yaw;
    char          pad13C[0x14];
    float         target[4];
    char          pad160[4];
    int           moving_timer;
    int           target_timer;
} L01WatchData;

typedef struct {
    char  pad00[0xD0];
    float aim[4];
} L01Player;

extern f32  scale_time(f32) __asm__("FUN_001f96b0");
extern void FUN_L00_0025a120(void *);
extern void update_joint_animation(float, float, void *, void *, int) __asm__("FUN_L00_002628d8");
extern void begin_dialogue_camera_transition(void *, float) __asm__("FUN_L01_002783a8");
extern void memcard_save_data(int, int) __asm__("FUN_0020b178");

extern G_q                      D_L01_0016CCE0_q __asm__("D_L01_0016CCE0");
extern unsigned char            D_L01_0017C8C0_lui[] __asm__("D_L01_0017C8C0") NOT_SDA;
float                           cutscene_big_head_scale_gp __asm__("D_L01_00161D30") = 2.1f;
extern unsigned char D_0013DD42 NOT_SDA;
extern char                     plumber_level_one_dialogue_data[] __asm__("D_L01_0020B600");
extern char                     plumber_level_eight_dialogue_data[] __asm__("D_L01_00161D38");
extern char                     plumber_fallback_dialogue_data[] __asm__("D_L01_00161D48");
extern char                     D_L01_00167240_c[] __asm__("D_L01_00167240");
extern char                     D_L01_001BAAA0[];

extern struct {
    char  pad[0x80];
    float f80;
    float f84;
    float f88;
} D_0013F350_w __asm__("D_0013F350");

float D_L08_00161FB0 = 2.1f;

/* Resets the marker record's scale once, for mobys of the caller's class or class 0x32B. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002F7700.c: func_L01_00300400), where it is exact; names translated to the US level program. */
void attach_cutscene_big_head_manipulator(struct Moby *class_reference_moby) __asm__("FUN_L01_002ff028");

void attach_cutscene_big_head_manipulator(struct Moby *class_reference_moby) {
    if (cutscene_mode == 2 && big_head_cheat_enabled != 0) {
        int i;
        for (i = 0; i < D_L01_0016CCE0_q.n; i++) {
            char *moby = D_L01_0016CCE0_q.m[i];
            short c = MOBY(moby)->oclass;
            if (c == class_reference_moby->oclass || c == 0x32B) {
                unsigned char *manipulator_record = D_L01_0017C8C0_lui;
                if (manipulator_record[1] == 0) {
                    attach_manipulator(moby, 0, manipulator_record);
                    *(float *)(manipulator_record + 0x20) = cutscene_big_head_scale_gp;
                    *(float *)(manipulator_record + 0x24) = cutscene_big_head_scale_gp;
                    *(float *)(manipulator_record + 0x28) = cutscene_big_head_scale_gp;
                }
            }
        }
    }
}

/* Watcher moby update with a level-specific state script, then turns its head toward the hero or a random target. */
void the_plumber_update(L01WatchMoby *m) __asm__("FUN_L01_002ff118");

/* Moby class 774 */
void the_plumber_update(L01WatchMoby *plumber_moby) {
    enum {
        PLUMBER_INITIALIZE = 0,
        PLUMBER_LEVEL_ONE_DIALOGUE = 1,
        PLUMBER_LEVEL_ONE_POST_DIALOGUE = 2,
        PLUMBER_REMOVE = 3,
        PLUMBER_LEVEL_EIGHT_DIALOGUE = 4,
        PLUMBER_LEVEL_EIGHT_POST_DIALOGUE = 5,
        PLUMBER_LEVEL_EIGHT_DIALOGUE_FINISHED = 6
    };
    L01WatchScratch look_tracking_vectors;
    L01WatchData   *plumber_vars = (L01WatchData *)plumber_moby->data;
    float           rate;
    float           head_rate;
    int             tracking;
    if (!plumber_vars)
        return;
    attach_cutscene_big_head_manipulator(plumber_moby);
    if (plumber_moby->near_flag) {
        if (fast_vec_dist(plumber_moby->position, D_L01_00167240_c) < 32.0f) {
            FUN_L00_0025a120(plumber_moby);
            plumber_moby->flag7F = 0x1A;
        }
    }
    switch (plumber_moby->state) {
    case PLUMBER_INITIALIZE:
        plumber_moby->opacity = 0xFF;
        plumber_vars->i44 = 0;
        if (current_level_index == 1) {
            if (D_0013DD42) {
                mark_moby_for_removal(plumber_moby);
                return;
            }
            plumber_vars->p20 = (void *)plumber_level_one_dialogue_data;
            plumber_moby->state = 1;
        } else if (current_level_index == 8) {
            plumber_vars->p20 = (void *)plumber_level_eight_dialogue_data;
            plumber_moby->state = 4;
        } else {
            plumber_vars->p20 = (void *)plumber_fallback_dialogue_data;
            plumber_moby->state = 7;
        }
        initialize_npc_dialogue(plumber_moby, plumber_vars);
        break;
    case PLUMBER_LEVEL_ONE_DIALOGUE:
        if (update_npc_dialogue(plumber_moby, plumber_vars)) {
            begin_dialogue_camera_transition(plumber_moby, 2.2f);
            plumber_moby->state = 2;
        }
        break;
    case PLUMBER_LEVEL_ONE_POST_DIALOGUE:
        if (cutscene_mode == 2)
            break;
        if (visited52[plumber_moby->bB0 + current_level_index * 16] != 0xFF) {
            jet_clear_persistent(plumber_moby->bB0);
            if (plumber_vars->i4C != -1) {
                LevelCuboid *camera_record = &l01_cuboids[plumber_vars->i4C];
                jet_reset_camera(&camera_record->pos, &camera_record->rot);
            }
        }
        plumber_moby->state = 1;
        if (plumber_vars->sub == 4) {
            unlock_planet(2);
            queue_planet_unlock_message(2);
            plumber_moby->state = 3;
            record_moby_context_bytes(
                (char *)plumber_moby + 0x20, 1, plumber_moby, 1, D_L01_001BAAA0);
            memcard_save_data(0, -1);
        }
        break;
    case PLUMBER_REMOVE:
        mark_moby_for_removal(plumber_moby);
        return;
    case PLUMBER_LEVEL_EIGHT_DIALOGUE:
        if (update_npc_dialogue(plumber_moby, plumber_vars)) {
            plumber_moby->state = 5;
            plumber_vars->b8 = 0;
        }
        break;
    case PLUMBER_LEVEL_EIGHT_POST_DIALOGUE:
        if (cutscene_mode == 2)
            break;
        if (plumber_vars->h6 == 2) {
            D_0013F350_w.f80 = 130.5f;
            D_0013F350_w.f84 = 299.5f;
            D_0013F350_w.f88 = 46.0f;
            plumber_moby->state = 6;
        } else {
            plumber_moby->state = 4;
        }
        break;
    case PLUMBER_LEVEL_EIGHT_DIALOGUE_FINISHED:
        break;
    }
    rate = 0.02f;
    head_rate = 0.3f;
    tracking = 0;
    if (plumber_moby->animation == 0) {
        char      *hero_position = (char *)&hero.motion.pos;
        L01Player *hero_aim_view;
        tracking = 1;
        if (vector_distance_xy(plumber_moby->position, hero_position) < 8.0f &&
            (hero_aim_view = (L01Player *)(hero_position - 0x80),
             fast_difference_between_rotations(
                 plumber_moby->yaw,
                 fast_arc_tan(hero_aim_view->aim[0] - plumber_moby->position[0],
                              hero_aim_view->aim[1] - plumber_moby->position[1]))) < 1.5707964f) {
            if (fast_vec_length(hero_position + 0x80) > 0.01f)
                plumber_vars->moving_timer = scale_game_frames(120);
            else
                fast_dec_timer(&plumber_vars->moving_timer);
        } else if (plumber_vars->moving_timer) {
            plumber_vars->moving_timer = 0;
            qcopy(plumber_vars->target, hero.motion.body_center.f);
        }
        if (fast_dec_timer(&plumber_vars->target_timer)) {
            float heading;
            plumber_vars->target_timer =
                truncate_float_to_s32(scale_time(random_float_between(180.0f, 300.0f)));
            heading = fast_add_rotations(plumber_moby->yaw,
                                         random_float_between(-90.0f, 90.0f) * DEG_TO_RAD);
            build_spherical_offset(plumber_vars->target,
                                   6.0f,
                                   heading,
                                   random_float_between(0.0f, 30.0f) * DEG_TO_RAD);
            fast_vec_add(plumber_vars->target, plumber_vars->target, plumber_moby->position);
        }
        if (plumber_vars->moving_timer) {
            qcopy(look_tracking_vectors.target, hero.motion.body_center.f);
            rate = 0.04f;
            head_rate = 0.3f;
        } else {
            qcopy(look_tracking_vectors.target, plumber_vars->target);
        }
    }
    if (tracking) {
        float yaw;
        float pitch;
        qcopy(look_tracking_vectors.eye, plumber_moby->position);
        look_tracking_vectors.eye[2] += 1.0f;
        fast_vec_sub(
            look_tracking_vectors.delta, look_tracking_vectors.target, look_tracking_vectors.eye);
        yaw = fast_subtract_rotations(
            fast_arc_tan(look_tracking_vectors.delta[0], look_tracking_vectors.delta[1]),
            plumber_moby->yaw);
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
        plumber_vars->yaw = yaw * 0.5f;
        plumber_vars->pitch = pitch * 1.25f;
        plumber_vars->head_yaw = yaw * 0.5f;
    }
    if (big_head_cheat_enabled)
        plumber_vars->height = 2.75f;
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, plumber_moby, plumber_vars->body, 0);
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, plumber_moby, plumber_vars->head, 1);
}
