/* Draws the level's water grids from its pool tile table. */

#include "types.h"
#include "rnc/overlay/shared/rendering/render_water_grids.h"

extern char pool_tiles[] __asm__("D_L01_001E34C0");

/* Register this level's vendor data set. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002F7700.c: func_L01_002FE498), where it is exact; names translated to the US level program. */
void render_level_water_grids(void) __asm__("FUN_L01_002fd0c0");

void render_level_water_grids(void) {
    render_water_grids(pool_tiles, 0x15);
}
