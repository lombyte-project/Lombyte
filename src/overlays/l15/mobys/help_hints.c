/* Level 15 moby class help_hints (0x5bd): plays the Quartu help messages and talk prompts in its clip volumes and records the usage stats they depend on. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/overlay/shared/ui/help/help_request.h"
#include "rnc/game/game_globals.h"
#include "rnc/game/level_state.h"
#include "rnc/game/item_state.h"
#include "rnc/overlay/shared/hero/hero_switch_item.h"
#include "rnc/rendering/clip_volume.h"

typedef struct Talk {
    int  zone[4];
    int  npc[3];
    int  giant_clank_hint_volume; /* help 0x80 */
    int  dwell_volumes[3];
    int  visited_volume; /* sets hint_clip_volume_visited */
    int  pad30;
    int  wait;
    int  dwell_frames;
    int  npc_zone_frames;
    int  last;
    char pad44[0x54 - 0x44];
    int  help_88_volume;
} Talk;

typedef struct Help {
    int  busy;
    char p0[0x24 - 4];
    int  cur;
} Help;

/* The help records (HelpRecord D_00141968) of help messages 0x7B and 0x7C. */
typedef struct Stats {
    char           p0[0x3DA];
    unsigned short help_7b_latest_time_bucket;
    char           p1[0x3E0 - 0x3DC];
    unsigned short help_7c_count;
    unsigned short help_7c_latest_time_bucket;
    int            help_7c_level_flags;
} Stats;

typedef struct Items {
    char          p0[0x6E];
    unsigned char hint_clip_volume_visited;
    unsigned char giant_clank_zone_hint_suppressed;
} Items;

extern unsigned char D_0013D4DF_2ed398[] __asm__("D_0013D4DF");

extern int D_L15_00161AC0_macro __asm__("D_L15_00161AC0") MACRO_ADDR;

extern struct Moby *l15_moby_array_macro __asm__("D_L15_0015FFD8") MACRO_ADDR;

extern char D_00141968_2ed398[] __asm__("D_00141968");

extern int D_L15_0015F5A4 MACRO_ADDR;

extern char quartu_help_message_state[] __asm__("D_L15_00179A10");

extern unsigned char l15_dwell_zone_hint_enabled[] __asm__("D_0013D4C4");

extern int D_0015EEA4_macro __asm__("D_0015EEA4") MACRO_ADDR;

extern s32 current_level_index_macro __asm__("D_0015ED84") MACRO_ADDR;

void help_hints_update(struct Moby *moby) __asm__("FUN_L15_002ed398");

/* Moby class 1469 */
void help_hints_update(struct Moby *moby) {
    enum {
        HINT_DIALOGUE_INITIALIZE = 0,
        HINT_DIALOGUE_CHECK_TRIGGERS = 1
    };
    Talk        *hint_dialogue_data = (struct Talk *)moby->pvars;
    int          hint_dialogue_state = moby->state;
    struct Moby *npc_moby;
    int          i, j, found, npc, pick;

    switch (hint_dialogue_state) {
    case HINT_DIALOGUE_INITIALIZE:
        moby->update_dist = 0xFF;
        hint_dialogue_data->wait = scale_game_frames(0x4B0);
        moby->state = HINT_DIALOGUE_CHECK_TRIGGERS;
        break;
    case HINT_DIALOGUE_CHECK_TRIGGERS:
        if (hint_dialogue_data->npc[0] != -1) {
            npc_moby =
                (struct Moby *)((char *)l15_moby_array_macro + (hint_dialogue_data->npc[0] << 8));
            if (npc_moby != 0 && npc_moby->oclass == 0x4E && npc_moby->state != 0xFE &&
                npc_moby->state != 0xFD && npc_moby->state != 3 &&
                is_point_inside_clip_volume(&hero.motion.pos, hint_dialogue_data->zone[3]) != 0 &&
                (hero.state.control_mode < 2u || hero.state.control_mode == 9) &&
                ((Help *)quartu_help_message_state)->busy == 0 &&
                ((Help *)quartu_help_message_state)->cur == -1) {
                if (D_0013D4DF_2ed398[0] == 0) {
                    int lim, now;
                    now = scale_game_frames(D_0015EEA4_macro);
                    lim = now - ((Stats *)D_00141968_2ed398)->help_7b_latest_time_bucket * 600;
                    if ((int)((float)scale_game_frames(0x12) * 60.0f) < lim ||
                        ((Stats *)D_00141968_2ed398)->help_7b_latest_time_bucket * 600 == 0) {
                        help_request(0x3A98, 0x7B);
                    } else {
                        now = scale_game_frames(D_0015EEA4_macro);
                        if (now / 600 > ((Stats *)D_00141968_2ed398)->help_7b_latest_time_bucket) {
                            ((Stats *)D_00141968_2ed398)->help_7b_latest_time_bucket =
                                scale_game_frames(D_0015EEA4_macro) / 600;
                        }
                    }
                } else if (((Stats *)D_00141968_2ed398)->help_7c_count != 0) {
                    int lim, now;
                    now = scale_game_frames(D_0015EEA4_macro);
                    lim = now - ((Stats *)D_00141968_2ed398)->help_7c_latest_time_bucket * 600;
                    if ((int)((float)scale_game_frames(0x12) * 60.0f) < lim ||
                        ((Stats *)D_00141968_2ed398)->help_7c_latest_time_bucket * 600 == 0) {
                        help_request(0x3A99, 0x7C);
                    } else {
                        now = scale_game_frames(D_0015EEA4_macro);
                        if (now / 600 > ((Stats *)D_00141968_2ed398)->help_7c_latest_time_bucket) {
                            ((Stats *)D_00141968_2ed398)->help_7c_latest_time_bucket =
                                scale_game_frames(D_0015EEA4_macro) / 600;
                        }
                    }
                } else {
                    int now;
                    ((Stats *)D_00141968_2ed398)->help_7c_count++;
                    now = scale_game_frames(D_0015EEA4_macro);
                    if (now / 600 > ((Stats *)D_00141968_2ed398)->help_7c_latest_time_bucket) {
                        ((Stats *)D_00141968_2ed398)->help_7c_latest_time_bucket =
                            scale_game_frames(D_0015EEA4_macro) / 600;
                    }
                    ((Stats *)D_00141968_2ed398)->help_7c_level_flags =
                        ((Stats *)D_00141968_2ed398)->help_7c_level_flags |
                        (1 << current_level_index_macro) | 0x80000000;
                }
            }
        }
        if (D_L15_0015F5A4 != 0 && ((Help *)quartu_help_message_state)->busy == 0 &&
            ((Help *)quartu_help_message_state)->cur == -1 &&
            *(unsigned short *)(item_usage + 0x730) == 0) {
            help_request(0x3A9F, 0x89);
            D_L15_0015F5A4 = 0;
        }
        if (is_point_inside_clip_volume(&hero.motion.pos, hint_dialogue_data->help_88_volume) !=
                0 &&
            hero.vehicle_mode == 3 && hero.glow_pulse_level == 0 && D_L15_00161AC0_macro == 0 &&
            *(unsigned short *)(item_usage + 0x728) == 0) {
            help_request(0x3A9E, 0x88);
        }
        pick = -1;
        npc = -1;
        found = 0;
        for (i = 0; i < 1; i++) {
            if (is_point_inside_clip_volume(&hero.motion.pos, hint_dialogue_data->zone[i]) != 0) {
                pick = hint_dialogue_data->zone[i];
                npc = hint_dialogue_data->npc[i];
                found = 1;
                break;
            }
        }
        if (found != 0) {
            hint_dialogue_data->npc_zone_frames = hint_dialogue_data->npc_zone_frames + 1;
        } else {
            hint_dialogue_data->npc_zone_frames = 0;
            hint_dialogue_data->last = -1;
        }
        if (npc != -1) {
            npc_moby = (struct Moby *)((char *)l15_moby_array_macro + (npc << 8));
            if (npc_moby != 0 && npc_moby->oclass == 0x4E && npc_moby->state != 0xFE &&
                npc_moby->state != 0xFD && npc_moby->state != 3 &&
                hint_dialogue_data->npc_zone_frames > hint_dialogue_data->wait && pick != -1 &&
                pick != hint_dialogue_data->last &&
                (hero.state.control_mode < 2u || hero.state.control_mode == 9) &&
                ((Help *)quartu_help_message_state)->busy == 0 &&
                ((Help *)quartu_help_message_state)->cur == -1 && D_0013D4DF_2ed398[0] != 0) {
                hint_dialogue_data->wait = scale_game_frames(0x708);
                if (*(int *)(item_usage + 0x6D4) >= 0) {
                    help_request(0x3A9A, 0x7D);
                }
                hint_dialogue_data->last = pick;
                hint_dialogue_data->npc_zone_frames = 0;
            }
        }
        if (hero.moby->seq == 0xC && hero.vehicle_mode == HERO_VEHICLE_MODE_GIANT_CLANK) {
            ((Items *)alternate_item_available)->giant_clank_zone_hint_suppressed = 1;
        }
        if (is_point_inside_clip_volume(&hero.motion.pos,
                                        hint_dialogue_data->giant_clank_hint_volume) != 0 &&
            hero.vehicle_mode == HERO_VEHICLE_MODE_GIANT_CLANK &&
            ((Items *)alternate_item_available)->giant_clank_zone_hint_suppressed == 0 &&
            ((Help *)quartu_help_message_state)->busy == 0 &&
            ((Help *)quartu_help_message_state)->cur == -1 && *(int *)(item_usage + 0x6EC) >= 0) {
            help_request(0x3A9C, 0x80);
        }
        if (is_point_inside_clip_volume(&hero.motion.pos, hint_dialogue_data->dwell_volumes[0]) !=
                0 &&
            ((Help *)quartu_help_message_state)->busy == 0 &&
            ((Help *)quartu_help_message_state)->cur == -1 && l15_dwell_zone_hint_enabled[0] == 0 &&
            !(*(unsigned int *)(item_usage + 0x6FC) & (1 << current_level_index_macro))) {
            help_request(0x3A9D, 0x82);
        }
        pick = -1;
        found = 0;
        for (j = 0; j < 3; j++) {
            if (is_point_inside_clip_volume(&hero.motion.pos,
                                            hint_dialogue_data->dwell_volumes[j]) != 0) {
                found = 1;
                if (hint_dialogue_data->last != hint_dialogue_data->dwell_volumes[j]) {
                    pick = hint_dialogue_data->dwell_volumes[j];
                }
                break;
            }
        }
        if (found == 0) {
            hint_dialogue_data->dwell_frames = 0;
            hint_dialogue_data->last = -1;
        } else {
            hint_dialogue_data->dwell_frames = hint_dialogue_data->dwell_frames + 1;
            if (hero.state.current == 0x35) {
                hint_dialogue_data->dwell_frames = 0;
                hint_dialogue_data->last = -1;
            }
        }
        if (scale_game_frames(0xE10) < hint_dialogue_data->dwell_frames && pick != -1 &&
            hero_is_in_special_state() != 0 && ((Help *)quartu_help_message_state)->busy == 0 &&
            ((Help *)quartu_help_message_state)->cur == -1 && l15_dwell_zone_hint_enabled[0] != 0) {
            help_request(0x4E2E, 0x78);
            hint_dialogue_data->last = pick;
            hint_dialogue_data->dwell_frames = 0;
        }
        if (((Items *)alternate_item_available)->hint_clip_volume_visited == 0 &&
            is_point_inside_clip_volume(&hero.motion.pos, hint_dialogue_data->visited_volume) !=
                0) {
            ((Items *)alternate_item_available)->hint_clip_volume_visited = 1;
        }
        break;
    }
}
