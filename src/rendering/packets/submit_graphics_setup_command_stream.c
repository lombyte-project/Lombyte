#include "types.h"
#include "asm.h"

#include "types.h"

#include "eetypes.h"
#include "rnc/rendering/dma_tag.h"
#include "rnc/rendering/vu1_registers.h"

/* GIF packets that program the GS for the NTSC and PAL video modes. */
extern u8 ntsc_graphics_setup_packet[0x70] __asm__("D_001D7E50");
extern u8 pal_graphics_setup_packet[0x70] __asm__("D_001D7EC0");

struct GraphicsSetupRecord {
    s32  command_count;
    s32  command_flags;
    f32  second_depth;
    f32  first_depth;
    u128 direction;
};

extern u8 *render_packet_bytes __asm__("D_00160F00");

struct VideoModeState {
    s32 v;
};

extern struct VideoModeState pal_mode __asm__("D_0015ED80");
extern s32                   graphics_setup_word __asm__("D_001603A0");
extern void                  append_fullscreen_setup_strips(void) __asm__("func_002271D0");
extern void prepare_scaled_direction_and_perpendicular(f32 *, f32) __asm__("func_00226FB8");
extern u8  *parse_typed_resource_record(u8 *) __asm__("func_002270E8");
extern void func_00228520(f32 *, f32 *);
extern void func_00227A08(u32, s32, s32);
extern void emit_selected_graphics_dma_packets(s32, s32, s32) __asm__("func_00227140");
extern void append_fullscreen_clear_strips(s64) __asm__("func_00227378");
void        submit_graphics_setup_command_stream(u8 *command_stream) __asm__("FUN_00227548");

void submit_graphics_setup_command_stream(u8 *command_stream) {
    f32                         first_vector[4];
    f32                         second_vector[4];
    f32                         direction[4];
    struct GraphicsSetupRecord *setup_record;
    s32                         command_index;

    append_fullscreen_setup_strips();
    setup_record = (struct GraphicsSetupRecord *)command_stream;
    *(u32 *)(render_packet_bytes + 0) = 0x30000007;
    *(u32 *)(render_packet_bytes + 4) =
        (u32)(pal_mode.v != 0 ? pal_graphics_setup_packet : ntsc_graphics_setup_packet);
    *(u32 *)(render_packet_bytes + 8) = 0x13000000;
    *(u32 *)(render_packet_bytes + 12) = 0x50000007;
    /* Remaining vector components are filled from each setup record. */
    second_vector[1] = first_vector[1] = second_vector[0] = first_vector[0] = 0.0f;
    render_packet_bytes += 16;
    while (setup_record->command_count != 0) {
        command_stream += 0x20;
        *(u128 *)direction = setup_record->direction;
        prepare_scaled_direction_and_perpendicular(direction, 1000.0f);
        first_vector[2] = setup_record->first_depth;
        second_vector[2] = setup_record->second_depth;
        for (command_index = 0; command_index < setup_record->command_count; command_index++) {
            command_stream = parse_typed_resource_record(command_stream);
            func_00228520(first_vector, second_vector);
            func_00227A08(0x70000000, graphics_setup_word, setup_record->command_flags);
            emit_selected_graphics_dma_packets(2, 1, 2);
            emit_selected_graphics_dma_packets(1, 0, 2);
        }
        setup_record = (struct GraphicsSetupRecord *)command_stream;
    }
    if (pal_mode.v) {
        vu1_add_g_sregister(0x4C, 0x80080);
    } else {
        vu1_add_g_sregister(0x4C, 0x80070);
    }
    vu1_add_g_sregister(0x42, 0x2000000064LL);
    append_fullscreen_clear_strips(0);
    vu1_add_g_sregister(0x47, 0x5360B);
    vu1_add_g_sregister(0x42, 0x8000000044LL);
}

extern __typeof__(submit_graphics_setup_command_stream) func_00227548
    __attribute__((alias("FUN_00227548")));
