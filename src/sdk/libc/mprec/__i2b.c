typedef struct Object Object;

struct Object {
    unsigned char pad0[16];
    int           state;
    int           value;
};

Object *_Balloc(Object *object, int index) __asm__("func_001154D0");

Object *__i2b(Object *unused, Object *object) __asm__("func_00115808");

Object *__i2b(Object *unused, Object *object) {
    Object *result = _Balloc(unused, 1);
    result->value = (int)object;
    result->state = 1;
    return result;
}
