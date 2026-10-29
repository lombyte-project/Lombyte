/* Level 02 moby classes: skids_agent, skid_mcmarx, liftcar_platform, ocean_aridia. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "asm.h"
#include "rnc/globals.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/gameplay/state/usage_stats.h"

#include "rnc/math_consts.h"
#include "rnc/gameplay/entities/moby.h"
#include "qcopy.h"
#include "rnc/data_labels.h"
#include "rnc/gameplay/state/options.h"
#include "rnc/math/fast_math.h"
#include "sda.h"

/* Pvars of the l02 watcher NPC: it idles, turns its head toward the hero and hands out a reward. */
typedef struct {
    u8 pad0[4];
    s16 talk_state;       /* 0x04: 2 once the conversation is over */
    u8 pad6[2];
    u8 zone_check;        /* 0x08: 1 when the talk radius depends on a clip volume */
    u8 pad9[3];
    f32 talk_radius;      /* 0x0C */
    u8 pad10[0x10];
    void *dialog;         /* 0x20 */
    u8 pad24[0x1C];
    u8 body[0x64];        /* 0x40: body joint driver */
    f32 pitch;            /* 0xA4 */
    f32 yaw;              /* 0xA8 */
    u8 padAC[4];
    f32 height;           /* 0xB0 */
    u8 padB4[0xC];
    u8 head[0x68];        /* 0xC0: head joint driver */
    f32 head_yaw;         /* 0x128 */
    u8 pad12C[0x3E];
    s16 idle_timer;       /* 0x16A */
    s32 zone;             /* 0x16C: clip volume index */
    Vec4f target;         /* 0x170: where it looks when the hero is away */
    s32 unk180;
    u8 pad184[4];
    s32 moving_timer;     /* 0x188: frames left looking at the moving hero */
    s32 target_timer;     /* 0x18C: frames until a new look target */
} WatcherVars;

/* The reward pickup the watcher drops when it leaves. */
typedef struct {
    u8 pad0[0x10];
    Vec4f pos;            /* 0x10 */
    Vec4f vel;            /* 0x20 */
    u8 pad30[0x16];
    s16 placed;           /* 0x46 */
} WatcherReward;

typedef struct {
    Vec4f target;
    Vec4f eye;
    Vec4f delta;
} WatcherLook;

extern u8 D_0013D506 NOT_SDA;
extern u8 big_head_cheat_enabled_macro __asm__("D_0015EDB0")MACRO_ADDR;
extern s32 level_game_mode_macro __asm__("D_L02_0015F5C4") MACRO_ADDR;
extern WatcherReward D_L02_0016CE60;
extern char skid_mcmarx_dialogue_data[] __asm__("D_L02_00161D00");
extern void attach_big_head_cheat_manipulator_to_matching_moby(struct Moby *) __asm__("FUN_L02_002e0cd8");
void update_skid_mcmarx_relocation_sequence(struct Moby *moby) __asm__("FUN_L02_002e1fb8");

extern void update_skid_mcmarx_relocation_sequence(struct Moby *) __asm__("FUN_L02_002e1fb8");
extern void initialize_moby_grounding(struct Moby *) __asm__("FUN_L02_0025c758");
extern void delete_moby(struct Moby *) __asm__("FUN_0020c828");
extern void watcher_init(struct Moby *, WatcherVars *) __asm__("FUN_L00_002668a0");
extern s32 watcher_talk(struct Moby *, WatcherVars *) __asm__("FUN_L00_00266448");
extern void begin_dialogue_camera_transition(struct Moby *, f32) __asm__("FUN_L01_002783a8");
extern s32 is_point_inside_clip_volume(void *, s32) __asm__("FUN_00214720");
extern void help_request(s32, s32) __asm__("FUN_L00_00203908");
extern void grant_inventory_item(s32, s32) __asm__("FUN_L00_00260860");
extern void jet_clear_persistent(s32) __asm__("FUN_L00_002502a0");
extern void jet_reset_camera(void *, void *) __asm__("FUN_L00_00284e50");
extern void memcard_save_data(s32, s32) __asm__("FUN_0020b178");
extern s32 count_down_16(s16 *) __asm__("FUN_001f9770");
extern s32 fast_dec_timer(s32 *) __asm__("FUN_001f9740");
extern f32 random_float_between(f32, f32) __asm__("FUN_002132a8");
extern f32 scale_time(f32) __asm__("FUN_001f96b0");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern s32 random_integer_below(s32) __asm__("FUN_00213260");
extern void blend_anim(struct Moby *, s32, s32, s32) __asm__("FUN_00212f90");
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");
extern f32 vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern f32 probe_ground_height(void *, s32, f32) __asm__("FUN_00213508");
extern f32 fast_arc_tan(f32, f32) __asm__("FUN_001f9e90");
extern f32 fast_add_rotations(f32, f32) __asm__("FUN_001fa580");
extern f32 fast_subtract_rotations(f32, f32) __asm__("FUN_001fa5c8");
extern void build_spherical_offset(void *, f32, f32, f32) __asm__("FUN_00214db0");
extern void update_joint_animation(f32, f32, struct Moby *, void *, s32) __asm__("FUN_L00_002628d8");

extern WatcherReward talk_camera __asm__("D_L02_0016CE60") NOT_SDA; /* dialogue camera spot */
extern char skids_agent_dialogue_data[] __asm__("D_L02_00161CF0");
extern char *D_L02_001600EC;
extern f32 D_L02_0015F3FC;
extern f32 skids_agent_prop_local_offset[2] __asm__("D_L02_00161CE0") __attribute__((sda)); /* prop offset vector; declared small so it is reached through $gp as in retail */
extern struct UsageStat D_00141968[] NOT_SDA; /* usage records 36 on (usage_stats + 0x120) */
extern u8 item_available[] __asm__("D_0013D4C0") NOT_SDA; /* by item id */
extern s32 level_game_mode __asm__("D_L02_0015F5C4");
extern struct Moby *create_moby(s32) __asm__("FUN_0020c4f8");
extern void initialize_npc_dialogue(struct Moby *, char *) __asm__("FUN_L00_002668a0");
extern s32 update_npc_dialogue(struct Moby *, char *) __asm__("FUN_L00_00266448");
extern void start_indexed_disc_stream(int) __asm__("FUN_L02_00298c68");
extern void advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("FUN_00213f38");
extern f32 approach_value(f32 *p, f32 target, f32 maxstep) __asm__("FUN_00213ed8");
extern void lerp_vector(void *out, void *a, void *b, f32 t) __asm__("FUN_001f9a40");
extern void set_camera_position(void *) __asm__("FUN_L00_002ea9d8");
extern void set_camera_rotation(void *) __asm__("FUN_L00_002eaa30");
extern void jet_end_mount(int) __asm__("FUN_L00_002eac18");
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void build_moby_bone_transform(void *, int, void *) __asm__("FUN_0020cca8");
extern void copy_matrix3x4(void *, void *) __asm__("FUN_001fa2b8");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void update_entity(void *) __asm__("FUN_L00_00250df8");

/* Aridia NPC with a carried prop moby (data+0x178): dialogue state machine (the hint in state 1 is usage
 * record 50), a path-camera ride (3/4) that fades D_L02_0015F3FC in and out, then the shared head-tracking tail. */
void skids_agent_update(struct Moby *moby) __asm__("FUN_L02_002e0dc0");
void skids_agent_update(struct Moby *moby) {
    enum {
        SKIDS_AGENT_INITIALIZE = 0,
        SKIDS_AGENT_IDLE_AND_TALK = 1,
        SKIDS_AGENT_CONVERSATION_ACTIVE = 2,
        SKIDS_AGENT_INTERPOLATE_TRANSFORM_AND_FADE = 3,
        SKIDS_AGENT_FADE_TO_ZERO_AND_DELETE = 4,
        SKIDS_AGENT_END_CONVERSATION = 5
    };
    char *data = (char *)moby->pvars;
    s32 usage_time_value;
    f32 fade;
    struct Moby *prop;

    attach_big_head_cheat_manipulator_to_matching_moby(moby);
    switch (moby->state) {
    case SKIDS_AGENT_INITIALIZE:
        if (item_available[0x2D] != 0) {
            goto kill;
        }
        *(char **)(data + 0x20) = skids_agent_dialogue_data;
        moby->state = 1;
        initialize_npc_dialogue(moby, data);
        initialize_moby_grounding(moby);
        prop = create_moby(0x38);
        *(struct Moby **)(data + 0x178) = prop;
        prop->draw_dist = 0x40;
        (*(struct Moby **)(data + 0x178))->visible = 1;
        (*(struct Moby **)(data + 0x178))->spawn_frame = moby->spawn_frame;
        (*(struct Moby **)(data + 0x178))->flags |= 0x100;
        break;
    case SKIDS_AGENT_IDLE_AND_TALK:
        if (*(u8 *)(data + 8) == 1 && is_point_inside_clip_volume(&hero.motion.pos, *(s32 *)(data + 0x154)) != 0) {
            *(f32 *)(data + 0xC) = 255.0f;
        } else {
            *(f32 *)(data + 0xC) = 3.7f;
        }
        if (update_npc_dialogue(moby, data) != 0) {
            begin_dialogue_camera_transition(moby, 2.2f);
            moby->state = 2;
        } else if (item_available[0x5] == 0 && vector_distance(&moby->pos, &hero.motion.pos) < 3.0f) {
            if (D_00141968[14].count != 0) {
                usage_time_value = scale_game_frames(D_0015EEA4) - D_00141968[14].unk2 * 600;
                if ((int)(scale_game_frames(0x12) * 60.0f) < usage_time_value || D_00141968[14].unk2 * 600 == 0) {
                    help_request(0x7D4, 0xE);
                } else {
                    usage_time_value = scale_game_frames(D_0015EEA4) / 600;
                    if (D_00141968[14].unk2 < usage_time_value) {
                        D_00141968[14].unk2 = scale_game_frames(D_0015EEA4) / 600;
                    }
                }
            } else {
                D_00141968[14].count++;
                usage_time_value = scale_game_frames(D_0015EEA4) / 600;
                if (D_00141968[14].unk2 < usage_time_value) {
                    D_00141968[14].unk2 = scale_game_frames(D_0015EEA4) / 600;
                }
                D_00141968[14].level_mask = D_00141968[14].level_mask | (1 << current_level_index) | 0x80000000;
            }
        }
        if (moby->prev_seq == 0 && count_down_16((s16 *)(data + 0x172)) != 0) {
            *(s16 *)(data + 0x172) = truncate_float_to_s32(scale_time(random_float_between(1200.0f, 2400.0f)));
            if (moby->prev_seq != 1) {
                blend_anim(moby, 1, 0, scale_game_frames(10));
            }
        } else if (moby->prev_seq == 1 && (moby->anim_flags & 2)) {
            blend_anim(moby, 0, 0, scale_game_frames(10));
        }
        break;
    case SKIDS_AGENT_CONVERSATION_ACTIVE:
        if (level_game_mode != 2) {
            moby->state = 1;
            if (*(s16 *)(data + 4) == 0) {
                jet_clear_persistent(moby->mission);
                if (*(s32 *)(data + 0x150) != -1) {
                    char *camera_transform_entry = D_L02_001600EC + (*(s32 *)(data + 0x150) << 7);
                    jet_reset_camera(camera_transform_entry + 0x30, camera_transform_entry + 0x70);
                }
            } else if (*(s16 *)(data + 4) == 2) {
                moby->visible = 0;
                moby->collision = 0;
                moby->flags |= 1;
                if (*(struct Moby **)(data + 0x178) != 0) {
                    (*(struct Moby **)(data + 0x178))->flags |= 1;
                    (*(struct Moby **)(data + 0x178))->visible = 0;
                }
                start_indexed_disc_stream(2);
                moby->state = 5;
            }
        } else if (talk_camera.placed == 0) {
            fast_vec_normalize(&talk_camera.pos, &moby->rot_row0, 2.5f);
            fast_vec_add(&talk_camera.pos, &talk_camera.pos, &moby->pos);
            talk_camera.pos.z = probe_ground_height(&talk_camera.pos, 0, 0.5f);
            clear_u64_value(&talk_camera.vel);
            talk_camera.vel.z = fast_add_rotations(moby->rot.z, 3.14159f);
            talk_camera.placed = 1;
        }
        break;
    case SKIDS_AGENT_END_CONVERSATION:
        grant_inventory_item(5, 1);
        memcard_save_data(0, -1);
        goto kill;
    case SKIDS_AGENT_INTERPOLATE_TRANSFORM_AND_FADE: {
        Vec4 pos;
        Vec4 rot;
        Vec4 from;
        Vec4 to;
        advance_accelerated_scalar((f32 *)(data + 0x180), (f32 *)(data + 0x184), 1.0f, frame_time_sq * 0.1f,
                           frame_time_sq * 0.25f, frame_time * 0.5f);
        lerp_vector(&pos, D_L02_001600EC + (*(s32 *)(data + 0x158) << 7) + 0x30,
                    D_L02_001600EC + (*(s32 *)(data + 0x15C) << 7) + 0x30, *(f32 *)(data + 0x180));
        qcopy_nc(&from, (char *)((*(s32 *)(data + 0x158) << 7) + (s32)D_L02_001600EC) + 0x70);
        qcopy_nc(&to, (char *)((*(s32 *)(data + 0x15C) << 7) + (s32)D_L02_001600EC) + 0x70);
        rot.f[2] = fast_add_rotations(fast_subtract_rotations(to.f[2], from.f[2]) * *(f32 *)(data + 0x180), from.f[2]);
        rot.f[1] = fast_add_rotations(fast_subtract_rotations(to.f[1], from.f[1]) * *(f32 *)(data + 0x180), from.f[1]);
        rot.f[0] = fast_add_rotations(fast_subtract_rotations(to.f[0], from.f[0]) * *(f32 *)(data + 0x180), from.f[0]);
        set_camera_position(&pos);
        set_camera_rotation(&rot);
        if (*(f32 *)(data + 0x180) < 1.0f) {
            approach_value((f32 *)(data + 0x17C), 0.0f, 0.1f);
        } else {
            approach_value((f32 *)(data + 0x17C), 1.0f, 0.1f);
            if (*(f32 *)(data + 0x17C) == 1.0f) {
                moby->state = 4;
                jet_end_mount(0);
            }
        }
        D_L02_0015F3FC = *(f32 *)(data + 0x17C);
        break;
    }
    case SKIDS_AGENT_FADE_TO_ZERO_AND_DELETE: {
        approach_value((f32 *)(data + 0x17C), 0.0f, 0.1f);
        fade = *(f32 *)(data + 0x17C);
        D_L02_0015F3FC = fade;
        if (fade == 0.0f) {
            if (*(struct Moby **)(data + 0x178) != 0) {
                delete_moby(*(struct Moby **)(data + 0x178));
            }
        kill:
            delete_moby(moby);
            return;
        }
        break;
    }
    }

    /* A statement expression: retail reuses this scope's stack for the head-tracking block below. */
    ({
        float mtx[16];
        Vec4 offset;
        if (*(struct Moby **)(data + 0x178) != 0) {
            moby_point_position(moby, 2, &(*(struct Moby **)(data + 0x178))->pos);
            build_moby_bone_transform(moby, 2, mtx);
            copy_matrix3x4(&(*(struct Moby **)(data + 0x178))->rot_row0, mtx);
            transform_vector_by_basis(&offset, skids_agent_prop_local_offset, &(*(struct Moby **)(data + 0x178))->rot_row0);
            fast_vec_add(&(*(struct Moby **)(data + 0x178))->pos, &(*(struct Moby **)(data + 0x178))->pos, &offset);
            update_entity(*(struct Moby **)(data + 0x178));
        }
    });

    {
        Vec4 target;
        Vec4 head;
        Vec4 head_to_target_delta;
        s32 look_enabled;
        f32 rotation_step;
        f32 rotation_limit;
        f32 heading;
        f32 yaw;
        f32 pitch;
        f32 delta;

        rotation_step = 0.02f;
        rotation_limit = 0.3f;
        look_enabled = 0;
        if (moby->prev_seq == 0) {
            look_enabled = 1;
            if (vector_distance(&moby->pos, &hero.motion.pos) < 8.0f &&
                fast_difference_between_rotations(moby->rot.z, fast_arc_tan(hero.motion.body_center.f[0] - moby->pos.x,
                                                                     hero.motion.body_center.f[1] - moby->pos.y)) <
                    1.5707964f) {
                if (fast_vec_length(&hero.motion.displacement) > 0.01f) {
                    *(s16 *)(data + 0x174) = scale_game_frames(120);
                } else {
                    count_down_16((s16 *)(data + 0x174));
                }
            } else if (*(s16 *)(data + 0x174) != 0) {
                *(s16 *)(data + 0x174) = 0;
                qcopy(data + 0x160, &hero.motion.body_center);
            }
            if (count_down_16((s16 *)(data + 0x176))) {
                *(s16 *)(data + 0x176) = truncate_float_to_s32(scale_time(random_float_between(180.0f, 300.0f)));
                heading = fast_add_rotations(moby->rot.z, random_float_between(-90.0f, 90.0f) * 0.017453292f);
                pitch = random_float_between(0.0f, 30.0f) * 0.017453292f;
                build_spherical_offset(data + 0x160, 6.0f, heading, pitch);
                fast_vec_add(data + 0x160, data + 0x160, &moby->pos);
            }
            if (*(s16 *)(data + 0x174) != 0) {
                qcopy(&target, &hero.motion.body_center);
                rotation_step = 0.04f;
                rotation_limit = 0.3f;
            } else {
                qcopy(&target, data + 0x160);
            }
        }
        if (look_enabled) {
            qcopy(&head, &moby->pos);
            head.f[2] += 1.0f;
            fast_vec_sub(&head_to_target_delta, &target, &head);
            yaw = fast_subtract_rotations(fast_arc_tan(head_to_target_delta.f[0], head_to_target_delta.f[1]), moby->rot.z);
            pitch = -fast_arc_tan(vector_length_xy(&head_to_target_delta), head_to_target_delta.f[2]);
            if (yaw > 1.5707964f)
                yaw = 1.5707964f;
            else if (yaw < -1.5707964f)
                yaw = -1.5707964f;
            if (pitch > 0.5235988f)
                pitch = 0.5235988f;
            else if (pitch < -0.5235988f)
                pitch = -0.5235988f;
            *(f32 *)(data + 0xA4) = pitch;
            *(f32 *)(data + 0xA8) = yaw * 0.7f;
            *(f32 *)(data + 0x128) = yaw * 0.3f;
        }
        if (big_head_cheat_enabled != 0) {
            *(f32 *)(data + 0xB0) = 2.75f;
        }
        delta = frame_scale_sq;
        update_joint_animation(rotation_step * delta, rotation_limit * delta, moby, data + 0x40, 0);
        update_joint_animation(rotation_step * frame_scale_sq, rotation_limit * frame_scale_sq, moby, data + 0xC0, 1);
    }
}

extern f32 frame_scale_sq_macro __asm__("D_0015ED64") MACRO_ADDR;
/* Watcher NPC update (twin of qwarks_bouncer_update): idles with random animations, talks to the hero and drops its reward, and turns its head toward the hero or a random look target. */
void skid_mcmarx_update(struct Moby *m) __asm__("FUN_L02_002e1950");
void skid_mcmarx_update(struct Moby *moby) {
    enum {
        SKID_MCMARX_INITIALIZE = 0,
        SKID_MCMARX_IDLE_AND_TALK = 1,
        SKID_MCMARX_CONVERSATION_ACTIVE = 2
    };
    WatcherVars *watcher_vars = (WatcherVars *)moby->pvars;
    WatcherLook look;
    f32 rate;
    f32 head_rate;
    s32 tracking;

    attach_big_head_cheat_manipulator_to_matching_moby(moby);
    update_skid_mcmarx_relocation_sequence((char *)moby);
    switch (moby->state) {
    case SKID_MCMARX_INITIALIZE:
        if (D_0013D506) {
            delete_moby(moby);
            return;
        }
        watcher_vars->unk180 = 0;
        watcher_vars->dialog = skid_mcmarx_dialogue_data;
        moby->state = 1;
        moby->update_dist = 0xFF;
        watcher_init(moby, watcher_vars);
        initialize_moby_grounding(moby);
        break;
    case SKID_MCMARX_IDLE_AND_TALK:
        if (watcher_vars->zone_check == 1 && is_point_inside_clip_volume(&hero.motion.pos, watcher_vars->zone))
            watcher_vars->talk_radius = 255.0f;
        else
            watcher_vars->talk_radius = 2.5f;
        if (watcher_talk(moby, watcher_vars)) {
            begin_dialogue_camera_transition(moby, 2.5f);
            moby->state = 2;
        }
        if (watcher_vars->talk_state == 2) {
            help_request(2000, 10);
            grant_inventory_item(30, 1);
            jet_clear_persistent(*((u8 *)moby + 0xB0));
            jet_reset_camera(&hero.motion.pos, &hero.motion.rot);
            memcard_save_data(0, -1);
            delete_moby(moby);
            return;
        }
        if (moby->prev_seq == 0 && count_down_16(&watcher_vars->idle_timer)) {
            s32 idle_animation = 1;
            watcher_vars->idle_timer = truncate_float_to_s32(scale_time(random_float_between(1200.0f, 2400.0f)));
            if (random_integer_below(2) == 0)
                idle_animation = 2;
            if (moby->prev_seq != idle_animation)
                blend_anim(moby, idle_animation, 0, scale_game_frames(10));
        } else if ((moby->anim_flags & 2) && moby->prev_seq) {
            blend_anim(moby, 0, 0, scale_game_frames(10));
        }
        break;
    case SKID_MCMARX_CONVERSATION_ACTIVE:
        if (D_L02_0016CE60.placed == 0) {
            fast_vec_normalize(&D_L02_0016CE60.pos, &moby->rot_row0, 2.5f);
            fast_vec_add(&D_L02_0016CE60.pos, &D_L02_0016CE60.pos, &moby->pos);
            D_L02_0016CE60.pos.z += 10.0f;
            D_L02_0016CE60.pos.z = probe_ground_height(&D_L02_0016CE60.pos, 0, 0.5f);
            clear_u64_value(&D_L02_0016CE60.vel);
            D_L02_0016CE60.vel.z = fast_add_rotations(moby->rot.z, 3.14159f);
            D_L02_0016CE60.placed = 1;
        }
        if (level_game_mode_macro != 2)
            moby->state = 1;
        break;
    }
    rate = 0.02f;
    head_rate = 0.3f;
    tracking = 0;
    if (moby->prev_seq == 0) {
        tracking = 1;
        if (vector_distance(&moby->pos, &hero.motion.pos) < 8.0f
            && fast_difference_between_rotations(moby->rot.z, fast_arc_tan(hero.motion.body_center.f[0] - moby->pos.x,
                                                hero.motion.body_center.f[1] - moby->pos.y)) < 1.5707964f) {
            if (fast_vec_length(&hero.motion.displacement) > 0.01f)
                watcher_vars->moving_timer = scale_game_frames(120);
            else
                fast_dec_timer(&watcher_vars->moving_timer);
        } else if (watcher_vars->moving_timer) {
            watcher_vars->moving_timer = 0;
            qcopy(&watcher_vars->target, &hero.motion.body_center);
        }
        if (fast_dec_timer(&watcher_vars->target_timer)) {
            f32 heading;
            watcher_vars->target_timer = truncate_float_to_s32(scale_time(random_float_between(180.0f, 300.0f)));
            heading = fast_add_rotations(moby->rot.z, random_float_between(-90.0f, 90.0f) * DEG_TO_RAD);
            build_spherical_offset(&watcher_vars->target, 6.0f, heading, random_float_between(0.0f, 30.0f) * DEG_TO_RAD);
            fast_vec_add(&watcher_vars->target, &watcher_vars->target, &moby->pos);
        }
        if (watcher_vars->moving_timer) {
            qcopy(&look.target, &hero.motion.body_center);
            rate = 0.04f;
            head_rate = 0.3f;
        } else {
            qcopy(&look.target, &watcher_vars->target);
        }
    }
    if (tracking) {
        f32 yaw;
        f32 pitch;
        qcopy(&look.eye, &moby->pos);
        look.eye.z += 2.0f;
        fast_vec_sub(&look.delta, &look.target, &look.eye);
        yaw = fast_subtract_rotations(fast_arc_tan(look.delta.x, look.delta.y), moby->rot.z);
        pitch = -fast_arc_tan(vector_length_xy(&look.delta), look.delta.z);
        if (yaw > 1.5707964f)
            yaw = 1.5707964f;
        else if (yaw < -1.5707964f)
            yaw = -1.5707964f;
        if (pitch > 0.5235988f)
            pitch = 0.5235988f;
        else if (pitch < -0.5235988f)
            pitch = -0.5235988f;
        watcher_vars->pitch = pitch;
        watcher_vars->yaw = yaw * 0.6f;
        watcher_vars->head_yaw = yaw * 0.4f;
    }
    if (big_head_cheat_enabled_macro)
        watcher_vars->height = 2.75f;
    update_joint_animation(rate * frame_scale_sq_macro, head_rate * frame_scale_sq_macro, moby, watcher_vars->body, 0);
    update_joint_animation(rate * frame_scale_sq_macro, head_rate * frame_scale_sq_macro, moby, watcher_vars->head, 1);
}
#include "qcopy.h"
extern char *D_L02_001600EC_n __asm__("D_L02_001600EC");
extern char *D_L02_0015FFD8_n __asm__("D_L02_0015FFD8");
extern int FUN_00214720_n(void *, int) __asm__("FUN_00214720");
extern void FUN_001ff570_n(int, int) __asm__("FUN_001ff570");
extern int tick_n(int *) __asm__("FUN_001f9770");
extern void FUN_001f4a58_n(int) __asm__("FUN_001f4a58");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern void FUN_L00_00216f90_n(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern int FUN_001ff308_n(int, int, void *, void *, void *, void *, int) __asm__("FUN_001ff308");
extern void initialize_numeric_hud_timer_and_base_dimensions(void) __asm__("FUN_L00_0023a760");
extern void update_hud_counter_transition(void) __asm__("FUN_L02_0023ccc8");
extern void draw_hud_counter(void) __asm__("FUN_L02_0023cda8");

/* Three-state controller (data+0x180): 0 waits for its trigger, 1 runs timed effects until released, then moves the moby to its spawn point, sets the two linked mobys' flags and enters state 2. */
void update_skid_mcmarx_relocation_sequence(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    float fx_pos[4] __attribute__((aligned(16)));
    float fx_rot[4] __attribute__((aligned(16)));
    char *entry;
    int entry_off;
    char *mobys;
    float yaw;
    char *table;
    if (*(int *)(data + 0x180) == 1) {
        if (FUN_00214720_n(&hero.motion.pos, *(int *)(data + 0x148)) == 0) {
            *(int *)(data + 0x180) = 0;
            FUN_001ff570_n(*(int *)(data + 0x160), 0);
            *(int *)(data + 0x160) = -1;
            FUN_001ff570_n(*(int *)(data + 0x164), 0);
            *(int *)(data + 0x164) = -1;
            return;
        }
        if (*(int *)(data + 0x14C) != 0 || *(int *)(data + 0x150) != 0)
            return;
        if (tick_n((int *)(data + 0x168)) == 0)
            return;
        *(int *)(data + 0x180) = 2;
        FUN_001f4a58_n(scale_game_frames(0x10));
        FUN_001ff570_n(*(int *)(data + 0x160), 0);
        *(int *)(data + 0x160) = -1;
        FUN_001ff570_n(*(int *)(data + 0x164), 0);
        table = D_L02_001600EC_n;
        entry_off = *(int *)(data + 0x154) << 7;
        mobys = table;
        *(int *)(data + 0x164) = -1;
        qcopy(&moby->pos, (char *)(entry_off + (int)mobys) + 0x30);
        entry = (char *)(entry_off + (int)mobys);
        moby->rot.z = yaw = fast_arc_tan(*(float *)(entry + 0), *(float *)(entry + 4));
        qcopy(fx_pos, &moby->pos);
        fx_pos[0] += fast_cos(yaw) * 1.5f;
        fx_pos[1] += fast_sin(moby->rot.z) * 1.5f;
        clear_u64_value(fx_rot);
        fx_rot[2] = fast_add_rotations(moby->rot.z, 3.1415927f);
        FUN_L00_00216f90_n(fx_pos, fx_rot, 0, 1);
        {
            int i0 = *(int *)(data + 0x158);
            int i1 = *(int *)(data + 0x15C);
            mobys = D_L02_0015FFD8_n;
            *(unsigned char *)(mobys + (i0 << 8) + 0xBC) = 1;
            mobys += i1 << 8;
            mobys[0xBC] = 1;
            *(short *)(data + 0x36) = 2;
            data[8] = 1;
        }
    } else if (*(int *)(data + 0x180) == 0) {
        if (FUN_00214720_n(&hero.motion.pos, *(int *)(data + 0x148)) != 0) {
            *(int *)(data + 0x180) = 1;
            *(int *)(data + 0x164) = FUN_001ff308_n(0x15, 0x7D0, initialize_numeric_hud_timer_and_base_dimensions, update_hud_counter_transition, draw_hud_counter, data + 0x14C, 100);
            *(int *)(data + 0x160) = FUN_001ff308_n(0x17, 0x7D1, initialize_numeric_hud_timer_and_base_dimensions, update_hud_counter_transition, draw_hud_counter, data + 0x150, 7);
        }
    }
}
extern struct Moby *D_L02_0015FFD8;
extern unsigned char D_0014C050[][16];
extern char voice_pool[] __asm__("D_0013E550");
extern float distance_xyz(void *a, void *b) __asm__("FUN_001f9b80");
extern void set_moby_animation(void *, int, int) __asm__("FUN_00212ed8");
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
extern int allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
extern void release_voice_slot(int) __asm__("FUN_0022d798");
extern void update_entity_matrix(void *, void *, void *, void *) __asm__("FUN_L00_00260738");

/* Lift platform carried by an animated driver moby: follows the driver's joint 0, waits at an end (3/5) for the
 * hero to step on, then plays the driver's up or down animation (2/4) with its loop sound until it ends. */
void liftcar_platform_update(struct Moby *moby) __asm__("FUN_L02_002e2228");
void liftcar_platform_update(struct Moby *moby) {
    enum {
        LIFTCAR_PLATFORM_INITIALIZE = 0,
        LIFTCAR_PLATFORM_WAIT_FOR_MISSION_GATE = 1,
        LIFTCAR_PLATFORM_ANIMATE_THEN_WAIT_FOR_RIDER_OR_HERO_ABOVE = 2,
        LIFTCAR_PLATFORM_WAIT_FOR_RIDER_OR_HERO_ABOVE = 3,
        LIFTCAR_PLATFORM_ANIMATE_THEN_WAIT_FOR_RIDER_OR_HERO_BELOW = 4,
        LIFTCAR_PLATFORM_WAIT_FOR_RIDER_OR_HERO_BELOW = 5,
        LIFTCAR_PLATFORM_FOLLOW_DRIVER_ONLY = 6
    };
    char *data = (char *)moby->pvars;
    struct Moby *driver = &D_L02_0015FFD8[*(int *)(data + 0xA0)];
    Vec4 delta;
    Vec4 old_rot;
    float joint_mtx[16];
    Vec4 pos;
    int ride;
    int voice;

    fast_vec_scale(&delta, &moby->pos, -1.0f);
    qcopy(&old_rot, &moby->rot);
    moby_point_position(driver, 0, &pos);
    qcopy(&moby->pos, &pos);
    switch (moby->state) {
    case LIFTCAR_PLATFORM_INITIALIZE:
        build_moby_bone_transform(driver, 0, joint_mtx);
        moby->rot.z = fast_arc_tan(joint_mtx[0], joint_mtx[1]);
        moby->state = 1;
        blend_moby_animation(driver, 3, 0, 0);
        break;
    case LIFTCAR_PLATFORM_WAIT_FOR_MISSION_GATE:
        if (D_0014C050[current_level_index][moby->mission] == 0xFF) {
            moby->state = 5;
        }
        break;
    case LIFTCAR_PLATFORM_WAIT_FOR_RIDER_OR_HERO_ABOVE:
    case LIFTCAR_PLATFORM_WAIT_FOR_RIDER_OR_HERO_BELOW:
        ride = 0;
        if (hero.ground_moby == moby && hero.air_frames.s == 0 && moby->command == 0) {
            ride = 1;
        } else if (distance_xyz(&moby->pos, &hero.motion.pos) < 16.0f) {
            if (moby->state == 5 && hero.motion.pos.f[2] < moby->pos.z - 5.0f) {
                ride = 1;
            } else if (moby->state == 3 && moby->pos.z + 5.0f < hero.motion.pos.f[2]) {
                ride = 1;
            }
        }
        if (ride) {
            if (moby->state == 5 || moby->state == 1) {
                moby->state = 2;
                set_moby_animation(driver, 0, 0);
                *(int *)(data + 0xA4) = allocate_voice_for_target_entry(2, 4, driver);
            } else {
                moby->state = 4;
                set_moby_animation(driver, 2, 0);
                *(int *)(data + 0xA4) = allocate_voice_for_target_entry(0, 4, driver);
            }
        } else if (hero.ground_moby != moby) {
            moby->command = 0;
        }
        break;
    case LIFTCAR_PLATFORM_ANIMATE_THEN_WAIT_FOR_RIDER_OR_HERO_ABOVE:
    case LIFTCAR_PLATFORM_ANIMATE_THEN_WAIT_FOR_RIDER_OR_HERO_BELOW:
        build_moby_bone_transform(driver, 0, joint_mtx);
        moby->rot.z = fast_arc_tan(joint_mtx[0], joint_mtx[1]);
        if (distance_xyz(&moby->pos, &hero.motion.pos) < 1.5f && hero.motion.pos.f[2] < moby->pos.z - 1.5f) {
            driver->anim_speed = 0.0f;
            voice = *(int *)(data + 0xA4);
            if (voice != -1) {
                char *e = voice_pool + voice * 0x70;
                if (*(struct Moby **)(e + 0x88) == driver && *(unsigned char *)(e + 0x74) != 0) {
                    release_voice_slot(voice);
                }
            }
            *(int *)(data + 0xA4) = -1;
        }
        if (driver->anim_speed == 0.0f &&
            (distance_xyz(&moby->pos, &hero.motion.pos) > 2.0f || moby->pos.z - 1.25f < hero.motion.pos.f[2])) {
            driver->anim_speed = 1.0f;
            if (moby->state == 2) {
                *(int *)(data + 0xA4) = allocate_voice_for_target_entry(2, 4, driver);
            } else {
                *(int *)(data + 0xA4) = allocate_voice_for_target_entry(0, 4, driver);
            }
        }
        if (driver->anim_flags & 2) {
            voice = *(int *)(data + 0xA4);
            if (voice != -1) {
                char *e = voice_pool + voice * 0x70;
                if (*(struct Moby **)(e + 0x88) == driver && *(unsigned char *)(e + 0x74) != 0) {
                    release_voice_slot(voice);
                }
            }
            *(int *)(data + 0xA4) = -1;
            if (moby->state == 2) {
                moby->state = 3;
                blend_moby_animation(driver, 1, 0, 0);
                allocate_voice_for_target_entry(3, 0, driver);
            } else {
                moby->state = 5;
                blend_moby_animation(driver, 3, 0, 0);
                allocate_voice_for_target_entry(1, 0, driver);
            }
            moby->command = 1;
        }
        break;
    case LIFTCAR_PLATFORM_FOLLOW_DRIVER_ONLY:
        break;
    }
    fast_vec_add(&delta, &delta, &moby->pos);
    update_entity_matrix(data + 0x60, &delta, &old_rot, &moby->rot);
}

#include "rnc/overlay/l02/gameplay/entities/skids_agent_globals.h"
extern char D_L02_001673C0[];
extern char aridia_ocean_first_clip_draw_data[] __asm__("D_L02_001F3C00");
extern char aridia_ocean_last_clip_draw_data[] __asm__("D_L02_001F3C60");
extern char aridia_ocean_middle_clip_first_draw_data[] __asm__("D_L02_001F3CD0");
extern char D_L02_001F3D30[];
extern char D_L02_001F3D90[];
extern char D_L02_001F3DD0[];
extern char aridia_ocean_middle_clip_last_draw_data[] __asm__("D_L02_001F3E10");
extern char aridia_ocean_animated_frame_packet[] __asm__("D_L02_001F3E80");
extern void begin_animated_frame_rendering(float, float, float, float, int, int, int, int, int, int, void *) __asm__("FUN_L02_002a4818");
extern void FUN_L02_002a47f8_c(void) __asm__("FUN_L02_002a47f8");
extern int is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");
extern void refresh_point_light(void *) __asm__("FUN_00201f58");

/* Draws the parts whose clip volumes contain the camera (all of them when
 * the override flag is set). */
void draw_aridia_ocean_parts(struct Moby *moby) __asm__("FUN_L02_002ea048");

void draw_aridia_ocean_parts(struct Moby *moby) {
    int *clip_volume_indices = (int *)moby->pvars;

    begin_animated_frame_rendering(0.0f, 260080.0f, 255.0f, 0.0f, 15, 15, 25, 30, 40, 64, aridia_ocean_animated_frame_packet);
    if (aridia_ocean_clip_culling_override != 0 || (clip_volume_indices[0] != -1 && is_point_inside_clip_volume(D_L02_001673C0, clip_volume_indices[0]))) {
        refresh_point_light(aridia_ocean_first_clip_draw_data);
    }
    if (aridia_ocean_clip_culling_override != 0 || (clip_volume_indices[1] != -1 && is_point_inside_clip_volume(D_L02_001673C0, clip_volume_indices[1]))) {
        refresh_point_light(aridia_ocean_middle_clip_first_draw_data);
        refresh_point_light(D_L02_001F3D30);
        refresh_point_light(D_L02_001F3D90);
        refresh_point_light(D_L02_001F3DD0);
        refresh_point_light(aridia_ocean_middle_clip_last_draw_data);
    }
    if (aridia_ocean_clip_culling_override != 0 || (clip_volume_indices[2] != -1 && is_point_inside_clip_volume(D_L02_001673C0, clip_volume_indices[2]))) {
        refresh_point_light(aridia_ocean_last_clip_draw_data);
    }
    FUN_L02_002a47f8_c();
}

extern void add_draw_callback(void *, void *) __asm__("FUN_001f4600");



/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002E21F8.c: func_L02_002EB5D0), where it is exact; names translated to the US level program. */

extern void prepare_scaled_table_on_stack(float) __asm__("FUN_L02_002a47f8");
extern void FUN_L02_002ea048_cb(void) __asm__("FUN_L02_002ea048");

void ocean_aridia_update(struct Moby *moby) __asm__("FUN_L02_002ea198");
void ocean_aridia_update(struct Moby *moby) {
    enum {
        OCEAN_ARIDIA_INITIALIZE = 0,
        OCEAN_ARIDIA_REGISTER_DRAW_CALLBACK = 1
    };
    switch (moby->state) {
    case OCEAN_ARIDIA_INITIALIZE:
        prepare_scaled_table_on_stack(0.16666667f);
        moby->state = 1;
        moby->update_dist = 0xFF;
        break;
    case OCEAN_ARIDIA_REGISTER_DRAW_CALLBACK:
        add_draw_callback(FUN_L02_002ea048_cb, moby);
        break;
    }
}
