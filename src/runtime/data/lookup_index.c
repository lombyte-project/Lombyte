#include "sda.h"
extern int LookupTable_lui[] __asm__("D_00154F80") NOT_SDA;

int lookup_index(int index) __asm__("func_0011A458");

int lookup_index(int index) {
    return LookupTable_lui[index];
}
