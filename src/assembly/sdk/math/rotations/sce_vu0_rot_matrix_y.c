#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
/* Exact Sony VU0 routine sceVu0RotMatrixY; target-derived body pending SDK source recovery. */
INCLUDE_ASM(
    "config/us/expected/asm/assembly/sdk/math/rotations/sce_vu0_rot_matrix_y/SceVu0RotMatrixY.s",
    SceVu0RotMatrixY);
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/math/rotations/sce_vu0_rot_matrix_y/"
            "SceVu0RotMatrixYBody.s",
            SceVu0RotMatrixYBody);
#else
/* No C body on purpose: this unit is intentional low-level assembly
   (config/us/unit_categories.json), so it has no C goal and no public
   fuzzy score. The assembly oracle above is the whole unit. */
#endif /* NON_MATCHING */
