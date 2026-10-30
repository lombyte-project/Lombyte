/* Level 08 moby classes: batalia_deserter, the_plumber_batalia. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/overlay/entities.h"
#include "rnc/overlay/watch.h"
#include "rnc/data_labels.h"
#include "rnc/game/game_globals.h"
#include "rnc/math/fast_math.h"

#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/mobys/002630b8.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/hero/hero_check_damage.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/attach_manipulator.h"
#include "rnc/overlay/shared/audio/streaming/initialize_npc_dialogue.h"
#include "rnc/overlay/shared/audio/streaming/update_npc_dialogue.h"
#include "rnc/overlay/shared/game/jet_reset_camera.h"
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

#include "qcopy.h"
#include "sda.h"

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
extern int           fast_dec_timer(void *) __asm__("FUN_001f9740");
extern float         scale_time(float) __asm__("FUN_001f96b0");
extern float         vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float         FUN_001fa688_cf(float, float) __asm__("FUN_001f9e90");
extern float         fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float         fast_subtract_rotations(float, float) __asm__("FUN_001fa5c8");
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
extern float                    fast_vec_dist(void *, void *) __asm__("FUN_001f9b48");

extern struct {
    char pad[0x2FC];
    int  f2FC;
    int  f300;
} D_0013F350_g __asm__("D_0013F350");

/* Watcher moby update: wakes when the hero is near, then turns its head toward the hero or a random target. */
void batalia_deserter_update(WM *m) __asm__("FUN_L08_00305270");

void batalia_deserter_update(WM *moby) {
    enum {
        DESERTER_INITIALIZE = 0,
        DESERTER_WAIT_FOR_PLAYER = 1,
        DESERTER_UPDATE_DIALOGUE = 2
    };
    L16WatchScratch scratch;
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
            qcopy(scratch.target, (char *)&D_0013E633_watch + 0xEED);
            rate = 0.04f;
            head_rate = 0.3f;
        } else {
            qcopy(scratch.target, deserter_data->target);
        }
    }
    if (tracking) {
        float yaw;
        float pitch;
        qcopy(scratch.eye, moby->position);
        scratch.eye[2] += 1.0f;
        fast_vec_sub(scratch.delta, scratch.target, scratch.eye);
        yaw =
            fast_subtract_rotations(FUN_001fa688_cf(scratch.delta[0], scratch.delta[1]), moby->yaw);
        pitch = -FUN_001fa688_cf(vector_length_xy(scratch.delta), scratch.delta[2]);
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

#include "sda.h"

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

extern short batalia_plumber_manipulator_replicated_value __asm__("D_L08_00162300")
    __attribute__((sda));
void attach_batalia_plumber_class_manipulator(struct Moby *moby) __asm__("FUN_L08_003063f0");

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

extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");

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
extern void           queue_scene_movie_with_game_language(int) __asm__("FUN_L00_00299460");
extern void           FUN_L08_002a1880(int);

/* Watcher moby update with a scripted reveal, then turns its head toward the hero or a random target. */
void the_plumber_batalia_update(L14WatchMoby *m) __asm__("FUN_L08_003065d8");

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
    if (D_0015EDB0_b)
        watch_data->height = 2.75f;
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, moby, watch_data->body, 0);
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, moby, watch_data->head, 1);
}

/* Set the object state, then apply the requested payload mode. */
void FUN_L08_00306b30(u8 *object, s32 mode) {
    u8 *payload;

    object[0x20] = 1;
    payload = *(u8 **)(object + 0x78);
    switch (mode) {
    case 1:
        payload[8] = 1;
        *(s16 *)(payload + 0x36) = 3;
        break;
    case 0:
        payload[8] = 1;
        *(s16 *)(payload + 0x36) = 4;
        break;
    case -1:
        hero_respawn();
        break;
    }
}

typedef struct {
    u16 count;
    u16 best;
    u32 flags;
} VendorEventRecord;

#include "rnc/game/item_state.h"

extern VendorEventRecord help_records[] __asm__("D_00141968");
extern u8 D_0013D413     NOT_SDA;
extern u8 D_0013D3B7     NOT_SDA;
extern void              help_request(s32, s32) __asm__("FUN_L00_00203908");
extern void              allocate_voice_for_bank_entry(s32, s32, s32) __asm__("FUN_0022db10");

typedef struct {
    s32 first;
    s32 second;
    s32 third;
    s32 fourth;
    s32 state;
} VendorEventData;

void update_batalia_item_triggers_and_help(struct Moby *moby) __asm__("FUN_L08_00307540");

void update_batalia_item_triggers_and_help(struct Moby *moby) {
    enum {
        VENDOR_EVENT_WAIT_FOR_ARMING = 0,
        VENDOR_EVENT_ARMED = 1
    };
    VendorEventData *data = *(VendorEventData **)((char *)moby + 0x78);
    s32              elapsed;
    s32              time;
    f32              volume[4];

    switch (data->state) {
    case VENDOR_EVENT_WAIT_FOR_ARMING:
        if (is_point_inside_clip_volume(&hero.motion.pos, data->third) &&
            hero.state.control_mode == 15)
            data->state = 1;
        break;
    case VENDOR_EVENT_ARMED:
        if (hero.state.current == 0x42) {
            data->state = 0;
        } else if (is_point_inside_clip_volume(&hero.motion.pos, data->fourth) &&
                   item_unlocked[11] == 0) {
            item_unlocked[11] = 1;
            allocate_voice_for_bank_entry(1, 0, 0);
            queue_dialogue_message(0x53d6, -1);
        }
        break;
    }
    if (item_available[29] == 0) {
        if (is_point_inside_clip_volume(&hero.motion.pos, data->first)) {
            elapsed = scale_game_frames(D_0015EEA4) - help_records[48].best * 600;
            if ((s32)((f32)scale_game_frames(18) * 60.0f) < elapsed ||
                help_records[48].best * 600 == 0) {
                help_request(8000, 0x30);
            } else {
                time = scale_game_frames(D_0015EEA4) / 600;
                if (help_records[48].best < time)
                    help_records[48].best = scale_game_frames(D_0015EEA4) / 600;
            }
        }
    }
    if (item_available[29] != 0) {
        if (is_point_inside_clip_volume(&hero.motion.pos, data->first) &&
            alternate_item_available[47] == 0) {
            if (help_records[49].count != 0) {
                elapsed = scale_game_frames(D_0015EEA4) - help_records[49].best * 600;
                if ((s32)((f32)scale_game_frames(18) * 60.0f) < elapsed ||
                    help_records[49].best * 600 == 0) {
                    help_request(8001, 0x31);
                } else {
                    time = scale_game_frames(D_0015EEA4) / 600;
                    if (help_records[49].best < time)
                        help_records[49].best = scale_game_frames(D_0015EEA4) / 600;
                }
            } else {
                help_records[49].count++;
                time = scale_game_frames(D_0015EEA4) / 600;
                if (help_records[49].best < time)
                    help_records[49].best = scale_game_frames(D_0015EEA4) / 600;
                help_records[49].flags =
                    help_records[49].flags | (1 << current_level_index) | 0x80000000;
            }
        }
    }
    if (item_available[28] == 0) {
        if (is_point_inside_clip_volume(&hero.motion.pos, data->second) && D_0013DD4A != 0) {
            elapsed = scale_game_frames(D_0015EEA4) - help_records[50].best * 600;
            if ((s32)((f32)scale_game_frames(18) * 60.0f) < elapsed ||
                help_records[50].best * 600 == 0) {
                help_request(8002, 0x32);
            } else {
                time = scale_game_frames(D_0015EEA4) / 600;
                if (help_records[50].best < time)
                    help_records[50].best = scale_game_frames(D_0015EEA4) / 600;
            }
            goto sample_volume;
        }
    }
    if (item_available[28] != 0) {
        if (is_point_inside_clip_volume(&hero.motion.pos, data->second) && D_0013DD4A != 0) {
            if (help_records[112].count != 0) {
                elapsed = scale_game_frames(D_0015EEA4) - help_records[112].best * 600;
                if ((s32)((f32)scale_game_frames(18) * 60.0f) < elapsed ||
                    help_records[112].best * 600 == 0) {
                    help_request(8005, 0x70);
                } else {
                    time = scale_game_frames(D_0015EEA4) / 600;
                    if (help_records[112].best < time)
                        help_records[112].best = scale_game_frames(D_0015EEA4) / 600;
                }
            } else {
                help_records[112].count++;
                time = scale_game_frames(D_0015EEA4) / 600;
                if (help_records[112].best < time)
                    help_records[112].best = scale_game_frames(D_0015EEA4) / 600;
                help_records[112].flags =
                    help_records[112].flags | (1 << current_level_index) | 0x80000000;
            }
        }
    }
sample_volume:
    *(u128 *)volume = 0;
    volume[2] = -1.0f;
    if (fast_vec_dot((char *)&hero + 0x290, volume) < 0.1f)
        help_records[112].count = -1;
    if (alternate_item_available[47] == 0 && hero.state.control_mode == 15)
        alternate_item_available[47] = 1;
}

/* per-frame update of a moby that fires a sound event when the game state allows */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002EAF48.c: func_L08_00309998), where it is exact; names translated to the US level program. */

struct G08c {
    char  pad0[0x30];
    int   a;
    int   b;
    char  pad1[0xC];
    short n;
    char  pad2[0x132];
    char *objs[1];
};

extern struct G08c D_L08_0016D060_c __asm__("D_L08_0016D060");
extern void        cutscene_set_camera_target(int) __asm__("FUN_L00_002637f8");

void update_batalia_selected_moby_effects(struct Moby *moby) __asm__("FUN_L08_003084d8");

void update_batalia_selected_moby_effects(struct Moby *moby) {
    enum {
        MOBY_INITIALIZE = 0,
        MOBY_ACTIVE = 1
    };
    int k;
    switch (moby->state) {
    case MOBY_INITIALIZE:
        moby->update_dist = 0xFF;
        moby->state = 1;
        break;
    case MOBY_ACTIVE:
        if (level_game_mode == 2) {
            int object_selector = D_L08_0016D060_c.a;
            if ((object_selector >= 1 && object_selector <= 2) || object_selector == 4 ||
                object_selector == 5) {
                if (D_L08_0016D060_c.a != 1 || scale_game_frames(0x8CE) < D_L08_0016D060_c.b) {
                    k = 0;
                    if (D_L08_0016D060_c.a == 1 || D_L08_0016D060_c.a == 4)
                        k = 3;
                    else if (D_L08_0016D060_c.a == 2 || D_L08_0016D060_c.a == 5)
                        k = 2;
                    cutscene_set_camera_target((int)D_L08_0016D060_c.objs[k]);
                }
            }
        }
        break;
    }
}

/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002EAF48.c: func_L08_0030A150), where it is exact; names translated to the US level program. */

extern void
spawn_effect_with_initial_motion(float *, float *, int, float) __asm__("FUN_L08_0027fe00");

void spawn_batalia_moby_effects_with_random_motion(struct Moby *moby) __asm__("FUN_L08_00308c90");

void spawn_batalia_moby_effects_with_random_motion(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    float effect_position[4];
    float rotation_basis[12];
    float effect_direction[4];
    int   i;
    for (i = 0; i < *(int *)(data + 0x14); i++) {
        effect_position[0] = -1.0f;
        effect_position[1] = random_float_between(-*(float *)(data + 4), *(float *)(data + 4));
        effect_position[2] = 0;
        effect_position[3] = 1.0f;
        euler_rotation_basis(rotation_basis, &moby->rot);
        transform_vector_by_basis(effect_position, effect_position, rotation_basis);
        fast_vec_add(effect_position, effect_position, &moby->pos);
        effect_direction[0] = 0;
        effect_direction[1] = random_float_between(*(float *)(data + 0x18) * frame_time,
                                                   *(float *)(data + 0x1C) * frame_time);
        effect_direction[2] = random_float_between(*(float *)(data + 0x20) * frame_time,
                                                   *(float *)(data + 0x24) * frame_time);
        effect_direction[3] = 0;
        spawn_effect_with_initial_motion(
            effect_position, effect_direction, *(int *)(data + 0x10), *(float *)(data + 0xC));
    }
}

/* Runs the two-entry setup loop for slot `x`: one call fills from the tables, the second applies them. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002EAF48.c: func_L08_0030A428), where it is exact; names translated to the US level program. */

extern Pair  D_L08_00162408[] MACRO_ADDR;
extern char  D_L08_001F8560[];
extern float D_L08_001EF7D0[];
extern float D_L08_001EF978[];
extern float D_L08_001EFB20[];
extern float D_L08_001EFC40[];
extern int   D_L08_001F00A8[];
extern int   D_L08_001F0138[];
int          D_L08_001623C8[2] = {0x23, 0x23};

int *D_L08_001623E8[2] = {D_L08_001F00A8, D_L08_001F0138};

extern void FUN_L00_001fde98(int, float *, int *, void *, int);
extern void FUN_L08_00258090(void *, float *, int, void *);

float *D_L08_001623D0[2] = {D_L08_001EF7D0, D_L08_001EF978};

float *D_L08_001623D8[2] = {D_L08_001EFB20, D_L08_001EFC40};

void queue_batalia_offset_textured_batches(int x) __asm__("FUN_L08_00308f68");

void queue_batalia_offset_textured_batches(int x) {
    int     i;
    int    *element_counts = D_L08_001623C8;
    float **float_pair_tables = D_L08_001623D8;
    float **float_triplet_tables = D_L08_001623D0;
    int   **element_word_tables = D_L08_001623E8;

    for (i = 0; i < 2; i++) {
        FUN_L08_00258090(
            D_L08_001F8560, float_pair_tables[i], element_counts[i], &D_L08_00162408[x]);
        FUN_L00_001fde98(
            element_counts[i], float_triplet_tables[i], element_word_tables[i], D_L08_001F8560, 1);
    }
}
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002EAF48.c: func_L08_0030AC70), where it is exact; names translated to the US level program. */

extern int  D_L08_001623F0_macro[] __asm__("D_L08_001623F0") MACRO_ADDR;
extern Pair D_L08_001623F8[];
extern Pair D_L08_001F8548[];
extern char D_L08_001675C0[];
extern s32  D_L08_001F01C8[];
extern s32  D_L08_001F4C48[];
extern s32  D_L08_001F4C60[];
extern s32  D_L08_001F8528[];
extern char batalia_effect_sine_offsets[] __asm__("D_L08_001F8550");

#include "rnc/overlay/l08/mobys/batalia_deserter_globals.h"

extern int   D_L08_001623F0;
extern short batalia_effect_reference_position_x __asm__("D_L08_00162380") __attribute__((sda));
int          D_L08_001623C4 = 0;
extern int  *D_L08_001623E8[2] __attribute__((sda));
extern int   D_L08_001623C8[2] __attribute__((sda));
extern void  draw_layered_mesh_effect(void) __asm__("FUN_L08_002f0dc8");
extern void  queue_batalia_textured_batch_passes(void) __asm__("FUN_L08_00309030");
extern void
scale_rgba_tables(void *, int, void *, float, float, float, float, int) __asm__("FUN_L08_002f0f68");
extern void
advance_wrapped_sine_pair(void *, float, float, float, void *) __asm__("FUN_L08_002f1158");
extern void advance_scroll_offsets(int) __asm__("FUN_L08_002f12a0");
extern void advance_scroll_entry(int) __asm__("FUN_L08_002f1378");
extern int  func_001FA898_r(float) __asm__("FUN_001fa6d0");

void update_batalia_textured_effect(struct Moby *moby) __asm__("FUN_L08_003097b0");

void update_batalia_textured_effect(struct Moby *moby) {
    enum {
        MOBY_INITIALIZE = 0,
        MOBY_ACTIVE = 1
    };
    int   i;
    float sphere_check_input[4];
    float position_delta[4];
    float distance;

    switch (moby->state) {
    case MOBY_INITIALIZE:
        if (D_L08_001623C4 == 0) {
            scale_rgba_tables(D_L08_001F8528, 8, D_L08_001F4C60, 0.5f, 0.5f, 0.65f, 0.5f, 1);
            scale_rgba_tables(D_L08_001623E8, 2, D_L08_001623C8, 0.5f, 0.5f, 0.5f, 1.0f, 1);
            scale_rgba_tables(D_L08_001F4C48, 6, D_L08_001F01C8, 0.25f, 0.25f, 0.25f, 1.0f, 1);
            D_L08_001623C4 = 1;
        }
        moby->state = 1;
        for (i = 0; i < 3; i++) {
            D_L08_001F8548[i].a = 0;
            D_L08_001F8548[i].b = 0;
            D_L08_001623F0_macro[8] = 0;
            D_L08_001623F0_macro[9] = 0;
        }
        for (i = 0; i < 2; i++) {
            D_L08_001623F0_macro[2 + i * 2] = 0;
            *(float *)&D_L08_001623F0_macro[3 + i * 2] = i * 0.4f;
        }
        for (i = 0; i < 1; i++) {
            D_L08_001623F0_macro[6 + i * 2] = 0;
            *(float *)&D_L08_001623F0_macro[7 + i * 2] = i * 0.4f;
        }
        break;
    case MOBY_ACTIVE:
        sphere_check_input[0] = *(float *)&batalia_effect_reference_position_x;
        sphere_check_input[1] = batalia_effect_reference_position_y;
        sphere_check_input[2] = batalia_effect_reference_position_z;
        sphere_check_input[3] = batalia_effect_culling_radius;
        if (fast_bsphere_check((char *)sphere_check_input, 1000.0f) >= 0) {
            position_delta[0] = *(float *)&batalia_effect_reference_position_x;
            position_delta[1] = batalia_effect_reference_position_y;
            position_delta[2] = batalia_effect_reference_position_z;
            position_delta[3] = 1.0f;
            fast_vec_sub(position_delta, D_L08_001675C0, position_delta);
            distance = fast_vec_length(position_delta);
            if (distance <= 84.0f) {
                advance_scroll_offsets(0);
                advance_wrapped_sine_pair(
                    batalia_effect_sine_offsets, 0.52f, 0.62f, 0.06f, &D_L08_001623F0_macro[8]);
                advance_scroll_offsets(2);
                if (distance < 20.0f) {
                    *(unsigned char *)&D_L08_001623F0 = 0xFF;
                } else if (distance < 84.0f) {
                    *(char *)&D_L08_001623F0 =
                        func_001FA898_r((1.0f - (distance - 20.0f) * 0.015625f) * 255.0f);
                } else {
                    *(char *)&D_L08_001623F0 = 0;
                }
            }
            advance_scroll_entry(0);
            advance_scroll_entry(0);
            advance_scroll_entry(1);
            add_draw_callback(queue_batalia_textured_batch_passes, moby);
        }
        break;
    }
}
