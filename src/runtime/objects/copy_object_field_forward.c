/* Copy object field 2 into field 3 and return the copied value. */

int copy_object_field_forward(int *object) __asm__("func_0012BC00");

int copy_object_field_forward(int *object) {
    int value = object[2];
    object[3] = value;
    return value;
}
