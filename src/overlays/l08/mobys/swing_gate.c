/* Level 08 moby classes 0x1d4, 0x1d5: swing gate. */
#include "types.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby.h"

#include "sda.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"

/* Pvars of a gate that swings with the progress of its driver moby. */
typedef struct {
    s32  driver;   /* 0x0: moby index whose pvars start with the 0..1 progress */
    f32  open_deg; /* 0x4: yaw at full progress, in degrees */
    f32  shut_deg; /* 0x8: yaw at zero progress, in degrees */
    s32  flag;     /* 0xC: byte of D_0013D388 set when fully open */
    Vec4 home;     /* 0x10: hinge position */
    s32  voice;    /* 0x20: creak voice slot, -1 if none */
} SwingGateVars;

extern int level_game_mode __asm__("D_L08_0015F5C4");

/* Voice table entries are 0x70 bytes; the volume sits 0x80 past an entry's start. */
typedef struct {
    u8  pad0[0x80];
    s32 volume; /* 0x80 */
} VoiceView;

extern u8           D_0013D388[];
extern void         vendor_volume(s32, s32) __asm__("FUN_L01_002a1968");
extern struct Moby *D_L08_0015FFD8;

/* Swings the gate to its driver's progress, fading a creak in while it
 * moves and out when it stops, and latches its flag once fully open. */
void swing_gate_update(struct Moby *moby) __asm__("FUN_L08_002ea4c0");

/* Moby classes 468, 469 */
void swing_gate_update(struct Moby *moby) {
    SwingGateVars *swing_gate_vars = (SwingGateVars *)moby->pvars;
    f32            progress = *(f32 *)D_L08_0015FFD8[swing_gate_vars->driver].pvars;
    Vec4           gate_rotation_matrix[4];
    Vec4           gate_position_offset;
    f32            yaw;

    gate_position_offset.q = 0;
    gate_position_offset.f[2] = 30.0f;
    if (level_game_mode == 2) {
        moby->visible = 0;
        moby->flags |= 1;
    } else {
        moby->visible = 1;
        moby->flags &= ~1;
    }
    if (moby->state == 0) {
        moby->state = 1;
        qcopy(&swing_gate_vars->home, &moby->pos);
        return;
    }
    if (moby->state != 1)
        return;
    yaw = fast_subtract_rotations(swing_gate_vars->open_deg * DEG_TO_RAD,
                                  swing_gate_vars->shut_deg * DEG_TO_RAD);
    yaw = fast_add_rotations(yaw * progress, swing_gate_vars->shut_deg * DEG_TO_RAD);
    if (fast_difference_between_rotations(moby->rot.y, yaw) != 0.0f) {
        moby->rot.y = yaw;
        if (moby->oclass == 0x1D4) {
            if (!voice_is_playing(moby, swing_gate_vars->voice)) {
                swing_gate_vars->voice = allocate_voice_for_target_entry(0, 4, moby);
                vendor_volume(swing_gate_vars->voice, 1);
            } else if (((VoiceView *)(((char *)&voice_pool) + swing_gate_vars->voice * 0x70))
                           ->volume < 0x400) {
                vendor_volume(
                    swing_gate_vars->voice,
                    ((VoiceView *)(((char *)&voice_pool) + swing_gate_vars->voice * 0x70))->volume +
                        0x400 / scale_game_frames(60));
            }
        }
    } else if (moby->oclass == 0x1D4 && voice_is_playing(moby, swing_gate_vars->voice)) {
        if (((VoiceView *)(((char *)&voice_pool) + swing_gate_vars->voice * 0x70))->volume <
            0x100) {
            release_voice_slot(swing_gate_vars->voice);
            swing_gate_vars->voice = -1;
        } else {
            vendor_volume(
                swing_gate_vars->voice,
                ((VoiceView *)(((char *)&voice_pool) + swing_gate_vars->voice * 0x70))->volume -
                    0x400 / scale_game_frames(60));
        }
    }
    euler_rotation_matrix(gate_rotation_matrix, &moby->rot);
    transform_vector(&gate_position_offset, &gate_position_offset, gate_rotation_matrix);
    fast_vec_add(&moby->pos, &swing_gate_vars->home, &gate_position_offset);
    if (progress == 1.0f && moby->oclass == 0x1D4) {
        D_0013D388[swing_gate_vars->flag + 0x30] = 1;
        moby->state = 2;
        allocate_voice_for_target_entry(1, 0, moby);
        if (moby->oclass == 0x1D4 && voice_is_playing(moby, swing_gate_vars->voice))
            release_voice_slot(swing_gate_vars->voice);
    }
}
