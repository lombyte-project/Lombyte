/* Level 18 moby class inert_moby (0x598): sets state 1 and flags 3 once, then does nothing. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/audio/voice_pool.h"

#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "qcopy.h"
#include "rnc/rendering/geometry_quad.h"
#include "sda.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/mobys/animation/build_moby_bone_transform.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/hero/hero_switch_item.h"
#include "rnc/overlay/shared/mobys/test_path_segment_strict_crossing.h"
#include "rnc/overlay/shared/math/interpolation/compute_directed_angle_delta.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/animation/set_animation_parameter.h"
#include "rnc/mobys/attach_manipulator.h"
#include "rnc/overlay/shared/game/restore_or_clear_level_state.h"
#include "rnc/overlay/shared/ui/hud/hud_update_counter.h"
#include "rnc/overlay/shared/hero/update_hero_and_vehicle_mode_effects.h"
#include "rnc/overlay/shared/mobys/capture_linked_moby_relative_transforms.h"
#include "rnc/data_labels.h"
#include "rnc/game/options.h"

/* On the first update sets state 1, flags 3 and substate 0. */
/* Ported from rac1-decomp (src/overlays/l18/vendor_002F2AE0.c: func_L18_002F8F10), where it is exact; names translated to the US level program. */

typedef struct {
    char           pad0[0x20];
    unsigned char  state;
    char           pad21[0x10];
    unsigned char  substate;
    char           pad32[2];
    unsigned short flags;
} Level18VendorMobyShort;

void inert_moby_update(Level18VendorMobyShort *moby) __asm__("FUN_L18_002f7ab0");

/* Moby class 1432 */
void inert_moby_update(Level18VendorMobyShort *moby) {
    if (moby->state == 0) {
        moby->state = 1;
        moby->flags |= 3;
        moby->substate = 0;
    }
}
