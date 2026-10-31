/* Moby animation slots: allocation and release, animation blends and frame packets, and class point offsets. */
#include "types.h"
#include "asm.h"

#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"

typedef struct {
    float m[16];
} __attribute__((aligned(16))) Mtx;

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) Vec4;

typedef struct {
    float a;
    float b;
    float pad[2];
    Vec4  v;
} Ent32;

typedef struct {
    char  h[0x10];
    Ent32 e[1];
} Tbl;

void set_moby_local_table_entry(char *p, int idx, void *v, float a, float b) __asm__("FUN_L00_0024eed8");
void set_moby_local_table_entry(

    char *moby, int table_entry_index, void *v, float unscaled_entry_w, float b) {
    Mtx    transposed_rotation;
    Vec4   local_offset;
    Ent32 *e = &(*(Tbl **)(moby + 0x94))->e[table_entry_index];
    float  k = 1024.0f / *(float *)(moby + 0x2C);
    transpose_rotation_matrix(&transposed_rotation, moby + 0xC0);
    fast_vec_sub(&local_offset, v, moby + 0x10);
    transform_vector(&local_offset, &local_offset, &transposed_rotation);
    fast_vec_scale(&local_offset, &local_offset, k);
    qcopy(&e->v, &local_offset);
    e->b = b * k;
    e->v.w = unscaled_entry_w * k;
}

/* Ported from rac1-decomp (src/overlays/shared/mobyfunc_0024FD50.c: func_L00_0024FFE8), where it is exact; names translated to the US level program. */

/* Offsets the point pairs of each of the a[4] + a[5] entries by dx, dy. */
void offset_moby_class_point_pairs(unsigned char *a, int dx, int dy) __asm__("FUN_L00_0024efb0");

void offset_moby_class_point_pairs(unsigned char *moby_class, int dx, int dy) {
    int  i;
    int  entry_count = moby_class[4] + moby_class[5];
    int *entry_cursor = *(int **)moby_class;
    for (i = 0; i < entry_count; i++, entry_cursor += 4) {
        unsigned short *point_pair_cursor = *(unsigned short **)entry_cursor;
        int             point_pair_count = ((unsigned char *)point_pair_cursor)[2];
        int             remaining_point_pair_count;
        point_pair_cursor += 2;
        if (point_pair_count != 0) {
            remaining_point_pair_count = point_pair_count;
            do {
                point_pair_cursor[0] = point_pair_cursor[0] + dx;
                point_pair_cursor[1] = point_pair_cursor[1] + dy;
                point_pair_cursor += 2;
                remaining_point_pair_count--;
            } while (remaining_point_pair_count != 0);
        }
    }
}

/* Ported from rac1-decomp (src/overlays/shared/mobyfunc_0024FD50.c: func_L00_00250060), where it is exact; names translated to the US level program. */

typedef struct Slot {
    short         id;
    short         s2;
    unsigned char used;
    char          pad5[7];
    void         *p0C;
    void         *p10;
    int           p14;
    char          pad18[4];
    void         *p1C;
    char          pad20[0x20];
} Slot;

extern Slot D_L00_0018EB40[];
/* One 0x1500-byte buffer per slot of D_L00_0018EB40, at each level's own address. */
typedef u8 SlotBuffer[0x1500];
SlotBuffer slot_buffers[6] __asm__("D_L00_00186D40") = {{0}};

SlotBuffer slot_buffers_l01[6] __asm__("D_L01_001871C0") = {{0}};

SlotBuffer slot_buffers_l02[6] __asm__("D_L02_00187340") = {{0}};

SlotBuffer slot_buffers_l03[6] __asm__("D_L03_00186E40") = {{0}};

SlotBuffer slot_buffers_l04[6] __asm__("D_L04_00186EC0") = {{0}};

SlotBuffer slot_buffers_l05[6] __asm__("D_L05_001871C0") = {{0}};

SlotBuffer slot_buffers_l06[6] __asm__("D_L06_001874C0") = {{0}};

SlotBuffer slot_buffers_l07[6] __asm__("D_L07_00186DC0") = {{0}};

SlotBuffer slot_buffers_l08[6] __asm__("D_L08_00187540") = {{0}};

SlotBuffer slot_buffers_l09[6] __asm__("D_L09_00186EC0") = {{0}};

SlotBuffer slot_buffers_l10[6] __asm__("D_L10_001871C0") = {{0}};

SlotBuffer slot_buffers_l11[6] __asm__("D_L11_00187740") = {{0}};

SlotBuffer slot_buffers_l12[6] __asm__("D_L12_001871C0") = {{0}};

SlotBuffer slot_buffers_l13[6] __asm__("D_L13_00187040") = {{0}};

SlotBuffer slot_buffers_l14[6] __asm__("D_L14_00187440") = {{0}};

SlotBuffer slot_buffers_l15[6] __asm__("D_L15_00187340") = {{0}};

SlotBuffer slot_buffers_l16[6] __asm__("D_L16_001871C0") = {{0}};

SlotBuffer slot_buffers_l17[6] __asm__("D_L17_00187640") = {{0}};

SlotBuffer slot_buffers_l18[6] __asm__("D_L18_00187740") = {{0}};

Slot *allocate_moby_animation_slot(char *owner, int id) __asm__("FUN_L00_0024f028");

Slot *allocate_moby_animation_slot(char *slot_owner_moby, int id) {
    int            i = 0;
    Slot          *selected_slot;
    unsigned char *selected_record;
    for (i = 0; i < 6 && D_L00_0018EB40[i].used != 0; i++) {
    }
    selected_slot = &D_L00_0018EB40[i];
    selected_slot->used = 1;
    selected_slot->id = id;
    id = (short)id;
    selected_slot->p10 = slot_buffers[i];
    selected_slot->p14 = *(int *)(slot_owner_moby + 0x24);
    selected_record =
        *(unsigned char **)(*(char **)(*(char **)(slot_owner_moby + 0x24) + 0x1C) + id * 4 + 4);
    selected_slot->s2 = selected_record[2];
    selected_slot->p0C = selected_record + (selected_record[0] + 4);
    selected_slot->p1C = *(void **)(slot_owner_moby + 0x60);
    *(Slot **)(slot_owner_moby + 0x60) = selected_slot;
    return selected_slot;
}

/* FUN_L00_0024f0e8: unlinks *slot from its owner's singly linked list
 * (head at owner+0x60, link at node+0x1C), frees the 0x40-byte node and
 * clears *slot. */
typedef struct ListNode ListNode;

struct ListNode {
    u8        pad0[0x1C];
    ListNode *next; /* 0x1C */
};

typedef struct {
    u8        pad0[0x60];
    ListNode *head; /* 0x60 */
} ListOwner;

extern void fill_transfer_words(void *, s32, s32) __asm__("FUN_001f97e8");

void release_moby_animation_slot(ListOwner *owner, ListNode **slot) __asm__("FUN_L00_0024f0e8");

void release_moby_animation_slot(ListOwner *moby_animation_owner, ListNode **slot) {
    ListNode *list_cursor;

    if (*slot == 0)
        return;
    if (moby_animation_owner->head == *slot) {
        moby_animation_owner->head = (*slot)->next;
    } else {
        list_cursor = moby_animation_owner->head;
        while (list_cursor->next != 0 && list_cursor->next != *slot)
            list_cursor = list_cursor->next;
        if (list_cursor->next == *slot)
            list_cursor->next = (*slot)->next;
    }
    fill_transfer_words(*slot, 0, 0x40);
    *slot = 0;
}

/* advances an animation blend timer and steps through its frame table */
/* Ported from rac1-decomp (src/overlays/shared/mobyfunc_0024FD50.c: func_L00_002501C8), where it is exact; names translated to the US level program. */

extern void FUN_L00_0024f440(void *, int, int, int);
extern void FUN_L00_00252990(void *);

void advance_moby_animation_blend(void *unused, unsigned char *a) __asm__("FUN_L00_0024f190");

void advance_moby_animation_blend(void *unused, unsigned char *animation_state) {
    float unscaled_frame_step = *(float *)(animation_state + 0x28);
    animation_state[5] = 0;
    if (unscaled_frame_step != 0.0f && *(float *)(animation_state + 0x2C) != 0.0f) {
        float advanced_frame_progress = *(float *)(animation_state + 0x24) + unscaled_frame_step * *(float *)(animation_state + 0x2C);
        *(float *)(animation_state + 0x24) = advanced_frame_progress;
        if (unscaled_frame_step > 0.0f) {
            if (advanced_frame_progress > 0.99f && advanced_frame_progress < 1.01f) {
                *(float *)(animation_state + 0x24) = 1.0f;
            }
            if (*(float *)(animation_state + 0x24) >= 1.0f) {
                int next_frame_remap_flag = animation_state[7];
                do {
                    int o, p, animation_table_byte_offset;
                    animation_state[6] = next_frame_remap_flag;
                    animation_state[5] |= 1;
                    animation_state[0x20] = animation_state[0x21];
                    if ((unsigned char)next_frame_remap_flag) {
                        *(int *)(animation_state + 0x38) = *(int *)(animation_state + 0x3C);
                    }
                    animation_state[0x22] = animation_state[0x23];
                    animation_state[0x21]++;
                    o = animation_state[0x23] * 4;
                    if (animation_state[0x21] >=
                        *(unsigned char *)(*(int *)(*(unsigned char **)(animation_state + 0x14) + o + 0x48) +
                                           0x10)) {
                        animation_state[0x21] = 0;
                        animation_state[5] |= 2;
                    }
                    {
                        float unscaled_frame_remainder = (*(float *)(animation_state + 0x24) - 1.0f) / *(float *)(animation_state + 0x2C);
                        float frame_progress_scale;
                        animation_table_byte_offset = animation_state[0x22] * 4;
                        p = animation_state[0x20] * 4;
                        *(float *)(animation_state + 0x24) = unscaled_frame_remainder;
                        frame_progress_scale = **(float **)(*(int *)(*(unsigned char **)(animation_state + 0x14) + animation_table_byte_offset + 0x48) + p +
                                         0x1C);
                        *(float *)(animation_state + 0x2C) = frame_progress_scale;
                        *(float *)(animation_state + 0x24) = unscaled_frame_remainder * frame_progress_scale;
                    }
                } while (*(float *)(animation_state + 0x24) >= 1.0f);
            }
        }
    }
    if (animation_state[6]) {
        FUN_L00_0024f440(animation_state, animation_state[0x22], animation_state[0x20], *(int *)(animation_state + 0x38));
    } else {
        int o = animation_state[0x22] * 4;
        int p = animation_state[0x20] * 4;
        *(int *)(animation_state + 0x38) =
            *(int *)(*(int *)(*(unsigned char **)(animation_state + 0x14) + o + 0x48) + p + 0x1C);
    }
    if (animation_state[7]) {
        FUN_L00_0024f440(animation_state, animation_state[0x23], animation_state[0x21], *(int *)(animation_state + 0x3C));
    } else {
        int o = animation_state[0x23] * 4;
        int p = animation_state[0x21] * 4;
        *(int *)(animation_state + 0x3C) =
            *(int *)(*(int *)(*(unsigned char **)(animation_state + 0x14) + o + 0x48) + p + 0x1C);
    }
    FUN_L00_00252990(animation_state);
}

typedef struct {
    u8  pad[0x20];
    u8  b20, b21, b22, b23;
    s32 i24;
    f32 f28;
    f32 f2c;
} S;

typedef struct {
    u8 pad[0x51];
    u8 b51, b52, b53;
} A;

/* advance_moby_animation_blend called without arguments: it receives the caller's own a0 and a1. */
int advance_moby_animation_blend_passthrough() __asm__("FUN_L00_0024f190");

/* Fills a1 from a0's bytes, a2/a3 and a 1/a4 step; calls advance_moby_animation_blend when a5 is set. */
void initialize_moby_animation_blend(A  *source,
                                     S  *destination,
                                     s32 target_animation_index,
                                     s32 target_frame_index,
                                     s32 step_divisor,
                                     s32 update_immediately) __asm__("FUN_L00_0024f3e0");

void initialize_moby_animation_blend(A  *source,
                                     S  *destination,
                                     s32 target_animation_index,
                                     s32 target_frame_index,
                                     s32 step_divisor,

                                     s32 update_immediately) {
    destination->b22 = source->b53;
    destination->b20 = source->b51;
    destination->b23 = target_animation_index;
    destination->b21 = target_frame_index;
    destination->f28 = 1.0f;
    destination->f2c = 1.0f / (f32)step_divisor;
    destination->i24 = 0;
    if (update_immediately)
        advance_moby_animation_blend_passthrough();
}

/* FUN_L00_0024f440: builds an animation frame packet in out from frame
 * (table i, frame j) of the moby's animation set: header halfwords, the
 * frame's packet body, then the per-joint quadwords scattered by the
 * index table, and makes sure the terminating quad is non-zero.
 * (Field roles are inferred from the code, not confirmed.) */
typedef unsigned long u64_24f440;

typedef struct {
    char       pad0[6];
    short      qwc; /* 0x06: size in quadwords */
    char       pad8[2];
    short      hA; /* 0x0A */
    char       padC[2];
    short      hE;   /* 0x0E */
    u64_24f440 q[1]; /* 0x10 */
} Frame_24f440;

typedef struct {
    char       pad0[6];
    short      h6;   /* 0x06 */
    short      h8;   /* 0x08 */
    short      hA;   /* 0x0A */
    short      hC;   /* 0x0C */
    short      hE;   /* 0x0E */
    u64_24f440 q[1]; /* 0x10 */
} Packet;

extern void flush_cache(int) __asm__("FUN_00118a80");

/* Defined under a local name: this unit already declares FUN_L00_0024f440
 * with a generic prototype for its callers. */
void FUN_L00_0024f440_c(char *obj, int i, int j, Packet *out) __asm__("FUN_L00_0024f440");

void FUN_L00_0024f440_c(char *animation_blend_state, int i, int j, Packet *animation_frame_packet) {
    char          *frame_table_container = *(char **)(animation_blend_state + 0x18);
    char          *output_layout_source = *(char **)(animation_blend_state + 0x14);
    int            nb = (unsigned char)frame_table_container[8];
    int            na = (unsigned char)output_layout_source[8];
    Frame_24f440  *selected_frame;
    unsigned short frame_header_a;
    int            trailing_byte_count;
    int            k;
    unsigned char *ix;
    u64_24f440    *scatter_source_cursor;
    u64_24f440    *q;

    {
        char         **tt = (char **)(frame_table_container + 0x48);
        char          *r = tt[i];
        Frame_24f440 **ss = (Frame_24f440 **)(r + 0x1C);
        selected_frame = ss[j];
    }
    fast_mem_zero16(animation_frame_packet, ((na + 3) * 8) & 0xFF0);
    k = na + selected_frame->hA;
    k += selected_frame->hE;
    animation_frame_packet->h8 = na * 8;
    {
        void *from = &((u64_24f440 *)selected_frame)[nb + 2];

        animation_frame_packet->h6 = (k + 1) >> 1;
        frame_header_a = selected_frame->hA;
        animation_frame_packet->hA = frame_header_a;
        animation_frame_packet->hC = (frame_header_a + na) * 8;
        animation_frame_packet->hE = selected_frame->hE;
        trailing_byte_count = selected_frame->qwc * 16 - nb * 8;
        if (trailing_byte_count != 0)
            copy_memory_aligned(
                &((u64_24f440 *)animation_frame_packet)[na + 2], from, trailing_byte_count);
    }
    ix = (unsigned char *)(*(char **)(*(char **)(animation_blend_state + 0x18) + 0x1C) + 4);
    scatter_source_cursor = selected_frame->q;
    if (nb != 0) {
        int remaining_scatter_count = nb;
        do {
            remaining_scatter_count--;
            ((u64_24f440 *)animation_frame_packet)[*ix + 2] = *scatter_source_cursor;
            ix++;
            scatter_source_cursor++;
        } while (remaining_scatter_count != 0);
    }
    q = (u64_24f440 *)((char *)animation_frame_packet + animation_frame_packet->h6 * 16);
    if (*q == 0)
        *q = 1;
    flush_cache(0);
}

Slot D_L01_0018EFC0 = {0};

Slot D_L02_0018F140 = {0};

Slot D_L03_0018EC40 = {0};

Slot D_L04_0018ECC0 = {0};

Slot D_L05_0018EFC0 = {0};

Slot D_L06_0018F2C0 = {0};

Slot D_L07_0018EBC0 = {0};

Slot D_L08_0018F340 = {0};

Slot D_L09_0018ECC0 = {0};

Slot D_L10_0018EFC0 = {0};

Slot D_L11_0018F540 = {0};

Slot D_L12_0018EFC0 = {0};

Slot D_L13_0018EE40 = {0};

Slot D_L14_0018F240 = {0};

Slot D_L15_0018F140 = {0};

Slot D_L16_0018EFC0 = {0};

Slot D_L17_0018F440 = {0};

Slot D_L18_0018F540 = {0};
