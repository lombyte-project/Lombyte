/* Sky shell rotation and rendering programs. Particle state and rendering
 * live in sky_particle_effects.c. */
#include "types.h"
#include "rnc/rendering/sky_shell.h"
#include "asm.h"
#include "rnc/rendering/sky_particles.h"
#include "sda.h"
#include "qzero.h"
#include "rnc/overlay/quad.h"
#include "qcopy.h"

extern char D_L00_001BD7E0[];
extern char *D_L00_0016051C;
void set_identity_matrix() __asm__("FUN_001f9fc8");
/* Runs set_identity_matrix on D_L00_001BD7E0 and draws each sky shell. */
void update_sky_shells_static(void) __asm__("FUN_L00_002892d0");
void update_sky_shells_static(void) {
    int i;
    set_identity_matrix(D_L00_001BD7E0);
    for (i = 0; i < *(short *)(D_L00_0016051C + 6); i++)
        sky_draw_shell(i);
}

/* rotates animated sky shells and draws each layer */
/* Ported from rac1-decomp (src/overlays/shared/shrubproc_0028A198.c: func_L00_0028A6F8), where it is exact; names translated to the US level program. */

extern char *D_L00_0016051C;
extern char D_L00_001BD7E0[] NOT_SDA;
extern int D_L00_0015F5CC;
extern void FUN_001f9fc8_c(void *) __asm__("FUN_001f9fc8");
void fun_001fa070(void *, void *) __asm__("FUN_001fa070");

void update_sky_shells_animated(void) __asm__("FUN_L00_00289420");
void update_sky_shells_animated(void) {
    enum {
        SKY_SHELL_SLOW_ROTATION = 2,
        SKY_SHELL_MEDIUM_ROTATION = 3,
        SKY_SHELL_FAST_ROTATION = 4
    };
    float v[4] __attribute__((aligned(16)));
    int i;
    *(short *)(D_L00_0016051C + 4) = 0;
    qzero(v);
    for (i = 0; i < *(short *)(D_L00_0016051C + 6); i++) {
        switch (i) {
        case SKY_SHELL_SLOW_ROTATION:
            v[2] = (D_L00_0015F5CC & 0x3ffff) * 0.000023968450f - 3.14159265f;
            fun_001fa070(D_L00_001BD7E0, v);
            break;
        case SKY_SHELL_MEDIUM_ROTATION:
            v[2] = (D_L00_0015F5CC & 0x1ffff) * 0.000047936900f - 3.14159265f;
            fun_001fa070(D_L00_001BD7E0, v);
            break;
        case SKY_SHELL_FAST_ROTATION:
            v[2] = (unsigned short)D_L00_0015F5CC * 0.000095873800f - 3.14159265f;
            fun_001fa070(D_L00_001BD7E0, v);
            break;
        default:
            FUN_001f9fc8_c(D_L00_001BD7E0);
        }
        sky_draw_shell(i);
    }
}

/* D_L00_0015F5CC is read as u16 for the first sky rotation and masked with 0x1FFFF for the second. */

typedef struct {
    f32 f[4];
} __attribute__((aligned(16))) SkyRotVec_2899a8;

extern char *D_L00_0016051C_2899a8 __asm__("D_L00_0016051C");
extern char D_L00_001BD7E0[] __asm__("D_L00_001BD7E0");
extern s32 D_L00_0015F5CC_2899a8 __asm__("D_L00_0015F5CC");
extern void FUN_001f9fc8_2899a8(void *) __asm__("FUN_001f9fc8");
extern void FUN_001fa070_2899a8(void *, void *) __asm__("FUN_001fa070");

void FUN_L00_002899a8_2899a8(void) __asm__("FUN_L00_002899a8");

void FUN_L00_002899a8_2899a8(void) {
    SkyRotVec_2899a8 rot;
    *(short *)(D_L00_0016051C_2899a8 + 4) = 0;
    FUN_001f9fc8_2899a8(D_L00_001BD7E0);
    sky_draw_shell(0);
    qzero(&rot);
    rot.f[2] = (f32) * (u16 *)&D_L00_0015F5CC_2899a8 * 9.5873802e-5f - 3.1415927f;
    FUN_001fa070_2899a8(D_L00_001BD7E0, &rot);
    sky_draw_shell(1);
    rot.f[2] =
        (f32)(*(volatile int *)&D_L00_0015F5CC_2899a8 & 0x1FFFF) * 4.7936901e-5f - 3.1415927f;
    FUN_001fa070_2899a8(D_L00_001BD7E0, &rot);
    sky_draw_shell(2);
}
/* draws sky shells with two animated rotation offsets */
/* Ported from rac1-decomp (src/overlays/shared/shrubproc_0028A198.c: func_L00_0028AD68), where it is exact; names translated to the US level program. */

extern void FUN_001fa070_cf(void *) __asm__("FUN_001f9fc8");
void FUN_001f9fc8_cf(void *, void *) __asm__("FUN_001fa070");

void FUN_L00_00289a90(void) {
    enum {
        SKY_SHELL_FAST_ROTATION = 1,
        SKY_SHELL_SLOW_ROTATION = 2
    };
    float v[4] __attribute__((aligned(16)));
    int i;
    *(short *)(D_L00_0016051C + 4) = 0;
    qzero(v);
    for (i = 0; i < *(short *)(D_L00_0016051C + 6); ++i) {
        switch (i) {
        case SKY_SHELL_FAST_ROTATION:
            v[2] = (float)*(unsigned short *)&D_L00_0015F5CC * 0.0000958738017f - 3.14159265f;
            FUN_001f9fc8_cf(D_L00_001BD7E0, v);
            break;
        case SKY_SHELL_SLOW_ROTATION:
            v[2] = (float)(D_L00_0015F5CC & 0x1FFFF) * 0.00004793690085f - 3.14159265f;
            FUN_001f9fc8_cf(D_L00_001BD7E0, v);
            break;
        default:
            FUN_001fa070_cf(D_L00_001BD7E0);
            break;
        }
        sky_draw_shell(i);
    }
}
typedef union {
    OvlQuad q;
    float f[4];
} U289bd8;
extern int D_L00_0015F5CC;
void set_identity_matrix(void *) __asm__("FUN_001f9fc8");
void sky_sprite_proc(void) __asm__("FUN_0022bba0");
void FUN_00233980(int, long);
void fun_001fa070(void *, void *) __asm__("FUN_001fa070");
void FUN_L00_00289bd8(void) {
    float v[4] __attribute__((aligned(16)));
    set_identity_matrix(D_L00_001BD7E0);
    sky_draw_shell(0);
    if (*(short *)(D_L00_0016051C + 8) == 0)
        sky_particles_init(0x78, 8);
    sky_particles_update();
    sky_sprite_proc();
    FUN_00233980(0x42, (0x8000L << 24) | 0x44);
    qzero(v);
    v[2] = (D_L00_0015F5CC % 50000) * 0.000125663704f - 3.14159265f;
    fun_001fa070(D_L00_001BD7E0, v);
    sky_draw_shell(1);
}
extern char D_L00_001BD7E0[];
extern char *D_L00_0016051C;
extern int D_L00_0015F5CC;
void set_identity_matrix(void *) __asm__("FUN_001f9fc8");
void sky_sprite_proc(void) __asm__("FUN_0022bba0");
void FUN_00233980(int, long);
void fun_001fa070(void *, void *) __asm__("FUN_001fa070");
void FUN_L00_0028a208(void) {
    float v[4] __attribute__((aligned(16)));
    set_identity_matrix(D_L00_001BD7E0);
    sky_draw_shell(0);
    if (*(short *)(D_L00_0016051C + 8) == 0)
        sky_particles_init(0x78, 8);
    sky_particles_update();
    sky_sprite_proc();
    FUN_00233980(0x42, (0x8000L << 24) | 0x44);
    sky_draw_shell(1);
    qzero(v);
    v[2] = (D_L00_0015F5CC % 50000) * 0.000125663704f - 3.14159265f;
    fun_001fa070(D_L00_001BD7E0, v);
    sky_draw_shell(2);
}
typedef union {
    OvlQuad q;
    f32 f[4];
} V0028a2f0;
extern char D_L00_001BD7E0[] __asm__("D_L00_001BD7E0");
extern u8 *D_L00_0016051C_c2 __asm__("D_L00_0016051C");
extern s32 D_L00_0015F5CC_c __asm__("D_L00_0015F5CC");
void set_identity_matrix(void *) __asm__("FUN_001f9fc8");
void FUN_L00_001ffa90(void *, void *);
void FUN_L00_0028a2f0(void) {
    V0028a2f0 shell_rotation;
    *(s16 *)(D_L00_0016051C_c2 + 4) = 0;
    set_identity_matrix(D_L00_001BD7E0);
    sky_draw_shell(0);
    qzero(&shell_rotation);
    shell_rotation.f[2] = (f32)(D_L00_0015F5CC_c % 40000) * 0.00015707963029854f - 3.1415927f;
    FUN_L00_001ffa90(D_L00_001BD7E0, &shell_rotation);
    sky_draw_shell(1);
    set_identity_matrix(D_L00_001BD7E0);
    sky_draw_shell(2);
}
extern int D_L00_0015F5CC;
void set_identity_matrix(void *) __asm__("FUN_001f9fc8");
void fun_001fa070(void *, void *) __asm__("FUN_001fa070");
void FUN_L00_0028a3a8(void) {
    float v[3];
    *(short *)(D_L00_0016051C + 4) = 0;
    set_identity_matrix(D_L00_001BD7E0);
    sky_draw_shell(0);
    sky_draw_shell(1);
    sky_draw_shell(2);
    v[0] = 0.0f;
    v[1] = 0.3f;
    v[2] = (D_L00_0015F5CC % 25000) * 0.000251327408f - 3.14159265f;
    fun_001fa070(D_L00_001BD7E0, v);
    sky_draw_shell(3);
    set_identity_matrix(D_L00_001BD7E0);
    sky_draw_shell(4);
}
extern int D_L00_0015F5CC_28a480g __asm__("D_L00_0015F5CC");
extern int D_L00_0015F5CC_28a480d __asm__("D_L00_0015F5CC");
extern char *D_L00_0016051C_28a480 __asm__("D_L00_0016051C");
extern char D_L00_001BD7E0_lui[] __asm__("D_L00_001BD7E0") __attribute__((section(".data")));
extern void FUN_001fa070_28a480(void *, void *) __asm__("FUN_001fa070");
extern void FUN_001f9fc8_28a480(void *) __asm__("FUN_001f9fc8");

void FUN_L00_0028a480(void) {
    enum {
        SKY_SHELL_DOUBLE_PHASE_ROTATION = 0,
        SKY_SHELL_QUINTUPLE_PHASE_ROTATION = 1,
        SKY_SHELL_TENFOLD_PHASE_WIDE_WRAP_ROTATION = 2,
        SKY_SHELL_TENFOLD_PHASE_ROTATION = 3
    };
    OvlQuad v[1];
    int i;

    qzero(v);
    for (i = 0; i < *(short *)(D_L00_0016051C_28a480 + 6); i++) {
        switch (i) {
        case SKY_SHELL_DOUBLE_PHASE_ROTATION:
            ((float *)v)[2] =
                (float)((D_L00_0015F5CC_28a480g * 2) & 0x1FFFF) * (3.14159265f / 65536.0f) -
                3.14159265f;
            FUN_001fa070_28a480(D_L00_001BD7E0_lui, v);
            break;
        case SKY_SHELL_QUINTUPLE_PHASE_ROTATION:
            ((float *)v)[2] =
                (float)((D_L00_0015F5CC_28a480d * 5) & 0x1FFFF) * (3.14159265f / 65536.0f) -
                3.14159265f;
            FUN_001fa070_28a480(D_L00_001BD7E0_lui, v);
            break;
        case SKY_SHELL_TENFOLD_PHASE_WIDE_WRAP_ROTATION:
            ((float *)v)[2] =
                (float)((D_L00_0015F5CC_28a480d * 10) & 0x3FFFF) * 2.39684496e-05f - 3.14159265f;
            FUN_001fa070_28a480(D_L00_001BD7E0_lui, v);
            break;
        case SKY_SHELL_TENFOLD_PHASE_ROTATION:
            ((float *)v)[2] =
                (float)((D_L00_0015F5CC_28a480g * 10) & 0x1FFFF) * (3.14159265f / 65536.0f) -
                3.14159265f;
            FUN_001fa070_28a480(D_L00_001BD7E0_lui, v);
            break;
        default:
            FUN_001f9fc8_28a480(D_L00_001BD7E0_lui);
            break;
        }
        sky_draw_shell(i);
    }
}
/* Ported from rac1-decomp (src/overlays/shared/shrubproc_0028A198.c: func_L00_0028B8F8), where it is exact; names translated to the US level program. */

void fun_001fa070(void *, void *) __asm__("FUN_001fa070");
extern short D_L00_0015F6B0_s __asm__("D_L00_0015F5CC");

void FUN_L00_0028a620(void) {
    enum {
        SKY_SHELL_SLOW_ROTATION = 1,
        SKY_SHELL_MEDIUM_ROTATION = 2,
        SKY_SHELL_FAST_ROTATION = 3
    };
    float v[4] __attribute__((aligned(16)));
    int i;
    *(short *)(D_L00_0016051C + 4) = 0;
    qzero(v);
    for (i = 0; i < *(short *)(D_L00_0016051C + 6); i++) {
        switch (i) {
        case SKY_SHELL_SLOW_ROTATION:
            v[2] = (D_L00_0015F5CC & 0x3FFFF) * (3.14159265f / 131072.0f) - 3.14159265f;
            fun_001fa070(D_L00_001BD7E0, v);
            break;
        case SKY_SHELL_MEDIUM_ROTATION:
            v[2] = (D_L00_0015F5CC & 0x1FFFF) * (3.14159265f / 65536.0f) - 3.14159265f;
            fun_001fa070(D_L00_001BD7E0, v);
            break;
        case SKY_SHELL_FAST_ROTATION:
            v[2] = *(unsigned short *)&D_L00_0015F6B0_s * (3.14159265f / 32768.0f) - 3.14159265f;
            fun_001fa070(D_L00_001BD7E0, v);
            break;
        default:
            set_identity_matrix(D_L00_001BD7E0);
            break;
        }
        sky_draw_shell(i);
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/shrubproc_0028A198.c: func_L00_0028BBF8), where it is exact; names translated to the US level program. */

extern char *D_L00_0016051C;
extern char D_L00_001BD7E0[];
extern void set_identity_matrix(void *) __asm__("FUN_001f9fc8");
extern void sky_sprite_proc(void) __asm__("FUN_0022bba0");
extern void vu1_add_g_sregister(s32 a0, s64 a1) __asm__("FUN_00233980");

void FUN_L00_0028a920(void) {
    set_identity_matrix(D_L00_001BD7E0);
    sky_draw_shell(0);
    sky_draw_shell(1);
    if (*(short *)(D_L00_0016051C + 8) == 0)
        sky_particles_init(0x78, 8);
    sky_particles_update();
    sky_sprite_proc();
    vu1_add_g_sregister(0x42, 0x8000000044L);
    sky_draw_shell(2);
}
void set_identity_matrix(void *) __asm__("FUN_001f9fc8");
void fun_001fa070(void *, void *) __asm__("FUN_001fa070");
void FUN_L00_0028a998(void) {
    float v[4] __attribute__((aligned(16)));
    set_identity_matrix(D_L00_001BD7E0);
    sky_draw_shell(0);
    qzero(v);
    v[2] = (D_L00_0015F5CC % 50000) * 0.000125663704f - 3.14159265f;
    fun_001fa070(D_L00_001BD7E0, v);
    sky_draw_shell(1);
    v[2] = (D_L00_0015F5CC % 100000) * 0.0000628318521f - 3.14159265f;
    fun_001fa070(D_L00_001BD7E0, v);
    sky_draw_shell(2);
}
extern char D_L00_001BD7E0[] __asm__("D_L00_001BD7E0");
extern u8 *D_L00_0016051C_0028aa98 __asm__("D_L00_0016051C");
void FUN_001f9fc8_0028aa98(void *) __asm__("FUN_001f9fc8");
void FUN_0022bba0_0028aa98(void) __asm__("FUN_0022bba0");
void FUN_00233980_0028aa98(s32, u64) __asm__("FUN_00233980");
void FUN_L00_0028aa98(void) {
    FUN_001f9fc8_0028aa98(D_L00_001BD7E0);
    sky_draw_shell(0);
    if (*(s16 *)(D_L00_0016051C_0028aa98 + 8) == 0)
        sky_particles_init(0x78, 8);
    sky_particles_update();
    FUN_0022bba0_0028aa98();
    FUN_00233980_0028aa98(0x42, 0x8000000044ULL);
    sky_draw_shell(1);
}
/* Hand-written from the US listing (closest relative FUN_L00_002895a0). */

void set_identity_matrix(void *) __asm__("FUN_001f9fc8");
void fun_001fa070(void *, void *) __asm__("FUN_001fa070");
void FUN_L00_0028b080(void) {
    float v[4] __attribute__((aligned(16)));
    *(short *)(D_L00_0016051C + 4) = 0;
    sky_draw_shell(0);
    sky_draw_shell(1);
    sky_draw_shell(2);
    sky_draw_shell(3);
    qzero(v);
    v[2] = (D_L00_0015F5CC % 40000) * 0.000157079637f - 3.14159265f;
    fun_001fa070(D_L00_001BD7E0, v);
    sky_draw_shell(4);
    v[2] = (D_L00_0015F5CC % 60000) * 0.000104719758f - 3.14159265f;
    fun_001fa070(D_L00_001BD7E0, v);
    sky_draw_shell(5);
    set_identity_matrix(D_L00_001BD7E0);
    sky_draw_shell(6);
}
typedef struct {
    short h0, h2, h4, h6;
    unsigned char pad[0x18];
    unsigned char *e[1];
} S_28b1a0;
extern S_28b1a0 *D_L00_0016051C_28b1a0 __asm__("D_L00_0016051C");
extern unsigned char D_L00_001BD7E0_28b1a0[] __asm__("D_L00_001BD7E0")
    __attribute__((section(".data")));
void FUN_001f9fc8_28b1a0(void *) __asm__("FUN_001f9fc8");
void FUN_001f99f8_28b1a0(void *) __asm__("FUN_001f99f8");
void FUN_001fa070_28b1a0(void *, void *) __asm__("FUN_001fa070");
void FUN_001f9a80_28b1a0(void *, void *, float) __asm__("FUN_001f9a80");

extern float D_L00_001604C0 __attribute__((sda));
extern OvlQuad D_L00_00160520 __attribute__((section(".sdata")));
void FUN_L00_0028b1a0(void) {
    enum {
        SKY_SHELL_TRIPLE_SCALE = 1,
        SKY_SHELL_TWO_AND_A_HALF_SCALE = 2,
        SKY_SHELL_DOUBLE_SCALE = 3,
        SKY_SHELL_ONE_AND_A_HALF_SCALE = 4,
        SKY_SHELL_ONE_AND_A_QUARTER_SCALE = 5
    };
    float v[4] __attribute__((aligned(16)));
    int i = 0;
    float *shell_rotation_phase;
    unsigned char *M;
    float k;
    D_L00_0016051C_28b1a0->h4 = 0;
    FUN_001f9fc8_28b1a0(D_L00_001BD7E0_28b1a0);
    FUN_001f99f8_28b1a0(v);
    for (; i < D_L00_0016051C_28b1a0->h6; i++) {
        shell_rotation_phase = (float *)(D_L00_0016051C_28b1a0->e[i] + 8);
        k = 1.0f;
        switch (i) {
        case SKY_SHELL_TRIPLE_SCALE:
            v[1] = 0.02f;
            k = 3.0f;
            *shell_rotation_phase += D_L00_001604C0 * 0.55f;
            break;
        case SKY_SHELL_TWO_AND_A_HALF_SCALE:
            v[1] = -0.02f;
            k = 2.5f;
            *shell_rotation_phase += D_L00_001604C0 * 0.6f;
            break;
        case SKY_SHELL_DOUBLE_SCALE:
            v[1] = 0.01f;
            k = 2.0f;
            *shell_rotation_phase += D_L00_001604C0 * 0.7f;
            break;
        case SKY_SHELL_ONE_AND_A_HALF_SCALE:
            v[1] = -0.01f;
            k = 1.5f;
            *shell_rotation_phase += D_L00_001604C0 * 0.75f;
            break;
        case SKY_SHELL_ONE_AND_A_QUARTER_SCALE:
            k = 1.25f;
            *shell_rotation_phase += D_L00_001604C0 * 0.8f;
            break;
        default:
            FUN_001f9fc8_28b1a0(D_L00_001BD7E0_28b1a0);
            break;
        }
        if (i > 0) {
            M = D_L00_001BD7E0_28b1a0;
            v[2] = (float)((int)(*shell_rotation_phase + i * 2000.0f) % 10000) * 0.00062831853f - 3.1415927f;
            FUN_001fa070_28b1a0(M, v);
            FUN_001f9a80_28b1a0(M, M, k);
            FUN_001f9a80_28b1a0(M + 0x10, M + 0x10, k);
            FUN_001f9a80_28b1a0(M + 0x20, M + 0x20, k);
            qcopy(M + 0x30, &D_L00_00160520);
        }
        sky_draw_shell(i);
    }
}
