/* Level 13 moby class help_hints (0x22e): while its forcefield tower (class 0xAA) stands, plays the Gemlik help message in its clip volume (or counts the usage stats once the menu items are on), and switches the hero's vent updraft by a second clip volume. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/overlay/shared/ui/help/help_request.h"

extern int D_0015EEA4;

extern int help_request_state[] __asm__("D_L13_00179710");

extern int scale_game_frames(int);

extern unsigned char menu_item_enabled_115[] __asm__("D_0013D407");

extern char D_00141968[];

extern char statistics_level_flags[] __asm__("D_00141CE4");

void update_help_hints(struct Moby *moby) __asm__("FUN_L13_002f3778");

void update_help_hints(struct Moby *moby) {
    char          *data;
    unsigned char *menu_item_enable_base;
    moby->update_dist = 0xFF;
    data = (char *)moby->pvars;
    if (help_request_state[0] == 0 && help_request_state[9] == -1) {
        unsigned int hero_control_mode = *(unsigned int *)(&hero.state.control_mode);
        if ((hero_control_mode < 2 || hero_control_mode == 9) ? D_L13_0015F5C4 == 0 : 0) {
            int   linked_moby_index = *(int *)(data + 4);
            char *linked_moby;
            if (linked_moby_index != -1) {
                linked_moby = moby_base + (linked_moby_index << 8);
                if (linked_moby != 0 && MOBY(linked_moby)->oclass == 0xAA) {
                    int linked_moby_state = (unsigned char)linked_moby[0x20];
                    if (linked_moby_state == 0xFE)
                        goto join;
                    if (linked_moby_state != 0xFD)
                        goto direct;
                }
            }
        join:
            menu_item_enable_base = menu_item_enabled_115 + 0xB9;
            if (menu_item_enable_base[0xB] != 0 || menu_item_enable_base[0xD] != 0) {
                unsigned char *statistics_base = ((char *)&D_00141968);
                unsigned int   saturating_count = *(unsigned short *)(statistics_base + 0x378);
                if (saturating_count <= 0xFFFE) {
                    *(unsigned short *)(statistics_base + 0x378) = saturating_count + 1;
                }
                if (scale_game_frames(D_0015EEA4) / 600 >
                    *(unsigned short *)(statistics_base + 0x37A)) {
                    *(unsigned short *)(statistics_base + 0x37A) =
                        scale_game_frames(D_0015EEA4) / 600;
                }
                *(unsigned int *)(statistics_base + 0x37C) =
                    *(unsigned int *)(statistics_base + 0x37C) | (1 << current_level_index) |
                    0x80000000;
            } else {
            direct:
                if (is_point_inside_clip_volume(&hero.motion.pos, *(int *)data) != 0) {
                    if (*(int *)(((char *)&statistics_level_flags)) >= 0) {
                        help_request(0x32C8, 0x6F);
                    }
                }
            }
        }
    }
    if (is_point_inside_clip_volume(&hero.motion.pos, *(int *)(data + 8)) != 0) {
        hero.vent_updraft = 0;
    } else {
        hero.vent_updraft = 1;
    }
}
