/* Level 16 moby class grindrail_gate (0x59f): glows, and while the hero is inside its local bounds sets its command for a while. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "qcopy.h"

extern int scale_game_frames(int);
/* Moves the moby through its trigger states and checks its local bounds. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E6478), where it is exact; names translated to the US level program. */

extern float grindrail_gate_local_trigger_bounds[] __asm__("D_L16_001D9750");
extern int   D_L16_0015F5CC;
extern int   fast_tween_color(float, int, int) __asm__("FUN_001fa6e0");
extern int   func_0022ED80_6478(int, int, void *) __asm__("FUN_0022da68");

int  D_L16_00161DD8 = 600;
int  D_L16_00161DDC = 0x80808080;
int  D_L16_00161DE0 = 0x80202020;
void grindrail_gate_update(struct Moby *gate_moby) __asm__("FUN_L16_002e5010");

/* Moby class 1439 */
void grindrail_gate_update(struct Moby *gate_moby) {
    enum {
        GRINDRAIL_GATE_INITIALIZE = 0,
        GRINDRAIL_GATE_CHECK_HERO_BOUNDS = 1,
        GRINDRAIL_GATE_WAIT_FOR_COMMAND_TIMEOUT = 2
    };
    char *command_timer = (char *)gate_moby->pvars;
    char *hero_position;
    float pos[16];
    float hero_offset_from_gate[4];
    float glow_cycle_phase;
    int   period = scale_game_frames(30);
    glow_cycle_phase =
        convert_integer_to_float(D_L16_0015F5CC % period) / convert_integer_to_float(period);
    gate_moby->glow_color =
        fast_tween_color(fast_sin(glow_cycle_phase * 6.28318f - 3.14159f) * 0.5f + 0.5f,
                         D_L16_00161DDC,
                         D_L16_00161DE0);
    switch (gate_moby->state) {
    case GRINDRAIL_GATE_INITIALIZE:
        gate_moby->state = 1;
        gate_moby->update_dist = 0xFF;
        break;
    case GRINDRAIL_GATE_CHECK_HERO_BOUNDS:
        hero_position = (char *)&hero.motion.pos;
        if (fast_vec_dist(&gate_moby->pos, hero_position) < 16.0f) {
            transpose_rotation_matrix(pos, &gate_moby->rot_row0);
            fast_vec_sub(hero_offset_from_gate, hero_position, &gate_moby->pos);
            transform_vector(hero_offset_from_gate, hero_offset_from_gate, pos);
            if (grindrail_gate_local_trigger_bounds[0] < hero_offset_from_gate[0] &&
                hero_offset_from_gate[0] < grindrail_gate_local_trigger_bounds[1] &&
                grindrail_gate_local_trigger_bounds[2] < hero_offset_from_gate[1] &&
                hero_offset_from_gate[1] < grindrail_gate_local_trigger_bounds[3] &&
                grindrail_gate_local_trigger_bounds[4] < hero_offset_from_gate[2] &&
                hero_offset_from_gate[2] < grindrail_gate_local_trigger_bounds[5]) {
                gate_moby->command = 1;
                gate_moby->state = 2;
                *(int *)command_timer = scale_game_frames(D_L16_00161DD8);
                func_0022ED80_6478(1, 0, gate_moby);
            }
        }
        break;
    case GRINDRAIL_GATE_WAIT_FOR_COMMAND_TIMEOUT:
        if (fast_dec_timer((int *)command_timer)) {
            gate_moby->state = 1;
            gate_moby->command = 0;
        }
        break;
    }
}
