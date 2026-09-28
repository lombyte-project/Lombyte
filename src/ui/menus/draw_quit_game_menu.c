typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;
struct M2c_arg0
{
  u8 pad_0[0x20];
  s32 unk20;
  s32 unk24;
};
extern void func_001F4280(s32);
extern void func_001F4398();
extern s32 func_001F6250();
extern void func_001F6530(s32, s32, u64, s32, s32);
extern void func_001F6AF0(s32, s32, u64, s32, s32);
extern void func_001F7580(s32, u64, s32, s32);
extern s32 func_001FDD10();
extern void func_00233980();
extern void func_001153FC();
s32 draw_quit_game_menu(struct M2c_arg0 *arg0) __asm__("FUN_0021e890");

s32 draw_quit_game_menu(struct M2c_arg0 *arg0)
{
  unsigned long new_var;
  s16 packet[0xC];
  s32 temp_16_69;
  s32 temp_17_18;
  register s32 temp_17_81;
  s32 temp_18_25;
  s32 temp_2_73;
  s32 temp_16_16;
  temp_16_16 = arg0->unk20;
  temp_17_18 = arg0->unk24;
  func_001153FC(packet, 0, 0x18);
  temp_18_25 = (s32) (temp_16_16 + (((u32) temp_16_16) >> 0x1F));
  temp_18_25 = temp_18_25 >> 1;
  packet[1] = temp_17_18;
  packet[2] = 4;
  packet[3] = temp_16_16 - 4;
  packet[4] = temp_18_25;
  packet[5] = 6;
  packet[6] = temp_16_16;
  packet[8] = 16;
  packet[9] = 1;
  packet[7] = temp_17_18;
  func_001F4280(0);
  new_var = 0x2004B;
  func_00233980(0x47, new_var);
  func_001F7580(packet, 0x80FFA888, func_001FDD10(0x4F6D), -1);
  func_001F6AF0(temp_18_25, temp_17_18 - 0x40, 0x80FFA888, func_001FDD10(0x4F3F), -1);
  temp_16_69 = func_001F6250(func_001FDD10(0x524B), -1);
  temp_2_73 = func_001F6250(func_001FDD10(0x524F), -1);
  temp_17_81 = ((s32) (arg0->unk20 - ((temp_2_73 >= temp_16_69) ? (temp_2_73) : (temp_16_69)))) >> 1;
  func_001F6530(temp_17_81, temp_17_18 - 0x28, 0x80FFA888, func_001FDD10(0x524B), -1);
  func_001F6530(temp_17_81, temp_17_18 - 0x14, 0x80FFA888, func_001FDD10(0x524F), -1);
  func_001F4398();
  return 2;
}
