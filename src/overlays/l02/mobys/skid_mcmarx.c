/* Level 02 moby class skid_mcmarx. */
#include "types.h"
#include "rnc/hero/hero.h"

#include "rnc/math_consts.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/rendering/effects/fade_to_black.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/animation/set_animation_parameter.h"
#include "rnc/overlay/shared/cutscenes/update_npc_dialogue.h"
#include "rnc/overlay/shared/game/restore_or_clear_level_state.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/mobys/initialize_moby_grounding.h"
#include "rnc/overlay/shared/mobys/attach_big_head_cheat_manipulator.h"

#include "rnc/overlay/l02/mobys/watcher_npc.h"

extern u8 D_0013D506 NOT_SDA;
extern u8            big_head_cheat_enabled_macro __asm__("D_0015EDB0") MACRO_ADDR;
extern s32           level_game_mode_macro __asm__("D_L02_0015F5C4") MACRO_ADDR;
extern WatcherReward D_L02_0016CE60;
extern char          skid_mcmarx_dialogue_data[] __asm__("D_L02_00161D00");
void        update_skid_mcmarx_relocation_sequence(struct Moby *moby) __asm__("FUN_L02_002e1fb8");
extern void begin_dialogue_camera_transition(struct Moby *, f32) __asm__("FUN_L01_002783a8");
extern void help_request(s32, s32) __asm__("FUN_L00_00203908");
extern void memcard_save_data(s32, s32) __asm__("FUN_0020b178");
extern f32  scale_time(f32) __asm__("FUN_001f96b0");
extern void
update_joint_animation(f32, f32, struct Moby *, void *, s32) __asm__("FUN_L00_002628d8");

#include "rnc/overlay/cuboid.h"

extern f32 frame_scale_sq_macro __asm__("D_0015ED64") MACRO_ADDR;
/* Watcher NPC update (twin of qwarks_bouncer_update): idles with random animations, talks to the hero and drops its reward, and turns its head toward the hero or a random look target. */
void skid_mcmarx_update(struct Moby *m) __asm__("FUN_L02_002e1950");

/* Moby class 788 */
void skid_mcmarx_update(struct Moby *moby) {
    enum {
        SKID_MCMARX_INITIALIZE = 0,
        SKID_MCMARX_IDLE_AND_TALK = 1,
        SKID_MCMARX_CONVERSATION_ACTIVE = 2
    };
    WatcherVars *watcher_vars = (WatcherVars *)moby->pvars;
    WatcherLook  look;
    f32          rate;
    f32          head_rate;
    s32          tracking;

    attach_big_head_cheat_manipulator_to_matching_moby(moby);
    update_skid_mcmarx_relocation_sequence((char *)moby);
    switch (moby->state) {
    case SKID_MCMARX_INITIALIZE:
        if (D_0013D506) {
            mark_moby_for_removal(moby);
            return;
        }
        watcher_vars->unk180 = 0;
        watcher_vars->dialog = skid_mcmarx_dialogue_data;
        moby->state = 1;
        moby->update_dist = 0xFF;
        initialize_npc_dialogue(moby, watcher_vars);
        initialize_moby_grounding(moby);
        break;
    case SKID_MCMARX_IDLE_AND_TALK:
        if (watcher_vars->zone_check == 1 &&
            is_point_inside_clip_volume(&hero.motion.pos, watcher_vars->zone))
            watcher_vars->talk_radius = 255.0f;
        else
            watcher_vars->talk_radius = 2.5f;
        if (update_npc_dialogue(moby, watcher_vars)) {
            begin_dialogue_camera_transition(moby, 2.5f);
            moby->state = 2;
        }
        if (watcher_vars->talk_state == 2) {
            help_request(2000, 10);
            grant_inventory_item(30, 1);
            jet_clear_persistent(*((u8 *)moby + 0xB0));
            jet_reset_camera(&hero.motion.pos, &hero.motion.rot);
            memcard_save_data(0, -1);
            mark_moby_for_removal(moby);
            return;
        }
        if (moby->prev_seq == 0 && fast_dec_timer_s16(&watcher_vars->idle_timer)) {
            s32 idle_animation = 1;
            watcher_vars->idle_timer =
                truncate_float_to_s32(scale_time(random_float_between(1200.0f, 2400.0f)));
            if (random_integer_below(2) == 0)
                idle_animation = 2;
            if (moby->prev_seq != idle_animation)
                blend_moby_animation(moby, idle_animation, 0, scale_game_frames(10));
        } else if ((moby->anim_flags & 2) && moby->prev_seq) {
            blend_moby_animation(moby, 0, 0, scale_game_frames(10));
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
        if (vector_distance_xy(&moby->pos, &hero.motion.pos) < 8.0f &&
            fast_difference_between_rotations(
                moby->rot.z,
                fast_arc_tan(hero.motion.body_center.f[0] - moby->pos.x,
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
            watcher_vars->target_timer =
                truncate_float_to_s32(scale_time(random_float_between(180.0f, 300.0f)));
            heading =
                fast_add_rotations(moby->rot.z, random_float_between(-90.0f, 90.0f) * DEG_TO_RAD);
            build_spherical_offset(&watcher_vars->target,
                                   6.0f,
                                   heading,
                                   random_float_between(0.0f, 30.0f) * DEG_TO_RAD);
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
    update_joint_animation(
        rate * frame_scale_sq_macro, head_rate * frame_scale_sq_macro, moby, watcher_vars->body, 0);
    update_joint_animation(
        rate * frame_scale_sq_macro, head_rate * frame_scale_sq_macro, moby, watcher_vars->head, 1);
}

extern char *D_L02_0015FFD8_n __asm__("D_L02_0015FFD8");
extern void  set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern int
queue_animation_update(int, int, void *, void *, void *, void *, int) __asm__("FUN_001ff308");
extern void initialize_numeric_hud_timer_and_base_dimensions(void) __asm__("FUN_L00_0023a760");
extern void update_hud_counter_transition(void) __asm__("FUN_L02_0023ccc8");
extern void draw_hud_counter(void) __asm__("FUN_L02_0023cda8");

/* Three-state controller (data+0x180): 0 waits for its trigger, 1 runs timed effects until released, then moves the moby to its spawn point, sets the two linked mobys' flags and enters state 2. */
void update_skid_mcmarx_relocation_sequence(struct Moby *moby) {
    char *relocation_vars = (char *)moby->pvars;
    float fx_pos[4] __attribute__((aligned(16)));
    float fx_rot[4] __attribute__((aligned(16)));
    char *relocation_transform_record;
    int   entry_off;
    char *mobys;
    float yaw;
    char *table;
    if (*(int *)(relocation_vars + 0x180) == 1) {
        if (is_point_inside_clip_volume(&hero.motion.pos, *(int *)(relocation_vars + 0x148)) == 0) {
            *(int *)(relocation_vars + 0x180) = 0;
            set_animation_parameter(*(int *)(relocation_vars + 0x160), 0);
            *(int *)(relocation_vars + 0x160) = -1;
            set_animation_parameter(*(int *)(relocation_vars + 0x164), 0);
            *(int *)(relocation_vars + 0x164) = -1;
            return;
        }
        if (*(int *)(relocation_vars + 0x14C) != 0 || *(int *)(relocation_vars + 0x150) != 0)
            return;
        if (fast_dec_timer_s16((int *)(relocation_vars + 0x168)) == 0)
            return;
        *(int *)(relocation_vars + 0x180) = 2;
        fade_to_black(scale_game_frames(0x10));
        set_animation_parameter(*(int *)(relocation_vars + 0x160), 0);
        *(int *)(relocation_vars + 0x160) = -1;
        set_animation_parameter(*(int *)(relocation_vars + 0x164), 0);
        table = (char *)l02_cuboids;
        entry_off = *(int *)(relocation_vars + 0x154) << 7;
        mobys = table;
        *(int *)(relocation_vars + 0x164) = -1;
        qcopy(&moby->pos, (char *)(entry_off + (int)mobys) + 0x30);
        relocation_transform_record = (char *)(entry_off + (int)mobys);
        moby->rot.z = yaw = fast_arc_tan(*(float *)(relocation_transform_record + 0),
                                         *(float *)(relocation_transform_record + 4));
        qcopy(fx_pos, &moby->pos);
        fx_pos[0] += fast_cos(yaw) * 1.5f;
        fx_pos[1] += fast_sin(moby->rot.z) * 1.5f;
        clear_u64_value(fx_rot);
        fx_rot[2] = fast_add_rotations(moby->rot.z, 3.1415927f);
        set_race_camera(fx_pos, fx_rot, 0, 1);
        {
            int i0 = *(int *)(relocation_vars + 0x158);
            int i1 = *(int *)(relocation_vars + 0x15C);
            mobys = D_L02_0015FFD8_n;
            *(unsigned char *)(mobys + (i0 << 8) + 0xBC) = 1;
            mobys += i1 << 8;
            mobys[0xBC] = 1;
            *(short *)(relocation_vars + 0x36) = 2;
            relocation_vars[8] = 1;
        }
    } else if (*(int *)(relocation_vars + 0x180) == 0) {
        if (is_point_inside_clip_volume(&hero.motion.pos, *(int *)(relocation_vars + 0x148)) != 0) {
            *(int *)(relocation_vars + 0x180) = 1;
            *(int *)(relocation_vars + 0x164) =
                queue_animation_update(0x15,
                                       0x7D0,
                                       initialize_numeric_hud_timer_and_base_dimensions,
                                       update_hud_counter_transition,
                                       draw_hud_counter,
                                       relocation_vars + 0x14C,
                                       100);
            *(int *)(relocation_vars + 0x160) =
                queue_animation_update(0x17,
                                       0x7D1,
                                       initialize_numeric_hud_timer_and_base_dimensions,
                                       update_hud_counter_transition,
                                       draw_hud_counter,
                                       relocation_vars + 0x150,
                                       7);
        }
    }
}
