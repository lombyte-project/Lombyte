/* Level 14 moby classes: morphoray, button, blarg_heavy_interceptor. */
#include "types.h"
#include "rnc/overlay/level_globals.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "rnc/gameplay/entities/moby.h"
#include "asm.h"

typedef struct {
    char pad0[0xB0];
    float fB0; /* tan of half the field of view */
} VendorView_5290;
extern VendorView_5290 D_L14_0016D140;
#include "qcopy.h"

/* Oltanis (level 14) vendor code.  Both bodies below were written from the
 * retail disassembly and reproduce it byte for byte on the game compiler
 * route (cc1, Ps2EeAs). */

extern int D_001413DC __attribute__((section(".data")));
extern u8 *D_L14_00167500 __attribute__((section(".data")));

/* 0x003135B8, 80 bytes.  When the level record's field at 0x86 is 7, seed the
 * sub-record reached through +0x70 with the caller's argument and two float
 * constants, and set both floats on the record itself. */
void FUN_L14_003135b8(u8 *flying_car_moby) {
    u8 *link_source_record = D_L14_00167500;
    u8 *linked_subrecord;
    u8 *linked_record;

    if (*(s16 *)(link_source_record + 0x86) != 7) {
        return;
    }
    linked_subrecord = *(u8 **)(link_source_record + 0x70) + 0x40;
    *(s32 *)(linked_subrecord + 0x40) = 1;
    *(s32 *)(linked_subrecord + 0x44) = (s32)flying_car_moby;
    *(f32 *)(linked_subrecord + 0x4C) = 2.0f;
    linked_record = *(u8 **)(link_source_record + 0x70);
    *(f32 *)(linked_record + 0xA0) = 7.0f;
    *(f32 *)(linked_record + 0xB0) = 2.0f;
}

/* 0x003167E0, 36 bytes.  While the executable's mode word is not 0xF, mark the
 * object's field at 0x7E as 3. */
void FUN_L14_003167e0(u8 *target_object) {
    if (D_001413DC != 0x0F) {
        *(s16 *)(target_object + 0x7E) = 3;
    }
}

extern u8 *create_moby(s32) __asm__("FUN_0020c4f8");
extern void FUN_L00_00250df8(u8 *);

/* Allocate 0x574 bytes, copy the two 16-byte records out of the caller, stamp
 * the header fields and hand it to the level's post-create hook.  Returns the
 * allocation, null when it failed.  The two 16-byte copies go through $2 with
 * each address in its own register -- the qcopy idiom, see include/qcopy.h. */
u8 *FUN_L14_00306158(u8 *self) {
    u8 *created_moby = create_moby(0x574);

    if (created_moby != 0) {
        qcopy(created_moby + 0x10, self + 0x10);
        qcopy(created_moby + 0x40, self + 0x40);
        created_moby[0x30] = 0x40;
        created_moby[0x20] = 0;
        *(s16 *)(created_moby + 0x32) = 0x40;
        created_moby[0x31] = 1;
        FUN_L00_00250df8(created_moby);
    }
    return created_moby;
}

/* A bouncing spark: falls under gravity leaving a three-particle trail while its trail timer runs, bounces off
 * collision (reflected and damped, a click per bounce) and after four bounces rolls on at a decaying speed
 * while it fades from red to its colour; it bursts when it hits another owner's target or its timer ends.
 * Its spin follows its speed. */
typedef struct {
    char pad0[0x10];
    Vec4f vel;                  /* w: gravity while bouncing, then the rolling speed */
    int fade;                   /* 0x20 */
    int owner;                  /* 0x24 */
    short trail;                /* 0x28 */
    short bounces;              /* 0x2A */
    float fade_rate;            /* 0x2C */
} Spark_0e00;
typedef struct {
    char pad0[0x18];
    int owner;                  /* 0x18 */
    int pad1C;
    Vec4f hit;                  /* 0x20 */
    Vec4f pad30;
    Vec4f normal;               /* 0x40 */
} Coll_0e00;
typedef struct { float r, g, b; } Rgb_0e00;
extern Coll_0e00 D_L14_00174540;
extern int D_L14_00174558 __attribute__((section(".data")));
extern int D_L14_0016208C_0e00 __asm__("D_L14_0016208C") __attribute__((sda));
extern float D_L14_00162088 __attribute__((sda));
extern float D_L14_00162090 __attribute__((sda));
extern float D_L14_00162094 __attribute__((sda));
extern float D_L14_001620A0 __attribute__((sda));
extern float D_L14_001620A4 __attribute__((sda));
extern Rgb_0e00 D_L14_001620A8;
extern char D_L14_0015F580[] __attribute__((section(".sdata")));
extern int on_ground_0e00(void *, int, void *, float) __asm__("FUN_L00_001f0d60");
extern int timer_done_0e00(int *) __asm__("FUN_001f9740");
extern int timer_tick_0e00(short *) __asm__("FUN_001f9770");
extern void add_0e00(void *, void *, void *) __asm__("FUN_001f9a10");
extern void lerp_0e00(void *, void *, void *, float) __asm__("FUN_L00_0025b6b8");
extern int rand_0e00(int) __asm__("FUN_00213260");
extern int frames_0e00(int) __asm__("FUN_001f96f8");
extern float itof_0e00(int) __asm__("FUN_001fa6c0");
extern char *spawn_0e00(void *, float, float, float, int, void *, float, int) __asm__("FUN_L00_0026d000");
extern int collide_0e00(void *, void *, int, void *, int) __asm__("FUN_001efa68");
extern void normalize_0e00(void *, void *, float) __asm__("FUN_001f9bf8");
extern void reflect_0e00(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern int sound_0e00(int, int, void *) __asm__("FUN_0022da68");
extern void scale_0e00(void *, void *, float) __asm__("FUN_001f9a68");
extern float length_0e00(void *) __asm__("FUN_001f9b20");
extern void cap_0e00(void *, void *, float) __asm__("FUN_001f9c48");
extern int ftoi_0e00(float) __asm__("FUN_001fa6d0");
extern void burst_0e00(void *, void *, void *, float, float, int, int, int, float, float, float, int, float, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void remove_0e00(void *) __asm__("FUN_0020c828");
extern float add_rot_0e00(float, float) __asm__("FUN_001fa580");

void FUN_L14_00300e00(struct Moby *spark_moby)
{
    enum {
        SPARK_COUNT_NEXT_BOUNCE = 0,
        SPARK_WAIT_FOR_DESCENT = 1,
        SPARK_ROLL_AND_FADE = 2,
        SPARK_BURST_AND_REMOVE = 3
    };
  Spark_0e00 *spark_data = (Spark_0e00 *) spark_moby->pvars;
  Vec4f next;
  Vec4f position_scratch;
  Vec4f from;
  float speed;
  float lo;
  float hi;
  if (((on_ground_0e00(&spark_moby->pos, 0, spark_moby, 0.65f) != 0) && (D_L14_00174558 != 0)) && (D_L14_00174558 != spark_data->owner))
  {
    spark_moby->state = 3;
  }
  if (timer_done_0e00(&spark_data->fade) != 0)
  {
    spark_moby->state = 3;
  }
  switch (spark_moby->state)
  {
    case SPARK_COUNT_NEXT_BOUNCE:

    case SPARK_WAIT_FOR_DESCENT:
    {
      add_0e00(&next, &spark_moby->pos, &spark_data->vel);
      spark_data->vel.z -= spark_data->vel.w;
      if ((spark_moby->state == 1) && (spark_data->vel.z < 0.0f))
      {
        spark_moby->state = 0;
      }
      timer_tick_0e00(&spark_data->trail);
      if (spark_data->trail != 0)
      {
        int i;
        for (i = 0; i < 3; i++)
        {
          float one = 1.0f;
          float third = 0.3333f;
          int spin;
          char *trail_particle;
          *((u128 *) (&from)) = *((u128 *) (&spark_moby->pos));
          lerp_0e00(&position_scratch, &from, &next, ((float) i) * third);
          spin = rand_0e00(6);
          if (rand_0e00(2) != 0)
          {
            spin = -spin;
          }
          trail_particle = spawn_0e00(&position_scratch, 0.025f, one, one, spin, D_L14_0015F580, ((((float) spark_data->trail) / itof_0e00(frames_0e00(D_L14_0016208C_0e00))) * 45000.0f) + 5000.0f, 0x80808080);
          if (trail_particle != 0)
          {
            char *trail_particle_subrecord = trail_particle + 0x20;
            *((short *) (trail_particle + 0xA)) = frames_0e00(0x78);
            *((int *) (trail_particle_subrecord + 4)) = 2;
            trail_particle_subrecord[0xA] = 0x7F;
            trail_particle_subrecord[0xB] = trail_particle[0xA];
          }
        }

      }
      if ((collide_0e00(&spark_moby->pos, &next, 0, spark_moby, 0) != 0) && ((D_L14_00174540.owner == 0) || (D_L14_00174540.owner != spark_data->owner)))
      {
        qcopy(&spark_moby->pos, &D_L14_00174540.hit);
      }
      else
      {
        qcopy(&spark_moby->pos, &next);
      }
      if (((on_ground_0e00(&spark_moby->pos, 0, spark_moby, D_L14_00162088) != 0) && ((spark_moby->state == 0) || ((spark_moby->pos.z - D_L14_00174540.hit.z) < 0.0f))) && ((D_L14_00174540.owner == 0) || (D_L14_00174540.owner != spark_data->owner)))
      {
        normalize_0e00(&position_scratch, &D_L14_00174540.normal, D_L14_00162088);
        add_0e00(&spark_moby->pos, &D_L14_00174540.hit, &position_scratch);
        reflect_0e00(&spark_data->vel, &spark_data->vel, &D_L14_00174540.normal);
        sound_0e00(1, 0, spark_moby);
        if (spark_moby->state == 0)
        {
          spark_moby->state = 1;
          if ((++spark_data->bounces) == 1)
          {
            cap_0e00(&spark_data->vel, &spark_data->vel, D_L14_001620A4 * frame_time);
            spark_data->vel.z = spark_data->vel.z * D_L14_001620A0;
          }
          else
          {
            scale_0e00(&from, &spark_data->vel, D_L14_001620A0);
            if (length_0e00(&from) < (D_L14_00162090 * frame_time))
            {
              cap_0e00(&spark_data->vel, &spark_data->vel, D_L14_00162090 * frame_time);
              spark_data->vel.z *= D_L14_001620A0;
            }
            else
            {
              qcopy(&spark_data->vel, &from);
            }
          }
        }
        if (spark_data->bounces >= 4)
        {
          float min;
          spark_moby->state = 2;
          spark_data->vel.z = 0.0f;
          spark_data->vel.w = length_0e00(&spark_data->vel);
          spark_data->vel.w *= D_L14_001620A0;
          min = D_L14_00162090 * frame_time;
          if (spark_data->vel.w < min)
          {
            spark_data->vel.w = min;
          }
          normalize_0e00(&spark_data->vel, &spark_data->vel, spark_data->vel.w);
          spark_data->fade_rate = 1.0f / itof_0e00(spark_data->fade);
        }
      }
      break;
    }

    case SPARK_ROLL_AND_FADE:
    {
      float fade_fraction;
      int r;
      int g;
      int b;
      float min;
      *((Rgb_0e00 *) (&next)) = D_L14_001620A8;
      fade_fraction = ((float) spark_data->fade) * spark_data->fade_rate;
      r = ftoi_0e00(((((Rgb_0e00 *) (&next))->r - 255.0f) * fade_fraction) + 255.0f);
      g = ftoi_0e00(((((Rgb_0e00 *) (&next))->g - 0.0f) * fade_fraction) + 0.0f);
      b = ftoi_0e00(((((Rgb_0e00 *) (&next))->b - 0.0f) * fade_fraction) + 0.0f);
      spark_moby->glow_color = 0xFF000000;
      spark_moby->glow_color = (((b << 16) | spark_moby->glow_color) | (g << 8)) | r;
      min = D_L14_00162090 * frame_time;
      spark_data->vel.w *= ((D_L14_00162094 - 1.0f) * frame_scale) + 1.0f;
      if (spark_data->vel.w < min)
      {
        spark_data->vel.w = min;
      }
      normalize_0e00(&spark_data->vel, &spark_data->vel, spark_data->vel.w);
      add_0e00(&spark_moby->pos, &spark_moby->pos, &spark_data->vel);
      break;
    }

    case SPARK_BURST_AND_REMOVE:
      burst_0e00(spark_moby, &spark_data->vel, &spark_moby->pos, 2.0f, 1.0f, 5, 2, 4, 2.0f, 1.0f, 9.0f, 0, 1.0f, 15.0f, 1, 1, -1, 0);
      remove_0e00(spark_moby);
      return;

  }

  if (spark_moby->state < 2)
  {
    speed = length_0e00(&spark_data->vel);
  }
  else
    if (spark_moby->state == 2)
  {
    speed = spark_data->vel.w;
  }
  else
  {
    return;
  }
  lo = frame_time * 0.2f;
  hi = frame_time * 6.0f;
  if (speed < lo)
  {
    speed = lo;
  }
  if (hi < speed)
  {
    speed = hi;
  }
  speed -= lo;
  spark_moby->rot.y = add_rot_0e00((((speed / (hi - lo)) * 7.0f) + 1.0f) * 0.017453292f, spark_moby->rot.y);
}
#include "sda.h"
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002FF358.c: func_L14_00302968), where it is exact; names translated to the US level program. */
extern char *FUN_0020c4f8_c(int) __asm__("FUN_0020c4f8");
extern float FUN_001f9e90_a(float, float) __asm__("FUN_001f9e90");
extern void FUN_L00_00250df8_f(char *) __asm__("FUN_L00_00250df8");
extern unsigned short D_L14_0016208C_g __asm__("D_L14_0016208C") __attribute__((sda));

extern float D_L14_00162098 __attribute__((sda));
char *FUN_L14_003014e0(int owner, float *pos, float *initial_velocity, int fade_timer, float gravity) {
    char *moby = FUN_0020c4f8_c(0x4A9);
    float initial_yaw, scaled_moby_value;
    unsigned short initial_trail_timer;
    if (moby != 0) {
        char *spark_data = *(char **)(moby + 0x78);
        ((unsigned char *)moby)[0x30] = 0xFF;
        *(short *)(moby + 0x32) = 0xFF;
        moby[0x31] = 1;
        moby[0x20] = 0;
        *(int *)(spark_data + 0x24) = owner;
        qcopy(moby + 0x10, pos);
        qcopy(spark_data + 0x10, initial_velocity);
        initial_trail_timer = D_L14_0016208C_g;
        *(float *)(spark_data + 0x1C) = gravity;
        *(int *)(spark_data + 0x20) = fade_timer;
        *(unsigned short *)(spark_data + 0x28) = initial_trail_timer;
        *(short *)(spark_data + 0x2A) = 0;
        *(int *)(moby + 0x40) = 0;
        *(int *)(moby + 0x44) = 0;
        initial_yaw = FUN_001f9e90_a(initial_velocity[0], initial_velocity[1]);
        scaled_moby_value = *(float *)(moby + 0x2C) * D_L14_00162098;
        *(float *)(moby + 0x48) = initial_yaw;
        *(float *)(moby + 0x2C) = scaled_moby_value;
        FUN_L00_00250df8_f(moby);
    }
    return moby;
}

typedef struct {
    u8 pad0[0x454];
    u8 collected[1];
} L14LevelState;

extern char *D_L14_0015FFD8;
extern L14LevelState D_L14_001BB930;
extern int D_L14_001BABD0[];
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern float convert_integer_to_float(int) __asm__("FUN_001fa6c0");
extern int fast_dec_timer(int *) __asm__("FUN_001f9740");
extern void FUN_L10_002f5a50(int);
extern void FUN_L14_00305600(struct Moby *);
extern void FUN_L14_00305680(struct Moby *);

extern s32 D_0014C190_257218[][64] __asm__("D_0014C190");
/* Moby that unlocks once: skips straight to done if this level already recorded it, otherwise waits for its trigger moby to reach state 2, records the unlock in the level's bit sets and runs a countdown. */
void FUN_L14_00305408(unsigned char *moby) {
    enum {
        UNLOCK_SEQUENCE_INITIALIZE = 0,
        UNLOCK_SEQUENCE_WAIT_FOR_TRIGGER = 1,
        UNLOCK_SEQUENCE_COUNTDOWN = 2
    };
    int *unlock_sequence_data = *(int **)(moby + 0x78);
    short id;

    switch (moby[0x20]) {
    case UNLOCK_SEQUENCE_INITIALIZE:
        if (unlock_sequence_data[3] < 0 || unlock_sequence_data[0] < 0 || unlock_sequence_data[4] < 0) {
            mark_moby_for_removal(moby);
            return;
        }
        id = *(short *)(moby + 0xB2);
        if (D_L14_001BB930.collected[id] != 0) {
            moby[0x20] = 3;
            return;
        }
        if (D_0014C190_257218[current_level_index][id >> 5] >> (*(unsigned short *)(moby + 0xB2) & 0x1F) & 1) {
            moby[0x20] = 3;
            return;
        }
        moby[0x20] = 1;
        FUN_L14_00305600((char *)moby);
        break;
    case UNLOCK_SEQUENCE_WAIT_FOR_TRIGGER:
        if (((unsigned char *)D_L14_0015FFD8)[unlock_sequence_data[3] * 0x100 + 0x20] != 2) {
            return;
        }
        D_0014C190_257218[current_level_index][*(short *)(moby + 0xB2) >> 5] |= 1 << (*(unsigned short *)(moby + 0xB2) & 0x1F);
        D_L14_001BABD0[*(short *)(moby + 0xB2) >> 5] |= 1 << (*(unsigned short *)(moby + 0xB2) & 0x1F);
        moby[0x20] = 2;
        unlock_sequence_data[1] = scale_game_frames(0x78);
        ((float *)unlock_sequence_data)[2] = 1.0f / convert_integer_to_float(unlock_sequence_data[1]);
        FUN_L10_002f5a50(unlock_sequence_data[4]);
        break;
    case UNLOCK_SEQUENCE_COUNTDOWN:
        fast_dec_timer(&unlock_sequence_data[1]);
        FUN_L14_00305680((char *)moby);
        if (unlock_sequence_data[1] == 0) {
            moby[0x20] = 3;
        }
        break;
    }
}
/* For each entry in a moby-id list, saves the moby's Y and lowers it by 20. */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002FF358.c: func_L14_00306A88), where it is exact; names translated to the US level program. */

extern char *D_L14_0015FFD8;
extern float D_L14_001EE5E0[];
extern unsigned short *D_L14_001ABF40[];

void FUN_L14_00305600(struct Moby *moby) {
    unsigned short *moby_id_cursor = D_L14_001ABF40[*(int *)moby->pvars];
    float *out;
    if (moby_id_cursor != 0) {
        int base = D_L14_0015FFD8;
        out = D_L14_001EE5E0;
        do {
            float *target_moby = (float *)(base + ((*moby_id_cursor & 0x7FFF) << 8));
            *out++ = target_moby[6];
            target_moby[6] -= 20.0f;
            *(unsigned short *)((char *)target_moby + 0x34) |= 0x41;
        } while (*(short *)moby_id_cursor++ >= 0);
    }
}
extern float convert_integer_to_float(int) __asm__("FUN_001fa6c0");
extern float cubic_hermite_scalar(float, float, float, float, float) __asm__("FUN_L00_00257e20");

/* Per-frame update: slides each listed part toward a rest height by a factor from the moby's animation value. */
void FUN_L14_00305680(struct Moby *moby) {
    int *unlock_sequence_data = (int *)moby->pvars;
    short *moby_id_cursor = (short *)D_L14_001ABF40[unlock_sequence_data[0]];
    float height_blend_factor;
    float *saved_height_cursor;
    char *base;
    float x;
    if (moby_id_cursor == 0) {
        return;
    }
    x = convert_integer_to_float(unlock_sequence_data[1]) * ((float *)unlock_sequence_data)[2];
    height_blend_factor = cubic_hermite_scalar(-1.0f, 0.0f, 1.0f, 0.0f, 1.0f - x);
    base = D_L14_0015FFD8;
    saved_height_cursor = D_L14_001EE5E0;
    do {
        char *target_moby = base + ((*(unsigned short *)moby_id_cursor & 0x7FFF) << 8);
        float saved_height = *saved_height_cursor++;
        float lo = saved_height - 20.0f;
        *(unsigned short *)(target_moby + 0x34) &= 0xFFBE;
        *(float *)(target_moby + 0x18) = lo + (saved_height - lo) * height_blend_factor;
    } while (*moby_id_cursor++ >= 0);
}
extern u8 D_0014C050[];
extern u8 D_0013D4D5 __attribute__((section(".data")));
extern s32 D_L14_0015F5CC;
extern s32 D_L14_0015F5C4;
extern f32 absolute_float(f32) __asm__("FUN_001f99c0");
extern void FUN_L14_00305b18(struct Moby *);
extern f32 D_L14_001621E8 __attribute__((sda));
extern f32 D_L14_001621EC __attribute__((sda));
extern f32 D_L14_001621F0 __attribute__((sda));
extern f32 probe_ground_height(void *, s32, f32) __asm__("FUN_00213508");
extern f32 fast_add_rotations(f32, f32) __asm__("FUN_001fa580");
extern void scale_vector_xyz(void *, void *, f32) __asm__("FUN_001f9a68");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern f32 vector_distance_xy(void *, void *) __asm__("FUN_001f9b80");
extern void FUN_L00_00260860(s32, s32);
extern void start_scene(s32) __asm__("FUN_L00_00298840");
extern void jet_clear_persistent(s32) __asm__("FUN_L00_002502a0");
extern int memcard_save_data(s32, s32) __asm__("FUN_0020b178");
extern void queue_dialogue_message(s32, s32) __asm__("FUN_L00_00263d40");
extern void help_request(s32, s32) __asm__("FUN_L00_00203908");
extern f32 approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");

void morphoray_update(struct Moby *moby) __asm__("FUN_L14_00305758");
void morphoray_update(struct Moby *moby) {
    enum {
        MORPHORAY_INITIALIZE = 0,
        MORPHORAY_WAIT_FOR_HERO = 1,
        MORPHORAY_WAIT_TO_REMOVE = 2,
        MORPHORAY_APPROACH_ZERO_AND_REMOVE = 3
    };
    u8 *moby_bytes = (u8 *)moby;
    u8 *morphoray_pvars = (u8 *)moby->pvars;
    f32 position_offset[4];
    u8 state = moby->state;

    switch (state) {
    case MORPHORAY_INITIALIZE:
        if (D_0014C050[moby->mission + current_level_index * 16] == 0xFF) {
            mark_moby_for_removal(moby);
            return;
        }
        FUN_L14_00305a38((void *)moby);
        probe_ground_height(moby_bytes + 0x10, 0, 0.5f);
        moby->rot.x = 0.0f;
        moby->rot.y = -1.5707964f;
        moby->pos.z = D_L14_00174568[0] + D_L14_001621E8;
        qcopy(morphoray_pvars, moby_bytes + 0x10);
        moby->state = 1;
        break;
    case MORPHORAY_WAIT_FOR_HERO:
        *(f32 *)(moby_bytes + 0x48) = fast_add_rotations(*(f32 *)(moby_bytes + 0x48), D_L14_001621EC);
        qcopy(moby_bytes + 0x10, morphoray_pvars);
        scale_vector_xyz(position_offset, moby_bytes + 0xd0, D_L14_001621F0);
        add_vector_xyz(moby_bytes + 0x10, moby_bytes + 0x10, position_offset);
        FUN_L14_00305b18(moby);
        if (D_L14_0015F5CC % 10 != 0)
            return;
        if (!(vector_distance_xy(moby_bytes + 0x10, hero.motion.pos.f) < 1.0f))
            return;
        if (!(absolute_float(*(f32 *)(moby_bytes + 0x18) - hero.motion.pos.f[2]) < 2.0f))
            return;
        if (D_0013D4D5 != 0)
            *(s32 *)(morphoray_pvars + 0x18) = state;
        else
            *(s32 *)(morphoray_pvars + 0x18) = 0;
        FUN_L00_00260860(0x15, 1);
        start_scene(6);
        moby->state = 2;
        break;
    case MORPHORAY_WAIT_TO_REMOVE:
        if (D_L14_0015F5C4 != state) {
            if (D_0014C050[moby->mission + current_level_index * 16] != 0xFF) {
                jet_clear_persistent(moby->mission);
                memcard_save_data(0, -1);
            }
            if (*(s32 *)(morphoray_pvars + 0x18) != 0)
                queue_dialogue_message(0x53e0, -1);
            else
                queue_dialogue_message(0x36b2, -1);
            help_request(0x36b0, 0x7a);
            mark_moby_for_removal(moby);
            return;
        }
        *(u16 *)(moby_bytes + 0x34) |= 0x41;
        break;
    case MORPHORAY_APPROACH_ZERO_AND_REMOVE:
        approach_value((f32 *)(moby_bytes + 0x2c), 0.0f, *(f32 *)(*(u8 **)(moby_bytes + 0x24) + 0x24) * 0.02f);
        if (*(f32 *)(moby_bytes + 0x2c) == 0.0f)
            mark_moby_for_removal(moby);
        break;
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Updates and draws four scrolling particle streaks for an Oltanis moby. */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002FF358.c: func_L14_00306FA0), where it is exact; names translated to the US level program. */

typedef struct {
    float pad[2];
    float z;
    float pad2[5];
    float a[4];
    float b[4];
    int t[4];
    float c[4];
} FxData14;

extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern unsigned char *FUN_L00_00272f68(float f, void *pos, int a1, unsigned char a2, int idx,
                                       int flag, int s, int mode);
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void normalize_vector_xyz(void *out, void *a, f32 len) __asm__("FUN_001f9bf8");
extern void subtract_vector_xyz(void *out, void *a, void *b) __asm__("FUN_001f9a28");
extern void tick_countdown_32(void *) __asm__("func_001F9740");
float absolute_float(float input) __asm__("func_001F99C0");
extern int func_001F9908_r(int *arg0) __asm__("FUN_001f9740");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

void FUN_L14_00305b18(struct Moby *morphoray_moby) {
    float particle_position_and_step[8];
    FxData14 *effect_data = (FxData14 *)morphoray_moby->pvars;
    float *a;
    int *t;
    int i, j, c, k;
    float unwrapped_particle_value, elapsed_timer_fraction;
    j = 0;
    effect_data->z += 0.1f;
    subtract_vector_xyz(particle_position_and_step, D_L14_001674C0, effect_data);
    t = effect_data->t;
    a = effect_data->a;
    normalize_vector_xyz(particle_position_and_step, particle_position_and_step, -0.3f);
    normalize_vector_xyz(&particle_position_and_step[4], particle_position_and_step, 0.1f);
    add_vector_xyz(particle_position_and_step, particle_position_and_step, effect_data);
    effect_data->z -= 0.1f;
    for (i = 3; i >= 0; i--) {
        unwrapped_particle_value = *a + effect_data->b[j];
        *a = unwrapped_particle_value;
        if (unwrapped_particle_value >= 255.0f) {
            *a = unwrapped_particle_value - 255.0f;
        } else if (unwrapped_particle_value <= 0.0f) {
            *a = unwrapped_particle_value + 255.0f;
        }
        if (func_001F9908_r(t)) {
            effect_data->t[j] = scale_game_frames(255);
        }
        t++;
        elapsed_timer_fraction = convert_integer_to_float(scale_game_frames(255) - effect_data->t[j]);
        elapsed_timer_fraction = elapsed_timer_fraction / (float)scale_game_frames(255);
        c = fast_tween_color(0x4040FFFF, 0x1040FFFF, absolute_float(0.5f - elapsed_timer_fraction));
        k = func_001FA898_r(*a++);
        FUN_L00_00272f68(effect_data->c[j], particle_position_and_step, c, k, 0x35, 1, 2, 0);
        j++;
        add_vector_xyz(particle_position_and_step, particle_position_and_step, &particle_position_and_step[4]);
    }
}
/* Update function
 * for moby class 1395 on level 14: fills the six-slot table of helper mobys, then lowers them one by one. */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002FF358.c: func_L14_003071B0), where it is exact; names translated to the US level program. */

typedef struct Moby_3071B0 {
    char pad0[0x10];
    float pos[4];                 /* 0x10 */
    unsigned char state;          /* 0x20 */
    char pad21[0x23];
    float f44;                    /* 0x44 */
    char pad48[0x30];
    struct Data_3071B0 *data;     /* 0x78 */
    char pad7C[0x44];
    float rot[4];                 /* 0xC0 */
    char padD0[0x10];
    float fE0[4];                 /* 0xE0 */
    char padF0[0x10];
} Moby_3071B0;

typedef struct Slots_3071B0 {
    Moby_3071B0 *m[6];
} Slots_3071B0;

typedef struct Data_3071B0 {
    Moby_3071B0 *owner;           /* 0x00 */
    Slots_3071B0 slots;           /* 0x04 */
    int trigger;                  /* 0x1C: index of the moby that starts it */
    short timer;                  /* 0x20 */
    short cur;                    /* 0x22 */
    float rate;                   /* 0x24 */
    int idx[6];                   /* 0x28 */
    int voice;                    /* 0x40 */
} Data_3071B0;

typedef struct Snd_3071B0 {
    char pad0[0x74];
    unsigned char f74;
    char pad75[0x13];
    Moby_3071B0 *f88;
} Snd_3071B0;

extern float FUN_L00_0025b750(float, float, float);
extern int D_L14_0015FFD8_c __asm__("D_L14_0015FFD8");
extern float D_L14_001621F4 __attribute__((sda)); /* no foreign declaration */
extern float D_L14_001621F8 __attribute__((sda)); /* no foreign declaration */
extern float D_L14_001621FC __attribute__((sda)); /* no foreign declaration */
extern float D_L14_00162200 __attribute__((sda)); /* no foreign declaration */
extern int D_L14_00162204 __attribute__((sda)); /* no foreign declaration */
extern int D_L14_0016220C __attribute__((sda)); /* no foreign declaration */
extern int fast_dec_timer_s16() __asm__("FUN_001f9770");
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern s32 allocate_voice_for_target_entry() __asm__("func_0022DA68");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
extern void scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");
f32 FUN_L00_0025b6a8(f32 a, f32 b, f32 t);
int voice_is_playing(int a, int i) __asm__("FUN_L00_0028d8c0");
int trooper_play_sound(int i, int a1, int v, int k) __asm__("FUN_L00_0028dc90");
u8 *FUN_L14_00306158_c(u8 *self) __asm__("FUN_L14_00306158");
extern Moby_3071B0 *func_L14_003075E0_3071B0(Moby_3071B0 *owner) __asm__("FUN_L14_00306158");
extern void func_001F9C30_3071B0(void *, void *, float) __asm__("FUN_001f9a68");
extern void func_001F9BD8_3071B0(void *, void *, void *) __asm__("FUN_001f9a10");
extern int func_001F9850_3071B0(int) __asm__("FUN_001f96f8");
extern float func_001FA888_3071B0(int) __asm__("FUN_001fa6c0");
extern int func_L00_0028EF68_3071B0(int i, int a1, Moby_3071B0 *v, int k) __asm__("FUN_L00_0028dc90");
extern int func_001F9938_3071B0(void *) __asm__("FUN_001f9770");
extern float func_L00_0025C7A8_3071B0(float, float, float) __asm__("FUN_L00_0025b750");
extern int func_L00_0028EB98_3071B0(void *, int) __asm__("FUN_L00_0028d8c0");
extern int func_0022ED80_3071B0(int, int, Moby_3071B0 *) __asm__("FUN_0022da68");
extern float func_L00_0025C700_3071B0(float, float, float) __asm__("FUN_L00_0025b6a8");
extern void func_L00_0028EBF0_3071B0(int) __asm__("FUN_0022d798");
extern char D_0013E633_3071B0[] __asm__("D_0013E550");

void FUN_L14_00305d28(Moby_3071B0 *moby) {
    enum {
        HELPER_SEQUENCE_INITIALIZE = 0,
        HELPER_SEQUENCE_WAIT_FOR_TRIGGER = 1,
        HELPER_SEQUENCE_ANIMATE_SLOTS = 2,
        HELPER_SEQUENCE_ANIMATE_OWNER = 3
    };
    Data_3071B0 *data = moby->data;
    float position_offset[4];

    switch (moby->state) {
    case HELPER_SEQUENCE_INITIALIZE: {
        int i;
        if (data->owner == 0) {
            data->owner = func_L14_003075E0_3071B0(moby);
            func_001F9C30_3071B0(position_offset, moby->rot, D_L14_001621F4);
            func_001F9BD8_3071B0(data->owner->pos, moby->pos, position_offset);
        }
        for (i = 0; i < 6; i++) {
            if (data->slots.m[i] == 0) {
                data->slots.m[i] = (Moby_3071B0 *)(D_L14_0015FFD8_c + (data->idx[i] << 8));
                func_001F9C30_3071B0(position_offset, moby->rot, D_L14_001621F4);
                func_001F9BD8_3071B0(data->slots.m[i]->pos, moby->pos, position_offset);
                data->slots.m[i]->pos[2] = data->slots.m[i]->pos[2] + (D_L14_001621F8 * (float)i + D_L14_001621FC);
            }
        }
        data->voice = -1;
        if (data->owner != 0) {
            moby->state = 1;
        }
        break;
    }
    case HELPER_SEQUENCE_WAIT_FOR_TRIGGER:
        if (((Moby_3071B0 *)(D_L14_0015FFD8_c + (data->trigger << 8)))->state == 2) {
            moby->state = 2;
            data->cur = 5;
            data->timer = func_001F9850_3071B0(D_L14_00162204);
            data->rate = 1.0f / func_001FA888_3071B0(data->timer);
            func_L00_0028EF68_3071B0(0, 0, data->slots.m[data->cur], 0x575);
        }
        break;
    case HELPER_SEQUENCE_ANIMATE_SLOTS: {
        Slots_3071B0 *slots = &data->slots;
        Moby_3071B0 *current_slot_moby = slots->m[data->cur];
        func_001F9938_3071B0(&data->timer);
        current_slot_moby->f44 = func_L00_0025C7A8_3071B0(1.5707964f, 0.0f, func_001FA888_3071B0(data->timer) * data->rate);
        if (data->timer == 0) {
            data->timer = func_001F9850_3071B0(D_L14_00162204);
            data->cur--;
            if (data->cur < 0) {
                moby->state = 3;
                data->timer = func_001F9850_3071B0(D_L14_0016220C);
                data->rate = 1.0f / func_001FA888_3071B0(data->timer);
            } else {
                func_L00_0028EF68_3071B0(0, 0, slots->m[data->cur], 0x575);
            }
        }
        break;
    }
    case HELPER_SEQUENCE_ANIMATE_OWNER: {
        Moby_3071B0 *owner = data->owner;
        Moby_3071B0 **slots;
        int i;
        float remaining_timer_fraction;
        if (!func_L00_0028EB98_3071B0(moby, data->voice)) {
            data->voice = func_0022ED80_3071B0(0, 4, moby);
        }
        func_001F9938_3071B0(&data->timer);
        slots = data->slots.m;
        remaining_timer_fraction = func_001FA888_3071B0(data->timer) * data->rate;
        owner->f44 = func_L00_0025C700_3071B0(D_L14_00162200 * 0.017453292f, 0.0f, remaining_timer_fraction);
        for (i = 0; i < 6; i++) {
            func_001F9C30_3071B0(position_offset, owner->fE0, D_L14_001621F8 * (float)i + D_L14_001621FC);
            func_001F9BD8_3071B0(slots[i]->pos, owner->pos, position_offset);
        }
        if (data->timer == 0) {
            int j;
            if (data->voice != -1) {
                Snd_3071B0 *voice_entry = (Snd_3071B0 *)(D_0013E633_3071B0 + data->voice * 0x70);
                if (voice_entry->f88 == moby && voice_entry->f74) {
                    func_L00_0028EBF0_3071B0(data->voice);
                }
            }
            data->voice = -1;
            moby->state = 4;
            for (j = 0; j < 6; j++) {
                slots[j]->state = 2;
            }
        }
        break;
    }
    }
}
void FUN_L14_003061d8(void *updated_moby) {
    enum {
        MOBY_PVARS_INITIALIZE = 0
    };
    char *moby = updated_moby;
    char *moby_pvars;
    switch ((unsigned char)moby[0x20]) {
    case MOBY_PVARS_INITIALIZE:
        moby_pvars = *(char **)(moby + 0x78);
        moby_pvars[0x28] = 4;
        *(int *)(moby_pvars + 0x20) = 0;
        *(short *)(moby_pvars + 0x24) = 0;
        *(short *)(moby_pvars + 0x3E) = 0;
        moby[0x20] = 1;
        break;
    case 2:
        *(short *)(*(char **)(moby + 0x78) + 0x3E) = 5;
        moby[0x20] = 3;
        break;
    }
}
extern char *D_L14_001B0BB0[];
extern u8 D_0014C050[];
extern f32 random_angle_radians(void) __asm__("FUN_00213308");
extern f32 fast_add_rotations(f32, f32) __asm__("FUN_001fa580");
extern f32 fast_sin(f32) __asm__("FUN_001f9de0");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern s32 allocate_voice_for_target_entry(s32, s32, struct Moby *) __asm__("FUN_0022da68");

void button_update(struct Moby *moby) __asm__("FUN_L14_00306b78");
void button_update(struct Moby *moby) {
    enum {
        BUTTON_INITIALIZE = 0,
        BUTTON_WAIT_FOR_HERO_TO_STAND = 1
    };
    s32 *button_pvars = (s32 *)moby->pvars;
    s16 id;
    s32 i;
    s32 list;
    char *path_entry_base;
    f32 angle;
    s32 color;
    u32 shade;
    u32 high;

    switch (moby->state) {
    case BUTTON_INITIALIZE:
        *(f32 *)(button_pvars + 3) = random_angle_radians();
        moby->pos.z -= 0.35f;
        id = (s16)moby->save_id;
        if (D_L14_001BB930.collected[id] != 0 ||
            (D_0014C190_257218[current_level_index][id >> 5] >> (moby->save_id & 31) & 1) != 0 ||
            (button_pvars[2] != 0 && D_0014C050[moby->mission + current_level_index * 16] == 0xFF)) {
            moby->command = 2;
            moby->glow_color = 0x80208020;
            moby->state = 2;
            list = button_pvars[0];
            if (list != -1) {
                i = 0;
                if (*(s32 *)D_L14_001B0BB0[list] > 0) {
                    do {
                        path_entry_base = D_L14_001B0BB0[list] + i * 16;
                        if (*(f32 *)(path_entry_base + 0x1C) == ((f32 *)button_pvars)[1])
                            *(s32 *)(path_entry_base + 0x1C) = 0;
                        list = button_pvars[0];
                        i++;
                    } while (i < *(s32 *)D_L14_001B0BB0[list]);
                }
            }
        } else {
            moby->state = 1;
        }
        break;
    case BUTTON_WAIT_FOR_HERO_TO_STAND:
        angle = fast_add_rotations(*(f32 *)(button_pvars + 3), frame_time * 6.2831855f);
        *(f32 *)(button_pvars + 3) = angle;
        color = truncate_float_to_s32((fast_sin(angle) * 4.0f - 3.0f) * 128.0f);
        if (color > 0x80) color = 0x80;
        else if (color < 0x20) color = 0x20;
        shade = 0x80000000 | (color << 8);
        high = color << 16;
        moby->glow_color = high | shade | color;
        if (hero.ground_moby == moby && hero.air_frames.s == 0) {
            id = (s16)moby->save_id;
            D_0014C190_257218[current_level_index][id >> 5] |= 1 << (moby->save_id & 31);
            D_L14_001BABD0[(s16)moby->save_id >> 5] |= 1 << (moby->save_id & 31);
            moby->state = 2;
            moby->command = 1;
            moby->glow_color = 0x80208020;
            allocate_voice_for_target_entry(0, 0, moby);
            list = button_pvars[0];
            if (list != -1) {
                i = 0;
                if (*(s32 *)D_L14_001B0BB0[list] > 0) {
                    do {
                        path_entry_base = D_L14_001B0BB0[list] + i * 16;
                        if (*(f32 *)(path_entry_base + 0x1C) == ((f32 *)button_pvars)[1])
                            *(s32 *)(path_entry_base + 0x1C) = 0;
                        list = button_pvars[0];
                        i++;
                    } while (i < *(s32 *)D_L14_001B0BB0[list]);
                }
            }
        }
        break;
    }
}
/* pvars of the Blarg heavy interceptor (class 1417) */
typedef struct {
    char pad0[2];
    unsigned char b2;
    char pad3[0x1D];
    float f20;
    float f24;
    float f28;
    char pad2C[0x14];
} Fx_6ee0;
typedef struct {
    char pad0[0x60];
    unsigned char effect[0x10];  /* 0x60 */
    int f70;
    int f74;
    int path;                    /* 0x78 */
    int sound;                   /* 0x7C */
    float f80;
    float f84;
    float f88;
    short timer;                 /* 0x8C */
    char pad8E[2];
    float f90;
    float f94;
    float f98;
    char pad9C[4];
    int part;                    /* 0xA0 */
    float fA4;
    float fA8;
    float fAC;
    int fB0;
    int fB4;
    int fB8;
    int fBC;
    int fC0;
    int trigger;                 /* 0xC4 */
    int fC8;
    int fCC;
    Fx_6ee0 fx;                  /* 0xD0 */
} Data_6ee0;
typedef struct { char pad0[0x20]; float f20; float f24; float f28; float f2C; } Part_6ee0;
typedef struct { char pad0[0x74]; unsigned char b74; char pad75[0x13]; struct Moby *f88; } Snd_6ee0;
typedef struct { char pad0[0x10]; Vec4f a; Vec4f b; } Path_6ee0;

extern char *FUN_L00_0025a420_6ee0(void *, int, int) __asm__("FUN_L00_0025a420");
extern void FUN_L00_0025f090_6ee0(void *, void *, float, float, int) __asm__("FUN_L00_0025f090");
extern void sound_release_6ee0(int) __asm__("FUN_0022d798");
extern int FUN_001e93b0_6ee0() __asm__("FUN_001e93b0");
extern void FUN_0020c828_6ee0(void *) __asm__("FUN_0020c828");
extern void FUN_L14_00307510_6ee0(void *) __asm__("FUN_L14_00307510");
extern int scale_ticks_6ee0(int) __asm__("FUN_001f96f8");
extern void fill_6ee0(void *, int, int) __asm__("FUN_001f97e8");
extern void FUN_0020cb10_6ee0(void *, int, void *) __asm__("FUN_0020cb10");
extern void FUN_L00_0025d458_6ee0(void *, void *) __asm__("FUN_L00_0025d458");
extern int inside_clip_6ee0(void *, int) __asm__("FUN_00214720");
extern void FUN_L00_00262b80_6ee0(void *, void *, void *, float, float, float) __asm__("FUN_L00_00262b80");
extern int FUN_L14_00307620_6ee0(void *) __asm__("FUN_L14_00307620");
extern void scale_vec_6ee0(void *, void *, float) __asm__("FUN_001f9a68");
extern void add_vec_6ee0(void *, void *, void *) __asm__("FUN_001f9a10");
extern int tick_6ee0(void *) __asm__("FUN_001f9740");
extern int tick_done_6ee0(void *) __asm__("FUN_001f9770");
extern void FUN_L00_002502f0_6ee0(void *, int, int, int) __asm__("FUN_L00_002502f0");
extern float atan2_6ee0(float, float) __asm__("FUN_001f9e90");
extern int FUN_L00_0028d8c0_6ee0(void *, int) __asm__("FUN_L00_0028d8c0");
extern int play_sound_6ee0(int, int, void *) __asm__("FUN_0022da68");
extern void FUN_L14_003135b8_6ee0(void *) __asm__("FUN_L14_003135b8");
extern Path_6ee0 *D_L14_001B0BB0_6ee0[] __asm__("D_L14_001B0BB0");
char D_L14_001FD118[] = "Flying Car (moby1417) with no flyPath. UID: %d - RAR\n";
char D_L14_001FD150[] = "Flying Car (moby1417) with a flyPath that has no points. UID: %d - RAR\n";
extern int D_L14_0016227C __attribute__((sda));
extern float D_L14_00162290 __attribute__((sda));
extern float D_L14_00162294 __attribute__((sda));
extern float D_L14_00162298 __attribute__((sda));
extern char *D_L14_0015FFD8_6ee0 __asm__("D_L14_0015FFD8") __attribute__((section(".sdata")));
extern char D_0013E550_6ee0[] __asm__("D_0013E550");

extern f32 frame_time_macro __asm__("D_0015ED6C") __attribute__((section(".sdata")));
/* Ported from rac1-decomp src/overlays/l14_oltanis/vendor_002FF358.c (func_L14_00308368) */
void FUN_L14_00306ee0(struct Moby *m);
void FUN_L14_00306ee0(struct Moby *flying_car_moby) {
    enum {
        FLYING_CAR_INITIALIZE = 0,
        FLYING_CAR_WAIT_FOR_TRIGGER = 1,
        FLYING_CAR_FOLLOW_PATH = 2,
        FLYING_CAR_WAIT_FOR_PATH_RESET = 3
    };
    Data_6ee0 *flying_car_data = (Data_6ee0 *)flying_car_moby->pvars;
    Vec4f vec __attribute__((aligned(16)));
    char *hit;

    hit = FUN_L00_0025a420_6ee0(flying_car_moby, 0x800000, 0);
    flying_car_moby->hit_slot = 0xFF;
    if (hit && flying_car_moby->state != 3) {
        flying_car_moby->state = 3;
        FUN_L00_0025f090_6ee0(flying_car_moby, &flying_car_moby->pos, 5.0f, 13.0f, -1);
        flying_car_moby->flags |= 0x41;
        flying_car_moby->collision = 0;
        if (flying_car_data->part >= 0) {
            struct Moby *o = (struct Moby *)(D_L14_0015FFD8_6ee0 + (flying_car_data->part << 8));
            o->pos.x = o->pos.y = o->pos.z = 5.0f;
            o->flags |= 0x41;
        }
        if (flying_car_data->sound != -1) {
            Snd_6ee0 *voice_entry = (Snd_6ee0 *)(D_0013E550_6ee0 + flying_car_data->sound * 0x70);
            if (voice_entry->f88 == flying_car_moby && voice_entry->b74 != 0) {
                sound_release_6ee0(flying_car_data->sound);
            }
        }
        flying_car_data->sound = -1;
    }
    switch (flying_car_moby->state) {
    case FLYING_CAR_INITIALIZE:
        if (flying_car_data->path == -1) {
            FUN_001e93b0_6ee0(D_L14_001FD118, (s16)flying_car_moby->save_id);
            FUN_0020c828_6ee0(flying_car_moby);
            return;
        }
        if (*(int *)D_L14_001B0BB0_6ee0[flying_car_data->path] == 0) {
            FUN_001e93b0_6ee0(D_L14_001FD150, (s16)flying_car_moby->save_id);
            FUN_0020c828_6ee0(flying_car_moby);
            return;
        }
        FUN_L14_00307510_6ee0(flying_car_moby);
        flying_car_moby->state = 2;
        flying_car_moby->update_dist = 0xFF;
        flying_car_moby->draw_dist = 0xFF;
        flying_car_data->sound = -1;
        flying_car_data->timer = scale_ticks_6ee0(D_L14_0016227C);
        flying_car_data->fB0 = 0;
        flying_car_data->fB4 = 0;
        flying_car_data->fBC = 0;
        flying_car_data->fC0 = 0;
        flying_car_data->fCC = 0;
        if (flying_car_data->part >= 0) {
            struct Moby *o = (struct Moby *)(D_L14_0015FFD8_6ee0 + (flying_car_data->part << 8));
            o->draw_dist = (u16)flying_car_moby->draw_dist;
            o->update_dist = flying_car_moby->update_dist;
        }
        fill_6ee0(&flying_car_data->fx, 0, 0x40);
        FUN_0020cb10_6ee0(flying_car_moby, 0, &flying_car_data->fx);
        FUN_L00_0025d458_6ee0(flying_car_moby, flying_car_data->effect);
        if (flying_car_data->trigger >= 0) {
            flying_car_moby->flags |= 0x41;
            flying_car_moby->collision = 0;
            if (flying_car_data->part >= 0) {
                struct Moby *o = (struct Moby *)(D_L14_0015FFD8_6ee0 + (flying_car_data->part << 8));
                o->flags |= 0x41;
            }
            flying_car_moby->state = 1;
        }
        break;
    case FLYING_CAR_WAIT_FOR_TRIGGER:
        if (inside_clip_6ee0(&hero.motion.pos, flying_car_data->trigger)) {
            flying_car_moby->flags &= 0xFFBE;
            flying_car_moby->collision = flying_car_moby->pclass->collision;
            if (flying_car_data->part >= 0) {
                struct Moby *o = (struct Moby *)(D_L14_0015FFD8_6ee0 + (flying_car_data->part << 8));
                o->flags &= 0xFFBE;
            }
            flying_car_moby->state = 2;
        }
        break;
    case FLYING_CAR_FOLLOW_PATH:
        flying_car_data->fx.b2 = 1;
        flying_car_data->fx.f20 = flying_car_data->fx.f24 = flying_car_data->fx.f28 = 0.0001f;
        FUN_L00_00262b80_6ee0(flying_car_moby, &flying_car_data->fBC, &flying_car_data->fC0, D_L14_00162290, D_L14_00162294, D_L14_00162298);
        if (FUN_L14_00307620_6ee0(flying_car_moby)) {
            if (flying_car_data->part >= 0) {
                struct Moby *o = (struct Moby *)(D_L14_0015FFD8_6ee0 + (flying_car_data->part << 8));
                scale_vec_6ee0(&vec, &flying_car_moby->rot_row0, flying_car_data->fA4);
                add_vec_6ee0(&o->pos, &flying_car_moby->pos, &vec);
                scale_vec_6ee0(&vec, &flying_car_moby->rot_row1, flying_car_data->fA8);
                add_vec_6ee0(&o->pos, &o->pos, &vec);
                scale_vec_6ee0(&vec, &flying_car_moby->rot_row2, flying_car_data->fAC);
                add_vec_6ee0(&o->pos, &o->pos, &vec);
                if ((hero.state.control_mode == 13 && hero.unk964 == o)
                    || (hero.state.control_mode == 14 && hero.unk994 == o)) {
                    FUN_L14_003135b8_6ee0(flying_car_moby);
                    flying_car_data->fCC = scale_ticks_6ee0(0x1E);
                } else {
                    tick_6ee0(&flying_car_data->fCC);
                    if (flying_car_data->fCC != 0) {
                        ((Part_6ee0 *)o->pvars)->f20 = 2.0f;
                    } else {
                        Part_6ee0 *pd = (Part_6ee0 *)o->pvars;
                        pd->f20 = 0.0f;
                        if (o->oclass == 0x323) {
                            pd->f20 = 40.0f;
                            pd->f24 = 0.01f;
                            pd->f28 = 0.2f;
                            pd->f2C = frame_time_macro * 24.0f;
                        }
                    }
                }
            }
        }
        break;
    case FLYING_CAR_WAIT_FOR_PATH_RESET:
        if (tick_done_6ee0(&flying_car_data->timer)) {
            Path_6ee0 *path = D_L14_001B0BB0_6ee0[flying_car_data->path];
            FUN_L00_002502f0_6ee0(flying_car_moby, flying_car_data->effect[4], flying_car_data->effect[5], flying_car_data->effect[6]);
            flying_car_moby->flags &= 0xFFBE;
            flying_car_moby->collision = flying_car_moby->pclass->collision;
            flying_car_data->timer = scale_ticks_6ee0(D_L14_0016227C);
            flying_car_moby->state = 2;
            flying_car_data->f70 = 0;
            flying_car_data->f74 = 0;
            qcopy(&flying_car_moby->pos, &path->a);
            flying_car_moby->rot.z = atan2_6ee0(path->b.x - path->a.x, path->b.y - path->a.y);
            flying_car_data->f80 = flying_car_data->f84 = flying_car_data->f88 = 0.0f;
            flying_car_data->f90 = flying_car_data->f94 = flying_car_data->f98 = 0.0f;
            if (flying_car_data->part >= 0) {
                struct Moby *o = (struct Moby *)(D_L14_0015FFD8_6ee0 + (flying_car_data->part << 8));
                o->flags &= 0xFFBE;
                scale_vec_6ee0(&vec, &flying_car_moby->rot_row0, flying_car_data->fA4);
                add_vec_6ee0(&o->pos, &flying_car_moby->pos, &vec);
                scale_vec_6ee0(&vec, &flying_car_moby->rot_row1, flying_car_data->fA8);
                add_vec_6ee0(&o->pos, &o->pos, &vec);
                scale_vec_6ee0(&vec, &flying_car_moby->rot_row2, flying_car_data->fAC);
                add_vec_6ee0(&o->pos, &o->pos, &vec);
            }
            if (flying_car_data->trigger >= 0) {
                flying_car_moby->flags |= 0x41;
                flying_car_moby->state = 1;
                flying_car_moby->collision = 0;
            }
        }
        break;
    }
    if (flying_car_moby->state == 0 || flying_car_moby->state == 3 || flying_car_moby->state == 1) {
        return;
    }
    if (FUN_L00_0028d8c0_6ee0(flying_car_moby, flying_car_data->sound) == 0) {
        flying_car_data->sound = play_sound_6ee0(0, 4, flying_car_moby);
    }
}
/* Precomputes per-point segment values of a path and snaps the moby to its first point. */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002FF358.c: func_L14_00308998), where it is exact; names translated to the US level program. */

extern char *D_L14_001B0BB0[];
extern float fast_vec_dist(void *, void *) __asm__("FUN_001f9b48");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");

void FUN_L14_00307510(struct Moby *moby) {
    char *flying_car_data = (char *)moby->pvars;
    char *path = D_L14_001B0BB0[*(int *)(flying_car_data + 0x78)];
    int i;
    for (i = 0; i < *(int *)path; i++) {
        int point_byte_offset = i * 16;
        float segment_length = fast_vec_dist(path + (point_byte_offset + 0x10), path + (((i + 1) % *(int *)path) * 16 + 0x10));
        *(float *)(path + point_byte_offset + 0x1C) = segment_length;
    }
    *(int *)(flying_car_data + 0x70) = 0;
    *(int *)(flying_car_data + 0x74) = 0;
    qcopy(&moby->pos, path + 0x10);
    moby->rot.z = fast_arc_tan(*(float *)(path + 0x20) - *(float *)(path + 0x10),
                                           *(float *)(path + 0x24) - *(float *)(path + 0x14));
    *(float *)(flying_car_data + 0x90) = *(float *)(flying_car_data + 0x94) = *(float *)(flying_car_data + 0x98) = *(float *)(flying_car_data + 0x80) =
        *(float *)(flying_car_data + 0x84) = *(float *)(flying_car_data + 0x88) = 0.0f;
}
#include "rnc/audio/voice_pool.h"
/* Flies the moby along its path. At the end of the path (or once it has sunk to the floor height) it bursts,
 * lets its rider go and returns 0; otherwise it speeds up while the hero rides it (or holds it), slows down
 * otherwise, sinks by its speed, eases onto the path point and banks into the turns, and returns 1. */
extern char *D_L14_001B0BB0_7620[] __asm__("D_L14_001B0BB0");
extern char *D_L14_0015FFD8_7620 __asm__("D_L14_0015FFD8");
extern float D_L14_0016226C __attribute__((sda));
extern float D_L14_00162270 __attribute__((sda));
extern float D_L14_00162274 __attribute__((sda));
extern float D_L14_00162278 __attribute__((sda));
extern float D_L14_00162280 __attribute__((sda));
extern float D_L14_00162288 __attribute__((sda));
extern float D_L14_0016228C __attribute__((sda));
extern int path_step_7620(char *path, float *out, void *pos, void *t, int dir, float step) __asm__("FUN_L00_0025d808");
extern void cart_burst(void *, void *, f32, f32, s32) __asm__("FUN_L00_0025f090");
extern void FUN_L00_00258278(struct Moby *, f32, f32 *, f32, f32, f32);
extern f32 vendor_spring(f32 *, f32 *, f32, f32, f32, f32) __asm__("FUN_L00_0025b8c0");
extern void release_voice_slot(s32 idx) __asm__("FUN_0022d798");
extern void subtract_vector_xyz(void *out, void *a, void *b) __asm__("FUN_001f9a28");
extern float AbsoluteFloat(float input) __asm__("FUN_001f99c0");
extern float atan2_7620(float, float) __asm__("FUN_001f9e90");
extern float fast_subtract_rotations(float, float) __asm__("FUN_001fa5c8");
extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");

int FUN_L14_00307620(struct Moby *moby) {
    float pos[4];
    float delta[4];
    char *flying_car_data = (char *)moby->pvars;
    float yaw;
    float bank;
    float pitch;
    int end;
    int voice_slot_index;

    end = path_step_7620(D_L14_001B0BB0_7620[*(int *)(flying_car_data + 0x78)], pos, flying_car_data + 0x70, flying_car_data + 0x74, *(short *)(flying_car_data + 0x8E),
                         *(float *)(flying_car_data + 0xC8) * frame_time);
    if ((*(short *)(flying_car_data + 0x8E) == 0 && end != 0) || moby->pos.z <= D_L14_0016228C) {
        cart_burst(moby, &moby->pos, 5.0f, 13.0f, -1);
        moby->state = 3;
        moby->flags |= 0x41;
        moby->collision = 0;
        if (*(int *)(flying_car_data + 0xA0) >= 0) {
            struct Moby *attached_part_moby = (struct Moby *)(D_L14_0015FFD8_7620 + *(int *)(flying_car_data + 0xA0) * 256);
            attached_part_moby->flags |= 0x41;
            attached_part_moby->pos.z = 5.0f;
            attached_part_moby->pos.y = 5.0f;
            attached_part_moby->pos.x = 5.0f;
        }
        voice_slot_index = *(int *)(flying_car_data + 0x7C);
        if (voice_slot_index != -1) {
            VoicePoolWindow *voice_entry = (VoicePoolWindow *)((u8 *)&voice_pool + voice_slot_index * 0x70);
            if (voice_entry->voice.owner == (VoiceMoby *)moby && voice_entry->voice.state != 0)
                release_voice_slot(voice_slot_index);
        }
        *(int *)(flying_car_data + 0x7C) = -1;
        return 0;
    }
    if (((unsigned)(hero.state.current - 0x25) < 2 && *(int *)(flying_car_data + 0xA0) >= 0 &&
         hero.unk964 == (struct Moby *)(D_L14_0015FFD8_7620 + *(int *)(flying_car_data + 0xA0) * 256)) ||
        (hero.state.control_mode == 0xE && *(int *)(flying_car_data + 0xA0) >= 0 &&
         hero.unk994 == (struct Moby *)(D_L14_0015FFD8_7620 + *(int *)(flying_car_data + 0xA0) * 256))) {
        float lim;
        float s;
        s = *(float *)(flying_car_data + 0xB4) + *(float *)(flying_car_data + 0xB8) * frame_scale_sq;
        lim = D_L14_00162280 * frame_time;
        *(float *)(flying_car_data + 0xB4) = s;
        if (lim < s)
            *(float *)(flying_car_data + 0xB4) = lim;
        *(float *)(flying_car_data + 0xB0) += *(float *)(flying_car_data + 0xB4);
    } else {
        *(float *)(flying_car_data + 0xB0) -= D_L14_00162288 * frame_time;
        *(int *)(flying_car_data + 0xB4) = 0;
        if (*(float *)(flying_car_data + 0xB0) < *(float *)(flying_car_data + 0xB4))
            *(float *)(flying_car_data + 0xB0) = *(float *)(flying_car_data + 0xB4);
    }
    pos[2] -= *(float *)(flying_car_data + 0xB0);
    if (pos[2] < 10.0f)
        pos[2] = 10.0f;
    vendor_spring(&moby->pos.x, (f32 *)(flying_car_data + 0x80), pos[0], D_L14_0016226C, D_L14_00162270, 0.0f);
    vendor_spring(&moby->pos.y, (f32 *)(flying_car_data + 0x84), pos[1], D_L14_0016226C, D_L14_00162270, 0.0f);
    vendor_spring(&moby->pos.z, (f32 *)(flying_car_data + 0x88), pos[2], D_L14_0016226C, D_L14_00162270, 0.0f);
    subtract_vector_xyz(delta, pos, &moby->pos);
    if (0.01f < AbsoluteFloat(delta[0]) && 0.01f < AbsoluteFloat(delta[1])) {
        yaw = atan2_7620(delta[0], delta[1]);
        bank = fast_subtract_rotations(moby->rot.z, yaw);
        FUN_L00_00258278(moby, yaw, (f32 *)(flying_car_data + 0x98), D_L14_0016226C, D_L14_00162270, 0.0f);
        pitch = -atan2_7620(vector_length_xy(delta), delta[2]);
        vendor_spring(&moby->rot.y, (f32 *)(flying_car_data + 0x94), pitch, D_L14_0016226C, D_L14_00162270, 0.0f);
        bank /= D_L14_00162278;
        if (1.0f < bank)
            bank = 1.0f;
        if (bank < -1.0f)
            bank = -1.0f;
        vendor_spring(&moby->rot.x, (f32 *)(flying_car_data + 0x90), bank * D_L14_00162274, 0.003f, D_L14_00162270, 0.0f);
    }
    return 1;
}
extern void FUN_L12_002e71b0(void);
void enqueue_callback_list_1(int arg0, int arg1) __asm__("FUN_001f4600");
/* 0x00307a80, 84 bytes.
 * Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002FF358.c:
 * func_L14_00308F08), where it is exact; names translated to the US level
 * program. The size-matched rac1-decomp neighbour was the callback this one enqueues,
 * not this body. */
void enqueue_moby_draw_callback(struct Moby *moby) __asm__("FUN_L14_00307a80");

void enqueue_moby_draw_callback(struct Moby *moby) {
    enum {
        DRAW_CALLBACK_INITIALIZE = 0,
        DRAW_CALLBACK_ENQUEUE = 1
    };
    switch (moby->state) {
    case DRAW_CALLBACK_INITIALIZE:
        moby->state = 1;
        moby->update_dist = 0xFF;
        break;
    case DRAW_CALLBACK_ENQUEUE:
        enqueue_callback_list_1((int)FUN_L12_002e71b0, (int)moby);
        break;
    }
}

/* Play the level sound once on first update, then handle the two-state moby. */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002FF358.c: func_L14_00309C48), where it is exact; names translated to the US level program. */

typedef struct {
    char pad0[0x30];
    int state;
    char pad34[0x144];
    int v178[2];
} Level14State;

extern Level14State D_L14_0016CF60;
extern int D_L14_0015F5C4;
extern int FUN_L01_0026e008(int, int);
extern unsigned char D_0013D408[];
extern void cutscene_set_camera_target(int) __asm__("FUN_L00_002637f8");
extern void queue_dialogue_message(int arg0, int arg1) __asm__("FUN_L00_00263d40");
s32 allocate_voice_for_bank_entry(s32 arg1, s32 arg0, s32 arg2) __asm__("FUN_0022db10");

void FUN_L14_003087c0(struct Moby *moby) {
    enum {
        MOBY_UPDATE_DISTANCE_INITIALIZE = 0,
        MOBY_UPDATE_CUTSCENE_TARGET = 1
    };
    int *moby_pvars = (int *)moby->pvars;
    unsigned level_state_offset;
    int i;
    if (moby_pvars[0] != -1) {
        if (FUN_L01_0026e008(moby_pvars[0], -1) == 0) {
            if (D_0013D408[0x17] == 0) {
                D_0013D408[0x17] = 1;
                allocate_voice_for_bank_entry(1, 0, 0);
                queue_dialogue_message(0x53d6, -1);
            }
        }
    }
    switch (moby->state) {
    case MOBY_UPDATE_DISTANCE_INITIALIZE:
        moby->update_dist = 0xFF;
        moby->state = 1;
        break;
    case MOBY_UPDATE_CUTSCENE_TARGET:
        if (D_L14_0015F5C4 == 2) {
            level_state_offset = D_L14_0016CF60.state - 3;
            if (level_state_offset < 3) {
                i = 0;
                if (level_state_offset <= D_L14_0015F5C4)
                    i = moby->state;
                cutscene_set_camera_target(D_L14_0016CF60.v178[i]);
            }
        }
        break;
    }
}
extern char *D_L14_0015EF50 __asm__("D_L14_0015EF50");
extern char *D_L14_001600EC __asm__("D_L14_001600EC");
typedef struct {
    char pad0[0x184];
    char *start;
} VendorSet_4f00;
extern VendorSet_4f00 D_L14_00167380 __asm__("D_L14_00167380");
extern s32 scale_game_frames_4f00(s32) __asm__("FUN_001f96f8");
extern f32 ConvertIntegerToFloat_4f00(s32) __asm__("FUN_001fa6c0");
extern float atan2_4f00(float, float) __asm__("FUN_001f9e90");
extern float FUN_001f9b48(void *, void *);
extern void clear_4f00(void *) __asm__("FUN_001f99f8");
extern void place_4f00(void *, void *, int, int) __asm__("FUN_L00_00216f90");

typedef struct {
    s16 seg[8];
    f32 fov[8];
    f32 fov_end;
} Stops_4f00;
typedef struct {
    s32 seg;
    f32 t;
    f32 len_a;
    f32 len_b;
    f32 dist;
    s16 started;
    s16 timer;
} Run_4f00;
void FUN_L14_00314f00(char *camera) {
    u128 stand;
    char *rec = *(char **)(D_L14_0015EF50 + *(short *)(camera + 0x84) * 32 + 0x1C);
    char *camera_saved_state;
    char *fov_stops_state_base;
    char *path;
    char *initial_camera_transform;
    int *camera_run;
    int *w;
    short *s;
    Stops_4f00 *fov_stops;
    float *wf;
    short *ss;
    float segment_length;
    int i;
    int fov_stop_count;

    (*&rec)[0x38] = 3;
    camera_saved_state = *(char **)(camera + 0x70);
    *(int *)(camera_saved_state + 0x40) = 0;
    camera_run = (int *)(camera_saved_state + 0x40);
    camera_run[1] = 0;
    camera_run[2] = 0;
    camera_run[3] = 0;
    camera_run[4] = 0;
    *(short *)(camera_run + 5) = 0;
    *((short *)(camera_run + 5) + 1) = scale_game_frames_4f00(*(short *)(rec + 0x3A));
    fov_stops_state_base = *(char **)(camera + 0x70);
    fov_stops = (Stops_4f00 *)(fov_stops_state_base + 0x58);
    {
        int k;
        for (k = 0; k < 8; k++) {
            fov_stops->fov[k] = 0.0f;
            fov_stops->seg[k] = -1;
        }
    }
    /* Fakematch: the dead update rotates the three address temps around
       the atan2 call into retail's v0/a0/a1. */
    D_L14_001600EC++;
    D_L14_001600EC--;
    fov_stops->fov_end = atan2_4f00(1.0f, D_L14_0016D140.fB0) * 2.0f;
    if (*(int *)(rec + 0x20) >= 0) {
        path = D_L14_001B0BB0[*(int *)(rec + 0x20)];
        fov_stop_count = 0;
        for (i = 0; i < *(int *)path; i++) {
            char *seg = path + i * 16;
            if (0.0f < *(float *)(seg + 0x1C)) {
                fov_stops->fov[fov_stop_count] = *(float *)(seg + 0x1C);
                fov_stops->seg[fov_stop_count] = i;
                fov_stop_count++;
            }
            segment_length = FUN_001f9b48(path + (i * 16 + 0x10), path + (((i + 1) % *(int *)path) * 16 + 0x10));
            *(float *)(seg + 0x1C) = segment_length;
            if (i < *(int *)path - 1)
                *(float *)(camera_run + 2) += segment_length;
        }
    }
    if (*(short *)(rec + 0x3A) > 0) {
        *(float *)(rec + 0x3C) =
            *(float *)(camera_run + 2) / ConvertIntegerToFloat_4f00(scale_game_frames_4f00(*(short *)(rec + 0x3A))) * 60.0f;
    }
    if (*(int *)(rec + 0x24) >= 0) {
        path = D_L14_001B0BB0[*(int *)(rec + 0x24)];
        for (i = 0; i < *(int *)path; i++) {
            char *seg;
            segment_length = FUN_001f9b48(path + (i * 16 + 0x10), path + (((i + 1) % *(int *)path) * 16 + 0x10));
            seg = path + i * 16;
            *(float *)(seg + 0x1C) = segment_length;
            if (i < *(int *)path - 1)
                *(float *)(camera_run + 3) += segment_length;
        }
    }
    camera_saved_state = *(char **)(camera + 0x70);
    *(float *)(camera_saved_state + 0x0) = 0.03f;
    *(float *)(camera_saved_state + 0x4) = 0.2f;
    clear_4f00(camera_saved_state + 0x10);
    camera_saved_state = *(char **)(camera + 0x70);
    *(float *)(camera_saved_state + 0x20) = 0.03f;
    *(float *)(camera_saved_state + 0x24) = 0.2f;
    clear_4f00(camera_saved_state + 0x30);
    initial_camera_transform = D_L14_00167380.start;
    qcopy(camera + 0x30, initial_camera_transform + 0x30);
    qcopy(camera, initial_camera_transform);
    qcopy(camera + 0x10, initial_camera_transform + 0x10);
    qcopy(camera + 0x20, initial_camera_transform + 0x20);
    path = D_L14_001600EC + *(int *)(rec + 0x54) * 128 + 0x30;
    stand = 0;
    ((float *)&stand)[3] = 1.0f;
    place_4f00(path, &stand, 0x72, 0);
}
#include "rnc/gameplay/camera/update_cam.h"
/* Runs the vendor camera along its slot path: on the first frame it starts at the path head looking at the
 * slot target; then it advances along the path, blends the field of view between the path stops, and keeps
 * looking at the target (a moby offset or the look path). At the end it hands the view back behind the hero. */
typedef struct {
    s16 seg[8];   /* 0x00: path segment of each stop (-1 ends the list) */
    f32 fov[8];   /* 0x10: field of view at each stop, degrees */
    f32 fov_end;  /* 0x30: field of view handed back at the end */
} VendorCamStops_5290;

typedef struct {
    s32 seg;      /* 0x40 */
    f32 t;        /* 0x44 */
    f32 len_a;    /* 0x48: length of the camera path */
    f32 len_b;    /* 0x4C: length of the look path */
    f32 dist;     /* 0x50: distance travelled */
    s16 started;  /* 0x54 */
    s16 timer;    /* 0x56 */
} VendorCamRun_5290;

extern char *D_L14_0015EF50_5290 __asm__("D_L14_0015EF50");
extern char *D_L14_001B0BB0_5290[] __asm__("D_L14_001B0BB0");
extern char *D_L14_0015FFD8_5290 __asm__("D_L14_0015FFD8");
extern s32 D_L14_0015F404;
extern char D_L14_001676D0[];
extern float D_L14_001675D8 __attribute__((section(".data")));
extern float D_0013F5E0_5290[] __asm__("D_0013F5E0");
extern int path_step_5290(char *path, float *out, void *seg, void *t, int dir, float step) __asm__("FUN_L00_0025d808");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
extern float cosine_interpolate(float, float, float) __asm__("FUN_002133d0");
extern void update_view_context(void) __asm__("FUN_001f2d98");
extern float fast_sin_5290(float) __asm__("FUN_001f9de0");
extern float fast_cos_5290(float) __asm__("FUN_001f9dc8");
extern void FUN_L14_002d6570(struct Moby *m);
extern float FUN_001f9b48_5290(void *, void *) __asm__("FUN_001f9b48");
extern void sub_5290(void *, void *, void *) __asm__("FUN_001f9a28");
extern void add_5290(void *, void *, void *) __asm__("FUN_001f9a10");
extern void scale_5290(void *, void *, float) __asm__("FUN_001f9a68");
extern void normalize_5290(void *, void *, float) __asm__("FUN_001f9bf8");
extern void cross_5290(void *, void *, void *) __asm__("FUN_001f9ad8");
extern s32 tick_countdown_16_5290(s16 *) __asm__("FUN_001f9770");

void FUN_L14_00315290(struct UpdateCam *path_camera) {
    float initial_look_direction_or_previous_position[4];
    float t[4];
    float u[4];
    float v[4];
    s32 seg;
    f32 tt;
    s32 seg2;
    f32 tt2;
    char *camera_saved_state = (char *)path_camera->saved_state;
    char *rec = *(char **)(D_L14_0015EF50_5290 + path_camera->descriptor_index * 32 + 0x1C);
    VendorCamRun_5290 *camera_run = (VendorCamRun_5290 *)(camera_saved_state + 0x40);
    VendorCamStops_5290 *fov_stops = (VendorCamStops_5290 *)(camera_saved_state + 0x58);
    char *path;
    int done;
    int i;

    if (camera_run->started == 0) {
        char *pa = D_L14_001B0BB0_5290[*(int *)(rec + 0x20)];
        char *pb = D_L14_001B0BB0_5290[*(int *)(rec + 0x24)];
        D_L14_0015F404 = 1;
        camera_run->started = 1;
        qcopy(&path_camera->pos, pa + 0x10);
        if (*(int *)(rec + 0x28) >= 0) {
            struct Moby *o = (struct Moby *)(D_L14_0015FFD8_5290 + *(int *)(rec + 0x28) * 256);
            float *ofs = (float *)(rec + 0x2C);
            t[0] = ofs[0];
            t[1] = ofs[1];
            t[2] = ofs[2];
            t[3] = 0.0f;
            transform_vector_by_basis(t, t, D_L14_001676D0);
            add_5290(u, &o->pos, t);
            sub_5290(initial_look_direction_or_previous_position, u, &path_camera->pos);
        } else {
            sub_5290(initial_look_direction_or_previous_position, pb + 0x10, &path_camera->pos);
        }
        scale_5290(t, D_0013F5E0_5290, -1.0f);
        normalize_5290(&path_camera->m0, initial_look_direction_or_previous_position, 1.0f);
        cross_5290(&path_camera->m1, &path_camera->m0, t);
        normalize_5290(&path_camera->m1, &path_camera->m1, 1.0f);
        cross_5290(&path_camera->m2, &path_camera->m1, &path_camera->m0);
        return;
    }
    path = D_L14_001B0BB0_5290[*(int *)(rec + 0x20)];
    qcopy(initial_look_direction_or_previous_position, &path_camera->pos);
    done = path_step_5290(path, &path_camera->pos.x, &camera_run->seg, &camera_run->t, 0, *(float *)(rec + 0x3C) * frame_time);
    if (camera_run->timer != 0 && tick_countdown_16_5290(&camera_run->timer) != 0)
        done = 1;
    if (*(u8 *)(rec + 0x39) != 0)
        done = 1;
    if (done) {
        float ang = fov_stops->fov_end * 0.5f;
        char *hp;
        char *mb;
        D_L14_0016D140.fB0 = fast_sin_5290(ang) / fast_cos_5290(ang);
        update_view_context();
        D_L14_001675D8 = 0.25f;
        path_camera->transition_state = 4;
        D_L14_0015F404 = 0;
        mb = D_L14_0015FFD8_5290;
        FUN_L14_002d6570((struct Moby *)(mb + *(int *)(rec + 0x50) * 256));
        hp = (char *)hero.unk560;
        seg = hero.unk564;
        tt = hero.unk568;
        path_step_5290(hp, &path_camera->pos.x, &seg, &tt, 0, 10.0f);
        path_camera->pos.z += 2.0f;
        seg = hero.unk564;
        tt = hero.unk568;
        path_step_5290(hp, u, &seg, &tt, 0, 0.0f);
        u[2] += 1.5f;
        sub_5290(t, u, &path_camera->pos);
        normalize_5290(&path_camera->m0, t, 1.0f);
        cross_5290(&path_camera->m1, &path_camera->m0, &hero.gravity_dir);
        normalize_5290(&path_camera->m1, &path_camera->m1, -1.0f);
        cross_5290(&path_camera->m2, &path_camera->m1, &path_camera->m0);
        return;
    }
    for (i = 0; i < 7 && fov_stops->seg[i] >= 0 && fov_stops->seg[i + 1] >= 0; i++) {
        int lo = fov_stops->seg[i];
        if (camera_run->seg >= lo && camera_run->seg < fov_stops->seg[i + 1]) {
            float sum = 0.0f;
            float total;
            float ang;
            int traversed_segment_index;
            int remaining_segment_index;
            for (traversed_segment_index = lo; traversed_segment_index < camera_run->seg; traversed_segment_index++)
                sum += *(float *)(path + (traversed_segment_index * 16 + 0x1C));
            total = sum;
            sum += camera_run->t;
            for (remaining_segment_index = camera_run->seg; remaining_segment_index < fov_stops->seg[i + 1]; remaining_segment_index++)
                total += *(float *)(path + (remaining_segment_index * 16 + 0x1C));
            ang = cosine_interpolate(fov_stops->fov[i], fov_stops->fov[i + 1], sum / total) * 0.017453292f * 0.5f;
            D_L14_0016D140.fB0 = fast_sin_5290(ang) / fast_cos_5290(ang);
            update_view_context();
        }
    }
    camera_run->dist += FUN_001f9b48_5290(initial_look_direction_or_previous_position, &path_camera->pos);
    if (*(int *)(rec + 0x28) >= 0) {
        struct Moby *o = (struct Moby *)(D_L14_0015FFD8_5290 + *(int *)(rec + 0x28) * 256);
        float *ofs = (float *)(rec + 0x2C);
        u[0] = ofs[0];
        u[1] = ofs[1];
        u[2] = ofs[2];
        u[3] = 0.0f;
        transform_vector_by_basis(u, u, D_L14_001676D0);
        add_5290(v, &o->pos, u);
        sub_5290(t, v, &path_camera->pos);
    } else {
        char *look = D_L14_001B0BB0_5290[*(int *)(rec + 0x24)];
        seg2 = 0;
        tt2 = 0.0f;
        path_step_5290(look, u, &seg2, &tt2, 0, camera_run->dist / camera_run->len_a * camera_run->len_b);
        sub_5290(t, u, &path_camera->pos);
    }
    scale_5290(u, D_0013F5E0_5290, -1.0f);
    normalize_5290(&path_camera->m0, t, 1.0f);
    cross_5290(&path_camera->m1, &path_camera->m0, u);
    normalize_5290(&path_camera->m1, &path_camera->m1, 1.0f);
    cross_5290(&path_camera->m2, &path_camera->m1, &path_camera->m0);
}
#ifndef NOT_SDA
#define NOT_SDA __attribute__((section(".data")))
#endif
#ifndef MACRO_ADDR
#define MACRO_ADDR __attribute__((section(".sdata")))
#endif
extern int func_L10_002F6E38_15920(void *) __asm__("FUN_L10_002f5a78");
extern char *D_L14_0015F050_15920 __asm__("D_L14_0015EF50");
int FUN_L14_00315920(char *camera, char *control_mode_gate_object) {
    char *rec = *(char **)(D_L14_0015F050_15920 + *(short *)(camera + 0x84) * 32 + 0x1C);
    int required_control_mode, required_hero_state;
    if (*(unsigned char *)(rec + 0x38) != 0)
        return -1;
    if (control_mode_gate_object != 0 && *(short *)(control_mode_gate_object + 0x7E) == 0) {
        if (hero.state.control_mode != 15)
            return 0;
    }
    if (func_L10_002F6E38_15920(camera) == 0)
        return 0;
    required_control_mode = *(int *)(rec + 0x48);
    if (required_control_mode >= 0 && hero.state.control_mode != required_control_mode)
        return 0;
    required_hero_state = *(int *)(rec + 0x44);
    if (required_hero_state >= 0 && hero.state.current != required_hero_state)
        return 0;
    return 1;
}
/* Starts the vendor camera: measures the three camera paths of the slot record into the length tables,
 * resets the camera's blend state, loads the default angles and aims the camera at the hero. */
extern char *D_L14_0015EF50_59d8 __asm__("D_L14_0015EF50");
extern char *D_L14_0015F70C_59d8 __asm__("D_L14_0015F70C");
/* the third path read goes through a volatile alias (retail reloads the zone table there) */
extern char *volatile D_L14_0015F70C_59v __asm__("D_L14_0015F70C");
extern float D_L14_001F6C80[][1000];
extern float D_L14_001F6488_59d8[] __asm__("D_L14_001F6488");
extern float D_L14_001624A8 __attribute__((sda));
extern float atan2_59d8(float, float) __asm__("FUN_001f9e90");
extern void FUN_L00_001ed580(void *, void *, void *);
extern void sub_59d8(void *, void *, void *) __asm__("FUN_001f9a28");
extern void scale_59d8(void *, void *, float) __asm__("FUN_001f9a68");
extern void add_59d8(void *, void *, void *) __asm__("FUN_001f9a10");
extern void normalize_59d8(void *, void *, float) __asm__("FUN_001f9bf8");
extern void cross_59d8(void *, void *, void *) __asm__("FUN_001f9ad8");

void FUN_L14_003159d8(struct UpdateCam *cam) {
    float camera_placement_parameters[4];
    float pad[4]; /* unused; keeps the frame layout */
    float hero_look_direction[4];
    float up[4];
    char *rec = *(char **)(D_L14_0015EF50_59d8 + cam->descriptor_index * 32 + 0x1C);
    char *path;
    char *tbl;
    char *camera_saved_state;
    float *camera_parameters;
    int *camera_state_block;
    int j;
    int *camera_parameter_state_block;
    int *camera_key_default_cursor;
    float sum;
    float hero_heading_radians;
    int i;

    tbl = D_L14_0015F70C_59d8;
    *(int *)(rec + 0x20) = 2;
    *(short *)(rec + 0x30) = 0;
    *(short *)(rec + 0x32) = 0;
    path = *(char **)(tbl + *(int *)(rec + 0x34) * 32 + 0x10);
    sum = 0.0f;
    for (i = 0; i < *(int *)path - 1; i++) {
        D_L14_001F6C80[0][i] = sum;
        sum += *(float *)(path + i * 16 + 0x1C);
    }
    D_L14_001F6C80[0][i] = sum;
    path = *(char **)(D_L14_0015F70C_59d8 + *(int *)(rec + 0x38) * 32 + 0x10);
    sum = 0.0f;
    for (i = 0; i < *(int *)path - 1; i++) {
        D_L14_001F6C80[1][i] = sum;
        sum += *(float *)(path + i * 16 + 0x1C);
    }
    D_L14_001F6C80[1][i] = sum;
    path = *(char **)(D_L14_0015F70C_59v + *(int *)(rec + 0x3C) * 32 + 0x10);
    sum = 0.0f;
    for (i = 0; i < *(int *)path - 1; i++) {
        D_L14_001F6C80[2][i] = sum;
        sum += *(float *)(path + i * 16 + 0x1C);
    }
    D_L14_001F6C80[2][i] = sum;
    camera_saved_state = cam->saved_state;
    *(int *)(camera_saved_state + 0x88) = 1;
    camera_state_block = (int *)(camera_saved_state + 0x88);
    camera_state_block[1] = 0;
    camera_state_block[2] = 0;
    camera_state_block[3] = 0;
    camera_state_block[4] = 0;
    D_L14_001624A8 = 1.0f;
    camera_parameters = (float *)((char *)cam->saved_state + 0x60);
    camera_parameters[0] = D_L14_001F6488_59d8[0] * 0.017453292f;
    camera_parameters[1] = D_L14_001F6488_59d8[1] * 0.017453292f;
    camera_parameters[2] = D_L14_001F6488_59d8[2];
    camera_parameters[3] = D_L14_001F6488_59d8[3] * 0.017453292f;
    camera_parameters[4] = D_L14_001F6488_59d8[4] * 0.017453292f;
    camera_parameter_state_block = (int *)((char *)cam->saved_state + 0x74);
    *camera_parameter_state_block = 0;
    camera_parameter_state_block[1] = 0;
    camera_parameter_state_block[2] = 0;
    camera_parameter_state_block[3] = 0;
    camera_parameter_state_block[4] = 0;
    hero_heading_radians = atan2_59d8(hero.moby->rot_row1.x, hero.moby->rot_row1.y);
    camera_placement_parameters[1] = camera_parameters[1];
    camera_placement_parameters[0] = hero_heading_radians;
    camera_placement_parameters[2] = camera_parameters[2];
    FUN_L00_001ed580(&cam->pos, camera_placement_parameters, &hero.motion);
    sub_59d8(hero_look_direction, &hero.motion, &cam->pos);
    scale_59d8(up, &hero.gravity_dir, -1.5f);
    add_59d8(hero_look_direction, hero_look_direction, up);
    normalize_59d8(&cam->m0, hero_look_direction, 1.0f);
    cross_59d8(&cam->m1, &cam->m0, up);
    normalize_59d8(&cam->m1, &cam->m1, 1.0f);
    cross_59d8(&cam->m2, &cam->m1, &cam->m0);
    /* give every key without a duration (+0x44) the default 10000 */
    camera_key_default_cursor = (int *)&D_L14_001F6488_59d8[0x11];
    for (j = 38; j >= 0; j--) {
        if (*camera_key_default_cursor == 0)
            goto fix;
    next:
        camera_key_default_cursor += 12;
        continue;
    fix:
        *camera_key_default_cursor = 10000;
        goto next;
    }
}
/* Camera 21 (level 14) layouts */
typedef struct {
    char pad0[0x24];
    int m24;
    int m28;
    int m2C;
    short n30;
    short n32;
    int p34;
    int p38;
} CamRec5d48;
typedef struct {
    int count;
    int idx;
    float f8;
    float fC;
    float f10;
} CamState5d48;
typedef struct {
    float f0;
    float f4;
    float f8;
    float fC;
    float f10;
} CamAngles5d48;
typedef struct {
    char pad0[0x60];
    CamAngles5d48 cur;
    float vel[5];
    CamState5d48 st;
} CamData5d48;
typedef struct {
    char pad0[0x70];
    CamData5d48 *data;
    char pad1[0x10];
    short slot;
} CamObj5d48;
typedef struct {
    char pad0[0x1C];
    CamRec5d48 *rec;
} CamSlot5d48;
typedef struct {
    char pad0[0x10];
    int id;
    char pad1[0xC];
} CamZone5d48;
typedef struct {
    char pad0[0x60];
    int zone;
    char pad1[0x164];
    int timer;
} CamMobyData5d48;
typedef struct {
    float a0;
    float a4;
    float a8;
    float aC;
    float a10;
    int n14;
    float t18;
    float t1C;
    float t20;
    float t24;
    float t28;
    float t2C;
} CamKey5d48;
typedef struct {
    char pad0[0x274];
    float f274;
    float f278;
} CamBones5d48;

#define MOBYDATA_5d48(m) ((CamMobyData5d48 *)(m)->pvars)
extern CamSlot5d48 *D_L14_0015EF50_s __asm__("D_L14_0015EF50") __attribute__((section(".sdata")));
extern CamZone5d48 *D_L14_0015F70C_z __asm__("D_L14_0015F70C") __attribute__((section(".sdata")));
extern char *D_L14_0015FFD8_5d48 __asm__("D_L14_0015FFD8") __attribute__((section(".sdata")));
extern int D_L14_001624B4 __attribute__((section(".sdata")));
extern int D_L14_001624B8 __attribute__((section(".sdata")));
extern float D_L14_001624BC __attribute__((section(".sdata")));
extern float D_L14_001624C0 __attribute__((section(".sdata")));
extern float D_L14_00162490 __attribute__((sda));
extern float D_L14_00162494 __attribute__((sda));
extern float D_L14_00162498 __attribute__((sda));
extern float D_L14_0016249C __attribute__((sda));
extern float D_L14_001624A0 __attribute__((sda));
extern float D_L14_001624A4 __attribute__((sda));
extern float D_L14_001624AC __attribute__((sda));
extern float D_L14_001624B0 __attribute__((sda));
extern float D_L14_001F6C38[][3];
extern float D_L14_001F6C08[][3];
extern CamKey5d48 D_L14_001F6488[];
extern char D_L14_0017AD80[];
extern int scale_ticks_5d48(int) __asm__("FUN_001f96f8");
extern int random_integer_below(int) __asm__("FUN_00213260");
extern float FUN_L00_0025b750(float, float, float);
extern float cam_interp_angle_5d48(void *, float, float, float, float, float) __asm__("FUN_L00_001eb328");
extern float cam_interp_5d48(void *, float, float, float, float, float) __asm__("FUN_001ebd78");
extern float atan2_5d48(float, float) __asm__("FUN_001f9e90");
extern float add_rots_5d48(float, float) __asm__("FUN_001fa580");
extern float ease_5d48(float, float, float, float, float) __asm__("FUN_L00_00257e20");
extern void FUN_L00_001ed580_5d48(void *, void *, void *) __asm__("FUN_L00_001ed580");
extern void vec_sub_5d48(void *, void *, void *) __asm__("FUN_001f9a28");
extern void vec_scale_5d48(void *, void *, float) __asm__("FUN_001f9a68");
extern void vec_add_5d48(void *, void *, void *) __asm__("FUN_001f9a10");
extern void vec_normalize_5d48(void *, void *, float) __asm__("FUN_001f9bf8");
extern void vec_cross_5d48(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void rotate_axis_5d48(void *dst, void *vec, void *axis, float angle) __asm__("FUN_00214890");

/* Ported from rac1-decomp src/overlays/l14_oltanis/vendor_002FF358.c (func_L14_003171D0) */
void FUN_L14_00315d48(char *cam) {
    CamData5d48 *data = ((CamObj5d48 *)cam)->data;
    CamState5d48 *st = &data->st;
    CamRec5d48 *rec = D_L14_0015EF50_s[((CamObj5d48 *)cam)->slot].rec;
    CamAngles5d48 *cur = &data->cur;
    float *vel = data->vel;
    int side;
    float key_interpolation_fraction;
    float k;
    float x;
    float camera_placement_heading;
    float current_key_heading_override;
    float heading_blend_target;
    float camera_placement_parameters[4];
    float unused[4];
    CamKey5d48 *keys;
    int next;
    float look_direction[4];
    float scaled_up_vector[4];

    D_L14_001624B4 = data->st.count;
    D_L14_001624B8 = st->idx;
    if (hero.unk560 == D_L14_0015F70C_z[rec->p34].id) {
        side = 0;
    } else if (hero.unk560 == D_L14_0015F70C_z[rec->p38].id) {
        side = 1;
    } else {
        side = 2;
    }
    D_L14_001624C0 = (D_L14_001F6C80[side][hero.unk564] + hero.unk568) / D_L14_001F6C80[side][999];
    if (rec->n32 < 2 && D_L14_001F6C38[rec->n32][side] <= D_L14_001624C0) {
        rec->n32++;
    }
    if (rec->n32 == 1) {
        ((CamBones5d48 *)D_L14_0017AD80)->f274 = D_L14_001624B0 * 0.017453292f;
        ((CamBones5d48 *)D_L14_0017AD80)->f278 = D_L14_001624AC * 0.017453292f;
    }
    if (rec->n30 < 3 && D_L14_001F6C08[rec->n30][side] <= D_L14_001624C0) {
        struct Moby *first_linked_moby = (struct Moby *)(D_L14_0015FFD8_5d48 + (rec->m24 << 8));
        struct Moby *second_linked_moby = (struct Moby *)(D_L14_0015FFD8_5d48 + (rec->m28 << 8));
        struct Moby *third_linked_moby = (struct Moby *)(D_L14_0015FFD8_5d48 + (rec->m2C << 8));

        if (hero.unk560 == D_L14_0015F70C_z[MOBYDATA_5d48(first_linked_moby)->zone].id) {
            first_linked_moby->command = 1;
            if (rec->n30 > 0) {
                {
                    CamMobyData5d48 *d = MOBYDATA_5d48(second_linked_moby);

                    second_linked_moby->command = 5;
                    if (rec->n30 == 2) {
                        CamMobyData5d48 *d2;

                        d->timer = scale_ticks_5d48(0x2D);
                        third_linked_moby->command = 5;
                        d2 = MOBYDATA_5d48(third_linked_moby);
                        d2->timer = scale_ticks_5d48(0x6E);
                    } else if (rec->n30 == 3) {
                        d->timer = scale_ticks_5d48(0x2D);
                    } else {
                        d->timer = scale_ticks_5d48(0x50);
                    }
                }
            }
        } else if (hero.unk560 == D_L14_0015F70C_z[MOBYDATA_5d48(second_linked_moby)->zone].id) {
            second_linked_moby->command = 1;
            if (rec->n30 > 0) {
                if (random_integer_below(2)) {
                    CamMobyData5d48 *d = MOBYDATA_5d48(first_linked_moby);

                    first_linked_moby->command = 5;
                    if (rec->n30 == 2) {
                        CamMobyData5d48 *d2;

                        d->timer = scale_ticks_5d48(0x2D);
                        third_linked_moby->command = 5;
                        d2 = MOBYDATA_5d48(third_linked_moby);
                        d2->timer = scale_ticks_5d48(0x6E);
                    } else if (rec->n30 == 3) {
                        d->timer = scale_ticks_5d48(0x2D);
                    } else {
                        d->timer = scale_ticks_5d48(0x50);
                    }
                } else {
                    CamMobyData5d48 *d = MOBYDATA_5d48(third_linked_moby);

                    third_linked_moby->command = 5;
                    if (rec->n30 == 2) {
                        CamMobyData5d48 *d2;

                        d->timer = scale_ticks_5d48(0x2D);
                        first_linked_moby->command = 5;
                        d2 = MOBYDATA_5d48(first_linked_moby);
                        d2->timer = scale_ticks_5d48(0x6E);
                    } else if (rec->n30 == 3) {
                        d->timer = scale_ticks_5d48(0x2D);
                    } else {
                        d->timer = scale_ticks_5d48(0x50);
                    }
                }
            }
        } else {
            third_linked_moby->command = 1;
            if (rec->n30 > 0) {
                {
                    CamMobyData5d48 *d = MOBYDATA_5d48(second_linked_moby);

                    second_linked_moby->command = 5;
                    if (rec->n30 == 2) {
                        CamMobyData5d48 *d2;

                        d->timer = scale_ticks_5d48(0x2D);
                        first_linked_moby->command = 5;
                        d2 = MOBYDATA_5d48(first_linked_moby);
                        d2->timer = scale_ticks_5d48(0x6E);
                    } else if (rec->n30 == 3) {
                        d->timer = scale_ticks_5d48(0x2D);
                    } else {
                        d->timer = scale_ticks_5d48(0x50);
                    }
                }
            }
        }
        rec->n30++;
    }
    /* Unused reads (not in rac1-decomp's SN build): with them the game compiler computes the key table's
     * high half and the next key index above the three side tests, as retail does. */
    keys = D_L14_001F6488;
    next = st->idx + 1;
    if (side == 0 && D_L14_001F6488[st->idx + 1].t24 <= D_L14_001624C0) {
        st->idx++;
    } else if (side == 1 && D_L14_001F6488[st->idx + 1].t28 <= D_L14_001624C0) {
        st->idx++;
    } else if (side == 2 && D_L14_001F6488[st->idx + 1].t2C <= D_L14_001624C0) {
        st->idx++;
    }
    if (D_L14_001F6488[st->idx + 1].n14 == 0x2710) {
        key_interpolation_fraction = 1.0f;
        st->idx--;
    } else {
        float num;
        float den;

        if (side == 0) {
            den = D_L14_001F6488[st->idx + 1].t24 - D_L14_001F6488[st->idx].t24;
            num = D_L14_001624C0 - D_L14_001F6488[st->idx].t24;
        } else if (side == 1) {
            den = D_L14_001F6488[st->idx + 1].t28 - D_L14_001F6488[st->idx].t28;
            num = D_L14_001624C0 - D_L14_001F6488[st->idx].t28;
        } else {
            den = D_L14_001F6488[st->idx + 1].t2C - D_L14_001F6488[st->idx].t2C;
            num = D_L14_001624C0 - D_L14_001F6488[st->idx].t2C;
        }
        key_interpolation_fraction = num / den;
    }
    k = D_L14_00162490;
    x = FUN_L00_0025b750(D_L14_001F6488[st->idx].a0 * 0.017453292f, D_L14_001F6488[st->idx + 1].a0 * 0.017453292f, key_interpolation_fraction);
    cur->f0 = cam_interp_angle_5d48(vel, cur->f0, x, k, 0.2f, 0.0f);
    k = D_L14_00162494;
    x = FUN_L00_0025b750(D_L14_001F6488[st->idx].a4 * 0.017453292f, D_L14_001F6488[st->idx + 1].a4 * 0.017453292f, key_interpolation_fraction);
    cur->f4 = cam_interp_angle_5d48(vel + 1, cur->f4, x, k, 0.2f, 0.0f);
    cur->f8 = cam_interp_5d48(vel + 2, cur->f8, D_L14_001F6488[st->idx].a8 + (D_L14_001F6488[st->idx + 1].a8 - D_L14_001F6488[st->idx].a8) * key_interpolation_fraction, D_L14_00162498, 0.2f, 0.0f);
    k = D_L14_0016249C;
    x = FUN_L00_0025b750(D_L14_001F6488[st->idx].aC * 0.017453292f, D_L14_001F6488[st->idx + 1].aC * 0.017453292f, key_interpolation_fraction);
    cur->fC = cam_interp_angle_5d48(vel + 3, cur->fC, x, k, 0.2f, 0.0f);
    k = D_L14_001624A0;
    x = FUN_L00_0025b750(D_L14_001F6488[st->idx].a10 * 0.017453292f, D_L14_001F6488[st->idx + 1].a10 * 0.017453292f, key_interpolation_fraction);
    cur->f10 = cam_interp_angle_5d48(vel + 4, cur->f10, x, k, 0.2f, 0.0f);
    camera_placement_heading = add_rots_5d48(atan2_5d48(hero.moby->rot_row1.x, hero.moby->rot_row1.y), cur->f0);
    D_L14_001624BC = camera_placement_heading;
    if (side == 0) {
        current_key_heading_override = D_L14_001F6488[st->idx].t18;
        heading_blend_target = D_L14_001F6488[st->idx + 1].t18;
    } else if (side == 1) {
        current_key_heading_override = D_L14_001F6488[st->idx].t1C;
        heading_blend_target = D_L14_001F6488[st->idx + 1].t1C;
    } else {
        current_key_heading_override = D_L14_001F6488[st->idx].t20;
        heading_blend_target = D_L14_001F6488[st->idx + 1].t20;
    }
    if (heading_blend_target == 0.0f && current_key_heading_override == 0.0f) {
        heading_blend_target = (st->fC != 0.0f) ? st->fC : st->f8;
        st->f10 += (0.0f - st->f10) * D_L14_001624A4;
        camera_placement_heading = FUN_L00_0025b750(camera_placement_heading, heading_blend_target, ease_5d48(1.0f, 0.0f, 1.0f, 0.0f, st->f10));
    } else {
        if (heading_blend_target != 0.0f) {
            st->f10 += (1.0f - st->f10) * D_L14_001624A4;
            camera_placement_heading = FUN_L00_0025b750(camera_placement_heading, heading_blend_target, ease_5d48(1.0f, 0.0f, 1.0f, 0.0f, st->f10));
            st->f8 = current_key_heading_override;
            st->fC = heading_blend_target;
        } else {
            st->f10 += (0.0f - st->f10) * D_L14_001624A4;
            camera_placement_heading = FUN_L00_0025b750(camera_placement_heading, current_key_heading_override, ease_5d48(1.0f, 0.0f, 1.0f, 0.0f, st->f10));
            st->f8 = current_key_heading_override;
            st->fC = heading_blend_target;
        }
    }
    camera_placement_parameters[1] = cur->f4;
    camera_placement_parameters[0] = camera_placement_heading;
    camera_placement_parameters[2] = cur->f8;
    FUN_L00_001ed580_5d48(cam + 0x30, camera_placement_parameters, &hero.motion.pos);
    vec_sub_5d48(look_direction, &hero.motion.pos, cam + 0x30);
    vec_scale_5d48(scaled_up_vector, &hero.gravity_dir, -1.5f);
    vec_add_5d48(look_direction, look_direction, scaled_up_vector);
    vec_normalize_5d48(cam, look_direction, 1.0f);
    vec_cross_5d48(cam + 0x10, cam, scaled_up_vector);
    if (cur->fC != 0.0f) {
        rotate_axis_5d48(cam, cam, cam + 0x10, cur->fC);
        vec_normalize_5d48(cam, cam, 1.0f);
        vec_cross_5d48(cam + 0x10, cam, scaled_up_vector);
    }
    if (cur->f10 != 0.0f) {
        rotate_axis_5d48(cam, cam, scaled_up_vector, cur->f10);
        vec_normalize_5d48(cam, cam, 1.0f);
        vec_cross_5d48(cam + 0x10, cam, scaled_up_vector);
    }
    vec_normalize_5d48(cam + 0x10, cam + 0x10, 1.0f);
    vec_cross_5d48(cam + 0x20, cam + 0x10, cam);
    st->count++;
}
/* Tells whether the vendor slot still blocks the menu (1) or lets it through (0). */

extern char *D_L14_0015EF50;
extern u8 *D_L14_00167500 __attribute__((section(".data")));

int FUN_L14_00316748(char *moby, char *other) {
    char *slot_record = *(char **)(D_L14_0015EF50 + *(short *)(moby + 0x84) * 32 + 0x1C);
    if (*(int *)(slot_record + 0x20) == 0 && *(short *)(D_L14_00167500 + 0x86) == 0x14) {
        *(int *)(slot_record + 0x20) = 1;
    } else if (*(int *)(slot_record + 0x20) == 1) {
        if (*(short *)(D_L14_00167500 + 0x86) != 0x14)
            return 1;
        if (other == 0 || *(short *)(other + 0x7E) != 0)
            return 1;
    }
    return 0;
}
