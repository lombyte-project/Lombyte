/* Level 08 moby class 0x478: batalia deserter. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/overlay/watch.h"
#include "rnc/data_labels.h"
#include "rnc/game/game_globals.h"
#include "rnc/math/fast_math.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/attach_manipulator.h"
#include "rnc/overlay/shared/cutscenes/update_npc_dialogue.h"
#include "rnc/overlay/shared/game/unlock_planet.h"

/* triggers objects matching the moby's class */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002EAF48.c: func_L08_003065F8), where it is exact; names translated to the US level program. */
struct G {
    char  pad0[0x30];
    int   a;
    int   b;
    char  pad1[0xC];
    short n;
    char  pad2[0x132];
    char *objs[1];
};

extern float                  D_L08_0015F59C;
extern float                  D_L08_0015F5A0;
extern int                    level_game_mode __asm__("D_L08_0015F5C4");
extern struct G               D_L08_0016D060;
extern volatile unsigned char D_0015EDB0[1];
extern unsigned char          D_L08_0017CC40[];
void attach_batalia_deserter_class_manipulator(struct Moby *moby) __asm__("FUN_L08_00305138");

#include "qcopy.h"

typedef struct {
    char           pad00[4];
    unsigned short sub;
    char           pad06[0x1A];
    void          *ptr20;
    char           pad24[0x1C];
    char           body[0x64];
    float          pitch;
    float          yaw;
    char           padAC[4];
    float          height;
    char           padB4[0xC];
    char           head[0x68];
    float          head_yaw;
    char           pad12C[0x14];
    float          target[4];
    char           pad150[4];
    int            idle_timer;
    int            moving_timer;
    int            target_timer;
} WD;

typedef struct {
    char          pad00[0x10];
    float         position[4];
    unsigned char state;
    char          pad21[0x10];
    unsigned char opacity;
    char          pad32[0x16];
    float         yaw;
    char          pad4C[7];
    unsigned char animation;
    char          pad54[0x1C];
    unsigned char flags70;
    char          pad71[7];
    WD           *data;
    char          pad7C[3];
    unsigned char b7F;
} WM;

extern void          begin_dialogue_camera_transition(void *, float) __asm__("FUN_L01_002783a8");
extern void          memcard_save_data(int, int) __asm__("FUN_0020b178");
extern float         scale_time(float) __asm__("FUN_001f96b0");
extern float         vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float         FUN_001fa688_cf(float, float) __asm__("FUN_001f9e90");
extern float         vector_length_xy(void *);
extern int           random_float_between_cf(float) __asm__("FUN_001fa6d0");
extern int           tick_countdown_32(int *) __asm__("func_001F9740");
extern unsigned char D_0015EDB0_b __asm__("D_0015EDB0");
extern void update_joint_animation(float, float, void *, void *, int) __asm__("FUN_L00_002628d8");
extern L16WatchPlayer D_0013E633_watch __asm__("D_0013E533");

typedef struct {
    char  pad00[0x24];
    short h24;
    char  pad26[6];
    float f2C;
    char  pad30[0x10];
    void *p40;
    char  pad44[0x12];
    short h56;
    char  pad58[0xC];
    int   i64;
    char  pad68[0x18];
    char  body[0x64];
    float pitch;
    float yaw;
    char  padEC[4];
    float height;
    char  padF4[0xC];
    char  head[0x68];
    float head_yaw;
    char  pad16C[0x14];
    float target[4];
    char  pad190[4];
    int   moving_timer;
    int   target_timer;
} WD8;

extern unsigned char D_0013DD49 NOT_SDA;
extern char                     batalia_deserter_name_text[] __asm__("D_L08_001622E8");
extern float                    D_L08_001675C0_c[] __asm__("D_L08_001675C0");
extern void                     FUN_L00_0025a120(void *);

extern struct {
    char pad[0x2FC];
    int  f2FC;
    int  f300;
} D_0013F350_g __asm__("D_0013F350");

/* Watcher moby update: wakes when the hero is near, then turns its head toward the hero or a random target. */
void         batalia_deserter_update(WM *m) __asm__("FUN_L08_00305270");
extern float vector_length_xy(void *) __asm__("FUN_001f9b20");

#include "rnc/game/item_state.h"

void attach_batalia_deserter_class_manipulator(struct Moby *moby) {
    int i;
    if (level_game_mode == 2 && D_0015EDB0[0]) {
        if (D_L08_0016D060.a == 3 && D_L08_0016D060.b >= 0x1BA && D_L08_0016D060.b < 0x284) {
            D_L08_0015F59C = 1.0f;
            D_L08_0015F5A0 = -0.3f;
        }
        for (i = 0; i < D_L08_0016D060.n; i++) {
            char *candidate_moby = D_L08_0016D060.objs[i];
            if (MOBY(candidate_moby)->oclass == moby->oclass && D_L08_0017CC40[1] == 0) {
                attach_manipulator(candidate_moby, 0, D_L08_0017CC40);
                *(float *)(D_L08_0017CC40 + 0x20) = 2.75f;
                *(float *)(D_L08_0017CC40 + 0x24) = 2.75f;
                *(float *)(D_L08_0017CC40 + 0x28) = 2.75f;
            }
        }
    }
}

/* Moby class 1144 */
void batalia_deserter_update(WM *moby) {
    enum {
        DESERTER_INITIALIZE = 0,
        DESERTER_WAIT_FOR_PLAYER = 1,
        DESERTER_UPDATE_DIALOGUE = 2
    };
    L16WatchScratch deserter_look_vectors;
    WD8            *deserter_data = (WD8 *)moby->data;
    float           rate;
    float           head_rate;
    int             tracking;
    attach_batalia_deserter_class_manipulator(moby);
    if (moby->opacity) {
        if (fast_vec_dist(moby->position, D_L08_001675C0_c) < 30.0f) {
            FUN_L00_0025a120(moby);
            moby->b7F = 0x18;
        }
    }
    switch (moby->state) {
    case DESERTER_INITIALIZE:
        if (moby->animation != 1)
            blend_moby_animation(moby, 1, 0, scale_game_frames(20));
        if (D_0013DD49 &&
            visited52[((unsigned char *)moby)[0xB0] + current_level_index * 16] == 0xFF) {
            mark_moby_for_removal(moby);
            return;
        }
        deserter_data->f2C = 4.0f;
        deserter_data->p40 = (void *)batalia_deserter_name_text;
        moby->state = 1;
        if (deserter_data->i64 >= 0)
            initialize_npc_dialogue(moby, (char *)deserter_data + 0x20);
        break;
    case DESERTER_WAIT_FOR_PLAYER:
        if (vector_distance(moby->position, (char *)&D_0013E633_watch + 0xE9D) < 8.0f) {
            jet_clear_persistent(((unsigned char *)moby)[0xB0]);
            moby->state = 2;
        }
        break;
    case DESERTER_UPDATE_DIALOGUE:
        if (moby->flags70 & 2) {
            if (moby->animation != random_integer_below(2) + 1)
                blend_moby_animation(moby, random_integer_below(2) + 1, 0, scale_game_frames(20));
        }
        if (D_0013F350_g.f2FC == 0 &&
            (scale_game_frames(30) < D_0013F350_g.f300 || deserter_data->h56 != 0)) {
            update_npc_dialogue(moby, (char *)deserter_data + 0x20);
            begin_dialogue_camera_transition(moby, 2.2f);
        }
        if (deserter_data->h24 == 4) {
            unlock_planet(9);
            memcard_save_data(0, -1);
            mark_moby_for_removal(moby);
            return;
        }
        break;
    }
    rate = 0.02f;
    head_rate = 0.3f;
    tracking = 0;
    if (moby->animation == 1) {
        char *player = (char *)&D_0013E633_watch + 0xE9D;
        tracking = 1;
        if (vector_distance(moby->position, player) < 8.0f &&
            fast_difference_between_rotations(
                moby->yaw,
                FUN_001fa688_cf(((L16WatchPlayer *)((char *)&D_0013E633_watch + 0xE1D))->aim[0] -
                                    moby->position[0],
                                ((L16WatchPlayer *)((char *)&D_0013E633_watch + 0xE1D))->aim[1] -
                                    moby->position[1])) < 1.5707964f) {
            if (fast_vec_length(player + 0x80) > 0.01f)
                deserter_data->moving_timer = scale_game_frames(120);
            else
                tick_countdown_32(&deserter_data->moving_timer);
        } else if (deserter_data->moving_timer) {
            deserter_data->moving_timer = 0;
            qcopy(deserter_data->target, (char *)&D_0013E633_watch + 0xEED);
        }
        if (tick_countdown_32(&deserter_data->target_timer)) {
            float heading;
            deserter_data->target_timer =
                random_float_between_cf(scale_time(random_float_between(180.0f, 300.0f)));
            heading =
                fast_add_rotations(moby->yaw, random_float_between(-90.0f, 90.0f) * DEG_TO_RAD);
            build_spherical_offset(deserter_data->target,
                                   6.0f,
                                   heading,
                                   random_float_between(0.0f, 30.0f) * DEG_TO_RAD);
            fast_vec_add(deserter_data->target, deserter_data->target, moby->position);
        }
        if (deserter_data->moving_timer) {
            qcopy(deserter_look_vectors.target, (char *)&D_0013E633_watch + 0xEED);
            rate = 0.04f;
            head_rate = 0.3f;
        } else {
            qcopy(deserter_look_vectors.target, deserter_data->target);
        }
    }
    if (tracking) {
        float yaw;
        float pitch;
        qcopy(deserter_look_vectors.eye, moby->position);
        deserter_look_vectors.eye[2] += 1.0f;
        fast_vec_sub(
            deserter_look_vectors.delta, deserter_look_vectors.target, deserter_look_vectors.eye);
        yaw = fast_subtract_rotations(
            FUN_001fa688_cf(deserter_look_vectors.delta[0], deserter_look_vectors.delta[1]),
            moby->yaw);
        pitch = -FUN_001fa688_cf(vector_length_xy(deserter_look_vectors.delta),
                                 deserter_look_vectors.delta[2]);
        if (yaw > 1.5707964f)
            yaw = 1.5707964f;
        else if (yaw < -1.5707964f)
            yaw = -1.5707964f;
        if (pitch > 0.5235988f)
            pitch = 0.5235988f;
        else if (pitch < -0.5235988f)
            pitch = -0.5235988f;
        deserter_data->pitch = pitch;
        deserter_data->head_yaw = deserter_data->yaw = yaw * 0.5f;
    }
    if (D_0015EDB0_b)
        deserter_data->height = 2.75f;
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, moby, deserter_data->body, 0);
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, moby, deserter_data->head, 1);
}
