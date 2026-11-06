/* Level 17 moby class camera_box_watcher (0x5be): watches the camera boxes around the hero, gives skill points and help messages, and turns the camera. */
#include "types.h"
#include "rnc/overlay/shared/camera/level_camera.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "rnc/overlay/shared/world/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/audio/voices/allocate_voice_for_bank_entry.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/ui/help/help_request.h"
#include "rnc/overlay/l17/hero/hero_set_state.h"
#include "rnc/overlay/shared/hero/update_hero_vehicle_attachments_and_effects.h"
#include "rnc/game/game_globals.h"
#include "rnc/game/item_state.h"
#include "rnc/overlay/shared/camera/follow_camera_angle_blend.h"
#include "rnc/overlay/shared/camera.h"
#include "rnc/overlay/shared/hero/hero_switch_item.h"
#include "rnc/sdk/debug.h"

/* Update for moby class 1470: watches camera boxes around the hero, gives skill points and help messages, and turns the camera. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F4210), where it is exact; names translated to the US level program. */

extern char *fleet_moby_array_end __asm__("D_L17_0015FFDC");
extern char  D_0013E52B[];

/* "magnacut cuboid error: detect:%d hero:%d init:%d end:%d \n" and its NUL. */
char fleet_camera_box_configuration_error_format[58] __asm__("D_L17_001E6F60") =
    "magnacut cuboid error: detect:%d hero:%d init:%d end:%d \n";

extern int D_00141DC4           NOT_SDA;
extern int                      fleet_help_request_gate_state[] __asm__("D_L17_00179D10");
extern int                      scale_game_frames(int);
extern unsigned char            D_0013D408[];
extern unsigned char D_0013D4DF NOT_SDA;
extern int                      pal_mode_macro[] __asm__("D_0015ED80") MACRO_ADDR;
extern char                     fleet_dwell_help_gate_value[] __asm__("D_00141D54");
extern char                     fleet_clip_volume_help_suppression_flags[] __asm__("D_00141D7C");

void fleet_camera_box_watcher_update(struct Moby *moby) __asm__("FUN_L17_002f26d0");

typedef struct CameraBoxWatcherVars {
    u8  pad_0[0xC];
    s32 dwell_frames;
    s32 moby_indices;
    u8  pad_14[0xC];
    s32 clip_volumes;
    u8  pad_24[0x8];
    s32 special_state_dwell_frames;
    s32 last_help_clip_volume;
    u8  pad_34[0x4];
    s32 dwell_help_delay;
    s32 camera_sequence_clip_volume;
    s32 second_camera_target_cuboid;
    s32 first_camera_target_cuboid;
    s32 hero_cuboid; /* "hero" in the cuboid error message */
    u8  camera_sequence_done;
    u8  pad_4D[0x3];
    s32 unk50;
    u8  pad_54[0x4];
    s32 mission;
    s32 camera_timer;
    s32 sequence_entry_clip_volume;
    s32 first_followup_clip_volume_index;
    s32 second_followup_clip_volume_index;
    s32 clip_volume_sequence_state;
    s32 scan_moby_index;
    s32 scan_outside_count;
    s32 scan_clip_volume;
} CameraBoxWatcherVars;

/* Moby class 1470 */
void fleet_camera_box_watcher_update(struct Moby *moby) {
    enum {
        CAMERA_BOX_WATCHER_INITIALIZE = 0,
        CAMERA_BOX_WATCHER_MONITOR = 1,
        CAMERA_BOX_WATCHER_ALIGN_FIRST_TARGET = 2,
        CAMERA_BOX_WATCHER_WAIT_AND_ALIGN_SECOND_TARGET = 3
    };
    enum {
        CLIP_VOLUME_SEQUENCE_WAIT_FOR_ENTRY = 0,
        CLIP_VOLUME_SEQUENCE_MONITOR_FOLLOWUP = 1
    };
    int                   clip_volume_entry_byte_offset;
    CameraBoxWatcherVars *pvars = (CameraBoxWatcherVars *)moby->pvars;
    int                   clip_volume_sequence_state;
    int                   i;
    int                   j;
    int                   found;
    int                   pending_help_clip_volume_index;
    int                   checked_moby_index;
    char                 *checked_moby;
    int                  *moby_indices;
    int                  *clip_volume_index_ptr;
    float                 target_heading;
    int                   next_moby_index;
    int                   mask;
    unsigned char        *sk;
    int                   wrap;

    clip_volume_sequence_state = pvars->clip_volume_sequence_state;
    switch (clip_volume_sequence_state) {
    case CLIP_VOLUME_SEQUENCE_WAIT_FOR_ENTRY: {
        char *h = (char *)&hero.motion.pos;
        if (is_point_inside_clip_volume(h, pvars->sequence_entry_clip_volume)) {
            if (*(int *)(h + 0x200C) == 0x11) {
                pvars->clip_volume_sequence_state = 1;
            }
        }
        break;
    }
    case CLIP_VOLUME_SEQUENCE_MONITOR_FOLLOWUP: {
        struct Hero *h = &hero;
        int          hero_current_state = h->state.current;
        if (hero_current_state == HERO_STATE_HURT_WITH_DISPLACEMENT_VELOCITY ||
            hero_current_state == HERO_STATE_OPTIONAL_IDLE_ANIMATION) {
            pvars->clip_volume_sequence_state = 0;
        } else if (is_point_inside_clip_volume(&h->motion.pos,
                                               pvars->first_followup_clip_volume_index) ||
                   is_point_inside_clip_volume(&h->motion.pos,
                                               pvars->second_followup_clip_volume_index)) {
            if (D_0013D408[0x1C] == 0) {
                D_0013D408[0x1C] = 1;
                allocate_voice_for_bank_entry(1, 0, 0);
                queue_dialogue_message(0x53D6, -1);
            }
        }
        break;
    }
    }

    if (D_0013D408[0x1B] == 0) {
        for (j = 0; j < 30; j++) {
            char *mb;
            sk = D_0013D408;
            mb = D_L17_0015FFD8 + (pvars->scan_moby_index << 8);
            if ((*(short *)(mb + 0xA6) == 0x2C || *(short *)(mb + 0xA6) == 0x566) &&
                *(signed char *)(mb + 0x20) >= 0 &&
                !is_point_inside_clip_volume(mb + 0x10, pvars->scan_clip_volume)) {
                pvars->scan_outside_count += 1;
            }
            next_moby_index = pvars->scan_moby_index + 1;
            wrap = D_L17_0015FFD8 + (next_moby_index << 8) < fleet_moby_array_end;
            pvars->scan_moby_index = next_moby_index;
            if (!wrap) {
                if (pvars->scan_outside_count == 0 && sk[0x1B] == 0) {
                    sk[0x1B] = 1;
                    allocate_voice_for_bank_entry(1, 0, 0);
                    queue_dialogue_message(0x53D6, -1);
                }
                pvars->scan_moby_index = 0;
                pvars->scan_outside_count = 0;
            }
        }
    }

    switch (moby->state) {
    case CAMERA_BOX_WATCHER_INITIALIZE:
        moby->update_dist = 0xFF;
        pvars->dwell_help_delay = scale_game_frames(600);
        pvars->camera_sequence_done = 0;
        if (pvars->first_camera_target_cuboid == -1 || pvars->second_camera_target_cuboid == -1 ||
            pvars->camera_sequence_clip_volume == -1 || pvars->hero_cuboid == -1) {
            DebugPrint(fleet_camera_box_configuration_error_format,
                       pvars->camera_sequence_clip_volume,
                       pvars->hero_cuboid,
                       pvars->second_camera_target_cuboid,
                       pvars->first_camera_target_cuboid);
            pvars->camera_sequence_done = 1;
        }
        moby->state = CAMERA_BOX_WATCHER_MONITOR;
        break;
    case CAMERA_BOX_WATCHER_MONITOR:
        pending_help_clip_volume_index = -1;
        checked_moby_index = -1;
        found = 0;
        for (i = 0; i < 1; i++) {
            clip_volume_entry_byte_offset = i * 4;
            if (is_point_inside_clip_volume(
                    &hero.motion.pos, *(int *)((u8 *)pvars + clip_volume_entry_byte_offset))) {
                found = 1;
                if (pvars->last_help_clip_volume !=
                    *(int *)((u8 *)pvars + clip_volume_entry_byte_offset)) {
                    pending_help_clip_volume_index =
                        *(int *)((u8 *)pvars + clip_volume_entry_byte_offset);
                    moby_indices = &pvars->moby_indices;
                    checked_moby_index = moby_indices[i];
                }
                break;
            }
        }
        if (found) {
            pvars->dwell_frames += 1;
        } else {
            pvars->dwell_frames = 0;
            pvars->last_help_clip_volume = -1;
        }
        if (checked_moby_index != -1) {
            checked_moby = D_L17_0015FFD8 + (checked_moby_index << 8);
            if (checked_moby == 0)
                goto skip1;
            if (MOBY(checked_moby)->oclass != 0x4E)
                goto skip1;
            if (MOBY(checked_moby)->state == 0xFE)
                goto skip1;
            if (MOBY(checked_moby)->state == 0xFD)
                goto skip1;
            if (MOBY(checked_moby)->state == 3)
                goto skip1;
        }
        if (pvars->dwell_frames > pvars->dwell_help_delay && pending_help_clip_volume_index != -1 &&
            ((unsigned int)hero.state.control_mode < 2 || hero.state.control_mode == 9) &&
            fleet_help_request_gate_state[0] == 0 && fleet_help_request_gate_state[9] == -1 &&
            D_0013D4DF != 0) {
            pvars->dwell_help_delay = scale_game_frames(0x708);
            if (*(int *)(((char *)&fleet_dwell_help_gate_value)) >= 0) {
                help_request(0x3A9A, 0x7D);
            }
            pvars->last_help_clip_volume = pending_help_clip_volume_index;
            pvars->dwell_frames = 0;
        }
    skip1:
        if (is_point_inside_clip_volume(&hero.motion.pos, pvars->clip_volumes) &&
            fleet_help_request_gate_state[0] == 0 && fleet_help_request_gate_state[9] == -1 &&
            item_unlocked[0xBC] == 0) {
            int help_gate_flags = *(int *)(((char *)&fleet_clip_volume_help_suppression_flags));
            mask = 1 << pal_mode_macro[1];
            if (!(help_gate_flags & mask)) {
                help_request(0x3A9D, 0x82);
            }
        }
        pending_help_clip_volume_index = -1;
        found = 0;
        for (i = 0; i < 3; i++) {
            clip_volume_entry_byte_offset = i * 4;
            clip_volume_index_ptr = (int *)((u8 *)pvars + 0x20 + clip_volume_entry_byte_offset);
            if (is_point_inside_clip_volume(&hero.motion.pos, *clip_volume_index_ptr)) {
                found = 1;
                if (pvars->last_help_clip_volume != *clip_volume_index_ptr) {
                    pending_help_clip_volume_index = *clip_volume_index_ptr;
                }
                break;
            }
        }
        if (!found || (pvars->special_state_dwell_frames += 1, hero.state.current == 0x35)) {
            pvars->special_state_dwell_frames = 0;
            pvars->last_help_clip_volume = -1;
        }
        if (scale_game_frames(0xE10) < pvars->special_state_dwell_frames &&
            pending_help_clip_volume_index != -1 && hero_is_in_special_state() &&
            fleet_help_request_gate_state[0] == 0 && fleet_help_request_gate_state[9] == -1 &&
            item_unlocked[0xBC] != 0) {
            help_request(0x4E2E, 0x78);
            pvars->last_help_clip_volume = pending_help_clip_volume_index;
            pvars->special_state_dwell_frames = 0;
        }
        if (pvars->mission != 0xFF &&
            visited52[pvars->mission + (current_level_index << 4)] == 0xFF) {
            pvars->camera_sequence_done = 1;
        }
        if (pvars->camera_sequence_done == 0 &&
            is_point_inside_clip_volume(&hero.motion.pos, pvars->camera_sequence_clip_volume)) {
            clear_active_help_record();
            if (*(int *)(((char *)&D_00141DC4)) >= 0) {
                help_request(0x4268, 0x8B);
            }
            if (fleet_help_request_gate_state[0] != 0 || fleet_help_request_gate_state[9] != -1) {
                pvars->unk50 = 0;
                moby->state = CAMERA_BOX_WATCHER_ALIGN_FIRST_TARGET;
                pvars->camera_timer = scale_game_frames(0xB4);
                hero_set_state(0x72, 0);
            }
        }
        break;
    case CAMERA_BOX_WATCHER_ALIGN_FIRST_TARGET: {
        char *cam;
        set_follow_camera_blend_output_flags();
        set_follow_camera_angle_blend_from_position(
            (int)(D_L17_001600EC + (pvars->first_camera_target_cuboid << 7) + 0x30),
            0.02617994f,
            0.0f);
        cam = (char *)&l17_camera;
        target_heading = fast_arc_tan(
            *(float *)(D_L17_001600EC + ((pvars->first_camera_target_cuboid << 7) + 0x30)) -
                *(float *)(cam + 0x140),
            *(float *)(D_L17_001600EC + ((pvars->first_camera_target_cuboid << 7) + 0x34)) -
                *(float *)(cam + 0x144));
        if (fast_difference_between_rotations(*(float *)(cam + 0x158), target_heading) <
            0.034906585f) {
            moby->state = CAMERA_BOX_WATCHER_WAIT_AND_ALIGN_SECOND_TARGET;
            pvars->camera_timer = scale_game_frames(0x3C);
        }
        break;
    }
    case CAMERA_BOX_WATCHER_WAIT_AND_ALIGN_SECOND_TARGET:
        set_follow_camera_blend_output_flags();
        if (fast_dec_timer(&pvars->camera_timer)) {
            char *cam;
            set_follow_camera_angle_blend_from_position(
                (int)(D_L17_001600EC + (pvars->second_camera_target_cuboid << 7) + 0x30),
                0.02617994f,
                0.0f);
            cam = (char *)&l17_camera;
            target_heading = fast_arc_tan(
                *(float *)(D_L17_001600EC + ((pvars->second_camera_target_cuboid << 7) + 0x30)) -
                    *(float *)(cam + 0x140),
                *(float *)(D_L17_001600EC + ((pvars->second_camera_target_cuboid << 7) + 0x34)) -
                    *(float *)(cam + 0x144));
            if (fast_difference_between_rotations(*(float *)(cam + 0x158), target_heading) <
                0.034906585f) {
                enter_hero_idle_state_for_current_vehicle_mode();
                pvars->camera_sequence_done = 1;
                moby->state = CAMERA_BOX_WATCHER_MONITOR;
            }
        }
        break;
    }
}
