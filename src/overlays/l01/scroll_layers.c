/* Level 01 scroll layers: scrolls and draws a set of two-texture overlay layers (with its per-layer step still in assembly), and the eight-layer draw callback. */
#include "types.h"
#include "asm.h"
#include "rnc/rendering/dma_tag.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/rendering/effect_texture.h"
#include "rnc/overlay/shared/ui/hud/hud_keep_visible.h"

/* One scrolling two-texture overlay layer (0x60 bytes). */
typedef struct {
    u8    pad0[8];
    void *bounds; /* 0x08: sphere tested against the view */
    s32   shape;  /* 0x0C: what FUN_L01_0021fa98 draws */
    f32   rate0;  /* 0x10 */
    f32   rate1;  /* 0x14 */
    u8    pad18[4];
    s32   frame_step; /* 0x1C */
    f32   du0;        /* 0x20 */
    f32   dv0;        /* 0x24 */
    f32   du1;        /* 0x28 */
    f32   dv1;        /* 0x2C */
    u8    pad30[4];
    s32   tex0;   /* 0x34 */
    s32   tex1;   /* 0x38 */
    u8    alpha0; /* 0x3C */
    u8    alpha1; /* 0x3D */
    u8    pad3E[2];
    f32   u0;    /* 0x40 */
    f32   v0;    /* 0x44 */
    f32   u1;    /* 0x48 */
    f32   v1;    /* 0x4C */
    s32   frame; /* 0x50 */
    u8    pad54[0xC];
} ScrollLayer;

extern union PacketCursor l01_packet __asm__("D_L01_001611C0") MACRO_ADDR;
extern u8                 D_L01_001CAC80[];
extern void               FUN_L01_00262618(ScrollLayer *);
extern void               FUN_L01_0021fa98(s32, s32, s32, s32, s32, s32);

/* Per-layer step of update_and_draw_scrolling_overlay_layers, run for each layer it draws (still in assembly). */
INCLUDE_ASM("config/us/overlays/asm/FUN_L01_00262618.s", FUN_L01_00262618);

/* Scrolls the UV offsets of count overlay layers (wrapping into [-1, 1]) and, for each one in view, queues its two-texture GS setup and draws it. */
void update_and_draw_scrolling_overlay_layers(s32 count, ScrollLayer *scroll_layer) __asm__("FUN_L01_002b96e0");

void update_and_draw_scrolling_overlay_layers(s32 count, ScrollLayer *scroll_layer) {
    struct DmaTag *tag;
    u64           *packet_payload;
    s32            vis;
    s32            i;

    for (i = 0; i < count; i++, scroll_layer++) {
        scroll_layer->u0 += scroll_layer->du0 * scroll_layer->rate0;
        scroll_layer->v0 += scroll_layer->dv0 * scroll_layer->rate0;
        if (scroll_layer->u0 > 1.0f)
            scroll_layer->u0 -= 1.0f;
        else if (scroll_layer->u0 < -1.0f)
            scroll_layer->u0 += 1.0f;
        if (scroll_layer->v0 > 1.0f)
            scroll_layer->v0 -= 1.0f;
        else if (scroll_layer->v0 < -1.0f)
            scroll_layer->v0 += 1.0f;
        scroll_layer->u1 += scroll_layer->du1 * scroll_layer->rate1;
        scroll_layer->v1 += scroll_layer->dv1 * scroll_layer->rate1;
        if (scroll_layer->u1 > 1.0f)
            scroll_layer->u1 -= 1.0f;
        else if (scroll_layer->u1 < -1.0f)
            scroll_layer->u1 += 1.0f;
        if (scroll_layer->v1 > 1.0f)
            scroll_layer->v1 -= 1.0f;
        else if (scroll_layer->v1 < -1.0f)
            scroll_layer->v1 += 1.0f;
        scroll_layer->frame = (scroll_layer->frame + scroll_layer->frame_step) & 0xFFFFFF;
        vis = fast_bsphere_check(scroll_layer->bounds, 400.0f);
        if (vis == -1)
            continue;
        l01_packet.tag->tag = 0x30000007;
        l01_packet.tag->addr = (u32)D_L01_001CAC80;
        l01_packet.tag->vif0 = 0;
        l01_packet.tag->vif1 = 0x50000007;
        l01_packet.tag++;
        l01_packet.tag->tag = 0x10000005;
        l01_packet.tag->addr = 0;
        l01_packet.tag->vif0 = 0;
        l01_packet.tag->vif1 = 0x50000005;
        tag = l01_packet.tag;
        packet_payload = (u64 *)(tag + 1);
        l01_packet.tag = tag + 1;
        packet_payload[0] = 0x4000000000008001;
        packet_payload[1] = 0xEEEE;
        packet_payload[2] = ((u64)scroll_layer->alpha0 << 32) | 100;
        packet_payload[3] = 0x42;
        packet_payload[4] = ((u64)scroll_layer->alpha1 << 32) | 100;
        packet_payload[5] = 0x43;
        packet_payload[6] = get_effect_texture(scroll_layer->tex0);
        packet_payload[7] = 6;
        packet_payload[8] = get_effect_texture(scroll_layer->tex1);
        packet_payload[9] = 7;
        l01_packet.tag = tag + 6;
        FUN_L01_00262618(scroll_layer);
        FUN_L01_0021fa98(
            scroll_layer->shape, 0x70000000, 0x70003000, 0x70001000, 0x70002000, 1 - vis);
    }
}

extern ScrollLayer l01_scrolling_overlay_layers[] __asm__("D_L01_001E2FC0");
void update_and_draw_scrolling_overlay_layers(s32, ScrollLayer *) __asm__("FUN_L01_002b96e0");

void update_and_draw_level01_scrolling_layers(void) __asm__("FUN_L01_002f60f8");

void update_and_draw_level01_scrolling_layers(void) {
    mine_blend_color();
    update_and_draw_scrolling_overlay_layers(8, l01_scrolling_overlay_layers);
}
