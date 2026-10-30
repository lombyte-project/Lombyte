/* Level 08 moby class 0x503: the plumber batalia. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/game/game_globals.h"
#include "rnc/game/options.h"
#include "rnc/math/fast_math.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/hero/hero_switch_item.h"
#include "rnc/mobys/attach_manipulator.h"
#include "rnc/overlay/shared/cutscenes/update_npc_dialogue.h"
#include "rnc/overlay/shared/game/restore_or_clear_level_state.h"
#include "rnc/overlay/shared/game/unlock_planet.h"

extern float D_L08_0015F59C;
extern float D_L08_0015F5A0;
extern int   level_game_mode __asm__("D_L08_0015F5C4");

#include "qcopy.h"

extern void  begin_dialogue_camera_transition(void *, float) __asm__("FUN_L01_002783a8");
extern void  memcard_save_data(int, int) __asm__("FUN_0020b178");
extern float scale_time(float) __asm__("FUN_001f96b0");
extern float vector_length_xy(void *);
extern void  update_joint_animation(float, float, void *, void *, int) __asm__("FUN_L00_002628d8");

/* triggers objects matching the moby's class, with per-segment settings */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002EAF48.c: func_L08_003078B0), where it is exact; names translated to the US level program. */
struct G_3063f0 {
    char  pad0[0x30];
    int   a;
    int   b;
    char  pad1[0xC];
    short n;
    char  pad2[0x132];
    char *objs[1];
};

extern struct G_3063f0 D_L08_0016D060_lui __asm__("D_L08_0016D060") NOT_SDA;
extern unsigned char   big_head_cheat_enabled_macro[] __asm__("D_0015EDB0") MACRO_ADDR;
extern unsigned char   D_L08_0017CC40_lui[] __asm__("D_L08_0017CC40") NOT_SDA;
extern short           batalia_plumber_manipulator_replicated_value __asm__("D_L08_00162300")
    __attribute__((sda));
void attach_batalia_plumber_class_manipulator(struct Moby *moby) __asm__("FUN_L08_003063f0");

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
    char  pad00[4];
    short sub;
    char  pad06[0x1A];
    void *p20;
    char  pad24[0x12];
    short h36;
    char  pad38[0x10];
    int   i48;
    char  pad4C[4];
    int   i50;
    char  pad54[0xC];
    char  body[0x64];
    float pitch;
    float yaw;
    char  padCC[4];
    float height;
    char  padD4[0xC];
    char  head[0x68];
    float head_yaw;
    char  pad14C[0x14];
    float target[4];
    char  pad170[4];
    int   moving_timer;
    int   target_timer;
} L14WatchData;

typedef struct {
    char  pad00[0xD0];
    float aim[4];
} L14Player;

extern float          FUN_001f9b80_c(void *, void *) __asm__("FUN_001f9b80");
extern float          vector_length_xy(void *) __asm__("FUN_001f9b20");
extern int            FUN_001f9740_c(int *) __asm__("FUN_001f9740");
extern int            FUN_001fa6d0(float);
extern char           batalia_plumber_name_text[] __asm__("D_L08_00162308");
extern char          *D_L08_001600EC;
extern unsigned char *D_L08_0015FFD8;
extern void           request_batalia_cannon_mount(void *) __asm__("FUN_L08_002e1698");
extern void           FUN_L08_002a1880(int);

/* Watcher moby update with a scripted reveal, then turns its head toward the hero or a random target. */
void the_plumber_batalia_update(L14WatchMoby *m) __asm__("FUN_L08_003065d8");

#include "rnc/game/item_state.h"

void attach_batalia_plumber_class_manipulator(struct Moby *moby) {
    int i;
    if (level_game_mode == 2 && big_head_cheat_enabled_macro[0]) {
        if (D_L08_0016D060_lui.a == 6 && D_L08_0016D060_lui.b >= 0x26C &&
            D_L08_0016D060_lui.b < 0x2CC) {
            D_L08_0015F59C = 1.0f;
            D_L08_0015F5A0 = -0.2f;
        }
        if (D_L08_0016D060_lui.a == 6 && D_L08_0016D060_lui.b >= 0x47E &&
            D_L08_0016D060_lui.b < 0x5F1) {
            D_L08_0015F59C = 1.0f;
            D_L08_0015F5A0 = 0.5f;
        }
        if (D_L08_0016D060_lui.a == 6 && D_L08_0016D060_lui.b >= 0x5FA &&
            D_L08_0016D060_lui.b < 0x6AE) {
            D_L08_0015F59C = 1.0f;
            D_L08_0015F5A0 = -0.1f;
        }
        for (i = 0; i < D_L08_0016D060_lui.n; i++) {
            char *candidate_moby = D_L08_0016D060_lui.objs[i];
            if (MOBY(candidate_moby)->oclass == moby->oclass ||
                MOBY(candidate_moby)->oclass == 0x306) {
                if (D_L08_0017CC40_lui[1] == 0) {
                    float replicated_value;
                    attach_manipulator(candidate_moby, 0, D_L08_0017CC40_lui);
                    replicated_value = *(float *)&batalia_plumber_manipulator_replicated_value;
                    *(float *)(D_L08_0017CC40_lui + 0x20) = replicated_value;
                    *(float *)(D_L08_0017CC40_lui + 0x24) = replicated_value;
                    *(float *)(D_L08_0017CC40_lui + 0x28) = replicated_value;
                }
            }
        }
    }
}

void the_plumber_batalia_update(L14WatchMoby *moby) {
    enum {
        PLUMBER_INITIALIZE = 0,
        PLUMBER_UPDATE_DIALOGUE = 1,
        PLUMBER_SCRIPTED_HANDOFF = 2,
        PLUMBER_SAVE_AND_REMOVE = 5
    };
    L14WatchScratch scratch;
    L14WatchData   *watch_data = (L14WatchData *)moby->data;
    float           rate;
    float           head_rate;
    int             tracking;
    attach_batalia_plumber_class_manipulator(moby);
    switch (moby->state) {
    case PLUMBER_INITIALIZE:
        moby->opacity = 0xFF;
        if (visited52[moby->bB0 + current_level_index * 16] == 0xFF) {
            mark_moby_for_removal(moby);
            return;
        }
        watch_data->p20 = (void *)batalia_plumber_name_text;
        moby->state = 1;
        initialize_npc_dialogue(moby, watch_data);
        break;
    case PLUMBER_UPDATE_DIALOGUE:
        if (update_npc_dialogue(moby, watch_data))
            begin_dialogue_camera_transition(moby, 2.7f);
        if (watch_data->h36 == 2) {
            char *camera_record = D_L08_001600EC + watch_data->i48 * 128;
            jet_reset_camera(camera_record + 0x30, camera_record + 0x70);
            moby->state = 2;
            request_batalia_cannon_mount(D_L08_0015FFD8 + watch_data->i50 * 256);
        }
        if (watch_data->sub == 3) {
            FUN_L08_002a1880(1);
            moby->state = 5;
        }
        if (watch_data->sub == 4) {
            hero_respawn();
            watch_data->sub = -1;
        }
        break;
    case PLUMBER_SCRIPTED_HANDOFF:
        break;
    case PLUMBER_SAVE_AND_REMOVE:
        jet_clear_persistent(moby->bB0);
        grant_inventory_item(0x1B, 1);
        memcard_save_data(0, -1);
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
        if (FUN_001f9b80_c(moby->position, hero_position) < 8.0f &&
            (hero_aim_view = (L14Player *)(hero_position - 0x80),
             fast_difference_between_rotations(
                 moby->yaw,
                 fast_arc_tan(hero_aim_view->aim[0] - moby->position[0],
                              hero_aim_view->aim[1] - moby->position[1]))) < 1.5707964f) {
            if (fast_vec_length(hero_position + 0x80) > 0.01f)
                watch_data->moving_timer = scale_game_frames(120);
            else
                FUN_001f9740_c(&watch_data->moving_timer);
        } else if (watch_data->moving_timer) {
            watch_data->moving_timer = 0;
            qcopy(watch_data->target, hero.motion.body_center.f);
        }
        if (FUN_001f9740_c(&watch_data->target_timer)) {
            float heading;
            watch_data->target_timer =
                FUN_001fa6d0(scale_time(random_float_between(180.0f, 300.0f)));
            heading =
                fast_add_rotations(moby->yaw, random_float_between(-90.0f, 90.0f) * DEG_TO_RAD);
            build_spherical_offset(
                watch_data->target, 6.0f, heading, random_float_between(0.0f, 30.0f) * DEG_TO_RAD);
            fast_vec_add(watch_data->target, watch_data->target, moby->position);
        }
        if (watch_data->moving_timer) {
            qcopy(scratch.target, hero.motion.body_center.f);
            rate = 0.03f;
            head_rate = 0.3f;
        } else {
            qcopy(scratch.target, watch_data->target);
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
        watch_data->pitch = pitch;
        watch_data->yaw = yaw * 0.7f;
        watch_data->head_yaw = yaw * 0.3f;
    }
    if (big_head_cheat_enabled)
        watch_data->height = 2.75f;
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, moby, watch_data->body, 0);
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, moby, watch_data->head, 1);
}
