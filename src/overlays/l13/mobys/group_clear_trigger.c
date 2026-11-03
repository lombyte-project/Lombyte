/* Level 13 moby class group_clear_trigger (0x4f7): once every moby of its (up to eight) moby lists is gone and the hero is in its clip volume, sets its configured state and command. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/overlay/shared/weapons/suckcannon.h"
#include "rnc/overlay/shared/world/level_labels.h"

extern char D_0013E533[];

extern short *D_L13_001ABB40[];

int count_live_mobys_in_list(int moby_list_index,
                             int excluded_moby_state) __asm__("FUN_L13_0030ae68");

int count_live_mobys_in_list(int moby_list_index, int excluded_moby_state) {
    short *moby_id_list = D_L13_001ABB40[moby_list_index];
    short *moby_id_cursor;
    int    eligible_moby_count = 0;
    if (moby_id_list == 0)
        return 0;
    moby_id_cursor = moby_id_list;
    do {
        int   moby_index = *moby_id_cursor & 0x7FFF;
        char *moby = (char *)(moby_index << 8) + (int)moby_base;
        if (moby[0x20] >= 0) {
            char *moby_callback_record = get_moby_suck_cannon_record(moby);
            if (moby_callback_record == 0 || *(short *)(moby_callback_record + 0x68) < 4) {
                if (excluded_moby_state == -1 ||
                    *(unsigned char *)((char *)(moby_index << 8) + (int)moby_base + 0x20) !=
                        excluded_moby_state) {
                    eligible_moby_count++;
                }
            }
        }
    } while (*moby_id_cursor++ >= 0);
    return eligible_moby_count;
}

void group_clear_trigger_update(struct Moby *moby) __asm__("FUN_L13_0030af50");

typedef struct GroupClearTriggerVars {
    u8  pad_0[0x20];
    s32 clear_state;
    s32 clear_command;
    s32 clip_volume_28;
} GroupClearTriggerVars;

/* Moby class 1271 */
void group_clear_trigger_update(struct Moby *moby) {
    GroupClearTriggerVars *moby_vars = (GroupClearTriggerVars *)moby->pvars;
    int                   *moby_list_index_cursor = (int *)moby_vars;
    int                    i;
    char                  *clip_volume_subject_base;
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
    if (moby_vars->clip_volume_28 != -1 &&
        is_point_inside_clip_volume(clip_volume_subject_base + 0x80, moby_vars->clip_volume_28) ==
            0)
        return;
    if (moby_vars->clear_state != -1)
        moby->state = ((char *)moby_vars)[0x20];
    if (moby_vars->clear_command == -1)
        return;
    moby->command = ((char *)moby_vars)[0x24];
}
