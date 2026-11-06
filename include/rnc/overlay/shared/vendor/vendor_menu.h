#ifndef LOMBYTE_RNC_OVERLAY_SHARED_VENDOR_VENDOR_MENU_H
#define LOMBYTE_RNC_OVERLAY_SHARED_VENDOR_VENDOR_MENU_H

#include "types.h"
#include "rnc/math/vector.h"

/* Vendor menu records: shared by src/overlays/shared/vendor/vendor_menu.c and the vendor data
 * that src/overlays/shared/cutscenes/update_cutscene_playback.c defines. */

typedef struct {
    s32 item;
    s32 selected;
    s32 state;
    s32 unused;
    s32 sound;
} VendorListEntry;

typedef struct {
    u8              pad[0xD0];
    VendorListEntry entries[16];
    s32             count;
} VendorList;

typedef struct {
    s32 sound;   /* 0x00: copied to the item's vendor row */
    f32 unk4[3]; /* 0x04 */
    s32 message; /* 0x10: help text shown for the item */
} VendorSound;

typedef struct {
    u8  b0;
    u8  hidden;
    u8  pad2[0x32];
    f32 offset;
    u8  pad38[8];
} VendorSlot;

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

void open_vendor_menu(void *) __asm__("FUN_L00_0029a6b0");

void update_vendor_menu(void) __asm__("FUN_L00_0029c648");
void initialize_vendor_selected_item_mobys(void) __asm__("FUN_L00_0029b680");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_VENDOR_VENDOR_MENU_H */
