/* Level 15 moby class help_hints (0x5bd): plays the Quartu help messages and talk prompts in its clip volumes and records the usage stats they depend on. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/overlay/shared/ui/help/help_request.h"
#include "rnc/game/game_globals.h"
#include "rnc/game/level_state.h"
#include "rnc/game/item_state.h"

extern int is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");

typedef struct Talk {
    int  zone[4];
    int  npc[3];
    int  vol1C;
    int  vol20[3];
    int  vol2C;
    int  pad30;
    int  wait;
    int  hold38;
    int  hold3C;
    int  last;
    char pad44[0x54 - 0x44];
    int  vol54;
} Talk;

typedef struct Help {
    int  busy;
    char p0[0x24 - 4];
    int  cur;
} Help;

typedef struct Stats {
    char           p0[0x3DA];
    unsigned short t3DA;
    char           p1[0x3E0 - 0x3DC];
    unsigned short n3E0;
    unsigned short t3E2;
    int            lv3E4;
} Stats;

typedef struct Items {
    char          p0[0x6E];
    unsigned char f6E;
    unsigned char f6F;
} Items;

extern int hero_is_in_special_state(void) __asm__("FUN_L00_0020d568");

extern unsigned char D_0013D4DF_2ed398[] __asm__("D_0013D4DF");

extern int D_L15_00161AC0_macro __asm__("D_L15_00161AC0") MACRO_ADDR;

extern struct Moby *l15_moby_array_macro __asm__("D_L15_0015FFD8") MACRO_ADDR;

extern char D_00141968_2ed398[] __asm__("D_00141968");

extern int D_L15_0015F5A4 MACRO_ADDR;

extern char quartu_help_message_state[] __asm__("D_L15_00179A10");

extern unsigned char l15_dwell_zone_hint_enabled[] __asm__("D_0013D4C4");

extern int D_0015EEA4_macro __asm__("D_0015EEA4") MACRO_ADDR;

extern s32 current_level_index_macro __asm__("D_0015ED84") MACRO_ADDR;

void update_help_hints(struct Moby *moby) __asm__("FUN_L15_002ed398");

void update_help_hints(struct Moby *moby) {
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
        moby->state = 1;
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
                    lim = now - ((Stats *)D_00141968_2ed398)->t3DA * 600;
                    if ((int)((float)scale_game_frames(0x12) * 60.0f) < lim ||
                        ((Stats *)D_00141968_2ed398)->t3DA * 600 == 0) {
                        help_request(0x3A98, 0x7B);
                    } else {
                        now = scale_game_frames(D_0015EEA4_macro);
                        if (now / 600 > ((Stats *)D_00141968_2ed398)->t3DA) {
                            ((Stats *)D_00141968_2ed398)->t3DA =
                                scale_game_frames(D_0015EEA4_macro) / 600;
                        }
                    }
                } else if (((Stats *)D_00141968_2ed398)->n3E0 != 0) {
                    int lim, now;
                    now = scale_game_frames(D_0015EEA4_macro);
                    lim = now - ((Stats *)D_00141968_2ed398)->t3E2 * 600;
                    if ((int)((float)scale_game_frames(0x12) * 60.0f) < lim ||
                        ((Stats *)D_00141968_2ed398)->t3E2 * 600 == 0) {
                        help_request(0x3A99, 0x7C);
                    } else {
                        now = scale_game_frames(D_0015EEA4_macro);
                        if (now / 600 > ((Stats *)D_00141968_2ed398)->t3E2) {
                            ((Stats *)D_00141968_2ed398)->t3E2 =
                                scale_game_frames(D_0015EEA4_macro) / 600;
                        }
                    }
                } else {
                    int now;
                    ((Stats *)D_00141968_2ed398)->n3E0++;
                    now = scale_game_frames(D_0015EEA4_macro);
                    if (now / 600 > ((Stats *)D_00141968_2ed398)->t3E2) {
                        ((Stats *)D_00141968_2ed398)->t3E2 =
                            scale_game_frames(D_0015EEA4_macro) / 600;
                    }
                    ((Stats *)D_00141968_2ed398)->lv3E4 = ((Stats *)D_00141968_2ed398)->lv3E4 |
                                                          (1 << current_level_index_macro) |
                                                          0x80000000;
                }
            }
        }
        if (D_L15_0015F5A4 != 0 && ((Help *)quartu_help_message_state)->busy == 0 &&
            ((Help *)quartu_help_message_state)->cur == -1 &&
            *(unsigned short *)(item_usage + 0x730) == 0) {
            help_request(0x3A9F, 0x89);
            D_L15_0015F5A4 = 0;
        }
        if (is_point_inside_clip_volume(&hero.motion.pos, hint_dialogue_data->vol54) != 0 &&
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
            hint_dialogue_data->hold3C = hint_dialogue_data->hold3C + 1;
        } else {
            hint_dialogue_data->hold3C = 0;
            hint_dialogue_data->last = -1;
        }
        if (npc != -1) {
            npc_moby = (struct Moby *)((char *)l15_moby_array_macro + (npc << 8));
            if (npc_moby != 0 && npc_moby->oclass == 0x4E && npc_moby->state != 0xFE &&
                npc_moby->state != 0xFD && npc_moby->state != 3 &&
                hint_dialogue_data->hold3C > hint_dialogue_data->wait && pick != -1 &&
                pick != hint_dialogue_data->last &&
                (hero.state.control_mode < 2u || hero.state.control_mode == 9) &&
                ((Help *)quartu_help_message_state)->busy == 0 &&
                ((Help *)quartu_help_message_state)->cur == -1 && D_0013D4DF_2ed398[0] != 0) {
                hint_dialogue_data->wait = scale_game_frames(0x708);
                if (*(int *)(item_usage + 0x6D4) >= 0) {
                    help_request(0x3A9A, 0x7D);
                }
                hint_dialogue_data->last = pick;
                hint_dialogue_data->hold3C = 0;
            }
        }
        if (hero.moby->seq == 0xC && hero.vehicle_mode == 2) {
            ((Items *)alternate_item_available)->f6F = 1;
        }
        if (is_point_inside_clip_volume(&hero.motion.pos, hint_dialogue_data->vol1C) != 0 &&
            hero.vehicle_mode == 2 && ((Items *)alternate_item_available)->f6F == 0 &&
            ((Help *)quartu_help_message_state)->busy == 0 &&
            ((Help *)quartu_help_message_state)->cur == -1 && *(int *)(item_usage + 0x6EC) >= 0) {
            help_request(0x3A9C, 0x80);
        }
        if (is_point_inside_clip_volume(&hero.motion.pos, hint_dialogue_data->vol20[0]) != 0 &&
            ((Help *)quartu_help_message_state)->busy == 0 &&
            ((Help *)quartu_help_message_state)->cur == -1 && l15_dwell_zone_hint_enabled[0] == 0 &&
            !(*(unsigned int *)(item_usage + 0x6FC) & (1 << current_level_index_macro))) {
            help_request(0x3A9D, 0x82);
        }
        pick = -1;
        found = 0;
        for (j = 0; j < 3; j++) {
            if (is_point_inside_clip_volume(&hero.motion.pos, hint_dialogue_data->vol20[j]) != 0) {
                found = 1;
                if (hint_dialogue_data->last != hint_dialogue_data->vol20[j]) {
                    pick = hint_dialogue_data->vol20[j];
                }
                break;
            }
        }
        if (found == 0) {
            hint_dialogue_data->hold38 = 0;
            hint_dialogue_data->last = -1;
        } else {
            hint_dialogue_data->hold38 = hint_dialogue_data->hold38 + 1;
            if (hero.state.current == 0x35) {
                hint_dialogue_data->hold38 = 0;
                hint_dialogue_data->last = -1;
            }
        }
        if (scale_game_frames(0xE10) < hint_dialogue_data->hold38 && pick != -1 &&
            hero_is_in_special_state() != 0 && ((Help *)quartu_help_message_state)->busy == 0 &&
            ((Help *)quartu_help_message_state)->cur == -1 && l15_dwell_zone_hint_enabled[0] != 0) {
            help_request(0x4E2E, 0x78);
            hint_dialogue_data->last = pick;
            hint_dialogue_data->hold38 = 0;
        }
        if (((Items *)alternate_item_available)->f6E == 0 &&
            is_point_inside_clip_volume(&hero.motion.pos, hint_dialogue_data->vol2C) != 0) {
            ((Items *)alternate_item_available)->f6E = 1;
        }
        break;
    }
}
