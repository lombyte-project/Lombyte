/* Shared part of the bolt thief class (l14/mobys/bolt_thief.c, l16/mobys/bolt_thief.c): its path setup, linked moby placement and exhaust particles. */
#include "types.h"
#include "rnc/overlay/level_path.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/math/random.h"
#include "qcopy.h"

/* Recomputes a path's segment lengths, aims the moby along its first segment, and recomputes a second path. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_002FFD88), where it is exact; names translated to the US level program. */

void initialize_moby_path_state(char *moby) __asm__("FUN_L14_002fe900");

void initialize_moby_path_state(char *moby) {
    char *moby_path_state = ((char *)MOBY(moby)->pvars);
    char *path;
    int   i;
    path = ((int *)l14_paths[*(int *)(moby_path_state + 0xD8)]);
    for (i = 0; i < *(int *)path - 1; i++) {
        *(float *)(path + 0x1C + i * 0x10) =
            fast_vec_dist(path + 0x10 + i * 0x10, path + 0x20 + i * 0x10);
    }
    *(int *)(moby_path_state + 0xD0) = 0;
    *(int *)(moby_path_state + 0xD4) = 0;
    qcopy(moby + 0x10, path + 0x10);
    MOBY(moby)->rot.z = fast_arc_tan(*(float *)(path + 0x20) - *(float *)(path + 0x10),
                                     *(float *)(path + 0x24) - *(float *)(path + 0x14));
    *(float *)(moby_path_state + 0xF0) = *(float *)(moby_path_state + 0xE4) =
        *(float *)(moby_path_state + 0xE8) = *(float *)(moby_path_state + 0xEC) = 0.0f;
    if (*(int *)(moby_path_state + 0xFC) != 1) {
        path = ((int *)l14_paths[*(int *)(moby_path_state + 0xDC)]);
        for (i = 0; i < *(int *)path - 1; i++) {
            *(float *)(path + 0x1C + i * 0x10) =
                fast_vec_dist(path + 0x10 + i * 0x10, path + 0x20 + i * 0x10);
        }
    }
}
/* rebuilds two vectors from the moby's transform and applies them to its linked object */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_003000B0), where it is exact; names translated to the US level program. */

float linked_moby_initial_offset_length_gp __asm__("D_L14_00161FC4") = 0.1f;
float linked_moby_additional_offset_length_gp __asm__("D_L14_00161FC8") = 0.1f;
void  update_linked_moby_position(char *moby) __asm__("FUN_L14_002fec28");

void update_linked_moby_position(char *moby) {
    char *moby_private_vars = ((char *)MOBY(moby)->pvars);
    float position_offset[4];
    if (*(char **)(moby_private_vars + 0xF4) != 0) {
        char *linked_position;
        fast_vec_normalize(position_offset, moby + 0xC0, linked_moby_initial_offset_length_gp);
        fast_vec_add(*(char **)(moby_private_vars + 0xF4) + 0x10, moby + 0x10, position_offset);
        fast_vec_normalize(position_offset, moby + 0xE0, linked_moby_additional_offset_length_gp);
        linked_position = *(char **)(moby_private_vars + 0xF4) + 0x10;
        fast_vec_add(linked_position, linked_position, position_offset);
    }
}

typedef struct {
    unsigned char pad0[4];
    int           w4;
    unsigned char pad8[2];
    unsigned char bA;
    unsigned char bB;
} T_eca8;

typedef struct {
    unsigned char pad0[8];
    unsigned char b8;
    unsigned char b9;
    short         hA;
    unsigned char padC[0x14];
    T_eca8        tail;
} P_eca8;

extern P_eca8 *FUN_L00_0026d000(void *, float, float, float, int, void *, float, int);

/* Exhaust puffs from the moby's nozzle: two coloured sparks and three white puffs of shrinking size. */
extern P_eca8 *
spawn_puff(void *, float, float, float, int, void *, float, int) __asm__("FUN_L00_0026d000");
extern float D_L14_0015F580_macro[] __asm__("D_L14_0015F580") MACRO_ADDR;

float      particle_pair_moby_c0_offset_scale_gp __asm__("D_L14_00161FCC") = -0.5f;
float      particle_pair_moby_e0_offset_scale_gp __asm__("D_L14_00161FD0") = 0.6f;
float      particle_trio_moby_c0_offset_scale_gp __asm__("D_L14_00161FD4") = -0.4f;
float      particle_trio_moby_e0_offset_scale_gp __asm__("D_L14_00161FD8") = 0.7f;
extern int particle_pair_color_gp __asm__("D_L14_00161FE4") __attribute__((sda));
void       emit_moby_offset_particles(unsigned char *moby) __asm__("FUN_L14_002feca8");

void emit_moby_offset_particles(unsigned char *moby) {
    float emission_position_offset[4];
    float particle_spawn_position[4];
    int   i;
    int   j;
    int   signed_particle_variation;
    int   life;
    float size;
    fast_vec_scale(emission_position_offset, moby + 0xC0, particle_pair_moby_c0_offset_scale_gp);
    fast_vec_add(particle_spawn_position, moby + 0x10, emission_position_offset);
    fast_vec_scale(emission_position_offset, moby + 0xE0, particle_pair_moby_e0_offset_scale_gp);
    fast_vec_add(particle_spawn_position, particle_spawn_position, emission_position_offset);
    for (i = 0; i < 2; i++) {
        P_eca8 *part;
        T_eca8 *tail;
        signed_particle_variation = random_integer_below(0x10);
        part = spawn_puff(particle_spawn_position,
                          0.2f,
                          1.0f,
                          0.9f,
                          random_integer_below(2) == 0 ? signed_particle_variation
                                                       : -signed_particle_variation,
                          D_L14_0015F580_macro,
                          100000.0f,
                          particle_pair_color_gp);
        if (part) {
            tail = &part->tail;
            part->hA = scale_game_frames(0xC);
            tail->w4 = 2;
            tail->bA = 0x7F;
            tail->bB = part->hA;
        }
    }
    fast_vec_scale(emission_position_offset, moby + 0xC0, particle_trio_moby_c0_offset_scale_gp);
    fast_vec_add(particle_spawn_position, moby + 0x10, emission_position_offset);
    fast_vec_scale(emission_position_offset, moby + 0xE0, particle_trio_moby_e0_offset_scale_gp);
    fast_vec_add(particle_spawn_position, particle_spawn_position, emission_position_offset);
    signed_particle_variation = 0x10;
    life = scale_game_frames(2);
    size = 80000.0f;
    for (j = 0; j < 3; j++) {
        P_eca8 *part;
        T_eca8 *tail;
        part = spawn_puff(particle_spawn_position,
                          0.05f,
                          1.0f,
                          1.0f,
                          signed_particle_variation,
                          D_L14_0015F580_macro,
                          size,
                          0x7FFFFFFF);
        if (part) {
            tail = &part->tail;
            part->hA = life;
            part->b8 = random_integer_below(0xFF);
            tail->w4 = 2;
            tail->bA = 0x7F;
            tail->bB = part->hA;
        }
        signed_particle_variation = -signed_particle_variation;
        size -= 20000.0f;
        life *= 2;
    }
}

float D_L16_00161D9C = 0.1f;
float D_L16_00161DA0 = 0.1f;
float D_L16_00161DA4 = -0.5f;
float D_L16_00161DA8 = 0.6f;
float D_L16_00161DAC = -0.4f;
float D_L16_00161DB0 = 0.7f;
int   D_L16_00161DBC = 0x7f204080;

int particle_pair_color_gp __asm__("D_L14_00161FE4") = 0x7f204080;
