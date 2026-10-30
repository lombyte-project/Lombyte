/* Ported from rac1-decomp (src/game/memcard.c, func_0020BCB0). */
typedef struct {
    int a;        /* 0x00 */
    int b;        /* 0x04 */
    int c;        /* 0x08 */
    int d;        /* 0x0C */
    char name[8]; /* 0x10 */
    int valid;    /* 0x18 */
} McEntry;        /* 0x1C */
typedef struct {
    char hdr[0x20];
    McEntry e[5];
    char pad[0xC];
} McSlot; /* 0xB8 */
extern McSlot D_0013D290[];
extern int validate_data_crc(char *) __asm__("func_0020AD38"); /* memcard_TestChecksum */
/* memcard_RestoreInfo(char *, int, int). Advancing the buf parameter
   itself and copying the name with memcpy both matter for retail's
   registers; re-indexing the entry per store keeps its daddu copies. */
void memcard_restore_info(char *buf, int slot, int idx) __asm__("FUN_0020ae60");

void memcard_restore_info(char *restore_data, int slot, int entry_index) {
    D_0013D290[slot].e[entry_index].valid = validate_data_crc(restore_data) == 0;
    restore_data += 0x10;
    D_0013D290[slot].e[entry_index].a = *(int *)restore_data;
    restore_data += 0xC;
    D_0013D290[slot].e[entry_index].b = *(int *)restore_data;
    restore_data += 0xC;
    D_0013D290[slot].e[entry_index].c = *(int *)restore_data;
    restore_data += 0xC;
    D_0013D290[slot].e[entry_index].d = *(int *)restore_data;
    restore_data += 0xC;
    memcpy(D_0013D290[slot].e[entry_index].name, restore_data, 8);
}

extern __typeof__(memcard_restore_info) func_0020AE60 __attribute__((alias("FUN_0020ae60")));
