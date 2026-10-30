#ifndef LOMBYTE_RNC_OVERLAY_HERO_TABLES_H
#define LOMBYTE_RNC_OVERLAY_HERO_TABLES_H

#include "types.h"

/* attack_records.kind selects the forward-motion profile, independently of
 * hero state IDs and the attack-record index. Keep the stored field as s32. */
enum HeroComboKind {
    HERO_COMBO_KIND_FRAME_GATED_FORWARD_ADVANCE = 0,
    HERO_COMBO_KIND_STAGED_FORWARD_DECELERATION = 1,
    HERO_COMBO_KIND_CONDITIONALLY_FRAME_GATED_FORWARD_ADVANCE = 2
};

/*
 * Per-level data tables the hero code reads (one copy per level overlay,
 * e.g. idle_fidgets, item_defs, attack_records in level 0).
 */

/*
 * 0x70-byte idle fidget entry of idle_fidgets, picked by
 * pick_idle_fidget(&index) with weight 1 / (interval * 60). The hero stores
 * the index, sets duration_frames from duration and then either enters hero
 * state 0x40 or calls set_hero_animation(unk48, 0, ...).
 */
typedef struct HeroTableEntry70 {
    u8  pad_0[0x44];
    s32 use_state_40; /* nonzero: hero_set_state(0x40, 1) instead of set_hero_animation */
    s32 unk48;        /* first argument of set_hero_animation */
    u8  pad_4C[0x4];
    s32 item_id; /* only while hero_slot_item_id(0) returns it; -1: any */
    u8  pad_54[0x4];
    f32 interval;        /* seconds; 0 = never picked */
    f32 duration;        /* seconds */
    s32 duration_frames; /* multiply_global_factor_ed64(duration) * 60 */
    u8  pad_64[0xC];
} HeroTableEntry70;

/*
 * 0x4C-byte row of item_defs, indexed by the id from
 * hero_slot_item_id(0); the hero plays anim when its moby is not already in it.
 */
typedef struct HeroAnimRow {
    u8  pad_0[0xC];
    s32 joint; /* hero.joint_matrices index the item moby follows */
    u8  pad_10[0x8];
    s32 unk18; /* tested != 0 */
    u8  pad_1C[0x8];
    s32 anim; /* animation id, compared with moby+0x53 */
    u8  pad_28[0x24];
} HeroAnimRow;

/*
 * 0x2C-byte row of attack_records, indexed by hero.attack_record. fC..f20 are
 * limits on the float hero.anim_frame (fC is subtracted from it; f10, f14,
 * f18 and f1C..f20 are compared with it).
 */
typedef struct HeroTableRow2C {
    s32 unk0;
    s32 kind; /* HeroComboKind; 1 also adds a turn-direction test */
    s32 unk8;
    s32 fC;
    s32 f10; /* frame from which a follow-up move may start */
    s32 f14;
    s32 f18;
    s32 f1C; /* f1C..f20: frames in which the wrench hits */
    s32 f20;
    u8  pad_24[0x8];
} HeroTableRow2C;

/*
 * 16-byte row of hero_speed_steps. The hero keeps a speed step while
 * ground_speed stays between lower and upper (times frame_time); a step sets
 * hero.target_speed from target_speed.
 */
typedef struct HeroSpeedStep {
    f32 lower;
    f32 upper;
    f32 target_speed;
    f32 unkC; /* compared with hero.target_speed */
} HeroSpeedStep;

extern HeroSpeedStep hero_speed_steps[] __asm__("D_L00_0017BDB8");

#endif /* LOMBYTE_RNC_OVERLAY_HERO_TABLES_H */
