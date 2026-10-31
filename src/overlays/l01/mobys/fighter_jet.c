/* Level 01 moby class fighter_jet. */
#include "types.h"
#include "rnc/game/item_state.h"
#include "rnc/overlay/level_labels.h"

#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/vehicles/gunship.h"

extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");

extern void spawn_explosion(void *,
                            void *,
                            void *,
                            float,
                            float,
                            int,
                            int,
                            int,
                            float,
                            float,
                            float,
                            float,
                            int,
                            float,
                            int,
                            int,
                            int,
                            int) __asm__("FUN_L00_0025e450");

#include "eetypes.h"

/* Ported from rac1-decomp (src/overlays/l01_novalis/vendor_002BA898.c: func_L01_002F5800), where it is exact; names translated to the US level program. */

extern char  D_L01_00167240[];
extern short fighter_jet_minimum_distance_scale_gp __asm__("D_L01_00161AF4") __attribute__((sda));
extern short fighter_jet_minimum_scale_camera_distance_gp __asm__("D_L01_00161AF8")
    __attribute__((sda));
extern short fighter_jet_full_scale_camera_distance_gp __asm__("D_L01_00161AFC")
    __attribute__((sda));
extern void spawn_explosion(void *,
                            void *,
                            void *,
                            float,
                            float,
                            int,
                            int,
                            int,
                            float,
                            float,
                            float,
                            float,
                            int,
                            float,
                            int,
                            int,
                            int,
                            int) __asm__("FUN_L00_0025e450");
extern void FUN_L00_00263d40(int, int);
extern void FUN_L01_0030be70(char *, void *, float);
extern void allocate_voice_for_bank_entry(int, int, int) __asm__("FUN_0022db10");

/* Per-frame update of a breakable moby: scales its speed by distance, and on a hit spawns the break effect, frees its child mobys and removes itself; otherwise fades its alpha by height. */
void fighter_jet_update(char *moby) __asm__("FUN_L01_002f4428");

/* Moby class 660 */
void fighter_jet_update(char *moby) {
    float hit_dir[4];
    float hit_vec[4];
    char *fighter_jet_vars = *(char **)(moby + 0x78);
    void *hit = find_moby_hit(moby, 0x800000, 0);
    float speed_scale;
    ((unsigned char *)moby)[0xA4] = 0xFF;
    if (((unsigned char *)moby)[0x20] == 0) {
        *(float *)(fighter_jet_vars + 0x170) = *(float *)(moby + 0x2C);
        *(float *)(fighter_jet_vars + 0x174) = *(float *)(fighter_jet_vars + 0xFC);
    }
    {
        float camera_distance = fast_vec_dist(moby + 0x10, D_L01_00167240);
        float min_scale = *(float *)&fighter_jet_minimum_distance_scale_gp;
        float near_dist = *(float *)&fighter_jet_full_scale_camera_distance_gp;
        float slope = -((1.0f - min_scale) /
                        (*(float *)&fighter_jet_minimum_scale_camera_distance_gp - near_dist));
        speed_scale = slope * camera_distance + (1.0f - slope * near_dist);
        if (1.0f < speed_scale)
            speed_scale = 1.0f;
        if (speed_scale < min_scale)
            speed_scale = min_scale;
    }
    *(float *)(moby + 0x2C) = *(float *)(fighter_jet_vars + 0x170) * speed_scale;
    *(float *)(fighter_jet_vars + 0xFC) = *(float *)(fighter_jet_vars + 0x174) * speed_scale;
    if (hit != 0) {
        int  i;
        int *child_ids;
        if (((unsigned char *)moby)[0x20] == 0x65)
            return;
        if (item_unlocked[0] == 0) {
            item_unlocked[0] = 1;
            allocate_voice_for_bank_entry(1, 0, 0);
            FUN_L00_00263d40(0x53D6, -1);
        }
        fast_vec_sub(hit_vec, fighter_jet_vars + 0xD0, fighter_jet_vars + 0xE0);
        moby[0x20] = 0x65;
        *(u128 *)hit_dir = *(u128 *)hit_vec;
        spawn_explosion(moby,
                        hit_dir,
                        moby + 0x10,
                        0.0f,
                        0.0f,
                        0x14,
                        3,
                        4,
                        4.0f,
                        2.0f,
                        100000.0f,
                        3.0f,
                        1,
                        15.0f,
                        1,
                        1,
                        -1,
                        0);
        FUN_L01_0030be70(moby, hit_dir, speed_scale);
        child_ids = (int *)(fighter_jet_vars + 0x140);
        for (i = 0; i < 4; i++) {
            if (child_ids[i] != -1) {
                mark_moby_for_removal((char *)(*(int *)&ents + (child_ids[i] << 8)));
            }
        }
        mark_moby_for_removal(moby);
        return;
    }
    if (((unsigned char *)moby)[0x20] == 0x65)
        return;
    gunship_update(moby);
    *(unsigned short *)(moby + 0x34) |= 0x1000;
    fighter_jet_vars[0x2B] = 1;
    fighter_jet_vars[0x2C] = 0x64;
    if (175.0f <= *(float *)(moby + 0x18)) {
        moby[0x23] = 0;
    } else if (125.0f < *(float *)(moby + 0x18) && *(float *)(moby + 0x18) < 175.0f) {
        moby[0x23] = truncate_float_to_s32((175.0f - *(float *)(moby + 0x18)) * 128.0f / 50.0f);
    } else if (*(float *)(moby + 0x18) <= 125.0f) {
        ((unsigned char *)moby)[0x23] = 0x80;
    }
}

#include "qcopy.h"
