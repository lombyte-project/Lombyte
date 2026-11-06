/* Ported from rac1-decomp (src/game/mobyutil.c, func_00213DE0). */
#include "qcopy.h"
#include "rnc/data_labels.h"
#include "rnc/game/game_globals.h"
#include "rnc/math/fast_math.h"

extern int find_or_allocate_moby_render_slot(int v) __asm__("func_0020CC18");

#include "rnc/mobys/animation/moby_anim.h"

extern void update_moby_animation_state(void *) __asm__("func_0020C880");
extern void FUN_0020ede8(void *, int);
/* As func_00213F28 below, for a plain sequence change: clamp the frame
   to the sequence's last one; if the moby hasn't settled, park the
   current pose in a D_001B2C00 blend slot (seq 0xFF, frame = slot). Then
   set the new sequence and frame and arm the blend timer from arg3. */
void blend_moby_animation(MobyAnim *moby,
                          int       new_seq,
                          int       frame_index,
                          int       blend_frames) __asm__("FUN_00212f90");

void blend_moby_animation(MobyAnim *moby, int new_seq, int frame_index, int blend_frames) {
    int           frame_count = moby->pClass->seqs[new_seq]->nframes;
    int           slot;
    unsigned char oldSeq;
    float         scale;

    if (frame_index >= frame_count) {
        frame_index = frame_count - 1;
    }
    if (moby->anim_time > 0.025f || moby->unk60 != 0 || moby->manips != 0) {
        slot = find_or_allocate_moby_render_slot((int)moby);
        if (slot >= 0) {
            FUN_0020ede8(moby, slot | 0x300);
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
    moby->unk70 = (unsigned char)(moby->unk70 & 0xFD);
    moby->unk5C = scale;
    moby->seq_sound = moby->pClass->seqs[new_seq]->sound;
}

extern __typeof__(blend_moby_animation) func_00212F90 __attribute__((alias("FUN_00212f90")));
