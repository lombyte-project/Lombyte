/* Ported from rac1-decomp, the PAL decompilation (src/game/effects.c, func_001EE3B0). */
extern int func_001F44B8(int);
extern float fast_cos(float) __asm__("func_001F9DC8");
extern float fast_sin(float) __asm__("func_001F9DE0");
extern float func_001FA580(float, float);
extern void FUN_001f5ab0(float, float, float, float, float, int, int, int, int, int, int, int,
                           float, float);
/*
 * Spawns a burst of particle effects around (arg1, arg2) using the source
 * object's unk10 (a size, scaled by 40.0f) and unk18 (looked up through
 * func_001F44B8 to get a handle passed on to FUN_001f5ab0). unk2C picks
 * the pattern: 0 -- a fan of unk26 particles, stepping the angle (unk1C)
 * by unk28 each time (func_001FA580 adds and wraps the angle); 1 -- four
 * particles offset from the point along and across the current angle;
 * 2 -- a single particle. Matching the case order (1, then 0 vs
 * negative, then 2 vs default) needed a real switch so gcc's own
 * binary-search lowering picked the same compare order as retail.
 *
 * The 40.0f constant is materialized once, before anything else (retail
 * loads it into $f24 before even the first call), because it is live
 * across every switch arm. The four offsets in case 1 live in a local
 * array, not scalars: retail spills them to fixed stack slots (0x0, 0x4,
 * 0x10, 0x14 with an 8-byte gap) instead of extra saved float registers.
 */
void spawn_particle_burst(void *arg0, float arg1, float arg2) __asm__("FUN_001ee008");

void spawn_particle_burst(void *arg0, float arg1, float arg2)
{
    float forty = 40.0f;
    char *p = (char *)arg0;
    int handle = func_001F44B8(*(int *)(p + 0x18));
    int mode = *(int *)(p + 0x2C);
    float angle = *(float *)(p + 0x1C);
    float k;
    int i;

    switch (mode) {
    case 0:
        for (i = 0; i < *(short *)(p + 0x26); i++) {
            FUN_001f5ab0(arg1, arg2, forty * *(float *)(p + 0x10),
                          forty * *(float *)(p + 0x10), angle,
                          0x3F, 0x3F, handle, 0xFFFFF3, *(int *)(p + 0x14), 0, 0,
                          0.0f, 0.0f);
            angle = func_001FA580(angle, *(float *)(p + 0x28));
        }
        break;
    case 1: {
        float tmp[6];

        tmp[0] = fast_sin(angle) * forty * *(float *)(p + 0x10);
        tmp[1] = fast_cos(angle) * forty * *(float *)(p + 0x10);
        tmp[4] = fast_cos(angle) * forty * *(float *)(p + 0x10);
        tmp[5] = fast_sin(angle) * -forty * *(float *)(p + 0x10);

        k = *(float *)(p + 0x10) * forty;
        FUN_001f5ab0(arg1, arg2, k, k, angle,
                      0x3F, 0x3F, handle, 0xFFFFF3, *(int *)(p + 0x14), 0, 0,
                      0.0f, 0.0f);

        k = *(float *)(p + 0x10);
        k *= forty;
        FUN_001f5ab0(arg1 + tmp[4], arg2 + tmp[5], k, k, angle,
                      0x3F, 0x3F, handle, 0xFFFFF3, *(int *)(p + 0x14), 1, 0,
                      0.0f, 0.0f);

        k = *(float *)(p + 0x10);
        k *= forty;
        FUN_001f5ab0(arg1 - tmp[0], arg2 - tmp[1], k, k, angle,
                      0x3F, 0x3F, handle, 0xFFFFF3, *(int *)(p + 0x14), 0, 1,
                      0.0f, 0.0f);

        k = *(float *)(p + 0x10);
        k *= forty;
        FUN_001f5ab0(arg1 + tmp[4] - tmp[0], arg2 + tmp[5] - tmp[1], k, k, angle,
                      0x3F, 0x3F, handle, 0xFFFFF3, *(int *)(p + 0x14), 1, 1,
                      0.0f, 0.0f);
        break;
    }
    case 2:
        k = *(float *)(p + 0x10);
        k *= forty;
        FUN_001f5ab0(arg1, arg2, k, k, angle,
                      0x3F, 0x3F, handle, 0xFFFFF3, *(int *)(p + 0x14), 0, 0,
                      0.5f, 0.5f);
        break;
    }
}

extern __typeof__(spawn_particle_burst) func_001EE008 __attribute__((alias("FUN_001ee008")));
