/* Level 16 moby class local_bounds_command_trigger (0x5a1, named grindrail_gate in the class record): the grindrail_gate behaviour with its own bounds and the glow colours swapped. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "qcopy.h"
#include "sda.h"
#include "rnc/audio/voice_alloc.h"

extern int scale_game_frames(int);
extern int D_L16_0015F5CC;

/* Moves the moby through its trigger states and checks its local bounds. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E66C0), where it is exact; names translated to the US level program. */

extern int FUN_001fa6e0_q(float, int, int) __asm__("FUN_001fa6e0");

extern float D_L16_001D9768[] NOT_SDA;
int          D_L16_00161DE4 = 600;
int          D_L16_00161DE8 = 0x80202020;
int          D_L16_00161DEC = 0x80808080;
extern char  D_0013F3D0[] NOT_SDA;
void update_local_bounds_command_trigger(struct Moby *trigger_moby) __asm__("FUN_L16_002e5258");

void update_local_bounds_command_trigger(struct Moby *trigger_moby) {
    enum {
        GRINDRAIL_TRIGGER_INITIALIZE = 0,
        GRINDRAIL_TRIGGER_CHECK_LOCAL_BOUNDS = 1,
        GRINDRAIL_TRIGGER_WAIT_FOR_COMMAND_TIMEOUT = 2
    };
    char *command_timer = (char *)trigger_moby->pvars;
    char *bounds_check_position;
    float pos[16];
    float bounds_check_offset_from_trigger[4];
    float glow_cycle_phase;
    int   period = scale_game_frames(30);
    glow_cycle_phase =
        convert_integer_to_float(D_L16_0015F5CC % period) / convert_integer_to_float(period);
    trigger_moby->glow_color =
        FUN_001fa6e0_q(fast_sin(glow_cycle_phase * 6.28318f - 3.14159f) * 0.5f + 0.5f,
                       D_L16_00161DE8,
                       D_L16_00161DEC);
    switch (trigger_moby->state) {
    case GRINDRAIL_TRIGGER_INITIALIZE:
        trigger_moby->state = 1;
        trigger_moby->update_dist = 0xFF;
        break;
    case GRINDRAIL_TRIGGER_CHECK_LOCAL_BOUNDS:
        bounds_check_position = D_0013F3D0;
        if (fast_vec_dist(&trigger_moby->pos, bounds_check_position) < 16.0f) {
            transpose_rotation_matrix(pos, &trigger_moby->rot_row0);
            fast_vec_sub(
                bounds_check_offset_from_trigger, bounds_check_position, &trigger_moby->pos);
            bounds_check_offset_from_trigger[3] = 0.0f;
            transform_vector(
                bounds_check_offset_from_trigger, bounds_check_offset_from_trigger, pos);
            if (D_L16_001D9768[0] < bounds_check_offset_from_trigger[0] &&
                bounds_check_offset_from_trigger[0] < D_L16_001D9768[1] &&
                D_L16_001D9768[2] < bounds_check_offset_from_trigger[1] &&
                bounds_check_offset_from_trigger[1] < D_L16_001D9768[3] &&
                D_L16_001D9768[4] < bounds_check_offset_from_trigger[2] &&
                bounds_check_offset_from_trigger[2] < D_L16_001D9768[5]) {
                allocate_voice_for_target_entry(1, 0, trigger_moby);
                trigger_moby->command = 1;
                trigger_moby->state = 2;
                *(int *)command_timer = scale_game_frames(D_L16_00161DE4);
            }
        }
        break;
    case GRINDRAIL_TRIGGER_WAIT_FOR_COMMAND_TIMEOUT:
        if (fast_dec_timer((int *)command_timer)) {
            trigger_moby->state = 1;
            trigger_moby->command = 0;
        }
        break;
    }
}
