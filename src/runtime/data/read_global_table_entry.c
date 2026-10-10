typedef int s32;

extern s32 GlobalTableStatus __asm__("D_0015ECC4");

void read_global_table_entry(void) __asm__("ReadGlobalTableEntry");

void read_global_table_entry(void) {
    GlobalTableStatus = 1;
}
