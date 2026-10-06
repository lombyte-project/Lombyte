/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9438.c, func_L00_002E0AE8). */
extern void func_0023A318(void *);
void FUN_0023a2c0(char *a) {
    if (*(unsigned char *)(a + 0x70) & 2) {
        float o = 1.0f;
        if (0.0f < *(float *)(a + 0x58))
            o = 0.0f;
        *(float *)(a + 0x54) = o;
        *(float *)(a + 0x58) = 0.0f;
    }
    func_0023A318(a);
}

extern __typeof__(FUN_0023a2c0) func_0023A2C0 __attribute__((alias("FUN_0023a2c0")));
