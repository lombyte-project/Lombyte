/* Level 13 moby class group_clear_trigger (0x4f7): once every moby of its (up to eight) moby lists is gone and the hero is in its clip volume, sets its configured state and command. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/overlay/shared/weapons/suckcannon.h"
#include "rnc/overlay/level_labels.h"

extern char D_0013E533[];

extern short *D_L13_001ABB40[];

int count_live_mobys_in_list(int moby_list_index,
                             int excluded_moby_state) __asm__("FUN_L13_0030ae68");

int count_live_mobys_in_list(int moby_list_index, int excluded_moby_state) {
    short *moby_id_list = D_L13_001ABB40[moby_list_index];
    short *moby_id_cursor;
    int    count = 0;
    if (moby_id_list == 0)
        return 0;
    moby_id_cursor = moby_id_list;
    do {
        int   id = *moby_id_cursor & 0x7FFF;
        char *moby = (char *)(id << 8) + (int)moby_base;
        if (moby[0x20] >= 0) {
            char *moby_callback_record = get_moby_suck_cannon_record(moby);
            if (moby_callback_record == 0 || *(short *)(moby_callback_record + 0x68) < 4) {
                if (excluded_moby_state == -1 ||
                    *(unsigned char *)((char *)(id << 8) + (int)moby_base + 0x20) !=
                        excluded_moby_state) {
                    count++;
                }
            }
        }
    } while (*moby_id_cursor++ >= 0);
    return count;
}

void group_clear_trigger_update(struct Moby *moby) __asm__("FUN_L13_0030af50");

/* Moby class 1271 */
void group_clear_trigger_update(struct Moby *moby) {
    char *moby_vars = (char *)moby->pvars;
    int  *moby_list_index_cursor = (int *)moby_vars;
    int   i;
    char *clip_volume_subject_base;
    for (i = 0; i < 8; i++) {
        if (*moby_list_index_cursor != -1) {
            if (count_live_mobys_in_list(*moby_list_index_cursor, -1) != 0)
                return;
            moby_list_index_cursor++;
        }
    }
    clip_volume_subject_base = D_0013E533 + 0xE1D;
    if (*(int *)(clip_volume_subject_base + 0x15F0) != 0 &&
        *(int *)(clip_volume_subject_base + 0x2084) == 0x32)
        return;
    if (*(int *)(moby_vars + 0x28) != -1 &&
        is_point_inside_clip_volume(clip_volume_subject_base + 0x80, *(int *)(moby_vars + 0x28)) ==
            0)
        return;
    if (*(int *)(moby_vars + 0x20) != -1)
        moby->state = moby_vars[0x20];
    if (*(int *)(moby_vars + 0x24) == -1)
        return;
    moby->command = moby_vars[0x24];
}
