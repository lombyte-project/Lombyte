#include "sda.h"

extern int sif_sregs[] __asm__("D_00154F80") NOT_SDA;

int sceSifGetSreg(int index) __asm__("func_0011A458");

int sceSifGetSreg(int index) {
    return sif_sregs[index];
}
