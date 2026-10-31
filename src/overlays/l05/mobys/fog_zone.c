/* Level 05 moby class fog_zone. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/overlay/cuboid.h"

extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");

/* Fog zone: switches the level fog between its two settings when the hero enters either trigger volume, and
 * tints a colour by the hero's side of a gate while inside the third. */
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_0030EB68.c: func_L05_0030F6C0), where it is exact; names translated to the US level program. */

extern float D_L05_0015F448;
extern float D_L05_0015F44C;
extern float D_L05_0015F450;
extern float D_L05_0015F454;

extern unsigned char D_L05_0015F444;
extern unsigned char D_L05_0015F445;
extern unsigned char D_L05_0015F446;
extern unsigned char l05_volume_blended_color_upper_middle_byte __asm__("D_L05_00161202");
extern unsigned char l05_volume_blended_color_high_byte __asm__("D_L05_00161203");
extern unsigned char l05_volume_blended_color_lower_middle_byte __asm__("D_L05_00161201");
extern unsigned char l05_volume_blended_color_low_byte __asm__("D_L05_00161200");

void fog_zone_update(struct Moby *moby) __asm__("FUN_L05_0030e1f8");

/* Moby class 841 */
void fog_zone_update(struct Moby *moby) {
    enum {
        FOG_ZONE_INITIALIZE = 0,
        FOG_ZONE_WAIT_TO_APPLY_PRESET = 1,
        FOG_ZONE_WAIT_TO_RESTORE_SAVED_SETTINGS = 2
    };
    char *fog_zone_vars = ((char *)moby->pvars);
    float hero_offset_from_volume_origin[4];
    float hero_position_in_volume_space[4];
    char *hero_position = (char *)&hero.motion.pos;
    if (is_point_inside_clip_volume(hero_position, *(int *)(fog_zone_vars + 0x28))) {
        *(float *)(hero_position + 0x270) = 500.0f;
    }
    switch (((unsigned char *)moby)[0x20]) {
    case FOG_ZONE_INITIALIZE:
        fog_zone_vars[0x20] = D_L05_0015F444;
        fog_zone_vars[0x21] = D_L05_0015F445;
        fog_zone_vars[0x22] = D_L05_0015F446;
        {
            float f0 = D_L05_0015F448;
            float f1 = D_L05_0015F44C;
            float f2 = D_L05_0015F450;
            float f3 = D_L05_0015F454;
            *(float *)(fog_zone_vars + 0) = f0;
            *(float *)(fog_zone_vars + 8) = f1;
            *(float *)(fog_zone_vars + 4) = f2;
            *(float *)(fog_zone_vars + 0xC) = f3;
        }
        fog_zone_vars[0x23] = 0x49;
        fog_zone_vars[0x24] = 0x53;
        fog_zone_vars[0x25] = 0xC;
        *(float *)(fog_zone_vars + 0x18) = 33792.0f;
        *(float *)(fog_zone_vars + 0x14) = 255.0f;
        *(int *)(fog_zone_vars + 0x10) = 0;
        *(int *)(fog_zone_vars + 0x1C) = 0;
        {
            int c = l05_volume_blended_color_low_byte +
                    (l05_volume_blended_color_lower_middle_byte << 8) +
                    (l05_volume_blended_color_upper_middle_byte << 16) +
                    (l05_volume_blended_color_high_byte << 24);
            *(int *)(fog_zone_vars + 0x34) = 0x40408070;
            *(int *)(fog_zone_vars + 0x30) = c;
        }
        moby->state = 1;
        break;
    case FOG_ZONE_WAIT_TO_APPLY_PRESET:
        if (is_point_inside_clip_volume(&hero.motion.pos, *(int *)(fog_zone_vars + 0x2C))) {
            D_L05_0015F444 = fog_zone_vars[0x23];
            D_L05_0015F445 = fog_zone_vars[0x24];
            D_L05_0015F446 = fog_zone_vars[0x25];
            {
                float f0 = *(float *)(fog_zone_vars + 0x10);
                float f1 = *(float *)(fog_zone_vars + 0x18);
                float f2 = *(float *)(fog_zone_vars + 0x14);
                float f3 = *(float *)(fog_zone_vars + 0x1C);
                D_L05_0015F448 = f0;
                D_L05_0015F44C = f1;
                D_L05_0015F450 = f2;
                D_L05_0015F454 = f3;
            }
            moby->state = 2;
        }
        break;
    case FOG_ZONE_WAIT_TO_RESTORE_SAVED_SETTINGS:
        if (is_point_inside_clip_volume(&hero.motion.pos, *(int *)(fog_zone_vars + 0x28))) {
            D_L05_0015F444 = fog_zone_vars[0x20];
            D_L05_0015F445 = fog_zone_vars[0x21];
            D_L05_0015F446 = fog_zone_vars[0x22];
            {
                float f0 = *(float *)(fog_zone_vars + 0);
                float f1 = *(float *)(fog_zone_vars + 8);
                float f2 = *(float *)(fog_zone_vars + 4);
                float f3 = *(float *)(fog_zone_vars + 0xC);
                D_L05_0015F448 = f0;
                D_L05_0015F44C = f1;
                D_L05_0015F450 = f2;
                D_L05_0015F454 = f3;
            }
            moby->state = 1;
        }
        break;
    }
    hero_position = (char *)&hero.motion.pos;
    if (is_point_inside_clip_volume(hero_position, *(int *)(fog_zone_vars + 0x38))) {
        int c;
        fast_vec_sub(hero_offset_from_volume_origin,
                     hero_position,
                     &l05_cuboids[*(int *)(fog_zone_vars + 0x38)].pos);
        hero_offset_from_volume_origin[3] = 0.0f;
        transform_vector_by_basis(hero_position_in_volume_space,
                                  hero_offset_from_volume_origin,
                                  l05_cuboids[*(int *)(fog_zone_vars + 0x38)].to_local);
        c = fast_tween_color(*(int *)(fog_zone_vars + 0x30),
                             *(int *)(fog_zone_vars + 0x34),
                             (hero_position_in_volume_space[0] + 1.0f) * 0.5f);
        l05_volume_blended_color_lower_middle_byte = c >> 8;
        l05_volume_blended_color_upper_middle_byte = c >> 16;
        l05_volume_blended_color_low_byte = c;
        l05_volume_blended_color_high_byte = c >> 24;
    }
}
