/* Shared moby class tresspasser (and hydrodisplacer): the gadget pickup and its sprite effect. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/math/interpolation.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/camera/scripted_camera.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/camera/follow_camera_set_up_spring.h"
#include "qcopy.h"

typedef struct Moby Moby;

typedef struct {
    int  i0;
    char p4[0x20];
    int  i24;
} Idle;

typedef struct {
    float a[4];
    float b[4];
    int   c[4];
    float d[4];
} L02SpriteData;

extern float D_0015ED6C MACRO_ADDR;
extern float D_0015ED70 MACRO_ADDR;
extern Idle             D_L02_00179A10;

extern s32 D_L02_0015F5CC;
extern int D_L02_0015F5CC; /* no foreign declaration */

extern void FUN_L02_002ea7d0_c(struct Moby *) __asm__("FUN_L02_002ea7d0");
extern void FUN_L02_002ea8b0_c(struct Moby *) __asm__("FUN_L02_002ea8b0");
extern void start_scene(int) __asm__("FUN_L00_00298840");
extern void clear_active_help_record(void) __asm__("FUN_L00_002039a0");
extern void help_request(int, int) __asm__("FUN_L00_00203908");
extern void set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern void
advance_accelerated_scalar(float, float, float, float, void *, void *) __asm__("FUN_00213f38");
extern void                     memcard_save_data(int, int) __asm__("FUN_0020b178");
extern unsigned char D_001413F4 NOT_SDA;
extern int                      D_L02_0015F640 __attribute__((sda));
extern char                     D_0013F3D0[];
extern float D_L02_00174468     NOT_SDA;
extern int D_L02_00167400       NOT_SDA;
extern float                    D_0015ED70;
extern float                    D_0015ED6C;
extern float                    D_L02_0015F3FC;

extern int            func_001FA8A8_2ebce8_ea8b0(int, int, float) __asm__("FUN_001fa6e0");
extern unsigned char *func_L00_00273E08_ea8b0(
    void *, int, unsigned char, int, int, int, int, float) __asm__("FUN_L00_00272f68");
extern char D_L02_00167440_ea8b0[] __asm__("D_L02_001673C0");

void tresspasser_update(struct Moby *moby) __asm__("FUN_L02_002ea210");

/* Moby classes 1005, 1016 */
void tresspasser_update(struct Moby *moby) {
    char *data;
    int   which;
    int   frames;
    float angle;
    float ground_offset;
    data = (char *)moby->pvars;
    frames = scale_game_frames(60);
    angle = 3.14159274f / (float)frames;
    which = moby->oclass == 0x3ED ? 0x1A : 0x16;
    switch (moby->state) {
    case 0:
        FUN_L02_002ea7d0_c(moby);
        ground_offset = 0.2f;
        if (items_acquired[which] == 0) {
            probe_ground_height(&moby->pos, 0, 0.5f);
            *(float *)((char *)moby + 0x40) = 1.57079637f;
            *(float *)((char *)moby + 0x44) = 3.14159274f;
            moby->pos.z = D_L02_00174468 + ground_offset;
            moby->state = 1;
            return;
        }
        mark_moby_for_removal(moby);
        return;
    case 1:
        moby->rot.z = fast_add_rotations(moby->rot.z, angle);
        FUN_L02_002ea8b0_c(moby);
        if (D_L02_0015F5CC % 10 != 0)
            return;
        if (!(vector_distance_xy(&moby->pos, D_0013F3D0) < 1.0f))
            return;
        if (absolute_float(moby->pos.z - *(float *)(D_0013F3D0 + 8)) < 2.0f) {
            moby->state = 3;
            moby->flags |= 1;
            moby->visible = 0;
            moby->collision = 0;
            if (moby->oclass == 0x3ED) {
                start_scene(4);
            } else {
                start_scene(2);
            }
        }
        return;
    case 3:
        if (level_game_mode == 2)
            return;
        if (D_001413F4 != 0) {
            if (which == 0x16) {
                grant_inventory_item(0x16, 0);
                D_L02_0015F640 = 0;
            } else {
                grant_inventory_item(which, 0);
            }
        } else {
            grant_inventory_item(which, 1);
        }
        if (moby->oclass == 0x3ED) {
            queue_dialogue_message(0x7DF, -1);
            clear_active_help_record();
            help_request(0x7D9, 0x13);
            D_L02_0015F3FC = 1.0f;
            moby->state = 4;
            set_race_camera(D_0013F3D0, D_0013F3D0 + 16, 0x72, 1);
            return;
        }
        memcard_save_data(0, -1);
        mark_moby_for_removal(moby);
        return;
    case 4: {
        float a[4] __attribute__((aligned(16))), b[4] __attribute__((aligned(16))),
            c[4] __attribute__((aligned(16))), d[4] __attribute__((aligned(16)));
        float position[4] __attribute__((aligned(16)));
        float output[4];
        /* Retail clears all four lanes before assigning the XYZ components. */
        *(u128 *)a = 0;
        *(u128 *)b = 0;
        *(u128 *)c = 0;
        *(u128 *)d = 0;
        a[0] = 91.2f;
        a[1] = 270.21f;
        a[2] = 64.85f;
        b[0] = 106.98f;
        b[1] = 273.46f;
        b[2] = 62.65f;
        c[0] = 0.07f;
        c[1] = 0.33f;
        c[2] = 0.55f;
        d[0] = 0.07f;
        d[1] = 0.43f;
        d[2] = 0.72f;
        advance_accelerated_scalar(1.0f,
                                   D_0015ED70 * 0.15f,
                                   D_0015ED70 * 0.15f,
                                   D_0015ED6C * 0.15f,
                                   data + 0x40,
                                   data + 0x44);
        lerp_vector(position, (void *)a, (void *)b, *(float *)(data + 0x40));
        output[2] =
            fast_add_rotations(fast_subtract_rotations(d[2], c[2]) * *(float *)(data + 0x40), c[2]);
        output[1] =
            fast_add_rotations(fast_subtract_rotations(d[1], c[1]) * *(float *)(data + 0x40), c[1]);
        output[0] =
            fast_add_rotations(fast_subtract_rotations(d[0], c[0]) * *(float *)(data + 0x40), c[0]);
        if (D_L02_00167400 && *(short *)(D_L02_00167400 + 0x86) != 5)
            jet_mount(position, output, 1, 0, 0);
        set_camera_position(position);
        set_camera_rotation(output);
        if (D_L02_00179A10.i0 == 0 && D_L02_00179A10.i24 == -1 && 0.2f < *(float *)(data + 0x40)) {
            approach_value(1.0f, frame_time * 4.0f, &D_L02_0015F3FC);
        } else {
            approach_value(0.0f, D_0015ED6C * 4.0f, &D_L02_0015F3FC);
        }
        if (D_L02_0015F3FC != 1.0f)
            return;
        set_race_camera(D_0013F3D0, D_0013F3D0 + 16, 0, 0);
        jet_end_mount(0);
        moby->state = 5;
        return;
    }
    case 5:
        approach_value(0.0f, D_0015ED6C * 4.0f, &D_L02_0015F3FC);
        if (D_L02_0015F3FC == 0.0f) {
            memcard_save_data(0, -1);
            mark_moby_for_removal(moby);
        }
        return;
    }
}

/* Initialises a moby's data block: four random floats, fixed constants, four random ints. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002EBC08), where it is exact; names translated to the US level program. */

void initialize_collectible_sprite_state(char *moby) __asm__("FUN_L02_002ea7d0");

void initialize_collectible_sprite_state(char *moby) {
    float *moby_data = ((float *)MOBY(moby)->pvars);
    int   *sprite_timers;
    int    i;

    for (i = 0; i < 4; i++) {
        moby_data[i] = (float)random_integer_below(0xFF);
    }
    moby_data[4] = -1.0f;
    moby_data[5] = -2.25f;
    moby_data[6] = 1.25f;
    moby_data[7] = 2.5f;
    sprite_timers = (int *)(moby_data + 8);
    for (i = 0; i < 4; i++) {
        sprite_timers[i] = scale_game_frames(0x3F + i * 0x40);
    }
    moby_data[12] = 2.5f;
    moby_data[13] = 3.0f;
    moby_data[15] = 2.5f;
    moby_data[14] = 3.0f;
}

void update_collectible_sprite_effect(char *moby) __asm__("FUN_L02_002ea8b0");

void update_collectible_sprite_effect(char *moby) {
    L02SpriteData *sprite_data = ((L02SpriteData *)MOBY(moby)->pvars);
    float          d[4] __attribute__((aligned(16)));
    float          sprite_position_step[4] __attribute__((aligned(16)));
    int            i, col;
    float          timer_elapsed_fraction;

    MOBY(moby)->pos.z += 0.5f;
    fast_vec_sub(d, D_L02_00167440_ea8b0, moby + 0x10);
    fast_vec_normalize(d, d, -0.3f);
    fast_vec_normalize(sprite_position_step, d, 0.1f);
    fast_vec_add(d, d, moby + 0x10);
    MOBY(moby)->pos.z -= 0.5f;
    for (i = 0; i < 4; i++) {
        float unwrapped_sprite_value = sprite_data->a[i] + sprite_data->b[i];
        sprite_data->a[i] = unwrapped_sprite_value;
        if (255.0f <= unwrapped_sprite_value) {
            sprite_data->a[i] = unwrapped_sprite_value - 255.0f;
        } else if (unwrapped_sprite_value <= 0.0f) {
            sprite_data->a[i] = unwrapped_sprite_value + 255.0f;
        }
        if (fast_dec_timer(&sprite_data->c[i])) {
            sprite_data->c[i] = scale_game_frames(0xFF);
        }
        timer_elapsed_fraction =
            convert_integer_to_float(scale_game_frames(0xFF) - sprite_data->c[i]);
        timer_elapsed_fraction = timer_elapsed_fraction / (float)scale_game_frames(0xFF);
        col = func_001FA8A8_2ebce8_ea8b0(
            0x4040FFFF, 0x1040FFFF, absolute_float(0.5f - timer_elapsed_fraction));
        func_L00_00273E08_ea8b0(d,
                                col,
                                truncate_float_to_s32(sprite_data->a[i]) & 0xFF,
                                0x35,
                                1,
                                2,
                                0,
                                sprite_data->d[i]);
        fast_vec_add(d, d, sprite_position_step);
    }
}
