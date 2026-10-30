/* Vendor: flags the vendor item at a table slot when the current moby is class 0x13. */
#include "types.h"
#include "asm.h"
#include "sda.h"

/* If the current moby is class 0x13, flag the vendor item at table slot `slot` (32-byte records). */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B8FC0.c: func_L13_003184A0), where it is exact; names translated to the US level program. */

#define VENDOR_CLASS_ID 0x13

typedef struct {
    char          pad0[0x39];
    unsigned char field39;
} Level13VendorItem;

typedef struct {
    char               pad0[0x1C];
    Level13VendorItem *item;
} Level13VendorRecord;

typedef struct {
    char  pad0[0x86];
    short class_id;
} Level13VendorCurrentMoby;

extern Level13VendorCurrentMoby *D_L13_00167100 NOT_SDA;
extern Level13VendorRecord                     *l13_vendor_item_records __asm__("D_L13_0015EF50");

void flag_vendor_item(int slot) __asm__("FUN_L13_00316f48");

void flag_vendor_item(int slot) {
    Level13VendorCurrentMoby *moby = D_L13_00167100;
    Level13VendorRecord      *vendor_item_record =
        (Level13VendorRecord *)(slot * 32 + (int)l13_vendor_item_records);
    Level13VendorItem *item = vendor_item_record->item;
    if (moby->class_id == VENDOR_CLASS_ID)
        item->field39 = 1;
}
