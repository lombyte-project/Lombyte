/* Level 17 door (classes 0x54f-0x55d). */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/input/pad_globals.h"
#include "rnc/mobys/moby_class_id.h"

#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"

#include "sda.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/quad.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/hero/hero_switch_item.h"
#include "rnc/overlay/shared/mobys/test_path_segment_strict_crossing.h"
#include "rnc/overlay/shared/math/interpolation/compute_directed_angle_delta.h"
#include "rnc/overlay/shared/vendor/vendor_spring.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/overlay/shared/mobys/step_moby_motion.h"
#include "rnc/rendering/effects/fade_to_black.h"
#include "rnc/overlay/l17/hero/hero_set_state.h"
#include "rnc/rendering/view/project_to_screen.h"
#include "rnc/ui/help/force_help_message.h"

/* Moves a level-17 platform moby between its stops along its heading, with a looping sound. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002AA068.c: func_L17_002EA2C8), where it is exact; names translated to the US level program. */

extern char          D_L17_001676C0[];
extern int           D_L17_0015F5C4;
extern unsigned char D_0013E633[] __asm__("D_0013F3D0");
void                 mark_moby_for_removal(struct Obj *obj) __asm__("FUN_0020c828");

void door_update(struct Moby *m) __asm__("FUN_L17_002e87d8");

/* Moby classes 1359-1364, 1367, 1369, 1372, 1373 */
void door_update(struct Moby *door_moby) {
    enum {
        DOOR_STATE_INITIALIZE = 0,
        DOOR_STATE_WAIT_AT_ORIGIN = 1,
        DOOR_STATE_MOVE_AWAY_FROM_ORIGIN = 2,
        DOOR_STATE_WAIT_DISPLACED = 4,
        DOOR_STATE_RETURN_TO_ORIGIN = 5
    };
    char *door_vars = (char *)door_moby->pvars;
    float position_offset[4];
    float updated_position[4];
    float movement_heading;
    float k;
    float k2;
    float displacement_sample;
    int   voice_slot_index;

    if ((*(int *)(door_vars + ((0x18)))) == -1 || (*(int *)(door_vars + ((0x1C)))) == -1) {
        mark_moby_for_removal(door_moby);
        return;
    }
    if (D_L17_0015F5C4 == 2)
        return;
    switch (door_moby->state) {
    case DOOR_STATE_INITIALIZE:
        if ((door_moby->oclass) == 0x550 || (door_moby->oclass) == 0x551 ||
            (door_moby->oclass) == 0x553 || (door_moby->oclass) == 0x557 ||
            (door_moby->oclass) == 0x55D) {
            door_moby->command = 1;
        }
        *(OvlQuad *)door_vars = *(OvlQuad *)&door_moby->pos;
        door_moby->state = 1;
        break;
    case DOOR_STATE_MOVE_AWAY_FROM_ORIGIN:
        if (door_moby->pclass->sound_defs != 0 &&
            voice_is_playing(door_moby, (*(int *)(door_vars + ((0x20))))) == 0) {
            (*(int *)(door_vars + ((0x20)))) =
                allocate_voice_for_target_entry(0, 4, (int)door_moby);
        }
        k = frame_time * 3.0f;
        displacement_sample = (*(float *)(door_vars + ((0x14))));
        if (door_moby->command == 0)
            k = -k;
        (*(float *)(door_vars + ((0x14)))) = displacement_sample + k;
        if ((door_moby->oclass) == 0x55C || (door_moby->oclass) == 0x55D ||
            (door_moby->oclass) == 0x557 || (door_moby->oclass) == 0x559) {
            movement_heading = fast_add_rotations(1.5707964f, door_moby->rot.z);
            position_offset[0] = fast_cos(movement_heading) * (*(float *)(door_vars + ((0x14))));
            position_offset[1] = fast_sin(movement_heading) * (*(float *)(door_vars + ((0x14))));
            position_offset[2] = 0.0f;
        } else {
            position_offset[0] = fast_cos(door_moby->rot.z) * (*(float *)(door_vars + ((0x14))));
            position_offset[1] = fast_sin(door_moby->rot.z) * (*(float *)(door_vars + ((0x14))));
            position_offset[2] = 0.0f;
        }
        fast_vec_add(updated_position, door_vars, position_offset);
        *(OvlQuad *)&door_moby->pos = *(OvlQuad *)updated_position;
        if (absolute_float((*(float *)(door_vars + ((0x14))))) > 2.0f) {
            voice_slot_index = (*(int *)(door_vars + ((0x20))));
            if (voice_slot_index != -1) {
                char *e = (char *)D_0013E633 - 0xE80 + voice_slot_index * 0x70;
                if (*(char **)(e + 0x88) == door_moby) {
                    if (*(unsigned char *)(e + 0x74) != 0)
                        release_voice_slot(voice_slot_index);
                }
            }
            (*(int *)(door_vars + ((0x20)))) = -1;
            door_moby->state = 4;
        }
        break;
    case DOOR_STATE_WAIT_AT_ORIGIN:
        (*(float *)(door_vars + ((0x14)))) = 0.0f;
        voice_slot_index = (*(int *)(door_vars + ((0x20))));
        if (voice_slot_index != -1) {
            char *e = (char *)D_0013E633 - 0xE80 + voice_slot_index * 0x70;
            if (*(char **)(e + 0x88) == door_moby) {
                if (*(unsigned char *)(e + 0x74) != 0)
                    release_voice_slot(voice_slot_index);
            }
        }
        (*(int *)(door_vars + ((0x20)))) = -1;
        if (absolute_float(door_moby->rot.x) < 0.7853982f || hero.unk308 != 0) {
            if (is_point_inside_clip_volume(&hero.motion.pos, (*(int *)(door_vars + ((0x18))))) ==
                0) {
                if ((*(int *)(door_vars + ((0x10)))) == -1)
                    break;
                if (is_point_inside_clip_volume(D_L17_0015FFD8 +
                                                    (*(int *)(door_vars + ((0x10)))) * 0x100 + 0x10,
                                                (*(int *)(door_vars + ((0x18))))) == 0)
                    break;
            }
            if (door_moby->pclass->sound_defs != 0 &&
                voice_is_playing(door_moby, (*(int *)(door_vars + ((0x20))))) == 0) {
                (*(int *)(door_vars + ((0x20)))) =
                    allocate_voice_for_target_entry(0, 4, (int)door_moby);
            }
            door_moby->state = 2;
        }
        break;
    case 3:
        break;
    case DOOR_STATE_WAIT_DISPLACED:
        voice_slot_index = (*(int *)(door_vars + ((0x20))));
        if (voice_slot_index != -1) {
            char *e = (char *)D_0013E633 - 0xE80 + voice_slot_index * 0x70;
            if (*(char **)(e + 0x88) == door_moby) {
                if (*(unsigned char *)(e + 0x74) != 0)
                    release_voice_slot(voice_slot_index);
            }
        }
        (*(int *)(door_vars + ((0x20)))) = -1;
        if (is_point_inside_clip_volume(&hero.motion.pos, (*(int *)(door_vars + ((0x1C))))))
            break;
        if (is_point_inside_clip_volume(D_L17_001676C0, (*(int *)(door_vars + ((0x1C))))))
            break;
        if ((*(int *)(door_vars + ((0x10)))) != -1 &&
            is_point_inside_clip_volume(D_L17_0015FFD8 + (*(int *)(door_vars + ((0x10)))) * 0x100 +
                                            0x10,
                                        (*(int *)(door_vars + ((0x18))))))
            break;
        if (door_moby->pclass->sound_defs != 0 &&
            voice_is_playing(door_moby, (*(int *)(door_vars + ((0x20))))) == 0) {
            (*(int *)(door_vars + ((0x20)))) =
                allocate_voice_for_target_entry(0, 4, (int)door_moby);
        }
        door_moby->state = 5;
        break;
    case DOOR_STATE_RETURN_TO_ORIGIN:
        if (door_moby->pclass->sound_defs != 0 &&
            voice_is_playing(door_moby, (*(int *)(door_vars + ((0x20))))) == 0) {
            (*(int *)(door_vars + ((0x20)))) =
                allocate_voice_for_target_entry(0, 4, (int)door_moby);
        }
        if (is_point_inside_clip_volume(&hero.motion.pos, (*(int *)(door_vars + ((0x18))))) ||
            is_point_inside_clip_volume(D_L17_001676C0, (*(int *)(door_vars + ((0x18))))) ||
            ((*(int *)(door_vars + ((0x10)))) != -1 &&
             is_point_inside_clip_volume(D_L17_0015FFD8 + (*(int *)(door_vars + ((0x10)))) * 0x100 +
                                             0x10,
                                         (*(int *)(door_vars + ((0x18))))))) {
            door_moby->state = 2;
            break;
        }
        displacement_sample = absolute_float((*(float *)(door_vars + ((0x14)))));
        k2 = frame_time * 3.0f;
        if (displacement_sample > k2) {
            float previous_displacement = (*(float *)(door_vars + ((0x14))));
            float return_displacement;
            if (door_moby->command == 0) {
                float negative_return_step = -k2;
                return_displacement = previous_displacement - negative_return_step;
            } else {
                return_displacement = previous_displacement - k2;
            }
            (*(float *)(door_vars + ((0x14)))) = return_displacement;
        } else {
            (*(float *)(door_vars + ((0x14)))) = 0.0f;
        }
        if ((door_moby->oclass) == 0x55C || (door_moby->oclass) == 0x55D ||
            (door_moby->oclass) == 0x557 || (door_moby->oclass) == 0x559) {
            movement_heading = fast_add_rotations(1.5707964f, door_moby->rot.z);
            position_offset[0] = fast_cos(movement_heading) * (*(float *)(door_vars + ((0x14))));
            position_offset[1] = fast_sin(movement_heading) * (*(float *)(door_vars + ((0x14))));
            position_offset[2] = 0.0f;
        } else {
            position_offset[0] = fast_cos(door_moby->rot.z) * (*(float *)(door_vars + ((0x14))));
            position_offset[1] = fast_sin(door_moby->rot.z) * (*(float *)(door_vars + ((0x14))));
            position_offset[2] = 0.0f;
        }
        fast_vec_add(updated_position, door_vars, position_offset);
        *(OvlQuad *)&door_moby->pos = *(OvlQuad *)updated_position;
        if ((*(float *)(door_vars + ((0x14)))) == 0.0f) {
            voice_slot_index = (*(int *)(door_vars + ((0x20))));
            if (voice_slot_index != -1) {
                char *e = (char *)D_0013E633 - 0xE80 + voice_slot_index * 0x70;
                if (*(char **)(e + 0x88) == door_moby) {
                    if (*(unsigned char *)(e + 0x74) != 0)
                        release_voice_slot(voice_slot_index);
                }
            }
            (*(int *)(door_vars + ((0x20)))) = -1;
            door_moby->state = 1;
        }
        break;
    }
}
