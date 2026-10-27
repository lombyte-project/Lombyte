/* advance_hero_animation split out of set_hero_animation.c: it reads D_L00_0015F5C4
   through $gp, while the functions there reach it with lui. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/overlay/level_labels.h"

struct AnimSeq670 {
    u8 pad0[0x10];
    u8 frame_count;   /* 0x10 */
    u8 unk11;
    u8 trigger_count; /* 0x12 */
    u8 pad13[9];
    void *frames[1];  /* 0x1C; the triggers follow the frames */
};
struct VoiceSlot670 {
    u8 pad0[0x7E];
    s16 sound;        /* 0x7E */
    u8 pad80[8];
    struct Moby *owner; /* 0x88 */
};

struct AnimClass670 {
    u8 pad0[0x48];
    struct AnimSeq670 *seqs[1]; /* 0x48 */
};
#define SEQ(m, s) (((struct AnimClass670 *)(m)->pclass)->seqs[s])

extern s32 D_L00_0015F5C4 __attribute__((sda));
extern u8 D_0013E550[];
extern void clear_hero_mapped_moby_animation_state(void) __asm__("FUN_L00_002321a8");
extern void clear_hero_queued_animation(void) __asm__("FUN_L00_00232628");
extern void sync_hero_mapped_moby_animation_frames(void) __asm__("FUN_L00_00232320");
extern void sync_hero_slot3_animation_speed(void) __asm__("FUN_L00_00232128");
extern void sync_hero_mapped_moby_animation_speed(void) __asm__("FUN_L00_002322c8");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern s32 allocate_voice_for_target_entry(s32, s32, struct Moby *) __asm__("FUN_0022da68");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
extern f32 compute_interpolated_record_value(struct Moby *) __asm__("FUN_0020c9e0");

/* Hero animation frame advance: steps the hero moby's frame position
   (unk54) by the rate in hero.anim_speed/unkA94 or a scripted table, moves to
   the next frame of the sequence each time it passes 1.0 (wrapping or
   jumping as hero.anim_loop_frame/unkAB4/unkAB8 request), fires the sequence's sound
   triggers crossed this frame, keeps the moby's looping sound in step,
   and records the distance moved (hero.anim_frame/unkAAC). */
void advance_hero_animation(void) __asm__("FUN_L00_00232670");
void advance_hero_animation(void) {
    struct Moby *hero_moby = hero.moby;
    s32 same;
    u8 frame0;
    f32 initial_anim_time;
    f32 dt;
    f32 previous_anim_frame;

    clear_hero_mapped_moby_animation_state();
    hero.anim_flags &= ~3;
    same = hero_moby->seq == hero_moby->prev_seq;
    frame0 = hero_moby->frame;
    initial_anim_time = hero_moby->anim_time;
    if (hero_moby->seq == hero_moby->prev_seq) {
        hero_moby->anim_time = initial_anim_time + hero.anim_speed * hero.anim_frame_rate;
    } else if (hero.anim_blend_curve >= 0) {
        hero_moby->anim_time = D_L00_0017BDF0[hero.anim_blend_curve][hero.anim_blend_step++];
    } else {
        hero_moby->anim_time = initial_anim_time + hero.anim_frame_rate;
    }
    if (0.99f < hero_moby->anim_time && hero_moby->anim_time < 1.01f)
        hero_moby->anim_time = 1.0f;
    if (-0.01f < hero_moby->anim_time && hero_moby->anim_time < 0.01f)
        hero_moby->anim_time = 0.0f;
    dt = hero_moby->anim_time - initial_anim_time;
    while (1.0f <= hero_moby->anim_time) {
        if (hero_moby->seq != hero_moby->prev_seq) {
            hero_moby->seq = hero_moby->prev_seq;
            hero_moby->anim_trigger_count = SEQ(hero_moby, hero_moby->seq)->trigger_count;
        }
        hero.anim_blend_curve = -1;
        hero.anim_flags |= 1;
        hero_moby->frame = hero_moby->prev_frame;
        hero_moby->cur_frame_data = hero_moby->prev_frame_data;
        hero_moby->prev_frame++;
        if (hero.queued_frame_request != 0) {
            hero.queued_frame_request = 0;
            hero.anim_frame_rate = 1.0f / 0.0f; /* +inf, from the level's literal pool */
            /* the just-cleared int, converted: retail holds this 0.0f in a
               register across the loop, which a literal 0.0f (stored from
               $zero, as in the next branch) does not give */
            hero_moby->anim_time = hero.queued_frame_request;
            hero_moby->prev_frame = hero.queued_anim_frame;
            hero_moby->prev_frame_data = SEQ(hero_moby, hero_moby->prev_seq)->frames[hero_moby->prev_frame];
            clear_hero_queued_animation();
            sync_hero_mapped_moby_animation_frames();
        } else if (hero.anim_loop_frame != -1 && hero.queued_anim_frame < hero_moby->prev_frame) {
            hero.anim_frame_rate = 0.33333334f;
            hero_moby->anim_time = 0.0f;
            hero_moby->prev_frame = hero.anim_loop_frame;
            hero_moby->prev_frame_data = SEQ(hero_moby, hero_moby->prev_seq)->frames[hero_moby->prev_frame];
            sync_hero_mapped_moby_animation_frames();
        } else {
            if (hero_moby->prev_frame >= SEQ(hero_moby, hero_moby->prev_seq)->frame_count) {
                hero_moby->prev_frame = 0;
                hero.anim_flags |= 2;
            }
            hero_moby->prev_frame_data = SEQ(hero_moby, hero_moby->prev_seq)->frames[hero_moby->prev_frame];
            hero_moby->anim_time -= 1.0f;
            hero_moby->anim_time /= hero.anim_frame_rate;
            hero.anim_frame_rate = *(f32 *)hero_moby->cur_frame_data;
            hero_moby->anim_time *= hero.anim_frame_rate;
        }
    }
    if (same && hero_moby->anim_trigger_count != 0) {
        s32 hi = (hero_moby->frame << 4) + truncate_float_to_s32(hero_moby->anim_time * 16.0f);
        s32 lo = (frame0 << 4) + truncate_float_to_s32(initial_anim_time * 16.0f);
        struct AnimSeq670 *seq = SEQ(hero_moby, hero_moby->seq);
        s32 *trig = (s32 *)&seq->frames[seq->frame_count];
        s32 i;
        for (i = 0; i < seq->trigger_count; i++, trig++) {
            s32 at = *trig >> 16;
            if (lo < at && !(hi < at)) {
                allocate_voice_for_target_entry(*trig & 0xFFFF, 0, hero_moby);
                break;
            }
        }
    }
    if (hero_moby->sound_slot != 0xFF) {
        struct VoiceSlot670 *active_voice_slot = (struct VoiceSlot670 *)(D_0013E550 + hero_moby->sound_slot * 0x70);
        if (active_voice_slot->owner != hero_moby) {
            hero_moby->sound_slot = 0xFF;
        } else if (active_voice_slot->sound != hero_moby->unk7C) {
            release_voice_slot(hero_moby->sound_slot);
            hero_moby->sound_slot = 0xFF;
        }
    } else if (hero_moby->unk7C != 0xFF && D_L00_0015F5C4 != 2 && D_L00_0015F5C4 != 6) {
        hero_moby->sound_slot = allocate_voice_for_target_entry(hero_moby->unk7C, 4, hero_moby);
    }
    sync_hero_slot3_animation_speed();
    sync_hero_mapped_moby_animation_speed();
    previous_anim_frame = hero.anim_frame;
    hero.anim_frame = compute_interpolated_record_value(hero_moby);
    if (previous_anim_frame <= hero.anim_frame)
        hero.anim_frame_delta = hero.anim_frame - previous_anim_frame;
    else
        hero.anim_frame_delta = dt;
    hero.anim_changing = hero_moby->seq != hero_moby->prev_seq;
}
