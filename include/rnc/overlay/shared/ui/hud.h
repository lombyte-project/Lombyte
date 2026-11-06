#ifndef LOMBYTE_RNC_OVERLAY_SHARED_UI_HUD_H
#define LOMBYTE_RNC_OVERLAY_SHARED_UI_HUD_H

#include "types.h"

/* One of the 13 HUD element records at D_L00_0017DC50. update_hud_elements
   counts visible_timer down each frame (mode bit 0x10 holds it up), moves slide
   between -6 and 30 with it and calls update; hud_keep_visible finds a
   record by key. */
typedef struct HudElem {
    int  anim_id; /* 0x00: id of the loaded HUD animation (load_animation_definition) */
    int  mode;    /* 0x04: current animation mode; bit 0x10 keeps the element shown */
    int  max_value;
    int *value_source;
    char pad10[0x4];
    void (*update)(struct HudElem *);
    char pad18[0x8];
    int next_anim_id; /* 0x20: queued by queue_animation_update, loaded by apply_pending_animation */
    int           next_mode; /* 0x24 */
    char          pad28[0x1C];
    int           icon; /* 0x44: icon sprite id */
    short         offset_x;
    short         offset_y;
    char          pad4C[0x4];
    int           x;
    int           y;
    int           w;
    int           h;
    int           flags;
    int           key;
    int           pending; /* 0x68: a queued animation waits for apply_pending_animation */
    int           slide;
    unsigned char cnt[4];
    int           shown_value;
    int           target_value;
    int           visible_timer;
    void         *animation_counters;
    char          pad84[0xC];
} HudElem;

extern HudElem hud_elements[13] __asm__("D_L00_0017DC50") __attribute__((section(".data")));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_UI_HUD_H */
