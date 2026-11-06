#include "rnc/mobys/moby_class_tables.h"
#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/game/game_globals.h"

typedef struct {
    char          pad00[0x10];
    unsigned char nframes; /* 0x10 */
    signed char   sound;   /* 0x11 */
} MobyISeq;

typedef struct {
    char           pad00[6];
    unsigned char  metal_count; /* 0x06 */
    char           pad07[5];
    unsigned char  seq_count; /* 0x0C */
    char           pad0D;
    unsigned char  lod_trans;    /* 0x0E */
    unsigned char  shadow_count; /* 0x0F */
    int            collision;    /* 0x10 */
    char           pad14[0x10];
    float          scale; /* 0x24 */
    char           pad28[0x18];
    int            glow_rgba; /* 0x40 */
    unsigned short flags;     /* 0x44 */
    char           pad46[2];
    MobyISeq      *seq; /* 0x48 */
} MobyIClass;

typedef struct {
    char           pad00[0x21];
    unsigned char  group;  /* 0x21 */
    unsigned char  oClass; /* 0x22 */
    unsigned char  alpha;  /* 0x23 */
    MobyIClass    *pClass; /* 0x24 */
    char           pad28[4];
    float          scale; /* 0x2C */
    char           pad30[4];
    unsigned short flags;     /* 0x34 */
    unsigned short occlusion; /* 0x36 */
    unsigned long  lights;    /* 0x38 */
    char           pad40[0x18];
    float          anim_speed; /* 0x58 */
    float          unk5C;      /* 0x5C */
    char           pad60[0x11];
    unsigned char  unk71;       /* 0x71 */
    unsigned char  lod_dist;    /* 0x72 */
    unsigned char  metal_alpha; /* 0x73 */
    int            update;      /* 0x74 */
    char           pad78[4];
    unsigned char  seq_sound;          /* 0x7C */
    unsigned char  sound_slot;         /* 0x7D */
    unsigned char  anim_trigger_count; /* 0x7E */
    unsigned char  shadow_range;       /* 0x7F */
    char           pad80[4];
    int            unk84; /* 0x84 */
    int            unk88; /* 0x88 */
    char           pad8C[4];
    int            glow_color; /* 0x90 */
    int            collision;  /* 0x94 */
    char           pad98[8];
    unsigned char  unkA0;    /* 0xA0 */
    unsigned char  unkA1;    /* 0xA1 */
    unsigned char  unkA2;    /* 0xA2 */
    unsigned char  unkA3;    /* 0xA3 */
    unsigned char  hit_slot; /* 0xA4 */
    char           padA5;
    short          original_class_id; /* 0xA6 */
    int            unkA8;             /* 0xA8 */
    int            unkAC;             /* 0xAC */
    char           padB0[0xD];
    unsigned char  unkBD; /* 0xBD */
    char           padBE[0x42];
} MobyI;

extern void fill_transfer_words() __asm__("func_001F97E8");
extern void update_moby_animation_state(void *) __asm__("func_0020C880");

/* InitMobyInstance: clears the 0x100-byte moby, fills its defaults (class
   byte from D_001B3AC0[oClass], colours, its slot index from the moby
   array base D_0015FF18), flags a class with no moby_class_updates entry, then
   copies the class record D_001B3200[class] (or marks the moby dead when
   there is none) and applies the animation-sequence rules after
   update_moby_animation_state. The moby is a real struct so its non-byte stores are "in
   struct" and D_0015FF18's load can move above them; the class load comes
   first, and the default stores are ordered so that sched1's
   register-pressure tie-break (stores that free a register go first)
   reproduces retail's store order. */
void init_moby_instance(void *moby_mem, int oClass) __asm__("FUN_0020c5f0");

void init_moby_instance(void *moby_mem, int oClass) {
    MobyI        *moby_instance = (MobyI *)moby_mem;
    unsigned char resident_class_slot;
    int           resident_pool_slot_index;
    MobyIClass   *pClass;

    fill_transfer_words(moby_instance, 0, 0x100);
    resident_class_slot = resident_class_slot_by_id[oClass];
    moby_instance->alpha = 0x80;
    moby_instance->oClass = resident_class_slot;
    moby_instance->hit_slot = 0xFF;
    moby_instance->group = 0xFF;
    moby_instance->unk71 = 0xFF;
    moby_instance->lod_dist = 0xFF;
    moby_instance->original_class_id = oClass;
    moby_instance->lights = 0x40404000000000L;
    moby_instance->occlusion = 0x7F80;
    resident_pool_slot_index = ((char *)moby_instance - (char *)resident_object_pool) >> 8;
    moby_instance->unkA8 = resident_pool_slot_index << 16;
    moby_instance->unkAC = resident_pool_slot_index;
    moby_instance->anim_trigger_count = 0;
    moby_instance->seq_sound = 0xFF;
    moby_instance->unkA0 = 0x7F;
    moby_instance->unkA2 = 0x80;
    moby_instance->sound_slot = 0xFF;
    moby_instance->unkA1 = 0x7F;
    moby_instance->unkA3 = 0x80;
    moby_instance->update = moby_class_updates[moby_instance->oClass];
    if (moby_instance->update == 0) {
        moby_instance->flags |= 2;
    }
    pClass = moby_class_resources[moby_instance->oClass];
    if (pClass != 0) {
        MobyIClass *class_resource;

        moby_instance->pClass = pClass;
        moby_instance->lod_dist = pClass->lod_trans;
        moby_instance->flags |= pClass->flags;
        moby_instance->collision = pClass->collision;
        moby_instance->scale = pClass->scale;
        moby_instance->anim_speed = 1.0f;
        moby_instance->unk5C = 1.0f;
        if (pClass->glow_rgba != 0) {
            moby_instance->flags |= 0x10;
            moby_instance->glow_color = pClass->glow_rgba;
        }
        {
            /* retail re-reads m->pClass (lw v0, 0x24(s1)) before each of the
               three tests below: the stores in between go to the same struct,
               so cc1 must not carry the pointer across them. Reading the field
               through a volatile copy of the pointer is what keeps every read
               a real load; the stores still go through the plain m. */
            volatile MobyI *vm = moby_instance;

            if (vm->pClass->shadow_count != 0) {
                moby_instance->shadow_range = 0x18;
                moby_instance->flags |= 0x400;
                moby_instance->unk84 = 0;
                moby_instance->unk88 = 0;
                moby_instance->unkBD = 0;
            }
            if (vm->pClass->metal_count != 0) {
                moby_instance->metal_alpha = 0x18;
            }
            if (vm->pClass->seq == 0) {
                return;
            }
        }
        update_moby_animation_state(moby_instance);
        if (moby_instance->pClass->seq->nframes >= 2) {
            moby_instance->flags &= 0xFFFD;
        }
        class_resource = moby_instance->pClass;
        if (class_resource->seq_count == 1) {
            if (class_resource->seq->nframes < 2) {
                moby_instance->anim_speed = 0.0f;
                if (class_resource->seq->sound < 0) {
                    moby_instance->flags |= 0x40;
                }
            }
        }
        return;
    }
    moby_instance->pClass = 0;
    moby_instance->flags |= 5;
    moby_instance->collision = 0;
}

extern __typeof__(init_moby_instance) func_0020C5F0 __attribute__((alias("FUN_0020c5f0")));
