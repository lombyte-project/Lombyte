/* Ported from rac1-decomp, the PAL decompilation (src/game/pause.c, func_0021D9C8). */
extern int D_00141EA0[];
typedef struct {
    char pad[0x30];
    int list[8]; /* 0x30 */
    int idx;     /* 0x50 */
} SelList_0021D9C8;
/* Reload the 8-entry list at +0x30 from D_00141EA0 (the reverse of
   func_0021DA60 below), then leave idx at +0x50 on the first empty entry,
   wrapped to 0..7. Indexing arg0->list directly in both loops (no local
   list pointer) is what gives retail's hoisted base; the scan is a do-while
   guarded by list[0] (a plain while/for rotated differently). */
int load_selection_list(SelList_0021D9C8 *arg0) __asm__("FUN_0021c9c8");

int load_selection_list(SelList_0021D9C8 *arg0) {
    int i;
    for (i = 0; i < 8; i++) {
        arg0->list[i] = D_00141EA0[i];
    }
    arg0->idx = 0;
    if (arg0->list[0] != 0) {
        do {
            arg0->idx++;
        } while (arg0->list[arg0->idx] != 0 && arg0->idx < 8);
    }
    arg0->idx %= 8;
    return 0;
}

extern __typeof__(load_selection_list) func_0021C9C8 __attribute__((alias("FUN_0021c9c8")));
