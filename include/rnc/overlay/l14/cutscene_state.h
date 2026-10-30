#ifndef LOMBYTE_RNC_OVERLAY_L14_CUTSCENE_STATE_H
#define LOMBYTE_RNC_OVERLAY_L14_CUTSCENE_STATE_H

/* Level 14 cutscene state, read by src/overlays/l14/mobys/cutscene_effects.c and group_clear_trigger.c. */
typedef struct {
    char  pad0[0x30];
    int   scene; /* 0x30 */
    int   frame; /* 0x34 */
    char  pad38[0x140];
    void *scene_mobys[2]; /* 0x178 */
} Level14CutsceneState;

extern Level14CutsceneState D_L14_0016CF60;

#endif
