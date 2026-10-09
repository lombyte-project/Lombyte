#ifndef LOMBYTE_RNC_GAMEPLAY_STATE_ITEM_STATE_H
#define LOMBYTE_RNC_GAMEPLAY_STATE_ITEM_STATE_H

#include "types.h"
#include "sda.h"

/* Per-item flag tables of the save state, indexed by item id. */
extern u8 alternate_item_available[128] __asm__("D_0013D388");
extern u8 item_unlocked[32] __asm__("D_0013D408");
extern s32 weapon_ammo_counts[37] __asm__("D_0013D428");
extern u8 item_available[35] __asm__("D_0013D4C0");
extern u8 discount_purchase_pricing[5] __asm__("D_0013D4E3") NOT_SDA;
extern u8 item_text_variant[40] __asm__("D_0013E520");

#endif /* LOMBYTE_RNC_GAMEPLAY_STATE_ITEM_STATE_H */
