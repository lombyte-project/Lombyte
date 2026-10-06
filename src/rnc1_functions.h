#ifndef LOMBYTE_RNC1_FUNCTIONS_H
#define LOMBYTE_RNC1_FUNCTIONS_H

#include "types.h"

/* Semantic names are bound to the original address-keyed ELF symbols. */
void ResetGlobalStateFields(void) __asm__("func_00217020");

void EnableGlobalStateFlag(void) __asm__("func_001F61E8");

void DisableGlobalStateFlag(void) __asm__("func_001F61F8");

int SetStateFields(int *state_fields, int first_field, int second_field) __asm__("func_0023BA48");

int GetStateField(int *state_fields) __asm__("func_0023CC80");

int video_dec_abort(int *state_fields) __asm__("func_0023CC70");

void ClearStateField(int *state_fields) __asm__("func_0023CC30");

void InitializeStateFields(int *state_fields) __asm__("func_0023B940");

void NoOpStateCallback(void) __asm__("func_0023B958");

int GetStateOffsetAddress(int *state_fields, int **address_out) __asm__("func_0023B960");

void sc_tag2(unsigned long long *output, unsigned int high_word, unsigned int middle_word,
                    unsigned int low_word) __asm__("func_0023BC20");

void SetGlobalStateSlot(int value) __asm__("func_001160C8");

int *GetStateResource(void *state_resource) __asm__("func_001144D8");

int *GetGlobalStatePointer(void) __asm__("func_001138A8");

void NoOpCoreUtility(void) __asm__("func_001154C0");

void NoOpCoreUtilitySecondary(void) __asm__("func_001154C8");

void ClearCoreGlobal(void) __asm__("func_00118BC0");
int ReturnMinusOne(void) __asm__("func_00118E00");
int ReturnMinusOneAt00118E08(void) __asm__("func_00118E08");
int ReturnSuccessCode(void) __asm__("func_00118EC0");
int ReturnSuccessAt0012B9E0(void) __asm__("func_0012B9E0");
int Func0012BA48(int *object) __asm__("func_0012BA48");
int Func0012BA58(int *object) __asm__("func_0012BA58");
int Func0012BC00(int *object) __asm__("func_0012BC00");
int Func0012BC10(int *object) __asm__("func_0012BC10");
int InitializeObject(int unused, int *object) __asm__("func_00118EC8");
int LookupIndex(int index) __asm__("func_0011A458");
void Func0011AD90(unsigned int *p) __asm__("func_0011AD90");
int InitBlockFn(int value) __asm__("func_00119568");

void NoOpMemoryCallback(void) __asm__("func_00208810");

int Func00208818(int a0, int a1, int a2, int a3, int t0, int t1) __asm__("func_00208818");

void NoOpGraphicsCallback(void) __asm__("func_00237A70");

void NoOpMainCallback(void) __asm__("func_001E93E8");

int IsStateFieldLarge(int *state_fields) __asm__("func_0023AEE0");

float MultiplyGlobalFactorEd64(float input) __asm__("func_001F96B0");

float MultiplyGlobalScale(float input) __asm__("func_001F96E8");
float MultiplyGlobalFactorEd70(float input) __asm__("func_001F9730");

float AbsoluteFloat(float input) __asm__("func_001F99C0");

float ConvertIntegerToFloat(int value) __asm__("func_001FA6C0");

int video_dec_set_state(int *state_fields, int replacement_value) __asm__("func_0023CC88");

void ClearStateFields(volatile int *state_fields) __asm__("func_0023D1E8");

void NoOpStateUpdate(void) __asm__("func_0023D1E0");

void NoOpCoreCallback(void) __asm__("func_001F21B0");

void NoOpCoreCallbackSecondary(void) __asm__("func_001F21B8");

int GetStateCallbackResult(void) __asm__("func_0023BA58");

int vo_buf_is_full(int *state_fields) __asm__("func_0023D1F8");

int vo_buf_is_empty(int *state_fields) __asm__("func_0023D2C8");

void vo_buf_dec_count(volatile int *state_fields) __asm__("func_0023D340");

s64 CheckStateRange(int value) __asm__("CheckStateRange");
int *GetStateResourceWrapper(void) __asm__("GetStateResourceWrapper");
int ConvertMultibyteCharacter(void *runtime_state, int *wide_character,
                              const char *multibyte_string,
                              unsigned int byte_count) __asm__("ConvertMultibyteCharacter");
void InsertLinkObject(void *link_owner, void *link_object) __asm__("InsertLinkObject");
void *Func00115808(void *unused, void *object) __asm__("func_00115808");
int ClassifyDoubleNaN(f64 value) __asm__("func_001161B0");
int Func00116408(void *resource) __asm__("func_00116408");
int CallDebugCharacter(int value, int character) __asm__("CallDebugCharacter");
int EnableInterrupts(void) __asm__("EnableInterrupts");
void PackRenderCommandFields(u64 *command_words, u64 upper_field, u64 middle_field, u64 low_field,
                             u64 tail_field) __asm__("PackRenderCommandFields");

void Func001E93F0(void) __asm__("func_001E93F0");
void Func001E93F8(void) __asm__("func_001E93F8");
void Func001E9400(void) __asm__("func_001E9400");
void Func001E9408(void) __asm__("func_001E9408");
void Func001E9410(void) __asm__("func_001E9410");
void Func001E9418(void) __asm__("func_001E9418");
void Func001E9420(void) __asm__("func_001E9420");
void Func001E9428(void) __asm__("func_001E9428");
void Func001E9430(void) __asm__("func_001E9430");
void Func001E9438(void) __asm__("func_001E9438");
void Func001E9440(void) __asm__("func_001E9440");
int Func001E9448(void) __asm__("func_001E9448");
void Func001E9450(void) __asm__("func_001E9450");
void Func001E9458(void) __asm__("func_001E9458");
void Func001E9460(void) __asm__("func_001E9460");
int Func001E9468(void) __asm__("func_001E9468");
void Func001E9470(void) __asm__("func_001E9470");
void Func001E9478(void) __asm__("func_001E9478");
void noop_callback_s(void) __asm__("func_001E9480");

#endif
