#ifndef LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_TEST_PATH_SEGMENT_STRICT_CROSSING_H
#define LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_TEST_PATH_SEGMENT_STRICT_CROSSING_H

#include "types.h"

void update_joint_animation(void *m, unsigned char *p, int c, float fa, float fb) __asm__("FUN_L00_002628d8");
void initialize_moby_transform_history(int   source_moby,
                                       char *history_state) __asm__("FUN_L00_00262500");

int  path_8808(int, void *, void *, void *) __asm__("FUN_L00_00261b48");
void path_sample_at_distance(int, float *, float *, void *, float, float) __asm__("FUN_L00_00262360");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_TEST_PATH_SEGMENT_STRICT_CROSSING_H */
