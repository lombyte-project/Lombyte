/* Level 15 poison vessel moby class and its spawn helper. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "qcopy.h"
#include "rnc/overlay/quad.h"
#include "rnc/game/level_state.h"
#include "rnc/overlay/l15/mobys/poison_vessel_globals.h"

extern void *create_moby() __asm__("FUN_0020c4f8");

extern void update_entity(void *, void *) __asm__("FUN_L00_00250df8");

char *spawn_poison_vessel(void *unused, void *vector) __asm__("FUN_L15_002e7ed8");

char *spawn_poison_vessel(void *unused, void *vector) {
    char *moby = create_moby(0x4E9);
    if (moby != 0) {
        char *data = ((char *)MOBY(moby)->pvars);
        char *field;
        ((unsigned char *)moby)[0x30] = 0xFF;
        MOBY(moby)->draw_dist = 0x7F;
        moby[0x31] = 1;
        moby[0x20] = 0;
        moby[0xBC] = 0;
        *(int *)(data + 0x10) = 0;
        clear_u64_value(data);
        qcopy(moby + 0x10, vector);
        field = ((char *)MOBY(moby)->pclass);
        MOBY(moby)->collision = *(int *)(field + 0x10);
        update_entity(moby, field);
    }
    return moby;
}

typedef union Vec {
    OvlQuad q;
    float   f[4];
} Vec;

typedef struct Data {
    float               f0;
    float               f4;
    float               f8;
    float               fC;
    struct Moby_2E9358 *target; /* 0x10 */
} Data;

typedef struct Moby_2E9358 {
    char          pad0[0x10];
    OvlQuad       pos;   /* 0x10 */
    unsigned char state; /* 0x20 */
    char          pad21[0x1F];
    OvlQuad       v40; /* 0x40 */
    char          pad50[0x28];
    Data         *data; /* 0x78 */
    char          pad7C[0x28];
    unsigned char bA4;
    char          padA5;
    short         hA6;
    char          padA8[0x14];
    unsigned char bBC;
} Moby_2E9358;

typedef struct Res {
    char         pad0[0x20];
    Moby_2E9358 *owner; /* 0x20 */
} Res;

typedef struct Tb {
    char    pad0[0x18];
    int     f18;
    int     f1C;
    OvlQuad v20;
    OvlQuad v30;
    OvlQuad v40;
} Tb;

extern float probe_ground_height(void *, int, float) __asm__("func_00213508");

extern float D_0015ED6C;

extern float poison_vessel_gravity_gp __asm__("D_L15_00161FBC") __attribute__((sda));

extern int collision_line(void *, void *, int, void *, void *) __asm__("FUN_001efa68");

extern void add_vectors(void *, void *, void *) __asm__("FUN_L00_001ff290");

extern void mark_moby_for_removal(void *) __asm__("func_0020C828");

extern char D_L15_001744C0_2E9358[] __asm__("D_L15_00174440");

extern int D_L15_00174458_lui[] __asm__("D_L15_00174458") NOT_SDA;

extern int D_001413D0_lui __asm__("D_001413D0") NOT_SDA;

extern s32 D_L15_00178500[] __asm__("D_L15_00178500");

extern int coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");

extern char *FUN_L00_002603d0(int);

extern void func_L00_001FF610_2E9358(void *, void *, void *) __asm__("FUN_L00_001ff660");

extern int
probe_ground_height_cf(void *, void *, void *, void *, void *, void *) __asm__("FUN_L00_00260400");

extern void func_L00_0025F4A8_2E9358(void *,
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

extern int trooper_play_sound(int, int, void *, int) __asm__("FUN_L00_0028dc90");

extern int probe_world_sphere(void *, int, void *, void *, float) __asm__("FUN_L00_001f2868");

extern void
mine_blast_mobys(void *, void *, void *, int, int, int, int, int, float, float, float) __asm__("FUN_L00_0025a9f8");

void poison_vessel_update(Moby_2E9358 *moby) __asm__("FUN_L15_002e7f68");

void poison_vessel_update(Moby_2E9358 *moby) {
    enum {
        POISON_VESSEL_FREE_FLIGHT = 0,
        POISON_VESSEL_FOLLOW_TARGET = 1
    };
    Vec           previous_position;
    Vec           collision_scratch_vector;
    Vec           temporary_vector;
    Data         *data = moby->data;
    char         *follow_context = 0;
    unsigned char state;
    int           blast_candidate_count;
    Res          *hit_result;

    previous_position.q = moby->pos;
    clear_u64_value(&collision_scratch_vector);
    if (coll_sphere(0.5f, &moby->pos, 1, moby)) {
        if (D_L15_00174458_lui[0] == 0 || FUN_L00_002603d0(D_L15_00174458_lui[0]) == 0) {
            moby->bBC = 1;
        }
    }
    hit_result = find_moby_hit(moby, 0x830000, 0);
    if (hit_result != 0 && hit_result->owner != 0 && hit_result->owner->hA6 != moby->hA6) {
        moby->bBC = 1;
    }
    moby->bA4 = 0xFF;

    state = moby->state;
    switch (state) {
    case POISON_VESSEL_FREE_FLIGHT:
        data->f8 = data->f8 - poison_vessel_gravity_gp * frame_time_sq;
        add_vectors(&temporary_vector, &moby->pos, data);
        if (collision_line(&previous_position, &moby->pos, 0x10, moby, 0)) {
            if (((Tb *)D_L15_001744C0_2E9358)->f18 != 0 &&
                FUN_L00_002603d0(((Tb *)D_L15_001744C0_2E9358)->f18) != 0) {
                data->target = (Moby_2E9358 *)((Tb *)D_L15_001744C0_2E9358)->f18;
                moby->state = 1;
                data->f8 = 0.0f;
                moby->pos = ((Tb *)D_L15_001744C0_2E9358)->v20;
            } else if (((Tb *)D_L15_001744C0_2E9358)->f18 == D_001413D0_lui ||
                       data->f8 < D_0015ED6C * -9.8f) {
                moby->bBC = 1;
                qcopy(&moby->pos, &((Tb *)D_L15_001744C0_2E9358)->v20);
                func_L00_001FF610_2E9358(
                    &collision_scratch_vector, data, &((Tb *)D_L15_001744C0_2E9358)->v40);
                fast_vec_normalize(
                    &collision_scratch_vector, &collision_scratch_vector, D_0015ED6C + D_0015ED6C);
            }
        }
        break;
    case POISON_VESSEL_FOLLOW_TARGET:
        probe_ground_height(&moby->pos, 0, 0.5f);
        if (data->target != 0) {
            Moby_2E9358 *target_moby = data->target;

            if (target_moby->state == 0xFE || data->target->state == 0xFD) {
                moby->bBC = state;
                data->target = 0;
            } else if (D_L15_00174458_lui[0] != 0) {
                follow_context = FUN_L00_002603d0(D_L15_00174458_lui[0]);
                if (follow_context != 0) {
                    probe_ground_height_cf(
                        moby, data->target, &moby->pos, &moby->v40, &moby->pos, &moby->v40);
                    fast_vec_sub(&temporary_vector, &moby->pos, &previous_position);
                    *(OvlQuad *)data = temporary_vector.q;
                }
            }
        }
        if (follow_context == 0) {
            moby->state = 0;
            data->f8 = 0.0f;
            data->target = 0;
        }
        break;
    }

    if (moby->bBC != 0) {
        func_L00_0025F4A8_2E9358(
            moby, data, 0, 0.0f, 0.0f, 3, 3, 5, 2.0f, 1.0f, 4.0f, 1.0f, -1, 7.0f, 0, 1, -1, 0);
        trooper_play_sound(0, 0, moby, 0x79);
        blast_candidate_count = probe_world_sphere(&moby->pos, 0x10, moby, 0, 2.0f);
        temporary_vector.q = moby->pos;
        mine_blast_mobys(moby,
                         &temporary_vector,
                         D_L15_00178500,
                         blast_candidate_count,
                         0,
                         0x830001,
                         2,
                         1,
                         1.0f,
                         1.0f,
                         1.0f);
        mark_moby_for_removal(moby);
    }
}

extern char *spawn_poison_vessel(void *, void *) __asm__("FUN_L15_002e7ed8");
