/* 18 hand-written MMI/COP2 motion-compensation kernels (qfsrv, pextlb/ub,
 * paddh, psrlh, psrah, pminh, pmaxh, pnor, por, pcpyld, pcgth, ppacb,
 * mtsab, lq/sq) plus the D_00127220 word. Not expressible in generic
 * EE-GCC C.
 */

#include "types.h"
#include "asm.h"

INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/domc_mmi/func_00126488.s", func_00126488);
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/domc_mmi/func_00126500.s", func_00126500);
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/domc_mmi/func_00126598.s", func_00126598);
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/domc_mmi/func_00126650.s", func_00126650);
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/domc_mmi/func_00126720.s", func_00126720);
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/domc_mmi/func_001267D0.s", func_001267D0);
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/domc_mmi/func_00126888.s", func_00126888);
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/domc_mmi/func_00126980.s", func_00126980);
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/domc_mmi/func_00126A80.s", func_00126A80);
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/domc_mmi/func_00126B20.s", func_00126B20);
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/domc_mmi/func_00126BD0.s", func_00126BD0);
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/domc_mmi/func_00126CB0.s", func_00126CB0);
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/domc_mmi/func_00126D98.s", func_00126D98);
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/domc_mmi/func_00126E70.s", func_00126E70);
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/domc_mmi/func_00126F40.s", func_00126F40);
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/domc_mmi/func_00127060.s", func_00127060);
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/domc_mmi/func_00127178.s", func_00127178);
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/domc_mmi/func_001271D8.s", func_001271D8);
