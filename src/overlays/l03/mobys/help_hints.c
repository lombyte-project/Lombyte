/* Level 03 moby class help_hints (0x53e): plays the Kerwan help messages and counts usage stats while the hero is inside its clip volumes. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/game/usage_stats.h"
#include "rnc/data_labels.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/audio/voices/allocate_voice_for_bank_entry.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/ui/help/help_request.h"

/* Help message playback state (same layout as S00203908 in shared/ui/help/help_request.c). */
struct L03HelpState {
    s32 state;
    u8  pad4[0x20];
    s32 cur;
};

/* Per-message help record at D_00141968 (same layout as P00203908): id 0xFFFF = disabled. */
struct L03HelpRecord {
    u16 id;
    u16 pad;
    s32 v;
};

/* One of the 37 item entries at D_L03_00179BC0 (stride 0x4C, as Entry_L02_0017A140 in l02). */
struct L03ItemEntry {
    s32 pad0;
    s32 pad4;
    s32 active;
    u8  rest[0x40];
};

extern struct L03HelpState l03_help_message_playback_state __asm__("D_L03_00179510");

extern struct L03HelpRecord help_records[] __asm__("D_00141968");
extern struct L03ItemEntry  D_L03_00179BC0[];
extern struct Moby         *D_L03_0015FFE4;

extern s32 get_ammo(s32) __asm__("FUN_L00_00233e98");

/* Kerwan tutorial hint triggers: plays help messages and counts usage stats while the hero is inside the moby's clip volumes. */
void help_hints_update(struct Moby *moby) __asm__("FUN_L03_002df520");

/* Moby class 1342 */
void help_hints_update(struct Moby *moby) {
    enum {
        TUTORIAL_HINTS_INITIALIZE = 0,
        TUTORIAL_HINTS_UPDATE = 1
    };
    s32         *tutorial_hint_state = ((s32 *)moby->pvars);
    struct Moby *candidate_moby;
    s32          count;
    s32          i;
    s32          scaled_time_value;

    fast_dec_timer(&tutorial_hint_state[22]);
    switch (moby->state) {
    case TUTORIAL_HINTS_INITIALIZE:
        tutorial_hint_state[20] = 0;
        tutorial_hint_state[21] = 0;
        moby->update_dist = 0xFF;
        moby->state = 1;
        break;
    case TUTORIAL_HINTS_UPDATE:
        if (is_point_inside_clip_volume(&hero.motion.pos, tutorial_hint_state[30]) &&
            ((u32)hero.state.control_mode < 2 || hero.state.control_mode == 9)) {
            alternate_item_available[0x1A] = 1;
        }
        if (alternate_item_available[0x1A] == 0) {
            if (is_point_inside_clip_volume(&hero.motion.pos, tutorial_hint_state[0]) &&
                ((u32)hero.state.control_mode < 2 || hero.state.control_mode == 9) &&
                l03_help_message_playback_state.state == 0 &&
                l03_help_message_playback_state.cur == -1 && help_records[0x14].v >= 0) {
                help_request(0xBB8, 0x14);
            }
            if (is_point_inside_clip_volume(&hero.motion.pos, tutorial_hint_state[3]) &&
                ((u32)hero.state.control_mode < 2 || hero.state.control_mode == 9) &&
                l03_help_message_playback_state.state == 0 &&
                l03_help_message_playback_state.cur == -1 && help_records[0x15].v >= 0) {
                help_request(0xBB9, 0x15);
            }
            if (is_point_inside_clip_volume(&hero.motion.pos, tutorial_hint_state[6]) &&
                ((u32)hero.state.control_mode < 2 || hero.state.control_mode == 9) &&
                l03_help_message_playback_state.state == 0 &&
                l03_help_message_playback_state.cur == -1 && help_records[0x17].v >= 0) {
                help_request(0xBBB, 0x17);
            }
        }
        if (is_point_inside_clip_volume(&hero.motion.pos, tutorial_hint_state[12]) &&
            usage_stats.stat[3].count == 0 &&
            ((u32)hero.state.control_mode < 2 || hero.state.control_mode == 9)) {
            usage_stats.stat[3].count++;
            scaled_time_value = scale_game_frames(D_0015EEA4) / 600;
            if (usage_stats.stat[3].unk2 < scaled_time_value) {
                usage_stats.stat[3].unk2 = scale_game_frames(D_0015EEA4) / 600;
            }
            usage_stats.stat[3].level_mask =
                usage_stats.stat[3].level_mask | (1 << current_level_index) | 0x80000000;
        }
        if (is_point_inside_clip_volume(&hero.motion.pos, tutorial_hint_state[9]) &&
            usage_stats.stat[3].count == 0 &&
            !((u32)hero.state.control_mode < 2 || hero.state.control_mode == 9) &&
            tutorial_hint_state[22] == 0) {
            tutorial_hint_state[20]++;
            tutorial_hint_state[22] = scale_game_frames(60);
        } else if (tutorial_hint_state[22] != 0 &&
                   ((u32)hero.state.control_mode < 2 || hero.state.control_mode == 9)) {
            tutorial_hint_state[22] = 0;
        }
        if (is_point_inside_clip_volume(&hero.motion.pos, tutorial_hint_state[9]) &&
            usage_stats.stat[3].count == 0 &&
            ((u32)hero.state.control_mode < 2 || hero.state.control_mode == 9) &&
            tutorial_hint_state[20] % 6 == 0 && tutorial_hint_state[20] != 0 &&
            l03_help_message_playback_state.state == 0 &&
            l03_help_message_playback_state.cur == -1 && help_records[0x16].id == 0) {
            help_request(0xBBA, 0x16);
        }
        if (is_point_inside_clip_volume(&hero.motion.pos, tutorial_hint_state[10]) &&
            ((u32)hero.state.control_mode < 2 || hero.state.control_mode == 9) &&
            l03_help_message_playback_state.state == 0 &&
            l03_help_message_playback_state.cur == -1 && help_records[0x18].id == 0) {
            scaled_time_value = scale_game_frames(D_0015EEA4) - usage_stats.stat[9].unk2 * 600;
            if (scale_game_frames(216000) < scaled_time_value) {
                help_request(0xBBC, 0x18);
            }
        }
        count = 0;
        for (candidate_moby = D_L03_0015FFE4; candidate_moby != 0;
             candidate_moby = candidate_moby->next) {
            if (candidate_moby->pclass != 0 && *(s16 *)((u8 *)candidate_moby->pclass + 0x46) == 5 &&
                vector_distance_xy(&candidate_moby->pos, &hero.motion.pos) < 8.0f) {
                count++;
            }
        }
        if (is_point_inside_clip_volume(&hero.motion.pos, tutorial_hint_state[15]) && count == 0 &&
            ((u32)hero.state.control_mode < 2 || hero.state.control_mode == 9) &&
            hero.state.current != 0x1E && item_available[0xF] != 0 && get_ammo(0xF) >= 0x15 &&
            l03_help_message_playback_state.state == 0 &&
            l03_help_message_playback_state.cur == -1 && help_records[0x4A].id == 0 &&
            usage_stats.stat[19].count == 0) {
            help_request(0x4E20, 0x4A);
        } else if (hero.state.current == 0x1E) {
            if (usage_stats.stat[19].count < 0xFFFF) {
                usage_stats.stat[19].count++;
            }
            scaled_time_value = scale_game_frames(D_0015EEA4) / 600;
            if (usage_stats.stat[19].unk2 < scaled_time_value) {
                usage_stats.stat[19].unk2 = scale_game_frames(D_0015EEA4) / 600;
            }
            usage_stats.stat[19].level_mask =
                usage_stats.stat[19].level_mask | (1 << current_level_index) | 0x80000000;
        }
        if (is_point_inside_clip_volume(&hero.motion.pos, tutorial_hint_state[14]) &&
            ((u32)hero.state.control_mode < 2 || hero.state.control_mode == 9) &&
            D_L03_0015F68C < 3 && l03_help_message_playback_state.state == 0 &&
            l03_help_message_playback_state.cur == -1 && help_records[0x40].id == 0) {
            help_request(0x3EC, 0x40);
        }
        if (is_point_inside_clip_volume(&hero.motion.pos, tutorial_hint_state[14]) &&
            ((u32)hero.state.control_mode < 2 || hero.state.control_mode == 9) &&
            hero.state.current == HERO_STATE_LOOK && D_L03_0015F688 == -1 &&
            l03_help_message_playback_state.state == 0 &&
            l03_help_message_playback_state.cur == -1 && help_records[0x40].id != 0 &&
            help_records[0x41].id == 0) {
            help_request(0x3ED, 0x41);
        }
        if (is_point_inside_clip_volume(&hero.motion.pos, tutorial_hint_state[16]) &&
            ((u32)hero.state.control_mode < 2 || hero.state.control_mode == 9) &&
            l03_help_message_playback_state.state == 0 &&
            l03_help_message_playback_state.cur == -1 && help_records[0x54].id == 0) {
            help_request(0xBC0, 0x54);
        }
        if (D_L03_0015F5C4 == 0 && item_available[0x34] != 0 &&
            l03_help_message_playback_state.state == 0 &&
            l03_help_message_playback_state.cur == -1 && help_records[0x19].id == 0) {
            help_request(0xBBD, 0x19);
        }
        if (is_point_inside_clip_volume(&hero.motion.pos, tutorial_hint_state[13]) &&
            usage_stats.stat[15].count == 0 &&
            ((u32)hero.state.control_mode < 2 || hero.state.control_mode == 9)) {
            usage_stats.stat[15].count++;
            scaled_time_value = scale_game_frames(D_0015EEA4) / 600;
            if (usage_stats.stat[15].unk2 < scaled_time_value) {
                usage_stats.stat[15].unk2 = scale_game_frames(D_0015EEA4) / 600;
            }
            usage_stats.stat[15].level_mask =
                usage_stats.stat[15].level_mask | (1 << current_level_index) | 0x80000000;
        }
        if (hero.unk988 != 0 && usage_stats.stat[15].count == 0) {
            hero.unk988 = 0;
            tutorial_hint_state[21]++;
        }
        if (usage_stats.stat[15].count == 0 &&
            ((u32)hero.state.control_mode < 2 || hero.state.control_mode == 9) &&
            tutorial_hint_state[21] >= 3 && l03_help_message_playback_state.state == 0 &&
            l03_help_message_playback_state.cur == -1 && help_records[0x1A].id == 0) {
            help_request(0xBBE, 0x1A);
        }
        if (hero.state.current == 0x74 && usage_stats.stat[16].count == 0) {
            usage_stats.stat[16].count++;
            scaled_time_value = scale_game_frames(D_0015EEA4) / 600;
            if (usage_stats.stat[16].unk2 < scaled_time_value) {
                usage_stats.stat[16].unk2 = scale_game_frames(D_0015EEA4) / 600;
            }
            usage_stats.stat[16].level_mask =
                usage_stats.stat[16].level_mask | (1 << current_level_index) | 0x80000000;
        }
        if (is_point_inside_clip_volume(&hero.motion.pos, tutorial_hint_state[11]) &&
            usage_stats.stat[16].count == 0 && hero.state.control_mode == 6 &&
            hero.items[0].item_id == 8 && l03_help_message_playback_state.state == 0 &&
            l03_help_message_playback_state.cur == -1 && help_records[0x1B].id == 0) {
            help_request(0xBBF, 0x1B);
        }
        if (is_point_inside_clip_volume(&hero.motion.pos, tutorial_hint_state[17]) &&
            ((u32)hero.state.control_mode < 2 || hero.state.control_mode == 9) &&
            l03_help_message_playback_state.state == 0 &&
            l03_help_message_playback_state.cur == -1 && help_records[0x79].id == 0) {
            help_request(0x4E2F, 0x79);
        }
        if (is_point_inside_clip_volume(&hero.motion.pos, tutorial_hint_state[31]) &&
            (u32)hero.state.control_mode < 2 && help_records[0x4F].id == 0 &&
            usage_stats.stat[20].count == 0) {
            s32 owned = 0;
            for (i = 0; i < 37; i++) {
                if (D_L03_00179BC0[i].active == 0 && i != 8 && i != 24) {
                    if (item_available[i] != 0) {
                        owned++;
                    }
                }
            }
            if (owned >= 2) {
                help_request(0x4E25, 0x4F);
            }
        }
        if (is_point_inside_clip_volume(&hero.motion.pos, tutorial_hint_state[28])) {
            alternate_item_available[0x18] = 1;
        }
        if (is_point_inside_clip_volume(&hero.motion.pos, tutorial_hint_state[29]) &&
            item_unlocked[3] == 0) {
            item_unlocked[3] = 1;
            allocate_voice_for_bank_entry(1, 0, 0);
            queue_dialogue_message(0x53D6, -1);
        }
        break;
    }
}
