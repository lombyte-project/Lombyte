/* Taxi path helpers: placement reset, path stepping, variant sounds, wobble and vector table transforms. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/ui/help/try_set_help_message.h"
#include "rnc/overlay/shared/world/level_path.h"
#include "rnc/overlay/shared/world/cuboid.h"
#include "rnc/overlay/l05/mobys.h"

typedef struct TaxiVars {
    u8   pad0[0x60];
    u8   pose_position[0x10];
    u8   pose_rotation[0x3c];
    int *route_path;
    u8   padB0[0x4];
    s16  stop_index;
    s16  destination_stop;
    u8   padB8[0x8];
    f32  path_t; /* 0xC0: 0..1 along route_path */
    f32  path_t_speed;
    f32  wobble_phase_1;
    f32  wobble_phase_2;
    f32  wobble_phase_3;
    f32  path_speed;
    f32  path_acceleration;
    s32  unkDC;
} TaxiVars;

typedef struct {
    char  pad[0x78];
    char *data;
    char  pad2[0x84];
} M_9598;

extern char *func_L05_0031AAA8_19510(void *, int) __asm__("FUN_L05_00319598");

extern unsigned short *D_L05_001ABCC0_9598[] __asm__("D_L05_001ABCC0");
extern char           *D_L05_0015FFD8_9598 __asm__("D_L05_0015FFD8");

extern float
advance_accelerated_scalar(float, float, float, float, float *, float *) __asm__("FUN_00213f38");
extern void sample_camera_path(int *, int, void *, float *, int, float) __asm__("FUN_00214e58");

extern char moby_vector_table_local_vectors[] __asm__("D_L05_00215C00");

void reset_selected_moby_to_current_placement(void *moby_v) __asm__("FUN_L05_00319510");

void reset_selected_moby_to_current_placement(void *moby_v) {
    struct Moby *moby = moby_v;
    TaxiVars    *taxi_vars = (TaxiVars *)moby->pvars;
    char        *spawn = func_L05_0031AAA8_19510(moby, taxi_vars->destination_stop);
    char        *position;
    if (spawn != 0) {
        int   index;
        char *slot = taxi_vars;
        char *placement_table_base;
        position = spawn + 0x10;
        slot += taxi_vars->stop_index << 2;
        index = *(int *)(slot + 0x80) << 7;
        placement_table_base = (char *)l05_cuboids;
        qcopy(position, (void *)(index + (int)placement_table_base + 0x30));
        qcopy(spawn + 0x40, (void *)(index + (int)placement_table_base + 0x70));
        initialize_taxi_stop_state(spawn);
    }
    taxi_vars->stop_index = *(unsigned short *)&taxi_vars->destination_stop;
}

/* Finds the pool moby in this moby's class list whose data id at 0xB4 matches. */

char *find_moby_in_owner_list_by_id(unsigned char *list_owner_moby,
                                    int            id) __asm__("FUN_L05_00319598");

char *find_moby_in_owner_list_by_id(unsigned char *list_owner_moby, int id) {
    unsigned short *moby_index_entry = D_L05_001ABCC0_9598[MOBY(list_owner_moby)->group];
    char           *pool;
    int             candidate_moby_pool_index;
    if (moby_index_entry != 0) {
        pool = D_L05_0015FFD8_9598;
        do {
            candidate_moby_pool_index = *moby_index_entry & 0x7FFF;
            if (*(short *)(((M_9598 *)pool)[candidate_moby_pool_index].data + 0xB4) == id)
                goto found;
        } while ((short)*moby_index_entry++ >= 0);
    }
    return 0;
found:
    return (char *)&((M_9598 *)D_L05_0015FFD8_9598)[candidate_moby_pool_index];
}

/* Advances a moby's path: steps the path position and returns whether it has reached the end. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_0031AB08), where it is exact; names translated to the US level program. */

int advance_moby_path(char *moby) __asm__("FUN_L05_003195f8");

int advance_moby_path(char *moby) {
    TaxiVars *moby_vars = (TaxiVars *)MOBY(moby)->pvars;
    int      *path;
    advance_accelerated_scalar(1.0f,
                               moby_vars->path_acceleration,
                               moby_vars->path_acceleration,
                               moby_vars->path_speed,
                               (float *)(&moby_vars->path_t),
                               (float *)(&moby_vars->path_t_speed));
    path = moby_vars->route_path;
    sample_camera_path(path,
                       0,
                       &moby_vars->pose_position,
                       (float *)(&moby_vars->pose_rotation),
                       0,
                       moby_vars->path_t * (float)(*path - 1));
    return moby_vars->path_t >= 1.0f;
}

/* Plays the sound that goes with the moby's current variant. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_0031ABA0), where it is exact; names translated to the US level program. */

void request_moby_variant_help_message(char *moby) __asm__("FUN_L05_00319690");

void request_moby_variant_help_message(char *moby) {
    switch (*(short *)(((char *)MOBY(moby)->pvars) + 0xB6)) {
    case 0:
        try_set_help_message(8, 0x13A5);
        break;
    case 1:
        try_set_help_message(8, 0x1398);
        break;
    case 2:
        try_set_help_message(8, 0x139A);
        break;
    case 3:
        try_set_help_message(8, 0x139C);
        break;
    case 4:
        try_set_help_message(8, 0x1399);
        break;
    default:
        try_set_help_message(8, 0x139B);
        break;
    }
}

/* Wobbles a moby's position and rotation from its sine-driven state. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_0031AC50), where it is exact; names translated to the US level program. */

void apply_moby_position_and_rotation_wobble(char *moby) __asm__("FUN_L05_00319740");

void apply_moby_position_and_rotation_wobble(char *moby) {
    TaxiVars *moby_vars = (TaxiVars *)MOBY(moby)->pvars;
    float     wobble_phase, wobble_result, wobble_phase_step;
    qcopy(&MOBY(moby)->pos, &moby_vars->pose_position);
    qcopy(moby + 0x40, &moby_vars->pose_rotation);
    wobble_phase = fast_add_rotations(moby_vars->wobble_phase_1, frame_time * 2.0943951f);
    moby_vars->wobble_phase_1 = wobble_phase;
    wobble_result = fast_sin(wobble_phase);
    wobble_phase_step = frame_time * 1.6929693f;
    MOBY(moby)->pos.z += wobble_result * 0.25f;
    wobble_phase = fast_add_rotations(moby_vars->wobble_phase_2, wobble_phase_step);
    moby_vars->wobble_phase_2 = wobble_phase;
    wobble_result = fast_add_rotations(MOBY(moby)->rot.y, fast_sin(wobble_phase) * 0.08726646f);
    wobble_phase_step = frame_time * 0.9250245f;
    MOBY(moby)->rot.y = wobble_result;
    wobble_phase = fast_add_rotations(moby_vars->wobble_phase_3, wobble_phase_step);
    moby_vars->wobble_phase_3 = wobble_phase;
    MOBY(moby)->rot.x = fast_add_rotations(MOBY(moby)->rot.x, fast_sin(wobble_phase) * 0.08726646f);
    if (MOBY(moby)->rot.x > 0.17453292f)
        MOBY(moby)->rot.x = 0.17453292f;
    else if (MOBY(moby)->rot.x < -0.17453292f)
        MOBY(moby)->rot.x = -0.17453292f;
    if (MOBY(moby)->rot.y > 0.17453292f)
        MOBY(moby)->rot.y = 0.17453292f;
    else if (MOBY(moby)->rot.y < -0.17453292f)
        MOBY(moby)->rot.y = -0.17453292f;
}

/* Transforms the 7 vectors of a level table entry into the moby's space and records its index. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_0031ADF8), where it is exact; names translated to the US level program. */

void transform_moby_vector_table_to_world_space(char *moby) __asm__("FUN_L05_003198e8");

void transform_moby_vector_table_to_world_space(char *moby) {
    TaxiVars *moby_private_vars = (TaxiVars *)MOBY(moby)->pvars;
    int       vector_table_index = moby_private_vars->unkDC;
    if (vector_table_index != -1) {
        char *vector_table_entry = ((char *)l05_paths[vector_table_index]);
        if (*(int *)vector_table_entry == 7) {
            int          i;
            struct Hero *player_hero;
            for (i = 0; i < 7; i++) {
                transform_vector_by_basis(vector_table_entry + 0x10 + i * 16,
                                          moby_vector_table_local_vectors + i * 16,
                                          moby + 0xC0);
                fast_vec_add(vector_table_entry + 0x10 + i * 16,
                             vector_table_entry + 0x10 + i * 16,
                             &MOBY(moby)->pos);
            }
            player_hero = &hero;
            *(unsigned short *)&player_hero->path_constrain_pending =
                *(unsigned short *)&moby_private_vars->unkDC;
        }
    }
}
