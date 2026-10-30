/* Level 02 moby class help_hints (0x52c): plays the Aridia help messages when the hero enters their clip volumes. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/audio/voices/allocate_voice_for_bank_entry.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/ui/help/help_request.h"
#include "qcopy.h"
#include "rnc/game/item_state.h"
#include "rnc/game/options.h"

/* UpdateMoby_1324: tutorial hint triggers around the player */
/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002E21F8.c: func_L02_002EFCC8), where it is exact; names translated to the US level program. */

typedef struct {
    int  pad0;
    int  pad4;
    int  active;
    char rest[0x40];
} Entry_L02_0017A140;

typedef struct {
    unsigned char b[0x20];
} Flags_0013D5C8;

extern Entry_L02_0017A140 D_L02_0017A0C0[];

extern unsigned char D_0014C050[][16];

extern int                       D_0015EEA4;
extern unsigned short D_001417A2 NOT_SDA;
extern unsigned short D_00141888 NOT_SDA;
extern unsigned short D_001418E8 NOT_SDA;
extern char                      D_00141968[];
extern s32                       D_0014EE90[];
extern int                       scale_game_frames(int) __asm__("FUN_001f96f8");

void update_help_hints(struct Moby *moby) __asm__("FUN_L02_002ee890");

void update_help_hints(struct Moby *moby) {
    enum {
        AIRBORNE_DIALOGUE_UNARMED = 0,
        AIRBORNE_DIALOGUE_ARMED = 1
    };
    int *data = (int *)moby->pvars;
    moby->update_dist = 0xFF;

    if (is_point_inside_clip_volume(&hero.motion.pos, data[0x17]) != 0 &&
        ((unsigned int)hero.state.control_mode) < 2) {
        char *b = ((char *)&D_00141968);
        if (*(unsigned short *)(b + 0x278) == 0 && D_001418E8 == 0) {
            int count = 0;
            int i;
            for (i = 0; i < 37; i++) {
                if (D_L02_0017A0C0[i].active == 0 && i != 8 && i != 24) {
                    if ((item_available)[i] != 0) {
                        count++;
                    }
                }
            }
            if (count >= 2) {
                help_request(0x4E25, 0x4F);
            }
        }
    }

    switch (data[0x15]) {
    case AIRBORNE_DIALOGUE_UNARMED:
        if (is_point_inside_clip_volume(&hero.motion.pos, data[0x12]) != 0) {
            if (hero.state.current == 0x2C) {
                data[0x15] = 1;
            }
        }
        break;
    case AIRBORNE_DIALOGUE_ARMED: {
        struct Hero *h = &hero;
        if (h->grounded_frames != 0) {
            data[0x15] = 0;
        } else if (is_point_inside_clip_volume(&h->motion.pos, data[0x13]) != 0) {
            unsigned char *dialogue_latch_bytes = item_unlocked;
            if (dialogue_latch_bytes[1] == 0) {
                dialogue_latch_bytes[1] = 1;
                allocate_voice_for_bank_entry(1, 0, 0);
                queue_dialogue_message(0x53D6, -1);
            }
        }
        break;
    }
    }

    {
        char *b = ((char *)&D_00141968);
        if (*(unsigned short *)(b + 0x60) == 0) {
            if (is_point_inside_clip_volume(&hero.motion.pos, data[4]) != 0) {
                int lvl = data[0];
                if (D_0014C050[current_level_index][lvl] != 0xFF) {
                    unsigned char *f = item_available;
                    if (f[0x10] != 0 && ((int *)(((char *)&D_0014EE90)))[lvl] >= 6) {
                        int t = scale_game_frames(D_0015EEA4) - D_001417A2 * 600;
                        if (scale_game_frames(0x4650) < t) {
                            help_request(0x7D2, 0xC);
                        }
                    }
                }
            }
        }
    }

    if (is_point_inside_clip_volume(&hero.motion.pos, data[0x10]) != 0) {
        data[0x16] = 1;
    }
    if (data[0x16] != 0) {
        if (D_00141888 < 3) {
            char *b = ((char *)&D_00141968);
            if (!(*(unsigned int *)(b + 0x264) & (1 << current_level_index))) {
                help_request(0x4E22, 0x4C);
            }
        }
        if (*(u8 *)&invert52 != 0 && *(u8 *)&opt_cam_invert != 0) {
            char *b = ((char *)&D_00141968);
            if (!(*(unsigned int *)(b + 0x3B4) & (1 << current_level_index))) {
                help_request(0x4E2C, 0x76);
            }
        } else {
            char *b = ((char *)&D_00141968);
            *(unsigned short *)(b + 0x3B0) = 0xFFFF;
        }
    }

    {
        char *b = ((char *)&D_00141968);
        if (*(unsigned short *)(b + 0x68) == 0) {
            if (is_point_inside_clip_volume(&hero.motion.pos, data[4]) != 0) {
                int lvl = data[0];
                if (D_0014C050[current_level_index][lvl] != 0xFF) {
                    unsigned char *f = item_available;
                    if (f[0x10] == 0 && (float)((int *)(((char *)&D_0014EE90)))[lvl] >= 3.0f) {
                        help_request(0x7D3, 0xD);
                    }
                }
            }
        }
    }

    {
        char *b = ((char *)&D_00141968);
        if (*(unsigned short *)(b + 0x80) == 0) {
            char *g = (char *)&hero.motion.pos;
            char *h;
            if (is_point_inside_clip_volume(g, data[6]) != 0) {
                *(unsigned short *)(b + 0x80) = 0xFFFF;
            }
            h = g - 0x80;
            if (*(int *)(h + 0x2084) != 0x2C && *(int *)(h + 0x2090) == 0x2C &&
                *(int *)(h + 0x198) == 1) {
                data[0xC]++;
            }
            if (data[0xC] >= 2) {
                if (is_point_inside_clip_volume(&hero.motion.pos, data[7]) != 0) {
                    help_request(0x7D6, 0x10);
                }
            }
        }
    }

    if (((Flags_0013D5C8 *)(item_available))->b[0xC] == 0) {
        if (is_point_inside_clip_volume(&hero.motion.pos, data[5]) != 0) {
            char *b = ((char *)&D_00141968);
            int   t = scale_game_frames(D_0015EEA4) - *(unsigned short *)(b + 0x8A) * 600;
            if ((int)(scale_game_frames(0x12) * 60.0f) < t ||
                *(unsigned short *)(b + 0x8A) * 600 == 0) {
                help_request(0x7D7, 0x11);
            } else {
                int n = scale_game_frames(D_0015EEA4) / 600;
                if (*(unsigned short *)(b + 0x8A) < n) {
                    *(unsigned short *)(b + 0x8A) = scale_game_frames(D_0015EEA4) / 600;
                }
            }
        }
    }
    if (((Flags_0013D5C8 *)(item_available))->b[0xC] != 0 &&
        is_point_inside_clip_volume(&hero.motion.pos, data[5]) != 0) {
        char *b = ((char *)&D_00141968);
        if (*(unsigned short *)(b + 0x90) != 0) {
            int t = scale_game_frames(D_0015EEA4) - *(unsigned short *)(b + 0x92) * 600;
            if ((int)(scale_game_frames(0x12) * 60.0f) < t ||
                *(unsigned short *)(b + 0x92) * 600 == 0) {
                help_request(0x7D8, 0x12);
            } else {
                int n = scale_game_frames(D_0015EEA4) / 600;
                if (*(unsigned short *)(b + 0x92) < n) {
                    *(unsigned short *)(b + 0x92) = scale_game_frames(D_0015EEA4) / 600;
                }
            }
        } else {
            int n;
            (*(unsigned short *)(b + 0x90))++;
            n = scale_game_frames(D_0015EEA4) / 600;
            if (*(unsigned short *)(b + 0x92) < n) {
                *(unsigned short *)(b + 0x92) = scale_game_frames(D_0015EEA4) / 600;
            }
            *(int *)(b + 0x94) = *(int *)(b + 0x94) | (1 << current_level_index) | 0x80000000;
        }
    } else if (is_point_inside_clip_volume(&hero.motion.pos, data[7]) != 0) {
        char *b = ((char *)&D_00141968);
        *(unsigned short *)(b + 0x90) = 0xFFFF;
    }

    {
        char          *b = ((char *)&D_00141968);
        unsigned short v = *(unsigned short *)(b + 0x230);
        if (v == 0 || *(unsigned short *)(b + 0x238) == 0) {
            struct Hero *h = &hero;
            if (h->state.current == 0x72) {
                char *ground_moby = ((char *)h->ground_moby);
                if (ground_moby != 0 && MOBY(ground_moby)->oclass == 0x267) {
                    if (v == 0) {
                        help_request(0x7DB, 0x46);
                    } else if (*(unsigned short *)(b + 0x238) == 0) {
                        data[0x14]++;
                        if (scale_game_frames(600) < data[0x14]) {
                            help_request(0x7DC, 0x47);
                        }
                    }
                }
            }
        }
    }

    {
        unsigned char *g = alternate_item_available;
        if (g[0x14] == 0) {
            if (is_point_inside_clip_volume(&hero.motion.pos, data[0x11]) != 0) {
                g[0x14] = 1;
            }
        }
    }
    qcopy(data + 8, &hero.motion.pos);
}
