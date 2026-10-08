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

/* The stream state (buffers, loaded animations, resource range) lives in
   struct MenuSystem (rnc/ui/menus/menu_system.h) at 0xA0..0xCC. */

typedef struct {
    s32 class_id;
    s32 animation_index;
} PreviewResourceBinding;

#endif /* LOMBYTE_RNC_UI_MENUS_ITEM_PREVIEW_PREVIEW_ANIMATION_H */
