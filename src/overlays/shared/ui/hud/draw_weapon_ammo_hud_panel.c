/* HUD: the weapon ammo panel and the HUD element updates. */
#include "types.h"
#include "rnc/overlay/hud.h"
#include "rnc/ui/vendor/vendor_item_info.h"
#include "rnc/hero/hero.h"
#include "asm.h"
#include "rnc/globals.h"
#include "rnc/game/item_state.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/ui/hud/get_icon_frame.h"
#include "rnc/mobys/animation/set_animation_parameter.h"
#include "rnc/ui/hud/sprites/draw_hud_sprite.h"
#include "rnc/overlay/shared/ui/hud/hud_update_counter.h"
#include "rnc/ui/hud/sprites/draw_hud_sprite_flipped.h"
#include "rnc/mobys/animation/apply_pending_animation.h"
#include "rnc/overlay/shared/ui/hud/hud_frame.h"

/* Draws the weapon HUD tile: three-part frame, weapon icon and ammo text with shadow, faded by the slide-in timers. */
/* Ported from rac1-decomp (src/overlays/shared/hud_00235960.c: func_L00_0023B140), where it is exact; names translated to the US level program. */

extern char D_L00_0015F7C8[];
extern int  fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern int  sprintf(char *, const char *, ...) __asm__("FUN_00116248");
extern s32  D_L00_0015F91C_gp __asm__("D_L00_0015F91C") __attribute__((sda));
extern s32  D_L00_0015F920_gp __asm__("D_L00_0015F920") __attribute__((sda));
void font_print_right(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) __asm__("FUN_001f6940");

extern short ammo_hud_frame_width_below_100 __asm__("D_L00_0015F924") __attribute__((sda));
extern short ammo_hud_frame_width_at_least_100 __asm__("D_L00_0015F928") __attribute__((sda));
extern short ammo_hud_nonzero_text_color_blend_start __asm__("D_L00_0015F92C") __attribute__((sda));
extern short ammo_hud_nonzero_text_color_blend_end __asm__("D_L00_0015F930") __attribute__((sda));
extern short ammo_hud_zero_text_color_blend_start __asm__("D_L00_0015F934") __attribute__((sda));
extern short ammo_hud_zero_text_color_blend_end __asm__("D_L00_0015F938") __attribute__((sda));
extern short ammo_hud_text_right_x_offset __asm__("D_L00_0015F93C") __attribute__((sda));
extern short ammo_hud_text_y_offset __asm__("D_L00_0015F940") __attribute__((sda));
int          draw_weapon_ammo_hud_panel(char *hud_element) __asm__("FUN_L00_0023a7a8");

int draw_weapon_ammo_hud_panel(char *hud_element) {
    char  formatted_count_text[16];
    char *fade_control_bytes = hud_element + 0x70;
    int   x4;
    int   y = 0x12;
    int   x, frame_alpha, alpha, base_frame_width, text_color, icon_frame_index, text_shadow_color,
        wd, displayed_numerator;
    float hud_fade_factor, text_color_blend_factor;
    if (pal_mode)
        y = 0xA;
    x = *(int *)(hud_element + 0x50);
    if (*(unsigned char *)(hud_element + 0x70) == 0)
        return *(int *)(hud_element + 0x58);
    {
        text_color_blend_factor = convert_integer_to_float(*(unsigned char *)(hud_element + 0x70));
        hud_fade_factor =
            text_color_blend_factor / convert_integer_to_float(*(int *)&D_L00_0015F91C_gp);
        if (hud_fade_factor > 1.0f)
            hud_fade_factor = 1.0f;
        else if (hud_fade_factor < 0.0f)
            hud_fade_factor = 0.0f;
        text_color_blend_factor =
            convert_integer_to_float(*(unsigned char *)(fade_control_bytes + 1));
        text_color_blend_factor =
            text_color_blend_factor / convert_integer_to_float(*(int *)&D_L00_0015F920_gp);
        if (text_color_blend_factor > 1.0f)
            text_color_blend_factor = 1.0f;
        else if (text_color_blend_factor < 0.0f)
            text_color_blend_factor = 0.0f;
        alpha = truncate_float_to_s32(hud_fade_factor * 128.0f);
        frame_alpha = truncate_float_to_s32((float)alpha * 0.7f);
        if (*(int *)(hud_element + 8) < 100)
            base_frame_width = *(int *)&ammo_hud_frame_width_below_100;
        else
            base_frame_width = *(int *)&ammo_hud_frame_width_at_least_100;
        x4 = x + 4;
        wd = x + truncate_float_to_s32((float)base_frame_width * hud_fade_factor);
        draw_hud_sprite(get_icon_frame(0x7580, 1), x - 0x1C, y, 0x20, 0x20, frame_alpha);
        draw_hud_sprite(get_icon_frame(0x7580, 0), x4, y, wd, 0x20, frame_alpha);
        draw_hud_sprite_flipped(get_icon_frame(0x7580, 1), x4 + wd, y, 0x20, 0x20, frame_alpha);
        icon_frame_index = get_icon_frame(*(int *)hud_element, 3);
        *(int *)(hud_element + 0x44) = icon_frame_index;
        hud_frame(hud_element, icon_frame_index, x, y, 0, alpha);
        displayed_numerator = *(int *)(hud_element + 0x74);
        sprintf(
            formatted_count_text, D_L00_0015F7C8, displayed_numerator, *(int *)(hud_element + 8));
        if (displayed_numerator != 0)
            text_color = fast_tween_color(*(int *)&ammo_hud_nonzero_text_color_blend_start,
                                          *(int *)&ammo_hud_nonzero_text_color_blend_end,
                                          text_color_blend_factor);
        else
            text_color = fast_tween_color(*(int *)&ammo_hud_zero_text_color_blend_start,
                                          *(int *)&ammo_hud_zero_text_color_blend_end,
                                          text_color_blend_factor);
        text_shadow_color = fast_tween_color(0, 0x80000000, text_color_blend_factor);
        {
            int tx = x + *(int *)&ammo_hud_text_right_x_offset + base_frame_width;
            font_print_right(tx + 1,
                             y + *(int *)&ammo_hud_text_y_offset + 1,
                             text_shadow_color,
                             formatted_count_text,
                             -1);
        }
        font_print_right(x + *(int *)&ammo_hud_text_right_x_offset + base_frame_width,
                         y + *(int *)&ammo_hud_text_y_offset,
                         text_color,
                         formatted_count_text,
                         -1);
    }
    return *(int *)(hud_element + 0x58);
}

typedef struct {
    unsigned short id;
    unsigned short n;
    unsigned short base;
    unsigned char  p6;
    unsigned char  rate;
} T_23aaa8;

typedef struct {
    short         id;
    unsigned char mode;
    unsigned char p3;
    int           frame;
    int           p8;
    int           start;
} O_23aaa8;

extern int                      D_L00_0015F5CC_23aaa8 __asm__("D_L00_0015F5CC");
extern T_23aaa8 *D_L00_0017E4F4 NOT_SDA;
int update_hud_animation_frame(O_23aaa8 *hud_frame_state) __asm__("FUN_L00_0023aaa8");

int update_hud_animation_frame(O_23aaa8 *hud_frame_state) {
    enum {
        HUD_FRAME_MODE_STATIC = 0,
        HUD_FRAME_MODE_LOOP = 1,
        HUD_FRAME_MODE_PING_PONG = 2,
        HUD_FRAME_MODE_PING_PONG_RESTART = 3
    };
    T_23aaa8 *frame_sequence = &D_L00_0017E4F4[hud_frame_state->id];
    int       frame_index;
    int       k, restart_start_offset;
    frame_index = 0;
    if (frame_sequence->id != 0xFFFF) {
        switch (hud_frame_state->mode) {
        case HUD_FRAME_MODE_STATIC:
            frame_index = frame_sequence->base;
            break;
        case HUD_FRAME_MODE_LOOP:
            frame_index = frame_sequence->base + ((D_L00_0015F5CC_23aaa8 - hud_frame_state->start) /
                                                  frame_sequence->rate) %
                                                     frame_sequence->n;
            break;
        case HUD_FRAME_MODE_PING_PONG:
            frame_index = (D_L00_0015F5CC_23aaa8 - hud_frame_state->start) / frame_sequence->rate;
            k = frame_sequence->n * 2;
            frame_index = frame_index % (k - 2);
            if (frame_index >= frame_sequence->n) {
                int b = frame_index + 2;
                frame_index = k - b;
            }
            frame_index += frame_sequence->base;
            break;
        case HUD_FRAME_MODE_PING_PONG_RESTART:
            if (hud_frame_state->start < D_L00_0015F5CC_23aaa8) {
                int doubled_frame_count;
                frame_index =
                    (D_L00_0015F5CC_23aaa8 - hud_frame_state->start) / frame_sequence->rate;
                doubled_frame_count = frame_sequence->n * 2;
                if (frame_index < doubled_frame_count - 2) {
                    if (frame_index >= frame_sequence->n) {
                        int b = frame_index + 2;
                        frame_index = doubled_frame_count - b;
                    }
                    frame_index += frame_sequence->base;
                } else {
                    frame_index = frame_sequence->base;
                    restart_start_offset = scale_game_frames(0xA);
                    hud_frame_state->start = frame_sequence->n * 2 + restart_start_offset +
                                             random_integer_below(scale_game_frames(0x1E));
                }
            } else {
                frame_index = hud_frame_state->frame;
            }
            break;
        }
    }
    hud_frame_state->frame = frame_index;
    return frame_index;
}

typedef struct {
    int a;
    int b;
} H_23ac78;

extern H_23ac78 D_L00_0017E4D8 NOT_SDA;
void                           update_weapon_hud_state(void) __asm__("FUN_L00_0023adb8");

extern void update_weapon_hud_state(void) __asm__("FUN_L00_0023adb8");

int update_hud_elements(int call) __asm__("FUN_L00_0023ac78");

int update_hud_elements(int call) {
    int       positive_timer_count = 0;
    HudElem  *hud_element;
    H_23ac78 *hud_timer_refresh_control;
    update_weapon_hud_state();
    hud_element = hud_elements;
    hud_timer_refresh_control = &D_L00_0017E4D8;
    do {
        if ((hud_element->unk04 & 0x10) || hud_timer_refresh_control->b != 0) {
            if (hud_element->unk7C < scale_game_frames(10)) {
                hud_element->unk7C = scale_game_frames(10);
            }
        }
        if (hud_element->unk7C > 0 && --hud_element->unk7C > 0) {
            positive_timer_count++;
            if (hud_element->unk6C < 30)
                hud_element->unk6C++;
        } else {
            if (hud_element->unk6C >= -5)
                hud_element->unk6C--;
        }
        if (hud_element->unk68 && hud_element->unk6C == -6) {
            apply_pending_animation(hud_element);
        }
        if (call && hud_element->update) {
            hud_element->update(hud_element);
        }
        hud_element++;
    } while ((int)hud_element < (int)(hud_elements + 13));
    return positive_timer_count;
}

extern int D_L00_0015F968_gp __asm__("D_L00_0015F968") __attribute__((sda));
extern int D_L00_0015F96C;
extern int D_L00_0015F970;
extern int
queue_animation_update(int, int, void *, void *, void *, void *, int) __asm__("FUN_001ff308");
extern void initialize_numeric_hud_timer_and_base_dimensions(void) __asm__("FUN_L00_0023a760");
extern void hud_weapon_update(void) __asm__("FUN_L00_0023cdb8");
extern void reset_numeric_hud_element(void) __asm__("FUN_L00_00235d80");
extern void draw_hud_cropped_gauge_panel(void) __asm__("FUN_L00_0023a640");

void update_weapon_hud_state(void) {
    struct Hero *hero_state;
    int          weapon_item_id, mode, k;
    if (D_L00_0015F968_gp != 0) {
        return;
    }
    hero_state = &hero;
    weapon_item_id = hero_state->items[0].item_id;
    mode = hero_state->vehicle_mode;
    weapon_item_id = (&D_L00_001C40B0[0] + weapon_item_id)->h8 ? weapon_item_id : 0;
    weapon_item_id = mode ? 0 : weapon_item_id;
    if (weapon_item_id != 0) {
        D_L00_0015F96C = weapon_item_id;
        *(volatile int *)&D_L00_0015F970 =
            queue_animation_update(0x10,
                                   weapon_item_id + 60000,
                                   initialize_numeric_hud_timer_and_base_dimensions,
                                   hud_weapon_update,
                                   draw_weapon_ammo_hud_panel,
                                   &weapon_ammo_counts[weapon_item_id],
                                   D_L00_001C40B0[weapon_item_id].hE);
    } else if (k = -1, mode == 2) {
        if (D_L00_0015F970 != k) {
            set_animation_parameter(D_L00_0015F970, 0);
            D_L00_0015F970 = k;
        }
        hero_state->unk1634 = queue_animation_update(0x10,
                                                     0xFFFF,
                                                     reset_numeric_hud_element,
                                                     hud_update_counter,
                                                     draw_hud_cropped_gauge_panel,
                                                     &hero_state->vehicle_hp,
                                                     200);
    } else {
        if (D_L00_0015F970 != k) {
            set_animation_parameter(D_L00_0015F970, 0);
            D_L00_0015F970 = k;
        }
    }
}

void reset_hud_animation_channels() __asm__("FUN_L00_00235878");
/* Resets via FUN_L00_00235878, then sets the three state words to 1, -1 and 0. */
void reset_hud_and_suspend_weapon_updates(void) __asm__("FUN_L00_0023aef8");

void reset_hud_and_suspend_weapon_updates(void) {
    reset_hud_animation_channels();
    D_L00_0015F968_gp = 1;
    D_L00_0015F970 = -1;
    D_L00_0015F96C = 0;
}

char                     D_L01_0015F7C8 = 0x25;
short                    D_L01_0015F924 = 0x46;
short                    D_L01_0015F928 = 0x5f;
short                    D_L01_0015F92C = -32672;
short                    D_L01_0015F930 = -32672;
short                    D_L01_0015F934 = 0x2080;
short                    D_L01_0015F938 = 0x2080;
short                    D_L01_0015F93C = 0x19;
short                    D_L01_0015F940 = 8;
T_23aaa8 *D_L01_0017E974 NOT_SDA = 0;
int                      D_L01_0015F968 = 0;
int                      D_L01_0015F96C = 0;
int                      D_L01_0015F970 = -1;
char                     D_L02_0015F7C8 = 0x25;
short                    D_L02_0015F924 = 0x46;
short                    D_L02_0015F928 = 0x5f;
short                    D_L02_0015F92C = -32672;
short                    D_L02_0015F930 = -32672;
short                    D_L02_0015F934 = 0x2080;
short                    D_L02_0015F938 = 0x2080;
short                    D_L02_0015F93C = 0x19;
short                    D_L02_0015F940 = 8;
T_23aaa8 *D_L02_0017EAF4 NOT_SDA = 0;
int                      D_L02_0015F968 = 0;
int                      D_L02_0015F96C = 0;
int                      D_L02_0015F970 = -1;
char                     D_L03_0015F7C8 = 0x25;
short                    D_L03_0015F924 = 0x46;
short                    D_L03_0015F928 = 0x5f;
short                    D_L03_0015F92C = -32672;
short                    D_L03_0015F930 = -32672;
short                    D_L03_0015F934 = 0x2080;
short                    D_L03_0015F938 = 0x2080;
short                    D_L03_0015F93C = 0x19;
short                    D_L03_0015F940 = 8;
T_23aaa8 *D_L03_0017E5F4 NOT_SDA = 0;
int                      D_L03_0015F968 = 0;
int                      D_L03_0015F96C = 0;
int                      D_L03_0015F970 = -1;
char                     D_L04_0015F7C8 = 0x25;
short                    D_L04_0015F924 = 0x46;
short                    D_L04_0015F928 = 0x5f;
short                    D_L04_0015F92C = -32672;
short                    D_L04_0015F930 = -32672;
short                    D_L04_0015F934 = 0x2080;
short                    D_L04_0015F938 = 0x2080;
short                    D_L04_0015F93C = 0x19;
short                    D_L04_0015F940 = 8;
T_23aaa8 *D_L04_0017E674 NOT_SDA = 0;
int                      D_L04_0015F968 = 0;
int                      D_L04_0015F96C = 0;
int                      D_L04_0015F970 = -1;
char                     D_L05_0015F7D8 = 0x25;
short                    D_L05_0015F934 = 0x46;
short                    D_L05_0015F938 = 0x5f;
short                    D_L05_0015F93C = -32672;
short                    D_L05_0015F940 = -32672;
short                    D_L05_0015F944 = 0x2080;
short                    D_L05_0015F948 = 0x2080;
short                    D_L05_0015F94C = 0x19;
short                    D_L05_0015F950 = 8;
T_23aaa8 *D_L05_0017E974 NOT_SDA = 0;
char                     D_L06_0015F7C8 = 0x25;
short                    D_L06_0015F924 = 0x46;
short                    D_L06_0015F928 = 0x5f;
short                    D_L06_0015F92C = -32672;
short                    D_L06_0015F930 = -32672;
short                    D_L06_0015F934 = 0x2080;
short                    D_L06_0015F938 = 0x2080;
short                    D_L06_0015F93C = 0x19;
short                    D_L06_0015F940 = 8;
T_23aaa8 *D_L06_0017EC74 NOT_SDA = 0;
int                      D_L06_0015F968 = 0;
int                      D_L06_0015F96C = 0;
int                      D_L06_0015F970 = -1;
char                     D_L07_0015F7C8 = 0x25;
short                    D_L07_0015F924 = 0x46;
short                    D_L07_0015F928 = 0x5f;
short                    D_L07_0015F92C = -32672;
short                    D_L07_0015F930 = -32672;
short                    D_L07_0015F934 = 0x2080;
short                    D_L07_0015F938 = 0x2080;
short                    D_L07_0015F93C = 0x19;
short                    D_L07_0015F940 = 8;
T_23aaa8 *D_L07_0017E574 NOT_SDA = 0;
int                      D_L07_0015F968 = 0;
int                      D_L07_0015F96C = 0;
int                      D_L07_0015F970 = -1;
char                     D_L08_0015F7C8 = 0x25;
short                    D_L08_0015F924 = 0x46;
short                    D_L08_0015F928 = 0x5f;
short                    D_L08_0015F92C = -32672;
short                    D_L08_0015F930 = -32672;
short                    D_L08_0015F934 = 0x2080;
short                    D_L08_0015F938 = 0x2080;
short                    D_L08_0015F93C = 0x19;
short                    D_L08_0015F940 = 8;
T_23aaa8 *D_L08_0017ECF4 NOT_SDA = 0;
int                      D_L08_0015F968 = 0;
int                      D_L08_0015F96C = 0;
int                      D_L08_0015F970 = -1;
char                     D_L09_0015F7C8 = 0x25;
short                    D_L09_0015F924 = 0x46;
short                    D_L09_0015F928 = 0x5f;
short                    D_L09_0015F92C = -32672;
short                    D_L09_0015F930 = -32672;
short                    D_L09_0015F934 = 0x2080;
short                    D_L09_0015F938 = 0x2080;
short                    D_L09_0015F93C = 0x19;
short                    D_L09_0015F940 = 8;
T_23aaa8 *D_L09_0017E674 NOT_SDA = 0;
int                      D_L09_0015F968 = 0;
int                      D_L09_0015F96C = 0;
int                      D_L09_0015F970 = -1;
char                     D_L10_0015F7C8 = 0x25;
short                    D_L10_0015F924 = 0x46;
short                    D_L10_0015F928 = 0x5f;
short                    D_L10_0015F92C = -32672;
short                    D_L10_0015F930 = -32672;
short                    D_L10_0015F934 = 0x2080;
short                    D_L10_0015F938 = 0x2080;
short                    D_L10_0015F93C = 0x19;
short                    D_L10_0015F940 = 8;
T_23aaa8 *D_L10_0017E974 NOT_SDA = 0;
int                      D_L10_0015F968 = 0;
int                      D_L10_0015F96C = 0;
int                      D_L10_0015F970 = -1;
char                     D_L11_0015F7C8 = 0x25;
short                    D_L11_0015F924 = 0x46;
short                    D_L11_0015F928 = 0x5f;
short                    D_L11_0015F92C = -32672;
short                    D_L11_0015F930 = -32672;
short                    D_L11_0015F934 = 0x2080;
short                    D_L11_0015F938 = 0x2080;
short                    D_L11_0015F93C = 0x19;
short                    D_L11_0015F940 = 8;
T_23aaa8 *D_L11_0017EEF4 NOT_SDA = 0;
int                      D_L11_0015F968 = 0;
int                      D_L11_0015F96C = 0;
int                      D_L11_0015F970 = -1;
char                     D_L12_0015F7C8 = 0x25;
short                    D_L12_0015F924 = 0x46;
short                    D_L12_0015F928 = 0x5f;
short                    D_L12_0015F92C = -32672;
short                    D_L12_0015F930 = -32672;
short                    D_L12_0015F934 = 0x2080;
short                    D_L12_0015F938 = 0x2080;
short                    D_L12_0015F93C = 0x19;
short                    D_L12_0015F940 = 8;
T_23aaa8 *D_L12_0017E974 NOT_SDA = 0;
int                      D_L12_0015F968 = 0;
int                      D_L12_0015F96C = 0;
int                      D_L12_0015F970 = -1;
char                     D_L13_0015F7C8 = 0x25;
short                    D_L13_0015F924 = 0x46;
short                    D_L13_0015F928 = 0x5f;
short                    D_L13_0015F92C = -32672;
short                    D_L13_0015F930 = -32672;
short                    D_L13_0015F934 = 0x2080;
short                    D_L13_0015F938 = 0x2080;
short                    D_L13_0015F93C = 0x19;
short                    D_L13_0015F940 = 8;
T_23aaa8 *D_L13_0017E7F4 NOT_SDA = 0;
int                      D_L13_0015F968 = 0;
int                      D_L13_0015F96C = 0;
int                      D_L13_0015F970 = -1;
char                     D_L14_0015F7C8 = 0x25;
short                    D_L14_0015F924 = 0x46;
short                    D_L14_0015F928 = 0x5f;
short                    D_L14_0015F92C = -32672;
short                    D_L14_0015F930 = -32672;
short                    D_L14_0015F934 = 0x2080;
short                    D_L14_0015F938 = 0x2080;
short                    D_L14_0015F93C = 0x19;
short                    D_L14_0015F940 = 8;
T_23aaa8 *D_L14_0017EBF4 NOT_SDA = 0;
int                      D_L14_0015F968 = 0;
int                      D_L14_0015F96C = 0;
int                      D_L14_0015F970 = -1;
char                     D_L15_0015F7C8 = 0x25;
short                    D_L15_0015F924 = 0x46;
short                    D_L15_0015F928 = 0x5f;
short                    D_L15_0015F92C = -32672;
short                    D_L15_0015F930 = -32672;
short                    D_L15_0015F934 = 0x2080;
short                    D_L15_0015F938 = 0x2080;
short                    D_L15_0015F93C = 0x19;
short                    D_L15_0015F940 = 8;
T_23aaa8 *D_L15_0017EAF4 NOT_SDA = 0;
int                      D_L15_0015F968 = 0;
int                      D_L15_0015F96C = 0;
int                      D_L15_0015F970 = -1;
char                     D_L16_0015F7D8 = 0x25;
short                    D_L16_0015F934 = 0x46;
short                    D_L16_0015F938 = 0x5f;
short                    D_L16_0015F93C = -32672;
short                    D_L16_0015F940 = -32672;
short                    D_L16_0015F944 = 0x2080;
short                    D_L16_0015F948 = 0x2080;
short                    D_L16_0015F94C = 0x19;
short                    D_L16_0015F950 = 8;
T_23aaa8 *D_L16_0017E974 NOT_SDA = 0;
int                      D_L16_0015F978 = 0;
int                      D_L16_0015F97C = 0;
int                      D_L16_0015F980 = -1;
char                     D_L17_0015F7C8 = 0x25;
short                    D_L17_0015F924 = 0x46;
short                    D_L17_0015F928 = 0x5f;
short                    D_L17_0015F92C = -32672;
short                    D_L17_0015F930 = -32672;
short                    D_L17_0015F934 = 0x2080;
short                    D_L17_0015F938 = 0x2080;
short                    D_L17_0015F93C = 0x19;
short                    D_L17_0015F940 = 8;
T_23aaa8 *D_L17_0017EDF4 NOT_SDA = 0;
int                      D_L17_0015F968 = 0;
int                      D_L17_0015F96C = 0;
int                      D_L17_0015F970 = -1;
char                     D_L18_0015F7C8 = 0x25;
short                    D_L18_0015F924 = 0x46;
short                    D_L18_0015F928 = 0x5f;
short                    D_L18_0015F92C = -32672;
short                    D_L18_0015F930 = -32672;
short                    D_L18_0015F934 = 0x2080;
short                    D_L18_0015F938 = 0x2080;
short                    D_L18_0015F93C = 0x19;
short                    D_L18_0015F940 = 8;
T_23aaa8 *D_L18_0017EEF4 NOT_SDA = 0;
int                      D_L18_0015F968 = 0;
int                      D_L18_0015F96C = 0;
int                      D_L18_0015F970 = -1;

short ammo_hud_frame_width_below_100 __asm__("D_L00_0015F924") = 0x46;
short ammo_hud_frame_width_at_least_100 __asm__("D_L00_0015F928") = 0x5f;
short ammo_hud_nonzero_text_color_blend_start __asm__("D_L00_0015F92C") = -32672;
short ammo_hud_nonzero_text_color_blend_end __asm__("D_L00_0015F930") = -32672;
short ammo_hud_zero_text_color_blend_start __asm__("D_L00_0015F934") = 0x2080;
short ammo_hud_zero_text_color_blend_end __asm__("D_L00_0015F938") = 0x2080;
short ammo_hud_text_right_x_offset __asm__("D_L00_0015F93C") = 0x19;
short ammo_hud_text_y_offset __asm__("D_L00_0015F940") = 8;
int   D_L00_0015F96C = 0;
int   D_L00_0015F970 = -1;
