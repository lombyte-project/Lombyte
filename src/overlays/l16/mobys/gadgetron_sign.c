/* Level 16 gadgetron_sign. */
#include "types.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"

#include "qcopy.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/entities.h"
#include "rnc/overlay/watch.h"
#include "rnc/game/item_state.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voices/allocate_voice_for_bank_entry.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/interpolation/compute_directed_angle_delta.h"
#include "rnc/overlay/shared/vendor/vendor_spring.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/overlay/shared/mobys/step_moby_motion.h"
#include "rnc/overlay/shared/ui/help/help_request.h"
#include "rnc/save/memcard_save_data.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/animation/set_moby_animation.h"
#include "rnc/overlay/shared/game/restore_or_clear_level_state.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/ui/text/font/font_queue_vu_state.h"

extern float fast_add_rotations(float, float);
extern float frame_time, D_0015EE70;
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002E9D48), where it is exact; names translated to the US level program. */

void gadgetron_sign_update(struct Moby *moby) __asm__("FUN_L16_002e88d0");

/* Moby class 1891 */
void gadgetron_sign_update(struct Moby *moby) {
    enum {
        GADGETRON_SIGN_INITIALIZE = 0,
        GADGETRON_SIGN_ROTATE = 1
    };
    switch (moby->state) {
    case GADGETRON_SIGN_INITIALIZE: {
        float size = moby->scale * 4.0f;
        int   ff = 0xFF;
        int   twohundred = 0x200;
        moby->state = 1;
        moby->update_dist = ff;
        moby->draw_dist = twohundred;
        moby->pos.z = 160.0f;
        moby->scale = size;
    } break;
    case GADGETRON_SIGN_ROTATE:
        moby->rot.z = fast_add_rotations(moby->rot.z, frame_time * -0.2617993950843811f);
        break;
    }
}
