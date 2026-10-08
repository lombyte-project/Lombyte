#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_0022abc0/FUN_0022abc0.s", FUN_0022abc0);
#else
#include "types.h"
#include "eetypes.h"

struct SkyShell {
    u8 pad_0[8];
    f32 angle;
};

struct SkyShellSet {
    u8 pad_0[4];
    u16 relocation_state;
    s16 shell_count;
    u8 pad_8[0x18];
    struct SkyShell *shells[1];
};

extern f32 D_00160404 __attribute__((sda));
extern struct SkyShellSet *D_0016045C;
extern u128 D_00160460;
extern u8 D_001D96E0[];
extern void FUN_001f9fc8(void *);
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern void FUN_001fa070(void *, f32 *);
extern void FUN_001f9a80(void *, void *, f32);
extern void sky_draw_shell(s32) __asm__("FUN_0022b690");

void FUN_0022abc0(void) {
    f32 rotation_angles[4];
    f32 shell_scale;
    struct SkyShellSet *shell_set;
    f32 *shell_angle;
    u8 *transform_row;
    s32 shell_index;

    shell_index = 0;
    D_0016045C->relocation_state = 0;
    FUN_001f9fc8(D_001D96E0);
    clear_u64_value(rotation_angles);

    shell_set = D_0016045C;
    if (shell_set->shell_count > 0) {
        do {
            shell_scale = 1.0f;
            shell_angle = &shell_set->shells[shell_index]->angle;
            switch (shell_index) {
            case 1:
                rotation_angles[1] = 0.02f;
                *shell_angle += D_00160404 * 0.55f;
                shell_scale = 3.0f;
                break;
            case 2:
                rotation_angles[1] = -0.02f;
                *shell_angle += D_00160404 * 0.6f;
                shell_scale = 2.5f;
                break;
            case 3:
                rotation_angles[1] = 0.01f;
                *shell_angle += D_00160404 * 0.7f;
                shell_scale = 2.0f;
                break;
            case 4:
                rotation_angles[1] = -0.01f;
                *shell_angle += D_00160404 * 0.75f;
                shell_scale = 1.5f;
                break;
            case 5:
                *shell_angle += D_00160404 * 0.8f;
                shell_scale = 1.25f;
                break;
            default:
                FUN_001f9fc8(D_001D96E0);
                break;
            }

            if (shell_index > 0) {
                rotation_angles[2] =
                    (f32)((s32)(*shell_angle + (f32)shell_index * 2000.0f) % 10000) *
                        0.0006283185f - 3.1415927f;
                FUN_001fa070(D_001D96E0, rotation_angles);
                transform_row = D_001D96E0;
                FUN_001f9a80(transform_row, transform_row, shell_scale);
                FUN_001f9a80(transform_row + 0x10, transform_row + 0x10, shell_scale);
                FUN_001f9a80(transform_row + 0x20, transform_row + 0x20, shell_scale);
                transform_row += 0x30;
                *(u128 *)transform_row = D_00160460;
            }
            sky_draw_shell(shell_index);
            shell_index++;
            shell_set = D_0016045C;
        } while (shell_index < shell_set->shell_count);
    }
}
#endif /* NON_MATCHING */
