struct InitBlock {
    int  input;
    int  zero;
    int *first;
    int *second;
};

extern struct InitBlock InitBlockData __asm__("D_00154940");

int init_ring_buffer(int value) __asm__("func_00119568");

int init_ring_buffer(int byte_capacity) {
    struct InitBlock *ring_header;
    int              *ring_storage_start;
    ring_header = &InitBlockData;
    ring_storage_start = (int *)((char *)ring_header + 16);
    ring_header->input = byte_capacity;
    ring_header->zero = 0;
    ring_header->second = ring_storage_start;
    ring_header->first = ring_storage_start;
    return (int)ring_header;
}
