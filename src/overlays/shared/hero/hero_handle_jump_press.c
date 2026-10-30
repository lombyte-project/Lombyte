/* Hero: jump press handling, locomotion state, idle fidgets, item idle animations and the forward collision height check. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "asm.h"
#include "qcopy.h"

#include "rnc/overlay/hero_tables.h"
#include "rnc/globals.h"
#include "sda.h"
#include "rnc/math/random.h"
#include "rnc/math/fast_math.h"

extern HeroTableEntry70   idle_fidgets[] __asm__("D_L00_00179890") NOT_SDA;
extern s32 D_L00_00179AB0 NOT_SDA;
extern s32                hero_slot_item_id(s32) __asm__("FUN_L00_0020d498");
extern s32                FUN_L00_001efc70(void *, void *, s32, void *, s32);
extern f32                scale_time(f32) __asm__("FUN_001f96b0");

/* Idle fidget picker: lists the eligible entries of idle_fidgets, then
   picks one weighted by 1 / (interval * 60). Returns 1 and the index in
   *index when one is picked. At 1 hit point it offers only entry 4. */
s32 pick_idle_fidget(s32 *index) __asm__("FUN_L00_00229010");

s32 pick_idle_fidget(s32 *index) {
    s32               choices[20];
    Vec4              end;
    Vec4              start;
    HeroTableEntry70 *entry;
    Vec4             *startp, *endp;
    s32               i, item, count, j;
    s32              *next;
    HeroTableEntry70 *table;
    f32               total, pick;

    count = 0;
    if (hero.fidget_cooldown != 0)
        return 0;
    if (hero.health.hp == 1) {
        *index = 4;
        if (D_L00_00179AB0 != 0)
            return 0;
        if (!(hero.anim_flags & 2))
            return 0;
        return random_float_between(0.0f, 1.0f) < 0.33f;
    }
    i = 0;
    startp = &start;
    endp = &end;
    next = choices;
    for (; i < 5; i++) {
        table = idle_fidgets;
        entry = &table[i];
        if (entry->duration_frames != 0)
            continue;
        if (entry->interval == 0.0f)
            continue;
        item = hero_slot_item_id(0);
        if (entry->item_id != -1 && entry->item_id != item)
            continue;
        if (entry->use_state_40 != 0 && hero.ground_moby != 0)
            continue;
        if (i == 3) {
            if (current_level_index != 12)
                continue;
            if (hero_slot_item_id(0) == 23)
                continue;
            /* Room for it: nothing in the 8 units above the hero. */
            qcopy(startp, &hero.motion.body_center);
            qcopy(endp, startp);
            end.f[2] += 8.0f;
            if (FUN_L00_001efc70(startp, endp, 2, hero.moby, 0))
                continue;
        }
        if (i == 4) {
            if (hero.state.prev_control_mode != 1)
                continue;
            if (scale_game_frames(50) < hero.state_timer)
                continue;
            if (hero.health.hp >= 2)
                continue;
        }
        *next = i;
        count++;
        next++;
    }
    if (count == 0)
        return 0;
    total = 0.0f;
    for (i = 0; i < count; i++) {
        HeroTableEntry70 *choice = &idle_fidgets[choices[i]];
        total += 1.0f / (f32)(s32)(scale_time(choice->interval) * 60.0f);
    }
    pick = random_float_between(0.0f, 1.0f);
    if (pick < total) {
        for (j = 0; j < count; j++) {
            s32               picked = choices[j];
            HeroTableEntry70 *choice;
            *index = picked;
            choice = &idle_fidgets[picked];
            total -= 1.0f / (f32)(s32)(scale_time(choice->interval) * 60.0f);
            if (total < pick)
                break;
        }
        return 1;
    }
    return 0;
}

extern u8  D_0013F350_002293a8[] __asm__("D_0013F350");
extern s32 pad_held __asm__("D_0013CAE0") NOT_SDA;
extern s32 D_L00_0015F6DC __asm__("D_L00_0015F6DC");
extern int hero_set_state(int, int) __asm__("FUN_L00_002223f8");
void       set_hero_animation(s32, s32, f32) __asm__("FUN_L00_002323b8");
void       stick_target(s32, f32) __asm__("FUN_L00_002118c8");
void       hero_queue_sequence(s32, s32) __asm__("FUN_L00_002325e0");
void       FUN_L00_0025f730_002293a8(void *, f32) __asm__("FUN_L00_0025f730");
s32        hero_idle_sequence(s32) __asm__("FUN_L00_00205000");
void       update_hero_locomotion_state(void) __asm__("FUN_L00_002293a8");

void update_hero_locomotion_state(void) {
    u8 *hero_byte_base, *hero_run_base, *hero_stop_idle_base;
    s32 animation_frames_or_id;
    hero_byte_base = D_0013F350_002293a8;
    *(s32 *)(hero_byte_base + 0x2284) = 0;
    if (hero_byte_base[0x12E2] != 0 && frame_time * 0.5f < *(f32 *)(hero_byte_base + 0x164)) {
        hero_set_state(0x2F, 1);
        return;
    }
    stick_target(0, 1.0f);
    if (pad_held & 0xA) {
        hero_set_state(HERO_STATE_CROUCH, 0);
        set_hero_animation(0xD, 2, -2.0f);
        hero_queue_sequence(3, 0x12);
        return;
    }
    hero_run_base = D_0013F350_002293a8;
    if (0.3f < *(f32 *)(hero_run_base + 0x229C) && *(s32 *)(hero_run_base + 0x1C4) == 0) {
        if (hero_run_base[0x12E2] != 0 || hero_run_base[0x20A9] != 0) {
            hero_set_state(HERO_STATE_RUN, 1);
            return;
        }
        if (hero_set_state(HERO_STATE_RUN, 0)) {
            *(s32 *)(hero_run_base + 0x2088) = 1;
            animation_frames_or_id = scale_game_frames(8);
            if (scale_game_frames(5) < *(s32 *)(hero_run_base + 0x418))
                animation_frames_or_id = scale_game_frames(14);
            set_hero_animation(4, D_L00_0015F6DC, (f32)animation_frames_or_id);
            *(s16 *)(hero_run_base + 0x3BC) = 1;
        }
        return;
    }
    hero_stop_idle_base = D_0013F350_002293a8;
    if ((frame_time * 3.0f < *(f32 *)(hero_stop_idle_base + 0x168) &&
         scale_game_frames(3) < *(s32 *)(hero_stop_idle_base + 0x418)) ||
        *(s32 *)(hero_stop_idle_base + 0x2084) == 0x10) {
        if (hero_set_state(HERO_STATE_STOP, 0)) {
            set_hero_animation(5, 0, -1.0f);
            FUN_L00_0025f730_002293a8(&hero.motion.momentum, frame_time * 5.0f);
        }
        return;
    }
    if (scale_game_frames(12) < *(s32 *)(hero_stop_idle_base + 0x418) ||
        (*(s32 *)(hero_stop_idle_base + 0xA98) & 2)) {
        if (hero_set_state(HERO_STATE_IDLE, 0)) {
            if (hero_idle_sequence(0) == 0x54) {
                animation_frames_or_id = hero_idle_sequence(0);
                set_hero_animation(animation_frames_or_id, 0, (f32)scale_game_frames(15));
            } else if (*(s32 *)(hero_stop_idle_base + 0xA98) & 2) {
                set_hero_animation(hero_idle_sequence(0), 0, -2.0f);
            }
        }
    }
}

extern s32 pad_held_lui __asm__("D_0013CAE0") NOT_SDA;
extern u8  D_0013D4C2_lui __asm__("D_0013D4C2") NOT_SDA;
extern int get_hero_high_jump_input_window_frames(void) __asm__("FUN_L00_00221d98");
extern int check_pad_press_separation(int, void *, int, int) __asm__("FUN_L00_00266e00");
extern int pad_pressed_within(int, int, void *) __asm__("FUN_L00_00266d60");
extern int hero_slot_item_id(int) __asm__("FUN_L00_0020d498");
/* A second C name for hero_set_state: retail keeps the two calls apart. */
extern void hero_set_state_2(int, int) __asm__("FUN_L00_002223f8");

int hero_handle_jump_press(void) __asm__("FUN_L00_00229660");

int hero_handle_jump_press(void) {
    int input_history_separation_limit, input_history_window_padding, jump_press_history_limit;
    int next_state;

    if (hero.state.current == 0xE)
        return 0;
    input_history_separation_limit = get_hero_high_jump_input_window_frames();
    input_history_window_padding = scale_game_frames(5);
    if ((pad_held_lui & 0xA) && input_history_separation_limit > 0 &&
        check_pad_press_separation(0x40,
                                   (void *)0x1F000,
                                   input_history_window_padding + input_history_separation_limit,
                                   input_history_separation_limit)) {
        next_state = 0xB;
    found:
        hero_set_state(next_state, 1);
        return 1;
    }
    jump_press_history_limit = scale_game_frames(9);
    if (scale_game_frames(6) < hero.state_timer) {
        if (!pad_pressed_within(0x40, jump_press_history_limit, 0))
            return 0;
        if (hero_slot_item_id(3) == 2 && D_0013D4C2_lui != 0 && hero.pack_disabled == 0) {
            if ((2.5f < hero.wall_probe_distance || hero.wall_normal_pitch < 0.7853982f) &&
                0.7f < hero.stick_magnitude && frame_time * 0.9f < hero.motion.forward_speed) {
                next_state = 0xA;
                goto found;
            }
        }
        if (hero_slot_item_id(3) == 3 && hero.pack_disabled == 0 && 0.7f < hero.stick_magnitude &&
            (2.5f < hero.wall_probe_distance || hero.wall_probe_moby_in_class_range) &&
            frame_time * 0.9f < hero.motion.forward_speed) {
            if (hero.long_jump_lockout)
                return 1;
            next_state = 0x10;
            goto found;
        }
        if (hero_slot_item_id(3) == 3 && hero.pack_disabled == 0) {
            next_state = 0xD;
            goto found;
        }
        if (hero_slot_item_id(3) == 2 && hero.pack_disabled == 0 && D_0013D4C2_lui) {
            next_state = 0xF;
            goto found;
        }
        hero_set_state_2(7, 1);
        return 1;
    }
    return 0;
}

typedef unsigned int u128_229910_00229910 __attribute__((mode(TI), aligned(16)));

#include "rnc/math/asm_helpers.h"

typedef union {
    u128_229910_00229910 q_00229910;
    float                f[4];
} V_229910_00229910;

#include "rnc/overlay/collision.h"

extern int collision_line(void *, void *, int, void *, int) __asm__("FUN_001efa68");

int check_hero_forward_collision_height(void) __asm__("FUN_L00_00229910");

int check_hero_forward_collision_height(void) {
    struct Hero      *hero_ptr = &hero;
    V_229910_00229910 collision_line_start, collision_line_end;
    float             height_delta;
    if (hero_ptr->motion.flat_speed < hero_ptr->capsule_radius / (float)scale_game_frames(20))
        return 0;
    if (0.52359879f <
        fast_difference_between_rotations(
            fast_arc_tan(hero_ptr->motion.displacement.f[0], hero_ptr->motion.displacement.f[1]),
            hero_ptr->motion.rot.f[2]))
        return 0;
    qcopy(&collision_line_start, &hero_ptr->motion.pos);
    collision_line_start.f[0] +=
        fast_cos(hero_ptr->motion.rot.f[2]) * hero_ptr->capsule_radius * 0.75f;
    collision_line_start.f[1] +=
        fast_sin(hero_ptr->motion.rot.f[2]) * hero_ptr->capsule_radius * 0.75f;
    qcopy(&collision_line_end, &collision_line_start);
    collision_line_start.f[2] += 0.47f;
    collision_line_end.f[2] -= 1.0f;
    if (collision_line(
            &collision_line_start, &collision_line_end, 2, ((void *)hero_ptr->moby), 0)) {
        height_delta = collision_hit_z - hero_ptr->motion.pos.f[2];
        if (height_delta < 0.37f && -1.0f < height_delta)
            return 1;
    }
    return 0;
}

#include "rnc/overlay/item_defs.h"

s32  hero_slot_item_id(s32) __asm__("FUN_L00_0020d498");
void cancel_hero_item_animation(void) __asm__("FUN_L00_0020e698");
s32  try_start_hero_item_idle_animation(void) __asm__("FUN_L00_00229a98");

s32 try_start_hero_item_idle_animation(void) {
    struct Hero *hero_ptr = &hero;
    s32          id, k;
    if (hero_ptr->item_anim_active == 0 || hero_ptr->unk20AA == 0 || hero_ptr->unk22C8 != 0)
        return 0;
    id = hero_slot_item_id(0);
    if (id != hero_ptr->item_anim_item_id) {
        cancel_hero_item_animation();
        return 0;
    }
    k = item_defs[id].hero_animation;
    if (hero_set_state(HERO_STATE_IDLE, 0) == 0)
        return 0;
    set_hero_animation(k, 0, (f32)scale_game_frames(0xB));
    if (((s32)hero_ptr->held_item_anim_blend[0]) != 0)
        hero_ptr->unkD14 = 1;
    hero_ptr->item_hold_anim = k;
    return 1;
}

s32 D_L01_00179F30 NOT_SDA = 0;
s32 D_L02_0017A0B0 NOT_SDA = 0;
s32 D_L03_00179BB0 NOT_SDA = 0;
s32 D_L04_00179C30 NOT_SDA = 0;
s32                D_L04_0015F6DC = 0;
s32 D_L05_00179F10 NOT_SDA = 0;
s32 D_L06_0017A230 NOT_SDA = 0;
s32 D_L07_00179B30 NOT_SDA = 0;
s32                D_L07_0015F6DC = 0;
s32 D_L08_0017A2B0 NOT_SDA = 0;
s32 D_L09_00179C30 NOT_SDA = 0;
s32                D_L09_0015F6DC = 0;
s32 D_L10_00179F30 NOT_SDA = 0;
s32                D_L10_0015F6DC = 0;
s32 D_L11_0017A4B0 NOT_SDA = 0;
s32 D_L12_00179F30 NOT_SDA = 0;
s32 D_L13_00179DB0 NOT_SDA = 0;
s32                D_L13_0015F6DC = 0;
s32 D_L14_0017A1B0 NOT_SDA = 0;
s32 D_L15_0017A0B0 NOT_SDA = 0;
s32 D_L16_00179F10 NOT_SDA = 0;
s32 D_L17_0017A3B0 NOT_SDA = 0;
s32 D_L18_0017A4B0 NOT_SDA = 0;

s32 D_L00_00179AB0 NOT_SDA = 0;
