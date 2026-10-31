/* Level 15 moby classes radial_exploding_prop (0x94, three times wider, and 0xff): when a hit with flag 0x80000 lands, bursts into debris chunks, an explosion and smoke scattered all around it, drops its bolts and deletes itself. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"

extern char D_0014C190[];

extern unsigned char D_001413F4[];

extern unsigned char D_0013DD50 NOT_SDA;

typedef union {
    u128  q;
    float f[4];
} Vec;

extern char D_L15_00178700[];

extern char destructible_radial_debris_color_palette[] __asm__("D_L15_00161568");

u64 D_L15_00161570 = 0xd2000000ce;

typedef struct {
    u32 words[3];
} Colors;

extern Colors l15_destructible_secondary_debris_color_palette __asm__("D_L15_00161578");

extern u8 D_001413F4[];

extern u8 D_0013DD50;

extern void normalize_vector_xyz_7880(float, void *, void *) __asm__("FUN_001f9bf8");

extern void scale_vector_xyz_7880(float, void *, void *) __asm__("FUN_001f9a68");

extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");

extern int FUN_L00_002371e0_7880(float, int, int) __asm__("FUN_L15_00249ce8");

extern void FUN_L00_00257470_7880(void *, int, long long) __asm__("FUN_L00_00257470");

extern void FUN_L00_0025e450_7880(void *,
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
                                  int,
                                  float,
                                  float,
                                  int,
                                  int,
                                  int,
                                  int) __asm__("FUN_L00_0025e450");

extern void *spawn_debris_chunk(void *, void *, int, float, int, float, float, float, int) __asm__("FUN_L01_002f8530");

extern void *mine_debris(void *, void *, int, int, float, int, int) __asm__("FUN_L00_0026bed0");

void radial_exploding_prop_update(struct Moby *moby) __asm__("FUN_L15_002a7880");

/* Moby classes 148, 255 */
void radial_exploding_prop_update(struct Moby *moby) {
    char *slot = 0;
    Vec   velocity, hit_direction_step, doubled_hit_direction_step, offset, debris_origin;
    u32   palette[4];
    u32   colors[4];
    Vec   position;
    int   i, index, slot_index;
    u32  *palette_entry;
    float radius, angle, size;
    int   color, color2, duration;
    void *effect;
    Vec  *base_position;

    if (D_001413F4[0] != 2 && moby->state == 1)
        return;

    for (slot_index = 0; slot_index < 64; slot_index++) {
        char *radial_debris_hit_record = D_L15_00178700 + slot_index * 0x40;
        if (*(struct Moby **)(radial_debris_hit_record + 0x34) != moby)
            continue;
        if ((*(u32 *)(radial_debris_hit_record + 0x24) & 0x80000) == 0)
            continue;
        slot = radial_debris_hit_record;
        break;
    }
    moby->hit_slot = 0xff;
    if (slot == 0)
        goto no_hit;

    __builtin_memcpy(palette, &destructible_radial_debris_color_palette, 16);
    normalize_vector_xyz_7880(frame_time * 10.0f, &hit_direction_step, slot + 0x10);
    scale_vector_xyz_7880(2.0f, &doubled_hit_direction_step, &hit_direction_step);
    if (fast_vec_length(slot) != 0.0f) {
        debris_origin.q = *(u128 *)slot;
    } else {
        debris_origin.q = *(u128 *)&moby->pos;
        debris_origin.f[2] += 6.0f;
    }

    base_position = &debris_origin;
    {
        float radius, angle;
        int   i = 0x18;
        do {
            int duration;
            radius = random_float_between(0.5f, 1.5f);
            angle = random_angle_radians();
            if (moby->oclass == 0x94)
                radius *= 3.0f;
            offset.f[0] = fast_cos(angle) * radius;
            offset.f[1] = fast_sin(angle) * radius;
            offset.f[2] = 0.0f;
            offset.f[2] = random_float_between(1.0f, 7.0f);
            add_vectors(colors, &offset, &moby->pos);
            fast_vec_sub(colors, base_position, &offset);
            velocity.q = *(u128 *)colors;
            normalize_vector_xyz_7880(frame_time * 5.0f, &velocity, &velocity);
            add_vectors(colors, &velocity, &doubled_hit_direction_step);
            velocity.f[2] += frame_time * 3.0f;
            size = random_float_between(0.75f, 1.25f);
            scale_vector_xyz_7880(size, &velocity, &velocity);
            index = random_integer_below(4);
            palette_entry = &palette[index];
            radius = random_float_between(0.1f, 0.3f);
            duration = random_int_between(60, 100);
            size = random_float_between(3.0f, 7.0f);
            spawn_debris_chunk(
                &offset, &velocity, *palette_entry, radius, duration, size, 0.15f, 2.0f, 1);
            i--;
        } while (i >= 0);
    }
    if (moby->oclass == 0x94) {
        offset.q = *(u128 *)&moby->pos;
        offset.f[2] += 10.0f;
        radius = 1.0f;
        fast_vec_sub(colors, base_position, &offset);
        velocity.q = *(u128 *)colors;
        normalize_vector_xyz_7880(frame_time * 5.0f, &velocity, &velocity);
        add_vectors(colors, &velocity, &doubled_hit_direction_step);
        velocity.f[2] += frame_time * 3.0f;
        size = random_float_between(0.75f, 1.25f);
        scale_vector_xyz_7880(size, &velocity, &velocity);
        duration = random_int_between(60, 100);
        effect =
            spawn_debris_chunk(&offset, &velocity, 0xc9, radius, duration, 5.0f, radius, 2.0f, 1);
        if (effect != 0)
            scale_vector_xyz_7880(0.35f, (char *)effect + 0x40, (char *)effect + 0x40);
    }
    clear_u64_value(&velocity);
    FUN_L00_0025e450_7880(moby,
                          &velocity,
                          base_position,
                          0.0f,
                          0.0f,
                          20,
                          3,
                          4,
                          10.0f,
                          5.0f,
                          100000.0f,
                          0,
                          3.0f,
                          15.0f,
                          1,
                          1,
                          -1,
                          0);

    i = 0x13;
    do {
        float radius, angle;
        int   color, color2, duration;
        __builtin_memcpy(colors, &l15_destructible_secondary_debris_color_palette, 12);
        radius = random_float_between(0.0f, 1.5f);
        angle = random_angle_radians();
        if (moby->oclass == 0x94)
            radius *= 3.0f;
        offset.f[0] = fast_cos(angle) * radius;
        offset.f[1] = fast_sin(angle) * radius;
        offset.f[2] = 0.0f;
        fast_vec_add(&offset, &offset, &moby->pos);
        offset.f[2] += random_float_between(1.0f, 5.5f);
        fast_vec_sub(&position, base_position, &offset);
        velocity.q = position.q;
        normalize_vector_xyz_7880(frame_time * 5.0f, &velocity, &velocity);
        add_vectors(&position, &velocity, &hit_direction_step);
        size = random_float_between(0.75f, 1.25f);
        scale_vector_xyz_7880(size, &velocity, &velocity);
        size = random_float_between(0.25f, 1.0f);
        color = FUN_L00_002371e0_7880(size, 0x7f000000, 0x7f182030);
        size = random_float_between(0.5f, 1.0f);
        color2 = FUN_L00_002371e0_7880(size, 0, 0x5f5f5f);
        size = random_float_between(1.0f, 3.0f) * 210000.0f;
        duration = scale_game_frames(180);
        index = random_integer_below(3);
        effect = mine_debris(&offset, &velocity, color, color2, size, duration, colors[index]);
        if (effect != 0)
            *(float *)((char *)effect + 0x2c) = moby->pos.z - 0.5f;
        i--;
    } while (i >= 0);
    FUN_L00_00257470_7880(moby, 0, -1);
    mark_moby_for_removal(moby);
    return;

no_hit:
    moby->hit_slot = 0xff;
    if (moby->state != 0)
        return;
    moby->update_dist = 0xff;
    moby->state = 1;
    moby->draw_dist = 0x80;
    moby->scale = moby->scale + moby->scale;
    {
        u8  saved_flag = D_0013DD50;
        u16 save_id = moby->save_id;
        u32 saved_bit =
            *(u32 *)(D_0014C190 + (((s16)save_id >> 5) * 4) + current_level_index * 0x100) &
            (1U << (save_id & 31));
        if (saved_flag != 0 && saved_bit != 0)
            mark_moby_for_removal(moby);
    }
}

extern void *spawn_debris_chunk(void *pos,
                                void *vel,
                                int   color,
                                float a,
                                int   life,
                                float size,
                                float b,
                                float c,
                                int   d) __asm__("FUN_L01_002f8530");

extern void *
mine_debris(void *pos, void *vel, int color, int color2, float size, int life, int tint) __asm__("FUN_L00_0026bed0");

Colors l15_destructible_secondary_debris_color_palette __asm__("D_L15_00161578") = {
    {0, 1, 2}
};
