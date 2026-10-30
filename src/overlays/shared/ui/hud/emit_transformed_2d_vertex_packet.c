/* Queues a GIF packet that draws a line strip through 2D points, rotated, scaled and offset on the screen (fighter jet HUD markers). */
#include "types.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "qcopy.h"
#include "rnc/rendering/screen.h"

typedef unsigned int u128_7d50 __attribute__((mode(TI)));

extern int       D_L13_001611C0;
extern u128_7d50 l13_transformed_2d_primitive_setup_template __asm__("D_L13_001608E0");
extern u128_7d50 l13_transformed_2d_vertex_header_template __asm__("D_L13_001608F0");
void             emit_transformed_2d_vertex_packet(short             *pts,
                                                   int                point_count,
                                                   float              x0,
                                                   float              y0,
                                                   unsigned int       col,
                                                   unsigned long long prim,
                                                   float              scale,
                                                   float z_rotation_angle) __asm__("FUN_L13_002b7d50");

void emit_transformed_2d_vertex_packet(short             *pts,
                                       int                point_count,
                                       float              x0,
                                       float              y0,
                                       unsigned int       col,
                                       unsigned long long prim,
                                       float              scale,

                                       float z_rotation_angle) {
    float               rotation_matrix[4][4];
    float               rotation_angles[4];
    float               transformed_point[4];
    float               input_point[4];
    int                 payload_qword_count = (point_count + 1) / 2 + 3;
    char               *t;
    unsigned long long *vertex_output;
    int                 x, y;
    int                 i;
    *(int *)(D_L13_001611C0 + 0) = payload_qword_count | 0x10000000;
    *(int *)(D_L13_001611C0 + 4) = 0;
    *(int *)(D_L13_001611C0 + 8) = 0;
    *(int *)(D_L13_001611C0 + 12) = payload_qword_count | 0x50000000;
    {
        char *packet_header = (char *)D_L13_001611C0;
        D_L13_001611C0 = (int)(packet_header + 0x10);
        qcopy((void *)D_L13_001611C0, &l13_transformed_2d_primitive_setup_template);
        *(short *)(packet_header + 0x10) = -0x7FFF;
    }
    {
        unsigned long long *primitive_payload =
            (unsigned long long *)((char *)D_L13_001611C0 + 0x10);
        D_L13_001611C0 = (int)primitive_payload;
        primitive_payload[0] = 0x144;
        primitive_payload[1] = prim;
    }
    {
        char *saved_packet_cursor = (char *)D_L13_001611C0;
        D_L13_001611C0 = (int)(saved_packet_cursor + 0x10);
        qcopy((void *)D_L13_001611C0, &l13_transformed_2d_vertex_header_template);
        *(short *)(saved_packet_cursor + 0x10) = point_count - 0x8000;
    }
    *(u128_7d50 *)rotation_angles = 0;
    rotation_angles[2] = z_rotation_angle;
    D_L13_001611C0 += 0x10;
    vertex_output = (unsigned long long *)D_L13_001611C0;
    euler_rotation_matrix(rotation_matrix, rotation_angles);
    for (i = 0; i < point_count; i++) {
        *(u128_7d50 *)input_point = 0;
        input_point[0] = convert_integer_to_float(pts[i * 2]);
        input_point[1] = convert_integer_to_float(pts[i * 2 + 1]);
        *(u128_7d50 *)transformed_point = *(u128_7d50 *)input_point;
        transform_vector(transformed_point, transformed_point, rotation_matrix);
        fast_vec_scale(transformed_point, transformed_point, scale);
        transformed_point[0] += x0;
        transformed_point[1] += y0;
        x = truncate_float_to_s32(transformed_point[0] * 16.0f);
        y = truncate_float_to_s32(transformed_point[1] * 16.0f);
        vertex_output[i] = (unsigned long long)(x + screen_extent.left - 8) |
                           ((unsigned long long)(y + screen_extent.top - 8) << 16) |
                           ((unsigned long long)col << 32);
    }
    D_L13_001611C0 += ((point_count + 1) / 2) * 16;
}

int D_L17_001611C0 = 0;
