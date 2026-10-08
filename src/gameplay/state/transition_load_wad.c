#include "types.h"
#include "rnc/storage/disc_table.h"
typedef struct 
{
  s32 x0;
  s32 data;
  s32 x8;
  s32 xC;
  s32 x10;
  s32 x14;
  s32 n18;
  s32 x1C;
  s32 n20;
  s32 x24;
  s32 n28;
  s32 x2C;
  s32 n30;
  s32 x34;
  s32 n38;
  s32 x3C;
  s32 n40;
  s32 x44;
  s32 n48;
  s32 x4C;
  s32 x50;
  s32 x54;
  s32 x58;
  s32 x5C;
  s32 x60;
  s32 x64;
  s32 x68;
  s32 x6C;
  s32 x70;
  s32 x74;
  s32 x78;
  s32 x7C;
  s32 x80;
  s32 x84;
} WadHeader;
typedef struct 
{
  s32 offset;
  u16 x4;
  u16 pad6;
  s32 pad8[2];
} WadTex;
typedef struct 
{
  s32 offset;
  s32 x4;
  s32 pad8[2];
  u8 x10[0x10];
} WadClass20;
typedef struct 
{
  s32 offset;
  s32 x4;
  s32 pad8[2];
  u8 x10[0x10];
  u8 x20[0x10];
} WadClass30;
typedef struct 
{
  s32 offset;
  s32 size;
} WadSound;
typedef struct 
{
  u8 pad0[0x14];
  u8 *hdr;
  s32 x18;
  s32 x1C;
} LoadState;
typedef struct 
{
  u8 pad0[0x58];
  s32 x58;
  s32 x5C;
  s32 x60[0x46];
} SoundBanks;
typedef struct 
{
  s32 offset;
  s32 pad[3];
} TextEntry;
typedef struct 
{
  u8 pad0[0x2C];
  s32 count;
} HelpState;
typedef struct 
{
  u8 pad0[0xD];
  u8 xD;
  u8 padE[0x1A];
  void *x28;
} MobyClass;
extern s32 D_0015EE74;
extern s32 D_0015EE78;
extern s32 D_0015EE8C;
extern s64 D_0015EF48;
extern s32 D_0015EF58;
extern s32 D_0015EF60;
extern s32 D_0015EF64;
extern s32 D_0015F460;
extern TextEntry *D_0015F6A0;
extern s32 D_0015FF00;
extern s32 D_0015FF08;
extern s32 D_001603CC;
extern s32 D_001603EC;
extern s32 D_00160E94;
extern s32 D_00160F0C;
extern s32 D_00160F4C;
extern s32 D_00160F64;
extern u8 D_001861E0[];
extern u8 D_00186310[];
extern SoundBanks D_0018CB20;
extern LoadState D_001940C0;
extern u8 D_00194180[];
extern HelpState D_001996D0;
extern u64 D_0019E6C0[];
extern MobyClass *D_001B3200[];
extern u8 D_001B3AC0[];
extern s32 D_001B5980[];
extern s32 D_001B6180[];
extern u8 D_001B6880[];
extern s32 D_001D84B0[];
extern s32 D_001E0900[];
extern s32 D_001E2600[];
extern void CalculateDmaTransferAddress(void);
extern void FillTransferWords(void *, s32, s32);
extern void FlushCache(s32);
extern void QueueDmaTransfer(s32);
extern void FUN_00120558(s32, s32);
extern s32 FUN_001e9b10(u8 *);
extern void init_view_context(void) __asm__("FUN_001f2c60");
extern void update_view_context(void) __asm__("FUN_001f2d98");
extern volatile char func_001F97A0(s32);
extern void init_mem_slots(void) __asm__("FUN_002015d8");
extern void parse_particle_textures(u8 *, u8 *, u8 *, s32) __asm__("FUN_002026c8");
extern void unpack_point_records(u8 *, s32) __asm__("FUN_00202800");
extern void relocate_sky_definition(u8 *) __asm__("FUN_002028e0");
extern void upload_texture_images(u8 *, s32, u8 *) __asm__("FUN_00203120");
extern void register_moby_class(u8 *, u8 *, u8 *, s32) __asm__("FUN_00203640");
extern void register_object_render_class(u8 *, u8 *, u8 *, s32) __asm__("func_00203730");
extern void register_shrub_render_class(u8 *, u8 *, u8 *, u8 *, s32) __asm__("func_00203B08");
extern void initialize_tfrag_render_data(u8 *, u8 *) __asm__("func_002040E0");
extern void parse_space_scene_chunk(s32) __asm__("FUN_002049f0");
extern s32 FUN_0020b618(u8 *, u8 *);
extern void start_audio_stream_read(u8 *, s32, s32) __asm__("FUN_00216788");
extern volatile unsigned int load(s32, s32, s32) __asm__("func_00216828");
extern void update_audio_stream_until_idle(s32) __asm__("FUN_002168a8");
extern void vu1_init_chain(void) __asm__("FUN_002335d0");
extern s32 sceGsSetDefLoadImage(void *, s16, s16, s16, s16, s16, s16, s16);
extern s32 sceGsExecLoadImage(void *, u8 *);



extern s32 D_0015EF64_far __asm__("D_0015EF64") __attribute__((section(".data")));
void transition_load_wad(void) __asm__("FUN_001ea830");

void transition_load_wad(void)
{
  u8 li[0x60];
  WadHeader *hdr;
  u8 *data;
  u8 *base;
  s32 size;
  s32 i;
  s32 j;
  s32 k;
  s32 cnt;
  s32 v;
  s32 *out;
  int new_var;
  WadTex *tex;
  s32 new_var4;
  WadClass20 *c20;
  u8 *new_var3;
  WadClass30 *c30;
  WadSound *snd;
  TextEntry *te;
  WadTex *new_var2;
  s32 k_800 = 0x800;
  register s32 adj;
  s64 t;
  s64 u;
  D_0015EF58 = 0;
  i = 0;
  CalculateDmaTransferAddress();
  init_mem_slots();
  D_00160F0C = 0x100000;
  D_0015EE8C = 0x2C0000;
  D_0015EE78 = 0x2C0000;
  D_0015EE74 = 0x2C0000;
  FillTransferWords(D_00194180, 0x87654321, 0x10);
  FillTransferWords(D_001B3AC0, -1, k_800);
  FillTransferWords(D_001B6880, -1, 0xE00);
  FillTransferWords(D_001B6180, 0, 0xE0);
  init_view_context();
  update_view_context();
  vu1_init_chain();
  start_audio_stream_read(D_001940C0.hdr + 0x1000000, disc_table.unk14E8.sector, disc_table.unk14E8.size);
  update_audio_stream_until_idle(1);
  FlushCache(0);
  size = FUN_0020b618(D_001940C0.hdr + 0x1000000, D_001940C0.hdr);
  FlushCache(0);
  hdr = (WadHeader *) D_001940C0.hdr;
  upload_texture_images(((u8 *) hdr) + hdr->x0, hdr->x8, ((u8 *) hdr) + hdr->xC);
  t = ((s64) ((D_0015EE8C + hdr->x70) >> 8)) | 0x1D308000;
  u = (((s64) ((D_0015EE8C + hdr->x74) >> 8)) << 37) | (((s64) 0xB800) << 19);
  data = ((u8 *) hdr) + hdr->data;
  base = data + hdr->x60;
  D_0019E6C0[0] = (t | u) | (((s64) (-1)) << 63);
  D_0019E6C0[1] = 0xFFA0000000E0;
  D_0019E6C0[2] = 0x0040000400004000;
  tex = (WadTex *) (((u8 *) hdr) + hdr->x34);
  if ((D_00160E94 = hdr->n30) > 0)
  {
    do
    {
      D_001E0900[i] = (((s32) base) + tex[i].offset) + (func_001F97A0(tex[i].x4) << 28);
      i++;
    }
    while (i < D_00160E94);
  }
  k = 0;
  tex = (new_var2 = (WadTex *) (((u8 *) hdr) + hdr->x3C));
  if ((D_0015FF08 = hdr->n38) > 0)
  {
    do
    {
      D_001B5980[k] = (((s32) base) + tex[k].offset) + (func_001F97A0(tex[k].x4) << 28);
      k++;
    }
    while (k < D_0015FF08);
  }
  k = 0;
  tex = (new_var2 = (WadTex *) (((u8 *) hdr) + hdr->x44));
  if ((D_00160F64 = hdr->n40) > 0)
  {
    do
    {
      D_001E2600[k] = (((s32) base) + tex[k].offset) + (func_001F97A0(tex[k].x4) << 28);
      k++;
    }
    while (k < D_00160F64);
  }
  k = 0;
  tex = (new_var2 = (WadTex *) (((u8 *) hdr) + hdr->x4C));
  if ((D_001603EC = hdr->n48) > 0)
  {
    do
    {
      D_001D84B0[k] = (((s32) base) + tex[k].offset) + (func_001F97A0(tex[k].x4) << 28);
      k++;
    }
    while (k < D_001603EC);
  }
  initialize_tfrag_render_data(data + hdr->x10, ((u8 *) hdr) + hdr->x34);
  relocate_sky_definition(data + hdr->x14);
  c20 = (WadClass20 *) (((u8 *) hdr) + hdr->x1C);
  D_0015FF00 = 0;
  D_00160F4C = 0;
  D_001603CC = 0;
  for (k = 0; k < hdr->n18; k++)
  {
    register_moby_class((c20->offset != 0) ? (data + c20->offset) : (0), ((u8 *) hdr) + hdr->x3C, c20->x10, c20->x4);
    c20++;
  }

  c20 = (WadClass20 *) (((u8 *) hdr) + hdr->x24);
  for (k = 0; k < hdr->n20; k++)
  {
    register_object_render_class(data + c20->offset, ((u8 *) hdr) + hdr->x44, c20->x10, c20->x4);
    c20++;
  }

  c30 = (WadClass30 *) (((u8 *) hdr) + hdr->x2C);
  for (k = 0; k < hdr->n28; k++)
  {
    register_shrub_render_class(data + c30->offset, ((u8 *) hdr) + hdr->x4C, c30->x10, c30->x20, c30->x4);
    c30++;
  }

  D_0015F460 = (s32) (data + hdr->x68);
  unpack_point_records(((u8 *) hdr) + hdr->x5C, hdr->x58);
  new_var3 = data + hdr->x64;
  new_var4 = hdr->x54;
  parse_particle_textures(((u8 *) hdr) + (new_var = hdr->x6C), new_var3, ((u8 *) hdr) + new_var4, hdr->x50);
  sceGsSetDefLoadImage(li, (D_0015EE74 << 8) >> 16, 4, 0, 0, 0, 0x100, 0x80);
  FlushCache(0);
  sceGsExecLoadImage(li, data + hdr->x84);
  FUN_00120558(0, 0);
  D_001940C0.x18 = ((s32) D_001940C0.hdr) + size;
  v = D_0015EE74;
  D_0015EF48 = ((v >> 8) | 0x20010000) | (((s64) 0xB800) << 19);
  D_0015EE74 = v + 0x20000;
  D_0015EE78 = v + 0x20000;
  D_001940C0.x1C = FUN_001e9b10(data + hdr->x7C);
  FillTransferWords(&D_0018CB20, 0, 0x1C0);
  FillTransferWords(D_00186310, 0, 0x40);
  D_0018CB20.x58 = D_001940C0.x1C;
  D_0018CB20.x5C = (D_001940C0.x1C += 0x40000);
  D_001940C0.x1C += 0x40000;
  snd = (WadSound *) (data + hdr->x80);
  cnt = 0;
  if (snd->size != 0)
  {
    out = D_0018CB20.x60;
    do
    {
      cnt++;
      *out = ((s32) (data + hdr->x80)) + (snd->offset + k_800);
      snd++;
      out++;
    }
    while ((cnt < 0x46) && (snd->size != 0));
  }
  parse_space_scene_chunk(0);
  D_0015EF60 = D_001940C0.x1C;
  load(D_001940C0.x1C, disc_table.help_text.sector, disc_table.help_text.size);
  D_0015EF64 = D_0015EF60;
  D_001940C0.x1C = D_0015EF60 + (disc_table.help_text.size << 11);
  for (k = 0; k < 8; k++)
  {
    QueueDmaTransfer(k);
    j = 0;
    if (D_001996D0.count > 0)
    {
      te = D_0015F6A0;
      adj = ((s32) te) - 8;
      {
        do
        {
          te[j].offset += adj;
          j++;
        }
        while (j < D_001996D0.count);
      }
    }
  }

  QueueDmaTransfer(0);
  cnt = (new_var = D_001B3AC0[0x472]);
  if (cnt >= 0)
  {
    D_001B3200[cnt]->x28 = D_001861E0;
    D_001B3200[cnt]->xD = 5;
  }
}

extern __typeof__(transition_load_wad) func_001EA830 __attribute__((alias("FUN_001ea830")));
