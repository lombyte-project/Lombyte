/* Return the fixed core-data table address used by parser helpers. */

typedef unsigned char u8;

extern u8 CoreDataTable[1] __asm__("D_00132D40") __attribute__((section(".data")));

void *GetCoreDataTable(void) __asm__("GetCoreDataTable");

void *GetCoreDataTable(void) {
    return CoreDataTable;
}
