/* Particles: spawns a particle between an owner and a target, and a type 47 particle. */
#include "types.h"
#include "asm.h"

#include "eetypes.h"
#include "rnc/math/vector.h"
#include "sda.h"

#include "qcopy.h"
#include "rnc/math/random.h"

#include "rnc/overlay/shared/rendering/effects/spawn_owner_target_particle_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/rendering/effects/create_part.h"
#include "rnc/ui/text/load_display_text_resource_entry.h"
#include "rnc/math/pack_vector_xyz_with_shared_scale.h"

extern int owner_target_particle_frame_count_gp __asm__("D_L15_001601FC") __attribute__((sda));

/* Spawns particle type 0x1F of colour `color` at `pos`, starting a quarter of the way from `owner` towards it. */
void spawn_owner_target_particle(int   color,
                                 u128 *target_position,
                                 char *owner) __asm__("FUN_L15_0025e2c0");

void spawn_owner_target_particle(int color, u128 *target_position, char *particle_owner_moby) {
    Vec4           target;
    Vec4           step;
    u128           at;
    unsigned char *particle;
    char          *from;
    u128          *vector_scratch;
    u128          *step_vector;

    target.q = *target_position;
    particle = create_part(0x1F);
    if (particle != 0) {
        vector_scratch = &at;
        from = particle_owner_moby + 0x10;
        fast_vec_sub(vector_scratch, &target, from);
        step_vector = &step.q;
        *step_vector = *vector_scratch;
        fast_vec_scale(step_vector, step_vector, 0.25f);
        fast_vec_add(vector_scratch, from, step_vector);
        *(u128 *)(particle + 0x10) = *vector_scratch;
        *(u128 *)(particle + 0x20) = target.q;
        *(int *)(particle + 4) = color;
        *(int *)(particle + 0xC) = color & 0xFFFFFF;
        particle[9] = truncate_float_to_s32(4.0f) + 0x70;
        particle[3] = 0x44;
        particle[1] = 2;
        *(short *)(particle + 0xA) = scale_game_frames(owner_target_particle_frame_count_gp);
        *(char **)(particle + 0x30) = particle_owner_moby;
    }
}

/* Ported from rac1-decomp (src/overlays/shared/partupd_0025F0A0.c: func_L15_002655D0), where it is exact; names translated to the US level program. */

extern int *D_L15_001B2770 NOT_SDA;
unsigned char             *create_type_47_particle(void  *particle_position,
                                                   float *p_vector,
                                                   float *c_vector,
                                                   int    particle_color,
                                                   int    e,
                                                   int    particle_frame_count,
                                                   int    g,
                                                   int    h,
                                                   char  *owner) __asm__("FUN_L15_002647f0");

unsigned char *create_type_47_particle(void  *particle_position,
                                       float *p_vector,
                                       float *c_vector,
                                       int    particle_color,
                                       int    secondary_particle_color,
                                       int    particle_frame_count,
                                       int    g,
                                       int    h,

                                       char *particle_owner) {
    unsigned char *particle = create_part(0x47);
    if (particle != 0) {
        int  *packed_p_vector, *packed_c_vector;
        float p_vector_scratch[4], c_vector_scratch[4];
        qcopy(particle + 0x10, particle_position);
        packed_p_vector = (int *)(particle + 0x20);
        packed_c_vector = (int *)(particle + 0x24);
        *(int *)(particle + 4) = particle_color;
        particle[9] = truncate_float_to_s32(4.0f) + 0x40;
        particle[3] = 0x44;
        particle[1] = 0;
        *(float *)(particle + 0xC) = truncate_float_to_s32(p_vector[3] * 210000.0f);
        particle[8] = random_integer_below(0xFF);
        particle[2] = *(unsigned char *)D_L15_001B2770;
        *(short *)(particle + 0xA) = particle_frame_count;
        pack_vector_xyz_with_shared_scale(p_vector, packed_p_vector);
        pack_vector_xyz_with_shared_scale(c_vector, packed_c_vector);
        load_display_text_resource_entry(p_vector_scratch, packed_p_vector);
        load_display_text_resource_entry(c_vector_scratch, packed_c_vector);
        packed_p_vector[2] = particle_color;
        packed_p_vector[3] = secondary_particle_color;
        *(short *)(packed_p_vector + 4) = particle_frame_count;
        *(short *)((char *)packed_p_vector + 0x12) = g;
        *(short *)((char *)packed_p_vector + 0x14) = h;
        packed_p_vector[6] = *(int *)(particle_owner + 0x78);
    }
    return particle;
}

s32 D_L17_001B2A70[1] __attribute__((section(".data"))) = {0};

int D_L18_001601FC = 0x5a;

int                 owner_target_particle_frame_count_gp __asm__("D_L15_001601FC") = 0x5a;
int *D_L15_001B2770 NOT_SDA = 0;
