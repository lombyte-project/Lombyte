#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_0022f288/FUN_0022f288.s", FUN_0022f288);
#else

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef volatile s8 vs8;
typedef volatile u8 vu8;
typedef volatile s16 vs16;
typedef volatile u16 vu16;
typedef volatile s32 vs32;
typedef volatile u32 vu32;
typedef volatile s64 vs64;
typedef volatile u64 vu64;
typedef float f32;
typedef double f64;
typedef s32 b32;
struct LevelOverlayState
{
  u8 pad_0[0xD4];
  s32 stage;
  u8 pad_D8[0x4];
  s32 selection;
};
struct LevelRenderState
{
  u8 pad_0[0x58];
  s32 mode;
};
struct LevelDisplayState
{
  u8 pad_0[0x4];
  s32 screen_height;
};
struct LevelProjectionState
{
  u8 pad_0[0xB0];
  f32 projection_scale;
};
extern struct LevelOverlayState D_0013D290;
extern u8 D_0013DD43[];
extern struct LevelRenderState D_0013E030;
extern struct LevelDisplayState D_0013E500;
extern s32 D_0015ED84;
extern s32 D_0015F438;
extern f32 D_0015F43C;
extern s32 D_0015F620;
extern s32 D_0018CB54[];
extern s32 D_0018CC98[];
extern struct LevelProjectionState D_0018CD00;
extern void AppendDmaTag(u32);
extern void func_001F2260();
extern void update_view_context() __asm__("func_001F2D98");
extern void func_001F3868();
extern void func_001F4280(s32);
extern void func_001F4398();
extern s64 get_effect_texture(s32) __asm__("func_001F44B8");
extern void emit_rgba_draw_packet(s32, s32, s32, s32) __asm__("func_001F5210");
extern void draw_textured_quad(s32, s32, s32, s32, s32, s32, s32, s32, s64, s64) __asm__("func_001F5450");
extern s32 truncate_float_to_s32(f32) __asm__("func_001FA6D0");
extern void append_gif_transfer_packet() __asm__("func_001FB368");
extern void draw_rotated_sprite(s32, s32, s64, f32, f32, f32, f32, f32) __asm__("FUN_00200600");
extern void func_0020CC60(void);
extern void func_0020CEF8();
extern void func_0020D460(void);
extern void draw_sky_shells() __asm__("func_0022B288");
extern void build_resident_indexed_texture_warp_meshes(s32) __asm__("func_0022E420");
extern void draw_resident_textured_quad() __asm__("func_0022E8C8");
extern void draw_resident_textured_banner(s32) __asm__("func_0022EA08");
extern void render_environment_mapped_object(s32) __asm__("func_002327A0");
extern void vu1_sync_chain(s32) __asm__("func_002337B0");
extern void vu1_add_g_sregister(s32, s64) __asm__("func_00233980");



extern struct LevelDisplayState D_0013E500_far __asm__("D_0013E500") __attribute__((section(".data")));
void render_level_frame(void) __asm__("FUN_0022f288");

void render_level_frame(void)
{
  f32 rotation_angle;
  f32 screen_y;
  f32 quad_extent;
  s32 overlay_alpha;
  s64 unused_texture;
  f32 fov;
  struct LevelDisplayState *display_state;
  s64 texture;
  append_gif_transfer_packet();
  func_001F2260();
  func_0020CC60();
  func_001F3868();
  fov = D_0018CD00.projection_scale;
  D_0015F620 = -1;
  if (fov < 0.63f)
  {
    D_0018CD00.projection_scale = 0.63f;
  }
  update_view_context();
  func_001F2260();
  draw_sky_shells();
  D_0018CD00.projection_scale = fov;
  update_view_context();
  func_001F2260();
  if (D_0013E030.mode == 4)
  {
    func_001F4280(1);
    draw_resident_textured_quad();
    func_001F4398();
  }
  func_0020D460();
  AppendDmaTag(0x02080000);
  func_001F4280(1);
  if ((D_0015ED84 != 0) && ((D_0015ED84 != 1) || (D_0013DD43[0] != 0)))
  {
    build_resident_indexed_texture_warp_meshes(D_0018CC98[0]);
  }
  if ((D_0013E030.mode == 4) && (D_0018CB54[0] >= 0x3D))
  {
    overlay_alpha = (D_0018CB54[0] - 0x3C) * 2;
    if (overlay_alpha >= 0x81)
    {
      overlay_alpha = 0x80;
    }
    draw_resident_textured_banner(overlay_alpha);
  }
  if ((D_0015ED84 != 0) && ((D_0015ED84 != 1) || (D_0013DD43[0] != 0)))
  {
    render_environment_mapped_object(D_0018CC98[0]);
  }
  if ((D_0013D290.stage >= 3) || (D_0013D290.selection >= 0))
  {
    vu1_add_g_sregister(0x47, 0x3004B);
    quad_extent = 272.0f;
    texture = get_effect_texture(2);
    display_state = &D_0013E500_far;
    draw_textured_quad(0x2C, display_state->screen_height - 0x60, 0x40, 0x40, 0, 0, 0x40, 0x40, 0x80808080, texture);
    screen_y = (display_state->screen_height - 0x40) * 16;
    rotation_angle = ((D_0015F438 % 55) * (-6.2831855f)) / 55.0f;
    draw_rotated_sprite(0x40, 0x40, get_effect_texture(3), 1216.0f, screen_y, quad_extent, quad_extent, rotation_angle);
  }
  func_001F4398();
  if (D_0015F43C > 0.0f)
  {
    if (D_0015F43C > 1.0f)
    {
      D_0015F43C = 1.0f;
    }
    emit_rgba_draw_packet(0, 0, 0, truncate_float_to_s32(D_0015F43C * 128.0f));
  }
  vu1_sync_chain(0x10);
  func_0020CEF8();
}
#endif /* NON_MATCHING */
