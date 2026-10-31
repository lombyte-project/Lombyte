/* Level 01 moby class help_hints (0x53d): plays the Novalis help messages and counts usage stats. */
#include "types.h"
#include "rnc/game/game_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/hero/hero.h"
#include "rnc/game/usage_stats.h"
#include "rnc/mobys/moby.h"
#include "rnc/game/level_state.h"

/* Help message playback state (same layout as S00203908 in shared/ui/help/help_request.c). */
struct L01HelpState {
    s32 state;
    u8  pad4[0x20];
    s32 cur;
};

/* Per-message help record at D_00141968: id 0xFFFF = disabled. */
struct L01HelpRecord {
    u16 id;
    u16 pad;
    s32 v;
};

extern struct L01HelpState help_state __asm__("D_L01_00179890");

extern struct L01HelpRecord help_records[] __asm__("D_00141968");
extern s32                  cutscene_mode_macro __asm__("D_L01_0015F5C4") MACRO_ADDR;
extern s32 D_L01_0015F688   MACRO_ADDR;
extern s32 D_L01_0015F68C   MACRO_ADDR;
extern struct Moby         *moby_list __asm__("D_L01_0015FFE4") MACRO_ADDR;
extern s32                  help_request(s32, s32) __asm__("FUN_L00_00203908");
extern s32                  is_moby_in_state_one(void *) __asm__("FUN_L00_002d8128");
extern s32                  check_scaled_vector_bsphere(f32, void *) __asm__("FUN_L01_00275690");

extern int D_0015EEA4_macro __asm__("D_0015EEA4") MACRO_ADDR;
extern s32 current_level_index_macro __asm__("D_0015ED84") MACRO_ADDR;
extern s32 level_frame_count __asm__("D_L01_0015F5CC");
/* Novalis tutorial hint triggers: plays help messages and counts usage stats. */
void help_hints_update(struct Moby *moby) __asm__("FUN_L01_0030acb8");

/* Moby class 1341 */
void help_hints_update(struct Moby *moby) {
    enum {
        TUTORIAL_HINT_STATE_INITIALIZE = 0,
        TUTORIAL_HINT_STATE_UPDATE = 1
    };
    s32         *tutorial_hint_vars;
    struct Moby *hint_candidate_moby;
    struct Moby *health_hint_candidate_moby;
    s32          usage_time_value;

    tutorial_hint_vars = (s32 *)moby->pvars;
    fast_dec_timer(&tutorial_hint_vars[6]);
    switch (moby->state) {
    case TUTORIAL_HINT_STATE_INITIALIZE:
        tutorial_hint_vars[5] = 0;
        moby->update_dist = 0xFF;
        moby->state = 1;
        tutorial_hint_vars[10] = 0;
        break;
    case TUTORIAL_HINT_STATE_UPDATE:
        if (level_frame_count < scale_game_frames(400) && usage_stats.stat[12].count != 0 &&
            usage_stats.stat[11].count == 0) {
            if (scale_game_frames(D_0015EEA4_macro) - usage_stats.stat[12].unk2 * 600 <
                    scale_game_frames(1100) &&
                help_records[0x51].v >= 0) {
                help_request(0x4E27, 0x51);
            }
        }
        if (level_frame_count < scale_game_frames(400) && usage_stats.stat[29].count != 0) {
            if (scale_game_frames(D_0015EEA4_macro) - usage_stats.stat[29].unk2 * 600 <
                    scale_game_frames(1100) &&
                help_records[0x72].v >= 0) {
                help_request(0x4E28, 0x72);
            }
        }
        if (cutscene_mode_macro == 0) {
            if (level_available[3] != 0 && level_available[2] == 0 && help_state.state == 0 &&
                help_state.cur == -1 && help_records[4].id == 0) {
                help_request(0x3E8, 4);
            } else if (level_available[3] == 0 && level_available[2] != 0 &&
                       help_state.state == 0 && help_state.cur == -1 && help_records[5].id == 0) {
                help_request(0x3E9, 5);
            } else if (level_available[3] != 0 && level_available[2] != 0 &&
                       help_state.state == 0 && help_state.cur == -1 && help_records[6].id == 0 &&
                       help_records[4].id == 0) {
                help_request(0x3EA, 6);
            }
        }
        if (is_point_inside_clip_volume(&hero.motion.pos, tutorial_hint_vars[15]) &&
            ((u32)hero.state.control_mode < 2 || hero.state.control_mode == 9) &&
            help_state.state == 0 && help_state.cur == -1) {
            usage_time_value = scale_game_frames(D_0015EEA4_macro) - usage_stats.stat[9].unk2 * 600;
            /* "< 1" rather than "!": cc1 would otherwise test the bit as (v >> level) & 1 */
            if (scale_game_frames(216000) < usage_time_value &&
                (u32)(help_records[0x43].v & (1 << current_level_index_macro)) < 1) {
                help_request(0x3EF, 0x43);
            }
        }
        if (is_point_inside_clip_volume(&hero.motion.pos, tutorial_hint_vars[14]) &&
            ((u32)hero.state.control_mode < 2 || hero.state.control_mode == 9) &&
            D_L01_0015F68C < 3 && help_state.state == 0 && help_state.cur == -1 &&
            help_records[0x40].id == 0) {
            help_request(0x3EC, 0x40);
        }
        if (is_point_inside_clip_volume(&hero.motion.pos, tutorial_hint_vars[14]) &&
            ((u32)hero.state.control_mode < 2 || hero.state.control_mode == 9) &&
            hero.state.current == 1 && D_L01_0015F688 == -1 && help_state.state == 0 &&
            help_state.cur == -1 && help_records[0x40].id != 0 && help_records[0x41].id == 0) {
            help_request(0x3ED, 0x41);
        }
        if (help_records[0x42].id < 2 &&
            is_point_inside_clip_volume(&hero.motion.pos, tutorial_hint_vars[12]) &&
            hero.state.prev == 0x3B && hero.state_timer == 0) {
            for (hint_candidate_moby = moby_list; hint_candidate_moby != 0;
                 hint_candidate_moby = hint_candidate_moby->next) {
                if (hint_candidate_moby->oclass == 0x118 &&
                    fast_vec_dist(&hint_candidate_moby->pos, &hero.motion.pos) < 3.0f &&
                    *(f32 *)hint_candidate_moby->pvars != 1.0f) {
                    tutorial_hint_vars[13]++;
                    if (tutorial_hint_vars[13] == 3 || tutorial_hint_vars[13] == 6) {
                        help_request(0x3EE, 0x42);
                    }
                }
            }
        }
        if (hero.state.control_mode == 0x12 && tutorial_hint_vars[10] == 0 &&
            help_state.state == 0 && help_state.cur == -1 &&
            (is_point_inside_clip_volume(&hero.motion.pos, tutorial_hint_vars[8]) ||
             is_point_inside_clip_volume(&hero.motion.pos, tutorial_hint_vars[9]))) {
            tutorial_hint_vars[11]++;
            if (is_point_inside_clip_volume(&hero.motion.pos, tutorial_hint_vars[8]) &&
                scale_game_frames(240) < tutorial_hint_vars[11] &&
                usage_stats.stat[22].count == 0) {
                help_request(0x3F0, 0x44);
                if (usage_stats.stat[22].count < 0xFFFF) {
                    usage_stats.stat[22].count++;
                }
                usage_time_value = scale_game_frames(D_0015EEA4_macro) / 600;
                if (usage_stats.stat[22].unk2 < usage_time_value) {
                    usage_stats.stat[22].unk2 = scale_game_frames(D_0015EEA4_macro) / 600;
                }
                usage_stats.stat[22].level_mask =
                    usage_stats.stat[22].level_mask | (1 << current_level_index_macro) | 0x80000000;
            } else if (is_point_inside_clip_volume(&hero.motion.pos, tutorial_hint_vars[9]) &&
                       scale_game_frames(360) < tutorial_hint_vars[11] &&
                       usage_stats.stat[23].count == 0) {
                help_request(0x3F0, 0x44);
                if (usage_stats.stat[23].count < 0xFFFF) {
                    usage_stats.stat[23].count++;
                }
                usage_time_value = scale_game_frames(D_0015EEA4_macro) / 600;
                if (usage_stats.stat[23].unk2 < usage_time_value) {
                    usage_stats.stat[23].unk2 = scale_game_frames(D_0015EEA4_macro) / 600;
                }
                usage_stats.stat[23].level_mask =
                    usage_stats.stat[23].level_mask | (1 << current_level_index_macro) | 0x80000000;
            }
        } else if (hero.state.control_mode == 0x11) {
            help_records[0x44].id = 0xFFFF;
        } else {
            tutorial_hint_vars[11] = 0;
        }
        for (health_hint_candidate_moby = moby_list; health_hint_candidate_moby != 0;
             health_hint_candidate_moby = health_hint_candidate_moby->next) {
            if (health_hint_candidate_moby->oclass == 0x326 &&
                fast_vec_dist(&health_hint_candidate_moby->pos, &hero.motion.pos) < 10.0f &&
                help_state.state == 0 && help_state.cur == -1 && help_records[1].id == 0 &&
                help_records[2].id == 0 && usage_stats.stat[13].count == 0 &&
                is_moby_in_state_one(health_hint_candidate_moby) == 0) {
                if (hero.health.hp < hero_max_health) {
                    if (usage_stats.stat[13].count < 0xFFFF) {
                        usage_stats.stat[13].count++;
                    }
                    usage_time_value = scale_game_frames(D_0015EEA4_macro) / 600;
                    if (usage_stats.stat[13].unk2 < usage_time_value) {
                        usage_stats.stat[13].unk2 = scale_game_frames(D_0015EEA4_macro) / 600;
                    }
                    usage_stats.stat[13].level_mask = usage_stats.stat[13].level_mask |
                                                      (1 << current_level_index_macro) | 0x80000000;
                    help_request(1, 1);
                } else if (check_scaled_vector_bsphere(255.0f, health_hint_candidate_moby) != 0) {
                    help_request(2, 2);
                }
            }
        }
        break;
    }
}
