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
#include "rnc/math/asm_helpers.h"
#include "rnc/rendering/vu1_registers.h"
#include "rnc/rendering/sky/sky_sprite_proc.h"

extern char  D_L00_001BD7E0[];
extern char *D_L00_0016051C;
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

extern char D_L00_001BD7E0[] NOT_SDA;
extern int  D_L00_0015F5CC;

void update_sky_shells_animated(void) __asm__("FUN_L00_00289420");

void update_sky_shells_animated(void) {
    enum {
        SKY_SHELL_SLOW_ROTATION = 2,
        SKY_SHELL_MEDIUM_ROTATION = 3,
        SKY_SHELL_FAST_ROTATION = 4
    };
    float animated_sky_shell_rotation_angles[4] __attribute__((aligned(16)));
    int   i;
    *(short *)(D_L00_0016051C + 4) = 0;
    qzero(animated_sky_shell_rotation_angles);
    for (i = 0; i < *(short *)(D_L00_0016051C + 6); i++) {
        switch (i) {
        case SKY_SHELL_SLOW_ROTATION:
            animated_sky_shell_rotation_angles[2] =
                (D_L00_0015F5CC & 0x3ffff) * 0.000023968450f - 3.14159265f;
            euler_to_matrix_ee(D_L00_001BD7E0, animated_sky_shell_rotation_angles);
            break;
        case SKY_SHELL_MEDIUM_ROTATION:
            animated_sky_shell_rotation_angles[2] =
                (D_L00_0015F5CC & 0x1ffff) * 0.000047936900f - 3.14159265f;
            euler_to_matrix_ee(D_L00_001BD7E0, animated_sky_shell_rotation_angles);
            break;
        case SKY_SHELL_FAST_ROTATION:
            animated_sky_shell_rotation_angles[2] =
                (unsigned short)D_L00_0015F5CC * 0.000095873800f - 3.14159265f;
            euler_to_matrix_ee(D_L00_001BD7E0, animated_sky_shell_rotation_angles);
            break;
        default:
            set_identity_matrix(D_L00_001BD7E0);
        }
        sky_draw_shell(i);
    }
}

/* D_L00_0015F5CC is read as u16 for the first sky rotation and masked with 0x1FFFF for the second. */

typedef struct {
    f32 f[4];
} __attribute__((aligned(16))) SkyRotVec_2899a8;

extern char *D_L00_0016051C_2899a8 __asm__("D_L00_0016051C");
extern s32   D_L00_0015F5CC_2899a8 __asm__("D_L00_0015F5CC");

void sky_shells_rotate_two(void) __asm__("FUN_L00_002899a8");

void sky_shells_rotate_two(void) {
    SkyRotVec_2899a8 shell_rotation;
    *(short *)(D_L00_0016051C_2899a8 + 4) = 0;
    set_identity_matrix(D_L00_001BD7E0);
    sky_draw_shell(0);
    qzero(&shell_rotation);
    shell_rotation.f[2] = (f32) * (u16 *)&D_L00_0015F5CC_2899a8 * 9.5873802e-5f - 3.1415927f;
    euler_to_matrix_ee(D_L00_001BD7E0, &shell_rotation);
    sky_draw_shell(1);
    shell_rotation.f[2] =
        (f32)(*(volatile int *)&D_L00_0015F5CC_2899a8 & 0x1FFFF) * 4.7936901e-5f - 3.1415927f;
    euler_to_matrix_ee(D_L00_001BD7E0, &shell_rotation);
    sky_draw_shell(2);
}
/* draws sky shells with two animated rotation offsets */
/* Ported from rac1-decomp (src/overlays/shared/shrubproc_0028A198.c: func_L00_0028AD68), where it is exact; names translated to the US level program. */

void draw_sky_shells_with_fast_and_slow_rotation(void) __asm__("FUN_L00_00289a90");

void draw_sky_shells_with_fast_and_slow_rotation(void) {
    enum {
        SKY_SHELL_FAST_ROTATION = 1,
        SKY_SHELL_SLOW_ROTATION = 2
    };
    float shell_rotation_angles[4] __attribute__((aligned(16)));
    int   i;
    *(short *)(D_L00_0016051C + 4) = 0;
    qzero(shell_rotation_angles);
    for (i = 0; i < *(short *)(D_L00_0016051C + 6); ++i) {
        switch (i) {
        case SKY_SHELL_FAST_ROTATION:
            shell_rotation_angles[2] =
                (float)*(unsigned short *)&D_L00_0015F5CC * 0.0000958738017f - 3.14159265f;
            euler_to_matrix_ee(D_L00_001BD7E0, shell_rotation_angles);
            break;
        case SKY_SHELL_SLOW_ROTATION:
            shell_rotation_angles[2] =
                (float)(D_L00_0015F5CC & 0x1FFFF) * 0.00004793690085f - 3.14159265f;
            euler_to_matrix_ee(D_L00_001BD7E0, shell_rotation_angles);
            break;
        default:
            set_identity_matrix(D_L00_001BD7E0);
            break;
        }
        sky_draw_shell(i);
    }
}

typedef union {
    OvlQuad q;
    float   f[4];
} U289bd8;

void FUN_00233980(int, long);
void draw_sky_shells_with_particles_and_modulo_50000_rotation(void) __asm__("FUN_L00_00289bd8");

void draw_sky_shells_with_particles_and_modulo_50000_rotation(void) {
    float shell_rotation_angles[4] __attribute__((aligned(16)));
    set_identity_matrix(D_L00_001BD7E0);
    sky_draw_shell(0);
    if (*(short *)(D_L00_0016051C + 8) == 0)
        sky_particles_init(0x78, 8);
    sky_particles_update();
    sky_sprite_proc();
    FUN_00233980(0x42, (0x8000L << 24) | 0x44);
    qzero(shell_rotation_angles);
    shell_rotation_angles[2] = (D_L00_0015F5CC % 50000) * 0.000125663704f - 3.14159265f;
    euler_to_matrix_ee(D_L00_001BD7E0, shell_rotation_angles);
    sky_draw_shell(1);
}

void draw_three_sky_shells_with_particles_and_modulo_50000_rotation(void) __asm__("FUN_L00_0028a208");

void draw_three_sky_shells_with_particles_and_modulo_50000_rotation(void) {
    float third_shell_rotation_angles[4] __attribute__((aligned(16)));
    set_identity_matrix(D_L00_001BD7E0);
    sky_draw_shell(0);
    if (*(short *)(D_L00_0016051C + 8) == 0)
        sky_particles_init(0x78, 8);
    sky_particles_update();
    sky_sprite_proc();
    FUN_00233980(0x42, (0x8000L << 24) | 0x44);
    sky_draw_shell(1);
    qzero(third_shell_rotation_angles);
    third_shell_rotation_angles[2] = (D_L00_0015F5CC % 50000) * 0.000125663704f - 3.14159265f;
    euler_to_matrix_ee(D_L00_001BD7E0, third_shell_rotation_angles);
    sky_draw_shell(2);
}

typedef union {
    OvlQuad q;
    f32     f[4];
} V0028a2f0;

extern u8 *D_L00_0016051C_c2 __asm__("D_L00_0016051C");
extern s32 D_L00_0015F5CC_c __asm__("D_L00_0015F5CC");
void       FUN_L00_001ffa90(void *, void *);
void       draw_sky_shells_with_modulo_40000_rotation(void) __asm__("FUN_L00_0028a2f0");

void draw_sky_shells_with_modulo_40000_rotation(void) {
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

void draw_sky_shells_with_modulo_25000_rotation(void) __asm__("FUN_L00_0028a3a8");

void draw_sky_shells_with_modulo_25000_rotation(void) {
    float shell_rotation[3];
    *(short *)(D_L00_0016051C + 4) = 0;
    set_identity_matrix(D_L00_001BD7E0);
    sky_draw_shell(0);
    sky_draw_shell(1);
    sky_draw_shell(2);
    shell_rotation[0] = 0.0f;
    shell_rotation[1] = 0.3f;
    shell_rotation[2] = (D_L00_0015F5CC % 25000) * 0.000251327408f - 3.14159265f;
    euler_to_matrix_ee(D_L00_001BD7E0, shell_rotation);
    sky_draw_shell(3);
    set_identity_matrix(D_L00_001BD7E0);
    sky_draw_shell(4);
}

extern int   D_L00_0015F5CC_28a480g __asm__("D_L00_0015F5CC");
extern int   D_L00_0015F5CC_28a480d __asm__("D_L00_0015F5CC");
extern char *D_L00_0016051C_28a480 __asm__("D_L00_0016051C");
extern char  D_L00_001BD7E0_lui[] __asm__("D_L00_001BD7E0") NOT_SDA;

void draw_sky_shells_with_scaled_phase_rotation(void) __asm__("FUN_L00_0028a480");

void draw_sky_shells_with_scaled_phase_rotation(void) {
    enum {
        SKY_SHELL_DOUBLE_PHASE_ROTATION = 0,
        SKY_SHELL_QUINTUPLE_PHASE_ROTATION = 1,
        SKY_SHELL_TENFOLD_PHASE_WIDE_WRAP_ROTATION = 2,
        SKY_SHELL_TENFOLD_PHASE_ROTATION = 3
    };
    OvlQuad shell_rotation[1];
    int     i;

    qzero(shell_rotation);
    for (i = 0; i < *(short *)(D_L00_0016051C_28a480 + 6); i++) {
        switch (i) {
        case SKY_SHELL_DOUBLE_PHASE_ROTATION:
            ((float *)shell_rotation)[2] =
                (float)((D_L00_0015F5CC_28a480g * 2) & 0x1FFFF) * (3.14159265f / 65536.0f) -
                3.14159265f;
            euler_to_matrix_ee(D_L00_001BD7E0_lui, shell_rotation);
            break;
        case SKY_SHELL_QUINTUPLE_PHASE_ROTATION:
            ((float *)shell_rotation)[2] =
                (float)((D_L00_0015F5CC_28a480d * 5) & 0x1FFFF) * (3.14159265f / 65536.0f) -
                3.14159265f;
            euler_to_matrix_ee(D_L00_001BD7E0_lui, shell_rotation);
            break;
        case SKY_SHELL_TENFOLD_PHASE_WIDE_WRAP_ROTATION:
            ((float *)shell_rotation)[2] =
                (float)((D_L00_0015F5CC_28a480d * 10) & 0x3FFFF) * 2.39684496e-05f - 3.14159265f;
            euler_to_matrix_ee(D_L00_001BD7E0_lui, shell_rotation);
            break;
        case SKY_SHELL_TENFOLD_PHASE_ROTATION:
            ((float *)shell_rotation)[2] =
                (float)((D_L00_0015F5CC_28a480g * 10) & 0x1FFFF) * (3.14159265f / 65536.0f) -
                3.14159265f;
            euler_to_matrix_ee(D_L00_001BD7E0_lui, shell_rotation);
            break;
        default:
            set_identity_matrix(D_L00_001BD7E0_lui);
            break;
        }
        sky_draw_shell(i);
    }
}
/* Ported from rac1-decomp (src/overlays/shared/shrubproc_0028A198.c: func_L00_0028B8F8), where it is exact; names translated to the US level program. */

extern short D_L00_0015F6B0_s __asm__("D_L00_0015F5CC");

void draw_sky_shells_with_slow_medium_and_fast_rotation(void) __asm__("FUN_L00_0028a620");

void draw_sky_shells_with_slow_medium_and_fast_rotation(void) {
    enum {
        SKY_SHELL_SLOW_ROTATION = 1,
        SKY_SHELL_MEDIUM_ROTATION = 2,
        SKY_SHELL_FAST_ROTATION = 3
    };
    float sky_shell_layer_rotation_angles[4] __attribute__((aligned(16)));
    int   i;
    *(short *)(D_L00_0016051C + 4) = 0;
    qzero(sky_shell_layer_rotation_angles);
    for (i = 0; i < *(short *)(D_L00_0016051C + 6); i++) {
        switch (i) {
        case SKY_SHELL_SLOW_ROTATION:
            sky_shell_layer_rotation_angles[2] =
                (D_L00_0015F5CC & 0x3FFFF) * (3.14159265f / 131072.0f) - 3.14159265f;
            euler_to_matrix_ee(D_L00_001BD7E0, sky_shell_layer_rotation_angles);
            break;
        case SKY_SHELL_MEDIUM_ROTATION:
            sky_shell_layer_rotation_angles[2] =
                (D_L00_0015F5CC & 0x1FFFF) * (3.14159265f / 65536.0f) - 3.14159265f;
            euler_to_matrix_ee(D_L00_001BD7E0, sky_shell_layer_rotation_angles);
            break;
        case SKY_SHELL_FAST_ROTATION:
            sky_shell_layer_rotation_angles[2] =
                *(unsigned short *)&D_L00_0015F6B0_s * (3.14159265f / 32768.0f) - 3.14159265f;
            euler_to_matrix_ee(D_L00_001BD7E0, sky_shell_layer_rotation_angles);
            break;
        default:
            set_identity_matrix(D_L00_001BD7E0);
            break;
        }
        sky_draw_shell(i);
    }
}

/* Ported from rac1-decomp (src/overlays/shared/shrubproc_0028A198.c: func_L00_0028BBF8), where it is exact; names translated to the US level program. */

void draw_three_sky_shells_with_particles(void) __asm__("FUN_L00_0028a920");

void draw_three_sky_shells_with_particles(void) {
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

void draw_three_sky_shells_with_modulo_50000_and_100000_rotation(void) __asm__("FUN_L00_0028a998");

void draw_three_sky_shells_with_modulo_50000_and_100000_rotation(void) {
    float sky_shell_rotation_angles[4] __attribute__((aligned(16)));
    set_identity_matrix(D_L00_001BD7E0);
    sky_draw_shell(0);
    qzero(sky_shell_rotation_angles);
    sky_shell_rotation_angles[2] = (D_L00_0015F5CC % 50000) * 0.000125663704f - 3.14159265f;
    euler_to_matrix_ee(D_L00_001BD7E0, sky_shell_rotation_angles);
    sky_draw_shell(1);
    sky_shell_rotation_angles[2] = (D_L00_0015F5CC % 100000) * 0.0000628318521f - 3.14159265f;
    euler_to_matrix_ee(D_L00_001BD7E0, sky_shell_rotation_angles);
    sky_draw_shell(2);
}

extern u8 *D_L00_0016051C_0028aa98 __asm__("D_L00_0016051C");
void       draw_two_sky_shells_with_particles(void) __asm__("FUN_L00_0028aa98");

void draw_two_sky_shells_with_particles(void) {
    set_identity_matrix(D_L00_001BD7E0);
    sky_draw_shell(0);
    if (*(s16 *)(D_L00_0016051C_0028aa98 + 8) == 0)
        sky_particles_init(0x78, 8);
    sky_particles_update();
    sky_sprite_proc();
    vu1_add_g_sregister(0x42, 0x8000000044ULL);
    sky_draw_shell(1);
}
/* Hand-written from the US listing (closest relative FUN_L00_002895a0). */

void draw_seven_sky_shells_with_modulo_40000_and_60000_rotation(void) __asm__("FUN_L00_0028b080");

void draw_seven_sky_shells_with_modulo_40000_and_60000_rotation(void) {
    float animated_shell_rotation_angles[4] __attribute__((aligned(16)));
    *(short *)(D_L00_0016051C + 4) = 0;
    sky_draw_shell(0);
    sky_draw_shell(1);
    sky_draw_shell(2);
    sky_draw_shell(3);
    qzero(animated_shell_rotation_angles);
    animated_shell_rotation_angles[2] = (D_L00_0015F5CC % 40000) * 0.000157079637f - 3.14159265f;
    euler_to_matrix_ee(D_L00_001BD7E0, animated_shell_rotation_angles);
    sky_draw_shell(4);
    animated_shell_rotation_angles[2] = (D_L00_0015F5CC % 60000) * 0.000104719758f - 3.14159265f;
    euler_to_matrix_ee(D_L00_001BD7E0, animated_shell_rotation_angles);
    sky_draw_shell(5);
    set_identity_matrix(D_L00_001BD7E0);
    sky_draw_shell(6);
}

typedef struct {
    short          h0, h2, h4, h6;
    unsigned char  pad[0x18];
    unsigned char *e[1];
} S_28b1a0;

extern S_28b1a0     *D_L00_0016051C_28b1a0 __asm__("D_L00_0016051C");
extern unsigned char D_L00_001BD7E0_28b1a0[] __asm__("D_L00_001BD7E0")
    __attribute__((section(".data")));
void scale_vector(void *, void *, float) __asm__("FUN_001f9a80");

#include "rnc/overlay/shared/rendering/sky/update_sky_shells_animated_globals.h"

extern float sky_shell_rotation_phase_step_gp __asm__("D_L00_001604C0") __attribute__((sda));
extern OvlQuad D_L00_00160520 MACRO_ADDR;
void draw_sky_shells_with_rotation_and_scaling(void) __asm__("FUN_L00_0028b1a0");

void draw_sky_shells_with_rotation_and_scaling(void) {
    enum {
        SKY_SHELL_TRIPLE_SCALE = 1,
        SKY_SHELL_TWO_AND_A_HALF_SCALE = 2,
        SKY_SHELL_DOUBLE_SCALE = 3,
        SKY_SHELL_ONE_AND_A_HALF_SCALE = 4,
        SKY_SHELL_ONE_AND_A_QUARTER_SCALE = 5
    };
    float          sky_shell_transform_rotation_angles[4] __attribute__((aligned(16)));
    int            i = 0;
    float         *shell_rotation_phase;
    unsigned char *M;
    float          k;
    D_L00_0016051C_28b1a0->h4 = 0;
    set_identity_matrix(D_L00_001BD7E0_28b1a0);
    clear_u64_value(sky_shell_transform_rotation_angles);
    for (; i < D_L00_0016051C_28b1a0->h6; i++) {
        shell_rotation_phase = (float *)(D_L00_0016051C_28b1a0->e[i] + 8);
        k = 1.0f;
        switch (i) {
        case SKY_SHELL_TRIPLE_SCALE:
            sky_shell_transform_rotation_angles[1] = 0.02f;
            k = 3.0f;
            *shell_rotation_phase += sky_shell_rotation_phase_step_gp * 0.55f;
            break;
        case SKY_SHELL_TWO_AND_A_HALF_SCALE:
            sky_shell_transform_rotation_angles[1] = -0.02f;
            k = 2.5f;
            *shell_rotation_phase += sky_shell_rotation_phase_step_gp * 0.6f;
            break;
        case SKY_SHELL_DOUBLE_SCALE:
            sky_shell_transform_rotation_angles[1] = 0.01f;
            k = 2.0f;
            *shell_rotation_phase += sky_shell_rotation_phase_step_gp * 0.7f;
            break;
        case SKY_SHELL_ONE_AND_A_HALF_SCALE:
            sky_shell_transform_rotation_angles[1] = -0.01f;
            k = 1.5f;
            *shell_rotation_phase += sky_shell_rotation_phase_step_gp * 0.75f;
            break;
        case SKY_SHELL_ONE_AND_A_QUARTER_SCALE:
            k = 1.25f;
            *shell_rotation_phase += sky_shell_rotation_phase_step_gp * 0.8f;
            break;
        default:
            set_identity_matrix(D_L00_001BD7E0_28b1a0);
            break;
        }
        if (i > 0) {
            M = D_L00_001BD7E0_28b1a0;
            sky_shell_transform_rotation_angles[2] =
                (float)((int)(*shell_rotation_phase + i * 2000.0f) % 10000) * 0.00062831853f -
                3.1415927f;
            euler_to_matrix_ee(M, sky_shell_transform_rotation_angles);
            scale_vector(M, M, k);
            scale_vector(M + 0x10, M + 0x10, k);
            scale_vector(M + 0x20, M + 0x20, k);
            qcopy(M + 0x30, &D_L00_00160520);
        }
        sky_draw_shell(i);
    }
}

float D_L01_001604C0 = 0;
float D_L02_001604C0 = 0;
float D_L03_001604C0 = 0;
float D_L04_001604C0 = 0;
float D_L05_001604C0 = 0;
float D_L06_001604C0 = 0;
float D_L07_001604C0 = 0;
float D_L08_001604C0 = 0;
float D_L09_001604C0 = 0;
float D_L10_001604C0 = 0;
float D_L11_001604C0 = 0;
float D_L12_001604C0 = 0;
float D_L13_001604C0 = 0;
float D_L14_001604C0 = 0;
float D_L15_001604C0 = 0;
float D_L16_001604C0 = 0;
float D_L17_001604C0 = 0;
float D_L18_001604C0 = 0;
