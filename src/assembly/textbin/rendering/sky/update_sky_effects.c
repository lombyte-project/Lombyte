#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/rendering/sky/update_sky_effects/FUN_0022ae70.s", FUN_0022ae70);
#else
#include "types.h"

struct LevelSkyEffectData {
    u8 pad_0[4];
    s16 relocation_state;
    u8 pad_6[2];
    s16 effect_count;
    u8 pad_A[0x12];
    struct SkyEffect *effects;
};

/* Orbiting effects use two halfword angles; randomized effects use RGBA. */
union SkyEffectColorOrAngles {
    struct {
        s16 azimuth;
        u16 elevation;
    } angles;
    u32 base_color;
};

struct SkyEffect {
    u8 randomize_color;
    u8 pad1[1];
    u8 texture_index;
    u8 flags;
    u32 color;
    f32 angle;
    union SkyEffectColorOrAngles state;
    f32 position_x;
    f32 position_y;
    f32 position_z;
    f32 size;
};

extern struct LevelSkyEffectData *level_sky_effect_data __asm__("D_0016045C");
extern u8 D_001D96E0[];
extern void func_001160C8(s32);
extern f32 AbsoluteFloat(f32) __asm__("func_001F99C0");
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern void func_001F9FC8(u8 *);
extern f32 fast_add_rotations(f32, f32) __asm__("func_001FA580");
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern s32 random_integer_below(s32) __asm__("func_00213260");
extern f32 func_00213308();
extern void sky_draw_shell(s32) __asm__("func_0022B690");
extern void func_0022BBA0();
extern void func_00233980(s32, u64);
extern s32 rand() __asm__("func_001160D8");

void update_sky_effects(void) __asm__("FUN_0022ae70");

void update_sky_effects(void) {
    f32 trig_product;
    f32 elevation;
    f32 azimuth;
    s32 color_delta;
    u32 base_color;
    s32 initialization_index;
    s32 effect_index;
    s32 alpha_delta;
    s32 random_bits;
    s32 random_color_enabled;
    s32 effect_flags;
    s16 effect_count;
    f32 radius;
    f32 angle_step;
    struct SkyEffect *effect;
    s16 *angles;

    level_sky_effect_data->relocation_state = 0;
    func_001F9FC8(D_001D96E0);
    sky_draw_shell(0);
    sky_draw_shell(1);
    effect_count = level_sky_effect_data->effect_count;
    if (effect_count == 0) {
        level_sky_effect_data->effect_count = 0x100;
        initialization_index = 0;
        func_001160C8(0x3039);
        if (level_sky_effect_data->effect_count <= 0) {
            goto end;
        }
        base_color = 0x30505050;
        radius = 50.0f;
        random_color_enabled = 1;
        effect_flags = 0x48;
        while (1) {
            effect = &level_sky_effect_data->effects[initialization_index];
            if (initialization_index >= 0xF6) {
                effect->randomize_color = 0;
                angles = &effect->state.angles.azimuth;
                effect->state.angles.azimuth = (s16) (rand() >> 0x10);
                angles[1] = (s16) (rand() >> 0x10);
                /* Retail stores the float bits 0x3E23D70A. */
                effect->size = 0.16f;
                effect->texture_index = random_color_enabled;
                effect->flags = effect_flags;
            } else {
                effect->randomize_color = random_color_enabled;
                color_delta = random_integer_below(0x100);
                effect->texture_index = random_color_enabled;
                effect->flags = effect_flags;
                effect->state.angles.azimuth = color_delta;
                effect->angle = func_00213308();
                effect->size = convert_integer_to_float(random_integer_below(0x18) + 0x20) * 0.00390625f;
                azimuth = fast_add_rotations(-3.0f, func_00213308() * 0.2f);
                elevation = func_00213308() * 0.09f + 1.2f;
                trig_product = fast_cos(azimuth);
                trig_product = trig_product * fast_sin(elevation);
                trig_product = trig_product * radius;
                effect->position_x = trig_product;
                trig_product = fast_sin(azimuth);
                trig_product = trig_product * fast_sin(elevation);
                trig_product = trig_product * radius;
                effect->position_y = trig_product;
                effect->position_z = fast_cos(elevation) * radius;
                color_delta = random_integer_below(0x18);
                alpha_delta = random_integer_below(0x20) << 0x18;
                if ((rand() >> 0x10) & 1) {
                    effect->state.base_color = alpha_delta + ((color_delta << 0x10) + base_color);
                } else {
                    effect->state.base_color = (alpha_delta + ((color_delta << 8) + base_color)) | color_delta;
                }
            }
            initialization_index += 1;
            if (initialization_index >= level_sky_effect_data->effect_count) {
                break;
            }
        }
        effect_count = level_sky_effect_data->effect_count;
    }
    if (effect_count <= 0) {
        goto end;
    }
    radius = 50.0f;
    angle_step = 0.0015339808f;
    effect_index = 0;
update_next_effect:
    effect = &level_sky_effect_data->effects[effect_index];
    if (effect->randomize_color == 0) {
        angles = &effect->state.angles.azimuth;
        effect->state.angles.azimuth = (s16) (effect->state.angles.azimuth + 1);
        angles[1] = (u16) (angles[1] + 1);
        azimuth = convert_integer_to_float((effect->state.angles.azimuth & 0xFFF) - 0x800) * angle_step;
        elevation = convert_integer_to_float((effect->state.angles.elevation & 0xFFF) - 0x800) * angle_step;
        trig_product = fast_cos(azimuth);
        trig_product = trig_product * fast_sin(elevation);
        trig_product = trig_product * radius;
        effect->position_x = trig_product;
        trig_product = fast_sin(azimuth);
        trig_product = trig_product * fast_sin(elevation);
        trig_product = trig_product * radius;
        effect->position_y = trig_product;
        effect->position_z = AbsoluteFloat(fast_cos(elevation)) * radius;
        if ((u32) (effect->state.angles.azimuth & 0x3F) < 8U) {
            effect->color = 0x702020F0;
        } else {
            effect->color = 0x202020F0;
        }
    } else {
        u32 color_value;
        u32 color_offset;
        random_bits = rand() >> 0x10;
        color_offset = ((random_bits & 0x1F00) << 0xA) + 0xFFDFDFE0;
        color_value = effect->state.base_color + color_offset;
        color_value += (random_bits & 0x1F0) << 6;
        color_value += (random_bits & 0x1F) << 2;
        effect->color = color_value;
    }
    effect_index += 1;
    if (effect_index < level_sky_effect_data->effect_count) {
        goto update_next_effect;
    }
end:
    func_0022BBA0();
    func_00233980(0x42, (0x8000ULL << 0x18) | 0x44);
    sky_draw_shell(2);
    sky_draw_shell(3);
}

extern __typeof__(update_sky_effects) func_0022AE70 __attribute__((alias("FUN_0022ae70")));

#endif /* NON_MATCHING */
