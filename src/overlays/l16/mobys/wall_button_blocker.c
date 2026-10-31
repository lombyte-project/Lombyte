/* Level 16 moby class wall_button_blocker (0x56b): a scaled moby whose collision a wall_button turns off. */
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
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/ui/text/font/font_queue_vu_state.h"

/* UpdateMoby: on the first tick scales two data floats by 0.25 into the moby and sets its state. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E4B50), where it is exact; names translated to the US level program. */

void wall_button_blocker_update(struct Moby *moby) __asm__("FUN_L16_002e36e8");

/* Moby class 1387 */
void wall_button_blocker_update(struct Moby *moby) {
    float *rotation_scale_data = (float *)moby->pvars;
    if (moby->state == 0) {
        fast_vec_normalize(&moby->rot_row1, &moby->rot_row1, rotation_scale_data[0] * 0.25f);
        fast_vec_normalize(&moby->rot_row2, &moby->rot_row2, rotation_scale_data[1] * 0.25f);
        moby->state = 1;
        moby->flags |= 0x101;
        update_entity(moby);
    }
}

/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E4BD8), where it is exact; names translated to the US level program. */

void set_wall_button_blocker_collision(void *moby, int on) __asm__("FUN_L16_002e3770");

void set_wall_button_blocker_collision(void *moby, int on) {
    if (*(short *)((char *)moby + 0xA6) == 0x56B) {
        if (on != 0) {
            *(int *)((char *)moby + 0x94) = *(int *)(*(char **)((char *)moby + 0x24) + 0x10);
        } else {
            *(int *)((char *)moby + 0x94) = 0;
        }
    }
}
