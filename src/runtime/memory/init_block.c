struct InitBlock {
    int input;
    int zero;
    int *first;
    int *second;
};

extern struct InitBlock InitBlockData __asm__("D_00154940");

int init_block_fn(int value) __asm__("func_00119568");

int init_block_fn(int value) {
    struct InitBlock *base;
    int *ptr;
    base = &InitBlockData;
    ptr = (int *)((char *)base + 16);
    base->input = value;
    base->zero = 0;
    base->second = ptr;
    base->first = ptr;
    return (int)base;
}
