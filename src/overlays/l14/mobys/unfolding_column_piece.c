/* Level 14 moby class unfolding_column_piece (0x575): one of the six pieces an unfolding_column (0x573) unfolds and swings; sets up its pvars, and when the column releases it (state 2) marks them and moves to state 3. */
#include "types.h"
#include "rnc/mobys/moby.h"

void update_unfolding_column_piece(void *updated_moby) __asm__("FUN_L14_003061d8");

void update_unfolding_column_piece(void *updated_moby) {
    enum {
        MOBY_PVARS_INITIALIZE = 0
    };
    struct Moby *moby = updated_moby;
    char        *moby_pvars;
    switch ((unsigned char)((char)moby->state)) {
    case MOBY_PVARS_INITIALIZE:
        moby_pvars = ((char *)moby->pvars);
        moby_pvars[0x28] = 4;
        *(int *)(moby_pvars + 0x20) = 0;
        *(short *)(moby_pvars + 0x24) = 0;
        *(short *)(moby_pvars + 0x3E) = 0;
        moby->state = 1;
        break;
    case 2:
        *(short *)(((char *)moby->pvars) + 0x3E) = 5;
        moby->state = 3;
        break;
    }
}
