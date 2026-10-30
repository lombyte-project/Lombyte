/* newlib srand: seeds _impure_ptr's _rand_next. */

#include "rnc/sdk/library/stdio.h"

void srand(int value) __asm__("func_001160C8");

void srand(int value) {
    _impure_ptr->_rand_next = value;
}
