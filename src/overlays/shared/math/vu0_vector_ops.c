/* VU0 vector helpers, the glint slot table and a twelve-byte clear. */
#include "types.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"

INCLUDE_ASM("config/us/overlays/asm/FUN_L00_001fd108.s", FUN_L00_001fd108);
INCLUDE_ASM("config/us/overlays/asm/FUN_L00_001fd1c8.s", FUN_L00_001fd1c8);
INCLUDE_ASM("config/us/overlays/asm/FUN_L00_001fde98.s", FUN_L00_001fde98);

#include "eetypes.h"
#include "qcopy.h"

extern char D_L00_0016EA40_001fe990[] __asm__("D_L00_0016EA40");
float       random_angle_radians() __asm__("FUN_00213308");
/* Fills the first free 0x20-byte glints slot with v, b and x; returns its index or -1. */
int allocate_glint_slot(u128 *input_payload, int b, float x) __asm__("FUN_L00_001fe990");

int allocate_glint_slot(u128 *input_payload, int moby_index, float x) {
    u128  input_copy;
    u128 *tp = &input_copy;
    char *slot;
    int   i;
    *tp = *input_payload;
    slot = D_L00_0016EA40_001fe990;
    for (i = 0; i < 16; i++, slot += 0x20) {
        if (*(short *)(slot + 0x10) <= 0) {
            qcopy(slot, tp);
            *(float *)(slot + 0x1c) = x;
            *(short *)(slot + 0x10) = 0x18;
            *(short *)(slot + 0x14) = moby_index;
            *(float *)(slot + 0x18) = random_angle_radians();
            return i;
        }
    }
    return -1;
}

/* Ported from rac1-decomp (src/overlays/shared/drawquad_001FD1D8.c: func_L00_001FE9C8), where it is exact; names translated to the US level program. */

typedef struct {
    char           pad[0x10];
    short          life;
    unsigned short ang;
    char           pad2[4];
    float          f;
    char           pad3[4];
} P;

extern P    glints[16] __asm__("D_L00_0016EA40");
extern void fill_transfer_words(u8 *, s32, s32) __asm__("func_001F97E8");

void update_glints(void) __asm__("FUN_L00_001fea18");

void update_glints(void) {
    int i;
    P  *glint_slot = glints;
    for (i = 15; i >= 0; i--, glint_slot++) {
        if (glint_slot->life > 0) {
            glint_slot->f = fast_subtract_rotations(glint_slot->f, frame_time * 5.2359877f);
            if (glint_slot->life > 12) {
                glint_slot->ang = glint_slot->ang + 5;
            } else {
                glint_slot->ang = glint_slot->ang - 5;
            }
            glint_slot->life = glint_slot->life - 1;
            if (glint_slot->life <= 0) {
                fill_transfer_words(glint_slot, 0, 0x20);
            }
        }
    }
}

INCLUDE_ASM("config/us/overlays/asm/FUN_L00_001fefc8.s", FUN_L00_001fefc8);
/* Ported from rac1-decomp (src/overlays/shared/fastfunc_001FEF78.c: func_L00_001FF200), where it is exact; names translated to the US level program. */

void clear_twelve_bytes(s64 *destination) __asm__("FUN_L00_001ff250");

void clear_twelve_bytes(s64 *destination) {
    destination[0] = 0;
    /* Fakematch: retail has a nop between the two stores that no C form gives. */
    __asm__ volatile("nop");
    ((s32 *)destination)[2] = 0;
}

INCLUDE_ASM("config/us/overlays/asm/FUN_L00_001ff290.s", FUN_L00_001ff290);
INCLUDE_ASM("config/us/overlays/asm/FUN_L00_001ff2c0.s", FUN_L00_001ff2c0);
INCLUDE_ASM("config/us/overlays/asm/FUN_L00_001ff318.s", FUN_L00_001ff318);
INCLUDE_ASM("config/us/overlays/asm/FUN_L00_001ff348.s", FUN_L00_001ff348);
INCLUDE_ASM("config/us/overlays/asm/FUN_L00_001ff378.s", FUN_L00_001ff378);
INCLUDE_ASM("config/us/overlays/asm/FUN_L00_001ff430.s", FUN_L00_001ff430);
INCLUDE_ASM("config/us/overlays/asm/FUN_L00_001ff600.s", FUN_L00_001ff600);
INCLUDE_ASM("config/us/overlays/asm/FUN_L00_001ff660.s", FUN_L00_001ff660);
INCLUDE_ASM("config/us/overlays/asm/FUN_L00_001ff768.s", FUN_L00_001ff768);
INCLUDE_ASM("config/us/overlays/asm/FUN_L00_001ffd48.s", FUN_L00_001ffd48);
INCLUDE_ASM("config/us/overlays/asm/FUN_L00_001ffe20.s", FUN_L00_001ffe20);
INCLUDE_ASM("config/us/overlays/asm/FUN_L00_001fff28.s", FUN_L00_001fff28);

INCLUDE_ASM("config/us/overlays/asm/FUN_L00_001ff9c0.s", FUN_L00_001ff9c0);

f32 D_L01_0016EED8[1] __attribute__((section(".data"))) = {0};

f32 D_L02_0016F058[1] __attribute__((section(".data"))) = {0};

f32 D_L03_0016EB58[1] __attribute__((section(".data"))) = {0};

f32 D_L04_0016EBD8[1] __attribute__((section(".data"))) = {0};

f32 D_L05_0016EE58[1] __attribute__((section(".data"))) = {0};

f32 D_L06_0016F1D8[1] __attribute__((section(".data"))) = {0};

f32 D_L07_0016EAD8[1] __attribute__((section(".data"))) = {0};

f32 D_L08_0016F258[1] __attribute__((section(".data"))) = {0};

f32 D_L09_0016EBD8[1] __attribute__((section(".data"))) = {0};

f32 D_L10_0016EED8[1] __attribute__((section(".data"))) = {0};

f32 D_L12_0016EED8[1] __attribute__((section(".data"))) = {0};

f32 D_L13_0016ED58[1] __attribute__((section(".data"))) = {0};

f32 D_L14_0016F158[1] __attribute__((section(".data"))) = {0};

f32 D_L15_0016F058[1] __attribute__((section(".data"))) = {0};

f32 D_L16_0016EE58[1] __attribute__((section(".data"))) = {0};

f32 D_L17_0016F358[1] __attribute__((section(".data"))) = {0};

P D_L01_0016EEC0[16] = {0};

P D_L02_0016F040[16] = {0};

P D_L03_0016EB40[16] = {0};

P D_L04_0016EBC0[16] = {0};

P D_L05_0016EE40[16] = {0};

P D_L06_0016F1C0[16] = {0};

P D_L07_0016EAC0[16] = {0};

P D_L08_0016F240[16] = {0};

P D_L09_0016EBC0[16] = {0};

P D_L10_0016EEC0[16] = {0};

P D_L11_0016F440[16] = {0};

P D_L12_0016EEC0[16] = {0};

P D_L13_0016ED40[16] = {0};

P D_L14_0016F140[16] = {0};

P D_L15_0016F040[16] = {0};

P D_L16_0016EE40[16] = {0};

P D_L17_0016F340[16] = {0};

P D_L18_0016F440[16] = {0};
