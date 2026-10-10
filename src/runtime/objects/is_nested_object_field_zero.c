/* Return whether object->field40->field4 is zero. */

int fun_0012ba58(int *object) __asm__("func_0012BA58");

int fun_0012ba58(int *object) {
    return *(int *)((char *)*(int **)((char *)object + 0x40) + 4) == 0;
}
