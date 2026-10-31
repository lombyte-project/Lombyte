/* Level 13 moby path follower class. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/level_labels.h"
#include "qcopy.h"

extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");

extern float fast_vec_dist(void *, void *);

extern char *D_L13_001B07B0[];

extern int FUN_L00_0025df68(void *, void *, float *, int *, float *, int, float, float, float);

extern int compute_advanced_moby_path_position(void *, void *, void *, int, int, float) __asm__("FUN_L09_00294ae0");

void moby_path_follower_update(struct Moby *moby) __asm__("FUN_L13_0030bb70");

/* Moby class 1403 */
void moby_path_follower_update(struct Moby *moby) {
    int  *moby_vars = (int *)moby->pvars;
    char *path;
    char *pth;
    char *target_moby;
    float nearest_path_position[4];
    int   nearest_segment_index;
    float nearest_segment_progress;
    int   i;
    if (moby->state == 0) {
        moby->update_dist = 0xFF;
        if (moby_vars[1] != -1) {
            path = D_L13_001B07B0[moby_vars[1]];
            for (i = 0; i < *(int *)path; i++) {
                *(float *)(path + (i << 4) + 0x1C) = fast_vec_dist(
                    path + (i * 16 + 0x10), path + ((i + 1) % *(int *)path * 16 + 0x10));
            }
        }
        moby_vars[2] = 0;
        moby_vars[3] = 0;
        moby->state = 1;
        moby_vars[4] = 0;
    }
    if (moby_vars[0] == -1)
        return;
    target_moby = (char *)(moby_base + (moby_vars[0] << 8));
    if (target_moby == 0 || ((unsigned char *)target_moby)[0x20] == 0xFE ||
        ((unsigned char *)target_moby)[0x20] == 0xFD) {
        mark_moby_for_removal(moby);
        return;
    }
    if (((unsigned char *)target_moby)[0x20] >= 7) {
        moby_vars[4]++;
        if (moby_vars[1] == -1)
            return;
        pth = D_L13_001B07B0[moby_vars[1]];
        if (moby->state == 1) {
            FUN_L00_0025df68(pth,
                             target_moby + 0x10,
                             nearest_path_position,
                             &nearest_segment_index,
                             &nearest_segment_progress,
                             0,
                             100.0f,
                             1.0f,
                             0.0f);
            if (nearest_segment_index > moby_vars[2] ||
                nearest_segment_progress > ((float *)moby_vars)[3] || moby_vars[4] < 0x1E) {
                moby_vars[2] = nearest_segment_index;
                ((float *)moby_vars)[3] = nearest_segment_progress;
                qcopy(&moby->pos, nearest_path_position);
            } else {
                moby->state = 2;
            }
        }
        if (moby->state == 2) {
            if (moby_vars[2] < *(int *)pth - 1) {
                int k = compute_advanced_moby_path_position(
                    moby, pth, &moby->pos, moby_vars[2], 1, frame_scale * 0.25f);
                moby_vars[2] = k;
                ((float *)moby_vars)[3] = fast_vec_dist(&moby->pos, pth + (k * 16 + 0x10));
            } else {
                moby->state = 3;
            }
        }
    } else {
        qcopy(&moby->pos, target_moby + 0x10);
    }
}
