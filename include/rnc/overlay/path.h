#ifndef LOMBYTE_RNC_OVERLAY_PATH_H
#define LOMBYTE_RNC_OVERLAY_PATH_H

/* Path helpers in the shared level code. */

/* FUN_L00_0020cd08: every overlay caller passes six pointers and two ints. */
int find_path_near_point(void *, void *, void *, void *, void *, void *, int, int) __asm__("FUN_L00_0020cd08");

#endif /* LOMBYTE_RNC_OVERLAY_PATH_H */
