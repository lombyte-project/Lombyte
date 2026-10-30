/* Level 16 moby class cutscene_joint_trails (0x714): during the cutscene draws two trails between joint pairs of the current cutscene moby. */
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

extern s32 D_L16_0015F5C4;

/* During the cutscene (scene 0) draws trails from joints 6 and 7 of cutscene moby 3, pointing away from joints 4 and 5. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002E9278), where it is exact; names translated to the US level program. */

typedef float W[4] __attribute__((aligned(16)));

extern char cutscene_state[] __asm__("D_L16_0016CC60");
extern int  D_L16_0015F5C4;

void update_cutscene_joint_trails(struct Moby *moby) __asm__("FUN_L16_002e7e00");

void update_cutscene_joint_trails(struct Moby *moby) {
    enum {
        JOINT_TRAIL_INITIALIZE = 0,
        JOINT_TRAIL_EMIT = 1
    };
    W joint_6_position, joint_7_position, joint_4_position, joint_5_position,
        joint_6_trail_velocity, joint_7_trail_velocity;
    float k1 = 0.075f, k2 = 20000.0f, k3 = 1000.0f;
    float j1 = 0.075f, j2 = 20000.0f, j3 = 1000.0f;
    switch (moby->state) {
    case JOINT_TRAIL_INITIALIZE:
        moby->update_dist = 0xFF;
        moby->state = 1;
        break;
    case JOINT_TRAIL_EMIT:
        if (D_L16_0015F5C4 == 2) {
            char *cutscene = cutscene_state;
            if (*(int *)(cutscene + 0x30) == 0) {
                char *joint_source_moby = *(char **)(cutscene + 0x184);
                moby_point_position(joint_source_moby, 6, joint_6_position);
                moby_point_position(joint_source_moby, 4, joint_4_position);
                fast_vec_sub(joint_6_trail_velocity, joint_6_position, joint_4_position);
                fast_vec_normalize(joint_6_trail_velocity, joint_6_trail_velocity, k1);
                draw_trail(joint_6_position, joint_6_position, joint_6_trail_velocity, k2, k3);
                moby_point_position(joint_source_moby, 7, joint_7_position);
                moby_point_position(joint_source_moby, 5, joint_5_position);
                fast_vec_sub(joint_7_trail_velocity, joint_7_position, joint_5_position);
                fast_vec_normalize(joint_7_trail_velocity, joint_7_trail_velocity, j1);
                draw_trail(joint_7_position, joint_7_position, joint_7_trail_velocity, j2, j3);
            }
        }
        break;
    }
}
