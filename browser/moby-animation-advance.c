/* advance_moby_animation for the browser builds: one C version of the EE routine, compiled into
   the game page's level.wasm (browser/level.c calls it once per moby per frame) and into the
   spike (browser/spike/ee-helpers.c binds it to 0x20d580 and the level copy at 0x250480).
   The callers pass the two sound callees and the frame counter, which differ between the boot
   program and each level program. */
#include "types.h"
#include "moby_animation_advance.h"

/* The EE saturates cvt.w.s; wasm's truncation traps out of range. */
static s32 saturating_s32(f32 x) {
    if (x != x || x >= 2147483648.0f) return 0x7FFFFFFF;
    if (x <= -2147483648.0f) return (s32)0x80000000;
    return (s32)x;
}

/* advance_moby_animation, 0x20d580 (186 instructions, ending at 0x20d864), and the level
   program's copy at 0x250480, which differs only in its two internal jumps, its two callees and
   the frame counter it reads. Fields are struct Moby (include/rnc/mobys/moby.h) and AnimSeq
   (include/rnc/mobys/animation/moby_anim.h): 0x24 pclass, 0x50 frame, 0x51 next frame,
   0x52/0x53 the two sequence bytes (they differ while a blend runs), 0x54 anim_time,
   0x58 anim_speed, 0x5C the current frame's duration, 0x68/0x6C frame data, 0x70 anim_flags,
   0x7C..0x7E the voice bytes and anim_trigger_count; pclass + 0x48 holds seqs[], a sequence
   has nframes at 0x10, a trigger count at 0x12, a rate word at 0x18, frame pointers from 0x1C
   and then 4-byte (id, time) trigger records.

   The `.word 0x...2e` instructions are dsub on float bit patterns sign-extended to 64 bits, so
   they are signed integer compares of the bits; they are written that way here.
   EE cvt.w.s truncates and saturates (saturating_s32). madd.s is computed as a separate
   multiply and add. */
#define AM_U8(o) (*(u8 *)(m + (o)))
#define AM_U32(o) (*(u32 *)(m + (o)))
#define AM_F32(o) (*(f32 *)(m + (o)))
static s32 am_bits(f32 f) { s32 b; __builtin_memcpy(&b, &f, 4); return b; }
static f32 am_float(u32 b) { f32 f; __builtin_memcpy(&f, &b, 4); return f; }

s32 rnc_advance_moby_animation(u8 *m, s32 frame_counter, RncAnimationTrigger trigger, RncAnimationVoice voice) {
    const s32 voice_none = 0xffff;
    u32 flags = 0;
    s32 voice_bytes = (s32)(AM_U32(0x7c) & 0x00ffffff);
    const u8 *seq;
    s32 s52 = AM_U8(0x52), s53 = AM_U8(0x53);
    s32 frame, next, old_frame;
    f32 dur = AM_F32(0x5c), t, old_time;

    if (AM_U32(0x5c) == 0 || AM_U32(0x58) == 0) goto voice_check;
    t = s52 != s53 ? AM_F32(0x54) + dur : AM_F32(0x54) + AM_F32(0x58) * dur;
    frame = AM_U8(0x50);
    next = AM_U8(0x51);
    old_time = AM_F32(0x54);
    old_frame = frame;
    seq = *(const u8 **)(*(u8 **)(m + 0x24) + 0x48 + 4 * s53);

    if (am_bits(t) >= 0x3F7F0000 && am_bits(t) <= 0x3F808000) {
        t = 1.0f; /* snapped to the frame boundary */
        goto forward;
    }
    if (am_bits(t) > 0x3F800000) goto forward;
    if (am_bits(t) < 0) goto backward;
    AM_F32(0x54) = t;
    AM_U8(0x70) = (u8)flags;
    if (voice_bytes != voice_none) goto triggers;
    return 0;

forward:
    for (;;) {
        u32 rate, frame_data;
        s32 nframes;
        t = t - 1.0f;
        frame = next;
        next = next + 1;
        frame_data = AM_U32(0x6c);
        if (s52 != s53) { /* the blend into s53 is complete */
            AM_U8(0x7e) = seq[0x12];
            s52 = s53;
            voice_bytes = voice_none;
        }
        t = t / dur;
        flags |= 1;
        nframes = seq[0x10];
        rate = *(const u32 *)(seq + 0x18);
        {
            u32 next_data = *(const u32 *)(seq + 0x1c + 4 * next);
            if (nframes - next <= 0) {
                next = 0;
                flags |= 2;
                next_data = *(const u32 *)(seq + 0x1c);
            }
            dur = rate != 0 ? am_float(rate) : *(const f32 *)frame_data;
            t = t * dur;
            AM_U8(0x52) = (u8)s52;
            AM_U8(0x50) = (u8)frame;
            AM_U8(0x51) = (u8)next;
            AM_U32(0x68) = frame_data;
            AM_U32(0x6c) = next_data;
            AM_F32(0x5c) = dur;
        }
        AM_F32(0x54) = t;
        if (!((s64)am_bits(t) - 0x3F800000 > 0)) break;
    }
    AM_U8(0x70) = (u8)flags;
    if (voice_bytes != voice_none) goto triggers;
    return 0;

backward:
    for (;;) {
        u32 rate, frame_data, old_data;
        t = t / dur;
        next = frame;
        frame = frame - 1;
        old_data = AM_U32(0x68);
        flags |= 1;
        rate = *(const u32 *)(seq + 0x18);
        if (frame < 0) {
            frame = seq[0x10] - 1;
            flags |= 2;
        }
        frame_data = *(const u32 *)(seq + 0x1c + 4 * frame);
        dur = rate != 0 ? am_float(rate) : *(const f32 *)frame_data;
        t = t * dur;
        AM_U32(0x68) = frame_data;
        AM_U8(0x50) = (u8)frame;
        AM_U8(0x51) = (u8)next;
        t = t + 1.0f;
        AM_U32(0x6c) = old_data;
        AM_F32(0x5c) = dur;
        AM_F32(0x54) = t;
        if (am_bits(t) >= 0) break;
    }
    AM_U8(0x70) = (u8)flags;
    if (voice_bytes != voice_none) goto triggers;
    return 0;

triggers:
    AM_U8(0x70) = (u8)flags;
    if (s52 == s53) {
        s32 count = voice_bytes >> 16;
        if (count > 0) {
            const s32 now = frame * 16 + saturating_s32(t * 16.0f);
            const s32 before = old_frame * 16 + saturating_s32(old_time * 16.0f);
            if (now - before > 0) {
                const u8 *record = seq + 0x1c + 4 * seq[0x10];
                for (; count > 0; count--, record += 4) {
                    const s32 time = *(const u16 *)(record + 2);
                    if (time - (before + 1) >= 0 && now - time >= 0) {
                        return trigger(*(const u16 *)record, 0, m);
                    }
                }
            }
        }
    }

voice_check:
    AM_U8(0x70) = (u8)flags;
    if ((voice_bytes & 0xffff) == 0xffff) return 0;
    if (((voice_bytes >> 8) & 0xff) != 0xff ||
        ((s32)(((u32)m >> 8) & 3) == (frame_counter & 3))) {
        voice(m);
    }
    return 0;
}
#undef AM_U8
#undef AM_U32
#undef AM_F32
