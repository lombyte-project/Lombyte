/* Ported from rac1-decomp (src/game/help.c, func_001FE438). */
typedef struct {
    int state;  /* 0x00 */
    int x04;    /* 0x04 */
    int pad[7]; /* 0x08 */
    int x24;    /* 0x24 */
    int x28;    /* 0x28 */
    int count;  /* 0x2C: entries in the D_0015F780 table */
} HelpState;
extern char D_001996D0[];
/* Advances the help screen's state (D_001996D0). Every access goes
   through the global, and cases 6 and 7 are spelled out: the jump table
   has eight entries. */
void dismiss_help(void) __asm__("FUN_001fdc08");

void dismiss_help(void) {
    switch (((HelpState *)D_001996D0)->state) {
    case 0:
        ((HelpState *)D_001996D0)->x24 = -1;
        break;
    case 1:
    case 2:
        ((HelpState *)D_001996D0)->state = 7;
        ((HelpState *)D_001996D0)->x04 = 0;
        break;
    case 3:
        ((HelpState *)D_001996D0)->state = 7;
        ((HelpState *)D_001996D0)->x04 = 0;
        break;
    case 4:
        ((HelpState *)D_001996D0)->state = 6;
        ((HelpState *)D_001996D0)->x04 = 4 - ((HelpState *)D_001996D0)->x04;
        break;
    case 5:
        ((HelpState *)D_001996D0)->state = 6;
        ((HelpState *)D_001996D0)->x04 = 0;
        break;
    case 6:
    case 7:
        break;
    }
}

extern __typeof__(dismiss_help) func_001FDC08 __attribute__((alias("FUN_001fdc08")));
