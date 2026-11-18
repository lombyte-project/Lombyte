/* Moby joint matrices for the browser builds, decoded from the EE assembly:

     get_moby_joint_matrices  0x210850 (88 instructions)
     compute_moby_joint_slots 0x2109b8 (659 instructions, ends 0x211400; the name is ours)

   The game page's level.wasm (browser/level.c) and the spike (browser/spike/ee-helpers.c) both
   link this file.

   0x2109b8 works in the 16 KiB scratchpad. It DMAs (channel 9, chain mode, tags built at
   0x1b2e00) three blocks to scratchpad 0x2000: the class's joint records (pclass + 0x18, one
   quadword per joint: bind translation xyz and a parent word), the current frame (moby + 0x68)
   and, when anim_time is not 0, the next frame (moby + 0x6C); it waits for each block by polling a
   -1 sentinel in the block's last word. Here the blocks are read in place instead: the DMA only
   copies them. Each joint has a 64-byte slot at scratchpad 64 * joint:
     +0x00 rotation quaternion (xyzw)   +0x0C during unpacking: scale-list link
     +0x10 scale xyz                    +0x1C scale flag (0: none)
     +0x20 translation xyz              +0x2C parent word (0 or 0x70000000 + 64 * parent)
     +0x30 next frame's translation     +0x3C during unpacking: translation-list link
   A frame is a 16-byte header (+0x6 quadword count, +0xA scale record count, +0xE translation
   record count) then one 8-byte packed quaternion per joint, then 8-byte records
   { x, y, z, flags } whose flags byte 6 is the joint; the MMI unpack is pextlh/pextuh then
   psraw 16 (signed) or psrlw 13 (scales: unsigned, so value = u16 / 4096 after vitof15).
   The rest (list blending, the two quaternion paths, parked poses at +0x60, manipulators at +0x64,
   matrices) is commented where it happens. Scratchpad addresses held in data (parent words, the
   slot pointers of parked-pose records and manipulators) are taken modulo 16 KiB, which is what
   both 0x70000000 and the spike's remapped 0x000F0000 reduce to.

   The real scratchpad is not written: callers only read the matrices this returns. EE float
   rounding (round toward zero, no denormals) is not reproduced; the last bit can differ. */
#include "types.h"
#include "moby_joint_matrices.h"

#define SPR_BYTES 0x4000
#define SPR_MARK 0x70000000u

static u8 spr[SPR_BYTES] __attribute__((aligned(16)));

static f32 *spr_vec(u32 offset) { return (f32 *)(spr + (offset & (SPR_BYTES - 1))); }
static u32 *spr_word(u32 offset) { return (u32 *)(spr + (offset & (SPR_BYTES - 1))); }
static u32 rd32(const u8 *p) { u32 v; __builtin_memcpy(&v, p, 4); return v; }
static u16 rd16(const u8 *p) { u16 v; __builtin_memcpy(&v, p, 2); return v; }
static s16 rds16(const u8 *p) { s16 v; __builtin_memcpy(&v, p, 2); return v; }
static f32 bits_float(u32 b) { f32 f; __builtin_memcpy(&f, &b, 4); return f; }
static u32 float_bits(f32 f) { u32 b; __builtin_memcpy(&b, &f, 4); return b; }
static const u8 *ptr(u32 address) { return (const u8 *)(unsigned long)address; }
static void set4(f32 *v, f32 x, f32 y, f32 z, f32 w) { v[0] = x; v[1] = y; v[2] = z; v[3] = w; }

/* The VU's dot products: y lane ((p.y + p.x) + p.z) + p.w (vaddax.y, vmaddaz.y, vmaddw.y), and
   sums of squares in the x lane ((q.x^2 + q.y^2) + q.z^2) + q.w^2 (vadday.x, vmaddaz.x, vmaddw.x). */
static f32 dot_y(const f32 *a, const f32 *b) {
    return ((a[1] * b[1] + a[0] * b[0]) + a[2] * b[2]) + a[3] * b[3];
}
static f32 length2_x(const f32 *q) {
    return ((q[0] * q[0] + q[1] * q[1]) + q[2] * q[2]) + q[3] * q[3];
}

/* One packed quaternion: four int16, vitof15. */
static void unpack_rotation(const u8 *p, f32 *q) {
    for (s32 i = 0; i < 4; i++) q[i] = (f32)rds16(p + 2 * i) * (1.0f / 32768.0f);
}

/* Blend two weighted quaternions the long-way-safe way the parked-pose and manipulator paths
   use: a and b are already scaled by their weights; the sum is used unless the dot product of
   a and b is negative (sign bit set), then the difference; the result is normalized (vrsqrt). */
static void blend_weighted(f32 *out, const f32 *a, const f32 *b) {
    f32 v[4];
    f32 inv;
    for (s32 i = 0; i < 4; i++) v[i] = a[i] + b[i];
    if (float_bits(dot_y(a, b)) >> 31) {
        for (s32 i = 0; i < 4; i++) v[i] = a[i] - b[i];
    }
    inv = 1.0f / __builtin_sqrtf(__builtin_fabsf(length2_x(v)));
    for (s32 i = 0; i < 4; i++) out[i] = v[i] * inv;
}

/* 0x2109b8 on the moby at m with the joint flag table (128 bytes; [0x7F] = highest joint + 1). */
void rnc_compute_moby_joint_slots(u8 *m, const u8 *joint_flags) {
    const u8 *pclass = ptr(rd32(m + 0x24));
    const u8 *cur = ptr(rd32(m + 0x68));
    const u8 *next = ptr(rd32(m + 0x6c));
    const s32 joints = pclass[8];
    const u32 time_bits = rd32(m + 0x54);
    const f32 t = bits_float(time_bits), s = 1.0f - t;
    const u8 *joint_records = ptr(rd32(pclass + 0x18));
    const u8 *cur_data = cur + 0x10;
    const u8 *next_data = next + 0x10;
    s32 cur_scales = rds16(cur + 0xa), cur_translations = rds16(cur + 0xe);
    s32 next_scales = rds16(next + 0xa), next_translations = rds16(next + 0xe);
    s32 same_run, n, j;
    const u8 *record;

    if (joints == 0) return;
    for (j = 0; j < joints; j++) {
        set4(spr_vec(64 * j), 1.0f, 1.0f, 1.0f, 0.0f);
        set4(spr_vec(64 * j + 0x10), 1.0f, 1.0f, 1.0f, 0.0f);
    }
    /* 0x210afc: a plain blend when the next frame is this frame or the one after (the frame
       difference & 0xfffe is 0) and no sequence blend runs (pceqw, andi 0x400). */
    same_run = ((((s32)m[0x51] - (s32)m[0x50]) & 0xfffe) + ((s32)m[0x52] - (s32)m[0x53])) == 0;
    /* 0x210b48: each joint record to +0x20 as loaded and to +0x30 with w = w * 0. */
    for (j = 0; j < joints; j++) {
        __builtin_memcpy(spr_vec(64 * j + 0x20), joint_records + 16 * j, 16);
        __builtin_memcpy(spr_vec(64 * j + 0x30), joint_records + 16 * j, 12);
        spr_vec(64 * j + 0x30)[3] = 0.0f;
    }
    *spr_word(0x2000) = 0; /* list heads: scales 0x2000, translations 0x2004 */
    *spr_word(0x2004) = 0;

    if (time_bits != 0) {
        u32 scale_tail = 0x1ff4, translation_tail = 0x1fc8;
        /* Current frame's scale records (flag bit 15 set only) to +0x00, linked through +0x0C. */
        record = cur_data + 8 * joints;
        for (; cur_scales != 0; cur_scales--, record += 8) {
            const u32 slot = 64u * record[6];
            const u16 flags = rd16(record + 6);
            f32 *v;
            if (!(flags & 0x8000)) continue;
            *spr_word(scale_tail + 0xc) = SPR_MARK | slot;
            scale_tail = slot;
            *spr_word(slot + 0x1c) = (u32)((flags >> 14) & 3) - 1;
            v = spr_vec(slot);
            set4(v, rd16(record) * (1.0f / 4096.0f), rd16(record + 2) * (1.0f / 4096.0f),
                 rd16(record + 4) * (1.0f / 4096.0f), 0.0f);
        }
        /* Translation records to +0x20, linked through +0x3C; the parent word is kept. */
        for (; cur_translations != 0; cur_translations--, record += 8) {
            const u32 slot = 64u * record[6];
            const u32 parent = *spr_word(slot + 0x2c);
            *spr_word(translation_tail + 0x3c) = SPR_MARK | slot;
            translation_tail = slot;
            set4(spr_vec(slot + 0x20), (f32)rds16(record), (f32)rds16(record + 2), (f32)rds16(record + 4), 0.0f);
            *spr_word(slot + 0x2c) = parent;
        }
        *spr_word(translation_tail + 0x3c) = 0;
        /* Next frame: scales to +0x10 and translations to +0x30; a slot not yet listed is
           appended (its link word is 0). */
        record = next_data + 8 * joints;
        for (; next_scales != 0; next_scales--, record += 8) {
            const u32 slot = 64u * record[6];
            const u16 flags = rd16(record + 6);
            const u32 link = *spr_word(slot + 0xc);
            if (!(flags & 0x8000)) continue;
            set4(spr_vec(slot + 0x10), rd16(record) * (1.0f / 4096.0f), rd16(record + 2) * (1.0f / 4096.0f),
                 rd16(record + 4) * (1.0f / 4096.0f), 0.0f);
            *spr_word(slot + 0x1c) = (u32)((flags >> 14) & 3) - 1;
            if (link != 0) continue;
            *spr_word(scale_tail + 0xc) = SPR_MARK | slot;
            scale_tail = slot;
            *spr_word(scale_tail + 0xc) = 0;
        }
        for (; next_translations != 0; next_translations--, record += 8) {
            const u32 slot = 64u * record[6];
            const u32 link = *spr_word(slot + 0x3c);
            set4(spr_vec(slot + 0x30), (f32)rds16(record), (f32)rds16(record + 2), (f32)rds16(record + 4), 0.0f);
            *spr_word(slot + 0x3c) = link;
            if (link != 0) continue;
            *spr_word(translation_tail + 0x3c) = SPR_MARK | slot;
            translation_tail = slot;
            *spr_word(translation_tail + 0x3c) = 0;
        }
        /* 0x210d50: scale list, +0x10 = +0x00 * (1 - t) + +0x10 * t (xyz). */
        for (u32 node = *spr_word(0x2000); node != 0;) {
            f32 *a = spr_vec(node), *b = spr_vec(node + 0x10);
            const u32 following = *spr_word(node + 0xc);
            for (s32 i = 0; i < 3; i++) b[i] = a[i] * s + b[i] * t;
            node = following;
        }
        /* 0x210d80: translation list, +0x20 = +0x20 * (1 - t) + +0x30 * t (xyz). */
        for (u32 node = *spr_word(0x2004); node != 0;) {
            f32 *a = spr_vec(node + 0x20), *b = spr_vec(node + 0x30);
            const u32 following = *spr_word(node + 0x3c);
            for (s32 i = 0; i < 3; i++) a[i] = a[i] * s + b[i] * t;
            node = following;
        }
        /* Rotations, two joints per pass up to joint_flags[0x7F]. */
        n = joint_flags[0x7f];
        for (j = 0; n > 0; n -= 2, j += 2) {
            for (s32 k = 0; k < 2; k++) {
                f32 a[4], b[4], q[4];
                f32 *out = spr_vec(64 * (j + k));
                unpack_rotation(cur_data + 8 * (j + k), a);
                unpack_rotation(next_data + 8 * (j + k), b);
                for (s32 i = 0; i < 4; i++) q[i] = a[i] * s + b[i] * t;
                if (!same_run) {
                    /* 0x210dc4: when the dot product's sign bit is set, blend towards -b; then
                       normalize. */
                    if (float_bits(dot_y(a, b)) >> 31) {
                        for (s32 i = 0; i < 4; i++) q[i] = a[i] * s + (-b[i]) * t;
                    }
                    {
                        const f32 inv = 1.0f / __builtin_sqrtf(__builtin_fabsf(length2_x(q)));
                        for (s32 i = 0; i < 4; i++) q[i] *= inv;
                    }
                }
                for (s32 i = 0; i < 4; i++) out[i] = q[i];
            }
        }
    } else {
        /* 0x210f7c: one frame. Scales to +0x10, translations to +0x20 (w 1, then the parent word
           restored; +0x30 = (0, 0, 0, 1)), rotations as stored. */
        record = cur_data + 8 * joints;
        for (; cur_scales != 0; cur_scales--, record += 8) {
            const u32 slot = 64u * record[6];
            const u16 flags = rd16(record + 6);
            if (!(flags & 0x8000)) continue;
            set4(spr_vec(slot + 0x10), rd16(record) * (1.0f / 4096.0f), rd16(record + 2) * (1.0f / 4096.0f),
                 rd16(record + 4) * (1.0f / 4096.0f), 0.0f);
            *spr_word(slot + 0x1c) = (u32)((flags >> 14) & 3) - 1;
        }
        for (; cur_translations != 0; cur_translations--, record += 8) {
            const u32 slot = 64u * record[6];
            const u32 parent = *spr_word(slot + 0x2c);
            set4(spr_vec(slot + 0x30), 0.0f, 0.0f, 0.0f, 1.0f);
            set4(spr_vec(slot + 0x20), (f32)rds16(record), (f32)rds16(record + 2), (f32)rds16(record + 4), 1.0f);
            *spr_word(slot + 0x2c) = parent;
        }
        n = joint_flags[0x7f];
        for (j = 0; n > 0; n -= 2, j += 2) {
            unpack_rotation(cur_data + 8 * j, spr_vec(64 * j));
            unpack_rotation(cur_data + 8 * (j + 1), spr_vec(64 * (j + 1)));
        }
    }

    /* 0x211080: parked poses (moby + 0x60): records { +0x2 count, +0x8 weight, +0x10 address of
       count 48-byte entries, +0x1C next }. Each entry (quaternion, scale + flag word, translation
       + slot address) is blended into its slot with weight w. */
    for (const u8 *park = ptr(rd32(m + 0x60)); park != 0; park = ptr(rd32(park + 0x1c))) {
        const s32 count = rd16(park + 2);
        const f32 w = bits_float(rd32(park + 8)), keep = 1.0f - w;
        const u8 *entry = ptr(rd32(park + 0x10));
        for (s32 e = 0; e < count; e++, entry += 0x30) {
            const u32 slot = rd32(entry + 0x2c);
            f32 *q = spr_vec(slot), *sc = spr_vec(slot + 0x10), *tr = spr_vec(slot + 0x20);
            f32 a[4], b[4];
            for (s32 i = 0; i < 4; i++) {
                a[i] = q[i] * keep;
                b[i] = bits_float(rd32(entry + 4 * i)) * w;
            }
            for (s32 i = 0; i < 3; i++) {
                sc[i] = sc[i] * keep + bits_float(rd32(entry + 0x10 + 4 * i)) * w;
                tr[i] = tr[i] * keep + bits_float(rd32(entry + 0x20 + 4 * i)) * w;
            }
            if (*spr_word(slot + 0x1c) == 0) *spr_word(slot + 0x1c) = rd32(entry + 0x1c);
            blend_weighted(q, a, b);
        }
    }

    /* 0x2111c4: manipulators (moby + 0x64): { +0x3 mode, +0x4 slot address, +0x8 next,
       +0xC weight, +0x10 quaternion, +0x20 scale, +0x30 translation }. Mode 0 composes
       (rotation = slot * manip, scale *= manip, translation += manip); any other mode blends
       towards the manipulator by the weight. The slot's scale flag becomes 1. */
    for (const u8 *manip = ptr(rd32(m + 0x64)); manip != 0; manip = ptr(rd32(manip + 8))) {
        const u32 slot = rd32(manip + 4);
        f32 *q = spr_vec(slot), *sc = spr_vec(slot + 0x10), *tr = spr_vec(slot + 0x20);
        f32 mq[4], ms[3], mt[3];
        for (s32 i = 0; i < 4; i++) mq[i] = bits_float(rd32(manip + 0x10 + 4 * i));
        for (s32 i = 0; i < 3; i++) {
            ms[i] = bits_float(rd32(manip + 0x20 + 4 * i));
            mt[i] = bits_float(rd32(manip + 0x30 + 4 * i));
        }
        if (manip[3] == 0) {
            const f32 a[4] = { q[0], q[1], q[2], q[3] };
            const f32 d = (mq[0] * a[0] + mq[1] * a[1]) + mq[2] * a[2];
            q[0] = (mq[0] * a[3] + a[0] * mq[3]) + (a[1] * mq[2] - mq[1] * a[2]);
            q[1] = (mq[1] * a[3] + a[1] * mq[3]) + (a[2] * mq[0] - mq[2] * a[0]);
            q[2] = (mq[2] * a[3] + a[2] * mq[3]) + (a[0] * mq[1] - mq[0] * a[1]);
            q[3] = mq[3] * a[3] - d;
            for (s32 i = 0; i < 3; i++) {
                sc[i] *= ms[i];
                tr[i] += mt[i];
            }
        } else {
            const f32 w = bits_float(rd32(manip + 0xc)), keep = 1.0f - w;
            f32 a[4], b[4];
            for (s32 i = 0; i < 4; i++) {
                a[i] = q[i] * keep;
                b[i] = mq[i] * w;
            }
            for (s32 i = 0; i < 3; i++) {
                sc[i] = sc[i] * keep + ms[i] * w;
                tr[i] = tr[i] * keep + mt[i] * w;
            }
            blend_weighted(q, a, b);
        }
        *spr_word(slot + 0x1c) = 1;
    }

    /* 0x2112d4: matrices for flagged joints, in order, each in its own slot. Rows from the
       quaternion (2q products), scaled per row when the scale flag is set, translation row
       (t, 1), then multiplied by the parent's matrix (identity for parent word 0):
       out_r = P0 * r.x + P1 * r.y + P2 * r.z (+ P3 for the translation row). */
    n = (s8)joint_flags[0x7f];
    for (j = 0; j < n; j++) {
        f32 *slot = spr_vec(64 * j);
        f32 r[4][4], p[4][4], o[4][4];
        f32 x, y, z, w, x2, y2, z2;
        u32 parent;
        if (joint_flags[j] == 0) continue;
        x = slot[0]; y = slot[1]; z = slot[2]; w = slot[3];
        x2 = x + x; y2 = y + y; z2 = z + z;
        {
            const f32 xw = x2 * w, yw = y2 * w, zw = z2 * w;
            const f32 xx = x2 * x, yx = y2 * x, zx = z2 * x;
            const f32 yy = y2 * y, zy = z2 * y, zz = z2 * z;
            set4(r[0], (1.0f - yy) - zz, yx - zw, zx + yw, 0.0f);
            set4(r[1], (zw + yx), (1.0f - xx) - zz, zy - xw, 0.0f);
            set4(r[2], (-yw + zx), (xw + zy), (1.0f - xx) - yy, 0.0f);
        }
        if (*spr_word(64 * j + 0x1c) != 0) {
            for (s32 i = 0; i < 3; i++) {
                r[0][i] *= slot[4];
                r[1][i] *= slot[5];
                r[2][i] *= slot[6];
            }
        }
        set4(r[3], slot[8], slot[9], slot[10], 1.0f);
        parent = *spr_word(64 * j + 0x2c);
        if (parent == 0) {
            for (s32 a = 0; a < 4; a++)
                for (s32 b = 0; b < 4; b++) p[a][b] = a == b ? 1.0f : 0.0f;
        } else {
            __builtin_memcpy(p, spr_vec(parent), 64);
        }
        for (s32 row = 0; row < 3; row++)
            for (s32 i = 0; i < 4; i++)
                o[row][i] = (p[0][i] * r[row][0] + p[1][i] * r[row][1]) + p[2][i] * r[row][2];
        for (s32 i = 0; i < 4; i++)
            o[3][i] = ((p[0][i] * r[3][0] + p[1][i] * r[3][1]) + p[2][i] * r[3][2]) + p[3][i];
        __builtin_memcpy(slot, o, 64);
    }
}

/* 0x210850: get_moby_joint_matrices(moby, count, bones, out). For each requested bone, the
   class's chain table (pclass + 0x1C, entry 4 * bone + 4 -> { u16 count, _, bytes from +4 }) marks
   every joint on its chain in the flag table (0x1b2d00 on the console); the highest requested
   joint + 1 goes to [0x7F] and 0xFF after it. Then 0x2109b8 runs and each requested bone's
   64-byte matrix is copied to out + 64 * i. */
void rnc_get_moby_joint_matrices(u8 *m, s32 count, const s32 *bones, u8 *out) {
    u8 flags[0x80];
    u8 requested[0x80];
    const u8 *chains = ptr(rd32(ptr(rd32(m + 0x24)) + 0x1c));
    s32 highest = 0;
    __builtin_memset(flags, 0, sizeof(flags));
    for (s32 i = 0; i < count && i < 0x7f; i++) {
        const u8 *chain = ptr(rd32(chains + 4 * bones[i] + 4));
        s32 left = rd16(chain);
        s32 joint = 0;
        for (const u8 *at = chain + 4; ; at++) {
            joint = *at;
            flags[joint & 0x7f] = 1;
            if (--left <= 0) break;
        }
        requested[i] = (u8)joint;
        if (joint > highest) highest = joint;
    }
    flags[0x7f] = (u8)(highest + 1);
    if (highest + 1 < 0x7f) flags[highest + 1] = 0xff;
    rnc_compute_moby_joint_slots(m, flags);
    for (s32 i = 0; i < count && i < 0x7f; i++) __builtin_memcpy(out + 64 * i, spr_vec(64u * requested[i]), 64);
}

/* For the page: every joint's matrix (flags set for joints 0 .. count - 1), copied to out. */
s32 rnc_moby_all_joint_matrices(u8 *m, u8 *out, s32 capacity) {
    u8 flags[0x80];
    const s32 joints = ptr(rd32(m + 0x24))[8];
    if (joints <= 0 || joints > capacity || joints >= 0x7f) return 0;
    __builtin_memset(flags, 0, sizeof(flags));
    for (s32 j = 0; j < joints; j++) flags[j] = 1;
    flags[0x7f] = (u8)joints;
    rnc_compute_moby_joint_slots(m, flags);
    __builtin_memcpy(out, spr, 64u * (u32)joints);
    return joints;
}
