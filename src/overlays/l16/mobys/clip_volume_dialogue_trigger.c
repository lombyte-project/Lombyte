/* Level 16 moby class clip_volume_dialogue_trigger (0x619): armed in one clip volume, plays a dialogue once in a second one. */
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
#include "rnc/rendering/clip_volume.h"

void update_clip_volume_dialogue_trigger(struct Moby *moby) __asm__("FUN_L16_002e7208");

void update_clip_volume_dialogue_trigger(struct Moby *moby) {
    enum {
        CLIP_TRIGGER_WAIT_FOR_ARMING = 0,
        CLIP_TRIGGER_CHECK_LATCH = 1
    };
    char *data = ((char *)moby->pvars);
    switch (*(int *)(data + 8)) {
    case CLIP_TRIGGER_WAIT_FOR_ARMING: {
        char *hero_position = (char *)&hero.motion.pos;
        if (is_point_inside_clip_volume(hero_position, *(int *)data) != 0) {
            if (*(int *)(hero_position + 0x200C) == 0xF) {
                *(int *)(data + 8) = 1;
            }
        }
        break;
    }
    case CLIP_TRIGGER_CHECK_LATCH: {
        struct Hero *player = &hero;
        if (player->state.current == 0x42) {
            *(int *)(data + 8) = 0;
        } else if (is_point_inside_clip_volume(&player->motion.pos, *(int *)(data + 4)) != 0) {
            unsigned char *flags = (unsigned char *)item_unlocked;
            if (flags[0x19] == 0) {
                flags[0x19] = 1;
                allocate_voice_for_bank_entry(1, 0, 0);
                queue_dialogue_message(0x53D6, -1);
            }
        }
        break;
    }
    }
}
