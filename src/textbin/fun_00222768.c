/* Ported from rac1-decomp, the PAL decompilation (src/game/pause.c, func_00223810). */
extern void func_001F4280(int);
extern void func_001F4398(void);
extern void *func_001FDD10(int);
extern void PackImageDescriptor(void *, char *);
extern void func_001F7580(void *, long, void *, int);
extern void func_00233980(int, long);
/* A text box on the menu's own geometry (PackImageDescriptor's box, then
   arg0+0x18..0x24: top y + 4, bottom y + h - 4, x, x + w, centre).
   Per state (arg0+0x50): the list states draw each text id of the -1
   terminated list at arg0+0x34 down from a start that the line count
   (arg0+0x3C >> 4) centres, taking each height from box[7] as the text
   call leaves it, and flag arg0+0x54 when the list ends 0x18 above the
   bottom; states 4, 11, 15 and 19 draw the single text id at +0x34. The
   line count's low nibble goes through `& 0xF` (retail's lbu + dsrl) and
   the start subtracts its own variable, which keeps retail's order. */
int FUN_00222768(char *arg0) {
    short box[12];
    int ypos;
    int n;
    int i;

    func_00233980(0x47, 0x30000);
    func_00233980(0x42, 0x8000000044L);
    func_001F4280(0);
    PackImageDescriptor(box, arg0);
    box[9] = 9;
    box[3] = *(int *)(arg0 + 0x18) + *(int *)(arg0 + 0x20);
    box[0] = *(int *)(arg0 + 0x1C) + 4;
    box[1] = *(int *)(arg0 + 0x1C) + *(int *)(arg0 + 0x24) - 4;
    box[2] = *(int *)(arg0 + 0x18);
    box[4] = *(int *)(arg0 + 0x18) + (*(int *)(arg0 + 0x20) >> 1);
    box[10] = 0;
    box[11] = 0;
    switch (*(int *)(arg0 + 0x50)) {
    case 1:
    case 2:
    case 3:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 12:
    case 13:
    case 14:
    case 16:
    case 17:
    case 18:
        n = (*(int *)(arg0 + 0x3C) >> 4) - 4;
        ypos = *(int *)(arg0 + 0x1C) - n;
        for (i = 0; (*(int **)(arg0 + 0x34))[i] != -1; i++) {
            box[5] = ypos;
            box[11] = (*(int *)(arg0 + 0x3C) >> 4) & 0xF;
            func_001F7580(box, 0x80FFA888L,
                          func_001FDD10((*(int **)(arg0 + 0x34))[i]), -1);
            ypos += box[7];
            ypos += 10;
        }
        if (ypos + 0x18 < *(int *)(arg0 + 0x1C) + *(int *)(arg0 + 0x24)) {
            *(int *)(arg0 + 0x54) = 1;
        }
        break;
    case 4:
    case 11:
    case 15:
    case 19:
        box[5] = *(int *)(arg0 + 0x1C) + 0x20;
        func_001F7580(box, 0x80FFA888L, func_001FDD10(*(int *)(arg0 + 0x34)), -1);
        break;
    case 0:
        break;
    }
    func_001F4398();
    return 2;
}

extern __typeof__(FUN_00222768) func_00222768 __attribute__((alias("FUN_00222768")));
