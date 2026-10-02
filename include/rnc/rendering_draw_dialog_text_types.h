#ifndef RNC_RENDERING_DRAW_DIALOG_TEXT_TYPES_H
#define RNC_RENDERING_DRAW_DIALOG_TEXT_TYPES_H

#include "rnc/text_region.h"

/* Only the fields read by the modal renderer are recovered here. */
struct DialogGameState {
    u8 pad0[0x894];
    s32 time_value;
    u8 pad898[2];
    s16 panel_variant;
    u8 pad89C[0xC];
    s32 count_value;
    u8 pad8AC[0x14];
    s32 display_value;
};

struct ModalScreenState {
    s32 mode;
    s32 countdown;
    u8 *primary_text;
    u8 *secondary_text;
    u8 *tertiary_text;
    s32 previous_stage;
    s32 argument;
    s32 phase;
    s32 elapsed_frames;
    s32 secondary_countdown;
};

struct DialogTextFrame {
    u8 text[0x200];
    struct TextRegion region;
    u8 pad218[8];
    struct TextRegion input_region;
    u8 pad238[8];
    u8 statistics_text[0x40];
    s32 text_colour;
};

#endif
