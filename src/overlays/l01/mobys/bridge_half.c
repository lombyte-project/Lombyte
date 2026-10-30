/* Level 01 moby class bridge_half. */
#include "types.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"

extern s32 FUN_0022da68(s32, s32, void *);

#include "qcopy.h"

typedef union {
    u128 q;
    f32  f[4];
} PendVec4;

typedef struct {
    u8       pad0[0x60];
    PendVec4 pivot;
    f32      swing;
    s32      sound;
} PendulumVars;

typedef struct {
    u8            pad0[0x10];
    PendVec4      pos;
    u8            state;
    u8            pad21[0x23];
    f32           rot_x;
    f32           rot_z;
    u8            pad4c[0x2C];
    PendulumVars *vars;
    u8            pad7c[0x34];
    u8            id;
    u8            padB1[0xB];
    u8            trigger;
} PendulumMoby;

extern u8   D_0014C050[];
extern void release_voice_slot(int) __asm__("FUN_0022d798");

void bridge_half_update(PendulumMoby *self) __asm__("FUN_L01_002fb8a8");

void bridge_half_update(PendulumMoby *self) {
    enum {
        BRIDGE_HALF_INITIALIZE = 0,
        BRIDGE_HALF_WAIT_FOR_TRIGGER = 1,
        BRIDGE_HALF_REDUCE_SWING_TO_ZERO = 2
    };
    PendulumVars *pendulum_vars;
    PendVec4      arm;
    PendVec4      swing_offset;
    s32           voice_slot_index;
    u8           *voice;

    pendulum_vars = self->vars;
    switch (self->state) {
    case BRIDGE_HALF_INITIALIZE:
        qcopy(&pendulum_vars->pivot, &self->pos);
        pendulum_vars->swing = 1.0f;
        self->state = 1;
        pendulum_vars->sound = -1;
        break;
    case BRIDGE_HALF_WAIT_FOR_TRIGGER:
        if (self->trigger == 2 || D_0014C050[self->id + current_level_index * 16] == 0xFF) {
            self->state = 2;
            self->trigger = 0;
        }
        break;
    case BRIDGE_HALF_REDUCE_SWING_TO_ZERO:
        pendulum_vars->swing -= frame_time * 0.5f;
        if (pendulum_vars->swing < 0.0f) {
            pendulum_vars->swing = 0.0f;
        }
        if (pendulum_vars->swing == 0.0f) {
            voice_slot_index = pendulum_vars->sound;
            if (voice_slot_index != -1) {
                voice = ((u8 *)&voice_pool) + voice_slot_index * 0x70;
                if (*(void **)(voice + 0x88) == self && voice[0x74] != 0) {
                    release_voice_slot(voice_slot_index);
                }
            }
            pendulum_vars->sound = -1;
            self->state = 1;
        } else if (!voice_is_playing(self, pendulum_vars->sound)) {
            pendulum_vars->sound = FUN_0022da68(0, 4, self);
        }
        break;
    }
    self->rot_x = pendulum_vars->swing * 0.62831855f;
    arm.q = 0;
    arm.f[2] = -11.0f;
    fast_vec_add(&arm, &arm, &pendulum_vars->pivot);
    swing_offset.f[0] = fast_cos(self->rot_z) * 11.0f * fast_sin(self->rot_x);
    swing_offset.f[1] = fast_sin(self->rot_z) * 11.0f * fast_sin(self->rot_x);
    swing_offset.f[2] = fast_cos(self->rot_x) * 11.0f;
    fast_vec_add(&self->pos, &arm, &swing_offset);
}

extern float fast_cos(float);
extern float fast_sin(float);
