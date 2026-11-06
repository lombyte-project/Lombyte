/* Ported from rac1-decomp (src/game/loaders.c, func_00204FC0). */

#include "sda.h"
#include "rnc/rendering/level_render_state.h"
#include "rnc/hero/hero.h"
#include "types.h"
#include "rnc/mobys/moby_spawn.h"

extern void FlushCache(int);

struct CutsceneMobyClass {
    unsigned char pad00[6];
    unsigned char metal_count; /* 0x06 */
    unsigned char pad07[5];
    unsigned char seq_count; /* 0x0C */
    unsigned char pad0D[0x3B];
    int           seqs[1]; /* 0x48 */
};

struct LightsSrc {
    unsigned char pad00[0x38];
    unsigned long lights; /* 0x38 */
};

struct Moby {
    unsigned char             pad00[0x24];
    struct CutsceneMobyClass *pclass; /* 0x24 */
    unsigned char             pad28[0xA];
    unsigned short            draw_dist; /* 0x32 */
    unsigned short            flags;     /* 0x34 */
    unsigned char             pad36[2];
    unsigned long             lights; /* 0x38 */
    unsigned char             pad40[8];
    int                       attach; /* 0x48 */
    unsigned char             pad4C[6];
    unsigned char             seq;      /* 0x52 */
    unsigned char             prev_seq; /* 0x53 */
    unsigned char             pad54[0x1E];
    unsigned char             lod_dist;    /* 0x72 */
    unsigned char             metal_alpha; /* 0x73 */
    unsigned char             pad74[4];
    int                       pvars; /* 0x78 */
    unsigned char             pad7C[0x18];
    int                       collision; /* 0x94 */
};

struct ModelRec {
    unsigned short end_time; /* 0x00 */
    unsigned char  pad02[2];
    int            subtitles_off; /* 0x04: subtitle table, present when >= 0x400 */
    unsigned short pad08;
    unsigned char  pad0A[2];
    unsigned short num_parts; /* 0x0C */
    unsigned char  pad0E[2];
    int            keys_off;    /* 0x10: camera keys */
    int            part_off[1]; /* 0x14 */
};

struct TransferState {
    unsigned char    pad00[0x38];
    int              frame; /* 0x38: chunk tick */
    unsigned char    pad3C[4];
    unsigned short   end; /* 0x40: end tick (record +0x00) */
    unsigned char    pad42[2];
    short            num_parts; /* 0x44 */
    unsigned char    pad46[2];
    unsigned short   unk48; /* 0x48 */
    unsigned char    pad4A[2];
    int              subs;       /* 0x4C: subtitle table (draw_subtitles) */
    int              unk50;      /* 0x50 */
    int              frames;     /* 0x54: camera keys */
    struct ModelRec *rec;        /* 0x58 */
    int              source_end; /* 0x5C: compressed chunk */
    unsigned char    pad60[0x118];
    struct Moby     *slots[1]; /* 0x178 */
};

extern struct TransferState D_0018CB20_lui __asm__("D_0018CB20") NOT_SDA;
extern int                  cutscene_moby_ids_by_content_variant[] __asm__("D_00160488");
extern int                  decompress_wad(int, int) __asm__("FUN_0020b618");
void                        update_world_object_animation(void *unused_arg) __asm__("FUN_00204790");

extern s32 game_mode_macro __asm__("D_0015F604") MACRO_ADDR;

void update_world_object_animation(void *unused_arg) {
    struct ModelRec *rec;
    struct Moby     *mob;
    int             *cp;
    int             *sp;
    int              i;
    int              k;
    int              idx;
    int              id;
    int              part_record_offset;
    int              endp;
    int              pc;

    FlushCache(0);
    decompress_wad(D_0018CB20_lui.source_end, (int)D_0018CB20_lui.rec);
    FlushCache(0);

    rec = D_0018CB20_lui.rec;
    D_0018CB20_lui.frame = 0;
    cp = rec->part_off;
    D_0018CB20_lui.end = rec->end_time;
    pc = rec->subtitles_off;
    D_0018CB20_lui.unk48 = rec->pad08;
    D_0018CB20_lui.num_parts = rec->num_parts;
    D_0018CB20_lui.frames = (int)rec + rec->keys_off;
    if (pc < 0x400) {
        D_0018CB20_lui.subs = 0;
    } else {
        D_0018CB20_lui.subs = (int)rec + pc;
    }

    for (i = 0; i < D_0018CB20_lui.num_parts; i++) {
        part_record_offset = *cp++;
        sp = (int *)((unsigned char *)rec + part_record_offset);
        id = sp[0];
        sp = (int *)((unsigned char *)sp + 0xC);
        endp = (int)rec + sp[0];
        sp = (int *)((unsigned char *)sp + 4);
        if (game_mode_macro == 6 && i == 0 && id == 0x215) {
            id = cutscene_moby_ids_by_content_variant[level_render_state.content_variant];
        }
        mob = D_0018CB20_lui.slots[i];
        if (mob == 0) {
            mob = create_moby(id);
            idx = mob->pclass->seq_count;
            mob->pclass->seq_count = idx + 1;
            mob->seq = idx;
            mob->prev_seq = idx;
            mob->draw_dist = 0x1FF;
            mob->flags |= 6;
            mob->lod_dist = 0xFF;
            mob->collision = 0;
            if (hero.moby == 0) {
                mob->lights = 0x38383800000000;
            } else {
                mob->lights = ((struct LightsSrc *)hero.moby)->lights;
            }
            if (mob->pclass->metal_count) {
                mob->metal_alpha = 0x18;
            }
            D_0018CB20_lui.slots[i] = mob;
        }
        mob->pvars = endp;
        mob->pclass->seqs[mob->seq] = sp;
        for (k = 0; k < ((unsigned char *)sp)[0x10]; k++) {
            int *pointer_fixup_entries = (int *)((unsigned char *)sp + 0x1C);

            pointer_fixup_entries[k] = (int)sp + pointer_fixup_entries[k];
        }
    }
}

extern __typeof__(update_world_object_animation) func_00204790
    __attribute__((alias("FUN_00204790")));

/* The moby class that takes the place of class 0x215 as the first part in game mode 6, per
   content variant (0, 1 or 2). Small data in retail, defined after the function so it keeps
   reaching the table through lui. */
int cutscene_moby_ids_by_content_variant[3]
    __attribute__((section(".sdata"))) = {0x213, 0x214, 0x215};
