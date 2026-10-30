/* Level 05 water: render_level_water_grids, which renders the level's water grids. */
#include "types.h"
#include "asm.h"

#include "qcopy.h"
#include "sda.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/overlay/water_grid.h"
#include "rnc/overlay/shared/rendering/render_water_grids.h"

void render_level_water_grids(void) __asm__("FUN_L05_0030d678");

void render_level_water_grids(void) {
    render_water_grids(l05_water_grids, 0x30);
}
