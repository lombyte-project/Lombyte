/* Level 16 moby classes: help_desk_lady, bolt_thief, taxi, grindrail_gate, grindrail_button, chomper, gadgetron_ceo, elevator, gadgetron_sign. */
#include "types.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"

#include "qcopy.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/entities.h"
#include "rnc/overlay/watch.h"
#include "rnc/game/item_state.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voices/allocate_voice_for_bank_entry.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/002630b8.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/interpolation/compute_directed_angle_delta.h"
#include "rnc/overlay/shared/vendor/vendor_spring.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/math/rotations/clip_negative_segment_endpoint_xyz.h"
#include "rnc/overlay/shared/math/vectors/compute_transformed_delta_component.h"
#include "rnc/overlay/shared/ui/help/help_request.h"
#include "rnc/save/memcard_save_data.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/animation/set_moby_animation.h"
#include "rnc/overlay/shared/audio/streaming/initialize_npc_dialogue.h"
#include "rnc/overlay/shared/audio/streaming/update_npc_dialogue.h"
#include "rnc/overlay/shared/game/jet_reset_camera.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/rendering/update_entity.h"
#include "rnc/ui/text/font/font_queue_vu_state.h"
#include "rnc/overlay/shared/audio/streaming/set_moby_dialogue_record_value.h"
#include "rnc/overlay/shared/mobys/draw_trail.h"
#include "rnc/overlay/shared/props/trackdrive_left.h"

/* Activate the watcher and steer its body and head toward the player. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E45F8), where it is exact; names translated to the US level program. */

typedef struct {
    char  pad00[0xC];
    float radius;
    char  pad10[0x30];
} L16WatchActivation;

typedef struct {
    char               pad00[0x20];
    L16WatchActivation activation;
    char               body[0x60];
    float              padC0;
    float              pitch;
    float              yaw;
    float              padCC;
    float              height;
    char               padD4[0xC];
    char               head[0x60];
    char               pad140[8];
    float              head_yaw;
    char               pad14C[0x14];
    int                activation_region;
    int                entry_region;
    int                moving_timer;
    int                target_timer;
    float              target[4];
} L16WatchData;

typedef struct {
    char          pad00[0x10];
    float         position[4];
    unsigned char state;
    char          pad21[0xF];
    unsigned char opacity;
    char          pad31[0x17];
    float         yaw;
    char          pad4C[7];
    unsigned char animation;
    char          pad54[0x24];
    L16WatchData *data;
    char          pad7C[0x34];
    unsigned char widget;
} L16WatchMoby;

typedef struct {
    char          pad00[0x70];
    unsigned char entered;
    unsigned char mission;
} L16WatchFlags;

extern char D_0013E633[], D_0014171B[], D_0013D24D[], D_L16_00167240[];
float       D_L16_001D96C0[8] = {300.73f, 206.04f, 123.22f, 0.0f, 0.0f, 0.11f, 0.0f, 0.0f};

extern float         scale_time(float) __asm__("FUN_001f96b0");
extern float         fast_add_rotations(float, float);
extern float         fast_subtract_rotations(float, float);
extern float         vector_length_xy(void *);
extern int           is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");
extern int           scale_game_frames(int);
extern s32           D_L16_0015F5C4;
extern unsigned char D_0015EDB0_b __asm__("D_0015EDB0");
extern void          set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern void update_joint_animation(float, float, void *, void *, int) __asm__("FUN_L00_002628d8");
extern void apply_npc_big_head_cheat(char *) __asm__("FUN_L12_002e6b60");
extern void build_spherical_offset(void *, float, float, float) __asm__("FUN_00214db0");
extern L16WatchPlayer D_0013E633_watch __asm__("D_0013E533");
extern L16WatchFlags  D_0013D355_watch __asm__("D_0013D24D");

void help_desk_lady_update(L16WatchMoby *m) __asm__("FUN_L16_002e3190");

void help_desk_lady_update(L16WatchMoby *moby) {
    enum {
        HELP_DESK_INITIALIZE_DIALOGUE = 0,
        HELP_DESK_POLL_DIALOGUE = 1,
        HELP_DESK_REQUEST_HELP_AND_SAVE = 2
    };
    L16WatchScratch scratch;
    L16WatchData   *watch_data = moby->data;
    L16WatchFlags  *flags;
    float           rate;
    float           head_rate;
    int             tracking;
    void           *activation;
    apply_npc_big_head_cheat((char *)moby);
    activation = &watch_data->activation;
    switch (moby->state) {
    case HELP_DESK_INITIALIZE_DIALOGUE:
        initialize_npc_dialogue(moby, activation);
        moby->state = 1;
        watch_data->activation.radius = 20.0f;
        moby->opacity = 0xFF;
        break;
    case HELP_DESK_POLL_DIALOGUE: {
        L16WatchPlayer *player = (L16WatchPlayer *)((char *)&D_0013E633_watch + 0xE1D);
        if (player->disabled == 0 && player->mode != 15 && player->mode != 22) {
            if (is_point_inside_clip_volume((char *)player + 0x80, watch_data->activation_region) &&
                update_npc_dialogue(moby, activation))
                moby->state = 2;
        }
        break;
    }
    case HELP_DESK_REQUEST_HELP_AND_SAVE:
        if (*(int *)&D_L16_0015F5C4 != 2) {
            char *player;
            moby->state = 1;
            set_race_camera(D_L16_001D96C0, D_L16_001D96C0 + 4, 0, 1);
            help_request(0x3E82, 0x84);
            grant_inventory_item(0x21, 1);
            jet_clear_persistent(moby->widget);
            player = (char *)&D_0013E633_watch + 0xE9D;
            jet_reset_camera(player, player + 0x10);
            memcard_save_data(0, -1);
            moby->state = 3;
        }
        break;
    }
    flags = (L16WatchFlags *)((char *)&D_0013D355_watch + 0x13B);
    if (!flags->entered &&
        is_point_inside_clip_volume((char *)&D_0013E633_watch + 0xE9D, watch_data->entry_region))
        flags->entered = 1;
    if (!((L16WatchFlags *)((char *)&D_0013D355_watch + 0x13B))->mission &&
        hero.state.control_mode == 15)
        ((L16WatchFlags *)((char *)&D_0013D355_watch + 0x13B))->mission = 1;
    rate = 0.02f;
    head_rate = 0.3f;
    tracking = 0;
    if (moby->animation == 0) {
        char *player = (char *)&D_0013E633_watch + 0xE9D;
        tracking = 1;
        if (vector_distance_xy(moby->position, player) < 8.0f &&
            fast_difference_between_rotations(
                moby->yaw,
                fast_arc_tan(((L16WatchPlayer *)((char *)&D_0013E633_watch + 0xE1D))->aim[0] -
                                 moby->position[0],
                             ((L16WatchPlayer *)((char *)&D_0013E633_watch + 0xE1D))->aim[1] -
                                 moby->position[1])) < 1.5707964f) {
            if (fast_vec_length(player + 0x80) > 0.01f)
                watch_data->moving_timer = scale_game_frames(120);
            else
                fast_dec_timer(&watch_data->moving_timer);
        } else if (watch_data->moving_timer) {
            watch_data->moving_timer = 0;
            qcopy(watch_data->target, (char *)&D_0013E633_watch + 0xEED);
        }
        if (fast_dec_timer(&watch_data->target_timer)) {
            float heading;
            watch_data->target_timer =
                truncate_float_to_s32(scale_time(random_float_between(180.0f, 300.0f)));
            heading =
                fast_add_rotations(moby->yaw, random_float_between(-90.0f, 90.0f) * DEG_TO_RAD);
            build_spherical_offset(
                watch_data->target, 6.0f, heading, random_float_between(0.0f, 30.0f) * DEG_TO_RAD);
            fast_vec_add(watch_data->target, watch_data->target, moby->position);
        }
        if (watch_data->moving_timer) {
            qcopy(scratch.target, (char *)&D_0013E633_watch + 0xEED);
            rate = 0.04f;
            head_rate = 0.3f;
        } else {
            qcopy(scratch.target, watch_data->target);
        }
    }
    if (tracking) {
        float yaw;
        float pitch;
        qcopy(scratch.eye, moby->position);
        scratch.eye[2] += 1.5f;
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
        rate * frame_scale_sq, head_rate * frame_scale_sq, moby, watch_data->head, 2);
}

/* UpdateMoby: on the first tick scales two data floats by 0.25 into the moby and sets its state. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E4B50), where it is exact; names translated to the US level program. */

void initialize_moby_rotation_scales(struct Moby *moby) __asm__("FUN_L16_002e36e8");

void initialize_moby_rotation_scales(struct Moby *moby) {
    float *rotation_scale_data = (float *)moby->pvars;
    if (moby->state == 0) {
        fast_vec_normalize(&moby->rot_row1, &moby->rot_row1, rotation_scale_data[0] * 0.25f);
        fast_vec_normalize(&moby->rot_row2, &moby->rot_row2, rotation_scale_data[1] * 0.25f);
        moby->state = 1;
        moby->flags |= 0x101;
        update_entity(moby);
    }
}

/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E4BD8), where it is exact; names translated to the US level program. */

void set_class_056b_collision_enabled(void *moby, int on) __asm__("FUN_L16_002e3770");

void set_class_056b_collision_enabled(void *moby, int on) {
    if (*(short *)((char *)moby + 0xA6) == 0x56B) {
        if (on != 0) {
            *(int *)((char *)moby + 0x94) = *(int *)(*(char **)((char *)moby + 0x24) + 0x10);
        } else {
            *(int *)((char *)moby + 0x94) = 0;
        }
    }
}

#include "qcopy.h"

/* Update of the crate platform: hit reaction, then its seven states. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E4C08), where it is exact; names translated to the US level program. */

typedef struct {
    char          pad00[0x60];
    char          effect[7]; /* 0x60: record func_L00_0025E590 updates */
    unsigned char ticks;     /* 0x67 */
    char          pad68[8];
    float         knock[4];      /* 0x70: record func_L00_0025D5B0 fills (velocity first) */
    float         knock_drag;    /* 0x80 */
    float         knock_gravity; /* 0x84 */
    float         knock_speed;   /* 0x88 */
    float         knock_rise;    /* 0x8C */
    int           unknown90;
    int           knock_flags; /* 0x94 */
    char          pad98[0x15];
    unsigned char knock_bAD; /* 0xAD */
    char          padAE[0x22];
    int           counter;    /* 0xD0 */
    float         timer;      /* 0xD4 */
    int           path;       /* 0xD8: index into D_L16_001B0C30 */
    int           other_path; /* 0xDC */
    int           trigger;    /* 0xE0 */
    float         vx;         /* 0xE4 */
    float         vy;         /* 0xE8 */
    float         vz;         /* 0xEC */
    float         turn;       /* 0xF0 */
    char         *child;      /* 0xF4 */
    int           unknownF8;
    int           mode; /* 0xFC */
} L16CrateData;

typedef struct {
    char pad00[0x10];
    int  bounds;
} L16CrateModel;

typedef struct {
    char           pad00[0x10];
    float          position[4]; /* 0x10 */
    unsigned char  state;       /* 0x20 */
    char           pad21[3];
    L16CrateModel *model; /* 0x24 */
    char           pad28[8];
    unsigned char  collision; /* 0x30 */
    char           pad31[3];
    unsigned short flags; /* 0x34 */
    char           pad36[10];
    float          rotation[4]; /* 0x40 */
    char           pad50[2];
    unsigned char  frame;     /* 0x52 */
    unsigned char  animation; /* 0x53 */
    char           pad54[0x1C];
    unsigned char  status; /* 0x70 */
    char           pad71[7];
    L16CrateData  *data; /* 0x78 */
    char           pad7C[0x18];
    int            bounds; /* 0x94 */
    char           pad98[0xC];
    unsigned char  opacity; /* 0xA4 */
    char           padA5[0xD];
    short          id; /* 0xB2 */
} L16CrateMoby;

typedef struct {
    char  pad00[0x80];
    float position[4]; /* 0x80 */
    char  pad90[0x1AC];
    void *standing; /* 0x23C: the moby the hero stands on */
} L16CratePlayer;

extern char *D_L16_001B0930[];
extern char *spawn_floating_mine(void *, void *) __asm__("FUN_L14_002ffaf8");
extern char  D_L16_001E86A0[], D_L16_001E89D8[], D_L16_001E8A20[], D_L16_001E8A58[],
    D_L16_001E8AA0[];
extern char D_L16_001E89A0[], D_L16_001E86D8[], D_L16_001E8A20[], D_L16_001E8A58[],
    D_L16_001E8AA0[] __asm__("D_L16_001E8AA0");
extern char D_L16_001E89A0[], D_L16_001E89D8[], D_L16_001E8720[], D_L16_001E8A58[],
    D_L16_001E8AA0[] __asm__("D_L16_001E8AA0");
extern char D_L16_001E89A0[], D_L16_001E89D8[], D_L16_001E8A20[], D_L16_001E8758[],
    D_L16_001E8AA0[] __asm__("D_L16_001E8AA0");
extern char D_L16_001E89A0[], D_L16_001E89D8[], D_L16_001E8A20[], D_L16_001E8A58[],
    D_L16_001E87A0[];
extern float frame_time, D_0015EE70;
extern int   debug_print() __asm__("FUN_001e93b0");
int          advance_bolt_thief_path(struct Moby *moby) __asm__("FUN_L16_002e3fa0");

extern int  advance_bolt_thief_path(struct Moby *moby) __asm__("FUN_L16_002e3fa0");
extern f32  D_L16_00161D94;
extern f32  D_L16_00161D98_gp __asm__("D_L16_00161D98") __attribute__((sda));
extern void FUN_L00_00257470(void *, int, int);
extern void
trooper_start_knockback(void *, void *, int, int, int, float) __asm__("FUN_L00_0025c558");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern void set_moby_state_14(void *) __asm__("FUN_L03_002c6c90");
extern void initialize_moby_path_state(void *) __asm__("FUN_L14_002fe900");
extern void update_linked_moby_position(void *) __asm__("FUN_L14_002fec28");
extern void emit_moby_offset_particles(void *) __asm__("FUN_L14_002feca8");
void        FUN_L00_00258278(unsigned char *m, float *ptr, float t, float b, float c, float limit);
extern void
func_L00_002592B0_path54(char *, float, float *, float, float, float) __asm__("FUN_L00_00258278");

void bolt_thief_update(L16CrateMoby *moby) __asm__("FUN_L16_002e37a0");

void bolt_thief_update(L16CrateMoby *moby) {
    enum {
        BOLT_THIEF_INITIALIZE = 0,
        BOLT_THIEF_FACE_HERO = 1,
        BOLT_THIEF_WAIT_FOR_HERO_TRIGGER = 2,
        BOLT_THIEF_SETTLE_AT_PATH_END = 3,
        BOLT_THIEF_FOLLOW_PRIMARY_PATH = 4,
        BOLT_THIEF_FOLLOW_OTHER_PATH = 5,
        BOLT_THIEF_KNOCKBACK = 6
    };
    L16CrateData *bolt_thief_data = moby->data;
    char         *hit;

    if ((moby->status & 2) && moby->frame == moby->animation && moby->frame == 1)
        blend_moby_animation(moby, 4, 0, scale_game_frames(5));
    if (hero.coll_hit_moby == moby && moby->animation != 1 && moby->frame != 1)
        blend_moby_animation(moby, 1, 0, scale_game_frames(5));
    hit = find_moby_hit(moby, 0x210000, 0);
    if (!hit && bolt_thief_data->child)
        hit = find_moby_hit(bolt_thief_data->child, 0x210000, 0);
    if (hit && moby->state != 6) {
        struct Hero *player = &hero;

        bolt_thief_data->knock_drag = 0.008f;
        bolt_thief_data->knock_gravity = 0.0005f;
        bolt_thief_data->knock_speed = frame_time * 24.0f;
        bolt_thief_data->knock_rise = frame_time * 12.0f;
        bolt_thief_data->knock_flags = 1;
        bolt_thief_data->knock_bAD = 0;
        trooper_start_knockback(moby,
                                bolt_thief_data->knock,
                                1,
                                1,
                                0,
                                fast_arc_tan(moby->position[0] - player->motion.pos.f[0],
                                             moby->position[1] - player->motion.pos.f[1]));
        bolt_thief_data->ticks = 0x78;
        FUN_L00_00257470(moby, 0, -1);
        moby->state = 6;
    }
    moby->opacity = 0xFF;
    switch (moby->state) {
    case BOLT_THIEF_INITIALIZE:
        if (bolt_thief_data->mode == 2) {
            moby->state = 1;
            bolt_thief_data->turn = 0.0f;
            set_moby_animation(moby, 4, 0);
        } else {
            if (bolt_thief_data->path == -1) {
                debug_print(D_L16_001E86A0, moby->id);
                mark_moby_for_removal(moby);
                return;
            }
            if (*(int *)D_L16_001B0930[bolt_thief_data->path] == 0) {
                debug_print(D_L16_001E86D8, moby->id);
                mark_moby_for_removal(moby);
                return;
            }
            if (bolt_thief_data->mode != 1) {
                if (bolt_thief_data->other_path == -1) {
                    debug_print(D_L16_001E8720, moby->id);
                    mark_moby_for_removal(moby);
                    return;
                }
                if (*(int *)D_L16_001B0930[bolt_thief_data->other_path] == 0) {
                    debug_print(D_L16_001E8758, moby->id);
                    mark_moby_for_removal(moby);
                    return;
                }
            }
            if (bolt_thief_data->trigger == -1) {
                debug_print(D_L16_001E87A0, moby->id);
                mark_moby_for_removal(moby);
                return;
            }
            initialize_moby_path_state(moby);
            moby->state = 2;
            moby->collision = 0x80;
            moby->flags |= 0x41;
            moby->bounds = 0;
        }
        bolt_thief_data->child = 0;
        break;
    case BOLT_THIEF_FACE_HERO: {
        struct Hero *player = &hero;

        func_L00_002592B0_path54((char *)moby,
                                 fast_arc_tan(player->motion.pos.f[0] - moby->position[0],
                                              player->motion.pos.f[1] - moby->position[1]),
                                 &bolt_thief_data->turn,
                                 0.005f,
                                 0.2f,
                                 0.0f);
        emit_moby_offset_particles(moby);
        break;
    }
    case BOLT_THIEF_WAIT_FOR_HERO_TRIGGER:
        if (is_point_inside_clip_volume(&hero.motion.pos, bolt_thief_data->trigger)) {
            if (bolt_thief_data->mode == 1) {
                set_moby_animation(moby, 0, 0);
            } else {
                bolt_thief_data->child = spawn_floating_mine(moby, moby->position);
                update_linked_moby_position(moby);
                set_moby_animation(moby, 2, 0);
            }
            moby->flags &= 0xFFBE;
            moby->bounds = moby->model->bounds;
            moby->state = 4;
        }
        break;
    case BOLT_THIEF_FOLLOW_PRIMARY_PATH:
        if (bolt_thief_data->mode == 0)
            update_linked_moby_position(moby);
        if (advance_bolt_thief_path((char *)moby)) {
            moby->state = 3;
            blend_moby_animation(moby, 3, 0, 5);
        }
        emit_moby_offset_particles(moby);
        break;
    case BOLT_THIEF_SETTLE_AT_PATH_END: {
        L16CrateData *settle_data = moby->data;
        char         *path = D_L16_001B0930[settle_data->path];
        float         target[4];
        float         zero;

        if (settle_data->mode == 0)
            update_linked_moby_position(moby);
        qcopy(target, path + *(int *)path * 16);
        zero = 0.0f;
        vendor_spring(moby->position,
                      &settle_data->vx,
                      target[0],
                      *(float *)&D_L16_00161D94,
                      *(float *)&D_L16_00161D98_gp,
                      zero);
        vendor_spring(moby->position + 1,
                      &settle_data->vy,
                      target[1],
                      *(float *)&D_L16_00161D94,
                      *(float *)&D_L16_00161D98_gp,
                      zero);
        vendor_spring(moby->position + 2,
                      &settle_data->vz,
                      target[2],
                      *(float *)&D_L16_00161D94,
                      *(float *)&D_L16_00161D98_gp,
                      zero);
        if ((moby->status & 2) && moby->frame == moby->animation) {
            if (settle_data->mode == 1) {
                moby->state = 1;
                settle_data->turn = zero;
                set_moby_animation(moby, 4, 0);
            } else {
                if (settle_data->child) {
                    set_moby_state_14(settle_data->child);
                    settle_data->child = 0;
                }
                moby->state = 5;
                blend_moby_animation(moby, 0, 0, 20);
                settle_data->timer = zero;
                settle_data->counter = 0;
            }
        }
        emit_moby_offset_particles(moby);
        break;
    }
    case BOLT_THIEF_FOLLOW_OTHER_PATH:
        if (advance_bolt_thief_path((char *)moby)) {
            char *path;

            moby->state = 2;
            moby->bounds = 0;
            moby->flags |= 0x41;
            bolt_thief_data->timer = 0.0f;
            bolt_thief_data->counter = 0;
            bolt_thief_data->turn = bolt_thief_data->vx = bolt_thief_data->vy =
                bolt_thief_data->vz = bolt_thief_data->timer;
            path = D_L16_001B0930[bolt_thief_data->path];
            qcopy(moby->position, path + 0x10);
            moby->rotation[2] = fast_arc_tan(*(float *)(path + 0x20) - *(float *)(path + 0x10),
                                             *(float *)(path + 0x24) - *(float *)(path + 0x14));
        } else {
            emit_moby_offset_particles(moby);
        }
        break;
    case BOLT_THIEF_KNOCKBACK:
        if (mine_fall(moby, bolt_thief_data->knock) & 3) {
            cart_burst(moby, moby->position, -1, 1.0f, 13.0f);
            mark_moby_for_removal(moby);
            return;
        }
        if (moby->position[2] < 5.0f) {
            mark_moby_for_removal(moby);
            return;
        }
        break;
    }
    trooper_anim_finish(moby, bolt_thief_data->effect);
}

/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E5408), where it is exact; names translated to the US level program. */

extern float frame_time, D_0015EE70;
extern int   sample_path() __asm__("FUN_L00_0025d808");

#include "rnc/overlay/l16/mobys/grindrail_gate_globals.h"

extern float bolt_thief_path_speed_gp __asm__("D_L16_00161D90") __attribute__((sda));
extern float D_L16_00161D94_gp __asm__("D_L16_00161D94") __attribute__((sda));
extern float D_L16_00161D98_gp __asm__("D_L16_00161D98") __attribute__((sda));
extern int
FUN_L00_0025d808_path54(char *, float *, float *, float *, int, float) __asm__("FUN_L00_0025d808");
extern void
func_L00_002592B0_path54(char *, float, float *, float, float, float) __asm__("FUN_L00_00258278");

int advance_bolt_thief_path(struct Moby *moby) {
    float pos[4];
    float delta[4];
    char *data = (char *)moby->pvars;
    float scaled_frame_time = bolt_thief_path_speed_gp * frame_time;
    int   path_sample_result;
    char *path;
    if (moby->state == 5)
        path = D_L16_001B0930[*(int *)(data + 0xDC)];
    else
        path = D_L16_001B0930[*(int *)(data + 0xD8)];
    path_sample_result = FUN_L00_0025d808_path54(
        path, pos, (float *)(data + 0xD0), (float *)(data + 0xD4), 0, scaled_frame_time);
    vendor_spring(
        &moby->pos.x, (float *)(data + 0xE4), pos[0], D_L16_00161D94_gp, D_L16_00161D98_gp, 0.0f);
    vendor_spring(
        &moby->pos.y, (float *)(data + 0xE8), pos[1], D_L16_00161D94_gp, D_L16_00161D98_gp, 0.0f);
    vendor_spring(
        &moby->pos.z, (float *)(data + 0xEC), pos[2], D_L16_00161D94_gp, D_L16_00161D98_gp, 0.0f);
    fast_vec_sub(delta, pos, &moby->pos);
    if (absolute_float(delta[0]) > 0.01f) {
        if (absolute_float(delta[1]) > 0.01f) {
            func_L00_002592B0_path54(moby,
                                     fast_arc_tan(delta[0], delta[1]),
                                     (float *)(data + 0xF0),
                                     D_L16_00161D94_gp,
                                     D_L16_00161D98_gp,
                                     0.0f);
        }
    }
    return path_sample_result;
}

#include "sda.h"

#include "qcopy.h"

/* Update the moving pickup, interpolate its path transitions, and manage its sound handle. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E5848), where it is exact; names translated to the US level program. */

typedef struct {
    char  pad00[0x30];
    float position[4];
    char  pad40[0x30];
    float direction[4];
} L16PickupPose;

extern L16PickupPose *D_L16_001600EC;
extern char          *find_moby_in_owner_list_by_id(void *, int) __asm__("FUN_L05_00319598");
extern char           D_0013CAE4[];
extern float
advance_accelerated_scalar_c(float, float, float, float, float *, float *) __asm__("FUN_00213f38");
extern float                    frame_time, D_0015EE70_c __asm__("D_0015EE70");
extern float                    D_0015EE6C_u, D_0015ED70_c2 __asm__("D_0015ED70");
extern int                      advance_moby_path(char *) __asm__("FUN_L05_003195f8");
extern s32                      D_L16_0015F594;
extern unsigned char D_0013D4E1 NOT_SDA;
extern void allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
extern void reset_selected_moby_to_current_placement(void *) __asm__("FUN_L05_00319510");
extern void request_moby_variant_help_message(char *) __asm__("FUN_L05_00319690");
extern void apply_moby_position_and_rotation_wobble(char *) __asm__("FUN_L05_00319740");
extern void transform_moby_vector_table_to_world_space(char *) __asm__("FUN_L05_003198e8");
extern void initialize_taxi_stop_state(void *) __asm__("FUN_L16_002e4900");
extern void FUN_L16_002e4a58_c(char *) __asm__("FUN_L16_002e4a58");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
extern void sample_camera_path_u(void *, s32, void *, void *, s32, f32) __asm__("func_00214E58");
extern int  func_0022ED80_i(int, int, void *) __asm__("FUN_0022da68");
extern void func_00215CA8_f5848(float, int *, int, void *, float *, int) __asm__("FUN_00214e58");

void taxi_update(struct Moby *moby) __asm__("FUN_L16_002e43e0");

void taxi_update(struct Moby *moby) {
    enum {
        TAXI_INITIALIZE = 0,
        TAXI_WAIT_FOR_DEPARTURE = 1,
        TAXI_BLEND_TO_PATH_START = 3,
        TAXI_FOLLOW_PATH = 4,
        TAXI_BLEND_TO_DESTINATION_POSE = 5,
        TAXI_WAIT_FOR_ENABLE = 6
    };
    char *taxi_data = (char *)moby->pvars;
    float previous_position[4], previous_rotation[4], position_delta[4],
        transposed_rotation_or_start_position[4], transition_end_position[4],
        transition_start_rotation[4], transition_end_rotation[4], hero_local_offset[4];
    struct Hero   *player;
    struct Hero   *travel_player;
    float          z;
    char          *matched_moby;
    char          *voice_slot_base;
    L16PickupPose *tab;
    float         *qa;
    int            voice_slot_index;
    int            s;
    qcopy(previous_position, &moby->pos);
    qcopy(previous_rotation, &moby->rot);
    set_moby_dialogue_record_value(moby, (moby->flags ^ 1) & 1);
    switch (moby->state) {
    case TAXI_INITIALIZE:
        *(short *)(taxi_data + 0xAA) = -1;
        initialize_taxi_stop_state(moby);
        break;
    case TAXI_WAIT_FOR_DEPARTURE:
        player = &hero;
        if (((char *)player->ground_moby) == moby && player->air_frames.s == 0) {
            transpose_rotation_matrix(transposed_rotation_or_start_position, &moby->rot_row0);
            fast_vec_sub(hero_local_offset, &player->motion.pos, &moby->pos);
            hero_local_offset[3] = 0;
            transform_vector(
                hero_local_offset, hero_local_offset, transposed_rotation_or_start_position);
            if (absolute_float(hero_local_offset[1]) < 1.2f) {
                FUN_L16_002e4a58_c(moby);
                request_moby_variant_help_message(moby);
                if ((*(int *)(D_0013CAE4) & 0x10) && *(int *)&D_L16_0015F594 == 8) {
                    moby->state = 3;
                    matched_moby =
                        find_moby_in_owner_list_by_id(moby, *(short *)(taxi_data + 0xAE));
                    if (matched_moby != 0) {
                        matched_moby[0x20] = 2;
                        MOBY(matched_moby)->flags |= 1;
                        matched_moby[0x31] = 0;
                        MOBY(matched_moby)->collision = 0;
                    }
                }
            }
        }
        break;
    case TAXI_BLEND_TO_PATH_START:
        advance_accelerated_scalar_c(1.0f,
                                     D_0015ED70_c2,
                                     D_0015ED70_c2,
                                     frame_time * 0.5f,
                                     (float *)(taxi_data + 0xB0),
                                     (float *)(taxi_data + 0xB4));
        qa = transposed_rotation_or_start_position;
        s = *(int *)(taxi_data + 0x80 - (-(*(short *)(taxi_data + 0xAC) * 4)));
        tab = D_L16_001600EC;
        qcopy(qa, tab[s].position);
        qcopy(transition_start_rotation, tab[s].direction);
        z = 0.0f;
        func_00215CA8_f5848(
            z, *(int **)(taxi_data + 0xA4), 0, transition_end_position, transition_end_rotation, 0);
        lerp_vector(taxi_data + 0x60,
                    transposed_rotation_or_start_position,
                    transition_end_position,
                    *(float *)(taxi_data + 0xB0));
        *(float *)(taxi_data + 0x74) = fast_add_rotations(
            fast_subtract_rotations(transition_end_rotation[1], transition_start_rotation[1]) *
                *(float *)(taxi_data + 0xB0),
            transition_start_rotation[1]);
        *(float *)(taxi_data + 0x78) = fast_add_rotations(
            fast_subtract_rotations(transition_end_rotation[2], transition_start_rotation[2]) *
                *(float *)(taxi_data + 0xB0),
            transition_start_rotation[2]);
        if (1.0f <= *(float *)(taxi_data + 0xB0)) {
            *(float *)(taxi_data + 0xB0) = z;
            *(float *)(taxi_data + 0xB4) = z;
            moby->state = 4;
        }
        break;
    case TAXI_FOLLOW_PATH:
        if (advance_moby_path(moby)) {
            *(int *)(taxi_data + 0xB0) = 0;
            *(int *)(taxi_data + 0xB4) = 0;
            moby->state = 5;
        }
        break;
    case TAXI_BLEND_TO_DESTINATION_POSE:
        advance_accelerated_scalar_c(1.0f,
                                     D_0015ED70_c2,
                                     D_0015ED70_c2,
                                     frame_time * 0.5f,
                                     (float *)(taxi_data + 0xB0),
                                     (float *)(taxi_data + 0xB4));
        qa = transition_end_position;
        s = *(int *)(taxi_data + 0x80 - (-(*(short *)(taxi_data + 0xAE) * 4)));
        tab = D_L16_001600EC;
        qcopy(qa, tab[s].position);
        qcopy(transition_end_rotation, tab[s].direction);
        sample_camera_path_u(*(int **)(taxi_data + 0xA4),
                             0,
                             transposed_rotation_or_start_position,
                             transition_start_rotation,
                             0,
                             (float)**(int **)(taxi_data + 0xA4));
        lerp_vector(taxi_data + 0x60,
                    transposed_rotation_or_start_position,
                    transition_end_position,
                    *(float *)(taxi_data + 0xB0));
        *(float *)(taxi_data + 0x74) = fast_add_rotations(
            fast_subtract_rotations(transition_end_rotation[1], transition_start_rotation[1]) *
                *(float *)(taxi_data + 0xB0),
            transition_start_rotation[1]);
        *(float *)(taxi_data + 0x78) = fast_add_rotations(
            fast_subtract_rotations(transition_end_rotation[2], transition_start_rotation[2]) *
                *(float *)(taxi_data + 0xB0),
            transition_start_rotation[2]);
        if (1.0f <= *(float *)(taxi_data + 0xB0)) {
            *(int *)(taxi_data + 0xB0) = 0;
            *(int *)(taxi_data + 0xB4) = 0;
            moby->state = 1;
            reset_selected_moby_to_current_placement(moby);
        }
        break;
    case TAXI_WAIT_FOR_ENABLE:
        if (D_0013D4E1) {
            char          *model = moby->pclass;
            unsigned short flags = moby->flags;
            int            value = *(int *)(model + 0x10);
            moby->visible = 1;
            moby->collision = value;
            moby->flags = flags & 0xFFFE;
            moby->state = 1;
        }
        break;
    }
    apply_moby_position_and_rotation_wobble(moby);
    if (moby->state >= 3 && moby->state <= 5) {
        travel_player = &hero;
        travel_player->jump_lock_timer = 2;
        travel_player->unk1F4 = 2;
        transform_moby_vector_table_to_world_space(moby);
        if (voice_is_playing(moby, *(short *)(taxi_data + 0xAA)) == 0) {
            *(short *)(taxi_data + 0xAA) = func_0022ED80_i(0, 4, moby);
        }
    } else if (voice_is_playing(moby, *(short *)(taxi_data + 0xAA)) != 0) {
        voice_slot_index = *(short *)(taxi_data + 0xAA);
        if (voice_slot_index != -1) {
            voice_slot_base = ((char *)&voice_pool) + voice_slot_index * 0x70;
            if (*(char **)(voice_slot_base + 0x88) == moby &&
                *(unsigned char *)(voice_slot_base + 0x74) != 0) {
                release_voice_slot(voice_slot_index);
            }
        }
        *(short *)(taxi_data + 0xAA) = -1;
    }
    fast_vec_sub(position_delta, &moby->pos, previous_position);
    update_entity_matrix(taxi_data + 0x20, position_delta, previous_rotation, &moby->rot);
}

#include "qcopy.h"

extern char         *D_L16_001601AC_slots_e4900 __asm__("D_L16_001600EC");
extern unsigned char D_0013D5C8_b_e4900[] __asm__("D_0013D4C0");

void initialize_taxi_stop_state(void *moby_v) {
    char *moby = moby_v;
    char *data = ((char *)MOBY(moby)->pvars);
    int  *list = (int *)(data + 0x80);
    int   i;
    int   selected_pose_byte_offset;
    char *pose_table_base;
    *(short *)(data + 0xAC) = -1;
    for (i = 0; i < 3; i++) {
        if (fast_vec_dist(moby + 0x10, D_L16_001601AC_slots_e4900 + (list[i] << 7) + 0x30) < 1.0f) {
            *(short *)(data + 0xAC) = i;
        }
    }
    {
        if (D_0013D5C8_b_e4900[0x21] != 0 || *(short *)(data + 0xAC) == 2) {
            int model_collision = *(int *)(((char *)MOBY(moby)->pclass) + 0x10);
            moby[0x31] = 1;
            MOBY(moby)->collision = model_collision;
            MOBY(moby)->flags = MOBY(moby)->flags & 0xFFFE;
            moby[0x20] = 1;
        } else {
            moby[0x20] = 6;
            moby[0x31] = 0;
            MOBY(moby)->flags = MOBY(moby)->flags | 1;
            MOBY(moby)->collision = 0;
        }
    }
    {
        int *selected = list;
        selected += *(short *)(data + 0xAC);
        selected_pose_byte_offset = *selected << 7;
    }
    pose_table_base = D_L16_001601AC_slots_e4900;
    qcopy(data + 0x60, (void *)(selected_pose_byte_offset + (int)pose_table_base + 0x30));
    qcopy(data + 0x70, (void *)(selected_pose_byte_offset + (int)pose_table_base + 0x70));
}
/* Pick the path for the current segment and set the travel speeds from its length. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E5EC0), where it is exact; names translated to the US level program. */

void prepare_taxi_departure_path(struct Moby *moby) __asm__("FUN_L16_002e4a58");

void prepare_taxi_departure_path(struct Moby *moby) {
    enum {
        TAXI_DEPART_FROM_FIRST_STOP = 0,
        TAXI_DEPART_FROM_SECOND_STOP = 1,
        TAXI_DEPART_FROM_THIRD_STOP = 2
    };
    char *taxi_data = (char *)moby->pvars;
    char *selected_path;
    float inv, frame_scaled_travel_step, frame_time_squared_travel_factor,
        first_path_segment_length;
    switch (*(short *)(taxi_data + 0xAC)) {
    case TAXI_DEPART_FROM_FIRST_STOP:
        *(char **)(taxi_data + 0xA4) = D_L16_001B0930[*(int *)(taxi_data + 0x90)];
        *(short *)(taxi_data + 0xAE) = 1;
        break;
    case TAXI_DEPART_FROM_SECOND_STOP:
        *(char **)(taxi_data + 0xA4) = D_L16_001B0930[*(int *)(taxi_data + 0x94)];
        *(short *)(taxi_data + 0xAE) = 0;
        break;
    case TAXI_DEPART_FROM_THIRD_STOP:
        *(char **)(taxi_data + 0xA4) = D_L16_001B0930[*(int *)(taxi_data + 0x9C)];
        *(short *)(taxi_data + 0xAE) = 1;
        break;
    }
    selected_path = *(char **)(taxi_data + 0xA4);
    first_path_segment_length = fast_vec_dist(selected_path + 0x10, selected_path + 0x20);
    frame_scaled_travel_step = frame_time * 20.0f;
    frame_time_squared_travel_factor = frame_time_sq * 5.0f;
    inv = 1.0f / (first_path_segment_length * (float)**(int **)(taxi_data + 0xA4));
    *(int *)(taxi_data + 0xB4) = 0;
    *(int *)(taxi_data + 0xB0) = 0;
    *(float *)(taxi_data + 0xC4) = frame_scaled_travel_step * inv;
    *(float *)(taxi_data + 0xC8) = frame_time_squared_travel_factor * inv;
}
/* Moves the moby through its trigger states and checks its local bounds. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E6478), where it is exact; names translated to the US level program. */

extern float grindrail_gate_local_trigger_bounds[] __asm__("D_L16_001D9750");
extern int   D_L16_0015F5CC;
extern int   FUN_001fa6e0_u(float, int, int) __asm__("FUN_001fa6e0");
extern int   func_0022ED80_6478(int, int, void *) __asm__("FUN_0022da68");

int  D_L16_00161DD8 = 600;
int  D_L16_00161DDC = 0x80808080;
int  D_L16_00161DE0 = 0x80202020;
void update_grindrail_gate(struct Moby *gate_moby) __asm__("FUN_L16_002e5010");

void update_grindrail_gate(struct Moby *gate_moby) {
    enum {
        GRINDRAIL_GATE_INITIALIZE = 0,
        GRINDRAIL_GATE_CHECK_HERO_BOUNDS = 1,
        GRINDRAIL_GATE_WAIT_FOR_COMMAND_TIMEOUT = 2
    };
    char *command_timer = (char *)gate_moby->pvars;
    char *hero_position;
    float pos[16];
    float hero_offset_from_gate[4];
    float glow_cycle_phase;
    int   period = scale_game_frames(30);
    glow_cycle_phase =
        convert_integer_to_float(D_L16_0015F5CC % period) / convert_integer_to_float(period);
    gate_moby->glow_color =
        FUN_001fa6e0_u(fast_sin(glow_cycle_phase * 6.28318f - 3.14159f) * 0.5f + 0.5f,
                       D_L16_00161DDC,
                       D_L16_00161DE0);
    switch (gate_moby->state) {
    case GRINDRAIL_GATE_INITIALIZE:
        gate_moby->state = 1;
        gate_moby->update_dist = 0xFF;
        break;
    case GRINDRAIL_GATE_CHECK_HERO_BOUNDS:
        hero_position = (char *)&hero.motion.pos;
        if (fast_vec_dist(&gate_moby->pos, hero_position) < 16.0f) {
            transpose_rotation_matrix(pos, &gate_moby->rot_row0);
            fast_vec_sub(hero_offset_from_gate, hero_position, &gate_moby->pos);
            transform_vector(hero_offset_from_gate, hero_offset_from_gate, pos);
            if (grindrail_gate_local_trigger_bounds[0] < hero_offset_from_gate[0] &&
                hero_offset_from_gate[0] < grindrail_gate_local_trigger_bounds[1] &&
                grindrail_gate_local_trigger_bounds[2] < hero_offset_from_gate[1] &&
                hero_offset_from_gate[1] < grindrail_gate_local_trigger_bounds[3] &&
                grindrail_gate_local_trigger_bounds[4] < hero_offset_from_gate[2] &&
                hero_offset_from_gate[2] < grindrail_gate_local_trigger_bounds[5]) {
                gate_moby->command = 1;
                gate_moby->state = 2;
                *(int *)command_timer = scale_game_frames(D_L16_00161DD8);
                func_0022ED80_6478(1, 0, gate_moby);
            }
        }
        break;
    case GRINDRAIL_GATE_WAIT_FOR_COMMAND_TIMEOUT:
        if (fast_dec_timer((int *)command_timer)) {
            gate_moby->state = 1;
            gate_moby->command = 0;
        }
        break;
    }
}
/* Moves the moby through its trigger states and checks its local bounds. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E66C0), where it is exact; names translated to the US level program. */

extern int  FUN_001fa6e0_q(float, int, int) __asm__("FUN_001fa6e0");
extern void FUN_0022da68_q(int, int, void *) __asm__("FUN_0022da68");
extern int  func_0022ED80_66C0_q(int, int, void *) __asm__("FUN_0022da68");

extern float D_L16_001D9768[] NOT_SDA;
int          D_L16_00161DE4 = 600;
int          D_L16_00161DE8 = 0x80202020;
int          D_L16_00161DEC = 0x80808080;
extern char  D_0013F3D0[] NOT_SDA;
void update_local_bounds_command_trigger(struct Moby *trigger_moby) __asm__("FUN_L16_002e5258");

void update_local_bounds_command_trigger(struct Moby *trigger_moby) {
    enum {
        GRINDRAIL_TRIGGER_INITIALIZE = 0,
        GRINDRAIL_TRIGGER_CHECK_LOCAL_BOUNDS = 1,
        GRINDRAIL_TRIGGER_WAIT_FOR_COMMAND_TIMEOUT = 2
    };
    char *command_timer = (char *)trigger_moby->pvars;
    char *bounds_check_position;
    float pos[16];
    float bounds_check_offset_from_trigger[4];
    float glow_cycle_phase;
    int   period = scale_game_frames(30);
    glow_cycle_phase =
        convert_integer_to_float(D_L16_0015F5CC % period) / convert_integer_to_float(period);
    trigger_moby->glow_color =
        FUN_001fa6e0_q(fast_sin(glow_cycle_phase * 6.28318f - 3.14159f) * 0.5f + 0.5f,
                       D_L16_00161DE8,
                       D_L16_00161DEC);
    switch (trigger_moby->state) {
    case GRINDRAIL_TRIGGER_INITIALIZE:
        trigger_moby->state = 1;
        trigger_moby->update_dist = 0xFF;
        break;
    case GRINDRAIL_TRIGGER_CHECK_LOCAL_BOUNDS:
        bounds_check_position = D_0013F3D0;
        if (fast_vec_dist(&trigger_moby->pos, bounds_check_position) < 16.0f) {
            transpose_rotation_matrix(pos, &trigger_moby->rot_row0);
            fast_vec_sub(
                bounds_check_offset_from_trigger, bounds_check_position, &trigger_moby->pos);
            bounds_check_offset_from_trigger[3] = 0.0f;
            transform_vector(
                bounds_check_offset_from_trigger, bounds_check_offset_from_trigger, pos);
            if (D_L16_001D9768[0] < bounds_check_offset_from_trigger[0] &&
                bounds_check_offset_from_trigger[0] < D_L16_001D9768[1] &&
                D_L16_001D9768[2] < bounds_check_offset_from_trigger[1] &&
                bounds_check_offset_from_trigger[1] < D_L16_001D9768[3] &&
                D_L16_001D9768[4] < bounds_check_offset_from_trigger[2] &&
                bounds_check_offset_from_trigger[2] < D_L16_001D9768[5]) {
                func_0022ED80_66C0_q(1, 0, trigger_moby);
                trigger_moby->command = 1;
                trigger_moby->state = 2;
                *(int *)command_timer = scale_game_frames(D_L16_00161DE4);
            }
        }
        break;
    case GRINDRAIL_TRIGGER_WAIT_FOR_COMMAND_TIMEOUT:
        if (fast_dec_timer((int *)command_timer)) {
            trigger_moby->state = 1;
            trigger_moby->command = 0;
        }
        break;
    }
}
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E6908), where it is exact; names translated to the US level program. */

typedef struct {
    float value;
} Level16VendorTurnData;

typedef struct {
    char                   pad0[0x20];
    unsigned char          state;
    char                   pad21[0x57];
    Level16VendorTurnData *data;
    char                   pad7C[0x40];
    unsigned char          active;
} Level16VendorTurnMoby;

float D_L16_00161DF0[4] __attribute__((section(".sdata"))) = {0.0f, -1.0f, 1.0f, 0.0f};

void        FUN_L16_002e56e0_c(Level16VendorTurnMoby *moby) __asm__("FUN_L16_002e56e0");
extern void func_L16_002E6B48_6908(void *) __asm__("FUN_L16_002e56e0");
extern int  func_0022ED80_6908(int, int, void *) __asm__("FUN_0022da68");

void grindrail_button_update(struct Moby *m) __asm__("FUN_L16_002e54a0");

void grindrail_button_update(struct Moby *button_moby) {
    enum {
        GRINDRAIL_BUTTON_INITIALIZE_GLOW_PHASE = 0,
        GRINDRAIL_BUTTON_CHECK_ACTIVATION = 1
    };
    char *button_data = (char *)button_moby->pvars;
    char *hero_position;
    char *base;
    float button_offset_from_hero[4];
    float glow_blend_factor;
    switch (button_moby->state) {
    case GRINDRAIL_BUTTON_INITIALIZE_GLOW_PHASE:
        button_moby->state = 1;
        *(float *)button_data = random_angle_radians();
        break;
    case GRINDRAIL_BUTTON_CHECK_ACTIVATION:
        if (fast_dec_timer((int *)(button_data + 4))) {
            hero_position = (char *)&hero.motion.pos;
            if (vector_distance_xy(&button_moby->pos, hero_position) < 4.0f) {
                base = hero_position - 0x80;
                if (*(int *)(base + 0x2084) == 0x28 && *(int *)(base + 0x5A8) != -1) {
                    transform_vector_by_basis(
                        button_offset_from_hero, &D_L16_00161DF0, &button_moby->rot_row0);
                    fast_vec_add(
                        button_offset_from_hero, button_offset_from_hero, &button_moby->pos);
                    fast_vec_sub(button_offset_from_hero, button_offset_from_hero, hero_position);
                    transform_vector_by_basis(
                        button_offset_from_hero, button_offset_from_hero, hero_position - 0x40);
                    if (absolute_float(button_offset_from_hero[1]) < 0.1f) {
                        if ((button_offset_from_hero[0] > 0.0f && *(int *)(base + 0x5A8) == 3) ||
                            (button_offset_from_hero[0] < 0.0f && *(int *)(base + 0x5A8) == 2)) {
                            func_L16_002E6B48_6908(button_moby);
                            func_0022ED80_6908(1, 0, button_moby);
                        }
                    }
                }
            }
        }
        if (find_moby_hit(button_moby, 0x330000, 0)) {
            func_L16_002E6B48_6908(button_moby);
            func_0022ED80_6908(0, 0, button_moby);
            button_moby->hit_slot = 0xFF;
        }
        break;
    }
    *(float *)button_data = fast_add_rotations(*(float *)button_data, frame_time * 12.566371f);
    glow_blend_factor = (fast_sin(*(float *)button_data) + 1.0f) * 0.5f;
    if (button_moby->command) {
        button_moby->glow_color = FUN_001fa6e0_u(glow_blend_factor, 0x80208020, 0x80202020);
    } else {
        button_moby->glow_color = FUN_001fa6e0_u(glow_blend_factor, 0x80202080, 0x80202020);
    }
}

/* Start the vendor moby's turn and mark it active. */
/* Ported from rac1-decomp (src/overlays/l16/vendor_002A50F0.c: func_L16_002E6B48), where it is exact; names translated to the US level program. */

void activate_grindrail_button(Level16VendorTurnMoby *moby) __asm__("FUN_L16_002e56e0");

void activate_grindrail_button(Level16VendorTurnMoby *moby) {
    moby->data->value = -3.1415925f;
    moby->active = 1;
    moby->state = 2;
}

#include "qcopy.h"

/* Update the wandering helper's motion, animations, interaction and damage states. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E7270), where it is exact; names translated to the US level program. */

typedef struct {
    char           pad0[0x10];
    char           pos[0x10];
    unsigned char  b20;
    char           pad21[0xB];
    float          f2C;
    unsigned char  b30;
    unsigned char  b31;
    short          s32;
    unsigned short h34;
    char           pad36[0x12];
    float          f48;
    char           pad4C[0x2C];
    Child         *child;
    char           pad7C[0x14];
    int            f90;
} Obj;

extern char  D_L16_001671C0[];
extern float move_chomper_toward_position(void *, void *) __asm__("FUN_L16_002e6428");
extern int   FUN_L00_002dc9f0(void *, void *);
extern s32   rand();
extern void  collect_mobies_in_sphere_at_moby_point(
     void *, int, int, float, float, float, int, int, int) __asm__("FUN_L00_002599e8");
extern void FUN_L00_0025a120(void *);
extern void FUN_L00_00263ac8(float, int, int, unsigned char *);
void update_chomper_hit_reaction_and_target(void *chomper_instance) __asm__("FUN_L16_002e6208");

void update_chomper_hit_reaction_and_target(void *arg) __asm__("FUN_L16_002e6208");
s32  is_value_within_interpolated_window(struct InterpolatedStateEntry *arg0,
                                         f32 fparg0) __asm__("FUN_00214cc8");

short D_L16_00161E48 = 0;
void  chomper_update(struct Moby *m) __asm__("FUN_L16_002e5e08");

void chomper_update(struct Moby *chomper_moby) {
    enum {
        CHOMPER_INITIALIZE = 0,
        CHOMPER_WAIT_TO_APPROACH_TARGET = 1,
        CHOMPER_APPROACH_TARGET = 4,
        CHOMPER_FACE_TARGET = 5,
        CHOMPER_RETURN_HOME = 6,
        CHOMPER_KNOCKBACK = 8
    };
    char *chomper_data = (char *)chomper_moby->pvars;
    update_chomper_hit_reaction_and_target(chomper_moby);
    FUN_L00_00263ac8(2.1f, (int)chomper_moby, 1, (unsigned char *)(chomper_data + 0x220));
    if (chomper_moby->visible != 0 && fast_vec_dist(&chomper_moby->pos, D_L16_001671C0) < 29.0f) {
        FUN_L00_0025a120(chomper_moby);
        chomper_moby->shadow_range = 23;
    }
    switch (chomper_moby->state) {
    case CHOMPER_INITIALIZE:
        if (*(int *)(chomper_data + 0x1F0) == -1) {
            mark_moby_for_removal(chomper_moby);
            return;
        }
        *(unsigned short *)(chomper_data + 0x24) = 1;
        *(unsigned char *)(chomper_data + 0x28) = 0;
        *(float *)(chomper_data + 0x20) = 1.0f;
        *(int *)(chomper_data + 0xD0) = (int)&D_L16_00161E48;
        if (rand() & 1)
            chomper_moby->flags |= 0x8000;
        qcopy(chomper_data + 0x1D0, &chomper_moby->pos);
        chomper_moby->state = 1;
        if (chomper_moby->prev_seq != 2)
            blend_moby_animation(chomper_moby, 2, 0, scale_game_frames(20));
        *(unsigned char *)(chomper_data + 0x58) = truncate_float_to_s32(8.0f);
        *(unsigned char *)(chomper_data + 0x5A) = truncate_float_to_s32(3.2f);
        break;
    case CHOMPER_WAIT_TO_APPROACH_TARGET:
        if (*(int *)(chomper_data + 0x1C4) != 2) {
            chomper_moby->state = 4;
            if (chomper_moby->prev_seq != 0)
                blend_moby_animation(chomper_moby, 0, 0, scale_game_frames(20));
        }
        break;
    case 2:
    case 3:
        break;
    case CHOMPER_APPROACH_TARGET:
        if (move_chomper_toward_position(chomper_moby, chomper_data + 0x180) < 1.25f) {
            chomper_moby->state = 5;
            if (chomper_moby->prev_seq != 1)
                blend_moby_animation(chomper_moby, 1, 0, scale_game_frames(20));
        } else if (*(int *)(chomper_data + 0x1C4) == 2)
            chomper_moby->state = 6;
        break;
    case CHOMPER_FACE_TARGET: {
        float *yaw = &chomper_moby->rot.z;
        float  angle = fast_arc_tan(*(float *)(chomper_data + 0x180) - chomper_moby->pos.x,
                                   *(float *)(chomper_data + 0x184) - chomper_moby->pos.y);
        approach_angle(yaw,
                       (float *)(chomper_data + 0x1F4),
                       angle,
                       frame_time_sq * 12.566371f,
                       frame_time_sq * 12.566371f,
                       frame_time * 12.566371f);
        if (is_value_within_interpolated_window(chomper_moby, 13.0f)) {
            collect_mobies_in_sphere_at_moby_point(chomper_moby, 0, 1, 0.333f, 1.0f, 1.0f, 0, 1, 0);
        } else if ((chomper_moby->anim_flags & 2) &&
                   fast_vec_dist(&chomper_moby->pos, chomper_data + 0x180) > 1.5f) {
            chomper_moby->state = 4;
            if (chomper_moby->prev_seq != 0)
                blend_moby_animation(chomper_moby, 0, 0, scale_game_frames(20));
        }
        break;
    }
    case CHOMPER_RETURN_HOME:
        if (move_chomper_toward_position(chomper_moby, chomper_data + 0x1D0) < 1.0f) {
            chomper_moby->state = 1;
            if (chomper_moby->prev_seq != 2)
                blend_moby_animation(chomper_moby, 2, 0, scale_game_frames(20));
        } else if (*(int *)(chomper_data + 0x1C4) != 2)
            chomper_moby->state = 4;
        break;
    case 7:
        if (FUN_L00_002dc9f0(chomper_moby, chomper_data + 0x120)) {
            chomper_moby->state = 1;
            *(unsigned short *)(chomper_data + 0xC8) = 0;
        }
        break;
    case CHOMPER_KNOCKBACK:
        if (mine_fall(chomper_moby, chomper_data + 0x120) & 0x140) {
            cart_burst(chomper_moby, &chomper_moby->pos, 6, 0.5f, 13.0f);
            FUN_L00_00257470(chomper_moby, 0, -1);
            mark_moby_for_removal(chomper_moby);
        }
        break;
    }
}

#include "qcopy.h"
#include "rnc/overlay/quad.h"

/* Start the knock-back when the moby is hit, then steer it along its path and note when it strays. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E7670), where it is exact; names translated to the US level program. */

typedef struct {
    char          pad0[0x110];
    short         reaction[3]; /* 0x110: record func_L00_0025E4B0 and func_L00_0025E590 take */
    char          pad116;
    unsigned char reaction_time; /* 0x117 */
    char          pad118[8];
    float         knock[4];     /* 0x120: record func_L00_0025D5B0 fills */
    float         speed;        /* 0x130 */
    float         acceleration; /* 0x134 */
    float         jump_speed;   /* 0x138 */
    float         turn_speed;   /* 0x13C */
    int           flags;        /* 0x140 */
    int           kind;         /* 0x144 */
    float         strength;     /* 0x148 */
    char          pad14C[0x11];
    unsigned char pending; /* 0x15D */
    char          pad15E[0xE];
    float         timer; /* 0x16C */
    float         f170;
    float         f174;
    char          pad178[8];
    float         target[4]; /* 0x180 */
    char          pad190[0x30];
    int           i1C0;
    int           i1C4;
    char          pad1C8[8];
    float         home[4]; /* 0x1D0 */
    char          pad1E0[0x10];
    int           path; /* 0x1F0: index into D_L16_001B0C30 */
} L16PursuitData;

typedef struct {
    char            pad0[0x10];
    float           position[4]; /* 0x10 */
    unsigned char   state;       /* 0x20 */
    char            pad21[0x13];
    unsigned short  flags; /* 0x34 */
    char            pad36[0x42];
    L16PursuitData *data; /* 0x78 */
    char            pad7C[0x18];
    int             i94;
    char            pad98[0xC];
    unsigned char   bA4;
} L16PursuitMoby;

extern float chomper_knockback_horizontal_speed_approach_rate_gp __asm__("D_L16_00161E58")
    __attribute__((sda));
extern float D_0015EE70;
extern float chomper_knockback_gravity_gp __asm__("D_L16_00161E54") __attribute__((sda));
extern float chomper_knockback_vertical_speed_gp __asm__("D_L16_00161E5C") __attribute__((sda));
extern float chomper_knockback_horizontal_speed_gp __asm__("D_L16_00161E60") __attribute__((sda));
extern int   FUN_L00_0025ff38(void *, void *, float, int, int, void *, int);
extern void  trooper_knockback_arc(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");
extern void  trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");

void update_chomper_hit_reaction_and_target(void *chomper_instance) {
    L16PursuitMoby *chomper_moby = chomper_instance;
    L16PursuitData *pursuit_data = chomper_moby->data;
    char           *hit;
    int             reaction_kind_or_path_address; /* the reaction kind, later the path */
    int reaction_flags_or_path_entry_address; /* the reaction flags, later the path's slot in the table */
    float scratch[8]; /* [0..3] where the hit came from, [4] the heading that gives */

    if (chomper_moby->state == 0) {
        return;
    }
    hit = find_moby_hit(chomper_moby, 0x330000, 0);
    if (hit) {
        reaction_flags_or_path_entry_address = 0x200;
        pursuit_data->flags = reaction_flags_or_path_entry_address;
        pursuit_data->strength = 0.5f;
        pursuit_data->speed = chomper_knockback_gravity_gp * frame_time_sq;
        pursuit_data->acceleration =
            chomper_knockback_horizontal_speed_approach_rate_gp * frame_time_sq;
        pursuit_data->jump_speed = chomper_knockback_horizontal_speed_gp * frame_time;
        pursuit_data->turn_speed = chomper_knockback_vertical_speed_gp * frame_time;
        reaction_kind_or_path_address = 0x29;
        pursuit_data->kind = reaction_kind_or_path_address;
        pursuit_data->pending = 0;
        pursuit_data->timer = frame_time * 2.0f;
        chomper_moby->flags &= ~0x1000;
        *(OvlQuad *)scratch = *(OvlQuad *)(hit + 0x10);
        trooper_knockback_arc(
            scratch, scratch + 4, &pursuit_data->jump_speed, &pursuit_data->turn_speed);
        trooper_start_knockback(chomper_moby, pursuit_data->knock, 5, 1, 0, scratch[4]);
        pursuit_data->f170 = 8.0f;
        pursuit_data->f174 = 16.0f;
        chomper_moby->state = 8;
        chomper_moby->i94 = 0;
        pursuit_data->reaction_time = 0x78;
        trooper_anim_update(chomper_moby, pursuit_data->reaction);
    }
    chomper_moby->bA4 = 0xFF;
    trooper_anim_finish(chomper_moby, pursuit_data->reaction);
    reaction_flags_or_path_entry_address = (int)&D_L16_001B0930[pursuit_data->path];
    reaction_kind_or_path_address = *(int *)reaction_flags_or_path_entry_address;
    if (FUN_L00_0025ff38(chomper_moby,
                         pursuit_data->target,
                         16.0f,
                         0,
                         0,
                         (void *)(reaction_kind_or_path_address + 0x10),
                         *(int *)reaction_kind_or_path_address) != 2) {
        if (vector_distance_xy(pursuit_data->home, pursuit_data->target) > 16.0f ||
            absolute_float(chomper_moby->position[2] - pursuit_data->target[2]) > 3.0f) {
            pursuit_data->i1C4 = 2;
        }
    }
    if (pursuit_data->i1C0 == 0) {
        struct Hero *player = &hero;
        pursuit_data->i1C0 = (int)player->moby;
        qcopy(pursuit_data->target, &player->motion.pos);
    }
}
/* Steers and moves a moby toward a target point, returning the distance. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E7890), where it is exact; names translated to the US level program. */

extern f32   advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("func_00213F38");
extern float D_0015EE6C_u, D_0015ED70_c __asm__("D_0015ED70");
extern void  FUN_L00_00258830(void *, void *, float, float, float, int);
extern void
resolve_point_path_segment_xy_clearance(float, void *, void *, void *) __asm__("FUN_L00_00261d78");
extern float
func_00214D88_7890(float *, float, float, float, float, float *) __asm__("FUN_00213f38");

float move_chomper_toward_position(void *m_v, void *t_v) {
    char  *moby = m_v;
    float *target_position = t_v;
    char  *moby_data = ((char *)MOBY(moby)->pvars);
    float  interpolated_travel_distance;
    float  movement_delta[4];
    float  distance_to_target;
    float  oldz;
    float  target_yaw = fast_arc_tan(target_position[0] - MOBY(moby)->pos.x,
                                    target_position[1] - MOBY(moby)->pos.y);
    target_yaw = fast_add_rotations(target_yaw, *(float *)(moby_data + 0x1F8));
    oldz = MOBY(moby)->pos.z;
    interpolated_travel_distance = 0.0f;
    distance_to_target = fast_vec_dist(moby + 0x10, target_position);
    approach_angle((float *)&MOBY(moby)->rot.z,
                   (float *)(moby_data + 0x1F4),
                   target_yaw,
                   D_0015ED70_c * 12.566371f,
                   D_0015ED70_c * 12.566371f,
                   frame_time * 12.566371f);
    func_00214D88_7890(&interpolated_travel_distance,
                       distance_to_target,
                       D_0015ED70_c * 8.0f,
                       D_0015ED70_c * 12.0f,
                       frame_time * 4.0f,
                       (float *)(moby_data + 0x204));
    fast_vec_normalize(movement_delta, moby + 0xC0, *(float *)(moby_data + 0x204));
    movement_delta[2] = *(float *)(moby_data + 0x208) - D_0015ED70_c * 10.0f;
    FUN_L00_00258830(moby, movement_delta, 0.5f, 0.5f, 0.0f, 0x10);
    resolve_point_path_segment_xy_clearance(
        0.333f, *(void **)(moby_data + 0x1F0), moby + 0x10, moby + 0x10);
    *(float *)(moby_data + 0x208) = oldz - MOBY(moby)->pos.z;
    return distance_to_target;
}

#include "qcopy.h"

/* Update the challenge NPC's interaction state and gaze. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002E7C70), where it is exact; names translated to the US level program. */

typedef struct {
    char          pad0[4];
    short         previous;
    char          pad6[2];
    unsigned char flag;
    char          pad9[0x2D];
    short         mode;
    int           pending;
} L16ChallengeActivation;

typedef struct {
    char  pad00[0x20];
    char  activation[0x40];
    char  pad60[0x64];
    float initial_yaw;
    int   moving_timer, target_timer;
    float target[4];
    char  body[0x60];
    char  pad140[4];
    float pitch, yaw;
    char  pad14C[4];
    float height;
    char  pad154[0xC];
    char  head[0x60];
    char  pad1C0[8];
    float head_yaw;
} L16ChallengeData;

typedef struct {
    char              pad00[0x10];
    float             position[4];
    unsigned char     state;
    char              pad21[0xF];
    unsigned char     opacity, visible;
    char              pad32[0x16];
    float             yaw;
    char              pad4C[7];
    unsigned char     animation;
    char              pad54[0x24];
    L16ChallengeData *data;
    char              pad7C[3];
    unsigned char     distance_class;
    char              pad80[0x3C];
    unsigned char     event;
} L16ChallengeMoby;

typedef struct {
    char  pad00[0x80];
    float position[4];
    char  pad90[0x40];
    float aim[4];
    char  padE0[0x20];
    float velocity[4];
    char  pad110[0x798];
    int   points;
    char  pad8AC[0x17D8];
    int   state;
    char  pad2088[4];
    int   mode;
} L16ChallengePlayer;

typedef struct {
    float target[4];
    float eye[4];
    float delta[4];
} L16ChallengeScratch;

extern char                     D_0013D50F[], D_0014161B[];
extern char                     gadgetron_ceo_challenge_start_pose[] __asm__("D_L16_001D9840");
extern char                     gadgetron_ceo_challenge_return_pose[] __asm__("D_L16_001D9860");
extern int                      freeze_stats_score_ref_b[] __asm__("D_0015EE6C") MACRO_ADDR;
extern volatile int             D_0015EE6C_p __asm__("D_0015EE6C");
extern float                    D_L16_0015F638;
extern int                      D_0015EEA4;
extern unsigned char D_0013D4DE NOT_SDA;
extern unsigned char D_0013D507 NOT_SDA;
extern unsigned char            D_0013D408[];
extern void                     FUN_L00_00263d40(int, int);
extern void queue_scene_movie_with_game_language(int) __asm__("FUN_L00_00299460");
extern void FUN_L16_002923d0(int);
extern void begin_dialogue_camera_transition(float, void *) __asm__("FUN_L01_002783a8");
void activate_gadgetron_ceo_challenge_environment(struct Moby *moby) __asm__("FUN_L16_002e70c0");

extern void activate_gadgetron_ceo_challenge_environment(struct Moby *) __asm__("FUN_L16_002e70c0");
void restore_gadgetron_ceo_non_challenge_environment(struct Moby *moby) __asm__("FUN_L16_002e7168");

extern void
restore_gadgetron_ceo_non_challenge_environment(struct Moby *) __asm__("FUN_L16_002e7168");
extern char               D_00141968[];
extern L16ChallengePlayer D_0013E633_challenge __asm__("D_0013E533");

void gadgetron_ceo_update(L16ChallengeMoby *m) __asm__("FUN_L16_002e6808");

void gadgetron_ceo_update(L16ChallengeMoby *ceo_moby) {
    enum {
        GADGETRON_CEO_INITIALIZE_DIALOGUE = 0,
        GADGETRON_CEO_POLL_DIALOGUE = 1,
        GADGETRON_CEO_WAIT_AFTER_DIALOGUE = 2,
        GADGETRON_CEO_MONITOR_CHALLENGE_EVENTS = 3,
        GADGETRON_CEO_WAIT_TO_SAVE = 4
    };
    L16ChallengeScratch     scratch;
    L16ChallengeData       *ceo_data = ceo_moby->data;
    L16ChallengeActivation *activation = (L16ChallengeActivation *)ceo_data->activation;
    float                   rate, head_rate;
    int                     tracking;
    if (ceo_moby->visible && fast_vec_dist(ceo_moby->position, D_L16_001671C0) < 28.0f) {
        FUN_L00_0025a120(ceo_moby);
        ceo_moby->distance_class = 22;
    }
    attach_big_head_cheat_manipulator_to_matching_moby(ceo_moby);
    switch (ceo_moby->state) {
    case GADGETRON_CEO_INITIALIZE_DIALOGUE:
        ceo_moby->opacity = 255;
        ceo_moby->state = 1;
        initialize_npc_dialogue(ceo_moby, activation);
        *(float *)((char *)ceo_data + 0x2C) = 3.7f;
        ceo_data->initial_yaw = ceo_moby->yaw;
        if (*(int *)(D_0013D408 + 0x814) == 0)
            ceo_moby->yaw = fast_add_rotations(ceo_moby->yaw, 3.14159265f);
        restore_gadgetron_ceo_non_challenge_environment((char *)ceo_moby);
        break;
    case GADGETRON_CEO_POLL_DIALOGUE: {
        if (((L16ChallengePlayer *)((char *)&D_0013E633_challenge + 0xE1D))->mode != 15)
            D_L16_0015F638 = 115.0f;
        else
            D_L16_0015F638 = 100.0f;
        if (update_npc_dialogue(ceo_moby, (char *)ceo_data + 0x20)) {
            ceo_moby->yaw = ceo_data->initial_yaw;
            begin_dialogue_camera_transition(3.5f, ceo_moby);
            ceo_moby->state = 2;
        }
        if (!D_0013D4DE && vector_distance_xy(&hero.motion.pos, ceo_moby->position) < 3.5f) {
            int   elapsed, period;
            char *statistics;
            elapsed = scale_game_frames(freeze_stats_score_ref_b[14]);
            period = 600;
            statistics = ((char *)&D_00141968);
            elapsed -= *(unsigned short *)(statistics + 0x43A) * period;
            if (elapsed > (int)((float)scale_game_frames(18) * 60.0f) ||
                *(unsigned short *)(statistics + 0x43A) * period == 0) {
                help_request(0x3E83, 0x87);
            } else if (scale_game_frames(D_0015EEA4) / 600 >
                       *(unsigned short *)(statistics + 0x43A)) {
                *(unsigned short *)(statistics + 0x43A) = scale_game_frames(D_0015EEA4) / 600;
            }
        }
        if (activation->mode == 3) {
            char *camera;
            char *pose;
            int   pending;
            pending = D_L16_0015F5CC;
            activation->previous = (unsigned short)activation->mode;
            activation->mode = 2;
            activation->pending = pending;
            ceo_moby->event = 0;
            pose = gadgetron_ceo_challenge_start_pose;
            set_race_camera(pose, pose + 0x10, 0, 1);
            camera = (char *)hero.saved_pos_rot;
            qcopy(camera, pose + 0x20);
            qcopy(camera + 0x10, pose + 0x30);
            ceo_moby->state = 3;
            activate_gadgetron_ceo_challenge_environment((char *)ceo_moby);
        }
        if (((L16ChallengePlayer *)((char *)&D_0013E633_challenge + 0xE1D))->mode == 22) {
            ceo_moby->state = 3;
            activate_gadgetron_ceo_challenge_environment((char *)ceo_moby);
        }
        break;
    }
    case GADGETRON_CEO_WAIT_AFTER_DIALOGUE:
        if (*(int *)&D_L16_0015F5C4 != 2) {
            int previous;
            ceo_moby->state = 1;
            previous = activation->previous;
            if (previous == 4) {
                FUN_L16_002923d0(0);
                ceo_moby->state = previous;
            }
        }
        break;
    case GADGETRON_CEO_WAIT_TO_SAVE:
        if (*(int *)&D_L16_0015F5C4 == 0) {
            ceo_moby->state = 1;
            grant_inventory_item(31, 1);
            memcard_save_data(0, -1);
        }
        break;
    case GADGETRON_CEO_MONITOR_CHALLENGE_EVENTS:
    default: {
        L16ChallengePlayer *player = (L16ChallengePlayer *)((char *)&D_0013E633_challenge + 0xE1D);
        unsigned char       event;
        if (player->points >= 4500) {
            unsigned char *flags = D_0013D408;
            if (!flags[0x1A]) {
                flags[0x1A] = 1;
                allocate_voice_for_bank_entry(1, 0, 0);
                FUN_L00_00263d40(0x53D6, -1);
            }
            if (D_0015EE6C_p < 4500)
                D_0015EE6C_p = player->points;
        }
        if (((L16ChallengePlayer *)((char *)&D_0013E633_challenge + 0xE1D))->mode == 0) {
            ceo_moby->state = 1;
            restore_gadgetron_ceo_non_challenge_environment((char *)ceo_moby);
        }
        event = ceo_moby->event;
        if (event == 1) {
            ceo_moby->state = event;
            set_race_camera(gadgetron_ceo_challenge_return_pose,
                            gadgetron_ceo_challenge_return_pose + 0x10,
                            0,
                            1);
            ceo_moby->event = 0;
            if (!D_0013D507) {
                int            pending;
                unsigned short previous;
                previous = activation->mode;
                pending = D_L16_0015F5CC;
                activation->previous = previous;
                activation->mode = 4;
                activation->pending = pending;
                activation->flag = event;
            }
            restore_gadgetron_ceo_non_challenge_environment((char *)ceo_moby);
        }
        if (ceo_moby->event == 2) {
            ceo_moby->state = 1;
            set_race_camera(gadgetron_ceo_challenge_return_pose,
                            gadgetron_ceo_challenge_return_pose + 0x10,
                            0,
                            1);
            ceo_moby->event = 0;
            restore_gadgetron_ceo_non_challenge_environment((char *)ceo_moby);
        }
        if (ceo_moby->event == 3) {
            char *camera;
            char *pose = gadgetron_ceo_challenge_start_pose;
            ceo_moby->event = 0;
            set_race_camera(pose, pose + 0x10, 0, 1);
            camera = (char *)hero.saved_pos_rot;
            qcopy(camera, pose + 0x20);
            qcopy(camera + 0x10, pose + 0x30);
        }
        break;
    }
    }
    rate = 0.02f;
    head_rate = 0.3f;
    tracking = 0;
    if (ceo_moby->animation == 0) {
        char *player = (char *)&D_0013E633_challenge + 0xE9D;
        tracking = 1;
        if (vector_distance_xy(ceo_moby->position, player) < 8.0f &&
            fast_difference_between_rotations(
                ceo_moby->yaw,
                fast_arc_tan(
                    ((L16ChallengePlayer *)((char *)&D_0013E633_challenge + 0xE1D))->aim[0] -
                        ceo_moby->position[0],
                    ((L16ChallengePlayer *)((char *)&D_0013E633_challenge + 0xE1D))->aim[1] -
                        ceo_moby->position[1])) < 1.5707964f) {
            if (fast_vec_length(player + 0x80) > 0.01f)
                ceo_data->moving_timer = scale_game_frames(120);
            else
                fast_dec_timer(&ceo_data->moving_timer);
        } else if (ceo_data->moving_timer) {
            ceo_data->moving_timer = 0;
            qcopy(ceo_data->target, (char *)&D_0013E633_challenge + 0xEED);
        }
        if (fast_dec_timer(&ceo_data->target_timer)) {
            float heading;
            ceo_data->target_timer =
                truncate_float_to_s32(scale_time(random_float_between(180.0f, 300.0f)));
            heading =
                fast_add_rotations(ceo_moby->yaw, random_float_between(-90.0f, 90.0f) * DEG_TO_RAD);
            build_spherical_offset(
                ceo_data->target, 6.0f, heading, random_float_between(0.0f, 30.0f) * DEG_TO_RAD);
            fast_vec_add(ceo_data->target, ceo_data->target, ceo_moby->position);
        }
        if (ceo_data->moving_timer) {
            qcopy(scratch.target, (char *)&D_0013E633_challenge + 0xEED);
            rate = 0.04f;
            head_rate = 0.3f;
        } else {
            qcopy(scratch.target, ceo_data->target);
        }
    }
    if (tracking) {
        float yaw;
        float pitch;
        qcopy(scratch.eye, ceo_moby->position);
        scratch.eye[2] += 1.0f;
        fast_vec_sub(scratch.delta, scratch.target, scratch.eye);
        yaw = fast_subtract_rotations(fast_arc_tan(scratch.delta[0], scratch.delta[1]),
                                      ceo_moby->yaw);
        pitch = -fast_arc_tan(vector_length_xy(scratch.delta), scratch.delta[2]);
        if (yaw > 1.5707964f)
            yaw = 1.5707964f;
        else if (yaw < -1.5707964f)
            yaw = -1.5707964f;
        if (pitch > 0.5235988f)
            pitch = 0.5235988f;
        else if (pitch < -0.5235988f)
            pitch = -0.5235988f;
        ceo_data->pitch = pitch;
        ceo_data->yaw = yaw * 0.7f;
        ceo_data->head_yaw = yaw * 0.3f;
    }
    if (D_0015EDB0_b)
        *(float *)((char *)ceo_data + 0x150) = 2.75f;
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, ceo_moby, ceo_data->body, 0);
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, ceo_moby, ceo_data->head, 1);
}
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002E8538), where it is exact; names translated to the US level program. */

extern f32  D_L16_0015F638;
extern void jet_game_state(int, int) __asm__("FUN_L01_0027a248");
extern void jet_group_state(int, int, int, int) __asm__("FUN_L13_002650a8");
extern void challenge_set_layout_param(float f) __asm__("FUN_L16_002de3b8");

void activate_gadgetron_ceo_challenge_environment(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    int  *group_index_entry = (int *)(data + 0x60);
    int   i;
    for (i = 15; i >= 0; i--) {
        jet_group_state(*group_index_entry, 1, 1, 1);
        group_index_entry++;
    }
    group_index_entry = (int *)(data + 0xA0);
    for (i = 7; i >= 0; i--) {
        jet_group_state(*group_index_entry, 0, 0, 0);
        group_index_entry++;
    }
    *(float *)&D_L16_0015F638 = 74.0f;
    challenge_set_layout_param(76.0f);
    jet_game_state(2, 6);
}
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002E85E0), where it is exact; names translated to the US level program. */

void restore_gadgetron_ceo_non_challenge_environment(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    int  *group_index_entry;
    int   i;
    group_index_entry = (int *)(data + 0x60);
    for (i = 15; i >= 0; i--) {
        jet_group_state(group_index_entry[0], 0, 0, 0);
        group_index_entry++;
    }
    group_index_entry = (int *)(data + 0xA0);
    for (i = 7; i >= 0; i--) {
        jet_group_state(group_index_entry[0], 1, 1, 0);
        group_index_entry++;
    }
    challenge_set_layout_param(100.0f);
    jet_game_state(0, 8);
}

extern int func_00215570_e7208(void *, int) __asm__("FUN_00214720");
void       update_two_region_trigger_latch(struct Moby *moby) __asm__("FUN_L16_002e7208");

void update_two_region_trigger_latch(struct Moby *moby) {
    enum {
        CLIP_TRIGGER_WAIT_FOR_ARMING = 0,
        CLIP_TRIGGER_CHECK_LATCH = 1
    };
    char *data = ((char *)moby->pvars);
    switch (*(int *)(data + 8)) {
    case CLIP_TRIGGER_WAIT_FOR_ARMING: {
        char *hero_position = (char *)&hero.motion.pos;
        if (func_00215570_e7208(hero_position, *(int *)data) != 0) {
            if (*(int *)(hero_position + 0x200C) == 0xF) {
                *(int *)(data + 8) = 1;
            }
        }
        break;
    }
    case CLIP_TRIGGER_CHECK_LATCH: {
        struct Hero *player = &hero;
        if (player->state.current == 0x42) {
            *(int *)(data + 8) = 0;
        } else if (func_00215570_e7208(&player->motion.pos, *(int *)(data + 4)) != 0) {
            unsigned char *flags = (unsigned char *)item_unlocked;
            if (flags[0x19] == 0) {
                flags[0x19] = 1;
                allocate_voice_for_bank_entry(1, 0, 0);
                queue_dialogue_message(0x53D6, -1);
            }
        }
        break;
    }
    }
}

#include "qcopy.h"

/* Update of a path walker: keeps its sound alive, follows the path behind its owner and jumps ahead while unobserved. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002E8B30), where it is exact; names translated to the US level program. */

typedef struct {
    int            path_index;
    float          parameter;
    float          speed;
    float          period;
    unsigned char *owner;
    float          height_step;
    float          target_speed;
    short          countdown;
    short          sound;
} L16WalkerData_c;

extern float approach_value(float *, float, float) __asm__("FUN_00213ed8");
extern float path_walker_gap_range_start_gp __asm__("D_L16_00161E68") __attribute__((sda));
extern float path_walker_gap_range_end_gp __asm__("D_L16_00161E6C") __attribute__((sda));
extern float path_walker_speed_range_start_gp __asm__("D_L16_00161E70") __attribute__((sda));
extern float path_walker_speed_range_end_gp __asm__("D_L16_00161E74") __attribute__((sda));
extern float path_walker_speed_approach_acceleration_gp __asm__("D_L16_00161E78")
    __attribute__((sda));
extern float path_walker_height_range_start_gp __asm__("D_L16_00161E7C") __attribute__((sda));
extern float path_walker_height_range_end_gp __asm__("D_L16_00161E80") __attribute__((sda));
extern float path_walker_reposition_advance_rate_gp __asm__("D_L16_00161E8C") __attribute__((sda));
extern int   path_walker_draw_distance_gp __asm__("D_L16_00161E90") __attribute__((sda));
extern int   allocate_voice_for_target_entry_alt(int, int, int) __asm__("FUN_0022da68");
void         emit_path_walker_particle(unsigned char *source_moby,
                                       void          *previous_position) __asm__("FUN_L16_002e7a30");

extern void emit_path_walker_particle(unsigned char *, void *) __asm__("FUN_L16_002e7a30");
void        initialize_path_walker_group(struct Moby *moby) __asm__("FUN_L16_002e7ba0");

extern void initialize_path_walker_group(struct Moby *) __asm__("FUN_L16_002e7ba0");
extern void sample_camera_path(void *, f32, s32, void *, void *, s32) __asm__("func_00214E58");

void update_path_walker(unsigned char *walker_moby) __asm__("FUN_L16_002e76b8");

void update_path_walker(unsigned char *walker_moby) {
    enum {
        PATH_WALKER_INITIALIZE_GROUP = 0,
        PATH_WALKER_FOLLOW_PATH = 1
    };
    float            old[4], rotation[4], trial[4];
    char            *path;
    L16WalkerData_c *walker_data;
    int              playing = 0;

    walker_data = ((L16WalkerData_c *)MOBY(walker_moby)->pvars);
    path = D_L16_001B0930[walker_data->path_index];
    if (((((unsigned short)MOBY(walker_moby)->oclass) ^ 1) & 1) != 0) {
        if (voice_is_playing(walker_moby, walker_data->sound) == 0) {
            walker_data->sound = allocate_voice_for_target_entry_alt(0, 4, (int)walker_moby);
        } else {
            playing = 1;
        }
    } else {
        qcopy(old, walker_moby + 0x10);
    }
    switch (walker_moby[0x20]) {
    case PATH_WALKER_INITIALIZE_GROUP:
        if (walker_moby[0x21] == 255) {
            mark_moby_for_removal(walker_moby);
            return;
        }
        initialize_path_walker_group(walker_moby);
        break;
    case PATH_WALKER_FOLLOW_PATH: {
        L16WalkerData_c *other = *(L16WalkerData_c **)(walker_data->owner + 0x78);
        float            gap, step, next, height, acceleration;
        /* `playing` again, in a variable of case 1's own. The logic does not need it: tested directly,
           `playing` is one value from the top of the function to the test, the compiler ranks it below
           `path` and gives them $s5 and $s4, the reverse of retail. The narrowing copy ends that value
           here and starts another; each ranks above `path`, both get $s4 and the copy itself leaves no
           instruction. A same-width copy (int) is folded away and does not do it. */
        char blocked = playing;

        sample_camera_path(path,
                           walker_data->parameter,
                           0,
                           walker_moby + 0x10,
                           (float *)&MOBY(walker_moby)->rot,
                           0);
        acceleration = path_walker_speed_approach_acceleration_gp * frame_time_sq;
        MOBY(walker_moby)->pos.z += walker_data->height_step;
        walker_data->parameter += walker_data->speed;
        approach_value(&walker_data->speed, walker_data->target_speed, acceleration);
        gap = other->parameter - walker_data->parameter;
        if (gap < 0.0f) {
            gap += walker_data->period;
        }
        if (gap < path_walker_gap_range_start_gp &&
            walker_data->target_speed > other->target_speed) {
            float saved = walker_data->target_speed;
            float theirs = other->target_speed;

            other->target_speed = saved;
            walker_data->target_speed = theirs;
        }
        if (gap < 1.5f) {
            step = frame_time_sq * 50.0f;
            approach_value(&walker_data->speed, walker_data->target_speed, step);
            approach_value(&other->speed, other->target_speed, step);
        }
        if (walker_moby[0x31] == 0 && blocked == 0) {
            if (gap > path_walker_gap_range_end_gp + 1.0f) {
                height = random_float_between(path_walker_height_range_start_gp,
                                              path_walker_height_range_end_gp);
                if (walker_data->owner[0x31] != 0 && walker_data->countdown != 0) {
                    next = other->parameter - random_float_between(path_walker_gap_range_start_gp,
                                                                   path_walker_gap_range_end_gp);
                    walker_data->countdown--;
                } else {
                    next = walker_data->parameter +
                           path_walker_reposition_advance_rate_gp * frame_time;
                }
                if (next < 0.0f) {
                    next += walker_data->period;
                }
                sample_camera_path(path, next, 0, trial, rotation, 1);
                trial[2] += height;
                trial[3] = 2.0f;
                if (fast_bsphere_check((char *)trial, (float)path_walker_draw_distance_gp) == -1) {
                    float random = random_float_between(path_walker_speed_range_start_gp,
                                                        path_walker_speed_range_end_gp);

                    walker_data->parameter = next;
                    walker_data->height_step = height;
                    walker_data->target_speed = random * frame_time;
                }
            }
        } else {
            walker_data->countdown = 30;
        }
        if (walker_data->parameter > walker_data->period) {
            walker_data->parameter -= walker_data->period;
        }
        break;
    }
    }
    if (((unsigned short)MOBY(walker_moby)->oclass) & 1) {
        emit_path_walker_particle(walker_moby, old);
    }
}
/* Emits a particle near an active moby when the target is close. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002E8EA8), where it is exact; names translated to the US level program. */

void spawn_effect_particle_with_target_scalar(
    OvlQuad *p, OvlQuad *q, int c, int d, int n, float s) __asm__("FUN_L00_0026ced0");
void build_spherical_offset_u(f32 *out, f32 scale, f32 a, f32 b) __asm__("FUN_00214db0");
extern void
func_L00_0026DD70_emit(void *, void *, int, int, float, int) __asm__("FUN_L00_0026ced0");

int          D_L16_00161E98 = 0x60d0d080;
int          D_L16_00161E9C = 0x801000;
extern float path_walker_particle_movement_velocity_scale_gp __asm__("D_L16_00161EA0")
    __attribute__((sda));
extern float path_walker_particle_random_velocity_scale_gp __asm__("D_L16_00161EA4")
    __attribute__((sda));
extern float path_walker_particle_target_scalar_scale_gp __asm__("D_L16_00161EA8")
    __attribute__((sda));

void emit_path_walker_particle(unsigned char *source_moby, void *previous_position) {
    float input[4];
    float offset[4];
    float out[4];
    float velocity[4];
    float random_offset_angle, second_random_offset_angle, size, movement_distance, final_size;
    *(OvlQuad *)input = *(OvlQuad *)previous_position;
    if ((unsigned char)source_moby[0x31] == 0)
        return;
    if ((D_L16_0015F5CC & 1) != (((int)source_moby >> 8) & 1))
        return;
    if (!(fast_vec_dist(source_moby + 0x10, D_L16_001671C0) < 75.0f))
        return;
    movement_distance = fast_vec_dist(source_moby + 0x10, input);
    fast_vec_scale(out,
                   source_moby + 0xC0,
                   path_walker_particle_movement_velocity_scale_gp * movement_distance);
    size = path_walker_particle_random_velocity_scale_gp;
    size *= frame_time;
    fast_vec_normalize(offset, source_moby + 0xC0, -0.5f);
    fast_vec_add(offset, offset, source_moby + 0x10);
    random_offset_angle = random_angle_radians();
    second_random_offset_angle = random_angle_radians();
    build_spherical_offset_u(velocity, size, random_offset_angle, second_random_offset_angle);
    fast_vec_add(velocity, velocity, out);
    final_size = path_walker_particle_target_scalar_scale_gp;
    final_size *= 210000.0f;
    func_L00_0026DD70_emit(
        offset, velocity, D_L16_00161E98, D_L16_00161E9C, final_size, scale_game_frames(0x23));
}

/* Start of a walker group: lists the group's mobys and spaces them along the path in random order. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002E9018), where it is exact; names translated to the US level program. */

typedef struct {
    int            path_index;
    float          parameter;
    float          speed;
    float          period;
    unsigned char *owner;
    float          height_step;
    float          target_speed;
    short          countdown;
    short          sound;
} L16WalkerData;

extern char *D_L16_0015FFD8;
extern float path_walker_initial_scale_multiplier __asm__("D_L16_00161E94")
    __attribute__((sda)); /* no foreign declaration */
extern short *D_L16_001ABCC0[];

void initialize_path_walker_group(struct Moby *moby) {
    unsigned char *objects[16];
    unsigned char *previous;
    L16WalkerData *walker_data = (L16WalkerData *)moby->pvars;
    short         *list = D_L16_001ABCC0[moby->group];
    short          count = 0, rank;
    int            i;
    float          period;

    if (list == 0 || walker_data->path_index == -1) {
        return;
    }
    period = (float)*(int *)D_L16_001B0930[walker_data->path_index] - 1.0f;
    do {
        objects[count] =
            (unsigned char *)(D_L16_0015FFD8 + ((*(unsigned short *)list & 0x7FFF) << 8));
        count++;
    } while (*list++ >= 0);
    previous = objects[count - 1];
    rank = count;
    for (i = 0; i < count; i++) {
        unsigned char *current;
        L16WalkerData *data;
        unsigned short flags;

        if (i == count - 1) {
            /* the last one takes the slot the loop index is on */
            current = objects[i];
        } else {
            /* the others take a random slot, or the next one still in use after it */
            int selected = random_integer_below(count - 1);

            for (;;) {
                if (objects[selected] != 0) {
                    current = objects[selected];
                    objects[selected] = 0;
                    break;
                }
                selected = (selected + 1) % (count - 1);
            }
        }
        flags = path_walker_draw_distance_gp;
        current[0x30] = 255;
        *(float *)(current + 0x2C) *= path_walker_initial_scale_multiplier;
        *(int *)(current + 0x94) = 0;
        *(unsigned short *)(current + 0x32) = flags;
        data = *(L16WalkerData **)(current + 0x78);
        data->parameter = (float)rank * (period / convert_integer_to_float(count));
        data->owner = previous;
        data->period = period;
        previous = current;
        data->height_step = random_float_between(path_walker_height_range_start_gp,
                                                 path_walker_height_range_end_gp);
        data->speed =
            random_float_between(path_walker_speed_range_start_gp, path_walker_speed_range_end_gp) *
            frame_time;
        data->target_speed = data->speed;
        current[0x20] = 1;
        rank--;
    }
}

/* Waits one tick, then pushes two pairs of the player's points apart along their difference. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002E9278), where it is exact; names translated to the US level program. */

typedef float W[4] __attribute__((aligned(16)));

extern char paired_joint_trail_source_context[] __asm__("D_L16_0016CC60");
extern int  D_L16_0015F5C4;

void update_paired_joint_particle_trails(struct Moby *moby) __asm__("FUN_L16_002e7e00");

void update_paired_joint_particle_trails(struct Moby *moby) {
    enum {
        JOINT_TRAIL_INITIALIZE = 0,
        JOINT_TRAIL_EMIT = 1
    };
    W joint_6_position, joint_7_position, joint_4_position, joint_5_position,
        joint_6_trail_velocity, joint_7_trail_velocity;
    float k1 = 0.075f, k2 = 20000.0f, k3 = 1000.0f;
    float j1 = 0.075f, j2 = 20000.0f, j3 = 1000.0f;
    switch (moby->state) {
    case JOINT_TRAIL_INITIALIZE:
        moby->update_dist = 0xFF;
        moby->state = 1;
        break;
    case JOINT_TRAIL_EMIT:
        if (D_L16_0015F5C4 == 2) {
            char *trail_source_context = paired_joint_trail_source_context;
            if (*(int *)(trail_source_context + 0x30) == 0) {
                char *joint_source_moby = *(char **)(trail_source_context + 0x184);
                moby_point_position(joint_source_moby, 6, joint_6_position);
                moby_point_position(joint_source_moby, 4, joint_4_position);
                fast_vec_sub(joint_6_trail_velocity, joint_6_position, joint_4_position);
                fast_vec_normalize(joint_6_trail_velocity, joint_6_trail_velocity, k1);
                draw_trail(joint_6_position, joint_6_position, joint_6_trail_velocity, k2, k3);
                moby_point_position(joint_source_moby, 7, joint_7_position);
                moby_point_position(joint_source_moby, 5, joint_5_position);
                fast_vec_sub(joint_7_trail_velocity, joint_7_position, joint_5_position);
                fast_vec_normalize(joint_7_trail_velocity, joint_7_trail_velocity, j1);
                draw_trail(joint_7_position, joint_7_position, joint_7_trail_velocity, j2, j3);
            }
        }
        break;
    }
}

#include "qcopy.h"

/* Moves the platform between two heights above its joint, with a particle while it moves. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002E93F8), where it is exact; names translated to the US level program. */

typedef union {
    OvlQuad q;
    float   v[4];
} L16PlatformVector;

typedef struct {
    char  pad00[0x60];
    float basis[4][4];
    int   joint;
    int   padA4;
    float goal;
    int   padAC;
    float speed;
    int   particle;
} L16PlatformData;

typedef struct {
    char             pad00[0x10];
    float            position[4];
    unsigned char    state;
    char             pad21[0x1F];
    float            rotation[4];
    char             pad50[0x28];
    L16PlatformData *data;
    char             pad7C[0x40];
    unsigned char    reverse;
} L16PlatformMoby;

typedef struct {
    char             pad00[0x80];
    float            position[4];
    char             pad90[0x26C];
    L16PlatformMoby *contact;
    char             pad300[0xE];
    short            grounded;
} L16PlatformHero;

extern char *elevator_reference_transform_table __asm__("D_L16_001600FC");
extern float elevator_height_offset __asm__("D_L16_00161EAC")
    __attribute__((sda)); /* no foreign declaration */
extern float elevator_height_acceleration __asm__("D_L16_00161EBC")
    __attribute__((sda)); /* no foreign declaration */
extern f32 D_L16_00161EB8_gp __asm__("D_L16_00161EB8") __attribute__((sda));
void       draw_elevator_rotating_rings(struct Moby *elevator_moby) __asm__("FUN_L16_002e84e8");

extern void draw_elevator_rotating_rings(struct Moby *) __asm__("FUN_L16_002e84e8");

void elevator_update(L16PlatformMoby *moby) __asm__("FUN_L16_002e7f80");

void elevator_update(L16PlatformMoby *moby) {
    enum {
        ELEVATOR_INITIALIZE = 0,
        ELEVATOR_WAIT_FOR_HERO = 2,
        ELEVATOR_UPDATE_HEIGHT = 3
    };
    L16PlatformVector delta, old_rotation, motion;
    L16PlatformData  *elevator_data = moby->data;
    float            *position = moby->position;
    float            *rotation = moby->rotation;

    fast_vec_scale(delta.v, position, -1.0f);
    qcopy(old_rotation.v, rotation);
    if (elevator_data) {
        switch (moby->state) {
        case ELEVATOR_INITIALIZE:
            motion.q = 0;
            motion.v[2] = 1.0f;
            elevator_data->particle = -1;
            transform_vector_by_basis(motion.v,
                                      motion.v,
                                      elevator_reference_transform_table +
                                          (elevator_data->joint << 7));
            elevator_data->goal = motion.v[2] + *(float *)(elevator_reference_transform_table +
                                                           elevator_data->joint * 0x80 + 0x38);
            moby->position[2] = motion.v[2] +
                                *(float *)(elevator_reference_transform_table +
                                           elevator_data->joint * 0x80 + 0x38) +
                                elevator_height_offset;
            moby->reverse = 1;
            moby->state = 2;
            break;
        case ELEVATOR_WAIT_FOR_HERO:
            if (voice_is_playing(moby, elevator_data->particle)) {
                release_voice_slot(elevator_data->particle);
                elevator_data->particle = -1;
            }
            if (vector_distance_xy(position, hero.motion.pos.f) < 20.0f) {
                clear_u64_value(motion.v);
                if (moby->reverse) {
                    motion.v[2] = -1.0f;
                } else {
                    motion.v[2] = 1.0f;
                }
                transform_vector(motion.v,
                                 motion.v,
                                 elevator_reference_transform_table + (elevator_data->joint << 7));
                motion.v[2] = motion.v[2] +
                              *(float *)(elevator_reference_transform_table +
                                         elevator_data->joint * 0x80 + 0x38) +
                              elevator_height_offset;
                if (absolute_float(hero.motion.pos.f[2] - motion.v[2]) < 1.0f) {
                    moby->reverse = (moby->reverse + 1) & 1;
                    elevator_data->goal = motion.v[2];
                    elevator_data->speed = 0.0f;
                    moby->state = 3;
                    break;
                }
            }
            if (hero.ground_moby == moby && hero.air_frames.s == 0) {
                clear_u64_value(motion.v);
                if (moby->reverse) {
                    motion.v[2] = -1.0f;
                } else {
                    motion.v[2] = 1.0f;
                }
                moby->reverse = (moby->reverse + 1) & 1;
                transform_vector(motion.v,
                                 motion.v,
                                 elevator_reference_transform_table + (elevator_data->joint << 7));
                elevator_data->goal = motion.v[2] +
                                      *(float *)(elevator_reference_transform_table +
                                                 elevator_data->joint * 0x80 + 0x38) +
                                      elevator_height_offset;
                elevator_data->speed = 0.0f;
                moby->state = 3;
            }
            break;
        case ELEVATOR_UPDATE_HEIGHT: {
            float *hero_position = hero.motion.pos.f;
            float  speed;

            if (vector_distance_xy(hero_position,
                                   elevator_reference_transform_table +
                                       (elevator_data->joint << 7) + 0x30) > 2.25f ||
                absolute_float(moby->position[2] - hero_position[2] - 2.0f) > 1.0f ||
                moby->position[2] < elevator_data->goal) {
                speed = advance_accelerated_scalar(&moby->position[2],
                                                   &elevator_data->speed,
                                                   elevator_data->goal,
                                                   elevator_height_acceleration * frame_time_sq,
                                                   elevator_height_acceleration * frame_time_sq,
                                                   *(float *)&D_L16_00161EB8_gp * frame_time);
            } else {
                speed =
                    advance_accelerated_scalar(&moby->position[2],
                                               &elevator_data->speed,
                                               moby->position[2],
                                               elevator_height_acceleration * 4.0f * frame_time_sq,
                                               elevator_height_acceleration * 4.0f * frame_time_sq,
                                               *(float *)&D_L16_00161EB8_gp * frame_time);
            }
            clear_u64_value(motion.v);
            motion.v[2] = speed;
            update_entity_matrix(
                (char *)elevator_data->basis, motion.v, moby->rotation, moby->rotation);
            if (motion.v[2] != 0.0f) {
                if (!voice_is_playing(moby, elevator_data->particle)) {
                    elevator_data->particle = allocate_voice_for_target_entry_alt(0, 4, (int)moby);
                }
            } else if (voice_is_playing(moby, elevator_data->particle)) {
                release_voice_slot(elevator_data->particle);
                elevator_data->particle = -1;
            }
            if (moby->position[2] == elevator_data->goal &&
                (hero.ground_moby != moby || hero.air_frames.s != 0)) {
                clear_u64_value(elevator_data->basis[1]);
                moby->state = 2;
                break;
            }
            if (elevator_data->speed != 0.0f) {
                add_draw_callback(draw_elevator_rotating_rings, moby);
            }
            break;
        }
        }
        fast_vec_add(delta.v, delta.v, moby->position);
        update_entity_matrix((char *)elevator_data->basis, delta.v, old_rotation.v, moby->rotation);
    }
}

#include "qcopy.h"

/* Draw stacked alternating rings with a rotating transformation and interpolated tint. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002E9960), where it is exact; names translated to the US level program. */

typedef float L16RingVector[4] __attribute__((aligned(16)));

typedef struct {
    L16RingVector position[4];
    int           color[4];
    struct {
        float u, v;
    } uv[4];
    long zero, texture, flags, mode;
} L16RingPacket;

typedef struct {
    L16RingVector basis[3];
    L16RingVector position;
} L16RingMatrix;

extern float elevator_ring_quad_uvs[][2] __asm__("D_L16_001D9880");
extern float fast_cos(float);
extern float fast_sin(float);
extern float D_L16_00161EB8_gp __asm__("D_L16_00161EB8") __attribute__((sda));
extern float elevator_ring_outer_radius_gp __asm__("D_L16_00161EC4") __attribute__((sda));
extern float elevator_ring_half_height_gp __asm__("D_L16_00161EC8") __attribute__((sda));
extern float elevator_ring_segment_angle_degrees_gp __asm__("D_L16_00161EE8") __attribute__((sda));
int          D_L16_00161ECC = 0;
int          D_L16_00161ED0 = 2;
int          D_L16_00161ED4 = 0;
int          D_L16_00161ED8 = 1;
int          D_L16_00161EDC = 0x408000;
int          D_L16_00161EE0 = 0x40c08030;
extern int   fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern int   get_effect_texture(int) __asm__("FUN_001f44b8");
extern void  draw_geometry_quad(void *, void *, int);

void draw_elevator_rotating_rings(struct Moby *elevator_moby) {
    L16RingPacket packet;
    L16RingMatrix matrix;
    char         *elevator_data = (char *)elevator_moby->pvars;
    int           bound_index = (*(int *)((char *)elevator_data + 0xA0)) << 7;
    char         *entry = (char *)(bound_index + (int)elevator_reference_transform_table);
    float         translation = (*(float *)((char *)elevator_data + 0xB8));
    float         lower = (*(float *)((char *)entry + 0x38));
    float         half = (*(float *)((char *)entry + 0x28));
    float         upper = lower + half + translation;
    int           color;
    float(*uv)[2];
    L16RingVector *point;
    float         *v, *u;
    L16RingMatrix *transform;
    float          radius, index, phase;
    int            i, j, sign, period;
    lower = lower - half;
    lower += translation;
    color = fast_tween_color(D_L16_00161EDC,
                             D_L16_00161EE0,
                             absolute_float((*(float *)((char *)elevator_data + 0xB0))) /
                                 (D_L16_00161EB8_gp * frame_time));
    copy_matrix_rotation(&matrix, &elevator_moby->rot_row0);
    qcopy(matrix.position, &elevator_moby->pos);
    matrix.position[2] = lower;
    matrix.position[3] = 1.0f;
    packet.texture = get_effect_texture(14);
    packet.flags = 0xFF9000000260L;
    packet.mode = (long)D_L16_00161ECC | ((long)D_L16_00161ED0 << 2) | ((long)D_L16_00161ED4 << 4) |
                  ((long)D_L16_00161ED8 << 6) | 0x8000000000L;
    packet.zero = 0;
    uv = elevator_ring_quad_uvs;
    point = packet.position;
    v = &packet.uv[0].v;
    u = &packet.uv[0].u;
    for (i = 0; i < 4; i++) {
        *u = (*uv)[0];
        *v = (*uv)[1];
        if (i & 1) {
            (*point)[2] = elevator_ring_half_height_gp;
            radius = elevator_ring_outer_radius_gp;
        } else {
            (*point)[2] = -elevator_ring_half_height_gp;
            radius = elevator_ring_outer_radius_gp - 0.05f;
        }
        index = (float)(i >> 1);
        (*point)[0] =
            fast_cos(index * (elevator_ring_segment_angle_degrees_gp * DEG_TO_RAD)) * radius;
        (*point)[1] =
            fast_sin(index * (elevator_ring_segment_angle_degrees_gp * DEG_TO_RAD)) * radius;
        (*point)[3] = 1.0f;
        v += 2;
        u += 2;
        uv++;
        point++;
    }
    transform = &matrix;
    sign = 1;
    if (matrix.position[2] < upper) {
        period = 120;
        do {
            phase = (float)(D_L16_0015F5CC % period) * 0.02617991715669632f;
            sign = -sign;
            for (j = 0; (float)j < 360.0f / elevator_ring_segment_angle_degrees_gp; j++) {
                matrix.basis[0][0] = fast_cos(fast_normalize_angle(
                    (elevator_ring_segment_angle_degrees_gp * DEG_TO_RAD) * (float)j +
                    phase * (float)sign));
                matrix.basis[0][1] = fast_sin(fast_normalize_angle(
                    (elevator_ring_segment_angle_degrees_gp * DEG_TO_RAD) * (float)j +
                    phase * (float)sign));
                matrix.basis[0][2] = 0.0f;
                matrix.basis[1][0] = fast_cos(fast_normalize_angle(
                    (elevator_ring_segment_angle_degrees_gp * DEG_TO_RAD) * (float)j + 1.5707964f +
                    phase * (float)sign));
                matrix.basis[1][1] = fast_sin(fast_normalize_angle(
                    (elevator_ring_segment_angle_degrees_gp * DEG_TO_RAD) * (float)j + 1.5707964f +
                    phase * (float)sign));
                matrix.basis[1][2] = 0.0f;
                matrix.basis[2][2] = 1.0f;
                packet.color[3] = color;
                packet.color[2] = color;
                packet.color[1] = color;
                packet.color[0] = color;
                draw_geometry_quad(&packet, transform, 0);
            }
            matrix.position[2] += (*(float *)((char *)elevator_data + 0xBC));
        } while (matrix.position[2] < upper);
    }
}
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002E9D48), where it is exact; names translated to the US level program. */

void gadgetron_sign_update(struct Moby *moby) __asm__("FUN_L16_002e88d0");

void gadgetron_sign_update(struct Moby *moby) {
    enum {
        GADGETRON_SIGN_INITIALIZE = 0,
        GADGETRON_SIGN_ROTATE = 1
    };
    switch (moby->state) {
    case GADGETRON_SIGN_INITIALIZE: {
        float size = moby->scale * 4.0f;
        int   ff = 0xFF;
        int   twohundred = 0x200;
        moby->state = 1;
        moby->update_dist = ff;
        moby->draw_dist = twohundred;
        moby->pos.z = 160.0f;
        moby->scale = size;
    } break;
    case GADGETRON_SIGN_ROTATE:
        moby->rot.z = fast_add_rotations(moby->rot.z, frame_time * -0.2617993950843811f);
        break;
    }
}

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002E9DE8), where it is exact; names translated to the US level program. */

char *find_group_class_010e_moby_in_state_6(int moby_group_index) __asm__("FUN_L16_002e8b08");

char       *find_group_class_010e_moby_in_state_6(int idx) __asm__("FUN_L16_002e8b08");
extern int  get_target_reposition_status(void *) __asm__("FUN_L16_002e0e28");
extern void reposition_moby_with_collision_check(void *, void *) __asm__("FUN_L16_002c44f0");
extern void request_target_reposition(void *) __asm__("FUN_L16_002e0de0");
f32         probe_ground_height(void *arg0, s32 arg1, f32 arg2) __asm__("FUN_00213508");
s32         is_point_inside_clip_volume_u(s32 arg0, s32 arg1) __asm__("FUN_00214720");

void update_target_reposition_cycle(struct Moby *moby) __asm__("FUN_L16_002e8970");

void update_target_reposition_cycle(struct Moby *moby) {
    enum {
        TARGET_CYCLE_INITIALIZE_DELAY = 0,
        TARGET_CYCLE_WAIT_TO_TRIGGER = 1,
        TARGET_CYCLE_POLL_STATUS = 2
    };
    enum {
        TARGET_STATUS_INACTIVE = 0,
        TARGET_STATUS_REPOSITION_READY = 2
    };
    char *hero_position = (char *)&hero.motion.pos;
    char *data = (char *)moby->pvars;
    char *target;
    int   target_status;
    float pos[4];
    if (!is_point_inside_clip_volume_u(hero_position, *(int *)(data + 8)))
        return;
    target = D_L16_0015FFD8 + (*(int *)(data + 4) << 8);
    switch (moby->state) {
    case TARGET_CYCLE_INITIALIZE_DELAY:
        moby->state = 1;
        *(int *)(data + 0xC) = scale_game_frames(random_int_between(0xB4, 0x168));
        break;
    case TARGET_CYCLE_WAIT_TO_TRIGGER:
        if (vector_distance_xy(hero_position, target + 0x10) > 3.0f) {
            if (find_group_class_010e_moby_in_state_6(*(int *)data) != 0) {
                if (fast_dec_timer(data + 0xC)) {
                    moby->state = 2;
                    request_target_reposition(target);
                    *(int *)(data + 0xC) = scale_game_frames(random_int_between(0xB4, 0x168));
                }
            }
        }
        break;
    case TARGET_CYCLE_POLL_STATUS:
        target_status = get_target_reposition_status(target);
        switch (target_status) {
        case TARGET_STATUS_INACTIVE:
            moby->state = 1;
            break;
        case TARGET_STATUS_REPOSITION_READY:
            moby->state = 1;
            {
                char *other = find_group_class_010e_moby_in_state_6(*(int *)data);
                if (other != 0) {
                    qcopy(pos, target + 0x10);
                    pos[2] += 1.0f;
                    pos[2] = probe_ground_height(pos, 0, 0.5f);
                    reposition_moby_with_collision_check(other, pos);
                }
            }
            break;
        }
        break;
    }
}

/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002E9F80), where it is exact; names translated to the US level program. */

extern int is_moby_in_state_6(void *) __asm__("FUN_L16_002c4710");

char *find_group_class_010e_moby_in_state_6(int moby_group_index) {
    short *moby_index_entry = D_L16_001ABCC0[moby_group_index];
    char  *moby;
    if (moby_index_entry == 0)
        return 0;
    for (;;) {
        moby = D_L16_0015FFD8 + ((*(unsigned short *)moby_index_entry & 0x7FFF) << 8);
        if (MOBY(moby)->oclass == 0x10E && is_moby_in_state_6(moby))
            return moby;
        if (*moby_index_entry++ < 0)
            return 0;
    }
}
/* For each of n point pairs, reflects a direction and writes its 2D map coordinates. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002EA010), where it is exact; names translated to the US level program. */

typedef struct {
    float x, y, z;
} Vec3f;

void compute_level16_reflection_map_coordinates(int    point_count,
                                                Vec3f *direction_vectors,
                                                Vec3f *point_positions,
                                                int    unused,
                                                float *out) __asm__("FUN_L16_002e8b98");

void compute_level16_reflection_map_coordinates(

    int point_count, Vec3f *direction_vectors, Vec3f *point_positions, int unused, float *out) {
    int   i;
    float relative_direction[4];
    float point_position[4];
    float reflection_projection_scratch[4];
    float reflection_direction_scratch[4];

    for (i = 0; i < point_count; i++) {
        float direction_dot_product;
        float projection_denominator;

        point_position[3] = 1.0f;
        point_position[0] = point_positions[i].x;
        point_position[1] = point_positions[i].y;
        point_position[2] = point_positions[i].z;
        fast_vec_sub(relative_direction, point_position, D_L16_001671C0);
        fast_vec_normalize(relative_direction, relative_direction, 1.0f);
        reflection_direction_scratch[3] = 1.0f;
        reflection_direction_scratch[0] = direction_vectors[i].x;
        reflection_direction_scratch[1] = direction_vectors[i].y;
        reflection_direction_scratch[2] = direction_vectors[i].z;
        fast_vec_normalize(reflection_direction_scratch, reflection_direction_scratch, 1.0f);
        direction_dot_product = fast_vec_dot(reflection_direction_scratch, relative_direction);
        fast_vec_scale(reflection_projection_scratch,
                       reflection_direction_scratch,
                       direction_dot_product + direction_dot_product);
        fast_vec_sub(
            reflection_projection_scratch, relative_direction, reflection_projection_scratch);
        fast_vec_normalize(reflection_projection_scratch, reflection_projection_scratch, 1.0f);
        reflection_projection_scratch[2] = reflection_projection_scratch[2] + 1.0f;
        projection_denominator = fast_vec_length(reflection_projection_scratch);
        projection_denominator = projection_denominator + projection_denominator;
        out[i * 2] = (reflection_projection_scratch[0] / projection_denominator + 0.5f) * 2.0f;
        out[i * 2 + 1] = (reflection_projection_scratch[1] / projection_denominator + 0.5f) * 2.0f;
    }
}
/* Sets up the draw state, then draws the three entries of the level's table. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002EA1B8), where it is exact; names translated to the US level program. */

extern char D_L16_001DC600[][16] NOT_SDA;
extern char D_L16_001DC630[] NOT_SDA;
int         D_L16_00161EF0_gp[4] __asm__("D_L16_00161EF0")
    __attribute__((section(".sdata"))) = {148, 148, 25, 0};

extern float D_L16_001D98D0[];
extern float D_L16_001D9FC0[];
extern float D_L16_001DA6B0[];
float       *D_L16_00161F00_gp[4] __asm__("D_L16_00161F00")
    __attribute__((section(".sdata"))) = {D_L16_001D98D0, D_L16_001D9FC0, D_L16_001DA6B0, 0};

extern float D_L16_001DA7E0[];
extern float D_L16_001DAC80[];
extern float D_L16_001DB120[];
float       *D_L16_00161F10[4]
    __attribute__((section(".sdata"))) = {D_L16_001DA7E0, D_L16_001DAC80, D_L16_001DB120, 0};

extern float D_L16_001DB1E8[];
extern float D_L16_001DB8D8[];
extern float D_L16_001DBFC8[];
float       *D_L16_00161F20[4]
    __attribute__((section(".sdata"))) = {D_L16_001DB1E8, D_L16_001DB8D8, D_L16_001DBFC8, 0};

extern float D_L16_001DC0F8[];
extern float D_L16_001DC348[];
extern float D_L16_001DC598[];
float       *D_L16_00161F30[4]
    __attribute__((section(".sdata"))) = {D_L16_001DC0F8, D_L16_001DC348, D_L16_001DC598, 0};

extern void FUN_L00_001fde98(int, int, int, char *, int);
extern void FUN_L16_002e8b98_i(int, int, int, int, char *) __asm__("FUN_L16_002e8b98");
extern void vu1_add_g_sregister_l16(int, long) __asm__("FUN_00233980");

void env_map_a_draw(void) __asm__("FUN_L16_002e8d40");

void env_map_a_draw(void) {
    int *mesh_point_counts = (int *)D_L16_00161EF0_gp;
    int *mesh_point_positions = (int *)D_L16_00161F00_gp;
    int *mesh_unused_reflection_arguments = (int *)D_L16_00161F10;
    int *mesh_direction_vectors = (int *)D_L16_00161F20;
    int *mesh_point_word_streams = (int *)D_L16_00161F30;
    int  i;

    vu1_add_g_sregister_l16(6, get_effect_texture(0x29));
    vu1_add_g_sregister_l16(0x42, 0x4000000064);
    vu1_add_g_sregister_l16(8, 0);
    vu1_add_g_sregister_l16(0x14, 0xFF9000000260);
    font_queue_vu_state();
    for (i = 0; i < 3; i++) {
        if (fast_bsphere_check(D_L16_001DC600[i], 512.0f) != -1) {
            FUN_L16_002e8b98_i(mesh_point_counts[i],
                               mesh_direction_vectors[i],
                               mesh_point_positions[i],
                               mesh_unused_reflection_arguments[i],
                               D_L16_001DC630);
            FUN_L00_001fde98(mesh_point_counts[i],
                             mesh_point_positions[i],
                             mesh_point_word_streams[i],
                             D_L16_001DC630,
                             1);
        }
    }
}

/* Bounding spheres for one mesh (three entries), then the renderer callback; same source as FUN_L16_002e93b0 without the 0x30/0x32 stores. */
typedef struct {
    float x, y, z, radius;
} L16SingleSphere_2e8e80;

extern int                    D_L16_00161EF0_gp[] __asm__("D_L16_00161EF0") __attribute__((sda));
extern float                 *D_L16_00161F00_gp[] __asm__("D_L16_00161F00") __attribute__((sda));
extern L16SingleSphere_2e8e80 D_L16_00161F60_bounds_2e8e80[] __asm__("D_L16_001DC600")
    __attribute__((section(".data")));

extern void FUN_L16_002e8d40_c(void *) __asm__("FUN_L16_002e8d40");
extern void draw_level16_visible_textured_batches(void) __asm__("FUN_L16_002e97a0");
void        update_level16_three_mesh_render_state(struct Moby *moby) __asm__("FUN_L16_002e8e80");

void update_level16_three_mesh_render_state(struct Moby *moby) {
    enum {
        MESH_RENDER_INITIALIZE_BOUNDING_SPHERES = 0,
        MESH_RENDER_QUEUE_CALLBACK = 1
    };
    int    i, j;
    float  maxx, maxy, maxz, minx, miny, minz;
    float  vertex_position[4];
    float *out;
    float *verts;
    int    count;
    switch (moby->state) {
    case MESH_RENDER_INITIALIZE_BOUNDING_SPHERES: {
        for (i = 0; i <= 2; i++) {
            maxx = maxy = maxz = -1024.0f;
            minx = miny = minz = 1024.0f;
            {
                int k;
                for (k = 0; k < D_L16_00161EF0_gp[i]; k++) {
                    float x, y, z;
                    verts = ((float **)D_L16_00161F00_gp)[i] + k * 3;
                    x = verts[0];
                    if (x > maxx)
                        maxx = x;
                    y = verts[1];
                    if (y > maxy)
                        maxy = y;
                    z = verts[2];
                    if (z > maxz)
                        maxz = z;
                    if (x < minx)
                        minx = x;
                    if (y < miny)
                        miny = y;
                    if (z < minz)
                        minz = z;
                }
            }
            {
                L16SingleSphere_2e8e80 *base = D_L16_00161F60_bounds_2e8e80;
                L16SingleSphere_2e8e80 *sphere = base + i;
                sphere->radius = 0.0f;
                sphere->x = (maxx + minx) * 0.5f;
                sphere->y = (maxy + miny) * 0.5f;
                sphere->z = (maxz + minz) * 0.5f;
            }
            for (j = 0; j < D_L16_00161EF0_gp[i]; j++) {
                float vertex_to_center_distance;
                out = (float *)(D_L16_00161F60_bounds_2e8e80 + i);
                vertex_position[0] = ((float **)D_L16_00161F00_gp)[i][j * 3 + 0];
                vertex_position[1] = ((float **)D_L16_00161F00_gp)[i][j * 3 + 1];
                vertex_position[2] = ((float **)D_L16_00161F00_gp)[i][j * 3 + 2];
                vertex_to_center_distance = fast_vec_dist(vertex_position, out);
                if (vertex_to_center_distance > out[3])
                    out[3] = vertex_to_center_distance;
            }
        }
        moby->state = 1;
        break;
    }
    case MESH_RENDER_QUEUE_CALLBACK:
        add_draw_callback(FUN_L16_002e8d40_c, moby);
        break;
    }
}
/* Sets up the draw state, then draws the one entry of the level's table; same source as env_map_a_draw. */

typedef struct {
    float x, y, z, radius;
} L16SingleSphere;

extern L16SingleSphere l16_single_reflection_mesh_bounds[] __asm__("D_L16_00161F80") MACRO_ADDR;
extern void            FUN_L16_002e8b98_t(int, int, int, int, char *) __asm__("FUN_L16_002e8b98");
extern void            vu1_add_g_sregister_2e9270(int, long) __asm__("FUN_00233980");

extern char D_L16_001DDC80[] NOT_SDA;
int         D_L16_00161F40[2] = {125, 0};

extern float D_L16_001DCAE0[];
float       *D_L16_00161F60[2] = {D_L16_001DCAE0, 0};

extern float D_L16_001DD0C0[];
float       *D_L16_00161F68[2] = {D_L16_001DD0C0, 0};

extern float D_L16_001DD4A8[];
float       *D_L16_00161F70[2] = {D_L16_001DD4A8, 0};

extern float D_L16_001DDA88[];
float       *D_L16_00161F78[2] = {D_L16_001DDA88, 0};

void draw_level16_visible_single_reflection_mesh(void) __asm__("FUN_L16_002e9270");

void draw_level16_visible_single_reflection_mesh(void) {
    int *point_counts = (int *)D_L16_00161F40;
    int *point_positions = (int *)D_L16_00161F60;
    int *unused_reflection_arguments = (int *)D_L16_00161F68;
    int *direction_vectors = (int *)D_L16_00161F70;
    int *point_word_streams = (int *)D_L16_00161F78;
    int  i;

    vu1_add_g_sregister_2e9270(6, get_effect_texture(0x29));
    vu1_add_g_sregister_2e9270(0x42, 0x4000000064);
    vu1_add_g_sregister_2e9270(8, 0);
    vu1_add_g_sregister_2e9270(0x14, 0xFF9000000260);
    font_queue_vu_state();
    for (i = 0; i < 1; i++) {
        if (fast_bsphere_check(&l16_single_reflection_mesh_bounds[i], 512.0f) != -1) {
            FUN_L16_002e8b98_t(point_counts[i],
                               direction_vectors[i],
                               point_positions[i],
                               unused_reflection_arguments[i],
                               D_L16_001DDC80);
            FUN_L00_001fde98(
                point_counts[i], point_positions[i], point_word_streams[i], D_L16_001DDC80, 1);
        }
    }
}
/* Compute bounding spheres for one mesh and register their renderer. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002EA828), where it is exact; names translated to the US level program. */

extern void draw_level16_visible_single_reflection_mesh(void) __asm__("FUN_L16_002e9270");

void update_level16_single_mesh_render_state(struct Moby *moby) __asm__("FUN_L16_002e93b0");

void update_level16_single_mesh_render_state(struct Moby *moby) {
    enum {
        SINGLE_MESH_RENDER_INITIALIZE_BOUNDING_SPHERE = 0,
        SINGLE_MESH_RENDER_QUEUE_CALLBACK = 1
    };
    int    i, j;
    float  maxx, maxy, maxz, minx, miny, minz;
    float  vertex_position[4];
    float *out;
    float *verts;
    int    count;
    switch (moby->state) {
    case SINGLE_MESH_RENDER_INITIALIZE_BOUNDING_SPHERE: {
        for (i = 0; i <= 0; i++) {
            maxx = maxy = maxz = -1024.0f;
            minx = miny = minz = 1024.0f;
            {
                int k;
                for (k = 0; k < D_L16_00161F40[i]; k++) {
                    float x, y, z;
                    verts = ((float **)D_L16_00161F60)[i] + k * 3;
                    x = verts[0];
                    if (x > maxx)
                        maxx = x;
                    y = verts[1];
                    if (y > maxy)
                        maxy = y;
                    z = verts[2];
                    if (z > maxz)
                        maxz = z;
                    if (x < minx)
                        minx = x;
                    if (y < miny)
                        miny = y;
                    if (z < minz)
                        minz = z;
                }
            }
            {
                L16SingleSphere *sphere = l16_single_reflection_mesh_bounds + i;
                sphere->radius = 0.0f;
                sphere->x = (maxx + minx) * 0.5f;
                sphere->y = (maxy + miny) * 0.5f;
                sphere->z = (maxz + minz) * 0.5f;
            }
            for (j = 0; j < D_L16_00161F40[i]; j++) {
                float vertex_to_center_distance;
                out = (float *)(l16_single_reflection_mesh_bounds + i);
                vertex_position[0] = ((float **)D_L16_00161F60)[i][j * 3 + 0];
                vertex_position[1] = ((float **)D_L16_00161F60)[i][j * 3 + 1];
                vertex_position[2] = ((float **)D_L16_00161F60)[i][j * 3 + 2];
                vertex_to_center_distance = fast_vec_dist(vertex_position, out);
                if (vertex_to_center_distance > out[3])
                    out[3] = vertex_to_center_distance;
            }
        }
        moby->state = 1;
        moby->update_dist = moby->draw_dist = 255;
        break;
    }
    case SINGLE_MESH_RENDER_QUEUE_CALLBACK:
        add_draw_callback(draw_level16_visible_single_reflection_mesh, moby);
        break;
    }
}

/* Bounding spheres for one mesh (two or three entries), then the renderer callback; same source as FUN_L16_002e93b0. */
typedef struct {
    float x, y, z, radius;
} L16SingleSphere_2e98e0;

extern short                  D_L16_00161F90_gp __asm__("D_L16_00161F90") __attribute__((sda));
extern short                  D_L16_00161F98_gp __asm__("D_L16_00161F98") __attribute__((sda));
extern L16SingleSphere_2e98e0 D_L16_00161F60_bounds_2e98e0[] __asm__("D_L16_001DF2B0")
    __attribute__((section(".data")));

void update_level16_two_mesh_render_state(struct Moby *moby) __asm__("FUN_L16_002e98e0");

void update_level16_two_mesh_render_state(struct Moby *moby) {
    enum {
        TWO_MESH_RENDER_INITIALIZE_BOUNDING_SPHERES = 0,
        TWO_MESH_RENDER_QUEUE_CALLBACK = 1
    };
    int    i, j;
    float  maxx, maxy, maxz, minx, miny, minz;
    float  vertex_position[4];
    float *out;
    float *verts;
    int    count;
    switch (moby->state) {
    case TWO_MESH_RENDER_INITIALIZE_BOUNDING_SPHERES: {
        for (i = 0; i <= 1; i++) {
            maxx = maxy = maxz = -1024.0f;
            minx = miny = minz = 1024.0f;
            {
                int k;
                for (k = 0; k < ((int *)&D_L16_00161F90_gp)[i]; k++) {
                    float x, y, z;
                    verts = ((float **)&D_L16_00161F98_gp)[i] + k * 3;
                    x = verts[0];
                    if (x > maxx)
                        maxx = x;
                    y = verts[1];
                    if (y > maxy)
                        maxy = y;
                    z = verts[2];
                    if (z > maxz)
                        maxz = z;
                    if (x < minx)
                        minx = x;
                    if (y < miny)
                        miny = y;
                    if (z < minz)
                        minz = z;
                }
            }
            {
                L16SingleSphere_2e98e0 *base = D_L16_00161F60_bounds_2e98e0;
                L16SingleSphere_2e98e0 *sphere = base + i;
                sphere->radius = 0.0f;
                sphere->x = (maxx + minx) * 0.5f;
                sphere->y = (maxy + miny) * 0.5f;
                sphere->z = (maxz + minz) * 0.5f;
            }
            for (j = 0; j < ((int *)&D_L16_00161F90_gp)[i]; j++) {
                float vertex_to_center_distance;
                out = (float *)(D_L16_00161F60_bounds_2e98e0 + i);
                vertex_position[0] = ((float **)&D_L16_00161F98_gp)[i][j * 3 + 0];
                vertex_position[1] = ((float **)&D_L16_00161F98_gp)[i][j * 3 + 1];
                vertex_position[2] = ((float **)&D_L16_00161F98_gp)[i][j * 3 + 2];
                vertex_to_center_distance = fast_vec_dist(vertex_position, out);
                if (vertex_to_center_distance > out[3])
                    out[3] = vertex_to_center_distance;
            }
        }
        moby->state = 1;
        moby->update_dist = moby->draw_dist = 255;
        break;
    }
    case TWO_MESH_RENDER_QUEUE_CALLBACK:
        add_draw_callback(draw_level16_visible_textured_batches, moby);
        break;
    }
}
/* Sets up the draw state then runs both per-slot calls over three entries. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002EB158), where it is exact; names translated to the US level program. */

extern char level16_reflection_mesh_coordinate_scratch[] __asm__("D_L16_001E28D0");
void        compute_level16_reflection_map_coordinates(
           int n, Vec3f *p1, Vec3f *p2, int unused, float *out) __asm__("FUN_L16_002e8b98");
void vu1_add_g_sregister(s32 a0, s64 a1) __asm__("FUN_00233980");

int D_L16_00161FB8[4] __attribute__((section(".sdata"))) = {148, 148, 53, 0};

extern float D_L16_001DF7B0[];
extern float D_L16_001DFEA0[];
extern float D_L16_001E0590[];
float       *D_L16_00161FC8[4]
    __attribute__((section(".sdata"))) = {D_L16_001DF7B0, D_L16_001DFEA0, D_L16_001E0590, 0};

extern float D_L16_001E0810[];
extern float D_L16_001E0CB0[];
extern float D_L16_001E1150[];
float       *D_L16_00161FD8[4]
    __attribute__((section(".sdata"))) = {D_L16_001E0810, D_L16_001E0CB0, D_L16_001E1150, 0};

extern float D_L16_001E12F8[];
extern float D_L16_001E19E8[];
extern float D_L16_001E20D8[];
float       *D_L16_00161FE8[4]
    __attribute__((section(".sdata"))) = {D_L16_001E12F8, D_L16_001E19E8, D_L16_001E20D8, 0};

extern float D_L16_001E2358[];
extern float D_L16_001E25A8[];
extern float D_L16_001E27F8[];
float       *D_L16_00161FF8[4]
    __attribute__((section(".sdata"))) = {D_L16_001E2358, D_L16_001E25A8, D_L16_001E27F8, 0};

void draw_level16_reflection_mesh_batch(void) __asm__("FUN_L16_002e9ce0");

void draw_level16_reflection_mesh_batch(void) {
    int i;
    vu1_add_g_sregister(6, get_effect_texture(0x29));
    vu1_add_g_sregister(0x42, 0x4000000064L);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xFF9000000260L);
    font_queue_vu_state();
    for (i = 0; i < 3; i++) {
        compute_level16_reflection_map_coordinates(D_L16_00161FB8[i],
                                                   ((int *)D_L16_00161FE8)[i],
                                                   ((int *)D_L16_00161FC8)[i],
                                                   ((int *)D_L16_00161FD8)[i],
                                                   level16_reflection_mesh_coordinate_scratch);
        FUN_L00_001fde98(D_L16_00161FB8[i],
                         ((int *)D_L16_00161FC8)[i],
                         ((int *)D_L16_00161FF8)[i],
                         level16_reflection_mesh_coordinate_scratch,
                         1);
    }
}

/* Bounding spheres for one mesh (two or three entries), then the renderer callback; same source as FUN_L16_002e93b0. */
typedef struct {
    float x, y, z, radius;
} L16SingleSphere_2ea128;

int D_L16_00162008[4] __attribute__((section(".sdata"))) = {148, 75, 45, 0};

extern float D_L16_001E2DB0[];
extern float D_L16_001E34A0[];
extern float D_L16_001E3828[];
float       *D_L16_00162018[4]
    __attribute__((section(".sdata"))) = {D_L16_001E2DB0, D_L16_001E34A0, D_L16_001E3828, 0};

extern L16SingleSphere_2ea128 D_L16_001E5390[] NOT_SDA;
void update_level16_three_mesh_bounds_and_callback(struct Moby *moby) __asm__("FUN_L16_002ea128");

void update_level16_three_mesh_bounds_and_callback(struct Moby *moby) {
    enum {
        MESH_RENDER_INITIALIZE_BOUNDING_SPHERES = 0,
        MESH_RENDER_QUEUE_CALLBACK = 1
    };
    int    i, j;
    float  maxx, maxy, maxz, minx, miny, minz;
    float  vertex_position[4];
    float *out;
    float *verts;
    int    count;
    switch (moby->state) {
    case MESH_RENDER_INITIALIZE_BOUNDING_SPHERES: {
        for (i = 0; i <= 2; i++) {
            maxx = maxy = maxz = -1024.0f;
            minx = miny = minz = 1024.0f;
            {
                int k;
                for (k = 0; k < D_L16_00162008[i]; k++) {
                    float x, y, z;
                    verts = ((float **)D_L16_00162018)[i] + k * 3;
                    x = verts[0];
                    if (x > maxx)
                        maxx = x;
                    y = verts[1];
                    if (y > maxy)
                        maxy = y;
                    z = verts[2];
                    if (z > maxz)
                        maxz = z;
                    if (x < minx)
                        minx = x;
                    if (y < miny)
                        miny = y;
                    if (z < minz)
                        minz = z;
                }
            }
            {
                L16SingleSphere_2ea128 *base = D_L16_001E5390;
                L16SingleSphere_2ea128 *sphere = base + i;
                sphere->radius = 0.0f;
                sphere->x = (maxx + minx) * 0.5f;
                sphere->y = (maxy + miny) * 0.5f;
                sphere->z = (maxz + minz) * 0.5f;
            }
            for (j = 0; j < D_L16_00162008[i]; j++) {
                float vertex_to_center_distance;
                out = (float *)(D_L16_001E5390 + i);
                vertex_position[0] = ((float **)D_L16_00162018)[i][j * 3 + 0];
                vertex_position[1] = ((float **)D_L16_00162018)[i][j * 3 + 1];
                vertex_position[2] = ((float **)D_L16_00162018)[i][j * 3 + 2];
                vertex_to_center_distance = fast_vec_dist(vertex_position, out);
                if (vertex_to_center_distance > out[3])
                    out[3] = vertex_to_center_distance;
            }
        }
        moby->state = 1;
        moby->update_dist = moby->draw_dist = 255;
        break;
    }
    case MESH_RENDER_QUEUE_CALLBACK:
        add_draw_callback(env_map_a_draw, moby);
        break;
    }
}

s32 D_L16_0016CDE4[1] __attribute__((section(".data"))) = {0};
