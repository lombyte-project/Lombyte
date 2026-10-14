/* FUN_L00_00232670 split out of 00231d08.c: it reads D_L00_0015F5C4
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
extern void FUN_L00_002321a8(void);
extern void FUN_L00_00232628(void);
extern void FUN_L00_00232320(void);
extern void FUN_L00_00232128(void);
extern void FUN_L00_002322c8(void);
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern s32 FUN_0022da68(s32, s32, struct Moby *);
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
extern f32 FUN_0020c9e0(struct Moby *);

/* Hero animation frame advance: steps the hero moby's frame position
   (unk54) by the rate in hero.anim_speed/unkA94 or a scripted table, moves to
   the next frame of the sequence each time it passes 1.0 (wrapping or
   jumping as hero.unkAB0/unkAB4/unkAB8 request), fires the sequence's sound
   triggers crossed this frame, keeps the moby's looping sound in step,
   and records the distance moved (hero.anim_frame/unkAAC). */
void FUN_L00_00232670(void) {
    struct Moby *m = hero.moby;
    s32 same;
    u8 frame0;
    f32 t0;
    f32 dt;
    f32 prev;

    FUN_L00_002321a8();
    hero.unkA98 &= ~3;
    same = m->seq == m->prev_seq;
    frame0 = m->frame;
    t0 = m->anim_time;
    if (m->seq == m->prev_seq) {
        m->anim_time = t0 + hero.anim_speed * hero.unkA94;
    } else if (hero.unkAA0 >= 0) {
        m->anim_time = D_L00_0017BDF0[hero.unkAA0][hero.unkAA4++];
    } else {
        m->anim_time = t0 + hero.unkA94;
    }
    if (0.99f < m->anim_time && m->anim_time < 1.01f)
        m->anim_time = 1.0f;
    if (-0.01f < m->anim_time && m->anim_time < 0.01f)
        m->anim_time = 0.0f;
    dt = m->anim_time - t0;
    while (1.0f <= m->anim_time) {
        if (m->seq != m->prev_seq) {
            m->seq = m->prev_seq;
            m->anim_trigger_count = SEQ(m, m->seq)->trigger_count;
        }
        hero.unkAA0 = -1;
        hero.unkA98 |= 1;
        m->frame = m->prev_frame;
        m->cur_frame_data = m->prev_frame_data;
        m->prev_frame++;
        if (hero.unkAB8 != 0) {
            hero.unkAB8 = 0;
            hero.unkA94 = 1.0f / 0.0f; /* +inf, from the level's literal pool */
            /* the just-cleared int, converted: retail holds this 0.0f in a
               register across the loop, which a literal 0.0f (stored from
               $zero, as in the next branch) does not give */
            m->anim_time = hero.unkAB8;
            m->prev_frame = hero.unkAB4;
            m->prev_frame_data = SEQ(m, m->prev_seq)->frames[m->prev_frame];
            FUN_L00_00232628();
            FUN_L00_00232320();
        } else if (hero.unkAB0 != -1 && hero.unkAB4 < m->prev_frame) {
            hero.unkA94 = 0.33333334f;
            m->anim_time = 0.0f;
            m->prev_frame = hero.unkAB0;
            m->prev_frame_data = SEQ(m, m->prev_seq)->frames[m->prev_frame];
            FUN_L00_00232320();
        } else {
            if (m->prev_frame >= SEQ(m, m->prev_seq)->frame_count) {
                m->prev_frame = 0;
                hero.unkA98 |= 2;
            }
            m->prev_frame_data = SEQ(m, m->prev_seq)->frames[m->prev_frame];
            m->anim_time -= 1.0f;
            m->anim_time /= hero.unkA94;
            hero.unkA94 = *(f32 *)m->cur_frame_data;
            m->anim_time *= hero.unkA94;
        }
    }
    if (same && m->anim_trigger_count != 0) {
        s32 hi = (m->frame << 4) + truncate_float_to_s32(m->anim_time * 16.0f);
        s32 lo = (frame0 << 4) + truncate_float_to_s32(t0 * 16.0f);
        struct AnimSeq670 *seq = SEQ(m, m->seq);
        s32 *trig = (s32 *)&seq->frames[seq->frame_count];
        s32 i;
        for (i = 0; i < seq->trigger_count; i++, trig++) {
            s32 at = *trig >> 16;
            if (lo < at && !(hi < at)) {
                FUN_0022da68(*trig & 0xFFFF, 0, m);
                break;
            }
        }
    }
    if (m->sound_slot != 0xFF) {
        struct VoiceSlot670 *v = (struct VoiceSlot670 *)(D_0013E550 + m->sound_slot * 0x70);
        if (v->owner != m) {
            m->sound_slot = 0xFF;
        } else if (v->sound != m->unk7C) {
            release_voice_slot(m->sound_slot);
            m->sound_slot = 0xFF;
        }
    } else if (m->unk7C != 0xFF && D_L00_0015F5C4 != 2 && D_L00_0015F5C4 != 6) {
        m->sound_slot = FUN_0022da68(m->unk7C, 4, m);
    }
    FUN_L00_00232128();
    FUN_L00_002322c8();
    prev = hero.anim_frame;
    hero.anim_frame = FUN_0020c9e0(m);
    if (prev <= hero.anim_frame)
        hero.unkAAC = hero.anim_frame - prev;
    else
        hero.unkAAC = dt;
    hero.unkA9C = m->seq != m->prev_seq;
}
