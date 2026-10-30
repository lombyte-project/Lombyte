/* Level 10 moby class height_adjusted_projectile. */
#include "types.h"
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
#include "rnc/overlay/quad.h"

extern float D_0015ED6C;

#include "sda.h"

void approach_moby_height(struct Moby *moby, float target_height) __asm__("FUN_L10_002d7b18");

void approach_moby_height(struct Moby *moby, float target_height) {
    float current_height = moby->pos.z;
    float height_delta = target_height - current_height;
    if (0.1f < height_delta) {
        moby->pos.z = current_height + 0.1f;
    } else if (height_delta < -0.1f) {
        moby->pos.z = current_height - 0.1f;
    } else {
        moby->pos.z = target_height;
    }
}

extern int          collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern void         reflect_vector(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern int          find_point_ahead(float, void *, void *) __asm__("FUN_L01_00274b78");
extern void         FUN_L00_0026dd20(void *, void *, int, int, float);
extern char         D_L10_001742C0[];
extern char         D_L10_00174300[];
extern OvlQuad      D_L10_00167240;
extern struct Moby *D_001413D0_lui __asm__("D_001413D0") NOT_SDA;

void update_height_adjusted_projectile(struct Moby *moby) __asm__("FUN_L10_002d7b80");

void update_height_adjusted_projectile(struct Moby *moby) {
    enum {
        HEIGHT_MODE_SKIP_ADJUSTMENT = 0,
        HEIGHT_MODE_APPROACH_FIXED_HEIGHT = 1,
        HEIGHT_MODE_FOLLOW_COLLISION_HEIGHT = 2,
        HEIGHT_MODE_FOLLOW_SELECTED_MOBY_HEIGHT = 3
    };
    char *data = (char *)moby->pvars;
    float old[4] __attribute__((aligned(16)));
    float found[20] __attribute__((aligned(16)));
    float check[4] __attribute__((aligned(16)));
    float raised[4] __attribute__((aligned(16)));
    float hit[12] __attribute__((aligned(16)));
    float rand_dir[4] __attribute__((aligned(16)));
    int   i;

    qcopy(old, &moby->pos);
    fast_vec_add(&moby->pos, &moby->pos, data);
    find_point_ahead(20.0f, moby, found);
    switch (*(short *)(data + 0x12)) {
    case HEIGHT_MODE_SKIP_ADJUSTMENT:
        break;
    case HEIGHT_MODE_APPROACH_FIXED_HEIGHT:
        approach_moby_height(moby, *(float *)(data + 0x1c));
        break;
    case HEIGHT_MODE_FOLLOW_COLLISION_HEIGHT:
        qcopy(check, &moby->pos);
        qcopy(raised, &moby->pos);
        check[2] += 0.1f;
        raised[2] -= *(float *)(data + 0x1c) + *(float *)(data + 0x1c);
        if (collision_line(check, raised, 2, (int)moby, 0))
            approach_moby_height(moby, *(float *)(D_L10_001742C0 + 0x28) + *(float *)(data + 0x1c));
        else
            approach_moby_height(moby, moby->pos.z - 0.1f);
        break;
    case HEIGHT_MODE_FOLLOW_SELECTED_MOBY_HEIGHT:
        if (*(int *)((char *)found + 0x40) != 0)
            approach_moby_height(moby, found[2] + *(float *)(data + 0x1c));
        break;
    case 4:
        break;
    }
    qcopy(check, data);
    check[2] = 0.0f;
    set_vector_length_xy(check, check, 1.0f);
    check[2] = 1.0f;
    make_hit_descriptor(hit, moby, 0x10001, *(float *)(data + 0x18), check);
    if (fast_dec_timer_s16(data + 0x10)) {
        mark_moby_for_removal(moby);
        return;
    }
    if (collision_line(old, &moby->pos, 0, *(int *)(data + 0x20), (int)hit)) {
        char        *collision = D_L10_001742C0;
        struct Moby *other = *(struct Moby **)(collision + 0x18);
        if (other == 0 || other->oclass != 0x359) {
            qcopy(&moby->pos, collision + 0x20);
            if (*(char **)(data + 0x14) != 0) {
                if (other != 0) {
                    if (other == D_001413D0_lui && *(struct Moby **)(data + 0x20) != 0 &&
                        (*(struct Moby **)(data + 0x20))->state != 0xfe &&
                        (*(struct Moby **)(data + 0x20))->state != 0xfd)
                        **(char **)(data + 0x14) = 1;
                }
            }
            for (i = 0; i < 5; i++) {
                *(OvlQuad *)rand_dir = 0;
                rand_dir[0] = random_float_between(-1.0f, 1.0f);
                rand_dir[1] = random_float_between(-1.0f, 1.0f);
                rand_dir[2] = random_float_between(-1.0f, 1.0f);
                *(OvlQuad *)raised = *(OvlQuad *)rand_dir;
                reflect_vector(rand_dir, data, D_L10_00174300);
                fast_vec_normalize(raised, raised, fast_vec_length(rand_dir) * 0.5f);
                fast_vec_add(raised, rand_dir, raised);
                fast_vec_normalize(
                    raised, raised, random_float_between(D_0015ED6C * 3.0f, D_0015ED6C * 6.0f));
                FUN_L00_0026dd20(&moby->pos,
                                 raised,
                                 0x7f2f4f6f,
                                 random_int_between(scale_game_frames(10), scale_game_frames(15)),
                                 30000.0f);
            }
            mark_moby_for_removal(moby);
            return;
        }
    }
    if (moby->pos.x < 2.0f || moby->pos.y < 2.0f || moby->pos.z < 2.0f || moby->pos.x > 510.0f ||
        moby->pos.y > 510.0f || moby->pos.z > 510.0f ||
        vector_distance_xy(&moby->pos, &D_L10_00167240) > 80.0f)
        mark_moby_for_removal(moby);
}
