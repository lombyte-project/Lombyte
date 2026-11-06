/* Level 05 moby class fog_zone. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/overlay/shared/world/cuboid.h"

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

typedef struct FogZoneVars {
    f32 saved_fog_448;
    f32 saved_fog_450;
    f32 saved_fog_44C;
    f32 saved_fog_454;
    s32 preset_fog_448;
    f32 preset_fog_450;
    f32 preset_fog_44C;
    s32 preset_fog_454;
    u8  saved_fog_444;
    u8  saved_fog_445;
    u8  saved_fog_446;
    u8  preset_fog_444;
    u8  preset_fog_445;
    u8  preset_fog_446;
    u8  pad_26[0x2];
    s32 clip_volume_28;
    s32 clip_volume_2C;
    s32 saved_blend_color;
    s32 fog_color;
    s32 fog_volume; /* the colour tweens from saved_blend_color to fog_color inside it */
} FogZoneVars;

/* Moby class 841 */
void fog_zone_update(struct Moby *moby) {
    enum {
        FOG_ZONE_INITIALIZE = 0,
        FOG_ZONE_WAIT_TO_APPLY_PRESET = 1,
        FOG_ZONE_WAIT_TO_RESTORE_SAVED_SETTINGS = 2
    };
    FogZoneVars *fog_zone_vars = (FogZoneVars *)moby->pvars;
    float        hero_offset_from_volume_origin[4];
    float        hero_position_in_volume_space[4];
    char        *hero_position = (char *)&hero.motion.pos;
    if (is_point_inside_clip_volume(hero_position, fog_zone_vars->clip_volume_28)) {
        *(float *)(hero_position + 0x270) = 500.0f;
    }
    switch (((unsigned char *)moby)[0x20]) {
    case FOG_ZONE_INITIALIZE:
        fog_zone_vars->saved_fog_444 = D_L05_0015F444;
        fog_zone_vars->saved_fog_445 = D_L05_0015F445;
        fog_zone_vars->saved_fog_446 = D_L05_0015F446;
        {
            float fog_448 = D_L05_0015F448;
            float fog_44C = D_L05_0015F44C;
            float fog_450 = D_L05_0015F450;
            float fog_454 = D_L05_0015F454;
            fog_zone_vars->saved_fog_448 = fog_448;
            fog_zone_vars->saved_fog_44C = fog_44C;
            fog_zone_vars->saved_fog_450 = fog_450;
            fog_zone_vars->saved_fog_454 = fog_454;
        }
        fog_zone_vars->preset_fog_444 = 0x49;
        fog_zone_vars->preset_fog_445 = 0x53;
        fog_zone_vars->preset_fog_446 = 0xC;
        fog_zone_vars->preset_fog_44C = 33792.0f;
        fog_zone_vars->preset_fog_450 = 255.0f;
        fog_zone_vars->preset_fog_448 = 0;
        fog_zone_vars->preset_fog_454 = 0;
        {
            int saved_volume_blend_color = l05_volume_blended_color_low_byte +
                                           (l05_volume_blended_color_lower_middle_byte << 8) +
                                           (l05_volume_blended_color_upper_middle_byte << 16) +
                                           (l05_volume_blended_color_high_byte << 24);
            fog_zone_vars->fog_color = 0x40408070;
            fog_zone_vars->saved_blend_color = saved_volume_blend_color;
        }
        moby->state = FOG_ZONE_WAIT_TO_APPLY_PRESET;
        break;
    case FOG_ZONE_WAIT_TO_APPLY_PRESET:
        if (is_point_inside_clip_volume(&hero.motion.pos, fog_zone_vars->clip_volume_2C)) {
            D_L05_0015F444 = fog_zone_vars->preset_fog_444;
            D_L05_0015F445 = fog_zone_vars->preset_fog_445;
            D_L05_0015F446 = fog_zone_vars->preset_fog_446;
            {
                float fog_448 = *(float *)&fog_zone_vars->preset_fog_448;
                float fog_44C = fog_zone_vars->preset_fog_44C;
                float fog_450 = fog_zone_vars->preset_fog_450;
                float fog_454 = *(float *)&fog_zone_vars->preset_fog_454;
                D_L05_0015F448 = fog_448;
                D_L05_0015F44C = fog_44C;
                D_L05_0015F450 = fog_450;
                D_L05_0015F454 = fog_454;
            }
            moby->state = FOG_ZONE_WAIT_TO_RESTORE_SAVED_SETTINGS;
        }
        break;
    case FOG_ZONE_WAIT_TO_RESTORE_SAVED_SETTINGS:
        if (is_point_inside_clip_volume(&hero.motion.pos, fog_zone_vars->clip_volume_28)) {
            D_L05_0015F444 = fog_zone_vars->saved_fog_444;
            D_L05_0015F445 = fog_zone_vars->saved_fog_445;
            D_L05_0015F446 = fog_zone_vars->saved_fog_446;
            {
                float fog_448 = fog_zone_vars->saved_fog_448;
                float fog_44C = fog_zone_vars->saved_fog_44C;
                float fog_450 = fog_zone_vars->saved_fog_450;
                float fog_454 = fog_zone_vars->saved_fog_454;
                D_L05_0015F448 = fog_448;
                D_L05_0015F44C = fog_44C;
                D_L05_0015F450 = fog_450;
                D_L05_0015F454 = fog_454;
            }
            moby->state = FOG_ZONE_WAIT_TO_APPLY_PRESET;
        }
        break;
    }
    hero_position = (char *)&hero.motion.pos;
    if (is_point_inside_clip_volume(hero_position, fog_zone_vars->fog_volume)) {
        int c;
        fast_vec_sub(hero_offset_from_volume_origin,
                     hero_position,
                     &l05_cuboids[fog_zone_vars->fog_volume].pos);
        hero_offset_from_volume_origin[3] = 0.0f;
        transform_vector_by_basis(hero_position_in_volume_space,
                                  hero_offset_from_volume_origin,
                                  l05_cuboids[fog_zone_vars->fog_volume].to_local);
        c = fast_tween_color(fog_zone_vars->saved_blend_color,
                             fog_zone_vars->fog_color,
                             (hero_position_in_volume_space[0] + 1.0f) * 0.5f);
        l05_volume_blended_color_lower_middle_byte = c >> 8;
        l05_volume_blended_color_upper_middle_byte = c >> 16;
        l05_volume_blended_color_low_byte = c;
        l05_volume_blended_color_high_byte = c >> 24;
    }
}
