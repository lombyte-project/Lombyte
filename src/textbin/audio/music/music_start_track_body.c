/* Ported from rac1-decomp, the PAL decompilation (src/game/music.c, func_00216B68). */
extern int D_00137B80[];
extern short D_001516D0[];
extern void func_0012EC08(int, int, int, int, short, int, int, int,
                          int, void (*)(int, long), long);
extern void FUN_00216ad0(int, long);
/* music_StartTrackBody(int, int, int): when the music record at +0x34 is
   already playing (its word neither 0 nor -1) and not in state 9, starts
   track arg0 + 1 of the table at D_00137B80 + 0x2AA8 on it: state 9, the
   track, arg1 and arg2 recorded, 10 and 48000 stored, then
   func_0012EC08 with the current word passed on as its eighth argument
   and 0x24 or 0x20 by arg1's bit 0. The handle is read twice as in
   func_00216A90 (CSE merges the loads, and the address stays in a
   register as in retail), and the 0x24/0x20 choice comes before the
   stores, which keeps arg1 in $a1. */
void music_start_track_body(int arg0, int arg1, int arg2) __asm__("FUN_00215d18");

void music_start_track_body(int arg0, int arg1, int arg2) {
    char *s = (char *)D_001516D0;
    char *base;
    int *tbl;
    unsigned int cur;
    long h;
    int i;
    int flags;

    if (*(short *)(s + 0x3E) == 9) {
        return;
    }
    cur = *(unsigned int *)(s + 0x34);
    if (cur == 0 || cur == 0xFFFFFFFF) {
        return;
    }
    base = (char *)D_00137B80;
    tbl = (int *)(base + 0x2AA8);
    i = arg0 + 1;
    if (tbl[i] == 0) {
        return;
    }
    h = tbl[i];
    flags = (arg1 & 1) ? 0x24 : 0x20;
    *(short *)(s + 0x38) = arg0;
    *(short *)(s + 0x3C) = arg1;
    *(short *)(s + 0x3E) = 9;
    *(int *)(s + 0x48) = 10;
    *(int *)(s + 0x4C) = 0xBB80;
    *(short *)(s + 0x3A) = arg2;
    *(short *)(s + 0x44) = 0;
    func_0012EC08(h, 0, 0, 0, arg2, 0, 1, cur, flags,
                  FUN_00216ad0, (long)(unsigned int)(s + 0x34));
}

extern __typeof__(music_start_track_body) func_00215D18 __attribute__((alias("FUN_00215d18")));
