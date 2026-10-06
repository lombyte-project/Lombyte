#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00239780/FUN_00239780.s", FUN_00239780);
#else
#include "types.h"
#include "eetypes.h"

typedef union {
    u128 q;
    f32 f[4];
} CaptureVector;
struct CaptureBoundsAdjustment {
    f32 first_origin;
    f32 second_origin;
    f32 first_extent;
    f32 second_extent;
};

extern struct CaptureBoundsAdjustment capture_bounds_adjustments[] __asm__("D_001E6218");
struct CaptureTransitionFlag {
    s32 v;
};
extern struct CaptureTransitionFlag capture_primary_transition __asm__("D_00161298");
extern s32 capture_secondary_transition_active __asm__("D_001612A0");
extern s32 capture_secondary_transition_step __asm__("D_0016129C");
extern s32 capture_primary_transition_step __asm__("D_00161294");
extern s32 pal_display_mode[] __asm__("D_0015ED80");
extern s64 capture_texture_tex0 __asm__("D_0015EED0");

extern void transform_attachment_points() __asm__("func_0020CD48");
extern void fast_vec_sub(void *out, void *first_edge, void *second_edge) __asm__("func_001F9A28");
extern void fast_vec_add(void *out, void *first_edge, void *second_edge) __asm__("func_001F9A10");
extern f32 fast_vec_length(void *v) __asm__("func_001F9AF0");
extern void scale_vector_to_length(void *out, void *v, f32 s) __asm__("func_001F9BF8");
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern s32 convert_float_to_integer(f32) __asm__("func_001FA6D0");
extern void project_graphics_bounds_float() __asm__("func_00237C80");
extern void configure_capture_target(s32, s32, f32) __asm__("func_00239690");
extern void draw_centered_capture_background(s32, s32) __asm__("func_001FB740");
extern void vu1_add_g_sregister(s32, s64) __asm__("func_00233980");
extern void append_capture_texture_reference(void) __asm__("func_00233B68");
extern void update_scrolling_status_message() __asm__("func_00238520");
extern void render_vendor_item_details_pass() __asm__("func_002389E0");
extern void render_capture_effect_pass() __asm__("func_00238630");
extern void render_vendor_buy_label_pass() __asm__("func_00238F08");
extern void render_vendor_purchase_prompt_pass() __asm__("func_00239160");
extern void render_vendor_item_icon_strip_pass() __asm__("func_002386E8");
extern void render_vendor_capture_texture_overlays_pass(s32, f32, f32) __asm__("func_00239328");
extern void restore_capture_projection(void) __asm__("func_00239750");
extern void append_subpixel_textured_screen_quad(f32, f32, f32, f32, s32, s32, s32, s32, s64,
                                                 s64) __asm__("func_001F55D8");

void render_vendor_capture_pass_sequence(s32 capture_context) __asm__("FUN_00239780");

void render_vendor_capture_pass_sequence(s32 capture_context) {
    CaptureVector first_edge;
    CaptureVector second_edge;
    CaptureVector origin;
    CaptureVector attachment_points[3];
    s32 point_indices[3];
    CaptureVector scaled_edge_offset;
    CaptureVector capture_corners[9];
    CaptureVector first_corner;
    CaptureVector opposite_corner;
    f32 outer_width, outer_height, outer_x, outer_y;
    f32 inner_width, inner_height, inner_x, inner_y;
    s32 pass_index;
    s32 point_base;
    f32 edge_length;
    f32 transition_factor;
    s64 texture_tex0;
    CaptureVector *attachment_point_buffer;
    s32 *attachment_point_indices;
    CaptureVector *origin_pointer;
    CaptureVector *corner_buffer;

    attachment_point_buffer = attachment_points;
    attachment_point_indices = point_indices;
    origin_pointer = &origin;
    for (pass_index = 0; pass_index < 6; pass_index++) {
        do {
            point_base = pass_index << 2;
            point_indices[0] = point_base;
            point_indices[1] = point_base + 1;
            point_indices[2] = point_base + 2;
            transform_attachment_points(capture_context, 3, attachment_point_indices,
                                        attachment_point_buffer);
            origin_pointer->q = attachment_point_buffer->q;
        } while (0);
        fast_vec_sub(&first_edge, &attachment_points[1], attachment_point_buffer);
        fast_vec_sub(&second_edge, &attachment_points[2], attachment_point_buffer);
        edge_length = fast_vec_length(&first_edge);
        scale_vector_to_length(&scaled_edge_offset, &first_edge,
                               capture_bounds_adjustments[pass_index].first_origin);
        fast_vec_add(origin_pointer, origin_pointer, &scaled_edge_offset);
        scale_vector_to_length(&first_edge, &first_edge,
                               edge_length -
                                   2.0f * capture_bounds_adjustments[pass_index].first_extent);
        edge_length = fast_vec_length(&second_edge);
        scale_vector_to_length(&scaled_edge_offset, &second_edge,
                               capture_bounds_adjustments[pass_index].second_origin);
        fast_vec_add(origin_pointer, origin_pointer, &scaled_edge_offset);
        scale_vector_to_length(&second_edge, &second_edge,
                               edge_length -
                                   2.0f * capture_bounds_adjustments[pass_index].second_extent);
        fast_vec_add(&scaled_edge_offset, origin_pointer, &first_edge);
        fast_vec_add(&scaled_edge_offset, &scaled_edge_offset, &second_edge);
        project_graphics_bounds_float(origin_pointer, &scaled_edge_offset, &outer_width,
                                      &outer_height, &outer_x, &outer_y);
        corner_buffer = capture_corners;
        if (capture_primary_transition.v != 0 || capture_secondary_transition_active != 0) {
            transition_factor = 1.0f;
            if (capture_secondary_transition_active != 0) {
                transition_factor =
                    convert_integer_to_float(8 - capture_secondary_transition_step) * 0.125f;
            }
            if (capture_primary_transition.v != 0) {
                transition_factor =
                    convert_integer_to_float(capture_primary_transition_step) * 0.125f;
            }
            edge_length = fast_vec_length(&first_edge);
            scale_vector_to_length(&scaled_edge_offset, &first_edge,
                                   edge_length * (1.0f - transition_factor) * 0.5f);
            scale_vector_to_length(&first_edge, &first_edge, edge_length * transition_factor);
            fast_vec_add(origin_pointer, origin_pointer, &scaled_edge_offset);
            edge_length = fast_vec_length(&second_edge);
            scale_vector_to_length(&scaled_edge_offset, &second_edge,
                                   edge_length * (1.0f - transition_factor) * 0.5f);
            scale_vector_to_length(&second_edge, &second_edge, edge_length * transition_factor);
            fast_vec_add(origin_pointer, origin_pointer, &scaled_edge_offset);
        }
        corner_buffer->q = origin_pointer->q;
        fast_vec_add(&capture_corners[1], origin_pointer, &first_edge);
        fast_vec_add(&capture_corners[2], origin_pointer, &second_edge);
        fast_vec_add(&capture_corners[3], &capture_corners[2], &first_edge);
        first_corner.q = capture_corners[0].q;
        opposite_corner.q = capture_corners[3].q;
        project_graphics_bounds_float(&first_corner, &opposite_corner, &inner_width, &inner_height,
                                      &inner_x, &inner_y);
        configure_capture_target(9, 7, 1.0f);
        draw_centered_capture_background(0x200, 0x200);
        vu1_add_g_sregister(0x42, 0x8000000064LL);
        switch (point_base >> 2) {
        case 0:
            update_scrolling_status_message(capture_context);
            break;
        case 1:
            render_vendor_item_details_pass(capture_context);
            break;
        case 2:
            render_capture_effect_pass(capture_context);
            break;
        case 3:
            render_vendor_buy_label_pass(capture_context, (s32)inner_width, (s32)inner_height);
            break;
        case 4:
            render_vendor_purchase_prompt_pass(capture_context);
            break;
        case 5:
            render_vendor_item_icon_strip_pass(capture_context);
            break;
        }
        render_vendor_capture_texture_overlays_pass(pass_index, inner_width, inner_height);
        restore_capture_projection();
        draw_centered_capture_background(0x200, 0x200);
        vu1_add_g_sregister(0x42, 0x8000000064LL);
        vu1_add_g_sregister(8, 5);
        append_capture_texture_reference();
        texture_tex0 = capture_texture_tex0;
        if (pal_display_mode[0] != 0) {
            f32 screen_x, screen_y, screen_width, screen_height;
            screen_x = inner_x;
            screen_y = inner_y;
            screen_width = inner_width;
            screen_height = inner_height;
            append_subpixel_textured_screen_quad(
                screen_x, screen_y, screen_width, screen_height, 0, 0, (s32)(outer_width - 1.0f),
                convert_float_to_integer(outer_height * 0.92857146f - 1.0f), 0x80808080,
                texture_tex0);
        } else {
            append_subpixel_textured_screen_quad(
                inner_x, inner_y, inner_width, inner_height, 0, 0, (s32)(outer_width - 1.0f),
                (s32)(outer_height - 1.0f), 0x80808080, texture_tex0);
        }
        vu1_add_g_sregister(8, 0);
    }
}

extern __typeof__(render_vendor_capture_pass_sequence) func_00239780
    __attribute__((alias("FUN_00239780")));

#endif /* NON_MATCHING */
