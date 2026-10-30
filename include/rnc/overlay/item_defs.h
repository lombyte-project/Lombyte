#ifndef LOMBYTE_RNC_OVERLAY_ITEM_DEFS_H
#define LOMBYTE_RNC_OVERLAY_ITEM_DEFS_H

#include "types.h"

/*
 * 0x4C-byte item definition, one per item id, in the level program's
 * item_defs table (the other level overlays keep their own copy; see
 * HeroAnimRow in hero_tables.h).
 */
typedef struct ItemDef {
    u8  pad_0[0x8];
    s32 slot;            /* 0x08: hero item slot the item goes into; 0 counts for rewards */
    s32 joint;           /* 0x0C: hero.joint_matrices index the item moby follows */
    s32 cls;             /* 0x10: moby class of the item */
    s32 cls2;            /* 0x14: moby class of its second moby */
    s32 slot_anim_count; /* 0x18: copied to hero.item_slot_anim_count: 0 none, 1 slot 0, 2 both
                                  * slots; read as a word and stored as a byte */
    u8  pad_1C[0x4];
    s32 unk20;                   /* 0x20: set to 0x20 when the item is selected */
    s32 hero_animation;          /* 0x24: animation the hero plays while holding the item */
    s32 animation;               /* 0x28: animation of the item's slot moby */
    s32 animation_mode_12;       /* 0x2C: used instead while state.control_mode is 12 */
    s32 unk30;                   /* 0x30: copied to hero.unk20AA */
    s16 pickup_message_multiple; /* 0x34: dialogue message when more than one ammo is picked up */
    s16 pickup_message_single;   /* 0x36: dialogue message when one ammo is picked up */
    u16 unk38;                   /* 0x38 */
    s16 ammo_pickup_cls;         /* 0x3A: moby class spawned as the item's ammo pickup; <= 0 none */
    u8  pad_3C[0x2];
    union {
        s16 value;
        u8  low_byte;
    } variant_limit; /* 0x3E: highest item_text_variant of the item */
    u8  pad_40[0x6];
    s16 name_text;         /* 0x46: help message id of the item's name */
    s16 name_text_variant; /* 0x48: help message id used while item_text_variant is set */
    u8  pad_4A[0x2];
} ItemDef;

extern ItemDef item_defs[] __asm__("D_L00_00179AC0");

#endif
