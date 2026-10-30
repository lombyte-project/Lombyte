#include "types.h"
#include "eetypes.h"
#include "qcopy.h"
#include "rnc/globals.h"
#include "rnc/rendering/sky_shell.h"
#include "rnc/rendering/vu1_registers.h"

struct SkyShellSet {
    u8  pad_0[4];
    u16 relocation_state;
    s16 shell_count;
};

struct SkyTransform {
    u8  pad[0x30];
    u64 translation;
};

extern f32                 D_00160404 __attribute__((sda));
extern struct SkyTransform sky_matrix __asm__("D_001D96E0");
extern void                clear_u64_value(f32 *) __asm__("func_001F99F8");
extern void                scale_vector(void *, void *, f32) __asm__("FUN_001f9a80");
extern void                set_identity_matrix(void *) __asm__("FUN_001f9fc8");
extern void                euler_to_matrix_ee(void *, f32 *) __asm__("FUN_001fa070");
extern f32                 fast_add_rotations(f32, f32) __asm__("func_001FA580");
extern void                setup_sky_gif_paging(void) __asm__("func_0022B4C8");
extern void                do_sky_gif_paging(void) __asm__("func_0022B558");

/* Draws the sky layers (shells) one by one, always starting with shell 0.
 * Later shells are drawn on top of earlier ones. Shells 0 and 1 are drawn
 * as they are. Shells 2 to 5 are tilted a little, turned a little, and made
 * bigger (1.25 to 2 times). */
void draw_sky_shells(void) __asm__("FUN_0022b288");

extern struct SkyShellSet *level_sky_effect_data __asm__("D_0016045C");

void draw_sky_shells(void) {
    enum {
        SKY_SHELL_FIRST_UNIT_SCALE = 0,
        SKY_SHELL_UNIT_SCALE = 1,
        SKY_SHELL_ONE_AND_QUARTER_SCALE = 2,
        SKY_SHELL_ONE_AND_HALF_SCALE = 3,
        SKY_SHELL_ONE_AND_THREE_QUARTERS_SCALE = 4,
        SKY_SHELL_DOUBLE_SCALE = 5
    };
    f32 rotation_angles[4];
    f32 shell_scale;
    u8 *transform_row;
    s32 shell_index;

    shell_index = 0;
    setup_sky_gif_paging();
    level_sky_effect_data->relocation_state = 0;
    set_identity_matrix(&sky_matrix);
    clear_u64_value(rotation_angles);
    if (level_sky_effect_data->shell_count > 0) {
        do {
            shell_scale = 1.0f;
            switch (shell_index) {
            case SKY_SHELL_FIRST_UNIT_SCALE:
                *(s32 *)&rotation_angles[1] = 0;
                rotation_angles[2] = D_00160404;
            case SKY_SHELL_UNIT_SCALE:
                *(s32 *)&rotation_angles[1] = 0;
                rotation_angles[2] = fast_add_rotations(D_00160404, rotation_angles[1]);
                shell_scale = 1.0f;
                break;
            case SKY_SHELL_ONE_AND_QUARTER_SCALE:
                rotation_angles[1] = -0.075f;
                rotation_angles[2] = fast_add_rotations(D_00160404, -0.15f);
                shell_scale = 1.25f;
                break;
            case SKY_SHELL_ONE_AND_HALF_SCALE:
                rotation_angles[1] = 0.05f;
                rotation_angles[2] = fast_add_rotations(D_00160404, 0.125f);
                shell_scale = 1.5f;
                break;
            case SKY_SHELL_ONE_AND_THREE_QUARTERS_SCALE:
                rotation_angles[1] = 0.1f;
                rotation_angles[2] = fast_add_rotations(D_00160404, -0.05f);
                shell_scale = 1.75f;
                break;
            case SKY_SHELL_DOUBLE_SCALE:
                rotation_angles[1] = -0.15f;
                rotation_angles[2] = fast_add_rotations(D_00160404, 0.1f);
                shell_scale = 2.0f;
                break;
            }
            euler_to_matrix_ee(&sky_matrix, rotation_angles);
            transform_row = (u8 *)&sky_matrix;
            scale_vector(transform_row, transform_row, shell_scale);
            scale_vector(transform_row + 0x10, transform_row + 0x10, shell_scale);
            scale_vector(transform_row + 0x20, transform_row + 0x20, shell_scale);
            transform_row += 0x30;
            qcopy(transform_row, &D_00160460);
            sky_draw_shell(shell_index);
            shell_index++;
        } while (shell_index < level_sky_effect_data->shell_count);
    }
    do_sky_gif_paging();
    vu1_add_g_sregister(0x47, 0x5360B);
    vu1_add_g_sregister(0x4E, 0x1000000 | (depth_buffer_address >> 13));
}

extern __typeof__(draw_sky_shells) func_0022B288 __attribute__((alias("FUN_0022b288")));
