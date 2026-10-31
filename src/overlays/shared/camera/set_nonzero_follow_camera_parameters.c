/* Setters for the follow camera's control word, offsets and position triplets. */

#include "types.h"
#include "rnc/camera/follow_cam.h"
#include "rnc/mobys/moby.h"

extern char *D_L02_00167400;
extern char *D_L02_00167400_lui __asm__("D_L02_00167400") NOT_SDA;

/* Store the selected value only while this level object is inactive. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002F79A8), where it is exact; names translated to the US level program. */
void set_follow_camera_control_word(int value) __asm__("FUN_L02_002f6570");

void set_follow_camera_control_word(int control_word) {
    char *moby = D_L02_00167400;
    if (MOBY(moby)->unk86 == 0) {
        char *follow_camera_data = *(char **)(moby + 0x70);
        *(int *)(follow_camera_data + 0x218) = control_word;
    }
}

/* Update the active object's coordinates without replacing zero fields. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002F79D0), where it is exact; names translated to the US level program. */
void set_nonzero_follow_camera_parameters(float x, float y, float z) __asm__("FUN_L02_002f6598");

void set_nonzero_follow_camera_parameters(float x, float y, float z) {
    char *moby = D_L02_00167400_lui;
    if (MOBY(moby)->unk86 == 0) {
        char *follow_camera_parameter_base = *(char **)(moby + 0x70) + 0x1D0;
        if (*(float *)(follow_camera_parameter_base + 0x3C) != 0.0f) {
            *(float *)(follow_camera_parameter_base + 0x3C) = x;
        }
        if (*(float *)(follow_camera_parameter_base + 0x40) != 0.0f) {
            *(float *)(follow_camera_parameter_base + 0x40) = y;
        }
        if (*(float *)(follow_camera_parameter_base + 0x44) != 0.0f) {
            *(float *)(follow_camera_parameter_base + 0x44) = z;
        }
    }
}

/* stores two coordinate triples in the selected object data */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002F9ED8), where it is exact; names translated to the US level program. */
void set_selected_object_float_triplets(
    float a, float b, float c, float d, float e, float f) __asm__("FUN_L02_002f8aa0");

void set_selected_object_float_triplets(float first_triplet_middle,
                                        float first_triplet_first,
                                        float first_triplet_last,
                                        float second_triplet_middle,
                                        float second_triplet_first,

                                        float second_triplet_last) {
    char *moby = D_L02_00167400_lui;
    char *selected_object_data = *(char **)(moby + 0x70);
    char *second_triplet_store_base = selected_object_data + 0x30;
    *(float *)(second_triplet_store_base + 0x20) = second_triplet_first;
    *(float *)(second_triplet_store_base + 0x24) = second_triplet_middle;
    *(float *)(second_triplet_store_base + 0x28) = second_triplet_last;
    {
        char *first_triplet_store_base = *(char **)(moby + 0x70);
        *(float *)(first_triplet_store_base + 0x10) = first_triplet_first;
        *(float *)(first_triplet_store_base + 0x14) = first_triplet_middle;
        *(float *)(first_triplet_store_base + 0x18) = first_triplet_last;
    }
}
