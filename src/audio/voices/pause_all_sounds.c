#include "types.h"
#include "sda.h"
#include "rnc/ui/menus/menu_system.h"

extern s32 D_0015ED84 MACRO_ADDR;
#include "rnc/gameplay/camera/update_cam.h"
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
extern s32 D_0015F604 MACRO_ADDR;

extern void snd_pause_all_sounds_in_group(s32) __asm__("FUN_0012e3e8");
extern void music_pause(s32);
extern s32 snd_flush_sound_commands() __asm__("FUN_0012dc80");
extern s32 update_mission_list() __asm__("FUN_0020b950");
extern void func_00226E58();

void pause_all_sounds(s32 mode) __asm__("FUN_00218d78");

void pause_all_sounds(s32 mode) {
    struct MenuSystem *g;
    snd_pause_all_sounds_in_group(0x1D);
    music_pause(0);
    snd_flush_sound_commands();
    if (camera_position_publication_suppressed[0] != 0) {
        D_0015F674 = 1;
        return;
    }
    if (D_00141660[0] == 0x24) {
        D_00141660[0] = 0;
    }
    {
        struct MenuSystem *g = &menu_system;
        g->unk134 = *(volatile s32 *)&D_0015ED84 == 0xD || D_0014161B != 0;
    }
    {
        struct MenuSystem *g = &menu_system;
        g->unk138 = D_0015ED84 == 0 || D_0015ED84 == 0xE;
    }
    {
        struct MenuSystem *g = &menu_system;
        g->unkD8 = D_0015EEA0 != 0 || D_0015EE20 != 0 || g->unkF8 != 0;
    }
    {
        struct MenuSystem *g = &menu_system;
        char *a = D_001CE5B8;
        g->unkDC = mode == 0x23;
        *(char **)(a + 0x38) = g->unkD8 ? D_001CE798 : D_001CE748;
    }
    {
        struct MenuSystem *g = &menu_system;
        char *b = D_001CE748;
        *(char **)(b + 0x3C) = g->unkD8 ? D_001CE798 : D_001CE5B8;
    }
    {
        struct MenuSystem *g = &menu_system;
        D_0015F604 = 3;
        g->state = mode;
        g->close_request = 0;
        g->unk10 = 0;
        g->update_count = 0;
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
        struct MenuSystem *g = &menu_system;
        g->unk13C = 1;
        g->unk140 = 0;
    }
}

extern __typeof__(pause_all_sounds) func_00218D78 __attribute__((alias("FUN_00218d78")));

/* Defined below their only users, so retail reaches them with lui. */
s32 D_0015F674 MACRO_ADDR = 0;
