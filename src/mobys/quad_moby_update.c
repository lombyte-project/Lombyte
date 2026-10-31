/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9438.c, func_L00_002E0AE8). */
extern void update_instance_attachment_points_and_lengths(void *) __asm__("func_0023A318");
void        quad_moby_update(char *instance) __asm__("FUN_0023a2c0");

/* Moby class 1138 */
void quad_moby_update(char *instance) {
    if (*(unsigned char *)(instance + 0x70) & 2) {
        float selected_reset_value = 1.0f;
        if (0.0f < *(float *)(instance + 0x58))
            selected_reset_value = 0.0f;
        *(float *)(instance + 0x54) = selected_reset_value;
        *(float *)(instance + 0x58) = 0.0f;
    }
    update_instance_attachment_points_and_lengths(instance);
}

extern __typeof__(quad_moby_update) func_0023A2C0 __attribute__((alias("FUN_0023a2c0")));
