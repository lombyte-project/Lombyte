#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/math/matrices/copy_matrix3x4/FUN_001fa2b8.s", FUN_001fa2b8);
#else
/* No C body on purpose: this unit is intentional low-level assembly
   (config/us/unit_categories.json). It copies three quadwords
   (lq $1/$2/$3 then sq) and the first goes through $1 ($at), which the
   compiler's MIPS port never allocates; part of the hand-written
   noreorder fastfunc object. */
#endif /* NON_MATCHING */
