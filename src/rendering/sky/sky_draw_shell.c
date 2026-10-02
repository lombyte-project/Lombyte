#include "types.h"

struct SkyShell {
    u8 pad_0[0x4];
    s32 gouraud_enabled;
};

struct SkyShellSet {
    u8 pad_0[0x6];
    s16 shell_count;
    u8 pad_8[0x18];
    struct SkyShell *shells[1];
};

extern struct SkyShellSet *D_0016045C;
extern s32 sky_draw_shell_textured() __asm__("func_0022B6E8");
extern s32 sky_draw_shell_gouraud() __asm__("func_0022B928");
void sky_draw_shell(s32 shell_index) __asm__("FUN_0022b690");

void sky_draw_shell(s32 shell_index) {
    struct SkyShell *shell;

    if (shell_index < D_0016045C->shell_count) {
        shell = D_0016045C->shells[shell_index];
        if (shell->gouraud_enabled != 0) {
            sky_draw_shell_gouraud(shell);
        } else {
            sky_draw_shell_textured(shell);
        }
    }
}

extern __typeof__(sky_draw_shell) func_0022B690 __attribute__((alias("FUN_0022b690")));
