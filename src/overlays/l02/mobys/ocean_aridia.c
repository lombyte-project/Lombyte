/* Level 02 moby class ocean_aridia: the ocean, drawn with the animated frame renderer. */
#include "types.h"

#include "rnc/mobys/moby.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/rendering/lights/refresh_point_light.h"

#include "qcopy.h"
#include "rnc/overlay/interpolated_frame_packet.h"

#include "rnc/overlay/l02/mobys/ocean_aridia_globals.h"

/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002A59D8), where it is exact; names translated to the US level program. */

extern void prepare_scaled_table_with_byte_128(void *, f32) __asm__("FUN_L02_002a40d0");

/* Builds the scaled table for scale in a stack scratch buffer. */
void prepare_scaled_table_on_stack(f32 scale) __asm__("FUN_L02_002a47f8");

void prepare_scaled_table_on_stack(f32 scale) {
    char scaled_table_buffer[0x40];
    prepare_scaled_table_with_byte_128(scaled_table_buffer, scale);
}

extern void write_dma_channel(void *, int, int) __asm__("FUN_001f9928");
extern void FUN_L02_002101b8(void *, int, int);

/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002A59F8), where it is exact; names translated to the US level program. */

extern char animated_frame_scratchpad_dma_source[] __asm__("D_L02_001CB400");
extern void build_interpolated_dma_packet(void *) __asm__("FUN_L02_002a40f0");

/* Sets up an animated frame draw: four header floats, the fog colour, ticks per frame, the frame
   range and its packet. The floats are declared first, in the order the caller passes them (they
   travel in their own registers). */
void begin_animated_frame_rendering(float x,
                                    float y,
                                    float z,
                                    float w,
                                    int   fog_red,
                                    int   fog_green,
                                    int   fog_blue,
                                    int   ticks_per_frame,
                                    int   frame_table_start_index,
                                    int   frame_count,
                                    void *frame_packet) __asm__("FUN_L02_002a4818");

void begin_animated_frame_rendering(float x,
                                    float y,
                                    float z,
                                    float w,
                                    int   fog_red,
                                    int   fog_green,
                                    int   fog_blue,
                                    int   ticks_per_frame,
                                    int   frame_table_start_index,
                                    int   frame_count,

                                    void *frame_packet) {
    char animated_frame_draw_parameters[0x40];
    animated_frame_draw_parameters[0x30] = fog_red;
    animated_frame_draw_parameters[0x31] = fog_green;
    animated_frame_draw_parameters[0x32] = fog_blue;
    *(float *)(animated_frame_draw_parameters + 0x20) = x;
    *(float *)(animated_frame_draw_parameters + 0x24) = y;
    *(float *)(animated_frame_draw_parameters + 0x28) = z;
    *(float *)(animated_frame_draw_parameters + 0x2C) = w;
    *(short *)(animated_frame_draw_parameters + 0x3C) = frame_table_start_index;
    animated_frame_draw_parameters[0x3E] = frame_count;
    animated_frame_draw_parameters[0x33] = ticks_per_frame;
    ((unsigned char *)animated_frame_draw_parameters)[0x3F] = 0x80;
    *(void **)(animated_frame_draw_parameters + 0x1C) = frame_packet;
    build_interpolated_dma_packet(animated_frame_draw_parameters);
    FUN_L02_002101b8(animated_frame_scratchpad_dma_source, 0x70002800, 0xF8);
}

extern char             D_L02_001673C0[];
extern char             aridia_ocean_first_clip_draw_data[] __asm__("D_L02_001F3C00");
extern char             aridia_ocean_last_clip_draw_data[] __asm__("D_L02_001F3C60");
extern char             aridia_ocean_middle_clip_first_draw_data[] __asm__("D_L02_001F3CD0");
extern char             D_L02_001F3D30[];
extern char             D_L02_001F3D90[];
extern char             D_L02_001F3DD0[];
extern char             aridia_ocean_middle_clip_last_draw_data[] __asm__("D_L02_001F3E10");
InterpolatedFramePacket aridia_ocean_animated_frame_packet __asm__("D_L02_001F3E80") = {0};

/* prepare_scaled_table_on_stack as the draw calls it: retail does not set the scale register first. */
extern void prepare_scaled_table_on_stack_unset_scale(void) __asm__("FUN_L02_002a47f8");

/* Draws the parts whose clip volumes contain the camera (all of them when
 * the override flag is set). */
void draw_aridia_ocean_parts(struct Moby *moby) __asm__("FUN_L02_002ea048");

void draw_aridia_ocean_parts(struct Moby *moby) {
    int *clip_volume_indices = (int *)moby->pvars;

    begin_animated_frame_rendering(
        0.0f, 260080.0f, 255.0f, 0.0f, 15, 15, 25, 30, 40, 64, &aridia_ocean_animated_frame_packet);
    if (aridia_ocean_clip_culling_override != 0 ||
        (clip_volume_indices[0] != -1 &&
         is_point_inside_clip_volume(D_L02_001673C0, clip_volume_indices[0]))) {
        refresh_point_light(aridia_ocean_first_clip_draw_data);
    }
    if (aridia_ocean_clip_culling_override != 0 ||
        (clip_volume_indices[1] != -1 &&
         is_point_inside_clip_volume(D_L02_001673C0, clip_volume_indices[1]))) {
        refresh_point_light(aridia_ocean_middle_clip_first_draw_data);
        refresh_point_light(D_L02_001F3D30);
        refresh_point_light(D_L02_001F3D90);
        refresh_point_light(D_L02_001F3DD0);
        refresh_point_light(aridia_ocean_middle_clip_last_draw_data);
    }
    if (aridia_ocean_clip_culling_override != 0 ||
        (clip_volume_indices[2] != -1 &&
         is_point_inside_clip_volume(D_L02_001673C0, clip_volume_indices[2]))) {
        refresh_point_light(aridia_ocean_last_clip_draw_data);
    }
    prepare_scaled_table_on_stack_unset_scale();
}

/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002E21F8.c: func_L02_002EB5D0), where it is exact; names translated to the US level program. */

void ocean_aridia_update(struct Moby *moby) __asm__("FUN_L02_002ea198");

/* Moby class 854 */
void ocean_aridia_update(struct Moby *moby) {
    enum {
        OCEAN_ARIDIA_INITIALIZE = 0,
        OCEAN_ARIDIA_REGISTER_DRAW_CALLBACK = 1
    };
    switch (moby->state) {
    case OCEAN_ARIDIA_INITIALIZE:
        prepare_scaled_table_on_stack(0.16666667f);
        moby->state = 1;
        moby->update_dist = 0xFF;
        break;
    case OCEAN_ARIDIA_REGISTER_DRAW_CALLBACK:
        add_draw_callback(draw_aridia_ocean_parts, moby);
        break;
    }
}
