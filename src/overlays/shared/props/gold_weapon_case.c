/* Shared moby class gold_weapon_case (and the gold weapons in it). */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/game/item_state.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/save/memcard_save_data.h"
#include "qcopy.h"

/* Bought-item price entry (0x18 bytes). */
typedef struct {
    u8  pad_00[0x14];
    u16 bolts; /* 0x14 */
    u8  pad_16[2];
} ItemPrice;

/* pvars of the moby run by gold_weapon_case_update: a scene trigger
   (update_npc_dialogue) followed by the item it sells. */
typedef struct {
    u8  pad_00[4];
    s16 result; /* 0x04: 2 once the scene finished with a sale */
    u8  pad_06[6];
    f32 radius; /* 0x0C */
    u8  pad_10[0x30];
    s32 slot; /* 0x40: -1 none, else index into D_L01_001DEB10 */
} SaleVars;

extern s32  initialize_npc_dialogue(struct Moby *, SaleVars *) __asm__("FUN_L00_002668a0");
extern s32  FUN_L00_00266448_2e1ac0(struct Moby *, SaleVars *) __asm__("FUN_L00_00266448");
extern void initialize_vendor_screen(struct Moby *, s32, ItemPrice *) __asm__("FUN_L01_002aea70");

extern s32       gold_weapon_case_item_ids_by_slot[] __asm__("D_L01_001DEB10");
extern ItemPrice l01_gold_weapon_case_item_prices[] __asm__("D_L01_001C4530");
extern s32       alternate_animation_sequence_gp __asm__("D_0015EE20") __attribute__((sda));

void gold_weapon_case_update(struct Moby *m) __asm__("FUN_L01_002e1ac0");

/* Moby classes 304, 1456-1465 */
void gold_weapon_case_update(struct Moby *moby) {
    enum {
        GOLD_WEAPON_CASE_INITIALIZE = 0,
        GOLD_WEAPON_CASE_PROCESS_SALE = 1,
        GOLD_WEAPON_CASE_REMOVE_WHEN_PURCHASED = 2,
        GOLD_WEAPON_CASE_IDLE = 3,
        GOLD_WEAPON_CASE_WAIT_FOR_CUTSCENE_END = 4
    };
    SaleVars  *sale_vars = (SaleVars *)moby->pvars;
    s32        purchased_item_id;
    ItemPrice *price;

    if (sale_vars != 0 && sale_vars->slot != -1 && ((sale_vars->slot ^ 1) & 1) &&
        alternate_animation_sequence_gp == 0) {
        mark_moby_for_removal(moby);
        return;
    }
    switch (moby->state) {
    case GOLD_WEAPON_CASE_INITIALIZE:
        if (moby->oclass != 0x130) {
            moby->state = 2;
        } else if (item_text_variant[gold_weapon_case_item_ids_by_slot[sale_vars->slot]] != 0 ||
                   initialize_npc_dialogue(moby, sale_vars) == -1) {
            moby->state = 3;
        } else {
            moby->state = 1;
        }
        break;
    case GOLD_WEAPON_CASE_PROCESS_SALE:
        sale_vars->radius = 1.9f;
        FUN_L00_00266448_2e1ac0(moby, sale_vars);
        if (sale_vars->result == 2) {
            item_text_variant[gold_weapon_case_item_ids_by_slot[sale_vars->slot]] = 1;
            moby->state = 4;
            purchased_item_id = gold_weapon_case_item_ids_by_slot[sale_vars->slot];
            price = &l01_gold_weapon_case_item_prices[purchased_item_id];
            current_bolt_count -= price->bolts;
            initialize_vendor_screen(moby, purchased_item_id, price);
        }
        break;
    case GOLD_WEAPON_CASE_IDLE: /* sold out or done: idle */
        break;
    case GOLD_WEAPON_CASE_WAIT_FOR_CUTSCENE_END:
        if (cutscene_mode == 0) {
            memcard_save_data(0, -1);
            moby->state = 3;
        }
        break;
    case GOLD_WEAPON_CASE_REMOVE_WHEN_PURCHASED:
        if (item_text_variant[gold_weapon_case_item_ids_by_slot[sale_vars->slot]] != 0) {
            mark_moby_for_removal(moby);
        }
        break;
    }
}
