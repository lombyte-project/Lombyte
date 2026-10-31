typedef struct PictureHeaderContext PictureHeaderContext;

struct PictureHeaderContext {
    unsigned char opaque;
};

#define FIELD(type, object, offset) (*(type *)((unsigned char *)(object) + (offset)))

extern int  _nextBit(PictureHeaderContext *context, int count) __asm__("_nextBit");
extern void ExtraInfo(PictureHeaderContext *context) __asm__("_extrainfo");
extern void _extensionAndUserData(PictureHeaderContext *context) __asm__("_extensionAndUserData");
extern void UpdateTempTrackData(PictureHeaderContext *context,
                                int temporal_reference) __asm__("UpdateTempTrackData");

void PictureHeader(PictureHeaderContext *context) __asm__("_pictureHeader");

void PictureHeader(PictureHeaderContext *decoder_state) {
    int temporal_reference = _nextBit(decoder_state, 10);

    FIELD(int, decoder_state, 0x150) = _nextBit(decoder_state, 3);
    _nextBit(decoder_state, 16);
    if ((FIELD(unsigned int, decoder_state, 0x150) - 2) < 2) {
        FIELD(int, decoder_state, 0x154) = _nextBit(decoder_state, 1);
        FIELD(int, decoder_state, 0x158) = _nextBit(decoder_state, 3);
    }
    if (FIELD(unsigned int, decoder_state, 0x150) == 3) {
        FIELD(int, decoder_state, 0x15C) = _nextBit(decoder_state, 1);
        FIELD(int, decoder_state, 0x160) = _nextBit(decoder_state, 3);
    }
    ExtraInfo(decoder_state);
    _extensionAndUserData(decoder_state);
    UpdateTempTrackData(decoder_state, temporal_reference);
}
