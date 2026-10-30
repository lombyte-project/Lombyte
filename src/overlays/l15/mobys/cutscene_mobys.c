/* Level 15 cutscene mobys, run from the level main loop: runs FUN_L00_00257024 (not understood yet) on every cutscene moby during scenes 3 and 4. */
#include "types.h"
#include "rnc/globals.h"
#include "asm.h"

/* Runs FUN_L00_00257024 on each cutscene moby while scene 3 or 4 plays. */
/* Ported from rac1-decomp (src/overlays/l15/initonce_0022E478.c: func_L15_0022E478), where it is exact; names translated to the US level program. */

/* Cutscene state (as CutsceneFx in l01/mobys/cutscene_effects.c). */
typedef struct {
    char  pad0[0x30];
    int   scene;
    char  pad34[0x10];
    short moby_count;
    char  pad46[0x132];
    void *mobys[1]; /* 0x178 */
} Level15CutsceneState;

extern void FUN_L00_00257024(void *);

extern Level15CutsceneState cutscene_state __asm__("D_L15_0016CE60");
void                        update_level15_cutscene_mobys(void) __asm__("FUN_L15_0022db50");

void update_level15_cutscene_mobys(void) {
    int i;
    if (current_level_index == 15 && (unsigned)(cutscene_state.scene - 3) < 2) {
        for (i = 0; i < cutscene_state.moby_count; i++) {
            FUN_L00_00257024(cutscene_state.mobys[i]);
        }
    }
}
