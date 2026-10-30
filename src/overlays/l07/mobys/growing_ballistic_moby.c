/* Level 07 moby class 0x419: growing ballistic moby. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/game/item_state.h"
#include "qcopy.h"

extern char *create_moby(int) __asm__("FUN_0020c4f8");
extern void  update_entity(void *) __asm__("FUN_L00_00250df8");
extern float random_angle_radians(void) __asm__("FUN_00213308");

/* Spawn the class 0x419 effect moby at a position with a data block, and start its particle. */
/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_0030F750), where it is exact; names translated to the US level program. */
extern char *spawn_source_attached_fading_effect_particle(
    char *src, int col, int w, int v, float x) __asm__("FUN_L00_0026daa0");
char       *spawn_growing_ballistic_moby(char *spawn_position,
                                         char *initial_velocity,
                                         int   spawning_moby) __asm__("FUN_L07_0030e370");
extern int  allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
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
extern int  collision_line(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern int  allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
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
extern int D_L07_0015F5CC MACRO_ADDR;
extern void               spawn_explosion(void *,
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
extern int                D_L07_0015F5CC;
extern void spawn_moby_point_offset_particle(char *, int, float, float) __asm__("FUN_L07_0029b8d8");
void        mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");
extern int  func_L00_001F10E0_f(float, void *, int, int) __asm__("FUN_L00_001f0d60");
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
extern char *D_L07_00173ED0[];
void         update_growing_ballistic_moby(struct Moby *moby) __asm__("FUN_L07_0030e458");
s32          allocate_voice_for_target_entry(s32   entry_index,
                                             s32   flags,
                                             void *target) __asm__("FUN_0022da68");

char *

spawn_growing_ballistic_moby(char *spawn_position, char *initial_velocity, int spawning_moby) {
    char *spawned_moby = create_moby(0x419);
    if (spawned_moby != 0) {
        char *data;
        MOBY(spawned_moby)->draw_dist = MOBY(spawned_moby)->update_dist = 0xFF;
        spawned_moby[0x31] = 1;
        data = ((char *)MOBY(spawned_moby)->pvars);
        MOBY(spawned_moby)->rot.x = random_angle_radians();
        MOBY(spawned_moby)->rot.y = random_angle_radians();
        MOBY(spawned_moby)->rot.z = random_angle_radians();
        qcopy(spawned_moby + 0x10, spawn_position);
        qcopy(data, initial_velocity);
        *(int *)(data + 0x10) = spawning_moby;
        spawned_moby[0x23] = 0x30;
        MOBY(spawned_moby)->flags |= 0x200;
        update_entity(spawned_moby);
        spawn_source_attached_fading_effect_particle(spawned_moby, 0x800040FF, 0x80, -1, 500000.0f);
    }
    return spawned_moby;
}

void update_growing_ballistic_moby(struct Moby *moby) {
    char *moby_data = (char *)moby->pvars;
    float previous_position[4];
    qcopy(previous_position, &moby->pos);
    fast_vec_add(&moby->pos, &moby->pos, moby_data);
    {
        float gravity_step = frame_time_sq * 9.8f;
        float rotation_z_step = frame_time * 6.2831855f;
        *(float *)(moby_data + 8) -= gravity_step;
        moby->rot.z += rotation_z_step;
    }
    if (moby->scale < moby->pclass->scale * 2.8f) {
        moby->scale *= 1.2f;
    }
    if (D_L07_0015F5CC % 3 == 0) {
        spawn_moby_point_offset_particle(moby, 0, 400000.0f, 5.0f);
    }
    if (collision_line(previous_position, &moby->pos, 0, *(int *)(moby_data + 0x10), 0) != 0 ||
        func_L00_001F10E0_f(0.1f, &moby->pos, 0, *(int *)(moby_data + 0x10)) != 0) {
        char *class_filter_moby = D_L07_00173ED0[2];
        if (class_filter_moby == 0 ||
            (MOBY(class_filter_moby)->oclass != 0x415 && MOBY(class_filter_moby)->oclass != 0x419 &&
             MOBY(class_filter_moby)->oclass != 0x452)) {
            if (moby->visible != 0) {
                spawn_explosion(moby,
                                moby_data,
                                0,
                                2.0f,
                                1.0f,
                                0,
                                3,
                                5,
                                0.0f,
                                0.0f,
                                1000.0f,
                                1.2f,
                                0,
                                10.0f,
                                0,
                                1,
                                -1,
                                0);
            } else {
                allocate_voice_for_target_entry(0, 0, (int)moby);
            }
            mark_moby_for_removal(moby);
            return;
        }
    }
    if (moby->pos.x < 2.0f || 1021.0f < moby->pos.x || moby->pos.y < 2.0f ||
        1021.0f < moby->pos.y || moby->pos.z < 2.0f || 1021.0f < moby->pos.z) {
        mark_moby_for_removal(moby);
    }
}
