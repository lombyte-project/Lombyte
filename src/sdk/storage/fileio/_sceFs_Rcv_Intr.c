#include "types.h"
#include "rnc/sdk/library/sif_file_slot.h"

typedef struct {
    s32 v;
} __attribute__((packed)) Word;

typedef struct {
    Word ret;
    Word func;
    Word dest;
    Word size;
} FsRcvHead;

typedef struct {
    void *dest;
} __attribute__((packed)) FsRcvPtr;

typedef struct {
    Word dest;
    Word size;
} FsRcvBuf;

typedef struct {
    s32 n1;
    s32 n2;
    u8 *d1;
    u8 *d2;
    u8  buf1[0x40];
    u8  buf2[0x40];
} FsRcvRead;

typedef struct {
    u8 b[0x144];
} __attribute__((packed)) FsDirent;

typedef struct {
    u8 b[0x40];
} __attribute__((packed)) FsStat;

/* Semaphore slot table: read plainly, released through a volatile store (below). */
extern u32 sif_file_completion_bank_index[] __asm__("D_0012FC90");
extern s32 D_0012FC98[];
extern u8  D_00157500[];

extern s32   iSignalSema(s32);
extern void *memcpy(void *, const void *, u32);

void _sceFs_Rcv_Intr(s32 *command_packet) {
    enum {
        FS_COMPLETION_COPY_READ_BUFFERS = 2,
        FS_COMPLETION_COPY_DIRENT = 11,
        FS_COMPLETION_COPY_STAT = 12
    };
    FsRcvHead  completion_header;
    FsRcvHead *packet_header;
    FsRcvBuf  *bs;
    union {
        FsRcvPtr p;
        FsRcvBuf b;
    } u;
    FsRcvRead *read_completion;
    u8        *pkt;
    u8        *completion_buffers_base;
    u8        *read_destination;
    u32       *bank = sif_file_completion_bank_index;
    s32        completion_bank_index;
    s32        i;
    s64        ret;
    u32        copy_byte_count;
    u32        addr;

    completion_bank_index = D_0012FC98[0] != 0 ? command_packet[3] : 0;
    *bank = completion_bank_index;
    completion_buffers_base = D_00157500;
    addr = (u32)completion_buffers_base;
    addr += (u32)completion_bank_index * 0x440;
    pkt = (u8 *)(addr | 0x20000000);
    packet_header = (FsRcvHead *)pkt;
    completion_header.ret = packet_header->ret;
    completion_header.func = packet_header->func;
    completion_header.dest = packet_header->dest;
    completion_header.size = packet_header->size;
    if (completion_header.ret.v >= 0) {
        memcpy((void *)completion_header.dest.v, pkt + 0x10, completion_header.size.v);
    }
    switch (completion_header.func.v) {
    case FS_COMPLETION_COPY_READ_BUFFERS:
        read_completion = (FsRcvRead *)(pkt + 0x14);
        if (read_completion->n1 > 0) {
            read_destination = read_completion->d1;
            for (i = 0; i < read_completion->n1; i++) {
                read_destination[i] = read_completion->buf1[i];
            }
        }
        if (read_completion->n2 > 0) {
            read_destination = read_completion->d2;
            for (i = 0; i < read_completion->n2; i++) {
                read_destination[i] = read_completion->buf2[i];
            }
        }
        break;
    case FS_COMPLETION_COPY_DIRENT:
        u.p = *(FsRcvPtr *)(pkt + 0x14);
        *(FsDirent *)u.p.dest = *(FsDirent *)(pkt + 0x18);
        break;
    case FS_COMPLETION_COPY_STAT:
        u.p = *(FsRcvPtr *)(pkt + 0x14);
        *(FsStat *)u.p.dest = *(FsStat *)(pkt + 0x18);
        break;
    case 23:
    case 25:
    case 26:
        bs = (FsRcvBuf *)(pkt + 0x14);
        u.b.dest = bs->dest;
        u.b.size = bs->size;
        copy_byte_count = u.b.size.v;
        if (copy_byte_count > 0x400) {
            u.b.size.v = 0x400;
            copy_byte_count = 0x400;
        }
        memcpy((void *)u.b.dest.v, pkt + 0x1C, copy_byte_count);
        break;
    }
    if (completion_header.ret.v < 0) {
        u32 semaphore = 0u - (u32)completion_header.ret.v;
        ret = (s32)semaphore;
        completion_header.ret.v = (s32)semaphore;
        for (i = 0; i < 32; i++) {
            if (fio_async_semas[i] == ret) {
                /* volatile keeps the store ahead of the branch, out of its delay slot */
                ((volatile s32 *)fio_async_semas)[i] = -1;
                break;
            }
        }
        return;
    }
    iSignalSema(completion_header.ret.v);
}
