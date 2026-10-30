/* Level 10 moby class help_hints (0x540): tracks nearby targets and plays the Orxon help messages. */
#include "types.h"

#include "rnc/overlay/level_globals.h"
#include "rnc/game/item_state.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/overlay/shared/ui/help/help_request.h"

typedef struct Moby  Moby;
extern struct Moby  *D_L10_0015FFD0_macro[] __asm__("D_L10_0015FFD0") MACRO_ADDR;
extern char          D_L10_00179890[];
extern unsigned char D_0013D4CB[];
extern unsigned char D_00141C48[];
/* Level 10 moby update (class 1344): tracks nearby targets and fires the level's event messages. */
/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_002E30F8.c: func_L10_002E9978), where it is exact; names translated to the US level program. */

typedef struct EvData {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
    int f18;
    int f1C;
    int f20;
} EvData;

typedef struct HeroPos {
    char         pad0[0x200C];
    unsigned int f200C;
} HeroPos;

typedef struct Msg {
    int  f0;
    char pad4[0x20];
    int  f24;
} Msg;

typedef struct Save_2E9978 {
    char           pad0[0x358];
    unsigned short f358;
    unsigned short f35A;
    int            f35C;
} Save_2E9978;

typedef struct Flags_2E9978 {
    char          pad0[4];
    unsigned char f4;
    unsigned char f5;
} Flags_2E9978;

extern unsigned char D_0013F3D0_r[] __asm__("D_0013F3D0");
extern unsigned char D_00141B20_r[] __asm__("D_00141B20");
extern unsigned char D_00141B28_r[] __asm__("D_00141B28");
extern unsigned char D_0013D504_r[] __asm__("D_0013D504");
extern unsigned char D_00141968_r[] __asm__("D_00141968");
extern Moby         *D_L10_0015FFD8_c2 __asm__("D_L10_0015FFD8");      /* no foreign declaration */
extern int   D_0015EEA4_gp __asm__("D_0015EEA4") __attribute__((sda)); /* no foreign declaration */
extern Moby *l10_conditional_help_moby_list_head __asm__("D_L10_0015FFE4");
extern int   hero_inside_conditional_help_volume __asm__("D_L10_00161F38") __attribute__((sda));

extern unsigned char l10_help_message_2710_gate[] __asm__("D_00141B10");
extern unsigned char l10_help_message_2711_gate[] __asm__("D_00141B18");
extern unsigned char l10_help_message_2717_gate[] __asm__("D_00141CB8");
void                 update_help_hints(Moby *moby) __asm__("FUN_L10_002e85b8");
/* no foreign declaration */

/* no foreign declaration */

void update_help_hints(Moby *moby) {
    enum {
        HELP_MESSAGE_STATE_INITIALIZE = 0,
        HELP_MESSAGE_STATE_CHECK_CONDITIONS = 1
    };
    EvData *data = (EvData *)((int *)moby->pvars);
    Moby   *other;
    int     any_active;
    int     elapsed;

    if (is_point_inside_clip_volume(D_0013F3D0_r, data->f1C)) {
        hero_inside_conditional_help_volume = 1;
    } else {
        hero_inside_conditional_help_volume = 0;
    }
    switch (moby->state) {
    case HELP_MESSAGE_STATE_INITIALIZE:
        moby->update_dist = 0xFF;
        moby->state = 1;
        break;
    case HELP_MESSAGE_STATE_CHECK_CONDITIONS:
        if (is_point_inside_clip_volume(D_0013F3D0_r, data->f0) != 0 &&
            (((HeroPos *)(D_0013F3D0_r))->f200C < 2 || ((HeroPos *)(D_0013F3D0_r))->f200C == 9) &&
            ((Msg *)D_L10_00179890)->f0 == 0 && ((Msg *)D_L10_00179890)->f24 == -1 &&
            *(unsigned short *)(l10_help_message_2710_gate) == 0) {
            help_request(0x2710, 0x35);
        }
        if (is_point_inside_clip_volume(D_0013F3D0_r, data->f4) != 0 &&
            (((HeroPos *)(D_0013F3D0_r))->f200C < 2 || ((HeroPos *)(D_0013F3D0_r))->f200C == 9) &&
            ((Msg *)D_L10_00179890)->f0 == 0 && ((Msg *)D_L10_00179890)->f24 == -1) {
            if (((Flags_2E9978 *)(alternate_item_available))->f4 == 0) {
                if (*(unsigned short *)(D_00141B20_r) == 0) {
                    help_request(0x2712, 0x37);
                }
            } else if (((Flags_2E9978 *)(alternate_item_available))->f5 == 0) {
                if (*(unsigned short *)(D_00141B28_r) == 0) {
                    help_request(0x2713, 0x38);
                }
            }
        }
        if (D_L10_0015F5C4_vendor == 0 && D_0013D504_r[0] != 0 &&
            ((Msg *)D_L10_00179890)->f0 == 0 && ((Msg *)D_L10_00179890)->f24 == -1 &&
            *(unsigned short *)(l10_help_message_2711_gate) == 0) {
            help_request(0x2711, 0x36);
        }
        if (is_point_inside_clip_volume(D_0013F3D0_r, data->f8) != 0 &&
            (((HeroPos *)(D_0013F3D0_r))->f200C < 2 || ((HeroPos *)(D_0013F3D0_r))->f200C == 9) &&
            D_0013D4CB[0] == 0 && ((Msg *)D_L10_00179890)->f0 == 0 &&
            ((Msg *)D_L10_00179890)->f24 == -1 && *(unsigned short *)(D_00141C48) == 0) {
            help_request(0x2715, 0x5C);
        }
        any_active = 0;
        for (other = l10_conditional_help_moby_list_head; other != 0; other = other->next) {
            if (other->oclass == 0x359 && other->state != 1)
                any_active = 1;
        }
        if (any_active == 0)
            break;
        if (data->f10 != -1 &&
            vector_distance_xy(D_0013F3D0_r, &D_L10_0015FFD0_macro[2][data->f10].pos) < 15.0f &&
            data->f14 != -1 &&
            vector_distance_xy(D_0013F3D0_r, &D_L10_0015FFD0_macro[2][data->f14].pos) < 15.0f &&
            *(unsigned short *)(l10_help_message_2717_gate) == 0) {
            help_request(0x2717, 0x6A);
        }
        if (any_active == 0)
            break;
        if (is_point_inside_clip_volume(D_0013F3D0_r, data->f18) == 0)
            break;
        if (data->f20 == -1)
            break;
        if (D_L10_0015FFD8_c2[data->f20].oclass != 0x3F7)
            break;
        if (D_L10_0015FFD8_c2[data->f20].state == 4)
            break;
        if (((Save_2E9978 *)(D_00141968_r))->f358 != 0) {
            elapsed =
                scale_game_frames(D_0015EEA4_gp) - ((Save_2E9978 *)(D_00141968_r))->f35A * 600;
            if ((int)((float)scale_game_frames(0x12) * 60.0f) < elapsed ||
                ((Save_2E9978 *)(D_00141968_r))->f35A * 600 == 0) {
                help_request(0x2718, 0x6B);
                break;
            }
            if (scale_game_frames(D_0015EEA4_gp) / 600 > ((Save_2E9978 *)(D_00141968_r))->f35A) {
                ((Save_2E9978 *)(D_00141968_r))->f35A = scale_game_frames(D_0015EEA4_gp) / 600;
            }
            break;
        }
        ((Save_2E9978 *)(D_00141968_r))->f358++;
        if (scale_game_frames(D_0015EEA4_gp) / 600 > ((Save_2E9978 *)(D_00141968_r))->f35A) {
            ((Save_2E9978 *)(D_00141968_r))->f35A = scale_game_frames(D_0015EEA4_gp) / 600;
        }
        ((Save_2E9978 *)(D_00141968_r))->f35C =
            ((Save_2E9978 *)(D_00141968_r))->f35C | (1 << current_level_index) | 0x80000000;
        break;
    }
}
