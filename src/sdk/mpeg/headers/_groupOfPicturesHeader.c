#include "rnc/sdk/mpeg/bitstream/nextbit.h"
#include "rnc/sdk/mpeg/headers/_extensionAndUserData.h"

typedef struct GroupOfPicturesContext GroupOfPicturesContext;

struct GroupOfPicturesContext {
    unsigned char opaque;
};

#define FIELD(type, object, offset) (*(type *)((unsigned char *)(object) + (offset)))

void _groupOfPicturesHeader(GroupOfPicturesContext *context) __asm__("_groupOfPicturesHeader");

void _groupOfPicturesHeader(GroupOfPicturesContext *decoder_state) {
    FIELD(int, decoder_state, 0xE8) = 0;
    FIELD(int, decoder_state, 0x854) = 1;
    FIELD(int, decoder_state, 0x84C) = FIELD(int, decoder_state, 0x850) + 1;
    _nextBit(decoder_state, 1);
    _nextBit(decoder_state, 5);
    _nextBit(decoder_state, 6);
    _nextBit(decoder_state, 1);
    _nextBit(decoder_state, 6);
    _nextBit(decoder_state, 6);
    FIELD(int, decoder_state, 0x1A4) = _nextBit(decoder_state, 1);
    FIELD(int, decoder_state, 0x1A8) = _nextBit(decoder_state, 1);
    _extensionAndUserData(decoder_state);
}
