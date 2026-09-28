#ifndef RNC_FUN_00234BD8_TYPES_H
#define RNC_FUN_00234BD8_TYPES_H

#include "types.h"

typedef struct cCoreSave {
    unsigned char *mData;
} cCoreSave;

typedef struct Ent {
    int unk0;
    struct Ent *next;
    int unk8;
    int id;
} Ent;

typedef struct Mgr {
    char pad0[0x500];
    Ent *head;
} Mgr;

typedef struct Node {
    char pad0[0x10];
    struct Node *next;
} Node;

typedef struct Owner {
    char pad0[0x8];
    Node *head;
} Owner;

#endif /* RNC_FUN_00234BD8_TYPES_H */
