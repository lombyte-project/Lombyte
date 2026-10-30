/* Level 15 poison vessel spawner moby class. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/l15/mobys/poison_vessel.h"

extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");

extern short poison_vessel_spawn_interval __asm__("D_L15_0016207C");

void update_poison_vessel_spawner(struct Moby *moby) __asm__("FUN_L15_002eabd8");

void update_poison_vessel_spawner(struct Moby *moby) {
    enum {
        INITIALIZE_SPAWN_TIMER = 0,
        WAIT_FOR_SPAWN_TIMER = 1
    };
    int *data = (int *)moby->pvars;
    switch (moby->state) {
    case INITIALIZE_SPAWN_TIMER:
        data[0] = truncate_float_to_s32(multiply_global_factor_ed64(*(float *)(data + 1) * 60.0f));
        moby->state = 1;
        moby->update_dist = 0xFF;
        break;
    case WAIT_FOR_SPAWN_TIMER:
        if (fast_dec_timer(data)) {
            data[0] = truncate_float_to_s32(
                multiply_global_factor_ed64(*(float *)&poison_vessel_spawn_interval * 60.0f));
            spawn_poison_vessel(moby, &moby->pos);
        }
        break;
    }
}
