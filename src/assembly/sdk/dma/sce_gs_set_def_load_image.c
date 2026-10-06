#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
/* Exact SDK/library unit sceGsSetDefLoadImage; symbolic expected assembly retained pending source recovery. */
INCLUDE_ASM(
    "config/us/expected/asm/assembly/sdk/dma/sce_gs_set_def_load_image/sceGsSetDefLoadImage.s",
    sceGsSetDefLoadImage);
#else
/* No C body on purpose: this unit is intentional low-level assembly
   (config/us/unit_categories.json), so it has no C goal and no public
   fuzzy score. The assembly oracle above is the whole unit. */
#endif /* NON_MATCHING */
