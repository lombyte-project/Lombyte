#include "types.h"

struct M2c_D_0013C940 { u8 pad_0[0x1C4]; s32 unk1C4; };
struct M2c_D_00141848 { u8 pad_0[0xA8]; u16 unkA8; u16 unkAA; s32 unkAC; };
struct M2c_temp_3_16 { u8 pad_0[0x3C]; s32 unk3C; u8 pad_40[0x8]; s32 unk48; };
struct M2c_D_001D5BF4 { u8 pad_0[0x40]; struct M2c_temp_3_16 *unk40; };
struct M2c_arg0 { u8 pad_0[0x14]; s32 unk14; u8 pad_18[0x18]; s32 unk30[8]; s32 unk50; };

extern struct M2c_D_0013C940 D_0013C940;
extern u8 D_0013D4C0[];
extern struct M2c_D_00141848 D_00141848;
extern s32 D_0015ED84;
extern s32 D_0015EEA4;
extern struct M2c_D_001D5BF4 *D_001D5BF4[];
extern s32 FUN_001f96f8();
extern s32 func_0022DA68();

s32 FUN_0021c7a0(struct M2c_arg0 *arg0) {
  s16 id;
  s32 *slots;
  s32 *scan;
  s32 cur;
  s32 tmp;
  int index;
  int start_index;
  int i;
  struct M2c_temp_3_16 *table;

  table = D_001D5BF4[0]->unk40;
  index = table->unk3C;
  id = *((s16 *) (((u8 *) (table->unk48 + (index * 0xA))) + 0x6));
  start_index = arg0->unk50;
  if (D_0013C940.unk1C4 & 8) {
    arg0->unk50 = (start_index + 1) % 8;
  }
  if (D_0013C940.unk1C4 & 4) {
    tmp = arg0->unk50;
    arg0->unk50 = (tmp + 7) % 8;
  }
  cur = arg0->unk50;
  if (cur != start_index) {
    func_0022DA68(1, 0x11, arg0->unk14);
  }
  if (id != 0) {
    if (D_0013D4C0[id] != 0 && (D_0013C940.unk1C4 & 0x40)) {
      if ((u16) D_00141848.unkA8 <= 0xFFFEU) {
        D_00141848.unkA8 = (u16) (D_00141848.unkA8 + 1);
      }
      tmp = FUN_001f96f8(D_0015EEA4) / 600;
      if ((s32) D_00141848.unkAA < tmp) {
        D_00141848.unkAA = (u16) (FUN_001f96f8(D_0015EEA4) / 600);
      }
      slots = arg0->unk30;
      D_00141848.unkAC = (s32) ((D_00141848.unkAC | (1 << D_0015ED84)) | 0x80000000);
      i = 0;
      if (arg0->unk30[0] != id) {
        scan = slots;
        loop_17:
        i += 1;
        scan += 1;
        if (i < 8) {
          if ((*scan) == id) {
            goto block_19;
          }
          goto loop_17;
        }
      } else {
        block_19:
        if (i < 8) {
          arg0->unk30[i] = 0;
        }
      }
      arg0->unk30[arg0->unk50] = (s32) id;
      arg0->unk50 = (arg0->unk50 + 1) % 8;
    }
  }
  return 0;
}

extern __typeof__(FUN_0021c7a0) func_0021C7A0 __attribute__((alias("FUN_0021c7a0")));
