/* Level 18 moby class child_carrier (0x630): keeps its child moby on its own transform, and the group helpers that flag or remove it. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "qcopy.h"

/* updates a moby that drags a child moby along with it */
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F9D48.c: func_L18_002FBB88), where it is exact; names translated to the US level program. */

extern char *create_moby(int);
extern char *create_moby(int) __asm__("FUN_0020c4f8");

void child_carrier_update(char *moby) __asm__("FUN_L18_002fa728");

typedef struct ChildCarrierVars {
    u8    pad_0[0x20];
    f32   entity_matrix[4];
    u8    pad_30[0x30];
    f32   previous_position[4];
    u8    previous_rotation;
    u8    pad_71[0xF];
    s32   spawns_child; /* nonzero: creates the class 0x764 child to carry */
    char *carried_child_moby;
} ChildCarrierVars;

/* Moby class 1584 */
void child_carrier_update(char *moby) {
    enum {
        INITIALIZE_TRANSFORM_HISTORY = 0,
        UPDATE_TRANSFORM_FROM_HISTORY = 1
    };
    ChildCarrierVars *transform_and_child_vars = (ChildCarrierVars *)MOBY(moby)->pvars;
    char              position_delta[16];
    char             *child;
    switch (MOBY(moby)->state) {
    case INITIALIZE_TRANSFORM_HISTORY:
        MOBY(moby)->state = UPDATE_TRANSFORM_FROM_HISTORY;
        break;
    case UPDATE_TRANSFORM_FROM_HISTORY:
        fast_vec_sub(position_delta, moby + 0x10, transform_and_child_vars->previous_position);
        update_entity_matrix(transform_and_child_vars->entity_matrix,
                             position_delta,
                             &transform_and_child_vars->previous_rotation,
                             moby + 0x40);
        break;
    }
    if (MOBY(moby)->command != 0 && transform_and_child_vars->spawns_child != 0 &&
        *(int *)&transform_and_child_vars->carried_child_moby == 0) {
        char *new_child = create_moby(0x764);
        transform_and_child_vars->carried_child_moby = new_child;
        if (new_child != 0) {
            *(unsigned short *)(new_child + 0x32) = ((unsigned short)MOBY(moby)->draw_dist);
            (transform_and_child_vars->carried_child_moby)[0x31] = 1;
            *(long *)(transform_and_child_vars->carried_child_moby + 0x38) =
                ((long)MOBY(moby)->spawn_frame);
            *(float *)(transform_and_child_vars->carried_child_moby + 0x2C) *=
                MOBY(moby)->scale / MOBY(moby)->pclass->scale;
        }
    }
    qcopy(transform_and_child_vars->previous_position, moby + 0x10);
    qcopy(&transform_and_child_vars->previous_rotation, moby + 0x40);
    child = transform_and_child_vars->carried_child_moby;
    if (child != 0) {
        qcopy(child + 0x10, moby + 0x10);
        qcopy(child + 0x40, moby + 0x40);
        update_entity(child);
    }
}

extern int D_L18_001AC240[];
extern int D_L18_0015FFD8;
/* 0x002fa888, 88 bytes.
 * Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F9D48.c:
 * func_L18_002FBCE8), where it is exact; names translated to the US level
 * program. */
void set_group_child_carrier_byte_bc(int group_index, int value) __asm__("FUN_L18_002fa888");

void set_group_child_carrier_byte_bc(int group_index, int moby_byte_bc_value) {
    unsigned short *table = (unsigned short *)D_L18_001AC240[group_index];

    if (table != 0) {
        unsigned short *group_moby_entry = table;
        do {
            unsigned char *moby =
                (unsigned char *)(((group_moby_entry[0] & 0x7FFF) << 8) + D_L18_0015FFD8);

            if (MOBY(moby)->oclass == 0x630) {
                MOBY(moby)->command = moby_byte_bc_value;
            }
        } while ((short)*group_moby_entry++ >= 0);
    }
}

extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");

/* Remove a vendor moby and any child it owns. */
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F9D48.c: func_L18_002FBD40), where it is exact; names translated to the US level program. */

typedef struct {
    char  pad0[0x84];
    void *child;
} L18VD_a8e0;

typedef struct {
    char        pad0[0x78];
    L18VD_a8e0 *data;
    char        pad7C[0x2A];
    short       class_id;
} L18VM_a8e0;

void remove_child_carrier_and_child(L18VM_a8e0 *moby) __asm__("FUN_L18_002fa8e0");

void remove_child_carrier_and_child(L18VM_a8e0 *moby) {
    if (moby->class_id == 0x630) {
        void *child = moby->data->child;
        if (child != 0) {
            mark_moby_for_removal(child);
        }
        mark_moby_for_removal(moby);
    }
}
