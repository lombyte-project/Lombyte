#ifndef LOMBYTE_RNC_OVERLAY_VENDOR_MENU_H
#define LOMBYTE_RNC_OVERLAY_VENDOR_MENU_H

#include "types.h"
#include "rnc/math/vector.h"

struct VendorMoby;

/* One row of the vendor's item list, built by build_vendor_menu_rows. */
typedef struct {
    s32 item;    /* 0x00: item id */
    s32 is_ammo; /* 0x04: 1 = the row sells ammo for the item */
    s32 state;   /* 0x08 */
    s32 unkC;
    s32 sound; /* 0x10 */
} VendorMenuRow;

/* The vendor menu state at D_L00_001CA4C0. */
typedef struct {
    s32                state;  /* 0x00: menu state: 0 opening, 1 open, 2 closing, 3 exit cutscene */
    s32                frames; /* 0x04: frames in the current state */
    s32                unk08;
    s32                unk0C;
    s32                unk10;
    struct VendorMoby *unk14;
    s32                unk18;       /* 0x18: decompressed resource data */
    struct VendorMoby *vendor_moby; /* 0x1C */
    struct VendorMoby *menu_mobys;  /* 0x20: the menu's mobys [0..5] */
    struct VendorMoby *unk24;
    struct VendorMoby *unk28;
    s32                unk2C;
    s32                unk30;
    s32                unk34;
    s32                timer; /* 0x38 */
    s32                unk3C;
    s32                mode; /* 0x40: nonzero: rows and prices are for ammo */
    s32                unk44;
    s32                purchased_item; /* 0x48 */
    s32                scroll;         /* 0x4C: list scroll, in steps of 0x38 */
    s32                item_text;      /* 0x50: hud animation of the item text, -1 = none */
    s32                title_text;     /* 0x54 */
    s32                cursor;         /* 0x58: selected row */
    s32                purchase_state; /* 0x5C */
    Vec4               eye;            /* 0x60 */
    Vec4               look;           /* 0x70 */
    Vec4               basis[4];       /* 0x80: the vendor's transform */
    Vec4               unkC0;
    VendorMenuRow      rows[16];  /* 0xD0 */
    s32                row_count; /* 0x210 */
    u8                 pad214[0xC];
} VendorMenu;

#endif /* LOMBYTE_RNC_OVERLAY_VENDOR_MENU_H */
