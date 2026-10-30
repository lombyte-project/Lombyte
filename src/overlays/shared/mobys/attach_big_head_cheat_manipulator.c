/* Big head cheat: three copies of the helper that attaches the head-scaling manipulator to every moby of the given moby's class while the level mode is 2. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/mobys/attach_manipulator.h"
#include "qcopy.h"
#include "rnc/game/options.h"
#include "rnc/overlay/level_labels.h"

typedef struct {
    char  pad0[0x44];
    short count;
    char  pad46[0x132];
    char *mobys[1];
} MobyList;

extern MobyList      D_L05_0016CC60;
extern int           D_L05_0015F5C4;
extern unsigned char D_L05_0017C8A0[];
extern short         big_head_manipulator_scale_gp __asm__("D_L05_00161EE8") __attribute__((sda));

void attach_big_head_cheat_manipulator(char *moby) __asm__("FUN_L05_00317398");

void attach_big_head_cheat_manipulator(char *moby) {
    int i;

    if (D_L05_0015F5C4 == 2 && big_head_cheat_enabled != 0) {
        for (i = 0; i < D_L05_0016CC60.count; i++) {
            char *candidate_moby = D_L05_0016CC60.mobys[i];

            if (MOBY(candidate_moby)->oclass == MOBY(moby)->oclass) {
                unsigned char *manipulator = D_L05_0017C8A0;

                if (manipulator[1] == 0) {
                    attach_manipulator(candidate_moby, 0, manipulator);
                    *(float *)(manipulator + 0x20) = *(float *)&big_head_manipulator_scale_gp;
                    *(float *)(manipulator + 0x24) = *(float *)&big_head_manipulator_scale_gp;
                    *(float *)(manipulator + 0x28) = *(float *)&big_head_manipulator_scale_gp;
                }
            }
        }
    }
}

extern unsigned char l02_big_head_cheat_manipulator[] __asm__("D_L02_0017CA40");

extern MobyList talk_camera __asm__("D_L02_0016CE60");

/* The same helper with the scale 2.75 as an immediate. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002E2110), where it is exact; names translated to the US level program. */

void attach_big_head_cheat_manipulator_to_matching_moby(char *moby) __asm__("FUN_L02_002e0cd8");

void attach_big_head_cheat_manipulator_to_matching_moby(char *moby) {
    int i;
    if (level_game_mode == 2 && big_head_cheat_enabled != 0) {
        for (i = 0; i < talk_camera.count; i++) {
            char *candidate_moby = talk_camera.mobys[i];
            if (MOBY(candidate_moby)->oclass == MOBY(moby)->oclass) {
                unsigned char *manipulator = l02_big_head_cheat_manipulator;
                if (manipulator[1] == 0) {
                    attach_manipulator(candidate_moby, 0, manipulator);
                    *(float *)(manipulator + 0x20) = 2.75f;
                    *(float *)(manipulator + 0x24) = 2.75f;
                    *(float *)(manipulator + 0x28) = 2.75f;
                }
            }
        }
    }
}

extern MobyList      D_L03_0016C960;
extern int           D_L03_0015F5C4;
extern unsigned char D_L03_0017C540[];
extern short shared_manipulator_attachment_scale __asm__("D_L03_00161BC4") __attribute__((sda));

/* The same helper on manipulator slot 1. */
void attach_shared_manipulator_to_matching_class_moby(char *moby) __asm__("FUN_L03_002db480");

void attach_shared_manipulator_to_matching_class_moby(char *moby) {
    int i;

    if (D_L03_0015F5C4 == 2 && big_head_cheat_enabled != 0) {
        for (i = 0; i < D_L03_0016C960.count; i++) {
            char *candidate_moby = D_L03_0016C960.mobys[i];

            if (MOBY(candidate_moby)->oclass == MOBY(moby)->oclass) {
                unsigned char *manipulator = D_L03_0017C540;

                if (manipulator[1] == 0) {
                    attach_manipulator(candidate_moby, 1, manipulator);
                    *(float *)(manipulator + 0x20) = *(float *)&shared_manipulator_attachment_scale;
                    *(float *)(manipulator + 0x24) = *(float *)&shared_manipulator_attachment_scale;
                    *(float *)(manipulator + 0x28) = *(float *)&shared_manipulator_attachment_scale;
                }
            }
        }
    }
}

short D_L11_001613C0 = 0x6666;

f32 D_L04_0017C5E0[1] __attribute__((section(".data"))) = {0};

f32 D_L04_0017C5E4[1] __attribute__((section(".data"))) = {0};

f32 D_L14_0017CB60[1] __attribute__((section(".data"))) = {0};

f32 D_L14_0017CB64[1] __attribute__((section(".data"))) = {0};

f32 D_L06_0017CBE0[1] __attribute__((section(".data"))) = {0};

f32 D_L06_0017CBE4[1] __attribute__((section(".data"))) = {0};

short D_L06_00162094 = 0x3333;
short D_L12_001619A4 = 0x3333;
