typedef struct PictureHeaderContext PictureHeaderContext;
struct PictureHeaderContext {
    unsigned char opaque;
};

#define FIELD(type, object, offset) (*(type *)((unsigned char *)(object) + (offset)))

extern int _nextBit(PictureHeaderContext *context, int count) __asm__("_nextBit");
extern void _extrainfo(PictureHeaderContext *context) __asm__("_extrainfo");
extern void _extensionAndUserData(PictureHeaderContext *context) __asm__("_extensionAndUserData");
extern void UpdateTempTrackData(PictureHeaderContext *context,
                                int temporal_reference) __asm__("UpdateTempTrackData");

void PictureHeader(PictureHeaderContext *context) __asm__("_pictureHeader");

void PictureHeader(PictureHeaderContext *context) {
    int temporal_reference = _nextBit(context, 10);

    FIELD(int, context, 0x150) = _nextBit(context, 3);
    _nextBit(context, 16);
    if ((FIELD(unsigned int, context, 0x150) - 2) < 2) {
        FIELD(int, context, 0x154) = _nextBit(context, 1);
        FIELD(int, context, 0x158) = _nextBit(context, 3);
    }
    if (FIELD(unsigned int, context, 0x150) == 3) {
        FIELD(int, context, 0x15C) = _nextBit(context, 1);
        FIELD(int, context, 0x160) = _nextBit(context, 3);
    }
    _extrainfo(context);
    _extensionAndUserData(context);
    UpdateTempTrackData(context, temporal_reference);
}
