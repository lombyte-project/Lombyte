#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
/* Exact Sony VU0 routine sceVu0RotMatrixX; target-derived body pending SDK source recovery. */
INCLUDE_ASM(
    "config/us/expected/asm/assembly/sdk/math/rotations/sce_vu0_rot_matrix_x/SceVu0RotMatrixX.s",
    SceVu0RotMatrixX);
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/math/rotations/sce_vu0_rot_matrix_x/"
            "SceVu0RotMatrixXBody.s",
            SceVu0RotMatrixXBody);
#else
/* No C body on purpose: this unit is intentional low-level assembly
   (config/us/unit_categories.json), so it has no C goal and no public
   fuzzy score. The assembly oracle above is the whole unit. */
#endif /* NON_MATCHING */
