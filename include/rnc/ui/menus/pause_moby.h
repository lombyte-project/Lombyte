#ifndef LOMBYTE_RNC_UI_MENUS_PAUSE_MOBY_H
#define LOMBYTE_RNC_UI_MENUS_PAUSE_MOBY_H

#include "types.h"

/* Partial layout used by the menu preview callbacks. The three basis columns
   at 0xC0 are followed by the cached resource vector at 0xF0. */
typedef struct {
    char pad00[0x10];
    f32 pos[4];
    char pad20[4];
    s32 resource_address;
    char pad28[0x28];
    s32 binding_state;
    s32 binding_blend_word;
    char pad58[0x10];
    char *primary_binding;
    char *secondary_binding;
    char pad70[8];
    char **vars;
    char pad7C[0x2A];
    s16 oclass;
    char padA8[0x18];
    f32 basis[12];
    f32 cached_vector[4];
} PauseMoby;

#endif /* LOMBYTE_RNC_UI_MENUS_PAUSE_MOBY_H */
