/* Level 10 moby class linked_moby_hitbox: a box that hits the mobys it sweeps through. */
#include "types.h"
#include "rnc/game/item_state.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/game_globals.h"
#include "rnc/mobys/moby_spawn.h"

#include "qcopy.h"
#include "rnc/overlay/quad.h"

extern int   debug_print() __asm__("FUN_001e93b0");
extern char *D_L10_0015FFD8_p __asm__("D_L10_0015FFD8");

extern struct Moby *D_L10_0015FFD8;

#include "qzero.h"

/* Box a moby sweeps through: hits other mobys inside it (FUN_L10_002c9958) */
typedef struct {
    f32 width; /* 0x00: extent along the moby's x */
    f32 depth; /* 0x04: extent along the moby's z */
    u32 flags; /* 0x08: 0x100 disables the box */
    u8  pad0C[0x3C];
    f32 margin_x; /* 0x48 */
    f32 margin_z; /* 0x4C */
    f32 strength; /* 0x50: passed to the hit */
} HitBox;

extern struct Moby *D_L10_00178380_u[] __asm__("D_L10_00178380");
extern int collect_mobies_in_sphere(void *, float, int, void *, void *) __asm__("FUN_L00_001f2868");
extern void *memcpy(void *, const void *, u32);
extern void  spawn_nearby_effect_without_direction(
     unsigned char *m, int x, int y, void *p, float f) __asm__("FUN_L10_0024c7f0");

void spawn_linked_hitbox_contact_effects(struct Moby *moby,
                                         HitBox      *box) __asm__("FUN_L10_002c9958");

void spawn_linked_hitbox_contact_effects(struct Moby *moby, HitBox *box) {
    OvlVec4 moby_rotation_basis[3];
    OvlVec4 center;
    OvlVec4 delta;
    OvlVec4 local;
    OvlVec4 inv[4];
    OvlVec4 point;
    f32     radius;
    s32     count;
    s32     i;
    s32     j;
    s32     point_moby_count;

    if (box->flags & 0x100) {
        return;
    }
    euler_rotation_basis(moby_rotation_basis, &moby->rot);
    if (box->width > box->depth) {
        radius = box->width * 0.5f;
    } else {
        radius = box->depth * 0.5f;
    }
    qzero(&center);
    center.f[2] = radius;
    transform_vector_by_basis(&center, &center, moby_rotation_basis);
    fast_vec_add(&center, &center, &moby->pos);
    count = collect_mobies_in_sphere(&center, radius, 0x10, moby, 0);
    if (count != 0) {
        struct Moby *found[count];

        memcpy(found, D_L10_00178380_u, count * 4);
        for (i = 0; i < count; i++) {
            struct Moby *hitbox_candidate_moby = found[i];

            if (hitbox_candidate_moby == 0 || hitbox_candidate_moby->state == 0xFE ||
                hitbox_candidate_moby->state == 0xFD) {
                continue;
            }
            fast_vec_sub(&delta, &hitbox_candidate_moby->pos, &moby->pos);
            transpose_rotation_matrix(inv, moby_rotation_basis);
            transform_vector(&local, &delta, inv);
            qcopy(&point, &local);
            if (point.f[0] < -(box->width * 0.5f + box->margin_x)) {
                point.f[0] = -box->width * 0.5f;
            } else if (box->width * 0.5f + box->margin_x < point.f[0]) {
                point.f[0] = box->width * 0.5f;
            }
            if (point.f[2] < -(box->depth * 0.5f + box->margin_z)) {
                point.f[2] = -box->depth * 0.5f;
            } else if (box->depth * 0.5f + box->margin_z < point.f[2]) {
                point.f[2] = box->depth * 0.5f;
            }
            point.f[1] = 0.0f;
            transform_vector_by_basis(&point, &point, moby_rotation_basis);
            fast_vec_add(&point, &point, &moby->pos);
            point_moby_count = collect_mobies_in_sphere(&point, box->margin_x, 0x10, moby, 0);
            for (j = 0; j < point_moby_count; j++) {
                if (D_L10_00178380_u[j] == hitbox_candidate_moby) {
                    spawn_nearby_effect_without_direction((unsigned char *)hitbox_candidate_moby,
                                                          (int)moby,
                                                          0x10001,
                                                          &point,
                                                          box->strength);
                }
            }
        }
    }
}

extern int FUN_001fa6d0_2c9c70(float) __asm__("truncate_float_to_s32");
extern int FUN_001f9740_2c9c70(int *) __asm__("FUN_001f9740");
extern int D_0015ED84;

char linked_moby_warning[46] __asm__("D_L10_001DCDA0") =
    "Warning! moby 794 does not have effect moby!\n";

char linked_moby_present_message[42] __asm__("D_L10_001DCDD0") =
    "M794 %d trigger %d = moby %d (oClass %d)\n";

char linked_moby_missing_message[36] __asm__("D_L10_001DCE00") =
    "M794 %d trigger %d = NO MOBY! (-1)\n";

void linked_moby_hitbox_update(struct Moby *moby) __asm__("FUN_L10_002c9c70");

/* Moby class 794 */
void linked_moby_hitbox_update(struct Moby *moby) {
    enum {
        LINKED_HITBOX_STATE_INITIALIZE = 0,
        LINKED_HITBOX_STATE_WAIT_FOR_LINKED_MOBIES = 1,
        LINKED_HITBOX_STATE_WAIT_FOR_TARGET_STATE_OR_TIMER = 2,
        LINKED_HITBOX_STATE_REMOVE = 3
    };
    char         *linked_hitbox_vars = (char *)moby->pvars;
    int           differences = 0;
    struct Moby  *target = 0;
    int          *slot = (int *)(linked_hitbox_vars + 0x18);
    unsigned char state;

    if (*(int *)(linked_hitbox_vars + 0x10) != -1) {
        target = &D_L10_0015FFD8[*(int *)(linked_hitbox_vars + 0x10)];
    }
    if ((moby->mission == 0xff || visited52[moby->mission + (D_0015ED84 << 4)] != 0xff) &&
        moby->state != 0) {
        int i;
        for (i = 0; i < 5; i++) {
            int *entry = slot + i * 2;
            int  index = *entry;
            if (index != -1) {
                struct Moby *linked_moby = &D_L10_0015FFD8[index];
                if (linked_moby != 0 && linked_moby->oclass == *(short *)(entry + 1) &&
                    linked_moby->state != 0xfe && linked_moby->state != 0xfd) {
                    unsigned short id = linked_moby->save_id;
                    if ((((*(int *)((unsigned char *)D_0014C190_257218 + (((short)id >> 5) * 4) +
                                    (D_0015ED84 << 8)) >>
                           (id & 31)) ^
                          1) &
                         1) != 0) {
                        if (*(signed char *)((char *)entry + 6) == -1 ||
                            linked_moby->state != *(signed char *)((char *)entry + 6))
                            differences++;
                    }
                }
            }
        }
    }
    switch (state = moby->state) {
    case LINKED_HITBOX_STATE_INITIALIZE:
        /* Position flags select opposite copy directions; rotation flags have priority. */
        if (target != 0) {
            *(short *)(linked_hitbox_vars + 0x14) = target->oclass;
            *(float *)(linked_hitbox_vars + 0x44) = convert_integer_to_float(target->alpha);
            if ((*(int *)(linked_hitbox_vars + 8) & 0x10) != 0) {
                qcopy(&moby->pos, &target->pos);
            } else if ((*(int *)(linked_hitbox_vars + 8) & 0x40) != 0) {
                qcopy(&target->pos, &moby->pos);
            }
            if ((*(int *)(linked_hitbox_vars + 8) & 0x20) != 0) {
                qcopy_nc(&moby->rot, &target->rot);
            } else if ((*(int *)(linked_hitbox_vars + 8) & 0x80) != 0) {
                qcopy_nc(&target->rot, &moby->rot);
            }
        } else {
            /* Retail logs only a missing target; neither rotation flag is silent. */
            debug_print(linked_moby_warning);
        }
        {
            int *entry = slot;
            int  i;
            for (i = 0; i < 5; i++, entry += 2) {
                int index = *entry;
                if (index != -1) {
                    char          *level_moby_array = D_L10_0015FFD8_p;
                    unsigned short oclass =
                        *(unsigned short *)((index << 8) + (int)level_moby_array + 0xa6);
                    *(short *)(entry + 1) = oclass;
                    debug_print(linked_moby_present_message,
                                ((char *)moby - level_moby_array) >> 8,
                                i,
                                index,
                                (short)oclass);
                } else {
                    debug_print(
                        linked_moby_missing_message, ((char *)moby - D_L10_0015FFD8_p) >> 8, i);
                }
            }
        }
        {
            float initial = *(float *)(linked_hitbox_vars + 4);
            float now = *(float *)linked_hitbox_vars;
            float scale = moby->pclass->scale;
            float size = (now < initial ? initial * scale : now * scale);
            moby->state = 1;
            moby->scale = size;
        }
        break;
    case LINKED_HITBOX_STATE_WAIT_FOR_LINKED_MOBIES:
        if (differences == 0) {
            if (target != 0 && *(signed char *)(linked_hitbox_vars + 0x16) != -1)
                target->state = *(unsigned char *)(linked_hitbox_vars + 0x16);
            *(int *)(linked_hitbox_vars + 0x40) = *(int *)(linked_hitbox_vars + 0xc);
            moby->state = 2;
        }
        spawn_linked_hitbox_contact_effects(moby, (HitBox *)linked_hitbox_vars);
        return;
    case LINKED_HITBOX_STATE_WAIT_FOR_TARGET_STATE_OR_TIMER:
        if (target != 0) {
            if ((*(int *)(linked_hitbox_vars + 8) & 2) == 0) {
                float now = convert_integer_to_float(*(int *)(linked_hitbox_vars + 0x40));
                float initial = convert_integer_to_float(*(int *)(linked_hitbox_vars + 0xc));
                target->alpha =
                    FUN_001fa6d0_2c9c70(*(float *)(linked_hitbox_vars + 0x44) * now / initial);
            }
        }
        if ((*(int *)(linked_hitbox_vars + 8) & 4) == 0 && target != 0 &&
            *(signed char *)(linked_hitbox_vars + 0x17) != -1 &&
            target->state == *(signed char *)(linked_hitbox_vars + 0x17))
            moby->state = 3;
        if ((*(int *)(linked_hitbox_vars + 8) & 8) == 0 &&
            FUN_001f9740_2c9c70((int *)(linked_hitbox_vars + 0x40)) != 0)
            moby->state = 3;
        spawn_linked_hitbox_contact_effects(moby, (HitBox *)linked_hitbox_vars);
        return;
    case LINKED_HITBOX_STATE_REMOVE:
        if (target != 0 && (*(int *)(linked_hitbox_vars + 8) & 1) == 0)
            mark_moby_for_removal(target);
        mark_moby_for_removal(moby);
        break;
    case 4:
        break;
    }
}
