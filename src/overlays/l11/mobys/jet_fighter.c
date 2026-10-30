/* Level 11 moby class jet_fighter: the player ship on Pokitaru, its flight, shots, lock-on and HUD. */
#include "types.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "qcopy.h"
#include "rnc/input/pad_state.h"
#include "rnc/game/usage_stats.h"
#include "rnc/game/item_state.h"
#include "sda.h"
#include "rnc/overlay/level_globals.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/mobys/animation/set_animation_parameter.h"
#include "rnc/mobys/refresh_moby_spatial_bounds_from_basis.h"
#include "rnc/overlay/shared/camera/scripted_camera.h"
#include "rnc/overlay/shared/camera/follow_camera_set_up_spring.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/rendering/view/update_view_context.h"
#include "rnc/ui/help/try_set_help_message.h"

extern float D_L11_0015F580[] MACRO_ADDR;
extern float D_L11_0015F3FC;

#include "rnc/overlay/cuboid.h"

extern float approach_value(float *, float, float) __asm__("FUN_00213ed8");
extern void
advance_accelerated_scalar(float *, float *, float, float, float, float) __asm__("FUN_00213f38");
extern void set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern void add_draw_callback(void *, void *) __asm__("FUN_001f4600");

extern float fast_add_rotations(float, float);
extern int   D_L11_0015FFD8;
extern int   allocate_voice_for_target_entry();

#include "qzero.h"

extern void  moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern int   coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern char  D_L11_001677C0[];

typedef struct {
    short tex;
    short u;
    short v;
    short w;
    short h;
    short fA;
    short x;
    short y;
} IconEC5D0;

extern IconEC5D0     l11_ship_radar_icon_definitions[] __asm__("D_L11_001F1058");
extern s64           get_effect_texture(s32) __asm__("func_001F44B8");
extern unsigned char frame_size[] __asm__("D_0013E504");
extern unsigned char D_0015EDB4[4];
extern void
    draw_textured_quad(s32, s32, s32, s32, s32, s32, s32, s32, s64, s64) __asm__("func_001F5450");

/* Draws a map icon for a moby relative to the hero, mirrored when the view is flipped. */
void draw_ship_radar_moby_icon(char *radar_reference_rotation,
                               char *from,
                               char *moby,
                               int   icon,
                               long  color) __asm__("FUN_L11_00311710");

void draw_ship_radar_moby_icon(

    char *radar_reference_rotation, char *from, char *moby, int icon, long color) {
    float      turn[4];
    float      radar_rotation_matrix[16];
    float      delta[4];
    float      radar_offset[4];
    float      radar_distance;
    float      side;
    int        x, y;
    IconEC5D0 *icon_definition;

    qzero(turn);
    turn[2] = *(float *)(radar_reference_rotation + 8);
    if (*(float *)(radar_reference_rotation + 4) > 1.5707964f ||
        *(float *)(radar_reference_rotation + 4) < -1.5707964f) {
        turn[2] = -*(float *)(radar_reference_rotation + 8);
    }
    turn[2] = fast_subtract_rotations(1.5707964f, turn[2]);
    euler_rotation_matrix(radar_rotation_matrix, turn);
    fast_vec_sub(delta, moby + 0x10, from);
    delta[2] = 0.0f;
    transform_vector(radar_offset, delta, radar_rotation_matrix);
    fast_vec_scale(radar_offset, radar_offset, 0.142857149f);
    radar_distance = fast_vec_length(radar_offset);
    side = 1.0f;
    if (D_0015EDB4[0] != 0) {
        side = -1.0f;
    }
    x = truncate_float_to_s32(side * radar_offset[0]);
    y = truncate_float_to_s32(-radar_offset[1]);
    if (radar_distance < 46.0f) {
        if (radar_distance > 38.0f) {
            float alpha_distance_product;
            radar_distance = 46.0f - radar_distance;
            alpha_distance_product =
                convert_integer_to_float(((unsigned long)color >> 24) & 0xFF) * radar_distance;
            color &= 0xFFFFFF;
            color = (truncate_float_to_s32(alpha_distance_product * 0.125f) << 24) | color;
        }
        icon_definition = &l11_ship_radar_icon_definitions[icon];
        y -= 0x50;
        y += *(int *)(frame_size);
        x += 0x1B0;
        draw_textured_quad(x - icon_definition->x,
                           y - icon_definition->y,
                           icon_definition->w,
                           icon_definition->h,
                           icon_definition->u,
                           icon_definition->v,
                           icon_definition->w,
                           icon_definition->h,
                           color,
                           get_effect_texture(icon_definition->tex));
    }
}
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_00312E10), where it is exact; names translated to the US level program. */

extern float compute_ship_target_reticle_distance_squared(void *, void *) __asm__("FUN_L11_00311a88"); /* no foreign declaration */
extern int    collect_oil_tanker_chain_mobys(char *moby, void **out) __asm__("FUN_L11_00318050");
extern short *D_L11_001AC240[];
void          count_active_and_visible_listed_mobys(void *moby,
                                                    void *active_moby_count,
                                                    void *visible_moby_count) __asm__("FUN_L11_00311be8");

void *select_ship_lock_on_target(void  *moby,
                                 char  *ship_vars,
                                 float *reference_position,
                                 float *reference_rotation,
                                 float  unused_float_parameter_0,
                                 float  unused_float_parameter_1,
                                 float  unused_float_parameter_2,
                                 void  *current_target,
                                 int    unused_parameter) __asm__("FUN_L11_00311948");
/* counts the listed mobys of the type that are active, and of those the ones with 0x31 set */
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_003130B0), where it is exact; names translated to the US level program. */

void *select_ship_lock_on_target(void  *moby,
                                 char  *ship_vars,
                                 float *reference_position,
                                 float *reference_rotation,
                                 float  unused_float_parameter_0,
                                 float  unused_float_parameter_1,
                                 float  unused_float_parameter_2,
                                 void  *current_target,

                                 int unused_parameter) {
    void  *best = 0;
    float  bestd = 1000.0f;
    short *moby_list_entry;
    void  *list[12];
    count_active_and_visible_listed_mobys(moby, ship_vars + 0xE8, ship_vars + 0xEA);
    if (current_target != 0) {
        if (compute_ship_target_reticle_distance_squared(moby, current_target) < bestd)
            return current_target;
    }
    moby_list_entry = D_L11_001AC240[*(short *)(ship_vars + 0x108)];
    if (moby_list_entry == 0)
        return 0;
    do {
        char *listed_moby =
            (char *)(((*(unsigned short *)moby_list_entry & 0x7FFF) << 8) + D_L11_0015FFD8);
        if (listed_moby[0x20] >= 0) {
            int moby_count = collect_oil_tanker_chain_mobys(listed_moby, list);
            int i;
            for (i = 1; i < moby_count; i++) {
                float reticle_distance_sq =
                    compute_ship_target_reticle_distance_squared(moby, list[i]);
                if (reticle_distance_sq < bestd) {
                    best = list[i];
                    bestd = reticle_distance_sq;
                }
            }
        }
    } while (*moby_list_entry++ >= 0);
    return best;
}
/* squared distance from a point to a projected target, or a large value when out of range */
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_00312F50), where it is exact; names translated to the US level program. */

extern char ship_target_projection_offset_reference[] __asm__("D_L11_001679E0");
extern void
project_world_point_to_screen_pixels(void *, void *, void *, int) __asm__("FUN_L11_00311210");

float compute_ship_target_reticle_distance_squared(void *moby_v, void *o_v) {
    struct Moby *moby = moby_v;
    char        *target_moby = o_v;
    char        *data = ((char *)moby->pvars);
    if (MOBY(target_moby)->visible != 0) {
        char target_projection_point[16];
        int  ax, ay, cx, cy;
        int  dx, dy, reticle_axis_limit;
        project_world_point_to_screen_pixels(target_moby + 0x10, &ax, &ay, 0);
        dx = ax - *(int *)(data + 0xE0);
        dy = ay - *(int *)(data + 0xE4);
        if (dx < 0)
            dx = -dx;
        if (dy < 0)
            dy = -dy;
        qcopy(target_projection_point, ship_target_projection_offset_reference);
        fast_vec_scale(target_projection_point,
                       ship_target_projection_offset_reference,
                       MOBY(target_moby)->bsphere.w / 1280.0f);
        fast_vec_add(target_projection_point, target_projection_point, target_moby + 0x10);
        project_world_point_to_screen_pixels(target_projection_point, &cx, &cy, 0);
        cx = cx - ax;
        cy = cy - ay;
        reticle_axis_limit =
            truncate_float_to_s32(square_root(convert_integer_to_float(cx * cx + cy * cy))) + 0x1C;
        if (dx < reticle_axis_limit && dy < reticle_axis_limit)
            return (float)(dx * dx + dy * dy);
    }
    return 10000.0f;
} /* no foreign declaration */

void count_active_and_visible_listed_mobys(void *moby,
                                           void *active_moby_count,

                                           void *visible_moby_count) {
    char  *moby_private_data = *(char **)((char *)moby + 0x78);
    int    type = *(short *)(moby_private_data + 0x108);
    short *moby_list_entry;
    *(unsigned short *)active_moby_count = 0;
    *(unsigned short *)visible_moby_count = 0;
    moby_list_entry = D_L11_001AC240[type];
    if (moby_list_entry == 0)
        return;
    do {
        int listed_moby_byte_offset = (*(unsigned short *)moby_list_entry & 0x7FFF) << 8;
        if (((char *)D_L11_0015FFD8)[listed_moby_byte_offset + 0x20] >= 0) {
            (*(unsigned short *)active_moby_count)++;
            if (((unsigned char *)D_L11_0015FFD8)[listed_moby_byte_offset + 0x31] != 0) {
                (*(unsigned short *)visible_moby_count)++;
            }
        }
    } while (*moby_list_entry++ >= 0);
}
/* Keeps the moby's current target, picking a new one when it is lost and restarting the timer. */

int          lock_hold_frames __asm__("D_L11_00162160") = 500;
int          lock_fade_frames __asm__("D_L11_00162164") = 100;
extern float D_L11_001677D0[];

void update_ship_lock_on_target(void *self, char *moby) __asm__("FUN_L11_00311c80");

void update_ship_lock_on_target(void *self, char *moby) {
    float          reference_rotation_copy[4] __attribute__((aligned(16)));
    float         *reference_rotation_source;
    unsigned char *current_target;
    void          *selected_target;
    if (hero.ship_ammo == 0) {
        *(void **)(moby + 0x88) = 0;
    } else {
        current_target = *(unsigned char **)(moby + 0x88);
        if (current_target != 0) {
            if (current_target[0x20] == 0xFE)
                *(void **)(moby + 0x88) = 0;
            else if (current_target[0x20] == 0xFD)
                *(void **)(moby + 0x88) = 0;
        }
        reference_rotation_source = D_L11_001677D0;
        qcopy(reference_rotation_copy, reference_rotation_source);
        selected_target = select_ship_lock_on_target(self,
                                                     moby,
                                                     reference_rotation_source - 4,
                                                     reference_rotation_copy,
                                                     0.19634955f,
                                                     0.19634955f,
                                                     255.0f,
                                                     *(void **)(moby + 0x88),
                                                     0);
        if (selected_target != *(void **)(moby + 0x88)) {
            int initial_lock_timer_frames = lock_hold_frames + lock_fade_frames;
            *(void **)(moby + 0x88) = selected_target;
            *(int *)(moby + 0x8C) = initial_lock_timer_frames;
        } else if (selected_target != 0) {
            fast_dec_timer(moby + 0x8C);
        }
    }
}

#include "rnc/rendering/screen.h"

/* pvars of the space-level ship: lock-on, reticle and HUD state. */
typedef struct {
    u8           pad0[0x88];
    struct Moby *target; /* 0x88: lock-on candidate picked by FUN_L11_00311c80 */
    s32 lock_timer; /* 0x8C: counts down from lock_hold_frames + lock_fade_frames once targeted */
    u8  pad90[0x50];
    s32 reticle_x; /* 0xE0: screen position of the aim reticle (FUN_L11_003125a8) */
    s32 reticle_y; /* 0xE4 */
    u8  padE8[4];
    struct Moby *locked;      /* 0xEC: target once the lock completed */
    f32          shown_hp;    /* 0xF0: health bar value, eased towards hero.ship_hp */
    s32          hp_bar_fill; /* 0xF4: palette entries currently lit in the health bar */
    u8           padF8[0x10];
    s16          target_class; /* 0x108: index into the lock-on lists D_L11_001AC240 */
    u8           pad10A[0x12];
    s32          low_hp_frames;   /* 0x11C: frames spent on low health since the last hint */
    s32          ammo_hint_timer; /* 0x120: delay before the out-of-ammo hint repeats */
    u8           pad124[4];
    s32          hint_frames; /* 0x128: frames until help 0x56 shows */
} ShipVars;

/* Help message records, laid out like the usage counters: FUN_L00_00203908
   shows message a for record b while b's count is below 0xFFFF. The ship
   uses 0x56 (message 0x2AFD), 0x57 (0x2AFE, low ammo) and 0x58 (0x2AFF, low
   health). */
extern struct UsageStat help_records[] __asm__("D_00141968");
extern s32              D_0015EEA4;
extern short ship_ammo_pip_points[] __asm__("D_L11_001F0CF8"); /* outline of an ammo pip */

/* The health bar's palette: jet_fighter_update points clut at it in texture
   memory and, the first time (saved), copies it to D_L11_001F1088. */
typedef struct {
    s32 *clut;
    s32  saved;
} HpBarPalette;

HpBarPalette hp_bar_palette __asm__("D_L11_0016216C") = {0};

extern s32
    ship_hp_bar_saved_palette[] __asm__("D_L11_001F1088"); /* the health bar palette as loaded */

extern void vu1_add_g_sregister(s32, u64) __asm__("FUN_00233980");
extern void append_rotated_sprite_quad(
    f32, f32, f32, f32, f32, s32, s32, s64, s32, s32, s32, s32, f32, f32) __asm__("FUN_001f5ab0");
extern void append_scaled_screen_triangle_strip(
    short *pts, int n, f32 x, f32 y, u32 col, u64 prim, f32 scale) __asm__("FUN_L11_00310ad0");
extern void draw_quarter_turn_textured_quads(
    f32 x, f32 y, f32 scale, f32 angle, u8 r, u8 g, u8 b, u8 a) __asm__("FUN_L11_00311048");
extern f32   normalize_angle(f32) __asm__("FUN_L00_0025e310");
extern s32   help_request(s32 message, s32 help) __asm__("FUN_L00_00203908");
extern char *get_help_message_text(s32) __asm__("func_001FDD10");
extern void
font_print_center_large(s32 x, s32 y, u64 color, char *text, s32 scale) __asm__("FUN_001f6c20");

/* Ship HUD: radar, ammo pips, reticle, lock-on marker, help hints and the health bar. */
void        draw_ship_hud(struct Moby *moby) __asm__("FUN_L11_00311d50");
extern void draw_ship_hud();

void draw_ship_hud(struct Moby *moby) {
    ShipVars *vars = (ShipVars *)moby->pvars;
    short    *list;
    void     *found[12];
    s32       sx;
    s32       sy;
    s32       i;
    s32       x;
    s32       y;
    u8        pips;
    s32       rx;
    s32       ry;
    s64       reticle_texture_tex0;
    f32       remaining_lock_fade_fraction;
    f32       grow;
    f32       fade;
    u8        alpha;
    s32       blink;
    f32       step;
    f32       size;
    s32       fill;
    s32       lo;
    s32       hi;
    s32       entry;
    s32       j;

    vu1_add_g_sregister(0x42, 0x8000000044);
    draw_textured_quad(0x170,
                       screen_extent.height - 0x90,
                       0x80,
                       0x80,
                       0,
                       0,
                       0x80,
                       0x80,
                       0x70808080,
                       get_effect_texture(0x3C));
    draw_textured_quad(0x170,
                       screen_extent.height - 0x90,
                       0x80,
                       0x80,
                       0,
                       0,
                       0x80,
                       0x80,
                       0x70808080,
                       get_effect_texture(0x3D));
    vu1_add_g_sregister(0x42, 0x8000000048);
    get_effect_texture(0x3E); /* fetched, but nothing below draws with it */

    list = D_L11_001AC240[vars->target_class];
    if (list != NULL) {
        do {
            /* D_L11_0015FFD8 holds the address of the level's moby array */
            struct Moby *listed_moby = (struct Moby *)(((*list & 0x7FFF) << 8) + D_L11_0015FFD8);

            if (listed_moby->state < 0x80) {
                s32 count = collect_oil_tanker_chain_mobys((char *)listed_moby, found);

                for (i = 0; i < count; i++) {
                    draw_ship_radar_moby_icon((char *)D_L11_001677D0,
                                              (char *)(D_L11_001677D0 - 4),
                                              found[i],
                                              0,
                                              0x75808080);
                }
            }
        } while (*list++ >= 0);
    }

    y = 0x40;
    x = 0x18;
    for (i = 0; i < hero.ship_ammo_max; i++) {
        if (i < hero.ship_ammo) {
            append_scaled_screen_triangle_strip(
                ship_ammo_pip_points, 0x19, x, y, 0xFFFFF3, 0x50008F00, 0.5f);
        } else {
            append_scaled_screen_triangle_strip(
                ship_ammo_pip_points, 0x19, x, y, 0xFFFFF3, 0x20004F00, 0.5f);
        }
        pips = hero.ship_ammo_max;
        if (i == pips >> 1) {
            y = 0x2E;
            x += 0x1E;
        }
        y += 0x12;
    }

    rx = vars->reticle_x;
    ry = vars->reticle_y;
    reticle_texture_tex0 = get_effect_texture(0x11);
    append_rotated_sprite_quad(rx,
                               ry,
                               40.0f,
                               40.0f,
                               0.0f,
                               0x3F,
                               0x3F,
                               reticle_texture_tex0,
                               0xFFFFF3,
                               0xFF20FF20,
                               0,
                               0,
                               0.5f,
                               0.5f);
    reticle_texture_tex0 = get_effect_texture(0x12);
    append_rotated_sprite_quad(rx,
                               ry,
                               40.0f,
                               40.0f,
                               0.0f,
                               0x3F,
                               0x3F,
                               reticle_texture_tex0,
                               0xFFFFF3,
                               0xFF20FF20,
                               0,
                               0,
                               0.5f,
                               0.5f);
    reticle_texture_tex0 = get_effect_texture(8);
    append_rotated_sprite_quad(rx,
                               ry,
                               10.0f,
                               10.0f,
                               0.0f,
                               0x1F,
                               0x1F,
                               reticle_texture_tex0,
                               0xFFFFF3,
                               0xFF20FF20,
                               0,
                               0,
                               0.5f,
                               0.5f);

    update_ship_lock_on_target(moby, (char *)vars);
    if (vars->target != NULL) {
        project_world_point_to_screen_pixels(&vars->target->pos, &sx, &sy, 0);
        if (vars->lock_timer > lock_hold_frames) {
            remaining_lock_fade_fraction =
                convert_integer_to_float(vars->lock_timer - lock_hold_frames) /
                convert_integer_to_float(lock_fade_frames);
            fade = 2.0f * (1.0f - remaining_lock_fade_fraction);
            grow = remaining_lock_fade_fraction * 5.0f + 1.0f;
            if (fade > 1.0f) {
                fade = 1.0f;
            }
            alpha = truncate_float_to_s32(fade * 96.0f);
            draw_quarter_turn_textured_quads(
                convert_integer_to_float(sx),
                convert_integer_to_float(sy),
                grow,
                normalize_angle(convert_integer_to_float(level_frame) / 30.0f),
                0,
                0xFF,
                0,
                alpha);
            vars->locked = NULL;
        } else {
            blink = 0xFF;
            if ((vars->lock_timer / scale_game_frames(0x14)) & 1) {
                blink = 0;
            }
            draw_quarter_turn_textured_quads(convert_integer_to_float(sx),
                                             convert_integer_to_float(sy),
                                             1.0f,
                                             0.0f,
                                             0xFF,
                                             blink,
                                             0,
                                             0x60);
            vars->locked = vars->target;
        }
    }

    if (hero.ship_hp < hero.ship_hp_max * 0.25f) {
        s32 shown = help_records[0x58].count;

        if (shown == 0) {
            help_request(0x2AFF, 0x58);
        } else if (shown < 2) {
            vars->low_hp_frames++;
            if (vars->low_hp_frames > scale_game_frames(1800)) {
                help_request(0x2AFF, 0x58);
            }
        }
    } else if (vars->low_hp_frames != 0) {
        if (help_records[0x58].count < 0xFFFF) {
            help_records[0x58].count++;
        }
        if (scale_game_frames(D_0015EEA4) / 600 > help_records[0x58].unk2) {
            help_records[0x58].unk2 = scale_game_frames(D_0015EEA4) / 600;
        }
        help_records[0x58].level_mask =
            help_records[0x58].level_mask | (1 << current_level_index) | 0x80000000;
        vars->low_hp_frames = 0;
    }

    fast_dec_timer(&vars->ammo_hint_timer);
    if ((hero.ship_ammo < 5 && help_records[0x57].count == 0) ||
        (hero.ship_ammo == 0 && help_records[0x57].count < 2 && vars->ammo_hint_timer == 0)) {
        help_request(0x2AFE, 0x57);
        vars->ammo_hint_timer = scale_game_frames(1200);
    }
    if (help_records[0x56].count == 0 && vars->hint_frames++ > scale_game_frames(3600)) {
        help_request(0x2AFD, 0x56);
    }

    if (moby->state == 8) {
        font_print_center_large(0x100, 0xC8, 0x80005080, get_help_message_text(0x523E), 0x11);
    }

    step = hero.ship_hp - vars->shown_hp;
    step *= 0.2f;
    size = absolute_float(step);
    if (size > 1.0f) {
        step /= size;
    }
    vars->shown_hp += step;
    fill = truncate_float_to_s32(vars->shown_hp * 251.0f / hero.ship_hp_max) + 2;
    if (fill > 0xFD) {
        fill = 0xFD;
    }
    if (fill < 2) {
        fill = 2;
    }
    lo = vars->hp_bar_fill <= fill ? vars->hp_bar_fill : fill;
    hi = vars->hp_bar_fill < fill ? fill : vars->hp_bar_fill;
    for (j = lo; j <= hi; j++) {
        /* CSM1 palette order: bits 3 and 4 of the entry swap */
        entry = (j & 0xE7) | ((j & 0x10) >> 1) | ((j & 8) << 1);
        if (j < fill) {
            hp_bar_palette.clut[entry] = ship_hp_bar_saved_palette[entry];
        } else {
            hp_bar_palette.clut[entry] = 0x80000000;
        }
    }
    vars->hp_bar_fill = fill;
}

extern int
spawn_fighter_jet_missile(void *, void *, void *, float, float) __asm__("FUN_L11_00308848");
extern f32           D_L11_00162090_gp __asm__("D_L11_00162090") __attribute__((sda));
extern unsigned char D_0013A4E0[];

extern short ship_shot_direction_base_scale_gp __asm__("D_L11_0016205C") __attribute__((sda));
int          D_L11_00162060 = 4;
void         update_ship_shot_firing_and_reticle(struct Moby *moby,
                                                 char        *ship_vars,
                                                 float        aim_azimuth,
                                                 float        aim_elevation) __asm__("FUN_L11_003125a8");
extern float D_L11_00162090_gp __asm__("D_L11_00162090") __attribute__((sda));

void update_ship_shot_firing_and_reticle(struct Moby *moby,
                                         char        *ship_vars,
                                         float        aim_azimuth,

                                         float aim_elevation) {
    char shot_direction[16];
    char projectile_spawn_position[16];
    char muzzle_forward_offset[16];
    char reticle_world_position[16];

    build_spherical_offset((f32 *)shot_direction,
                           *(float *)&ship_shot_direction_base_scale_gp *
                               *(float *)&D_L11_00162090_gp,
                           aim_azimuth,
                           aim_elevation);
    fast_vec_add(shot_direction, shot_direction, ship_vars);
    if (fast_dec_timer(ship_vars + 0x80) && (*(int *)(D_0013A4E0 + 0x2610) & 0x84)) {
        fast_vec_normalize(muzzle_forward_offset, shot_direction, 2.2f);
        moby_point_position(moby, 3 + (ship_vars[0x61] & 1), projectile_spawn_position);
        fast_vec_add(projectile_spawn_position, projectile_spawn_position, muzzle_forward_offset);
        spawn_fighter_jet_missile(moby, shot_direction, projectile_spawn_position, 200.0f, -1.0f);
        *(int *)(ship_vars + 0x80) = D_L11_00162060;
        ship_vars[0x61] ^= 1;
        allocate_voice_for_target_entry(3, 0, (int)moby);
    }
    fast_vec_normalize(shot_direction, shot_direction, 23.0f);
    fast_vec_add(reticle_world_position, shot_direction, &moby->pos);
    project_world_point_to_screen_pixels(
        reticle_world_position, ship_vars + 0xE0, ship_vars + 0xE4, 0);
}
/* on a pad press, fires the aimed shot and resets the cooldown */
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_00313BC0), where it is exact; names translated to the US level program. */

/* spawn_ship_rocket (ship_rocket.c) with the float scale before the int duration: the call only
   matches with its arguments in this order. */
extern int spawn_ship_rocket(void *, void *, int, void *, float, int) __asm__("FUN_L11_00309378");

extern short ship_projectile_base_scale_gp __asm__("D_L11_00162064") __attribute__((sda));
int          D_L11_00162068 = 30;
void try_fire_ship_projectile(struct Moby *moby, char *ship_vars) __asm__("FUN_L11_003126f8");

void try_fire_ship_projectile(struct Moby *moby, char *ship_vars) {
    if (fast_dec_timer(ship_vars + 0x84) && (*(int *)(D_0013A4E0 + 0x2610) & 0x28)) {
        if (hero.ship_ammo != 0) {
            char projectile_spawn_position[16];
            char projectile_spawn_offset[16];
            int  projectile_target = 0;
            int  projectile_spawn_slot = (*(unsigned short *)(ship_vars + 0x6A) + 1) & 3;
            *(unsigned short *)(ship_vars + 0x6A) = projectile_spawn_slot;
            moby_point_position(moby, projectile_spawn_slot + 7, projectile_spawn_position);
            fast_vec_normalize(projectile_spawn_offset, ship_vars, 0.3f);
            fast_vec_add(
                projectile_spawn_position, projectile_spawn_position, projectile_spawn_offset);
            if (*(int *)(ship_vars + 0x88) != 0 && !(500 < *(int *)(ship_vars + 0x8C)))
                projectile_target = *(int *)(ship_vars + 0x88);
            if (spawn_ship_rocket(moby,
                                  projectile_spawn_position,
                                  projectile_target,
                                  &moby->rot,
                                  *(float *)&ship_projectile_base_scale_gp *
                                      *(float *)&D_L11_00162090_gp,
                                  scale_game_frames(300))) {
                allocate_voice_for_target_entry(2, 0, (int)moby);
                hero.ship_ammo -= 1;
                *(int *)(ship_vars + 0x84) = D_L11_00162068;
            }
        }
    }
}
/* Flies the player's ship on Pokitaru: throttle and stick input, camera, and collision response.
   Same source as the exact FUN_L17_002ec360 without the path steering. */

typedef float        W4_2838[4] __attribute__((aligned(16)));
typedef unsigned int Q_2838 __attribute__((mode(TI), aligned(16)));

typedef union {
    Q_2838  q;
    W4_2838 v;
} UQ_2838;

typedef struct {
    UQ_2838 e[2];
} T2_2838;

typedef struct {
    char          pad00[0x10];
    W4_2838       v10;
    float         f20, f24, f28, f2C;
    float         f30, f34, f38, f3C;
    W4_2838       home_pos;
    W4_2838       home_rot;
    unsigned char sel;
    unsigned char flip;
    short         s62;
    float         speed;
    short         count;
    short         s6A;
    float         f6C;
    float         f70;
    char          pad74[0xC];
    int           i80;
    int           i84;
    char          pad88[8];
    float         f90, f94, f98, f9C;
    float         fA0, fA4, fA8, fAC;
    float         fB0, fB4, fB8, fBC;
    float         fC0, fC4, fC8, fCC;
    float         fD0, fD4, fD8;
    int           iDC;
    char          padE0[8];
    short         sE8;
    short         sEA;
    char          padEC[4];
    float         shown_hp;
    int           hp_bar_fill;
    int           iF8;
    int           exit_path;
    int           exit_path2;
    char          pad104[8];
    int           i10C;
    char          pad110[4];
    int           i114;
    int           i118;
    char          pad11C[0x10];
    short         voice;
    short         voice2;
    float         fade;
} ShipVars_2838;

extern unsigned short D_00141C10 NOT_SDA;
extern float D_L11_0016D4F0      NOT_SDA;
extern char                      jet_fighter_collision_query_result[] __asm__("D_L11_00174840");
int                              D_L11_00161FF0 = 120;
int                              D_L11_00162004 = 5;
int                              D_L11_0016214C = 10;

#include "rnc/overlay/l11/mobys/jet_fighter_data/update_player_ship_flight_15.h"

extern float D_L11_00161FF4_gp __asm__("D_L11_00161FF4") __attribute__((sda));

#include "rnc/overlay/l11/mobys/jet_fighter_data/update_player_ship_flight_14.h"

extern float D_L11_00162014 __attribute__((sda));
extern float D_L11_00162018 __attribute__((sda));
extern float D_L11_0016201C __attribute__((sda));
extern float D_L11_00162020 __attribute__((sda));

#include "rnc/overlay/l11/mobys/jet_fighter_data/update_player_ship_flight_13.h"

extern float D_L11_00162034 __attribute__((sda));
extern float D_L11_00162038 __attribute__((sda));

#include "rnc/overlay/l11/mobys/jet_fighter_data/update_player_ship_flight_12.h"

extern float D_L11_0016204C __attribute__((sda));

#include "rnc/overlay/l11/mobys/jet_fighter_data/update_player_ship_flight_11.h"

extern float D_L11_0016207C __attribute__((sda));
extern float D_L11_00162080 __attribute__((sda));
extern float D_L11_0016208C __attribute__((sda));
extern float D_L11_0016209C __attribute__((sda));
extern float D_L11_001620A0 __attribute__((sda));

#include "rnc/overlay/l11/mobys/jet_fighter_data/update_player_ship_flight_10.h"

extern float D_L11_001620AC __attribute__((sda));
extern float D_L11_001620B0 __attribute__((sda));

#include "rnc/overlay/l11/mobys/jet_fighter_data/update_player_ship_flight_9.h"

extern float D_L11_001620BC __attribute__((sda));
extern float D_L11_001620C0 __attribute__((sda));

#include "rnc/overlay/l11/mobys/jet_fighter_data/update_player_ship_flight_8.h"

extern float D_L11_001620E0 __attribute__((sda));
extern float D_L11_001620E4 __attribute__((sda));

#include "rnc/overlay/l11/mobys/jet_fighter_data/update_player_ship_flight_7.h"

extern float D_L11_001620EC __attribute__((sda));
extern float D_L11_001620F0 __attribute__((sda));

#include "rnc/overlay/l11/mobys/jet_fighter_data/update_player_ship_flight_6.h"

extern float D_L11_001620F8 __attribute__((sda));
extern float D_L11_001620FC __attribute__((sda));

#include "rnc/overlay/l11/mobys/jet_fighter_data/update_player_ship_flight_5.h"

extern float D_L11_00162104 __attribute__((sda));
extern float D_L11_00162108 __attribute__((sda));

#include "rnc/overlay/l11/mobys/jet_fighter_data/update_player_ship_flight_4.h"

extern float D_L11_00162110 __attribute__((sda));
extern float D_L11_00162114 __attribute__((sda));

#include "rnc/overlay/l11/mobys/jet_fighter_data/update_player_ship_flight_3.h"

extern float D_L11_0016211C __attribute__((sda));
extern float D_L11_00162120 __attribute__((sda));

#include "rnc/overlay/l11/mobys/jet_fighter_data/update_player_ship_flight_2.h"

extern float D_L11_00162128 __attribute__((sda));
extern float D_L11_0016212C __attribute__((sda));
extern float D_L11_00162130 __attribute__((sda));

#include "rnc/overlay/l11/mobys/jet_fighter_data/update_player_ship_flight.h"

extern float D_L11_00162150 __attribute__((sda));
extern float D_L11_00162154 __attribute__((sda));
extern float D_L11_00162158 __attribute__((sda));
extern float vendor_spring(float *p, float *v, float t, float u1, float u2, float eps) __asm__("FUN_L00_0025b8c0");
extern float
spring_angle(float *, float *, int, float, float, float, float) __asm__("FUN_L00_0025bc98");
extern void spawn_explosion(void *,
                            void *,
                            void *,
                            float,
                            float,
                            int,
                            int,
                            int,
                            float,
                            float,
                            float,
                            float,
                            int,
                            float,
                            int,
                            int,
                            int,
                            int) __asm__("FUN_L00_0025e450");
void        adjust_ship_steering_for_flight_bounds(
           struct Moby *moby,
           float       *horizontal_steering_input,
           float       *vertical_steering_input) __asm__("FUN_L11_00313f60");

void adjust_ship_steering_for_flight_bounds(struct Moby *moby,
                                            float       *a,
                                            float       *b) __asm__("FUN_L11_00313f60");

void update_player_ship_flight(struct Moby   *ship_moby,
                               ShipVars_2838 *ship_vars) __asm__("FUN_L11_00312838");

void update_player_ship_flight(struct Moby *ship_moby, ShipVars_2838 *ship_vars) {
    W4_2838          old;
    W4_2838          death_explosion_velocity;
    UQ_2838          up;
    W4_2838          ship_movement_vector;
    W4_2838          side;
    W4_2838          camera_rotation;
    W4_2838          target;
    W4_2838          camera_local_offset;
    float            horizontal_steering_input;
    float            vertical_steering_input;
    float            ka;
    float            kb;
    float            x, y;
    float            half_fov, tn;
    float            ratio;
    float            collision_adjusted_pitch;
    float            k;
    float            fov;
    struct PadState *pad;
    struct Hero     *hero_input_source;
    char            *collision_result;
    int              hc;

    up.q = 0;
    ship_moby->flags &= 0xFEFF;
    up.v[2] = frame_time * 8.0f * D_L11_00162090_gp;
    *(Q_2838 *)death_explosion_velocity = up.q;
    up.q = 0;
    up.v[2] = 1.0f;
    if (controller_state.raw_pressed & PAD_LEFT) {
        if (++ship_vars->sel >= 2) {
            ship_vars->sel = 0;
        }
    }
    if (D_L11_00161FF0 < ship_vars->count) {
        pad = &controller_state;
        if (pad->raw_held & 0x40) {
            float accelerated_speed;
            float max;
            if (usage_stats.stat[28].count < 0xFFFF) {
                usage_stats.stat[28].count++;
            }
            if (scale_game_frames(D_0015EEA4) / 600 > usage_stats.stat[28].unk2) {
                usage_stats.stat[28].unk2 = scale_game_frames(D_0015EEA4) / 600;
            }
            usage_stats.stat[28].level_mask =
                usage_stats.stat[28].level_mask | (1 << current_level_index) | 0x80000000;
            accelerated_speed =
                ship_vars->speed + jet_fighter_boost_speed_increment_gp * D_L11_00162090_gp;
            max = jet_fighter_boost_speed_limit_gp * D_L11_00162090_gp;
            ship_vars->speed = accelerated_speed;
            if (max < accelerated_speed) {
                ship_vars->speed = max;
            }
        } else {
            float hi = D_L11_00161FF4_gp * D_L11_00162090_gp;
            float step = jet_fighter_cruise_speed_increment_gp * D_L11_00162090_gp;
            float current_speed = ship_vars->speed;
            if (current_speed < hi - step) {
                ship_vars->speed = current_speed + step;
            } else if (hi < current_speed) {
                ship_vars->speed =
                    current_speed +
                    (hi - current_speed) * jet_fighter_cruise_speed_relaxation_factor_gp;
            } else {
                ship_vars->speed = hi;
            }
        }
    } else {
        ship_vars->speed += jet_fighter_startup_speed_increment_gp * D_L11_00162090_gp;
        ship_vars->count += 1;
    }

    if (D_L11_00162004 < ship_vars->count) {
        hero_input_source = &hero;
        x = hero_input_source->stick_x;
        y = hero_input_source->stick_y;
        ka = jet_fighter_horizontal_steering_snap_threshold_scale_gp;
        kb = jet_fighter_vertical_steering_snap_threshold_scale_gp;
        horizontal_steering_input = x;
        vertical_steering_input = y;
        if (controller_state.held & PAD_L3) {
            horizontal_steering_input = x / D_L11_00162018 * D_L11_00162020;
            vertical_steering_input = y / D_L11_00162014 * D_L11_0016201C;
            ka = jet_fighter_l3_horizontal_steering_snap_threshold_scale_gp;
            kb = jet_fighter_l3_vertical_steering_snap_threshold_scale_gp;
        }
        adjust_ship_steering_for_flight_bounds(
            ship_moby, &horizontal_steering_input, &vertical_steering_input);
        vendor_spring(&ship_vars->f6C,
                      &ship_vars->f90,
                      horizontal_steering_input,
                      D_L11_0016209C,
                      D_L11_001620A0,
                      ka);
        vendor_spring(&ship_vars->f70,
                      &ship_vars->f94,
                      vertical_steering_input,
                      D_L11_001620AC,
                      D_L11_001620B0,
                      kb);
        spring_angle(&ship_vars->f30,
                     &ship_vars->f98,
                     0,
                     ship_vars->f6C * D_L11_0016207C,
                     D_L11_001620BC,
                     D_L11_001620C0,
                     jet_fighter_rotation_x_snap_threshold_scale_gp);
        ship_vars->f34 = fast_subtract_rotations(ship_vars->f34, ship_vars->f70 * D_L11_00162014);
        if (ship_vars->f34 > 1.3962634f) {
            ship_vars->f34 = 1.3962634f;
        } else if (ship_vars->f34 < -1.3962634f) {
            ship_vars->f34 = -1.3962634f;
        }
        ship_vars->f38 = fast_subtract_rotations(ship_vars->f38, ship_vars->f6C * D_L11_00162018);
    }

    build_spherical_offset(ship_movement_vector, ship_vars->speed, ship_vars->f38, -ship_vars->f34);
    fast_vec_normalize(ship_vars->v10, ship_movement_vector, 1.0f);
    qcopy(old, &ship_moby->pos);
    fast_vec_add(&ship_moby->pos, &ship_moby->pos, ship_movement_vector);
    {
        float lo = D_L11_00161FF4_gp;
        float range = jet_fighter_boost_speed_limit_gp - lo;
        float sp = ship_vars->speed - lo;
        if (sp < 0.0f) {
            ratio = 0.0f / range;
        } else {
            ratio = sp / range;
        }
    }
    ship_vars->fC4 = ratio;
    {
        T2_2838 camera_offset_targets;
        T2_2838 wr;
        float   ship_rotation_basis[12];
        wr.e[0].q = 0;
        wr.e[1].q = 0;
        wr.e[0].v[0] = (jet_fighter_camera_base_local_x_offset_gp -
                        ship_vars->f70 * D_L11_00162128 - ratio * D_L11_00162130) *
                       D_L11_0016208C;
        wr.e[0].v[1] = ship_vars->f6C * D_L11_00162080 * D_L11_0016208C;
        wr.e[0].v[2] =
            (jet_fighter_camera_base_local_z_offset_gp + ship_vars->f70 * D_L11_0016212C) *
            D_L11_0016208C;
        wr.e[1].v[0] = jet_fighter_alternate_camera_local_x_offset_gp * D_L11_0016208C;
        wr.e[1].v[2] = jet_fighter_alternate_camera_local_z_offset_gp * D_L11_0016208C;
        camera_offset_targets.e[0].q = wr.e[0].q;
        camera_offset_targets.e[1].q = wr.e[1].q;
        vendor_spring(&ship_vars->fB4,
                      &ship_vars->fC8,
                      camera_offset_targets.e[ship_vars->sel].v[0],
                      D_L11_00162104,
                      D_L11_00162108,
                      jet_fighter_camera_local_x_snap_threshold_scale_gp);
        vendor_spring(&ship_vars->fB8,
                      &ship_vars->fCC,
                      camera_offset_targets.e[ship_vars->sel].v[1],
                      D_L11_00162110,
                      D_L11_00162114,
                      jet_fighter_camera_local_y_snap_threshold_scale_gp);
        vendor_spring(&ship_vars->fBC,
                      &ship_vars->fD0,
                      camera_offset_targets.e[ship_vars->sel].v[2],
                      D_L11_0016211C,
                      D_L11_00162120,
                      jet_fighter_camera_local_z_snap_threshold_scale_gp);
        camera_local_offset[0] = ship_vars->fB4;
        camera_local_offset[1] = ship_vars->fB8;
        camera_local_offset[2] = ship_vars->fBC;
        camera_local_offset[3] = 0.0f;
        euler_rotation_basis(ship_rotation_basis, &ship_vars->f30);
        transform_vector_by_basis(target, camera_local_offset, ship_rotation_basis);
        fast_vec_add(wr.e[0].v, &ship_moby->pos, target);
        set_camera_position(wr.e[0].v);
        wr.e[1].v[0] =
            fast_add_rotations(ship_vars->f30 * D_L11_00162038, ship_vars->f6C * D_L11_00162034);
        wr.e[1].v[1] = fast_subtract_rotations(
            fast_add_rotations(ship_vars->f34, jet_fighter_camera_pitch_offset_radians_gp),
            ship_vars->f70 * D_L11_00162014);
        wr.e[1].v[2] = ship_vars->f38;
        spring_angle(&ship_vars->f20,
                     &ship_vars->fA0,
                     0,
                     wr.e[1].v[0],
                     D_L11_001620E0,
                     D_L11_001620E4,
                     jet_fighter_camera_rotation_x_snap_threshold_scale_gp);
        spring_angle(&ship_vars->f24,
                     &ship_vars->fA4,
                     0,
                     wr.e[1].v[1],
                     D_L11_001620EC,
                     D_L11_001620F0,
                     jet_fighter_camera_rotation_y_snap_threshold_scale_gp);
        spring_angle(&ship_vars->f28,
                     &ship_vars->fA8,
                     0,
                     wr.e[1].v[2],
                     D_L11_001620F8,
                     D_L11_001620FC,
                     jet_fighter_camera_rotation_z_snap_threshold_scale_gp);
    }
    camera_rotation[0] = ship_vars->f20;
    camera_rotation[2] = fast_subtract_rotations(ship_vars->f28, ship_vars->fD4);
    camera_rotation[1] = fast_subtract_rotations(ship_vars->f24, ship_vars->fD8);
    set_camera_rotation(camera_rotation);
    if (controller_state.raw_held & PAD_CROSS) {
        fov = jet_fighter_boost_camera_fov_radians_gp;
    } else {
        fov = jet_fighter_cruise_camera_fov_radians_gp;
    }
    advance_accelerated_scalar(&ship_vars->fB0,
                               &ship_vars->fC0,
                               fov,
                               frame_time_sq + frame_time_sq,
                               frame_time_sq + frame_time_sq,
                               frame_time + frame_time);
    half_fov = ship_vars->fB0 * 0.5f;
    tn = fast_sin(half_fov) / fast_cos(half_fov);
    D_L11_0016D4F0 = tn;
    update_view_context();
    ship_moby->rot.x = fast_add_rotations(ship_vars->f30, ship_vars->f6C * D_L11_00162150);
    ship_moby->rot.y = fast_add_rotations(ship_vars->f34, -ship_vars->f70 * D_L11_00162154);
    ship_moby->rot.z = fast_add_rotations(ship_vars->f38, -ship_vars->f6C * D_L11_00162158);
    if (coll_sphere(2.0f, &ship_moby->pos, 0, ship_moby)) {
        collision_result = jet_fighter_collision_query_result;
        if (*(char **)(collision_result + 0x18) == 0) {
            spawn_explosion(ship_moby,
                            D_L11_0015F580,
                            0,
                            8.0f,
                            100.0f,
                            0x1E,
                            0xA,
                            0x18,
                            16.0f,
                            8.0f,
                            9.0f,
                            1.0f,
                            -1,
                            50.0f,
                            1,
                            1,
                            -1,
                            0);
            ship_moby->visible = 0;
            ship_moby->state = 7;
            ship_moby->flags |= 1;
            ship_vars->iF8 = scale_game_frames(0x5A);
        } else {
            hc = *(short *)(*(char **)(collision_result + 0x18) + 0xA6);
            if (hc != 0 && hc != 0x192 && hc != 0x4C2 && hc != 0x4C3) {
                qcopy(&ship_moby->pos, collision_result + 0x30);
                reflect_vector(ship_movement_vector, ship_movement_vector, collision_result + 0x40);
                ship_vars->f38 = fast_add_rotations(
                    ship_vars->f38,
                    fast_subtract_rotations(
                        fast_arc_tan(ship_movement_vector[0], ship_movement_vector[1]),
                        ship_vars->f38) *
                        jet_fighter_yaw_deflection_scale_gp);
                collision_adjusted_pitch = fast_add_rotations(
                    ship_vars->f34,
                    fast_subtract_rotations(-fast_arc_tan(vector_length_xy(ship_movement_vector),
                                                          ship_movement_vector[2]),
                                            ship_vars->f34) *
                        jet_fighter_pitch_deflection_scale_gp);
                k = ship_vars->speed * D_L11_0016204C;
                ship_vars->f34 = collision_adjusted_pitch;
                ship_vars->speed = k;
                if (ship_vars->iDC == 0) {
                    hero.ship_hp = hero.ship_hp - jet_fighter_collision_damage_gp;
                    if (hero.ship_hp < 0.0f) {
                        fast_vec_sub(side, collision_result + 0x50, collision_result + 0x60);
                        fast_vec_normalize(side, side, 1.0f);
                        reflect_vector(death_explosion_velocity,
                                       ship_movement_vector,
                                       collision_result + 0x40);
                        fast_vec_normalize(death_explosion_velocity,
                                           death_explosion_velocity,
                                           frame_time + frame_time);
                        fast_vec_normalize(up.v, collision_result + 0x40, 1.0f);
                        spawn_explosion(ship_moby,
                                        death_explosion_velocity,
                                        0,
                                        8.0f,
                                        100.0f,
                                        0x1E,
                                        0xA,
                                        0x18,
                                        16.0f,
                                        8.0f,
                                        9.0f,
                                        1.0f,
                                        -1,
                                        50.0f,
                                        1,
                                        1,
                                        -1,
                                        0);
                        ship_moby->state = 7;
                    }
                    ship_vars->iDC = D_L11_0016214C;
                }
            }
        }
    }
    if (D_00141C10 == 0) {
        help_request(0x2AFC, 0x55);
    }
    add_draw_callback(draw_ship_hud, ship_moby);
    try_fire_ship_projectile(ship_moby, (char *)ship_vars);
    update_ship_shot_firing_and_reticle(
        ship_moby,
        (char *)ship_vars,
        fast_add_rotations(ship_vars->f28, -ship_vars->f6C / 7.0f),
        -fast_add_rotations(ship_vars->f24, -(ship_vars->f70 / 7.0f) - 0.1f));
}
/* Per-frame update for the player's ship moby on Pokitaru: takes hits, boards and leaves the ship,
   and runs the flight states. Same source as the exact fighter_jet_update with Pokitaru's states. */

typedef struct {
    char    pad00[0x10];
    W4_2838 push;
    char    pad20[0xC];
    float   damage;
    int     flags;
} ShipHit_3290;

typedef struct {
    char           pad0[0xA];
    unsigned short idx;
    char           padC[4];
} ShipLevelEntry_3290;

typedef struct {
    char b[0x400];
} Block400_3290;

extern ShipLevelEntry_3290 D_L11_0016DA80[];
extern char               *ship_hp_bar_palette_address_base __asm__("D_L11_0015F420");
extern unsigned char       D_0014C050[][16];
extern char jet_fighter_missing_victory_jet_destination_message[] __asm__("D_L11_0021B610");
extern char jet_fighter_missing_victory_hero_destination_message[] __asm__("D_L11_0021B648");
extern int  D_L11_0015F608;

#include "rnc/overlay/l11/mobys/jet_fighter_data/jet_fighter_update_2.h"

unsigned char l11_jet_fighter_initial_ammo __asm__("D_L11_00162054") = 0xa;
unsigned char l11_jet_fighter_max_ammo __asm__("D_L11_00162058") = 0x14;

#include "rnc/overlay/l11/mobys/jet_fighter_data/jet_fighter_update.h"

extern float D_L11_00162078 __attribute__((sda));
int          D_L11_00162168 = 180;
extern float D_L11_0016D4F0_a[] __asm__("D_L11_0016D4F0");
extern void  print_3290(char *) __asm__("FUN_001e93b0");
extern void  FUN_L11_0024db50(int, int);
extern float jet_distance(void *, void *) __asm__("FUN_L00_00250430");
extern void  request_music_track(int, int) __asm__("FUN_L01_0027a248");
extern void  hero_respawn(void) __asm__("FUN_L00_00211250");
extern void  jet_clear_persistent(int) __asm__("FUN_L00_002502a0");
extern void  emit_type_74_particles_from_moby_joints(void *, void *) __asm__("FUN_L11_003112b8");
extern int   voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern int   count_listed_blarg_ships_in_state_5(int) __asm__("FUN_L11_0031a758");
extern int   activate_listed_blarg_ship(int) __asm__("FUN_L11_0031a7d8");

s32  hp_bar_saved __asm__("D_L11_00162170") = 0;
void jet_fighter_update(struct Moby *moby) __asm__("FUN_L11_00313290");

void jet_fighter_update(struct Moby *moby) {
    enum {
        JET_FIGHTER_STATE_INITIALIZE = 0,
        JET_FIGHTER_STATE_WAIT_FOR_BOARDING = 1,
        JET_FIGHTER_STATE_BOARD = 2,
        JET_FIGHTER_STATE_FLIGHT = 4,
        JET_FIGHTER_STATE_DISMOUNT = 5,
        JET_FIGHTER_STATE_WAIT_FOR_RESPAWN = 7,
        JET_FIGHTER_STATE_DELAYED_EXIT = 8,
        JET_FIGHTER_STATE_INACTIVE = 9
    };
    ShipVars_2838 *ship_vars;
    ShipHit_3290  *hit;
    W4_2838        ship_velocity_scratch;
    char          *voice_entry;
    int            idx;
    int            frames_since_help_timestamp;
    float          k;
    float          fov;

    if (moby != 0) {
        ship_vars = (ShipVars_2838 *)moby->pvars;
        fast_dec_timer(&ship_vars->iDC);
        if (moby->state != 7 && moby->state != 1 && moby->state != 2) {
            hit = find_moby_hit(moby, 0x30001, 0);
            if (hit != 0 && moby->state == 4) {
                struct Hero *g = &hero;
                if (hit->damage < g->ship_hp) {
                    g->ship_hp -= hit->damage;
                    if (hit->flags & 1) {
                        k = hit->damage * D_L11_00162078;
                        build_spherical_offset(ship_velocity_scratch,
                                               ship_vars->speed,
                                               ship_vars->f38,
                                               -ship_vars->f34);
                        fast_vec_add(&moby->pos, &moby->pos, hit->push);
                        reflect_vector(ship_velocity_scratch, ship_velocity_scratch, hit->push);
                        ship_vars->f38 = fast_add_rotations(
                            ship_vars->f38,
                            fast_subtract_rotations(
                                fast_arc_tan(ship_velocity_scratch[0], ship_velocity_scratch[1]),
                                ship_vars->f38) *
                                jet_fighter_yaw_deflection_scale_gp * k);
                        ship_vars->f34 = fast_add_rotations(
                            ship_vars->f34,
                            fast_subtract_rotations(
                                fast_arc_tan(vector_length_xy(ship_velocity_scratch),
                                             ship_velocity_scratch[2]),
                                ship_vars->f34) *
                                jet_fighter_pitch_deflection_scale_gp * k);
                    }
                } else {
                    build_spherical_offset(
                        ship_velocity_scratch, ship_vars->speed, ship_vars->f38, -ship_vars->f34);
                    g->ship_hp = 0.0f;
                    moby->state = 7;
                    ship_vars->s62 = 0xB4;
                    g->motion.pos.f[2] += 0.2f;
                    g->moby->collision_disable = 0;
                    FUN_L11_0024db50(0, 1);
                    g->ship_moby = 0;
                    g->ship_oclass = -1;
                    spawn_explosion(moby,
                                    ship_velocity_scratch,
                                    0,
                                    0.0f,
                                    0.0f,
                                    10,
                                    3,
                                    16,
                                    4.0f,
                                    2.0f,
                                    9.0f,
                                    1.0f,
                                    -1,
                                    15.0f,
                                    1,
                                    1,
                                    -1,
                                    0);
                }
            }
        }
        moby->hit_slot = 0xFF;
        {
            struct Hero *g = &hero;
            if (g->ship_moby == moby) {
                g->ship_flags &= ~2;
                g->ship_hp_percent = g->ship_hp / jet_fighter_max_health_gp * 200.0f;
            }
        }
        switch (moby->state) {
        case JET_FIGHTER_STATE_INITIALIZE:
            moby->state = 1;
            ship_vars->sel = 0;
            moby->scale = *(float *)((char *)moby->pclass + 0x24);
            ship_vars->f20 = ship_vars->f30;
            ship_vars->f24 = ship_vars->f34;
            fov = fast_arc_tan(1.0f, D_L11_0016D4F0_a[0]) * 2.0f;
            ship_vars->fC0 = 0.0f;
            ship_vars->fAC = fov;
            ship_vars->fB0 = fov;
            qcopy_nc(&ship_vars->f30, &moby->rot);
            qcopy_nc(ship_vars->home_rot, &moby->rot);
            qcopy_nc(ship_vars->home_pos, &moby->pos);
            hp_bar_palette.clut =
                (s32 *)(ship_hp_bar_palette_address_base + D_L11_0016DA80[0x4C].idx * 16);
            if (hp_bar_saved == 0) {
                *(Block400_3290 *)ship_hp_bar_saved_palette = *(Block400_3290 *)hp_bar_palette.clut;
            }
            hp_bar_saved = 1;
            ship_vars->iF8 = 1;
            ship_vars->sE8 = 1;
            ship_vars->fade = 0.0f;
            if (moby->mission != 0xFF && D_0014C050[current_level_index][moby->mission] == 0xFF) {
                moby->state = 9;
                moby->collision = 0;
                moby->flags |= 1;
                moby->visible = 0;
            }
            break;
        case JET_FIGHTER_STATE_WAIT_FOR_BOARDING: {
            struct Hero *g = &hero;
            if (jet_distance(moby, g->moby) < jet_fighter_boarding_distance_gp) {
                if (item_available[7] != 0) {
                    try_set_help_message(7, 0x2B09);
                } else {
                    frames_since_help_timestamp =
                        scale_game_frames(D_0015EEA4) - help_records[0x3C].unk2 * 600;
                    if (frames_since_help_timestamp >
                            (int)((float)scale_game_frames(0x12) * 60.0f) ||
                        help_records[0x3C].unk2 * 600 == 0) {
                        help_request(0x2AFA, 0x3C);
                    } else if (scale_game_frames(D_0015EEA4) / 600 > help_records[0x3C].unk2) {
                        help_records[0x3C].unk2 = scale_game_frames(D_0015EEA4) / 600;
                    }
                }
                if (item_available[7] != 0 && (D_0013CAE4 & 0x10)) {
                    moby->state = 2;
                    ship_vars->i118 = -1;
                }
            } else {
                ship_vars->i118 = -1;
                if (ship_vars->fade != 0.0f) {
                    approach_value(&ship_vars->fade, 0.0f, frame_time * 4.0f);
                    D_L11_0015F3FC = ship_vars->fade;
                }
            }
            break;
        }
        case JET_FIGHTER_STATE_BOARD:
            approach_value(&ship_vars->fade, 1.0f, frame_time * 4.0f);
            D_L11_0015F3FC = ship_vars->fade;
            if (ship_vars->fade == 1.0f) {
                struct Hero *g;
                FUN_L11_0024db50(0x32, 1);
                moby->pos.z += 30.0f;
                request_music_track(2, 6);
                clear_u64_value(&ship_vars->f30);
                clear_u64_value(&ship_vars->f20);
                ship_vars->f38 = ship_vars->f28 = moby->rot.z;
                ship_vars->i114 = scale_game_frames(0x708);
                if (help_records[0x55].count == 0) {
                    help_request(0x2AFC, 0x55);
                }
                g = &hero;
                moby->state = 4;
                g->ship_moby = moby;
                g->ship_oclass = moby->oclass;
                g->ship_ammo = l11_jet_fighter_initial_ammo;
                g->unk160E = 1;
                g->ship_ammo_max = l11_jet_fighter_max_ammo;
                g->unk15F8 = l11_jet_fighter_max_ammo;
                g->ship_hp = jet_fighter_initial_health_gp;
                ship_vars->shown_hp = 0.0f;
                ship_vars->hp_bar_fill = 0xFF;
                g->ship_hp_max = jet_fighter_max_health_gp;
                g->unk1604 = 100.0f;
                g->unk15F9 = 3;
                g->unk15FA = 3;
                g->unk15FB = 3;
                g->ship_flags = 0;
                jet_mount(D_L11_001677C0, D_L11_001677C0 + 0x10, 1, 0, 0);
                ship_vars->count = 0;
                ship_vars->speed = jet_fighter_initial_flight_speed_gp;
                ship_vars->f6C = 0.0f;
                ship_vars->f70 = 0.0f;
                ship_vars->i84 = 0;
                ship_vars->i80 = 0;
                ship_vars->fD4 = 0.0f;
                ship_vars->fD8 = 0.0f;
                moby->draw_dist = 0xFF;
                moby->update_dist = 0xFF;
                ship_vars->fB4 = jet_fighter_camera_base_local_x_offset_gp * D_L11_0016208C;
                ship_vars->fBC = jet_fighter_camera_base_local_z_offset_gp * D_L11_0016208C;
                ship_vars->fB8 = 0.0f;
                g->moby->collision_disable = -1;
                moby->scale = *(float *)((char *)moby->pclass + 0x24) * D_L11_0016208C;
                g->unk20A5 = 1;
            }
            break;
        case JET_FIGHTER_STATE_FLIGHT:
            if (ship_vars->fade != 0.0f) {
                approach_value(&ship_vars->fade, 0.0f, frame_time * 4.0f);
                D_L11_0015F3FC = ship_vars->fade;
            }
            D_L11_0015F608 = 2;
            update_player_ship_flight(moby, ship_vars);
            {
                int alive = count_listed_blarg_ships_in_state_5(ship_vars->i10C);
                if (ship_vars->sEA != 0 && alive < 5 - ship_vars->sE8 &&
                    fast_dec_timer(&ship_vars->i114) &&
                    activate_listed_blarg_ship(ship_vars->i10C)) {
                    ship_vars->i114 = scale_game_frames(0xF0);
                }
            }
            if (ship_vars->sE8 <= 0) {
                ship_vars->iF8 = D_L11_00162168;
                moby->state = 8;
            }
            if (hero.ship_flags & 1) {
                moby->state = 5;
            }
            break;
        case JET_FIGHTER_STATE_DISMOUNT: {
            char        *p;
            struct Hero *g = &hero;
            g->unk20A5 = 0;
            g->moby->collision_disable = 0;
            FUN_L11_0024db50(0, 1);
            jet_end_mount(0);
            set_animation_parameter(g->unk160C, 0);
            request_music_track(0, 8);
            qcopy_nc(&moby->pos, ship_vars->home_pos);
            qcopy_nc(&moby->rot, ship_vars->home_rot);
            moby->scale = *(float *)((char *)moby->pclass + 0x24);
            refresh_moby_spatial_bounds_from_basis(moby);
            p = (char *)l11_cuboids + (ship_vars->exit_path << 7);
            set_race_camera(p + 0x30, p + 0x70, 0, 1);
            idx = ship_vars->voice;
            if (idx != -1) {
                voice_entry = ((char *)&voice_pool) + idx * 0x70;
                if (*(struct Moby **)(voice_entry + 0x88) == moby &&
                    *(unsigned char *)(voice_entry + 0x74) != 0) {
                    release_voice_slot(idx);
                }
            }
            ship_vars->voice = -1;
            idx = ship_vars->voice2;
            if (idx != -1) {
                voice_entry = ((char *)&voice_pool) + idx * 0x70;
                if (*(struct Moby **)(voice_entry + 0x88) == moby &&
                    *(unsigned char *)(voice_entry + 0x74) != 0) {
                    release_voice_slot(idx);
                }
            }
            ship_vars->voice2 = -1;
            moby->state = 1;
            ship_vars->fade = 0.99f;
            D_L11_0015F3FC = 0.99f;
            break;
        }
        case JET_FIGHTER_STATE_WAIT_FOR_RESPAWN:
            D_L11_0015F608 = 2;
            if (fast_dec_timer(&ship_vars->iF8)) {
                hero_respawn();
            }
            break;
        case JET_FIGHTER_STATE_DELAYED_EXIT:
            D_L11_0015F608 = 2;
            update_player_ship_flight(moby, ship_vars);
            if (fast_dec_timer(&ship_vars->iF8)) {
                approach_value(&ship_vars->fade, 1.0f, frame_time * 4.0f);
                D_L11_0015F3FC = ship_vars->fade;
                if (ship_vars->fade >= 1.0f) {
                    char        *p;
                    struct Hero *g = &hero;
                    g->unk20A5 = 0;
                    g->moby->collision_disable = 0;
                    FUN_L11_0024db50(0, 1);
                    jet_end_mount(0);
                    request_music_track(0, 8);
                    moby->visible = 0;
                    moby->collision = 0;
                    moby->flags |= 1;
                    if (ship_vars->exit_path2 == -1) {
                        print_3290(jet_fighter_missing_victory_jet_destination_message);
                        qcopy_nc(&moby->pos, ship_vars->home_pos);
                        qcopy_nc(&moby->rot, ship_vars->home_rot);
                    } else {
                        qcopy_nc(&moby->pos,
                                 ((char(*)[128])(char *)l11_cuboids)[ship_vars->exit_path2] + 0x30);
                        qcopy_nc(&moby->rot,
                                 ((char(*)[128])(char *)l11_cuboids)[ship_vars->exit_path2] + 0x70);
                    }
                    refresh_moby_spatial_bounds_from_basis(moby);
                    if (ship_vars->exit_path == -1) {
                        print_3290(jet_fighter_missing_victory_hero_destination_message);
                    } else {
                        p = (char *)l11_cuboids + (ship_vars->exit_path << 7);
                        set_race_camera(p + 0x30, p + 0x70, 0, 1);
                    }
                    if (moby->mission != 0xFF) {
                        jet_clear_persistent(moby->mission);
                    }
                    moby->state = 9;
                }
            }
            break;
        case JET_FIGHTER_STATE_INACTIVE:
            if (ship_vars->fade != 0.0f) {
                approach_value(&ship_vars->fade, 0.0f, frame_time * 4.0f);
                D_L11_0015F3FC = ship_vars->fade;
            }
            break;
        case 10:
            break;
        }
        if (moby->pos.x < 15.0f) {
            moby->pos.x = 15.0f;
        }
        if (moby->pos.x > 1008.0f) {
            moby->pos.x = 1008.0f;
        }
        if (moby->pos.y < 15.0f) {
            moby->pos.y = 15.0f;
        }
        if (moby->pos.y > 1008.0f) {
            moby->pos.y = 1008.0f;
        }
        if (moby->pos.z < 15.0f) {
            moby->pos.z = 15.0f;
        }
        if (moby->pos.z > 1008.0f) {
            moby->pos.z = 1008.0f;
        }
        if (moby->state == 4) {
            qcopy_nc(&hero.motion.pos, &moby->pos);
            qcopy_nc(&hero.motion.rot, &ship_vars->f30);
        }
        if (moby->state == 3 || moby->state == 4) {
            emit_type_74_particles_from_moby_joints(moby, ship_vars);
            if (voice_is_playing(moby, ship_vars->voice) == 0) {
                ship_vars->voice = allocate_voice_for_target_entry(0, 4, moby);
            }
        } else {
            idx = ship_vars->voice;
            if (idx != -1) {
                voice_entry = ((char *)&voice_pool) + idx * 0x70;
                if (*(struct Moby **)(voice_entry + 0x88) == moby &&
                    *(unsigned char *)(voice_entry + 0x74) != 0) {
                    release_voice_slot(idx);
                }
            }
            ship_vars->voice = -1;
        }
    }
}
/* Same source as the exact FUN_L17_002edf10, with its path index at 0x104 and a fixed floor of 250. */

extern LevelCuboid *l11_cuboids_macro __asm__("D_L11_001600EC") MACRO_ADDR;

void adjust_ship_steering_for_flight_bounds(struct Moby *moby,
                                            float       *horizontal_steering_input,

                                            float *vertical_steering_input) {
    char *ship_vars = (char *)moby->pvars;
    float predicted_position_scratch[4];
    float zz, predicted_boundary_distance_xy, boundary_radius, hi, lo, boundary_center_heading,
        upper_altitude_limit, k, radial_steering_blend_weight, heading_difference;
    fast_vec_scale(predicted_position_scratch,
                   ship_vars + 0x10,
                   *(float *)(ship_vars + 0x64) * (float)scale_game_frames(0x3C));
    fast_vec_add(predicted_position_scratch, predicted_position_scratch, &moby->pos);
    zz = predicted_position_scratch[2];
    fast_vec_sub(predicted_position_scratch,
                 predicted_position_scratch,
                 (*(int *)(ship_vars + 0x104) << 7) + (char *)l11_cuboids + 0x30);
    predicted_boundary_distance_xy = vector_length_xy(predicted_position_scratch);
    boundary_radius = vector_length_xy((*(int *)(ship_vars + 0x104) << 7) + (char *)l11_cuboids);
    hi = l11_cuboids_macro[*(int *)(ship_vars + 0x104)].axes[2].z;
    lo = l11_cuboids_macro[*(int *)(ship_vars + 0x104)].pos.z;
    upper_altitude_limit = hi + lo;
    boundary_center_heading = fast_add_rotations(
        fast_arc_tan(predicted_position_scratch[0], predicted_position_scratch[1]),
        3.141590118408203f);
    if (boundary_radius - 10.0f < predicted_boundary_distance_xy) {
        radial_steering_blend_weight =
            (predicted_boundary_distance_xy - boundary_radius + 10.0f) / 10.0f;
        heading_difference =
            fast_subtract_rotations(boundary_center_heading, *(float *)(ship_vars + 0x38));
        if (radial_steering_blend_weight > 1.0f)
            radial_steering_blend_weight = 1.0f;
        else if (radial_steering_blend_weight < 0.0f)
            radial_steering_blend_weight = 0.0f;
        if (heading_difference > 0.0f) {
            *horizontal_steering_input =
                (-1.0f - *horizontal_steering_input) * radial_steering_blend_weight +
                *horizontal_steering_input;
        } else {
            *horizontal_steering_input =
                (1.0f - *horizontal_steering_input) * radial_steering_blend_weight +
                *horizontal_steering_input;
        }
        hero.ship_flags |= 2;
    }
    if (upper_altitude_limit - 10.0f < zz) {
        float q, w;
        q = (zz - upper_altitude_limit + 10.0f) / 10.0f;
        w = (0.5235987901687622f - *(float *)(ship_vars + 0x34)) / 0.5235987901687622f;
        if (q > 1.0f)
            q = 1.0f;
        else if (q < 0.0f)
            q = 0.0f;
        if (w > 1.0f)
            w = 1.0f;
        else if (w < 0.0f)
            w = 0.0f;
        q = q * w;
        *vertical_steering_input =
            (-1.0f - *vertical_steering_input) * q + *vertical_steering_input;
        hero.ship_flags |= 2;
    } else if (zz < 260.0f) {
        float q, w;
        q = (250.0f - zz + 10.0f) / 10.0f;
        w = (*(float *)(ship_vars + 0x34) + 0.5235987901687622f) / 0.5235987901687622f;
        if (q > 1.0f)
            q = 1.0f;
        else if (q < 0.0f)
            q = 0.0f;
        if (w > 1.0f)
            w = 1.0f;
        else if (w < 0.0f)
            w = 0.0f;
        q = q * w;
        *vertical_steering_input = (1.0f - *vertical_steering_input) * q + *vertical_steering_input;
        hero.ship_flags |= 2;
    }
}
