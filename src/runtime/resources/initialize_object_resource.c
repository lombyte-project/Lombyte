typedef struct Object Object;
struct Object {
    unsigned char pad0[16];
    int state;
    int value;
};

Object *fun_001154d0(Object *object, int index) __asm__("func_001154D0");

Object *fun_00115808(Object *unused, Object *object) __asm__("func_00115808");

Object *fun_00115808(Object *unused, Object *object) {
    Object *result = fun_001154d0(unused, 1);
    result->value = (int)object;
    result->state = 1;
    return result;
}
