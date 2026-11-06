/* Ported from rac1-decomp (src/game/mobyutil.c, func_00213F28). */
#include "qcopy.h"
#include "rnc/data_labels.h"
#include "rnc/math/fast_math.h"

extern int find_or_allocate_moby_render_slot(int v) __asm__("func_0020CC18");

#include "rnc/mobys/animation/moby_anim.h"

extern void update_moby_animation_state(void *) __asm__("func_0020C880");
extern void FUN_0020ede8(void *, int);
/* Re-registers this MobyAnim in the shared slot table (D_001B2F40, via
   find_or_allocate_moby_render_slot) when it hasn't settled yet (anim_time > 0.025, or unk60/manips
   nonzero) or the caller forces it (arg4 & 4): builds a flags byte from
   arg4 bits 0/1 (0x100/0x200), hands it to FUN_0020ede8, snapshots this
   moby's unkF0 vector into the matching D_001B2C00 slot, remembers the
   old seq in saved_seq (unless it was already 0xFF), then marks seq 0xFF and
   frame = slot. Either way it then sets nextFrame/prevSeq from arg2/arg1,
   refreshes frame pointers (update_moby_animation_state), arms the timer (anim_speed = 1),
   resets anim_time, clears unk70 bit 1, stores 1/convert_integer_to_float(arg3) into
   unk5C, and copies sound out of
   pClass->seqs[arg1] into seq_sound. */
void blend_moby_animation_ex(MobyAnim *moby,
                             int       new_seq,
                             int       frame_index,
                             int       blend_frames,
                             int       options) __asm__("FUN_002130d8");

void blend_moby_animation_ex(

    MobyAnim *moby, int new_seq, int frame_index, int blend_frames, int options) {
    int           slot;
    int           flags;
    unsigned char oldSeq;
    float         scale;

    if (moby->anim_time > 0.025f || moby->pose_layers != 0 || moby->manips != 0 || (options & 4)) {
        slot = find_or_allocate_moby_render_slot((int)moby);
        if (slot >= 0) {
            flags = (options & 1) ? (slot | 0x100) : slot;
            FUN_0020ede8(moby, (options & 2) ? (flags | 0x200) : flags);
            qcopy(D_001B2C00 + slot * 0x10, moby->unkF0);
            oldSeq = moby->seq;
            if (oldSeq != 0xFF) {
                moby->saved_seq = oldSeq;
            }
            moby->seq = 0xFF;
            moby->frame = slot;
        }
    }
    moby->nextFrame = frame_index;
    moby->prevSeq = new_seq;
    update_moby_animation_state(moby);
    moby->anim_speed = 1.0f;
    scale = 1.0f / convert_integer_to_float(blend_frames);
    moby->anim_time = 0.0f;
    moby->anim_flags = (unsigned char)(moby->anim_flags & 0xFD);
    moby->anim_rate = scale;
    moby->seq_sound = moby->pClass->seqs[new_seq]->sound;
}

extern __typeof__(blend_moby_animation_ex) func_002130D8 __attribute__((alias("FUN_002130d8")));
