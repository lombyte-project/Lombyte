/* Ported from rac1-decomp (src/game/mobyutil.c, func_00213D28). */
typedef struct {
    char _pad00[0x10];
    unsigned char nframes; /* 0x10 */
} AnimSeq;
typedef struct {
    char _pad00[0x48];
    AnimSeq *seqs[1]; /* 0x48 */
} AnimClass;
typedef struct {
    char _pad00[0x24];
    AnimClass *pClass;       /* 0x24 */
    char _pad28[0x50 - 0x28];
    unsigned char frame;     /* 0x50 */
    unsigned char nextFrame; /* 0x51 */
    unsigned char seq;       /* 0x52 */
    unsigned char prevSeq;   /* 0x53 */
    char _pad54[0x5C - 0x54];
    float unk5C;             /* 0x5C */
    char _pad60[0x68 - 0x60];
    float *frameData;        /* 0x68 */
    char _pad6C[4];
    unsigned char unk70;     /* 0x70 */
} MobyAnim;
extern void update_moby_animation_state(void *) __asm__("func_0020C880");
/* Sets moby m's animation to sequence seq at frame (clamped to the
   sequence's last frame), the next frame to frame + 1 (clamped the same
   way, 0 if still out of range), refreshes the frame pointers
   (func_0020C880) and copies the first float of the new frame to +0x5C.
   The frame count is re-read through the class at each test, as retail
   reloads it after the byte stores. */
void set_moby_animation(MobyAnim *m, int seq, int frame) __asm__("FUN_00212ed8");

void set_moby_animation(MobyAnim *m, int seq, int frame) {
    int n = m->pClass->seqs[seq]->nframes;

    m->seq = seq;
    if (frame >= n) {
        frame = n - 1;
    }
    m->frame = frame;
    m->nextFrame = frame + 1;
    if (m->nextFrame > m->pClass->seqs[seq]->nframes - 1) {
        m->nextFrame = m->pClass->seqs[seq]->nframes - 1;
    }
    m->prevSeq = seq;
    if (m->nextFrame >= m->pClass->seqs[seq]->nframes) {
        m->nextFrame = 0;
    }
    update_moby_animation_state(m);
    m->unk5C = *m->frameData;
    m->unk70 &= ~2;
}

extern __typeof__(set_moby_animation) func_00212ED8 __attribute__((alias("FUN_00212ed8")));
