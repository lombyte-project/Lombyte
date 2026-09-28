/* Ported from rac1-decomp, the PAL decompilation (src/game/music.c, func_00216F48). */
extern void func_0012ECA0(void *);
typedef struct {
    unsigned int handle; /* 0x00: 0 none, 0xFFFFFFFF starting/released */
    short id;            /* 0x04 */
    short unk06;
    short unk08;
    short state;         /* 0x0A: low bits the state, 0x8000 paused */
    short fade;          /* 0x0C: 0x8000 fading */
    short fadeT;         /* 0x0E */
} MusicPlaying;
extern void func_0012E368(int);
extern void FUN_0012ec70(int);
extern int FUN_001f9770(void *);
extern void FUN_0012ecd0(int, void (*)(int, long), long);
extern void FUN_0012e448(int, void (*)(int, long), long);
extern void FUN_0012ed00(int, void (*)(int, long), long);
extern void FUN_00216bc0(int, long);
extern void func_00216B68(int, long);
extern void FUN_00216990(int, long);
/* music_UpdateStream(music_Playing &): with a live handle and state other
   than 9, state 5 stops the stream (state 6) and state 6 without a
   handle resets; a fading record (+0xC bit 15) pauses the handle once
   (+0xA bit 15) and polls FUN_001f9770 until it reports 2 (+0xC = 4),
   otherwise a paused one is resumed. Unpaused, states 1/8/9 are left
   alone, 2 hands the handle to FUN_0012ed00 unless it is 0xFFFFFFFF,
   and anything but 2/3 releases the handle through FUN_0012ecd0 and
   FUN_0012e448. With no live handle (or state 9) the state is cleared
   when it is 7 or the handle is 0. The handle is unsigned (retail builds
   0xFFFFFFFF with lui/ori), the special case is the else arm (retail
   places it last), and the dispatch reads p->state directly so the 2/3
   range test works on the loaded halfword as retail's does. */
void music_update_stream(MusicPlaying *p) __asm__("FUN_002160a8");

void music_update_stream(MusicPlaying *p) {
    int h;

    if (p->state != 9 && p->handle != 0 && p->handle != 0xFFFFFFFF) {
        if (p->state == 5) {
            if (p->handle != 0) {
                func_0012E368(p->handle);
                p->state = 6;
            } else {
                p->state = 0;
            }
        } else if (p->state == 6) {
            if (p->handle == 0) {
                p->state = 0;
            }
        }
        if (p->handle == 0) {
            return;
        }
        if (p->fade & 0x8000) {
            if (!(p->state & 0x8000)) {
                FUN_0012ec70(p->handle);
                p->state |= 0x8000;
            }
            if (FUN_001f9770(&p->fadeT) == 2) {
                p->fade = 4;
            }
        } else if (p->state & 0x8000) {
            func_0012ECA0((void *)p->handle);
            p->state ^= 0x8000;
        }
        if (p->state & 0x8000) {
            return;
        }
        if (p->state == 1 || p->state == 8 || p->state == 9) {
            return;
        }
        if (p->state != 2 && p->state != 3) {
            h = p->handle;
            p->handle = 0xFFFFFFFF;
            FUN_0012ecd0(h, FUN_00216bc0, (long)(unsigned int)p);
            FUN_0012e448(h, func_00216B68, (long)(unsigned int)p);
            return;
        }
        if (p->handle != 0xFFFFFFFF && p->state == 2) {
            FUN_0012ed00(p->handle, FUN_00216990, (long)(unsigned int)p);
        }
    } else if (p->state == 7 || p->handle == 0) {
        p->state = 0;
    }
}

extern __typeof__(music_update_stream) func_002160A8 __attribute__((alias("FUN_002160a8")));
