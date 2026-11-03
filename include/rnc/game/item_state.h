#ifndef LOMBYTE_RNC_GAME_ITEM_STATE_H
#define LOMBYTE_RNC_GAME_ITEM_STATE_H

#include "types.h"
#include "sda.h"
#include "rnc/game/level_progress.h"

/* Per-item flag tables of the save state, indexed by item id. */
extern u8  alternate_item_available[128] __asm__("D_0013D388");
extern u8  item_unlocked[32] __asm__("D_0013D408");
extern s32 weapon_ammo_counts[37] __asm__("D_0013D428");
/* item_available and items_acquired hold 37 items: mc_gamedata saves 0x25 bytes of each.
   Single items read by name: 2 the heli-pack (hero jump code), 6 the O2 mask (no air
   loss when swimming, Fred gives it on Pokitaru), 33 shifts map ids by 0x100
   (pick_next_map), 35 the vendor discount. */
extern u8 item_available[37] __asm__("D_0013D4C0");
#define ITEM_HELI_PACK       2
#define ITEM_O2_MASK         6
#define ITEM_MAP_SHIFT       33
#define ITEM_VENDOR_DISCOUNT 35
extern u8 item_text_variant[40] __asm__("D_0013E520");

extern u8 D_0015EDD0[] __attribute__((sda));
extern u8 items_acquired[37] __asm__("D_0013D4E8");
/* Per-weapon totals (37 items, mc_gamedata blocks 0x3E8-0x3EA of 0x94 bytes). */
extern s32 ammo_bought_stats[37] __asm__("D_0013DD70");
extern s32 weapon_ammo_pickup_totals[37] __asm__("D_0013DE08");
extern s32 weapon_ammo_used_counts[37] __asm__("D_0013DEA0");
extern s32 pause_iop_sprite_lists[8] __asm__("D_00141EA0");
extern s32 D_00141660;

#endif /* LOMBYTE_RNC_GAME_ITEM_STATE_H */
