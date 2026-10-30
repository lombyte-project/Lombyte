/* Mobys: sets up a rig probe. */
#include "types.h"
#include "asm.h"
#include "eetypes.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "qcopy.h"
#include "rnc/mobys/moby.h"

extern void scale_vector(float scale, void *out, void *joint) __asm__("FUN_001f9a80");
extern void compute_moby_joint_matrices(void *moby, short joint, int flags) __asm__("FUN_0020fa90");

/* Joint positions in scratchpad, 16 bytes each. */
#define JOINT_POSITIONS 0x70000000

int rig_probe_setup(char         *moby,
                    int           index,
                    unsigned int *type,
                    float        *result,
                    float        *radius,
                    float        *distance) __asm__("FUN_L01_00263a28");
int rig_probe_setup(

    char         *moby,
    int           index,
    unsigned int *type,
    float        *probe_position,
    float        *radius,

    float *distance) {
    char         *probe_entry;
    float         scale;
    u128          matrix[4];
    sceVu0FVECTOR first_joint_position;
    sceVu0FVECTOR second_joint_position;
    short        *joints;

    probe_entry = *(char **)(moby + 0x94);
    if (probe_entry == 0) {
        return 0;
    }
    probe_entry += index * 0x20 + 0x10;
    *type = *(unsigned char *)probe_entry;
    scale = MOBY(moby)->scale * 0.0009765625f;
    euler_rotation_matrix(matrix, moby + 0x40);

    if (*type < 2 || *type == 3) {
        qcopy(probe_position, probe_entry + 0x10);
        fast_vec_scale(probe_position, probe_position, scale);
        transform_vector(probe_position, probe_position, matrix);
        fast_vec_add(probe_position, probe_position, moby + 0x10);
        *radius = probe_position[3] * scale;
        if (*type < 2) {
            *distance = 0.0f;
        } else {
            *distance = *(float *)(probe_entry + 4) * scale;
        }
        return 1;
    } else if (*type == 2) {
        short joint = *(short *)(*(char **)(moby + 0x94) + 2);
        if (joint != 0) {
            compute_moby_joint_matrices(moby, joint, 0);
        }
        qcopy(probe_position, (void *)(JOINT_POSITIONS + *(int *)(probe_entry + 4) * 0x10));
        fast_vec_scale(probe_position, probe_position, scale);
        transform_vector(probe_position, probe_position, matrix);
        fast_vec_add(probe_position, probe_position, moby + 0x10);
        *radius = *(float *)(probe_entry + 0xc) * scale;
        *distance = 0.0f;
        return 1;
    } else if (*type == 4) {
        short joint = *(short *)(*(char **)(moby + 0x94) + 2);
        if (joint != 0) {
            compute_moby_joint_matrices(moby, joint, 0);
        }
        joints = (short *)(probe_entry + 4);
        scale_vector(scale, first_joint_position, (void *)(JOINT_POSITIONS + joints[0] * 0x10));
        scale_vector(scale, second_joint_position, (void *)(JOINT_POSITIONS + joints[1] * 0x10));
        if (first_joint_position[2] < second_joint_position[2]) {
            qcopy(probe_position, first_joint_position);
        } else {
            qcopy(probe_position, second_joint_position);
        }
        transform_vector(probe_position, probe_position, matrix);
        fast_vec_add(probe_position, probe_position, moby + 0x10);
        *radius = *(float *)(probe_entry + 0xc) * scale;
        fast_vec_sub(second_joint_position, second_joint_position, first_joint_position);
        *distance = fast_vec_length(second_joint_position);
        return 1;
    } else {
        return 0;
    }
}
