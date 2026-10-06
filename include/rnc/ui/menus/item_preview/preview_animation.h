#ifndef LOMBYTE_RNC_UI_MENUS_ITEM_PREVIEW_PREVIEW_ANIMATION_H
#define LOMBYTE_RNC_UI_MENUS_ITEM_PREVIEW_PREVIEW_ANIMATION_H

#include "types.h"

typedef struct {
    s32 status;
    s32 animation_id;
    s32 trigger_mode;
    s32 delay_frames;
    s32 item_index;
    s32 item_animation;
    s32 attachment0_class;
    s32 attachment0_animation;
    s32 attachment1_class;
    s32 attachment1_animation;
    s32 attachment2_class;
    s32 attachment2_animation;
    s32 resource_first;
    s32 resource_count;
} PreviewAnimationRequest;

typedef struct {
    u8 pad0[0x30];
    s32 active_items[3];
    u8 pad3C[0x64];
    s32 buffer_address[2];
    s32 resource_first;
    s32 resource_count;
    s32 resource_buffer_address[3];
    s32 read_offset;
    u8 padC0[8];
    u8 loaded_animation[2];
    u8 read_buffer_index;
    u8 pending_buffer;
    u32 streamed_animation_base;
} PreviewAnimationStreamState;

typedef struct {
    s32 class_id;
    s32 animation_index;
} PreviewResourceBinding;

#endif /* LOMBYTE_RNC_UI_MENUS_ITEM_PREVIEW_PREVIEW_ANIMATION_H */
