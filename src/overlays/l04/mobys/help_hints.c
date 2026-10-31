/* Level 04 moby class help_hints (0x53f): plays the Eudora help messages and counts usage stats around the hero. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/data_labels.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/game/usage_stats.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/game/game_globals.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/overlay/shared/ui/help/help_request.h"
#include "rnc/overlay/level_labels.h"

/* UpdateMoby_1343: tutorial hint and statistic triggers around the player on Eudora. */
/* Ported from rac1-decomp (src/overlays/l04_eudora/vendor_002CB800.c: func_L04_002E57F8), where it is exact; names translated to the US level program. */

typedef struct Hero Hero;

typedef struct Moby Moby;

typedef struct {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
    int f18;
    int f1C;
    int f20;
    int f24;
    int f28;
    int f2C;
    int f30;
    int f34;
    int f38;
    int f3C;
    int f40;
    int f44;
    int f48;
    int f4C;
    int f50;
} Data;

typedef struct {
    unsigned short count;
    unsigned short time;
    unsigned int   mask;
} Help;

extern int                       D_L04_00179590[] NOT_SDA;
extern unsigned char D_0013D4D4  NOT_SDA;
extern unsigned short D_001417C2 NOT_SDA;
extern unsigned short D_00141892 NOT_SDA;
extern unsigned short D_001418D0 NOT_SDA;
extern unsigned short D_00141908 NOT_SDA;
extern unsigned short D_00141910 NOT_SDA;
extern unsigned short D_00141940 NOT_SDA;
extern char                      D_00141968[];
extern Moby                     *D_L04_00160064_2E57F8 __asm__("D_L04_0015FFE4");

extern s32 current_level_index_macro __asm__("D_0015ED84") MACRO_ADDR;
extern int D_0015EEA4_macro __asm__("D_0015EEA4") MACRO_ADDR;
extern s32 D_0015EEAC_macro __asm__("D_0015EEAC") MACRO_ADDR;
void       help_hints_update(Moby *moby) __asm__("FUN_L04_002e4418");

/* Moby class 1343 */
void help_hints_update(Moby *moby) {
    enum {
        TUTORIAL_TRIGGER_STATE_INITIALIZE = 0,
        TUTORIAL_TRIGGER_STATE_UPDATE_HINTS_AND_STATS = 1
    };
    Data *tutorial_trigger_vars = *(void **)&moby->pvars;

    switch (moby->state) {
    case TUTORIAL_TRIGGER_STATE_INITIALIZE:
        moby->update_dist = 0xFF;
        moby->state = 1;
        tutorial_trigger_vars->f14 = -1;
        break;
    case TUTORIAL_TRIGGER_STATE_UPDATE_HINTS_AND_STATS:
        if (is_point_inside_clip_volume(((char *)&D_0013F3D0), tutorial_trigger_vars->f0) != 0 &&
            D_L04_00179590[0] == 0 && D_L04_00179590[9] == -1 &&
            visited52[moby->mission + (current_level_index_macro << 4)] != 0xFF) {
            if (tutorial_trigger_vars->f14 == -1) {
                struct UsageStat *usage = usage_stats.stat;
                tutorial_trigger_vars->f14 = D_0015EEAC_macro;
                if (usage[0x18].count < 0xFFFF) {
                    usage[0x18].count++;
                }
                if (scale_game_frames(D_0015EEA4_macro) / 600 > usage[0x18].unk2) {
                    usage[0x18].unk2 = scale_game_frames(D_0015EEA4_macro) / 600;
                }
                usage[0x18].level_mask =
                    usage[0x18].level_mask | (1 << current_level_index_macro) | 0x80000000;
            } else if (D_00141908 >= 3) {
                if (D_0013D4D4 == 0) {
                    Help *help = (Help *)(((char *)&D_00141968));
                    if (help[0x1D].count == 0) {
                        help_request(0xFA1, 0x1D);
                    }
                } else {
                    int elapsed = scale_game_frames(D_0015EEA4_macro) - D_001417C2 * 600;
                    if (scale_game_frames(0x4650) < elapsed) {
                        Help *help = (Help *)(((char *)&D_00141968));
                        if (help[0x1C].count == 0) {
                            help_request(0xFA0, 0x1C);
                        }
                    }
                }
            }
        }

        {
            Help *help = (Help *)(((char *)&D_00141968));
            if (help[0x42].count < 4) {
                if (is_point_inside_clip_volume(((char *)&D_0013F3D0),
                                                tutorial_trigger_vars->f34) != 0 ||
                    is_point_inside_clip_volume(((char *)&D_0013F3D0),
                                                tutorial_trigger_vars->f38) != 0) {
                    Hero *hero = (Hero *)(((char *)&D_0013F3D0 - 0x80));
                    if (hero->state.prev == 0x3B && hero->state_timer == 0) {
                        Moby *hint_candidate_moby;
                        for (hint_candidate_moby = D_L04_00160064_2E57F8; hint_candidate_moby != 0;
                             hint_candidate_moby = hint_candidate_moby->next) {
                            if (hint_candidate_moby->oclass == 0x118 &&
                                fast_vec_dist(((float *)&hint_candidate_moby->pos),
                                              ((char *)&D_0013F3D0)) < 3.0f &&
                                *(float *)*(void **)&hint_candidate_moby->pvars != 1.0f) {
                                tutorial_trigger_vars->f20++;
                                if (tutorial_trigger_vars->f20 == 3 ||
                                    tutorial_trigger_vars->f20 == 6) {
                                    help_request(0x3EE, 0x42);
                                }
                            }
                        }
                    }
                }
            }
        }

        {
            unsigned char *item_flags = ((char *)&items_acquired);
            if (item_flags[9] != 0 && D_L04_00179590[0] == 0 && D_L04_00179590[9] == -1) {
                Help *help = (Help *)(((char *)&D_00141968));
                if (help[0x1E].count == 0) {
                    help_request(0xFA2, 0x1E);
                }
            }
        }

        if (is_point_inside_clip_volume(((char *)&D_0013F3D0), tutorial_trigger_vars->f3C) != 0 &&
            D_00141940 == 0 && D_L04_00179590[0] == 0 && D_L04_00179590[9] == -1) {
            Help *help = (Help *)(((char *)&D_00141968));
            if (help[0x77].count == 0) {
                help_request(0x4E2D, 0x77);
            }
        }

        if (tutorial_trigger_vars->f48 != -1) {
            if (((struct Moby *)moby_array)[tutorial_trigger_vars->f48].oclass == 0x267 &&
                ((struct Moby *)moby_array)[tutorial_trigger_vars->f48].state == 4) {
                menu_item_enabled_16 = 1;
            }
        }
        if (tutorial_trigger_vars->f4C != -1) {
            if (((struct Moby *)moby_array)[tutorial_trigger_vars->f4C].oclass == 0x267 &&
                ((struct Moby *)moby_array)[tutorial_trigger_vars->f4C].state == 4) {
                menu_item_enabled_17 = 1;
            }
        }
        if (tutorial_trigger_vars->f40 != -1) {
            if (((struct Moby *)moby_array)[tutorial_trigger_vars->f40].oclass == 0x118 &&
                ((struct Moby *)moby_array)[tutorial_trigger_vars->f40].state == 5) {
                menu_item_enabled_18 = 1;
            }
        }
        if (tutorial_trigger_vars->f44 != -1) {
            if (((struct Moby *)moby_array)[tutorial_trigger_vars->f44].oclass == 0x118 &&
                ((struct Moby *)moby_array)[tutorial_trigger_vars->f44].state == 5) {
                menu_item_enabled_19 = 1;
            }
        }

        if (is_point_inside_clip_volume(((char *)&D_0013F3D0), tutorial_trigger_vars->f4) != 0 &&
            (((unsigned int)((Hero *)(((char *)&D_0013F3D0 - 0x80)))->state.control_mode) < 2 ||
             ((unsigned int)((Hero *)(((char *)&D_0013F3D0 - 0x80)))->state.control_mode) == 9) &&
            D_L04_00179590[0] == 0 && D_L04_00179590[9] == -1) {
            if (D_0013D4DA == 0) {
                Help *help = (Help *)(((char *)&D_00141968));
                int   elapsed = scale_game_frames(D_0015EEA4_macro) - help[0x1F].time * 600;
                if ((int)(scale_game_frames(0x12) * 60.0f) < elapsed ||
                    help[0x1F].time * 600 == 0) {
                    help_request(0xFA3, 0x1F);
                } else {
                    int now = scale_game_frames(D_0015EEA4_macro) / 600;
                    if (help[0x1F].time < now) {
                        help[0x1F].time = scale_game_frames(D_0015EEA4_macro) / 600;
                    }
                }
            } else {
                if (tutorial_trigger_vars->f48 != -1 &&
                    ((struct Moby *)moby_array)[tutorial_trigger_vars->f48].oclass == 0x267 &&
                    ((struct Moby *)moby_array)[tutorial_trigger_vars->f48].state == 4) {
                    struct UsageStat *usage = usage_stats.stat;
                    unsigned int      count = usage[0x11].count;
                    if (count <= 0xFFFE) {
                        usage[0x11].count = count + 1;
                    }
                    if (scale_game_frames(D_0015EEA4_macro) / 600 > usage[0x11].unk2) {
                        usage[0x11].unk2 = scale_game_frames(D_0015EEA4_macro) / 600;
                    }
                    usage[0x11].level_mask =
                        usage[0x11].level_mask | (1 << current_level_index_macro) | 0x80000000;
                } else if (tutorial_trigger_vars->fC == 0) {
                    if (D_001418D0 == 0) {
                        Help *help;
                        tutorial_trigger_vars->f8++;
                        help = (Help *)(((char *)&D_00141968));
                        if (help[0x20].count != 0) {
                            int elapsed =
                                scale_game_frames(D_0015EEA4_macro) - help[0x20].time * 600;
                            if ((int)(scale_game_frames(0x12) * 60.0f) < elapsed ||
                                help[0x20].time * 600 == 0) {
                                help_request(0xFA4, 0x20);
                            } else {
                                int now = scale_game_frames(D_0015EEA4_macro) / 600;
                                if (help[0x20].time < now) {
                                    help[0x20].time = scale_game_frames(D_0015EEA4_macro) / 600;
                                }
                            }
                        } else {
                            int now;
                            help[0x20].count++;
                            now = scale_game_frames(D_0015EEA4_macro) / 600;
                            if (help[0x20].time < now) {
                                help[0x20].time = scale_game_frames(D_0015EEA4_macro) / 600;
                            }
                            help[0x20].mask =
                                help[0x20].mask | (1 << current_level_index_macro) | 0x80000000;
                        }
                    }
                }
            }
        } else if (tutorial_trigger_vars->fC != 0) {
            if (is_point_inside_clip_volume(((char *)&D_0013F3D0), tutorial_trigger_vars->f4) ==
                0) {
                tutorial_trigger_vars->fC = 0;
            }
        }

        if (is_point_inside_clip_volume(((char *)&D_0013F3D0), tutorial_trigger_vars->f50) != 0 &&
            (((unsigned int)((Hero *)(((char *)&D_0013F3D0 - 0x80)))->state.control_mode) < 2 ||
             ((unsigned int)((Hero *)(((char *)&D_0013F3D0 - 0x80)))->state.control_mode) == 9) &&
            D_L04_00179590[0] == 0 && D_L04_00179590[9] == -1) {
            int elapsed = scale_game_frames(D_0015EEA4_macro) - D_00141892 * 600;
            if (scale_game_frames(0x34BC0) < elapsed) {
                Help *help = (Help *)(((char *)&D_00141968));
                if (!(help[0x43].mask & (1 << current_level_index_macro))) {
                    help_request(0x3EF, 0x43);
                }
            }
        }

        {
            Help *help = (Help *)(((char *)&D_00141968));
            if (help[0x46].count == 0 || help[0x47].count == 0) {
                Hero *hero = (Hero *)(((char *)&D_0013F3D0 - 0x80));
                Moby *ground;
                if (hero->state.current == 0x72 && (ground = hero->ground_moby) != 0 &&
                    ground->oclass == 0x267 &&
                    is_point_inside_clip_volume((char *)hero + 0x80, tutorial_trigger_vars->f4) !=
                        0) {
                    if (help[0x46].count == 0) {
                        help_request(0x7DB, 0x46);
                    } else if (help[0x47].count == 0) {
                        tutorial_trigger_vars->f1C++;
                        if (scale_game_frames(0x708) < tutorial_trigger_vars->f1C) {
                            help_request(0x7DC, 0x47);
                        }
                    }
                } else {
                    tutorial_trigger_vars->f1C = 0;
                }
            }
        }

        if (is_point_inside_clip_volume(((char *)&D_0013F3D0), tutorial_trigger_vars->f30) != 0) {
            struct UsageStat *usage = usage_stats.stat;
            if (usage[0x19].count == 0 &&
                (((unsigned int)((Hero *)(((char *)&D_0013F3D0 - 0x80)))->state.control_mode) < 2 ||
                 ((unsigned int)((Hero *)(((char *)&D_0013F3D0 - 0x80)))->state.control_mode) ==
                     9)) {
                int now;
                usage[0x19].count++;
                now = scale_game_frames(D_0015EEA4_macro) / 600;
                if (usage[0x19].unk2 < now) {
                    usage[0x19].unk2 = scale_game_frames(D_0015EEA4_macro) / 600;
                }
                usage[0x19].level_mask =
                    usage[0x19].level_mask | (1 << current_level_index_macro) | 0x80000000;
            }
        }

        if (is_point_inside_clip_volume(((char *)&D_0013F3D0), tutorial_trigger_vars->f2C) != 0 &&
            D_00141910 == 0 &&
            ((unsigned int)((Hero *)(((char *)&D_0013F3D0 - 0x80)))->state.control_mode) >= 2 &&
            ((unsigned int)((Hero *)(((char *)&D_0013F3D0 - 0x80)))->state.control_mode) != 9 &&
            tutorial_trigger_vars->f28 == 0) {
            tutorial_trigger_vars->f24++;
            tutorial_trigger_vars->f28 = scale_game_frames(0xB4);
        } else if (tutorial_trigger_vars->f28 != 0) {
            if (((unsigned int)((Hero *)(((char *)&D_0013F3D0 - 0x80)))->state.control_mode) < 2 ||
                ((unsigned int)((Hero *)(((char *)&D_0013F3D0 - 0x80)))->state.control_mode) == 9) {
                tutorial_trigger_vars->f28 = 0;
            }
        }

        if (is_point_inside_clip_volume(((char *)&D_0013F3D0), tutorial_trigger_vars->f2C) != 0 &&
            D_00141910 == 0 &&
            (((unsigned int)((Hero *)(((char *)&D_0013F3D0 - 0x80)))->state.control_mode) < 2 ||
             ((unsigned int)((Hero *)(((char *)&D_0013F3D0 - 0x80)))->state.control_mode) == 9) &&
            tutorial_trigger_vars->f24 >= 3 && D_L04_00179590[0] == 0 && D_L04_00179590[9] == -1) {
            Help *help = (Help *)(((char *)&D_00141968));
            if (help[0x49].count == 0) {
                help_request(0xFA5, 0x49);
            }
        }
        break;
    }
}
