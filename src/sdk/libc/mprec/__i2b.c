typedef struct Object Object;

struct Object {
    unsigned char pad0[16];
    int           state;
    int           value;
};

Object *_Balloc(Object *object, int index) __asm__("func_001154D0");

Object *__i2b(Object *unused, Object *object) __asm__("func_00115808");

Object *__i2b(Object *unused, Object *object) {
    Object *allocated_bigint = _Balloc(unused, 1);
    allocated_bigint->value = (int)object;
    allocated_bigint->state = 1;
    return allocated_bigint;
}
