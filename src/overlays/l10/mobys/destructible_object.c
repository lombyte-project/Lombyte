/* Level 10 moby class destructible_object (class 1047): explodes into debris when hit, stays destroyed and leaves its wreck mobys. */
#include "types.h"
#include "rnc/overlay/level_save_state.h"
#include "rnc/game/item_state.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_00296BD8.c: func_L10_002DEC08), where it is exact; names translated to the US level program. */

extern void update_entity(void *) __asm__("FUN_L00_00250df8");

char        *spawn_fixed_height_destruction_moby(struct Moby *owner) __asm__("FUN_L10_002dd848");
extern int   D_L10_001BA950[];
extern float D_0015ED6C;
extern void  add_vectors(void *, void *, void *) __asm__("FUN_L00_001ff290");
extern int   D_0015ED84;
extern int   trooper_play_sound(int, int, void *, int) __asm__("FUN_L00_0028dc90");
/* Ported from rac1-decomp src/overlays/l10_orxon/vendor_00296BD8.c (func_L10_002DB278) */

/* Orxon moby update: destroys itself when its item is taken, else fires its effect once per pass. */

typedef union {
    u128  q;
    float f[4];
} UVec;

char       *spawn_destruction_replacement_moby(struct Moby *owner) __asm__("FUN_L10_002eb8c0");
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
extern void spawn_debris_chunk(
    void *, void *, unsigned int, float, int, float, float, float, int) __asm__("FUN_L01_002f8530");

void update_destructible_object(char *moby) __asm__("FUN_L10_002d9eb8");

void update_destructible_object(char *moby) {
    typedef union {
        u128  q;
        float f[4];
    } UVec;
    UVec           zero_vector, transformed_offset, debris_vector, translated_position;
    unsigned short id = MOBY(moby)->save_id;
    int            debris_loop_countdown;
    int            k;
    int            debris_initial_timer;
    int            debris_option_roll;
    float          debris_gravity_multiplier, debris_scale_multiplier;

    if (l10_save_state.cleared[(short)id] != 0 ||
        ((*(int *)((char *)D_0014C190_257218 + (((short)id >> 5) * 4 + (D_0015ED84 << 8))) >>
          (id & 0x1F)) &
         1)) {
        spawn_fixed_height_destruction_moby(moby);
        spawn_destruction_replacement_moby(moby);
        mark_moby_for_removal(moby);
        return;
    }
    if (find_moby_hit(moby, 0x40000, 0)) {
        char *moby_rotation_basis = moby + 0xC0;
        zero_vector.q = 0;
        spawn_explosion(moby,
                        &zero_vector,
                        moby + 0x10,
                        0.0f,
                        0.0f,
                        5,
                        3,
                        4,
                        4.0f,
                        2.0f,
                        100000.0f,
                        3.0f,
                        -1,
                        15.0f,
                        1,
                        1,
                        -1,
                        0);
        trooper_play_sound(0, 0, moby, 0x99);
        debris_loop_countdown = 0x1D;
        do {
            k = 0x70F;
            debris_vector.q = 0;
            debris_vector.f[0] = -1.0f;
            debris_vector.f[1] = random_float_between(-3.0f, 3.0f);
            debris_vector.f[2] = random_float_between(-3.0f, 3.0f);
            debris_loop_countdown--;
            transformed_offset.q = debris_vector.q;
            transform_vector_by_basis(
                &transformed_offset, &transformed_offset, moby_rotation_basis);
            fast_vec_normalize(&debris_vector,
                               &transformed_offset,
                               random_float_between(5.0f, 10.0f) * D_0015ED6C);
            add_vectors(&translated_position, &transformed_offset, moby + 0x10);
            if (random_integer_below(1) == 0)
                k = 0x70E;
            debris_scale_multiplier = random_float_between(0.5f, 1.0f);
            debris_initial_timer = random_int_between(0x3C, 0x78);
            debris_gravity_multiplier = random_float_between(2.0f, 3.0f);
            debris_option_roll = random_integer_below(4);
            spawn_debris_chunk(&transformed_offset,
                               &debris_vector,
                               k,
                               debris_scale_multiplier,
                               debris_initial_timer,
                               debris_gravity_multiplier,
                               1.0f,
                               3.0f,
                               debris_option_roll == 0);
        } while (debris_loop_countdown >= 0);
        transformed_offset.q = 0;
        transformed_offset.f[0] = 2.8599999f;
        transformed_offset.f[1] = 1.57000005f;
        transformed_offset.f[2] = 1.83099997f;
        transform_vector_by_basis(&transformed_offset, &transformed_offset, moby_rotation_basis);
        spawn_explosion(moby,
                        &zero_vector,
                        &transformed_offset,
                        0.0f,
                        0.0f,
                        5,
                        3,
                        4,
                        4.0f,
                        2.0f,
                        100000.0f,
                        3.0f,
                        -1,
                        15.0f,
                        1,
                        1,
                        -1,
                        0);
        spawn_fixed_height_destruction_moby(moby);
        spawn_destruction_replacement_moby(moby);
        *(int *)((char *)D_0014C190_257218 +
                 (((short)MOBY(moby)->save_id >> 5) * 4 + (D_0015ED84 << 8))) |=
            1 << (MOBY(moby)->save_id & 0x1F);
        D_L10_001BA950[(short)MOBY(moby)->save_id >> 5] |= 1 << (MOBY(moby)->save_id & 0x1F);
        mark_moby_for_removal(moby);
    }
}

char *spawn_fixed_height_destruction_moby(struct Moby *owner) {
    char *moby = create_moby(0x462);
    if (moby != 0) {
        ((unsigned char *)moby)[0x30] = 0xFF;
        MOBY(moby)->draw_dist = 0xFF;
        moby[0x31] = 1;
        *(int *)(moby + 0x40) = 0;
        *(int *)(moby + 0x44) = 0;
        MOBY(moby)->rot.z = owner->rot.z;
        MOBY(moby)->spawn_frame = owner->spawn_frame;
        qcopy(moby + 0x10, &owner->pos);
        MOBY(moby)->pos.z = 59.0f;
        update_entity(moby);
    }
    return moby;
}

char *spawn_destruction_replacement_moby(struct Moby *owner) {
    char *moby = create_moby(0x781);
    if (moby != 0) {
        ((unsigned char *)moby)[0x30] = 0xFF;
        MOBY(moby)->draw_dist = 0xFF;
        moby[0x31] = 1;
        *(int *)(moby + 0x40) = 0;
        *(int *)(moby + 0x44) = 0;
        MOBY(moby)->rot.z = owner->rot.z;
        MOBY(moby)->spawn_frame = owner->spawn_frame;
        qcopy(moby + 0x10, &owner->pos);
        MOBY(moby)->pos.z = 59.0f;
        update_entity(moby);
    }
    return moby;
}
