#include "types.h"
#include "sda.h"

extern s32 D_0015ED84 MACRO_ADDR;
extern s32 D_0018C32C[];
extern s32 D_0015F674 MACRO_ADDR;
extern s32 D_00141660[];
extern u8 D_0014161B NOT_SDA;
extern s32 D_0015EEA0 MACRO_ADDR;
extern s32 D_0015EE20 MACRO_ADDR;
extern char D_001CE5B8[];
extern char D_001CE748[];
extern char D_001CE798[];
extern s32 D_001A0314[];
extern s32 D_001D0398[];
extern char D_001D5BF0[] NOT_SDA;
extern s32 D_0015F604 MACRO_ADDR;

extern void snd_pause_all_sounds_in_group(s32) __asm__("FUN_0012e3e8");
extern void music_pause(s32);
extern s32 snd_flush_sound_commands() __asm__("FUN_0012dc80");
extern s32 update_mission_list() __asm__("FUN_0020b950");
extern void func_00226E58();

void pause_all_sounds(s32 mode) __asm__("FUN_00218d78");

void pause_all_sounds(s32 mode) {
    char *g;
    snd_pause_all_sounds_in_group(0x1D);
    music_pause(0);
    snd_flush_sound_commands();
    if (D_0018C32C[0] != 0) {
        D_0015F674 = 1;
        return;
    }
    if (D_00141660[0] == 0x24) {
        D_00141660[0] = 0;
    }
    {
        char *g = D_001D5BF0;
        *(s32 *)(g + 0x134) = *(volatile s32 *)&D_0015ED84 == 0xD || D_0014161B != 0;
    }
    {
        char *g = D_001D5BF0;
        *(s32 *)(g + 0x138) = D_0015ED84 == 0 || D_0015ED84 == 0xE;
    }
    {
        char *g = D_001D5BF0;
        *(s32 *)(g + 0xD8) = D_0015EEA0 != 0 || D_0015EE20 != 0 || *(s32 *)(g + 0xF8) != 0;
    }
    {
        char *g = D_001D5BF0;
        char *a = D_001CE5B8;
        *(s32 *)(g + 0xDC) = mode == 0x23;
        *(char **)(a + 0x38) = *(s32 *)(g + 0xD8) ? D_001CE798 : D_001CE748;
    }
    {
        char *g = D_001D5BF0;
        char *b = D_001CE748;
        *(char **)(b + 0x3C) = *(s32 *)(g + 0xD8) ? D_001CE798 : D_001CE5B8;
    }
    {
        char *g = D_001D5BF0;
        D_0015F604 = 3;
        *(s32 *)g = mode;
        *(s32 *)(g + 0xC) = 0;
        *(s32 *)(g + 0x10) = 0;
        *(s32 *)(g + 0x110) = 0;
    }
    {
        s32 m = *(volatile s32 *)&D_0015ED84;
        if (m < 0x13) {
            D_001A0314[0] = m;
        } else {
            D_001A0314[0] = 0;
        }
    }
    update_mission_list();
    D_001D0398[0] = 0;
    func_00226E58();
    {
        char *g = D_001D5BF0;
        *(s32 *)(g + 0x13C) = 1;
        *(s32 *)(g + 0x140) = 0;
    }
}

extern __typeof__(pause_all_sounds) func_00218D78 __attribute__((alias("FUN_00218d78")));
