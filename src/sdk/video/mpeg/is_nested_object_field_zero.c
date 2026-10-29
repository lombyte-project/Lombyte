/* Return whether object->field40->field4 is zero. */

int is_nested_object_field_zero(int *object) __asm__("func_0012BA58");

int is_nested_object_field_zero(int *object) {
    return *(int *)((char *)*(int **)((char *)object + 0x40) + 4) == 0;
}
