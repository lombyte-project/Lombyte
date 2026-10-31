/* Shared moby class trespasser: the trespasser held by the hero, and its pickup. */

#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/collision.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/data_labels.h"

extern void update_trespasser_pickup(void *) __asm__("FUN_L00_002c1778");
void        FUN_00213508_2c1778(void *, int, float) __asm__("FUN_00213508");
float       FUN_001fa580_2c1778(float, float) __asm__("FUN_001fa580");
float       FUN_001f9b80_2c1778(void *, void *) __asm__("FUN_001f9b80");
float       FUN_001f99c0_2c1778(float) __asm__("FUN_001f99c0");
float       approach_value(void *, float, float) __asm__("FUN_00213ed8");

extern unsigned char D_0013A4E0[];
extern int           D_L00_0015F5CC_2c1778[1] __asm__("D_L00_0015F5CC");
extern const float   l00_trespasser_pickup_initial_height_offset __asm__("D_L00_00161828")
    __attribute__((sda));
float trespasser_pickup_z_rotation_step_gp __asm__("D_L00_00161830") = 0;

float D_L01_00161858 = 0.2f;
float D_L01_00161860 = 0;
float D_L02_001617F8 = 0.2f;
float D_L02_00161800 = 0;
float D_L03_00161808 = 0.2f;
float D_L03_00161810 = 0;
float D_L04_001617F8 = 0.2f;
float D_L04_00161800 = 0;
float D_L05_00161848 = 0.2f;
float D_L05_00161850 = 0;
float D_L06_00161828 = 0.2f;
float D_L06_00161830 = 0;
float D_L07_001617F8 = 0.2f;
float D_L07_00161800 = 0;
float D_L08_00161848 = 0.2f;
float D_L08_00161850 = 0;
float D_L09_001618B8 = 0.2f;
float D_L09_001618C0 = 0;
float D_L10_00161808 = 0.2f;
float D_L10_00161810 = 0;
float D_L11_00161888 = 0.2f;
float D_L11_00161890 = 0;
float D_L12_00161808 = 0.2f;
float D_L12_00161810 = 0;
float D_L13_001618D8 = 0.2f;
float D_L13_001618E0 = 0;
float D_L14_00161A18 = 0.2f;
float D_L14_00161A20 = 0;
float D_L15_001619B8 = 0.2f;
float D_L15_001619C0 = 0;
float D_L16_001618B8 = 0.2f;
float D_L16_001618C0 = 0;
float D_L17_00161908 = 0.2f;
float D_L17_00161910 = 0;
float D_L18_00161898 = 0.2f;
float D_L18_001618A0 = 0;

/* Runs FUN_L00_002c1778 when the moby data word 4 is set, else sets byte 0xBC from a mask test. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C12B0.c: func_L00_002C2A20), where it is exact; names translated to the US level program. */
void trespasser_update(struct Moby *m) __asm__("FUN_L00_002c1718");

/* Moby class 188 */
void trespasser_update(struct Moby *trespasser_moby) {
    if (*(int *)(((char *)trespasser_moby->pvars) + 4) != 0) {
        update_trespasser_pickup(trespasser_moby);
    } else if (*(int *)(D_0013A4E0 + 0x2600) & *(int *)(((unsigned char *)&D_0013E533) + 0x1EBD)) {
        trespasser_moby->command = 1;
    } else {
        trespasser_moby->command = 0;
    }
}

void update_trespasser_pickup(void *mv) {
    enum {
        TRESPASSER_INITIALIZE = 0,
        TRESPASSER_WAIT_FOR_HERO_PROXIMITY = 1,
        TRESPASSER_SHRINK_FOR_REMOVAL = 2
    };
    unsigned char *trespasser_moby = mv;
    struct Hero   *P;
    switch (trespasser_moby[0x20]) {
    case TRESPASSER_INITIALIZE:
        FUN_00213508_2c1778(trespasser_moby + 0x10, 0, 0.5f);
        *(float *)(trespasser_moby + 0x40) = 1.5707964f;
        *(float *)(trespasser_moby + 0x44) = 3.1415927f;
        *(float *)(trespasser_moby + 0x18) =
            collision_hit_z + l00_trespasser_pickup_initial_height_offset;
        trespasser_moby[0x20] = 1;
        break;
    case TRESPASSER_WAIT_FOR_HERO_PROXIMITY: {
        float updated_z_rotation = FUN_001fa580_2c1778(*(float *)(trespasser_moby + 0x48),
                                                       trespasser_pickup_z_rotation_step_gp);
        int   proximity_check_counter = D_L00_0015F5CC_2c1778[0];
        *(float *)(trespasser_moby + 0x48) = updated_z_rotation;
        if (proximity_check_counter % 10 == 0) {
            P = &hero;
            if (P->health.hp) {
                if (FUN_001f9b80_2c1778(trespasser_moby + 0x10, &P->motion.pos) < 1.0f) {
                    if (FUN_001f99c0_2c1778(*(float *)(trespasser_moby + 0x18) -
                                            P->motion.pos.f[2]) < 2.0f) {
                        grant_inventory_item(0x1A, 1);
                        trespasser_moby[0x20] = 2;
                    }
                }
            }
        }
    } break;
    case TRESPASSER_SHRINK_FOR_REMOVAL:
        approach_value(trespasser_moby + 0x2C,
                       0.0f,
                       *(float *)(*(char **)(trespasser_moby + 0x24) + 0x24) * 0.02f);
        if (*(float *)(trespasser_moby + 0x2C) == 0.0f)
            mark_moby_for_removal(trespasser_moby);
        break;
    }
}
