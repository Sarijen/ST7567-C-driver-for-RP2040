#ifndef FONT_H
#define FONT_H

#include <stdint.h>

typedef struct {
  char character;
  uint8_t* bitmap_data;
} font_table;

extern font_table font_4x8[10];

extern uint8_t zero_4x8[4];
extern uint8_t one_4x8[4];
extern uint8_t two_4x8[4];
extern uint8_t three_4x8[4];
extern uint8_t four_4x8[4];
extern uint8_t five_4x8[4];
extern uint8_t six_4x8[4];
extern uint8_t seven_4x8[4];
extern uint8_t eight_4x8[4];
extern uint8_t nine_4x8[4];

#endif
