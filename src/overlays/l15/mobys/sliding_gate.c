/* Level 15 sliding gate moby class. */
#include "types.h"
#include "rnc/overlay/level_save_state.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/rendering/clip_volume.h"

extern struct Moby *l15_moby_array __asm__("D_L15_0015FFD8");

typedef struct {
    Vec4 home;
    s32  open_volume;
    s32  close_volume;
    f32  travel;
    s32  trigger;
    s32  sound;
    f32  duration;
    s32  lock;
} MovingVars;

typedef struct {
    char         pad00[0x74];
    u8           active;
    char         pad75[0x13];
    struct Moby *owner;
} VoiceSlot_29aff0;

extern u8 D_0013F3D0[];

extern char D_L15_001673C0[];

extern char D_0014C190[];

extern int D_L15_001BAAD0[];

void sliding_gate_update(struct Moby *gate_moby) __asm__("FUN_L15_0029aff0");

/* Moby class 67 */
void sliding_gate_update(struct Moby *gate_moby) {
    enum {
        GATE_INITIALIZE = 0,
        GATE_WAIT_TO_OPEN = 1,
        GATE_OPENING = 2,
        GATE_OPEN = 3,
        GATE_CLOSING = 4
    };
    MovingVars       *gate_vars = (MovingVars *)gate_moby->pvars;
    Vec4              offset;
    Vec4              pos;
    s32               slot;
    VoiceSlot_29aff0 *voice_slot;
    s32               trigger_active;
    u32               mask;
    u32              *bits;

    switch (gate_moby->state) {
    case GATE_INITIALIZE:
        gate_vars->home.q = *(u128 *)&gate_moby->pos;
        gate_moby->state = 1;
        return;
    case GATE_WAIT_TO_OPEN:
        trigger_active = 0;
        gate_vars->travel = 0;
        if (gate_vars->trigger != -1) {
            struct Moby *gate_trigger_moby =
                (struct Moby *)((char *)l15_moby_array + gate_vars->trigger * 0x100);
            if (gate_trigger_moby->oclass == 0x4b9) {
                if (gate_trigger_moby->state == 4) {
                    trigger_active = 1;
                    goto check_lock;
                }
            }
            if (gate_trigger_moby->oclass == 0x596) {
                trigger_active = gate_trigger_moby->state == 4;
            }
        }
    check_lock:
        if (gate_vars->lock != 0) {
            if (l15_save_state.cleared[(s16)gate_moby->save_id] != 0 ||
                ((*(s32 *)(D_0014C190 +
                           (current_level_index * 0x100 + ((s16)gate_moby->save_id >> 5) * 4)) >>
                  (gate_moby->save_id & 31)) &
                 1) != 0)
                trigger_active = 1;
        }
        if (is_point_inside_clip_volume(D_0013F3D0, gate_vars->open_volume)) {
            gate_moby->state = 2;
        } else if (trigger_active) {
            gate_moby->state = 2;
        }
        return;
    case GATE_OPENING:
        if (((VoiceSlot_29aff0 *)(((char *)&voice_pool) + gate_vars->sound * 0x70))->owner !=
                gate_moby ||
            ((VoiceSlot_29aff0 *)(((char *)&voice_pool) + gate_vars->sound * 0x70))->active == 0)
            gate_vars->sound = allocate_voice_for_target_entry(0, 4, (int)gate_moby);
        if (gate_vars->duration <
            (gate_vars->travel = gate_vars->travel + (gate_vars->duration / 0.6f) * frame_time)) {
            slot = gate_vars->sound;
            if (slot != -1) {
                if (((VoiceSlot_29aff0 *)(((char *)&voice_pool) + slot * 0x70))->owner ==
                        gate_moby &&
                    ((VoiceSlot_29aff0 *)(((char *)&voice_pool) + slot * 0x70))->active != 0)
                    release_voice_slot(slot);
            }
            gate_vars->travel = gate_vars->duration;
            gate_vars->sound = -1;
            if (gate_vars->trigger != -1 || gate_vars->lock != 0) {
                mask = 1U << (gate_moby->save_id & 31);
                bits = (u32 *)(D_0014C190 +
                               (((s16)gate_moby->save_id >> 5) * 4 + current_level_index * 0x100));
                *bits |= mask;
                D_L15_001BAAD0[((s16)gate_moby->save_id >> 5)] |= 1U << (gate_moby->save_id & 31);
                gate_moby->state = 5;
            } else {
                gate_moby->state = 3;
            }
        }
        offset.f[0] = fast_cos(gate_moby->rot.z) * -gate_vars->travel;
        offset.f[1] = fast_sin(gate_moby->rot.z) * -gate_vars->travel;
        offset.f[2] = 0.0f;
        fast_vec_add(&pos, &gate_vars->home, &offset);
        *(u128 *)&gate_moby->pos = pos.q;
        return;
    case GATE_OPEN:
        if (is_point_inside_clip_volume(D_0013F3D0, gate_vars->close_volume) ||
            is_point_inside_clip_volume(D_L15_001673C0, gate_vars->close_volume))
            return;
        gate_moby->state = 4;
        return;
    case GATE_CLOSING:
        if (((VoiceSlot_29aff0 *)(((char *)&voice_pool) + gate_vars->sound * 0x70))->owner !=
                gate_moby ||
            ((VoiceSlot_29aff0 *)(((char *)&voice_pool) + gate_vars->sound * 0x70))->active == 0)
            gate_vars->sound = allocate_voice_for_target_entry(0, 4, (int)gate_moby);
        if ((gate_vars->travel = gate_vars->travel - (gate_vars->duration / 0.6f) * frame_time) <
            0.0f) {
            slot = gate_vars->sound;
            if (slot != -1 &&
                ((VoiceSlot_29aff0 *)(((char *)&voice_pool) + slot * 0x70))->owner == gate_moby &&
                ((VoiceSlot_29aff0 *)(((char *)&voice_pool) + slot * 0x70))->active != 0)
                release_voice_slot(slot);
            gate_vars->travel = 0;
            gate_vars->sound = -1;
            gate_moby->state = 1;
        }
        offset.f[0] = fast_cos(gate_moby->rot.z) * -gate_vars->travel;
        offset.f[1] = fast_sin(gate_moby->rot.z) * -gate_vars->travel;
        offset.f[2] = 0.0f;
        fast_vec_add(&pos, &gate_vars->home, &offset);
        *(u128 *)&gate_moby->pos = pos.q;
        if (is_point_inside_clip_volume(D_0013F3D0, gate_vars->open_volume) ||
            is_point_inside_clip_volume(D_L15_001673C0, gate_vars->open_volume))
            gate_moby->state = 2;
        return;
    default:
        return;
    }
}
