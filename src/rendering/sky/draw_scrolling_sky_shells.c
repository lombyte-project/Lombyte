#include "types.h"
#include "eetypes.h"
#include "qcopy.h"
#include "rnc/rendering/sky_shell.h"

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

extern f32 D_00160400 __attribute__((sda));
extern struct SkyShellSet *D_0016045C;
extern u8 sky_matrix[] __asm__("D_001D96E0");
extern void set_identity_matrix(void *) __asm__("FUN_001f9fc8");
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern void fun_001fa070(void *, f32 *) __asm__("FUN_001fa070");
extern void scale_vector(void *, void *, f32) __asm__("FUN_001f9a80");

void draw_scrolling_sky_shells(void) __asm__("FUN_0022abd8");

void draw_scrolling_sky_shells(void) {
    enum {
        SKY_SHELL_TRIPLE_SCALE_LAYER = 1,
        SKY_SHELL_TWO_AND_HALF_SCALE_LAYER = 2,
        SKY_SHELL_DOUBLE_SCALE_LAYER = 3,
        SKY_SHELL_ONE_AND_HALF_SCALE_LAYER = 4,
        SKY_SHELL_ONE_AND_QUARTER_SCALE_LAYER = 5
    };
    f32 rotation_angles[4];
    f32 shell_scale;
    struct SkyShellSet *shell_set;
    f32 *shell_angle;
    u8 *transform_row;
    s32 shell_index;

    shell_index = 0;
    D_0016045C->relocation_state = 0;
    set_identity_matrix(sky_matrix);
    clear_u64_value(rotation_angles);

    shell_set = D_0016045C;
    if (shell_set->shell_count > 0) {
        do {
            shell_scale = 1.0f;
            shell_angle = &shell_set->shells[shell_index]->angle;
            switch (shell_index) {
            case SKY_SHELL_TRIPLE_SCALE_LAYER:
                rotation_angles[1] = 0.02f;
                *shell_angle += D_00160400 * 0.55f;
                shell_scale = 3.0f;
                break;
            case SKY_SHELL_TWO_AND_HALF_SCALE_LAYER:
                rotation_angles[1] = -0.02f;
                *shell_angle += D_00160400 * 0.6f;
                shell_scale = 2.5f;
                break;
            case SKY_SHELL_DOUBLE_SCALE_LAYER:
                rotation_angles[1] = 0.01f;
                *shell_angle += D_00160400 * 0.7f;
                shell_scale = 2.0f;
                break;
            case SKY_SHELL_ONE_AND_HALF_SCALE_LAYER:
                rotation_angles[1] = -0.01f;
                *shell_angle += D_00160400 * 0.75f;
                shell_scale = 1.5f;
                break;
            case SKY_SHELL_ONE_AND_QUARTER_SCALE_LAYER:
                *shell_angle += D_00160400 * 0.8f;
                shell_scale = 1.25f;
                break;
            default:
                set_identity_matrix(sky_matrix);
                break;
            }

            if (shell_index > 0) {
                rotation_angles[2] =
                    (f32)((s32)(*shell_angle + (f32)shell_index * 2000.0f) % 10000) *
                        0.0006283185f - 3.1415927f;
                fun_001fa070(sky_matrix, rotation_angles);
                transform_row = sky_matrix;
                scale_vector(transform_row, transform_row, shell_scale);
                scale_vector(transform_row + 0x10, transform_row + 0x10, shell_scale);
                scale_vector(transform_row + 0x20, transform_row + 0x20, shell_scale);
                transform_row += 0x30;
                qcopy(transform_row, &D_00160460);
            }
            sky_draw_shell(shell_index);
            shell_index++;
            shell_set = D_0016045C;
        } while (shell_index < shell_set->shell_count);
    }
}

extern __typeof__(draw_scrolling_sky_shells) func_0022ABD8 __attribute__((alias("FUN_0022abd8")));
