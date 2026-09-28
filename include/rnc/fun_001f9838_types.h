#ifndef RNC_FUN_001F9838_TYPES_H
#define RNC_FUN_001F9838_TYPES_H

#include "types.h"

typedef struct Sub {
    unsigned char pad0[0x10];
    int f10;
} Sub;

typedef struct Node {
    unsigned char pad0[0x24];
    struct Node *next;
    Sub *sub;
} Node;

typedef struct Owner {
    unsigned char pad0[0x3C];
    Node *head;
    unsigned char pad1[0x8];
    unsigned char flag;
} Owner;

#endif /* RNC_FUN_001F9838_TYPES_H */
