typedef struct GroupOfPicturesContext GroupOfPicturesContext;
struct GroupOfPicturesContext {
    unsigned char opaque;
};

#define FIELD(type, object, offset) (*(type *)((unsigned char *)(object) + (offset)))

int _nextBit(GroupOfPicturesContext *context, int count) __asm__("_nextBit");
void _extensionAndUserData(GroupOfPicturesContext *context) __asm__("_extensionAndUserData");

void _groupOfPicturesHeader(GroupOfPicturesContext *context) __asm__("_groupOfPicturesHeader");

void _groupOfPicturesHeader(GroupOfPicturesContext *context) {
    FIELD(int, context, 0xE8) = 0;
    FIELD(int, context, 0x854) = 1;
    FIELD(int, context, 0x84C) = FIELD(int, context, 0x850) + 1;
    _nextBit(context, 1);
    _nextBit(context, 5);
    _nextBit(context, 6);
    _nextBit(context, 1);
    _nextBit(context, 6);
    _nextBit(context, 6);
    FIELD(int, context, 0x1A4) = _nextBit(context, 1);
    FIELD(int, context, 0x1A8) = _nextBit(context, 1);
    _extensionAndUserData(context);
}
