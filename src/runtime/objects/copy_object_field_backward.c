/* Copy object field 3 into field 2 and return the copied value. */

int copy_object_field_backward(int *object) __asm__("func_0012BC10");

int copy_object_field_backward(int *object) {
    int value = object[3];
    object[2] = value;
    return value;
}
