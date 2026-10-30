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

    char *moby, int index, unsigned int *type, float *result, float *radius, float *distance) {
    char         *entry;
    float         scale;
    u128          matrix[4];
    sceVu0FVECTOR first;
    sceVu0FVECTOR second;
    short        *joints;

    entry = *(char **)(moby + 0x94);
    if (entry == 0) {
        return 0;
    }
    entry += index * 0x20 + 0x10;
    *type = *(unsigned char *)entry;
    scale = MOBY(moby)->scale * 0.0009765625f;
    euler_rotation_matrix(matrix, moby + 0x40);

    if (*type < 2 || *type == 3) {
        qcopy(result, entry + 0x10);
        fast_vec_scale(result, result, scale);
        transform_vector(result, result, matrix);
        fast_vec_add(result, result, moby + 0x10);
        *radius = result[3] * scale;
        if (*type < 2) {
            *distance = 0.0f;
        } else {
            *distance = *(float *)(entry + 4) * scale;
        }
        return 1;
    } else if (*type == 2) {
        short joint = *(short *)(*(char **)(moby + 0x94) + 2);
        if (joint != 0) {
            compute_moby_joint_matrices(moby, joint, 0);
        }
        qcopy(result, (void *)(JOINT_POSITIONS + *(int *)(entry + 4) * 0x10));
        fast_vec_scale(result, result, scale);
        transform_vector(result, result, matrix);
        fast_vec_add(result, result, moby + 0x10);
        *radius = *(float *)(entry + 0xc) * scale;
        *distance = 0.0f;
        return 1;
    } else if (*type == 4) {
        short joint = *(short *)(*(char **)(moby + 0x94) + 2);
        if (joint != 0) {
            compute_moby_joint_matrices(moby, joint, 0);
        }
        joints = (short *)(entry + 4);
        scale_vector(scale, first, (void *)(JOINT_POSITIONS + joints[0] * 0x10));
        scale_vector(scale, second, (void *)(JOINT_POSITIONS + joints[1] * 0x10));
        if (first[2] < second[2]) {
            qcopy(result, first);
        } else {
            qcopy(result, second);
        }
        transform_vector(result, result, matrix);
        fast_vec_add(result, result, moby + 0x10);
        *radius = *(float *)(entry + 0xc) * scale;
        fast_vec_sub(second, second, first);
        *distance = fast_vec_length(second);
        return 1;
    } else {
        return 0;
    }
}
