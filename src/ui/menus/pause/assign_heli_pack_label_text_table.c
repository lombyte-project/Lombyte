/* Ported from rac1-decomp (src/game/pause.c, func_0021DA98). */
#include "sda.h"
#include "types.h"

extern unsigned char D_0013D4C2 NOT_SDA;
extern s32                      heli_pack_label_text_ids[] __asm__("D_001D06D0");
extern s32                      no_heli_pack_label_text_ids[] __asm__("D_001D0708");

int assign_heli_pack_label_text_table(void *destination) __asm__("FUN_0021ca98");

int assign_heli_pack_label_text_table(void *destination) {
    *(s32 **)((char *)destination + 0x34) =
        (D_0013D4C2 != 0) ? heli_pack_label_text_ids : no_heli_pack_label_text_ids;
    return 0;
}

extern __typeof__(assign_heli_pack_label_text_table) func_0021CA98
    __attribute__((alias("FUN_0021ca98")));
